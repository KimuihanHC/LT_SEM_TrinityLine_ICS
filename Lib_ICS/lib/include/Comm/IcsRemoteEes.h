#pragma once
#include "Comm/IcsRemote.h"
#include "Comm/IcsCommunicator.h"
#include "Util/StringUtil.hpp"
//¿Ã∫•∆Æ
#include "Event/IcsCommEventEes.h"
//XML
#if (USE_XML) || 1
#include "xmlArgs.h"
#include "xml3all.h"
#include "Markup.h"
using namespace XML3;
#endif 

namespace lt
{
	template<typename TypeSender, typename TypeArgs>
	class CCancellableEventNotifier;

	class LTICS_LIB_CLASS CIcsRemoteEes final : public CIcsCommunicator
	{
	public:

		//////////////////////////////////////////////////////////////////////////////////
		CIcsRemoteEes();
		CIcsRemoteEes(const CIcsRemoteEes &) = delete;
		~CIcsRemoteEes();		

		CIcsRemoteEes & operator=(const CIcsRemoteEes &) = delete;
#if (USE_XML) || 1
		Request_Link_Test_EventNotifier<CIcsRemoteEes> &
			GetRequestLinkTestEventNotifier();
		Request_User_Command_EventNotifier<CIcsRemoteEes> &
			GetRequestUserCommandEventNotifier();
		Equipment_State_Display_EventNotifier<CIcsRemoteEes> &
			GetRequestEquipmentStateEventNotifier();
		Request_Loss_Window_EventNotifier<CIcsRemoteEes> &
			GetRequestLossWindowEventNotifier();
		Request_Alarm_List_EventNotifier<CIcsRemoteEes> &
			GetRequestAlarmListEventNotifier();
		Request_Set_DateTime_EventNotifier<CIcsRemoteEes> &
			GetRequestSetDateTimeEventNotifier();
		Request_Terminal_Message_EventNotifier<CIcsRemoteEes> &
			GetRequestTerminalMessageEventNotifier();
		Request_Opcall_EventNotifier<CIcsRemoteEes> &
			GetRequestOPCallEventNotifier();
		//////////////////////////////////////////////////////////////////////////////////
	public:
		//////////////////////////////////////////////////////////////////////////////////
		//Link Test
		uint32 CommandReplyLinkTest(Reply_Link_Test_Args::Args & xml);
		//OnlineState
		uint32 CommandReportOnlineState(Report_Online_State_Args::Args & xml);
		//UserChange
		uint32 CommandReportUserChange(Report_User_Change_Args::Args  & xml);
		uint32 CommandReplyUserCommand(Reply_User_Change_Args::Args & xml);
		//Equipment State Change
		uint32 CommandReportEquipmentState(Report_Equipment_State_Args::Args & xml);
		uint32 CommandReplyEquipmentStateDisplay(Reply_Equipment_State_Display_Args::Args & xml);
		//Loss
		uint32 CommandReportLossState(Report_Loss_State_Args::Args & xml);	
		uint32 CommandReplyLossWindow(Reply_Loss_Window_Args::Args & xml);
		//Alarm Report
		uint32 CommandReportAlarmState(Report_Alarm_State_Args::Args & xml);
		uint32 CommandReplyAlarmList(Reply_Alarm_List_Args::Args & xml);
		//Date/Time Synchronization
		uint32 CommandReplySetDateTime(Reply_Set_DateTime_Args::Args & xml);
		//RMS Change
		uint32 CommandReportRMSMode(Report_Rms_Mode_Args::Args & xml);
		//Terminal Message
		uint32 CommandReplyTerminalMessage(Reply_Terminal_Message_Args::Args & xml);
		//Op Call
		uint32 CommandReplyOpCall(Reply_Opcall_Args::Args & xml);
		//UnitId Read
#if TEST
		uint32 CommandUnitIdRead(const ST_xml_Msg & xml);
		uint32 CommandReplyUnitIdRead(const ST_xml_Msg & xml);
		uint32 CommandReportStartLot(const ST_xml_Msg & xml);
		uint32 CommandReportStartProcess(const ST_xml_Msg & xml);
		uint32 CommandReportEndProcess(const ST_xml_Msg & xml);
		uint32 CommandReportEndEvent(const ST_xml_Msg & xml);
		//========================================================
#endif
		//////////////////////////////////////////////////////////////////////////////////
	private:
		//////////////////////////////////////////////////////////////////////////////////
		//EPT
		bool ProcRequestLinkTest(const DataCntr & dataCntr);
		bool ProcRequestAlarmList(const DataCntr & dataCntr);	
		bool ProcRequestSetDateTime(const DataCntr & dataCntr);
		//TerminalMessage
		bool ProcRequestTerminalMessage(const DataCntr & dataCntr);
		//Op Call
		bool ProcRequestOPCall(const DataCntr & dataCntr);

		bool ProcRequestUserCommand(const DataCntr & dataCntr);
		bool ProcRequestEquipmentStateDisplay(const DataCntr & dataCntr);
		bool ProcRequestLossWindow(const DataCntr & dataCntr);

		//Unit read
		//bool ProcRequestUnitRead(const DataCntr & dataCntr);

		void RaiseRequestLinkTestEvent(Request_Link_Test_Args::Args & args);
		//Use Command
		void RaiseRequestUserCommandEvent(Request_User_Cpmmand_Args::Args & args);
		//Equipment Display
		void RaiseRequestEqipmentStateDisplayEvent(Request_Equipment_State_Display_Args::Args & args);
		//Loss State Window
		void RaiseRequestLossWindowEvent(Request_Loss_Window_Args::Args & args);
		//Alarm List
		void RaiseRequestAlarmListEvent(Request_Alarm_List_Args::Args & args);
		//Date Time
		void RaiseRequestSetDateTimeEvent(Request_Set_DateTime_Args::Args & args);
		//Terminal Message
		void RaiseRequestTerminalMessageEvent(Request_Terminal_Message_Args::Args & args);
		//Op Call
		void RaiseRequestOPCallEvent(Request_Opcall_Args::Args & args);

		//void RaiseRequestUnitReadEvent(XmlArgs::Args & args);
		//////////////////////////////////////////////////////////////////////////////////
	private:
		//////////////////////////////////////////////////////////////////////////////////
		//EPT
		//using xmlEventNotifierImpl =
		//	CCancellableEventNotifier<CIcsRemoteEes, XmlArgs>;

		using Request_Link_Test_EventNotifierImpl =
			CCancellableEventNotifier<CIcsRemoteEes, Request_Link_Test_Args>;
		Request_Link_Test_EventNotifierImpl * m_pRequestLinkTestEventNotifier	= nullptr;
		using Request_User_Cpmmand_EventNotifierImpl =
			CCancellableEventNotifier<CIcsRemoteEes, Request_User_Cpmmand_Args>;
		Request_User_Cpmmand_EventNotifierImpl * m_pRequestUserCommandEventNotifier = nullptr;
		using Request_Equipment_State_Display_EventNotifierImpl =
			CCancellableEventNotifier<CIcsRemoteEes, Request_Equipment_State_Display_Args>;
		Request_Equipment_State_Display_EventNotifierImpl * m_pRequestEquipmentStateEventNotifier = nullptr;


		using Request_Loss_Window_EventNotifierImpl =
			CCancellableEventNotifier<CIcsRemoteEes, Request_Loss_Window_Args>;
		Request_Loss_Window_EventNotifierImpl * m_pRequestLossWindowEventNotifier = nullptr;
		using Request_Alarm_List_EventNotifierImpl =
			CCancellableEventNotifier<CIcsRemoteEes, Request_Alarm_List_Args>;
		Request_Alarm_List_EventNotifierImpl * m_pRequestAlarmListEventNotifier			= nullptr;
		using Request_Set_DateTime_EventNotifierImpl =
			CCancellableEventNotifier<CIcsRemoteEes, Request_Set_DateTime_Args>;
		Request_Set_DateTime_EventNotifierImpl * m_pRequestSetDateTimeEventNotifier		= nullptr;
		using Request_Terminal_Message_EventNotifierImpl =
			CCancellableEventNotifier<CIcsRemoteEes, Request_Terminal_Message_Args>;
		Request_Terminal_Message_EventNotifierImpl * m_pRequestTerminalMessageEventNotifier	= nullptr;
		using Request_Opcall_EventNotifierImpl =
			CCancellableEventNotifier<CIcsRemoteEes, Request_Opcall_Args>;
		Request_Opcall_EventNotifierImpl		* m_pRequestOPCallEventNotifier			= nullptr;

		//xmlEventNotifierImpl * m_pRequestUnitReadEventNotifier			= nullptr;
		//////////////////////////////////////////////////////////////////////////////////
		//XML
		//
	protected:
		HANDLE	m_hTimerCheck = NULL;
		HANDLE	m_hTimerQueue = NULL;
		void	CreateTimeCheck();
		void	DeleteTimerCheck();

		static VOID CALLBACK TimerRoutineCheck(__in PVOID lpParam, __in BOOLEAN TimerOrWaitFired);
	//LINK_TEST
	private:
		using LinkTestCntr = StdMapContainer <XUUID, Link_Test_Args::Args>::Map;
		LinkTestCntr * m_pLinkTestCntr = nullptr;
		StdMutex * m_pLinkTestCntrMutex = nullptr;
	public:
		XUUID LinkTestCtrl();
		Link_Test_Args::Args LinkTestCtrl(XUUID UUID);
		bool CreateLinkTestProcedure(XUUID UUID);
		bool AddLinkTestProcedure(XUUID UUID, Request_Link_Test_Args::Args & proc);
		bool AddLinkTestProcedure(XUUID UUID, Reply_Link_Test_Args::Args & proc);
		inline Request_Link_Test_Args::Args ParseArg_REQUEST_LINK_TEST(StdStringA pDataz);
		inline StdStringA Make_REPLY_LINK_TEST(Reply_Link_Test_Args::Args & proc);
		bool bGetLinkTestProcedure(XUUID UUID) const;
		void RemoveLinkTestProcedure(XUUID UUID);
	//ONLINE_STATE
	private:
		using OnlineStateCntr = StdMapContainer <XUUID, Online_State_Args::Args >::Map;
		OnlineStateCntr * m_pOnlineStateCntr = nullptr;
		StdMutex * m_pOnlineStateCntrMutex = nullptr;
	public:
		XUUID OnlineStateCtrl();
		Online_State_Args::Args OnlineStateCtrl(XUUID UUID);
		bool CreateOnlineStateProcedure(XUUID UUID);
		bool CreateOnlineStateProcedure();
		bool AddOnlineStateProcedure(XUUID UUID, Report_Online_State_Args::Args & proc);
		inline StdStringA Make_REPORT_ONLINE_STATE(Report_Online_State_Args::Args & pData);
		bool bGetOnlineStateProcedure(XUUID UUID) const;
		void RemoveOnlineStateProcedure(XUUID UUID);
	//CUSER_COMMAND
	private:
		using UserCommandCntr = StdMapContainer <XUUID, User_Change_Args::Args >::Map;
		UserCommandCntr * m_pUserCommandCntr = nullptr;
		StdMutex * m_pUserCommandCntrMutex = nullptr;
	public:
		XUUID UserCommandCtrl();
		User_Change_Args::Args UserCommandCtrl(XUUID UUID);
		bool CreateUserCommandProcedure(XUUID UUID);
		bool CreateUserCommandProcedure();
		bool AddeUserCommandProcedure(XUUID UUID, Report_User_Change_Args::Args & proc);
		bool AddeUserCommandProcedure(XUUID UUID, Request_User_Cpmmand_Args::Args & proc);
		bool AddeUserCommandProcedure(XUUID UUID, Reply_User_Change_Args::Args & proc);
		inline StdStringA Make_REPORT_USER_CHANGE(Report_User_Change_Args::Args & pData);
		inline Request_User_Cpmmand_Args::Args ParseArg_REQUEST_USER_COMMAND(StdStringA pDataz);
		inline StdStringA Make_REPLY_USER_COMMAND(Reply_User_Change_Args::Args & pData);
		bool bGetUserCommandProcedure(XUUID UUID) const;
		void RemoveUserCommandProcedure(XUUID UUID);

	//CEQUIPMENT_STATE
	private:
		using EquipmentStateCntr = StdMapContainer <XUUID, Equipment_State_Args::Args >::Map;
		EquipmentStateCntr * m_pEquipmentStateCntr = nullptr;
		StdMutex * m_pEquipmentStateCntrMutex = nullptr;
	public:
		XUUID EquipmentStateCtrl();
		Equipment_State_Args::Args EquipmentStateCtrl(XUUID UUID);
		bool CreateEquipmentStateProcedure(XUUID UUID);
		bool CreateEquipmentStateProcedure();
		bool AddEquipmentStateProcedure(XUUID UUID, Report_Equipment_State_Args::Args & proc);
		bool AddEquipmentStateProcedure(XUUID UUID, Request_Equipment_State_Display_Args::Args & proc);
		bool AddEquipmentStateProcedure(XUUID UUID, Reply_Equipment_State_Display_Args::Args & proc);
		inline StdStringA Make_REPORT_EQUIPMENT_STATE(Report_Equipment_State_Args::Args & pData);
		inline Request_Equipment_State_Display_Args::Args ParseArg_REQUEST_EQUIPMENT_STATE_DISPLAY (StdStringA pDataz);
		inline StdStringA Make_REPLY_EQUIPMENT_STATE_DISPLAY(Reply_Equipment_State_Display_Args::Args & pData);
		bool bGetEquipmentStateProcedure(XUUID UUID) const;
		void RemoveEquipmentStateProcedure(XUUID UUID);

	//CLOSS_WINDOW
	private:
		using LossWindowCntr = StdMapContainer <XUUID, Loss_Window_Args::Args >::Map;
		LossWindowCntr * m_pLossWindowCntr = nullptr;
		StdMutex * m_pLossWindowCntrMutex = nullptr;
	public:
		XUUID LossWindowCtrl();
		Loss_Window_Args::Args LossWindowCtrl(XUUID UUID);
		bool CreateLossWindowProcedure(XUUID UUID);
		bool CreateLossWindowProcedure();
		bool AddLossWindowProcedure(XUUID UUID, Report_Loss_State_Args::Args & proc);
		bool AddLossWindowProcedure(XUUID UUID, Request_Loss_Window_Args::Args & proc);
		bool AddLossWindowProcedure(XUUID UUID, Reply_Loss_Window_Args::Args & proc);
		inline StdStringA Make_REPORT_LOSS_STATE(Report_Loss_State_Args::Args & pData);
		inline Request_Loss_Window_Args::Args ParseArg_REQUEST_LOSS_WINDOW(StdStringA pDataz);
		inline StdStringA Make_REPLY_LOSS_WINDOW(Reply_Loss_Window_Args::Args & pData);
		bool bGetLossWindowProcedure(XUUID UUID) const;
		void RemoveLossWindowProcedure(XUUID UUID);

	//CALARM_STATE
	private:
		using AlarmStateCntr = StdMapContainer <XUUID, Alarm_State_Args::Args >::Map;
		AlarmStateCntr * m_pAlarmStateCntr = nullptr;
		StdMutex * m_pAlarmStateCntrMutex = nullptr;
	public:
		XUUID AlarmStateCtrl();
		Alarm_State_Args::Args AlarmStateCtrl(XUUID UUID);
		bool CreateAlarmStateProcedure(XUUID UUID);
		bool CreateAlarmStateProcedure();
		bool AddAlarmStateProcedure(XUUID UUID, Report_Alarm_State_Args::Args & proc);
		inline StdStringA Make_REPORT_ALARM_STATE(Report_Alarm_State_Args::Args & pData);
		bool bGetAlarmStateProcedure(XUUID UUID) const;
		void RemoveAlarmStateProcedure(XUUID UUID);

	//CALARM_LIST
	private:
		using AlarmListCntr = StdMapContainer <XUUID, Alarm_List_Args::Args >::Map;
		AlarmListCntr * m_pAlarmListCntr = nullptr;
		StdMutex * m_pAlarmListCntrMutex = nullptr;
	public:
		XUUID AlarmListCtrl();
		Alarm_List_Args::Args  AlarmListCtrl(XUUID UUID);
		bool CreateAlarmListProcedure(XUUID UUID);
		bool CreateAlarmListProcedure();
		bool AddAlarmListProcedure(XUUID UUID, Request_Alarm_List_Args::Args & proc);
		bool AddAlarmListProcedure(XUUID UUID, Reply_Alarm_List_Args::Args & proc);
		inline Request_Alarm_List_Args::Args ParseArg_REQUEST_ALARM_LIST(StdStringA pData);
		inline StdStringA Make_REPLY_ALARM_LIST(Reply_Alarm_List_Args::Args & pData);
		bool bGetAlarmListProcedure(XUUID UUID) const;
		void RemoveAlarmListProcedure(XUUID UUID);

		//CALARM_DataListCntr LoadAlarmList();
		CALARM_DataListCntr LoadXML_ALInfo(__in StdStringA szPath);

	//CRMS_MODE
	private:
		using RmsModeCntr = StdMapContainer <XUUID, Rms_Mode_Args::Args >::Map;
		RmsModeCntr * m_pRmsModeCntr = nullptr;
		StdMutex * m_pRmsModeCntrMutex = nullptr;
	public:
		XUUID m_pRmsModeCtrl();
		Rms_Mode_Args::Args m_pRmsModeCtrl(XUUID UUID);
		bool CreateRmsModeProcedure(XUUID UUID);
		bool CreateRmsModeProcedure();
		bool AddRmsModeProcedure(XUUID UUID, Report_Rms_Mode_Args::Args & proc);
		inline StdStringA Make_REPORT_RMS_MODE(Report_Rms_Mode_Args::Args & pData);
		bool bGetRmsModeProcedure(XUUID UUID) const;
		void RemoveRmsModeProcedure(XUUID UUID);

	//CSET_DATETIME
	private:
		using SetDateTimeCntr = StdMapContainer <XUUID, Set_DateTime_Args::Args >::Map;
		SetDateTimeCntr * m_pSetDateTimeCntr = nullptr;
		StdMutex * m_pSetDateTimeCntrMutex = nullptr;
	public:
		XUUID SetDateTimeCtrl();
		Set_DateTime_Args::Args  SetDateTimeCtrl(XUUID UUID);
		bool CreateSetDateTimeProcedure(XUUID UUID);
		bool CreateSetDateTimeProcedure();
		bool AddSetDateTimeProcedure(XUUID UUID, Request_Set_DateTime_Args::Args & proc);
		bool AddSetDateTimeProcedure(XUUID UUID, Reply_Set_DateTime_Args::Args & proc);
		inline Request_Set_DateTime_Args::Args ParseArg_REQUEST_SET_DATETIME(StdStringA pData);
		inline StdStringA Make_REPLY_SET_DATETIME(Reply_Set_DateTime_Args::Args & pData);
		bool bGetSetDateTimeProcedure(XUUID UUID) const;
		void RemoveSetDateTimeProcedure(XUUID UUID);

	//CTERMINAL_MESSAGE
	private:
		using TerminalMessageCntr = StdMapContainer <XUUID, Terminal_Message_Args::Args >::Map;
		TerminalMessageCntr * m_pTerminalMessageCntr = nullptr;
		StdMutex * m_pTerminalMessageCntrMutex = nullptr;
	public:
		XUUID TerminalMessageCtrl();
		Terminal_Message_Args::Args  TerminalMessageCtrl(XUUID UUID);
		bool CreateTerminalMessageProcedure(XUUID UUID);
		bool CreateTerminalMessageProcedure();
		bool AddTerminalMessageProcedure(XUUID UUID, Request_Terminal_Message_Args::Args & proc);
		bool AddTerminalMessageProcedure(XUUID UUID, Reply_Terminal_Message_Args::Args & proc);
		inline Request_Terminal_Message_Args::Args ParseArg_REQUEST_TERMINAL_MESSAGE(StdStringA pData);
		inline StdStringA Make_REPLY_TERMINAL_MESSAGE(Reply_Terminal_Message_Args::Args & pData);
		bool bGetTerminalMessageProcedure(XUUID UUID) const;
		void RemoveTerminalMessageProcedure(XUUID UUID);
	//COPCALL
	private:
		using OpcallCntr = StdMapContainer <XUUID, Opcall_Args::Args >::Map;
		OpcallCntr * m_pOpcallCntr = nullptr;
		StdMutex * m_pOpcallCntrMutex = nullptr;
	public:
		XUUID OpcallCtrl();
		Opcall_Args::Args  OpcallCtrl(XUUID UUID);
		bool CreateOpcallProcedure(XUUID UUID);
		bool CreateOpcallProcedure();
		bool AddOpcallProcedure(XUUID UUID, Request_Opcall_Args::Args & proc);
		bool AddOpcallProcedure(XUUID UUID, Reply_Opcall_Args::Args & proc);
		inline Request_Opcall_Args::Args ParseArg_REQUEST_OPCALL(StdStringA pData);
		inline StdStringA Make_REPLY_OPCALL(Reply_Opcall_Args::Args & pData);
		bool bGetOpcallProcedure(XUUID UUID) const;
		void RemoveOpcallProcedure(XUUID UUID);
#endif 
	};

}
