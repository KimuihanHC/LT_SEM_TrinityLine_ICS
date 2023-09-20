#include "pch.h"
#include "Comm/Communicator.h"
#include "Logger/Logger.h"
#include "Event/BasicEventNotifier.hpp"
#include <tchar.h>
#include <thread>

using namespace lt;

CCommunicator::CCommunicator(ConstStringT strPrefix)
{
	m_pLogger = new CLogger();
	m_pLogger->SetPrefix(strPrefix);
	m_pConnectionMutex = new StdMutex();

	m_hEventCommProcTerminated = ::CreateEvent(nullptr, TRUE, TRUE, nullptr);
	m_hEventReceivingProcTerminated = ::CreateEvent(nullptr, TRUE, TRUE, nullptr);

	m_pCommConnectedEventNotifier = new CommConnectionEventNotifierImpl();
	m_pCommDisconnectingEventNotifier = new CommConnectionEventNotifierImpl();
	m_pCommTxEventNotifier = new CommTxEventNotifierImpl();
	m_pCommRxEventNotifier = new CommRxEventNotifierImpl();
}

CCommunicator::~CCommunicator()
{
	Disconnect();
	::WaitForSingleObject(m_hEventCommProcTerminated, INFINITE);

	::CloseHandle(m_hEventCommProcTerminated);
	::CloseHandle(m_hEventReceivingProcTerminated);

	delete m_pConnectionMutex;

	delete m_pCommConnectedEventNotifier;
	delete m_pCommDisconnectingEventNotifier;
	delete m_pCommTxEventNotifier;
	delete m_pCommRxEventNotifier;
	delete m_pLogger;
}

bool CCommunicator::Connect(const SCommOption & option)
{
	GetLogger()
		.SetLogLevel(LogLevel::Info)
		+= _T("Connect");

	SimpleLockPtr(m_pConnectionMutex);

	if(IsConnected())
	{
		GetLogger()
			.SetLogLevel(LogLevel::Error)
			+= _T("The communicator has already been connected");

		return false;
	}

	GetLogger()
		.SetLogLevel(LogLevel::Info)
		+= _T("Wait for the communication procedure terminates");

	::WaitForSingleObject(m_hEventCommProcTerminated, INFINITE);

	if(!SetOption(option))
	{
		GetLogger()
			.SetLogLevel(LogLevel::Error)
			+= _T("SetOption() failed");

		return false;
	}

	if(!ConnectComm())
	{
		GetLogger()
			.SetLogLevel(LogLevel::Error)
			+= _T("ConnectComm() failed");

		return false;
	}

	m_bCommShutdown = false;

	GetLogger()
		.SetLogLevel(LogLevel::Info)
		+= _T("Create a communication thread");

	if(option.sizeOfRxBuffer > 0)
	{
		StdThread(&CCommunicator::ProcComm, this, option.sizeOfRxBuffer)
			.detach();
	}

	m_bConnected = true;

	OnConnectedEvent();

	GetLogger()
		.SetLogLevel(LogLevel::Notice)
		+= _T("Connected");

	return true;
}

void CCommunicator::Disconnect()
{
	GetLogger()
		.SetLogLevel(LogLevel::Info)
		+= _T("Disconnect");

	SimpleLockPtr(m_pConnectionMutex);

	if(!IsConnected())
		return;

	OnDisconnectingEvent();

	m_bCommShutdown = true;

	DisconnectComm();

	GetLogger()
		.SetLogLevel(LogLevel::Info)
		+= _T("Wait for the receiving procedure terminates");

	::WaitForSingleObject(m_hEventReceivingProcTerminated, INFINITE);

	m_bConnected = false;

	GetLogger()
		.SetLogLevel(LogLevel::Notice)
		+= _T("Disconnected");
}

bool CCommunicator::IsConnected() const
{
	return m_bConnected;
}

size_t CCommunicator::Send(const SCommData & data)
{
	StdStringA string(data.bufferPtr, data.sizeOfBuffer);
	GetLogger()
		.SetLogLevel(LogLevel::Info)
		#if defined(UNICODE)
		//2022.12.06a uhkim
		//.AddLog(_T("Send (clear = %d, buffer = %S, sizeOfBuffer = %d)"),
		//		data.bRxClear, string.c_str(), data.sizeOfBuffer);
		.AddLog(_T("CCommunicator::Send (bRxClear = %d, buffer = %S, sizeOfBuffer = %d)"),
				data.bRxClear, string.c_str(), data.sizeOfBuffer);
		#else
		.AddLog(_T("Send (clear = %d, buffer = %s, sizeOfBuffer = %d)"),
				data.bRxClear, string.c_str(), data.sizeOfBuffer);
		#endif

	if(data.bRxClear)
	{
		if(!ClearRx())
		{
			GetLogger()
				.SetLogLevel(LogLevel::Error)
				+= _T("ClearRx() failed");

			return 0;
		}
	}

	auto sent = SendComm(data.bufferPtr, data.sizeOfBuffer);

	if(sent != data.sizeOfBuffer)
	{
		GetLogger()
			.SetLogLevel(LogLevel::Error)
			.AddLog(_T("SendComm(%d) failed (sent = %d)"), data.sizeOfBuffer, sent);
	}

	if(sent > 0)
	{
		SCommTxData txData = {};

		txData.bufferPtr = data.bufferPtr;
		txData.sizeOfBuffer = data.sizeOfBuffer;
		txData.sizeOfSent = sent;

		OnTxEvent(txData);
	}

	//2022.12.06a uhkim
	GetLogger()
		.SetLogLevel(LogLevel::Info)
		.AddLog(_T("CCommunicator::Send () return %d"), sent);
	//
	return sent;
}

bool CCommunicator::IsCommShutdown() const
{
	//2022.12.06a uhkim
	GetLogger()
		.SetLogLevel(LogLevel::Info)
		.AddLog(_T("CCommunicator::IsCommShutdown () return %d"), m_bCommShutdown);
	//
	return m_bCommShutdown;
}

void CCommunicator::ProcComm(size_t sizeOfRxBuffer)
{
	GetLogger()
		.SetLogLevel(LogLevel::Info)
		+= _T("Communication procedure begins");

	SetOrResetEvent(m_hEventCommProcTerminated, false);

	ProcReceiving(sizeOfRxBuffer);

	SetOrResetEvent(m_hEventCommProcTerminated, true);

	Disconnect();

	GetLogger()
		.SetLogLevel(LogLevel::Notice)
		+= _T("Communication procedure ends");
}

void CCommunicator::ProcReceiving(size_t sizeOfRxBuffer)
{
	GetLogger()
		.SetLogLevel(LogLevel::Info)
		+= _T("Receiving procedure begins");

	SetOrResetEvent(m_hEventReceivingProcTerminated, false);

	GetLogger()
		.SetLogLevel(LogLevel::Info)
		.AddLog(_T("SizeOfRxBuffer = %d"), sizeOfRxBuffer);

	auto sizeOfBuffer = sizeOfRxBuffer;
	auto bufferPtr = new int8 [sizeOfBuffer + 1];

	for(; !IsCommShutdown();)
	{
		auto received = ReceiveComm(bufferPtr, sizeOfBuffer);

		if(received == static_cast<decltype(received)>(-1))
			break;

		if(received == 0)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			continue;
		}

		bufferPtr[received] = 0;

		GetLogger()
			.SetLogLevel(LogLevel::Info)
			#if defined(UNICODE)
			.AddLog(_T("Received (buffer = %S, sizeOfBuffer = %d)"), bufferPtr, received);
		#else
			.AddLog(_T("Received (buffer = %s, sizeOfBuffer = %d)"), bufferPtr, received);
		#endif

		SCommRxData rxData = {};

		rxData.bufferPtr = bufferPtr;
		rxData.sizeOfReceived = received;

		//2023.06.30a uhkim TEST [memory Test] Àá±ñ »èÁ¦ÇÔ.
		OnRxEvent(rxData);
	}

	delete [] bufferPtr;

	SetOrResetEvent(m_hEventReceivingProcTerminated, true);

	GetLogger()
		.SetLogLevel(LogLevel::Notice)
		+= _T("Receiving procedure ends");
}

CCommunicator::CommConnectionEventNotifier & CCommunicator::GetCommConnectedEventNotifier()
{
	return *m_pCommConnectedEventNotifier;
}

CCommunicator::CommConnectionEventNotifier & CCommunicator::GetCommDisconnectingEventNotifier()
{
	return *m_pCommDisconnectingEventNotifier;
}

CCommunicator::CommTxEventNotifier & CCommunicator::GetCommTxEventNotifier()
{
	return *m_pCommTxEventNotifier;
}

CCommunicator::CommRxEventNotifier & CCommunicator::GetCommRxEventNotifier()
{
	return *m_pCommRxEventNotifier;
}

ILogDispatcher * CCommunicator::SetLogDispatcher(ILogDispatcher * pLogDispatcher)
{
	return m_pLogger->Attach(pLogDispatcher);
}

ILoggable & CCommunicator::GetLogger() const
{
	return *m_pLogger;
}

void CCommunicator::OnConnectedEvent()
{
	m_pCommConnectedEventNotifier->OnEvent(*this, GetOption());
}

void CCommunicator::OnDisconnectingEvent()
{
	m_pCommDisconnectingEventNotifier->OnEvent(*this, GetOption());
}

void CCommunicator::OnTxEvent(const SCommTxData & data)
{
	m_pCommTxEventNotifier->OnEvent(*this, data);
}

void CCommunicator::OnRxEvent(const SCommRxData & data)
{
	m_pCommRxEventNotifier->OnEvent(*this, data);
}
