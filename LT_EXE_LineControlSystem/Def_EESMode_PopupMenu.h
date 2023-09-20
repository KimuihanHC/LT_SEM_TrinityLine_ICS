//*****************************************************************************
// Filename	: 	Def_PopupMenu.h
// Created	:	2022/6/14 - 16:09
// Modified	:	2022/6/14 - 16:09
//
// Author	:	piring
//	
// Purpose	:	
//*****************************************************************************
#ifndef Def_EESMode_PopupMenu_h__
#define Def_EESMode_PopupMenu_h__



#pragma once

#include <windows.h>
#include <tchar.h>
#include "Def_CompileOption.h"



//-----------------------------------------------
// 개별 설비 제어 커맨드 (UI 버튼 처리용도)
//-----------------------------------------------
typedef enum
{
	EM_OFFLINE,
	EM_TKIN,
	EM_RMS,

	EM_MaxCount,
}enEES_Mode_Bn;

static LPCTSTR g_szEES_Mode[] =
{
	_T("OFFLINE"),
	_T("TKIN"),
	_T("RMS"),
	nullptr,
};

static LPCTSTR g_szEES_Mode_Bn[] =
{
	_T("OFFLINE"),
	_T("TKIN"),
	_T("RMS"),
	nullptr,
};
#endif
