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
#if defined(EES_XML)//20231003
#include "Xml/xmlArgs.h"
#include "IcsCommunicatorEes.h"
#endif
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
#if !(EES_XML)//20231003
	// 전체 주변장치와 통신 연결 시도
	virtual void	OnConnect_Devicez				();
	// 전체 주변장치의 연결 해제
	virtual void	OnDisconnect_Devicez			();
#endif
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
#if defined(EES_XML)//20231003
protected:
	CIcsCommunicatorEes*	m_pIcsServer = nullptr;
	// 전체 주변장치와 통신 연결 시도
	virtual void	OnConnect_Devicez				(int nIn);
	// 전체 주변장치의 연결 해제
	virtual void	OnDisconnect_Devicez			(int nIn);
public:
	lt::Link_Test_Args::Args Add_Link_TestArgs(
		__in lt::ConstStringT equipmentId, lt::Request_Link_Test_Args::Args & Args);
	void RemoveLinkTestProcedure(__in lt::ConstStringT equipmentId, lt::XUUID ID);
	lt::Online_State_Args::Args Add_Online_StateArgs(
		__in lt::ConstStringT equipmentId,		lt::Report_Online_State_Args::Args & Args);
	void RemoveOnlineStateProcedure(__in lt::ConstStringT equipmentId, lt::XUUID ID);
	lt::User_Change_Args::Args Add_User_CommandArgs(
		__in lt::ConstStringT equipmentId,	lt::Report_User_Change_Args::Args & Args);
	lt::User_Change_Args::Args Add_User_CommandArgs(
		__in lt::ConstStringT equipmentId, lt::Request_User_Cpmmand_Args::Args & Args);
	lt::User_Change_Args::Args Add_User_CommandArgs(
		__in lt::ConstStringT equipmentId, lt::Reply_User_Change_Args::Args & Args);
	void RemoveUserCommandProcedure(__in lt::ConstStringT equipmentId, lt::XUUID ID);
	lt::Equipment_State_Args::Args Add_Equipment_StateArgs(
		__in lt::ConstStringT equipmentId, lt::Report_Equipment_State_Args::Args & Args);
	lt::Equipment_State_Args::Args Add_Equipment_StateArgs(
		__in lt::ConstStringT equipmentId, lt::Request_Equipment_State_Display_Args::Args & Args);
	lt::Equipment_State_Args::Args Add_Equipment_StateArgs(
		__in lt::ConstStringT equipmentId, lt::Reply_Equipment_State_Display_Args::Args & Args);
	void RemoveEquipmentStateProcedure(__in lt::ConstStringT equipmentId, lt::XUUID ID);
	lt::Alarm_State_Args::Args Add_Alarm_StateArgs(
		__in lt::ConstStringT equipmentId, lt::Report_Alarm_State_Args::Args & Args);
	void RemoveAlarmStateProcedure(__in lt::ConstStringT equipmentId, lt::XUUID ID);
	lt::Alarm_List_Args::Args Add_Alarm_ListArgs(
		__in lt::ConstStringT equipmentId, lt::Request_Alarm_List_Args::Args & Args);
	lt::Alarm_List_Args::Args Add_Alarm_ListArgs(
		__in lt::ConstStringT equipmentId, lt::Reply_Alarm_List_Args::Args & Args);
	void RemoveAlarmListProcedure(__in lt::ConstStringT equipmentId, lt::XUUID ID);
	lt::Set_DateTime_Args::Args Add_Set_DateTimeArgs(
		__in lt::ConstStringT equipmentId, lt::Request_Set_DateTime_Args::Args & Args);
	lt::Set_DateTime_Args::Args Add_Set_DateTimeArgs(
		__in lt::ConstStringT equipmentId, lt::Reply_Set_DateTime_Args::Args & Args);
	void RemoveSetDateTimeProcedure(__in lt::ConstStringT equipmentId, lt::XUUID ID);
#endif
};

#endif // TestManager_Device_h__

