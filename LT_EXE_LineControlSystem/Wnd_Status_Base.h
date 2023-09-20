//*****************************************************************************
// Filename	: 	Wnd_Status_Base.h
// Created	:	2021/11/19 - 11:14
// Modified	:	2021/11/19 - 11:14
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
#ifndef Wnd_Status_Base_h__
#define Wnd_Status_Base_h__

#pragma once

#include <stdint.h>
#include "VGStatic.h"

//-----------------------------------------------------------------------------
// CWnd_Status_Base
//-----------------------------------------------------------------------------
class CWnd_Status_Base : public CWnd
{
	DECLARE_DYNAMIC(CWnd_Status_Base)

public:
	CWnd_Status_Base();
	virtual ~CWnd_Status_Base();

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

	CSize	m_sizeCaption;			// Item 명칭 UI 크기
	CSize	m_sizeData;				// Item Data UI 크기
	RECT	m_Margin;				// 화면 가장자리 여백
	UINT	m_nSpacing				= 5;	// Item 사이 간격
	UINT	m_nCateSpacing			= 10;;	// 카테고리 사이 간격

	CSize	m_nFontSize_Caption;	// Item 명칭 UI 글꼴 크기
	CSize	m_nFontSize_Data;		// Item Data UI 글꼴 크기

	// 버튼 이벤트 처리
	bool	m_bUse_ControlEvent		= false;
	bool	m_bUseFixedSize			= true;		// UI 컨트롤의 너비가 고정 or 윈도우 크기에 맞춤

	// 외곽 프레임 설정값
	uint8_t		m_nTitle_Height		= 30;
	COLORREF	m_clrTitleBk		= RGB(255, 255, 255);
	uint8_t		m_nBorderThickness	= 1;
	COLORREF	m_clrBorder			= RGB(0, 0, 0);
	COLORREF	m_clrBackground		= RGB(255, 255, 255);
	CBrush		m_brBkgr;			// 배경색 처리용

	// UI  컨트롤
	CVGStatic		m_st_Title;		// Title 바에 버튼 삽입(??) - 상태화면/결과화면, 설비 제어버튼
	CVGStatic		m_st_Border;	// 외곽선

	uint8_t			m_nLanguage		= 1;

public:

	virtual void	OnLanguage			(__in uint8_t IN_nLanguage);

	// 외곽 보더 크기, 색상 설정
	virtual void	Set_Border			(__in uint8_t nThickness, __in COLORREF clrBorder);
	// 타이틀바 크기, 색상 설정
	virtual void	Set_TitleBar		(__in uint8_t nHeight, __in COLORREF clrBack);
	// 내부 배경색 설정
	virtual void	Set_Background		(__in COLORREF color, __in BOOL bRepaint = TRUE);

	// 내부 컨트롤 크기 설정
	void	Set_CaptionSize			(__in UINT nWidth, __in UINT nHeight);
	void	Set_DataSize			(__in UINT nWidth, __in UINT nHeight);
	void	Set_Margin				(__in UINT nLeft, __in UINT nTop, __in UINT nRight, __in UINT nBottom);
	void	Set_Spacing				(__in UINT nSpacing, __in UINT nCateSpacing);
	// 내부 컨트롤 글꼴 설정
	void	Set_FontSize_Caption	(__in UINT nWidth, __in UINT nHeghit);
	void	Set_FontSize_Data		(__in UINT nWidth, __in UINT nHeghit);
	
	// 버튼 이벤트 실시간 처리 옵션
	void	Enable_ControlEvent		(__in bool bEnable);
	// 내부 컨트롤의 크기가 가변 or 고정 선택
	void	Set_UseFixedSize		(__in bool bUse);


	

	// 내부 배경 색상 설정

	// 깜박임 ?


	void	SetText_Title			(__in LPCTSTR szTitle);
	void	SetColor_Title			(__in COLORREF clrTitle);
};

#endif // Wnd_Status_Base_h__


