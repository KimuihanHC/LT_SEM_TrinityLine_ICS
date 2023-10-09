//*****************************************************************************
// Filename	: Wnd_SocketView.h
// Created	: 2016/03/11
// Modified	: 2016/03/11
//
// Author	: PiRing
//	
// Purpose	: 기본 화면용 윈도우
//*****************************************************************************
#ifndef Wnd_SocketView_h__
#define Wnd_SocketView_h__

#pragma once

#include "Wnd_BaseView.h"
#include "VGStatic.h"
#include "Def_DataStruct.h"
#include "List_SocketInfo.h"

//=============================================================================
// CWnd_SocketView
//=============================================================================
class CWnd_SocketView : public CWnd_BaseView
{
	DECLARE_DYNAMIC(CWnd_SocketView)

public:
	CWnd_SocketView();
	virtual ~CWnd_SocketView();

	protected:
	DECLARE_MESSAGE_MAP()

	afx_msg int		OnCreate				(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnSize					(UINT nType, int cx, int cy);

	afx_msg void	OnBnClicked_Find		();
	afx_msg void	OnBnClicked_ResetYield	();
	afx_msg void	OnBnClicked_ResetYield_A();
	afx_msg void	OnBnClicked_Unregister	();
	afx_msg void	OnBnClicked_Unregister_A();
	afx_msg void	OnBnClicked_ScrollUp	();
	afx_msg void	OnBnClicked_ScrollDown	();
	afx_msg void	OnBnClicked_SaveCSV		();
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

	// 소켓 정보 삭제
	// - 수율 초기화
	// - 모든 소켓의 수율 초기화
	CMFCButton			m_bn_ResetYield;
	CMFCButton			m_bn_ResetYield_All;

	// Unregister
	CMFCButton			m_bn_Unregister;
	CMFCButton			m_bn_Unregister_All;

	// CSV 파일로 소켓별 수율 저장
	CMFCButton			m_bn_Save_CSV;

	
	// 데이터
	enPermissionMode	m_InspMode			= enPermissionMode::Permission_Operator;
	CSocketMonitoring*	m_pstSocketInfo		= nullptr;

	CString				m_szReportPath;		// 리포트 파일 기본 저장 경로

	// 소켓 리스트 UI 초기화
	void	Init_SocketUI_List				();

	// 소켓 RFID로 소켓 순서 구하기
	int		Find_SocketOrder				(__in LPCTSTR IN_szRFID);

	void	Reset_Yield_SelectedSocket		();
	void	Reset_Yield_All					();
	void	Unregister_SocketInfo			();
	void	Unregister_SocketInfo_All		();

public:

	virtual void	OnLanguage				(__in uint8_t IN_nLanguage);

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

	void	SetPath_Report					(__in LPCTSTR IN_szReportPath);
#if defined(EES_XML)//20231003
public:
	void	Update_SocketStatus_LOTID		(__in LPCTSTR IN_szRFID);
#endif	
};

#endif // Wnd_SocketView_h__
