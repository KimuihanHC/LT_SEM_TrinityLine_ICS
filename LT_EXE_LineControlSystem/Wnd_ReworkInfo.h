//*****************************************************************************
// Filename	: Wnd_ReworkInfo.h
// Created	: 2016/03/11
// Modified	: 2016/03/11
//
// Author	: PiRing
//	
// Purpose	: 기본 화면용 윈도우
//*****************************************************************************
#ifndef Wnd_ReworkInfo_h__
#define Wnd_ReworkInfo_h__

#pragma once

#include "Wnd_BaseView.h"
#include "VGStatic.h"
#include "Rework_Info.h"
#include "List_ReworkInfo.h"

//=============================================================================
// CWnd_ReworkInfo
//=============================================================================
class CWnd_ReworkInfo : public CWnd_BaseView
{
	DECLARE_DYNAMIC(CWnd_ReworkInfo)

public:
	CWnd_ReworkInfo();
	virtual ~CWnd_ReworkInfo();

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg int		OnCreate				(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnSize					(UINT nType, int cx, int cy);

	afx_msg void	OnBnClicked_Find		();
	afx_msg void	OnBnClicked_ScrollUp	();
	afx_msg void	OnBnClicked_ScrollDown	();

	afx_msg LRESULT	OnWM_UpdateReworkData	(WPARAM wParam, LPARAM lParam);

	CFont			m_font_Default;	
	CFont			m_Font;

	// UI
	CList_ReworkInfo	m_lc_ReworkList;

	// 페이지 Up/Dn
	CMFCButton			m_bn_PgUp;
	CMFCButton			m_bn_PgDn;

	// Barcode 검색 (??)
	CVGStatic			m_st_Find;
	CMFCMaskedEdit		m_ed_Barcode;
	CMFCButton			m_bn_Find;
	
	// 데이터
	enPermissionMode	m_InspMode			= enPermissionMode::Permission_Operator;
	CRework_Info*		m_pstReworkInfo		= nullptr;

	// 제품 불량 리스트 UI 초기화
	void	Init_ReworkUI_List				();

	// 제품 불량 Barcode로 제품 불량 순서 구하기
	int		Find_ReworkOrder				(__in LPCTSTR IN_szBarcode);


public:
	
	void	SetPtr_ReworkInfo				(__in CRework_Info* IN_pstReworkInfo)
	{
		m_pstReworkInfo = IN_pstReworkInfo;
	};

	// 검사 모드 설정
	void	Set_PermissionMode				(__in enPermissionMode InspMode);
	
	// 전체 제품 불량 정보 갱신
	void	Update_ReworkInfo				();


	// 제품 불량 데이터 갱신
	void	OnUpdate_ReworkData				(__in uint16_t nFlag, __in LPCTSTR szBarcode);
	void	OnUpdate_ReworkData_All			(__in uint16_t nFlag);

};

#endif // Wnd_ReworkInfo_h__
