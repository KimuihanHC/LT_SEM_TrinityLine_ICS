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
#define SYS_CUSTOMER			_T("Luritech")

#define SYS_DEV_MOD_1	0
#define SYS_DEV_MOD_2	1
#define SYS_DEV_MOD_3	2	
#define SYS_DEV_MOD_4	3	

typedef enum 
{
	Sys_Dev_Mod_1	= SYS_DEV_MOD_1,	
	Sys_Dev_Mod_2	= SYS_DEV_MOD_2,			
	Sys_Dev_Mod_3	= SYS_DEV_MOD_3,
	Sys_Dev_Mod_4	= SYS_DEV_MOD_4
}enInsptrSysType;

static LPCTSTR g_szInsptrSysType[] =
{
	_T("In-Line Control System"),
	_T(""),	
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
	bool	UseDIO;			// Digital I/O 사용 구분
	int		DAQBrd_Cnt;		// 그래버 보드 사용 개수
	int		CtrlBrd_Cnt;	// Cam Main 보드 사용 개수
}ST_InspectorTable;

static ST_InspectorTable g_InspectorTable[] =
{	
	//검사기				IO,		영상,	전원	
	{ Sys_Dev_Mod_1,	false,	1,		1,	},
	{ Sys_Dev_Mod_2,	false,	1,		1,	},
	{ Sys_Dev_Mod_3,	false,	1,		1,	},
	{ Sys_Dev_Mod_4,	false,	1,		1,	},
	NULL
};




#endif // Def_Equipment_Cm_h__
