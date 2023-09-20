//*****************************************************************************
// Filename	: VGButton.h
// Created	: 2013/3/7
// Modified	: 2013/3/7 - 12:55
//
// Author	: PiRing
//	
// Purpose	: 
//*****************************************************************************
#ifndef VGButton_h__
#define VGButton_h__


#pragma once
#include "VGBase_Panel.h"

//=============================================================================
// CVGButton
//=============================================================================
class CVGButton : public CButton,
				  public CVGBase_Panel
{
	DECLARE_DYNAMIC(CVGButton)

public:
	CVGButton();
	virtual ~CVGButton();

protected:
	DECLARE_MESSAGE_MAP()

	virtual void	PreSubclassWindow		();
	afx_msg void	OnSize					(UINT nType, int cx, int cy);
	afx_msg void	OnPaint					();
	afx_msg void	OnLButtonDown			(UINT nFlags, CPoint point);
	afx_msg void	OnLButtonUp				(UINT nFlags, CPoint point);	
	afx_msg void	OnLButtonDblClk			(UINT nFlags, CPoint point);
	afx_msg void	OnMouseMove				(UINT nFlags, CPoint point);
	afx_msg void	OnMouseHover			(UINT nFlags, CPoint point);
	afx_msg void	OnMouseLeave			();

	LRESULT			OnBMClick				(WPARAM,LPARAM);
	LRESULT			OnBMSetCheck			(WPARAM wParam,LPARAM lParam);	
	LRESULT			OnBMGetCheck			(WPARAM,LPARAM);
	LRESULT			OnBMSetState			(WPARAM wParam,LPARAM lParam);
	LRESULT			OnBMGetState			(WPARAM,LPARAM);

	CachedBitmap*	m_pCachedBitmap;	
	virtual void	ReDrawPanel				();	
	virtual void	PaintCachedBitmap		(Graphics& gfx, Gdiplus::Rect& argRect, BOOL bSelected = FALSE, int nChecked = BST_UNCHECKED);

	int				m_nChecked;
	BOOL			m_bHighlight; // Highlight 상태

	// Mouse Move 이벤트 트래킹용 플래그
	BOOL			m_bTracking;

	// Content 부분
	void			InitContent				(UINT nCount = 1);
	

public:

	virtual	void	SetContentBackColorRedraw	(Gdiplus::Color clrBack, UINT nContentIndex = 0);
	

	
	
};

#endif // VGButton_h__