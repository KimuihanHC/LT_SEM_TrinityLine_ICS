#include "stdafx.h"
#include "IcsCommunicatorEes.h"

#include "SvrEes.h"
#include "Logger/DebugLogDispatcher.h"
#include "Logger/LogDispatcher.h"
#include "Logger/Logger.h"
#include "Comm/IcsTcpServer.h"
#include "Comm/SocketCommOption.h"
#include "Event/EventMfnListener.hpp"
#include "Enumerator/Enumerator.hpp"
#include "Util/SyncUtil.hpp"
#include "Util/StringUtil.hpp"

#include <map>
#include <algorithm>
#include <future>

#include "IcsRemoteServerEes.h"


//using namespace lt;

CIcsCommunicatorEes::CIcsCommunicatorEes()
{	
	// Logger
	m_pDebugLogDispatcher =
		new lt::CDebugLogDispatcher(_T("ICS_EES_SERVER"), lt::LogFilter::Detail);
	m_pLogDispatcher = new lt::CLogDispatcher();
	m_pLogger = new lt::CLogger();
	m_pLogAddedEventListener =
		new LogAddedEventListener(this, &CIcsCommunicatorEes
			::OnLogAddedEvent);

	static_cast<lt::ILogDispatcher *>(*m_pDebugLogDispatcher)->Dispatch(
		lt::LogLevel::None,
		_T("============================= ICS EES Server begins =============================")
	);

	m_pLogDispatcher->SetLogFilter(lt::LogFilter::Detail);
	m_pLogDispatcher->GetLogAddedEventNotifier() +=
		m_pLogAddedEventListener;
	m_pLogger->Attach(m_pLogDispatcher);

	// Server
	m_pServer = new lt::CIcsTcpServer();
	m_pRemoteConnectedEventListener =
		new RemoteConnectionEventListener(this, &CIcsCommunicatorEes::OnRemoteConnectedEvent);
	m_pRemoteDisconnectedEventListener =
		new RemoteConnectionEventListener(this, &CIcsCommunicatorEes::OnRemoteDisconnectedEvent);

	m_pServer->SetLogDispatcher(*m_pDebugLogDispatcher);
	m_pServer->GetRemoteConnectedEventNotifier() +=
		m_pRemoteConnectedEventListener;
	m_pServer->GetRemoteDisconnectedEventNotifier() +=
		m_pRemoteDisconnectedEventListener;

	// Remote
	m_pRemoteCntr = new RemoteCntr();
	m_pRemoteCntrMutex = new lt::StdRecursiveMutex();
}

CIcsCommunicatorEes::~CIcsCommunicatorEes()
{
	// Remote
	//RemoveRemotes();
	delete m_pRemoteCntr;
	delete m_pRemoteCntrMutex;

	// Server
	m_pServer->GetRemoteConnectedEventNotifier() -=
		m_pRemoteConnectedEventListener;
	m_pServer->GetRemoteDisconnectedEventNotifier() -=
		m_pRemoteDisconnectedEventListener;
	m_pServer->SetLogDispatcher(nullptr);

	delete m_pRemoteConnectedEventListener;
	delete m_pRemoteDisconnectedEventListener;
	delete m_pServer;

	// Logger
	m_pLogger->Detach();
	m_pLogDispatcher->GetLogAddedEventNotifier() -=
		m_pLogAddedEventListener;
	static_cast<lt::ILogDispatcher *>(*m_pDebugLogDispatcher)->Dispatch(
		lt::LogLevel::None,
		_T("============================= ICS Server ends =============================")
	);
	Sleep(100);

	delete m_pLogAddedEventListener;
	delete m_pLogger;
	delete m_pLogDispatcher;
	delete m_pDebugLogDispatcher;
}

CIcsCommunicatorEes & CIcsCommunicatorEes::GetInstance()
{
	static CIcsCommunicatorEes instance;
	return instance;
}

lt::CLogger & CIcsCommunicatorEes::GetLogger()
{
	return *m_pLogger;
}

lt::ILoggable & CIcsCommunicatorEes::GetLogger() const
{
	return *m_pLogger;
}

void CIcsCommunicatorEes::OnLogAddedEvent(LogAddedEventArgs & eventArgs)
{
	static constexpr lt::ConstStringT LogLevels[] =
	{
		_T("None"),
		_T("Info"),
		_T("Proc"),
		_T("Noti"),
		_T("Warn"),
		_T("Erro"),
		_T("Pass"),
		_T("Fail")
	};

	auto & logData = eventArgs.GetArgs();
	auto & loggingTime = logData.GetLoggingTime();
	auto logLevel = LogLevels[static_cast<int>(logData.GetLogLevel())];
	auto string = lt::Format(_T("%02d:%02d:%02d.%03d (%s) %s"),
							 loggingTime.wHour, loggingTime.wMinute,
							 loggingTime.wSecond, loggingTime.wMilliseconds,
							 logLevel,
							 logData.GetLogString());
}

bool CIcsCommunicatorEes::Open(lt::ConstStringT address, lt::ushort port)
{
	static constexpr size_t SizeOfRxBuffer = 1000;

	auto result = GetServer().Open(nullptr, port, SizeOfRxBuffer);

	GetLogger()
		.SetLogLevel(lt::LogLevel::Notice)
		.AddLog(_T("The server open (port = %d, rx buffer = %d) %s"),
				port,
				SizeOfRxBuffer,
				result ? _T("succeeded") : _T("failed"));

	return result;
}

void CIcsCommunicatorEes::Close()
{
	GetServer().Close();

	GetLogger()
		.SetLogLevel(lt::LogLevel::Notice)
		+= _T("The server closed");
}

bool CIcsCommunicatorEes::IsOpened() const
{
	return GetServer().IsOpened();
}

lt::CIcsTcpServer & CIcsCommunicatorEes::GetServer()
{
	return *m_pServer;
}

const lt::CIcsTcpServer & CIcsCommunicatorEes::GetServer() const
{
	return *m_pServer;
}

void CIcsCommunicatorEes::OnRemoteConnectedEvent(RemoteConnectionEventArgs & eventArgs)
{
	auto & comm = eventArgs.GetArgs();
	auto & commOption = reinterpret_cast<const lt::SSocketCommOption &>(comm.GetOption());

	GetLogger()
		.SetLogLevel(lt::LogLevel::Notice)
		.AddLog(_T("%s:%d connected"),
			commOption.remoteAddress,
			commOption.remotePort);

	AttachRemoteComm(commOption.remoteAddress, comm);
}

void CIcsCommunicatorEes::OnRemoteDisconnectedEvent(RemoteConnectionEventArgs & eventArgs)
{
	auto & comm = eventArgs.GetArgs();
	auto & commOption = reinterpret_cast<const lt::SSocketCommOption &>(comm.GetOption());

	GetLogger()
		.SetLogLevel(lt::LogLevel::Notice)
		.AddLog(_T("%s:%d disconnected"),
			commOption.remoteAddress,
			commOption.remotePort);

	DetachRemoteComm(comm);
}

bool CIcsCommunicatorEes::CreateRemote(CSvrEes & server)
{
	auto remotePtr = new CIcsRemoteServerEes(server);

	if (AddRemote(server.Get_ServerId(), remotePtr))
		return true;

	delete remotePtr;

	return false;
}


bool CIcsCommunicatorEes::AddRemote(lt::ConstStringT equipmentId,
	CIcsRemoteServer * remotePtr)
{
	SimpleLockPtr(m_pRemoteCntrMutex);

	auto res = m_pRemoteCntr->emplace(equipmentId, remotePtr);

	if (!res.second)
	{
		GetLogger()
			.SetLogLevel(lt::LogLevel::Error)
			.AddLog(_T("Failed to add the remote(%s)"),
				equipmentId);

		return false;
	}

	GetLogger()
		.SetLogLevel(lt::LogLevel::Notice)
		.AddLog(_T("The remote(%s) has been added"),
			equipmentId);

	auto & config = remotePtr->GetConfig();
	auto address = lt::Format(_T("%d.%d.%d.%d"),
		(config.Get_IP_Address() >> 24) & 0xFF,
		(config.Get_IP_Address() >> 16) & 0xFF,
		(config.Get_IP_Address() >> 8) & 0xFF,
		(config.Get_IP_Address()) & 0xFF);

	using CommEnumerator =
		lt::CEnumerator<std::remove_reference_t<decltype(GetServer())>::Value>;

	CommEnumerator comms(&GetServer());
	auto iter = std::find_if(comms.begin(),
		comms.end(),
		[&address](CommEnumerator::Value & ref) {
		auto & commOption =
			reinterpret_cast<const lt::SSocketCommOption &>(ref.GetOption());

		return address.compare(commOption.remoteAddress) == 0;
	}
	);

	if (iter != comms.end())
	{
		remotePtr->Attach(std::addressof(*iter));

		GetLogger()
			.SetLogLevel(lt::LogLevel::Notice)
			.AddLog(_T("Communicator(%p)(%s) has been attached to the remote(%s)"),
				std::addressof(*iter),
				address.c_str(),
				equipmentId);
	}

	return true;
}
CIcsRemoteServer * CIcsCommunicatorEes::GetRemote(lt::ConstStringT equipmentId)
{
	SimpleLockPtr(m_pRemoteCntrMutex);

	auto iter = m_pRemoteCntr->find(equipmentId);

	if (iter == m_pRemoteCntr->end())
	{
		return nullptr;
	}

	return (*iter).second;
}
void CIcsCommunicatorEes::RemoveRemote(const CServer & server)
{
	SimpleLockPtr(m_pRemoteCntrMutex);

	auto iter = m_pRemoteCntr->find(server.Get_ServerId());
	auto found = iter != m_pRemoteCntr->end();

	if (!found)
	{
		GetLogger()
			.SetLogLevel(lt::LogLevel::Error)
			.AddLog(_T("Failed to remove the remote(%s)"),
				server.Get_ServerId());

		return;
	}

	(*iter).second->Detach();
	delete (*iter).second;
	m_pRemoteCntr->erase(iter);

	GetLogger()
		.SetLogLevel(lt::LogLevel::Notice)
		.AddLog(_T("The remote(%s) has been removed"),
			server.Get_ServerId());
}

void CIcsCommunicatorEes::RemoveRemotes()
{
	SimpleLockPtr(m_pRemoteCntrMutex);

	for (auto & ref : *m_pRemoteCntr)
	{
		ref.second->Detach();
		delete ref.second;
	}

	m_pRemoteCntr->clear();

	GetLogger()
		.SetLogLevel(lt::LogLevel::Notice)
		+= _T("The Remotes have been removed");
}


bool CIcsCommunicatorEes::AttachRemoteComm(lt::ConstStringT address,
	lt::ICommunicator & comm)
{
	SimpleLockPtr(m_pRemoteCntrMutex);

	auto iter = std::find_if(m_pRemoteCntr->begin(),
		m_pRemoteCntr->end(),
		[address](RemoteCntr::const_reference & ref) {
		auto & config = ref.second->GetConfig();
		auto _address = lt::Format(_T("%d.%d.%d.%d"),
			(config.Get_IP_Address() >> 24) & 0xFF,
			(config.Get_IP_Address() >> 16) & 0xFF,
			(config.Get_IP_Address() >> 8) & 0xFF,
			(config.Get_IP_Address()) & 0xFF);

		return _address.compare(address) == 0;
	}
	);

	if (iter == m_pRemoteCntr->end())
		return false;

	(*iter).second->Attach(std::addressof(comm));

	GetLogger()
		.SetLogLevel(lt::LogLevel::Notice)
		.AddLog(_T("Communicator(%p)(%s) has been attached to the remote(%s)"),
			std::addressof(comm),
			address,
			(*iter).first);

	return true;
}

bool CIcsCommunicatorEes::DetachRemoteComm(const lt::ICommunicator & comm)
{
	SimpleLockPtr(m_pRemoteCntrMutex);

	auto iter = std::find_if(m_pRemoteCntr->begin(),
		m_pRemoteCntr->end(),
		[&comm](RemoteCntr::const_reference & ref) {
		return *ref.second == std::addressof(comm);
	}
	);

	if (iter == m_pRemoteCntr->end())
		return false;

	(*iter).second->Detach();

	GetLogger()
		.SetLogLevel(lt::LogLevel::Notice)
		.AddLog(_T("Communicator(%p) has been detached from the remote(%s)"),
			std::addressof(comm),
			(*iter).first);

	return true;
}

template<typename TypeRemote, typename... TypeArgs>
bool CIcsCommunicatorEes::InvokeRemote(lt::uint32(TypeRemote::*fn)(TypeArgs... args),
	TypeArgs... args)
{
	SimpleLockPtr(m_pRemoteCntrMutex);

	std::map<lt::ConstStringT, std::future<lt::uint32>> cntr;


	for (auto & ref : *m_pRemoteCntr)
	{
		cntr.emplace(ref.first,
			std::async(std::launch::async,
				&CIcsCommunicatorEes::InvokeRemote<
				std::remove_pointer_t<decltype(ref.second)>,
				TypeRemote,
				TypeArgs...>,
				this,
				ref.second,
				fn,
				std::forward<TypeArgs>(args)...)
		);
	}

	auto succeeded = true;

	for (auto & ref : cntr)
	{
		auto result = ref.second.get();

		if (result != ICS_COMM_OK)
		{
			GetLogger()
				.SetLogLevel(lt::LogLevel::Error)
				.AddLog(_T("%s CIcsCommunicatorEes::InvokeRemote() failed %d"),
					ref.first,
					result);
		}

		succeeded &= result == ICS_COMM_OK;
	}

	return succeeded;
}

template<typename TypeRemote, typename... TypeArgs>
lt::uint32 CIcsCommunicatorEes::InvokeRemote(lt::ConstStringT equipmentId,
	lt::uint32(TypeRemote::*fn)(TypeArgs... args),
	TypeArgs... args)
{
	SimpleLockPtr(m_pRemoteCntrMutex);

	auto iter = m_pRemoteCntr->find(equipmentId);

	if (iter == m_pRemoteCntr->end())
	{
		GetLogger()
			.SetLogLevel(lt::LogLevel::Error)
			.AddLog(_T("The Remote of Equipment Id %s does not exist"),
				equipmentId);

		return ICS_COMM_ERROR;
	}

	return InvokeRemote((*iter).second,
		fn,
		std::forward<TypeArgs>(args)...);
}

template<typename TypeRemote1, typename TypeRemote2, typename... TypeArgs>

lt::uint32 CIcsCommunicatorEes::InvokeRemote(TypeRemote1 * remotePtr,
	lt::uint32(TypeRemote2::*fn)(TypeArgs... args),
	TypeArgs... args)
{
	auto _remotePtr = dynamic_cast<TypeRemote2 *>(remotePtr);

	if (_remotePtr == nullptr)
	{
		GetLogger()
			.SetLogLevel(lt::LogLevel::Error)
			.AddLog(_T("The instance (%p) of %s could not cast to %s"),
				remotePtr,
				lt::ToTypeChar(typeid(TypeRemote1).name()).c_str(),
				lt::ToTypeChar(typeid(TypeRemote2).name()).c_str());

		return ICS_COMM_ERROR;
	}

	return (_remotePtr->*fn)(std::forward<TypeArgs>(args)...);
}
#if (USE_XML)
bool CIcsCommunicatorEes::SendReplyLinkTestMassage(__in lt::Reply_Link_Test_Args::Args  & pInData){
	auto result = InvokeRemote(&CIcsRemoteServer::SendReplyLinkTestMassage,
		pInData);
	if (result == ICS_COMM_OK)
		return true;
	GetLogger()
		.SetLogLevel(lt::LogLevel::Error)
		+= _T("CIcsCommunicatorEes::SendReplyLinkTestMassage() failed");
	return result;
}
bool CIcsCommunicatorEes::SendReplyLinkTestMassage(__in lt::ConstStringT serverId,
	__in lt::Reply_Link_Test_Args::Args  & pInData){
	auto result = InvokeRemote(serverId,
		&CIcsRemoteServer::SendReplyLinkTestMassage,
		pInData);
	if (result == ICS_COMM_OK)
		return true;
	GetLogger()
		.SetLogLevel(lt::LogLevel::Error)
		.AddLog(_T("%s CIcsCommunicatorEes::SendReplyLinkTestMassage() failed %d"),
			serverId,
			result);
	return false;
}

bool CIcsCommunicatorEes::SendReportOnlineStateMassage(__in lt::Report_Online_State_Args::Args & pInData){
	auto result = InvokeRemote(&CIcsRemoteServer::SendReportOnlineStateMassage,
			 pInData);
	if (result == ICS_COMM_OK)
		return true;
	GetLogger()
		.SetLogLevel(lt::LogLevel::Error)
			+= _T("CIcsCommunicatorEes::SendReportOnlineStateMassage() failed");
	return result;
}
bool CIcsCommunicatorEes::SendReportOnlineStateMassage(__in lt::ConstStringT serverId,
	__in lt::Report_Online_State_Args::Args & pInData){
	auto result = InvokeRemote(serverId,
		&CIcsRemoteServer::SendReportOnlineStateMassage,
		pInData);
	if (result == ICS_COMM_OK)
		return true;
	GetLogger()
		.SetLogLevel(lt::LogLevel::Error)
		.AddLog(_T("%s CIcsCommunicatorEes::SendReportOnlineStateMassage() failed %d"),
			serverId,
			result);
	return false;
}

bool CIcsCommunicatorEes::SendReportUserChangeMassage(__in lt::Report_User_Change_Args::Args &pInData){
	auto result = InvokeRemote(&CIcsRemoteServer::SendReportUserChangeMassage,
		pInData);
	if (result == ICS_COMM_OK)
		return true;
	GetLogger()
		.SetLogLevel(lt::LogLevel::Error)
		.AddLog(_T("CIcsCommunicatorEes::SendReportUserChangeMassage() failed %d"),
			result);
	return false;
}
bool CIcsCommunicatorEes::SendReportUserChangeMassage(__in lt::ConstStringT serverId,
	__in lt::Report_User_Change_Args::Args &pInData){
	auto result = InvokeRemote(serverId,
		&CIcsRemoteServer::SendReportUserChangeMassage,
		pInData);
	if (result == ICS_COMM_OK)
		return true;
	GetLogger()
		.SetLogLevel(lt::LogLevel::Error)
		.AddLog(_T("%s CIcsCommunicatorEes::SendReportUserChangeMassage() failed %d"),
			serverId,
			result);
	return false;
}

bool CIcsCommunicatorEes::SendReplyUserCommandMassage(
	__in lt::Reply_User_Change_Args::Args &pInData){
	auto result = InvokeRemote(&CIcsRemoteServer::SendReplyUserCommandMassage,
		pInData);
	if (result == ICS_COMM_OK)
		return true;
	GetLogger()
		.SetLogLevel(lt::LogLevel::Error)
		+= _T("CIcsCommunicatorEes::SendReplyUserCommandMassage() failed");
	return result;
}
bool CIcsCommunicatorEes::SendReplyUserCommandMassage(__in lt::ConstStringT serverId,
	__in lt::Reply_User_Change_Args::Args &pInData){
	auto result = InvokeRemote(serverId,
		&CIcsRemoteServer::SendReplyUserCommandMassage,
		pInData);
	if (result == ICS_COMM_OK)
		return true;
	GetLogger()
		.SetLogLevel(lt::LogLevel::Error)
		.AddLog(_T("%s CIcsCommunicatorEes::SendReplyUserCommandMassage() failed %d"),
			serverId,
			result);
	return false;
}

bool CIcsCommunicatorEes::SendReportEquipmentStateMassage(
	__in lt::Report_Equipment_State_Args::Args &pInData){
	auto result = InvokeRemote(&CIcsRemoteServer::SendReportEquipmentStateMassage,
		pInData);

	if (result == ICS_COMM_OK)
		return true;

	GetLogger()
		.SetLogLevel(lt::LogLevel::Error)
		+= _T("CIcsCommunicatorEes::SendReportEquipmentStateMassage() failed");
	return result;
}
bool CIcsCommunicatorEes::SendReportEquipmentStateMassage(__in lt::ConstStringT equipmentId,
	__in lt::Report_Equipment_State_Args::Args &pInData){
	auto result = InvokeRemote(equipmentId,
		&CIcsRemoteServer::SendReportEquipmentStateMassage,
		pInData);

	if (result == ICS_COMM_OK)
		return true;

	GetLogger()
		.SetLogLevel(lt::LogLevel::Error)
		.AddLog(_T("%s CIcsCommunicatorEes::SendReportEquipmentStateMassage() failed %d"),
			equipmentId,
			result);

	return false;
}

bool CIcsCommunicatorEes::SendReplyEquipmentStateDisplayMassage(
	__in lt::Reply_Equipment_State_Display_Args::Args &pInData){
	auto result = InvokeRemote(&CIcsRemoteServer::SendReplyEquipmentStateDisplayMassage,
		pInData);

	if (result == ICS_COMM_OK)
		return true;

	GetLogger()
		.SetLogLevel(lt::LogLevel::Error)
		+= _T("CIcsCommunicatorEes::SendReplyEquipmentStateDisplayMassage() failed");
	return result;
}

bool CIcsCommunicatorEes::SendReplyEquipmentStateDisplayMassage(__in lt::ConstStringT equipmentId, 
	__in lt::Reply_Equipment_State_Display_Args::Args &pInData){
	auto result = InvokeRemote(equipmentId,
		&CIcsRemoteServer::SendReplyEquipmentStateDisplayMassage,
		pInData);

	if (result == ICS_COMM_OK)
		return true;

	GetLogger()
		.SetLogLevel(lt::LogLevel::Error)
		.AddLog(_T("%s CIcsCommunicatorEes::SendReplyEquipmentStateDisplayMassage() failed %d"),
			equipmentId,
			result);

	return false;
}

bool CIcsCommunicatorEes::SendReportLossStateMassage(
	__in lt::Report_Loss_State_Args::Args &pInData){
	auto result = InvokeRemote(&CIcsRemoteServer::SendReportLossStateMassage,
		pInData);
	if (result == ICS_COMM_OK)
		return true;
	GetLogger()
		.SetLogLevel(lt::LogLevel::Error)
		+= _T("CIcsCommunicatorEes::SendReportLossStateMassage() failed");
	return result;
}
bool CIcsCommunicatorEes::SendReportLossStateMassage(__in lt::ConstStringT equipmentId,
	__in lt::Report_Loss_State_Args::Args &pInData){
	auto result = InvokeRemote(equipmentId,
		&CIcsRemoteServer::SendReportLossStateMassage,
		pInData);
	if (result == ICS_COMM_OK)
		return true;
	GetLogger()
		.SetLogLevel(lt::LogLevel::Error)
		.AddLog(_T("%s CIcsCommunicatorEes::SendReportLossStateMassage() failed %d"),
			equipmentId,
			result);
	return false;
}

bool CIcsCommunicatorEes::SendReplyLossWindowMassage(
	__in lt::Reply_Loss_Window_Args::Args & pInData){
	auto result = InvokeRemote(&CIcsRemoteServer::SendReplyLossWindowMassage,
		pInData);
	if (result == ICS_COMM_OK)
		return true;
	GetLogger()
		.SetLogLevel(lt::LogLevel::Error)
		+= _T("CIcsCommunicatorEes::SendReplyLossWindowMassage() failed");
	return result;
}
bool CIcsCommunicatorEes::SendReplyLossWindowMassage(__in lt::ConstStringT equipmentId,
	__in lt::Reply_Loss_Window_Args::Args &pInData){
	auto result = InvokeRemote(equipmentId,
		&CIcsRemoteServer::SendReplyLossWindowMassage,
		pInData);
	if (result == ICS_COMM_OK)
		return true;
	GetLogger()
		.SetLogLevel(lt::LogLevel::Error)
		.AddLog(_T("%s CIcsCommunicatorEes::SendReplyLossWindowMassage() failed %d"),
			equipmentId,
			result);
	return false;
}

bool CIcsCommunicatorEes::SendReportAlarmStateMassage(
	__in lt::Report_Alarm_State_Args::Args &pInData){
	auto result = InvokeRemote(&CIcsRemoteServer::SendReportAlarmStateMassage,
		pInData);
	if (result == ICS_COMM_OK)
		return true;
	GetLogger()
		.SetLogLevel(lt::LogLevel::Error)
		+= _T("CIcsCommunicatorEes::SendReportAlarmStateMassage() failed");
	return result;
}
bool CIcsCommunicatorEes::SendReportAlarmStateMassage(__in lt::ConstStringT equipmentId,
	__in  lt::Report_Alarm_State_Args::Args &pInData){
	auto result = InvokeRemote(equipmentId,
		&CIcsRemoteServer::SendReportAlarmStateMassage,
		pInData);
	if (result == ICS_COMM_OK)
		return true;
	GetLogger()
		.SetLogLevel(lt::LogLevel::Error)
		.AddLog(_T("%s CIcsCommunicatorEes::SendReportAlarmStateMassage() failed %d"),
			equipmentId,
			result);

	return false;
}

bool CIcsCommunicatorEes::SendReplyAlarmListMassage(
	__in lt::Reply_Alarm_List_Args::Args &pInData){
	auto result = InvokeRemote(&CIcsRemoteServer::SendReplyAlarmListMassage,
		pInData);
	if (result == ICS_COMM_OK)
		return true;
	GetLogger()
		.SetLogLevel(lt::LogLevel::Error)
		+= _T("CIcsCommunicatorEes::SendReplyAlarmListMassage() failed");
	return result;
}
bool CIcsCommunicatorEes::SendReplyAlarmListMassage(__in lt::ConstStringT equipmentId,
	__in lt::Reply_Alarm_List_Args::Args & pInData){
	auto result = InvokeRemote(equipmentId,
		&CIcsRemoteServer::SendReplyAlarmListMassage,
		pInData);
	if (result == ICS_COMM_OK)
		return true;
	GetLogger()
		.SetLogLevel(lt::LogLevel::Error)
		.AddLog(_T("%s CIcsCommunicatorEes::SendReplyAlarmListMassage() failed %d"),
			equipmentId,
			result);

	return false;
}

bool CIcsCommunicatorEes::SendReportRmsModeMassage(
	__in lt::Report_Rms_Mode_Args::Args &pInData){
	auto result = InvokeRemote(&CIcsRemoteServer::SendReportRmsModeMassage,
		pInData);
	if (result == ICS_COMM_OK)
		return true;
	GetLogger()
		.SetLogLevel(lt::LogLevel::Error)
		+= _T("CIcsCommunicatorEes::SendReportRmsModeMassage() failed");
	return result;
}
bool CIcsCommunicatorEes::SendReportRmsModeMassage(__in lt::ConstStringT equipmentId,
	__in lt::Report_Rms_Mode_Args::Args &pInData){
	auto result = InvokeRemote(equipmentId,
		&CIcsRemoteServer::SendReportRmsModeMassage,
		pInData);
	if (result == ICS_COMM_OK)
		return true;
	GetLogger()
		.SetLogLevel(lt::LogLevel::Error)
		.AddLog(_T("%s CIcsCommunicatorEes::SendReportRmsModeMassage() failed %d"),
			equipmentId,
			result);
	return false;
}

bool CIcsCommunicatorEes::SendReplySetDateTimeMassage(
	__in lt::Reply_Set_DateTime_Args::Args & pInData){
	auto result = InvokeRemote(&CIcsRemoteServer::SendReplySetDateTimeMassage,
		pInData);

	if (result == ICS_COMM_OK)
		return true;

	GetLogger()
		.SetLogLevel(lt::LogLevel::Error)
		+= _T("CIcsCommunicatorEes::SendReplySetDateTimeMassage() failed");
	return result;
}
bool CIcsCommunicatorEes::SendReplySetDateTimeMassage(__in lt::ConstStringT equipmentId,
	__in lt::Reply_Set_DateTime_Args::Args & pInData){
	auto result = InvokeRemote(equipmentId,
		&CIcsRemoteServer::SendReplySetDateTimeMassage,
		pInData);
	if (result == ICS_COMM_OK)
		return true;
	GetLogger()
		.SetLogLevel(lt::LogLevel::Error)
		.AddLog(_T("%s CIcsCommunicatorEes::SendReplySetDateTimeMassage() failed %d"),
			equipmentId,
			result);
	return false;
}

bool CIcsCommunicatorEes::SendReplyTerminalMessageMassage(
	__in lt::Reply_Terminal_Message_Args::Args & pInData){
	auto result = InvokeRemote(&CIcsRemoteServer::SendReplyTerminalMessageMassage,
		pInData);

	if (result == ICS_COMM_OK)
		return true;

	GetLogger()
		.SetLogLevel(lt::LogLevel::Error)
		+= _T("CIcsCommunicatorEes::SendReplyTerminalMessageMassage() failed");
	return result;
}
bool CIcsCommunicatorEes::SendReplyTerminalMessageMassage(__in lt::ConstStringT equipmentId,
	__in lt::Reply_Terminal_Message_Args::Args & pInData){
	auto result = InvokeRemote(equipmentId,
		&CIcsRemoteServer::SendReplyTerminalMessageMassage,
		pInData);
	if (result == ICS_COMM_OK)
		return true;
	GetLogger()
		.SetLogLevel(lt::LogLevel::Error)
		.AddLog(_T("%s CIcsCommunicatorEes::SendReplyTerminalMessageMassage() failed %d"),
			equipmentId,
			result);
	return false;
}

bool CIcsCommunicatorEes::SendReplyOpCallMassage(
	__in lt::Reply_Opcall_Args::Args & pInData){
	auto result = InvokeRemote(&CIcsRemoteServer::SendReplyOpCallMassage,
		pInData);

	if (result == ICS_COMM_OK)
		return true;

	GetLogger()
		.SetLogLevel(lt::LogLevel::Error)
		+= _T("CIcsCommunicatorEes::SendReplyOpCallMassage() failed");
	return result;
}
bool CIcsCommunicatorEes::SendReplyOpCallMassage(__in lt::ConstStringT equipmentId,
	__in lt::Reply_Opcall_Args::Args & pInData){
	auto result = InvokeRemote(equipmentId,
		&CIcsRemoteServer::SendReplyOpCallMassage,
		pInData);
	if (result == ICS_COMM_OK)
		return true;
	GetLogger()
		.SetLogLevel(lt::LogLevel::Error)
		.AddLog(_T("%s CIcsCommunicatorEes::SendReplyOpCallMassage() failed %d"),
			equipmentId,
			result);
	return false;
}
#endif
#if TEST
bool CIcsCommunicatorEes::SendUnitReadMassage(
	ST_xml_UNITID_READ * pInData)
{
	auto result = InvokeRemote(&CIcsRemoteServer::SendUnitReadMassage,
		pInData);
	if (result == ICS_COMM_OK)
		return true;
	GetLogger()
		.SetLogLevel(lt::LogLevel::Error)
		+= _T("CIcsCommunicatorEes::SendUnitReadMassage() failed");
	return result;
}
bool CIcsCommunicatorEes::SendUnitReadMassage(lt::ConstStringT equipmentId,
	ST_xml_UNITID_READ * pInData)
{
	auto result = InvokeRemote(equipmentId,
		&CIcsRemoteServer::SendUnitReadMassage,
		pInData);
	if (result == ICS_COMM_OK)
		return true;
	GetLogger()
		.SetLogLevel(lt::LogLevel::Error)
		.AddLog(_T("%s CIcsCommunicatorEes::SendUnitReadMassage() failed %d"),
			equipmentId,
			result);

	return false;
}
//=============================================================================
// Method		: SendReplyUnitReadMassage
// Access		: public  
// Returns		: bool
// Parameter	: __in ST_xml_REPLY_UNITID_READ * pInData
// Qualifier	:
// Last Update	: 2023/4/6 - 11:34
// Desc.		:
//=============================================================================
bool CIcsCommunicatorEes::SendReplyUnitReadMassage(
	__in ST_xml_REPLY_UNITID_READ * pInData)
{
	auto result = InvokeRemote(&CIcsRemoteServer::SendReplyUnitReadMassage,
		pInData);
	if (result == ICS_COMM_OK)
		return true;
	GetLogger()
		.SetLogLevel(lt::LogLevel::Error)
		+= _T("CIcsCommunicatorEes::SendxmlReportUserChangeMassage() failed");
	return result;
}
bool CIcsCommunicatorEes::SendReplyUnitReadMassage(__in lt::ConstStringT equipmentId,
	__in ST_xml_REPLY_UNITID_READ * pInData)
{
	auto result = InvokeRemote(equipmentId,
		&CIcsRemoteServer::SendReplyUnitReadMassage,
		pInData);
	if (result == ICS_COMM_OK)
		return true;
	GetLogger()
		.SetLogLevel(lt::LogLevel::Error)
		.AddLog(_T("%s CIcsCommunicatorEes::SendxmlReplyUserCommandMassage() failed %d"),
			equipmentId,
			result);
	return false;
}
//=============================================================================
// Method		: SendReportStartLotMassage
// Access		: public  
// Returns		: bool
// Parameter	: __in ST_xml_REPORT_START_LOT * pInData
// Qualifier	:
// Last Update	: 2023/4/6 - 11:34
// Desc.		:
//=============================================================================
bool CIcsCommunicatorEes::SendReportStartLotMassage(
	ST_xml_REPORT_START_LOT * pInData)
{
	auto result = InvokeRemote(&CIcsRemoteServer::SendReportStartLotMassage,
		pInData);
	if (result == ICS_COMM_OK)
		return true;
	GetLogger()
		.SetLogLevel(lt::LogLevel::Error)
		+= _T("CIcsCommunicatorEes::SendReportStartLotMassage() failed");
	return result;
}
bool CIcsCommunicatorEes::SendReportStartLotMassage(lt::ConstStringT equipmentId,
	ST_xml_REPORT_START_LOT * pInData)
{
	auto result = InvokeRemote(equipmentId,
		&CIcsRemoteServer::SendReportStartLotMassage,
		pInData);
	if (result == ICS_COMM_OK)
		return true;
	GetLogger()
		.SetLogLevel(lt::LogLevel::Error)
		.AddLog(_T("%s CIcsCommunicatorEes::SendReportStartLotMassage() failed %d"),
			equipmentId,
			result);

	return false;
}
//=============================================================================
// Method		: SendReportEndEventMassage
// Access		: public  
// Returns		: bool
// Parameter	: __in ST_xml_REPORT_END_EVENT * pInData
// Qualifier	:
// Last Update	: 2023/4/6 - 11:34
// Desc.		:
//=============================================================================
bool CIcsCommunicatorEes::SendReportEndEventMassage(
	ST_xml_REPORT_END_EVENT * pInData)
{
	auto result = InvokeRemote(&CIcsRemoteServer::SendReportEndEventMassage,
		pInData);
	if (result == ICS_COMM_OK)
		return true;
	GetLogger()
		.SetLogLevel(lt::LogLevel::Error)
		+= _T("CIcsCommunicatorEes::SendReportEndEventMassage() failed");
	return result;
}
bool CIcsCommunicatorEes::SendReportEndEventMassage(lt::ConstStringT equipmentId,
	ST_xml_REPORT_END_EVENT * pInData)
{
	auto result = InvokeRemote(equipmentId,
		&CIcsRemoteServer::SendReportEndEventMassage,
		pInData);
	if (result == ICS_COMM_OK)
		return true;
	GetLogger()
		.SetLogLevel(lt::LogLevel::Error)
		.AddLog(_T("%s CIcsCommunicatorEes::SendReportEndEventMassage() failed %d"),
			equipmentId,
			result);
	return false;
}
//=============================================================================
// Method		: SendReportStartProcessMassage
// Access		: public  
// Returns		: bool
// Parameter	: __in ST_xml_REPORT_START_PROCESS * pInData
// Qualifier	:
// Last Update	: 2023/4/6 - 11:34
// Desc.		:
//=============================================================================
bool CIcsCommunicatorEes::SendReportStartProcessMassage(
	ST_xml_REPORT_START_PROCESS * pInData)
{
	auto result = InvokeRemote(&CIcsRemoteServer::SendReportStartProcessMassage,
		pInData);
	if (result == ICS_COMM_OK)
		return true;
	GetLogger()
		.SetLogLevel(lt::LogLevel::Error)
		+= _T("CIcsCommunicatorEes::SendReportStartProcessMassage() failed");
	return result;
}
bool CIcsCommunicatorEes::SendReportStartProcessMassage(lt::ConstStringT equipmentId,
	ST_xml_REPORT_START_PROCESS * pInData)
{
	auto result = InvokeRemote(equipmentId,
		&CIcsRemoteServer::SendReportStartProcessMassage,
		pInData);
	if (result == ICS_COMM_OK)
		return true;
	GetLogger()
		.SetLogLevel(lt::LogLevel::Error)
		.AddLog(_T("%s CIcsCommunicatorEes::SendReportStartProcessMassage() failed %d"),
			equipmentId,
			result);
	return false;
}

//=============================================================================
// Method		: SendReportEndProcessMassage
// Access		: public  
// Returns		: bool
// Parameter	: __in ST_xml_REPORT_END_PROCESS * pInData
// Qualifier	:
// Last Update	: 2023/4/6 - 11:34
// Desc.		:
//=============================================================================
bool CIcsCommunicatorEes::SendReportEndProcessMassage(
	ST_xml_REPORT_END_PROCESS * pInData)
{
	auto result = InvokeRemote(&CIcsRemoteServer::SendReportEndProcessMassage,
		pInData);
	if (result == ICS_COMM_OK)
		return true;
	GetLogger()
		.SetLogLevel(lt::LogLevel::Error)
		+= _T("CIcsCommunicatorEes::SendxmlReportUserChangeMassage() failed");
	return result;
}
bool CIcsCommunicatorEes::SendReportEndProcessMassage(lt::ConstStringT equipmentId,
	ST_xml_REPORT_END_PROCESS * pInData)
{
	auto result = InvokeRemote(equipmentId,
		&CIcsRemoteServer::SendReportEndProcessMassage,
		pInData);
	if (result == ICS_COMM_OK)
		return true;
	GetLogger()
		.SetLogLevel(lt::LogLevel::Error)
		.AddLog(_T("%s CIcsCommunicatorEes::SendxmlReplyUserCommandMassage() failed %d"),
			equipmentId,
			result);
	return false;
}
#endif	//TEST

