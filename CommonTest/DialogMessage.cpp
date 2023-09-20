// DialogMessage.cpp: 구현 파일
//

#include "stdafx.h"
#include "resource.h"
#include "DialogMessage.h"

#include "VGStatic.h"

#define IDC_BUTTON_RESOURCE (10000)

// DialogMessage 대화 상자
IMPLEMENT_DYNAMIC(CDialogMessage, CDialogEx)

CDialogMessage::CDialogMessage(LPCTSTR IN_szTitle, LPCTSTR IN_szText, UINT IN_nMB_Type /*= MB_OK*/, CWnd* pParent /*=nullptr*/)
: CDialogEx(IDD_DIALOG_MESSAGE, pParent)
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

CDialogMessage::CDialogMessage(LPCTSTR IN_szText, UINT IN_nMB_Type /*= MB_OK*/, CWnd* pParent /*=nullptr*/)
: CDialogEx(IDD_DIALOG_MESSAGE, pParent)
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

CDialogMessage::~CDialogMessage()
{
	ReleaseResource();
}

BEGIN_MESSAGE_MAP(CDialogMessage, CDialogEx)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

void CDialogMessage::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);

	// static
	DDX_Control(pDX, IDC_MESSAGE_STATIC_TITLE, *m_pst_Title);
	DDX_Control(pDX, IDC_MESSAGE_STATIC_SUBTITLE, *m_pst_Text);
	DDX_Control(pDX, IDC_MESSAGE_STATIC_LINE, *m_pst_Line);
	
	// button
	DDX_Control(pDX, IDOK, *m_pbn_Ok);
	DDX_Control(pDX, IDCANCEL, *m_pbn_Cancel);
}

// DialogMessage 메시지 처리기
BOOL CDialogMessage::OnInitDialog()
{
	__super::OnInitDialog();

	switch (m_nMB_Type)
	{
	case MB_OK:
		m_pbn_Ok->SetWindowText(_T("OK"));
		m_pbn_Cancel->ShowWindow(SW_HIDE);
		break;

	case MB_OKCANCEL:
		m_pbn_Ok->SetWindowText(_T("OK"));
		m_pbn_Cancel->SetWindowText(_T("CANCEL"));
		break;

	case MB_YESNO:
		m_pbn_Ok->SetWindowText(_T("YES"));
		m_pbn_Cancel->SetWindowText(_T("NO"));
		break;

	case MB_RETRYCANCEL:
		m_pbn_Ok->SetWindowText(_T("RETRY"));
		m_pbn_Cancel->SetWindowText(_T("NO"));
		break;

	default:
		m_pbn_Ok->SetWindowText(_T("OK"));
		m_pbn_Cancel->SetWindowText(_T("CANCEL"));
		break;
	}

	SetBackgroundColor(RGB(255, 255, 255));

	StyleResource();
	
	RecalcLayout();

	return TRUE;  
}

int CDialogMessage::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	DWORD dwStyle = WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	CRect rectDummy;
	rectDummy.SetRectEmpty();




	return 0;
}

void CDialogMessage::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);
}

void CDialogMessage::OnDestroy()
{
	__super::OnDestroy();
}

void CDialogMessage::CreateResource()
{
	LOGFONT logFont;
	::ZeroMemory(&logFont, sizeof(LOGFONT));
	logFont.lfHeight = 20;
	logFont.lfWeight = FW_BOLD;
	::lstrcpy(logFont.lfFaceName, (LPCTSTR)_T("Tahoma"));
	m_font.CreateFontIndirect(&logFont);

	//m_pst_Background = new CVGStatic();
	m_pst_Line = new CVGStatic();

	m_pst_Title = new CVGStatic();
	m_pst_Text = new CVGStatic();

	m_pbn_Ok = new CButton();
	m_pbn_Cancel = new CButton();
}

void CDialogMessage::ReleaseResource()
{
	m_font.DeleteObject();

	//delete m_pst_Background;
	delete m_pst_Line;

	delete m_pst_Title;
	delete m_pst_Text;
	
	delete m_pbn_Ok;
	delete m_pbn_Cancel;
}

void CDialogMessage::StyleResource()
{
	//CRect rt;
	//GetClientRect(rt);
	//m_pst_Background->SetBorderTickness(1);
	//m_pst_Background->SetBorderColor_COLORREF(RGB(184, 184, 184));
	//m_pst_Background->Create(nullptr, WS_VISIBLE | WS_CLIPSIBLINGS, rt, this, IDC_STATIC);
	
	// static
	m_pst_Title->SetSpacing(15);
	m_pst_Title->SetUseBorder(FALSE);
	m_pst_Title->SetTextAlignment(StringAlignmentNear);
	m_pst_Title->SetFont_Gdip(L"Tahoma", 25.0F, FontStyleBoldItalic);
	m_pst_Title->SetText(m_szTitle);
	m_pst_Title->ModifyStyle(WS_BORDER, NULL);
	m_pst_Title->ModifyStyleEx(WS_EX_CLIENTEDGE, NULL, SWP_FRAMECHANGED);

	m_pst_Line->SetStaticStyle(CVGStatic::StaticStyle_Default);
	m_pst_Line->SetColorStyle(CVGStatic::ColorStyle_Red);
	m_pst_Line->SetBorderTickness(0);
	m_pst_Line->ModifyStyle(WS_BORDER, NULL);
	m_pst_Line->ModifyStyleEx(WS_EX_CLIENTEDGE, NULL, SWP_FRAMECHANGED);

	m_pst_Text->SetColorStyle(CVGStatic::ColorStyle_Default);
	m_pst_Text->SetSpacing(15);
	m_pst_Text->SetTextAlignment(StringAlignmentNear);
	m_pst_Text->SetFont_Gdip(L"Tahoma", 15.0F);
	m_pst_Text->SetText(m_szText);
	m_pst_Text->ModifyStyle(WS_BORDER, NULL);
	m_pst_Text->ModifyStyleEx(WS_EX_CLIENTEDGE, NULL, SWP_FRAMECHANGED);

	m_pbn_Ok->SetFont(&m_font);
	m_pbn_Cancel->SetFont(&m_font);
}

void CDialogMessage::RecalcLayout()
{
	CRect rt;

	switch (m_nMB_Type)
	{
	case MB_OK:
		m_pbn_Cancel->GetWindowRect(rt);
		m_pbn_Ok->MoveWindow(rt.left, rt.top, rt.Width(), rt.Height());
		break;

	case MB_OKCANCEL:
	case MB_YESNO:
	case MB_RETRYCANCEL:
	default:
		break;
	}
}

void CDialogMessage::OnOK()
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

void CDialogMessage::OnCancel()
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
