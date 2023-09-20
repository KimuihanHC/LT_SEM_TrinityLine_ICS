//*****************************************************************************
// Filename	: Dlg_TransferProgress.cpp
// Created	: 2011/1/6
// Modified	: 2011/1/6 - 10:36
//
// Author	: PiRing
//	
// Purpose	: 
//*****************************************************************************
// Dlg_TransferProgress.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Dlg_TransferProgress.h"

//=============================================================================
// CDlg_TransferProgress 대화 상자입니다.
//=============================================================================
IMPLEMENT_DYNAMIC(CDlg_TransferProgress, CDialogEx)

//=============================================================================
//
//=============================================================================
CDlg_TransferProgress::CDlg_TransferProgress(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlg_TransferProgress::IDD, pParent)
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
}

CDlg_TransferProgress::~CDlg_TransferProgress()
{
	m_font_Default.DeleteObject();
}

void CDlg_TransferProgress::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ST_DLG_TRANS_STATE, m_st_State);
	DDX_Control(pDX, IDC_LIST_DLG_TRANS_MSG, m_list_Msg);
}


BEGIN_MESSAGE_MAP(CDlg_TransferProgress, CDialogEx)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CDlg_TransferProgress 메시지 처리기입니다.

//=============================================================================
// Method		: CDlg_TransferProgress::OnCtlColor
// Access		: public 
// Returns		: HBRUSH
// Parameter	: CDC * pDC
// Parameter	: CWnd * pWnd
// Parameter	: UINT nCtlColor
// Qualifier	:
// Last Update	: 2011/1/6 - 10:38
// Desc.		:
//=============================================================================
HBRUSH CDlg_TransferProgress::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
// Method		: CDlg_TransferProgress::OnInitDialog
// Access		: public 
// Returns		: BOOL
// Qualifier	:
// Last Update	: 2011/1/6 - 10:37
// Desc.		:
//=============================================================================
BOOL CDlg_TransferProgress::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetBackgroundColor(RGB(0xFF, 0xFF, 0xFF));

	m_st_State.ModifyStyleEx(WS_EX_CLIENTEDGE | WS_EX_CLIENTEDGE, NULL, SWP_FRAMECHANGED);
	m_st_State.ModifyStyle (NULL, WS_BORDER, SWP_FRAMECHANGED);

	m_st_State.SetFont(&m_font_Default);
	
	POSITION pos;
	for (pos = m_strMessage.GetHeadPosition(); pos != NULL;)
	{
		m_list_Msg.AddString (m_strMessage.GetNext(pos));
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

//=============================================================================
// Method		: CDlg_TransferProgress::PreTranslateMessage
// Access		: virtual protected 
// Returns		: BOOL
// Parameter	: MSG * pMsg
// Qualifier	:
// Last Update	: 2011/1/6 - 10:43
// Desc.		:
//=============================================================================
BOOL CDlg_TransferProgress::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
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
// Method		: CDlg_TransferProgress::SetInit
// Access		: public 
// Returns		: void
// Qualifier	:
// Last Update	: 2011/1/6 - 10:49
// Desc.		:
//=============================================================================
void CDlg_TransferProgress::SetInit()
{
	if (m_st_State.GetSafeHwnd())
		m_st_State.SetWindowText (_T(""));

	if (m_list_Msg.GetSafeHwnd())
		m_list_Msg.ResetContent();
}

//=============================================================================
// Method		: CDlg_TransferProgress::SetStatus
// Access		: public 
// Returns		: void
// Parameter	: LPCTSTR lpszStatus
// Qualifier	:
// Last Update	: 2011/1/6 - 10:49
// Desc.		:
//=============================================================================
void CDlg_TransferProgress::SetStatus( LPCTSTR lpszStatus )
{
	m_st_State.SetWindowText (lpszStatus);
}

//=============================================================================
// Method		: CDlg_TransferProgress::AddMessage
// Access		: public 
// Returns		: void
// Parameter	: LPCTSTR lpszMsg
// Qualifier	:
// Last Update	: 2011/1/6 - 10:49
// Desc.		:
//=============================================================================
void CDlg_TransferProgress::AddMessage( LPCTSTR lpszMsg )
{
	m_strMessage.AddTail(lpszMsg);
}

//=============================================================================
// Method		: CDlg_TransferProgress::AddMessageList
// Access		: public 
// Returns		: void
// Parameter	: CStringList & strMessage
// Qualifier	:
// Last Update	: 2011/1/6 - 15:30
// Desc.		:
//=============================================================================
void CDlg_TransferProgress::AddMessageList( CStringList& strMessage )
{
	m_strMessage.AddTail(&strMessage);	
}