//*****************************************************************************
// Filename	: 	Dlg_PopupMenu.cpp
// Created	:	2021/12/28 - 12:02
// Modified	:	2021/12/28 - 12:02
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
// Dlg_PopupMenu.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Dlg_EESMode_PopupMenu.h"
#include "afxdialogex.h"


#define		IDC_BN_CTRL_FST		1001
#define		IDC_BN_CTRL_LST		IDC_BN_CTRL_FST + enEES_Mode::EES_MAX - 1

// CDlg_PopupMenu 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlg_EESMode_PopupMenu, CDialogEx)

CDlg_EESMode_PopupMenu::CDlg_EESMode_PopupMenu(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlg_EESMode_PopupMenu::IDD, pParent)
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

CDlg_EESMode_PopupMenu::~CDlg_EESMode_PopupMenu()
{
	// 테마 비활성화가 다른 윈도우에 영향을 주어서 소멸자에서 테마 원복 시킴
	for (auto nIdx = 0; nIdx < enEES_Mode::EES_MAX; nIdx++)
	{
		m_bn_CtrlCmd[nIdx].EnableWindowsTheming(TRUE);
		m_bn_CtrlCmd[nIdx].m_nFlatStyle = CMFCButton::BUTTONSTYLE_3D;
		m_bn_CtrlCmd[nIdx].m_bTransparent = true;
	}

	m_font_Default.DeleteObject();
}

void CDlg_EESMode_PopupMenu::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlg_EESMode_PopupMenu, CDialogEx)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_WINDOWPOSCHANGING()
	ON_COMMAND_RANGE(IDC_BN_CTRL_FST, IDC_BN_CTRL_LST, OnBnClicked_EES_Mode)
END_MESSAGE_MAP()


// CDlg_PopupMenu 메시지 처리기입니다.
//=============================================================================
// Method		: OnCreate
// Access		: public  
// Returns		: int
// Parameter	: LPCREATESTRUCT lpCreateStruct
// Qualifier	:
// Last Update	: 2021/12/27 - 16:32
// Desc.		:
//=============================================================================
int CDlg_EESMode_PopupMenu::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetBackgroundColor(RGB(0, 0, 0));

	CRect rectDummy;
	DWORD dwStyle = WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	rectDummy.SetRectEmpty();

	m_st_frame.SetBackColor(RGB(255, 255, 255));
	m_st_frame.SetBorderColor_COLORREF(RGB(255, 255, 255));
	m_st_frame.SetBorderTickness(2);
	m_st_frame.Create(_T(""), dwStyle | SS_CENTER | SS_CENTERIMAGE | SS_WHITEFRAME, rectDummy, this, 0);
	m_st_frame.SetFont_Gdip(L"arial", 16.0f, Gdiplus::FontStyleBold);

	UINT nWinID = IDC_BN_CTRL_FST;
	for (auto nIdx = 0; nIdx < enEES_Mode::EES_MAX; nIdx++)
	{
		m_bn_CtrlCmd[nIdx].EnableWindowsTheming(FALSE);
		m_bn_CtrlCmd[nIdx].m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT;
		m_bn_CtrlCmd[nIdx].m_bTransparent = false;
		m_bn_CtrlCmd[nIdx].SetFaceColor(RGB(0, 0, 0), true);
		m_bn_CtrlCmd[nIdx].SetTextColor(RGB(255, 255, 255));

		m_bn_CtrlCmd[nIdx].Create(g_sEES_Mode[nIdx], WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | WS_BORDER, rectDummy, this, nWinID++);
		m_bn_CtrlCmd[nIdx].SetFont(&m_font_Default);
	}

#ifndef USE_DISABLE_NOT_USE_EQUIPMENT_CONTROL
	m_bn_CtrlCmd[CC_Reset].SetTextColor(RGB(255, 255, 0));
#endif
	//m_bn_CtrlCmd[CC_Run].SetTextColor(RGB(0, 200, 0));
	//m_bn_CtrlCmd[CC_Stop].SetTextColor(RGB(200, 0, 0));
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
// Last Update	: 2021/12/27 - 16:32
// Desc.		:
//=============================================================================
void CDlg_EESMode_PopupMenu::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	int nWidthN1 = 2;
	int nAddWidthN1 = 0;
	if ((enEES_Mode::EES_MAX % 2) > 0 ) {
		nAddWidthN1 = 1;
	}
	int nLeft		= 5;
	int nTop		= 5;
	int nWidth		= cx - 10;
	int nHeight		= cy - 10;
	int nBnWidth	= (nWidth - 5) / nWidthN1;
	int nBnHeight	= //(nHeight - (5 * (enRMS_Mode::RM_MaxCount / 2 - 1))) / (enRMS_Mode::RM_MaxCount / 2);
		(nHeight - (5 * ((enEES_Mode::EES_MAX / nWidthN1) + nAddWidthN1))) / ((enEES_Mode::EES_MAX / nWidthN1)+nAddWidthN1);
	int nLeftSub	= nLeft + nBnWidth + 5;

	m_st_frame.MoveWindow(0, 0, cx, cy);

	for (auto nIdx = 0; nIdx < enEES_Mode::EES_MAX; ++nIdx)
	{
		m_bn_CtrlCmd[nIdx].MoveWindow(nLeft, nTop, nBnWidth, nBnHeight);

		++nIdx;

		if (enEES_Mode::EES_MAX <= nIdx)
			break;
		
		m_bn_CtrlCmd[nIdx].MoveWindow(nLeftSub, nTop, nBnWidth, nBnHeight);

		nTop += nBnHeight + 5;
	}
}

//=============================================================================
// Method		: OnWindowPosChanging
// Access		: public  
// Returns		: void
// Parameter	: WINDOWPOS * lpwndpos
// Qualifier	:
// Last Update	: 2021/12/27 - 18:09
// Desc.		:
//=============================================================================
void CDlg_EESMode_PopupMenu::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
	// Dialog 이동 방지
	if (m_bActivted)
	{
		lpwndpos->flags |= SWP_NOMOVE;
	}

	CDialogEx::OnWindowPosChanging(lpwndpos);
}

//=============================================================================
// Method		: OnBnClicked_CtrlCmd
// Access		: public  
// Returns		: void
// Parameter	: UINT nID
// Qualifier	:
// Last Update	: 2021/12/27 - 18:10
// Desc.		:
//=============================================================================
void CDlg_EESMode_PopupMenu::OnBnClicked_EES_Mode(UINT nID)
{
	UINT nBnIndex = nID - IDC_BN_CTRL_FST;

	CString EQUIPMENTID;
	CString LOTID;
	CString EESMODE;

	EESMODE.Format(_T("%s"), g_sEES_Mode[nBnIndex]);
#if SOCKET
	if (m_Svr->Get_ClientConnection()) {

		for (int n_Port = 0; n_Port < m_Svr->Get_EquipmentIDCount(); n_Port++) {
			EQUIPMENTID = m_Svr->Get_EquipmentIDStatus(n_Port).szEquipID;
#if TEST
			auto IN_Para = m_Svr->GetXmlEes().Set_ReportRmsModeParameter(
				EQUIPMENTID,
				LOTID,
				EESMODE);
			LPARAM IN_Data = (LPARAM)IN_Para;
			AfxGetMainWnd()->PostMessage(WM_EVENT_SERVER_REPORT_RMS_MODE, (WPARAM)m_nEqpOrder, (LPARAM)IN_Data);
#endif	//TEST
		}

	}
#endif	//SOCKET
	OnOK();
}

//=============================================================================
// Method		: OnInitDialog
// Access		: virtual protected  
// Returns		: BOOL
// Qualifier	:
// Last Update	: 2021/12/27 - 15:38
// Desc.		:
//=============================================================================
BOOL CDlg_EESMode_PopupMenu::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	int extendedStyle = GetWindowLong(m_hWnd, GWL_EXSTYLE);
	SetWindowLong(m_hWnd, GWL_EXSTYLE, extendedStyle | WS_EX_DLGMODALFRAME);

	// 표시위치 지정
	SetWindowPos(NULL, m_nInitPos.x, m_nInitPos.y, 0, 0, SWP_NOSIZE);
	SetWindowText (m_szTitle.GetBuffer());
	
	m_bActivted  = true;

	return TRUE;
}

//=============================================================================
// Method		: Set_InitPos
// Access		: public  
// Returns		: void
// Parameter	: __in int x
// Parameter	: __in int y
// Qualifier	:
// Last Update	: 2021/12/27 - 15:59
// Desc.		: 팝업 초기 위치 설정
//=============================================================================
void CDlg_EESMode_PopupMenu::Set_InitPos(__in int x, __in int y)
{
	m_nInitPos.x = x;
	m_nInitPos.y = y;
}

//=============================================================================
// Method		: Set_Title
// Access		: public  
// Returns		: void
// Parameter	: __in LPCTSTR IN_szTitle
// Qualifier	:
// Last Update	: 2021/12/27 - 16:39
// Desc.		:
//=============================================================================
void CDlg_EESMode_PopupMenu::Set_Title(__in LPCTSTR IN_szTitle)
{
	m_szTitle = IN_szTitle;
}

//=============================================================================
// Method		: Set_EquipmentID
// Access		: public  
// Returns		: void
// Parameter	: __in LPCTSTR IN_szEqpID
// Parameter	: __in uint8_t IN_nEqpOrder
// Qualifier	:
// Last Update	: 2021/12/27 - 19:00
// Desc.		:
//=============================================================================
void CDlg_EESMode_PopupMenu::Set_EquipmentID(__in LPCTSTR IN_szEqpID, __in uint8_t IN_nEqpOrder)
{
	m_szEqpID	= IN_szEqpID;
	m_nEqpOrder = IN_nEqpOrder;
}

//=============================================================================
// Method		: Set_Loader
// Access		: public  
// Returns		: void
// Parameter	: __in bool IN_bLoader
// Qualifier	:
// Last Update	: 2022/7/27 - 12:02
// Desc.		:
//=============================================================================
void CDlg_EESMode_PopupMenu::Set_Loader(__in bool IN_bLoader)
{
	m_bLoader = IN_bLoader;
}
//=============================================================================
// Method		: Set_ReportRMSMode
// Access		: public  
// Returns		: void
// Parameter	: __in bool IN_bLoader
// Qualifier	:
// Last Update	: 2022/7/27 - 12:02
// Desc.		:
//=============================================================================
void CDlg_EESMode_PopupMenu::Set_Server(__in CServer * IN_Svr)
{
	m_Svr = IN_Svr;
}