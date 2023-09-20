//*****************************************************************************
// Filename	: 	Dlg_TerminalMessage_Popup.cpp
// Created	:	
// Modified	:	2023.04.20 - 15:00
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
// Dlg_TerminalMessage_Popup.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "Dlg_TerminalMessage_Popup.h"
#include "afxdialogex.h"


#define		IDC_ED_TERMINALMESSAGE	1001

// CDlg_PopupMenu 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlg_TerminalMessage_Popup, CDialogEx)

CDlg_TerminalMessage_Popup::CDlg_TerminalMessage_Popup(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlg_TerminalMessage_Popup::IDD, pParent)
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

CDlg_TerminalMessage_Popup::~CDlg_TerminalMessage_Popup()
{
	m_font_Default.DeleteObject();
}

void CDlg_TerminalMessage_Popup::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlg_TerminalMessage_Popup, CDialogEx)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CDlg_TerminalMessage_Popup 메시지 처리기입니다.
//=============================================================================
// Method		: OnCreate
// Access		: public  
// Returns		: int
// Parameter	: LPCREATESTRUCT lpCreateStruct
// Qualifier	:
// Last Update	: 2023.04.20 - 15:00
// Desc.		:
//=============================================================================
int CDlg_TerminalMessage_Popup::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	DWORD dwStyle = WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	rectDummy.SetRectEmpty();

	UINT nWndID = 20;

	m_st_Title.SetBackColor_COLORREF(RGB(0, 0, 0));
	m_st_Title.SetTextColor(Color::White, Color::White);
	m_st_Title.SetFont_Gdip(L"Arial", 24.0F);
	m_st_Title.Create(_T("Terminal Message"), dwStyle | SS_CENTER | SS_CENTERIMAGE, rectDummy, this, IDC_STATIC);

	m_st_TerminalMessage.SetFont_Gdip(L"Arial", 24.0F);
	m_st_TerminalMessage.Create(m_TerminalMessage, dwStyle /*| WS_BORDER*/ | SS_CENTER, rectDummy, this, IDC_STATIC);

	m_bn_OK.Create(_T("OK"), dwStyle | WS_TABSTOP | BS_PUSHBUTTON, rectDummy, this, IDOK);
	m_bn_OK.SetFont(&m_font_Default);

	//2023.05.02 No Use
	//if (!m_ed_Log.Create(WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, rectDummy, this, nWndID++))
	//{
	//	TRACE0("출력 창을 만들지 못했습니다.\n");
	//	return -1;
	//}

#ifndef USE_DISABLE_NOT_USE_EQUIPMENT_CONTROL
#endif
	MoveWindow(0, 0, 600, 300);
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
void CDlg_TerminalMessage_Popup::OnSize(UINT nType, int cx, int cy)
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
	int iCtrlHeight = 40;
	int iStWidth = 180;
	int iEdWidth = iWidth - iStWidth - iSpacing;
	int iSubLeft = iLeft + iStWidth + iSpacing;
	int iListHeight = 300 - 25;

	m_st_Title.MoveWindow(iLeft, iTop, iWidth, iCtrlHeight);
	iTop += iCtrlHeight + iSpacing;

	iLeft = iMagrin;
	m_st_TerminalMessage.MoveWindow(iLeft, iTop, iWidth, iCtrlHeight);

	//2023.05.02 No Use
	//iTop += iCtrlHeight + iSpacing;
	//m_ed_Log.MoveWindow(iLeft, iTop, iWidth, iHeight - (iTop+ 80));

	iTop += iHeight - (iTop + 80) + iCateSpacing;
	iCtrlWidth = (iWidth - (iSpacing * 2)) * 2 / 5;
	m_bn_OK.MoveWindow(iLeft, iTop, iCtrlWidth, 70);
}
//=============================================================================
// Method		: OnInitDialog
// Access		: virtual protected  
// Returns		: BOOL
// Qualifier	:
// Last Update	: 2023.04.20 - 15:00
// Desc.		:
//=============================================================================
BOOL CDlg_TerminalMessage_Popup::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	int extendedStyle = GetWindowLong(m_hWnd, GWL_EXSTYLE);
	SetWindowLong(m_hWnd, GWL_EXSTYLE, extendedStyle | WS_EX_DLGMODALFRAME);

	// 표시위치 지정
	CenterWindow();
	//2023.05.02 No Use
	/*
	for (int i = 0;i < m_nTerminalLog.size(); i++) {
		OnAdd_Log(m_nTerminalLog.at(i).m_szTime + (" : ") + m_nTerminalLog.at(i).m_szText);
	}*/	
	return TRUE;
}

//=============================================================================
// Method		: OnOK
// Access		: virtual protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2023.04.20 - 15:00
// Desc.		:
//=============================================================================
void CDlg_TerminalMessage_Popup::OnOK()
{
	CDialogEx::OnOK();
}

//=============================================================================
// Method		: OnCancel
// Access		: virtual protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2023.04.20 - 15:00
// Desc.		:
//=============================================================================
void CDlg_TerminalMessage_Popup::OnCancel()
{
	CDialogEx::OnCancel();
}
//=============================================================================
// Method		: OnClose
// Access		: virtual protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2023.04.20 - 15:00
// Desc.		:
//=============================================================================
void CDlg_TerminalMessage_Popup::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CDialogEx::OnClose();
}

//=============================================================================
// Method		: Set_TerminalMessage
// Access		: public  
// Returns		: void
// Parameter	: __in LPCTSTR IN_DATA
// Qualifier	:
// Last Update	: 2023.04.20 - 15:00
// Desc.		:
//=============================================================================
void CDlg_TerminalMessage_Popup::Set_TerminalMessage(__in LPCTSTR IN_DATA)
{
	m_TerminalMessage.Format(_T("%s"), IN_DATA);
}
//=============================================================================
// Method		: Set_EquipmentID
// Access		: public  
// Returns		: void
// Parameter	: __in LPCTSTR IN_DATA
// Parameter	: __in uint8_t IN_nDATA
// Qualifier	:
// Last Update	: 2023.04.20 - 15:00
// Desc.		:
//=============================================================================
void CDlg_TerminalMessage_Popup::Set_ServerID(__in LPCTSTR IN_DATA, __in uint8_t IN_nDATA)
{
	m_szSvrID	= IN_DATA;
	m_nSvrOrder = IN_nDATA;
}
//=============================================================================
// Method		: Set_Server
// Access		: public  
// Returns		: void
// Parameter	: __in CServer * IN_Svr
// Qualifier	:
// Last Update	: 2023.04.20 - 15:00
// Desc.		:
//=============================================================================
void CDlg_TerminalMessage_Popup::Set_Server(__in CServer * IN_Svr)
{
	m_Svr = IN_Svr;
}

/*
//=============================================================================
// Method		: OnAdd_Log
// Access		: public  
// Returns		: void
// Parameter	: LPCTSTR lpszLog
// Parameter	: BOOL bError
// Parameter	: UINT nLogType
// Parameter	: COLORREF clrTextColor
// Qualifier	:
// Last Update	: 2023.05.01
// Desc.		:
//=============================================================================
void CDlg_TerminalMessage_Popup::Refresh_UI()
{
	m_ed_Log.SetSel(0, -1);
	m_ed_Log.SetSel(-1, -1);
}
void CDlg_TerminalMessage_Popup::OnAdd_Log(LPCTSTR lpszLog,  COLORREF clrTextColor)
{
	m_ed_Log.AddText(lpszLog, clrTextColor);
}*/