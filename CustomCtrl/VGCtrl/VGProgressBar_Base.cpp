//*****************************************************************************
// Filename	: 	VGProgressBar_Base.cpp
// Created	:	2016/3/10 - 11:07
// Modified	:	2016/3/10 - 11:07
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
#include "stdafx.h"
#include "VGProgressBar_Base.h"


CVGProgressBar_Base::CVGProgressBar_Base()
{
	m_pFont					= new Gdiplus::Font(L"Tahoma", 10);
	m_iFillDegree			= 0;
	m_iRoundedCornerAngle	= 10;
	m_strMainText			= L"";
	m_iStatusBarColor		= 99;

	m_clrStatus1		= Color::IndianRed;
	m_clrStatus2		= Color::DarkRed;	
	m_clrFirst			= Color::Red;
	m_clrSecond			= Color::DarkRed;

	m_clrBorder			= Color::Black;
	m_iBorderTickness	= 1;

	m_pCachedBitmap		= NULL;
	m_bRedrawPanel		= FALSE;
}

CVGProgressBar_Base::~CVGProgressBar_Base()
{
	delete m_pFont;
}

//=============================================================================
// Method		: PaintCachedBitmap
// Access		: virtual protected  
// Returns		: void
// Parameter	: IN Graphics & gfx
// Parameter	: IN Gdiplus::Rect argRect
// Qualifier	:
// Last Update	: 2016/7/19 - 10:09
// Desc.		:
//=============================================================================
void CVGProgressBar_Base::PaintCachedBitmap(IN Graphics& gfx, IN Gdiplus::Rect argRect)
{
	Bitmap	*pBit = new Bitmap(argRect.GetRight(), argRect.GetBottom(), &gfx);
	Graphics *memG = new Graphics(pBit);

	RenderBar(*memG, Gdiplus::RectF((REAL)argRect.X, (REAL)argRect.Y, (REAL)argRect.Width, (REAL)argRect.Height));

	if (NULL != m_pCachedBitmap)
	{
		delete m_pCachedBitmap;
		m_pCachedBitmap = NULL;
	}

	m_pCachedBitmap = new CachedBitmap(pBit, &gfx);

	delete pBit;
	delete memG;
}

//=============================================================================
// Method		: ReDrawPanel
// Access		: virtual protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2016/7/18 - 17:17
// Desc.		:
//=============================================================================
void CVGProgressBar_Base::ReDrawPanel()
{
	m_bRedrawPanel = TRUE;
}

//=============================================================================
// Method		: GetStatusBarColor
// Access		: protected  
// Returns		: int
// Qualifier	:
// Last Update	: 2016/7/18 - 17:17
// Desc.		:
//=============================================================================
int CVGProgressBar_Base::GetStatusBarColor()
{
	return m_iStatusBarColor;
};

//=============================================================================
// Method		: SetStatusBarColor
// Access		: protected  
// Returns		: void
// Parameter	: IN int value
// Qualifier	:
// Last Update	: 2016/7/19 - 10:09
// Desc.		:
//=============================================================================
void CVGProgressBar_Base::SetStatusBarColor(IN int value)
{
	m_iStatusBarColor = value;

	switch (value)
	{
	case 0:
		// Raw active
		m_clrStatus1 = Color::OliveDrab;
		m_clrStatus2 = Color::DarkOliveGreen;
		break;

	case 1:
		// Raw inactive
		m_clrStatus1 = Color::OliveDrab;
		m_clrStatus2 = Color::Gray;
		break;

	case 2:
		// Dry active
		m_clrStatus1 = Color::Goldenrod;
		m_clrStatus2 = Color::DarkGoldenrod;
		break;

	case 3:
		// Dry inactive
		m_clrStatus1 = Color::Goldenrod;
		m_clrStatus2 = Color::Gray;
		break;

	default:
		m_clrStatus1 = Color::DimGray;
		m_clrStatus2 = Color::DimGray;
		break;
	}
};

//=============================================================================
// Method		: GetFillDegree
// Access		: protected  
// Returns		: int
// Qualifier	:
// Last Update	: 2016/7/18 - 17:17
// Desc.		:
//=============================================================================
int CVGProgressBar_Base::GetFillDegree()
{
	return m_iFillDegree;
};

//=============================================================================
// Method		: SetFillDegree
// Access		: protected  
// Returns		: void
// Parameter	: IN int value
// Qualifier	:
// Last Update	: 2016/7/19 - 10:09
// Desc.		:
//=============================================================================
void CVGProgressBar_Base::SetFillDegree(IN int value)
{
	if (value >= 100)
	{
		m_clrFirst = Color::Red;
		m_clrSecond = Color::DarkRed;
	}
	else if (value > 90)
	{
		m_clrFirst = Color::Orange;
		m_clrSecond = Color::DarkOrange;
	}
	else if (value > 80)
	{
		m_clrFirst = Color::Gold;
		m_clrSecond = Color::DarkGoldenrod;
	}
	else
	{
		m_clrFirst = Color::Green;
		m_clrSecond = Color::DarkGreen;
	}

	m_iFillDegree = value;
};

//=============================================================================
// Method		: RenderBar
// Access		: virtual protected  
// Returns		: void
// Parameter	: IN Gdiplus::Graphics & graphics
// Parameter	: IN Gdiplus::RectF ClientRect
// Qualifier	:
// Last Update	: 2016/7/19 - 10:10
// Desc.		:
//=============================================================================
void CVGProgressBar_Base::RenderBar(IN Gdiplus::Graphics& graphics, IN Gdiplus::RectF ClientRect)
{
	// Textformat
	Gdiplus::StringFormat strFormat;
	strFormat.SetAlignment(StringAlignmentCenter);
	strFormat.SetLineAlignment (StringAlignmentCenter);

	// Misc	
	//LinearGradientBrush _SideBrush		(GetMainArea(ClientRect.Width, ClientRect.Height), Color::DimGray, Color::Black, LinearGradientModeVertical);
	//LinearGradientBrush _StatusBrush	(GetMainArea(ClientRect.Width, ClientRect.Height), m_clrStatus1, m_clrStatus2, LinearGradientModeVertical);
	LinearGradientBrush _MainBrush		(GetMainArea(ClientRect.Width, ClientRect.Height), m_clrFirst, m_clrSecond, LinearGradientModeVertical);

	SolidBrush drawBrush(Color::White);	
	SolidBrush BkBrush(Color::DimGray);

	// Draw main background
	RectF rectMainBk = GetMainAreaBackground(ClientRect.Width, ClientRect.Height);
	FillRoundedRectangle(graphics, &BkBrush, rectMainBk.X, rectMainBk.Y, rectMainBk.Width, rectMainBk.Height, (REAL)m_iRoundedCornerAngle, RectangleEdgeFilter::None);

	// Draw main
	RectF rectMain = GetMainArea(ClientRect.Width, ClientRect.Height);
	FillRoundedRectangle(graphics, &_MainBrush, rectMain.X, rectMain.Y, rectMain.Width, rectMain.Height, (REAL)m_iRoundedCornerAngle, RectangleEdgeFilter::None);
	graphics.DrawString(m_strMainText, m_strMainText.GetLength(), m_pFont, rectMainBk, &strFormat, &drawBrush);

	// Clean up
	
	// 보더 그리기
	GraphicsPath	graphicsPath;
	Pen				borderPen(m_clrBorder, (REAL)1.0);
	INT iAdjust = m_iBorderTickness / 2;
	INT iAdjust2 = m_iBorderTickness;

	borderPen.SetColor(m_clrBorder);
	borderPen.SetWidth((REAL)m_iBorderTickness);
	
	if (SmoothingModeAntiAlias <= graphics.GetSmoothingMode())
		iAdjust2 = ((INT)m_iBorderTickness % 2) ? m_iBorderTickness : m_iBorderTickness + 1;

	graphics.DrawRectangle(&borderPen, RectF((REAL)ClientRect.X + iAdjust, (REAL)ClientRect.Y + iAdjust, (REAL)(ClientRect.Width - iAdjust2), (REAL)(ClientRect.Height - iAdjust2)));
}
