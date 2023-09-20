//*****************************************************************************
// Filename	: View_SubMonitoring.h
// Created	: 2016/03/11
// Modified	: 2016/03/11
//
// Author	: PiRing
//	
// Purpose	: 기본 화면용 윈도우
//*****************************************************************************
#ifndef View_SubMonitoring_h__
#define View_SubMonitoring_h__

#pragma once

#include <afxwin.h>
#include "Wnd_BaseView.h"
#include "VGStatic.h"
#include "Def_DataStruct.h"
#include "List_SocketInfo.h"
#include "Wnd_Judgment.h"

//=============================================================================
// CView_SubMonitoring
//=============================================================================
class CView_SubMonitoring : public CWnd_BaseView
{
	DECLARE_DYNAMIC(CView_SubMonitoring)

public:
	CView_SubMonitoring();
	virtual ~CView_SubMonitoring();

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg int		OnCreate				(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnSize					(UINT nType, int cx, int cy);
	afx_msg void	OnClose					();
	afx_msg void	OnGetMinMaxInfo			(MINMAXINFO* lpMMI);
	virtual BOOL	PreCreateWindow			(CREATESTRUCT& cs);

	afx_msg void	OnBnClicked_Find		();
	afx_msg void	OnBnClicked_ScrollUp	();
	afx_msg void	OnBnClicked_ScrollDown	();
	afx_msg void	OnCbnSelendokSocketType	();

	afx_msg LRESULT	OnWM_UpdateSocketData	(WPARAM wParam, LPARAM lParam);

	CFont			m_font_Default;	
	CFont			m_Font;

	// UI
	CVGStatic			m_st_SocketType;
	CComboBox			m_cb_SocketType;
	CList_SocketInfo	m_lc_SocketList[Max_SocketTypeCount]; // 소켓 타입으로 구분 (H , V)

	// 페이지 Up/Dn
	CMFCButton			m_bn_PgUp;
	CMFCButton			m_bn_PgDn;

	// RFID 검색 (??)
	CVGStatic			m_st_Find;
	CMFCMaskedEdit		m_ed_RFID;
	CMFCButton			m_bn_Find;


	// Unload  소켓 판정 결과
	CWnd_Judgment		m_wndJudge;


	
	// 데이터
	enPermissionMode	m_InspMode			= enPermissionMode::Permission_Operator;
	CSocketMonitoring*	m_pstSocketInfo		= nullptr;

	// 소켓 리스트 UI 초기화
	void	Init_SocketUI_List				();

	// 소켓 RFID로 소켓 순서 구하기
	int		Find_SocketOrder				(__in LPCTSTR IN_szRFID);


public:
	
	void	SetPtr_SocketInfo				(__in CSocketMonitoring* IN_pstSocketInfo)
	{
		m_pstSocketInfo = IN_pstSocketInfo;
	};

	// 화면에 표시되는 소켓JIG 타입 설정
	void	Set_SocketType					(__in uint8_t IN_nSocketType);

	// 검사 모드 설정
	void	Set_PermissionMode				(__in enPermissionMode InspMode);
	
	// 전체 소켓 정보 갱신
	void	Update_SocketInfo				();

	// 개별 소켓 상태 정보 갱신
	void	Update_SocketStatus				(__in LPCTSTR IN_szRFID);
	void	Update_SocketStatus_All			();
	void	Update_SocketStatus_Barcode		(__in LPCTSTR IN_szRFID);
	void	Update_SocketStatus_Status		(__in LPCTSTR IN_szRFID);
	void	Update_SocketStatus_Equipment	(__in LPCTSTR IN_szRFID);
	void	Update_SocketStatus_Location	(__in LPCTSTR IN_szRFID);
	void	Update_SocketStatus_Target		(__in LPCTSTR IN_szRFID);
	void	Update_SocketStatus_Yield		(__in LPCTSTR IN_szRFID);
	void	Update_SocketStatus_Yield_All	();
	void	Update_SocketStatus_TestResult	(__in LPCTSTR IN_szRFID);

	// 소켓 데이터 갱신
	void	OnUpdate_SocketData				(__in uint16_t nFlag, __in LPCTSTR szRFID);
	void	OnUpdate_SocketData_All			(__in uint16_t nFlag);

	// 검사 결과
	void	Set_TestResult					(__in LPCTSTR IN_szRFID, __in LPCTSTR IN_szBarcode, __in ST_TestResult* IN_pstResult);
	void	Set_TestResult					(__in const CSocketInfo_Unit* IN_Socket);
};

#endif // View_SubMonitoring_h__
