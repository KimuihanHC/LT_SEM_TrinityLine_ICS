//*****************************************************************************
// Filename	: Def_CompileOption.h
// Created	: 2021/10/21
// Modified	: 2021/10/21
//
// Author	: PiRing
//	
// Purpose	: 물류 제어 시스템
//*****************************************************************************
#ifndef Def_CompileOption_h__
#define Def_CompileOption_h__

#include "Def_Enum_Cm.h"


//=============================================================================
// 프로그램 종류
//=============================================================================
// #define SYS_ICS_TRINITY_LINE			0	// In-Line Control System
// #define SYS_ICS_RIVIAN_LINE			1	// 
// #define SYS_DEV_MOD_3				2	// 
// #define SYS_DEV_MOD_4				3	// 
// #define SYS_DEV_MOD_5				4	// 

//=============================================================================
// 검사기 선택
//=============================================================================
// 프로젝트 속성->명령줄 /DLT_EQP#0 // 프로그램 종류 설정 (0, 1, ...)

#ifdef SET_INSPECTOR
#undef SET_INSPECTOR
#endif

#ifdef LT_EQP
#define	SET_INSPECTOR	LT_EQP
#else			// 수동 설정
#define		SET_INSPECTOR		SYS_ICS_TRINITY_LINE
//#define		SET_INSPECTOR		SYS_ICS_RIVIAN_LINE
#endif

// 프로젝트 속성->명령줄 /D "USE_DEBUG_MODE" // 디버그 모드 사용 여부 (물류 제어 조건 사용)


//=============================================================================
// 프로그램 환경설정 레지스트리 주소
//=============================================================================
#define REG_PATH_BASE			_T("Software\\Luritech")
//#define REG_PATH_OPTION_BASE	_T("Software\\Luritech\\Option")

//=============================================================================
// 기본 사용 장치 선택
//=============================================================================

#if (SET_INSPECTOR == SYS_ICS_TRINITY_LINE)
#define		REG_PATH_APP_BASE		_T("Software\\Luritech\\SEM\\ICS")
#define		REG_PATH_OPTION_BASE	_T("Software\\Luritech\\SEM\\ICS\\Option")
#elif (SET_INSPECTOR == SYS_ICS_RIVIAN_LINE)
#define		REG_PATH_APP_BASE		_T("Software\\Luritech\\SEM\\ICS_Rivian")
#define		REG_PATH_OPTION_BASE	_T("Software\\Luritech\\SEM\\ICS_Rivian\\Option")
#else
#define		REG_PATH_APP_BASE		_T("Software\\Luritech\\SEM\\ICS")
#define		REG_PATH_OPTION_BASE	_T("Software\\Luritech\\SEM\\ICS\\Option")
#endif

//=============================================================================
// 프로그램 테스트 모드로 빌드시 장치 사용 여부 
//=============================================================================
#ifndef _DEBUG
	#define USE_HW_LOCK_KEY	// 하드웨어 락키 사용여부 (고객사 제공시 사용)
#endif


#define USE_NO_VERIFY_EQPID					// 설비에서 보내는 Eqp id 확인하지 않음 (기본: 사용)
#define USE_EQP_TACTTIME					// 설비별 Tacttime 체크 (** 삭제 예정)

//#define USE_TARGET_EQP_DISTANCE			// 목적 설비 적용시 출발->목적까지 거리로 재 판단하는 기능 사용 여부 (기본: 미사용) *** 삭제 예정

#define HIDE_NOT_USE_EQP_CONTROL			// 사용하지 않는 설비 개별 제어 버튼 감춤 (** 나중에 다시 고려)
#define USE_FORCED_TRACK_OUT_SEQ			// 소켓 트랙 아웃 무조건 배출 (기본: 사용) 

//#define USE_LISTCTRL_LANGUAGE				// 리스트 컨트롤에 언어설정 사용 여부 (Release에서 오류 발생하여 수정 필요) (기본: 미사용)
//#define NEW_INSPECTION_TYPE_APPLY			// 설비 종류 및 검사 종류 분리 (2022.10.20) (개발 중지)

#define USE_SET_TARGET_EQP_GROUP_TRACK_IN	// 동일 검사 설비가 여러대인 경우 첫번째 설비에 소켓이 도착했을 때 타겟 설비를 재 설정한다. (2023.03.23) (기본: 사용)

//#define USE_NG_CODE_UPDATE_AT_UNLOAD		// 로더에서 소켓 배출 할 때 NG Code 갯수 업데이트 (기존: 설비에서 검사 결과 수신할 때 업데이트) 2023.03.29
//#define USE_EQP_YIELD_UPDATE_AT_UNLOAD	// 로더에서 소켓 배출 할 때 설비별 수율(NG인 경우에만) 업데이트 (기존: 설비에서는 검사 결과 수실할 때 업데이트) 2023.03.29
//#define USE_IN_OUT_COUNT_UI				// 소켓 투입/배출 현황 UI 사용 여부 (2023.03.29)


#if (SET_INSPECTOR == SYS_ICS_RIVIAN_LINE)
#define USE_AUTO_TO_MANUAL_AUTOCHANGE		// Auto 모드에서 일정시간 제품 투입이 없으면 Manual 모드로 자동 전환 기능 (2023.07.19) RIVIAN 우선 적용
#endif


#endif // Def_CompileOption_h__
