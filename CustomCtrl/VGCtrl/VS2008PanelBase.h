//*****************************************************************************
// Filename	: VS2008PanelBase.h
// Created	: 2013/2/21
// Modified	: 2013/2/21 - 11:48
//
// Author	: PiRing
//	
// Purpose	: 
//*****************************************************************************
#ifndef VS2008PanelBase_h__
#define VS2008PanelBase_h__

#pragma once

#include "VGBase.h"
#include "Define_ColorizePanel.h"


//=============================================================================
// CVS2008PanelBase
//=============================================================================
class CVS2008PanelBase : public CVGBase
{
public:
	CVS2008PanelBase();
	virtual ~CVS2008PanelBase();

private:
	CStringW		m_title;
	
	Color			m_BackColor;	// 배경 색

	StringAlignment	m_titleAlignment;	

protected:
	ColorizerPanel	m_panelColorizer;
	CachedBitmap*	m_pCachedBitmap;

	virtual void	DrawCornersAndText(Graphics& gfx, Gdiplus::Rect argRect);
	virtual void	DrawCornersAndText(Graphics& gfx, Gdiplus::Rect argRect, ColorizerPanel& PanelColor);
	void			AddSelectionPath(IN Gdiplus::Rect rectClient, OUT GraphicsPath& gPath);

	BOOL			m_bRedrawPanel;
	virtual void	ReDrawPanel			();
	virtual void	PaintCachedBitmap(Graphics& gfx, Gdiplus::Rect argRect);
	virtual void	PaintCachedBitmap(Graphics& gfx, Gdiplus::Rect argRect, ColorizerPanel& PanelColor);

	CStringW		m_FontFamilyName;
	REAL			m_FontSize;

	virtual void	DrawPanel(Graphics& graphics, Gdiplus::Rect rectClient, ColorizerPanel& PanelColor);


	BOOL			m_bUseSelectionFlag;
	BOOL			m_bSelectedPanel;
	Color			m_SelectionColor;

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
	void		SetColorOption		(bool isColorizerEnabled, bool isTransparencyEnabled);
	void		SetColorStyle		(ColorMatrixStyle paintingStyle);

	// Font
	void		SetFont				(const WCHAR * familyName);
	void		SetFontSize			(REAL emSize);

};
#endif // VS2008PanelBase_h__
