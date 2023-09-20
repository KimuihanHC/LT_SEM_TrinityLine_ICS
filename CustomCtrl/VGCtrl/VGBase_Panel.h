//*****************************************************************************
// Filename	: VGBase_Panel.h
// Created	: 2013/2/27
// Modified	: 2013/2/27 - 16:11
//
// Author	: PiRing
//	
// Purpose	: 
//*****************************************************************************
#ifndef VGBase_Panel_h__
#define VGBase_Panel_h__

#pragma once

#include "Define_ColorTable.h"
#include "VGBase_Content.h"

// The minimum height for the captionbars in the panels
#define  CAPTION_MIN_HEIGHT		18
#define  CAPTION_MIN_WIDTH		38

// Gets the thickness, in pixels, of a flat-style control border.
#define  BORDER_THICKNESS		2

//=============================================================================
//
//=============================================================================
class CVGBase_Panel : public CVGBase_Content
{
public:
	CVGBase_Panel(void);
	virtual ~CVGBase_Panel(void);

	enum PanelStyle
	{
		// Draws the panels caption in the default office 2003 style.
		PanelStyle_Default,
		// Draws the panels caption in the office 2007 style.
		PanelStyle_Office2007,
	};
	
	enum HoverState
	{
		// The hoverstate in its normal state (none of the other states apply).
		HoverState_None,
		// The hoverstate over which a mouse pointer is resting.
		HoverState_Hover
	};	

	enum CaptionStyle
	{
		//  The normal style of a caption.
		CaptionStyle_Normal,
		// The flat style of a caption.
		CaptionStyle_Flat
	};

	enum ColorStyle
	{
		ColorStyle_Default	= 0,
		ColorStyle_Sky,
		ColorStyle_SkyBlue,
		ColorStyle_Blue,
		ColorStyle_Blue2007,
		ColorStyle_Black,
		ColorStyle_Black2007,
		ColorStyle_Red,
		ColorStyle_Orange,
		ColorStyle_Green,
	};

protected:

	CaptionStyle	m_CaptionStyle;
	int				m_iCaptionHeight;
	int				m_iCaptionWidth;
	
	Gdiplus::Font*	m_pFontCaption;
	CStringW		m_strCaptionText;
	RightToLeft		m_RightToLeft;
	
	FontStyle		m_FontStyle;
	REAL			m_FontSize;
	StringAlignment	m_CaptionAlignment;

	Image*			m_pImage;		// Caption에 표시될 아이콘 이미지
	Gdiplus::Rect	m_rectImage;	// Rectangle(CaptionSpacing, CaptionHeight, m_imageSize.Width, m_imageSize.Height);

	SmoothingMode	m_SmoothingMode;
	INT				m_BorderTickness;
	BOOL			m_bShowBorder;	// 외곽선 그리기 여부
	PanelColors		m_PanelColors;
	PanelStyle		m_PanelStyle;

	BOOL			m_bExpand;		// TRUE : Caption + Content, FALSE : Only Caption
	BOOL			m_bHover;
	BOOL			m_bChecked;
	BOOL			m_bPressed;

	Gdiplus::Rect	m_rectCaption;	// Rect (0, 0, this.rectClient.Width, m_iCaptionHeight);	
	Gdiplus::Rect	m_rectContent;

protected:
	
	DockStyle		m_DockStyle;
	BOOL			m_bRedrawPanel;

	//-------------------------------------------------------------------------
	// Panel 그리기 함수
	//-------------------------------------------------------------------------
	void	DrawCaptionbar				(Graphics& graphics, BOOL bExpand, BOOL bShowBorder, PanelStyle panelStyle, Gdiplus::Rect rectClient);
	void	DrawBorders					(Graphics& graphics, Gdiplus::Rect rectClient);	
	void	DrawInnerBorders			(Graphics& graphics, Gdiplus::Rect rectClient, DockStyle dockStyle = DockStyle_Fill, int PaddingAll = 0);

	void	CalculatePanelHeights		(Gdiplus::Rect rectClient);
	void	DrawBackground				(Graphics& graphics, Gdiplus::Rect rectClient);
	void	DrawPanel					(Graphics& graphics, Gdiplus::Rect rectClient);		

	virtual void DrawPanelAndBackground	(Graphics& graphics, Gdiplus::Rect rectClient);
	virtual void DrawPanelWithContent	(Graphics& graphics, Gdiplus::Rect rectClient);

	virtual void ReDrawPanel			();

public:

	//-------------------------------------------------------------------------
	// 멤버 변수 설정 함수
	//-------------------------------------------------------------------------
	void	SetDockStyle				(DockStyle dockStyle, BOOL bReDraw = TRUE);
	void	ChangeColorStyle			(ColorStyle nColorStyle);

	// Caption Text
	void	SetCaptionFont				(IN const WCHAR * familyName, REAL emSize = 8.0, FontStyle fontStyle = FontStyleBold);
	void	GetCaptionText				(OUT CStringW& szValue);
	void	SetCaptionText				(IN const WCHAR* szValue);
	void	SetCaptionTextAlignment		(StringAlignment CationTextAlignment);
	void	SetCaptionTextColor			(Color TextColor, Color SelectedTextColor = Color::White, Color CollapsedTextColor = Color::White);
	void	SetCaptionHeight			(int iHeight);

 	virtual void	SetContentBatchType	(BOOL bBatchVertical);
 	virtual void	SetContentText		(LPCTSTR szText, UINT nContentIndex = 0);

	
};

#endif // VGBase_Panel_h__
