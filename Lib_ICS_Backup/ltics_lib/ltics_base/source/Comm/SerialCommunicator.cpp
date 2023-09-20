#include "pch.h"
#include "Comm/SerialCommunicator.h"
#include "Comm/SerialCommOption.h"
#include "Exception/Win32Exception.h"
#include "Util/StringUtil.hpp"
#include "Util/MemoryUtil.hpp"

using namespace lt;

CSerialCommunicator::CSerialCommunicator(ConstStringT strPrefix)
: CCommunicator(strPrefix)
{
	m_pOption = new SSerialCommOption();
	*m_pOption = {};
}

CSerialCommunicator::~CSerialCommunicator()
{
	delete m_pOption;
}

const SCommOption & CSerialCommunicator::GetOption() const
{
	return *m_pOption;
}

bool CSerialCommunicator::SetOption(const SCommOption & option)
{
	if(option.sizeOfStructure != sizeof(SSerialCommOption))
	{
		GetLogger()
			.SetLogLevel(LogLevel::Error)
			+= _T("The inputted SCommOption structure is not a SSerialCommOption");

		return false;
	}

	*m_pOption = static_cast<const SSerialCommOption &>(option);

	return true;
}

bool CSerialCommunicator::ConnectComm()
{
	auto & option = static_cast<const SSerialCommOption &>(GetOption());

	GetLogger()
		.SetLogLevel(LogLevel::Info)
		.AddLog(_T("Port = %d, BaudRate = %d, DataBits = %d, Parity = %d, StopBit = %d"),
				option.port, option.baudRate, option.dataBits, option.parity, option.stopBit);

	auto strPort = Format(_T("\\\\.\\COM%d"), option.port);
	auto handle = ::CreateFile(strPort.c_str(),
							   GENERIC_READ | GENERIC_WRITE,
							   0,
							   nullptr,
							   OPEN_EXISTING,
							   FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
							   nullptr);

	try
	{
		if(handle == INVALID_HANDLE_VALUE)
			throw _T("::CreateFile() failed");

		COMMTIMEOUTS commTimeouts = {};

		commTimeouts.ReadIntervalTimeout = 10;
		commTimeouts.ReadTotalTimeoutMultiplier = MAXDWORD;
		commTimeouts.ReadTotalTimeoutConstant = MAXDWORD;
		commTimeouts.WriteTotalTimeoutMultiplier = 10;
		commTimeouts.WriteTotalTimeoutConstant = 0;

		if(!::SetCommTimeouts(handle, &commTimeouts))
			throw _T("::SetCommTimeouts() failed");

		if(!::SetCommMask(handle, EV_RXCHAR | EV_BREAK | EV_ERR))
			throw _T("::SetCommMask() failed");

		DCB dcb = {};

		if(!::GetCommState(handle, &dcb))
			throw _T("::GetCommState() failed");

		dcb.BaudRate = option.baudRate;
		dcb.ByteSize = option.dataBits;
		dcb.Parity = static_cast<BYTE>(option.parity);
		dcb.fParity = option.parity != SerialCommParity::None ? TRUE : FALSE;
		dcb.StopBits = static_cast<BYTE>(option.stopBit);
		dcb.fRtsControl = RTS_CONTROL_ENABLE;
		dcb.fDtrControl = DTR_CONTROL_ENABLE;

		if(!::SetCommState(handle, &dcb))
			throw _T("::SetCommState() failed");

		if(!::PurgeComm(handle, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT))
			throw _T("::PurgeComm() failed");

		m_hComm = handle;
		m_commTimeouts = commTimeouts;
		m_dcb = dcb;
		m_ovWrite.hEvent = ::CreateEvent(nullptr, TRUE, FALSE, nullptr);
		m_ovRead.hEvent = ::CreateEvent(nullptr, TRUE, FALSE, nullptr);
	}
	catch(ConstStringT e)
	{
		SafeCloseHandle(handle);

		auto error = ::GetLastError();

		GetLogger()
			.SetLogLevel(LogLevel::Error)
			.AddLog(_T("%s (Error Code = 0x%08X)"), e, error);

		ThrowWin32Exception(error);
	}

	return true;
}

void CSerialCommunicator::DisconnectComm()
{
	SafeCloseHandle(m_hComm);
	SafeCloseHandle(m_ovWrite.hEvent);
	SafeCloseHandle(m_ovRead.hEvent);

	*m_pOption = {};
	m_commTimeouts = {};
	m_dcb = {};
	m_ovWrite = {};
	m_ovRead = {};
}

size_t CSerialCommunicator::SendComm(const int8 * bufferPtr, size_t sizeOfBuffer)
{
	DWORD dwWritten = 0;
	auto result = ::WriteFile(m_hComm,
							  bufferPtr,
							  static_cast<DWORD>(sizeOfBuffer),
							  &dwWritten,
							  &m_ovWrite);

	if(!result)
	{
		if(::GetLastError() == ERROR_IO_PENDING)
			result = ::GetOverlappedResult(m_hComm, &m_ovWrite, &dwWritten, TRUE);
	}

	if(!result)
	{
		GetLogger()
			.SetLogLevel(LogLevel::Error)
			.AddLog(_T("::WriteFile() failed (Error Code = 0x%08X)"), ::GetLastError());

		DWORD dwErrorFlags = 0;
		COMSTAT comstat = {};

		::ClearCommError(m_hComm, &dwErrorFlags, &comstat);
	}

	return dwWritten;
}

size_t CSerialCommunicator::ReceiveComm(int8 * bufferPtr, size_t sizeOfBuffer)
{
	DWORD dwEventMask = 0;

	if(!::WaitCommEvent(m_hComm, &dwEventMask, nullptr))
	{
		auto error = ::GetLastError();

		if(error == ERROR_IO_PENDING)
			return 0;

		GetLogger()
			.SetLogLevel(LogLevel::Error)
			.AddLog(_T("::WaitCommEvent() failed (Error Code = 0x%08X)"), error);

		return -1;
	}

	if((dwEventMask & EV_ERR) ||
	   (dwEventMask & EV_BREAK) ||
	   (dwEventMask & EV_RXCHAR) != EV_RXCHAR)
		return 0;

	size_t totalRead = 0;

	for(;;)
	{
		DWORD dwErrorFlags = 0;
		COMSTAT comstat = {};

		::ClearCommError(m_hComm, &dwErrorFlags, &comstat);

		if(comstat.cbInQue <= 0)
			break;

		m_ovRead.Offset = m_ovRead.OffsetHigh = 0;

		DWORD dwRead = 0;
		auto result = ::ReadFile(m_hComm,
								 bufferPtr + totalRead,
								 static_cast<DWORD>(sizeOfBuffer - totalRead),
								 &dwRead,
								 &m_ovRead);

		if(!result)
		{
			if(::GetLastError() == ERROR_IO_PENDING)
				result = ::GetOverlappedResult(m_hComm, &m_ovRead, &dwRead, TRUE);
		}

		totalRead += dwRead;

		if(!result)
		{
			GetLogger()
				.SetLogLevel(LogLevel::Error)
				.AddLog(_T("::ReadFile() failed (Error Code = 0x%08X)"), ::GetLastError());

			::ClearCommError(m_hComm, &dwErrorFlags, &comstat);

			break;
		}

		if(totalRead >= sizeOfBuffer)
			break;

		Sleep(1);
	}

	return totalRead;
}

bool CSerialCommunicator::ClearRx()
{
	return Clear(false, true);
}

bool CSerialCommunicator::Clear(bool bTxClear, bool bRxClear)
{
	DWORD dwFlag = (bTxClear ? PURGE_TXCLEAR | PURGE_TXABORT : 0) |
				   (bRxClear ? PURGE_RXCLEAR | PURGE_RXABORT : 0);

	if(dwFlag != 0)
	{
		if(!::PurgeComm(m_hComm, dwFlag))
		{
			GetLogger()
				.SetLogLevel(LogLevel::Error)
				.AddLog(_T("::PurgeComm() failed (Error Code = 0x%08X)"), ::GetLastError());

			return false;
		}
	}

	return true;
}
