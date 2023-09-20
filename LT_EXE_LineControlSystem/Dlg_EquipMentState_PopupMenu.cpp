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
#include "Dlg_EquipMentState_PopupMenu.h"
#include "afxdialogex.h"


#define		IDC_BN_CTRL_FST		1001
#define		IDC_BN_CTRL_LST		IDC_BN_CTRL_FST + enEquipmentState::ES_MaxCount - 1

// CDlg_PopupMenu 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlg_EquipMentState_PopupMenu, CDialogEx)

CDlg_EquipMentState_PopupMenu::CDlg_EquipMentState_PopupMenu(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlg_EquipMentState_PopupMenu::IDD, pParent)
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

CDlg_EquipMentState_PopupMenu::~CDlg_EquipMentState_PopupMenu()
{
	// 테마 비활성화가 다른 윈도우에 영향을 주어서 소멸자에서 테마 원복 시킴
	for (auto nIdx = 0; nIdx < enEquipmentState::ES_MaxCount; nIdx++)
	{
		m_bn_CtrlCmd[nIdx].EnableWindowsTheming(TRUE);
		m_bn_CtrlCmd[nIdx].m_nFlatStyle = CMFCButton::BUTTONSTYLE_3D;
		m_bn_CtrlCmd[nIdx].m_bTransparent = true;
	}

	m_font_Default.DeleteObject();
}

void CDlg_EquipMentState_PopupMenu::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlg_EquipMentState_PopupMenu, CDialogEx)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_WINDOWPOSCHANGING()
	ON_COMMAND_RANGE(IDC_BN_CTRL_FST, IDC_BN_CTRL_LST, OnBnClicked_EquipMentState)
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
int CDlg_EquipMentState_PopupMenu::OnCreate(LPCREATESTRUCT lpCreateStruct)
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
	for (auto nIdx = 0; nIdx < enEquipmentState::ES_MaxCount; nIdx++)
	{
		m_bn_CtrlCmd[nIdx].EnableWindowsTheming(FALSE);
		m_bn_CtrlCmd[nIdx].m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT;
		m_bn_CtrlCmd[nIdx].m_bTransparent = false;
		m_bn_CtrlCmd[nIdx].SetFaceColor(RGB(0, 0, 0), true);
		m_bn_CtrlCmd[nIdx].SetTextColor(RGB(255, 255, 255));

		m_bn_CtrlCmd[nIdx].Create(g_szEquipment_State[nIdx], WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | WS_BORDER, rectDummy, this, nWinID++);
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
void CDlg_EquipMentState_PopupMenu::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	int nWidthN1 = 2;
	int nAddWidthN1 = 0;
	if ((enEquipmentState::ES_MaxCount % 2) > 0) {
		nAddWidthN1 = 1;
	}
	int nLeft = 5;
	int nTop = 5;
	int nWidth = cx - 10;
	int nHeight = cy - 10;
	int nBnWidth = (nWidth - 5) / nWidthN1;
	int nBnHeight = //(nHeight - (5 * (enRMS_Mode::RM_MaxCount / 2 - 1))) / (enRMS_Mode::RM_MaxCount / 2);
		(nHeight - (5 * ((enEquipmentState::ES_MaxCount / nWidthN1) + nAddWidthN1))) / ((enEquipmentState::ES_MaxCount / nWidthN1) + nAddWidthN1);
	int nLeftSub = nLeft + nBnWidth + 5;

	m_st_frame.MoveWindow(0, 0, cx, cy);

	for (auto nIdx = 0; nIdx < enEquipmentState::ES_MaxCount; ++nIdx)
	{
		m_bn_CtrlCmd[nIdx].MoveWindow(nLeft, nTop, nBnWidth, nBnHeight);

		++nIdx;

		if (enEquipmentState::ES_MaxCount <= nIdx)
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
void CDlg_EquipMentState_PopupMenu::OnWindowPosChanging(WINDOWPOS* lpwndpos)
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
void CDlg_EquipMentState_PopupMenu::OnBnClicked_EquipMentState(UINT nID)
{
	UINT nBnIndex = nID - IDC_BN_CTRL_FST;
	//AfxGetMainWnd()->PostMessage(WM_EVENT_SERVER_REPORT_ONLINE_STATE, (WPARAM)nBnIndex, (LPARAM)m_nSvrOrder);
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
BOOL CDlg_EquipMentState_PopupMenu::OnInitDialog()
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
void CDlg_EquipMentState_PopupMenu::Set_InitPos(__in int x, __in int y)
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
void CDlg_EquipMentState_PopupMenu::Set_Title(__in LPCTSTR IN_szTitle)
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
void CDlg_EquipMentState_PopupMenu::Set_ServerID(__in LPCTSTR IN_szSvrID, __in uint8_t IN_nSvrOrder)
{
	m_szSvrID	= IN_szSvrID;
	m_nSvrOrder = IN_nSvrOrder;
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
void CDlg_EquipMentState_PopupMenu::Set_Loader(__in bool IN_bLoader)
{
	m_bLoader = IN_bLoader;
}
