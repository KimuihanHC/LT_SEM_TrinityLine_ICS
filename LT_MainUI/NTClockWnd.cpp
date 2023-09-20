//*****************************************************************************
// Filename	: NTClockWnd.cpp
// Created	: 2010/4/15
// Modified	: 2010/4/15 - 17:28
//
// Author	: PiRing
//	
// Purpose	: 시간을 표시하는 윈도우
//*****************************************************************************
// NTClockWnd.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NTClockWnd.h"
#include "../TextDesigner/OutlineText.h"
#include "../TextDesigner/PngOutlineText.h"

// CNTClockWnd
using namespace Gdiplus;
using namespace TextDesigner;

IMPLEMENT_DYNAMIC(CNTClockWnd, CStatic)

//=============================================================================
// 생성자
//=============================================================================
CNTClockWnd::CNTClockWnd()
{
	GetLocalTime( &m_timePrev );
	m_idTimer			= 1;

	m_nDateFontSize		= 24;
	m_nTimeFontSize		= 36;
	m_nSecondFontSize	= 24;

	//m_ptDate	.SetPoint(2, 0);
	//m_ptTime	.SetPoint(0, 24);
	//m_ptSecond	.SetPoint(150, 36);

	m_ptDate	.SetPoint(5, 0);
	m_ptTime	.SetPoint(0, 24);	
	m_ptSecond	.SetPoint(130, 36);

	m_pCBit				= NULL;
}

//=============================================================================
// 소멸자
//=============================================================================
CNTClockWnd::~CNTClockWnd()
{
	if (NULL != m_pCBit)
	{
		delete m_pCBit;
		m_pCBit = NULL;
	}
}

BEGIN_MESSAGE_MAP(CNTClockWnd, CStatic)
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

//=============================================================================
// CNTClockWnd 메시지 처리기입니다.
//=============================================================================
void CNTClockWnd::OnDestroy()
{
	Clean ();

	CStatic::OnDestroy();	
}

//=============================================================================
// Method		: CNTClockWnd::OnSize
// Access		: protected 
// Returns		: void
// Parameter	: UINT nType
// Parameter	: int cx
// Parameter	: int cy
// Qualifier	:
// Last Update	: 2010/4/15 - 18:23
// Desc.		:
//=============================================================================
void CNTClockWnd::OnSize(UINT nType, int cx, int cy)
{
	CStatic::OnSize(nType, cx, cy);

	//m_ptDate.SetPoint(0, 0);
	//m_ptTime.SetPoint(0, 0);
	//m_ptSecond.SetPoint(0, 0);
}

//=============================================================================
// Method		: CNTClockWnd::OnTimer
// Access		: protected 
// Returns		: void
// Parameter	: UINT_PTR nIDEvent
// Qualifier	:
// Last Update	: 2010/4/15 - 18:23
// Desc.		: 
//=============================================================================
void CNTClockWnd::OnTimer(UINT_PTR nIDEvent)
{
	SYSTEMTIME time={0};
	GetLocalTime( &time );

	m_strDate.Format(_T("%.4d / %.2d / %.2d"), time.wYear, time.wMonth, time.wDay);
	m_strTime.Format(_T("%.2d : %.2d"), time.wHour, time.wMinute);
	m_strSecond.Format(_T("%.2d"), time.wSecond);

	if (::IsWindow(m_hWnd))
	{
		DrawClock ();		
	}

	m_timePrev = time;

	CStatic::OnTimer(nIDEvent);
}

//=============================================================================
// Method		: CNTClockWnd::OnPaint
// Access		: public 
// Returns		: void
// Qualifier	:
// Last Update	: 2010/4/16 - 13:25
// Desc.		: 캐시 비트맵을 이용한 더블 버퍼링
//=============================================================================
void CNTClockWnd::OnPaint()
{
	CPaintDC dc(this);

	Graphics G(dc.GetSafeHdc());

	if (m_pCBit == NULL) 
	{
		//DrawClock (GetDC());
		DrawClock(dc.GetSafeHdc());
	}

	G.DrawCachedBitmap(m_pCBit,0,0);
}

//=============================================================================
// Method		: CNTClockWnd::OnEraseBkgnd
// Access		: protected 
// Returns		: BOOL
// Parameter	: CDC * pDC
// Qualifier	:
// Last Update	: 2010/4/16 - 16:40
// Desc.		:
//=============================================================================
BOOL CNTClockWnd::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
	//return CStatic::OnEraseBkgnd(pDC);
}

//=============================================================================
// Method		: CNTClockWnd::Clean
// Access		: protected 
// Returns		: void
// Qualifier	:
// Last Update	: 2010/4/15 - 18:23
// Desc.		:
//=============================================================================
void CNTClockWnd::Clean()
{
	KillTimer(m_idTimer);
}

//=============================================================================
// Method		: CNTClockWnd::Start
// Access		: public 
// Returns		: void
// Qualifier	:
// Last Update	: 2010/4/15 - 18:23
// Desc.		:
//=============================================================================
void CNTClockWnd::Start()
{
	SetTimer(m_idTimer, 1000, NULL);
}

//=============================================================================
// Method		: Set_Adjust_Date
// Access		: public  
// Returns		: void
// Parameter	: int nLeft
// Parameter	: int nTop
// Qualifier	:
// Last Update	: 2021/5/11 - 13:07
// Desc.		:
//=============================================================================
void CNTClockWnd::Set_Adjust_Date(int nLeft, int nTop)
{
	m_ptDate.Offset(nLeft, nTop);
}

void CNTClockWnd::Set_Adjust_Time(int nLeft, int nTop)
{
	m_ptTime.Offset(nLeft, nTop);
}

void CNTClockWnd::Set_Adjust_Second(int nLeft, int nTop)
{
	m_ptSecond.Offset(nLeft, nTop);
}

//=============================================================================
// Method		: CNTClockWnd::DrawClock
// Access		: protected 
// Returns		: void
// Parameter	: 
// Qualifier	:
// Last Update	: 2010/4/16 - 13:58
// Desc.		:
//=============================================================================
void CNTClockWnd::DrawClock()
{
	CDC* pDc = GetDC();
	Graphics graphics(*pDc);

	RECT crt;
	GetClientRect(&crt);
	Bitmap	*pBit	= new Bitmap(crt.right, crt.bottom, &graphics);
	Graphics *memG	= new Graphics(pBit);

	memG->SetSmoothingMode(SmoothingModeAntiAlias);
	memG->SetInterpolationMode(InterpolationModeHighQualityBicubic);

	//-------------------------
	DrawClockText	(memG, m_strDate, m_ptDate.x, m_ptDate.y, CT_DATE);
	DrawClockText	(memG, m_strTime, m_ptTime.x, m_ptTime.y, CT_TIME);
	DrawClockText	(memG, m_strSecond, m_ptSecond.x, m_ptSecond.y, CT_SECOND);		
	//---------------------

	if (NULL != m_pCBit) 
	{
		delete m_pCBit;
		m_pCBit = NULL;
	}

	m_pCBit = new CachedBitmap(pBit, &graphics);

	delete pBit;
	delete memG;

	ReleaseDC(pDc);

	Invalidate();
}

void CNTClockWnd::DrawClock( HDC hDC )
{
	Graphics graphics(hDC);

	RECT crt;
	GetClientRect(&crt);
	Bitmap	*pBit	= new Bitmap(crt.right, crt.bottom, &graphics);
	Graphics *memG	= new Graphics(pBit);

	memG->SetSmoothingMode(SmoothingModeAntiAlias);
	memG->SetInterpolationMode(InterpolationModeHighQualityBicubic);

	//-------------------------
	DrawClockText	(memG, m_strDate, m_ptDate.x, m_ptDate.y, CT_DATE);
	DrawClockText	(memG, m_strTime, m_ptTime.x, m_ptTime.y, CT_TIME);
	DrawClockText	(memG, m_strSecond, m_ptSecond.x, m_ptSecond.y, CT_SECOND);		
	//---------------------

	if (NULL != m_pCBit) 
	{
		delete m_pCBit;
		m_pCBit = NULL;
	}

	m_pCBit = new CachedBitmap(pBit, &graphics);

	delete pBit;
	delete memG;

	Invalidate();
}

//=============================================================================
// Method		: CNTClockWnd::DrawClockText
// Access		: protected 
// Returns		: void
// Parameter	: Graphics * pGrp
// Parameter	: CString strText
// Parameter	: UINT nOffsetX
// Parameter	: UINT nOffsetY
// Parameter	: UINT nType
// Qualifier	:
// Last Update	: 2010/4/16 - 17:43
// Desc.		:
//=============================================================================
void CNTClockWnd::DrawClockText( Graphics* pGrp, CString strText, UINT nOffsetX, UINT nOffsetY, UINT nType )
{
	FontFamily fontFamily(L"Arial Black");
	StringFormat strformat;		

	OutlineText text;

	UINT	nFontSize = 20;
	switch (nType)
	{
	case CT_DATE:
		text.TextDblOutline(Color(218, 165, 32), Color(0, 0, 0), Color(0, 0, 0), 2, 2);
		nFontSize = m_nDateFontSize;
		break;

	case CT_TIME:
		text.TextDblOutline(Color(192, 192, 192), Color(0, 0, 0), Color(0, 0, 0), 2, 2);
		nFontSize = m_nTimeFontSize;
		break;

	case CT_SECOND:
		text.TextDblOutline(Color(255,255,255), Color(0,0,0), Color(0,0,0), 2, 2);
		nFontSize = m_nSecondFontSize;
		break;

	default:
		text.TextDblOutline(Color(255,255,255), Color(0,128,128), Color(0,255,0), 2, 2);		
		break;
	}

	text.EnableShadow(true);		
	text.Shadow(Color(128,0,0,0), 4, Point(2,4));

#ifdef _MBCS
	// convert the CString to wchar_t
	size_t cssize = strlen(strText) + 1;
	size_t wcsize = cssize*sizeof(wchar_t);		
	wchar_t *pwstr = (wchar_t*)malloc(wcsize);
	MultiByteToWideChar(CP_ACP, 0, strText, (int)cssize, pwstr, (int)wcsize);
	text.DrawString(pGrp, &fontFamily, FontStyleRegular, nFontSize, pwstr, Gdiplus::Point(nOffsetX, nOffsetY), &strformat);
	free(pwstr);
#else
	text.DrawString(pGrp, &fontFamily, FontStyleRegular, nFontSize, strText, Gdiplus::Point(nOffsetX, nOffsetY), &strformat);
#endif
	
}
