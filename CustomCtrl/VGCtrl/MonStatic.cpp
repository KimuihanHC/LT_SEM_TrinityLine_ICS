//*****************************************************************************
// Filename	: MonStatic.cpp
// Created	: 2013/2/25
// Modified	: 2013/2/25 - 14:29
//
// Author	: PiRing
//	
// Purpose	: 
//*****************************************************************************

#include "stdafx.h"
#include "MonStatic.h"


// CMonStatic

IMPLEMENT_DYNAMIC(CMonStatic, CStatic)

CMonStatic::CMonStatic()
{

}

CMonStatic::~CMonStatic()
{
}


BEGIN_MESSAGE_MAP(CMonStatic, CStatic)
	ON_WM_PAINT()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CMonStatic 메시지 처리기입니다.



//=============================================================================
// Method		: CMonStatic::OnPaint
// Access		: public 
// Returns		: void
// Qualifier	:
// Last Update	: 2013/2/25 - 18:26
// Desc.		:
//=============================================================================
void CMonStatic::OnPaint()
{
	CPaintDC dc(this);
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
// Method		: CMonStatic::OnSize
// Access		: public 
// Returns		: void
// Parameter	: UINT nType
// Parameter	: int cx
// Parameter	: int cy
// Qualifier	:
// Last Update	: 2013/2/25 - 18:26
// Desc.		:
//=============================================================================
void CMonStatic::OnSize(UINT nType, int cx, int cy)
{
	CStatic::OnSize(nType, cx, cy);

	m_bRedrawPanel = TRUE;
}

//=============================================================================
// Method		: CMonStatic::ReDrawPanel
// Access		: virtual public 
// Returns		: void
// Qualifier	:
// Last Update	: 2013/2/25 - 18:26
// Desc.		:
//=============================================================================
void CMonStatic::ReDrawPanel()
{
	CVGCtrlBase::ReDrawPanel();

	if (GetSafeHwnd())
		Invalidate();
}