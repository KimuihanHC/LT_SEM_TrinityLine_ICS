#include "pch.h"
#include "Comm/UdpCommunicator.h"
#include "Util/StringUtil.hpp"

using namespace lt;

CUdpCommunicator::CUdpCommunicator(ConstStringT strPrefix)
: CSocketCommunicator(strPrefix)
{
	//2022.11.28a uhkim log
	GetLogger()
		.SetLogLevel(LogLevel::Info)
		.AddLog(_T("CUdpCommunicator::CUdpCommunicator (%s) "), strPrefix);
	//
}

bool CUdpCommunicator::ConnectComm(const SSocketCommOption & option)
{
	if(!sOpen(SocketType::Dgram))
	{
		GetLogger()
			.SetLogLevel(LogLevel::Error)
			+= _T("sOpen() failed");

		return false;
	}

	if(!sBind(option.localPort, option.localAddress))
	{
		GetLogger()
			.SetLogLevel(LogLevel::Error)
			+= _T("sBind() failed");

		return false;
	}

	if(!sConnect(option.remotePort, option.remoteAddress))
	{
		GetLogger()
			.SetLogLevel(LogLevel::Error)
			+= _T("sConnect() failed");

		return false;
	}

	return true;
}
