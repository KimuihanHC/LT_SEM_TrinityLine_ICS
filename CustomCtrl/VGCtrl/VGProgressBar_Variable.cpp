//*****************************************************************************
// Filename	: 	VGProgressBar_Variable.cpp
// Created	:	2016/3/10 - 18:24
// Modified	:	2016/3/10 - 18:24
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
#include "stdafx.h"
#include "VGProgressBar_Variable.h"


CVGProgressBar_Variable::CVGProgressBar_Variable()
{
	m_nEndPos		= 100;
	m_nStartPos		= 0;
	m_fPos			= 0;
	m_fStepAmount	= 1;
}


CVGProgressBar_Variable::~CVGProgressBar_Variable()
{
}
BEGIN_MESSAGE_MAP(CVGProgressBar_Variable, CWnd)
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
void CVGProgressBar_Variable::OnPaint()
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
LRESULT CVGProgressBar_Variable::OnStep(WPARAM wParam, LPARAM lParam)
{
	Step();
	return 1;
}

LRESULT CVGProgressBar_Variable::OnSetRange(WPARAM wParam, LPARAM lParam)
{
	SetRange((UINT)wParam, (UINT)lParam);
	return 1;
}

LRESULT CVGProgressBar_Variable::OnGetPos(WPARAM wParam, LPARAM lParam)
{
	return (UINT)GetPosition();
}

LRESULT CVGProgressBar_Variable::OnSetPosF(WPARAM wParam, LPARAM lParam)
{
	SetPosition(*(float*)wParam);
	return 1;
}

LRESULT CVGProgressBar_Variable::OnSetStepF(WPARAM wParam, LPARAM lParam)
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
void CVGProgressBar_Variable::ReDrawPanel()
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
float CVGProgressBar_Variable::GetStepAmount() const
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
void CVGProgressBar_Variable::Step()
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
void CVGProgressBar_Variable::SetStepAmount(__in float fStepAmount)
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
void CVGProgressBar_Variable::SetPosition(__in float fPos)
{
	if (fPos < m_nStartPos -m_fOffsetPos)
		fPos = (float)m_nStartPos - m_fOffsetPos;
	if (fPos > m_nEndPos - m_fOffsetPos)
		fPos = (float)m_nEndPos - m_fOffsetPos;
	m_bMode = PRGB_M_POSITION;
	m_fPos = fPos;
	//m_iFillDegree = fPos;
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
float CVGProgressBar_Variable::GetPosition() const
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
void CVGProgressBar_Variable::SetRange(__in UINT nStartPos, __in UINT nEndPos)
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
// Method		: SetOffset
// Access		: public  
// Returns		: void
// Parameter	: __in FLOAT fOffsetPos
// Qualifier	:
// Last Update	: 2018/8/24 - 10:25
// Desc.		:
//=============================================================================
void CVGProgressBar_Variable::SetOffset(__in FLOAT fOffsetPos)
{
	m_fOffsetPos = fOffsetPos;
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
void CVGProgressBar_Variable::SetPercent(__in FLOAT fPercent)
{
	if (100.0f < fPercent)
		fPercent = 100.0f;

	if (fPercent < 0.0f)
		fPercent = 0.0f;

	m_iFillDegree = (int)fPercent;
	m_strMainText.Format(L"%.1f %%", fPercent);
	m_bMode = PRGB_M_PERCENT;

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
void CVGProgressBar_Variable::SetPercent(__in BYTE byPercent)
{
	if (100 < byPercent)
		byPercent = 100;

	m_iFillDegree = byPercent;
	m_strMainText.Format(L"%d %%", byPercent);
	m_bMode = PRGB_M_PERCENT;

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
void CVGProgressBar_Variable::SetText(__in LPCTSTR szText)
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
void CVGProgressBar_Variable::ResetProgressBar()
{
	m_iFillDegree = 0;
	m_strMainText.Empty();

	//Invalidate();
	ReDrawPanel();
}
