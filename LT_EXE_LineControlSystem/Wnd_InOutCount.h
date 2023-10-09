//*****************************************************************************
// Filename	: Wnd_InOutCount.h
// Created	: 2016/03/11
// Modified	: 2016/03/11
//
// Author	: PiRing
//	
// Purpose	: 기본 화면용 윈도우
//*****************************************************************************
#ifndef Wnd_InOutCount_h__
#define Wnd_InOutCount_h__

#pragma once

#include "Wnd_BaseView.h"
#include "VGStatic.h"
#include "Def_InOutCount.h"

//=============================================================================
// CWnd_InOutCount
//=============================================================================
class CWnd_InOutCount : public CWnd_BaseView
{
	DECLARE_DYNAMIC(CWnd_InOutCount)

public:
	CWnd_InOutCount();
	virtual ~CWnd_InOutCount();



protected:
	DECLARE_MESSAGE_MAP()

	afx_msg int		OnCreate				(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnSize					(UINT nType, int cx, int cy);

	CFont			m_font_Default;	
	CFont			m_Font;

	// UI
	CVGStatic			m_st_Caption[SCnt_MaxCount];
	CVGStatic			m_st_Data[SCnt_MaxCount];
	
	ST_InOutCount*		m_pstCountInfo		= nullptr;

	// 제품 불량 리스트 UI 초기화
	void	Init							();

	// 제품 불량 Barcode로 제품 불량 순서 구하기


public:
	
	void	SetPtr_CountInfo				(__in ST_InOutCount* IN_pstCountInfo)
	{
		m_pstCountInfo = IN_pstCountInfo;
	};

	
	// 전체 제품 불량 정보 갱신
	void	Update_CountInfo				();
	void	Update_CountInfo				(__in const ST_InOutCount* IN_pstCountInfo);


};

#endif // Wnd_InOutCount_h__
