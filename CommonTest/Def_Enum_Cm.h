//*****************************************************************************
// Filename	: Def_Enum_Cm.h
// Created	: 2010/11/23
// Modified	: 2016/06/30
//
// Author	: PiRing
//	
// Purpose	: 
//*****************************************************************************
#ifndef Def_Enum_Cm_h__
#define Def_Enum_Cm_h__

#include <afxwin.h>
#include "Def_Equipment_Cm.h"

//-------------------------------------------------------------------
// UI 관련
//-------------------------------------------------------------------
// 로그 탭
typedef enum 
{
	LOGTYPE_NONE = 0,
	LOGTYPE_PROC,		
	LOGTYPE_ERROR,		
}LoggerType;

static COLORREF	clrLoggerColor[] =
{
	RGB(10, 10, 10),
	RGB(0, 0, 255),
	RGB(255, 0, 0),
};

static LPCTSTR lpszLogType[] = { _T(""), _T("GUI"), _T("COM"), _T("OPR"), NULL };

// 통신 연결 상태
typedef enum 
{
	COMM_STATUS_NOTCONNECTED = 0,
	COMM_STATUS_CONNECT,
// 	COMM_STATUS_DISCONNECT,
// 	COMM_STATUS_ERROR,
// 	COMM_STATUS_SYNC_OK,
// 	COMM_STATUS_NO_ACK,
}enCommStatusType;

static LPCTSTR lpszCommStatus[] = 
{
	_T("미 연결"),
	_T("연결"),
// 	_T("연결 해제"),
// 	_T("오류"),
// 	_T("싱크 완료"),
// 	_T("No ACK"),
	NULL
};

typedef enum 
{
	LOT_End = 0,
	LOT_Start = 1
}enLotStatus;

typedef enum 
{
	Bit_Clear	= 0,
	Bit_Set		= 1,
	Bit_EnumMax,
}enBitStatus;

typedef enum 
{
	MES_Offline,
	MES_Online,
}enMES_Online;

static LPCTSTR g_szMES_Online[] =
{
	_T("Offline"),
	_T("Online"),
	NULL
};

//---------------------------------------------------------
// Access 모드 설정값
typedef enum 
{
	Permission_Operator,		// 옵션 보기/수정 불가
	Permission_Engineer,		// 옵션 보기 가능, 수정 불가
	Permission_Administrator,	// 옵션 보기/수정 가능
	Permission_MaxCount,
}enPermissionMode;

static LPCTSTR g_szPermissionMode[] =
{
	_T("Operator"),
	_T("Engineer"),
	_T("Administrator"),
	NULL
};

static COLORREF g_clrPermissionMode[] =
{
	RGB(  0, 255, 0),
	RGB(190, 190, 0),
	RGB(255,   0, 0),
};

//---------------------------------------------------------

typedef enum 
{
	Barcode_UnknownErr,
	Barcode_Pass,	// 사용 가능
	Barcode_Overap,	// 중복
	Barcode_Length,	// 글자수 오류
	Barcode_Count,	// 입력 개수 오류
}enBarcodeChk;

static LPCTSTR g_szBarcodeChk[] = 
{
	_T("Unknown Error"),
	_T("OK"),
	_T("Barcode Overlap"),
	_T("Barcode Length Err"),
	_T("바코드 입력 개수 오류"),
	NULL
};


//---------------------------------------------------------
// 레시피 설정 파일 확장자
#define	RECIPE_FILE_EXT			_T("luri")
// 소모품 설정 파일 확장자
#define	CONSUM_FILE_EXT			_T("ini")
// 모터 설정 파일 확장자
#define	MOTOR_FILE_EXT			_T("mot")
// 광원 설정 파일 확장자
#define	LIGHT_FILE_EXT			_T("lit")
// I2C 레지스터 세팅 파일 확장자
#define	I2C_FILE_EXT			_T("set")


//---------------------------------------------------------
typedef enum 
{
	MAX_OPERATION_THREAD			= 20,	// 최대 개별 검사용 쓰레드 개수
	MAX_MODULE_CNT					= 2,	// 최대 제품 투입 개수 (Left, Right)

}_ConstVar;

#if defined(EES_XML)//20231003
typedef enum {
	ICS_SERVER_MODULE,
	ICS_SERVER_EES,
	ICS_SERVER_MAX
}_ICS_SERVER_Type;

static LPCTSTR strLogName[] = {
	_T("ICS_SERVER"),
	_T("ICS_SERVER_EES"),
	_T("ICS_MAX"),
};
#endif


#endif // Def_Enum_Cm_h__

