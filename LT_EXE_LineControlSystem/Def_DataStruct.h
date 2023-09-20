//*****************************************************************************
// Filename	: Def_DataStruct.h
// Created	: 2012/11/1
// Modified	: 2016/12/29
//
// Author	: PiRing
//	
// Purpose	: 
//*****************************************************************************
#ifndef Def_DataStruct_h__
#define Def_DataStruct_h__

#include <afxwin.h>
#include "Def_DataStruct_Cm.h"
#include "Def_RecipeInfo_Cm.h"
#include "Def_ProductInfo.h"	// -> Equipment
#include "Def_ConfigLine.h"
#include "Def_ModelConfig.h"
#include "Def_Socket_Info.h"

#include "LineInfo.h"
#include "SocketMonitoring.h"
#include "Rework_Info.h"
#include "FailInfo.h"

#include "Def_DebugInfo.h"

#if (USE_XML)
#include "Def_Config_EES_LIST.h"
#include "Def_Config_ALID_LIST.h"
#include "Def_Config_Loss_LIST.h"
#include "Def_RecipeInfo_Cm.h"
#include "ServerInfo.h"
#endif

#pragma pack(push, 1)



//---------------------------------------------------------
// 레시피 설정 구조체
//---------------------------------------------------------
class CRecipeInfo : public CRecipeInfo_Base
{
public:
	CConfig_Line			LineInfo;
#if (USE_XML)
public:
	CConfig_EES_LIST		EES_Info;
	CConfig_AL_LIST			AL_Info;
	CConfig_Server			ServerInfo;
	CConfig_Loss_LIST		Loss_Info;
#endif
	CRecipeInfo()
	{
		Reset();
	};

	CRecipeInfo& operator= (CRecipeInfo& ref)
	{
		LineInfo		= ref.LineInfo;

#if (USE_XML)
		EES_Info		= ref.EES_Info;
		AL_Info			= ref.AL_Info;
		ServerInfo		= ref.ServerInfo;
		Loss_Info		= ref.Loss_Info;
#endif
		return *this;
	};

	void Reset()
	{
		LineInfo.RemoveAll();

#if (USE_XML)
		EES_Info.RemoveAll();
		AL_Info.RemoveAll();
		ServerInfo.RemoveAll();
		Loss_Info.RemoveAll();
#endif
	};
};
//---------------------------------------------------------
// 프로그램에 사용되는 경로
//---------------------------------------------------------
typedef struct _tag_ProgramPath
{
	CString		szProgram;		// 프로그램 시작 경로
	CString		szLog;			// LOG 경로
	CString		szReport;		// 검사 결과 Report 경로
	CString		szRecipe;		// 라인 설비 구조 파일
	CString		szSocket;		// 소켓 JIG 설정 파일
	CString		szShared;		// NG Code 파일 저장 경로
	CString		szFailInfo;		// 불량정보 ini 파일 저장 경로
}ST_ProgramPath, *PST_ProgramPath;
//---------------------------------------------------------
// 검사 진행 시간 측정용 구조체
//---------------------------------------------------------
typedef struct _tag_TestTime //: public ST_TestTime_Base
{
	// tacttime 체크 시작 여부 flag
	bool		m_bFlag_timeCheck = false;

	// 이전 소켓의 Unload 시간
	SYSTEMTIME	m_tmUnload_PrevSocket;

	// 계산된 tacttime
	double		m_dTacttime		= 0.0f;

	// 누적 평균 tacttime
	SYSTEMTIME	m_tmUnload_FstSocket;
	double		m_dTacttime_Ave	= 0.0f;
	uint32_t	m_nCount		= 0;

	_tag_TestTime()
	{
		ZeroMemory(&m_tmUnload_PrevSocket, sizeof(SYSTEMTIME));
	};

	void Reset()
	{
		//__super::Reset();

		m_bFlag_timeCheck = false;
		ZeroMemory(&m_tmUnload_PrevSocket, sizeof(SYSTEMTIME));
	};

	double Set_UnloadTime(__in SYSTEMTIME* IN_pUnloadTime)
	{
		if (m_bFlag_timeCheck)
		{
			m_dTacttime = CompareSystemTime(IN_pUnloadTime, &m_tmUnload_PrevSocket);

			memcpy(&m_tmUnload_FstSocket, IN_pUnloadTime, sizeof(SYSTEMTIME));
			memcpy(&m_tmUnload_PrevSocket, IN_pUnloadTime, sizeof(SYSTEMTIME));
		}
		else
		{
			memcpy(&m_tmUnload_PrevSocket, IN_pUnloadTime, sizeof(SYSTEMTIME));

			m_bFlag_timeCheck = true;

			m_dTacttime = 0;
		}

		return m_dTacttime;
	}

	double Get_TactTime()
	{
		return m_dTacttime;
	}

	double Get_TactTime_Ave()
	{
		return m_dTacttime_Ave;
	}
}ST_TestTime, *PST_TestTime;

//---------------------------------------------------------
// 전체 검사에 관련된 데이터 기록용 구조체
//---------------------------------------------------------
typedef struct _tag_SystemInfo : public ST_SystemInfo_Base
{
public:
	CRecipeInfo			SettingInfo;		// 기본 레시피 설정
	ST_ProgramPath		Path;				// 프로그램 경로 모음

	CLineInfo			LineInfo;			// 라인의 설비 정보
	CSocketMonitoring	SocketInfo;			// 소켓 상태 정보
	CFailInfo			FailInfo;			// NG Code 카운트
	ST_DebugInfo		DebugMode;			// 프로그램 운영 조건 (디버그 용도)
	
	ST_TestTime			Tacttime;

#if (USE_XML)
	CServerInfo			ServerInfo;
#endif
	// MES Rework NG Code
	//int16_t				m_nMES_ReworkCode = 0;	// REWORK 제품 MES 결과 코드: 설비에서 해당코드로 리턴 할 경우 REWORK 제품으로 양품 처리

	// 옵션?
	bool				m_bCrash_Executed = false;	// 이전 프로그램 실행할대 비정상 종료 여부

	_tag_SystemInfo()
	{
		SocketInfo.SetPtr_LineInfo(&LineInfo);
		LineInfo.SetPtr_DebugMode(&DebugMode);
		SocketInfo.SetPtr_DebugMode(&DebugMode);
#if (USE_XML)
		ServerInfo.SetPtr_DebugMode(&DebugMode);
#endif
	};

	// 검사기 종류 설정
	void OnSet_SystemType(__in enInsptrSysType nSysType)
	{
		//RecipeInfo.OnSet_SystemType(nSysType);
	};

	//------------------------------------------------------------------
	// 검사 정보 갱신
	//------------------------------------------------------------------

	
}ST_SystemInfo, *PST_InspectionInfo;


#pragma pack (pop)

#endif // Def_DataStruct_h__
