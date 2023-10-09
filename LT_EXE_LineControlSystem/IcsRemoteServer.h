#pragma once
//DLL

#include "Comm/IcsRemoteEes.h"
#include "Event/IcsCommEventEes.h"
#include "Comm/IcsCommError.h"
#include "Xml/xmlArgs.h"

#include "TypeDecl.h"

#include "Def_ConfigServer.h"

namespace lt
{
	class CDebugLogDispatcher;
	class CLogger;
	class ILoggable;
	class CIcsRemote;
	class ICommunicator;
	template<typename TypeSender, typename TypeArgs>
	class IEventArgs;
	template<typename TypeOwner, typename TypeEventArgs>
	class CEventMfnListener;
	class CIcsRemoteEes;
}
class CServer;

class CIcsRemoteServer abstract
{
public:

	CIcsRemoteServer(CServer & server);
	CIcsRemoteServer(const CIcsRemoteServer &) = delete;
	virtual ~CIcsRemoteServer();
	CIcsRemoteServer & operator=(const CIcsRemoteServer &) = delete;
	bool operator==(const lt::ICommunicator * commPtr) const;
	bool operator!=(const lt::ICommunicator * commPtr) const;
	lt::ICommunicator * Attach(lt::ICommunicator * commPtr);
	lt::ICommunicator * Detach();
	bool IsConnected() const;
	const CConfig_Svr & GetConfig() const;
#if defined(EES_XML)//20231003
	//Link State.
	lt::uint32 SendReplyLinkTestMassage(__in lt::Reply_Link_Test_Args::Args pInData);
	//Online State	
	lt::uint32 SendReportOnlineStateMassage(__in lt::Report_Online_State_Args::Args pInData);
	//User Change
	lt::uint32 SendReportUserChangeMassage(__in lt::Report_User_Change_Args::Args pInData);
	lt::uint32 SendReplyUserCommandMassage(__in lt::Reply_User_Change_Args::Args pInData);
	//Equipment State
	lt::uint32 SendReportEquipmentStateMassage(__in lt::Report_Equipment_State_Args::Args pInData);
	lt::uint32 SendReplyEquipmentStateDisplayMassage(__in lt::Reply_Equipment_State_Display_Args::Args pInData);
	//Loss State
	lt::uint32 SendReportLossStateMassage(__in lt::Report_Loss_State_Args::Args pInData);
	lt::uint32 SendReplyLossWindowMassage(__in lt::Reply_Loss_Window_Args::Args pInData);
	//Alarm Report
	lt::uint32 SendReportAlarmStateMassage(__in lt::Report_Alarm_State_Args::Args pInData);
	//Alarm List
	lt::uint32 SendReplyAlarmListMassage(__in lt::Reply_Alarm_List_Args::Args pInData);
	//RMS
	lt::uint32 SendReportRmsModeMassage(__in lt::Report_Rms_Mode_Args::Args pInData);
	//SetDateTime
	lt::uint32 SendReplySetDateTimeMassage(__in lt::Reply_Set_DateTime_Args::Args pInData);
	//TerminalMessage
	lt::uint32 SendReplyTerminalMessageMassage(__in lt::Reply_Terminal_Message_Args::Args pInData);
	//OpCall
	lt::uint32 SendReplyOpCallMassage(__in lt::Reply_Opcall_Args::Args pInData);
	//==============================================================================
	//lt::uint32 SendUnitReadMassage			(__in ST_xml_UNITID_READ * pInData);
	//lt::uint32 SendReplyUnitReadMassage		(__in ST_xml_REPLY_UNITID_READ * pInData);
	//lt::uint32 SendReportStartLotMassage	(__in ST_xml_REPORT_START_LOT * pInData);
	//lt::uint32 SendReportEndEventMassage	(__in ST_xml_REPORT_END_EVENT * pInData);
	//lt::uint32 SendReportStartProcessMassage(__in ST_xml_REPORT_START_PROCESS * pInData);
	//lt::uint32 SendReportEndProcessMassage	(__in ST_xml_REPORT_END_PROCESS * pInData);
	//
#endif
public:
	virtual lt::CIcsRemoteEes & GetRemoteEes() = 0;
	virtual const lt::CIcsRemoteEes & GetRemoteEes() const = 0;

protected:
	lt::CLogger & GetLogger();
	lt::ILoggable & GetLogger() const;
private:
	CServer & GetServer();
	const CServer & GetServer() const;
#if defined(EES_XML)//20231003
	// Remote
	using Request_Link_Test_EventArgs =
		lt::Request_Link_Test_EventArgs<lt::CIcsRemoteEes>;
	using Request_User_Command_EventArgs =
		lt::Request_User_Command_EventArgs<lt::CIcsRemoteEes>;
	using Request_Equipment_State_Display_EventArgs =
		lt::Request_Equipment_State_Display_EventArgs<lt::CIcsRemoteEes>;
	using Request_Loss_Window_EventArgs =
		lt::Request_Loss_Window_EventArgs<lt::CIcsRemoteEes>;
	using Request_Alarm_List_EventArgs =
		lt::Request_Alarm_List_EventArgs<lt::CIcsRemoteEes>;
	using Request_Set_DateTime_EventArgs =
		lt::Request_Set_DateTime_EventArgs<lt::CIcsRemoteEes>;
	using Request_Terminal_Message_EventArgs =
		lt::Request_Terminal_Message_EventArgs<lt::CIcsRemoteEes>;
	using Request_Opcall_EventArgs =
		lt::Request_Opcall_EventArgs<lt::CIcsRemoteEes>;	

	void OnRequestLinkTestEvent(Request_Link_Test_EventArgs & eventArgs);
	void OnRequestUserCommandEvent(Request_User_Command_EventArgs & eventArgs);
	void OnRequestEquipmentStateEvent(Request_Equipment_State_Display_EventArgs & eventArgs);
	void OnRequestLossWindowEvent(Request_Loss_Window_EventArgs & eventArgs);
	void OnRequestAlarmListEvent(Request_Alarm_List_EventArgs & eventArgs);
	void OnRequestSetDateTimeEvent(Request_Set_DateTime_EventArgs & eventArgs);
	void OnRequestTerminalMessageEvent(Request_Terminal_Message_EventArgs & eventArgs);
	void OnRequestOpCallEvent(Request_Opcall_EventArgs & eventArgs);
	//void OnRequestUnitIdReadEvent(XmlEventArgs & eventArgs);
#endif
private:
	CServer & m_Server;
	lt::CDebugLogDispatcher * m_pDebugLogDispatcher = nullptr;
	lt::CLogger * m_pLogger = nullptr;
#if defined(EES_XML)//20231003
	using Request_Link_Test_EventListener =
		lt::CEventMfnListener<CIcsRemoteServer, Request_Link_Test_EventArgs>;
	using Request_User_Command_EventListener =
		lt::CEventMfnListener<CIcsRemoteServer, Request_User_Command_EventArgs>;
	using Request_Equipment_State_Display_EventListener =
		lt::CEventMfnListener<CIcsRemoteServer, Request_Equipment_State_Display_EventArgs>;	
	using Request_Loss_Window_EventListener =
		lt::CEventMfnListener<CIcsRemoteServer, Request_Loss_Window_EventArgs>;	
	using Request_Alarm_List_EventListener =
		lt::CEventMfnListener<CIcsRemoteServer, Request_Alarm_List_EventArgs>;	
	using Request_Set_DateTime_EventListener =
		lt::CEventMfnListener<CIcsRemoteServer, Request_Set_DateTime_EventArgs>;	
	using Request_Terminal_Message_EventListener =
		lt::CEventMfnListener<CIcsRemoteServer, Request_Terminal_Message_EventArgs>;	
	using Request_Opcall_EventListener =
		lt::CEventMfnListener<CIcsRemoteServer, Request_Opcall_EventArgs>;	

	Request_Link_Test_EventListener * m_pRequestLinkTestEventListener = nullptr;
	Request_User_Command_EventListener * m_pRequestUserCommandEventListener = nullptr;
	Request_Equipment_State_Display_EventListener * m_pRequestEquipmentStateEventListener = nullptr;
	Request_Loss_Window_EventListener * m_pRequestLossWindowEventListener = nullptr;
	Request_Alarm_List_EventListener *  m_pRequestAlarmListEventListener = nullptr;
	Request_Set_DateTime_EventListener *  m_pRequestSetDateTimeEventListener = nullptr;
	Request_Terminal_Message_EventListener *  m_pRequestTerminalMessageEventListener = nullptr;
	Request_Opcall_EventListener *  m_pRequestOpcallEventListener = nullptr;
#endif
};
