//*****************************************************************************
// Filename	: 	TestManager_Device.h
// Created	:	2016/9/28 - 19:54
// Modified	:	2016/9/28 - 19:54
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
#ifndef TestManager_Device_h__
#define TestManager_Device_h__

#pragma once

#include <stdint.h>
#include "TestManager_Base.h"
#include "LT_Option.h"
#include "Def_ErrorCode.h"

#include "IcsCommunicator.h"

//-----------------------------------------------------------------------------
// CTestManager_Device
//-----------------------------------------------------------------------------
class CTestManager_Device : public CTestManager_Base
{
public:
	CTestManager_Device();
	virtual ~CTestManager_Device();

protected:

	//-------------------------------------------------------------------------
	// 옵션
	//-------------------------------------------------------------------------
	stLT_Option		m_stOption;
	// 환경설정 데이터 불러오기
	virtual BOOL	OnLoad_Option					();
	//virtual void	OnInitUISetting					(__in HWND hWndOwner = NULL);

	//-------------------------------------------------------------------------
	// 주변장치 제어
	//-------------------------------------------------------------------------

	// 전체 주변장치와 통신 연결 전의 초기 작업
	virtual void	OnInit_Devicez					(__in HWND hWndOwner = NULL);
	// 전체 주변장치와 통신 연결 시도
	virtual void	OnConnect_Devicez				();
	// 전체 주변장치의 연결 해제
	virtual void	OnDisconnect_Devicez			();

	//-------------------------------------------------------------------------
	// 통신 연결 상태 UI에 표시 
	//-------------------------------------------------------------------------
	
	virtual void	OnSetStatus_Server				(__in uint8_t nStatus){};
	virtual void	OnSetStatus_Client				(__in uint8_t nClientIndex, __in uint8_t nStatus){};
	
	//-------------------------------------------------------------------------
	// Error
	//-------------------------------------------------------------------------
	virtual void	OnAdd_ErrorInfo					(__in enErrorCode lErrorCode){};

	//-----------------------------------------------------
	// 주변 장치 제어용 클래스 모음
	//-----------------------------------------------------
	CIcsCommunicator*	m_pIcsComm;

public:

	// 생성자 처리용 코드
	virtual void	OnInitialize					();
	// 소멸자 처리용 코드
	virtual	void	OnFinalize						();

};

#endif // TestManager_Device_h__

