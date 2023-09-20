//*****************************************************************************
// Filename	: 	VGProgressBar_Base.h
// Created	:	2016/3/10 - 11:07
// Modified	:	2016/3/10 - 11:07
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
#ifndef VGProgressBar_Base_h__
#define VGProgressBar_Base_h__

#pragma once
#include "VGBase.h"

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
class CVGProgressBar_Base : public CVGBase
{
public:
	CVGProgressBar_Base();
	virtual ~CVGProgressBar_Base();

public:
	int				m_iRoundedCornerAngle;
	Gdiplus::Rect	m_rectPadding;

	Gdiplus::Font*	m_pFont;
	CStringW		m_strMainText;

protected:
	BOOL			m_bRedrawPanel;
	CachedBitmap*	m_pCachedBitmap;
	virtual void	PaintCachedBitmap	(IN  Gdiplus::Graphics& gfx, IN  Gdiplus::Rect argRect);
	virtual void	ReDrawPanel			();

	Color			m_clrBorder;
	INT				m_iBorderTickness;

	Gdiplus::Color	m_clrStatus1;
	Gdiplus::Color	m_clrStatus2;
	int				m_iStatusBarColor;

	int				GetStatusBarColor	();
	void			SetStatusBarColor	(IN int value);
	
	Gdiplus::Color	m_clrFirst;
	Gdiplus::Color	m_clrSecond;
	int				m_iFillDegree;

	int				GetFillDegree		();
	void			SetFillDegree		(IN int value);

	Gdiplus::RectF GetMainArea(IN REAL ClientWidth, IN REAL ClientHeight)
	{
		return Gdiplus::RectF(	(REAL)(m_rectPadding.GetLeft()), 
								(REAL)m_rectPadding.GetTop(), 
								(REAL)(((ClientWidth - (m_rectPadding.GetLeft() + m_rectPadding.GetRight())) * m_iFillDegree) / 100), 
								(REAL)(ClientHeight - m_rectPadding.GetBottom() - m_rectPadding.GetTop()));
	};

	Gdiplus::RectF GetMainAreaBackground(IN REAL ClientWidth, IN REAL ClientHeight)
	{
		return Gdiplus::RectF(	(REAL)(m_rectPadding.GetLeft()), 
								(REAL)m_rectPadding.GetTop(), 
								(REAL)(ClientWidth - (m_rectPadding.GetLeft() + m_rectPadding.GetRight())), 
								(REAL)(ClientHeight - m_rectPadding.GetBottom() - m_rectPadding.GetTop()));
	};

	virtual void RenderBar(IN Gdiplus::Graphics& graphics, IN Gdiplus::RectF ClientRect);

public:

	void	SetBorder(IN Gdiplus::Color clrBorder, IN INT iTickness)
	{
		m_clrBorder			= clrBorder;
		m_iBorderTickness	= iTickness;
	};

	void	SetBorder(IN COLORREF clrBorder, IN INT iTickness)
	{
		m_clrBorder.SetFromCOLORREF(clrBorder);
		m_iBorderTickness = iTickness;
	};

	void	SetForegroundColor(IN Gdiplus::Color clrFirst, IN Gdiplus::Color clrSecond)
	{
		m_clrFirst = clrFirst;
		m_clrSecond = clrSecond;
	};

	void	SetForegroundColorRef(IN COLORREF clrFirst, IN COLORREF clrSecond)
	{
		m_clrFirst.SetFromCOLORREF(clrFirst);
		m_clrSecond.SetFromCOLORREF(clrSecond);
	};
};

#endif // VGProgressBar_Base_h__

