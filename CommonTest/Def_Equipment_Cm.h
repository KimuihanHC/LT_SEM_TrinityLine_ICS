//*****************************************************************************
// Filename	: 	Def_Equipment_Cm.h
// Created	:	2021/2/8 - 11:03
// Modified	:	2021/2/8 - 11:03
//
// Author	:	piring
//	
// Purpose	:	
//*****************************************************************************
#ifndef Def_Equipment_Cm_h__
#define Def_Equipment_Cm_h__

#pragma once

#include <afxwin.h>

//-----------------------------------------------------------------------------
// 검사기 구분용도
//-----------------------------------------------------------------------------
#define SYS_CUSTOMER			_T("SEM")

#define SYS_ICS_TRINITY_LINE	0
#define SYS_ICS_RIVIAN_LINE		1
#define SYS_DEV_MOD_3			2	
#define SYS_DEV_MOD_4			3	

typedef enum 
{
	Sys_Ics_Trinity_Line	= SYS_ICS_TRINITY_LINE,	
	Sys_Ics_Rivian_Line		= SYS_ICS_RIVIAN_LINE,			
	Sys_Dev_Mod_3			= SYS_DEV_MOD_3,
	Sys_Dev_Mod_4			= SYS_DEV_MOD_4
}enInsptrSysType;

static LPCTSTR g_szInsptrSysType[] =
{
	_T("In-Line Control System"),
	_T("In-Line Control System"),	
	_T(""),
	_T(""),
	nullptr
};

//-----------------------------------------------------------------------------
// 검사기 구분 테이블 (광원, 전원, 바코드, CAN, MES, PLC)
//-----------------------------------------------------------------------------
typedef struct _tag_InspectorTable
{
	LONG	SysType;		// 검사기 구분 코드
	bool	UseEES;			// EES 사용
	int		Resered_01;		// 
	int		Resered_02;		// 
}ST_InspectorTable;

static ST_InspectorTable g_InspectorTable[] =
{	
	//검사기					EES,		예약1,	예약2	
	{ Sys_Ics_Trinity_Line,	true,		0,		0,	},
	{ Sys_Ics_Rivian_Line,	false,		0,		0,	},
	{ Sys_Dev_Mod_3,		false,		0,		0,	},
	{ Sys_Dev_Mod_4,		false,		0,		0,	},
	NULL
};




#endif // Def_Equipment_Cm_h__
