//*****************************************************************************
// Filename	: 	Wnd_MainView.h
// Created	:	2021/11/22 - 12:29
// Modified	:	2021/11/22 - 12:29
//
// Author	:	piring
//	
// Purpose	:	 기본 화면용 윈도우
//*****************************************************************************
#ifndef Wnd_MainView_h__
#define Wnd_MainView_h__

#pragma once

#include "Wnd_BaseView.h"
#include "VGStatic.h"
#include "Def_DataStruct.h"
#include "Wnd_Status_Tester.h"
#include "Wnd_Status_Loader.h"
#include "Wnd_Status_Returner.h"
#include "Wnd_Status_Handler.h"
#include "Wnd_TesterSelection.h"
#include <vector>
#if defined(EES_XML)//20231003
#include "Wnd_Status_Server.h"
#include "Wnd_Status_EES.h"
#endif

//#define		USE_DYNAMIC_WND_CREATE		// 동적으로 CWnd 생성하는 방식

//=============================================================================
// CWnd_MainView
//=============================================================================
class CWnd_MainView : public CWnd_BaseView
{
	DECLARE_DYNAMIC(CWnd_MainView)

public:
	CWnd_MainView();
	virtual ~CWnd_MainView();

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg int		OnCreate				(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnSize					(UINT nType, int cx, int cy);

	void			MoveWindow_TestSelect	(int x, int y, int nWidth, int nHeight, BOOL bRepaint = TRUE);
	void			MoveWindow_Equipment	(UINT nEquipmentIdx, int x, int y, int nWidth, int nHeight, BOOL bRepaint = TRUE);
	void			MoveWindow_EquipmentEx	(UINT nEquipmentIdx, int x, int y, int nWidth, int nHeight, BOOL bRepaint = TRUE);


	CFont			m_font_Default;	
	CFont			m_Font;

	// 설비 상태 UI 
#ifdef USE_DYNAMIC_WND_CREATE
	std::vector<CWnd_Status_Equipment*>		m_wndEqpTesterList;
#else
	CWnd_Status_Tester						m_wndEqpTesterList[MAX_EQUIPMENT_COUNT];
#endif

	CWnd_Status_Handler						m_wndEqpHandler;
	CWnd_Status_Loader						m_wndEqpLoader;
	CWnd_Status_Returner					m_wndEqpReturner;

	std::vector<CWnd_Status_Equipment*>		m_pwndEquipmentList;

	// 검사 진행 여부 UI
	CWnd_TesterSelection					m_wndTestSelect;


	// 설비 열 최대 개수
	uint8_t				m_nMaxCol			= 14;	// 14 -> 15 (2022.11.04)
	uint8_t				m_nMinCol			= 14;	// (2022.11.04)
	// 설비 행 최대 개수
	uint8_t				m_nMaxRow			= 10;

	// 데이터
	enPermissionMode	m_PermissionMode	= enPermissionMode::Permission_Operator;
	ST_SystemInfo*		m_pstSysInfo		= nullptr;

	// 핸들러 사용 여부
	bool				m_bUseHandler		= false;

	void	RemoveAll_EquipmentUI		();
	void	Create_EquipmentUI			(__in CConfig_Eqp* IN_pEqpuipment, __in const RECT& IN_Rect);

	// 설비 리스트 UI 초기화
	void	Init_EquipmentUI_List		();

public:

	virtual void	OnLanguage			(__in uint8_t IN_nLanguage);
	
	// 메인 시스템 정보 구조체의 포인터를 설정
	void	SetPtr_SystemInfo			(__in ST_SystemInfo* IN_pstSysInfo)
	{
		m_pstSysInfo = IN_pstSysInfo;
	};
	
	// 권한 설정
	void	Set_PermissionMode			(__in enPermissionMode IN_PermissionMode);

	// 설비 윈도우의 핸들 구하기
	HWND	Get_EquipmentHWND			(__in uint8_t IN_nEqpOrder);
	
	// 레시피 정보 갱신
	void	Update_LineInfo				();

	// 설비를 사용 유/무 선택
	void	UpdateUI_EnableEquipment	(__in uint8_t IN_nEqpNo, __in bool IN_bEnable);
#if defined(EES_XML)//20231003
protected:
	std::vector<CWnd_Status_Server*>		m_pwndServerList;
	bool									m_bUseEES = false;	
	void									Init_ServerUI_List();
public:
	CWnd_Status_EES							m_wndSvrEES;
	HWND									Get_ServerHWND(__in uint8_t IN_nSvrOrder);
public:
	void									Update_ServerInfo();
#endif
};
#if defined(EES_XML)//20231003
extern CWnd_MainView* mWnd_MainView;
#endif
#endif // Wnd_MainView_h__
