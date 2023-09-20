//*****************************************************************************
// Filename	: 	Wnd_Dummy.cpp
// Created	:	2021/1/26 - 14:21
// Modified	:	2021/1/26 - 14:21
//
// Author	:	piring
//	
// Purpose	:	
//*****************************************************************************
#include "stdafx.h"
#include "Wnd_Dummy.h"

// CWnd_Dummy
IMPLEMENT_DYNAMIC(CWnd_Dummy, CWnd_BaseView)

//=============================================================================
//
//=============================================================================
CWnd_Dummy::CWnd_Dummy()
{
	LOGFONT logFont;
	::ZeroMemory(&logFont, sizeof(LOGFONT));
	logFont.lfHeight = 14;
	logFont.lfWeight = FW_NORMAL;
	::lstrcpy(logFont.lfFaceName, _T("Tahoma"));
	m_font.CreateFontIndirect(&logFont);
}

//=============================================================================
//
//=============================================================================
CWnd_Dummy::~CWnd_Dummy()
{
	m_font.DeleteObject();
}

BEGIN_MESSAGE_MAP(CWnd_Dummy, CWnd_BaseView)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()


//=============================================================================
// Method		: OnCreate
// Access		: protected  
// Returns		: int
// Parameter	: LPCREATESTRUCT lpCreateStruct
// Qualifier	:
// Last Update	: 2021/1/26 - 14:25
// Desc.		:
//=============================================================================
int CWnd_Dummy::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;

	DWORD dwStyle = WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS;


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
// Last Update	: 2021/1/26 - 14:26
// Desc.		:
//=============================================================================
void CWnd_Dummy::OnSize(UINT nType, int cx, int cy)
{
	CWnd_BaseView::OnSize(nType, cx, cy);

	if ((cx == 0) && (cy == 0))
		return;



}

//=============================================================================
// Method		: PreCreateWindow
// Access		: virtual protected  
// Returns		: BOOL
// Parameter	: CREATESTRUCT & cs
// Qualifier	:
// Last Update	: 2021/1/26 - 14:26
// Desc.		:
//=============================================================================
BOOL CWnd_Dummy::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1), NULL);

	return __super::PreCreateWindow(cs);
}

//=============================================================================
// Method		: PreTranslateMessage
// Access		: virtual protected  
// Returns		: BOOL
// Parameter	: MSG * pMsg
// Qualifier	:
// Last Update	: 2021/1/26 - 14:26
// Desc.		:
//=============================================================================
BOOL CWnd_Dummy::PreTranslateMessage(MSG* pMsg)
{
	return __super::PreTranslateMessage(pMsg);
}
