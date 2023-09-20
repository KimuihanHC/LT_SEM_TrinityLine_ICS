//*****************************************************************************
// Filename	: 	NTCaptionBar.h
//
// Created	:	2010/03/10
// Modified	:	2010/03/10 - 10:42
//
// Author	:	PiRing
//	
// Purpose	:	Custom 캡션바
//*****************************************************************************
#ifndef __NTCaptionBar_H__
#define __NTCaptionBar_H__

#include "NTCaptionBarButton.h"

#pragma once

static const UINT uiCaprionBarID = AFX_IDW_DIALOGBAR + 1;

//=============================================================================
// CNTCaptionBar
//=============================================================================
class CNTCaptionBar : public CPane
{
	DECLARE_DYNAMIC(CNTCaptionBar)

public:
	CNTCaptionBar();
	virtual ~CNTCaptionBar();

	// Overrides
protected:
	virtual BOOL	PreCreateWindow			(CREATESTRUCT& cs);
	virtual void	DoPaint					(CDC* pDCPaint);
	virtual BOOL	PreTranslateMessage		(MSG* pMsg);
	virtual BOOL	OnCommand				(WPARAM wParam, LPARAM lParam);

public:
	virtual BOOL	Create					(CWnd* pParentWnd, UINT nID = uiCaprionBarID);
	virtual BOOL	CreateEx				(CWnd* pParentWnd, UINT nID = uiCaprionBarID);
	virtual CSize	CalcFixedLayout			(BOOL, BOOL);


protected:
	afx_msg LRESULT OnSetText				(WPARAM, LPARAM lParam);
	afx_msg void	OnSize					(UINT nType, int cx, int cy);
	afx_msg void	OnContextMenu			(CWnd* pWnd, CPoint point);
	afx_msg void	OnSysCommand			(UINT nID, LPARAM lParam);

	virtual UINT	HitTest					(const CPoint& pt) const;
	virtual void	ShowSysMenu				(const CPoint& point);

	DECLARE_MESSAGE_MAP()

public:
	CString 				m_strCaption;

	HICON					m_hIcon;
	CSize					m_szIcon;

	BOOL					m_bParentActive;
	BOOL					m_bParentMaximize;

	int						m_SystemHeight;
	int						m_CaptionHeight;
	CFont					m_CaptionFont;

	CNTCaptionBarButton		m_BtnMinimize;
	CNTCaptionBarButton		m_BtnMaximize;
	CNTCaptionBarButton		m_BtnClose;

	CNTCaptionBarButton		m_BtnOption;
	UINT				m_wmOption;
	void				SetWM_Option			(UINT nWindowMessage) {m_wmOption = nWindowMessage;};

public:
	virtual void		SetIcon					(HICON hIcon);

	void				SetCaptionHeight		(int nHeight);
	int					GetCaptionHeight		() const;
	void				SetCaptionFont			(const LOGFONT& lf);
	HFONT				GetCaptionFont			() const;

	virtual COLORREF	GetCaptionTextColor		() const;

	void				SetParentActive			(BOOL bParentActive = true);
	BOOL				IsParentActive			() const;
	void				SetParentMaximize		(BOOL bParentMaximize = true);
	BOOL				IsParentMaximize		() const;

	void				Enable_OptionButton		(__in bool bEnable);
};


inline
int CNTCaptionBar::GetCaptionHeight () const
{
	return m_CaptionHeight;
}

inline
HFONT CNTCaptionBar::GetCaptionFont () const
{
	return (HFONT)m_CaptionFont.GetSafeHandle ();
}

inline
BOOL CNTCaptionBar::IsParentActive () const
{
	return m_bParentActive;
}

inline
BOOL CNTCaptionBar::IsParentMaximize () const
{
	return m_bParentMaximize;
}

#endif // __NTCaptionBar_H__

