//*****************************************************************************
// Filename	: VGGroupWnd.h
// Created	: 2015/12/5 - 16:34
// Modified	: 2015/12/5 - 16:34
//
// Author	: PiRing
//	
// Purpose	: 
//*****************************************************************************
#ifndef VGGroupWnd_h__
#define VGGroupWnd_h__

#pragma once
#include "VS2008Panel.h"

//=============================================================================
// CVGGroupWnd
//=============================================================================
class CVGGroupWnd : public CButton,
				    public CVS2008PanelBase
{
	DECLARE_DYNAMIC(CVGGroupWnd)

public:
	CVGGroupWnd();
	virtual ~CVGGroupWnd();

protected:
	DECLARE_MESSAGE_MAP()

	virtual void	PreSubclassWindow	();
	afx_msg void	OnSize				(UINT nType, int cx, int cy);
	afx_msg void	OnPaint				();
	afx_msg BOOL	OnEraseBkgnd		(CDC* pDC);
	afx_msg void	OnLButtonDown		(UINT nFlags, CPoint point);
	afx_msg void	OnLButtonUp			(UINT nFlags, CPoint point);
	afx_msg void	OnLButtonDblClk		(UINT nFlags, CPoint point);

	// 마우스 오른 버튼 -> 팝업 메뉴
	//afx_msg	void	OnRButtonUp			(UINT nFlags, CPoint point);

	LRESULT			OnBMClick			(WPARAM,LPARAM);
	LRESULT			OnBMSetCheck		(WPARAM wParam,LPARAM lParam);	
	LRESULT			OnBMGetCheck		(WPARAM,LPARAM);
	LRESULT			OnBMSetState		(WPARAM wParam,LPARAM lParam);
	LRESULT			OnBMGetState		(WPARAM,LPARAM);

	void			DrawPanel			(CDC* pDC);
	virtual void	ReDrawPanel			();
	
	ColorizerPanel	m_ColorizerSelected;
	ColorizerPanel	m_ColorizerHighlighted;

	UINT			m_nChecked;
	BOOL			m_nHighlight;

	virtual void	OnRedrawControl		();

	// 검사기 상태 UI 갱신
	virtual void	UpdateStatusByTimer	();

	// (초) 단위
 	BOOL			m_bTimeCheckFlag;

public:

	// 스테이지 상태 모니터링용
	virtual void	SetStatusBackColor	(Color clrBack);
	
};

#endif // VGGroupWnd_h__

