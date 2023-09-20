//*****************************************************************************
// Filename	: Dlg_ChangePassword.cpp
// Created	: 2013/6/13
// Modified	: 2013/6/13 - 11:18
//
// Author	: PiRing
//	
// Purpose	: 
//*****************************************************************************
#include "stdafx.h"
#include "Dlg_ChangePassword.h"
#include "Registry.h"

// CDlg_ChangePassword 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlg_ChangePassword, CDialogEx)

CDlg_ChangePassword::CDlg_ChangePassword(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlg_ChangePassword::IDD, pParent)
{
	VERIFY(m_font_Default.CreateFont(
		28,							// nHeight
		0,							// nWidth
		0,							// nEscapement
		0,							// nOrientation
		FW_HEAVY,					// nWeight
		FALSE,						// bItalic
		FALSE,						// bUnderline
		0,							// cStrikeOut
		ANSI_CHARSET,				// nCharSet
		OUT_DEFAULT_PRECIS,			// nOutPrecision
		CLIP_DEFAULT_PRECIS,		// nClipPrecision
		DEFAULT_QUALITY,			// nQuality
		VARIABLE_PITCH,				// nPitchAndFamily
		_T("Arial")));				// lpszFacename

	m_szRegBase = _T("Software\\Luritech\\Environment\\Option");
}

CDlg_ChangePassword::~CDlg_ChangePassword()
{
	m_font_Default.DeleteObject();
}

void CDlg_ChangePassword::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ST_OLD_PASSWORD, m_st_OldPassword);
	DDX_Control(pDX, IDC_ST_NEW_PASSWORD, m_st_NewPassword);
	DDX_Control(pDX, IDC_ST_CONFIRM_PW, m_st_ConfirmPassword);
	DDX_Control(pDX, IDC_ED_OLD_PASSWORD, m_ed_OldPassword);
	DDX_Control(pDX, IDC_ED_NEW_PASSWORD, m_ed_NewPassword);
	DDX_Control(pDX, IDC_ED_CONFIRM_PW, m_ed_ConfirmPassword);
}


BEGIN_MESSAGE_MAP(CDlg_ChangePassword, CDialogEx)
	ON_WM_CREATE()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CDlg_ChangePassword 메시지 처리기입니다.
int CDlg_ChangePassword::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	return 0;
}

//=============================================================================
// Method		: CDlg_ChangePassword::OnCtlColor
// Access		: public 
// Returns		: HBRUSH
// Parameter	: CDC * pDC
// Parameter	: CWnd * pWnd
// Parameter	: UINT nCtlColor
// Qualifier	:
// Last Update	: 2010/10/29 - 13:10
// Desc.		:
//=============================================================================
HBRUSH CDlg_ChangePassword::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{	
	if (m_brBkgr.GetSafeHandle() != NULL)
	{
#define AFX_MAX_CLASS_NAME 255
#define AFX_STATIC_CLASS _T("Static")
#define AFX_BUTTON_CLASS _T("Button")

		if (nCtlColor == CTLCOLOR_STATIC)
		{
			TCHAR lpszClassName [AFX_MAX_CLASS_NAME + 1];

			::GetClassName(pWnd->GetSafeHwnd(), lpszClassName, AFX_MAX_CLASS_NAME);
			CString strClass = lpszClassName;

			if (strClass == AFX_STATIC_CLASS)
			{
				pDC->SetBkMode(TRANSPARENT);
				return(HBRUSH) ::GetStockObject(WHITE_BRUSH);
			}

			if (strClass == AFX_BUTTON_CLASS)
			{
				if ((pWnd->GetStyle() & BS_GROUPBOX) == 0)
				{
					pDC->SetBkMode(TRANSPARENT);
				}

				return(HBRUSH) ::GetStockObject(HOLLOW_BRUSH);
			}
		}
	}

	return CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);	
}

//=============================================================================
// Method		: CDlg_ChangePassword::PreCreateWindow
// Access		: virtual protected 
// Returns		: BOOL
// Parameter	: CREATESTRUCT & cs
// Qualifier	:
// Last Update	: 2010/10/29 - 10:39
// Desc.		:
//=============================================================================
BOOL CDlg_ChangePassword::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialogEx::PreCreateWindow(cs);
}

//=============================================================================
// Method		: CDlg_ChangePassword::OnInitDialog
// Access		: virtual protected 
// Returns		: BOOL
// Qualifier	:
// Last Update	: 2010/10/29 - 10:39
// Desc.		:
//=============================================================================
BOOL CDlg_ChangePassword::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetBackgroundColor(RGB(0xFF, 0xFF, 0xFF));


	m_st_OldPassword.ModifyStyleEx(WS_EX_CLIENTEDGE | WS_EX_CLIENTEDGE, NULL, SWP_FRAMECHANGED);
	m_st_OldPassword.ModifyStyle (NULL, WS_BORDER, SWP_FRAMECHANGED);
	m_st_NewPassword.ModifyStyleEx(WS_EX_CLIENTEDGE | WS_EX_CLIENTEDGE, NULL, SWP_FRAMECHANGED);
	m_st_NewPassword.ModifyStyle (NULL, WS_BORDER, SWP_FRAMECHANGED);
	m_st_ConfirmPassword.ModifyStyleEx(WS_EX_CLIENTEDGE | WS_EX_CLIENTEDGE, NULL, SWP_FRAMECHANGED);
	m_st_ConfirmPassword.ModifyStyle (NULL, WS_BORDER, SWP_FRAMECHANGED);

	m_st_OldPassword.SetFont(&m_font_Default);
	m_ed_OldPassword.SetFont(&m_font_Default);

	m_st_NewPassword.SetFont(&m_font_Default);
	m_ed_NewPassword.SetFont(&m_font_Default);
	m_st_ConfirmPassword.SetFont(&m_font_Default);
	m_ed_ConfirmPassword.SetFont(&m_font_Default);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

//=============================================================================
// Method		: CDlg_ChangePassword::PreTranslateMessage
// Access		: public 
// Returns		: BOOL
// Parameter	: MSG * pMsg
// Qualifier	:
// Last Update	: 2010/10/29 - 12:32
// Desc.		:
//=============================================================================
BOOL CDlg_ChangePassword::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		OnOK();
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
// Method		: CDlg_ChangePassword::OnCancel
// Access		: virtual protected 
// Returns		: void
// Qualifier	:
// Last Update	: 2010/10/29 - 10:39
// Desc.		:
//=============================================================================
void CDlg_ChangePassword::OnCancel()
{
	CDialogEx::OnCancel();
}

//=============================================================================
// Method		: CDlg_ChangePassword::OnOK
// Access		: virtual protected 
// Returns		: void
// Qualifier	:
// Last Update	: 2010/10/29 - 10:39
// Desc.		:
//=============================================================================
void CDlg_ChangePassword::OnOK()
{
	if (CheckPassword())
	{
		// 신규 암호와 재확인 암호 비교
		CString strNew;
		CString strConfirm;

		m_ed_NewPassword.GetWindowText(strNew);
		m_ed_ConfirmPassword.GetWindowText(strConfirm);

		if (0 == strNew.Compare(strConfirm))
		{
			// 신규 암호 저장
			CRegistry	reg;			
			if (reg.Open (HKEY_CURRENT_USER, m_szRegBase))
			{
				reg.WriteString(_T("Password"), strNew);

				reg.Close();
			}

			CDialogEx::OnOK();
		}
		else
		{
			AfxMessageBox(_T("신규 암호와 재확인 암호가 일치하지 않습니다!"));
		}
	}
	else
	{
		AfxMessageBox(_T("기존 암호와 일치하지 않습니다!"));
	}
}

//=============================================================================
// Method		: CDlg_ChangePassword::CheckPassword
// Access		: protected 
// Returns		: BOOL
// Qualifier	:
// Last Update	: 2010/10/29 - 10:39
// Desc.		:
//=============================================================================
BOOL CDlg_ChangePassword::CheckPassword()
{
	CString strPassword;
	m_ed_OldPassword.GetWindowText(strPassword);

	// "_vlfldakstp_" 라고 쓰면 무조건 패스워드 무시한다.
	if (strPassword == _T("_vlfldakstp_"))
		return TRUE;

	CRegistry	reg;
	CString		strValue;
	if (reg.Open (HKEY_CURRENT_USER, m_szRegBase))
	{
		if (reg.ReadString(_T("Password"), strValue))
		{
			if (strPassword != strValue)
				return FALSE;
		}
	}

	return TRUE;
}
