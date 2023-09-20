//*****************************************************************************
// Filename	: 	Dlg_OpCall_Popup.h
// Created	:	
// Modified	:	2023.04.20 - 15:00
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
// Dlg_OpCall_Popup.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Dlg_OpCall_Popup.h"
#include "afxdialogex.h"


#define		IDC_BN_CTRL_FST		1001

// CDlg_OpCall_Popup 대화 상자입니다.
IMPLEMENT_DYNAMIC(CDlg_OpCall_Popup, CDialogEx)

CDlg_OpCall_Popup::CDlg_OpCall_Popup(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlg_OpCall_Popup::IDD, pParent)
{

	VERIFY(m_font_Default.CreateFont(
		20,						// nHeight
		0,						// nWidth
		0,						// nEscapement
		0,						// nOrientation
		FW_HEAVY,				// nWeight
		FALSE,					// bItalic
		FALSE,					// bUnderline
		0,						// cStrikeOut
		ANSI_CHARSET,			// nCharSet
		OUT_DEFAULT_PRECIS,		// nOutPrecision
		CLIP_DEFAULT_PRECIS,	// nClipPrecision
		ANTIALIASED_QUALITY,	// nQuality
		DEFAULT_PITCH,			// nPitchAndFamily
		_T("arial")));			// lpszFacename
}

CDlg_OpCall_Popup::~CDlg_OpCall_Popup()
{
	// 테마 비활성화가 다른 윈도우에 영향을 주어서 소멸자에서 테마 원복 시킴
	m_font_Default.DeleteObject();
}

void CDlg_OpCall_Popup::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlg_OpCall_Popup, CDialogEx)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDlg_PopupMenu 메시지 처리기입니다.
//=============================================================================
// Method		: OnCreate
// Access		: public  
// Returns		: int
// Parameter	: LPCREATESTRUCT lpCreateStruct
// Qualifier	:
// Last Update	: 2023.04.20 - 15:00
// Desc.		:
//=============================================================================
int CDlg_OpCall_Popup::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	CRect rectDummy;
	DWORD dwStyle = WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	rectDummy.SetRectEmpty();

	m_st_Title.SetBackColor_COLORREF(RGB(0, 0, 0));
	m_st_Title.SetTextColor(Color::White, Color::White);
	m_st_Title.SetFont_Gdip(L"Arial", 24.0F);
	m_st_Title.Create(_T("OP CALL"), dwStyle | SS_CENTER | SS_CENTERIMAGE, rectDummy, this, IDC_STATIC);

	m_st_IP_Adress.SetFont_Gdip(L"Arial", 8.0F);
	m_st_IP_Adress.Create(m_szIP_Adress, dwStyle /*| WS_BORDER*/ | SS_CENTER, rectDummy, this, IDC_STATIC);

	m_st_BuzzerName.SetFont_Gdip(L"Arial", 8.0F);
	m_st_BuzzerName.Create(_T("BUZZER"), dwStyle /*| WS_BORDER*/ | SS_CENTER, rectDummy, this, IDC_STATIC);
	m_st_Buzzer.SetFont_Gdip(L"Arial", 8.0F);
	m_st_Buzzer.Create(m_szBuzzer, dwStyle /*| WS_BORDER*/ | SS_CENTER, rectDummy, this, IDC_STATIC);

	m_st_TowerLampName.SetFont_Gdip(L"Arial", 8.0F);
	m_st_TowerLampName.Create(_T("TOWSER LAMP"), dwStyle /*| WS_BORDER*/ | SS_CENTER, rectDummy, this, IDC_STATIC);
	m_st_TowerLamp.SetFont_Gdip(L"Arial", 8.0F);
	m_st_TowerLamp.Create(m_szTowerLamp, dwStyle /*| WS_BORDER*/ | SS_CENTER, rectDummy, this, IDC_STATIC);

	m_st_Message.SetFont_Gdip(L"Arial", 24.0F);
	m_st_Message.Create(m_szMessage, dwStyle /*| WS_BORDER*/ | SS_CENTER, rectDummy, this, IDC_STATIC);

	m_bn_OK.Create(_T("OK"), dwStyle | WS_TABSTOP | BS_PUSHBUTTON, rectDummy, this, IDOK);
	m_bn_OK.SetFont(&m_font_Default);

	MoveWindow(0, 0, 600, 400);
	SetTimer(3, 1000, NULL);
	return 0;
}

//=============================================================================
// Method		: OnSize
// Access		: public  
// Returns		: void
// Parameter	: UINT nType
// Parameter	: int cx
// Parameter	: int cy
// Qualifier	:
// Last Update	: 2023.04.20 - 15:00
// Desc.		:
//=============================================================================
void CDlg_OpCall_Popup::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if ((0 == cx) || (0 == cy))
		return;

	int iMagrin = 10;
	int iSpacing = 5;
	int iCateSpacing = 10;

	int iLeft = iMagrin;
	int iTop = iMagrin;
	int iWidth = cx - iMagrin - iMagrin;
	int iHeight = cy - iMagrin - iMagrin;
	int iCtrlWidth = (iWidth - iSpacing) / 2;
	int iCtrlHeight = 16;
	int iStWidth = 180;
	int iEdWidth = iWidth - iStWidth - iSpacing;
	int iSubLeft = iLeft + iStWidth + iSpacing;
	int iListHeight = 300 - 25;

	m_st_Title.MoveWindow(iLeft, iTop, iWidth, (iCtrlHeight*3));
	iTop += (iCtrlHeight * 3) + iSpacing;

	m_st_BuzzerName.MoveWindow		(iLeft, iTop, iStWidth, (iCtrlHeight));
	m_st_Buzzer.MoveWindow			(iSubLeft, iTop, iEdWidth, (iCtrlHeight));
	iTop += (iCtrlHeight ) + iSpacing;
	m_st_TowerLampName.MoveWindow	(iLeft, iTop, iStWidth, (iCtrlHeight ));
	m_st_TowerLamp.MoveWindow		(iSubLeft, iTop, iEdWidth, (iCtrlHeight));
	iTop += (iCtrlHeight ) + iSpacing;
	m_st_Message.MoveWindow			(iLeft, iTop, iWidth, (iCtrlHeight * 3));
	iTop += (iCtrlHeight * 3) + iSpacing;

	iCtrlWidth = (iWidth - (iSpacing * 2)) * 2 / 5;
	m_bn_OK.MoveWindow				(iLeft, iTop, iCtrlWidth, (iCtrlHeight * 3));
}

//=============================================================================
// Method		: OnWindowPosChanging
// Access		: public  
// Returns		: void
// Parameter	: WINDOWPOS * lpwndpos
// Qualifier	:
// Last Update	: 2023.04.20 - 15:00
// Desc.		:
//=============================================================================
void CDlg_OpCall_Popup::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
	CDialogEx::OnWindowPosChanging(lpwndpos);
}

//=============================================================================
// Method		: OnTimer
// Access		: public  
// Returns		: void
// Parameter	: __in UINT_PTR nIDEvent
// Qualifier	:
// Last Update	: 2023.04.20 - 15:00
// Desc.		:
//=============================================================================
void CDlg_OpCall_Popup::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	static bool bit = false;

	if (IsWindowVisible()) {
		if (bit) {
			if (m_szTowerLamp == "ON") {
				m_st_TowerLamp.SetBackColor_COLORREF(RGB(0, 0, 0));
			}
			if (m_szBuzzer == "ON") {
				m_st_Buzzer.SetBackColor_COLORREF(RGB(0, 0, 0));
			}
			bit = false;
		}
		else {
			if (m_szTowerLamp == "ON") {
				m_st_TowerLamp.SetBackColor_COLORREF(RGB(255, 255, 255));
			}
			if (m_szBuzzer == "ON") {
				m_st_Buzzer.SetBackColor_COLORREF(RGB(255, 255, 255));
			}
			bit = true;
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}

//=============================================================================
// Method		: OnInitDialog
// Access		: virtual protected  
// Returns		: BOOL
// Qualifier	:
// Last Update	: 2023.04.20 - 15:00
// Desc.		:
//=============================================================================
BOOL CDlg_OpCall_Popup::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	int extendedStyle = GetWindowLong(m_hWnd, GWL_EXSTYLE);
	SetWindowLong(m_hWnd, GWL_EXSTYLE, extendedStyle | WS_EX_DLGMODALFRAME);

	// 표시위치 지정
	CenterWindow();

	return TRUE;
}
//=============================================================================
// Method		: OnOK
// Access		: virtual protected  
// Returns		: void
// Parameter	: 
// Qualifier	:
// Last Update	: 2023.04.20 - 15:00
// Desc.		: OK 될떄 Reply이벤트를 보냄.
//=============================================================================
void CDlg_OpCall_Popup::OnOK()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CString transactionId;
	CString EQUIPMENTID;
	CString SUBEQPID;
	CString MESSAGE;
	CString BUZZER;
	CString TOWERLAMP;

	transactionId.Format(_T("%s"), m_transactionId);
#if SOCKET
	SUBEQPID.Format(_T("%s"), m_Svr->Get_SubEqpID());
	MESSAGE.Format(_T("%s"), m_szMessage);
	BUZZER.Format(_T("%s"), m_szBuzzer);
	TOWERLAMP.Format(_T("%s"), m_szTowerLamp);
#endif
#if TEST
	for (int n_Port = 0; n_Port < m_Svr->Get_EquipmentIDCount(); n_Port++) {
		EQUIPMENTID = m_Svr->Get_EquipmentIDStatus(n_Port).szEquipID;
		auto IN_Para = m_Svr->GetXmlEes().Set_ReplyOpCallParameter(
			transactionId,
			//SUBEQPID,
			EQUIPMENTID,
			MESSAGE,
			BUZZER,
			TOWERLAMP);
		LPARAM IN_Data = (LPARAM)IN_Para;
		AfxGetMainWnd()->PostMessage(WM_EVENT_SERVER_REPLY_OPCALL_MESSAGE, (WPARAM)m_nSvrOrder, (LPARAM)IN_Data);
	}
#endif
	KillTimer(3);
	CDialogEx::OnOK();
}
//=============================================================================
// Method		: Set_Title
// Access		: public  
// Returns		: void
// Parameter	: __in LPCTSTR IN_DATA
// Qualifier	:
// Last Update	: 2023.04.20
// Desc.		:
//=============================================================================
void CDlg_OpCall_Popup::Set_Title(__in LPCTSTR IN_DATA)
{
	m_szTitle = IN_DATA;
}
//=============================================================================
// Method		: Set_IP_Adress
// Access		: public  
// Returns		: void
// Parameter	: __in LPCTSTR IN_DATA
// Qualifier	:
// Last Update	: 2023.04.20 - 15:00
// Desc.		:
//=============================================================================
void CDlg_OpCall_Popup::Set_IP_Adress(__in LPCTSTR IN_DATA)
{
	m_szIP_Adress = IN_DATA;
}
//=============================================================================
// Method		: Set_ServerID
// Access		: public  
// Returns		: void
// Parameter	: __in LPCTSTR IN_DATA
// Parameter	: __in uint8_t IN_nDATA
// Qualifier	:
// Last Update	: 2023.04.20
// Desc.		:
//=============================================================================
void CDlg_OpCall_Popup::Set_ServerID(__in LPCTSTR IN_DATA, __in uint8_t IN_nDATA)
{
	m_szSubEqpID = IN_DATA;
	m_nSvrOrder = IN_nDATA;
}
//=============================================================================
// Method		: Set_Message
// Access		: public  
// Returns		: void
// Parameter	: __in LPCTSTR IN_DATA
// Qualifier	:
// Last Update	: 2023.04.20 - 15:00
// Desc.		:
//=============================================================================
void CDlg_OpCall_Popup::Set_Message(__in LPCTSTR IN_DATA)
{
	m_szMessage = IN_DATA;
}
//=============================================================================
// Method		: Set_Buzzer
// Access		: public  
// Returns		: void
// Parameter	: __in LPCTSTR IN_DATA
// Qualifier	:
// Last Update	: 2023.04.20
// Desc.		:
//=============================================================================
void CDlg_OpCall_Popup::Set_Buzzer(__in LPCTSTR IN_DATA)
{
	m_szBuzzer = IN_DATA;
}
//=============================================================================
// Method		: Set_TowerLamp
// Access		: public  
// Returns		: void
// Parameter	: __in LPCTSTR IN_DATA
// Qualifier	:
// Last Update	: 2023.04.20 - 15:00
// Desc.		:
//=============================================================================
void CDlg_OpCall_Popup::Set_TowerLamp(__in LPCTSTR IN_DATA)
{
	m_szTowerLamp = IN_DATA;
}
//=============================================================================
// Method		: Set_Server
// Access		: public  
// Returns		: void
// Parameter	: __in CServer * IN_DATA
// Qualifier	:
// Last Update	: 2023.04.20 - 15:00
// Desc.		:
//=============================================================================
void CDlg_OpCall_Popup::Set_Server(__in CServer * IN_DATA)
{
	m_Svr = IN_DATA;
}

//=============================================================================
// Method		: Set_ServerID
// Access		: public  
// Returns		: void
// Parameter	: __in LPCTSTR IN_DATA
// Parameter	: __in uint8_t IN_nDATA
// Qualifier	:
// Last Update	: 2023.04.20
// Desc.		:
//=============================================================================
void CDlg_OpCall_Popup::Set_transactionId(__in LPCTSTR IN_DATA)
{
	m_transactionId = IN_DATA;
}
