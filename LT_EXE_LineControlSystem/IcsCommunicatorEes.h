#pragma once

#include "TypeDecl.h"

#include "Event/IcsCommEventEes.h"

namespace lt
{
	class CDebugLogDispatcher;
	class ILogDispatcher;
	class CLogDispatcher;
	class ILogData;
	class CLogger;
	class ILoggable;
	class CIcsTcpServer;

	class CIcsRemoteEes;
	class ICommunicator;
	template<typename TypeSender, typename TypeArgs>
	class IEventArgs;
	template<typename TypeOwner, typename TypeEventArgs>
	class CEventMfnListener;
}

class CSvrEes;
class CServer;
class CIcsRemoteServer;

class CIcsCommunicatorEes final
{
private:
	CIcsCommunicatorEes();
	CIcsCommunicatorEes(const CIcsCommunicatorEes &) = delete;

	CIcsCommunicatorEes & operator=(const CIcsCommunicatorEes &) = delete;

public:
	~CIcsCommunicatorEes();

	static CIcsCommunicatorEes & GetInstance();

	// Server
	bool Open(lt::ConstStringT address, lt::ushort port);
	void Close();

	bool IsOpened() const;

	// Remote
	bool CreateRemote(CSvrEes & server);
	CIcsRemoteServer * GetRemote(lt::ConstStringT equipmentId);
	void RemoveRemote(const CServer & server);
	void RemoveRemotes();
#if defined(EES_XML)
	//Online State.
	bool SendReplyLinkTestMassage(__in lt::Reply_Link_Test_Args::Args  & pInData);
	bool SendReplyLinkTestMassage(__in lt::ConstStringT equipmentId, __in lt::Reply_Link_Test_Args::Args  & pInData);
	//Online State.
	bool SendReportOnlineStateMassage(__in lt::Report_Online_State_Args::Args & pInData);
	bool SendReportOnlineStateMassage(__in lt::ConstStringT equipmentId, __in lt::Report_Online_State_Args::Args  &pInData);
	//User Change.
	bool SendReportUserChangeMassage(__in lt::Report_User_Change_Args::Args & pInData);
	bool SendReportUserChangeMassage(__in lt::ConstStringT serverId, __in lt::Report_User_Change_Args::Args & pInData);
	//Reply User Change.
	bool SendReplyUserCommandMassage(__in lt::Reply_User_Change_Args::Args & pInData);
	bool SendReplyUserCommandMassage(__in lt::ConstStringT serverId, __in lt::Reply_User_Change_Args::Args & pInData);
	//EquipMent State.
	bool SendReportEquipmentStateMassage(__in lt::Report_Equipment_State_Args::Args & pInData);
	bool SendReportEquipmentStateMassage(__in lt::ConstStringT serverId, __in lt::Report_Equipment_State_Args::Args &pInData);
	//Reply Equipment State Display
	bool SendReplyEquipmentStateDisplayMassage(__in lt::Reply_Equipment_State_Display_Args::Args  &pInData);
	bool SendReplyEquipmentStateDisplayMassage(__in lt::ConstStringT equipmentId, __in lt::Reply_Equipment_State_Display_Args::Args  &pInData);
	//Loss Window
	bool SendReportLossStateMassage(__in lt::Report_Loss_State_Args::Args &pInData);
	bool SendReportLossStateMassage(__in lt::ConstStringT equipmentId, __in lt::Report_Loss_State_Args::Args  &pInData);
	bool SendReplyLossWindowMassage(__in lt::Reply_Loss_Window_Args::Args &pInData);
	bool SendReplyLossWindowMassage(__in lt::ConstStringT equipmentId, __in lt::Reply_Loss_Window_Args::Args & pInData);
	//Alarm State
	bool SendReportAlarmStateMassage(__in lt::Report_Alarm_State_Args::Args &pInData);
	bool SendReportAlarmStateMassage(__in lt::ConstStringT serverId, __in  lt::Report_Alarm_State_Args::Args &pInData);
	//Alarm List
	bool SendReplyAlarmListMassage(__in lt::Reply_Alarm_List_Args::Args &pInData);
	bool SendReplyAlarmListMassage(__in lt::ConstStringT serverId, __in lt::Reply_Alarm_List_Args::Args &pInData);
	//RMS.
	bool SendReportRmsModeMassage(__in lt::Report_Rms_Mode_Args::Args &pInData);
	bool SendReportRmsModeMassage(__in lt::ConstStringT equipmentId, __in lt::Report_Rms_Mode_Args::Args &pInData);
	//Set DateTime
	bool SendReplySetDateTimeMassage(__in lt::Reply_Set_DateTime_Args::Args &pInData);
	bool SendReplySetDateTimeMassage(__in lt::ConstStringT equipmentId, __in lt::Reply_Set_DateTime_Args::Args  &pInData);
	//Terminal Massage
	bool SendReplyTerminalMessageMassage(__in lt::Reply_Terminal_Message_Args::Args &pInData);
	bool SendReplyTerminalMessageMassage(__in lt::ConstStringT equipmentId, __in lt::Reply_Terminal_Message_Args::Args &pInData);
	//OpCall.
	bool SendReplyOpCallMassage(__in lt::Reply_Opcall_Args::Args &pInData);
	bool SendReplyOpCallMassage(__in lt::ConstStringT equipmentId, __in lt::Reply_Opcall_Args::Args &pInData);
#endif 
	//UnitRead.
#if TEST
	bool SendUnitReadMassage(__in ST_xml_UNITID_READ * pInData);
	bool SendUnitReadMassage(__in lt::ConstStringT equipmentId, __in 	ST_xml_UNITID_READ * pInData);

	//Reply Unit Read.
	bool SendReplyUnitReadMassage(__in ST_xml_REPLY_UNITID_READ * pInData);
	bool SendReplyUnitReadMassage(__in lt::ConstStringT equipmentId, __in ST_xml_REPLY_UNITID_READ * pInData);
	//=============================================================================
	//Start Lot.
	//=============================================================================
	bool SendReportStartLotMassage(__in ST_xml_REPORT_START_LOT * pInData);
	bool SendReportStartLotMassage(__in lt::ConstStringT equipmentId, __in ST_xml_REPORT_START_LOT * pInData);
	//=============================================================================
	//End Event.
	//=============================================================================
	bool SendReportEndEventMassage(__in ST_xml_REPORT_END_EVENT * pInData);
	bool SendReportEndEventMassage(__in lt::ConstStringT equipmentId, __in ST_xml_REPORT_END_EVENT * pInData);
	//=============================================================================
	//End Process.
	//=============================================================================
	bool SendReportEndProcessMassage(__in ST_xml_REPORT_END_PROCESS * pInData);
	bool SendReportEndProcessMassage(__in lt::ConstStringT equipmentId, __in ST_xml_REPORT_END_PROCESS * pInData);
	//=============================================================================
	//End Process.
	//=============================================================================
	bool SendReportStartProcessMassage(__in ST_xml_REPORT_START_PROCESS * pInData);
	bool SendReportStartProcessMassage(__in lt::ConstStringT equipmentId, __in ST_xml_REPORT_START_PROCESS * pInData);
#endif
private:
	// Logger
	lt::CLogger & GetLogger();
	lt::ILoggable & GetLogger() const;

	using LogAddedEventArgs =
		lt::IEventArgs<lt::ILogDispatcher, lt::ILogData>;

	void OnLogAddedEvent(LogAddedEventArgs & eventArgs);

	// Server
	lt::CIcsTcpServer & GetServer();
	const lt::CIcsTcpServer & GetServer() const;

	using RemoteConnectionEventArgs =
		lt::IEventArgs<lt::CIcsTcpServer, lt::ICommunicator>;

	void OnRemoteConnectedEvent(RemoteConnectionEventArgs & eventArgs);
	void OnRemoteDisconnectedEvent(RemoteConnectionEventArgs & eventArgs);

	// Remote
	bool AddRemote(lt::ConstStringT equipmentId, CIcsRemoteServer * remotePtr);

	bool AttachRemoteComm(lt::ConstStringT address, lt::ICommunicator & comm);
	bool DetachRemoteComm(const lt::ICommunicator & comm);

	template<typename TypeRemote, typename... TypeArgs>
	bool InvokeRemote(lt::uint32(TypeRemote::*fn)(TypeArgs... args),
		TypeArgs... args);

	template<typename TypeRemote, typename... TypeArgs>
	lt::uint32 InvokeRemote(lt::ConstStringT equipmentId,
		lt::uint32(TypeRemote::*fn)(TypeArgs... args),
		TypeArgs... args);

	template<typename TypeRemote1, typename TypeRemote2, typename... TypeArgs>
	lt::uint32 InvokeRemote(TypeRemote1 * remotePtr,
		lt::uint32(TypeRemote2::*fn)(TypeArgs... args),
		TypeArgs... args);
private:
	HANDLE			m_hThr_TIME_OUT;
	// Logger
	lt::CDebugLogDispatcher * m_pDebugLogDispatcher = nullptr;
	lt::CLogDispatcher * m_pLogDispatcher = nullptr;
	lt::CLogger * m_pLogger = nullptr;

	using LogAddedEventListener =
		lt::CEventMfnListener<CIcsCommunicatorEes, LogAddedEventArgs>;

	LogAddedEventListener * m_pLogAddedEventListener = nullptr;

	// Server
	lt::CIcsTcpServer * m_pServer = nullptr;

	using RemoteConnectionEventListener =
		lt::CEventMfnListener<CIcsCommunicatorEes, RemoteConnectionEventArgs>;

	RemoteConnectionEventListener * m_pRemoteConnectedEventListener = nullptr;
	RemoteConnectionEventListener * m_pRemoteDisconnectedEventListener = nullptr;

	using RemoteCntr = lt::StdMapContainer<CString, CIcsRemoteServer *>::Map;

	RemoteCntr * m_pRemoteCntr = nullptr;
	lt::StdRecursiveMutex * m_pRemoteCntrMutex = nullptr;
};
