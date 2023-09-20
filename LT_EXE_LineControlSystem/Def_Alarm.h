//*****************************************************************************
// Filename	: 	Def_Alarm.h
// Created	:	2022/1/17 - 13:39
// Modified	:	2022/1/17 - 13:39
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#ifndef Def_Alarm_h__
#define Def_Alarm_h__


#pragma once

#include <afxwin.h>
#include <stdint.h>

// 설비
// 시간

// 알람 코드
// 에러 내용

typedef struct  _tag_Alarm /*:: public ST_AlarmStatus*/
{
	CString		szEquipmentID;
	SYSTEMTIME	Time;
	uint32_t	nAlarmCode;
	CString		szAlarmDesc;
}ST_Alarm;




#endif // Def_Alarm_h__
