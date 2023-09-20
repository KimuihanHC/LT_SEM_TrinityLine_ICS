//*****************************************************************************
// Filename	: Def_WindowMessage.h
// Created	: 2012/1/16
// Modified	: 2016/08/17
//
// Author	: PiRing
//	
// Purpose	: 윈도우 메세지 정의 (0x0400 ~ 0x7FFF)
//*****************************************************************************
#ifndef Def_WindowMessage_h__
#define Def_WindowMessage_h__

#include "Def_WindowMessage_Cm.h"

//-------------------------------------------------------------------
// 프로그램 운영
//-------------------------------------------------------------------
#define		WM_CHANGE_SITE_VIEW			WM_USER + 400

#define		WM_TEST_SELECT				WM_USER + 401	// 검사 진행 여부 선택
#define		WM_LINE_CTRLCMD				WM_USER + 402	// 설비 개별 제어 명령
#define		WM_EQP_CTRLCMD				WM_USER + 403	// 설비 전체 제어 명령
#define		WM_NGCODE_NOTIFY			WM_USER + 404

//-------------------------------------------------------------------
// 설정 정보 데이터가 변경 되었음을 알림
//-------------------------------------------------------------------
#define		WM_CHANGED_SETTING_INFO		WM_USER + 600	// 설정 정보 데이터가 변경 되었음을 알림
typedef enum
{
	WM_Setting_Line,		// 
	WM_Setting_Socket,		// 
	WM_Setting_User,		// 
	WM_Setting_Alarm,		// 
	WM_Setting_NGCode,		// 
	WM_Setting_Debug,
}WM_Setting_Para;

//-------------------------------------------------------------------
// 설비에서 통신 수신됨
//-------------------------------------------------------------------
#define		WM_EQUIPMENT_NOTIFY			WM_USER + 601
typedef enum
{
	WM_EqpNotify_ClientConnection,		// 설비 -> 서버로 통신 접속됨 
	WM_EqpNotify_VerifyEqpConnection,	// 설비의 ip address와 eqp id 설정 확인
	WM_EqpNotify_PortClear,				// 설비의 port 초기화
	WM_EqpNotify_PortStatus,			// 설비의 port 상태 통지
	WM_EqpNotify_ConveyorStatus,		// 설비의 컨베이어 상태 통지
	WM_EqpNotify_OperatingMode,			// 설비의 운영 모드 통지
	WM_EqpNotify_ProcessStatus,			// 설비 상태 통지
	WM_EqpNotify_SocketRequest,			// 소켓 승인 요청

	WM_EqpNotify_Skip,					// 
	WM_EqpNotify_TimeSync,				// 

	WM_EqpNotify_Yield,					// 
	WM_EqpNotify_Production,
}WM_Eqp_Nofity_Para;

#define		WM_EVENT_LOADER_RESISTER_SOCKET		WM_USER + 602
#define		WM_EVENT_LOADER_CHEKCK_TRACKOUT		WM_USER + 603
#define		WM_EVENT_TESTER_TRACKIN				WM_USER + 604
#define		WM_EVENT_TESTER_END_INSPECTION		WM_USER + 605
#define		WM_EVENT_UNLOAD_REQ_TEST_RESULT		WM_USER + 606
#define		WM_EVENT_UNLOAD_NG_INFO				WM_USER + 607
#define		WM_EVENT_UNLOAD_UNREGISTER_SOCKET	WM_USER + 608

// 설비의 Port, Conveyor "상태"가 바뀜 (배출 대기 중인 소켓의 배출 가능 여부 재 판단)
#define		WM_EVENT_EQUIPMENT_PORT_STATUS		WM_USER + 609
#define		WM_EVENT_EQUIPMENT_CONVEYOR_STATUS	WM_USER + 610
#define		WM_EVENT_EQUIPMENT_CONNECTION		WM_USER + 611
#define		WM_EVENT_EQUIPMENT_AUTO_MODE		WM_USER + 612	// 설비에서 auto mode 설정함
#define		WM_EVENT_EQUIPMENT_ALARM			WM_USER + 613	// 설비에서 Alarm 발생
#define		WM_EVENT_EQUIPMENT_END_PRODUCTION	WM_USER + 614	// 설비에서 생산 종료 됨
#define		WM_EVENT_EQUIPMENT_EMPTY_EQP		WM_USER + 615	// 설비에서 제품 없음

//-------------------------------------------------------------------
// 소켓 JIG 데이터가 변경 되었음
//-------------------------------------------------------------------
#define		WM_UPDATE_SOCKET_DATA				WM_USER + 620
#define		WM_UPDATE_SOCKET_DATA_ALL			WM_USER + 621
typedef enum 
{
	WM_Socket_All			= 0xFFFF,
	WM_Socket_Barcode		= 0x0001,
	WM_Socket_Status		= 0x0002,
	WM_Socket_Equipment		= 0x0004,
	WM_Socket_Location		= 0x0008,
	WM_Socket_Target		= 0x0010,
	WM_Socket_Yield			= 0x0020,
	WM_Socket_TestResult	= 0x0040,
}WM_Socket_Update_Para;








//-------------------------------------------------------------------
//
//-------------------------------------------------------------------





//-------------------------------------------------------------------
//
//-------------------------------------------------------------------





#endif // Def_WindowMessage_h__
