//*****************************************************************************
// Filename	: VGCtrlBase.h
// Created	: 2013/2/25
// Modified	: 2013/2/25 - 14:28
//
// Author	: PiRing
//	
// Purpose	: 
//*****************************************************************************
#ifndef VGCtrlBase_h__
#define VGCtrlBase_h__

#pragma once

enum VisualGdipShapeStyle
{
	VGSS_DEFAULT		= 0,
	VGSS_RECT,
	VGSS_TABLET_H,
	VGSS_TABLET_V,
	VGSS_ROUNDRECT,		
	VGSS_DOT_RECT,
};

enum VisualGdipColorStyle
{
	VGCS_DEFAULT		= 0,
	VGCS_MONO,			// 단색
	VGCS_RECT_2TONE,	// 2 톤
	VGCS_RECT_GRAD,		// 그라데이션	
	VGCS_BLUR,			// 블러
	VGCS_GLOW,			// 그로우
};

//=============================================================================
// ColorizerPanel
//=============================================================================
class ColorizerCtrl
{	
public:
	bool IsTransparencyEnabled;

	byte Red;
	byte Green;
	byte Blue;
	byte Alpha;

	VisualGdipShapeStyle ShapeStyle;
	VisualGdipColorStyle ColorStyle;

	ColorizerCtrl ()
	{
		IsTransparencyEnabled	= false;

		Red		= 0;
		Green	= 0;
		Blue	= 0;
		Alpha	= 255;

		ShapeStyle = VGSS_DEFAULT;
		ColorStyle = VGCS_DEFAULT;
	}

	ColorizerCtrl(byte red, byte green, byte blue, byte alpha, bool isTransparencyEnabled, VisualGdipShapeStyle shapeStyle, VisualGdipColorStyle colorStyle)
	{
		// Sets RGBA
		Red		= red;
		Green	= green;
		Blue	= blue;
		Alpha	= alpha;

		// Sets Options
		IsTransparencyEnabled	= isTransparencyEnabled;

		// Set Painting Style
		ShapeStyle = shapeStyle;
		ColorStyle = colorStyle;
	}
};

//=============================================================================
//
//=============================================================================
class CVGCtrlBase
{
public:
	CVGCtrlBase(void);
	~CVGCtrlBase(void);

private:
	CStringW		m_title;

	Color			m_ForeColor; // 글자 색
	Color			m_BackColor; // 배경 색

	StringAlignment	m_titleAlignment;	

protected:
	ColorizerCtrl	m_panelColorizer;
	CachedBitmap*	m_pCachedBitmap;

	virtual void	DrawCornersAndText	(Graphics& gfx, Rect argRect);
	virtual void	DrawCornersAndText	(Graphics& gfx, Rect argRect, ColorizerCtrl& PanelColor);

	BOOL			m_bRedrawPanel;
	virtual void	ReDrawPanel		();
	virtual void	PaintCachedBitmap	(Graphics& gfx, Rect argRect);
	virtual void	PaintCachedBitmap	(Graphics& gfx, Rect argRect, ColorizerCtrl& PanelColor);

	// 백그라운드 사용 할 것인가?
	BOOL			m_bUseBackColor;

	CStringW		m_FontFamilyName;
	REAL			m_FontSize;

	// Caption : 위치, 크기
	// Padding : 크기, 두께
	// Margin : 간격
	// Border : 굵기
	//  -> outer, middle, inner

public:

	// Title Text
	CStringW	GetTitle			();
	void		SetTitle			(WCHAR* szValue);
	void		SetTitleAlignment	(StringAlignment TitleAlignment);
	void		SetTitleColor		(Color TextColor);	

	// Color Setting
	void		SetBackgroundColor	(Color BackColor);

	void		SetColor			(byte red, byte green, byte blue);
	void		SetColorAlpha		(byte alpha);
	void		SetColorOption		(bool isTransparencyEnabled);

	void		SetShapeStyle		(VisualGdipShapeStyle shapeStyle);

	// Font
	void		SetFont				(const WCHAR * familyName);
	void		SetFontSize			(REAL emSize);

	// Shape Style
};

#endif // VGCtrlBase_h__
