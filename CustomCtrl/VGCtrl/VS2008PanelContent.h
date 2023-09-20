//*****************************************************************************
// Filename	: VS2008PanelContent.h
// Created	: 2013/2/21
// Modified	: 2013/2/21 - 11:48
//
// Author	: PiRing
//	
// Purpose	: 
//*****************************************************************************
#ifndef VS2008PanelContent_h__
#define VS2008PanelContent_h__

#pragma once

#include "VGBase_Content.h"
#include "Define_ColorizePanel.h"

//=============================================================================
// CVS2008PanelContent
//=============================================================================
class CVS2008PanelContent : public CVGBase_Content
{
public:
	CVS2008PanelContent();
	virtual ~CVS2008PanelContent();

private:
	CStringW		m_title;

	Color			m_ContentColor; // 컨텐츠 배경 색
	Color			m_BackColor;	// 배경 색

	StringAlignment	m_titleAlignment;

protected:
	ColorizerPanel	m_panelColorizer;
	CachedBitmap*	m_pCachedBitmap;

	virtual void	DrawCornersAndText(Graphics& gfx, Rect argRect);
	virtual void	DrawCornersAndText(Graphics& gfx, Rect argRect, ColorizerPanel& PanelColor);
	void			AddContentPath(IN Rect rectClient, OUT GraphicsPath& gPath);
	void			AddSelectionPath(IN Rect rectClient, OUT GraphicsPath& gPath);

	BOOL			m_bRedrawPanel;
	virtual void	ReDrawPanel();
	virtual void	PaintCachedBitmap(Graphics& gfx, Rect argRect);
	virtual void	PaintCachedBitmap(Graphics& gfx, Rect argRect, ColorizerPanel& PanelColor);

	// 컨텐츠 백그라운드 사용 할 것인가?
	BOOL			m_bUseContentColor;

	CStringW		m_FontFamilyName;
	REAL			m_FontSize;

	virtual void	DrawPanel(Graphics& graphics, Rect rectClient, ColorizerPanel& PanelColor);


	BOOL			m_bUseSelectionFlag;
	BOOL			m_bSelectedPanel;
	Color			m_SelectionColor;

public:

	// Title Text
	CStringW	GetTitle();
	void		SetTitle(WCHAR* szValue);
	void		SetTitleAlignment(StringAlignment TitleAlignment);
	void		SetTitleColor(Color TextColor);

	// Color Setting
	void		SetBackgroundColor(Color BackColor);

	void		SetColor(byte red, byte green, byte blue);
	void		SetColorAlpha(byte alpha);
	void		SetColorOption(bool isColorizerEnabled, bool isTransparencyEnabled);
	void		SetColorStyle(ColorMatrixStyle paintingStyle);

	// Font
	void		SetFont(const WCHAR * familyName);
	void		SetFontSize(REAL emSize);


	void			SetContentColor(Color BackColor);
	virtual void	SetContentBatchType(BOOL bBatchVertical);
	virtual void	SetContentText(LPCTSTR szText, UINT nContentIndex = 0);

};
#endif // VS2008PanelContent_h__
