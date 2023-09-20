//*****************************************************************************
// Filename	: 	View_MainCtrl_ICS.h
// Created	:	2021/1/27 - 13:55
// Modified	:	2021/1/27 - 13:55
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#ifndef View_MainCtrl_ICS_h__
#define View_MainCtrl_ICS_h__


#pragma once

#include "View_MainCtrl.h"
#include "TestManager_EQP_ICS.h"

//=============================================================================
// CView_MainCtrl_ICS
//=============================================================================
class CView_MainCtrl_ICS : public CView_MainCtrl, public CTestManager_EQP_ICS
{
public:
	CView_MainCtrl_ICS();
	virtual ~CView_MainCtrl_ICS();

protected:
	afx_msg int		OnCreate		(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnSize			(UINT nType, int cx, int cy);

	// 로그 메세지
	afx_msg	LRESULT	OnWM_LogMsg					(WPARAM wParam, LPARAM lParam);

	// 검사 제어
	afx_msg	LRESULT	OnWM_SwitchPermissionMode	(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnWM_ChangeRecipe			(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnWM_ChangedSetting			(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnWM_EqpCtrlCmd				(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnWM_TestSelect				(WPARAM wParam, LPARAM lParam);
	
	afx_msg LRESULT	OnWM_Eqp_Connection			(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnWM_Eqp_AutoMode			(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnWM_Eqp_Alarm				(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnWM_Eqp_PortStatus			(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnWM_Eqp_ConveyorStatus		(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnWM_Eqp_EndOfProduction	(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnWM_Eqp_CheckEmptyEqp		(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT	OnWM_Loader_RegisterSocket	(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnWM_Loader_Check_TrackOut	(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnWM_Tester_TrackIn			(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnWM_Tester_EndInspection	(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnWM_Unload_ReqTestResult	(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnWM_Unload_NG_Info			(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnWM_Unload_UnregisterSocket(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT	OnWM_Update_SocketData		(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnWM_Update_SocketData_All	(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

	//=========================================================================
	// CView_MainCtrl 재정의
	//=========================================================================
	//-----------------------------------------------------
	// 초기 설정 관련
	//-----------------------------------------------------
	// 생성자에서 초기화 할 세팅
	virtual void	OnInit_ConstructionSetting		();
	// Window 생성 후 세팅
	virtual void	OnInit_UISetting				();
	// 주변장치들 기본 설정
	virtual void	OnInit_DeviceSetting			();

	//-------------------------------------------------------------------------
	// 파일 불러오기 및 세팅
	bool			OnLoad_LineInfo					(__in bool IN_bNotifySettingWnd = true);
	bool			OnLoad_SocketInfo				(__in bool IN_bNotifySettingWnd = true);
	bool			OnLoad_UserInfo					(__in bool IN_bNotifySettingWnd = true);
	//bool			OnLoad_AlarmInfo				(__in bool IN_bNotifySettingWnd = true);
	bool			OnLoad_NGCodeInfo				(__in bool IN_bNotifySettingWnd = true);
	bool			OnLoad_DebugInfo				(__in bool IN_bNotifySettingWnd = true);
	bool			OnLoad_ModelInfo				();
	bool			OnLoad_FailInfo					();

	bool			OnLoad_Prev_LineInfo			();
	bool			OnLoad_Prev_LineInfo_OnlyYield	();

	// 설비 데이터, 통신, GUI 매칭
	virtual void	OnMatchingEquipment				();
	bool			Compare_LineConfig_ServRestart	(__in CConfig_Line* IN_Old, __in CConfig_Line* IN_New);

	//=========================================================================
	// CTestManager_EQP_ICS 재정의
	//=========================================================================
	//-----------------------------------------------------
	// 로그/파일 처리
	//-----------------------------------------------------
	virtual void	OnInit_LogFolder				();

	//-----------------------------------------------------
	// UI 업데이트
	//-----------------------------------------------------
	virtual void	OnSetStatus_Production			(__in bool IN_bEnd);

	//-----------------------------------------------------
	// 결과 데이터 처리
	//-----------------------------------------------------
	virtual void	OnLog_EquipmentAlarm			(__in uint8_t IN_nEqpOder, __in const ST_AlarmStatus* IN_pAlarm);
	virtual void	OnLog_RegisterSocket			(__in LPCTSTR IN_szRFID);

	// 제품 결과 화면에 표시
	virtual void	OnReport_Socket					(__in LPCTSTR IN_szRFID);
	virtual void	OnReport_Socket_NG				(__in LPCTSTR IN_szRFID);

	// 검사 진행 시간 체크 (설비, 파라 L/R)
	virtual void	OnUpdate_ElapsedTime_Unit		(__in UINT nEquipmentIdx, __in uint8_t IN_nPara);
	virtual void	OnUpdate_ElapsedTime_All		();

public: 
	//=========================================================================
	// CView_MainCtrl 재정의
	//=========================================================================
	// 차일드 윈도우 전환 시 사용
	virtual UINT	SwitchWindow				(__in UINT nIndex);
	// 장치 통신 상태 표시 윈도우 포인터 설정
	virtual void	SetCommPanePtr				(__in CWnd* pwndCommPane);

	// 옵션이 변경 되었을 경우 다시 UI나 데이터를 세팅하기 위한 함수
	virtual void	ReloadOption				();

	// 프로그램 로딩 끝난 후 자동 처리를 위한 함수
	virtual void	InitStartProgress			();	
	virtual BOOL	InitStartDeviceProgress		();	

	// 프로그램 종료시 처리해야 할 기능들을 처리하는 함수
	virtual void	FinalExitProgress			();

	//=========================================================================
	// CTestManager_EQP_ICS 재정의
	//=========================================================================
	// 검사기 종류 설정
	virtual void	OnSet_SystemType			(__in enInsptrSysType nSysType);

	// 로그 메세지 처리용 함수
	virtual void	OnAdd_Log					(__in LPCTSTR lpszLog, __in BOOL bError = FALSE, __in UINT nLogType = LOGTYPE_NONE, __in BOOL bOnlyLogType = FALSE);
	virtual void	OnAdd_Log					(__in LPCTSTR lpszLog, __in UINT nLogType = LOGTYPE_NONE);

	// 제어 권한 설정
	virtual void	OnInit_PermissionMode		(__in enPermissionMode nAcessMode);	
	virtual void	OnSet_PermissionMode		(__in enPermissionMode nAcessMode);	

	void			Proc_LineCtrlCmd			(__in uint8_t IN_nCmdIndex);
	void			Proc_EquipmentCtrlCmd		(__in uint8_t IN_nCmdIndex, __in uint8_t IN_nEqpOrder);


	//=========================================================================
	// TEST 코드
	//=========================================================================
	virtual void	Test_Process				(__in UINT nTestNo);	
	//--------------------- TEST --------------------------





};

#endif // View_MainCtrl_ICS_h__
