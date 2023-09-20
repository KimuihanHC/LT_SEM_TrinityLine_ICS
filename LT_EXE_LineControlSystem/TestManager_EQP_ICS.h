//*****************************************************************************
// Filename	: 	TestManager_EQP_ICS.h
// Created	:	2021/1/27 - 13:54
// Modified	:	2021/1/27 - 13:54
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#ifndef TestManager_EQP_ICS_h__
#define TestManager_EQP_ICS_h__


#pragma once

#include "TestManager_EQP.h"

//-----------------------------------------------------------------------------
// CTestManager_EQP_ICS
//-----------------------------------------------------------------------------
class CTestManager_EQP_ICS : public CTestManager_EQP
{
public:
	CTestManager_EQP_ICS();
	virtual ~CTestManager_EQP_ICS();

protected:

	//-----------------------------------------------------
	// 옵션
	//-----------------------------------------------------
	// 환경설정 데이터 불러오기
	virtual BOOL	OnLoad_Option					();	
	virtual void	OnInit_Devicez					(__in HWND hWndOwner = NULL);

	//-----------------------------------------------------
	// 설비 공용 항목
	//-----------------------------------------------------
	// 설비 데이터, 통신, GUI 매칭
	virtual void	OnMatchingEquipment				();
	
	//-----------------------------------------------------
	// 설비 이벤트
	//-----------------------------------------------------
	// Equipment : 설비 통신 연결 처리 (시간 동기, 언어, 설비 ID)
	void OnEvent_EquipmentConnection			(__in uint8_t IN_FromEqp, __in bool bConnect);
	void OnEvent_AutoMode_Notify				(__in uint8_t IN_FromEqp);
	void OnEvent_EquipmentAlarm					(__in uint8_t IN_FromEqp, __in uint8_t IN_nProcessStatus);
	void OnEvent_Tester_PortStatus				(__in uint8_t IN_FromEqp, __in uint8_t IN_nPortIndex);
	void OnEvent_Tester_CoveyorStatus			(__in uint8_t IN_FromEqp, __in uint8_t IN_nConveyorIndex);
	void OnEvent_Eqp_EndOfProduction			(__in uint8_t IN_FromEqp);
	void OnEvent_CheckEmptyEquipment			(__in uint8_t IN_FromEqp);

	// Loader : 소켓 등록 
	// - 제품이 불량 제품으로 등록된 경우 불량난 검사의 이전 설비들은 Bypass 처리
	void OnEvent_Loader_RegisterSocket			(__in LPCTSTR IN_szRFID);
	// Loader : 버퍼3 포트에 소켓 도착
	// - 목적 설비 설정
	void OnEvent_Loader_CheckTrackOut			(__in LPCTSTR IN_szRFID);
	// Loader : 소켓 배출 승인 (로더 투입전 버퍼3의 상태가 없음 -> 제품 있음으로 바뀌면, 배출 승인을 알려준다.)
	void OnEvent_Loader_TrackOut				(__in LPCTSTR IN_szRFID);

	// Tester : 소켓 투입 요청
	void OnEvent_Tester_TrackIn					(__in uint8_t IN_FromEqp, __in LPCTSTR IN_szRFID);
	// Tester : 소켓 검사 결과 통보
	// - 불량인 경우 제품정보 저장(불량난 검사 저장)
	// - 목적 설비 설정
	void OnEvent_Tester_EndInspection			(__in uint8_t IN_FromEqp, __in LPCTSTR IN_szRFID);
	// Tester : 소켓 배출 승인
	void OnEvent_Tester_TrackOut				(__in uint8_t IN_FromEqp, __in LPCTSTR IN_szRFID) {};
	
	// Unloader : 소켓 검사 결과 요청
	void OnEvent_Unloader_TrackIn				(__in LPCTSTR IN_szRFID);
	void OnEvent_Unloader_NG_Info				(__in LPCTSTR IN_szRFID);
	// Unloader : 소켓 등록 해제
	// - 소켓 최종 Log 작성 (최종 수율, 불량 제품 이력)
	void OnEvent_Unloader_UnregisterSocket		(__in LPCTSTR IN_szRFID);
	
	//-----------------------------------------------------
	// 
	//-----------------------------------------------------
	void	Proc_SetSocketTargetEquipment		(__in LPCTSTR IN_szRFID, __in uint8_t IN_nTargetEqpOrder);
	// Func : 목적 설비 설정 (그룹 내)
	void	Proc_SetTagetEquipment_Group		(__in LPCTSTR IN_szRFID, __in uint8_t IN_FromEqp);

	// Func : 목적 설비 설정
	bool	Proc_SetTagetEquipment				(__in uint8_t IN_FromEqp, __in LPCTSTR IN_szRFID);
	//bool	Proc_SetTagetEquipment_Rework		(__in uint8_t IN_FromEqp, __in LPCTSTR IN_szRFID);

	// Func : 소켓 배출 가능 판단
	bool	Proc_IsPossible_TrackOut_byLoader	(__in uint8_t IN_FromEqp, __in LPCTSTR IN_szRFID);
	bool	Proc_IsPossible_TrackOut			(__in uint8_t IN_FromEqp, __in LPCTSTR IN_szRFID, __in uint8_t IN_nPara);
	// Func : 검사 설비 상태 변경에 따른 배출 대기중인 소켓의 배출 가능 판단
	bool	Proc_IsPossible_TrackOut_byEvent	(__in uint8_t IN_FromEqp, __in LPCTSTR IN_szRFID);

	bool	Proc_EndOfProduction				(__in bool bEnd);

	bool	Proc_IsFarAway_TargetEqp			(__in uint8_t IN_nFromEqp, __in uint8_t IN_nTargetEqp);

	
	//-----------------------------------------------------
	// 설비 통신 (송신)
	//-----------------------------------------------------
	void	OnSend_TimeSync					();																// 시간 동기
	void	OnSend_TimeSync					(__in uint8_t IN_nEqpOrder);									// 시간 동기
	void	OnSend_UserLevel				(__in enPermissionMode IN_nLevel, __in LPCTSTR IN_szUserId);	// 사용자 등급
	void	OnSend_UserLevel				(__in uint8_t IN_nEqpOrder, __in enPermissionMode IN_nLevel, __in LPCTSTR IN_szUserId);	// 사용자 등급
	void	OnSend_Language					(__in uint8_t IN_nLanguage);									// 언어
	void	OnSend_Language					(__in uint8_t IN_nEqpOrder, __in uint8_t IN_nLanguage);			// 언어
	void	OnSend_Model					(__in uint8_t IN_nEqpOrder);									// 모델
	void	OnSend_OperationActiveStatus	(__in uint8_t IN_nStatus);										// 인라인 가동/비가동 상태

	void	OnSend_Accept_SocketTrackIn		(__in uint8_t IN_nEqpOrder, __in LPCTSTR IN_szRFID, __in uint8_t IN_nFlag, __in LPCTSTR IN_szBarcode);	// 소켓 투입 승인
	void	OnSend_Accept_SocketTrackOut	(__in uint8_t IN_nEqpOrder, __in LPCTSTR IN_szRFID);			// 배출 승인
	bool	OnSend_Forced_SocketTrackOut	(__in bool IN_bOnOff);											// 강제 배출
	bool	OnSend_Forced_SocketTrackOut	(__in uint8_t IN_nEqpOrder, __in bool IN_bOnOff);				// 강제 배출
	bool	OnSend_EndOfProduction			(__in bool bEnd = true);
	bool	OnSend_EndOfProduction			(__in uint8_t IN_nEqpOrder, __in bool bEnd = true);				// 생산 종료 (제품 도착시 앞 단계 설비들이 비어 있나 체크)
	
	bool	OnSend_Init						();								// 제어: 초기화
	bool	OnSend_Init						(__in uint8_t IN_nEqpOrder);	// 제어: 초기화
	bool	OnSend_Reset					();								// 제어: 리셋
	bool	OnSend_Reset					(__in uint8_t IN_nEqpOrder);	// 제어: 리셋
	bool	OnSend_Run						();								// 제어: 시작 (Auto)
	bool	OnSend_Run						(__in uint8_t IN_nEqpOrder);	// 제어: 시작 (Auto)
	bool	OnSend_Stop						();								// 제어: 정지 (Manual)
	bool	OnSend_Stop						(__in uint8_t IN_nEqpOrder);	// 제어: 정지 (Manual)
	bool	OnSend_BuzzerOff				();								// 제어: 부저 오프
	bool	OnSend_BuzzerOff				(__in uint8_t IN_nEqpOrder);	// 제어: 부저 오프
	bool	OnSend_ShowHide					(__in bool bShow);								// 제어: 핸들러 UI Show/Hide
	bool	OnSend_ShowHide					(__in uint8_t IN_nEqpOrder, __in bool bShow);	// 제어: 핸들러 UI Show/Hide
	
	//-----------------------------------------------------	
	// 검사전 체크 
	//-----------------------------------------------------
	// 모델 설정 데이터가 정상인가 판단
	virtual LRESULT	OnVerify_RecipeInfo				();	

	//-----------------------------------------------------
	// 결과 데이터 처리
	//-----------------------------------------------------
	// 제품 결과 판정 및 리포트 처리
	//virtual void	OnLog_EquipmentAlarm			(__in uint8_t IN_nEqpOder, __in const ST_AlarmStatus* IN_pAlarm);
	//virtual void	OnLog_SocketEvent				(__in LPCTSTR IN_szRFID, __in LPCTSTR IN_szLog);
	virtual void	OnReport_Socket					(__in LPCTSTR IN_szRFID);

	//virtual void	OnReport_Yield_Socket_Day		(__in LPCTSTR IN_szRFID);
	//virtual void	OnReport_Yield_Socket_Cumulative(__in LPCTSTR IN_szRFID);
	//virtual void	OnReport_Yield_Eqp_Day			(__in uint8_t IN_nEqpOder);
	//virtual void	OnReport_Yield_Eqp_Cumulative	(__in uint8_t IN_nEqpOder);

	//virtual void	OnUpdate_InOutCount				(__in bool IN_nIn, __in bool IN_nOut, __in bool IN_nRemove);

	//-----------------------------------------------------
	// 타이머 
	//-----------------------------------------------------
	virtual void	OnCreateTimer_UpdateUI			(__in DWORD DueTime = 5000, __in DWORD Period = 250);
	virtual void	OnDeleteTimer_UpdateUI			();
	virtual void	OnMonitor_TimeCheck				();
	virtual void	OnMonitor_UpdateUI				();

	//-----------------------------------------------------
	// 멤버 변수 & 클래스
	//-----------------------------------------------------

public:

	// 생성자 처리용 코드
	virtual void	OnInitialize					();
	// 소멸자 처리용 코드
	virtual	void	OnFinalize						();

	// 제어 권한 상태 설정	
	virtual void	OnSet_PermissionMode			(__in enPermissionMode nAcessMode);

};

#endif // TestManager_EQP_ICS_h__
