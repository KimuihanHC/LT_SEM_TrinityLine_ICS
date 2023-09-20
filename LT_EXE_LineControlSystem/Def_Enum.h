//*****************************************************************************
// Filename	: Def_Enum.h
// Created	: 2010/11/23
// Modified	: 2016/06/30
//
// Author	: PiRing
//	
// Purpose	: 
//*****************************************************************************
#ifndef Def_Enum_h__
#define Def_Enum_h__

#include "Def_Enum_Cm.h"

//-------------------------------------------------------------------
// UI 관련
//-------------------------------------------------------------------

// 통신 Device
// typedef enum enumCommStatus_Device
// {
// 	COMM_DEV_BASE		= 100,
// 	COMM_DEV_FTDI		= 200,
// 	COMM_DEV_MCTRL		= 300,
// 	COMM_DEV_LPRNT		= 400,
// 	COMM_DEV_LAST		= COMM_DEV_LPRNT,// 마지막으로 설정할것.
// 	COMM_DEV_MAX,
// }enumCommStatus_Device;

typedef enum _ConstVar_Eqp
{
 	//USE_CHANNEL_CNT			= 4,	// 장비에서 실제 사용하는 채널 수 (소켓, 사이트 등)
	//USE_TEST_ITEM_CNT		= 1,
}_ConstVar_Eqp;

typedef enum
{
	TC_BEGIN = 0,
	TC_PAUSE,
	TC_RESUME,
	TC_END,
}enTestCondition;









#endif // Def_Enum_h__

