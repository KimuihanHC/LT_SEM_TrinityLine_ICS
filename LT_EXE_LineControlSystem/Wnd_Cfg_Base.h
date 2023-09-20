//*****************************************************************************
// Filename	: 	Wnd_Cfg_Base.h
// Created	:	2016/3/14 - 10:57
// Modified	:	2016/3/14 - 10:57
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
#ifndef Wnd_Cfg_Base_h__
#define Wnd_Cfg_Base_h__

#pragma once
#include "Wnd_BaseView.h"

//-----------------------------------------------------------------------------
// CWnd_Cfg_Base
//-----------------------------------------------------------------------------
class CWnd_Cfg_Base : public CWnd_BaseView
{
	DECLARE_DYNAMIC(CWnd_Cfg_Base)

public:
	CWnd_Cfg_Base();
	virtual ~CWnd_Cfg_Base();

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg int		OnCreate				(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnSize					(UINT nType, int cx, int cy);
	virtual BOOL	PreCreateWindow			(CREATESTRUCT& cs);
	virtual BOOL	PreTranslateMessage		(MSG* pMsg);
	
	void	Create_Font				();
	void	Release_Font			();


	CFont	m_font_Default;			// Item 글꼴
	CFont	m_font_Data;			// Item Data 글꼴

	CSize	m_sizeCaption;			// Item 명칭 UI 크기
	CSize	m_sizeData;				// Item Data UI 크기
	RECT	m_Margin;				// 화면 가장자리 여백
	UINT	m_nSpacing;				// Item 사이 간격
	UINT	m_nCateSpacing;			// 카테고리 사이 간격

	CSize	m_nFontSize_Caption;	// Item 명칭 UI 글꼴 크기
	CSize	m_nFontSize_Data;		// Item Data UI 글꼴 크기

	// 버튼 이벤트 처리
	bool	m_bUse_ControlEvent		= false;
	bool	m_bUseFixedSize			= true;		// UI 컨트롤의 너비가 고정 or 윈도우 크기에 맞춤

public:

	void	Set_CaptionSize			(__in UINT nWidth, __in UINT nHeight);
	void	Set_DataSize			(__in UINT nWidth, __in UINT nHeight);
	void	Set_Margin				(__in UINT nLeft, __in UINT nTop, __in UINT nRight, __in UINT nBottom);
	void	Set_Spacing				(__in UINT nSpacing, __in UINT nCateSpacing);

	void	Set_FontSize_Caption	(__in UINT nWidth, __in UINT nHeghit);
	void	Set_FontSize_Data		(__in UINT nWidth, __in UINT nHeghit);
	
	// 버튼 이벤트 실시간 처리 옵션
	void	Enable_ControlEvent		(__in bool bEnable);

	void	Set_UseFixedSize		(__in bool bUse);
};

#endif // Wnd_Cfg_Base_h__


