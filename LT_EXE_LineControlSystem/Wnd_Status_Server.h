//*****************************************************************************
// Filename	: 	Wnd_Status_Server.h
// Created	:	2023/3/24 - 10:57
// Created	:	2023/3/27 - 10:57
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
#ifndef Wnd_Status_Server_h__
#define Wnd_Status_Server_h__

#pragma once
#include <stdint.h>
#include "Wnd_Status_Base.h"

#include "Def_ConfigServer.h"
#include "Server.h"
#include "Wnd_Yield_Eqp.h"
#include "Dlg_LossWindow_PopupMenu.h"
#include "Dlg_TerminalMessage_Popup.h"
#include "Dlg_OpCall_Popup.h"

#include "TestManager_Device.h"
//#include "RegServer.h"
//-----------------------------------------------------------------------------
// CWnd_Status_Server
//-----------------------------------------------------------------------------
class CWnd_Status_Server : public CWnd_Status_Base
{
	DECLARE_DYNAMIC(CWnd_Status_Server)

public:
	CWnd_Status_Server();
	virtual ~CWnd_Status_Server();

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg int		OnCreate				(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnSize					(UINT nType, int cx, int cy);
	afx_msg LRESULT	OnWmServerNotify		(WPARAM wParam, LPARAM lParam);
	
	virtual void	MoveWindow_Status		(int x, int y, int nWidth, int nHeight);

	virtual void	Create_Font				();
	virtual void	Release_Font			();

	afx_msg void	OnBnClicked_EES_Mode_Control		();
	afx_msg void	OnBnClicked_LossWindow_Control		();
	// 설비 상태
	//CServer	*			m_pEquipmentStatus	= nullptr;
	CServer	*			m_pServerStatus = nullptr;
	uint8_t				m_nEqpOrder			= 0;		// 라인의 설비 순서

	// 컨트롤
	CMFCButton			m_bn_RMS;	
	CMFCButton			m_bn_LossWindow;

	// 경광등
	CVGStatic			m_st_LED[enLED_Type::LED_MaxCount];

	CVGStatic			m_st_EqpName;		// 설비 명칭
	CVGStatic			m_st_EqpID;			// Eqp id -> 설비 매칭 여부 (필요하면 사용...)
	CVGStatic			m_st_IpAddress;		// ip Addres, 통신 연결 상태
	CVGStatic			m_st_OperMode;		// 운용모드 (Auto, Manual, ..)
	/*
	CVGStatic			m_st_EquipMentState;// IDLE,	
	CVGStatic			m_st_EquipMentStateDisplay;// IDLE,
	CVGStatic			m_st_LossCode;
	CVGStatic			m_st_RMS;
	*/
	//CWnd_Yield_Eqp		m_wnd_Yield;		// 생산 수율

	CDlg_LossWindow_PopupMenu	* LossWindow		= nullptr;
	CDlg_TerminalMessage_Popup	* TerminalMessage	= nullptr;
	CDlg_OpCall_Popup			* OpCall			= nullptr;
	//std::vector<ST_TerminalLog>		m_nTerminalLog;
	//2023.05.02a
	//CWnd_LogView				m_wndLogView;

#ifdef USE_EQP_TACTTIME

	enum enTacttime
	{
		TT_Curr,
		TT_Curr_Data,
		TT_Ave,
		TT_Ave_Data,

		TT_MaxCount,
	};
	CVGStatic			m_st_Tacttime[TT_MaxCount];

#endif
	
	uint8_t				m_nPortCount		= 0;	// UI 설정용 포트 개수
	uint8_t				m_nConveyorCount	= 0;	// UI 설정용 컨베이어 개수

	// 컨트롤 크기 설정
	int16_t				m_nTop_Status		= 123;	// 설비 상태가 표시될 y 좌표 (Port, 컨베이어 상태)
	uint16_t			m_nCtrl_Height		= 20;	// 경광등 Static 높이
	uint16_t			m_nPort_Width		= 10;	// Port Static 너비
	uint16_t			m_nLine_Depth		= 90;	// 리터너, 로더의 트랜스퍼 길이
	uint16_t			m_nDefInfo_Height	= 320;
	uint16_t			m_nYield_Height		= 140;	// 수율 표시 높이

	//virtual void Popup_SubMenu_EquipMentState();
	COLORREF			Get_Color(__in CString lParam);

	virtual void Popup_SubMenu_EESMode();
	virtual void Popup_SubMenu_LossWindow();
#if TESTTERMINAL
	virtual void Popup_Terminal_Message(ST_xml_REQUEST_TERMINAL_MESSAGE * IN_DATA);
#endif
#if TESTOPCALL
	virtual void Popup_OPCALL(ST_xml_REQUEST_OPCALL * IN_DATA);
#endif
#if SOCKET
	void SetUI_PortStatus		(__in CVGStatic* IN_pwndPort, __in uint8_t IN_nStatus);
	void SetUI_PortStatus		(__in CVGStatic* IN_pwndPort, __in const ST_PortStatus* IN_nStatus);
	void SetUI_ConveyorStatus	(__in CVGStatic* IN_pwndConveyor, __in uint8_t IN_nStatus, __in uint8_t IN_nExistSocket);
	void SetUI_ConveyorStatus	(__in CVGStatic* IN_pwndConveyor, __in const ST_ConveyorStatus* IN_nStatus);
#endif	//SOCKET
	//2023.04.28a uhkim
	void	SetUI_EQUIPMENTSTATE(__in CVGStatic* IN_nPort, LPCTSTR IN_DATA);
public:
	// 통신 상태
	void Set_ClientConnection(__in uint8_t IN_nConStatus);
	// 설비 인증
	void Set_VerifySvrConnection(__in bool bVerified);

	// 권한 설정
	void	Set_PermissionMode		(__in enPermissionMode IN_PermissionMode);

	void	SetPtr_EquipmentInfo	(__in uint8_t IN_nEqpNo,  CServer* IN_pEquipment);
#if TEST
	void	SetBaseDataConfig		(CServer* IN_pServer);
#endif
	// 설비를 사용 유/무 선택
	virtual void Set_EnableEquipment(__in bool IN_bEnable);

	// 설비 사용 스킵 여부
	virtual void Set_Skip			(__in bool IN_bSkip);
	
	// 검사 운영 모드 (Equipment Operate Mode)
	void Set_OperatingMode			(__in uint8_t IN_OperMode);
	
	// 설비 가동 상태 (Equipment Process Status)
	void Set_ProcessStatus			(__in uint8_t IN_nStatus);

	// 경광등 상태 (LED Status)
	void Set_LED_Status				(__in uint8_t IN_nLED_Status, __in bool IN_bBuzzer);

	// 설비에서 핸들러/검사 프로그램 Show/Hide 선택
	void Set_ShowHandler			(__in bool IN_bShow);

	// 알람 종류 및 메세지
	void Set_Alarm					(__in uint8_t IN_nAlarmCode, __in LPCTSTR IN_szMessage);

	// 설비와 서버의 시간 동기화 여부
	void Set_TimeSync				(__in bool IN_bTimeSync);
	
	// 수율 갱신
	//void Set_Yield					(__in const CYield_Equipment* IN_pYield);

	// 생산 모드 (시작/종료)
	void Set_Production				(__in bool IN_bEnd, __in uint8_t IN_nChecking);

	// 설비내 소켓 정보
	// buffer, test 1, tese 2, 컨베이어, 리턴 컨베이어
#if SOCKET
	virtual void Set_PortClear		(__in uint8_t IN_nPortIndex);
	virtual void Set_PortStatus		(__in uint8_t IN_nPortIndex, __in uint8_t IN_nStatus);
	virtual void Set_PortStatus		(__in uint8_t IN_nPortIndex, __in const ST_PortStatus* IN_nStatus);
	virtual void Set_ConveyorStatus	(__in uint8_t IN_nConveyorIndex, __in uint8_t IN_nStatus, __in uint8_t IN_nExistSocket);
	virtual void Set_ConveyorStatus	(__in uint8_t IN_nConveyorIndex, __in const ST_ConveyorStatus* IN_nStatus);
#endif//SOCKET

	//====================================================
	// Add
	//====================================================
	//2023.05.30
	void Start();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
#if ADD_SOCKET_EES_XML
	virtual void Set_SvrNotify_EQUIPMENTSTATE(__in lt::Report_Equipment_State_Args::Args& IN_DATA);
	virtual void Set_SvrNotify_EQUIPMENTSTATEDISPLAY(__in lt::Reply_Equipment_State_Display_Args::Args& IN_DATA);
	void	Set_SvrNotify_RGBDISPLAY(LPCTSTR IN_DATA);
	void	Set_SvrNotify_LOSSWINDOW(LPCTSTR IN_DATA);
	void	Set_SvrNotify_LOSSCODE(LPCTSTR IN_DATA);
	void	Set_SvrNotify_TERMINAL_MESSAGE(__in lt::Reply_Terminal_Message_Args::Args&    IN_DATA);
	void	Set_SvrNotify_OPCALL(__in lt::Reply_Opcall_Args::Args& IN_DATA);
	void	Set_SvrNotify_RMS(LPCTSTR IN_DATA);
	
	void	SetUI_EQUIPMENTSTATEDISPLAY(__in CVGStatic* IN_nPort, LPCTSTR IN_DATA);
	void	SetUI_RGBDISPLAY(__in CVGStatic* IN_nPort, LPCTSTR IN_DATA);


#endif
	virtual void	Set_EquipmentID(__in CServer* IN_Data);
};

#endif // Wnd_Status_Equipment_h__


