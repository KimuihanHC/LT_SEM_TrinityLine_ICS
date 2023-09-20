//*****************************************************************************
// Filename	: GroupLogWnd.cpp
// Created	: 2010/12/2
// Modified	: 2010/12/2 - 19:13
//
// Author	: PiRing
//	
// Purpose	: 
//*****************************************************************************
#include "StdAfx.h"
#include "GroupLogWnd.h"

//=============================================================================
//
//=============================================================================
CGroupLogWnd::CGroupLogWnd(void)
{
	LOGFONT logFont;
	::ZeroMemory(&logFont, sizeof(LOGFONT));
	logFont.lfHeight = 12;
	logFont.lfWeight = FW_BOLD;
	::lstrcpy(logFont.lfFaceName, (LPCTSTR)_T("Tahoma"));
	m_Font.CreateFontIndirect(&logFont);
}

//=============================================================================
//
//=============================================================================
CGroupLogWnd::~CGroupLogWnd(void)
{
	m_Font.DeleteObject();
}

BEGIN_MESSAGE_MAP(CGroupLogWnd, CVGGroupWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

//=============================================================================
// Method		: CGroupLogWnd::OnCreate
// Access		: protected 
// Returns		: int
// Parameter	: LPCREATESTRUCT lpCreateStruct
// Qualifier	:
// Last Update	: 2010/12/2 - 19:16
// Desc.		:
//=============================================================================
int CGroupLogWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CVGGroupWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_ed_Log.Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_TABSTOP, CRect(0, 0, 0, 0), this, 1))
	{
		TRACE0("출력 창을 만들지 못했습니다.\n");
		return -1;
	}
	m_ed_Log.SetFont(&m_Font);	
	m_ed_Log.SetBackColor(RGB(0xFF, 0xFF, 0xFF));

	return 0;
}

//=============================================================================
// Method		: CGroupLogWnd::OnSize
// Access		: protected 
// Returns		: void
// Parameter	: UINT nType
// Parameter	: int cx
// Parameter	: int cy
// Qualifier	:
// Last Update	: 2010/12/2 - 19:16
// Desc.		:
//=============================================================================
void CGroupLogWnd::OnSize(UINT nType, int cx, int cy)
{
	CVGGroupWnd::OnSize(nType, cx, cy);
	
	m_ed_Log.MoveWindow(10, 30, cx - 20, cy - 40);
}

//=============================================================================
// Method		: ReDrawPanel
// Access		: virtual protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2017/8/25 - 19:58
// Desc.		:
//=============================================================================
void CGroupLogWnd::ReDrawPanel()
{
	if (GetSafeHwnd())
	{
		m_ed_Log.Invalidate();
	}
}
