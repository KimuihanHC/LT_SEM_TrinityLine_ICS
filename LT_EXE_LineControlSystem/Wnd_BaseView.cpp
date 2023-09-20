//*****************************************************************************
// Filename	: Wnd_BaseView.cpp
// Created	: 2010/11/23
// Modified	: 2010/11/23 - 10:02
//
// Author	: PiRing
//	
// Purpose	: 
//*****************************************************************************
// Wnd_BaseView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Wnd_BaseView.h"
#include "Def_Language.h"

//=============================================================================
// CWnd_BaseView
//=============================================================================
IMPLEMENT_DYNAMIC(CWnd_BaseView, CWnd)

//=============================================================================
// 생성자
//=============================================================================
CWnd_BaseView::CWnd_BaseView()
{
	/*SetRegistryPath(REG_PATH_APP_BASE);*/
}

//=============================================================================
// 소멸자
//=============================================================================
CWnd_BaseView::~CWnd_BaseView()
{
	
}

BEGIN_MESSAGE_MAP(CWnd_BaseView, CWnd)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


//=============================================================================
// CWnd_BaseView 메시지 처리기
//=============================================================================
//=============================================================================
// Method		: CWnd_BaseView::PreCreateWindow
// Access		: virtual protected 
// Returns		: BOOL
// Parameter	: CREATESTRUCT & cs
// Qualifier	:
// Last Update	: 2010/11/23 - 10:02
// Desc.		:
//=============================================================================
BOOL CWnd_BaseView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	//cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

//=============================================================================
// Method		: CWnd_BaseView::OnCreate
// Access		: public 
// Returns		: int
// Parameter	: LPCREATESTRUCT lpCreateStruct
// Qualifier	:
// Last Update	: 2010/11/23 - 10:02
// Desc.		:
//=============================================================================
int CWnd_BaseView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	//SetBackgroundColor (RGB(0x0, 0x0, 0x0));
	
	return 0;
}

//=============================================================================
// Method		: CWnd_BaseView::OnEraseBkgnd
// Access		: protected 
// Returns		: BOOL
// Parameter	: CDC * pDC
// Qualifier	:
// Last Update	: 2010/11/23 - 10:02
// Desc.		:
//=============================================================================
BOOL CWnd_BaseView::OnEraseBkgnd(CDC* pDC)
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
// Method		: CWnd_BaseView::OnCtlColor
// Access		: protected 
// Returns		: HBRUSH
// Parameter	: CDC * pDC
// Parameter	: CWnd * pWnd
// Parameter	: UINT nCtlColor
// Qualifier	:
// Last Update	: 2010/11/23 - 10:02
// Desc.		:
//=============================================================================
HBRUSH CWnd_BaseView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
// Method		: CWnd_BaseView::SetBackgroundColor
// Access		: protected 
// Returns		: void
// Parameter	: COLORREF color
// Parameter	: BOOL bRepaint
// Qualifier	:
// Last Update	: 2010/11/23 - 10:02
// Desc.		:
//=============================================================================
void CWnd_BaseView::SetBackgroundColor( COLORREF color, BOOL bRepaint /*= TRUE*/ )
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
// Method		: CWnd_BaseView::SetUseBlurSubWnd
// Access		: public 
// Returns		: void
// Parameter	: BOOL bUse
// Qualifier	:
// Last Update	: 2010/11/23 - 10:02
// Desc.		: 서브 윈도우의 블러효과 사용여부
//=============================================================================
void CWnd_BaseView::SetUseBlurSubWnd( BOOL bUse /*= TRUE*/ )
{

}

//=============================================================================
// Method		: OnLanguage
// Access		: virtual public  
// Returns		: void
// Parameter	: __in uint8_t IN_nLanguage
// Qualifier	:
// Last Update	: 2022/7/18 - 13:45
// Desc.		:
//=============================================================================
void CWnd_BaseView::OnLanguage(__in uint8_t IN_nLanguage)
{
	if (IN_nLanguage < enLanguage::Lang_MaxCount)
	{
		m_nLanguage = IN_nLanguage;
	}
}
