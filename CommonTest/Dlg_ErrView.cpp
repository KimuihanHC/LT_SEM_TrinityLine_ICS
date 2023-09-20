// Dlg_ErrView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Dlg_ErrView.h"
#include "afxdialogex.h"

#define IDC_BN_OK	100

// CDlg_ErrView 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlg_ErrView, CDialogEx)

CDlg_ErrView::CDlg_ErrView(CWnd* pParent /*=NULL*/)
: CDialogEx(CDlg_ErrView::IDD, pParent)
{
	m_hTimer_InputCheck = NULL;
	m_hTimerQueue = NULL;

	CreateTimerQueue_Mon();

	m_nErrNum = 0;

	VERIFY(m_font_Default.CreateFont(
		70,						// nHeight
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
		_T("Arial")));			// lpszFacename
}

CDlg_ErrView::~CDlg_ErrView()
{
	m_font_Default.DeleteObject();
	DeleteTimerQueue_Mon();
}

void CDlg_ErrView::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlg_ErrView, CDialogEx)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BN_OK, &CDlg_ErrView::OnBnClickedButtenOK)
END_MESSAGE_MAP()


// CDlg_ErrView 메시지 처리기입니다.

int CDlg_ErrView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rt;
	GetWindowRect(rt);
	DWORD dwStyle = WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	m_st_frame.SetBackColor(RGB(255, 000, 000));
	m_st_frame.Create(_T(""), dwStyle | SS_CENTER | SS_CENTERIMAGE | SS_BLACKFRAME, rectDummy, this, 0);
	m_st_frame.SetFont_Gdip(L"맑은 고딕", 50.0f, Gdiplus::FontStyleBold);
	m_st_frame.SetText(g_szErrorCode_H[m_nErrNum]);

	m_bn_Cancel.Create(_T("O K"), WS_VISIBLE | WS_CHILD | BS_PUSHLIKE | BS_AUTOCHECKBOX, rectDummy, this, IDC_BN_OK);
	::SetWindowPos(GetSafeHwnd(), HWND_TOPMOST, rt.left, rt.top, rt.Width(), rt.Height(), 0);

	return 0;
}

void CDlg_ErrView::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if ((cx == 0) && (cy == 0))
		return;

	m_st_frame.MoveWindow(0, 0, cx, cy - 50);
	m_bn_Cancel.MoveWindow(0, cy - 50, cx, 50);
}

void CDlg_ErrView::ErrMessage(UINT nNum)
{
	m_nErrNum = nNum;
}

//=============================================================================
// Method		: TimerRoutine_InputCheck
// Access		: protected static  
// Returns		: VOID CALLBACK
// Parameter	: __in PVOID lpParam
// Parameter	: __in BOOLEAN TimerOrWaitFired
// Qualifier	:
// Last Update	: 2016/5/29 - 10:55
// Desc.		:
//=============================================================================
VOID CALLBACK CDlg_ErrView::TimerRoutine_InputCheck(__in PVOID lpParam, __in BOOLEAN TimerOrWaitFired)
{
	CDlg_ErrView* pThis = (CDlg_ErrView*)lpParam;

	pThis->OnMonitor_InputCheck();
}

//=============================================================================
// Method		: CreateTimerQueue_Mon
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2016/5/29 - 10:55
// Desc.		:
//=============================================================================
void CDlg_ErrView::CreateTimerQueue_Mon()
{
	__try
	{
		// Create the timer queue.
		m_hTimerQueue = CreateTimerQueue();
		if (NULL == m_hTimerQueue)
		{
			TRACE(_T("CreateTimerQueue failed (%d)\n"), GetLastError());
			return;
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		//	OnAdd_Log(_T("*** Exception Error : CreateTimerQueue_Mon ()"));
	}
}

//=============================================================================
// Method		: DeleteTimerQueue_Mon
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2016/5/29 - 10:55
// Desc.		:
//=============================================================================
void CDlg_ErrView::DeleteTimerQueue_Mon()
{
	// 타이머가 종료될때까지 대기
	__try
	{
		if (!DeleteTimerQueue(m_hTimerQueue))
			TRACE(_T("DeleteTimerQueue failed (%d)\n"), GetLastError());
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		TRACE(_T("*** Exception Error : CDlg_ErrView::DeleteTimerQueue_Mon()\n"));
	}

	TRACE(_T("타이머 종료 : CDlg_ErrView::DeleteTimerQueue_Mon()\n"));
}

//=============================================================================
// Method		: CreateTimer_InputCheck
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2016/5/29 - 10:55
// Desc.		:
//=============================================================================
void CDlg_ErrView::CreateTimer_InputCheck()
{
	__try
	{
		// Time Check Timer
		if (NULL == m_hTimer_InputCheck)
			if (!CreateTimerQueueTimer(&m_hTimer_InputCheck, m_hTimerQueue, (WAITORTIMERCALLBACK)TimerRoutine_InputCheck, (PVOID)this, 100, 600, WT_EXECUTEDEFAULT))
			{
				TRACE(_T("CreateTimerQueueTimer failed (%d)\n"), GetLastError());
			}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		TRACE(_T("*** Exception Error : CDlg_ErrView::CreateTimer_InputCheck()\n"));
	}
}

//=============================================================================
// Method		: DeleteTimer_InputCheck
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2016/5/29 - 10:55
// Desc.		:
//=============================================================================
void CDlg_ErrView::DeleteTimer_InputCheck()
{
	__try
	{
		if (DeleteTimerQueueTimer(m_hTimerQueue, m_hTimer_InputCheck, NULL))
		{
			m_hTimer_InputCheck = NULL;
		}
		else
		{
			TRACE(_T("DeleteTimerQueueTimer : m_hTimer_InputCheck failed (%d)\n"), GetLastError());
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		TRACE(_T("*** Exception Error : CDlg_ErrView::DeleteTimer_InputCheck()\n"));
	}
}

//=============================================================================
// Method		: OnMonitor_InputCheck
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2016/5/29 - 10:55
// Desc.		:
//=============================================================================
void CDlg_ErrView::OnMonitor_InputCheck()
{
	if (GetSafeHwnd())
	{
		if (IsWindowVisible())
		{
			m_st_frame.SetTextColor(Color::Red, Color::Red);
			m_st_frame.SetBackColor(RGB(255, 255, 255));
		}
	}
	
	Sleep(100);

	if (GetSafeHwnd())
	{
		if (IsWindowVisible())
		{
			m_st_frame.SetTextColor(Color::White, Color::White);
			m_st_frame.SetBackColor(RGB(255, 000, 000));
		}
	}
}

//=============================================================================
// Method		: OnShowWindow
// Access		: public  
// Returns		: void
// Parameter	: BOOL bShow
// Parameter	: UINT nStatus
// Qualifier	:
// Last Update	: 2016/11/9 - 21:02
// Desc.		:
//=============================================================================
void CDlg_ErrView::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (bShow)
		CreateTimer_InputCheck();
	else
		DeleteTimer_InputCheck();
}

void CDlg_ErrView::OnBnClickedButtenOK()
{
	OnCancel();
}

void CDlg_ErrView::OnCancel()
{
	CDialogEx::OnCancel();
}
