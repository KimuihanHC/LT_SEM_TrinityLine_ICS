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
#if defined(EES_XML)//20231003
	WM_Setting_EES,
	WM_Setting_ALARMID,
	WM_Setting_Sever,
	WM_Setting_Loss,
#endif
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
#if defined(EES_XML)//20231003
	WM_EqpNotify_EQUIPMENTSTATE,
	WM_EqpNotify_EQUIPMENTSTATEDISPLAY,
	WM_EqpNotify_RGBDISPLAY,
	WM_EqpNotify_LOTID,
#endif	
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

//-------------------------------------------------------------------
// 서버에서 통신 수신됨
//-------------------------------------------------------------------
#define		WM_SERVER_NOTIFY					WM_USER + 623

typedef enum
{
	WM_SvrNotify_ClientConnection,		// 설비 -> 서버로 통신 접속됨 

	WM_SvrNotify_VerifyEqpConnection,
	WM_SvrNotify_PortClear,				// 설비의 port 초기화
	WM_SvrNotify_PortStatus,			// 설비의 port 상태 통지
	WM_SvrNotify_ConveyorStatus,		// 설비의 컨베이어 상태 통지
	WM_SvrNotify_OperatingMode,			// 설비의 운영 모드 통지
	WM_SvrNotify_ProcessStatus,			// 설비 상태 통지
	WM_SvrNotify_SocketRequest,			// 소켓 승인 요청
	WM_SvrNotify_Skip,					// 
	WM_SvrNotify_TimeSync,				// 
	WM_SvrNotify_Yield,					// 
	WM_SvrNotify_Production,
#if defined(EES_XML)//20231003
	WM_SvrNotify_EQUIPMENTSTATE,
	WM_SvrNotify_EQUIPMENTSTATEDISPLAY,
	WM_SvrNotify_RGBDISPLAY,
	WM_SvrNotify_LOSSWINDOW,
	WM_SvrNotify_LOSSCODE,
	WM_SvrNotify_REQUEST_TERMINAL_MESSAGE,
	WM_SvrNotify_TERMINAL_MESSAGE,
	WM_SvrNotify_OPCALL,
	WM_SvrNotify_RMS,
#endif
}WM_Svr_Nofity_Para;
#if defined(EES_XML)//20231003
//=============================================================================
// WM_EVENT_SERVER(TC -> ICS)
//=============================================================================
#define		WM_EVENT_SERVER_CONNECTION						WM_USER + 624
#define		WM_EVENT_SERVER_REQUEST_LINK_TEST				WM_USER + 625
#define		WM_EVENT_SERVER_REPLY_LINK_TEST					WM_USER + 626
#define		WM_EVENT_SERVER_REPORT_ONLINE_STATE				WM_USER + 627
#define		WM_EVENT_SERVER_REPORT_USER_CHANGE				WM_USER + 628
#define		WM_EVENT_SERVER_REQUEST_USER_COMMAND			WM_USER + 629
#define		WM_EVENT_SERVER_REPLY_USER_COMMAND				WM_USER + 630
#define		WM_EVENT_SERVER_REPORT_EQUIPMENT_STATE			WM_USER + 631
#define		WM_EVENT_SERVER_REQUEST_EQUIPMENT_STATE_DISPLAY	WM_USER + 632
#define		WM_EVENT_SERVER_REPLY_EQUIPMENT_STATE_DISPLAY	WM_USER + 633
#define		WM_EVENT_SERVER_REPORT_LOSS_STATE				WM_USER + 634
#define		WM_EVENT_SERVER_REQUEST_LOSS_WINDOW				WM_USER + 635
#define		WM_EVENT_SERVER_REPLY_LOSS_WINDOW				WM_USER + 636
#define		WM_EVENT_SERVER_REPORT_ALARM_STATE				WM_USER + 637
#define		WM_EVENT_SERVER_REQUEST_ALARM_LIST				WM_USER + 638
#define		WM_EVENT_SERVER_REPLY_ALARM_LIST				WM_USER + 639
#define		WM_EVENT_SERVER_REPORT_RMS_MODE					WM_USER + 640
#define		WM_EVENT_SERVER_REQUEST_SET_DATETIME			WM_USER + 641
#define		WM_EVENT_SERVER_REPLY_SET_DATETIME				WM_USER + 642
#define		WM_EVENT_SERVER_REQUEST_TERMINAL_MESSAGE		WM_USER + 643
#define		WM_EVENT_SERVER_REPLY_TERMINAL_MESSAGE			WM_USER + 644
#define		WM_EVENT_SERVER_REQUEST_OPCALL_MESSAGE			WM_USER + 645
#define		WM_EVENT_SERVER_REPLY_OPCALL_MESSAGE			WM_USER + 646
#define		WM_EVENT_SERVER_UNITID_READ						WM_USER + 647
#define		WM_EVENT_SERVER_REQUEST_UNITID_READ				WM_USER + 648
#define		WM_EVENT_SERVER_REPLY_UNITID_READ				WM_USER + 649
#define		WM_EVENT_SERVER_REPORT_START_LOT				WM_USER + 650
#define		WM_EVENT_SERVER_REPORT_START_PROCESS			WM_USER + 651
#define		WM_EVENT_SERVER_REPORT_END_PROCESS				WM_USER + 652
#define		WM_EVENT_SERVER_REPORT_END_EVENT				WM_USER + 653
//=============================================================================
// WM_EVENT_EQUIPMENT(Eqp -> ICS)
//=============================================================================
#define		WM_EVENT_EQUIPMENT_REQUEST_LINK_TEST				WM_USER + 654
#define		WM_EVENT_EQUIPMENT_REPLY_LINK_TEST					WM_USER + 655
#define		WM_EVENT_EQUIPMENT_REPORT_ONLINE_STATE				WM_USER + 656
#define		WM_EVENT_EQUIPMENT_REPORT_USER_CHANGE				WM_USER + 657
#define		WM_EVENT_EQUIPMENT_REQUEST_USER_COMMAND				WM_USER + 658
#define		WM_EVENT_EQUIPMENT_REPLY_USER_COMMAND				WM_USER + 659
#define		WM_EVENT_EQUIPMENT_REPORT_EQUIPMENT_STATE			WM_USER + 660
#define		WM_EVENT_EQUIPMENT_REQUEST_EQUIPMENT_STATE_DISPLAY	WM_USER + 661
#define		WM_EVENT_EQUIPMENT_REPLY_EQUIPMENT_STATE_DISPLAY	WM_USER + 662
#define		WM_EVENT_EQUIPMENT_REPORT_LOSS_STATE				WM_USER + 663
#define		WM_EVENT_EQUIPMENT_REQUEST_LOSS_WINDOW				WM_USER + 664
#define		WM_EVENT_EQUIPMENT_REPLY_LOSS_WINDOW				WM_USER + 665
#define		WM_EVENT_EQUIPMENT_REPORT_ALARM_STATE				WM_USER + 666
#define		WM_EVENT_EQUIPMENT_REQUEST_ALARM_LIST				WM_USER + 667
#define		WM_EVENT_EQUIPMENT_REPLY_ALARM_LIST					WM_USER + 668
#define		WM_EVENT_EQUIPMENT_REPORT_RMS_MODE					WM_USER + 669
#define		WM_EVENT_EQUIPMENT_REQUEST_SET_DATETIME				WM_USER + 670
#define		WM_EVENT_EQUIPMENT_REPLY_SET_DATETIME				WM_USER + 671
#define		WM_EVENT_EQUIPMENT_REQUEST_TERMINAL_MESSAGE			WM_USER + 672
#define		WM_EVENT_EQUIPMENT_REPLY_TERMINAL_MESSAGE			WM_USER + 673
#define		WM_EVENT_EQUIPMENT_REQUEST_OPCALL_MESSAGE			WM_USER + 674
#define		WM_EVENT_EQUIPMENT_REPLY_OPCALL_MESSAGE				WM_USER + 675
#define		WM_EVENT_EQUIPMENT_UNITID_READ						WM_USER + 676
#define		WM_EVENT_EQUIPMENT_REQUEST_UNITID_READ				WM_USER + 677
#define		WM_EVENT_EQUIPMENT_REPLY_UNITID_READ				WM_USER + 678
#define		WM_EVENT_EQUIPMENT_REPORT_START_LOT					WM_USER + 679
#define		WM_EVENT_EQUIPMENT_REPORT_START_PROCESS				WM_USER + 680
#define		WM_EVENT_EQUIPMENT_REPORT_END_PROCESS				WM_USER + 681
#define		WM_EVENT_EQUIPMENT_REPORT_END_EVENT					WM_USER + 682
//=============================================================================
// WM_EVENT_USER WM_EVENT_EQUIPMENT(User -> ICS)
//=============================================================================
#define		WM_EVENT_REQUEST_LINK_TEST							WM_USER + 683
#define		WM_EVENT_REPLY_LINK_TEST							WM_USER + 684
#define		WM_EVENT_REPORT_ONLINE_STATE						WM_USER + 685
#define		WM_EVENT_REPORT_USER_CHANGE							WM_USER + 686
#define		WM_EVENT_REQUEST_USER_COMMAND						WM_USER + 687
#define		WM_EVENT_REPLY_USER_COMMAND							WM_USER + 688
#define		WM_EVENT_REPORT_EQUIPMENT_STATE						WM_USER + 689
#define		WM_EVENT_REQUEST_EQUIPMENT_STATE_DISPLAY			WM_USER + 690
#define		WM_EVENT_REPLY_EQUIPMENT_STATE_DISPLAY				WM_USER + 691
#define		WM_EVENT_REPORT_LOSS_STATE							WM_USER + 692
#define		WM_EVENT_REQUEST_LOSS_WINDOW						WM_USER + 693
#define		WM_EVENT_REPLY_LOSS_WINDOW							WM_USER + 694
#define		WM_EVENT_REPORT_ALARM_STATE							WM_USER + 695
#define		WM_EVENT_REQUEST_ALARM_LIST							WM_USER + 696
#define		WM_EVENT_REPLY_ALARM_LIST							WM_USER + 697
#define		WM_EVENT_REPORT_RMS_MODE							WM_USER + 698
#define		WM_EVENT_REQUEST_SET_DATETIME						WM_USER + 699
#define		WM_EVENT_REPLY_SET_DATETIME							WM_USER + 700
#define		WM_EVENT_REQUEST_TERMINAL_MESSAGE					WM_USER + 701
#define		WM_EVENT_REPLY_TERMINAL_MESSAGE						WM_USER + 702
#define		WM_EVENT_REQUEST_OPCALL_MESSAGE						WM_USER + 703
#define		WM_EVENT_REPLY_OPCALL_MESSAGE						WM_USER + 704
#define		WM_EVENT_UNITID_READ								WM_USER + 705
#define		WM_EVENT_REQUEST_UNITID_READ						WM_USER + 706
#define		WM_EVENT_REPLY_UNITID_READ							WM_USER + 707
#define		WM_EVENT_REPORT_START_LOT							WM_USER + 708
#define		WM_EVENT_REPORT_START_PROCESS						WM_USER + 709
#define		WM_EVENT_REPORT_END_PROCESS							WM_USER + 710
#define		WM_EVENT_REPORT_END_EVENT							WM_USER + 711
#endif
//-------------------------------------------------------------------
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
#if defined(EES_XML)//20231003
	WM_Socket_LOTID			= 0x0040,
#endif
}WM_Socket_Update_Para;








//-------------------------------------------------------------------
//
//-------------------------------------------------------------------





//-------------------------------------------------------------------
//
//-------------------------------------------------------------------





#endif // Def_WindowMessage_h__
