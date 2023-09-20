//*****************************************************************************
// Filename	: 	Def_Equipment.h
// Created	:	2021/10/21 - 15:42
// Modified	:	2021/10/21 - 15:42
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#ifndef Def_Constant_h__
#define Def_Constant_h__


#pragma once

//-----------------------------------------------
// 포트 상태 (Port Status)
//-----------------------------------------------
// 0 : 비어 있음, 1 : 제품 있음, 2 : 배출 대기, 3 : 비활성, 4 : 알람
typedef enum
{
	PtS_Empty			= 0,	// 0 : 비어 있음
	PtS_Exist_Socket,			// 1 : 제품 있음	
	PtS_Wait_Out,				// 2 : 배출대기
	PtS_Disable,				// 3 : 비활성
	PtS_Alarm,					// 4 : Alarm
}enPortStatus;

static LPCTSTR g_szPortStatus[] =
{
	_T("Empty"),
	_T("Exist_Socket"),
	_T("Wait Out"),
	_T("Disable"),
	_T("Alarm"),
	NULL
};

static COLORREF g_clrPortStatus[] =
{
	RGB(255, 255, 255),			// PtS_Empty		
	RGB(  0, 255,   0),			// PtS_Exist_Socket
	RGB(  0,   0, 255),			// PtS_Wait_Out,
	RGB(100, 100, 100),			// PtS_Disable,	
	RGB(255,   0,   0),			// PtS_Alarm,		
};

//-----------------------------------------------
// 컨베이어 상태 (Conveyor Status)
//-----------------------------------------------
// 0 : 중지, 1 : 가동, 2 : 알람
typedef enum
{
	CoS_Stop			= 0,	// 0 : 중지
	CoS_Run				= 1,	// 1 : 가동
	CoS_Alarm			= 2,	// 2 : 알람
}enConveyorStatus;

static LPCTSTR g_szConveyorStatus[] =
{
	_T("Stop"),
	_T("Run"),
	_T("Alarm"),
	NULL
};

static COLORREF g_clrConveyorStatus[] =
{
	RGB(255, 255,   0),			// CoS_Stop
	RGB(  0, 255,   0),			// CoS_Run	
	RGB(255,   0,   0),			// CoS_Alarm
};

typedef enum
{
	CoSE_Empty	= 0,	// 0 : 비어 있음
	CoSE_Exist	= 1,	// 1 : 제품 있음
}enConveyorStatus_Exist;

static LPCTSTR g_szConveyorStatus_Exist[] =
{
	_T("Empty"),
	_T("Exist"),
	NULL
};

//-----------------------------------------------
// 포트 인덱스
//-----------------------------------------------
// Loader
typedef enum
{
	PtI_L_Load,
	PtI_L_RFID,
	PtI_L_Transfer,
	PtI_L_Buffer_1,
	PtI_L_Buffer_2,
	PtI_L_Buffer_3,
	PtI_L_Un_RFID,
	PtI_L_Un_NG,
	PtI_L_Un_OK_Wait,
	PtI_L_Un_OK,
	PtI_L_Un_Clean,
	
	PtI_L_MaxCount,
	PtI_L_All = 99,	// => 99로 변경
}enPortIndex_Loader;

static LPCTSTR g_szPort_Loader[] =
{
	_T("Load CM-In"),
	_T("Load RFID"),
	_T("Load Transfer"),
	_T("Load Buffer 1"),
	_T("Load Buffer 2"),
	_T("Load Buffer 3"),
	_T("Unload RFID"),
	_T("Unload NG"),
	_T("Unload OK Wait"),
	_T("Unload OK"),
	_T("Unload Clean"),
	NULL
};

// Tester
typedef enum
{
	PtI_T_RFID,
	PtI_T_Buffer,
	PtI_T_Test_L,
	PtI_T_Test_R,
	PtI_T_Test_C,
	
	PtI_T_MaxCount,
	PtI_T_All	= 99,	// => 99로 변경
	//PtI_T_All	= PtI_T_MaxCount,	// => 99로 변경
}enPortIndex_Tester;

static LPCTSTR g_szPort_Tester[] =
{
	_T("RFID"),
	_T("Buffer"),
	_T("Test L"),
	_T("Test R"),
	_T("Test C"),
	NULL
};

// Returner
typedef enum
{
	PtI_R_Buffer_1,
	PtI_R_Transfer,
	PtI_R_Buffer_2,

	PtI_R_MaxCount,
	PtI_R_All = 99,	// => 99로 변경
}enPortIndex_Returner;

static LPCTSTR g_szPort_Returner[] =
{
	_T("Buffer 1"),
	_T("Transfer"),
	_T("Buffer 2"),
	NULL
};

//-----------------------------------------------
// 컨베이어 인덱스
//-----------------------------------------------
// Loader
typedef enum
{
	CvI_L_Load,
	CvI_L_RFID,
	CvI_L_Transfer,
	CvI_L_Buffer_1,
	CvI_L_Buffer_2,
	CvI_L_Buffer_3,
	CvI_L_Un_RFID,
	CvI_L_Un_NG,
	CvI_L_Un_OK,
	CvI_L_Un_Clean,

	CvI_L_MaxCount,
	CvI_L_All = 99,
}enConveyorIndex_Loader;

static LPCTSTR g_szConveyor_Loader[] =
{
	_T("Load CM-In"),
	_T("Load RFID"),
	_T("Load Transfer"),
	_T("Load Buffer 1"),
	_T("Load Buffer 2"),
	_T("Load Buffer 3"),
	_T("Unload RFID"),
	_T("Unload NG"),
	_T("Unload OK"),
	_T("Unload Clean"),
	NULL
};

// Tester
typedef enum
{
	CvI_T_RFID,
	CvI_T_Test,
	CvI_T_Return,
	
	CvI_T_MaxCount,
	CvI_T_All = 99,
}enConveyorIndex_Tester;

static LPCTSTR g_szConveyor_Tester[] =
{
	_T("RFID"),
	_T("Test"),
	_T("Return"),
	NULL
};

// Returner
typedef enum
{
	CvI_R_Buffer_1,
	CvI_R_Transfer,
	CvI_R_Buffer_2,
	
	CvI_R_MaxCount,
	CvI_R_All = 99,
}enConveyorIndex_Returner;

static LPCTSTR g_szConveyor_Returner[] =
{
	_T("Buffer 1"),
	_T("Transfer"),
	_T("Buffer 2"),
	NULL
};

//-----------------------------------------------
// 설비 제어 명령 (Equipment Control Command)
//-----------------------------------------------
typedef enum
{
	ECC_Init,			// 0 : Init
	ECC_Reset,			// 1 : Reset
	ECC_Run,			// 2 : Run
	ECC_Stop,			// 3 : Stop
	ECC_AlarmOff,		// 4 : Alarm Off

	ECC_MaxCount,
}enEqpCtrlCmd;

static LPCTSTR g_szControlCommand[] =
{
	_T("Init"),
	_T("Reset"),
	_T("Run"),
	_T("Stop"),
	_T("Alarm Off"),
	NULL
};

//-----------------------------------------------
// 설비 가동 상태 (Equipment Process Status)
//-----------------------------------------------
// 0 : 준비 (황), 1 : 시작 (녹), 2 : 정지 (적), 3 : 알람 (적, 부저)
typedef enum
{
	EPC_None,			// 초기화 이전	(적)
	EPC_Init,			// 원점 수행 중	(황, 점멸)
	EPC_Idle,			// 대기, 준비	(황)
	EPC_Run,			// 동작 중, 시작(녹)
	EPC_Alarm,			// 알람			(적, 부저)

	EPC_MaxCount,
}enEqpProcessStatus;

static LPCTSTR g_szProcessStatus[] =
{
	_T("None"),
	_T("Init"),
	_T("Idle"),
	_T("Run"),
	_T("Alarm"),
	NULL
};

static COLORREF g_clrProcessStatus[] =
{
	RGB(255, 255, 255),			// EPC_None,
	RGB(255, 255,   0),			// EPC_Init,
	RGB(255, 255, 255),			// EPC_Idle,
	RGB(  0, 255,   0),			// EPC_Run,
	RGB(255,   0,   0),			// EPC_Alarm
};

//-----------------------------------------------
// 검사 운영 모드 (Equipment Operate Mode)
//-----------------------------------------------
typedef enum
{
	EOM_Auto,			// 0 : Auto
	EOM_Manual,			// 1 : Manual
	EOM_SemiAuto,		// 2 : Semi - Auto
	EOM_Commencement,	// 3 : 시업점검
	EOM_DryRun,			// 4 : Dry Run

	EOM_MaxCount,
}enEqpOperatingMode;

static LPCTSTR g_szOperatingMode[] =
{
	_T("Auto"),
	_T("Manual"),
	_T("Semi-Auto"),
	_T("Start-up"),
	_T("DryRun"),
	NULL
};

static COLORREF g_clrOperatingMode[] =
{
	RGB(255, 255, 255),			// EOM_Auto,		
	RGB(255, 255, 255),			// EOM_Manual,		
	RGB(255, 255, 255),			// EOM_SemiAuto,	
	RGB(255, 255, 255),			// EOM_Commencement
	RGB(255, 255, 255),			// EOM_DryRun,		
};

//-----------------------------------------------
// 경광등 구분 (LED Type)
//-----------------------------------------------
typedef enum
{
	LED_Buzzer,
	LED_Red,
	LED_Yellow,
	LED_Green,
	LED_MaxCount,
}enLED_Type;

//-----------------------------------------------
// 경광등 상태 (LED Status)
//-----------------------------------------------
typedef enum 
{
	LED_Off,
	LED_On_Red,
	LED_On_Green,
	LED_On_Yellow,
	LED_On_Red_Blink,
	LED_On_Green_Blink,
	LED_On_Yellow_Blink,
	LED_On_Red_Buzzer,
}enLED_Status;

// 알람 상태 / 이력 정보

//-----------------------------------------------
// 소켓 투입 승인
//-----------------------------------------------
typedef enum 
{
	AS_Error,
	AS_Accept,
	AS_Accept_EndProduct,
	AS_Bypass,
	AS_Bypass_SameTest,
}enAcceptSocket;

static LPCTSTR g_szAcceptSocket[] =
{
	_T("Error"),
	_T("Accept Socket"),
	_T("Accept Socket (End)"),
	_T("Bypass"),
	_T("Bypass - Same Test"),
	NULL
};


//-----------------------------------------------
// 설비내 존의 사용여부 상태 (사용, 예약)
// typedef enum
// {
// 	LoS_Wait,		// 미 사용
// 	Los_Using,		// 사용 중
// 	Los_Reserved,	// 예약됨
// }enLocStatus;


//-----------------------------------------------
// 소켓 상태 (OK, NG, Error, Bypass)
//-----------------------------------------------
typedef enum 
{
	SoS_Ready,	// Loader 투입 전
	SoS_Pass,	// 검사 양품 (Loader에서 소켓 등록 후)
	SoS_Fail,	// 검사 불량 (Tester에서 검사 결과 : 불량)
	SoS_Bypass,	// 제품 없이 Bypass 인 경우
	SoS_Error,	// Error
}enSocketStatus;

static LPCTSTR g_szSocketStatus[] =
{
	_T("Ready"),	
	_T("Pass"),
	_T("Fail"),
	_T("Bypass"),
	_T("Error"),
	NULL
};

// User Permission
// 0: Administrator
// 1:
// 2: Engineer
// 3: Maintance
// 4: Operator
// 5: User

//-----------------------------------------------
// 서버 상태
//-----------------------------------------------
// typedef enum
// {
// 	SERV_NOTCONNECT = 0,
// 	SERV_STARTED,
// 	SERV_STOPED,
// 	SERV_CONNECT_DROP,
// 	SERV_CONNECT_ERROR,
// 	SERV_ADD_CONNECTION,
// 	SERV_DEL_CONNECTION,
// 	SERV_CONNECTION_FAIL,
// }enumServerState;
//-----------------------------------------------
// 클라이언트 연결 상태
//-----------------------------------------------
// typedef enum 
// {
// 	Cli_NotConnect	= 0,
// 	Cli_Connected,
// 	Cli_Disconnect,
// 	Cli_Drop,
// 	Cli_Error,
// 	Cli_Connection_Fail,
// }enClientStatus;


// 설비 사용 가능 상태
typedef enum 
{
	IAS_NoInput,	// 전체 사용 불가	
	IAS_Bypass,		// Bypass 가능
	IAS_Test,		// 검사 가능	

	IAS_MaxCount,
}enInputAvailabilityStatus;

static LPCTSTR g_szInputAvailabilityStatus[] =
{
	_T("No Input"),
	_T("Bypass"),
	_T("Testable"),
	NULL
};

// 설비 생산 상태
typedef enum
{
	PS_Producting,
	PS_Wait_End,
	PS_End_Completed,

}enProductionStatus;


#if (SET_INSPECTOR == SYS_ICS_RIVIAN_LINE)
	#define		LINE_INFO_FILENAME			_T("SEM_RLine_LineInfo.xml")
	#define		SOCKET_INFO_FILENAME		_T("SEM_RLine_SocketInfo.xml")
	#define		NGCODE_INFO_FILENAME		_T("SEM_TLine_NGCodeInfo.ini")	// 설비 제어 프로그램하고 연동
	#define		USER_INFO_FILENAME			_T("SEM_TLine_UserInfo.ini")	// 설비 제어 프로그램하고 연동
#else
	#define		LINE_INFO_FILENAME			_T("SEM_TLine_LineInfo.xml")
	#define		SOCKET_INFO_FILENAME		_T("SEM_TLine_SocketInfo.xml")
	#define		NGCODE_INFO_FILENAME		_T("SEM_TLine_NGCodeInfo.ini")
	#define		USER_INFO_FILENAME			_T("SEM_TLine_UserInfo.ini")
#endif // (SET_INSPECTOR == SYS_ICS_RIVIAN_LINE)

// 설비 프로그램 버전 정보

//#define			MAX_TEST_ZONE				2	// 테스터에서의 검사 Para 최대 갯수
//#define			MAX_CONVOYER_COUNT			3	// 테스터에서 사용하는 컨베이어 최대 갯수
#define			MAX_EQUIPMENT_COUNT			30	// UI상에 표시가능한 설비 최대 갯수

#define			MAX_RESERVE_COUNT_TESTER	3	// 테스터에서 소켓 투입 예약 가능한 최대 갯수


#endif // Def_Constant_h__
