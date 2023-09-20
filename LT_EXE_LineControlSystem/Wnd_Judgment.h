//*****************************************************************************
// Filename	: 	Wnd_Judgment.h
// Created	:	2021/11/19 - 11:14
// Modified	:	2021/11/19 - 11:14
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
#ifndef Wnd_Judgment_h__
#define Wnd_Judgment_h__

#pragma once

#include <stdint.h>
#include "VGStatic.h"
#include "Def_Socket_Info.h"

//-----------------------------------------------------------------------------
// CWnd_Judgment
//-----------------------------------------------------------------------------
class CWnd_Judgment : public CWnd
{
	DECLARE_DYNAMIC(CWnd_Judgment)

public:
	CWnd_Judgment();
	virtual ~CWnd_Judgment();

	// Unload  소켓 판정 결과
	enum enHead_Item
	{
		HI_Judgment,		// 1. Judgment
		HI_NG_Code,			// 2. Barcode
		HI_NG_Equipment,	// 3. Equipment
		HI_NG_Para,			// 4. Para
		HI_RFID,			// 5. NG Code
		HI_Barcode,			// 6. RFID
		HI_OutTime,			// 7. Time

		HI_MaxCount,
	};

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg int		OnCreate				(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnSize					(UINT nType, int cx, int cy);
	afx_msg BOOL	OnEraseBkgnd			(CDC* pDC);
	afx_msg HBRUSH	OnCtlColor				(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
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

	uint8_t		m_nLanguage			= 1;

	// Header Judgment (Pass, Fail, Total)
	CVGStatic		m_st_Header[HI_MaxCount];

	// Data 9개
	CVGStatic		m_st_Data[HI_MaxCount];

	void	Init_TestResult			();

public:

	void	OnLanguage				(__in uint8_t IN_nLanguage);

	void	Set_Background			(__in COLORREF color, __in BOOL bRepaint = TRUE);

	void	Set_Margin				(__in UINT nLeft, __in UINT nTop, __in UINT nRight, __in UINT nBottom);
	void	Set_Spacing				(__in UINT nSpacing, __in UINT nCateSpacing);
	// 내부 컨트롤 글꼴 설정
	void	Set_FontSize_Caption	(__in UINT nWidth, __in UINT nHeghit);
	void	Set_FontSize_Data		(__in UINT nWidth, __in UINT nHeghit);
	
	void	Set_TestResult			(__in LPCTSTR IN_szRFID, __in LPCTSTR IN_szBarcode, __in ST_TestResult* IN_pstResult);
	void	Set_TestResult			(__in const CSocketInfo_Unit* IN_Socket);
};

#endif // Wnd_Judgment_h__


