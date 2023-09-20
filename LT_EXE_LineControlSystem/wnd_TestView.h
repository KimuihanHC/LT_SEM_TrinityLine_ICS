#ifndef wnd_TestView_h__
#define wnd_TestView_h__

#pragma once

#include "IcsCommunicator.h"
#include "IcsRemoteServerEes.h"

class CSvrEes;
// Cwnd_TestView 폼 보기
typedef enum
{
	Report_Online_State,
	Report_User_Change,
	Report_Equipment_State,
	Report_Loss_State,
	Report_Alarm_State,
	Report_Rms_Mode,
	Unit_Read,
	Report_Start_Lot,
	Report_End_Event,
	Report_End_Process,
	Report_Start_Process,
	Report_Max
};
static LPCTSTR g_szReportMessage[] =
{
	_T("REPORT_OLINE_STATE"),
	_T("REPORT_USER_CHANGE"),
	_T("REPORT_EQUIPMENT_STATE"),
	_T("REPORT_LOSS_STATE"),	
	_T("REPORT_ALARM_STATE"),
	_T("REPORT_RMS_MODE"),
	_T("UNITED_READ"),
	_T("REPORT_START_LOT"),
	_T("REPORT_END_EVENT"),
	_T("REPORT_END_PROCESS"),
	_T("REPORT_START_PROCESS"),
	nullptr
};

typedef enum
{
	//Reply_Link_Test,
	Reply_User_Command,
	Reply_Equipment_State_Display,
	Reply_Loss_State,
	//Reply_Alarm_State,
	//Reply_Set_DateTime,
	//Reply_Terminal_Message,
	Reply_OpCall,
	Reply_Unitid_Read,
	Reply_Max
};
static LPCTSTR g_szReplyMessage[] =
{
	//_T("REPLY_LINK_TEST"),
	_T("REPLY_USER_COMMAND"),
	_T("REPLY_EQUIPMENT_STATE_DISPLAY"),
	_T("REPLY_LOSS_STATE"),
	//_T("REPLY_ALARM_STATE"),
	//_T("REPLY_SET_DATETIME"),
	//_T("REPLY_TERMINAL_MESSAGER"),
	_T("REPLY_OPCALL"),
	_T("REPLY_UNITID_READ"),
	nullptr
};
typedef enum
{
	UI_Select_Report,
	UI_Select_Request,
	UI_Select_Reply,
	UI_Select_Max
};
static LPCTSTR g_szUI_Select[] =
{
	_T("REPORT"),
	_T("REQUEST"),
	_T("REPLY"),
	nullptr
};
namespace lt
{
	class CIcsRemoteEes;
}
class Cwnd_TestView : public CFormView
{
	DECLARE_DYNCREATE(Cwnd_TestView)
//protected:
public:
	Cwnd_TestView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~Cwnd_TestView();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TEST_FORM };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.	

	//-----------------------------------------------------
	// 주변 장치 제어용 클래스 모음
	//-----------------------------------------------------
	CIcsCommunicator *	m_pIcsComm;
	CIcsRemoteServer *  m_pIcsRemoteSvrEes;
private:
	//2023.02.15a uhkim UI 이벤트.
	using XmlEventArgs =
		lt::XmlEventArgs<CIcsRemoteServerEes>;
	// Remote
	using EesUIRequestReplyEventListener =
		lt::CEventMfnListener<Cwnd_TestView, XmlEventArgs>;
	EesUIRequestReplyEventListener * m_pUIRequestLinkTestEventListener = nullptr;
	EesUIRequestReplyEventListener * m_pUIRequestAlarmListEventListener = nullptr;
	EesUIRequestReplyEventListener * m_pUIRequestSetDateTimeEventListener = nullptr;
	EesUIRequestReplyEventListener * m_pUIRequestTerminalMessageEventListener = nullptr;
	EesUIRequestReplyEventListener * m_pUIRequestOPCALLEventListener = nullptr;
	EesUIRequestReplyEventListener * m_pUIRequestUnitIdReadEventListener = nullptr;

	CIcsRemoteServer & GetRemoteEqp();
	CIcsXmlBase & GetXmlEes();

	void OnUIRequestLinkTestEvent(XmlEventArgs & eventArgs);
	void OnUIRequestAlarmListEvent(XmlEventArgs & eventArgs);
	void OnUIRequestSetDateTimeEvent(XmlEventArgs & eventArgs);
	void OnUIRequestTerminalMessageEvent(XmlEventArgs & eventArgs);
	void OnUIRequestOPCALLEvent(XmlEventArgs & eventArgs);
	void OnUIRequestUnitIdReadEvent(XmlEventArgs & eventArgs);

public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	afx_msg void OnClose();
//	virtual BOOL CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, LPVOID lpParam = NULL);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void OnInitialUpdate();
	afx_msg void OnBnClickedButtonReport();
	CListBox m_ReportList;



	void SetDlgTxt(int ID, CStringA text);
	CStringA GetDlgTxt(int ID);
	void ClearData(int nIndex);
	void GetUIListBox(CListBox & ListBox,
		lt::ConstStringT stInData);

	void CreateUIRequestEventListener();
	void CloseUIRequestEventListener();

	//Connection.
	void GetUIRequestLinkTestChange(ST_xml_REQUEST_LINK_TEST & stInData);
	void SetUIReplyLinkTestChange(ST_xml_REPLY_LINK_TEST & stInData);
	//Online State.
	void SetUIReportOnlineState(ST_xml_REPORT_ONLINE_STATE & stOutData);
	//User change.
	void SetUIReportUserChange(ST_xml_REPORT_USER_CHANGE & stOutData);
	void GetUIRequestUserCommand(ST_xml_REQUEST_USER_COMMAND & stInData);
	void SetUIReplyUserCommandChange(ST_xml_REPLY_USER_COMMAND & stOutData);
	//Equipment State Change.
	void SetUIReportEquipmentState(ST_xml_REPORT_EQUIPMENT_STATE & stOutData);
	void GetUIRequestEquipmentStateDisplay(ST_xml_REQUEST_EQUIPMENT_STATE_DISPLAY & stInData);
	void SetUIReplyEquipmentState(ST_xml_REPLY_EQUIPMENT_STATE_DISPLAY & stOutData);
	//Loss State Change
	void SetUIReportLossState(ST_xml_REPORT_LOSS_STATE & stOutData);
	void GetUIRequestLossWindow(ST_xml_REQUEST_LOSS_WINDOW & stInData);
	void SetUIReplyLossWindow(ST_xml_REPLY_LOSS_WINDOW & stOutData);
	//Alarm State Change
	void SetUIReportAlarmState(ST_xml_REPORT_ALARM_STATE & stOutData);
	void GetUIRequestAlarmWindow(ST_xml_REQUEST_ALARM_LIST & stInData);
	void SetUIReplyAlarmList(ST_xml_REPLY_ALARM_LIST & stOutData);
	afx_msg void OnBnClickedButtonSaveAlarmlist();
	CListBox m_RequestList;


	//2023.02.01a uhkim
	//Report Rms Mode.
	void SetUIReportRmsMode(ST_xml_REPORT_RMS_MODE & stOutData);
	//Request Set DateTime.
	void GetUIRequestSetDateTime(ST_xml_REQUEST_SET_DATETIME & stInData);
	void SetUIReplySetDateTime(ST_xml_REPLY_SET_DATETIME & stOutData);
	//Request Terminal Message.
	void GetUIRequestTerminalMessage(ST_xml_REQUEST_TERMINAL_MESSAGE & stInData);
	void SetUIReplyTerminalMessage(ST_xml_REPLY_TERMINAL_MESSAGE & stOutData);
	//Request Op Call
	void GetUIRequestOpCall(ST_xml_REQUEST_OPCALL & stInData);
	void SetUIReplyOpCall(ST_xml_REPLY_OPCALL & stOutData);
	//Unit ID Read
	void SetUIUnitIdRead(ST_xml_UNITID_READ & stOutData);
	void GetUIRequestUnitIdRead(ST_xml_REQUEST_UNITID_READ & stInData);
	void SetUIReplyUnitIdRead(ST_xml_REPLY_UNITID_READ & stOutData);
	//Report Lot
	void SetUIReportStartLot(ST_xml_REPORT_START_LOT & stOutData);
	void SetUIReportEndEvent(ST_xml_REPORT_END_EVENT & stOutData);
	void SetUIReportEndProcess(ST_xml_REPORT_END_PROCESS & stOutData);
	void SetUIReportStartProcess(ST_xml_REPORT_START_PROCESS & stOutData);
	
	
	afx_msg void OnBnClickedButtonReply();
	CListBox m_ReplyList;
	afx_msg void OnBnClickedButtonSaveUnitidRead();
	CListBox m_FileData;
	CListBox m_ReportList2;
	afx_msg void OnBnClickedButtonReplyUnitidList();
	CListBox m_ReplyList2;
	afx_msg void OnBnClickedButtonDataClear();
	afx_msg void OnBnClickedButtonReportDataclear();
	afx_msg void OnBnClickedButtonReplyDataclear();
	afx_msg void OnBnClickedButtonRequestDataclear();
};
extern Cwnd_TestView* pwndTestView;

#endif wnd_TestView_h__

