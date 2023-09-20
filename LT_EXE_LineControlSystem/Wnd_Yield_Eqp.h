//*****************************************************************************
// Filename	: 	Wnd_Yield_Eqp.h
// Created	:	2021/11/19 - 11:14
// Modified	:	2021/11/19 - 11:14
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
#ifndef Wnd_Yield_Eqp_h__
#define Wnd_Yield_Eqp_h__

#pragma once

#include <stdint.h>
#include "VGStatic.h"
#include "Def_Yield.h"

//-----------------------------------------------------------------------------
// CWnd_Yield_Eqp
//-----------------------------------------------------------------------------
class CWnd_Yield_Eqp : public CWnd
{
	DECLARE_DYNAMIC(CWnd_Yield_Eqp)

public:
	CWnd_Yield_Eqp();
	virtual ~CWnd_Yield_Eqp();

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg int		OnCreate				(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnSize					(UINT nType, int cx, int cy);
	afx_msg BOOL	OnEraseBkgnd			(CDC* pDC);
	afx_msg HBRUSH	OnCtlColor				(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL	PreCreateWindow			(CREATESTRUCT& cs);
	virtual BOOL	PreTranslateMessage		(MSG* pMsg);
	
	virtual void	Create_Font				();
	virtual void	Release_Font			();

	

	CFont	m_font_Default;			// Item 글꼴
	CFont	m_font_Data;			// Item Data 글꼴

	RECT	m_Margin;				// 화면 가장자리 여백
	UINT	m_nSpacing				= 5;	// Item 사이 간격
	UINT	m_nCateSpacing			= 10;;	// 카테고리 사이 간격

	CSize	m_nFontSize_Caption;	// Item 명칭 UI 글꼴 크기
	CSize	m_nFontSize_Data;		// Item Data UI 글꼴 크기

	uint8_t		m_nBorderThickness	= 1;
	COLORREF	m_clrBorder			= RGB(0, 0, 0);
	COLORREF	m_clrBackground		= RGB(255, 255, 255);
	CBrush		m_brBkgr;			// 배경색 처리용


	enum enHead_Judge
	{
		HJ_Pass,
		HJ_Fail,
		HJ_Total,

		HJ_MaxCount,
	};

	enum enHead_Para
	{
		HP_Left,
		HP_Center,
		HP_Right,
		HP_All,

		HP_MaxCount,
	};

	// 제목
	CVGStatic		m_st_Title;
	
	// Header Judgment (Pass, Fail, Total)
	CVGStatic		m_st_Head_Judge[HJ_MaxCount];

	// Header Para (L, R, Total)
	CVGStatic		m_st_Head_Para[HP_MaxCount];

	// Data 9개
	CVGStatic		m_st_Data[HJ_MaxCount][HP_MaxCount];

	// 설비가 로더인가? (로더는 L/R 양품 구분이 안됨, 불량은 가능)
	bool			m_bLoader		= false;
	uint8_t			m_nParaCount	= 2;

public:

	void	Set_Background			(__in COLORREF color, __in BOOL bRepaint = TRUE);

	void	Set_Margin				(__in UINT nLeft, __in UINT nTop, __in UINT nRight, __in UINT nBottom);
	void	Set_Spacing				(__in UINT nSpacing, __in UINT nCateSpacing);
	// 내부 컨트롤 글꼴 설정
	void	Set_FontSize_Caption	(__in UINT nWidth, __in UINT nHeghit);
	void	Set_FontSize_Data		(__in UINT nWidth, __in UINT nHeghit);
	
	void	SetText_Title			(__in LPCTSTR szTitle);
	void	SetColor_Title			(__in COLORREF clrTitle);

	// 윈도우 활성화/비활성화
	void	Set_EnableWindow		(__in bool IN_bEnable);
	void	Set_SkipWindow			(__in bool IN_bSkip);

	// 로더 설정
	void	Set_EqpLoader			(__in bool IN_bLoader);

	// 수율 설정
	void	Set_Yield				(__in const CYield_Equipment* IN_pYield);

	// 검사 파라 설정
	void	Set_TestParaCount			(__in uint8_t IN_nCount);
};

#endif // Wnd_Yield_Eqp_h__


