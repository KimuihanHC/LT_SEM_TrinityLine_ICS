//*****************************************************************************
// Filename	: 	Dlg_AcessMode.cpp
// Created	:	2016/11/6 - 19:09
// Modified	:	2016/11/6 - 19:09
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
// Dlg_AcessMode.cpp : implementation file
//

#include "stdafx.h"
#include "Dlg_AcessMode.h"
#include "afxdialogex.h"
#include "Def_WindowMessage_Cm.h"


#define		IDC_ST_ACESS_MODE_T		1001
#define		IDC_ED_ACESS_PASSWORD	1003

#define		IDC_RB_OPER_MODE		1201
#define		IDC_RB_MANAGER_MODE		1202

// CDlg_AcessMode dialog

IMPLEMENT_DYNAMIC(CDlg_AcessMode, CDialogEx)

CDlg_AcessMode::CDlg_AcessMode(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlg_AcessMode::IDD, pParent)
{
	VERIFY(m_font_Large.CreateFont(
		20,							// nHeight
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
		36,							// nHeight
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

	m_AcessMode = Permission_Operator;
}

CDlg_AcessMode::~CDlg_AcessMode()
{
	m_font_Large.DeleteObject();
	m_font_Default.DeleteObject();
}

void CDlg_AcessMode::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlg_AcessMode, CDialogEx)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_EN_CHANGE(IDC_ED_ACESS_PASSWORD, &CDlg_AcessMode::OnEnChangeEdPassword)
	ON_BN_CLICKED(IDC_RB_OPER_MODE, OnBnClickedRbOperMode)
	ON_BN_CLICKED(IDC_RB_MANAGER_MODE, OnBnClickedRbManagerMode)
END_MESSAGE_MAP()


// CDlg_AcessMode message handlers
//=============================================================================
// Method		: OnCreate
// Access		: protected  
// Returns		: int
// Parameter	: LPCREATESTRUCT lpCreateStruct
// Qualifier	:
// Last Update	: 2016/11/4 - 11:45
// Desc.		:
//=============================================================================
int CDlg_AcessMode::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	DWORD dwStyle = WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	m_st_Title.SetBackColor_COLORREF(RGB(0, 0, 0));
	m_st_Title.SetTextColor(Color::White, Color::White);
	m_st_Title.SetFont_Gdip(L"Arial", 24.0F);

	m_rb_OperMode.Create(_T("Operator Mode"), WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON | WS_GROUP, rectDummy, this, IDC_RB_OPER_MODE);
	m_rb_AdminMode.Create(_T("Administrator Mode"), WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON, rectDummy, this, IDC_RB_MANAGER_MODE);

	m_rb_OperMode.SetFont(&m_font_Large);
	m_rb_AdminMode.SetFont(&m_font_Large);

	m_rb_OperMode.m_nFlatStyle = CMFCButton::BUTTONSTYLE_SEMIFLAT;
	m_rb_AdminMode.m_nFlatStyle = CMFCButton::BUTTONSTYLE_SEMIFLAT;
	m_rb_OperMode.SetImage(IDB_SELECTNO_16);
	m_rb_AdminMode.SetImage(IDB_SELECTNO_16);
	m_rb_OperMode.SetCheckedImage(IDB_SELECT_16);
	m_rb_AdminMode.SetCheckedImage(IDB_SELECT_16);
	m_rb_OperMode.SizeToContent();
	m_rb_AdminMode.SizeToContent();
	m_rb_OperMode.SetCheck(TRUE);

	m_st_Password.SetBackColor_COLORREF(RGB(0, 0, 0));
	m_st_Password.SetTextColor(Color::White, Color::White);
	m_st_Password.SetFont_Gdip(L"Arial", 20.0F);

	m_st_Title.Create(_T("Acess Mode"), dwStyle | SS_CENTER | SS_CENTERIMAGE, rectDummy, this, IDC_STATIC);
	m_st_Password.Create(_T("PW"), dwStyle | SS_CENTER | SS_CENTERIMAGE, rectDummy, this, IDC_STATIC);
	m_ed_Password.Create(dwStyle | WS_BORDER | WS_TABSTOP | ES_CENTER | ES_PASSWORD, rectDummy, this, IDC_ED_ACESS_PASSWORD);

	m_bn_OK.Create(_T("OK"), dwStyle | WS_TABSTOP, rectDummy, this, IDOK);
	m_bn_Cancel.Create(_T("Cancel"), dwStyle | WS_TABSTOP, rectDummy, this, IDCANCEL);

	m_ed_Password.SetFont(&m_font_Default);
	m_ed_Password.SetFocus();
	m_ed_Password.EnableWindow(FALSE);

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
void CDlg_AcessMode::OnSize(UINT nType, int cx, int cy)
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
	int iCtrlHeight = (iHeight - iSpacing - iSpacing - iCateSpacing) / 4;
	int iStaticWidth = 80;
	int iTempWidth	= iWidth - iStaticWidth;
	int iSubLeft	= iLeft + iStaticWidth;

	m_st_Title.MoveWindow(iLeft, iTop, iWidth, iCtrlHeight);
	iTop += iCtrlHeight + iSpacing;
	
	m_rb_OperMode.MoveWindow(iLeft, iTop, iCtrlWidth, iCtrlHeight);
	iLeft += iSpacing + iCtrlWidth;
	m_rb_AdminMode.MoveWindow(iLeft, iTop, iCtrlWidth, iCtrlHeight);
	
	iLeft = iMagrin;
	iTop += iCtrlHeight + iSpacing;
	m_st_Password.MoveWindow(iLeft, iTop, iStaticWidth, iCtrlHeight);
	m_ed_Password.MoveWindow(iSubLeft, iTop, iTempWidth, iCtrlHeight);

	iTop += iCtrlHeight + iCateSpacing;
	iCtrlWidth = (iWidth - (iSpacing * 2)) / 3;
	m_bn_OK.MoveWindow(iLeft, iTop, iCtrlWidth, iCtrlHeight);
	iLeft += iCtrlWidth + iSpacing;
	iLeft += iCtrlWidth + iSpacing;
	m_bn_Cancel.MoveWindow(iLeft, iTop, iCtrlWidth, iCtrlHeight);

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
BOOL CDlg_AcessMode::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		// 여기에 Enter키 기능 작성
		return TRUE;
	}
	else if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		// 여기에 ESC키 기능 작성       
		return TRUE;
	}

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
BOOL CDlg_AcessMode::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	LoadAcessMode();

	m_ed_Password.SetFocus();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

//=============================================================================
// Method		: OnEnChangeEdPassword
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2017/1/2 - 11:33
// Desc.		:
//=============================================================================
void CDlg_AcessMode::OnEnChangeEdPassword()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

//=============================================================================
// Method		: OnBnClickedRbOperMode
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2017/1/2 - 11:33
// Desc.		:
//=============================================================================
void CDlg_AcessMode::OnBnClickedRbOperMode()
{
	m_ed_Password.EnableWindow(FALSE);
}

//=============================================================================
// Method		: OnBnClickedRbManagerMode
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2017/1/2 - 11:33
// Desc.		:
//=============================================================================
void CDlg_AcessMode::OnBnClickedRbManagerMode()
{
	m_ed_Password.EnableWindow(TRUE);
	m_ed_Password.SetFocus();
}

//=============================================================================
// Method		: OnOK
// Access		: virtual protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2016/11/7 - 14:19
// Desc.		:
//=============================================================================
void CDlg_AcessMode::OnOK()
{
	if (BST_CHECKED == m_rb_AdminMode.GetCheck())
	{
		if (CheckPassword())
		{
			SaveAcessMode();

			AfxGetApp()->GetMainWnd()->SendMessage(WM_PERMISSION_MODE, (LPARAM)m_AcessMode, 0);

			// Delay?
		}
		else
		{
			// 암호가 틀림
			AfxMessageBox(_T("Password incorrect!!"));
			return;
		}
	}
	else
	{
		SaveAcessMode();

		AfxGetApp()->GetMainWnd()->SendMessage(WM_PERMISSION_MODE, (LPARAM)m_AcessMode, 0);
	}

	CDialogEx::OnOK();
}

//=============================================================================
// Method		: OnCancel
// Access		: virtual protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2016/11/7 - 19:10
// Desc.		:
//=============================================================================
void CDlg_AcessMode::OnCancel()
{
	CDialogEx::OnCancel();
}

//=============================================================================
// Method		: LoadAcessMode
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2016/11/7 - 14:31
// Desc.		:
//=============================================================================
void CDlg_AcessMode::LoadAcessMode()
{
	// Password
	// Acess Mode

	CString szText;

	if (m_AcessMode == Permission_Administrator)
	{
		m_rb_AdminMode.SetCheck(BST_CHECKED);
		m_rb_OperMode.SetCheck(BST_UNCHECKED);
	}
	else
	{
		m_rb_AdminMode.SetCheck(BST_UNCHECKED);
		m_rb_OperMode.SetCheck(BST_CHECKED);
	}
}

//=============================================================================
// Method		: SaveAcessMode
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2016/11/8 - 9:52
// Desc.		:
//=============================================================================
void CDlg_AcessMode::SaveAcessMode()
{
	// Password
	// Acess Mode
	if (BST_CHECKED == m_rb_AdminMode.GetCheck())
	{
		m_AcessMode = Permission_Administrator;
	}
	else
	{
		m_AcessMode = Permission_Operator;
	}
}

//=============================================================================
// Method		: CheckPassword
// Access		: protected  
// Returns		: BOOL
// Qualifier	:
// Last Update	: 2016/11/7 - 18:00
// Desc.		:
//=============================================================================
BOOL CDlg_AcessMode::CheckPassword()
{
// 	Permission_Operator,		// 옵션 보기/수정 불가
// 	Permission_Engineer,			// 옵션 보기 가능, 수정 불가
// 	Permission_Administrator,	// 옵션 보기/수정 가능

	CString szPass;
	CString szLoadPass;
	m_ed_Password.GetWindowText(szPass);

	szLoadPass = m_regManagement.LoadPassword_Admin();
	if (0 == szPass.Compare(szLoadPass))
	{
		m_AcessMode = Permission_Administrator;
		//m_bn_ModifyPW.ShowWindow(SW_SHOW);
		return TRUE;
	}
	else
	{
// 		szLoadPass = m_regManagement.LoadPassword_Mgr();
// 		if (0 == szPass.Compare(szLoadPass))
// 		{
// 			m_AcessMode = Permission_Engineer;
// 			//m_bn_ModifyPW.ShowWindow(SW_SHOW);
// 			return TRUE;
// 		}

		return FALSE;
	}

	if (0 == szPass.GetLength())
	{
		m_AcessMode = Permission_Operator;
		return TRUE;
	}

	return FALSE;
}

//=============================================================================
// Method		: SetAcessMode
// Access		: public  
// Returns		: void
// Parameter	: __in enPermissionMode nAcessMode
// Qualifier	:
// Last Update	: 2016/11/7 - 17:53
// Desc.		:
//=============================================================================
void CDlg_AcessMode::SetAcessMode(__in enPermissionMode nAcessMode)
{
	m_AcessMode = nAcessMode;
}
