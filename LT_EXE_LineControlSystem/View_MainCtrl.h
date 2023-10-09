//*****************************************************************************
// Filename	: View_MainCtrl.h
// Created	: 2010/11/26
// Modified	: 2016/06/07
//
// Author	: PiRing
//	
// Purpose	: 
//*****************************************************************************
#ifndef View_MainCtrl_h__
#define View_MainCtrl_h__

#pragma once

#include "TestManager_EQP.h"
// UI
#include "Wnd_MainView.h"
#include "Wnd_SettingView.h"
#include "Wnd_SocketView.h"
#include "Wnd_LogView.h"
#include "View_MonitoringData.h"
#include "Define_MonitorInfoFunc.h"

typedef enum _enumSubView
{
	SUBVIEW_MAIN = 0,
	SUBVIEW_SOCKET,
	SUBVIEW_RECIPE,
	SUBVIEW_LOG,
	SUBVIEW_MAX,
}enumSubView;

//=============================================================================
// CView_MainCtrl 창
//=============================================================================
class CView_MainCtrl : public CWnd
{
// 생성입니다.
public:
	CView_MainCtrl();
	virtual ~CView_MainCtrl();

protected:

	afx_msg int		OnCreate				(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnSize					(UINT nType, int cx, int cy);
	afx_msg void	OnPaint					();
	afx_msg BOOL	OnEraseBkgnd			(CDC* pDC);
	afx_msg HBRUSH	OnCtlColor				(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void	OnTimer					(UINT_PTR nIDEvent);

	DECLARE_MESSAGE_MAP()
	
	virtual BOOL	PreCreateWindow			(CREATESTRUCT& cs);

	// 배경색 처리용.
	CBrush				m_brBkgr;

	//-----------------------------------------------------
	// 차일드 윈도우 구분용
	//-----------------------------------------------------	
	UINT					m_nWndIndex;
	CWnd*					m_pWndPtr[SUBVIEW_MAX];

	CWnd_MainView			m_wndMainView;
	CWnd_SocketView			m_wndSocketView;
	CWnd_SettingView		m_wndSettingView;
	CWnd_LogView			m_wndLogView;

	// 통신 상태 표시 Pane의 포인터.
	CWnd*					m_pwndCommPane				= nullptr;

	CView_MonitoringData	m_ViewSub;			// Sub 윈도우


	ST_MONITORINFO			m_infoMonitor;		// 모니터 정보 구조체
	
	uint8_t					m_nLanguage					= 1;

	//-----------------------------------------------------
	// 초기 설정 관련
	//-----------------------------------------------------
	// 생성자에서 초기화 할 세팅
	virtual void	OnInit_ConstructionSetting			(){};
	// Window 생성 후 세팅
	virtual void	OnInit_UISetting					(){};
	// 주변장치들 기본 설정
	virtual void	OnInit_DeviceSetting				(){};
	
	//-----------------------------------------------------
	// Sub 윈도우 설정 관련
	//-----------------------------------------------------
	void			Create_SubWindows			();
	virtual void	ShowWindow_SubMonitoring	(__in BOOL bShow);

	void			Update_Language				(__in uint8_t IN_nLanguage);

//=============================================================================
public: 
//=============================================================================
	
	// 윈도우 배경색 설정용 함수
	void			SetBackgroundColor			(__in COLORREF color, __in BOOL bRepaint = TRUE);

	// 차일드 윈도우 전환 시 사용
	virtual UINT	SwitchWindow				(__in UINT nIndex);
	// 장치 통신 상태 표시 윈도우 포인터 설정
	virtual void	SetCommPanePtr				(__in CWnd* pwndCommPane);
	
	// 옵션이 변경 되었을 경우 다시 UI나 데이터를 세팅하기 위한 함수
	virtual void	ReloadOption				(){};

	// 프로그램 로딩 끝난 후 자동 처리를 위한 함수
	virtual void	InitStartProgress			(){};	
	// 프로그램 종료시 처리해야 할 기능들을 처리하는 함수
	virtual void	FinalExitProgress			(){};
	
	//--------------------- TEST --------------------------
	virtual void	Test_Process				(__in UINT nTestNo) = 0;
	//--------------------- TEST --------------------------
		

};

#endif // View_MainCtrl_h__


