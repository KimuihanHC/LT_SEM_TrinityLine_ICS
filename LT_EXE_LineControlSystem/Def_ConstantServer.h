//*****************************************************************************
// Filename	: 	Def_ConstantServer.h
// Created	:	
// Modified	:	
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#ifndef Def_ConstantServer_h__
#define Def_ConstantServer_h__
#pragma once

//-----------------------------------------------
//Alarm Set 
//-----------------------------------------------
typedef enum
{
	ALARMSET_NULL,
	ALARMSET_SET,
	ALARMSET_RESET,
	ALARMSET_MAX,
}enALARMSET;
static LPCTSTR g_sALARMSET[] =
{
	_T("NULL"),
	_T("1"),
	_T("2"),
	NULL
};
//-----------------------------------------------
//Popup 
//-----------------------------------------------
typedef enum
{
	LOSS_WINDOW_ADD,
	LOSS_WINDOW_EDIT,
	LOSS_WINDOW_DELETE,
	LOSS_WINDOW_MAX,
}enLOSS_WINDOW_Mode;
static LPCTSTR g_sLOSS_WINDOW_Mode[] =
{
	_T("ADD"),
	_T("EDIT"),
	_T("DELETE"),
	NULL
};

typedef enum
{
	EES_OFFLINE,
	EES_TRACKIN,
	EES_RMS,
	EES_MAX,
}enEES_Mode;
static LPCTSTR g_sEES_Mode[] =
{
	_T("1"),
	_T("2"),
	_T("3"),
	nullptr,
};
static LPCTSTR g_sEES_Mode_UI[] =
{
	_T("OFFLINE"),
	_T("TRACKIN"),
	_T("RMS"),
	nullptr,
};

typedef enum
{
	ES_IDLE,
	ES_RUN,
	ES_STOP,
	ES_SUDDENSTOP,
	ES_MaxCount,
}enEquipmentState;
static LPCTSTR g_szEquipment_State[] =
{
	_T("IDLE"),
	_T("RUN"),
	_T("STOP"),
	_T("SUDDENSTOP"),
	nullptr,
};
//Online State
typedef enum
{
	ONLINESTATE_OFFLINE,
	ONLINESTATE_ONLINE,
	ONLINESTATE_MaxCount,
}enOnlineState;
static LPCTSTR g_szOnLine_State[] =
{
	_T("1"),
	_T("2"),
	nullptr,
};
static LPCTSTR g_szOnLine_StateUI[] =
{
	_T("OFFLINE"),
	_T("ONLINE"),
	nullptr,
};


static LPCTSTR g_szSocket_StateUI[] =
{
	_T("OPEN"),
	_T("CLOSE"),
	nullptr,
};
typedef enum
{
	SOCKETSTATE_OPEN,
	SOCKETSTATE_CLOSE,
	SOCKETSTATE_MaxCount,
}enSocketState;
typedef enum
{
	PtI_EES_0,

	PtI_EES_MaxCount,
	PtI_EES_All = 99,	// => 99로 변경
}enPortIndex_EES;

static LPCTSTR g_szPort_EES[] =
{
	_T("Port"),
	NULL
};

class CRegEquipment;

#define     EES_INFO_FILENAME			_T("SEM_EES_INFO.xml")
#define     SERVER_INFO_FILENAME		_T("SEM_SERVER_INFO.xml")
#define     AL_INFO_FILENAME			_T("SEM_AL_INFO.xml")
#define     LOSS_INFO_FILENAME			_T("SEM_LOSS_INFO.xml")
//=============================================================================
#define			MAX_EQUIPMENT_COUNT			30	// UI상에 표시가능한 설비 최대 갯수

#define			MAX_RESERVE_COUNT_TESTER	3	// 테스터에서 소켓 투입 예약 가능한 최대 갯수

#endif // Def_ConstantServer_h__
