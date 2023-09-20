//*****************************************************************************
// Filename	: 	Def_Test_Cm.h
// Created	:	2016/06/30
// Modified	:	2016/08/08
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
#ifndef Def_Test_Cm_h__
#define Def_Test_Cm_h__

#include "afxwin.h"


static COLORREF	clrJudgeColor[] =
{
	RGB(0xEE, 0x50, 0x50),	// NG 
	RGB(0x90, 0xEE, 0x90),	// OK
	RGB(0xFF, 0xFF, 0xFF),	// Wait	
	RGB(0xFF, 0xFF, 0x00),	// Error
	RGB(0xAA, 0xAA, 0xAA),	// No Data
	RGB(0x77, 0x77, 0x77),	// Not Used
};

typedef enum
{
	JUDGE_NG = 0,
	JUDGE_OK = 1,
	JUDGE_EnumMax,
}enJudgment;

typedef struct _tag_StaticInf
{
	LPCTSTR		szText;
	LPCTSTR		szTextInitial;
	COLORREF	TextColor;
	COLORREF	BackColor;
	COLORREF	BorderColor;
}ST_StaticInf, *PST_StaticInf;

typedef enum 
{
	TP_Ready,		// Idle
	TP_Loading,
	TP_WaitTest,
	TP_Testing,		// Run
	TP_Unloading,
	TP_CycleMode,
	TP_DummyMode,
	TP_Completed,
}enTestProcess;

//적색, 황색, 녹색, 백색, 회색, 흑색, 청색, 자색
static ST_StaticInf g_TestProcess[] =
{	// Text					Initial		Text Color			Back Color			Border Color
	{ _T("Ready"),			_T("R"),	RGB(  0, 176,  80), RGB(230, 230, 230),		RGB(  0,   0,   0)	},	// 하양 바탕 - 녹색 글씨 (???)
	{ _T("Loading"),		_T("L"),	RGB(  0,   0,   0), RGB(  0, 176,  80),		RGB(  0,   0,   0)	},	// 녹색
	{ _T("Wait Test"),		_T("O"),	RGB(  0,   0,   0), RGB(192, 192, 255),		RGB(  0,   0,   0)	},	// 보라
	{ _T("Testing"),		_T("T"),	RGB(192,   0,   0), RGB(255, 192,   0),		RGB(  0,   0,   0)	},	// 노랑 바탕 - 빨강 글씨
	{ _T("Unloading"),		_T("U"),	RGB(  0,   0,   0), RGB(192, 192, 255),		RGB(  0,   0,   0)	},	// 보라
	{ _T("Cycle Mode"),		_T("C"),	RGB(  0,   0,   0),	RGB(255, 255, 255),		RGB(255, 255, 255)	},
	{ _T("Dummy Mode"),		_T("D"),	RGB(  0,   0,   0),	RGB(255, 255, 255),		RGB(255, 255, 255)	},
	{ _T("Completed"),		_T("F"),	RGB(  0, 176,  80),	RGB(230, 230, 230),		RGB(  0,   0,   0)	},
	NULL
};

// 검사 완료
typedef enum 
{
 	TR_Fail,		// 불량
 	TR_Pass,		// 양품
	TR_Check,		// 기구, 보드, 통신 이상으로 검사가 정상적으로 이루어지지 않은 경우
	TR_Stop,		// 작업자가 검사 중지 시킨 경우
	TR_Ready,		// 제품 투입전 초기 상태
	TR_Testing,

 	TR_Empty,		// 팔레트에 카메라가 없음
 	TR_Skip,		// ByPass : 검사 진행하지 않고 바로 배출
	TR_Rework,		// EMO, 사용자 정지로 인한 재검해야 하는 경우
	TR_Timeout,		// 
}enTestResult;

#define COUNT_TEST_RESULT	TR_Timeout + 1

static ST_StaticInf g_TestResult[] =
{	// Text					Initial			Text Color			Back Color			Border Color
	{ _T("FAIL"),		_T("NG"),		RGB(  0,   0,   0),	RGB(192,   0,   0), RGB(174,  90,  33)	},	// 빨강 바탕 - 검정 글씨
	{ _T("PASS"),		_T("OK"),		RGB(  0,   0,   0),	RGB(  0, 176,  80), RGB( 80, 126,  50)	},	// 녹색 바탕 - 검정 글씨
	{ _T("CHECK"),		_T("Chk"),		RGB(  0,   0,   0),	RGB(255, 192,   0), RGB( 62,  20, 110)	},	// 노랑 바탕 - 검정 글씨
	{ _T("STOP"),		_T("Stop"),		RGB(  0,   0,   0),	RGB(192,   0,   0), RGB(174,  90,  33)	},
	{ _T("READY"),		_T("Ready"),	RGB(  0, 176,  80),	RGB(255, 255, 255), RGB(  0,   0,   0)	},	// 하양 바탕 - 녹색 글씨 (???)
	{ _T("TESTING"),	_T("Test"),		RGB(192,   0,   0), RGB(255, 192,   0), RGB(188, 140,   0)	},	// 노랑 바탕 - 빨강 글씨

	{ _T("EMPTY"),		_T("Empty"),	RGB(  0,   0,   0),	RGB(255, 255, 255), RGB(255, 255, 255)	},	// 회색
	{ _T("BYPASS"),		_T("Bypas"),	RGB(255, 255, 255), RGB(255, 255, 255),	RGB( 47,  82, 143)	},	// 검정
	{ _T("REWORK"),		_T("Rewok"),	RGB(  0,   0,   0),	RGB(237, 125,  49), RGB(  0,   0,   0)	},	// 주황
	{ _T("TIMEOUT"),	_T("Time"),		RGB(255, 255, 255), RGB(192,   0,   0), RGB(174,  90,  33)  },	// 노랑
	NULL
};

typedef enum 
{
	TER_Fail,
	TER_Pass,
	TER_Check,
	TER_Stop,
	TER_Ready,
	TER_Testing,
	TER_SKIP,
	TER_Timeout,
}enTestItemResult;

static ST_StaticInf g_TestItemResult[] =
{	// Text				Initial		Text Color			Back Color			Border Color
	{ _T("Fail"),		_T("F"),	RGB(255, 255, 255), RGB(192,   0,   0),	RGB(174,  90,  33) },
	{ _T("Pass"),		_T("P"),	RGB(255, 255, 255), RGB(  0, 122, 204),	RGB( 20,  86, 150) },
	{ _T("Check"),		_T("C"),	RGB(255, 255, 255), RGB(192,   0,   0),	RGB(174,  90,  33) },
	{ _T("Stop"),		_T("S"),	RGB(255, 255, 255), RGB(192,   0,   0),	RGB(174,  90,  33) },
	{ _T("Ready"),		_T("R"),	RGB(  0, 176,  80), RGB(255, 255, 255), RGB(  0,   0,   0) },	// 하양 바탕 - 녹색 글씨 (???)
	{ _T("Testing"),	_T("T"),	RGB(192,   0,   0), RGB(255, 192,   0), RGB(188, 140,   0) },	// 노랑 바탕 - 빨강 글씨

	{ _T("Skip"),		_T("K"),	RGB(255, 255, 255), RGB(255, 255, 255), RGB( 62,  20, 110) },
	{ _T("Timeout"),	_T("O"),	RGB(255, 255, 255), RGB(192,   0,   0),	RGB(174,  90,  33) },
	NULL
};

typedef enum 
{
	TSS_Fail,
	TSS_Pass,
	TSS_Check,
	TSS_Stop,
	TSS_Ready,
	TSS_Testing,
}enStepStatus;

static ST_StaticInf g_StepStatus[] =
{	// Text				Initial		Text Color			Back Color			Border Color
	{ _T("Fail"),		_T("F"),	RGB(255, 255, 255), RGB(192,   0,   0), RGB(174,  90,  33) },
	{ _T("Pass"),		_T("P"),	RGB(255, 255, 255), RGB(  0, 176,  80), RGB( 80, 126,  50) },
	{ _T("Check"),		_T("C"),	RGB(255, 255, 255), RGB(192,   0,   0), RGB(174,  90,  33) },
	{ _T("Stop"),		_T("S"),	RGB(255, 255, 255), RGB(255, 192,   0), RGB( 62,  20, 110) },
	{ _T(""),			_T("R"),	RGB(255, 255, 255), RGB(255, 192,   0), RGB( 62,  20, 110) },
	{ _T("Testing"),	_T("T"),	RGB(255, 255, 255), RGB(255, 192,   0), RGB( 62,  20, 110) },
	NULL
};



#endif // Def_Test_Cm_h__
