//*****************************************************************************
// Filename	: Def_WindowMessage_Cm.h
// Created	: 2012/1/16
// Modified	: 2016/08/17
//
// Author	: PiRing
//	
// Purpose	: 윈도우 메세지 정의 (0x0400 ~ 0x7FFF)
//*****************************************************************************
#ifndef Def_WindowMessage_Cm_h__
#define Def_WindowMessage_Cm_h__

//-------------------------------------------------------------------
// 프로그램 운영
//-------------------------------------------------------------------
#define		WM_ALIVE_PROCESS		(WM_USER + 108)

#define		WM_OPTION				WM_USER + 1		// 옵션 창 열기
#define		WM_OPTION_CHANGED		WM_USER + 2		// 옵션이 변경됨
#define		WM_LOGMSG				WM_USER + 3		// 로그
#define		WM_LOGMSG_DEV			WM_USER + 4		// 주변기기 로그 처리용
#define		WM_LOGMSG_PCB_CAM		WM_USER + 5		// 카메라 보드 로그 메세지
#define		WM_LOGMSG_HANDLER		WM_USER + 6		// 핸들러 로그 메세지
#define		WM_LOGMSG_TESTER		WM_USER + 7		// 테스터 로그 메세지

#define		WM_LOAD_COMPLETE		WM_USER + 10	// 프로그램 로딩 끝
#define		WM_TEST_FUNCTION		WM_USER + 11	// 테스트 함수 실행
#define		WM_SYSTEM_MODE			WM_USER + 12	// 프로그램 운용모드(wparam - 운용모드[관리자, LOT모드, MES모드] lParam - Type[LotStart, LotStop])

//-------------------------------------------------------------------
// 주변 기기 통신
//-------------------------------------------------------------------
#define		WM_RECV_LIGHT_BRD_ACK	WM_USER + 30	// 광원 보드로부터 데이터 수신
#define		WM_RECV_BARCODE			WM_USER + 31	// BCR로부터 데이터 수신
#define		WM_RECV_VIDEO			WM_USER + 32	// 카메라 영상 데이터가 영상보드로부터 수신됨
#define		WM_CAMERA_STATUS		WM_USER + 33	// 카메라 영상 데이터가 영상보드로부터 수신됨

#define		WM_CAMERA_SELECT		WM_USER + 34	// 화면에 보여줄 Camera 선택
#define		WM_CAMERA_CHG_STATUS	WM_USER + 35	// 카메라 영상 상태가 변경됨
#define		WM_CAMERA_RECV_VIDEO	WM_USER + 36	// 카메라 영상 데이터가 영상보드로부터 수신됨
#define		WM_RECV_MES				WM_USER + 27	// MES에서 데이터 수신

//-------------------------------------------------------------------
// 계측기 장비 제어
//-------------------------------------------------------------------
#define		WM_EXE_START			WM_USER + 40

#define		WM_CHANGE_VIEW			WM_USER + 41	// 윈도우 전환
#define		WM_PERMISSION_MODE		WM_USER + 42	// 관리자 모드 설정

#define		WM_TEST_START			WM_USER + 43	// 검사 시작
typedef enum
{
	WM_Test_Start_Default,			// 검사 시작 : 기본
	WM_Test_Start_Main_Setting,		// 검사 시작 : 메인 화면 설정값으로 시작
	WM_Test_Start_Seq_Init,			// 검사 시작 : 시퀀스(Init) 화면 Initialize
	WM_Test_Start_Seq_Final,		// 검사 시작 : 시퀀스(Init) 화면 Finalize
	WM_Test_Start_Seq_Custom,		// 검사 시작 : 시퀀스 화면 설정 시퀀스 시작 (레시피 설정값)
	WM_Test_Start_Recipe,			// 검사 시작 : 레시피 파일 설정
}WM_Test_Para;

#define		WM_TEST_STOP			WM_USER + 44	// 검사 중지(검사 실패, 오류)
#define		WM_TEST_COMPLETED		WM_USER + 45	// 검사 완료, 제품 배출
#define		WM_TEST_INIT			WM_USER + 46	// 테스트 초기화

#define		WM_FILE_RECIPE			WM_USER + 47	// 설정된 모델 폴더의 파일들이 변경,추가,삭제 되었음을 알림
#define		WM_CHANGED_RECIPE		WM_USER + 48	// 모델 파일의 데이터가 변경 되었음을 알림
#define		WM_REFESH_RECIPE		WM_USER + 49	// 모델 파일 리스트 갱신

#define		WM_MANUAL_DEV_CTRL		WM_USER + 50	// 주변장치 수동제어
#define		WM_MANUAL_TEST_ITEMID	WM_USER + 51	// 테스트 아이템 ID로 수동 검사

//#define		WM_INCREASE_POGO_CNT	WM_USER + 61	// 포고 카운트 증가 (A~D)
//#define		WM_UPDATE_POGO_CNT		WM_USER + 62	// 포고 카운트의 설정 데이터가 변경됨
//#define		WM_CHANGED_SITENAME		WM_USER + 63	// Site 및 테스트 명칭이 변경됨
//#define		WM_CHANGE_LOT			WM_USER + 64	// Lot 변경
//#define		WM_CHANGE_USERNAME		WM_USER + 65	// User or Operator Name 변경됨

#define		WM_MANUAL_BARCODE		WM_USER + 66	// 수동 바코드 입력

#define		WM_UI_ERR_RESET			WM_USER + 98	// 에러 초기화

#define		WM_EXE_END				WM_USER + 99

//-------------------------------------------------------------------
//
//-------------------------------------------------------------------


#endif // Def_WindowMessage_Cm_h__
