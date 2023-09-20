//*****************************************************************************
// Filename	: 	Dlg_UserLogin.cpp
// Created	:	2021/11/6 - 19:09
// Modified	:	2021/11/6 - 19:09
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
// Dlg_UserLogin.cpp : implementation file
//

#include "stdafx.h"
#include <afxdialogex.h>
#include "Dlg_Server.h"
#include "Def_WindowMessage_Cm.h"
//#include "Reg_InspInfo.h"
#include "CommonFunction.h"
#include "Def_Language_Message.h"

#if (USE_XML)
#include "resource.h"
#include "Pane_CommStatus.h"
#endif

//#include "MainFrm.h"
enum enCtrlText
{
	Txt_Title,

	Txt_MaxCount,
};

static LPCTSTR g_szCtrlText_T[Lang_MaxCount][Txt_MaxCount] =
{
	// 한국어
	{
		_T("SERVER"),				// Txt_Title
	},

	// 영어
	{
		_T("SERVER"),				// Txt_Title
	},

	// 베트남어
	{
		_T("SERVER"),				// Txt_Title
	},

	// 중국어
	{
		_T("SERVER"),				// Txt_Title
	},
};


#define		IDC_LC_USER_LIST		1010
#define		IDC_LC_USER_LIST_OPERA	1011
#define		IDC_LC_USER_LIST_ENGIN	1012
#define		IDC_LC_USER_LIST_ADMIN	1013
#define		IDC_RB_AUTHORITY_FST	1021
#define		IDC_RB_AUTHORITY_LST	1022

#define		IDC_RB_AUTHORITY_0		1023
#define		IDC_RB_AUTHORITY_2		1025

#define		IDC_CB_ID				1100
#define		IDC_ST_ACESS_MODE_T		1101
#define		IDC_ED_ACESS_PASSWORD	1102

#define		IDC_BN_LOGIN			1201
#define		IDC_BN_LOGOUT			1202
#define		IDC_BN_KEYBOARD			1203


//-----------------------------------------------------------------------------
// CDlg_UserLogin dialog
//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CDlg_Server, CDialogEx)

CDlg_Server::CDlg_Server(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlg_Server::IDD, pParent)
{
	VERIFY(m_font_Large.CreateFont(
		24,							// nHeight
		0,							// nWidth
		0,							// nEscapement
		0,							// nOrientation
		FW_BOLD,					// nWeight
		FALSE,						// bItalic
		FALSE,						// bUnderline
		0,							// cStrikeOut
		ANSI_CHARSET,				// nCharSet
		OUT_DEFAULT_PRECIS,			// nOutPrecision
		CLIP_DEFAULT_PRECIS,		// nClipPrecision
		DEFAULT_QUALITY,			// nQuality
		VARIABLE_PITCH,				// nPitchAndFamily
		_T("Arial")));		// lpszFacename

	VERIFY(m_font_Default.CreateFont(
		30,							// nHeight
		0,							// nWidth
		0,							// nEscapement
		0,							// nOrientation
		FW_BOLD,					// nWeight
		FALSE,						// bItalic
		FALSE,						// bUnderline
		0,							// cStrikeOut
		ANSI_CHARSET,				// nCharSet
		OUT_DEFAULT_PRECIS,			// nOutPrecision
		CLIP_DEFAULT_PRECIS,		// nClipPrecision
		DEFAULT_QUALITY,			// nQuality
		VARIABLE_PITCH,				// nPitchAndFamily
		_T("Arial")));		// lpszFacename

}

CDlg_Server::~CDlg_Server()
{
	m_font_Large.DeleteObject();
	m_font_Default.DeleteObject();
}

void CDlg_Server::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlg_Server, CDialogEx)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND_RANGE(IDC_RB_AUTHORITY_FST, IDC_RB_AUTHORITY_LST, OnAuthoritySelect)
	ON_COMMAND_RANGE(IDC_RB_AUTHORITY_0, IDC_RB_AUTHORITY_2, OnAuthorityRMSSelect)
END_MESSAGE_MAP()


// CDlg_UserLogin message handlers
//=============================================================================
// Method		: OnCreate
// Access		: protected  
// Returns		: int
// Parameter	: LPCREATESTRUCT lpCreateStruct
// Qualifier	:
// Last Update	: 2016/11/4 - 11:45
// Desc.		:
//=============================================================================
int CDlg_Server::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	DWORD dwStyle = WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	m_st_Title.SetBackColor_COLORREF(RGB(0, 0, 0));
	m_st_Title.SetTextColor(Color::White, Color::White);
	m_st_Title.SetFont_Gdip(L"Arial", 24.0F);
	m_st_Title.Create(g_szCtrlText_T[m_nLanguage][Txt_Title], dwStyle | SS_CENTER | SS_CENTERIMAGE, rectDummy, this, IDC_STATIC);

	for (auto nIdx = 0; nIdx < enOnlineState::ONLINESTATE_MaxCount; nIdx++)
	{
		m_bn_CtrlCmd[nIdx].EnableWindowsTheming(FALSE);
		m_bn_CtrlCmd[nIdx].m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT;
		m_bn_CtrlCmd[nIdx].m_bTransparent = false;
		m_bn_CtrlCmd[nIdx].SetFaceColor(RGB(0, 0, 0), true);
		m_bn_CtrlCmd[nIdx].SetTextColor(RGB(255, 255, 255));

		m_bn_CtrlCmd[nIdx].Create(g_szSocket_StateUI[nIdx], WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | WS_BORDER, rectDummy, this, IDC_RB_AUTHORITY_FST + nIdx);
		m_bn_CtrlCmd[nIdx].SetFont(&m_font_Default);
	}
	for (auto nIdx = 0; nIdx < enEES_Mode::EES_MAX; nIdx++)
	{
		m_bn_CtrlRms[nIdx].EnableWindowsTheming(FALSE);
		m_bn_CtrlRms[nIdx].m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT;
		m_bn_CtrlRms[nIdx].m_bTransparent = false;
		m_bn_CtrlRms[nIdx].SetFaceColor(RGB(0, 0, 0), true);
		m_bn_CtrlRms[nIdx].SetTextColor(RGB(255, 255, 255));

		m_bn_CtrlRms[nIdx].Create(g_sEES_Mode_UI[nIdx], WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | WS_BORDER, rectDummy, this, IDC_RB_AUTHORITY_0 + nIdx);
		m_bn_CtrlRms[nIdx].SetFont(&m_font_Default);
	}
	return 0;
}
void CDlg_Server::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if ((0 == cx) || (0 == cy))
		return;

	int iMagrin			= 10;
	int iSpacing		= 5;
	int iCateSpacing	= 10;

	int iLeft			= iMagrin;
	int iTop			= iMagrin;
	int iWidth			= cx - iMagrin - iMagrin;
	int iHeight			= cy - iMagrin - iMagrin;
	int iCtrlWidth		= (iWidth - iSpacing) / 2;
	int iCtrlHeight		= 40;
	int iStWidth		= 180;
	int iEdWidth		= iWidth - iStWidth - iSpacing;
	int iSubLeft		= iLeft + iStWidth + iSpacing;
	int iListHeight		= 300 - 25;

	m_st_Title.MoveWindow(iLeft, iTop, iWidth, iCtrlHeight);
	iTop += iCtrlHeight + iSpacing;

	for (auto nIdx = 0; nIdx < enOnlineState::ONLINESTATE_MaxCount;  ++nIdx)
	{
		m_bn_CtrlCmd[nIdx].MoveWindow(iLeft, iTop, iCtrlWidth, iCtrlHeight);

		iLeft += iCtrlWidth + iSpacing;
	}

	iTop += iCtrlHeight + iSpacing;
	iTop += 30 + iSpacing;

	iLeft = iMagrin;
	iListHeight -= (iCtrlHeight + iSpacing);
	iCtrlWidth = (iWidth - (iSpacing * (enEES_Mode::EES_MAX - 1))) / enEES_Mode::EES_MAX;
	for (auto nIdx = 0; nIdx < enEES_Mode::EES_MAX; ++nIdx)
	{
		m_bn_CtrlRms[nIdx].MoveWindow(iLeft, iTop, iCtrlWidth, iCtrlHeight);

		iLeft += iCtrlWidth + iSpacing;
	}

	iTop += iCtrlHeight + iSpacing;
	iLeft = iMagrin;
}

//=============================================================================
// Method		: PreTranslateMessage
// Access		: virtual protected  
// Returns		: BOOL
// Parameter	: MSG * pMsg
// Qualifier	:
// Last Update	: 2016/11/5 - 20:03
// Desc.		:
//=============================================================================
BOOL CDlg_Server::PreTranslateMessage(MSG* pMsg)
{
	return CDialogEx::PreTranslateMessage(pMsg);
}

//=============================================================================
// Method		: OnInitDialog
// Access		: virtual protected  
// Returns		: BOOL
// Qualifier	:
// Last Update	: 2016/11/5 - 20:03
// Desc.		:
//=============================================================================
BOOL CDlg_Server::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetWindowText(m_szTitle.GetBuffer());

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
//=============================================================================
// Method		: OnAuthoritySelect
// Access		: protected  
// Returns		: void
// Parameter	: UINT nID
// Qualifier	:
// Last Update	: 2022/7/20 - 15:08
// Desc.		:
//=============================================================================

void CDlg_Server::OnAuthoritySelect(UINT nID)
{
	UINT nTabIndex = nID - IDC_RB_AUTHORITY_FST;
#if (USE_XML)
	AfxGetApp()->GetMainWnd()->SendMessage(WM_EVENT_SERVER_CONNECTION, 0, (LPARAM)nTabIndex);
#endif
}

void CDlg_Server::OnAuthorityRMSSelect(UINT nID)
{
	UINT nTabIndex = nID - IDC_RB_AUTHORITY_0;
#if (USE_XML)
	AfxGetApp()->GetMainWnd()->SendMessage(WM_EVENT_REPORT_RMS_MODE, 0, (LPARAM)nTabIndex);
#endif
}

void CDlg_Server::OnLanguage(__in uint8_t IN_nLanguage) 
{

}
void CDlg_Server::Set_Wnd_Status_Server(__in std::vector<CWnd_Status_Server*> IN_Status_Server)
{
	m_pwndServerList = IN_Status_Server;
}
HWND CDlg_Server::Get_ServerHWND(__in uint8_t IN_nSvrOrder)
{
	return m_pwndServerList.at(IN_nSvrOrder)->GetSafeHwnd();
}
