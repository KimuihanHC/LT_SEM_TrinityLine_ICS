
#include "stdafx.h"
#include "IcsRemoteServer.h"

#include "Server.h"
#include "Logger/DebugLogDispatcher.h"
#include "Logger/Logger.h"
#include "Comm/IcsRemoteEes.h"
#include "Event/EventMfnListener.hpp"
#include "Util/StringUtil.hpp"


CIcsRemoteServer::CIcsRemoteServer(CServer & server)
: m_Server(server)
{
	// Logger
	m_pDebugLogDispatcher =
		new lt::CDebugLogDispatcher(m_Server.Get_Alias(), lt::LogFilter::Detail);
	m_pLogger = new lt::CLogger();

	static_cast<lt::ILogDispatcher *>(*m_pDebugLogDispatcher)->Dispatch(
		lt::LogLevel::None,
		lt::Format(_T("============================= %s begins ============================="),
			m_Server.Get_Alias()).c_str()
	);
	m_pLogger->Attach(*m_pDebugLogDispatcher);
#if (USE_XML)
	m_pRequestLinkTestEventListener =
		new Request_Link_Test_EventListener(this, &CIcsRemoteServer::OnRequestLinkTestEvent);
	m_pRequestUserCommandEventListener =
		new Request_User_Command_EventListener(this, &CIcsRemoteServer::OnRequestUserCommandEvent);
	m_pRequestEquipmentStateEventListener =
		new Request_Equipment_State_Display_EventListener(this, &CIcsRemoteServer::OnRequestEquipmentStateEvent);
	m_pRequestLossWindowEventListener =
		new Request_Loss_Window_EventListener(this, &CIcsRemoteServer::OnRequestLossWindowEvent);
	m_pRequestAlarmListEventListener =
		new Request_Alarm_List_EventListener(this, &CIcsRemoteServer::OnRequestAlarmListEvent);
	m_pRequestSetDateTimeEventListener =
		new Request_Set_DateTime_EventListener(this, &CIcsRemoteServer::OnRequestSetDateTimeEvent);
	m_pRequestTerminalMessageEventListener =
		new Request_Terminal_Message_EventListener(this, &CIcsRemoteServer::OnRequestTerminalMessageEvent);
	m_pRequestOpcallEventListener =
		new Request_Opcall_EventListener(this, &CIcsRemoteServer::OnRequestOpCallEvent);
#endif
}

CIcsRemoteServer::~CIcsRemoteServer()
{
#if (USE_XML)
	delete m_pRequestLinkTestEventListener;
	delete m_pRequestUserCommandEventListener;
	delete m_pRequestEquipmentStateEventListener;
	delete m_pRequestAlarmListEventListener;
	delete m_pRequestLossWindowEventListener;
	delete m_pRequestSetDateTimeEventListener;
	delete m_pRequestTerminalMessageEventListener;
	delete m_pRequestOpcallEventListener;
	//delete m_pRequestUnitIdReadEventListener;
#endif
	m_pLogger->Detach();
	static_cast<lt::ILogDispatcher *>(*m_pDebugLogDispatcher)->Dispatch(
		lt::LogLevel::None,
		lt::Format(_T("============================= %s ends ============================="),
			m_Server.Get_Alias()).c_str()
	);
	Sleep(100);

	delete m_pLogger;
	delete m_pDebugLogDispatcher;
}
bool CIcsRemoteServer::operator==(const lt::ICommunicator * commPtr) const{
	return GetRemoteEes() == commPtr;}
bool CIcsRemoteServer::operator!=(const lt::ICommunicator * commPtr) const{
	return GetRemoteEes() != commPtr;}

lt::ICommunicator * CIcsRemoteServer::Attach(lt::ICommunicator * commPtr)
{
#if (USE_XML)
	GetRemoteEes().GetRequestLinkTestEventNotifier() +=
		m_pRequestLinkTestEventListener;
	GetRemoteEes().GetRequestUserCommandEventNotifier() +=
		m_pRequestUserCommandEventListener;
	GetRemoteEes().GetRequestEquipmentStateEventNotifier() +=
		m_pRequestEquipmentStateEventListener;
	GetRemoteEes().GetRequestAlarmListEventNotifier() +=
		m_pRequestAlarmListEventListener;
	GetRemoteEes().GetRequestLossWindowEventNotifier() +=
		m_pRequestLossWindowEventListener;
	GetRemoteEes().GetRequestSetDateTimeEventNotifier() +=
		m_pRequestSetDateTimeEventListener;
	GetRemoteEes().GetRequestTerminalMessageEventNotifier() +=
		m_pRequestTerminalMessageEventListener;
	GetRemoteEes().GetRequestOPCallEventNotifier() +=
		m_pRequestOpcallEventListener;
	//UnitIDRead
	//GetRemoteEes().GetRequestUnitReadEventNotifier() +=
	//	m_pRequestUnitIdReadEventListener;
#endif
	GetRemoteEes().SetLogDispatcher(*m_pDebugLogDispatcher);
	auto _commPtr = GetRemoteEes().Attach(commPtr);
	GetLogger()
		.SetLogLevel(lt::LogLevel::Notice)
		.AddLog(_T("The communicator(%p) attached"),
				commPtr);

	GetServer().Set_ClientConnection(IsConnected());

	return _commPtr;
}

lt::ICommunicator * CIcsRemoteServer::Detach()
{
	auto commPtr = GetRemoteEes().Detach();
	GetLogger()
		.SetLogLevel(lt::LogLevel::Notice)
		.AddLog(_T("The communicator(%p) detached"),
				commPtr);

	GetServer().Set_ClientConnection(IsConnected());

#if (USE_XML)
	GetRemoteEes().GetRequestLinkTestEventNotifier() -=
		m_pRequestLinkTestEventListener;
	GetRemoteEes().GetRequestUserCommandEventNotifier() -=
		m_pRequestUserCommandEventListener;
	GetRemoteEes().GetRequestEquipmentStateEventNotifier() -=
		m_pRequestEquipmentStateEventListener;
	GetRemoteEes().GetRequestAlarmListEventNotifier() -=
		m_pRequestAlarmListEventListener;
	GetRemoteEes().GetRequestLossWindowEventNotifier() -=
		m_pRequestLossWindowEventListener;
	GetRemoteEes().GetRequestSetDateTimeEventNotifier() -=
		m_pRequestSetDateTimeEventListener;
	GetRemoteEes().GetRequestTerminalMessageEventNotifier() -=
		m_pRequestTerminalMessageEventListener;
	GetRemoteEes().GetRequestOPCallEventNotifier() -=
		m_pRequestOpcallEventListener;
	//UnitIDRead
	//GetRemoteEes().GetRequestUnitReadEventNotifier() -=
		//m_pRequestUnitIdReadEventListener;
#endif
	GetRemoteEes().SetLogDispatcher(nullptr);
	return commPtr;
}

bool CIcsRemoteServer::IsConnected() const{
	return GetRemoteEes().IsConnected();}

const CConfig_Svr & CIcsRemoteServer::GetConfig() const{
	return GetServer();}

CServer & CIcsRemoteServer::GetServer(){
	return m_Server;}
const CServer & CIcsRemoteServer::GetServer() const{
	return m_Server;}

lt::CLogger & CIcsRemoteServer::GetLogger(){
	return *m_pLogger;}
lt::ILoggable & CIcsRemoteServer::GetLogger() const{
	return *m_pLogger;}

#if (USE_XML)
//=============================================================================
// Method		: SendReplyLinkTestMassage
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2023.06.05 - 19:51
// Desc.		:
//=============================================================================
lt::uint32 CIcsRemoteServer::SendReplyLinkTestMassage(__in lt::Reply_Link_Test_Args::Args pInData){
	auto result = GetRemoteEes().CommandReplyLinkTest(
		{ std::forward<lt::Reply_Link_Test_Args::Args &>(pInData) });
	return result;
}
lt::uint32 CIcsRemoteServer::SendReportOnlineStateMassage(__in lt::Report_Online_State_Args::Args pInData){
	auto result = GetRemoteEes().CommandReportOnlineState(
		{ std::forward<lt::Report_Online_State_Args::Args &>(pInData) });
	return result;
}
lt::uint32 CIcsRemoteServer::SendReportUserChangeMassage(__in lt::Report_User_Change_Args::Args  pInData){
	auto result = GetRemoteEes().CommandReportUserChange(
		{ std::forward<lt::Report_User_Change_Args::Args &>(pInData) });
	return result;
}
lt::uint32 CIcsRemoteServer::SendReplyUserCommandMassage(__in lt::Reply_User_Change_Args::Args pInData){
	auto result = GetRemoteEes().CommandReplyUserCommand(
			{ std::forward<lt::Reply_User_Change_Args::Args &>(pInData) });
	return result;
}
lt::uint32 CIcsRemoteServer::SendReportEquipmentStateMassage(__in lt::Report_Equipment_State_Args::Args pInData){
	auto result = GetRemoteEes().CommandReportEquipmentState(
		{ std::forward<lt::Report_Equipment_State_Args::Args &>(pInData) }
	);
	return result;
}
lt::uint32 CIcsRemoteServer::SendReplyEquipmentStateDisplayMassage(__in lt::Reply_Equipment_State_Display_Args::Args  pInData){
	auto result = GetRemoteEes().CommandReplyEquipmentStateDisplay(
		{ std::forward<__in lt::Reply_Equipment_State_Display_Args::Args &>(pInData) });
	return result;
}
lt::uint32 CIcsRemoteServer::SendReportLossStateMassage(__in lt::Report_Loss_State_Args::Args pInData)
{
	auto result = GetRemoteEes().CommandReportLossState(
		{ std::forward<__in lt::Report_Loss_State_Args::Args &>(pInData) });
	return result;
}
lt::uint32 CIcsRemoteServer::SendReplyLossWindowMassage(__in lt::Reply_Loss_Window_Args::Args pInData){
	auto result = GetRemoteEes().CommandReplyLossWindow(
			{ std::forward<__in lt::Reply_Loss_Window_Args::Args &>(pInData) });
	return result;
}
lt::uint32 CIcsRemoteServer::SendReportAlarmStateMassage(__in lt::Report_Alarm_State_Args::Args pInData){
	auto result = GetRemoteEes().CommandReportAlarmState(
		{ std::forward<lt::Report_Alarm_State_Args::Args &>(pInData) });
	return result;
}
lt::uint32 CIcsRemoteServer::SendReplyAlarmListMassage(__in lt::Reply_Alarm_List_Args::Args pInData){
	lt::Reply_Alarm_List_Args::Args Data;
	auto result = GetRemoteEes().CommandReplyAlarmList(
		{ std::forward<lt::Reply_Alarm_List_Args::Args &>(pInData) });
	return result;
}
lt::uint32 CIcsRemoteServer::SendReportRmsModeMassage(__in lt::Report_Rms_Mode_Args::Args pInData){
	auto result = GetRemoteEes().CommandReportRMSMode(
		{ std::forward<lt::Report_Rms_Mode_Args::Args &>(pInData) });
	return result;
}
lt::uint32 CIcsRemoteServer::SendReplySetDateTimeMassage(__in lt::Reply_Set_DateTime_Args::Args  pInData){
	auto result = GetRemoteEes().CommandReplySetDateTime(
		{ std::forward<lt::Reply_Set_DateTime_Args::Args &>(pInData) }	);
	return result;
}
lt::uint32 CIcsRemoteServer::SendReplyTerminalMessageMassage(__in lt::Reply_Terminal_Message_Args::Args pInData){
	auto result = GetRemoteEes().CommandReplyTerminalMessage(
		{ std::forward<lt::Reply_Terminal_Message_Args::Args &>(pInData) }	);
	return result;
}
lt::uint32 CIcsRemoteServer::SendReplyOpCallMassage(__in lt::Reply_Opcall_Args::Args  pInData){
	auto result = GetRemoteEes().CommandReplyOpCall(
		{ std::forward<lt::Reply_Opcall_Args::Args &>(pInData) }	);
	return result;
}
#if TEST
lt::uint32 CIcsRemoteServer::SendUnitReadMassage(__in ST_xml_UNITID_READ * pInData)
{
	pInData->Hd.transactionId = MainFrame->CreateTransactionID();
	MainFrame->GetTransactionID(pInData->Hd.transactionId)->ReportMsg = GetServer().GetXmlEes().MakeXML_UNITID_READ(pInData);

	lt::ST_xml_Base state = {
		MainFrame->GetTransactionID(pInData->Hd.transactionId)->ReportMsg,
	};
	auto result = GetRemoteEes().CommandUnitIdRead(
		{ static_cast<lt::ST_xml_Base>(state) }
	);
	return result;
}
lt::uint32 CIcsRemoteServer::SendReplyUnitReadMassage(__in ST_xml_REPLY_UNITID_READ * pInData)
{
	if (MainFrame->bGetTransactionID(pInData->Hd.transactionId)) {
		MainFrame->GetTransactionID(pInData->Hd.transactionId)->ReplyMsg = GetServer().GetXmlEes().MakeXML_REPLY_UNITID_READ(pInData);

		lt::ST_xml_Base state = {
			MainFrame->GetTransactionID(pInData->Hd.transactionId)->ReplyMsg,
		};
		auto result = GetRemoteEes().CommandReplyUnitIdRead(
			{ static_cast<lt::ST_xml_Base>(state) }
		);
		MainFrame->ClearTransactionID(pInData->Hd.transactionId);
		return result;
	}
	else {
		CString transactionId(pInData->Hd.transactionId);
		GetLogger()
			.SetLogLevel(lt::LogLevel::Notice)
			.AddLog(_T("SendReplyOpCallMassage (transactionId Fail = %s)"),
				transactionId);
	}
	return 0;
}
lt::uint32 CIcsRemoteServer::SendReportStartLotMassage(__in ST_xml_REPORT_START_LOT * pInData)
{	
	pInData->Hd.transactionId = MainFrame->CreateTransactionID();
	MainFrame->GetTransactionID(pInData->Hd.transactionId)->ReportMsg = GetServer().GetXmlEes().MakeXML_REPORT_START_LOT(pInData);

	lt::ST_xml_Base state = {
		MainFrame->GetTransactionID(pInData->Hd.transactionId)->ReportMsg,
	};
	auto result = GetRemoteEes().CommandReportStartLot(
		{ static_cast<lt::ST_xml_Base>(state) }
	);
	MainFrame->ClearTransactionID(pInData->Hd.transactionId);
	return result;
}
lt::uint32 CIcsRemoteServer::SendReportEndEventMassage(__in ST_xml_REPORT_END_EVENT * pInData)
{
	pInData->Hd.transactionId = MainFrame->CreateTransactionID();
	MainFrame->GetTransactionID(pInData->Hd.transactionId)->ReportMsg = GetServer().GetXmlEes().MakeXML_REPORT_END_EVENT(pInData);

	lt::ST_xml_Base state = {
		MainFrame->GetTransactionID(pInData->Hd.transactionId)->ReportMsg,
	};
	auto result = GetRemoteEes().CommandReportEndEvent(
		{ static_cast<lt::ST_xml_Base>(state) }
	);
	MainFrame->ClearTransactionID(pInData->Hd.transactionId);
	return result;
}
lt::uint32 CIcsRemoteServer::SendReportStartProcessMassage(__in ST_xml_REPORT_START_PROCESS * pInData)
{
	pInData->Hd.transactionId = MainFrame->CreateTransactionID();
	MainFrame->GetTransactionID(pInData->Hd.transactionId)->ReportMsg = GetServer().GetXmlEes().MakeXML_REPORT_START_PROCESS(pInData);

	lt::ST_xml_Base state = {
		MainFrame->GetTransactionID(pInData->Hd.transactionId)->ReportMsg,
	};
	auto result = GetRemoteEes().CommandReportStartProcess(
		{ static_cast<lt::ST_xml_Base>(state) }
	);
	MainFrame->ClearTransactionID(pInData->Hd.transactionId);
	return result;
}
lt::uint32 CIcsRemoteServer::SendReportEndProcessMassage(__in ST_xml_REPORT_END_PROCESS * pInData)
{
	pInData->Hd.transactionId = MainFrame->CreateTransactionID();
	MainFrame->GetTransactionID(pInData->Hd.transactionId)->ReportMsg = GetServer().GetXmlEes().MakeXML_REPORT_END_PROCESS(pInData);

	lt::ST_xml_Base state = {
		MainFrame->GetTransactionID(pInData->Hd.transactionId)->ReportMsg,
	};

	auto result = GetRemoteEes().CommandReportEndProcess(
		{ static_cast<lt::ST_xml_Base>(state) }
	);
	MainFrame->ClearTransactionID(pInData->Hd.transactionId);
	return result;
}
#endif


#if (USE_XML)
void CIcsRemoteServer::OnRequestLinkTestEvent(Request_Link_Test_EventArgs & eventArgs){
	auto & args = eventArgs.GetArgs();
	auto & reqArgs = args.GetTypedArgs();

	GetRemoteEes().CreateLinkTestProcedure(reqArgs.Hd.Get_transactionId());
	GetRemoteEes().AddLinkTestProcedure(reqArgs.Hd.Get_transactionId(), reqArgs);
	GetServer().Set_REQUEST_LINK_TEST(nullptr);
}
void CIcsRemoteServer::OnRequestUserCommandEvent(Request_User_Command_EventArgs & eventArgs){
	auto & args = eventArgs.GetArgs();
	auto & reqArgs = args.GetTypedArgs();
	GetRemoteEes().AddeUserCommandProcedure(reqArgs.Hd.Get_transactionId(), reqArgs);
	GetServer().Set_REQUEST_USER_COMMAND(nullptr);

}
void CIcsRemoteServer::OnRequestEquipmentStateEvent(Request_Equipment_State_Display_EventArgs & eventArgs){
	auto & args = eventArgs.GetArgs();
	auto & reqArgs = args.GetTypedArgs();

	//GetServer().Set_REQUEST_EQUIPMENT_STATE_DISPLAY(Request);
}
void CIcsRemoteServer::OnRequestLossWindowEvent(Request_Loss_Window_EventArgs & eventArgs){
	auto & args = eventArgs.GetArgs();
	auto & reqArgs = args.GetTypedArgs();

	//GetServer().Set_REQUEST_LOSS_WINDOW(Request);
}
void CIcsRemoteServer::OnRequestAlarmListEvent(Request_Alarm_List_EventArgs & eventArgs){
	auto & args = eventArgs.GetArgs();
	auto & reqArgs = args.GetTypedArgs();
	//GetServer().Set_REQUEST_ALARM_LIST(Request);
}
void CIcsRemoteServer::OnRequestSetDateTimeEvent(Request_Set_DateTime_EventArgs & eventArgs)
{
	auto & args = eventArgs.GetArgs();
	auto & reqArgs = args.GetTypedArgs();
	//GetServer().Set_REQUEST_SET_DATETIME(Request);
}
void CIcsRemoteServer::OnRequestTerminalMessageEvent(Request_Terminal_Message_EventArgs & eventArgs)
{
	auto & args = eventArgs.GetArgs();
	auto & reqArgs = args.GetTypedArgs();
	//GetServer().Set_REQUEST_TERMINAL_MESSAGE(Request);
}
void CIcsRemoteServer::OnRequestOpCallEvent(Request_Opcall_EventArgs & eventArgs)
{
	auto & args = eventArgs.GetArgs();
	auto & reqArgs = args.GetTypedArgs();
	//GetServer().Set_REQUEST_OPCALL_MESSAGE(Request);
}
#endif

#if TEST
void CIcsRemoteServer::OnRequestUnitIdReadEvent(XmlEventArgs & eventArgs)
{
	auto & args = eventArgs.GetArgs();
	auto & reqArgs = args.GetTypedArgs();

	//Create.
	ST_xml_REQUEST_UNITID_READ * Request = new ST_xml_REQUEST_UNITID_READ();
	LPARAM para = (LPARAM)Request;
	MainFrame->AddLPARAM(para);
	Request = GetXmlEes().ClearXML_REQUEST_UNITID_READ(Request);
	GetXmlEes().ParseXML_REQUEST_UNITID_READ((reqArgs.ReportMsg),
		Request);

	if (MainFrame->bGetTransactionID(Request->Hd.transactionId)) {
		MainFrame->GetTransactionID(Request->Hd.transactionId)->RequestMsg = reqArgs.ReportMsg;
		GetServer().Set_REQUEST_UNITID_READ(Request);
	}
	else {
		delete Request;
		Request = nullptr;
		MainFrame->RemoveLPARAM(para);
	}
}
#endif
#endif