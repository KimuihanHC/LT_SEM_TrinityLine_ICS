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
// #define SYS_DEV_MOD_1			0	// In-Line Control System
// #define SYS_DEV_MOD_2			1	// 
// #define SYS_DEV_MOD_3			2	// 
// #define SYS_DEV_MOD_4			3	// 
// #define SYS_DEV_MOD_5			4	// 

//=============================================================================
// 검사기 선택
//=============================================================================
#ifdef SET_INSPECTOR
#undef SET_INSPECTOR
#endif

#define		SET_INSPECTOR		SYS_DEV_MOD_1

// 프로젝트 속성->명령줄 /D "USE_DEBUG_MODE" // 디버그 모드 사용 여부 (물류 제어 조건 사용)


//=============================================================================
// 프로그램 환경설정 레지스트리 주소
//=============================================================================
#define REG_PATH_BASE			_T("Software\\Luritech")
//#define REG_PATH_OPTION_BASE	_T("Software\\Luritech\\Option")

//=============================================================================
// 기본 사용 장치 선택
//=============================================================================

#define		REG_PATH_APP_BASE		_T("Software\\Luritech\\SEM\\ICS")
#define		REG_PATH_OPTION_BASE	_T("Software\\Luritech\\SEM\\ICS\\Option")

//=============================================================================
// 프로그램 테스트 모드로 빌드시 장치 사용 여부 
//=============================================================================
#ifndef _DEBUG
	#define USE_HW_LOCK_KEY	// 하드웨어 락키 사용여부 (고객사 제공시 사용)
#endif


#define USE_NO_VERIFY_EQPID		// 설비에서 보내는 Eqp id 확인하지 않음 (기본: 사용)

#define USE_EQP_TACTTIME		// 설비별 Tacttime 체크 (** 삭제 예정)

//#define USE_TARGET_EQP_DISTANCE	// 목적 설비 적용시 출발->목적까지 거리로 재 판단하는 기능 사용 여부 (기본: 미사용)

#define USE_DISABLE_NOT_USE_EQUIPMENT_CONTROL	// 사용하지 않는 설비 개별 제어 버튼 감춤 (** 나중에 다시 고려)
#define USE_FORCED_TRACK_OUT_SEQ	// 소켓 트랙 아웃 무조건 배출 (기본: 사용) 


//#define USE_LISTCTRL_LANGUAGE // 리스트 컨트롤에 언어설정 사용 여부 (Release에서 오류 발생하여 수정 필요) (기본: 미사용)

//#define NEW_INSPECTION_TYPE_APPLY		// 설비 종류 및 검사 종류 분리 (2022.10.20) (개발 중지)



#endif // Def_CompileOption_h__
