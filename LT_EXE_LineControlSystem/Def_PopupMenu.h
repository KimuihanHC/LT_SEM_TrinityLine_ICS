//*****************************************************************************
// Filename	: 	Def_PopupMenu.h
// Created	:	2022/6/14 - 16:09
// Modified	:	2022/6/14 - 16:09
//
// Author	:	piring
//	
// Purpose	:	
//*****************************************************************************
#ifndef Def_PopupMenu_h__
#define Def_PopupMenu_h__



#pragma once

#include <windows.h>
#include <tchar.h>
#include "Def_CompileOption.h"


//-----------------------------------------------
// 개별 설비 제어 커맨드 (UI 버튼 처리용도)
//-----------------------------------------------
typedef enum
{
#ifndef HIDE_NOT_USE_EQP_CONTROL
	CC_Init,			// 0 : Init
	CC_Reset,			// 1 : Reset
	CC_Run,				// 2 : Run (Auto)
	CC_Stop,			// 3 : Stop (Manual)
	CC_AlarmOff,		// 4 : Alarm Off
	CC_Skip,			// 설비 사용 안함
	CC_Show,			// 설비에서 Handler 화면 맨 앞으로
	CC_Hide,			// 설비에서 Handler 화면 숨김
	CC_ForcedEject,		// 설비에서 제품 강제 배출
	CC_EndOfProduction,	// 제품 생산 종료

	CC_ResetYield,		// 수율 초기화
	CC_SaveYield,		// 수율 CSV파일로 ㅈ장
	//CC_Dummy,
#else
	CC_Init,			// 0 : Init
	CC_EndOfProduction,	// 제품 생산 종료
	CC_Run,				// 2 : Run (Auto)
	CC_Stop,			// 3 : Stop (Manual)
	CC_AlarmOff,		// 4 : Alarm Off
	CC_Skip,			// 설비 사용 안함
	CC_Show,			// 설비에서 Handler 화면 맨 앞으로
	CC_Hide,			// 설비에서 Handler 화면 숨김
	
	CC_ResetYield,		// 수율 초기화
	CC_SaveYield,		// 수율 CSV파일로 ㅈ장

	CC_Reset,			// 1 : Reset
	//CC_Dummy,
#endif
	CC_MaxCount,
}enConrolCode;

static LPCTSTR g_szCtrlCode[] =
{
#ifndef HIDE_NOT_USE_EQP_CONTROL
	_T("Init"),					// CC_Init	
	_T("Reset"),				// CC_Reset
	_T("Run"),					// CC_Run		
	_T("Stop"),					// CC_Stop	
	_T("Buzzer Off"),			// CC_AlarmOff
	_T("Use/Skip"),				// CC_Skip
	_T("Show"),					// CC_Show
	_T("Hide"),					// CC_Hide
	_T("Forced Eject"),			// CC_ForcedEject
	_T("End Of Production"),	// CC_EndOfProduction

	_T("Reset Yield"),			// CC_ResetYield
	_T("Save Yield"),			// CC_SaveYield
	//_T(""),					// CC_Dummy
#else
	_T("Init"),					// CC_Init
	_T("End Of Production"),	// CC_EndOfProduction
	_T("Run"),					// CC_Run		
	_T("Stop"),					// CC_Stop	
	_T("Buzzer Off"),			// CC_AlarmOff
	_T("Use/Skip"),				// CC_Skip
	_T("Show"),					// CC_Show
	_T("Hide"),					// CC_Hide

	_T("Reset Yield"),			// CC_ResetYield
	_T("Save Yield"),			// CC_SaveYield

	_T("Reset"),				// CC_Reset
	//_T(""),					// CC_Dummy
#endif

};

static LPCTSTR g_szCtrlCode_Bn[] =
{
#ifndef HIDE_NOT_USE_EQP_CONTROL
	_T("Init"),					// CC_Init	
	_T("Reset"),				// CC_Reset
	_T("Run\n(Auto)"),			// CC_Run	
	_T("Stop\n(Manual)"),		// CC_Stop	
	_T("Buzzer\nOff"),			// CC_AlarmOff
	_T("Use\n/ Skip"),			// CC_Skip
	_T("Show"),					// CC_Show
	_T("Hide"),					// CC_Hide
	_T("Forced\nEject"),		// CC_ForcedEject
	_T("End of\nProduction"),	// CC_EndOfProduction

	_T("Reset\nYield"),			// CC_ResetYield
	_T("Save\nYield"),			// CC_SaveYield
	_T(""),						// CC_Dummy
#else
	_T("Init"),					// CC_Init	
	_T("End of\nProduction"),	// CC_EndOfProduction
	_T("Run\n(Auto)"),			// CC_Run
	_T("Stop\n(Manual)"),		// CC_Stop	
	_T("Buzzer\nOff"),			// CC_AlarmOff
	_T("Use\n/ Skip"),			// CC_Skip
	_T("Show"),					// CC_Show
	_T("Hide"),					// CC_Hide

	_T("Reset\nYield"),			// CC_ResetYield
	_T("Save\nYield"),			// CC_SaveYield

	_T("Reset"),				// CC_Reset
	//_T(""),					// CC_Dummy
#endif

};


#endif // Def_PopupMenu_h__
