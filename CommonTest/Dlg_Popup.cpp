// Dlg_Popup.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Dlg_Popup.h"
#include "afxdialogex.h"

#define IDC_BN_OK		100
#define IDC_BN_CANCEL	200

// CDlg_Popup 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlg_Popup, CDialogEx)

CDlg_Popup::CDlg_Popup(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlg_Popup::IDD, pParent)
{

	VERIFY(m_font_Default.CreateFont(
		50,						// nHeight
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
		_T("맑은 고딕")));		// lpszFacename
}

CDlg_Popup::~CDlg_Popup()
{
	m_font_Default.DeleteObject();
}

void CDlg_Popup::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlg_Popup, CDialogEx)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BN_OK, &CDlg_Popup::OnBnClickedButtenOK)
	ON_BN_CLICKED(IDC_BN_CANCEL, &CDlg_Popup::OnBnClickedButtenCancel)
END_MESSAGE_MAP()


// CDlg_Popup 메시지 처리기입니다.


void CDlg_Popup::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	m_st_frame.MoveWindow(0, 0, cx, cy - 70);

	m_bn_Ok.MoveWindow(0, cy - 70, (cx / 2) - 2, 70);

	m_bn_Cancel.MoveWindow((cx / 2) + 4, cy - 70, (cx / 2) - 2, 70);
}


int CDlg_Popup::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	DWORD dwStyle = WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	rectDummy.SetRectEmpty();

	CRect rt;
	GetWindowRect(rt);
	::SetWindowPos(GetSafeHwnd(), HWND_TOPMOST, rt.left, rt.top, rt.Width(), rt.Height(), 0);

	m_st_frame.SetBackColor(RGB(255, 237, 237));
	m_st_frame.Create(_T(""), dwStyle | SS_CENTER | SS_CENTERIMAGE | SS_BLACKFRAME, rectDummy, this, 0);
	m_st_frame.SetFont_Gdip(L"맑은 고딕", 16.0f, Gdiplus::FontStyleBold);
	m_st_frame.SetText(m_strMassage);

	m_bn_Cancel.Create(_T("CANCEL"), WS_VISIBLE | WS_CHILD | BS_PUSHLIKE | BS_AUTOCHECKBOX, rectDummy, this, IDC_BN_CANCEL);
	m_bn_Cancel.SetFont(&m_font_Default);

	m_bn_Ok.Create(_T("OK"), WS_VISIBLE | WS_CHILD | BS_PUSHLIKE | BS_AUTOCHECKBOX, rectDummy, this, IDC_BN_OK);
	m_bn_Ok.SetFont(&m_font_Default);

	return 0;
}

void CDlg_Popup::OnBnClickedButtenOK()
{
	CDialogEx::OnOK();
}

void CDlg_Popup::OnBnClickedButtenCancel()
{
	CDialogEx::OnCancel();
}

void CDlg_Popup::PopupMessage(CString str)
{
	m_strMassage = str;
}
