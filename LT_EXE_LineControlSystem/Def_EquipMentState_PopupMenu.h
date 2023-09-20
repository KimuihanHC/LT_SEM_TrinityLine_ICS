//*****************************************************************************
// Filename	: 	Def_PopupMenu.h
// Created	:	2022/6/14 - 16:09
// Modified	:	2022/6/14 - 16:09
//
// Author	:	piring
//	
// Purpose	:	
//*****************************************************************************
#ifndef Def_EquipMentState_PopupMenu_h__
#define Def_EquipMentState_PopupMenu_h__



#pragma once

#include <windows.h>
#include <tchar.h>
#include "Def_CompileOption.h"


//-----------------------------------------------
// 개별 설비 제어 커맨드 (UI 버튼 처리용도)
//-----------------------------------------------
typedef enum
{
	ES_IDLE,
	ES_RUN,
	ES_STOP,
	ES_SUDDENSTOP,

	ES_MaxCount,
}enEquipmentState_Bn;

static LPCTSTR g_szEquipment_State_Bt[] =
{
	_T("IDLE"),
	_T("RUN"),
	_T("STOP"),
	_T("SUDDENSTOP"),
	nullptr,
};
#endif
