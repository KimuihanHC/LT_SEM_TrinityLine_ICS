#include "pch.h"
#include "Comm/TcpCommunicator.h"
#include "Util/StringUtil.hpp"

using namespace lt;

CTcpCommunicator::CTcpCommunicator(ConstStringT strPrefix)
: CSocketCommunicator(strPrefix)
{

}

bool CTcpCommunicator::WaitForIncomingConnection(uintPtr & sock,
												 ushort & port,
												 tchar * address,
												 size_t sizeOfAddress)
{
	if(!sListen())
	{
		GetLogger()
			.SetLogLevel(LogLevel::Error)
			+= _T("sListen() failed");

		return false;
	}

	uintPtr _sock = 0;
	ushort _port = 0;
	tchar _strAddress[16] = {};

	if(!sAccept(_sock, _port, _strAddress, _countof(_strAddress)))
	{
		GetLogger()
			.SetLogLevel(LogLevel::Error)
			+= _T("sAccept() failed");

		return false;
	}

	sock = _sock;
	port = _port;
	_tcscpy_s(address, sizeOfAddress, _strAddress);

	return true;
}

bool CTcpCommunicator::ConnectComm(const SSocketCommOption & option)
{
	if(!sOpen(SocketType::Stream))
	{
		GetLogger()
			.SetLogLevel(LogLevel::Error)
			+= _T("sOpen() failed");

		return false;
	}

	if(_tcscmp(option.remoteAddress, _T("")) != 0)
	{
		if(sConnect(option.remotePort, option.remoteAddress))
			return true;

		GetLogger()
			.SetLogLevel(LogLevel::Error)
			+= _T("sConnect() failed");
	}
	else
	{
		if(sBind(option.localPort, option.localAddress))
			return true;

		GetLogger()
			.SetLogLevel(LogLevel::Error)
			+= _T("sBind() failed");
	}
	return false;
}
