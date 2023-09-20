//*****************************************************************************
// Filename	: View_MonitoringData.h
// Created	: 2016/03/11
// Modified	: 2016/03/11
//
// Author	: PiRing
//	
// Purpose	: 기본 화면용 윈도우
//*****************************************************************************
#ifndef View_MonitoringData_h__
#define View_MonitoringData_h__

#pragma once

#include <afxwin.h>
#include "Wnd_BaseView.h"
#include "VGStatic.h"
#include "Wnd_Judgment.h"
#include "Wnd_SocketView.h"
#include "Wnd_FailInfo.h"

//=============================================================================
// CView_MonitoringData
//=============================================================================
class CView_MonitoringData : public CWnd/*_BaseView*/
{
	DECLARE_DYNAMIC(CView_MonitoringData)

public:
	CView_MonitoringData();
	virtual ~CView_MonitoringData();

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg int		OnCreate				(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnSize					(UINT nType, int cx, int cy);
	afx_msg BOOL	OnEraseBkgnd			(CDC* pDC);
	afx_msg HBRUSH	OnCtlColor				(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void	OnClose					();
	afx_msg void	OnGetMinMaxInfo			(MINMAXINFO* lpMMI);
	virtual BOOL	PreCreateWindow			(CREATESTRUCT& cs);

	void			MoveWindow_Tab			(int x, int y, int nWidth, int nHeight);
	afx_msg void	OnTabSelect				(UINT nID);

	CFont			m_font_Default;	
	CFont			m_Font;

	enum enTab
	{
		Tab_Socket = 0,
		Tab_FailInfo,

		Tab_MaxCount,
	};

	int						m_nTabWidth		= 140;
	int						m_nTabHeight	= 50;
	CMFCButton				m_rb_Tab[Tab_MaxCount];
	CVGStatic				m_frame_Tab;
	std::vector<CWnd*>		m_pwndTab;

	CWnd_SocketView		m_wndSocket;	// 소켓 모니터링
	CWnd_FailInfo		m_wndFailInfo;	// NG Code 카운트 모니터링
	CWnd_Judgment		m_wndJudge;		// Unload  소켓 판정 결과

	// 배경색 처리용//
	CBrush				m_brBkgr;

	// 데이터//
	enPermissionMode	m_InspMode			= enPermissionMode::Permission_Operator;

	uint8_t				m_nLanguage			= 1;
	
	void			Set_TabWindow			(__in uint8_t IN_nTabIndex);

public:

	virtual void	OnLanguage				(__in uint8_t IN_nLanguage);

	// 윈도우 배경색 설정용 함수//
	void	SetBackgroundColor				(__in COLORREF color, __in BOOL bRepaint = TRUE);
	
	// 소켓 정보 포인터 설정//
	void	SetPtr_SocketInfo				(__in CSocketMonitoring* IN_pstSocketInfo);

	// 화면에 표시되는 소켓JIG 타입 설정//
	void	Set_SocketType					(__in uint8_t IN_nSocketType);

	// 검사 모드 설정//
	void	Set_PermissionMode				(__in enPermissionMode InspMode);
	
	// 전체 소켓 정보 갱신//
	void	Update_SocketInfo				();

	// 개별 소켓 상태 정보 갱신//
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

	// 소켓 데이터 갱신//
	void	OnUpdate_SocketData				(__in uint16_t nFlag, __in LPCTSTR szRFID);
	void	OnUpdate_SocketData_All			(__in uint16_t nFlag);

	// 검사 결과//
	void	Set_TestResult					(__in LPCTSTR IN_szRFID, __in LPCTSTR IN_szBarcode, __in ST_TestResult* IN_pstResult);
	void	Set_TestResult					(__in const CSocketInfo_Unit* IN_Socket);

	// 불량 정보 갱신//
	void	SetPtr_FailInfo					(__in CFailInfo* IN_pstFailInfo);
	void	Update_FailInfo					();

	// 리포트 파일 저장 경로//
	void	SetPath_Report					(__in LPCTSTR IN_szReportPath);
};

#endif // View_MonitoringData_h__
