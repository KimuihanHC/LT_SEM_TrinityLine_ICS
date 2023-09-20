//*****************************************************************************
// Filename	: 	VGProgressBar_Variable_Base.h
// Created	:	2016/3/10 - 11:07
// Modified	:	2016/3/10 - 11:07
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
#ifndef VGProgressBar_Variable_Base_h__
#define VGProgressBar_Variable_Base_h__

#pragma once
#include "VGBase.h"


enum enProgressbarType
{
	PRGB_LEFT,
	PRGB_RIGHT,
	PRGB_CENTER,
	PRGB_MaxNum,
};

enum enProgressbarMode
{
	PRGB_M_PERCENT,
	PRGB_M_POSITION,
	PRGB_M_MaxNum,
};
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
class CVGProgressBar_Variable_Base : public CVGBase
{
public:
	CVGProgressBar_Variable_Base();
	virtual ~CVGProgressBar_Variable_Base();

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

	UINT	m_nStartPos;
	UINT	m_nEndPos;

	float	m_fOffsetPos;

	float	m_fPos;
	BOOL	m_bMode = PRGB_M_POSITION;

	

	int				GetFillDegree		();
	void			SetFillDegree		(IN int value);


	Gdiplus::RectF GetMainArea(IN REAL ClientWidth, IN REAL ClientHeight)
	{

		RectF rectMain;
		if (m_bMode == PRGB_M_POSITION)
		{
			rectMain = GetMainArea_Pos(ClientWidth, ClientHeight);
		}
		else{
			double dData = m_iFillDegree + m_fOffsetPos;
			rectMain = Gdiplus::RectF((REAL)(m_rectPadding.GetLeft()),
				(REAL)m_rectPadding.GetTop(),
				(REAL)(((ClientWidth - (m_rectPadding.GetLeft() + m_rectPadding.GetRight())) * dData) / 100),
				(REAL)(ClientHeight - m_rectPadding.GetBottom() - m_rectPadding.GetTop()));
		}
		return rectMain;
	};


	Gdiplus::RectF GetMainArea_Right(IN REAL ClientWidth, IN REAL ClientHeight)
	{

		RectF rectMain;
		if (m_bMode == PRGB_M_POSITION)
		{
			rectMain = GetMainArea_Pos(ClientWidth, ClientHeight, PRGB_RIGHT);
		}
		else{
			double dData = m_iFillDegree + m_fOffsetPos;
			rectMain = Gdiplus::RectF((REAL)(((ClientWidth - (m_rectPadding.GetLeft() + m_rectPadding.GetRight())) * (100 - dData)) / 100),
				(REAL)m_rectPadding.GetTop(),
				(REAL)(ClientWidth),
				(REAL)(ClientHeight - m_rectPadding.GetBottom() - m_rectPadding.GetTop()));

		}
		return rectMain;
	};
	Gdiplus::RectF GetMainArea_Center(IN REAL ClientWidth, IN REAL ClientHeight)
	{

		RectF rectMain;
		if (m_bMode == PRGB_M_POSITION)
		{
			rectMain = GetMainArea_Pos(ClientWidth, ClientHeight, PRGB_CENTER);
		}
		else{

			double dData = m_iFillDegree + m_fOffsetPos;

			if (dData < 50)
			{
				rectMain = Gdiplus::RectF((REAL)(((ClientWidth - (m_rectPadding.GetLeft() + m_rectPadding.GetRight())) * (dData)) / 100),
					(REAL)m_rectPadding.GetTop(),
					(REAL)(((ClientWidth - (m_rectPadding.GetLeft() + m_rectPadding.GetRight())) * (50)) / 100),
					(REAL)(ClientHeight - m_rectPadding.GetBottom() - m_rectPadding.GetTop()));			
			}
			else{
				rectMain = Gdiplus::RectF((REAL)(((ClientWidth - (m_rectPadding.GetLeft() + m_rectPadding.GetRight())) * (50)) / 100), 
					(REAL)m_rectPadding.GetTop(),
					(REAL)(((ClientWidth - (m_rectPadding.GetLeft() + m_rectPadding.GetRight())) * (dData)) / 100),
					(REAL)(ClientHeight - m_rectPadding.GetBottom() - m_rectPadding.GetTop()));

			}
	
		}
		return rectMain;
	};

	Gdiplus::RectF GetMainArea_Pos(IN REAL ClientWidth, IN REAL ClientHeight, UINT nMode = 0)
	{
		RectF rectMain;
		int nGap = m_nEndPos - m_nStartPos;
		double dHalf = (m_nEndPos - m_nStartPos) / 2;

		double dOneRect = ClientWidth / nGap;

		double dData = m_fPos + m_fOffsetPos;

		switch (nMode)
		{
		case PRGB_LEFT:
		{
						  rectMain = Gdiplus::RectF((REAL)(m_rectPadding.GetLeft()),
							  (REAL)m_rectPadding.GetTop(),
							  (REAL)((dOneRect * dData)),
							  (REAL)(ClientHeight - m_rectPadding.GetBottom() - m_rectPadding.GetTop()));
		}
				break;
		case PRGB_RIGHT:
		{
						   rectMain = Gdiplus::RectF((REAL)(dOneRect * (m_nEndPos - dData)),
							   (REAL)m_rectPadding.GetTop(),
							   (REAL)(ClientWidth),
							   (REAL)(ClientHeight - m_rectPadding.GetBottom() - m_rectPadding.GetTop()));
		}
			break;
		case PRGB_CENTER:
		{
							if (dHalf <= dData)
							{
								double dbW = abs((dOneRect * (dHalf)) - (dOneRect * (dData)));

								rectMain = Gdiplus::RectF((REAL)(((ClientWidth - (m_rectPadding.GetLeft() + m_rectPadding.GetRight())) * (50)) / 100),
									(REAL)m_rectPadding.GetTop(),
									(REAL)(dbW),
									(REAL)(ClientHeight - m_rectPadding.GetBottom() - m_rectPadding.GetTop()));
							}
							else{

								double dbW = abs((dOneRect * (dHalf)) - (dOneRect * (dData)));

								rectMain = Gdiplus::RectF((REAL)((dOneRect * (dData))),
									(REAL)m_rectPadding.GetTop(),
									(REAL)(dbW),
									(REAL)(ClientHeight - m_rectPadding.GetBottom() - m_rectPadding.GetTop()));
							}
						
		}
			break;
		default:
			break;
		}
	
		return rectMain;
	};


	Gdiplus::RectF GetMainAreaBackground(IN REAL ClientWidth, IN REAL ClientHeight)
	{
		return Gdiplus::RectF(	(REAL)(m_rectPadding.GetLeft()), 
								(REAL)m_rectPadding.GetTop(), 
								(REAL)(ClientWidth - (m_rectPadding.GetLeft() + m_rectPadding.GetRight())), 
								(REAL)(ClientHeight - m_rectPadding.GetBottom() - m_rectPadding.GetTop()));
	};

	virtual void RenderBar(IN Gdiplus::Graphics& graphics, IN Gdiplus::RectF ClientRect);
	enProgressbarType m_enPrg_Type = PRGB_LEFT;
public:
	void	SetType(enProgressbarType nType){

		m_enPrg_Type = nType;
	};

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

#endif // VGProgressBar_Variable_Base_h__

