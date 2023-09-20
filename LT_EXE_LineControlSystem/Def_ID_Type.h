//*****************************************************************************
// Filename	: 	Def_ID_Type.h
// Created	:	2021/10/21 - 15:52
// Modified	:	2021/10/21 - 15:52
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#ifndef Def_ID_Type_h__
#define Def_ID_Type_h__

#include <stdint.h>
#include <afxwin.h>
#include "Def_CompileOption.h"

#pragma once
typedef enum
{
	ALCD_0,		// 0
	ALCD_1,		// 0
	ALCD_2,		// 0
	ALCD_Max,
}enALCD;
static LPCTSTR g_szALCD[] =
{
	_T("A"),
	_T("B"),
	_T("C"),
	nullptr
};
typedef enum
{
	EES_USE_ON,	
	EES_USE_OFF,
	EES_USE_MAX,
}enEES_USE;
static LPCTSTR g_szEES_USE[] =
{
	_T("OFF"),
	_T("ON"),
	nullptr
};

static LPCTSTR g_szEES_PORTID[] =
{
	_T("TEST1"),
	_T("TEST2"),
	_T("LEFT"),
	_T("RIGHT"),
	nullptr
};
#endif // Def_ID_Type_h__
