//*****************************************************************************
// Filename	: VGCtrlBase.cpp
// Created	: 2013/2/25
// Modified	: 2013/2/25 - 14:29
//
// Author	: PiRing
//	
// Purpose	: 
//*****************************************************************************
#include "StdAfx.h"
#include "VGCtrlBase.h"

//=============================================================================
//
//=============================================================================
CVGCtrlBase::CVGCtrlBase(void)
{
	m_title				= "";	

	m_ForeColor			= Color::White;
	m_BackColor			= Color::White;
	m_bUseBackColor		= TRUE;

	m_pCachedBitmap		= NULL;
	m_bRedrawPanel		= TRUE;
	m_titleAlignment	= StringAlignmentCenter;

	m_FontFamilyName	= L"Tahoma";
	m_FontSize			= 14;
}

CVGCtrlBase::~CVGCtrlBase(void)
{
	if (NULL != m_pCachedBitmap) 
	{
		delete m_pCachedBitmap;
		m_pCachedBitmap = NULL;
	}
}
//=============================================================================
// Method		: CVGCtrlBase::DrawCornersAndText
// Access		: virtual protected 
// Returns		: void
// Parameter	: Graphics & gfx
// Parameter	: Rect argRect
// Parameter	: ColorizerCtrl & PanelColor
// Qualifier	:
// Last Update	: 2013/2/21 - 15:12
// Desc.		:
//=============================================================================
void CVGCtrlBase::DrawCornersAndText( Graphics& gfx, Rect argRect, ColorizerCtrl& PanelColor )
{
	Rect	destRect(argRect);	

	if (destRect.Width < 28 || destRect.Height < 44)
		return;

	// Background
	// Border
	// Foreground
	// Text

	Bitmap overlay(destRect.Width, destRect.Height, PixelFormat32bppRGB);
	Graphics gr(&overlay);
	SolidBrush brush(Color::White);
	gr.FillRectangle(&brush, 0, 0, overlay.GetWidth(), overlay.GetHeight());

	Pen pen(Color::DarkGray, 2);
	gr.DrawRectangle(&pen, 0, 0, overlay.GetWidth() - 2, overlay.GetHeight() - 2);

	pen.SetColor(Color::Black);
	pen.SetWidth(1);
	gr.DrawRectangle(&pen, 1, 1, overlay.GetWidth() - 3, overlay.GetHeight() - 3);

	destRect.X += 5;
	destRect.Y += 1;
	destRect.Width -= 11;
	destRect.Height = destRect.Height > 48 ? 24 : destRect.Height / 2;
	
	// 상단
	brush.SetColor(Color::DarkGray);
	gr.FillRectangle(&brush, destRect);

	pen.SetColor(Color(0,0,0));
	pen.SetWidth(2);
	gr.DrawRectangle(&pen, destRect);


	destRect = argRect;		

	// Background 사용한다면
	if (m_bUseBackColor)
	{
		SolidBrush BackBrush (m_BackColor);
		gfx.FillRectangle(&BackBrush, destRect);
	}

	// Panel 그리기
	gfx.DrawImage(&overlay, RectF((REAL)destRect.X, (REAL)destRect.Y, (REAL)destRect.Width, (REAL)destRect.Height), 0, 0, (REAL)overlay.GetWidth(), (REAL)overlay.GetHeight(), UnitPixel);

	// Draw String
	destRect.Inflate(-14, -4);	

	if (destRect.Width > 0)
	{		
		Gdiplus::Font captionFont(m_FontFamilyName, m_FontSize, FontStyleBold, UnitPixel);
		SolidBrush captionTextBrush (m_ForeColor);
		StringFormat captionTextFormat(StringFormatFlagsNoWrap);

		captionTextFormat.SetAlignment (m_titleAlignment);
		captionTextFormat.SetTrimming (StringTrimmingEllipsisCharacter);
		captionTextFormat.SetHotkeyPrefix (HotkeyPrefixHide);

		gfx.SetTextRenderingHint(TextRenderingHintAntiAliasGridFit);
		gfx.DrawString(m_title, -1, &captionFont, RectF((REAL)destRect.X, (REAL)destRect.Y, (REAL)destRect.Width, (REAL)destRect.Height), &captionTextFormat, &captionTextBrush);				
	}
}

void CVGCtrlBase::DrawCornersAndText( Graphics& gfx, Rect argRect )
{	
	DrawCornersAndText (gfx, argRect, m_panelColorizer);
}

//=============================================================================
// Method		: CVGCtrlBase::DrawCachedBitmap
// Access		: virtual protected 
// Returns		: void
// Parameter	: Graphics & gfx
// Parameter	: Rect argRect
// Parameter	: ColorizerCtrl & PanelColor
// Qualifier	:
// Last Update	: 2013/2/21 - 15:13
// Desc.		:
//=============================================================================
void CVGCtrlBase::PaintCachedBitmap( Graphics& gfx, Rect argRect, ColorizerCtrl& PanelColor )
{
	Bitmap	*pBit	= new Bitmap(argRect.GetRight(), argRect.GetBottom(), &gfx);
	Graphics *memG	= new Graphics(pBit);

	DrawCornersAndText (*memG, argRect, PanelColor);	

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
// Method		: CVGCtrlBase::DrawCachedBitmap
// Access		: virtual protected 
// Returns		: void
// Parameter	: Graphics & gfx
// Parameter	: Rect argRect
// Qualifier	:
// Last Update	: 2013/2/21 - 11:51
// Desc.		: 더블 버퍼링용
//=============================================================================
void CVGCtrlBase::PaintCachedBitmap( Graphics& gfx, Rect argRect )
{
	PaintCachedBitmap (gfx, argRect, m_panelColorizer);	
}

//=============================================================================
// Method		: CVGCtrlBase::ReDrawPanel
// Access		: virtual protected 
// Returns		: void
// Qualifier	:
// Last Update	: 2013/2/20 - 17:09
// Desc.		:
//=============================================================================
void CVGCtrlBase::ReDrawPanel()
{
	m_bRedrawPanel = TRUE;	
}

//=============================================================================
// Method		: CVGCtrlBase::GetTitle
// Access		: public 
// Returns		: CStringW
// Qualifier	:
// Last Update	: 2013/2/20 - 17:29
// Desc.		:
//=============================================================================
CStringW CVGCtrlBase::GetTitle()
{
	return m_title;
}

//=============================================================================
// Method		: CVGCtrlBase::SetTitle
// Access		: public 
// Returns		: void
// Parameter	: WCHAR * szValue
// Qualifier	:
// Last Update	: 2013/2/20 - 17:29
// Desc.		:
//=============================================================================
void CVGCtrlBase::SetTitle( WCHAR* szValue )
{
	if (NULL == szValue)
	{
		m_title.Empty();
	}
	else
	{
		if (szValue != m_title)
		{
			m_title = szValue;

			// Update text on the panel.			
			ReDrawPanel();
		}
	}
}

//=============================================================================
// Method		: CVGCtrlBase::SetTitleAlignment
// Access		: public 
// Returns		: void
// Parameter	: StringAlignment TitleAlignment
// Qualifier	:
// Last Update	: 2013/2/20 - 17:29
// Desc.		:
//=============================================================================
void CVGCtrlBase::SetTitleAlignment( StringAlignment TitleAlignment )
{
	if (m_titleAlignment != TitleAlignment)
	{
		m_titleAlignment = TitleAlignment;
		ReDrawPanel();
	}
}

//=============================================================================
// Method		: CVGCtrlBase::SetTitleColor
// Access		: public 
// Returns		: void
// Parameter	: Color TextColor
// Qualifier	:
// Last Update	: 2013/2/20 - 17:53
// Desc.		:
//=============================================================================
void CVGCtrlBase::SetTitleColor( Color TextColor )
{
	m_ForeColor = TextColor;
	ReDrawPanel();
}

//=============================================================================
// Method		: CVGCtrlBase::SetBackgroundColor
// Access		: public 
// Returns		: void
// Parameter	: Color BackColor
// Qualifier	:
// Last Update	: 2013/2/20 - 17:29
// Desc.		:
//=============================================================================
void CVGCtrlBase::SetBackgroundColor( Color BackColor )
{
	m_BackColor = BackColor;

	ReDrawPanel();
}

//=============================================================================
// Method		: CVGCtrlBase::SetColor
// Access		: public 
// Returns		: void
// Parameter	: byte red
// Parameter	: byte green
// Parameter	: byte blue
// Qualifier	:
// Last Update	: 2013/2/20 - 17:29
// Desc.		:
//=============================================================================
void CVGCtrlBase::SetColor( byte red, byte green, byte blue )
{
	m_panelColorizer.Red	= red;
	m_panelColorizer.Green	= green;
	m_panelColorizer.Blue	= blue;

	ReDrawPanel();
}

//=============================================================================
// Method		: CVGCtrlBase::SetColorAlpha
// Access		: public 
// Returns		: void
// Parameter	: byte alpha
// Qualifier	:
// Last Update	: 2013/2/20 - 17:29
// Desc.		:
//=============================================================================
void CVGCtrlBase::SetColorAlpha( byte alpha )
{
	if (m_panelColorizer.Alpha != alpha)
	{
		m_panelColorizer.Alpha = alpha;

		if (m_panelColorizer.IsTransparencyEnabled)
			ReDrawPanel();
	}
}

//=============================================================================
// Method		: CVGCtrlBase::SetColorOption
// Access		: public 
// Returns		: void
// Parameter	: bool isTransparencyEnabled
// Qualifier	:
// Last Update	: 2013/2/20 - 17:29
// Desc.		:
//=============================================================================
void CVGCtrlBase::SetColorOption( bool isTransparencyEnabled )
{
	m_panelColorizer.IsTransparencyEnabled	= isTransparencyEnabled;

	ReDrawPanel();
}

//=============================================================================
// Method		: CVGCtrlBase::SetColorStyle
// Access		: public 
// Returns		: void
// Parameter	: VisualGdipShapeStyle shapeStyle
// Qualifier	:
// Last Update	: 2013/2/20 - 17:29
// Desc.		:
//=============================================================================
void CVGCtrlBase::SetShapeStyle( VisualGdipShapeStyle shapeStyle )
{
	if (m_panelColorizer.ShapeStyle != shapeStyle)
	{
		m_panelColorizer.ShapeStyle = shapeStyle;
		ReDrawPanel();
	}		
}

//=============================================================================
// Method		: CVGCtrlBase::SetFont
// Access		: public 
// Returns		: void
// Parameter	: const WCHAR * familyName
// Qualifier	:
// Last Update	: 2013/2/22 - 17:58
// Desc.		:
//=============================================================================
void CVGCtrlBase::SetFont( const WCHAR * familyName )
{
	if (m_FontFamilyName != familyName)
	{
		m_FontFamilyName = familyName;
		ReDrawPanel();
	}
}

//=============================================================================
// Method		: CVGCtrlBase::SetFontSize
// Access		: public 
// Returns		: void
// Parameter	: REAL emSize
// Qualifier	:
// Last Update	: 2013/2/22 - 17:58
// Desc.		:
//=============================================================================
void CVGCtrlBase::SetFontSize( REAL emSize )
{
	if (m_FontSize != emSize)
	{
		m_FontSize = emSize;
		ReDrawPanel();
	}
}