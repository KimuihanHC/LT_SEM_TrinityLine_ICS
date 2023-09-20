//*****************************************************************************
// Filename	: 	TestManager_Base.h
// Created	:	2016/5/16 - 17:01
// Modified	:	2016/5/16 - 17:01
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
#ifndef TestManager_Base_h__
#define TestManager_Base_h__

#pragma once

#include "LogFile.h"
#include "LogFile_V2.h"
#include "SplashScreenEx.h"
#include "Def_Enum_Cm.h"

//-----------------------------------------------------------------------------
// CTestManager_Base
//-----------------------------------------------------------------------------
class CTestManager_Base
{
public:
	CTestManager_Base();
	~CTestManager_Base();

protected:

	// 프로그램 종료 이벤트
	HANDLE			m_hEvent_ProgramExit;
	// 강제 정지 이벤트
	HANDLE			m_hEvent_ForcedStop;

	// 프로그램 종료 시 TRUE 설정
	BOOL			m_bExitFlag;

	void			SetEvent_ForcedStop			();

	//-----------------------------------------------------
	// 동기화 처리
	//-----------------------------------------------------
	HANDLE			m_hSemaphore;
	DWORD			m_dwSemaphoreWaitTime;

	DWORD			LockSemaphore				();
	BOOL			UnlockSemaphore				();
	BOOL			WaitSemaphore				(__in DWORD dwTimeout = 2000);

	//-----------------------------------------------------
	// LOG
	//-----------------------------------------------------
	CLogFile_V2		m_logFile;
	CLogFile_V2		m_Log_ErrLog;

	// 로그 폴더 초기 설정
	virtual void	OnInit_LogFolder			();
	// 로그 저장
	virtual void	OnLog						(__in LPCTSTR szLog, ...);
	virtual void	OnLog						(__in UINT nLogType, __in LPCTSTR szLog, ...);
	// 오류 로그 저장
	virtual void	OnLog_Err					(__in LPCTSTR szLog, ...);
	// UI상에 로그 표시
	virtual void	OnSetLog_UI					(__in LPCTSTR lpszLog, __in BOOL bError = FALSE, __in UINT nLogType = 0, __in COLORREF clrTextColor = RGB(0, 0, 0)){};

	//-----------------------------------------------------
	// 스플래시 윈도우 설정
	//-----------------------------------------------------
	CSplashScreenEx		m_wndSplash;
	// 스플래쉬 윈도우 생성
	void			CreateSplashScreen			(__in CWnd* pWndParent, __in UINT nBitmapID);
	// 스플래쉬 윈도우 소멸
	void			DeleteSplashScreen			();
	// 스플래쉬 윈도우 팝업
	void			ShowWindow_SplashScreen		(__in BOOL bShow = TRUE);
	// 스플래쉬 윈도우 표시 및 메세지 출력
	virtual void	OnShow_SplashScreen			(__in BOOL bShow, __in LPCTSTR szText = NULL);

	//-----------------------------------------------------
	// 주변장치 제어
	//-----------------------------------------------------
	// 주변 장치 초기 상태 설정
	virtual void	OnInit_Devicez				(__in HWND hWndOwner = NULL){};
	// 주변 장치 통신 연결
	virtual void	OnConnect_Devicez			(){};
	// 주변 장치 통신 연결 해제
	virtual void	OnDisconnect_Devicez		(){};
	// 주변 장치와 클래스 연동 설정
	virtual void	OnLinkDevicezToClass		(){};

	//-----------------------------------------------------
	// 타이머
	//-----------------------------------------------------
	HANDLE			m_hTimer_TimeCheck;
	HANDLE			m_hTimer_Update_UI;
	HANDLE			m_hTimerQueue;

	static VOID CALLBACK TimerRoutine_TimeCheck	(__in PVOID lpParam, __in BOOLEAN TimerOrWaitFired);
	static VOID CALLBACK TimerRoutine_UpdateUI	(__in PVOID lpParam, __in BOOLEAN TimerOrWaitFired);

	void			CreateTimerQueue_Mon		();
	void			DeleteTimerQueue_Mon		();

	virtual void	OnCreateTimer_TimeCheck		(__in DWORD DueTime = 2000, __in DWORD Period = 153);
	virtual void	OnDeleteTimer_TimeCheck		();
	virtual void	OnCreateTimer_UpdateUI		(__in DWORD DueTime = 5000, __in DWORD Period = 250);
	virtual void	OnDeleteTimer_UpdateUI		();

	virtual void	OnMonitor_TimeCheck			();
	virtual void	OnMonitor_UpdateUI			();

	enInsptrSysType		m_InspectionType		= Sys_Dev_Mod_1;

public:

	// 검사기 종류 설정
	virtual void	OnSet_SystemType			(__in enInsptrSysType nSysType);

	// 생성자 처리용 코드
	virtual void	OnInitialize				();
	// 소멸자 처리용 코드
	virtual	void	OnFinalize					();

	// 로그 메세지 처리용 함수
	virtual void	OnAdd_Log					(__in LPCTSTR lpszLog, __in UINT nLogType = LOGTYPE_NONE);
	//virtual void	OnAdd_Log					(__in LPCTSTR lpszLog, __in BOOL bError = FALSE, __in UINT nLogType = 0, __in BOOL bOnlyLogType = FALSE);
	virtual void	OnAdd_Log_F					(__in LPCTSTR lpszLog, ...);

};

#endif // TestManager_Base_h__

