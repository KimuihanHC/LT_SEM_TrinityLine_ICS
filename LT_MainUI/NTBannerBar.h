//*****************************************************************************
// Filename	: 	NTBannerBar.h
//
// Created	:	2010/03/18
// Modified	:	2010/03/18 - 17:09
//
// Author	:	PiRing
//	
// Purpose	:	배너 & 메뉴 버튼 & 시계 출력용 윈도우
//*****************************************************************************
#ifndef __NTBannerBar_H__
#define __NTBannerBar_H__

#pragma once

//#include "ImageMenuButton.h"
#include "NTClockWnd.h"
#include "NTStyle.h"

static const UINT uiBannerBarID = AFX_IDW_DIALOGBAR +2;

//#define SC_OPTION	0xF200

#define USE_CLOCK		// 시계 표시
#define USE_BANNER		// 배너 영역 표시 

//=============================================================================
// CNTBannerBar
//=============================================================================
class CNTBannerBar : public CPane
{
	DECLARE_DYNAMIC(CNTBannerBar)

public:
	CNTBannerBar();
	virtual ~CNTBannerBar();

	// Overrides
protected:
	virtual BOOL	PreCreateWindow			(CREATESTRUCT& cs);
	virtual void	DoPaint					(CDC* pDCPaint);
	virtual BOOL	PreTranslateMessage		(MSG* pMsg);
	virtual UINT	HitTest					(const CPoint& pt) const;

public:
	virtual BOOL	Create					(CWnd* pParentWnd, UINT nID = uiBannerBarID);
	virtual BOOL	CreateEx				(CWnd* pParentWnd, UINT nID = uiBannerBarID);
	virtual CSize	CalcFixedLayout			(BOOL, BOOL);

protected:
	afx_msg LRESULT OnSetText				(WPARAM, LPARAM lParam);
	afx_msg void	OnSize					(UINT nType, int cx, int cy);
	afx_msg int		OnCreate				(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnExitProgram			();
	afx_msg void	OnOption				();
	DECLARE_MESSAGE_MAP()

	// Context 팝업 메뉴 동작을 막음..
	//virtual BOOL	OnShowControlBarMenu(CPoint point) {return FALSE;};
	bool			m_bUseClock				= true;

protected:

	CString				m_strCaption;

	HICON				m_hIcon;
	CSize				m_szIcon;

	BOOL				m_bParentActive;
	BOOL				m_bParentMaximize;

	int					m_SystemHeight;
	int					m_CaptionHeight;
	CFont				m_CaptionFont;

	CSize				m_szBtnSize;
	//CImageMenuButton	m_BtnOption;
	//CImageMenuButton	m_BtnExit;

#ifdef USE_CLOCK
	CNTClockWnd			m_wndClock;
#endif

	UINT				m_nID_BnExit;
	UINT				m_nID_BnOption;

	UINT				m_wmOption;

	int					m_nTextFontSize			= 22;
	int					m_nTextOffset_Left		= 40;
	int					m_nTextOffset_Top		= 0;
	int					m_nIconOffset_Left		= 20;
	int					m_nIconOffset_Top		= 0;


	Gdiplus::Color		m_clrText				= Color(220, 0, 0);
	Gdiplus::Color		m_clrOutline1			= Color(0, 128, 128);
	Gdiplus::Color		m_clrOutline2			= Color(255, 255, 255);
	int					m_nThickness1			= 1;
	int					m_nThickness2			= 1;

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

	void				SetButtonImage			(UINT nIDExit, UINT nIDOption)
	{
		m_nID_BnExit	= nIDExit;
		m_nID_BnOption	= nIDOption;
	};

	void				SetWM_Option			(UINT nWindowMessage) {m_wmOption = nWindowMessage;};

	void				Set_UseClock			(__in bool bUse)
	{
		m_bUseClock = bUse;
	};

	void				Set_TextFontSize		(__in int IN_nFontSize)
	{
		m_nTextFontSize = IN_nFontSize;
	};

	void				Set_TextPosition_Adjust(__in int IN_nLeft, __in int IN_nTop)
	{
		m_nTextOffset_Left = IN_nLeft;
		m_nTextOffset_Top = IN_nTop;
	};

	void				Set_IconPosition_Adjust(__in int IN_nLeft, __in int IN_nTop)
	{
		m_nIconOffset_Left = IN_nLeft;
		m_nIconOffset_Top = IN_nTop;
	};

	void				Set_TextColor(__in Gdiplus::Color clrText, __in Gdiplus::Color clrOutline1, __in Gdiplus::Color clrOutline2, __in int nThickness1, __in int nThickness2)
	{
		m_clrText		= clrText;
		m_clrOutline1	= clrOutline1;
		m_clrOutline2	= clrOutline2; 
		m_nThickness1	= nThickness1;
		m_nThickness2	= nThickness2;
	};

	void			Set_ClockFontSize		 (__in UINT nDateFontSize, __in UINT nTimeFontSize, __in UINT nSecondFontSize);
	void			Set_ClockPosition_Adjust (__in int nDate_x, __in int nDate_y, __in int nTime_x, __in int nTime_y, __in int nSec_x, __in int nSec_y);
};

inline
int CNTBannerBar::GetCaptionHeight () const
{
	return m_CaptionHeight;
}

inline
HFONT CNTBannerBar::GetCaptionFont () const
{
	return (HFONT)m_CaptionFont.GetSafeHandle ();
}

inline
BOOL CNTBannerBar::IsParentActive () const
{
	return m_bParentActive;
}

inline
BOOL CNTBannerBar::IsParentMaximize () const
{
	return m_bParentMaximize;
}

#endif // __NTBannerBar_H__


