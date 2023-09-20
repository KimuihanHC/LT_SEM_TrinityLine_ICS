//*****************************************************************************
// Filename	: 	Def_Server_Type.h
// Created	:	2021/10/21 - 15:52
// Modified	:	2021/10/21 - 15:52
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#ifndef Def_Server_Type_h__
#define Def_Server_Type_h__

#include <stdint.h>
#include <afxwin.h>
#include "Def_CompileOption.h"

#pragma once
typedef enum
{
	SERVER_UI_EES,
	SERVER_UI_MAX
}enServerUI;

static LPCTSTR g_szServerUIName[] =
{
	_T("ICS_EES"),
	nullptr
};
typedef enum
{
	SERVER_EES,
	SERVER_MAX
}enServerType;

static LPCTSTR g_szServerTypeName[] =
{
	_T("ICS_EES"),
	nullptr
};
#endif // Def_Equipment_Type_h__
