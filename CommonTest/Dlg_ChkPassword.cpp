//*****************************************************************************
// Filename	: Dlg_ChkPassword.cpp
// Created	: 2010/10/29
// Modified	: 2010/10/29 - 10:16
//
// Author	: PiRing
//	
// Purpose	: 
//*****************************************************************************
// Dlg_ChkPassword.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Dlg_ChkPassword.h"
#include "Registry.h"
#include "resource.h"

//=============================================================================
// CDlg_ChkPassword 대화 상자입니다.
//=============================================================================
IMPLEMENT_DYNAMIC(CDlg_ChkPassword, CDialogEx)

//=============================================================================
// 생성자
//=============================================================================
CDlg_ChkPassword::CDlg_ChkPassword(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlg_ChkPassword::IDD, pParent)
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
		_T("Arial")));		// lpszFacename

	m_nPassIndex = 0;
	m_szRegBase = _T("Software\\Luritech\\Environment\\Option");
}

CDlg_ChkPassword::CDlg_ChkPassword(UINT nIndex, CWnd* pParent /*= NULL*/)
	: CDialogEx(CDlg_ChkPassword::IDD, pParent)
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
		_T("Arial")));		// lpszFacename

	m_nPassIndex = nIndex;
	m_szRegBase = _T("Software\\Luritech");
}

//=============================================================================
// 소멸자
//=============================================================================
CDlg_ChkPassword::~CDlg_ChkPassword()
{
	m_font_Default.DeleteObject();
}

void CDlg_ChkPassword::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ST_PASSWORD, m_st_Password);
	DDX_Control(pDX, IDC_ED_PASSWORD, m_ed_Password);
	DDX_Control(pDX, IDOK, m_bn_OK);
	DDX_Control(pDX, IDCANCEL, m_bn_Cancel);	
}


BEGIN_MESSAGE_MAP(CDlg_ChkPassword, CDialogEx)	
	ON_WM_CREATE()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

//=============================================================================
// CDlg_ChkPassword 메시지 처리기입니다.
//=============================================================================
//=============================================================================
// Method		: CDlg_ChkPassword::OnCreate
// Access		: protected 
// Returns		: int
// Parameter	: LPCREATESTRUCT lpCreateStruct
// Qualifier	:
// Last Update	: 2010/10/29 - 10:39
// Desc.		:
//=============================================================================
int CDlg_ChkPassword::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	return 0;
}

//=============================================================================
// Method		: CDlg_ChkPassword::OnCtlColor
// Access		: public 
// Returns		: HBRUSH
// Parameter	: CDC * pDC
// Parameter	: CWnd * pWnd
// Parameter	: UINT nCtlColor
// Qualifier	:
// Last Update	: 2010/10/29 - 13:10
// Desc.		:
//=============================================================================
HBRUSH CDlg_ChkPassword::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
// Method		: CDlg_ChkPassword::PreCreateWindow
// Access		: virtual protected 
// Returns		: BOOL
// Parameter	: CREATESTRUCT & cs
// Qualifier	:
// Last Update	: 2010/10/29 - 10:39
// Desc.		:
//=============================================================================
BOOL CDlg_ChkPassword::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialogEx::PreCreateWindow(cs);
}

//=============================================================================
// Method		: CDlg_ChkPassword::OnInitDialog
// Access		: virtual protected 
// Returns		: BOOL
// Qualifier	:
// Last Update	: 2010/10/29 - 10:39
// Desc.		:
//=============================================================================
BOOL CDlg_ChkPassword::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetBackgroundColor(RGB(0xFF, 0xFF, 0xFF));

	m_st_Password.ModifyStyleEx(WS_EX_CLIENTEDGE | WS_EX_CLIENTEDGE, NULL, SWP_FRAMECHANGED);
	m_st_Password.ModifyStyle (NULL, WS_BORDER, SWP_FRAMECHANGED);

	m_st_Password.SetFont(&m_font_Default);
	m_ed_Password.SetFont(&m_font_Default);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

//=============================================================================
// Method		: CDlg_ChkPassword::PreTranslateMessage
// Access		: public 
// Returns		: BOOL
// Parameter	: MSG * pMsg
// Qualifier	:
// Last Update	: 2010/10/29 - 12:32
// Desc.		:
//=============================================================================
BOOL CDlg_ChkPassword::PreTranslateMessage(MSG* pMsg)
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
// Method		: CDlg_ChkPassword::OnCancel
// Access		: virtual protected 
// Returns		: void
// Qualifier	:
// Last Update	: 2010/10/29 - 10:39
// Desc.		:
//=============================================================================
void CDlg_ChkPassword::OnCancel()
{
	CDialogEx::OnCancel();
}

//=============================================================================
// Method		: CDlg_ChkPassword::OnOK
// Access		: virtual protected 
// Returns		: void
// Qualifier	:
// Last Update	: 2010/10/29 - 10:39
// Desc.		:
//=============================================================================
void CDlg_ChkPassword::OnOK()
{
	if (CheckPassword())
	{
		CDialogEx::OnOK();
	}
	else
	{
		m_st_Password.SetWindowText (_T("Wrong Password"));
	}
}

//=============================================================================
// Method		: CDlg_ChkPassword::CheckPassword
// Access		: protected 
// Returns		: BOOL
// Qualifier	:
// Last Update	: 2010/10/29 - 10:39
// Desc.		:
//=============================================================================
BOOL CDlg_ChkPassword::CheckPassword()
{
	CString strPassword;
	m_ed_Password.GetWindowText(strPassword);
	
	// "_vlfldakstp_" 라고 쓰면 무조건 패스워드 무시한다.
	if (strPassword == _T("_vlfldakstp_"))
		return TRUE;

	//CRegistry	reg;
	CString		strValue;
// 	if (reg.Open (HKEY_CURRENT_USER, REG_PATH_OPTION_BASE))
// 	{
// 		if (reg.ReadString(_T("Password"), strValue))
// 		{
// 			if (strPassword != strValue)
// 				return FALSE;
// 		}
// 	}

	strValue = GetPassword();
	if (strPassword != strValue)
		return FALSE;

	return TRUE;
}

CString CDlg_ChkPassword::GetPassword()
{
	CRegistry	reg;
	CString		strValue;
	if (reg.Open(HKEY_CURRENT_USER, m_szRegBase))
	{
		if (0 == m_nPassIndex)
		{
			reg.ReadString(_T("Password_Admin"), strValue);
		}
		else
		{
			reg.ReadString(_T("Password_2"), strValue);

			//strValue = _T("Ruli_Motor_QL");
		}

		reg.Close();
	}

	return strValue;
}

void CDlg_ChkPassword::SetPasswordIndex(UINT nIndex)
{
	m_nPassIndex = nIndex;
}
