//*****************************************************************************
// Filename	: VS2008Panel.cpp
// Created	: 2013/2/19
// Modified	: 2013/2/19 - 14:59
//
// Author	: PiRing
//	
// Purpose	: 
//*****************************************************************************

#include "stdafx.h"
#include "VS2008Panel.h"


// CVS2008Panel

IMPLEMENT_DYNAMIC(CVS2008Panel, CWnd)

//=============================================================================
//
//=============================================================================
CVS2008Panel::CVS2008Panel()
{
	
}

CVS2008Panel::~CVS2008Panel()
{
	
}


BEGIN_MESSAGE_MAP(CVS2008Panel, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CVS2008Panel 메시지 처리기입니다.
//=============================================================================
// Method		: CVS2008Panel::OnSize
// Access		: public 
// Returns		: void
// Parameter	: UINT nType
// Parameter	: int cx
// Parameter	: int cy
// Qualifier	:
// Last Update	: 2013/2/20 - 17:09
// Desc.		:
//=============================================================================
void CVS2008Panel::OnSize(UINT nType, int cx, int cy)
{	
	CWnd::OnSize(nType, cx, cy);

	m_bRedrawPanel = TRUE;	
}

//=============================================================================
// Method		: CVS2008Panel::OnPaint
// Access		: protected 
// Returns		: void
// Qualifier	:
// Last Update	: 2013/2/20 - 16:51
// Desc.		:
//=============================================================================
void CVS2008Panel::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	Graphics graphics(dc.GetSafeHdc());	

	if((m_pCachedBitmap == NULL) || (m_bRedrawPanel))
	{
		CRect rect;
		GetClientRect(&rect);
		PaintCachedBitmap (graphics, Rect(rect.left, rect.top, rect.Width(), rect.Height()));
	}	

	graphics.DrawCachedBitmap(m_pCachedBitmap, 0, 0);
	m_bRedrawPanel = FALSE;
}

//=============================================================================
// Method		: CVS2008Panel::ReDrawPanel
// Access		: virtual protected 
// Returns		: void
// Qualifier	:
// Last Update	: 2013/2/20 - 17:09
// Desc.		:
//=============================================================================
void CVS2008Panel::ReDrawPanel()
{
	CVS2008PanelBase::ReDrawPanel();

	if (GetSafeHwnd())
		Invalidate();
}
