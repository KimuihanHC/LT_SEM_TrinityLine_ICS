#include "pch.h"
#include "Comm/IcsTcpServer.h"
#include "Logger/Logger.h"
#include "Comm/TcpCommunicator.h"
#include "Event/EventMfnListener.hpp"
#include "Event/BasicEventNotifier.hpp"
#include "Event/CancellableEventNotifier.hpp"
#include "Util/SyncUtil.hpp"
#include "Util/StringUtil.hpp"
#include <tchar.h>
#include <assert.h>
#include <thread>
#include <vector>

using namespace lt;

CIcsTcpServer::CIcsTcpServer()
{
	m_pLogger = new CLogger();
	m_pCommServer = new CTcpCommunicator(_T("<Server> "));

	m_hEventListeningTerminated = ::CreateEvent(nullptr, TRUE, TRUE, nullptr);

	m_pRemoteCommCntr = new RemoteCommCntr();
	m_pRemoteCommCntrMutex = new StdMutex();

	m_pRemoteConnectedEventNotifier = new RemoteConnectionEventNotifierImpl();
	m_pRemoteDisconnectedEventNotifier = new RemoteConnectionEventNotifierImpl();

	m_pCommConnectedEventListener =
		new CommConnectionEventListenerImpl(this, &CIcsTcpServer::OnCommConnectedEvent);
	m_pCommDisconnectingEventListener =
		new CommConnectionEventListenerImpl(this, &CIcsTcpServer::OnCommDisconnectingEvent);
}

CIcsTcpServer::~CIcsTcpServer()
{
	delete m_pCommConnectedEventListener;
	delete m_pCommDisconnectingEventListener;

	delete m_pRemoteConnectedEventNotifier;
	delete m_pRemoteDisconnectedEventNotifier;

	delete m_pRemoteCommCntr;
	delete m_pRemoteCommCntrMutex;

	m_pCommServer->SetLogDispatcher(nullptr);
	delete m_pCommServer;

	::CloseHandle(m_hEventListeningTerminated);

	m_pLogger->Detach();
	delete m_pLogger;
}

ILogDispatcher * CIcsTcpServer::SetLogDispatcher(ILogDispatcher * logDispatcherPtr)
{
	auto _logDispatcherPtr = GetLogger().Attach(logDispatcherPtr);

	GetCommServer().SetLogDispatcher(logDispatcherPtr);

	SimpleLockPtr(m_pRemoteCommCntrMutex);

	for(auto & ref : *m_pRemoteCommCntr)
		ref->SetLogDispatcher(logDispatcherPtr);

	return _logDispatcherPtr;
}

bool CIcsTcpServer::Open(ConstStringT address, ushort port, size_t sizeOfRxBuffer)
{
	GetLogger()
		.SetLogLevel(LogLevel::Info)
		+= _T("Open");

	SSocketCommOption commOption = {};

	commOption.sizeOfStructure = sizeof(commOption);
	commOption.sizeOfRxBuffer = 0;
	commOption.socket = 0;
	commOption.localPort = port;

	if(address != nullptr)
	{
		_tcscpy_s(commOption.localAddress,
				  _countof(commOption.localAddress),
				  address);
	}

	auto connected = GetCommServer().Connect(commOption);

	GetLogger()
		.SetLogLevel(connected ? LogLevel::Notice : LogLevel::Error)
		.AddLog(_T("%s to open server (address = %s, port = %d)"),
				connected ? _T("Succeeded") : _T("Failed"),
				commOption.localAddress,
				commOption.localPort);

	if(!connected)
		return false;

	m_sizeOfRxBuffer = sizeOfRxBuffer;
	m_opened = true;

	BeginListening();

	GetLogger()
		.SetLogLevel(LogLevel::Notice)
		+= _T("Opened");

	return true;
}

void CIcsTcpServer::Close()
{
	GetLogger()
		.SetLogLevel(LogLevel::Info)
		+= _T("Close");

	DisconnectRemoteComms();

	if(GetCommServer().IsConnected())
	{
		GetCommServer().Disconnect();

		GetLogger()
			.SetLogLevel(LogLevel::Notice)
			+= _T("The server has been closed");
	}

	EndListening();

	m_sizeOfRxBuffer = 0;
	m_opened = false;

	GetLogger()
		.SetLogLevel(LogLevel::Notice)
		+= _T("Closed");
}

bool CIcsTcpServer::IsOpened() const
{
	return m_opened;
}

CIcsTcpServer::Value & CIcsTcpServer::operator[](size_t index)
{
	return *GetRemoteComm(index);
}

CIcsTcpServer::ConstValue & CIcsTcpServer::operator[](size_t index) const
{
	return *GetRemoteComm(index);
}

size_t CIcsTcpServer::GetCount() const
{
	return GetNumberOfRemoteComms();
}

CIcsTcpServer::RemoteConnectionEventNotifier &
CIcsTcpServer::GetRemoteConnectedEventNotifier()
{
	return *m_pRemoteConnectedEventNotifier;
}

CIcsTcpServer::RemoteConnectionEventNotifier &
CIcsTcpServer::GetRemoteDisconnectedEventNotifier()
{
	return *m_pRemoteDisconnectedEventNotifier;
}

CLogger & CIcsTcpServer::GetLogger()
{
	return *m_pLogger;
}

ILoggable & CIcsTcpServer::GetLogger() const
{
	return *m_pLogger;
}

CTcpCommunicator & CIcsTcpServer::GetCommServer()
{
	return *m_pCommServer;
}

const CTcpCommunicator & CIcsTcpServer::GetCommServer() const
{
	return *m_pCommServer;
}

size_t CIcsTcpServer::GetSizeOfRxBuffer() const
{
	return m_sizeOfRxBuffer;
}

CIcsTcpServer::CommConnectionEventListener *
CIcsTcpServer::GetCommConnectedEventListener() const
{
	return m_pCommConnectedEventListener;
}

CIcsTcpServer::CommConnectionEventListener *
CIcsTcpServer::GetCommDisconnectingEventListener() const
{
	return m_pCommDisconnectingEventListener;
}

void CIcsTcpServer::BeginListening()
{
	GetLogger()
		.SetLogLevel(LogLevel::Info)
		+= _T("Begin listening for incoming connections");

	if(!IsListeningTerminated())
	{
		GetLogger()
			.SetLogLevel(LogLevel::Info)
			+= _T("Listening for incoming connections has already been in progress");

		return;
	}

	SetListeningShutdown(false);

	std::thread listeningThread(
		[this]() {
			SetListeningTerminated(false);

			for(; !IsListeningShutdown();)
			{
				SSocketCommOption option = {};

				if(!GetCommServer().WaitForIncomingConnection(option.socket,
															  option.remotePort,
															  option.remoteAddress,
															  _countof(option.remoteAddress)))
					break;

				option.sizeOfStructure = sizeof(option);
				option.sizeOfRxBuffer = GetSizeOfRxBuffer();
				option.localPort = GetCommServer().GetLocalPort();

				auto localAddress = GetCommServer().GetLocalAddress();

				if(localAddress != nullptr)
				{
					_tcscpy_s(option.localAddress,
							  _countof(option.localAddress),
							  localAddress);
				}

				auto commPtr = new CTcpCommunicator(Format(_T("%s(%d) "),
														   option.remoteAddress,
														   option.remotePort).c_str());

				commPtr->SetLogDispatcher(GetLogger().GetLogDispatcher());
				commPtr->GetCommConnectedEventNotifier()
					+= GetCommConnectedEventListener();
				commPtr->GetCommDisconnectingEventNotifier()
					+= GetCommDisconnectingEventListener();

				if(commPtr->Connect(option))
					AddRemoteComm(commPtr);
				else
					delete commPtr;
			}

			SetListeningTerminated(true);
		}
	);

	listeningThread.detach();
}

void CIcsTcpServer::EndListening()
{
	SetListeningShutdown(true);

	GetLogger()
		.SetLogLevel(LogLevel::Info)
		+= _T("Wait for listening for incoming connections terminates");

	WaitForListeningTerminated(INFINITE);
}

void CIcsTcpServer::SetListeningShutdown(bool shutdown)
{
	m_bListeningShutdown = shutdown;
}

bool CIcsTcpServer::IsListeningShutdown() const
{
	return m_bListeningShutdown;
}

void CIcsTcpServer::SetListeningTerminated(bool terminated)
{
	SetOrResetEvent(m_hEventListeningTerminated, terminated);
}

bool CIcsTcpServer::IsListeningTerminated() const
{
	return WaitForListeningTerminated(0);
}

bool CIcsTcpServer::WaitForListeningTerminated(DWORD milliseconds) const
{
	return ::WaitForSingleObject(m_hEventListeningTerminated, milliseconds) == WAIT_OBJECT_0;
}

void CIcsTcpServer::AddRemoteComm(CTcpCommunicator * commPtr)
{
	SimpleLockPtr(m_pRemoteCommCntrMutex);

	m_pRemoteCommCntr->emplace_back(commPtr);
}

CTcpCommunicator * CIcsTcpServer::GetRemoteComm(size_t index) const
{
	SimpleLockPtr(m_pRemoteCommCntrMutex);

	return (*m_pRemoteCommCntr)[index];
}

void CIcsTcpServer::RemoveRemoteComm(const ICommunicator * commPtr)
{
	SimpleLockPtr(m_pRemoteCommCntrMutex);

	for(auto iter = m_pRemoteCommCntr->begin();
		iter != m_pRemoteCommCntr->end();
		++iter)
	{
		if(commPtr != *iter)
			continue;

		(*iter)->GetCommConnectedEventNotifier()
			-= GetCommConnectedEventListener();
		(*iter)->GetCommDisconnectingEventNotifier()
			-= GetCommDisconnectingEventListener();

		delete (*iter);
		m_pRemoteCommCntr->erase(iter);

		break;
	}
}

void CIcsTcpServer::DisconnectRemoteComms()
{
	SimpleLockPtr(m_pRemoteCommCntrMutex);

	for(auto & ref : *m_pRemoteCommCntr)
	{
		if(ref->IsConnected())
			ref->Disconnect();
	}
}

size_t CIcsTcpServer::GetNumberOfRemoteComms() const
{
	SimpleLockPtr(m_pRemoteCommCntrMutex);

	return m_pRemoteCommCntr->size();
}

void CIcsTcpServer::RaiseRemoteConnectedEvent(ICommunicator & comm)
{
	m_pRemoteConnectedEventNotifier->OnEvent(*this, comm);
}

void CIcsTcpServer::RaiseRemoteDisconnectedEvent(ICommunicator & comm)
{
	m_pRemoteDisconnectedEventNotifier->OnEvent(*this, comm);
}

void CIcsTcpServer::OnCommConnectedEvent(CommConnectionEventArgs & eventArgs)
{
	auto & option = static_cast<const SSocketCommOption &>(eventArgs.GetArgs());

	GetLogger()
		.SetLogLevel(LogLevel::Notice)
		.AddLog(_T("Remote has been connected (address = %s, port = %d)"),
				option.remoteAddress, option.remotePort);

	RaiseRemoteConnectedEvent(eventArgs.GetSender());
}

void CIcsTcpServer::OnCommDisconnectingEvent(CommConnectionEventArgs & eventArgs)
{
	auto & option = static_cast<const SSocketCommOption &>(eventArgs.GetArgs());

	GetLogger()
		.SetLogLevel(LogLevel::Notice)
		.AddLog(_T("Remote has been disconnected (address = %s, port = %d)"),
				option.remoteAddress, option.remotePort);

	std::thread disposingThread(
		[this](ICommunicator * commPtr) {
			for(; commPtr->IsConnected();)
				std::this_thread::sleep_for(std::chrono::milliseconds(1));

			RaiseRemoteDisconnectedEvent(*commPtr);
			RemoveRemoteComm(commPtr);
		}, std::addressof(eventArgs.GetSender())
	);

	disposingThread.detach();
}
