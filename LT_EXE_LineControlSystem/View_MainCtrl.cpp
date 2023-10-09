//*****************************************************************************
// Filename	: 	View_MainCtrl.cpp
// Created	:	2021/1/29 - 14:28
// Modified	:	2021/1/29 - 14:28
//
// Author	:	piring
//	
// Purpose	:	
//*****************************************************************************
// View_MainCtrl.cpp : CView_MainCtrl 클래스의 구현
//

#include "stdafx.h"
#include "resource.h"

#include "View_MainCtrl.h"
#include "Def_CompileOption.h"
#include "CommonFunction.h"
#include "Pane_CommStatus.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//=============================================================================
// CView_MainCtrl 생성자
//=============================================================================
CView_MainCtrl::CView_MainCtrl()
{
	// 윈도우 포인터	
	m_nWndIndex				= -1;
		
	for (int iCnt = 0; iCnt < SUBVIEW_MAX; iCnt++)
	{
		m_pWndPtr[iCnt] = NULL;
	}
	
	m_pwndCommPane			= NULL;

	OnInit_ConstructionSetting();
}

//=============================================================================
// CView_MainCtrl 소멸자
//=============================================================================
CView_MainCtrl::~CView_MainCtrl()
{
	TRACE(_T("<<< Start ~CView_MainCtrl >>> \n"));

	

	TRACE(_T("<<< End ~CView_MainCtrl >>> \n"));
}


BEGIN_MESSAGE_MAP(CView_MainCtrl, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
END_MESSAGE_MAP()


//=============================================================================
// CView_MainCtrl 메시지 처리기
//=============================================================================
//=============================================================================
// Method		: CView_MainCtrl::PreCreateWindow
// Access		: virtual protected 
// Returns		: BOOL
// Parameter	: CREATESTRUCT & cs
// Qualifier	:
// Last Update	: 2010/11/26 - 13:59
// Desc.		:
//=============================================================================
BOOL CView_MainCtrl::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

//=============================================================================
// Method		: Create_SubWindows
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2022/2/15 - 16:33
// Desc.		:
//=============================================================================
void CView_MainCtrl::Create_SubWindows()
{
	CString strClassTester = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW, 0, (HBRUSH)(COLOR_WINDOW + 1));

	if (!m_ViewSub.CreateEx(WS_EX_DLGMODALFRAME, strClassTester, _T(""), WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, CRect(0, 0, 0, 0), GetDesktopWindow(), NULL))
	{
		TRACE0("출력 창을 만들지 못했습니다.\n");
	}
	m_ViewSub.SetOwner(this);
}

//=============================================================================
// Method		: ShowWindow_SubMonitoring
// Access		: virtual protected  
// Returns		: void
// Parameter	: __in BOOL bShow
// Qualifier	:
// Last Update	: 2022/2/15 - 17:23
// Desc.		:
//=============================================================================
void CView_MainCtrl::ShowWindow_SubMonitoring(__in BOOL bShow)
{
	if (bShow)
	{
		GetMonitorInformation(m_infoMonitor);

		int iMonCnt = m_infoMonitor.nMonitor;
		int iResWidth = GetSystemMetrics(SM_CXSCREEN);
		int iResHeight = GetSystemMetrics(SM_CYSCREEN);

		UINT iMtrIdx = 2;
		if (2 <= iMonCnt)
		{
			iMtrIdx = 1;
		}
		else if (1 <= iMonCnt)
		{
			iMtrIdx = 0;
		}

		RECT rt = m_infoMonitor.aryMonitors.GetAt(iMtrIdx).rcMonitor;
		RECT rtWork = m_infoMonitor.aryMonitors.GetAt(iMtrIdx).rcWork;

		//m_ViewSub.MoveWindow(rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top);
		m_ViewSub.MoveWindow(rt.left, rt.top, 1920, 1080);
		m_ViewSub.ShowWindow(SW_SHOWMAXIMIZED);
	}
	else
	{
		m_ViewSub.ShowWindow(SW_HIDE);
	}
}

//=============================================================================
// Method		: Update_Language
// Access		: protected  
// Returns		: void
// Parameter	: __in uint8_t IN_nLanguage
// Qualifier	:
// Last Update	: 2022/7/18 - 14:21
// Desc.		:
//=============================================================================
void CView_MainCtrl::Update_Language(__in uint8_t IN_nLanguage)
{
	if (m_nLanguage != IN_nLanguage)
	{
		m_nLanguage = IN_nLanguage;

		m_wndMainView.OnLanguage(IN_nLanguage);
		m_wndSocketView.OnLanguage(IN_nLanguage);
		m_wndSettingView.OnLanguage(IN_nLanguage);
		m_wndLogView.OnLanguage(IN_nLanguage);

		m_ViewSub.OnLanguage(IN_nLanguage);
		if (m_pwndCommPane)
		{
			((CPane_CommStatus*)m_pwndCommPane)->OnLanguage(IN_nLanguage);
		}
	}
}

//=============================================================================
// Method		: CView_MainCtrl::OnPaint
// Access		: protected 
// Returns		: void
// Qualifier	:
// Last Update	: 2010/11/26 - 14:00
// Desc.		:
//=============================================================================
void CView_MainCtrl::OnPaint() 
{
	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트
	
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	
	// 그리기 메시지에 대해서는 CWnd::OnPaint()를 호출하지 마십시오.
}

//=============================================================================
// Method		: CView_MainCtrl::OnCreate
// Access		: protected 
// Returns		: int
// Parameter	: LPCREATESTRUCT lpCreateStruct
// Qualifier	:
// Last Update	: 2010/11/26 - 14:06
// Desc.		:
//=============================================================================
int CView_MainCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	//m_wndMainView.SetBackgroundColor(RGB(0xFF, 0xFF, 0xFF));
	//m_wndMainView.SetBackgroundColor(RGB(0, 0, 0));

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	UINT nWnd_ID = AFX_IDW_PANE_FIRST + 1;

	if (!m_wndMainView.Create(NULL, NULL, WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, rectDummy, this, nWnd_ID, NULL))
	{
		TRACE0("m_wndMainView 뷰 창을 만들지 못했습니다.\n");
		return -1;
	}
	
	if (!m_wndSocketView.Create(NULL, NULL, WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, rectDummy, this, ++nWnd_ID, NULL))
	{
		TRACE0("m_wndSequenceView 뷰 창을 만들지 못했습니다.\n");
		return -1;
	}

	if (!m_wndSettingView.Create(NULL, NULL, WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, rectDummy, this, ++nWnd_ID, NULL))
	{
		TRACE0("m_wndRecipeView 뷰 창을 만들지 못했습니다.\n");
		return -1;
	}

	if (!m_wndLogView.Create(NULL, NULL, WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, rectDummy, this, ++nWnd_ID, NULL))
	{
		TRACE0("m_wndLogView 뷰 창을 만들지 못했습니다.\n");
		return -1;
	}	

	m_wndMainView.ShowWindow(SW_SHOW);

	auto nWndIdx = 0;
	m_pWndPtr[nWndIdx++]	= (CWnd*)&m_wndMainView;		// Main 화면
	m_pWndPtr[nWndIdx++]	= (CWnd*)&m_wndSocketView;		// 소켓 정보
	m_pWndPtr[nWndIdx++]	= (CWnd*)&m_wndSettingView;		// 레시피 설정
	m_pWndPtr[nWndIdx++]	= (CWnd*)&m_wndLogView;			// 로그

	//SetBackgroundColor (RGB(0x0, 0x0, 0x0));

	Create_SubWindows();

	// 초기 세팅
 	OnInit_UISetting ();
 	OnInit_DeviceSetting();


	return 0;
}

//=============================================================================
// Method		: CView_MainCtrl::OnSize
// Access		: protected 
// Returns		: void
// Parameter	: UINT nType
// Parameter	: int cx
// Parameter	: int cy
// Qualifier	:
// Last Update	: 2010/11/26 - 14:06
// Desc.		:
//=============================================================================
void CView_MainCtrl::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	if ((0 == cx) || (0 == cy))
		return;

	m_wndMainView.MoveWindow(0, 0, cx, cy);
	m_wndSocketView.MoveWindow(0, 0, cx, cy);
	m_wndSettingView.MoveWindow(0, 0, cx, cy);
	m_wndLogView.MoveWindow(0, 0, cx, cy);
}

//=============================================================================
// Method		: CView_MainCtrl::OnEraseBkgnd
// Access		: protected 
// Returns		: BOOL
// Parameter	: CDC * pDC
// Qualifier	:
// Last Update	: 2010/10/13 - 10:40
// Desc.		:
//=============================================================================
BOOL CView_MainCtrl::OnEraseBkgnd(CDC* pDC)
{
	if (m_brBkgr.GetSafeHandle() == NULL)
	{
		return CWnd::OnEraseBkgnd(pDC);
	}

	ASSERT_VALID(pDC);

	CRect rectClient;
	GetClientRect(rectClient);

	if (m_brBkgr.GetSafeHandle() != NULL)
	{
		pDC->FillRect(rectClient, &m_brBkgr);
	}
	else
	{
		CWnd::OnEraseBkgnd(pDC);
	}

	return TRUE;
}

//=============================================================================
// Method		: CView_MainCtrl::OnCtlColor
// Access		: protected 
// Returns		: HBRUSH
// Parameter	: CDC * pDC
// Parameter	: CWnd * pWnd
// Parameter	: UINT nCtlColor
// Qualifier	:
// Last Update	: 2010/10/12 - 17:35
// Desc.		:
//=============================================================================
HBRUSH CView_MainCtrl::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
				return(HBRUSH) ::GetStockObject(HOLLOW_BRUSH);
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

	return CWnd::OnCtlColor(pDC, pWnd, nCtlColor);
}

//=============================================================================
// Method		: CView_MainCtrl::OnTimer
// Access		: protected 
// Returns		: void
// Parameter	: UINT_PTR nIDEvent
// Qualifier	:
// Last Update	: 2010/12/9 - 15:57
// Desc.		:
//=============================================================================
void CView_MainCtrl::OnTimer(UINT_PTR nIDEvent)
{
	// 타이머 처리
	

	CWnd::OnTimer(nIDEvent);
}

//=============================================================================
// Method		: CView_MainCtrl::SetBackgroundColor
// Access		: protected 
// Returns		: void
// Parameter	: COLORREF color
// Parameter	: BOOL bRepaint
// Qualifier	:
// Last Update	: 2010/10/13 - 10:40
// Desc.		:
//=============================================================================
void CView_MainCtrl::SetBackgroundColor(COLORREF color, BOOL bRepaint /*= TRUE*/)
{
	if (m_brBkgr.GetSafeHandle() != NULL)
	{
		m_brBkgr.DeleteObject();
	}

	if (color != (COLORREF)-1)
	{
		m_brBkgr.CreateSolidBrush(color);
	}

	if (bRepaint && GetSafeHwnd() != NULL)
	{
		Invalidate();
		UpdateWindow();
	}
}

//=============================================================================
// Method		: CView_MainCtrl::SwitchWindow
// Access		: public 
// Returns		: UINT
// Parameter	: UINT nIndex
// Qualifier	:
// Last Update	: 2010/11/26 - 14:06
// Desc.		: 자식 윈도우 전환하는 함수
// MainView에서 선택된 검사기 번호를 다른 윈도우로 넘긴다.
//=============================================================================
UINT CView_MainCtrl::SwitchWindow(UINT nIndex)
{
	if (m_nWndIndex == nIndex)
		return m_nWndIndex;

	UINT nOldView = m_nWndIndex;
	m_nWndIndex = nIndex;

	if (nOldView != -1)
	{
		if (m_pWndPtr[nOldView]->GetSafeHwnd())
		{
			m_pWndPtr[nOldView]->ShowWindow(SW_HIDE);
		}
	}

	if (m_pWndPtr[m_nWndIndex]->GetSafeHwnd())
	{
		m_pWndPtr[m_nWndIndex]->ShowWindow(SW_SHOW);
	}

	if ((SUBVIEW_LOG == m_nWndIndex))
 	{
 		m_wndLogView.Refresh_UI();
 	}

	return m_nWndIndex;
}

//=============================================================================
// Method		: CView_MainCtrl::SetCommPanePtr
// Access		: public 
// Returns		: void
// Parameter	: CWnd * pwndCommPane
// Qualifier	:
// Last Update	: 2013/7/16 - 16:51
// Desc.		:
//=============================================================================
void CView_MainCtrl::SetCommPanePtr(CWnd* pwndCommPane)
{
	m_pwndCommPane = pwndCommPane;
	if (m_pwndCommPane)
	{
		((CPane_CommStatus*)m_pwndCommPane)->OnLanguage(m_nLanguage);
	}
}

