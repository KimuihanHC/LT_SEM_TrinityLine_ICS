//*****************************************************************************
// Filename	: 	Def_ErrorCode.h
// Created	:	2016/10/31 - 0:48
// Modified	:	2016/10/31 - 0:48
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
#ifndef Def_ErrorCode_h__
#define Def_ErrorCode_h__

#include <afxwin.h>

typedef enum enErrorCode
{
	Err_Fail	= 0,
	Err_Pass,
	Err_EMO,				
	Err_AreaSensor,			
	Err_FrontDoorSensor,	
	Err_LeftDoorSensor,		
	Err_RightDoorSensor,	
	Err_RearDoorSensor,		
	Err_ElericitySensor,	
	Err_MotorIndexSensor,	
	Err_EAS,
	Err_DiconnectedFromServer,
	Err_Motor,	
	Err_Comm,	
	Err_Device,
	Err_MainPower,
	Err_MainPress,
}enErrorCode;

static LPCTSTR	g_szErrorCode_H[] =
{
	_T("Fail"),
	_T("Pass"),
	_T("E M O"),
	_T("Error! Detect Area Sensor!"),
	_T("FRONT DOOR SENSOR"),
	_T("LEFT DOOR SENSOR"),
	_T("RIGHT DOOR SENSOR"),
	_T("READ DOOR SENSOR"),
	_T("ELECRICITY DOOR SENSOR"),
	_T("IDEX MOTOR SENSER CHECK"),
	_T("연속 불량 알람"),
	_T("서버와의 통신이 끊김"),
	_T("Error Motor"),
	_T("Error Comm"),
	_T("Error Device"),
	_T("Error! MainPower OFF PGM Reset"),
	_T("Error! MainPress Check PGM Reset"),
	NULL
};

// 오류 발생 시 조치법
static LPCTSTR g_szErrorCode_H_Desc[] = 
{
	_T("Fail"),
	_T("Pass"),
	_T("EMO 발생! 전원 재 인가후 원점 조정을 하세요.\r\n 현재 검사 중인 제품은 재 검사하세요."),
	_T("Detect Area Sensor"),
	_T("Detect Front Door Sensor"),
	_T("Detect Left Door Sensor"),
	_T("Detect Right Door Sensor"),
	_T("Detect Rear Door Sensor"),
	_T("Detect Elericity Sensor"),
	_T("Detect Motor Index Sensor"),
	_T("연속 불량 알람"),
	_T("서버와의 통신이 끊김"),

	_T("Motor Error"),
	_T("통신 오류"),
	_T("주변 장치 오류"),
	
	NULL
};


#endif // Def_ErrorCode_h__
