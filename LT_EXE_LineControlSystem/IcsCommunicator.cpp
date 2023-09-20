#include "stdafx.h"
#include "IcsCommunicator.h"
#include "Logger/DebugLogDispatcher.h"
#include "Logger/LogDispatcher.h"
#include "Logger/Logger.h"
#include "Comm/IcsTcpServer.h"
#include "Comm/SocketCommOption.h"
#include "IcsRemoteEquipmentLoUnloader.h"
#include "IcsRemoteEquipmentTester.h"
#include "IcsRemoteEquipmentReturner.h"
#include "IcsRemoteEquipmentHandler.h"

#include "EqpLoader.h"
#include "EqpTester.h"
#include "EqpReturner.h"
#include "EqpHandler.h"

#include "Def_Equipment_Type.h"
#include "Event/EventMfnListener.hpp"
#include "Enumerator/Enumerator.hpp"
#include "Util/SyncUtil.hpp"
#include "Util/StringUtil.hpp"
#include <map>
#include <algorithm>
#include <future>

CIcsCommunicator::CIcsCommunicator()
{	
	// Logger
	m_pDebugLogDispatcher =
		new lt::CDebugLogDispatcher(_T("ICS_SERVER"), lt::LogFilter::Detail);
	m_pLogDispatcher = new lt::CLogDispatcher();
	m_pLogger = new lt::CLogger();
	m_pLogAddedEventListener =
		new LogAddedEventListener(this, &CIcsCommunicator
			::OnLogAddedEvent);

	static_cast<lt::ILogDispatcher *>(*m_pDebugLogDispatcher)->Dispatch(
		lt::LogLevel::None,
		_T("============================= ICS Server begins =============================")
	);
	m_pLogDispatcher->SetLogFilter(lt::LogFilter::Detail);
	m_pLogDispatcher->GetLogAddedEventNotifier() +=
		m_pLogAddedEventListener;
	m_pLogger->Attach(m_pLogDispatcher);

	// Server
	m_pServer = new lt::CIcsTcpServer();
	m_pRemoteConnectedEventListener =
		new RemoteConnectionEventListener(this, &CIcsCommunicator::OnRemoteConnectedEvent);
	m_pRemoteDisconnectedEventListener =
		new RemoteConnectionEventListener(this, &CIcsCommunicator::OnRemoteDisconnectedEvent);

	m_pServer->SetLogDispatcher(*m_pDebugLogDispatcher);
	m_pServer->GetRemoteConnectedEventNotifier() +=
		m_pRemoteConnectedEventListener;
	m_pServer->GetRemoteDisconnectedEventNotifier() +=
		m_pRemoteDisconnectedEventListener;


	// Remote
	m_pRemoteCntr = new RemoteCntr();
	m_pRemoteCntrMutex = new lt::StdRecursiveMutex();
}

CIcsCommunicator::~CIcsCommunicator()
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

CIcsCommunicator & CIcsCommunicator::GetInstance()
{
	static CIcsCommunicator instance;
	return instance;
}

lt::CLogger & CIcsCommunicator::GetLogger()
{
	return *m_pLogger;
}

lt::ILoggable & CIcsCommunicator::GetLogger() const
{
	return *m_pLogger;
}

void CIcsCommunicator::OnLogAddedEvent(LogAddedEventArgs & eventArgs)
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

bool CIcsCommunicator::Open(lt::ConstStringT address, lt::ushort port)
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

void CIcsCommunicator::Close()
{
	GetServer().Close();

	GetLogger()
		.SetLogLevel(lt::LogLevel::Notice)
		+= _T("The server closed");
}

bool CIcsCommunicator::IsOpened() const
{
	return GetServer().IsOpened();
}

lt::CIcsTcpServer & CIcsCommunicator::GetServer()
{
	return *m_pServer;
}

const lt::CIcsTcpServer & CIcsCommunicator::GetServer() const
{
	return *m_pServer;
}

void CIcsCommunicator::OnRemoteConnectedEvent(RemoteConnectionEventArgs & eventArgs)
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

void CIcsCommunicator::OnRemoteDisconnectedEvent(RemoteConnectionEventArgs & eventArgs)
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

bool CIcsCommunicator::CreateRemote(CEqpLoader & equipment)
{
	auto remotePtr = new CIcsRemoteEquipmentLoUnloader(equipment);

	if(AddRemote(equipment.Get_EquipmentId(),
				 remotePtr))
		return true;

	delete remotePtr;

	return false;
}

bool CIcsCommunicator::CreateRemote(CEqpTester & equipment)
{
	auto remotePtr = new CIcsRemoteEquipmentTester(equipment);

	if(AddRemote(equipment.Get_EquipmentId(),
				 remotePtr))
		return true;

	delete remotePtr;

	return false;
}

bool CIcsCommunicator::CreateRemote(CEqpReturner & equipment)
{
	auto remotePtr = new CIcsRemoteEquipmentReturner(equipment);

	if(AddRemote(equipment.Get_EquipmentId(),
				 remotePtr))
		return true;

	delete remotePtr;

	return false;
}

bool CIcsCommunicator::CreateRemote(CEqpHandler & equipment)
{
	auto remotePtr = new CIcsRemoteEquipmentHandler(equipment);

	if (AddRemote(equipment.Get_EquipmentId(),
		remotePtr))
		return true;

	delete remotePtr;

	return false;
}

bool CIcsCommunicator::AddRemote(lt::ConstStringT equipmentId,
								 CIcsRemoteEquipment * remotePtr)
{
	SimpleLockPtr(m_pRemoteCntrMutex);

	auto res = m_pRemoteCntr->emplace(equipmentId, remotePtr);

	if(!res.second)
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

	if(iter != comms.end())
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

void CIcsCommunicator::RemoveRemote(const CEquipment & equipment)
{
	SimpleLockPtr(m_pRemoteCntrMutex);

	auto iter = m_pRemoteCntr->find(equipment.Get_EquipmentId());
	auto found = iter != m_pRemoteCntr->end();

	if(!found)
	{
		GetLogger()
			.SetLogLevel(lt::LogLevel::Error)
			.AddLog(_T("Failed to remove the remote(%s)"),
					equipment.Get_EquipmentId());

		return;
	}

	(*iter).second->Detach();
	delete (*iter).second;
	m_pRemoteCntr->erase(iter);

	GetLogger()
		.SetLogLevel(lt::LogLevel::Notice)
		.AddLog(_T("The remote(%s) has been removed"),
				equipment.Get_EquipmentId());
}

void CIcsCommunicator::RemoveRemotes()
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

bool CIcsCommunicator::AttachRemoteComm(lt::ConstStringT address,
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

	if(iter == m_pRemoteCntr->end())
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

bool CIcsCommunicator::DetachRemoteComm(const lt::ICommunicator & comm)
{
	SimpleLockPtr(m_pRemoteCntrMutex);

	auto iter = std::find_if(m_pRemoteCntr->begin(),
							 m_pRemoteCntr->end(),
							 [&comm](RemoteCntr::const_reference & ref) {
								 return *ref.second == std::addressof(comm);
							 }
	);

	if(iter == m_pRemoteCntr->end())
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
bool CIcsCommunicator::InvokeRemote(lt::uint32 (TypeRemote::*fn)(TypeArgs... args),
									TypeArgs... args)
{
	SimpleLockPtr(m_pRemoteCntrMutex);

	std::map<lt::ConstStringT, std::future<lt::uint32>> cntr;

	
	for(auto & ref : *m_pRemoteCntr)
	{		
		cntr.emplace(ref.first,
					 std::async(std::launch::async,
								&CIcsCommunicator::InvokeRemote<
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

	for(auto & ref : cntr)
	{
		auto result = ref.second.get();

		if(result != ICS_COMM_OK)
		{
			GetLogger()
				.SetLogLevel(lt::LogLevel::Error)
				.AddLog(_T("%s CIcsCommunicator::InvokeRemote() failed %d"),
						ref.first,
						result);
		}

		succeeded &= result == ICS_COMM_OK;
	}

	return succeeded;
}

template<typename TypeRemote, typename... TypeArgs>
lt::uint32 CIcsCommunicator::InvokeRemote(lt::ConstStringT equipmentId,
										  lt::uint32 (TypeRemote::*fn)(TypeArgs... args),
										  TypeArgs... args)
{
	SimpleLockPtr(m_pRemoteCntrMutex);

	auto iter = m_pRemoteCntr->find(equipmentId);

	if(iter == m_pRemoteCntr->end())
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
lt::uint32 CIcsCommunicator::InvokeRemote(TypeRemote1 * remotePtr,
										  lt::uint32 (TypeRemote2::*fn)(TypeArgs... args),
										  TypeArgs... args)
{
	auto _remotePtr = dynamic_cast<TypeRemote2 *>(remotePtr);

	if(_remotePtr == nullptr)
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

bool CIcsCommunicator::SendTimeSync()
{
	auto result = InvokeRemote(&CIcsRemoteEquipment::SendTimeSync);

	if(!result)
	{
		GetLogger()
			.SetLogLevel(lt::LogLevel::Error)
			+= _T("CIcsRemoteEquipment::SendTimeSync() failed");
	}
	return result;
}

bool CIcsCommunicator::SendTimeSync(lt::ConstStringT equipmentId)
{
	auto result = InvokeRemote(equipmentId,
							   &CIcsRemoteEquipment::SendTimeSync);

	if(result == ICS_COMM_OK)
		return true;

	GetLogger()
		.SetLogLevel(lt::LogLevel::Error)
		.AddLog(_T("%s CIcsRemoteEquipment::SendTimeSync() failed %d"),
				equipmentId,
				result);

	return false;
}

bool CIcsCommunicator::SendUserLevel(enPermissionMode mode, lt::ConstStringT userId)
{
	auto result = InvokeRemote(&CIcsRemoteEquipment::SendUserLevel,
							   mode,
							   userId);

	if(!result)
	{
		GetLogger()
			.SetLogLevel(lt::LogLevel::Error)
			.AddLog(_T("CIcsRemoteEquipment::SendUserLevel(%d, %s) failed"),
					mode,
					userId);
	}

	return result;
}

bool CIcsCommunicator::SendUserLevel(lt::ConstStringT equipmentId,
									 enPermissionMode mode, lt::ConstStringT userId)
{
	auto result = InvokeRemote(equipmentId,
							   &CIcsRemoteEquipment::SendUserLevel,
							   mode,
							   userId);

	if(result == ICS_COMM_OK)
		return true;

	GetLogger()
		.SetLogLevel(lt::LogLevel::Error)
		.AddLog(_T("%s CIcsRemoteEquipment::SendTimeSync() failed %d"),
				equipmentId,
				result);

	return false;
}

bool CIcsCommunicator::SendLanguage(lt::uint32 language)
{
	auto result = InvokeRemote(&CIcsRemoteEquipment::SendLanguage,
							   language);

	if(!result)
	{
		GetLogger()
			.SetLogLevel(lt::LogLevel::Error)
			.AddLog(_T("CIcsRemoteEquipment::SendLanguage(%d) failed"),
					language);
	}

	return result;
}

bool CIcsCommunicator::SendLanguage(lt::ConstStringT equipmentId,
									lt::uint32 language)
{
	auto result = InvokeRemote(equipmentId,
							   &CIcsRemoteEquipment::SendLanguage,
							   language);

	if(result == ICS_COMM_OK)
		return true;

	GetLogger()
		.SetLogLevel(lt::LogLevel::Error)
		.AddLog(_T("%s CIcsRemoteEquipment::SendLanguage(%d) failed %d"),
				equipmentId,
				language,
				result);

	return false;
}

bool CIcsCommunicator::SendModel(lt::ConstStringT equipmentId,
								 lt::uint32 model,
								 lt::uint32 socketType)
{
	auto result = InvokeRemote(equipmentId,
							   &CIcsRemoteEquipment::SendModel,
							   model,
							   socketType);

	if(result == ICS_COMM_OK)
		return true;

	GetLogger()
		.SetLogLevel(lt::LogLevel::Error)
		.AddLog(_T("%s CIcsRemoteEquipmentTester::SendModel(%d, %d) failed %d"),
				equipmentId,
				model,
				socketType,
				result);

	return false;
}

bool CIcsCommunicator::SendInitialization()
{
	auto result = InvokeRemote(&CIcsRemoteEquipment::SendInitialization);

	if(!result)
	{
		GetLogger()
			.SetLogLevel(lt::LogLevel::Error)
			+= _T("CIcsRemoteEquipment::SendInitialization() failed");
	}

	return result;
}

bool CIcsCommunicator::SendInitialization(lt::ConstStringT equipmentId)
{
	auto result = InvokeRemote(equipmentId,
							   &CIcsRemoteEquipment::SendInitialization);

	if(result == ICS_COMM_OK)
		return true;

	GetLogger()
		.SetLogLevel(lt::LogLevel::Error)
		.AddLog(_T("%s CIcsRemoteEquipment::SendInitialization() failed %d"),
				equipmentId,
				result);

	return false;
}

bool CIcsCommunicator::SendReset()
{
	auto result = InvokeRemote(&CIcsRemoteEquipment::SendReset);

	if(!result)
	{
		GetLogger()
			.SetLogLevel(lt::LogLevel::Error)
			+= _T("CIcsRemoteEquipment::SendReset() failed");
	}

	return result;
}

bool CIcsCommunicator::SendReset(lt::ConstStringT equipmentId)
{
	auto result = InvokeRemote(equipmentId,
							   &CIcsRemoteEquipment::SendReset);

	if(result == ICS_COMM_OK)
		return true;

	GetLogger()
		.SetLogLevel(lt::LogLevel::Error)
		.AddLog(_T("%s CIcsRemoteEquipment::SendReset() failed %d"),
				equipmentId,
				result);

	return false;
}

bool CIcsCommunicator::SendRun()
{
	auto result = InvokeRemote(&CIcsRemoteEquipment::SendRun);

	if(!result)
	{
		GetLogger()
			.SetLogLevel(lt::LogLevel::Error)
			+= _T("CIcsRemoteEquipment::SendRun() failed");
	}

	return result;
}

bool CIcsCommunicator::SendRun(lt::ConstStringT equipmentId)
{
	auto result = InvokeRemote(equipmentId,
							   &CIcsRemoteEquipment::SendRun);

	if(result == ICS_COMM_OK)
		return true;

	GetLogger()
		.SetLogLevel(lt::LogLevel::Error)
		.AddLog(_T("%s CIcsRemoteEquipment::SendRun() failed %d"),
				equipmentId,
				result);

	return false;
}

bool CIcsCommunicator::SendStop()
{
	auto result = InvokeRemote(&CIcsRemoteEquipment::SendStop);

	if(!result)
	{
		GetLogger()
			.SetLogLevel(lt::LogLevel::Error)
			+= _T("CIcsRemoteEquipment::SendStop() failed");
	}

	return result;
}

bool CIcsCommunicator::SendStop(lt::ConstStringT equipmentId)
{
	auto result = InvokeRemote(equipmentId,
							   &CIcsRemoteEquipment::SendStop);

	if(result == ICS_COMM_OK)
		return true;

	GetLogger()
		.SetLogLevel(lt::LogLevel::Error)
		.AddLog(_T("%s CIcsRemoteEquipment::SendStop() failed %d"),
				equipmentId,
				result);

	return false;
}

bool CIcsCommunicator::SendBuzzerOff()
{
	auto result = InvokeRemote(&CIcsRemoteEquipment::SendBuzzerOff);

	if(!result)
	{
		GetLogger()
			.SetLogLevel(lt::LogLevel::Error)
			+= _T("CIcsRemoteEquipment::SendBuzzerOff() failed");
	}

	return result;
}

bool CIcsCommunicator::SendBuzzerOff(lt::ConstStringT equipmentId)
{
	auto result = InvokeRemote(equipmentId,
							   &CIcsRemoteEquipment::SendBuzzerOff);

	if(result == ICS_COMM_OK)
		return true;

	GetLogger()
		.SetLogLevel(lt::LogLevel::Error)
		.AddLog(_T("%s CIcsRemoteEquipment::SendBuzzerOff() failed %d"),
				equipmentId,
				result);

	return false;
}

bool CIcsCommunicator::SendUiVisible(int cmdShow)
{
	auto result = InvokeRemote(&CIcsRemoteEquipment::SendUiVisible,
							   cmdShow);

	if(!result)
	{
		GetLogger()
			.SetLogLevel(lt::LogLevel::Error)
			.AddLog(_T("CIcsRemoteEquipment::SendUiVisible(%d) failed"),
					cmdShow);
	}

	return result;
}

bool CIcsCommunicator::SendUiVisible(lt::ConstStringT equipmentId,
									 int cmdShow)
{
	auto result = InvokeRemote(equipmentId,
							   &CIcsRemoteEquipment::SendUiVisible,
							   cmdShow);

	if(result == ICS_COMM_OK)
		return true;

	GetLogger()
		.SetLogLevel(lt::LogLevel::Error)
		.AddLog(_T("%s CIcsRemoteEquipment::SendUiVisible(%d) failed %d"),
				equipmentId,
				cmdShow,
				result);

	return false;
}

bool CIcsCommunicator::SendSocketAccept(lt::ConstStringT equipmentId,
										lt::ConstStringT rfid,
										lt::uint32 flag,
										lt::ConstStringT info)
{
	auto result = InvokeRemote(equipmentId,
							   &CIcsRemoteEquipment::SendSocketAccept,
							   rfid,
							   flag,
							   info);

	if(result == ICS_COMM_OK)
		return true;

	GetLogger()
		.SetLogLevel(lt::LogLevel::Error)
		.AddLog(_T("%s CIcsRemoteEquipment::SendSocketAccept(%s, %d, %s) failed %d"),
				equipmentId,
				rfid, flag, info,
				result);

	return false;
}

bool CIcsCommunicator::SendEjectAccept(lt::ConstStringT equipmentId,
									   lt::ConstStringT rfid)
{
	auto result = InvokeRemote(equipmentId,
							   &CIcsRemoteEquipment::SendEjectAccept,
							   rfid);

	if(result == ICS_COMM_OK)
		return true;

	GetLogger()
		.SetLogLevel(lt::LogLevel::Error)
		.AddLog(_T("%s CIcsRemoteEquipment::SendEjectAccept(%s) failed %d"),
				equipmentId,
				rfid,
				result);

	return false;
}

bool CIcsCommunicator::SendProductionStartEnd(lt::uint32 flag)
{
	auto result = InvokeRemote(&CIcsRemoteEquipment::SendProductionStartEnd,
							   flag);

	if(!result)
	{
		GetLogger()
			.SetLogLevel(lt::LogLevel::Error)
			+= _T("CIcsRemoteEquipment::SendProductionStartEnd() failed");
	}

	return result;
}

bool CIcsCommunicator::SendProductionStartEnd(lt::ConstStringT equipmentId,
											  lt::uint32 flag)
{
	auto result = InvokeRemote(equipmentId,
							   &CIcsRemoteEquipment::SendProductionStartEnd,
							   flag);

	if(result == ICS_COMM_OK)
		return true;

	GetLogger()
		.SetLogLevel(lt::LogLevel::Error)
		.AddLog(_T("%s CIcsRemoteEquipment::SendProductionStartEnd() failed %d"),
				equipmentId,
				result);

	return false;
}

bool CIcsCommunicator::SendForceEject(bool onOff)
{
	auto result = InvokeRemote(&CIcsRemoteEquipment::SendForceEject,
							   onOff);

	if(!result)
	{
		GetLogger()
			.SetLogLevel(lt::LogLevel::Error)
			.AddLog(_T("CIcsRemoteEquipment::SendUiVisible(%d) failed"),
					onOff);
	}

	return result;
}

bool CIcsCommunicator::SendForceEject(lt::ConstStringT equipmentId,
									  bool onOff)
{
	auto result = InvokeRemote(equipmentId,
							   &CIcsRemoteEquipment::SendForceEject,
							   onOff);

	if(result == ICS_COMM_OK)
		return true;

	GetLogger()
		.SetLogLevel(lt::LogLevel::Error)
		.AddLog(_T("%s CIcsRemoteEquipment::SendForceEject(%d) failed %d"),
				equipmentId,
				onOff,
				result);

	return false;
}