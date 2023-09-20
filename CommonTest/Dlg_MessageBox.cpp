// Dlg_MessageBox.cpp: 구현 파일
//

#include "stdafx.h"
#include "resource.h"
#include "Dlg_MessageBox.h"

#include "VGStatic.h"

#define IDC_BUTTON_RESOURCE (10000)

// Dlg_MessageBox 대화 상자
IMPLEMENT_DYNAMIC(CDlg_MessageBox, CDialogEx)

CDlg_MessageBox::CDlg_MessageBox(LPCTSTR IN_szTitle, LPCTSTR IN_szText, UINT IN_nMB_Type /*= MB_OK*/, CWnd* pParent /*=nullptr*/)
: CDialogEx(IDD_DLG_ERR, pParent)
, m_szTitle(IN_szTitle)
, m_szText(IN_szText)
, m_nMB_Type(IN_nMB_Type)
{
	switch (m_nMB_Type)
	{
	case MB_OK:
		if (m_szTitle.IsEmpty())
		{
			m_szTitle = _T("Warning");
		}
		break;

	case MB_OKCANCEL:
	case MB_YESNO:
		if (m_szTitle.IsEmpty())
		{
			m_szTitle = _T("Confirm");
		}
		break;

	case MB_RETRYCANCEL:
	default:
		if (m_szTitle.IsEmpty())
		{
			m_szTitle = _T("Message");
		}
		break;
	}

	CreateResource();
}

CDlg_MessageBox::CDlg_MessageBox(LPCTSTR IN_szText, UINT IN_nMB_Type /*= MB_OK*/, CWnd* pParent /*=nullptr*/)
: CDialogEx(IDD_DLG_ERR, pParent)
, m_szText(IN_szText)
, m_nMB_Type(IN_nMB_Type)
{
	switch (m_nMB_Type)
	{
	case MB_OK:
		m_szTitle = _T("Warning");
		break;

	case MB_OKCANCEL:
	case MB_YESNO:
		m_szTitle = _T("Confirm");
		break;

	case MB_RETRYCANCEL:
	default:
		m_szTitle = _T("Message");
		break;
	}

	CreateResource();
}

CDlg_MessageBox::~CDlg_MessageBox()
{
	ReleaseResource();
}

BEGIN_MESSAGE_MAP(CDlg_MessageBox, CDialogEx)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// Dlg_MessageBox 메시지 처리기
//=============================================================================
// Method		: OnCreate
// Access		: protected  
// Returns		: int
// Parameter	: LPCREATESTRUCT lpCreateStruct
// Qualifier	:
// Last Update	: 2022/7/28 - 9:45
// Desc.		:
//=============================================================================
int CDlg_MessageBox::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	DWORD dwStyle = WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// static
	m_st_Title.SetSpacing(15);
	m_st_Title.SetUseBorder(FALSE);
	m_st_Title.SetTextAlignment(StringAlignmentNear);
	m_st_Title.SetFont_Gdip(L"Tahoma", 25.0F, FontStyleBoldItalic);
	m_st_Title.Create(m_szTitle, dwStyle, rectDummy, this, IDC_STATIC);

	m_st_Line.SetStaticStyle(CVGStatic::StaticStyle_Default);
	m_st_Line.SetColorStyle(CVGStatic::ColorStyle_Red);
	m_st_Line.SetBorderTickness(0);
	m_st_Line.Create(_T(""), dwStyle, rectDummy, this, IDC_STATIC);

	m_st_Text.SetColorStyle(CVGStatic::ColorStyle_Default);
	m_st_Text.SetSpacing(15);
	m_st_Text.SetTextAlignment(StringAlignmentNear);
	m_st_Text.SetFont_Gdip(L"Arial", 15.0F);
	m_st_Text.Create(m_szText, dwStyle, rectDummy, this, IDC_STATIC);

	m_st_Solution.SetColorStyle(CVGStatic::ColorStyle_Default);
	m_st_Solution.SetSpacing(15);
	m_st_Solution.SetTextAlignment(StringAlignmentNear);
	m_st_Solution.SetFont_Gdip(L"Tahoma", 15.0F);
	m_st_Solution.Create(m_szSolution, dwStyle, rectDummy, this, IDC_STATIC);

	m_bn_Ok.Create(_T("OK"), dwStyle | BS_PUSHBUTTON, rectDummy, this, IDOK);
	m_bn_Cancel.Create(_T("CANCEL"), dwStyle | BS_PUSHBUTTON, rectDummy, this, IDCANCEL);
	m_bn_Ok.SetFont(&m_font);
	m_bn_Cancel.SetFont(&m_font);

	m_st_Border.SetBorderTickness(5);
	if (MB_OK == m_nMB_Type)
		m_st_Border.SetBorderColor_COLORREF(RGB(192, 0, 0));
	m_st_Border.Create(_T(""), dwStyle | SS_BLACKFRAME, rectDummy, this, IDC_STATIC);

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
// Last Update	: 2022/7/28 - 9:46
// Desc.		:
//=============================================================================
void CDlg_MessageBox::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	int nWidth = cx - 30;
	int nHeight = cy - 30;

	
	m_st_Title.MoveWindow(15, 10, nWidth, 50);
	m_st_Line.MoveWindow( 15, 60, nWidth, 5);
	if (m_szSolution.IsEmpty())
	{
		int nMsgHeight = cy - 150;
		m_st_Text.MoveWindow(15, 70, nWidth, nMsgHeight);
		m_st_Solution.MoveWindow(0, 0, 0, 0);
	}
	else
	{
		int nMsgHeight = cy - 200;
		m_st_Text.MoveWindow(15, 70, nWidth, 50);
		m_st_Solution.MoveWindow(15, 120, nWidth, nMsgHeight);
	}

	int nTop = cy - 15 - 50;
	int nLeft = cx - 15 - 250;
	if (MB_OK == m_nMB_Type)
	{
		m_bn_Ok.MoveWindow(nLeft, nTop, 250, 50);
		m_bn_Cancel.MoveWindow(0, 0, 0, 0);
	}
	else
	{
		m_bn_Ok.MoveWindow(15, nTop, 250, 50);
		m_bn_Cancel.MoveWindow(nLeft, nTop, 250, 50);
	}

	m_st_Border.MoveWindow(0, 0, cx, cy);
}

//=============================================================================
// Method		: OnDestroy
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2022/7/28 - 9:46
// Desc.		:
//=============================================================================
void CDlg_MessageBox::OnDestroy()
{
	__super::OnDestroy();
}

//=============================================================================
// Method		: OnInitDialog
// Access		: virtual protected  
// Returns		: BOOL
// Qualifier	:
// Last Update	: 2022/7/28 - 9:46
// Desc.		:
//=============================================================================
BOOL CDlg_MessageBox::OnInitDialog()
{
	__super::OnInitDialog();

	switch (m_nMB_Type)
	{
	case MB_OK:
		m_bn_Ok.SetWindowText(_T("OK"));
		m_bn_Cancel.ShowWindow(SW_HIDE);
		break;

	case MB_OKCANCEL:
		m_bn_Ok.SetWindowText(_T("OK"));
		m_bn_Cancel.SetWindowText(_T("CANCEL"));
		break;

	case MB_YESNO:
		m_bn_Ok.SetWindowText(_T("YES"));
		m_bn_Cancel.SetWindowText(_T("NO"));
		break;

	case MB_RETRYCANCEL:
		m_bn_Ok.SetWindowText(_T("RETRY"));
		m_bn_Cancel.SetWindowText(_T("NO"));
		break;

	default:
		m_bn_Ok.SetWindowText(_T("OK"));
		m_bn_Cancel.SetWindowText(_T("CANCEL"));
		break;
	}

	SetBackgroundColor(RGB(255, 255, 255));

	return TRUE;
}

//=============================================================================
// Method		: CreateResource
// Access		: private  
// Returns		: void
// Qualifier	:
// Last Update	: 2022/7/28 - 9:46
// Desc.		:
//=============================================================================
void CDlg_MessageBox::CreateResource()
{
	LOGFONT logFont;
	::ZeroMemory(&logFont, sizeof(LOGFONT));
	logFont.lfHeight = 20;
	logFont.lfWeight = FW_BOLD;
	::lstrcpy(logFont.lfFaceName, (LPCTSTR)_T("Tahoma"));
	m_font.CreateFontIndirect(&logFont);

}

//=============================================================================
// Method		: ReleaseResource
// Access		: private  
// Returns		: void
// Qualifier	:
// Last Update	: 2022/7/28 - 9:46
// Desc.		:
//=============================================================================
void CDlg_MessageBox::ReleaseResource()
{
	m_font.DeleteObject();
}

//=============================================================================
// Method		: Set_Solution
// Access		: public  
// Returns		: void
// Parameter	: LPCTSTR IN_szSolution
// Qualifier	:
// Last Update	: 2022/7/28 - 9:46
// Desc.		:
//=============================================================================
void CDlg_MessageBox::Set_Solution(LPCTSTR IN_szSolution)
{
	m_szSolution = IN_szSolution;
}

//=============================================================================
// Method		: OnOK
// Access		: virtual protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2022/7/28 - 9:46
// Desc.		:
//=============================================================================
void CDlg_MessageBox::OnOK()
{
	switch (m_nMB_Type)
	{
	case MB_YESNO:
		EndDialog(IDYES);
		break;

	case MB_RETRYCANCEL:
		EndDialog(IDRETRY);
		break;

	default:
		CDialogEx::OnOK();
		break;
	}
}

//=============================================================================
// Method		: OnCancel
// Access		: virtual protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2022/7/28 - 9:46
// Desc.		:
//=============================================================================
void CDlg_MessageBox::OnCancel()
{
	switch (m_nMB_Type)
	{
	case MB_YESNO:
		EndDialog(IDNO);
		break;

	default:
		CDialogEx::OnCancel();
		break;
	}
}
