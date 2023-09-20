//*****************************************************************************
// Filename	: Wnd_BaseView.h
// Created	: 2010/11/23
// Modified	: 2010/11/23 - 9:57
//
// Author	: PiRing
//	
// Purpose	: 
//*****************************************************************************
#ifndef Wnd_BaseView_h__
#define Wnd_BaseView_h__

#pragma once

//=============================================================================
// CWnd_BaseView
//=============================================================================
class CWnd_BaseView : public CWnd
{
	DECLARE_DYNAMIC(CWnd_BaseView)
	// 생성입니다.
public:
	CWnd_BaseView();
	virtual ~CWnd_BaseView();

protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	DECLARE_MESSAGE_MAP()

	afx_msg int		OnCreate				(LPCREATESTRUCT lpCreateStruct);	
	afx_msg BOOL	OnEraseBkgnd			(CDC* pDC);
	afx_msg HBRUSH	OnCtlColor				(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	// 배경색 처리용
	CBrush					m_brBkgr;

	uint8_t					m_nLanguage		= 1;

public:

	virtual void	SetBackgroundColor		(COLORREF color, BOOL bRepaint = TRUE);
	virtual void	SetUseBlurSubWnd		(BOOL bUse = TRUE);
	
	virtual void	OnLanguage				(__in uint8_t IN_nLanguage);
};

#endif // Wnd_BaseView_h__
