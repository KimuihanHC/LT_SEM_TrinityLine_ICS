#include "pch.h"
#include "Comm/IcsRemoteEes.h"
#include "IcsRequestEvent.hpp"
#include "Util/StringUtil.hpp"

#include <tchar.h>
#include <map>
#include <future>
#include <sstream>

using namespace lt;

struct SendingCommand
{	
	//EPT Scenario
	//Connection
	static constexpr ConstStringT REPLY_LINK_TEST				= _T("REPLY_LINK_TEST");
	//OnLine State
	static constexpr ConstStringT REPORT_ONLINE_STATE			= _T("REPORT_ONLINE_STATE");
	//User Change
	static constexpr ConstStringT REPORT_USER_CHANGE			= _T("REPORT_USER_CHANGE");
	static constexpr ConstStringT REPLY_USER_COMMAND			= _T("REPLY_USER_COMMAND");
	//Equipment State Change
	static constexpr ConstStringT REPORT_EQUIPMENT_STATE		= _T("REPORT_EQUIPMENT_STATE");
	static constexpr ConstStringT REPLY_EQUIPMENT_STATE_DISPLAY	= _T("REPLY_EQUIPMENT_STATE_DISPLAY");
	//Loss State Report
	static constexpr ConstStringT REPORT_LOSS_STATE				= _T("REPORT_LOSS_STATE");
	static constexpr ConstStringT REPLY_LOSS_WINDOW				= _T("REPLY_LOSS_WINDOW");
	//ALARM Report
	static constexpr ConstStringT REPORT_ALARM_STATE			= _T("REPORT_ALARM_STATE");
	//Alarm List
	static constexpr ConstStringT REPLY_ALARM_LIST				= _T("REPLY_ALARM_LIST");
	//RMS Mode Change.
	static constexpr ConstStringT REPORT_RMS_MODE				= _T("REPORT_RMS_MODE");
	//Date/Time Synchronization
	static constexpr ConstStringT REPLY_SET_DATETIME			= _T("REPLY_SET_DATETIME");
	//Terminal Message
	static constexpr ConstStringT REPLY_TERMINAL_MESSAGE		= _T("REPLY_TERMINAL_MESSAGE");
	//Operation
	static constexpr ConstStringT REPLY_OPCALL					= _T("REPLY_OPCALL");
	//Lot
	static constexpr ConstStringT REPORT_START_LOT				= _T("REPORT_START_LOT");
	static constexpr ConstStringT REPORT_START_PROCESS			= _T("REPORT_START_PROCESS");
	static constexpr ConstStringT REPORT_END_PROCESS			= _T("REPORT_END_PROCESS");
	static constexpr ConstStringT REPORT_END_EVENT				= _T("REPORT_END_EVENT");
	//UNITID_READ
	static constexpr ConstStringT UNITID_READ					= _T("UNITID_READ");
	static constexpr ConstStringT REPLY_UNITID_READ				= _T("REPLY_UNITID_READ");
};

struct ReceivingCommand
{
	//EPT Scenario
	//Connection 
	static constexpr ConstStringT REQUEST_LINK_TEST					= _T("REQUEST_LINK_TEST");
	//OnLine State
	//User Change
	static constexpr ConstStringT REQUEST_USER_COMMAND				= _T("REQUEST_USER_COMMAND");
	//Equipment State Change
	static constexpr ConstStringT REQUEST_EQUIPMENT_STATE_DISPLAY	= _T("REQUEST_EQUIPMENT_STATE_DISPLAY");
	//Loss State Report
	static constexpr ConstStringT REQUEST_LOSS_WINDOW				= _T("REQUEST_LOSS_WINDOW");
	//Alarm List
	static constexpr ConstStringT REQUEST_ALARM_LIST				= _T("REQUEST_ALARM_LIST");
	//Date/Time Synchronization
	static constexpr ConstStringT REQUEST_SET_DATETIME				= _T("REQUEST_SET_DATETIME");
	//Terminal Message
	static constexpr ConstStringT REQUEST_TERMINAL_MESSAGE			= _T("REQUEST_TERMINAL_MESSAGE");
	//Operation
	static constexpr ConstStringT REQUEST_OPCALL					= _T("REQUEST_OPCALL");
	//UNITID_READ
	static constexpr ConstStringT REQUEST_UNITID_READ				= _T("REQUEST_UNITID_READ");
};

CIcsRemoteEes::CIcsRemoteEes()
	: CIcsCommunicator(CommId::Server)
{
#if USE_XML
	m_pRequestLinkTestEventNotifier			= new Request_Link_Test_EventNotifierImpl();
	m_pRequestUserCommandEventNotifier		= new Request_User_Cpmmand_EventNotifierImpl();
	m_pRequestEquipmentStateEventNotifier	= new Request_Equipment_State_Display_EventNotifierImpl();
	m_pRequestLossWindowEventNotifier		= new Request_Loss_Window_EventNotifierImpl();
	m_pRequestAlarmListEventNotifier		= new Request_Alarm_List_EventNotifierImpl();	
	m_pRequestSetDateTimeEventNotifier		= new Request_Set_DateTime_EventNotifierImpl();
	m_pRequestTerminalMessageEventNotifier	= new Request_Terminal_Message_EventNotifierImpl();
	m_pRequestOPCallEventNotifier			= new Request_Opcall_EventNotifierImpl();
	//m_pRequestUnitReadEventNotifier			= new xmlEventNotifierImpl();
	
	//List Test	
	AddRequestProcedure(ReceivingCommand::REQUEST_LINK_TEST,
		reinterpret_cast<RequestProc>(
			&CIcsRemoteEes::ProcRequestLinkTest));
	//Alarm List
	AddRequestProcedure(ReceivingCommand::REQUEST_ALARM_LIST,
		reinterpret_cast<RequestProc>(
			&CIcsRemoteEes::ProcRequestAlarmList));
	//User Change
	AddRequestProcedure(ReceivingCommand::REQUEST_USER_COMMAND,
		reinterpret_cast<RequestProc>(
			&CIcsRemoteEes::ProcRequestUserCommand));
	//Equipment State
	AddRequestProcedure(ReceivingCommand::REQUEST_EQUIPMENT_STATE_DISPLAY,
		reinterpret_cast<RequestProc>(
			&CIcsRemoteEes::ProcRequestEquipmentStateDisplay));
	//Date/Time Synchronization
	AddRequestProcedure(ReceivingCommand::REQUEST_SET_DATETIME,
		reinterpret_cast<RequestProc>(
			&CIcsRemoteEes::ProcRequestSetDateTime));
	//Terminal Message
	AddRequestProcedure(ReceivingCommand::REQUEST_TERMINAL_MESSAGE,
		reinterpret_cast<RequestProc>(
			&CIcsRemoteEes::ProcRequestTerminalMessage));
	//Op Call
	AddRequestProcedure(ReceivingCommand::REQUEST_OPCALL,
		reinterpret_cast<RequestProc>(
			&CIcsRemoteEes::ProcRequestOPCall));
	//LOSS_WINDOW 추가
	AddRequestProcedure(ReceivingCommand::REQUEST_LOSS_WINDOW,
		reinterpret_cast<RequestProc>(
			&CIcsRemoteEes::ProcRequestLossWindow));

	//UNIT READ
	//AddRequestProcedure(ReceivingCommand::REQUEST_UNITID_READ,
	//	reinterpret_cast<RequestProc>(
	//		&CIcsRemoteEes::ProcRequestUnitRead));

	//CreateTimeCheck();
	m_pLinkTestCntr = new LinkTestCntr();
	m_pLinkTestCntrMutex = new StdMutex();

	m_pOnlineStateCntr = new OnlineStateCntr();
	m_pOnlineStateCntrMutex = new StdMutex();

	m_pUserCommandCntr = new UserCommandCntr();
	m_pUserCommandCntrMutex = new StdMutex();

	m_pEquipmentStateCntr = new EquipmentStateCntr();
	m_pEquipmentStateCntrMutex = new StdMutex();

	m_pLossWindowCntr = new LossWindowCntr();
	m_pLossWindowCntrMutex = new StdMutex();

	m_pAlarmStateCntr = new AlarmStateCntr();
	m_pAlarmStateCntrMutex = new StdMutex();

	m_pAlarmListCntr = new AlarmListCntr();
	m_pAlarmListCntrMutex = new StdMutex();

	m_pRmsModeCntr = new RmsModeCntr();
	m_pRmsModeCntrMutex = new StdMutex();

	m_pSetDateTimeCntr = new SetDateTimeCntr();
	m_pSetDateTimeCntrMutex = new StdMutex();

	m_pTerminalMessageCntr = new TerminalMessageCntr();
	m_pTerminalMessageCntrMutex = new StdMutex();

	m_pOpcallCntr = new OpcallCntr();
	m_pOpcallCntrMutex = new StdMutex();
#endif
}
CIcsRemoteEes::~CIcsRemoteEes()
{
#if USE_XML	
	//DeleteTimerCheck();
	delete m_pOpcallCntr;
	delete m_pOpcallCntrMutex;

	delete m_pTerminalMessageCntr;
	delete m_pTerminalMessageCntrMutex;

	delete m_pSetDateTimeCntr ;
	delete m_pSetDateTimeCntrMutex ;

	delete m_pRmsModeCntr;
	delete m_pRmsModeCntrMutex;

	delete m_pAlarmListCntr;
	delete m_pAlarmListCntrMutex;

	delete m_pAlarmStateCntrMutex;
	delete m_pAlarmStateCntr;	

	delete m_pLossWindowCntr;
	delete m_pLossWindowCntrMutex;

	delete m_pEquipmentStateCntr;
	delete m_pEquipmentStateCntrMutex;

	delete m_pUserCommandCntr;
	delete m_pUserCommandCntrMutex;

	delete m_pLinkTestCntr;
	delete m_pLinkTestCntrMutex;

	delete m_pOnlineStateCntr;
	delete m_pOnlineStateCntrMutex;

	delete m_pRequestLinkTestEventNotifier;
	delete m_pRequestAlarmListEventNotifier;
	delete m_pRequestUserCommandEventNotifier;
	delete m_pRequestEquipmentStateEventNotifier;
	delete m_pRequestLossWindowEventNotifier;
	delete m_pRequestSetDateTimeEventNotifier;
	delete m_pRequestTerminalMessageEventNotifier;
	delete m_pRequestOPCallEventNotifier;
	//delete m_pRequestUnitReadEventNotifier;
#endif	
}
#if USE_XML
//Link Test
uint32 CIcsRemoteEes::CommandReplyLinkTest(Reply_Link_Test_Args::Args & xml)
{
	return EesXmlProcCommand(SendingCommand::REPLY_LINK_TEST,
		{
			#if defined(UNICODE)
			Make_REPLY_LINK_TEST(xml),//xml.Msg.Get_MSG(),
			#else
			(xml.Msg),
			#endif
		});
}
uint32 CIcsRemoteEes::CommandReportOnlineState(Report_Online_State_Args::Args  & xml)
{
	return EesXmlProcCommand(SendingCommand::REPORT_ONLINE_STATE,
		{
			#if defined(UNICODE)
			Make_REPORT_ONLINE_STATE(xml),
			#else
			xml.msg
			#endif
		});
}
//User Change
uint32 CIcsRemoteEes::CommandReportUserChange(Report_User_Change_Args::Args & xml)
{
	
	return EesXmlProcCommand(SendingCommand::REPORT_USER_CHANGE,
		{
			#if defined(UNICODE)
			Make_REPORT_USER_CHANGE(xml),
			#else
			xml.msg
			#endif
		});
}
uint32 CIcsRemoteEes::CommandReplyUserCommand(Reply_User_Change_Args::Args & xml)
{
	return EesXmlProcCommand(SendingCommand::REPLY_USER_COMMAND,
		{
			#if defined(UNICODE)
			Make_REPLY_USER_COMMAND(xml),
			#else
			xml.msg
			#endif
		});
}

//Equipment State Chage.
uint32 CIcsRemoteEes::CommandReportEquipmentState(Report_Equipment_State_Args::Args & xml)
{
	return EesXmlProcCommand(SendingCommand::REPORT_EQUIPMENT_STATE,
		{
			#if defined(UNICODE)
			Make_REPORT_EQUIPMENT_STATE(xml),
			#else
			xml.msg
			#endif
		});
}
uint32 CIcsRemoteEes::CommandReplyEquipmentStateDisplay(Reply_Equipment_State_Display_Args::Args & xml)
{
	return EesXmlProcCommand(SendingCommand::REPLY_EQUIPMENT_STATE_DISPLAY,
		{
			#if defined(UNICODE)
			Make_REPLY_EQUIPMENT_STATE_DISPLAY(xml),
			#else
			xml.msg
			#endif
		});
}

//Loss State Report
uint32 CIcsRemoteEes::CommandReportLossState(Report_Loss_State_Args::Args & xml)
{
	//2023.04.14a uhkim 변경
	return EesXmlProcCommand(SendingCommand::REPORT_LOSS_STATE,
		{
			#if defined(UNICODE)
			Make_REPORT_LOSS_STATE(xml),
			#else
			xml.msg
			#endif
		});
}

//Reply Loss Window.
uint32 CIcsRemoteEes::CommandReplyLossWindow(Reply_Loss_Window_Args::Args  & xml)
{
	return EesXmlProcCommand(SendingCommand::REPLY_LOSS_WINDOW,
		{
			#if defined(UNICODE)
			Make_REPLY_LOSS_WINDOW(xml),
			#else
			xml.ReportMsg
			#endif
		});
}
//Alarm State.
uint32 CIcsRemoteEes::CommandReportAlarmState(Report_Alarm_State_Args::Args & xml)
{
	return EesXmlProcCommand(SendingCommand::REPORT_ALARM_STATE,
		{
			#if defined(UNICODE)
			Make_REPORT_ALARM_STATE(xml),
			#else
			xml.msg
			#endif
		});
}
uint32 CIcsRemoteEes::CommandReplyAlarmList(Reply_Alarm_List_Args::Args & xml)
{
	return EesXmlProcCommand(SendingCommand::REPLY_ALARM_LIST,
		{
			#if defined(UNICODE)
			Make_REPLY_ALARM_LIST(xml),
			#else
			xml.msg
			#endif
		});
}
//REPORT RMS MODE
uint32 CIcsRemoteEes::CommandReportRMSMode(Report_Rms_Mode_Args::Args  & xml)
{
	return EesXmlProcCommand(SendingCommand::REPORT_RMS_MODE,
		{
			#if defined(UNICODE)
			Make_REPORT_RMS_MODE(xml),
			#else
			xml.msg
			#endif
		});
}
uint32 CIcsRemoteEes::CommandReplySetDateTime(Reply_Set_DateTime_Args::Args & xml)
{

	return EesXmlProcCommand(SendingCommand::REPLY_SET_DATETIME,
		{
			#if defined(UNICODE)
			Make_REPLY_SET_DATETIME(xml),
			#else
			xml.ReportMsg
			#endif
		});
}
//Terminal Message
uint32 CIcsRemoteEes::CommandReplyTerminalMessage(Reply_Terminal_Message_Args::Args & xml) {
	return EesXmlProcCommand(SendingCommand::REPLY_TERMINAL_MESSAGE,
		{
			#if defined(UNICODE)
			Make_REPLY_TERMINAL_MESSAGE(xml),
			#else
			xml.ReportMsg
			#endif
		});
}
//Op Call.
uint32 CIcsRemoteEes::CommandReplyOpCall(Reply_Opcall_Args::Args & xml)
{

	return EesXmlProcCommand(SendingCommand::REPLY_OPCALL,
		{
			#if defined(UNICODE)
			Make_REPLY_OPCALL(xml),
			#else
			xml.msg
			#endif
		});
}

#if TEST
uint32 CIcsRemoteEes::CommandUnitIdRead(const ST_xml_Msg & xml)
{
	return EesXmlProcCommand(SendingCommand::UNITID_READ,
		{
			#if defined(UNICODE)
			ToMultiByte(xml.Msg)
			#else
			xml.msg
			#endif
		});
}
uint32 CIcsRemoteEes::CommandReplyUnitIdRead(const ST_xml_Msg & xml)
{
	return EesXmlProcCommand(SendingCommand::REPLY_UNITID_READ,
		{
			#if defined(UNICODE)
			ToMultiByte(xml.Msg)
			#else
			xml.msg
			#endif
		});
}

//REPORT_START_LOT
uint32 CIcsRemoteEes::CommandReportStartLot(const ST_xml_Msg & xml)
{
	return EesXmlProcCommand(SendingCommand::REPORT_START_LOT,
		{
			#if defined(UNICODE)
			ToMultiByte(xml.Msg)
			#else
			xml.msg
			#endif
		});
}
//REPORT_START_PROCESS
uint32 CIcsRemoteEes::CommandReportStartProcess(const ST_xml_Msg & xml)
{
	return EesXmlProcCommand(SendingCommand::REPORT_START_PROCESS,
		{
			#if defined(UNICODE)
			ToMultiByte(xml.Msg)
			#else
			xml.msg
			#endif
		});
}
uint32 CIcsRemoteEes::CommandReportEndProcess(const ST_xml_Msg & xml)
{
	return EesXmlProcCommand(SendingCommand::REPORT_END_PROCESS,
		{
			#if defined(UNICODE)
			ToMultiByte(xml.Msg)
			#else
			xml.msg
			#endif
		});
}
uint32 CIcsRemoteEes::CommandReportEndEvent(const ST_xml_Msg & xml)
{
	return EesXmlProcCommand(SendingCommand::REPORT_END_EVENT,
		{
			#if defined(UNICODE)
			ToMultiByte(xml.Msg)
			#else
			xml.msg
			#endif
		});
}
#endif


//==================================================================================================
bool CIcsRemoteEes::ProcRequestLinkTest(const DataCntr & dataCntr) {
	if (dataCntr.empty())
		return false;
	RaiseRequestLinkTestEvent(	
		{ ParseArg_REQUEST_LINK_TEST(dataCntr[0])}
	);
	return true;
}
//User change
bool CIcsRemoteEes::ProcRequestUserCommand(const DataCntr & dataCntr) {
	if (dataCntr.empty())
		return false;
	RaiseRequestUserCommandEvent(
		{ ParseArg_REQUEST_USER_COMMAND(dataCntr[0]), }
	);
	return true;
}
//Equipment State change
bool CIcsRemoteEes::ProcRequestEquipmentStateDisplay(const DataCntr & dataCntr)
{
	if (dataCntr.empty())
		return false;
	RaiseRequestEqipmentStateDisplayEvent(
		{ ParseArg_REQUEST_EQUIPMENT_STATE_DISPLAY(dataCntr[0]), });
	return true;
}
//Loss Window
bool CIcsRemoteEes::ProcRequestLossWindow(const DataCntr & dataCntr)
{
	if (dataCntr.empty())
		return false;
	RaiseRequestLossWindowEvent(
		{ ParseArg_REQUEST_LOSS_WINDOW(dataCntr[0]) });
	return true;
}

//Alarm List.
bool CIcsRemoteEes::ProcRequestAlarmList(const DataCntr & dataCntr) {
	if (dataCntr.empty())
		return false;
	RaiseRequestAlarmListEvent(
		{ ParseArg_REQUEST_ALARM_LIST(dataCntr[0]), });
	return true;
}
//Data/Time Synchronization
bool CIcsRemoteEes::ProcRequestSetDateTime(const DataCntr & dataCntr) {
	if (dataCntr.empty())
		return false;
	//RaiseRequestSetDateTimeEvent(
	//	{ ToTypeChar(dataCntr[0]).c_str(), });
	return true;
}
//TerminalMessage
bool CIcsRemoteEes::ProcRequestTerminalMessage(const DataCntr & dataCntr) {
	if (dataCntr.empty())
		return false;
	//RaiseRequestTerminalMessageEvent(
	//	{ ToTypeChar(dataCntr[0]).c_str(), });
	return true;
}
//Op Call
bool CIcsRemoteEes::ProcRequestOPCall(const DataCntr & dataCntr) {
	if (dataCntr.empty())
		return false;
	// RaiseRequestOPCallEvent(
	//	{ ToTypeChar(dataCntr[0]).c_str(), });
	return true;
}

//==================================================================================================
/*Request Unit Read
bool CIcsRemoteEes::ProcRequestUnitRead(const DataCntr & dataCntr) {
	if (dataCntr.empty())
		return false;
	RaiseRequestUnitReadEvent(
		{ ToTypeChar(dataCntr[0]).c_str(), });
	return true;
}*/

void CIcsRemoteEes::RaiseRequestLinkTestEvent(Request_Link_Test_Args::Args & args)
{		
	EESRaiseEvent(*this,
		*m_pRequestLinkTestEventNotifier,
		args);
}
void CIcsRemoteEes::RaiseRequestUserCommandEvent(Request_User_Cpmmand_Args::Args & args)
{
	EESRaiseEvent(*this,
		*m_pRequestUserCommandEventNotifier,
		args);
}
void CIcsRemoteEes::RaiseRequestEqipmentStateDisplayEvent(Request_Equipment_State_Display_Args::Args & args)
{
	EESRaiseEvent(*this,
		*m_pRequestEquipmentStateEventNotifier,
		args);
}
void CIcsRemoteEes::RaiseRequestLossWindowEvent(Request_Loss_Window_Args::Args & args)
{
	EESRaiseEvent(*this,
		*m_pRequestLossWindowEventNotifier,
		args);
}
void CIcsRemoteEes::RaiseRequestAlarmListEvent(Request_Alarm_List_Args::Args & args)
{
	EESRaiseEvent(*this,
		*m_pRequestAlarmListEventNotifier,
		args);
}
//Data/Time Synchronization
void CIcsRemoteEes::RaiseRequestSetDateTimeEvent(Request_Set_DateTime_Args::Args & args)
{
	EESRaiseEvent(*this,
		*m_pRequestSetDateTimeEventNotifier,
		args);
}
void CIcsRemoteEes::RaiseRequestTerminalMessageEvent(Request_Terminal_Message_Args::Args & args)
{
	EESRaiseEvent(*this,
		*m_pRequestTerminalMessageEventNotifier,
		args);
}
void CIcsRemoteEes::RaiseRequestOPCallEvent(Request_Opcall_Args::Args & args)
{
	EESRaiseEvent(*this,
		*m_pRequestOPCallEventNotifier,
		args);
}
/*
void CIcsRemoteEes::RaiseRequestUnitReadEvent(XmlArgs::Args & args)
{
	EESRaiseEvent(*this,
		*m_pRequestUnitReadEventNotifier,
		args);
}*/
//==================================================================================================



//Request Replay
//1. Request Link Test
Request_Link_Test_EventNotifier<CIcsRemoteEes> &
CIcsRemoteEes::GetRequestLinkTestEventNotifier()
{	
	return *m_pRequestLinkTestEventNotifier;
}
Request_User_Command_EventNotifier<CIcsRemoteEes> &
CIcsRemoteEes::GetRequestUserCommandEventNotifier()
{
	return *m_pRequestUserCommandEventNotifier;
}
Equipment_State_Display_EventNotifier<CIcsRemoteEes> &
CIcsRemoteEes::GetRequestEquipmentStateEventNotifier()
{
	return *m_pRequestEquipmentStateEventNotifier;
}
Request_Loss_Window_EventNotifier<CIcsRemoteEes> &
CIcsRemoteEes::GetRequestLossWindowEventNotifier()
{
	return *m_pRequestLossWindowEventNotifier;
}
Request_Alarm_List_EventNotifier<CIcsRemoteEes> &
CIcsRemoteEes::GetRequestAlarmListEventNotifier()
{
	return *m_pRequestAlarmListEventNotifier;
}
Request_Set_DateTime_EventNotifier<CIcsRemoteEes> &
CIcsRemoteEes::GetRequestSetDateTimeEventNotifier()
{
	return *m_pRequestSetDateTimeEventNotifier;
}
Request_Terminal_Message_EventNotifier<CIcsRemoteEes> &
CIcsRemoteEes::GetRequestTerminalMessageEventNotifier()
{
	return *m_pRequestTerminalMessageEventNotifier;
}
Request_Opcall_EventNotifier<CIcsRemoteEes> &
CIcsRemoteEes::GetRequestOPCallEventNotifier()
{
	return *m_pRequestOPCallEventNotifier;
}
/*
XmlEventNotifier<CIcsRemoteEes> &
CIcsRemoteEes::GetRequestUnitReadEventNotifier()
{
	return *m_pRequestUnitReadEventNotifier;
}*/

void CIcsRemoteEes::CreateTimeCheck()
{
	__try
	{
		// Time Check Timer
		if (NULL == m_hTimerCheck)
			if (!CreateTimerQueueTimer(&m_hTimerCheck, m_hTimerQueue, (WAITORTIMERCALLBACK)TimerRoutineCheck, (PVOID)this, 3000, 50, WT_EXECUTEDEFAULT))
			{
			}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
	}
}
void CIcsRemoteEes::DeleteTimerCheck()
{
	__try
	{
		if (DeleteTimerQueueTimer(m_hTimerQueue, m_hTimerCheck, NULL))
		{
			m_hTimerCheck = NULL;
		}
		else
		{
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
	}
}
VOID CALLBACK CIcsRemoteEes::TimerRoutineCheck(__in PVOID lpParam, __in BOOLEAN TimerOrWaitFired)
{
	CIcsRemoteEes* pThis = (CIcsRemoteEes*)lpParam;

	//pThis->OnMonitorSensorCheck();
}

XUUID CIcsRemoteEes::LinkTestCtrl(){
	SimpleLockPtr(m_pLinkTestCntrMutex);
	for (auto iter = m_pLinkTestCntr->begin();
		iter != m_pLinkTestCntr->end(); )	{
		if (iter != m_pLinkTestCntr->end()) {
			return (*iter).first;
		}
		iter++;
	}
	return XUUID();
}
Link_Test_Args::Args  CIcsRemoteEes::LinkTestCtrl(XUUID UUID){
	SimpleLockPtr(m_pLinkTestCntrMutex);
	for (auto iter = m_pLinkTestCntr->begin();
		iter != m_pLinkTestCntr->end(); )	{
		auto csttCommand = (*iter).first;
		if (csttCommand == UUID) 	{
			return (*iter).second;
		}
		iter++;
	}
	return  Link_Test_Args::Args();
}
bool CIcsRemoteEes::CreateLinkTestProcedure(XUUID UUID){
	SimpleLockPtr(m_pLinkTestCntrMutex);
	for (auto iter = m_pLinkTestCntr->begin();	
		iter != m_pLinkTestCntr->end(); )	{
		auto csttCommand = (*iter).first;
		if (csttCommand == UUID) 		{
			return false;
		}
		iter++;
	}	
	auto result = m_pLinkTestCntr->emplace(UUID, Link_Test_Args::Args());
	return result.second;
}
bool CIcsRemoteEes::AddLinkTestProcedure(XUUID UUID, Request_Link_Test_Args::Args &proc){
	SimpleLockPtr(m_pLinkTestCntrMutex);
	for (auto iter = m_pLinkTestCntr->begin();
		iter != m_pLinkTestCntr->end(); )	{
		auto csttCommand = (*iter).first;
		if (csttCommand == UUID) 		{
			(*iter).second.REQUEST = proc;
			return true;
		}
		iter++;
	}
	return false;
}
bool CIcsRemoteEes::AddLinkTestProcedure(XUUID UUID, Reply_Link_Test_Args::Args & proc){
	SimpleLockPtr(m_pLinkTestCntrMutex);
	for (auto iter = m_pLinkTestCntr->begin();
		iter != m_pLinkTestCntr->end(); )	{
		auto csttCommand = (*iter).first;
		if (csttCommand == UUID) 		{
			(*iter).second.REPLY = proc;
			return true;
		}
		iter++;
	}
	return false;
}
inline Request_Link_Test_Args::Args CIcsRemoteEes::ParseArg_REQUEST_LINK_TEST(StdStringA pDataz) {
	Request_Link_Test_Args::Args Data;
	XML3::XML m_xml_Parse;
	m_xml_Parse.Clear();
	XML3::XML_PARSE xmlResult = m_xml_Parse.Parse((pDataz).c_str(), pDataz.size());
	if (XML3::XML_PARSE::OK == xmlResult) {
		auto&root = m_xml_Parse.GetRootElement();
		StdStringA Temp = root["header"]["messageName"].GetContent().c_str();
		Data.Hd.Set_messageName(Temp);
		Temp = root["header"]["transactionId"].GetContent().c_str();
		Data.Hd.Set_transactionId_StrToXuuid(Temp);
		Temp = root["header"]["timeStamp"].GetContent().c_str();
		Data.Hd.Set_timeStamp(Temp);

		Temp = root["body"]["EQUIPMENTID"].GetContent().c_str();
		Data.Body.Set_EQUIPMENTID(Temp);
		Temp = root["body"]["IPADDRESS"].GetContent().c_str();
		Data.Body.Set_IPADDRESS(Temp);
		Temp = root["body"]["APPLICATIONID"].GetContent().c_str();
		Data.Body.Set_APPLICATIONID(Temp);
	}
	return Data;
}
inline StdStringA CIcsRemoteEes::Make_REPLY_LINK_TEST(Reply_Link_Test_Args::Args & pDataz) {
	StdStringW szTemp;
	CMarkup		m_xml;
	m_xml.RemoveElem();
	m_xml.ResetPos();
	m_xml.FindElem();
	m_xml.IntoElem();
	{
		m_xml.AddElem(_T("message"));
		m_xml.IntoElem(); {
			m_xml.AddElem(_T("header"));
			{				
				m_xml.IntoElem();
				szTemp = ToWideChar(pDataz.Hd.Get_messageName());
				m_xml.AddElem(_T("messageName"), szTemp);
				szTemp = ToWideChar(pDataz.Hd.Get_transactionId_XuuidToStr());
				m_xml.AddElem(_T("transactionId"), szTemp);
				szTemp = ToWideChar(pDataz.Hd.Get_timeStamp());
				m_xml.AddElem(_T("timeStamp"), szTemp);
			}m_xml.OutOfElem();
			m_xml.AddElem(_T("body"));
			{
				m_xml.IntoElem();
				szTemp = ToWideChar(pDataz.Body.Get_EQUIPMENTID());
				m_xml.AddElem(_T("EQUIPMENTID"), szTemp);
				szTemp = ToWideChar(pDataz.Body.Get_IPADDRESS());
				m_xml.AddElem(_T("IPADDRESS"), szTemp);
				szTemp = ToWideChar(pDataz.Body.Get_APPLICATIONID());
				m_xml.AddElem(_T("APPLICATIONID"), szTemp);
			}m_xml.OutOfElem();
			m_xml.AddElem(_T("return"));
			{
				m_xml.IntoElem();
				szTemp = ToWideChar(pDataz.Rtrn.Get_returnCode());
				m_xml.AddElem(_T("returnCode"), szTemp);
				szTemp = ToWideChar(pDataz.Rtrn.Get_returnMessage());
				m_xml.AddElem(_T("returnMessage"), szTemp);
			}m_xml.OutOfElem();
		}m_xml.OutOfElem();
	}
	m_xml.OutOfElem();
	return ToMultiByte(m_xml.GetSubDoc());
}
bool CIcsRemoteEes::bGetLinkTestProcedure(XUUID UUID) const{
	SimpleLockPtr(m_pLinkTestCntrMutex);
	for (auto iter = m_pLinkTestCntr->begin();
		iter != m_pLinkTestCntr->end(); )	{
		auto csttCommand = (*iter).first;
		if (csttCommand == UUID)		{
			return true;
		}
		iter++;
	}
	return false;
}
void CIcsRemoteEes::RemoveLinkTestProcedure(XUUID UUID){
	SimpleLockPtr(m_pLinkTestCntrMutex);
	auto iter = m_pLinkTestCntr->find(UUID);
	if (iter == m_pLinkTestCntr->end()) {
		return;
	}
	m_pLinkTestCntr->erase(iter);
}

XUUID CIcsRemoteEes::OnlineStateCtrl(){
	SimpleLockPtr(m_pOnlineStateCntrMutex);
	for (auto iter = m_pOnlineStateCntr->begin();
		iter != m_pOnlineStateCntr->end(); )	{
		if (iter != m_pOnlineStateCntr->end()) {
			return (*iter).first;
		}
		iter++;
	}
	return XUUID();
}
Online_State_Args::Args CIcsRemoteEes::OnlineStateCtrl(XUUID UUID){
	SimpleLockPtr(m_pOnlineStateCntrMutex);
	for (auto iter = m_pOnlineStateCntr->begin();
		iter != m_pOnlineStateCntr->end(); )	{
		auto csttCommand = (*iter).first;
		if (csttCommand == UUID) 		{
			return (*iter).second;
		}
		iter++;
	}
	return Online_State_Args::Args();
}
bool CIcsRemoteEes::CreateOnlineStateProcedure(XUUID UUID){
	SimpleLockPtr(m_pOnlineStateCntrMutex);
	for (auto iter = m_pOnlineStateCntr->begin();
		iter != m_pOnlineStateCntr->end(); )	{
		auto csttCommand = (*iter).first;
		if (csttCommand == UUID)		{
			return false;
		}
		iter++;
	}
	auto result = m_pOnlineStateCntr->emplace(UUID, Online_State_Args::Args());
	return result.second;
}
bool CIcsRemoteEes::CreateOnlineStateProcedure(){
	SimpleLockPtr(m_pOnlineStateCntrMutex);
	Online_State_Args::Args Data = {};
	auto transactionId = Data.REPORT.Hd.CreateUUID();
	auto result = m_pOnlineStateCntr->emplace(transactionId, Data);
	return result.second;
}
bool CIcsRemoteEes::AddOnlineStateProcedure(XUUID UUID, Report_Online_State_Args::Args & proc){
	SimpleLockPtr(m_pOnlineStateCntrMutex);
	for (auto iter = m_pOnlineStateCntr->begin();
		iter != m_pOnlineStateCntr->end(); )	{
		auto csttCommand = (*iter).first;
		if (csttCommand == UUID) 		{
			(*iter).second.REPORT = proc;
			return true;
		}
		iter++;
	}
	return false;
}

inline StdStringA CIcsRemoteEes::Make_REPORT_ONLINE_STATE(Report_Online_State_Args::Args & pData) {
	StdStringW szTemp;
	CMarkup			m_xml;
	m_xml.RemoveElem();
	m_xml.ResetPos();
	m_xml.FindElem();
	m_xml.IntoElem();
	{
		m_xml.AddElem(_T("message"));
		m_xml.IntoElem(); {
			m_xml.AddElem(_T("header"));
			{
				m_xml.IntoElem();
				szTemp = ToWideChar(pData.Hd.Get_messageName());
				m_xml.AddElem(_T("messageName"), szTemp);
				szTemp = ToWideChar(pData.Hd.Get_transactionId_XuuidToStr());
				m_xml.AddElem(_T("transactionId"), szTemp);
				szTemp = ToWideChar(pData.Hd.Get_timeStamp());
				m_xml.AddElem(_T("timeStamp"), szTemp);
			}m_xml.OutOfElem();
			m_xml.AddElem(_T("body"));
			{
				m_xml.IntoElem();
				szTemp = ToWideChar(pData.Body.Get_EQUIPMENTID());
				m_xml.AddElem(_T("EQUIPMENTID"), szTemp);
				szTemp = ToWideChar(pData.Body.Get_SUBEQPID());
				m_xml.AddElem(_T("SUBEQPID"), szTemp);
				szTemp = ToWideChar(pData.Body.Get_IPADDRESS());
				m_xml.AddElem(_T("IPADDRESS"), szTemp);
				szTemp = ToWideChar(pData.Body.Get_USERID());
				m_xml.AddElem(_T("USERID"), szTemp);
				szTemp = ToWideChar(pData.Body.Get_EESMODE());
				m_xml.AddElem(_T("EESMODE"), szTemp);
				szTemp = ToWideChar(pData.Body.Get_LOTID());
				m_xml.AddElem(_T("LOTID"), szTemp);
				szTemp = ToWideChar(pData.Body.Get_RECIPEID());
				m_xml.AddElem(_T("RECIPEID"), szTemp);
				szTemp = ToWideChar(pData.Body.Get_PRODUCTID());
				m_xml.AddElem(_T("PRODUCTID"), szTemp);
				szTemp = ToWideChar(pData.Body.Get_PRGVER());
				m_xml.AddElem(_T("PRGVER"), szTemp);
				szTemp = ToWideChar(pData.Body.Get_ONLINESTATE());
				m_xml.AddElem(_T("ONLINESTATE"), szTemp);
				szTemp = ToWideChar(pData.Body.Get_EQUIPMENTSTATE());
				m_xml.AddElem(_T("EQUIPMENTSTATE"), szTemp);
			}m_xml.OutOfElem();
		}m_xml.OutOfElem();
	}
	m_xml.OutOfElem();
	return ToMultiByte(m_xml.GetSubDoc());
}
bool CIcsRemoteEes::bGetOnlineStateProcedure(XUUID UUID) const{
	SimpleLockPtr(m_pOnlineStateCntrMutex);
	for (auto iter = m_pOnlineStateCntr->begin();
		iter != m_pOnlineStateCntr->end(); )	{
		auto csttCommand = (*iter).first;
		if (csttCommand == UUID)	{
			return true;
		}
		iter++;
	}
	return false;
}
void CIcsRemoteEes::RemoveOnlineStateProcedure(XUUID UUID){
	SimpleLockPtr(m_pOnlineStateCntrMutex);
	auto iter = m_pOnlineStateCntr->find(UUID);
	if (iter == m_pOnlineStateCntr->end()) {
		return;
	}
	m_pOnlineStateCntr->erase(iter);
}


XUUID CIcsRemoteEes::UserCommandCtrl(){
	SimpleLockPtr(m_pUserCommandCntrMutex);
	for (auto iter = m_pUserCommandCntr->begin();
		iter != m_pUserCommandCntr->end(); )	{
		if (iter != m_pUserCommandCntr->end()) {
			return (*iter).first;
		}
		iter++;
	}
	return XUUID();
}
User_Change_Args::Args CIcsRemoteEes::UserCommandCtrl(XUUID UUID){
	SimpleLockPtr(m_pUserCommandCntrMutex);
	for (auto iter = m_pUserCommandCntr->begin();
		iter != m_pUserCommandCntr->end(); )	{
		auto csttCommand = (*iter).first;
		if (csttCommand == UUID)		{
			return (*iter).second;
		}
		iter++;
	}
	return User_Change_Args::Args();
}
bool CIcsRemoteEes::CreateUserCommandProcedure(XUUID UUID){
	SimpleLockPtr(m_pUserCommandCntrMutex);
	for (auto iter = m_pUserCommandCntr->begin();
		iter != m_pUserCommandCntr->end(); )	{
		auto csttCommand = (*iter).first;
		if (csttCommand == UUID)		{
			return false;
		}
		iter++;
	}
	auto result = m_pUserCommandCntr->emplace(UUID, User_Change_Args::Args());
	return result.second;
}
bool CIcsRemoteEes::CreateUserCommandProcedure(){
	SimpleLockPtr(m_pUserCommandCntrMutex);
	User_Change_Args::Args Data;
	auto transactionId = Data.REPORT.Hd.CreateUUID();
	auto result = m_pUserCommandCntr->emplace(transactionId, Data);
	return result.second;
}
bool CIcsRemoteEes::AddeUserCommandProcedure(XUUID UUID, Report_User_Change_Args::Args & proc){
	SimpleLockPtr(m_pUserCommandCntrMutex);
	for (auto iter = m_pUserCommandCntr->begin();
		iter != m_pUserCommandCntr->end(); )	{
		auto csttCommand = (*iter).first;
		if (csttCommand == UUID)		{
			(*iter).second.REPORT = proc;
			return true;
		}
		iter++;
	}
	return false;
}
bool CIcsRemoteEes::AddeUserCommandProcedure(XUUID UUID, Request_User_Cpmmand_Args::Args & proc){
	SimpleLockPtr(m_pUserCommandCntrMutex);
	for (auto iter = m_pUserCommandCntr->begin();
		iter != m_pUserCommandCntr->end(); )	{
		auto csttCommand = (*iter).first;
		if (csttCommand == UUID)		{
			(*iter).second.REQUEST = proc;
			return true;
		}
		iter++;
	}
	return false;
}
bool CIcsRemoteEes::AddeUserCommandProcedure(XUUID UUID, Reply_User_Change_Args::Args & proc){
	SimpleLockPtr(m_pUserCommandCntrMutex);
	for (auto iter = m_pUserCommandCntr->begin();
		iter != m_pUserCommandCntr->end(); )	{
		auto csttCommand = (*iter).first;
		if (csttCommand == UUID)		{
			(*iter).second.REPLY = proc;
			return true;
		}
		iter++;
	}
	return false;
}
inline StdStringA CIcsRemoteEes::Make_REPORT_USER_CHANGE(Report_User_Change_Args::Args & pData) {
	StdStringW szTemp;
	CMarkup	m_xml;
	m_xml.RemoveElem();
	m_xml.ResetPos();
	m_xml.FindElem();
	m_xml.IntoElem();
	{
		m_xml.AddElem(_T("message"));
		m_xml.IntoElem(); {
			m_xml.AddElem(_T("header"));
			{
				m_xml.IntoElem();
				szTemp = ToWideChar(pData.Hd.Get_messageName());
				m_xml.AddElem(_T("messageName"), szTemp);
				szTemp = ToWideChar(pData.Hd.Get_transactionId_XuuidToStr());
				m_xml.AddElem(_T("transactionId"), szTemp);
				szTemp = ToWideChar(pData.Hd.Get_messageName());
				m_xml.AddElem(_T("timeStamp"), szTemp);
			}m_xml.OutOfElem();
			m_xml.AddElem(_T("body"));
			{
				m_xml.IntoElem();
				szTemp = ToWideChar(pData.Body.Get_EQUIPMENTID());
				m_xml.AddElem(_T("EQUIPMENTID"), szTemp);
				szTemp = ToWideChar(pData.Body.Get_IPADDRESS());
				m_xml.AddElem(_T("IPADDRESS"), szTemp);
				szTemp = ToWideChar(pData.Body.Get_USERID());
				m_xml.AddElem(_T("USERID"), szTemp);
				szTemp = ToWideChar(pData.Body.Get_EESMODE());
				m_xml.AddElem(_T("EESMODE"), szTemp);
			}m_xml.OutOfElem();
		}m_xml.OutOfElem();
	}
	m_xml.OutOfElem();
	return ToMultiByte(m_xml.GetSubDoc());
}
inline Request_User_Cpmmand_Args::Args CIcsRemoteEes::ParseArg_REQUEST_USER_COMMAND(StdStringA pDataz) {
	Request_User_Cpmmand_Args::Args Data;
	XML3::XML	m_xml_Parse;
	m_xml_Parse.Clear();
	XML3::XML_PARSE xmlResult = m_xml_Parse.Parse((pDataz).c_str(), pDataz.size());
	if (XML3::XML_PARSE::OK == xmlResult) {
		auto&root = m_xml_Parse.GetRootElement();
		StdStringA Temp = root["header"]["messageName"].GetContent().c_str();
		Data.Hd.Set_messageName(Temp);
		Temp = root["header"]["transactionId"].GetContent().c_str();
		Data.Hd.Set_transactionId_StrToXuuid(Temp);
		Temp = root["header"]["timeStamp"].GetContent().c_str();
		Data.Hd.Set_timeStamp(Temp);

		Temp = root["body"]["EQUIPMENTID"].GetContent().c_str();
		Data.Body.Set_EQUIPMENTID(Temp);
		Temp = root["body"]["IPADDRESS"].GetContent().c_str();
		Data.Body.Set_IPADDRESS(Temp);
		Temp = root["body"]["USERID"].GetContent().c_str();
		Data.Body.Set_USERID(Temp);

		Temp = root["return"]["returnCode"].GetContent().c_str();
		Data.Rtrn.Set_returnCode(Temp);
		Temp = root["return"]["returnMessage"].GetContent().c_str();
		Data.Rtrn.Set_returnMessage(Temp);
	}
	return Data;
}
inline StdStringA CIcsRemoteEes::Make_REPLY_USER_COMMAND(Reply_User_Change_Args::Args & pData) {
	StdStringW szTemp;
	CMarkup		m_xml;
	m_xml.RemoveElem();
	m_xml.ResetPos();
	m_xml.FindElem();
	m_xml.IntoElem();
	{
		m_xml.AddElem(_T("message"));
		m_xml.IntoElem(); {
			m_xml.AddElem(_T("header"));
			{
				m_xml.IntoElem();
				szTemp = ToWideChar(pData.Hd.Get_messageName());
				m_xml.AddElem(_T("messageName"), szTemp);
				szTemp = ToWideChar(pData.Hd.Get_transactionId_XuuidToStr());
				m_xml.AddElem(_T("transactionId"), szTemp);
				szTemp = ToWideChar(pData.Hd.Get_messageName());
				m_xml.AddElem(_T("timeStamp"), szTemp);
			}m_xml.OutOfElem();
			m_xml.AddElem(_T("body"));
			{
				m_xml.IntoElem();
				szTemp = ToWideChar(pData.Body.Get_EQUIPMENTID());
				m_xml.AddElem(_T("EQUIPMENTID"), szTemp);
				szTemp = ToWideChar(pData.Body.Get_IPADDRESS());
				m_xml.AddElem(_T("IPADDRESS"), szTemp);
				szTemp = ToWideChar(pData.Body.Get_USERID());
				m_xml.AddElem(_T("USERID"), szTemp);
			}m_xml.OutOfElem();
			m_xml.AddElem(_T("return"));
			{
				m_xml.IntoElem();
				szTemp = ToWideChar(pData.Rtrn.Get_returnCode());
				m_xml.AddElem(_T("returnCode"), szTemp);
				szTemp = ToWideChar(pData.Rtrn.Get_returnMessage());
				m_xml.AddElem(_T("returnMessage"), szTemp);
			}m_xml.OutOfElem();
		}m_xml.OutOfElem();
	}
	m_xml.OutOfElem();
	return ToMultiByte(m_xml.GetSubDoc());
}
bool CIcsRemoteEes::bGetUserCommandProcedure(XUUID UUID) const{
	SimpleLockPtr(m_pUserCommandCntrMutex);
	for (auto iter = m_pUserCommandCntr->begin();
		iter != m_pUserCommandCntr->end(); )	{
		auto csttCommand = (*iter).first;
		if (csttCommand == UUID)	{
			return true;
		}
		iter++;
	}
	return false;
}
void CIcsRemoteEes::RemoveUserCommandProcedure(XUUID UUID){
	SimpleLockPtr(m_pUserCommandCntrMutex);
	auto iter = m_pUserCommandCntr->find(UUID);
	if (iter == m_pUserCommandCntr->end()) {
		return;
	}	
	m_pUserCommandCntr->erase(iter);
}

XUUID CIcsRemoteEes::EquipmentStateCtrl(){
	SimpleLockPtr(m_pEquipmentStateCntrMutex);
	for (auto iter = m_pEquipmentStateCntr->begin();
		iter != m_pEquipmentStateCntr->end(); )	{
		if (iter != m_pEquipmentStateCntr->end()) {
			return (*iter).first;
		}
		iter++;
	}
	return XUUID();
}
Equipment_State_Args::Args CIcsRemoteEes::EquipmentStateCtrl(XUUID UUID)
{
	SimpleLockPtr(m_pEquipmentStateCntrMutex);
	for (auto iter = m_pEquipmentStateCntr->begin();
		iter != m_pEquipmentStateCntr->end(); )	{
		auto csttCommand = (*iter).first;
		if (csttCommand == UUID)		{
			return (*iter).second;
		}
		iter++;
	}
	return Equipment_State_Args::Args();
}
bool CIcsRemoteEes::CreateEquipmentStateProcedure(XUUID UUID){
	SimpleLockPtr(m_pEquipmentStateCntrMutex);
	for (auto iter = m_pEquipmentStateCntr->begin();
		iter != m_pEquipmentStateCntr->end(); )	{
		auto csttCommand = (*iter).first;
		if (csttCommand == UUID)	{
			return false;
		}
		iter++;
	}
	auto result = m_pEquipmentStateCntr->emplace(UUID, Equipment_State_Args::Args());
	return result.second;
}
bool CIcsRemoteEes::CreateEquipmentStateProcedure(){
	SimpleLockPtr(m_pEquipmentStateCntrMutex);
	Equipment_State_Args::Args Data;
	auto transactionId = Data.REPORT.Hd.CreateUUID();
	auto result = m_pEquipmentStateCntr->emplace(transactionId, Data);
	return result.second;
}
bool CIcsRemoteEes::AddEquipmentStateProcedure(XUUID UUID, Report_Equipment_State_Args::Args & proc){
	SimpleLockPtr(m_pEquipmentStateCntrMutex);
	for (auto iter = m_pEquipmentStateCntr->begin();
		iter != m_pEquipmentStateCntr->end(); )	{
		auto csttCommand = (*iter).first;
		if (csttCommand == UUID)
		{
			(*iter).second.REPORT = proc;
			return true;
		}
		iter++;
	}
	return false;
}
bool CIcsRemoteEes::AddEquipmentStateProcedure(XUUID UUID, Request_Equipment_State_Display_Args::Args & proc){
	SimpleLockPtr(m_pEquipmentStateCntrMutex);
	for (auto iter = m_pEquipmentStateCntr->begin();
		iter != m_pEquipmentStateCntr->end(); )	{
		auto csttCommand = (*iter).first;
		if (csttCommand == UUID)		{
			(*iter).second.REQUEST = proc;
			return true;
		}
		iter++;
	}
	return false;
}
bool CIcsRemoteEes::AddEquipmentStateProcedure(XUUID UUID, Reply_Equipment_State_Display_Args::Args & proc){
	SimpleLockPtr(m_pEquipmentStateCntrMutex);
	for (auto iter = m_pEquipmentStateCntr->begin();
		iter != m_pEquipmentStateCntr->end(); )	{
		auto csttCommand = (*iter).first;
		if (csttCommand == UUID)		{
			(*iter).second.REPLY = proc;
			return true;
		}
		iter++;
	}
	return false;
}
inline StdStringA CIcsRemoteEes::Make_REPORT_EQUIPMENT_STATE(Report_Equipment_State_Args::Args & pData) {
	StdStringW szTemp;
	CMarkup		m_xml;
	m_xml.RemoveElem();
	m_xml.ResetPos();
	m_xml.FindElem();
	m_xml.IntoElem();
	{
		m_xml.AddElem(_T("message"));
		m_xml.IntoElem(); {
			m_xml.AddElem(_T("header"));
			{
				m_xml.IntoElem();
				szTemp = ToWideChar(pData.Hd.Get_messageName());
				m_xml.AddElem(_T("messageName"), szTemp);
				szTemp = ToWideChar(pData.Hd.Get_transactionId_XuuidToStr());
				m_xml.AddElem(_T("transactionId"), szTemp);
				szTemp = ToWideChar(pData.Hd.Get_messageName());
				m_xml.AddElem(_T("timeStamp"), szTemp);
			}m_xml.OutOfElem();
			m_xml.AddElem(_T("body"));
			{
				m_xml.IntoElem();
				szTemp = ToWideChar(pData.Body.Get_EQUIPMENTID());
				m_xml.AddElem(_T("EQUIPMENTID"), szTemp);
				szTemp = ToWideChar(pData.Body.Get_SUBEQPID());
				m_xml.AddElem(_T("SUBEQPID"), szTemp);
				szTemp = ToWideChar(pData.Body.Get_IPADDRESS());
				m_xml.AddElem(_T("IPADDRESS"), szTemp);
				szTemp = ToWideChar(pData.Body.Get_USERID());
				m_xml.AddElem(_T("USERID"), szTemp);
				szTemp = ToWideChar(pData.Body.Get_EESMODE());
				m_xml.AddElem(_T("EESMODE"), szTemp);
				szTemp = ToWideChar(pData.Body.Get_LOTID());
				m_xml.AddElem(_T("LOTID"), szTemp);
				szTemp = ToWideChar(pData.Body.Get_RECIPEID());
				m_xml.AddElem(_T("RECIPEID"), szTemp);
				szTemp = ToWideChar(pData.Body.Get_PRGVER());
				m_xml.AddElem(_T("PRODUCTID"), szTemp);
				szTemp = ToWideChar(pData.Body.Get_PRGVER());
				m_xml.AddElem(_T("PRGVER"), szTemp);
				szTemp = ToWideChar(pData.Body.Get_EQUIPMENTSTATE());
				m_xml.AddElem(_T("EQUIPMENTSTATE"), szTemp);

			}m_xml.OutOfElem();
		}m_xml.OutOfElem();
	}
	m_xml.OutOfElem();
	return ToMultiByte(m_xml.GetSubDoc());
}
inline Request_Equipment_State_Display_Args::Args CIcsRemoteEes::ParseArg_REQUEST_EQUIPMENT_STATE_DISPLAY(StdStringA pDataz) {
	Request_Equipment_State_Display_Args::Args Data;
	XML3::XML	m_xml_Parse;
	m_xml_Parse.Clear();
	XML3::XML_PARSE xmlResult = m_xml_Parse.Parse((pDataz).c_str(), pDataz.size());
	if (XML3::XML_PARSE::OK == xmlResult) {
		auto&root = m_xml_Parse.GetRootElement();
		StdStringA Temp = root["header"]["messageName"].GetContent().c_str();
		Data.Hd.Set_messageName(Temp);
		Temp = root["header"]["transactionId"].GetContent().c_str();
		Data.Hd.Set_transactionId_StrToXuuid(Temp);
		Temp = root["header"]["timeStamp"].GetContent().c_str();
		Data.Hd.Set_timeStamp(Temp);

		Temp = root["body"]["EQUIPMENTID"].GetContent().c_str();
		Data.Body.Set_EQUIPMENTID(Temp);
		Temp = root["body"]["SUBEQPID"].GetContent().c_str();
		Data.Body.Set_SUBEQPID(Temp);
		Temp = root["body"]["EQUIPMENTSTATEDISPLAY"].GetContent().c_str();
		Data.Body.Set_EQUIPMENTSTATEDISPLAY(Temp);
		Temp = root["body"]["RGBDISPLAY"].GetContent().c_str();
		Data.Body.Set_EQUIPMENTSTATEDISPLAY(Temp);

		//Temp = root["return"]["returnCode"].GetContent().c_str();
		//Data.Rtrn.Set_returnCode(Temp);
		//Temp = root["return"]["returnMessage"].GetContent().c_str();
		//Data.Rtrn.Set_returnMessage(Temp);
	}
	return Data;
}
inline StdStringA CIcsRemoteEes::Make_REPLY_EQUIPMENT_STATE_DISPLAY(Reply_Equipment_State_Display_Args::Args & pData) {
	StdStringW szTemp;
	CMarkup		m_xml;
	m_xml.RemoveElem();
	m_xml.ResetPos();
	m_xml.FindElem();
	m_xml.IntoElem();
	{
		m_xml.AddElem(_T("message"));
		m_xml.IntoElem(); {
			m_xml.AddElem(_T("header"));
			{
				m_xml.IntoElem();
				szTemp = ToWideChar(pData.Hd.Get_messageName());
				m_xml.AddElem(_T("messageName"), szTemp);
				szTemp = ToWideChar(pData.Hd.Get_transactionId_XuuidToStr());
				m_xml.AddElem(_T("transactionId"), szTemp);
				szTemp = ToWideChar(pData.Hd.Get_messageName());
				m_xml.AddElem(_T("timeStamp"), szTemp);
			}m_xml.OutOfElem();
			m_xml.AddElem(_T("body"));
			{
				m_xml.IntoElem();
				szTemp = ToWideChar(pData.Body.Get_EQUIPMENTID());
				m_xml.AddElem(_T("EQUIPMENTID"), szTemp);
				szTemp = ToWideChar(pData.Body.Get_SUBEQPID());
				m_xml.AddElem(_T("SUBEQPID"), szTemp);
				szTemp = ToWideChar(pData.Body.Get_EQUIPMENTSTATEDISPLAY());
				m_xml.AddElem(_T("EQUIPMENTSTATEDISPLAY"), szTemp);
				szTemp = ToWideChar(pData.Body.Get_RGBDISPLAY());
				m_xml.AddElem(_T("RGBDISPLAY"), szTemp);
			}m_xml.OutOfElem();

			m_xml.AddElem(_T("return"));
			{
				m_xml.IntoElem();
				szTemp = ToWideChar(pData.Rtrn.Get_returnCode());
				m_xml.AddElem(_T("returnCode"), szTemp);
				szTemp = ToWideChar(pData.Rtrn.Get_returnMessage());
				m_xml.AddElem(_T("returnMessage"), szTemp);
			}m_xml.OutOfElem();
		}m_xml.OutOfElem();
	}
	m_xml.OutOfElem();
	return ToMultiByte(m_xml.GetSubDoc());
}

bool CIcsRemoteEes::bGetEquipmentStateProcedure(XUUID UUID) const{
	SimpleLockPtr(m_pEquipmentStateCntrMutex);
	for (auto iter = m_pEquipmentStateCntr->begin();
		iter != m_pEquipmentStateCntr->end(); )	{
		auto csttCommand = (*iter).first;
		if (csttCommand == UUID)		{
			return true;
		}
		iter++;
	}
	return false;
}
void CIcsRemoteEes::RemoveEquipmentStateProcedure(XUUID UUID){
	SimpleLockPtr(m_pEquipmentStateCntrMutex);
	auto iter = m_pEquipmentStateCntr->find(UUID);
	if (iter == m_pEquipmentStateCntr->end()){
		return;}
	m_pEquipmentStateCntr->erase(iter);
}


XUUID CIcsRemoteEes::LossWindowCtrl(){
	SimpleLockPtr(m_pLossWindowCntrMutex);
	for (auto iter = m_pLossWindowCntr->begin();
		iter != m_pLossWindowCntr->end(); )	{
		if (iter != m_pLossWindowCntr->end()) {
			return (*iter).first;		}
		iter++;
	}
	return XUUID();
}
Loss_Window_Args::Args CIcsRemoteEes::LossWindowCtrl(XUUID UUID){
	SimpleLockPtr(m_pLossWindowCntrMutex);
	for (auto iter = m_pLossWindowCntr->begin();
		iter != m_pLossWindowCntr->end(); )	{
		auto csttCommand = (*iter).first;
		if (csttCommand == UUID)	{
			return (*iter).second;
		}
		iter++;
	}
	return Loss_Window_Args::Args();
}
bool CIcsRemoteEes::CreateLossWindowProcedure(XUUID UUID){
	SimpleLockPtr(m_pLossWindowCntrMutex);
	for (auto iter = m_pLossWindowCntr->begin();
		iter != m_pLossWindowCntr->end(); )	{
		auto csttCommand = (*iter).first;
		if (csttCommand == UUID)	{
			return false;
		}
		iter++;
	}
	auto result = m_pLossWindowCntr->emplace(UUID, Loss_Window_Args::Args());
	return result.second;
}
bool CIcsRemoteEes::CreateLossWindowProcedure(){
	SimpleLockPtr(m_pLossWindowCntrMutex);
	Loss_Window_Args::Args Data ;
	auto transactionId = Data.REPORT.Hd.CreateUUID();
	auto result = m_pLossWindowCntr->emplace(transactionId, Data);
	return result.second;
}
bool CIcsRemoteEes::AddLossWindowProcedure(XUUID UUID, Report_Loss_State_Args::Args & proc){
	SimpleLockPtr(m_pLossWindowCntrMutex);
	for (auto iter = m_pLossWindowCntr->begin();
		iter != m_pLossWindowCntr->end(); )	{
		auto csttCommand = (*iter).first;
		if (csttCommand == UUID)		{
			(*iter).second.REPORT = proc;
			return true;
		}
		iter++;
	}
	return false;
}
bool CIcsRemoteEes::AddLossWindowProcedure(XUUID UUID, Request_Loss_Window_Args::Args & proc){
	SimpleLockPtr(m_pLossWindowCntrMutex);
	for (auto iter = m_pLossWindowCntr->begin();
		iter != m_pLossWindowCntr->end(); )	{
		auto csttCommand = (*iter).first;
		if (csttCommand == UUID)	{
			(*iter).second.REQUEST = proc;
			return true;
		}
		iter++;
	}
	return false;
}
bool CIcsRemoteEes::AddLossWindowProcedure(XUUID UUID, Reply_Loss_Window_Args::Args & proc){
	SimpleLockPtr(m_pLossWindowCntrMutex);
	for (auto iter = m_pLossWindowCntr->begin();
		iter != m_pLossWindowCntr->end(); )	{
		auto csttCommand = (*iter).first;
		if (csttCommand == UUID)	{
			(*iter).second.REPLY = proc;
			return true;
		}
		iter++;
	}
	return false;
}
inline StdStringA CIcsRemoteEes::Make_REPORT_LOSS_STATE(Report_Loss_State_Args::Args & pData) {
	StdStringW szTemp;
	CMarkup	m_xml;
	m_xml.RemoveElem();
	m_xml.ResetPos();
	m_xml.FindElem();
	m_xml.IntoElem();
	{
		m_xml.AddElem(_T("message"));
		m_xml.IntoElem(); {
			m_xml.AddElem(_T("header"));
			{
				m_xml.IntoElem();
				szTemp = ToWideChar(pData.Hd.Get_messageName());
				m_xml.AddElem(_T("messageName"), szTemp);
				szTemp = ToWideChar(pData.Hd.Get_transactionId_XuuidToStr());
				m_xml.AddElem(_T("transactionId"), szTemp);
				szTemp = ToWideChar(pData.Hd.Get_messageName());
				m_xml.AddElem(_T("timeStamp"), szTemp);
			}m_xml.OutOfElem();
			m_xml.AddElem(_T("body"));
			{
				m_xml.IntoElem();
				szTemp = ToWideChar(pData.Body.Get_EQUIPMENTID());
				m_xml.AddElem(_T("EQUIPMENTID"), szTemp);
				szTemp = ToWideChar(pData.Body.Get_SUBEQPID());
				m_xml.AddElem(_T("SUBEQPID"), szTemp);
				szTemp = ToWideChar(pData.Body.Get_IPADDRESS());
				m_xml.AddElem(_T("IPADDRESS"), szTemp);
				szTemp = ToWideChar(pData.Body.Get_USERID());
				m_xml.AddElem(_T("USERID"), szTemp);
				szTemp = ToWideChar(pData.Body.Get_EESMODE());
				m_xml.AddElem(_T("EESMODE"), szTemp);
				szTemp = ToWideChar(pData.Body.Get_LOTID());
				m_xml.AddElem(_T("LOTID"), szTemp);
				szTemp = ToWideChar(pData.Body.Get_RECIPEID());
				m_xml.AddElem(_T("RECIPEID"), szTemp);
				szTemp = ToWideChar(pData.Body.Get_PRODUCTID());
				m_xml.AddElem(_T("PRODUCTID"), szTemp);
				szTemp = ToWideChar(pData.Body.Get_PRGVER());
				m_xml.AddElem(_T("PRGVER"), szTemp);
				szTemp = ToWideChar(pData.Body.Get_EQUIPMENTSTATE());
				m_xml.AddElem(_T("EQUIPMENTSTATE"), szTemp);
				szTemp = ToWideChar(pData.Body.Get_LOSSSTATE());
				m_xml.AddElem(_T("LOSSSTATE"), szTemp);
				szTemp = ToWideChar(pData.Body.Get_LOSSCOUNT());
				m_xml.AddElem(_T("LOSSCOUNT"), szTemp);
			}m_xml.OutOfElem();
		}m_xml.OutOfElem();
	}
	m_xml.OutOfElem();
	return ToMultiByte(m_xml.GetSubDoc());
}
inline Request_Loss_Window_Args::Args CIcsRemoteEes::ParseArg_REQUEST_LOSS_WINDOW(StdStringA pDataz) {
	Request_Loss_Window_Args::Args Data;
	XML3::XML	m_xml_Parse;
	m_xml_Parse.Clear();
	XML3::XML_PARSE xmlResult = m_xml_Parse.Parse((pDataz).c_str(), pDataz.size());
	if (XML3::XML_PARSE::OK == xmlResult) {
		auto&root = m_xml_Parse.GetRootElement();
		StdStringA Temp = root["header"]["messageName"].GetContent().c_str();
		Data.Hd.Set_messageName(Temp);
		Temp = root["header"]["transactionId"].GetContent().c_str();
		Data.Hd.Set_transactionId_StrToXuuid(Temp);
		Temp = root["header"]["timeStamp"].GetContent().c_str();
		Data.Hd.Set_timeStamp(Temp);

		Temp = root["body"]["EQUIPMENTID"].GetContent().c_str();
		Data.Body.Set_EQUIPMENTID(Temp);
		Temp = root["body"]["SUBEQPID"].GetContent().c_str();
		Data.Body.Set_SUBEQPID(Temp);
		Temp = root["body"]["IPADDRESS"].GetContent().c_str();
		Data.Body.Set_IPADDRESS(Temp);
		Temp = root["body"]["LOSSWINDOW"].GetContent().c_str();
		Data.Body.Set_LOSSWINDOW(Temp);

		//Temp = root["return"]["returnCode"].GetContent().c_str();
		//Data.Rtrn.Set_returnCode(Temp);
		//Temp = root["return"]["returnMessage"].GetContent().c_str();
		//Data.Rtrn.Set_returnMessage(Temp);
	}
	return Data;
}
inline StdStringA CIcsRemoteEes::Make_REPLY_LOSS_WINDOW(Reply_Loss_Window_Args::Args & pData) {
	StdStringW szTemp;
	CMarkup	m_xml;
	m_xml.RemoveElem();
	m_xml.ResetPos();
	m_xml.FindElem();
	m_xml.IntoElem();
	{
		m_xml.AddElem(_T("message"));
		m_xml.IntoElem(); {
			m_xml.AddElem(_T("header"));
			{
				m_xml.IntoElem();
				szTemp = ToWideChar(pData.Hd.Get_messageName());
				m_xml.AddElem(_T("messageName"), szTemp);
				szTemp = ToWideChar(pData.Hd.Get_transactionId_XuuidToStr());
				m_xml.AddElem(_T("transactionId"), szTemp);
				szTemp = ToWideChar(pData.Hd.Get_messageName());
				m_xml.AddElem(_T("timeStamp"), szTemp);
			}m_xml.OutOfElem();
			m_xml.AddElem(_T("body"));
			{
				m_xml.IntoElem();
				szTemp = ToWideChar(pData.Body.Get_EQUIPMENTID());
				m_xml.AddElem(_T("EQUIPMENTID"), szTemp);
				szTemp = ToWideChar(pData.Body.Get_SUBEQPID());
				m_xml.AddElem(_T("SUBEQPID"), szTemp);
				szTemp = ToWideChar(pData.Body.Get_IPADDRESS());
				m_xml.AddElem(_T("IPADDRESS"), szTemp);
				szTemp = ToWideChar(pData.Body.Get_LOSSWINDOW());
				m_xml.AddElem(_T("LOSSWINDOW"), szTemp);
			}m_xml.OutOfElem();

			m_xml.AddElem(_T("return"));
			{
				m_xml.IntoElem();
				szTemp = ToWideChar(pData.Rtrn.Get_returnCode());
				m_xml.AddElem(_T("returnCode"), szTemp);
				szTemp = ToWideChar(pData.Rtrn.Get_returnMessage());
				m_xml.AddElem(_T("returnMessage"), szTemp);
			}m_xml.OutOfElem();
		}m_xml.OutOfElem();
	}
	m_xml.OutOfElem();
	return ToMultiByte(m_xml.GetSubDoc());
}

bool CIcsRemoteEes::bGetLossWindowProcedure(XUUID UUID) const{
	SimpleLockPtr(m_pLossWindowCntrMutex);
	for (auto iter = m_pLossWindowCntr->begin();
		iter != m_pLossWindowCntr->end(); )	{
		auto csttCommand = (*iter).first;
		if (csttCommand == UUID)	{
			return true;
		}
		iter++;
	}
	return false;
}
void CIcsRemoteEes::RemoveLossWindowProcedure(XUUID UUID){
	SimpleLockPtr(m_pLossWindowCntrMutex);
	auto iter = m_pLossWindowCntr->find(UUID);
	if (iter == m_pLossWindowCntr->end()) {
		return;
	}
	m_pLossWindowCntr->erase(iter);
}



XUUID CIcsRemoteEes::AlarmStateCtrl(){
	SimpleLockPtr(m_pAlarmStateCntrMutex);
	for (auto iter = m_pAlarmStateCntr->begin();
		iter != m_pAlarmStateCntr->end(); )	{
		if (iter != m_pAlarmStateCntr->end()) {
			return (*iter).first;
		}
		iter++;
	}
	return XUUID();
}
Alarm_State_Args::Args CIcsRemoteEes::AlarmStateCtrl(XUUID UUID){
	SimpleLockPtr(m_pAlarmStateCntrMutex);
	for (auto iter = m_pAlarmStateCntr->begin();
		iter != m_pAlarmStateCntr->end(); )	{
		auto csttCommand = (*iter).first;
		if (csttCommand == UUID)	{
			return (*iter).second;
		}
		iter++;
	}
	return Alarm_State_Args::Args();
}
bool CIcsRemoteEes::CreateAlarmStateProcedure(XUUID UUID){
	SimpleLockPtr(m_pAlarmStateCntrMutex);
	for (auto iter = m_pAlarmStateCntr->begin();
		iter != m_pAlarmStateCntr->end(); )	{
		auto csttCommand = (*iter).first;
		if (csttCommand == UUID)	{
			return false;
		}
		iter++;
	}
	auto result = m_pAlarmStateCntr->emplace(UUID, Alarm_State_Args::Args());
	return result.second;
}
bool CIcsRemoteEes::CreateAlarmStateProcedure()
{
	SimpleLockPtr(m_pAlarmStateCntrMutex);
	Alarm_State_Args::Args Data;
	auto transactionId = Data.REPORT.Hd.CreateUUID();
	auto result = m_pAlarmStateCntr->emplace(transactionId, Data);
	return result.second;
}
bool CIcsRemoteEes::AddAlarmStateProcedure(XUUID UUID, Report_Alarm_State_Args::Args & proc)
{
	SimpleLockPtr(m_pAlarmStateCntrMutex);
	for (auto iter = m_pAlarmStateCntr->begin();
		iter != m_pAlarmStateCntr->end(); )
	{
		auto csttCommand = (*iter).first;
		if (csttCommand == UUID)
		{
			(*iter).second.REPORT = proc;
			return true;
		}
		iter++;
	}
	return false;
}
inline StdStringA CIcsRemoteEes::Make_REPORT_ALARM_STATE(Report_Alarm_State_Args::Args & pData) {
	StdStringW szTemp;
	CMarkup	m_xml;
	m_xml.RemoveElem();
	m_xml.ResetPos();
	m_xml.FindElem();
	m_xml.IntoElem();
	{
		m_xml.AddElem(_T("message"));
		m_xml.IntoElem(); {
			m_xml.AddElem(_T("header"));
			{
				m_xml.IntoElem();
				szTemp = ToWideChar(pData.Hd.Get_messageName());
				m_xml.AddElem(_T("messageName"), szTemp);
				szTemp = ToWideChar(pData.Hd.Get_transactionId_XuuidToStr());
				m_xml.AddElem(_T("transactionId"), szTemp);
				szTemp = ToWideChar(pData.Hd.Get_messageName());
				m_xml.AddElem(_T("timeStamp"), szTemp);
			}m_xml.OutOfElem();
			m_xml.AddElem(_T("body"));
			{
				m_xml.IntoElem();
				szTemp = ToWideChar(pData.Body.Get_EQUIPMENTID());
				m_xml.AddElem(_T("EQUIPMENTID"), szTemp);
				szTemp = ToWideChar(pData.Body.Get_SUBEQPID());
				m_xml.AddElem(_T("SUBEQPID"), szTemp);
				szTemp = ToWideChar(pData.Body.Get_IPADDRESS());
				m_xml.AddElem(_T("IPADDRESS"), szTemp);
				szTemp = ToWideChar(pData.Body.Get_USERID());
				m_xml.AddElem(_T("USERID"), szTemp);
				szTemp = ToWideChar(pData.Body.Get_EESMODE());
				m_xml.AddElem(_T("EESMODE"), szTemp);
				szTemp = ToWideChar(pData.Body.Get_LOTID());
				m_xml.AddElem(_T("LOTID"), szTemp);
				szTemp = ToWideChar(pData.Body.Get_RECIPEID());
				m_xml.AddElem(_T("RECIPEID"), szTemp);
				szTemp = ToWideChar(pData.Body.Get_PRODUCTID());
				m_xml.AddElem(_T("PRODUCTID"), szTemp);
				szTemp = ToWideChar(pData.Body.Get_PRGVER());
				m_xml.AddElem(_T("PRGVER"), szTemp);
				szTemp = ToWideChar(pData.Body.Get_ALARMCODE());
				m_xml.AddElem(_T("ALARMCODE"), szTemp);
				szTemp = ToWideChar(pData.Body.Get_ALARMID());
				m_xml.AddElem(_T("ALARMID"), szTemp);
				szTemp = ToWideChar(pData.Body.Get_ALARMMESSAGE());
				m_xml.AddElem(_T("ALARMMESSAGE"), szTemp);
			}m_xml.OutOfElem();
		}m_xml.OutOfElem();
	}
	m_xml.OutOfElem();
	return ToMultiByte(m_xml.GetSubDoc());
}
bool CIcsRemoteEes::bGetAlarmStateProcedure(XUUID UUID) const{
	SimpleLockPtr(m_pAlarmStateCntrMutex);
	for (auto iter = m_pAlarmStateCntr->begin();
		iter != m_pAlarmStateCntr->end(); )	{
		auto csttCommand = (*iter).first;
		if (csttCommand == UUID)	{
			return true;
		}
		iter++;
	}
	return false;
}
void CIcsRemoteEes::RemoveAlarmStateProcedure(XUUID UUID){
	SimpleLockPtr(m_pAlarmStateCntrMutex);
	auto iter = m_pAlarmStateCntr->find(UUID);
	if (iter == m_pAlarmStateCntr->end()) {
		return;
	}
	m_pAlarmStateCntr->erase(iter);
}


XUUID CIcsRemoteEes::AlarmListCtrl(){
	SimpleLockPtr(m_pAlarmListCntrMutex);
	for (auto iter = m_pAlarmListCntr->begin();
		iter != m_pAlarmListCntr->end(); )	{
		if (iter != m_pAlarmListCntr->end()) {
			return (*iter).first;
		}
		iter++;
	}
	return XUUID();
}
Alarm_List_Args::Args  CIcsRemoteEes::AlarmListCtrl(XUUID UUID)
{
	SimpleLockPtr(m_pAlarmListCntrMutex);
	for (auto iter = m_pAlarmListCntr->begin();
		iter != m_pAlarmListCntr->end(); )	{
		auto csttCommand = (*iter).first;
		if (csttCommand == UUID)		{
			return (*iter).second;
		}
		iter++;
	}
	return Alarm_List_Args::Args();
}
bool CIcsRemoteEes::CreateAlarmListProcedure(XUUID UUID){
	SimpleLockPtr(m_pAlarmListCntrMutex);
	for (auto iter = m_pAlarmListCntr->begin();
		iter != m_pAlarmListCntr->end(); )	{
		auto csttCommand = (*iter).first;
		if (csttCommand == UUID)	{
			return false;
		}
		iter++;
	}
	auto result = m_pAlarmListCntr->emplace(UUID, Alarm_List_Args::Args());
	return result.second;
}
bool CIcsRemoteEes::CreateAlarmListProcedure()
{
	SimpleLockPtr(m_pAlarmListCntrMutex);
	Alarm_List_Args::Args Data;
	auto transactionId = Data.REQUEST.Hd.CreateUUID();
	auto result = m_pAlarmListCntr->emplace(transactionId, Data);
	return result.second;
}
bool CIcsRemoteEes::AddAlarmListProcedure(XUUID UUID, Request_Alarm_List_Args::Args & proc){
	SimpleLockPtr(m_pAlarmListCntrMutex);
	for (auto iter = m_pAlarmListCntr->begin();
		iter != m_pAlarmListCntr->end(); )	{
		auto csttCommand = (*iter).first;
		if (csttCommand == UUID)	{
			(*iter).second.REQUEST = proc;
			return true;
		}
		iter++;
	}
	return false;
}
bool CIcsRemoteEes::AddAlarmListProcedure(XUUID UUID, Reply_Alarm_List_Args::Args & proc)
{
	SimpleLockPtr(m_pAlarmListCntrMutex);
	for (auto iter = m_pAlarmListCntr->begin();
		iter != m_pAlarmListCntr->end(); )	{
		auto csttCommand = (*iter).first;
		if (csttCommand == UUID)	{
			(*iter).second.REPLY = proc;
			return true;
		}
		iter++;
	}
	return false;
}
inline Request_Alarm_List_Args::Args CIcsRemoteEes::ParseArg_REQUEST_ALARM_LIST(StdStringA pDataz) {
	Request_Alarm_List_Args::Args Data;
	XML3::XML	m_xml_Parse;
	m_xml_Parse.Clear();
	XML3::XML_PARSE xmlResult = m_xml_Parse.Parse((pDataz).c_str(), pDataz.size());
	if (XML3::XML_PARSE::OK == xmlResult) {
		auto&root = m_xml_Parse.GetRootElement();
		StdStringA Temp = root["header"]["messageName"].GetContent().c_str();
		Data.Hd.Set_messageName(Temp);
		Temp = root["header"]["transactionId"].GetContent().c_str();
		Data.Hd.Set_transactionId_StrToXuuid(Temp);
		Temp = root["header"]["timeStamp"].GetContent().c_str();
		Data.Hd.Set_timeStamp(Temp);

		Temp = root["body"]["EQUIPMENTID"].GetContent().c_str();
		Data.Body.Set_EQUIPMENTID(Temp);
		Temp = root["body"]["SUBEQPID"].GetContent().c_str();
		Data.Body.Set_SUBEQPID(Temp);
		Temp = root["body"]["IPADDRESS"].GetContent().c_str();
		Data.Body.Set_IPADDRESS(Temp);

		//Temp = root["return"]["returnCode"].GetContent().c_str();
		//Data.Rtrn.Set_returnCode(Temp);
		//Temp = root["return"]["returnMessage"].GetContent().c_str();
		//Data.Rtrn.Set_returnMessage(Temp);
	}
	return Data;
}
inline StdStringA CIcsRemoteEes::Make_REPLY_ALARM_LIST(Reply_Alarm_List_Args::Args & pData) {
	StdStringW szTemp;
	CMarkup	m_xml;
	m_xml.RemoveElem();
	m_xml.ResetPos();
	m_xml.FindElem();
	m_xml.IntoElem();
	{
		m_xml.AddElem(_T("message"));
		m_xml.IntoElem(); {
			m_xml.AddElem(_T("header"));
			{
				m_xml.IntoElem();
				szTemp = ToWideChar(pData.Hd.Get_messageName());
				m_xml.AddElem(_T("messageName"), szTemp);
				szTemp = ToWideChar(pData.Hd.Get_transactionId_XuuidToStr());
				m_xml.AddElem(_T("transactionId"), szTemp);
				szTemp = ToWideChar(pData.Hd.Get_messageName());
				m_xml.AddElem(_T("timeStamp"), szTemp);
			}m_xml.OutOfElem();
			m_xml.AddElem(_T("body"));
			{
				m_xml.IntoElem();
				szTemp = ToWideChar(pData.Body.Get_EQUIPMENTID());
				m_xml.AddElem(_T("EQUIPMENTID"), szTemp);
				szTemp = ToWideChar(pData.Body.Get_SUBEQPID());
				m_xml.AddElem(_T("SUBEQPID"), szTemp);
				szTemp = ToWideChar(pData.Body.Get_IPADDRESS());
				m_xml.AddElem(_T("IPADDRESS"), szTemp);
				m_xml.AddElem(_T("ALARMLIST"));
				m_xml.IntoElem(); {
					for (int nCount = 0; nCount < pData.Body.Get_ALARMLIST().Get_Count(); nCount++) {

						m_xml.AddElem(_T("A"));
						m_xml.IntoElem(); {
							szTemp = ToWideChar(pData.Body.Get_ALARMLIST().GetAt(nCount).Get_ID());
							m_xml.AddElem(_T("ID"), szTemp);
							szTemp = ToWideChar(pData.Body.Get_ALARMLIST().GetAt(nCount).Get_GRADE());
							m_xml.AddElem(_T("GRADE"), szTemp);
							szTemp = ToWideChar(pData.Body.Get_ALARMLIST().GetAt(nCount).Get_MESSAGE());
							m_xml.AddElem(_T("MESSAGE"), szTemp);
						}m_xml.OutOfElem();
					}
				}m_xml.OutOfElem();

			}m_xml.OutOfElem();
			m_xml.AddElem(_T("return"));
			{
				m_xml.IntoElem();
				szTemp = ToWideChar(pData.Rtrn.Get_returnCode());
				m_xml.AddElem(_T("returnCode"), szTemp);
				szTemp = ToWideChar(pData.Rtrn.Get_returnMessage());
				m_xml.AddElem(_T("returnMessage"), szTemp);
			}m_xml.OutOfElem();
		}m_xml.OutOfElem();
	}
	m_xml.OutOfElem();
	return ToMultiByte(m_xml.GetSubDoc());
}
bool CIcsRemoteEes::bGetAlarmListProcedure(XUUID UUID) const{
	SimpleLockPtr(m_pAlarmListCntrMutex);
	for (auto iter = m_pAlarmListCntr->begin();
		iter != m_pAlarmListCntr->end(); )	{
		auto csttCommand = (*iter).first;
		if (csttCommand == UUID)	{
			return true;
		}
		iter++;
	}
	return false;
}
void CIcsRemoteEes::RemoveAlarmListProcedure(XUUID UUID)
{
	SimpleLockPtr(m_pAlarmListCntrMutex);
	auto iter = m_pAlarmListCntr->find(UUID);
	if (iter == m_pAlarmListCntr->end()) {
		return;}
	m_pAlarmListCntr->erase(iter);
}
/*
CALARM_DataListCntr CIcsRemoteEes::LoadAlarmList()
{
	CALARM_DataListCntr	XmlConfig;
	StdStringA szFullPath;
	szFullPath = Format(("C:\\_System\\Recipe\\%s"), DLL_AL_INFO_FILENAME);
	LoadXML_ALInfo(szFullPath, XmlConfig);
	return XmlConfig;
}*/

CALARM_DataListCntr CIcsRemoteEes::LoadXML_ALInfo(__in StdStringA szPat)
{
	CALARM_DataListCntr Data;
	XML3::XML		m_xml_Parse;
	XML3::XML_PARSE xmlResult = m_xml_Parse.Load(_T("C:\\_System\\Recipe\\SEM_AL_INFO.xml"));
	if (XML3::XML_PARSE::OK == xmlResult) {
		auto&root = m_xml_Parse.GetRootElement();
		StdStringA Temp = root["Luritech"]["ALInfo"].GetContent().c_str();
	}
	return Data;
}





XUUID CIcsRemoteEes::m_pRmsModeCtrl() {
	SimpleLockPtr(m_pRmsModeCntrMutex);
	for (auto iter = m_pRmsModeCntr->begin();
		iter != m_pRmsModeCntr->end(); ) {
		if (iter != m_pRmsModeCntr->end()) {
			return (*iter).first;
		}
		iter++;
	}
	return XUUID();
}
Rms_Mode_Args::Args CIcsRemoteEes::m_pRmsModeCtrl(XUUID UUID) {
	SimpleLockPtr(m_pRmsModeCntrMutex);
	for (auto iter = m_pRmsModeCntr->begin();
		iter != m_pRmsModeCntr->end(); ) {
		auto csttCommand = (*iter).first;
		if (csttCommand == UUID) {
			return (*iter).second;
		}
		iter++;
	}
	return Rms_Mode_Args::Args();
}
bool CIcsRemoteEes::CreateRmsModeProcedure(XUUID UUID) {
	SimpleLockPtr(m_pRmsModeCntrMutex);
	for (auto iter = m_pRmsModeCntr->begin();
		iter != m_pRmsModeCntr->end(); ) {
		auto csttCommand = (*iter).first;
		if (csttCommand == UUID) {
			return false;
		}
		iter++;
	}
	auto result = m_pRmsModeCntr->emplace(UUID, Rms_Mode_Args::Args());
	return result.second;
}
bool CIcsRemoteEes::CreateRmsModeProcedure()
{
	SimpleLockPtr(m_pRmsModeCntrMutex);
	Rms_Mode_Args::Args Data;
	auto transactionId = Data.REPORT.Hd.CreateUUID();
	auto result = m_pRmsModeCntr->emplace(transactionId, Data);
	return result.second;
}
bool CIcsRemoteEes::AddRmsModeProcedure(XUUID UUID, Report_Rms_Mode_Args::Args & proc)
{
	SimpleLockPtr(m_pRmsModeCntrMutex);
	for (auto iter = m_pRmsModeCntr->begin();
		iter != m_pRmsModeCntr->end(); )
	{
		auto csttCommand = (*iter).first;
		if (csttCommand == UUID)
		{
			(*iter).second.REPORT = proc;
			return true;
		}
		iter++;
	}
	return false;
}
inline StdStringA CIcsRemoteEes::Make_REPORT_RMS_MODE(Report_Rms_Mode_Args::Args & pData) {
	StdStringW szTemp;
	CMarkup	m_xml;
	m_xml.RemoveElem();
	m_xml.ResetPos();
	m_xml.FindElem();
	m_xml.IntoElem();
	{
		m_xml.AddElem(_T("message"));
		m_xml.IntoElem(); {
			m_xml.AddElem(_T("header"));
			{
				m_xml.IntoElem();
				szTemp = ToWideChar(pData.Hd.Get_messageName());
				m_xml.AddElem(_T("messageName"), szTemp);
				szTemp = ToWideChar(pData.Hd.Get_transactionId_XuuidToStr());
				m_xml.AddElem(_T("transactionId"), szTemp);
				szTemp = ToWideChar(pData.Hd.Get_messageName());
				m_xml.AddElem(_T("timeStamp"), szTemp);
			}m_xml.OutOfElem();
			m_xml.AddElem(_T("body"));
			{
				m_xml.IntoElem();
				szTemp = ToWideChar(pData.Body.Get_EQUIPMENTID());
				m_xml.AddElem(_T("EQUIPMENTID"), szTemp);
				szTemp = ToWideChar(pData.Body.Get_SUBEQPID());
				m_xml.AddElem(_T("SUBEQPID"), szTemp);
				szTemp = ToWideChar(pData.Body.Get_IPADDRESS());
				m_xml.AddElem(_T("IPADDRESS"), szTemp);
				szTemp = ToWideChar(pData.Body.Get_USERID());
				m_xml.AddElem(_T("USERID"), szTemp);
				szTemp = ToWideChar(pData.Body.Get_EESMODE());
				m_xml.AddElem(_T("EESMODE"), szTemp);
				szTemp = ToWideChar(pData.Body.Get_LOTID());
				m_xml.AddElem(_T("LOTID"), szTemp);
				szTemp = ToWideChar(pData.Body.Get_RECIPEID());
				m_xml.AddElem(_T("RECIPEID"), szTemp);
				szTemp = ToWideChar(pData.Body.Get_PRODUCTID());
				m_xml.AddElem(_T("PRODUCTID"), szTemp);
				szTemp = ToWideChar(pData.Body.Get_PRGVER());
				m_xml.AddElem(_T("PRGVER"), szTemp);
			}m_xml.OutOfElem();
		}m_xml.OutOfElem();
	}
	m_xml.OutOfElem();
	return ToMultiByte(m_xml.GetSubDoc());
}
bool CIcsRemoteEes::bGetRmsModeProcedure(XUUID UUID) const {
	SimpleLockPtr(m_pRmsModeCntrMutex);
	for (auto iter = m_pRmsModeCntr->begin();
		iter != m_pRmsModeCntr->end(); ) {
		auto csttCommand = (*iter).first;
		if (csttCommand == UUID) {
			return true;
		}
		iter++;
	}
	return false;
}
void CIcsRemoteEes::RemoveRmsModeProcedure(XUUID UUID) {
	SimpleLockPtr(m_pRmsModeCntrMutex);
	auto iter = m_pRmsModeCntr->find(UUID);
	if (iter == m_pRmsModeCntr->end()) {
		return;
	}
	m_pRmsModeCntr->erase(iter);
}







XUUID CIcsRemoteEes::SetDateTimeCtrl() {
	SimpleLockPtr(m_pSetDateTimeCntrMutex);
	for (auto iter = m_pSetDateTimeCntr->begin();
		iter != m_pSetDateTimeCntr->end(); ) {
		if (iter != m_pSetDateTimeCntr->end()) {
			return (*iter).first;
		}
		iter++;
	}
	return XUUID();
}
Set_DateTime_Args::Args  CIcsRemoteEes::SetDateTimeCtrl(XUUID UUID)
{
	SimpleLockPtr(m_pSetDateTimeCntrMutex);
	for (auto iter = m_pSetDateTimeCntr->begin();
		iter != m_pSetDateTimeCntr->end(); ) {
		auto csttCommand = (*iter).first;
		if (csttCommand == UUID) {
			return (*iter).second;
		}
		iter++;
	}
	return Set_DateTime_Args::Args();
}
bool CIcsRemoteEes::CreateSetDateTimeProcedure(XUUID UUID) {
	SimpleLockPtr(m_pSetDateTimeCntrMutex);
	for (auto iter = m_pSetDateTimeCntr->begin();
		iter != m_pSetDateTimeCntr->end(); ) {
		auto csttCommand = (*iter).first;
		if (csttCommand == UUID) {
			return false;
		}
		iter++;
	}
	auto result = m_pSetDateTimeCntr->emplace(UUID, Set_DateTime_Args::Args());
	return result.second;
}
bool CIcsRemoteEes::CreateSetDateTimeProcedure()
{
	SimpleLockPtr(m_pSetDateTimeCntrMutex);
	Set_DateTime_Args::Args Data;
	auto transactionId = Data.REQUEST.Hd.CreateUUID();
	auto result = m_pSetDateTimeCntr->emplace(transactionId, Data);
	return result.second;
}
bool CIcsRemoteEes::AddSetDateTimeProcedure(XUUID UUID, Request_Set_DateTime_Args::Args & proc) {
	SimpleLockPtr(m_pSetDateTimeCntrMutex);
	for (auto iter = m_pSetDateTimeCntr->begin();
		iter != m_pSetDateTimeCntr->end(); ) {
		auto csttCommand = (*iter).first;
		if (csttCommand == UUID) {
			(*iter).second.REQUEST = proc;
			return true;
		}
		iter++;
	}
	return false;
}
bool CIcsRemoteEes::AddSetDateTimeProcedure(XUUID UUID, Reply_Set_DateTime_Args::Args & proc)
{
	SimpleLockPtr(m_pSetDateTimeCntrMutex);
	for (auto iter = m_pSetDateTimeCntr->begin();
		iter != m_pSetDateTimeCntr->end(); ) {
		auto csttCommand = (*iter).first;
		if (csttCommand == UUID) {
			(*iter).second.REPLY = proc;
			return true;
		}
		iter++;
	}
	return false;
}
inline Request_Set_DateTime_Args::Args CIcsRemoteEes::ParseArg_REQUEST_SET_DATETIME(StdStringA pDataz) {
	Request_Set_DateTime_Args::Args Data;
	XML3::XML	m_xml_Parse;
	m_xml_Parse.Clear();
	XML3::XML_PARSE xmlResult = m_xml_Parse.Parse((pDataz).c_str(), pDataz.size());
	if (XML3::XML_PARSE::OK == xmlResult) {
		auto&root = m_xml_Parse.GetRootElement();
		StdStringA Temp = root["header"]["messageName"].GetContent().c_str();
		Data.Hd.Set_messageName(Temp);
		Temp = root["header"]["transactionId"].GetContent().c_str();
		Data.Hd.Set_transactionId_StrToXuuid(Temp);
		Temp = root["header"]["timeStamp"].GetContent().c_str();
		Data.Hd.Set_timeStamp(Temp);

		Temp = root["body"]["EQUIPMENTID"].GetContent().c_str();
		Data.Body.Set_EQUIPMENTID(Temp);
		Temp = root["body"]["IPADDRESS"].GetContent().c_str();
		Data.Body.Set_IPADDRESS(Temp);
		Temp = root["body"]["DATETIME"].GetContent().c_str();
		Data.Body.Set_DATETIME(Temp);

		//Temp = root["return"]["returnCode"].GetContent().c_str();
		//Data.Rtrn.Set_returnCode(Temp);
		//Temp = root["return"]["returnMessage"].GetContent().c_str();
		//Data.Rtrn.Set_returnMessage(Temp);
	}
	return Data;
}
inline StdStringA CIcsRemoteEes::Make_REPLY_SET_DATETIME(Reply_Set_DateTime_Args::Args & pData) {
	StdStringW szTemp;
	CMarkup	m_xml;
	m_xml.RemoveElem();
	m_xml.ResetPos();
	m_xml.FindElem();
	m_xml.IntoElem();
	{
		m_xml.AddElem(_T("message"));
		m_xml.IntoElem(); {
			m_xml.AddElem(_T("header"));
			{
				m_xml.IntoElem();
				szTemp = ToWideChar(pData.Hd.Get_messageName());
				m_xml.AddElem(_T("messageName"), szTemp);
				szTemp = ToWideChar(pData.Hd.Get_transactionId_XuuidToStr());
				m_xml.AddElem(_T("transactionId"), szTemp);
				szTemp = ToWideChar(pData.Hd.Get_messageName());
				m_xml.AddElem(_T("timeStamp"), szTemp);
			}m_xml.OutOfElem();
			m_xml.AddElem(_T("body"));
			{
				m_xml.IntoElem();
				szTemp = ToWideChar(pData.Body.Get_EQUIPMENTID());
				m_xml.AddElem(_T("EQUIPMENTID"), szTemp);
				szTemp = ToWideChar(pData.Body.Get_IPADDRESS());
				m_xml.AddElem(_T("IPADDRESS"), szTemp);
				szTemp = ToWideChar(pData.Body.Get_DATETIME());
				m_xml.AddElem(_T("DATETIME"), szTemp);
			}m_xml.OutOfElem();
			m_xml.AddElem(_T("return"));
			{
				m_xml.IntoElem();
				szTemp = ToWideChar(pData.Rtrn.Get_returnCode());
				m_xml.AddElem(_T("returnCode"), szTemp);
				szTemp = ToWideChar(pData.Rtrn.Get_returnMessage());
				m_xml.AddElem(_T("returnMessage"), szTemp);
			}m_xml.OutOfElem();
		}m_xml.OutOfElem();
	}
	m_xml.OutOfElem();
	return ToMultiByte(m_xml.GetSubDoc());
}
bool CIcsRemoteEes::bGetSetDateTimeProcedure(XUUID UUID) const {
	SimpleLockPtr(m_pSetDateTimeCntrMutex);
	for (auto iter = m_pSetDateTimeCntr->begin();
		iter != m_pSetDateTimeCntr->end(); ) {
		auto csttCommand = (*iter).first;
		if (csttCommand == UUID) {
			return true;
		}
		iter++;
	}
	return false;
}
void CIcsRemoteEes::RemoveSetDateTimeProcedure(XUUID UUID)
{
	SimpleLockPtr(m_pSetDateTimeCntrMutex);
	auto iter = m_pSetDateTimeCntr->find(UUID);
	if (iter == m_pSetDateTimeCntr->end()) {
		return;
	}
	m_pSetDateTimeCntr->erase(iter);
}





XUUID CIcsRemoteEes::TerminalMessageCtrl() {
	SimpleLockPtr(m_pTerminalMessageCntrMutex);
	for (auto iter = m_pTerminalMessageCntr->begin();
		iter != m_pTerminalMessageCntr->end(); ) {
		if (iter != m_pTerminalMessageCntr->end()) {
			return (*iter).first;
		}
		iter++;
	}
	return XUUID();
}
Terminal_Message_Args::Args  CIcsRemoteEes::TerminalMessageCtrl(XUUID UUID)
{
	SimpleLockPtr(m_pTerminalMessageCntrMutex);
	for (auto iter = m_pTerminalMessageCntr->begin();
		iter != m_pTerminalMessageCntr->end(); ) {
		auto csttCommand = (*iter).first;
		if (csttCommand == UUID) {
			return (*iter).second;
		}
		iter++;
	}
	return Terminal_Message_Args::Args();
}
bool CIcsRemoteEes::CreateTerminalMessageProcedure(XUUID UUID) {
	SimpleLockPtr(m_pTerminalMessageCntrMutex);
	for (auto iter = m_pTerminalMessageCntr->begin();
		iter != m_pTerminalMessageCntr->end(); ) {
		auto csttCommand = (*iter).first;
		if (csttCommand == UUID) {
			return false;
		}
		iter++;
	}
	auto result = m_pTerminalMessageCntr->emplace(UUID, Terminal_Message_Args::Args());
	return result.second;
}
bool CIcsRemoteEes::CreateTerminalMessageProcedure()
{
	SimpleLockPtr(m_pTerminalMessageCntrMutex);
	Terminal_Message_Args::Args Data;
	auto transactionId = Data.REQUEST.Hd.CreateUUID();
	auto result = m_pTerminalMessageCntr->emplace(transactionId, Data);
	return result.second;
}
bool CIcsRemoteEes::AddTerminalMessageProcedure(XUUID UUID, Request_Terminal_Message_Args::Args & proc) {
	SimpleLockPtr(m_pTerminalMessageCntrMutex);
	for (auto iter = m_pTerminalMessageCntr->begin();
		iter != m_pTerminalMessageCntr->end(); ) {
		auto csttCommand = (*iter).first;
		if (csttCommand == UUID) {
			(*iter).second.REQUEST = proc;
			return true;
		}
		iter++;
	}
	return false;
}
bool CIcsRemoteEes::AddTerminalMessageProcedure(XUUID UUID, Reply_Terminal_Message_Args::Args & proc)
{
	SimpleLockPtr(m_pTerminalMessageCntrMutex);
	for (auto iter = m_pTerminalMessageCntr->begin();
		iter != m_pTerminalMessageCntr->end(); ) {
		auto csttCommand = (*iter).first;
		if (csttCommand == UUID) {
			(*iter).second.REPLY = proc;
			return true;
		}
		iter++;
	}
	return false;
}
inline Request_Terminal_Message_Args::Args CIcsRemoteEes::ParseArg_REQUEST_TERMINAL_MESSAGE(StdStringA pDataz) {
	Request_Terminal_Message_Args::Args Data;
	XML3::XML	m_xml_Parse;
	m_xml_Parse.Clear();
	XML3::XML_PARSE xmlResult = m_xml_Parse.Parse((pDataz).c_str(), pDataz.size());
	if (XML3::XML_PARSE::OK == xmlResult) {
		auto&root = m_xml_Parse.GetRootElement();
		StdStringA Temp = root["header"]["messageName"].GetContent().c_str();
		Data.Hd.Set_messageName(Temp);
		Temp = root["header"]["transactionId"].GetContent().c_str();
		Data.Hd.Set_transactionId_StrToXuuid(Temp);
		Temp = root["header"]["timeStamp"].GetContent().c_str();
		Data.Hd.Set_timeStamp(Temp);

		Temp = root["body"]["EQUIPMENTID"].GetContent().c_str();
		Data.Body.Set_EQUIPMENTID(Temp);
		Temp = root["body"]["SUBEQPID"].GetContent().c_str();
		Data.Body.Set_SUBEQPID(Temp);
		Temp = root["body"]["IPADDRESS"].GetContent().c_str();
		Data.Body.Set_IPADDRESS(Temp);
		Temp = root["body"]["TERMINALMESSAGE"].GetContent().c_str();
		Data.Body.Set_TERMINALMESSAGE(Temp);

		//Temp = root["return"]["returnCode"].GetContent().c_str();
		//Data.Rtrn.Set_returnCode(Temp);
		//Temp = root["return"]["returnMessage"].GetContent().c_str();
		//Data.Rtrn.Set_returnMessage(Temp);
	}
	return Data;
}
inline StdStringA CIcsRemoteEes::Make_REPLY_TERMINAL_MESSAGE(Reply_Terminal_Message_Args::Args & pData) {
	StdStringW szTemp;
	CMarkup	m_xml;
	m_xml.RemoveElem();
	m_xml.ResetPos();
	m_xml.FindElem();
	m_xml.IntoElem();
	{
		m_xml.AddElem(_T("message"));
		m_xml.IntoElem(); {
			m_xml.AddElem(_T("header"));
			{
				m_xml.IntoElem();
				szTemp = ToWideChar(pData.Hd.Get_messageName());
				m_xml.AddElem(_T("messageName"), szTemp);
				szTemp = ToWideChar(pData.Hd.Get_transactionId_XuuidToStr());
				m_xml.AddElem(_T("transactionId"), szTemp);
				szTemp = ToWideChar(pData.Hd.Get_messageName());
				m_xml.AddElem(_T("timeStamp"), szTemp);
			}m_xml.OutOfElem();
			m_xml.AddElem(_T("body"));
			{
				m_xml.IntoElem();
				szTemp = ToWideChar(pData.Body.Get_EQUIPMENTID());
				m_xml.AddElem(_T("EQUIPMENTID"), szTemp);
				szTemp = ToWideChar(pData.Body.Get_SUBEQPID());
				m_xml.AddElem(_T("SUBEQPID"), szTemp);
				szTemp = ToWideChar(pData.Body.Get_IPADDRESS());
				m_xml.AddElem(_T("IPADDRESS"), szTemp);
				szTemp = ToWideChar(pData.Body.Get_TERMINALMESSAGE());
				m_xml.AddElem(_T("TERMINALMESSAGE"), szTemp);
			}m_xml.OutOfElem();
			m_xml.AddElem(_T("return"));
			{
				m_xml.IntoElem();
				szTemp = ToWideChar(pData.Rtrn.Get_returnCode());
				m_xml.AddElem(_T("returnCode"), szTemp);
				szTemp = ToWideChar(pData.Rtrn.Get_returnMessage());
				m_xml.AddElem(_T("returnMessage"), szTemp);
			}m_xml.OutOfElem();
		}m_xml.OutOfElem();
	}
	m_xml.OutOfElem();
	return ToMultiByte(m_xml.GetSubDoc());
}
bool CIcsRemoteEes::bGetTerminalMessageProcedure(XUUID UUID) const {
	SimpleLockPtr(m_pTerminalMessageCntrMutex);
	for (auto iter = m_pTerminalMessageCntr->begin();
		iter != m_pTerminalMessageCntr->end(); ) {
		auto csttCommand = (*iter).first;
		if (csttCommand == UUID) {
			return true;
		}
		iter++;
	}
	return false;
}
void CIcsRemoteEes::RemoveTerminalMessageProcedure(XUUID UUID)
{
	SimpleLockPtr(m_pTerminalMessageCntrMutex);
	auto iter = m_pTerminalMessageCntr->find(UUID);
	if (iter == m_pTerminalMessageCntr->end()) {
		return;
	}
	m_pTerminalMessageCntr->erase(iter);
}






XUUID CIcsRemoteEes::OpcallCtrl() {
	SimpleLockPtr(m_pOpcallCntrMutex);
	for (auto iter = m_pOpcallCntr->begin();
		iter != m_pOpcallCntr->end(); ) {
		if (iter != m_pOpcallCntr->end()) {
			return (*iter).first;
		}
		iter++;
	}
	return XUUID();
}
Opcall_Args::Args  CIcsRemoteEes::OpcallCtrl(XUUID UUID)
{
	SimpleLockPtr(m_pOpcallCntrMutex);
	for (auto iter = m_pOpcallCntr->begin();
		iter != m_pOpcallCntr->end(); ) {
		auto csttCommand = (*iter).first;
		if (csttCommand == UUID) {
			return (*iter).second;
		}
		iter++;
	}
	return Opcall_Args::Args();
}
bool CIcsRemoteEes::CreateOpcallProcedure(XUUID UUID) {
	SimpleLockPtr(m_pOpcallCntrMutex);
	for (auto iter = m_pOpcallCntr->begin();
		iter != m_pOpcallCntr->end(); ) {
		auto csttCommand = (*iter).first;
		if (csttCommand == UUID) {
			return false;
		}
		iter++;
	}
	auto result = m_pOpcallCntr->emplace(UUID, Opcall_Args::Args());
	return result.second;
}
bool CIcsRemoteEes::CreateOpcallProcedure()
{
	SimpleLockPtr(m_pOpcallCntrMutex);
	Opcall_Args::Args Data;
	auto transactionId = Data.REQUEST.Hd.CreateUUID();
	auto result = m_pOpcallCntr->emplace(transactionId, Data);
	return result.second;
}
bool CIcsRemoteEes::AddOpcallProcedure(XUUID UUID, Request_Opcall_Args::Args & proc) {
	SimpleLockPtr(m_pOpcallCntrMutex);
	for (auto iter = m_pOpcallCntr->begin();
		iter != m_pOpcallCntr->end(); ) {
		auto csttCommand = (*iter).first;
		if (csttCommand == UUID) {
			(*iter).second.REQUEST = proc;
			return true;
		}
		iter++;
	}
	return false;
}
bool CIcsRemoteEes::AddOpcallProcedure(XUUID UUID, Reply_Opcall_Args::Args & proc)
{
	SimpleLockPtr(m_pOpcallCntrMutex);
	for (auto iter = m_pOpcallCntr->begin();
		iter != m_pOpcallCntr->end(); ) {
		auto csttCommand = (*iter).first;
		if (csttCommand == UUID) {
			(*iter).second.REPLY = proc;
			return true;
		}
		iter++;
	}
	return false;
}
inline Request_Opcall_Args::Args CIcsRemoteEes::ParseArg_REQUEST_OPCALL(StdStringA pDataz) {
	Request_Opcall_Args::Args Data;
	XML3::XML	m_xml_Parse;
	m_xml_Parse.Clear();
	XML3::XML_PARSE xmlResult = m_xml_Parse.Parse((pDataz).c_str(), pDataz.size());
	if (XML3::XML_PARSE::OK == xmlResult) {
		auto&root = m_xml_Parse.GetRootElement();
		StdStringA Temp = root["header"]["messageName"].GetContent().c_str();
		Data.Hd.Set_messageName(Temp);
		Temp = root["header"]["transactionId"].GetContent().c_str();
		Data.Hd.Set_transactionId_StrToXuuid(Temp);
		Temp = root["header"]["timeStamp"].GetContent().c_str();
		Data.Hd.Set_timeStamp(Temp);

		Temp = root["body"]["EQUIPMENTID"].GetContent().c_str();
		Data.Body.Set_EQUIPMENTID(Temp);
		Temp = root["body"]["SUBEQPID"].GetContent().c_str();
		Data.Body.Set_SUBEQPID(Temp);
		Temp = root["body"]["IPADDRESS"].GetContent().c_str();
		Data.Body.Set_IPADDRESS(Temp);
		Temp = root["body"]["MESSAGE"].GetContent().c_str();
		Data.Body.Set_MESSAGE(Temp);
		Temp = root["body"]["BUZZER"].GetContent().c_str();
		Data.Body.Set_BUZZER(Temp);
		Temp = root["body"]["TOWERLAMP"].GetContent().c_str();
		Data.Body.Set_TOWERLAMP(Temp);
		//Temp = root["return"]["returnCode"].GetContent().c_str();
		//Data.Rtrn.Set_returnCode(Temp);
		//Temp = root["return"]["returnMessage"].GetContent().c_str();
		//Data.Rtrn.Set_returnMessage(Temp);
	}
	return Data;
}
inline StdStringA CIcsRemoteEes::Make_REPLY_OPCALL(Reply_Opcall_Args::Args & pData) {
	StdStringW szTemp;
	CMarkup	m_xml;
	m_xml.RemoveElem();
	m_xml.ResetPos();
	m_xml.FindElem();
	m_xml.IntoElem();
	{
		m_xml.AddElem(_T("message"));
		m_xml.IntoElem(); {
			m_xml.AddElem(_T("header"));
			{
				m_xml.IntoElem();
				szTemp = ToWideChar(pData.Hd.Get_messageName());
				m_xml.AddElem(_T("messageName"), szTemp);
				szTemp = ToWideChar(pData.Hd.Get_transactionId_XuuidToStr());
				m_xml.AddElem(_T("transactionId"), szTemp);
				szTemp = ToWideChar(pData.Hd.Get_messageName());
				m_xml.AddElem(_T("timeStamp"), szTemp);
			}m_xml.OutOfElem();
			m_xml.AddElem(_T("body"));
			{
				m_xml.IntoElem();
				szTemp = ToWideChar(pData.Body.Get_EQUIPMENTID());
				m_xml.AddElem(_T("EQUIPMENTID"), szTemp);
				szTemp = ToWideChar(pData.Body.Get_SUBEQPID());
				m_xml.AddElem(_T("SUBEQPID"), szTemp);
				szTemp = ToWideChar(pData.Body.Get_IPADDRESS());
				m_xml.AddElem(_T("IPADDRESS"), szTemp);
				szTemp = ToWideChar(pData.Body.Get_MESSAGE());
				m_xml.AddElem(_T("MESSAGE"), szTemp);
				szTemp = ToWideChar(pData.Body.Get_BUZZER());
				m_xml.AddElem(_T("BUZZER"), szTemp);
				szTemp = ToWideChar(pData.Body.Get_TOWERLAMP());
				m_xml.AddElem(_T("TOWERLAMP"), szTemp);
			}m_xml.OutOfElem();
			m_xml.AddElem(_T("return"));
			{
				m_xml.IntoElem();
				szTemp = ToWideChar(pData.Rtrn.Get_returnCode());
				m_xml.AddElem(_T("returnCode"), szTemp);
				szTemp = ToWideChar(pData.Rtrn.Get_returnMessage());
				m_xml.AddElem(_T("returnMessage"), szTemp);
			}m_xml.OutOfElem();
		}m_xml.OutOfElem();
	}
	m_xml.OutOfElem();
	return ToMultiByte(m_xml.GetSubDoc());
}
bool CIcsRemoteEes::bGetOpcallProcedure(XUUID UUID) const {
	SimpleLockPtr(m_pOpcallCntrMutex);
	for (auto iter = m_pOpcallCntr->begin();
		iter != m_pOpcallCntr->end(); ) {
		auto csttCommand = (*iter).first;
		if (csttCommand == UUID) {
			return true;
		}
		iter++;
	}
	return false;
}
void CIcsRemoteEes::RemoveOpcallProcedure(XUUID UUID){
	SimpleLockPtr(m_pOpcallCntrMutex);
	auto iter = m_pOpcallCntr->find(UUID);
	if (iter == m_pOpcallCntr->end()) {
		return;
	}
	m_pOpcallCntr->erase(iter);
}
#endif