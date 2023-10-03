//*****************************************************************************
// Filename	: 	Def_NGCode.h
// Created	:	2021/12/6 - 13:34
// Modified	:	2021/12/6 - 13:34
//
// Author	:	piring
//	
// Purpose	:	
//*****************************************************************************
#ifndef Def_NGCode_h__
#define Def_NGCode_h__


#pragma once

#include <afxwin.h>
#include <stdint.h>
#include <map>


#define		NG_CODE_MINI_VERSION	// 전체 코드 중에 일부만 화면에 표시

//-----------------------------------------------------------------------------
// Site, Prd Name, 
// NG Code, Section(NG구분), Retry Mode, Retry count, Move Retry, Reset, 연속 NG 알람 수,  Description
// Module, Location, Section1, Section2, Status, Serial, Update Date, Description
//-----------------------------------------------------------------------------

//-------------------------------------
// Section
//-------------------------------------
typedef enum
{
	NGSec_00_OK,			// 00 : OK
	NGSec_01_ChartAlign,	// 01 : Chart Align
	NGSec_02_Contact,		// 02 : Contact
	NGSec_03_Illuminate,	// 03 : Illuminate
	NGSec_04_Board,			// 04 : Board
	NGSec_05_InspSW,		// 05 : Insp S / W
	NGSec_06_Resolution,	// 06 : Resolution
	NGSec_07_Logic,			// 07 : Logic
	NGSec_08_Normal,		// 08 : Normal
	NGSec_09_SINK,			// 09 : SINK
	NGSec_10_Reserved,		// 10 : -
	NGSec_11_Reserved,		// 11 : -
	NGSec_12_SocketContact,	// 12 : Socket Contact
}enNG_Section;

//-------------------------------------
// Retry Mode
//-------------------------------------
typedef enum
{
	NGRe_0_Contact,		// 0 : Contact
	NGRe_1_ReAttach,	// 1 : Re - Attach
	NGRe_2_ReClamping,	// 2 : Re - Clamping
}enNG_RetryMode;

//-------------------------------------
// Reset, Move Retry
//-------------------------------------
typedef enum
{
	NGUse_0_NotUse,		// 0 : Not Use
	NGUse_1_Use,		// 1 : Use
}enNG_Usable;

typedef struct  _tag_NGCode
{
	int16_t		m_nNG_Code;			// Code (NGCode	)
	uint16_t	m_nNG_Section;		// Section (NG구분)	
	uint16_t	m_nRetry_Mode;		// Retry Mode	
	uint16_t	m_nRetryCnt;		// Retry Count	
	uint16_t	m_nMove_Retry;		// Move Retry	=> (0: 미사용, 1: 사용)
	uint16_t	m_nReset;			// Reset		=> (0: 미사용, 1: 사용)
	uint16_t	m_nAlarmCnt;		// Alarm Count(연속NG알람수)
	CString		m_szDescription;	// Descript

	_tag_NGCode()
	{
		Reset();
	};

	_tag_NGCode& operator= (const _tag_NGCode& ref)
	{
		m_nNG_Code			= ref.m_nNG_Code;
		m_nNG_Section		= ref.m_nNG_Section;
		m_nRetry_Mode		= ref.m_nRetry_Mode;
		m_nRetryCnt			= ref.m_nRetryCnt;
		m_nMove_Retry		= ref.m_nMove_Retry;
		m_nReset			= ref.m_nReset;
		m_nAlarmCnt			= ref.m_nAlarmCnt;
		m_szDescription		= ref. m_szDescription;

		return *this;
	};

	void Reset()
	{
		m_nNG_Code			= 0;
		m_nNG_Section		= 0;
		m_nRetry_Mode		= 0;
		m_nRetryCnt			= 0;
		m_nMove_Retry		= 0;
		m_nReset			= 1;
		m_nAlarmCnt			= 0;
		m_szDescription.Empty();
	};
}ST_NGCode;

#endif // Def_NGCode_h__
