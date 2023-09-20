//*****************************************************************************
// Filename	: 	VGProgressBar.cpp
// Created	:	2016/3/10 - 18:24
// Modified	:	2016/3/10 - 18:24
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
#include "stdafx.h"
#include "VGProgressBar.h"


CVGProgressBar::CVGProgressBar()
{
	m_nEndPos		= 100;
	m_nStartPos		= 0;
	m_fPos			= 0;
	m_fStepAmount	= 1;
}


CVGProgressBar::~CVGProgressBar()
{
}
BEGIN_MESSAGE_MAP(CVGProgressBar, CWnd)
	ON_WM_PAINT()
	//ON_MESSAGE(PBM_FLOAT_SETSTEP, OnSetStepF)
	//ON_MESSAGE(PBM_FLOAT_SETPOS, OnSetPosF)
	ON_MESSAGE(PBM_GETPOS, OnGetPos)
	ON_MESSAGE(PBM_SETRANGE32, OnSetRange)
	ON_MESSAGE(PBM_STEPIT, OnStep)
END_MESSAGE_MAP()


//=============================================================================
// Method		: OnPaint
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2016/7/19 - 10:05
// Desc.		:
//=============================================================================
void CVGProgressBar::OnPaint()
{
	CPaintDC dc(this); // device context for painting

 	Graphics graphics(dc.GetSafeHdc());
 
 	if ((m_pCachedBitmap == NULL) || (m_bRedrawPanel))
 	{
 		CRect rect;
 		GetClientRect(&rect);
 		PaintCachedBitmap(graphics, Rect(rect.left, rect.top, rect.Width(), rect.Height()));
 	}
 
 	graphics.DrawCachedBitmap(m_pCachedBitmap, 0, 0);
 	m_bRedrawPanel = FALSE;


	/*Graphics Graph(dc);
	
	CRect rcClient;
	GetClientRect(&rcClient);
	RectF rect((REAL)rcClient.left, (REAL)rcClient.top, (REAL)rcClient.Width(), (REAL)rcClient.Height());

	RenderBar(Graph, rect);
	*/
}

//=============================================================================
// Method		: OnStep
// Access		: virtual protected  
// Returns		: LRESULT
// Parameter	: WPARAM wParam
// Parameter	: LPARAM lParam
// Qualifier	:
// Last Update	: 2016/7/19 - 10:16
// Desc.		:
//=============================================================================
LRESULT CVGProgressBar::OnStep(WPARAM wParam, LPARAM lParam)
{
	Step();
	return 1;
}

LRESULT CVGProgressBar::OnSetRange(WPARAM wParam, LPARAM lParam)
{
	SetRange((UINT)wParam, (UINT)lParam);
	return 1;
}

LRESULT CVGProgressBar::OnGetPos(WPARAM wParam, LPARAM lParam)
{
	return (UINT)GetPosition();
}

LRESULT CVGProgressBar::OnSetPosF(WPARAM wParam, LPARAM lParam)
{
	SetPosition(*(float*)wParam);
	return 1;
}

LRESULT CVGProgressBar::OnSetStepF(WPARAM wParam, LPARAM lParam)
{
	SetStepAmount(*(float*)wParam);
	return 1;
}

//=============================================================================
// Method		: ReDrawPanel
// Access		: virtual protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2016/7/19 - 10:16
// Desc.		:
//=============================================================================
void CVGProgressBar::ReDrawPanel()
{
	__super::ReDrawPanel();

	if (GetSafeHwnd())
		Invalidate();
}

//=============================================================================
// Method		: GetStepAmount
// Access		: public  
// Returns		: float
// Qualifier	: const
// Last Update	: 2016/7/19 - 10:16
// Desc.		:
//=============================================================================
float CVGProgressBar::GetStepAmount() const
{
	return m_fStepAmount;
}

//=============================================================================
// Method		: Step
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2016/7/19 - 10:16
// Desc.		:
//=============================================================================
void CVGProgressBar::Step()
{
	m_fPos += m_fStepAmount;

	if (m_fPos < m_nStartPos)
		m_fPos = (float)m_nStartPos;
	if (m_fPos > m_nEndPos)
		m_fPos = (float)m_nEndPos;

	//Invalidate();
	ReDrawPanel();
}

//=============================================================================
// Method		: SetStepAmount
// Access		: public  
// Returns		: void
// Parameter	: __in float fStepAmount
// Qualifier	:
// Last Update	: 2016/7/19 - 10:16
// Desc.		:
//=============================================================================
void CVGProgressBar::SetStepAmount(__in float fStepAmount)
{
	m_fStepAmount = fStepAmount;
}

//=============================================================================
// Method		: SetPosition
// Access		: public  
// Returns		: void
// Parameter	: __in float fPos
// Qualifier	:
// Last Update	: 2016/7/19 - 10:16
// Desc.		:
//=============================================================================
void CVGProgressBar::SetPosition(__in float fPos)
{
	if (fPos < m_nStartPos)
		fPos = (float)m_nStartPos;
	if (fPos > m_nEndPos)
		fPos = (float)m_nEndPos;

	m_fPos = fPos;
	
	//Invalidate();
	ReDrawPanel();
}

//=============================================================================
// Method		: GetPosition
// Access		: public  
// Returns		: float
// Qualifier	: const
// Last Update	: 2016/7/19 - 10:16
// Desc.		:
//=============================================================================
float CVGProgressBar::GetPosition() const
{
	return m_fPos;
}

//=============================================================================
// Method		: SetRange
// Access		: public  
// Returns		: void
// Parameter	: __in UINT nStartPos
// Parameter	: __in UINT nEndPos
// Qualifier	:
// Last Update	: 2016/7/19 - 10:16
// Desc.		:
//=============================================================================
void CVGProgressBar::SetRange(__in UINT nStartPos, __in UINT nEndPos)
{
	ASSERT(nStartPos < nEndPos);
	ASSERT(nStartPos >= 0);
	ASSERT(nEndPos >= 0);

	m_nStartPos = nStartPos;
	m_nEndPos = nEndPos;

	m_fPos = (float)nStartPos;

	//Invalidate();
	ReDrawPanel();
}

//=============================================================================
// Method		: SetPercent
// Access		: public  
// Returns		: void
// Parameter	: __in FLOAT fPercent
// Qualifier	:
// Last Update	: 2016/7/19 - 10:16
// Desc.		:
//=============================================================================
void CVGProgressBar::SetPercent(__in FLOAT fPercent)
{
	if (100.0f < fPercent)
		fPercent = 100.0f;

	if (fPercent < 0.0f)
		fPercent = 0.0f;

	m_iFillDegree = (int)fPercent;
	m_strMainText.Format(L"%.1f %%", fPercent);

	//Invalidate();
	ReDrawPanel();
}

//=============================================================================
// Method		: SetPercent
// Access		: public  
// Returns		: void
// Parameter	: __in BYTE byPercent
// Qualifier	:
// Last Update	: 2016/7/19 - 10:16
// Desc.		:
//=============================================================================
void CVGProgressBar::SetPercent(__in BYTE byPercent)
{
	if (100 < byPercent)
		byPercent = 100;

	m_iFillDegree = byPercent;
	m_strMainText.Format(L"%d %%", byPercent);

	//Invalidate();
	ReDrawPanel();
}

//=============================================================================
// Method		: SetText
// Access		: public  
// Returns		: void
// Parameter	: __in LPCTSTR szText
// Qualifier	:
// Last Update	: 2016/7/19 - 10:16
// Desc.		:
//=============================================================================
void CVGProgressBar::SetText(__in LPCTSTR szText)
{
	m_strMainText = szText;

	//Invalidate();
	ReDrawPanel();
}

//=============================================================================
// Method		: ResetProgressBar
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2016/7/19 - 10:16
// Desc.		:
//=============================================================================
void CVGProgressBar::ResetProgressBar()
{
	m_iFillDegree = 0;
	m_strMainText.Empty();

	//Invalidate();
	ReDrawPanel();
}
