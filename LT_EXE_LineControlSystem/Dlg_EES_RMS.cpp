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
#include "Dlg_EES_RMS.h"
#include "Def_WindowMessage_Cm.h"
#include "Reg_InspInfo.h"
#include "CommonFunction.h"
#include "Def_Language_Message.h"

#include "MainFrm.h"
enum enCtrlText
{
	Txt_Title,
	Txt_ID,
	Txt_Password,
	Txt_Login,
	Txt_Logout,
	Txt_Keyboard,

	Txt_MaxCount,
};

static LPCTSTR g_szCtrlText_T[Lang_MaxCount][Txt_MaxCount] =
{
	// 한국어
	{
		_T("RMS"),				// Txt_Title
	},

	// 영어
	{
		_T("RMS"),				// Txt_Title
	},

	// 베트남어
	{
		_T("RMS"),				// Txt_Title
	},

	// 중국어
	{
		_T("RMS"),				// Txt_Title
	},
};


#define		IDC_LC_USER_LIST		1010
#define		IDC_LC_USER_LIST_OPERA	1011
#define		IDC_LC_USER_LIST_ENGIN	1012
#define		IDC_LC_USER_LIST_ADMIN	1013
#define		IDC_RB_AUTHORITY_FST	1021
#define		IDC_RB_AUTHORITY_LST	1023

#define		IDC_CB_ID				1100
#define		IDC_ST_ACESS_MODE_T		1101
#define		IDC_ED_ACESS_PASSWORD	1102

#define		IDC_BN_LOGIN			1201
#define		IDC_BN_LOGOUT			1202
#define		IDC_BN_KEYBOARD			1203


//-----------------------------------------------------------------------------
// CDlg_UserLogin dialog
//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CDlg_EES_RMS, CDialogEx)

CDlg_EES_RMS::CDlg_EES_RMS(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlg_EES_RMS::IDD, pParent)
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

CDlg_EES_RMS::~CDlg_EES_RMS()
{
	m_font_Large.DeleteObject();
	m_font_Default.DeleteObject();
}

void CDlg_EES_RMS::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlg_EES_RMS, CDialogEx)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND_RANGE(IDC_RB_AUTHORITY_FST, IDC_RB_AUTHORITY_LST, OnAuthoritySelect)
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
int CDlg_EES_RMS::OnCreate(LPCREATESTRUCT lpCreateStruct)
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

	for (auto nIdx = 0; nIdx < enEES_Mode::EES_MAX; nIdx++)
	{
		m_bn_CtrlCmd[nIdx].EnableWindowsTheming(FALSE);
		m_bn_CtrlCmd[nIdx].m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT;
		m_bn_CtrlCmd[nIdx].m_bTransparent = false;
		m_bn_CtrlCmd[nIdx].SetFaceColor(RGB(0, 0, 0), true);
		m_bn_CtrlCmd[nIdx].SetTextColor(RGB(255, 255, 255));
		m_bn_CtrlCmd[nIdx].Create(g_sEES_Mode_UI[nIdx], WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | WS_BORDER, rectDummy, this, IDC_RB_AUTHORITY_FST + nIdx);
		m_bn_CtrlCmd[nIdx].SetFont(&m_font_Default);
	}

	return 0;
}

//=============================================================================
// Method		: OnSize
// Access		: protected  
// Returns		: void
// Parameter	: UINT nType
// Parameter	: int cx
// Parameter	: int cy
// Qualifier	:
// Last Update	: 2016/11/4 - 11:45
// Desc.		:
//=============================================================================
void CDlg_EES_RMS::OnSize(UINT nType, int cx, int cy)
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
	
	m_bn_Keyboard.MoveWindow(cx - iMagrin - iStWidth, iTop, iStWidth, 30);

	iTop += 30 + iSpacing;

	iListHeight -= (iCtrlHeight + iSpacing);
	iCtrlWidth = (iWidth - (iSpacing * (enEES_Mode::EES_MAX - 1))) / enEES_Mode::EES_MAX;
	for (auto nIdx = 0; nIdx < enEES_Mode::EES_MAX; ++nIdx)
	{
		m_bn_CtrlCmd[nIdx].MoveWindow(iLeft, iTop, iCtrlWidth, iCtrlHeight);

		iLeft += iCtrlWidth + iSpacing;
	}

	iTop += iCtrlHeight + iSpacing;
	iLeft = iMagrin;
	/*
	for (auto nIdx = 0; nIdx < Permission_MaxCount; ++nIdx)
	{
		m_lc_UserList[nIdx].MoveWindow(iLeft, iTop, iWidth, iListHeight);
	}

	iTop += iListHeight + iCateSpacing;
	m_st_ID.MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
	m_cb_ID.MoveWindow(iSubLeft, iTop, iEdWidth, iCtrlHeight);

	iTop += iCtrlHeight + iSpacing;
	m_st_Password.MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
	m_ed_Password.MoveWindow(iSubLeft, iTop, iEdWidth, iCtrlHeight);

	iTop += iCtrlHeight + iCateSpacing;
	iCtrlWidth = (iWidth - (iSpacing * 2)) * 2 / 5;
	m_bn_LogIn.MoveWindow(iLeft, iTop, iCtrlWidth, 70);
	iLeft = cx - iMagrin - iCtrlWidth;
	m_bn_LogOut.MoveWindow(iLeft, iTop, iCtrlWidth, 70);
	*/
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
BOOL CDlg_EES_RMS::PreTranslateMessage(MSG* pMsg)
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
BOOL CDlg_EES_RMS::OnInitDialog()
{
	CDialogEx::OnInitDialog();

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
void CDlg_EES_RMS::OnAuthoritySelect(UINT nID)
{
	UINT nTabIndex = nID - IDC_RB_AUTHORITY_FST;
	m_nEES = (enEES_Mode)nTabIndex;
#if defined(EES_XML)//20231003
	AfxGetApp()->GetMainWnd()->SendMessage(WM_EVENT_REPORT_RMS_MODE, 0, (LPARAM)m_nEES);
#endif 
	OnOK();
}

//=============================================================================
// Method		: Get_Login_User
// Access		: public  
// Returns		: ST_User_Unit&
// Qualifier	:
// Last Update	: 2022/1/6 - 18:01
// Desc.		:
//=============================================================================
uint8_t CDlg_EES_RMS::Get_EES()
{
	return m_nEES;
}