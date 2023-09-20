#include "pch.h"
#include "Comm/SocketCommunicator.h"
#include "Util/StringUtil.hpp"
#include <Ws2tcpip.h>

//2022.11.28a uhkim header
#include "Logger/ILoggable.h"

using namespace lt;

CSocketCommunicator::CSocketCommunicator(ConstStringT strPrefix)
: CCommunicator(strPrefix)
{
	m_pWsaData = new WSAData();
	m_pOption = new SSocketCommOption();
	m_pStrLocalAddress = new StdStringT();
	m_pStrRemoteAddress = new StdStringT();
}

CSocketCommunicator::~CSocketCommunicator()
{
	delete m_pWsaData;
	delete m_pOption;
	delete m_pStrLocalAddress;
	delete m_pStrRemoteAddress;
}

bool CSocketCommunicator::SetOption(const SCommOption & option)
{
	if(option.sizeOfStructure != sizeof(SSocketCommOption))
	{
		GetLogger()
			.SetLogLevel(LogLevel::Error)
			+= _T("The inputted SCommOption structure is not a STcpCommOption");

		return false;
	}

	*m_pOption = static_cast<const SSocketCommOption &>(option);
	return true;
}

const SCommOption & CSocketCommunicator::GetOption() const
{
	return *m_pOption;
}

ushort CSocketCommunicator::GetLocalPort() const
{
	return m_localPort;
}

ConstStringT CSocketCommunicator::GetLocalAddress() const
{
	return m_pStrLocalAddress->c_str();
}

ushort CSocketCommunicator::GetRemotePort() const
{
	return m_remotePort;
}

ConstStringT CSocketCommunicator::GetRemoteAddress() const
{
	return m_pStrRemoteAddress->c_str();
}

bool CSocketCommunicator::sStartup()
{
	static constexpr auto SocketVersion = MAKEWORD(2, 2);

	auto error = WSAStartup(SocketVersion, m_pWsaData);
	auto result = error == 0;

	if(!result)
	{
		GetLogger()
			.SetLogLevel(LogLevel::Error)
			.AddLog(_T("WSAStartup() failed (error = %d)"), error);
	}

	return result;
}

void CSocketCommunicator::sCleanup()
{
	WSACleanup();
}

bool CSocketCommunicator::sAttach(uintPtr sock,
								  ushort localPort, ConstStringT strLocalAddress,
								  ushort remotePort, ConstStringT strRemoteAddress)
{
	if(sIsOpened())
	{
		GetLogger()
			.SetLogLevel(LogLevel::Error)
			+= _T("Socket has already been opened");

		return false;
	}

	m_socket = sock;
	m_localPort = localPort;
	m_remotePort = remotePort;

	if(strLocalAddress == nullptr)
		m_pStrLocalAddress->clear();
	else
		m_pStrLocalAddress->assign(strLocalAddress);

	if(strRemoteAddress == nullptr)
		m_pStrRemoteAddress->clear();
	else
		m_pStrRemoteAddress->assign(strRemoteAddress);

	return true;
}

bool CSocketCommunicator::sOpen(SocketType type)
{
	static constexpr int SockTypes[] =
	{
		SOCK_STREAM,
		SOCK_DGRAM
	};

	if(sIsOpened())
	{
		GetLogger()
			.SetLogLevel(LogLevel::Error)
			+= _T("Socket has already been opened");

		return false;
	}

	auto af = AF_INET;
	auto stype = SockTypes[static_cast<int>(type)];
	auto protocol = 0;
	auto sock = socket(af, stype, protocol);
	auto result = sock != INVALID_SOCKET;

	if(!result)
	{
		GetLogger()
			.SetLogLevel(LogLevel::Error)
			.AddLog(_T("socket(%d, %d, %d) failed (error = %d)"),
					af, stype, protocol, WSAGetLastError());

		return false;
	}

	m_socket = sock;

	return true;
}

void CSocketCommunicator::sClose()
{
	if(m_socket == INVALID_SOCKET)
		return;

	auto error = closesocket(m_socket);

	if(error == SOCKET_ERROR)
	{
		GetLogger()
			.SetLogLevel(LogLevel::Error)
			.AddLog(_T("closesocket() failed (error = %d)"), WSAGetLastError());
	}

	m_socket = INVALID_SOCKET;
	m_localPort = 0;
	m_remotePort = 0;
	m_pStrLocalAddress->clear();
	m_pStrRemoteAddress->clear();
}

bool CSocketCommunicator::sIsOpened() const
{
	return m_socket != INVALID_SOCKET;
}

bool CSocketCommunicator::sBind(ushort port, ConstStringT strAddress)
{
	if(!sIsOpened())
	{
		GetLogger()
			.SetLogLevel(LogLevel::Error)
			+= _T("Socket has not yet been opened");

		return false;
	}

	auto addr = CSocketCommunicator::GetSockAddr(port, strAddress);
	auto error = bind(m_socket, reinterpret_cast<const sockaddr *>(&addr), sizeof(addr));

	if(error == SOCKET_ERROR)
	{
		GetLogger()
			.SetLogLevel(LogLevel::Error)
			.AddLog(_T("bind(%d) failed (error = %d)"),
					port, WSAGetLastError());

		return false;
	}

	m_localPort = CSocketCommunicator::GetPort(addr);
	*m_pStrLocalAddress = CSocketCommunicator::GetAddress(addr);

	return true;
}

bool CSocketCommunicator::sConnect(ushort port, ConstStringT strAddress)
{
	if(!sIsOpened())
	{
		GetLogger()
			.SetLogLevel(LogLevel::Error)
			+= _T("Socket has not yet been opened");

		return false;
	}

	auto addr = CSocketCommunicator::GetSockAddr(port, strAddress);
	auto error = connect(m_socket, reinterpret_cast<const sockaddr *>(&addr), sizeof(addr));

	if(error == SOCKET_ERROR)
	{
		GetLogger()
			.SetLogLevel(LogLevel::Error)
			.AddLog(_T("connect(%d, %s) failed (error = %d)"),
					port, strAddress, WSAGetLastError());

		return false;
	}

	m_remotePort = CSocketCommunicator::GetPort(addr);
	*m_pStrRemoteAddress = CSocketCommunicator::GetAddress(addr);

	return true;
}

bool CSocketCommunicator::sListen()
{
	auto error = listen(m_socket, SOMAXCONN);

	if(error == SOCKET_ERROR)
	{
		GetLogger()
			.SetLogLevel(LogLevel::Error)
			.AddLog(_T("listen() failed (error = %d)"),
					WSAGetLastError());

		return false;
	}

	return true;
}

bool CSocketCommunicator::sAccept(uintPtr & sock, ushort & port, tchar * strAddress, size_t sizeOfAddress)
{
	sockaddr_in addr = {};
	int sizeOfAddr = sizeof(addr);
	auto _sock = accept(m_socket, reinterpret_cast<sockaddr *>(&addr), &sizeOfAddr);

	if(_sock == INVALID_SOCKET)
	{
		GetLogger()
			.SetLogLevel(LogLevel::Error)
			.AddLog(_T("accept() failed (error = %d)"), WSAGetLastError());

		return false;
	}

	sock = _sock;
	port = CSocketCommunicator::GetPort(addr);
	_tcscpy_s(strAddress, sizeOfAddress, CSocketCommunicator::GetAddress(addr).c_str());

	return true;
}

bool CSocketCommunicator::ConnectComm()
{
	GetLogger()
		.SetLogLevel(LogLevel::Info)
		.AddLog(_T("Socket = %p, Local (port = %d, address = %s), Remote (port = %d, address = %s)"),
				m_pOption->socket,
				m_pOption->localPort, m_pOption->localAddress,
				m_pOption->remotePort, m_pOption->remoteAddress);

	if(!sStartup())
	{
		GetLogger()
			.SetLogLevel(LogLevel::Error)
			+= _T("sStartup() failed");

		return false;
	}

	if(m_pOption->socket != 0)
	{
		if(sAttach(m_pOption->socket,
				   m_pOption->localPort, m_pOption->localAddress,
				   m_pOption->remotePort, m_pOption->remoteAddress))
			return true;

		GetLogger()
			.SetLogLevel(LogLevel::Error)
			+= _T("sAttach() failed");

		return false;
	}

	if(ConnectComm(*m_pOption))
		return true;

	DisconnectComm();

	return false;
}

void CSocketCommunicator::DisconnectComm()
{
	sClose();

	sCleanup();
}

size_t CSocketCommunicator::SendComm(const int8 * bufferPtr, size_t sizeOfBuffer)
{
	size_t sent = send(m_socket, bufferPtr, static_cast<int>(sizeOfBuffer), 0);

	if(sent == SOCKET_ERROR)
	{
		GetLogger()
			.SetLogLevel(LogLevel::Error)
			.AddLog(_T("send(%s, %d) failed (error = %d)"),
					ToTypeChar(bufferPtr).c_str(), sizeOfBuffer, WSAGetLastError());
	}

	return sent;
}

size_t CSocketCommunicator::ReceiveComm(int8 * bufferPtr, size_t sizeOfBuffer)
{
	size_t received = recv(m_socket, bufferPtr, static_cast<int>(sizeOfBuffer), 0);

	if(received == 0)
	{
		GetLogger()
			.SetLogLevel(LogLevel::Error)
			.AddLog(_T("recv(%d) failed (The connection has been gracefully closed)"),
					sizeOfBuffer);

		received = -1;
	}
	else if(received == SOCKET_ERROR)
	{
		GetLogger()
			.SetLogLevel(LogLevel::Error)
			.AddLog(_T("recv(%d) failed (error = %d)"),
					sizeOfBuffer, WSAGetLastError());
	}

	return received;
}

bool CSocketCommunicator::ClearRx()
{
	return true;
}

sockaddr_in CSocketCommunicator::GetSockAddr(ushort port, ConstStringT strAddress)
{
	sockaddr_in addr = {};

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);

	if(strAddress == nullptr || _tcslen(strAddress) == 0)
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
	else
		InetPton(AF_INET, strAddress, &addr.sin_addr);

	return addr;
}

ushort CSocketCommunicator::GetPort(const sockaddr_in & addr)
{
	return ntohs(addr.sin_port);
}

StdStringT CSocketCommunicator::GetAddress(const sockaddr_in & addr)
{
	static constexpr auto SizeOfAddress = 16;

	auto ptr = std::make_unique<tchar[]>(SizeOfAddress);

	InetNtop(addr.sin_family, &addr.sin_addr, ptr.get(), SizeOfAddress);

	return StdStringT(ptr.get());
}
