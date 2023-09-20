// wnd_TestView.cpp: 구현 파일
//

#include "stdafx.h"
#include "resource.h"
#include "wnd_TestView.h"
#include "TestManager_EQP.h"
#include "IcsCommunicator.h"

#include "Util/StringUtil.hpp"
#include "Util/SyncUtil.hpp"

//#include "Comm/IcsRemoteEes.h"
#include "Event/EventMfnListener.hpp"
// Cwnd_TestView

IMPLEMENT_DYNCREATE(Cwnd_TestView, CFormView)

Cwnd_TestView* pwndTestView = nullptr;

Cwnd_TestView::Cwnd_TestView()
	: CFormView(IDD_TEST_FORM)
{
	pwndTestView = this;

	//Link Test
	m_pUIRequestLinkTestEventListener =
		new EesUIRequestReplyEventListener(this, &Cwnd_TestView::OnUIRequestLinkTestEvent);
	m_pUIRequestAlarmListEventListener =
		new EesUIRequestReplyEventListener(this, &Cwnd_TestView::OnUIRequestAlarmListEvent);
	m_pUIRequestSetDateTimeEventListener =
		new EesUIRequestReplyEventListener(this, &Cwnd_TestView::OnUIRequestSetDateTimeEvent);
	m_pUIRequestTerminalMessageEventListener =
		new EesUIRequestReplyEventListener(this, &Cwnd_TestView::OnUIRequestTerminalMessageEvent);
	m_pUIRequestOPCALLEventListener =
		new EesUIRequestReplyEventListener(this, &Cwnd_TestView::OnUIRequestOPCALLEvent);
	m_pUIRequestUnitIdReadEventListener =
		new EesUIRequestReplyEventListener(this, &Cwnd_TestView::OnUIRequestUnitIdReadEvent);

}

Cwnd_TestView::~Cwnd_TestView()
{
	delete m_pUIRequestLinkTestEventListener;
	delete m_pUIRequestAlarmListEventListener;
	delete m_pUIRequestSetDateTimeEventListener;
	delete m_pUIRequestTerminalMessageEventListener;
	delete m_pUIRequestOPCALLEventListener;
	delete m_pUIRequestUnitIdReadEventListener;
}

void Cwnd_TestView::CreateUIRequestEventListener()
{
	/*
	GetRemoteEqp().GetUIRequestLinkTestEventNotifier() +=
		m_pUIRequestLinkTestEventListener;
	GetRemoteEqp().GetUIRequestAlarmListEventNotifier() +=
		m_pUIRequestAlarmListEventListener;
	GetRemoteEqp().GetUIRequestSetDateTimeEventNotifier() +=
		m_pUIRequestSetDateTimeEventListener;
	GetRemoteEqp().GetUIRequestTerminalMessageEventNotifier() +=
		m_pUIRequestTerminalMessageEventListener;
	GetRemoteEqp().GetUIRequestOPCallEventNotifier() +=
		m_pUIRequestOPCALLEventListener;
	GetRemoteEqp().GetUIRequestUnitReadEventNotifier() +=
		m_pUIRequestUnitIdReadEventListener;
	*/
}
void Cwnd_TestView::CloseUIRequestEventListener()
{
	/*
	GetRemoteEqp().GetUIRequestLinkTestEventNotifier() -=
		m_pUIRequestLinkTestEventListener;
	GetRemoteEqp().GetUIRequestAlarmListEventNotifier() -=
		m_pUIRequestAlarmListEventListener;
	GetRemoteEqp().GetUIRequestSetDateTimeEventNotifier() -=
		m_pUIRequestSetDateTimeEventListener;
	GetRemoteEqp().GetUIRequestOPCallEventNotifier() -=
		m_pUIRequestOPCALLEventListener;
	GetRemoteEqp().GetUIRequestUnitReadEventNotifier() -=
		m_pUIRequestUnitIdReadEventListener;
		*/
}

void Cwnd_TestView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REQUEST_LIST, m_RequestList);
	DDX_Control(pDX, IDC_REPORT_LIST, m_ReportList);
	DDX_Control(pDX, IDC_REPLY_LIST, m_ReplyList);
	DDX_Control(pDX, IDC_FILEDATA_LIST, m_FileData);
	DDX_Control(pDX, IDC_REPORT_LIST2, m_ReportList2);
	DDX_Control(pDX, IDC_REPLY_LIST2, m_ReplyList2);
}

BEGIN_MESSAGE_MAP(Cwnd_TestView, CFormView)
ON_WM_CLOSE()
ON_WM_CREATE()
ON_BN_CLICKED(IDC_BUTTON_REPORT, &Cwnd_TestView::OnBnClickedButtonReport)
ON_BN_CLICKED(IDC_BUTTON_SAVE_ALARMLIST, &Cwnd_TestView::OnBnClickedButtonSaveAlarmlist)
ON_BN_CLICKED(IDC_BUTTON_REPLY, &Cwnd_TestView::OnBnClickedButtonReply)
ON_BN_CLICKED(IDC_BUTTON_DATACLEAR, &Cwnd_TestView::OnBnClickedButtonDataClear)
ON_BN_CLICKED(IDC_BUTTON_SAVE_UNITID_READ, &Cwnd_TestView::OnBnClickedButtonSaveUnitidRead)
ON_BN_CLICKED(IDC_BUTTON_REPLY_UNITID_LIST, &Cwnd_TestView::OnBnClickedButtonReplyUnitidList)
ON_BN_CLICKED(IDC_BUTTON_REPORT_DATACLEAR, &Cwnd_TestView::OnBnClickedButtonReportDataclear)
ON_BN_CLICKED(IDC_BUTTON_REPLY_DATACLEAR, &Cwnd_TestView::OnBnClickedButtonReplyDataclear)
ON_BN_CLICKED(IDC_BUTTON_REQUEST_DATACLEAR, &Cwnd_TestView::OnBnClickedButtonRequestDataclear)
END_MESSAGE_MAP()


// Cwnd_TestView 진단


#ifdef _DEBUG
void Cwnd_TestView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void Cwnd_TestView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG

CIcsRemoteServer & Cwnd_TestView::GetRemoteEqp()
{
	return *m_pIcsRemoteSvrEes;
}
CIcsXmlBase & Cwnd_TestView::GetXmlEes()
{
	return GetRemoteEqp().GetXmlEes();
}

BOOL Cwnd_TestView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	return CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void Cwnd_TestView::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CFormView::OnClose();
}

int Cwnd_TestView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	return 0;
}

void Cwnd_TestView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CComboBox * p1 = static_cast<CComboBox *>(GetDlgItem(IDC_COMBO_REPORT));
	for (int i = 0; i < Report_Max; i++) {
		p1->InsertString(i, g_szReportMessage[i]);
	}
	CComboBox * p2 = static_cast<CComboBox *>(GetDlgItem(IDC_COMBO_REPLY));
	for (int i = 0; i < Reply_Max; i++) {
		p2->InsertString(i, g_szReplyMessage[i]);
	}
}

void Cwnd_TestView::OnBnClickedButtonReport()
{
	/*
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	static constexpr lt::uint64 Timeout = 10000;
	CComboBox * p1 = static_cast<CComboBox *>(GetDlgItem(IDC_COMBO_REPORT));
	CComboBox * p2 = static_cast<CComboBox *>(GetDlgItem(IDC_COMBO_REPLY));
	lt::uint32 rtrn;
	int nCnt = p1->GetCurSel();
	switch (nCnt) {
	case Report_Online_State:
		SetUIReportOnlineState(GetXmlEes().xmlReportOnlineState);
		rtrn = GetRemoteEqp().SendxmlReportOnlineStateMassage(
			&GetXmlEes().xmlReportOnlineState);
		GetUIListBox(m_ReportList,
			GetXmlEes().xmlReportOnlineState.ReportMsg);
		break;
	case Report_User_Change:
		SetUIReportUserChange(GetXmlEes().xmlReportUserChange);

		rtrn = GetRemoteEqp().SendxmlReportUserChangeMassage(
			&GetXmlEes().xmlReportUserChange,
			&GetXmlEes().xmlRequestUserCommand,
			Timeout);

		GetUIListBox(m_ReportList,
			GetXmlEes().xmlReportUserChange.ReportMsg);
		GetUIListBox(m_RequestList,
			GetXmlEes().xmlRequestUserCommand.RequestMsg);

		GetUIRequestUserCommand(GetXmlEes().xmlRequestUserCommand);
		p2->SetCurSel(Reply_User_Command);
		break;
	case Report_Equipment_State:
		SetUIReportEquipmentState(GetXmlEes().xmlReportEquipmentState);

		rtrn = GetRemoteEqp().SendxmlReportEquipmentStateMassage(
			&GetXmlEes().xmlReportEquipmentState,
			&GetXmlEes().xmlRequiestEquipmentStateDisplay,
			Timeout);

		GetUIListBox(m_ReportList,
			GetXmlEes().xmlReportEquipmentState.ReportMsg);
		GetUIListBox(m_RequestList,
			GetXmlEes().xmlRequiestEquipmentStateDisplay.RequestMsg);

		GetUIRequestEquipmentStateDisplay(GetXmlEes().xmlRequiestEquipmentStateDisplay);
		break;
	case Report_Loss_State:
		SetUIReportLossState(GetXmlEes().xmlReportLossState);

		rtrn = GetRemoteEqp().SendxmlReportLossStateMassage(
			&GetXmlEes().xmlReportLossState,
			&GetXmlEes().xmlRequestLossWindow,
			Timeout);

		GetUIListBox(m_ReportList,
			GetXmlEes().xmlReportLossState.ReportMsg);
		GetUIListBox(m_RequestList,
			GetXmlEes().xmlRequestLossWindow.RequestMsg);

		GetUIRequestLossWindow(GetXmlEes().xmlRequestLossWindow);
		p2->SetCurSel(Reply_Equipment_State_Display);
		break;
	case Report_Alarm_State:
		SetUIReportAlarmState(GetXmlEes().xmlReportAlarmState);

		rtrn = GetRemoteEqp().SendxmlReportAlarmStateMassage(
			&GetXmlEes().xmlReportAlarmState);

		GetUIListBox(m_ReportList,
			GetXmlEes().xmlReportAlarmState.ReportMsg);

		p2->SetCurSel(Reply_Loss_State);
		break;
	case Report_Rms_Mode:
		SetUIReportRmsMode(GetXmlEes().xmlReportRmsMode);
		rtrn = GetRemoteEqp().SendxmlReportRmsModeMassage(
			&GetXmlEes().xmlReportRmsMode);		
		GetUIListBox(m_ReportList,
			GetXmlEes().xmlReportRmsMode.ReportMsg);
		break;
	case Unit_Read:
		SetUIUnitIdRead(GetXmlEes().xmlUnitRead);
		rtrn = GetRemoteEqp().SendxmlUnitRead(
			&GetXmlEes().xmlUnitRead);
		GetUIListBox(m_ReportList,
			GetXmlEes().xmlUnitRead.ReportMsg);
		break;
	case Report_Start_Lot:
		SetUIReportStartLot(GetXmlEes().xmlReportStartLot);
		rtrn = GetRemoteEqp().SendxmlReportStartLotMassage(
			&GetXmlEes().xmlReportStartLot);
		GetUIListBox(m_ReportList,
			GetXmlEes().xmlReportStartLot.ReportMsg);
		break;
	case Report_End_Event:
		SetUIReportEndEvent(GetXmlEes().xmlReportEndEvent);
		rtrn = GetRemoteEqp().SendxmlReportEndEventMassage(
			&GetXmlEes().xmlReportEndEvent);
		GetUIListBox(m_ReportList,
			GetXmlEes().xmlReportEndEvent.ReportMsg);
		break;
	case Report_End_Process:
		SetUIReportEndProcess(GetXmlEes().xmlReportEndProcess);
		rtrn = GetRemoteEqp().SendxmlReportEndProcessMassage(
			&GetXmlEes().xmlReportEndProcess);
		GetUIListBox(m_ReportList,
			GetXmlEes().xmlReportEndProcess.ReportMsg);
		break;
	case Report_Start_Process:
		SetUIReportStartProcess(GetXmlEes().xmlReportStartProcess);
		rtrn = GetRemoteEqp().SendxmlReportStartProcessMassage(
			&GetXmlEes().xmlReportStartProcess);
		GetUIListBox(m_ReportList,
			GetXmlEes().xmlReportStartProcess.ReportMsg);
		break;
	}
	*/
}

void Cwnd_TestView::OnBnClickedButtonReply()
{
	/*
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	static constexpr lt::uint64 Timeout = 10000;
	CComboBox * p = static_cast<CComboBox *>(GetDlgItem(IDC_COMBO_REPLY));
	int nCnt = p->GetCurSel();
	switch (nCnt) {
	//case Reply_Link_Test:
		//SetUIReplyLinkTestChange(GetXmlEes().xmlReplyLinkTest);
		//GetRemoteEqp().SendReply
		//break;
	case Reply_User_Command:
		SetUIReplyUserCommandChange(GetXmlEes().xmlReplyUserCommand);
		GetRemoteEqp().SendxmlReplyUserCommandMassage(
			&GetXmlEes().xmlReplyUserCommand);
		GetUIListBox(m_ReplyList,
			GetXmlEes().xmlReplyUserCommand.ReplyMsg);
		break;
	case Reply_Equipment_State_Display:
		SetUIReplyEquipmentState(GetXmlEes().xmlReplyEquipmentStateDisplay);
		GetRemoteEqp().SendxmlReplyEquipmentStateDisplayMassage(
			&GetXmlEes().xmlReplyEquipmentStateDisplay,
			Timeout);
		GetUIListBox(m_ReplyList,
			GetXmlEes().xmlReplyEquipmentStateDisplay.ReplyMsg);
		break;
	case Reply_Loss_State:
		SetUIReplyLossWindow(GetXmlEes().xmlReplyLossWindow);
		GetRemoteEqp().SendxmlReplyLossWindowMassage(
			&GetXmlEes().xmlReplyLossWindow);
		GetUIListBox(m_ReplyList,
			GetXmlEes().xmlReplyLossWindow.ReplyMsg);
		break;
	//case Reply_Alarm_State:
		//break;
	//case Reply_Set_DateTime:
		//break;
	//case Reply_Terminal_Message:
		//break;
	case Reply_OpCall:
		SetUIReplyOpCall(GetXmlEes().xmlReplyOpCall);
		GetRemoteEqp().SendxmlReplyOpCallMassage(
			&GetXmlEes().xmlReplyOpCall);
		GetUIListBox(m_ReplyList,
			GetXmlEes().xmlReplyOpCall.ReplyMsg);
		break;
	case Reply_Unitid_Read:
		SetUIReplyUnitIdRead(GetXmlEes().xmlReplyUnitIdRead);
		GetRemoteEqp().SendxmlReplyUnitRead(
			&GetXmlEes().xmlReplyUnitIdRead);
		GetUIListBox(m_ReplyList,
			GetXmlEes().xmlReplyUnitIdRead.ReplyMsg);
		break;
	}
	*/
}

void Cwnd_TestView::SetDlgTxt(int ID, CStringA text) {
	SetDlgItemText(ID, CA2T(text));
}
CStringA Cwnd_TestView::GetDlgTxt(int ID) {
	CString	GetText;
	GetDlgItemText(ID, GetText);
	return CT2A(GetText);
}
void Cwnd_TestView::OnBnClickedButtonDataClear()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	for (int n = 0; n < UI_Select_Max; n++) {
		ClearData(n);
	}
	
}
void Cwnd_TestView::ClearData(int nIndex)
{
	static constexpr int ReportIds[] =
	{
		IDC_REPORT_TRANSACTIONID,
		IDC_REPORT_TIMESTAMP,
		IDC_REPORT_EQUIPMENTID,
		IDC_REPORT_IPADDRESS,
		IDC_REPORT_USERID,
		IDC_REPORT_EESMODE,
		IDC_REPORT_LOTID,
		IDC_REPORT_RECIPEID,
		IDC_REPORT_PRODUCTID,
		IDC_REPORT_PRGVER,
		IDC_REPORT_ONLINESTATE,
		IDC_REPORT_EQUIPMENTSTATE,
		IDC_REPORT_SUBEQPID,
		IDC_REPORT_LOSSSTATE,
		IDC_REPORT_LOSSCOUNT,
		IDC_REPORT_ALARMCODE,
		IDC_REPORT_ALARMID,
		IDC_REPORT_ALARMMESSAGE,
		IDC_REPORT_PORTID,
		IDC_REPORT_CARRIERID,
		IDC_REPORT_CARRTYPE,
		IDC_REPORT_OUTCARRIERID,
		IDC_REPORT_OUTCARRTYPE,
		IDC_REPORT_OUTCARRPORTID,
	};
	static constexpr int RequestIds[] =
	{
		IDC_REQUEST_MESSAGENAME,
		IDC_REQUEST_TIMESTAMP,
		IDC_REQUEST_TRANSACTIONID,
		IDC_REQUEST_EQUIPMENTID,
		IDC_REQUEST_IPADDRESS,
		IDC_REQUEST_USERID,
		IDC_REQUEST_APPLICATIONID,
		IDC_REQUEST_SUBEQPID,
		IDC_REQUEST_EQUIPMENTSTATEDISPLAY,
		IDC_REQUEST_RGBDISPLAY,
		IDC_REQUEST_LOSSWINDOW,
		IDC_REQUEST_DATETIME,
		IDC_REQUEST_TERMINALMESSAGE,
		IDC_REQUEST_MESSAGE,
		IDC_REQUEST_BUZZER,
		IDC_REQUEST_TOWERLAMP,
		IDC_REQUEST_EQPTYPECODE,
		IDC_REQUEST_LOTID,
		IDC_REQUEST_PRODUCTID,
		IDC_REQUEST_PORTID,
		IDC_REQUEST_STEPID,
		IDC_REQUEST_CONSUMLIST,
		IDC_REQUEST_RETURNCODE,
		IDC_REQUEST_RETURNMESSAGE,
	};
	static constexpr int ReplyIds[] =
	{
		IDC_REPLY_TRANSACTIONID,
		IDC_REPLY_TIMESTAMP,
		IDC_REPLY_EQUIPMENTID,
		IDC_REPLY_IPADDRESS,
		IDC_REPLY_USERID,
		IDC_REPLY_APPLICATIONID,
		IDC_REPLY_SUBEQPID,
		IDC_REPLY_EQUIPMENTSTATEDISPLAY,
		IDC_REPLY_RGBDISPLAY,
		IDC_REPLY_LOSSWINDOW,
		IDC_REPLY_DATETIME,
		IDC_REPLY_TERMINALMESSAGE,
		IDC_REPLY_MESSAGE,
		IDC_REPLY_BUZZER,
		IDC_REPLY_TOWERLAMP,
	};
	switch (nIndex) {
	case UI_Select_Report:
		for (auto index = 0; index < _countof(ReportIds); ++index)
		{
			auto ctrlPtr = GetDlgItem(ReportIds[index]);

			if (ctrlPtr == nullptr)
				continue;
			ctrlPtr->SetWindowText(nullptr);
		}
		break;
	case UI_Select_Request:
		for (auto index = 0; index < _countof(RequestIds); ++index)
		{
			auto ctrlPtr = GetDlgItem(RequestIds[index]);

			if (ctrlPtr == nullptr)
				continue;
			ctrlPtr->SetWindowText(nullptr);
		}
		break;
	case UI_Select_Reply:
		for (auto index = 0; index < _countof(ReplyIds); ++index)
		{
			auto ctrlPtr = GetDlgItem(ReplyIds[index]);

			if (ctrlPtr == nullptr)
				continue;
			ctrlPtr->SetWindowText(nullptr);
		}
		break;
	}
}
void Cwnd_TestView::GetUIListBox(CListBox & ListBox,
	lt::ConstStringT stListData) {
	if (!stListData) {
		return;
	}
	ListBox.ResetContent();
	lt::StdStringA ssaText = lt::ToMultiByte(stListData);
	lt::StdStringA ssaTest;
	auto length = ssaText.length();
	int cnt = 0;
	auto start_found = 0;
	auto end_found   = ssaText.find(("\n</"), 0);
	for (int index = 0; index < length; )
	{
		//auto index_found = ssaText.find_first_of(("\r\n"), index);
		auto index_found = ssaText.find(("\n"), 0);

		if (index_found == lt::StdStringA::npos)
		{
			index_found = length;
			break;
		}
		if (index_found > 0)
		{
			auto strSubPath = ssaText.substr(0, (index_found - 1));
			auto test = strSubPath.find(("</"), 0);
			if (test == 0) {
				start_found--;
				ssaTest.erase(start_found);
			}

			ssaText = ssaText.substr((index_found + 1), (length - index));
			auto text = ssaTest + strSubPath;			
			ListBox.InsertString(cnt, lt::ConvertCtoCT(text.c_str()));
			cnt++;

			if ((test == lt::StdStringA::npos)){
				
				ssaTest.insert(start_found, "_");				
				start_found++;
			}
		}
		index = index_found +2;
	}
}
//Request Event
void Cwnd_TestView::OnUIRequestLinkTestEvent(XmlEventArgs & eventArgs)
{
	auto & args = eventArgs.GetArgs();
	auto & reqArgs = args.GetTypedArgs();
	
	GetUIListBox(m_ReportList,
		reqArgs.ReportMsg);
	GetUIListBox(m_RequestList,
		reqArgs.RequestMsg);
}
void Cwnd_TestView::OnUIRequestAlarmListEvent(XmlEventArgs & eventArgs)
{
	auto & args = eventArgs.GetArgs();
	auto & reqArgs = args.GetTypedArgs();

	GetUIListBox(m_ReportList,
		reqArgs.ReportMsg);
	GetUIListBox(m_RequestList,
		reqArgs.RequestMsg);
}
void Cwnd_TestView::OnUIRequestSetDateTimeEvent(XmlEventArgs & eventArgs)
{
	auto & args = eventArgs.GetArgs();
	auto & reqArgs = args.GetTypedArgs();

	GetUIListBox(m_ReportList,
		reqArgs.ReportMsg);
	GetUIListBox(m_RequestList,
		reqArgs.RequestMsg);
}
void Cwnd_TestView::OnUIRequestTerminalMessageEvent(XmlEventArgs & eventArgs)
{
	auto & args = eventArgs.GetArgs();
	auto & reqArgs = args.GetTypedArgs();

	GetUIListBox(m_ReportList,
		reqArgs.ReportMsg);
	GetUIListBox(m_RequestList,
		reqArgs.RequestMsg);
}
void Cwnd_TestView::OnUIRequestOPCALLEvent(XmlEventArgs & eventArgs)
{
	auto & args = eventArgs.GetArgs();
	auto & reqArgs = args.GetTypedArgs();

	GetUIListBox(m_ReportList,
		reqArgs.ReportMsg);
	GetUIListBox(m_RequestList,
		reqArgs.RequestMsg);
}
void Cwnd_TestView::OnUIRequestUnitIdReadEvent(XmlEventArgs & eventArgs)
{
	auto & args = eventArgs.GetArgs();
	auto & reqArgs = args.GetTypedArgs();

	GetUIListBox(m_ReportList,
		reqArgs.ReportMsg);
	GetUIListBox(m_RequestList,
		reqArgs.RequestMsg);
}
//Connection 
void Cwnd_TestView::GetUIRequestLinkTestChange(ST_xml_REQUEST_LINK_TEST & stInData)
{
	SetDlgTxt(IDC_REQUEST_MESSAGENAME, stInData.Hd.messageName);
	SetDlgTxt(IDC_REQUEST_TRANSACTIONID, stInData.Hd.transactionId);
	SetDlgTxt(IDC_REQUEST_TIMESTAMP, stInData.Hd.timeStamp);

	SetDlgTxt(IDC_REQUEST_EQUIPMENTID, stInData.Body.EQUIPMENTID);
	SetDlgTxt(IDC_REQUEST_IPADDRESS, stInData.Body.IPADDRESS);
	SetDlgTxt(IDC_REQUEST_APPLICATIONID, stInData.Body.APPLICATIONID);
	
}

void Cwnd_TestView::SetUIReplyLinkTestChange(ST_xml_REPLY_LINK_TEST & stOutData)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	stOutData.Hd.transactionId = GetDlgTxt(IDC_REPLY_TRANSACTIONID);
	stOutData.Hd.timeStamp = GetDlgTxt(IDC_REPLY_TIMESTAMP);

	stOutData.Body.EQUIPMENTID = GetDlgTxt(IDC_REPLY_EQUIPMENTID);
	stOutData.Body.IPADDRESS = GetDlgTxt(IDC_REPLY_IPADDRESS);
	stOutData.Body.APPLICATIONID = GetDlgTxt(IDC_REPLY_APPLICATIONID);

	stOutData.Rtrn.returnCode = GetDlgTxt(IDC_REPLY_RETURNCODE);
	stOutData.Rtrn.returnMessage = GetDlgTxt(IDC_REPLY_RETURNMESSAGE);
}

//Report Online State.
void Cwnd_TestView::SetUIReportOnlineState(ST_xml_REPORT_ONLINE_STATE & stOutData) 
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	stOutData.Hd.transactionId = GetDlgTxt(IDC_REPORT_TRANSACTIONID);
	stOutData.Hd.timeStamp = GetDlgTxt(IDC_REPORT_TIMESTAMP);

	stOutData.Body.EQUIPMENTID = GetDlgTxt(IDC_REPORT_EQUIPMENTID);
	stOutData.Body.IPADDRESS = GetDlgTxt(IDC_REPORT_IPADDRESS);
	stOutData.Body.USERID = GetDlgTxt(IDC_REPORT_USERID);
	stOutData.Body.EESMODE = GetDlgTxt(IDC_REPORT_EESMODE);

	stOutData.Body.LOTID = GetDlgTxt(IDC_REPORT_LOTID);
	stOutData.Body.RECIPEID = GetDlgTxt(IDC_REPORT_RECIPEID);
	stOutData.Body.PRODUCTID = GetDlgTxt(IDC_REPORT_PRODUCTID);
	stOutData.Body.PRGVER = GetDlgTxt(IDC_REPORT_PRGVER);
	stOutData.Body.ONLINESTATE = GetDlgTxt(IDC_REPORT_ONLINESTATE);
	stOutData.Body.EQUIPMENTSTATE = GetDlgTxt(IDC_REPORT_EQUIPMENTSTATE);	
}

//Report User Change.
void Cwnd_TestView::SetUIReportUserChange(ST_xml_REPORT_USER_CHANGE & stOutData)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	stOutData.Hd.transactionId = GetDlgTxt(IDC_REPORT_TRANSACTIONID);
	stOutData.Hd.timeStamp = GetDlgTxt(IDC_REPORT_TIMESTAMP);

	stOutData.Body.EQUIPMENTID = GetDlgTxt(IDC_REPORT_EQUIPMENTID);
	stOutData.Body.IPADDRESS = GetDlgTxt(IDC_REPORT_IPADDRESS);
	stOutData.Body.USERID = GetDlgTxt(IDC_REPORT_USERID);
	stOutData.Body.EESMODE = GetDlgTxt(IDC_REPORT_EESMODE);	
}
void Cwnd_TestView::GetUIRequestUserCommand(ST_xml_REQUEST_USER_COMMAND & stInData) 
{
	SetDlgTxt(IDC_REQUEST_MESSAGENAME, stInData.Hd.messageName);
	SetDlgTxt(IDC_REQUEST_TRANSACTIONID, stInData.Hd.transactionId);
	SetDlgTxt(IDC_REQUEST_TIMESTAMP, stInData.Hd.timeStamp);

	SetDlgTxt(IDC_REQUEST_EQUIPMENTID, stInData.Body.EQUIPMENTID);
	SetDlgTxt(IDC_REQUEST_IPADDRESS, stInData.Body.IPADDRESS);
	SetDlgTxt(IDC_REQUEST_USERID, stInData.Body.USERID);

	SetDlgTxt(IDC_REQUEST_RETURNCODE, stInData.Rtrn.returnCode);
	SetDlgTxt(IDC_REQUEST_RETURNMESSAGE, stInData.Rtrn.returnMessage);
}
void Cwnd_TestView::SetUIReplyUserCommandChange(ST_xml_REPLY_USER_COMMAND & stOutData)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	stOutData.Hd.transactionId = GetDlgTxt(IDC_REPLY_TRANSACTIONID);
	stOutData.Hd.timeStamp = GetDlgTxt(IDC_REPLY_TIMESTAMP);

	stOutData.Body.EQUIPMENTID = GetDlgTxt(IDC_REPLY_EQUIPMENTID);
	stOutData.Body.IPADDRESS = GetDlgTxt(IDC_REPLY_IPADDRESS);
	stOutData.Body.USERID = GetDlgTxt(IDC_REPLY_USERID);

	stOutData.Rtrn.returnCode = GetDlgTxt(IDC_REPLY_RETURNCODE);
	stOutData.Rtrn.returnMessage = GetDlgTxt(IDC_REPLY_RETURNMESSAGE);
}
//Equipment State Change
void Cwnd_TestView::SetUIReportEquipmentState(ST_xml_REPORT_EQUIPMENT_STATE & stOutData)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	stOutData.Hd.transactionId = GetDlgTxt(IDC_REPORT_TRANSACTIONID);
	stOutData.Hd.timeStamp = GetDlgTxt(IDC_REPORT_TIMESTAMP);

	stOutData.Body.EQUIPMENTID = GetDlgTxt(IDC_REPORT_EQUIPMENTID);
	stOutData.Body.IPADDRESS = GetDlgTxt(IDC_REPORT_IPADDRESS);
	stOutData.Body.USERID = GetDlgTxt(IDC_REPORT_USERID);
	stOutData.Body.EESMODE = GetDlgTxt(IDC_REPORT_EESMODE);
}
void Cwnd_TestView::GetUIRequestEquipmentStateDisplay(ST_xml_REQUEST_EQUIPMENT_STATE_DISPLAY & stInData)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SetDlgTxt(IDC_REQUEST_MESSAGENAME, stInData.Hd.messageName);
	SetDlgTxt(IDC_REQUEST_TRANSACTIONID, stInData.Hd.transactionId);
	SetDlgTxt(IDC_REQUEST_TIMESTAMP, stInData.Hd.timeStamp);

	SetDlgTxt(IDC_REQUEST_EQUIPMENTID, stInData.Body.EQUIPMENTID);
	SetDlgTxt(IDC_REQUEST_SUBEQPID, stInData.Body.SUBEQPID);
	SetDlgTxt(IDC_REQUEST_EQUIPMENTSTATEDISPLAY, stInData.Body.EQUIPMENTSTATEDISPLAY);
	SetDlgTxt(IDC_REQUEST_RGBDISPLAY, stInData.Body.RGBDISPLAY);
	
}
void Cwnd_TestView::SetUIReplyEquipmentState(ST_xml_REPLY_EQUIPMENT_STATE_DISPLAY & stOutData)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	stOutData.Hd.transactionId = GetDlgTxt(IDC_REPLY_TRANSACTIONID);
	stOutData.Hd.timeStamp = GetDlgTxt(IDC_REPLY_TIMESTAMP);

	stOutData.Body.EQUIPMENTID = GetDlgTxt(IDC_REPLY_EQUIPMENTID);
	stOutData.Body.SUBEQPID = GetDlgTxt(IDC_REPLY_SUBEQPID);
	stOutData.Body.EQUIPMENTSTATEDISPLAY = GetDlgTxt(IDC_REPLY_EQUIPMENTSTATEDISPLAY);
	stOutData.Body.RGBDISPLAY = GetDlgTxt(IDC_REPLY_RGBDISPLAY);

	stOutData.Rtrn.returnCode = GetDlgTxt(IDC_REPLY_RETURNCODE);
	stOutData.Rtrn.returnMessage = GetDlgTxt(IDC_REPLY_RETURNMESSAGE);
}
//Loss State Change
void Cwnd_TestView::SetUIReportLossState(ST_xml_REPORT_LOSS_STATE & stOutData)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	stOutData.Hd.transactionId = GetDlgTxt(IDC_REPORT_TRANSACTIONID);
	stOutData.Hd.timeStamp = GetDlgTxt(IDC_REPORT_TIMESTAMP);

	stOutData.Body.EQUIPMENTID = GetDlgTxt(IDC_REPORT_EQUIPMENTID);
	stOutData.Body.SUBEQPID = GetDlgTxt(IDC_REPORT_SUBEQPID);
	stOutData.Body.IPADDRESS = GetDlgTxt(IDC_REPORT_IPADDRESS);
	stOutData.Body.USERID = GetDlgTxt(IDC_REPORT_USERID);
	stOutData.Body.EESMODE = GetDlgTxt(IDC_REPORT_EESMODE);
	stOutData.Body.LOTID = GetDlgTxt(IDC_REPORT_LOTID);
	stOutData.Body.RECIPEID = GetDlgTxt(IDC_REPORT_RECIPEID);
	stOutData.Body.PRODUCTID = GetDlgTxt(IDC_REPORT_PRODUCTID);
	stOutData.Body.PRGVER = GetDlgTxt(IDC_REPORT_PRGVER);
	stOutData.Body.EQUIPMENTSTATE = GetDlgTxt(IDC_REPORT_EQUIPMENTSTATE);
	stOutData.Body.LOSSSTATE = GetDlgTxt(IDC_REPORT_LOSSSTATE);
	stOutData.Body.LOSSCOUNT = GetDlgTxt(IDC_REPORT_LOSSCOUNT);
}
void Cwnd_TestView::GetUIRequestLossWindow(ST_xml_REQUEST_LOSS_WINDOW & stInData)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SetDlgTxt(IDC_REQUEST_MESSAGENAME, stInData.Hd.messageName);
	SetDlgTxt(IDC_REQUEST_TRANSACTIONID, stInData.Hd.transactionId);
	SetDlgTxt(IDC_REQUEST_TIMESTAMP, stInData.Hd.timeStamp);

	SetDlgTxt(IDC_REQUEST_EQUIPMENTID, stInData.Body.EQUIPMENTID);
	SetDlgTxt(IDC_REQUEST_SUBEQPID, stInData.Body.SUBEQPID);
	SetDlgTxt(IDC_REQUEST_EQUIPMENTSTATEDISPLAY, stInData.Body.IPADDRESS);
	SetDlgTxt(IDC_REQUEST_LOSSWINDOW, stInData.Body.LOSSWINDOW);
	
}
void Cwnd_TestView::SetUIReplyLossWindow(ST_xml_REPLY_LOSS_WINDOW & stOutData)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	stOutData.Hd.transactionId = GetDlgTxt(IDC_REPLY_TRANSACTIONID);
	stOutData.Hd.timeStamp = GetDlgTxt(IDC_REPLY_TIMESTAMP);

	stOutData.Body.EQUIPMENTID = GetDlgTxt(IDC_REPLY_EQUIPMENTID);
	stOutData.Body.SUBEQPID = GetDlgTxt(IDC_REPLY_SUBEQPID);
	stOutData.Body.IPADDRESS = GetDlgTxt(IDC_REPLY_IPADDRESS);
	stOutData.Body.LOSSWINDOW = GetDlgTxt(IDC_REPLY_LOSSWINDOW);

	stOutData.Rtrn.returnCode = GetDlgTxt(IDC_REPLY_RETURNCODE);
	stOutData.Rtrn.returnMessage = GetDlgTxt(IDC_REPLY_RETURNMESSAGE);
}
//Alarm State Change
void Cwnd_TestView::SetUIReportAlarmState(ST_xml_REPORT_ALARM_STATE & stOutData)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	stOutData.Hd.transactionId = GetDlgTxt(IDC_REPORT_TRANSACTIONID);
	stOutData.Hd.timeStamp = GetDlgTxt(IDC_REPORT_TIMESTAMP);

	stOutData.Body.EQUIPMENTID = GetDlgTxt(IDC_REPORT_EQUIPMENTID);
	stOutData.Body.SUBEQPID = GetDlgTxt(IDC_REPORT_SUBEQPID);
	stOutData.Body.IPADDRESS = GetDlgTxt(IDC_REPORT_IPADDRESS);
	stOutData.Body.USERID = GetDlgTxt(IDC_REPORT_USERID);
	stOutData.Body.EESMODE = GetDlgTxt(IDC_REPORT_EESMODE);
	stOutData.Body.LOTID = GetDlgTxt(IDC_REPORT_LOTID);
	stOutData.Body.RECIPEID = GetDlgTxt(IDC_REPORT_RECIPEID);
	stOutData.Body.PRODUCTID = GetDlgTxt(IDC_REPORT_PRODUCTID);
	stOutData.Body.PRGVER = GetDlgTxt(IDC_REPORT_PRGVER);
	stOutData.Body.ALARMCODE = GetDlgTxt(IDC_REPORT_ALARMCODE);
	stOutData.Body.ALARMID = GetDlgTxt(IDC_REPORT_ALARMID);
	stOutData.Body.ALARMMESSAGE = GetDlgTxt(IDC_REPORT_ALARMMESSAGE);
}

void Cwnd_TestView::GetUIRequestAlarmWindow(ST_xml_REQUEST_ALARM_LIST & stInData)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SetDlgTxt(IDC_REQUEST_MESSAGENAME, stInData.Hd.messageName);
	SetDlgTxt(IDC_REQUEST_TRANSACTIONID, stInData.Hd.transactionId);
	SetDlgTxt(IDC_REQUEST_TIMESTAMP, stInData.Hd.timeStamp);

	SetDlgTxt(IDC_REQUEST_EQUIPMENTID, stInData.Body.EQUIPMENTID);
	SetDlgTxt(IDC_REQUEST_SUBEQPID, stInData.Body.SUBEQPID);
	SetDlgTxt(IDC_REQUEST_EQUIPMENTSTATEDISPLAY, stInData.Body.IPADDRESS);
}
void Cwnd_TestView::SetUIReplyAlarmList(ST_xml_REPLY_ALARM_LIST & stOutData)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	stOutData.Hd.transactionId = GetDlgTxt(IDC_REPLY_TRANSACTIONID);
	stOutData.Hd.timeStamp = GetDlgTxt(IDC_REPLY_TIMESTAMP);

	stOutData.Body.EQUIPMENTID = GetDlgTxt(IDC_REPLY_EQUIPMENTID);
	stOutData.Body.SUBEQPID = GetDlgTxt(IDC_REPLY_SUBEQPID);
	stOutData.Body.IPADDRESS = GetDlgTxt(IDC_REPLY_EQUIPMENTSTATEDISPLAY);

	stOutData.Rtrn.returnCode = GetDlgTxt(IDC_REPLY_RETURNCODE);
	stOutData.Rtrn.returnMessage = GetDlgTxt(IDC_REPLY_RETURNMESSAGE);
}
void Cwnd_TestView::OnBnClickedButtonSaveAlarmlist()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.	
	//m_pIcsCommunicator[ICS_SERVER_EES]->SaveAlarmList();
}
void Cwnd_TestView::OnBnClickedButtonSaveUnitidRead()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//가능성 없음.
	//m_pIcsCommunicator[ICS_SERVER_EES]->SaveUnitIdRead();
	pwndTestView->GetUIListBox(pwndTestView->m_FileData,
		GetXmlEes().SaveTestUnitReadList());
	pwndTestView->GetUIListBox(pwndTestView->m_ReportList2,
		GetXmlEes().MakeXML_UNITID_READ(&GetXmlEes().xmlUnitRead));	
}

void Cwnd_TestView::OnBnClickedButtonReplyUnitidList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	pwndTestView->GetUIListBox(pwndTestView->m_FileData,
		GetXmlEes().SaveTestReplyUnitReadList());
	pwndTestView->GetUIListBox(pwndTestView->m_ReplyList2,
		GetXmlEes().MakeXML_REPLY_UNITID_READ(&GetXmlEes().xmlReplyUnitIdRead));
}


//2023.02.01a uhkim
//Report Rms Mode.
void Cwnd_TestView::SetUIReportRmsMode(ST_xml_REPORT_RMS_MODE & stOutData)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	stOutData.Hd.transactionId = GetDlgTxt(IDC_REPORT_TRANSACTIONID);
	stOutData.Hd.timeStamp = GetDlgTxt(IDC_REPORT_TIMESTAMP);

	stOutData.Body.EQUIPMENTID = GetDlgTxt(IDC_REPORT_EQUIPMENTID);
	stOutData.Body.SUBEQPID = GetDlgTxt(IDC_REPORT_SUBEQPID);
	stOutData.Body.IPADDRESS = GetDlgTxt(IDC_REPORT_IPADDRESS);
	stOutData.Body.USERID = GetDlgTxt(IDC_REPORT_USERID);
	stOutData.Body.EESMODE = GetDlgTxt(IDC_REPORT_EESMODE);
	stOutData.Body.LOTID = GetDlgTxt(IDC_REPORT_LOTID);
	stOutData.Body.RECIPEID = GetDlgTxt(IDC_REPORT_RECIPEID);
	stOutData.Body.PRODUCTID = GetDlgTxt(IDC_REPORT_PRODUCTID);
	stOutData.Body.PRGVER = GetDlgTxt(IDC_REPORT_PRGVER);
}

void Cwnd_TestView::SetUIUnitIdRead(ST_xml_UNITID_READ & stOutData)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	stOutData.Hd.transactionId = GetDlgTxt(IDC_REPORT_TRANSACTIONID);
	stOutData.Hd.timeStamp = GetDlgTxt(IDC_REPORT_TIMESTAMP);

	stOutData.Body.EQUIPMENTID = GetDlgTxt(IDC_REPORT_EQUIPMENTID);
	stOutData.Body.IPADDRESS = GetDlgTxt(IDC_REPORT_IPADDRESS);
	stOutData.Body.EQPTYPECODE = GetDlgTxt(IDC_REPORT_EQPTYPECODE);
	stOutData.Body.LOTID = GetDlgTxt(IDC_REPORT_LOTID);
	stOutData.Body.PRODUCTID = GetDlgTxt(IDC_REPORT_PRODUCTID);
	stOutData.Body.USERID = GetDlgTxt(IDC_REPORT_USERID);
	stOutData.Body.PORTID = GetDlgTxt(IDC_REPORT_PORTID);
	stOutData.Body.STEPID = GetDlgTxt(IDC_REPORT_STEPID);
	stOutData.Body.RECIPEID = GetDlgTxt(IDC_REPORT_RECIPEID);
	stOutData.Body.EESMODE = GetDlgTxt(IDC_REPORT_EESMODE);

	stOutData.Body.SWVERSION = GetDlgTxt(IDC_REPORT_SWVERSION);
	stOutData.Body.SWHASHCODE = GetDlgTxt(IDC_REPORT_SWHASHCODE);
	stOutData.Body.SWMODULETYPE = GetDlgTxt(IDC_REPORT_SWMODULETYPE);

	stOutData.Body.CARRIERID = GetDlgTxt(IDC_REPORT_CARRIERID);
	stOutData.Body.CARRTYPE = GetDlgTxt(IDC_REPORT_CARRTYPE);
	stOutData.Body.RETRY = GetDlgTxt(IDC_REPORT_RETRY);
	stOutData.Body.CONSUMLIST = GetDlgTxt(IDC_REPORT_CONSUMLIST);
}
void Cwnd_TestView::GetUIRequestUnitIdRead(ST_xml_REQUEST_UNITID_READ & stInData)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SetDlgTxt(IDC_REQUEST_MESSAGENAME, stInData.Hd.messageName);
	SetDlgTxt(IDC_REQUEST_TRANSACTIONID, stInData.Hd.transactionId);
	SetDlgTxt(IDC_REQUEST_TIMESTAMP, stInData.Hd.timeStamp);

	SetDlgTxt(IDC_REQUEST_EQUIPMENTID, stInData.Body.EQUIPMENTID);
	SetDlgTxt(IDC_REQUEST_IPADDRESS, stInData.Body.IPADDRESS);
	SetDlgTxt(IDC_REQUEST_LOTID, stInData.Body.LOTID);
	SetDlgTxt(IDC_REQUEST_PRODUCTID, stInData.Body.PRODUCTID);
	SetDlgTxt(IDC_REQUEST_USERID, stInData.Body.USERID);
	SetDlgTxt(IDC_REQUEST_PORTID, stInData.Body.PORTID);
	SetDlgTxt(IDC_REQUEST_STEPID, stInData.Body.STEPID);
	//SetDlgTxt(IDC_REQUEST_CONSUMLIST, stInData.Body.CONSUMLIST);

	SetDlgTxt(IDC_REQUEST_RETURNCODE, stInData.Rtrn.returnCode);
	SetDlgTxt(IDC_REQUEST_RETURNMESSAGE, stInData.Rtrn.returnMessage);
}
void Cwnd_TestView::SetUIReplyUnitIdRead(ST_xml_REPLY_UNITID_READ & stOutData)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	stOutData.Hd.transactionId = GetDlgTxt(IDC_REPLY_TRANSACTIONID);
	stOutData.Hd.timeStamp = GetDlgTxt(IDC_REPLY_TIMESTAMP);

	stOutData.Body.EQUIPMENTID = GetDlgTxt(IDC_REPLY_EQUIPMENTID);
	stOutData.Body.IPADDRESS = GetDlgTxt(IDC_REPLY_IPADDRESS);
	stOutData.Body.USERID = GetDlgTxt(IDC_REPLY_USERID);
	stOutData.Body.PORTID = GetDlgTxt(IDC_REPLY_PORTID);
	stOutData.Body.LOTID = GetDlgTxt(IDC_REPLY_LOTID);
	stOutData.Body.PRODUCTID = GetDlgTxt(IDC_REPLY_PRODUCTID);
	//stOutData.Body.CONSUMLIST = GetDlgTxt(IDC_REPLY_CONSUMLIST);
}

//2023.02.01a uhkim
//Report Set DateTime.
void Cwnd_TestView::GetUIRequestSetDateTime(ST_xml_REQUEST_SET_DATETIME & stInData)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SetDlgTxt(IDC_REQUEST_MESSAGENAME, stInData.Hd.messageName);
	SetDlgTxt(IDC_REQUEST_TRANSACTIONID, stInData.Hd.transactionId);
	SetDlgTxt(IDC_REQUEST_TIMESTAMP, stInData.Hd.timeStamp);

	SetDlgTxt(IDC_REQUEST_EQUIPMENTID, stInData.Body.EQUIPMENTID);
	SetDlgTxt(IDC_REQUEST_IPADDRESS, stInData.Body.IPADDRESS);
	SetDlgTxt(IDC_REQUEST_DATETIME, stInData.Body.DATETIME);
}
void Cwnd_TestView::SetUIReplySetDateTime(ST_xml_REPLY_SET_DATETIME & stOutData)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	stOutData.Hd.transactionId = GetDlgTxt(IDC_REPLY_TRANSACTIONID);
	stOutData.Hd.timeStamp = GetDlgTxt(IDC_REPLY_TIMESTAMP);

	stOutData.Body.EQUIPMENTID = GetDlgTxt(IDC_REPLY_EQUIPMENTID);
	stOutData.Body.IPADDRESS = GetDlgTxt(IDC_REPLY_IPADDRESS);
	stOutData.Body.DATETIME = GetDlgTxt(IDC_REPLY_DATETIME);

	stOutData.Rtrn.returnCode = GetDlgTxt(IDC_REPLY_RETURNCODE);
	stOutData.Rtrn.returnMessage = GetDlgTxt(IDC_REPLY_RETURNMESSAGE);
}

void Cwnd_TestView::GetUIRequestTerminalMessage(ST_xml_REQUEST_TERMINAL_MESSAGE & stInData)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SetDlgTxt(IDC_REQUEST_MESSAGENAME, stInData.Hd.messageName);
	SetDlgTxt(IDC_REQUEST_TRANSACTIONID, stInData.Hd.transactionId);
	SetDlgTxt(IDC_REQUEST_TIMESTAMP, stInData.Hd.timeStamp);

	SetDlgTxt(IDC_REQUEST_EQUIPMENTID, stInData.Body.EQUIPMENTID);
	SetDlgTxt(IDC_REQUEST_SUBEQPID, stInData.Body.SUBEQPID);
	SetDlgTxt(IDC_REQUEST_IPADDRESS, stInData.Body.IPADDRESS);
	SetDlgTxt(IDC_REQUEST_TERMINALMESSAGE, stInData.Body.TERMINALMESSAGE);
}
void Cwnd_TestView::SetUIReplyTerminalMessage(ST_xml_REPLY_TERMINAL_MESSAGE & stOutData)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	stOutData.Hd.transactionId = GetDlgTxt(IDC_REPLY_TRANSACTIONID);
	stOutData.Hd.timeStamp = GetDlgTxt(IDC_REPLY_TIMESTAMP);

	stOutData.Body.EQUIPMENTID = GetDlgTxt(IDC_REPLY_EQUIPMENTID);
	stOutData.Body.SUBEQPID = GetDlgTxt(IDC_REPLY_SUBEQPID);
	stOutData.Body.IPADDRESS = GetDlgTxt(IDC_REPLY_IPADDRESS);
	stOutData.Body.TERMINALMESSAGE = GetDlgTxt(IDC_REPLY_TERMINALMESSAGE);

	stOutData.Rtrn.returnCode = GetDlgTxt(IDC_REPLY_RETURNCODE);
	stOutData.Rtrn.returnMessage = GetDlgTxt(IDC_REPLY_RETURNMESSAGE);
}
void Cwnd_TestView::GetUIRequestOpCall(ST_xml_REQUEST_OPCALL & stInData)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SetDlgTxt(IDC_REQUEST_MESSAGENAME, stInData.Hd.messageName);
	SetDlgTxt(IDC_REQUEST_TRANSACTIONID, stInData.Hd.transactionId);
	SetDlgTxt(IDC_REQUEST_TIMESTAMP, stInData.Hd.timeStamp);

	SetDlgTxt(IDC_REQUEST_EQUIPMENTID, stInData.Body.EQUIPMENTID);
	SetDlgTxt(IDC_REQUEST_SUBEQPID, stInData.Body.SUBEQPID);
	SetDlgTxt(IDC_REQUEST_IPADDRESS, stInData.Body.IPADDRESS);
	SetDlgTxt(IDC_REQUEST_MESSAGE, stInData.Body.MESSAGE);
	SetDlgTxt(IDC_REQUEST_BUZZER, stInData.Body.BUZZER);
	SetDlgTxt(IDC_REQUEST_TOWERLAMP, stInData.Body.TOWERLAMP);
}
void Cwnd_TestView::SetUIReplyOpCall(ST_xml_REPLY_OPCALL & stOutData)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	stOutData.Hd.transactionId = GetDlgTxt(IDC_REPLY_TRANSACTIONID);
	stOutData.Hd.timeStamp = GetDlgTxt(IDC_REPLY_TIMESTAMP);

	stOutData.Body.EQUIPMENTID = GetDlgTxt(IDC_REPLY_EQUIPMENTID);
	stOutData.Body.SUBEQPID = GetDlgTxt(IDC_REPLY_SUBEQPID);
	stOutData.Body.IPADDRESS = GetDlgTxt(IDC_REPLY_IPADDRESS);
	stOutData.Body.MESSAGE = GetDlgTxt(IDC_REPLY_MESSAGE);
	stOutData.Body.BUZZER = GetDlgTxt(IDC_REPLY_BUZZER);
	stOutData.Body.TOWERLAMP = GetDlgTxt(IDC_REPLY_TOWERLAMP);

	stOutData.Rtrn.returnCode = GetDlgTxt(IDC_REPLY_RETURNCODE);
	stOutData.Rtrn.returnMessage = GetDlgTxt(IDC_REPLY_RETURNMESSAGE);
}
void Cwnd_TestView::SetUIReportStartLot(ST_xml_REPORT_START_LOT & stOutData)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	stOutData.Hd.transactionId = GetDlgTxt(IDC_REPORT_TRANSACTIONID);
	stOutData.Hd.timeStamp = GetDlgTxt(IDC_REPORT_TIMESTAMP);

	stOutData.Body.EQUIPMENTID = GetDlgTxt(IDC_REPORT_EQUIPMENTID);
	stOutData.Body.SUBEQPID = GetDlgTxt(IDC_REPORT_SUBEQPID);
	stOutData.Body.IPADDRESS = GetDlgTxt(IDC_REPORT_IPADDRESS);
	stOutData.Body.USERID = GetDlgTxt(IDC_REPORT_USERID);
	stOutData.Body.EESMODE = GetDlgTxt(IDC_REPORT_EESMODE);
	stOutData.Body.PORTID = GetDlgTxt(IDC_REPORT_PORTID);
	stOutData.Body.CARRIERID = GetDlgTxt(IDC_REPORT_CARRIERID);
	stOutData.Body.CARRTYPE = GetDlgTxt(IDC_REPORT_CARRTYPE);
	stOutData.Body.OUTCARRIERID = GetDlgTxt(IDC_REPORT_OUTCARRIERID);
	stOutData.Body.OUTCARRTYPE = GetDlgTxt(IDC_REPORT_OUTCARRTYPE);
	stOutData.Body.OUTCARRPORTID = GetDlgTxt(IDC_REPORT_OUTCARRPORTID);
	stOutData.Body.RECIPEID = GetDlgTxt(IDC_REPORT_RECIPEID);
	stOutData.Body.PRODUCTID = GetDlgTxt(IDC_REPORT_PRODUCTID);
	stOutData.Body.PRGVER = GetDlgTxt(IDC_REPORT_PRGVER);
}
void Cwnd_TestView::SetUIReportEndEvent(ST_xml_REPORT_END_EVENT & stOutData)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	stOutData.Hd.transactionId = GetDlgTxt(IDC_REPORT_TRANSACTIONID);
	stOutData.Hd.timeStamp = GetDlgTxt(IDC_REPORT_TIMESTAMP);

	stOutData.Body.EQUIPMENTID = GetDlgTxt(IDC_REPORT_EQUIPMENTID);
	stOutData.Body.SUBEQPID = GetDlgTxt(IDC_REPORT_SUBEQPID);
	stOutData.Body.IPADDRESS = GetDlgTxt(IDC_REPORT_IPADDRESS);
	stOutData.Body.USERID = GetDlgTxt(IDC_REPORT_USERID);
	stOutData.Body.EESMODE = GetDlgTxt(IDC_REPORT_EESMODE);
	stOutData.Body.PORTID = GetDlgTxt(IDC_REPORT_PORTID);
	stOutData.Body.CARRIERID = GetDlgTxt(IDC_REPORT_CARRIERID);
	stOutData.Body.CARRTYPE = GetDlgTxt(IDC_REPORT_CARRTYPE);
	stOutData.Body.OUTCARRIERID = GetDlgTxt(IDC_REPORT_OUTCARRIERID);
	stOutData.Body.OUTCARRTYPE = GetDlgTxt(IDC_REPORT_OUTCARRTYPE);
	stOutData.Body.OUTCARRPORTID = GetDlgTxt(IDC_REPORT_OUTCARRPORTID);
	stOutData.Body.RECIPEID = GetDlgTxt(IDC_REPORT_RECIPEID);
	stOutData.Body.PRODUCTID = GetDlgTxt(IDC_REPORT_PRODUCTID);
	stOutData.Body.PRGVER = GetDlgTxt(IDC_REPORT_PRGVER);
	stOutData.Body.RETRY = GetDlgTxt(IDC_REPORT_RETRY);
}
void Cwnd_TestView::SetUIReportEndProcess(ST_xml_REPORT_END_PROCESS & stOutData)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	stOutData.Hd.transactionId = GetDlgTxt(IDC_REPORT_TRANSACTIONID);
	stOutData.Hd.timeStamp = GetDlgTxt(IDC_REPORT_TIMESTAMP);

	stOutData.Body.EQUIPMENTID = GetDlgTxt(IDC_REPORT_EQUIPMENTID);
	stOutData.Body.SUBEQPID = GetDlgTxt(IDC_REPORT_SUBEQPID);
	stOutData.Body.IPADDRESS = GetDlgTxt(IDC_REPORT_IPADDRESS);
	stOutData.Body.USERID = GetDlgTxt(IDC_REPORT_USERID);
	stOutData.Body.EESMODE = GetDlgTxt(IDC_REPORT_EESMODE);
	stOutData.Body.PORTID = GetDlgTxt(IDC_REPORT_PORTID);
	stOutData.Body.CARRIERID = GetDlgTxt(IDC_REPORT_CARRIERID);
	stOutData.Body.CARRTYPE = GetDlgTxt(IDC_REPORT_CARRTYPE);
	stOutData.Body.OUTCARRIERID = GetDlgTxt(IDC_REPORT_OUTCARRIERID);
	stOutData.Body.OUTCARRTYPE = GetDlgTxt(IDC_REPORT_OUTCARRTYPE);
	stOutData.Body.OUTCARRPORTID = GetDlgTxt(IDC_REPORT_OUTCARRPORTID);
	stOutData.Body.RECIPEID = GetDlgTxt(IDC_REPORT_RECIPEID);
	stOutData.Body.PRODUCTID = GetDlgTxt(IDC_REPORT_PRODUCTID);
	stOutData.Body.PRGVER = GetDlgTxt(IDC_REPORT_PRGVER);
}
void Cwnd_TestView::SetUIReportStartProcess(ST_xml_REPORT_START_PROCESS & stOutData)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	stOutData.Hd.transactionId = GetDlgTxt(IDC_REPORT_TRANSACTIONID);
	stOutData.Hd.timeStamp = GetDlgTxt(IDC_REPORT_TIMESTAMP);

	stOutData.Body.EQUIPMENTID = GetDlgTxt(IDC_REPORT_EQUIPMENTID);
	stOutData.Body.SUBEQPID = GetDlgTxt(IDC_REPORT_SUBEQPID);
	stOutData.Body.IPADDRESS = GetDlgTxt(IDC_REPORT_IPADDRESS);
	stOutData.Body.USERID = GetDlgTxt(IDC_REPORT_USERID);
	stOutData.Body.EESMODE = GetDlgTxt(IDC_REPORT_EESMODE);
	stOutData.Body.PORTID = GetDlgTxt(IDC_REPORT_PORTID);
	stOutData.Body.CARRIERID = GetDlgTxt(IDC_REPORT_CARRIERID);
	stOutData.Body.CARRTYPE = GetDlgTxt(IDC_REPORT_CARRTYPE);
	stOutData.Body.OUTCARRIERID = GetDlgTxt(IDC_REPORT_OUTCARRIERID);
	stOutData.Body.OUTCARRTYPE = GetDlgTxt(IDC_REPORT_OUTCARRTYPE);
	stOutData.Body.OUTCARRPORTID = GetDlgTxt(IDC_REPORT_OUTCARRPORTID);
	stOutData.Body.RECIPEID = GetDlgTxt(IDC_REPORT_RECIPEID);
	stOutData.Body.PRODUCTID = GetDlgTxt(IDC_REPORT_PRODUCTID);
	stOutData.Body.PRGVER = GetDlgTxt(IDC_REPORT_PRGVER);
}

void Cwnd_TestView::OnBnClickedButtonReportDataclear()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ClearData(UI_Select_Report);
}

void Cwnd_TestView::OnBnClickedButtonRequestDataclear()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ClearData(UI_Select_Request);
}

void Cwnd_TestView::OnBnClickedButtonReplyDataclear()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ClearData(UI_Select_Reply);
}