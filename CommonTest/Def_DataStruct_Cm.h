//*****************************************************************************
// Filename	: Def_DataStruct_Cm.h
// Created	: 2012/11/1
// Modified	: 2012/11/1 - 16:43
//
// Author	: PiRing
//	
// Purpose	: 
//*****************************************************************************
#ifndef Def_DataStruct_Cm_h__
#define Def_DataStruct_Cm_h__

#include <afxwin.h>
#include "Def_Enum_Cm.h"
#include "Def_Test_Cm.h"
#include "Def_Tacttime_Cm.h"

#pragma pack(push, 1)

//---------------------------------------------------------
// 자동 테스트 구동용 구조체
//---------------------------------------------------------
typedef struct _tag_SelfRun
{
	UINT	nModuleCnt;		// 현재 제품 카운트
	UINT	nCycleCnt;		// 현재 주기 카운트
	DWORD	dwRunTime;		// 현재 진행 시간

	UINT	nModuleCnt_Set;	// 설정: 제품 개수
	UINT	nCycleCnt_Set;	// 설정: 주기 회수
	DWORD	dwRunTime_Set;	// 설정: 진행 시간

	BOOL	bAutoBarcode;	// 자동으로 바코드 입력
	BOOL	bOnlyHandler;	// or With Tester;
	BOOL	bTypeCycleCnt;	// or Time

	_tag_SelfRun()
	{
		nModuleCnt		= 0;
		nCycleCnt		= 0;
		dwRunTime		= 0;

		nModuleCnt_Set	= 0;
		nCycleCnt_Set	= 0;
		dwRunTime_Set	= 0;

		bAutoBarcode	= FALSE;
		bOnlyHandler	= TRUE;
		bTypeCycleCnt	= TRUE;
	};

	void Reset()
	{
		nModuleCnt		= 0;
		nCycleCnt		= 0;
		dwRunTime		= 0;
	};

}ST_SelfRun;

//---------------------------------------------------------
// Cycle Time, Tact Time 설정용 구조체
//---------------------------------------------------------
// typedef struct _tag_CycleTime
// {
// 	// 생산가동율
// 	// 생산 가동시간
// 
// 	// Cycle Time
// 	DWORD	dwMinCT;
// 	DWORD	dwMaxCT;
// 	DOUBLE	dAverageCT;			// 평균 제품 Cycle Time
// 	DWORD64	dwCumulativeCT;		// 누적 제품 Cycle Time
// 	DWORD	dwOutputCnt;		// 생산 수량
// 
// 	// Tact Time
// 	DWORD	dwMinTactTime;
// 	DWORD	dwMaxTactTime;
// 	DOUBLE	dAveTactTime;		// 장비 Tact Time
// 	DWORD64	dwCumulativeTactTm;	// 장비 누적 Tact Time
// 
// 	// 소켓별 실제 검사시간
// 	DWORD	dwMinTT_Sock[MAX_SITE_CNT];
// 	DWORD	dwMaxTT_Sock[MAX_SITE_CNT];
// 	DOUBLE	dAverageTT_Sock[MAX_SITE_CNT];
// 	DWORD64	dwCumulativeTT_Sock[MAX_SITE_CNT];
// 	DWORD	dwPassCnt_Sock[MAX_SITE_CNT];		// 소켓별 양품 개수
// 
// 	// Site별 실제 검사시간
// 	DWORD	dwMinTT_Site[MAX_SITE_CNT];
// 	DWORD	dwMaxTT_Site[MAX_SITE_CNT];
// 	DOUBLE	dAverageTT_Site[MAX_SITE_CNT];
// 	DWORD64	dwCumulativeTT_Site[MAX_SITE_CNT];
// 	DWORD	dwTTCount_Site[MAX_SITE_CNT];
// 
// 	_tag_CycleTime()
// 	{
// 		dwMinCT				= 0xFFFFFFFF;
// 		dwMaxCT				= 0;
// 		dAverageCT			= 0.0f;
// 		dwCumulativeCT		= 0;
// 		dwOutputCnt			= 0;
// 
// 		dwMinTactTime		= 0xFFFFFFFF;
// 		dwMaxTactTime		= 0;
// 		dAveTactTime		= 0.0f;
// 		dwCumulativeTactTm	= 0;
// 
// 		for (UINT nIdx = 0; nIdx < MAX_SITE_CNT; nIdx++)
// 		{
// 			dwMinTT_Sock[nIdx]			= 0xFFFFFFFF;
// 			dwMaxTT_Sock[nIdx]			= 0;
// 			dAverageTT_Sock[nIdx]		= 0.0f;
// 			dwCumulativeTT_Sock[nIdx]	= 0;
// 			dwPassCnt_Sock[nIdx]		= 0;
// 
// 			dwMinTT_Site[nIdx]			= 0;
// 			dwMaxTT_Site[nIdx]			= 0;
// 			dAverageTT_Site[nIdx]		= 0.0f;
// 			dwCumulativeTT_Site[nIdx]	= 0;
// 			dwTTCount_Site[nIdx]		= 0;
// 		}
// 	};
// 
// 	void Reset()
// 	{
// 		dwMinCT				= 0xFFFFFFFF;
// 		dwMaxCT				= 0;
// 		dAverageCT			= 0.0f;
// 		dwCumulativeCT		= 0;
// 		dwOutputCnt			= 0;
// 
// 		dwMinTactTime		= 0xFFFFFFFF;
// 		dwMaxTactTime		= 0;
// 		dAveTactTime		= 0.0f;
// 		dwCumulativeTactTm	= 0;
// 
// 		for (UINT nIdx = 0; nIdx < MAX_SITE_CNT; nIdx++)
// 		{
// 			dwMinTT_Sock[nIdx]			= 0xFFFFFFFF;
// 			dwMaxTT_Sock[nIdx]			= 0;
// 			dAverageTT_Sock[nIdx]		= 0.0f;
// 			dwCumulativeTT_Sock[nIdx]	= 0;
// 			dwPassCnt_Sock[nIdx]		= 0;
// 
// 			dwMinTT_Site[nIdx]			= 0;
// 			dwMaxTT_Site[nIdx]			= 0;
// 			dAverageTT_Site[nIdx]		= 0.0f;
// 			dwCumulativeTT_Site[nIdx]	= 0;
// 			dwTTCount_Site[nIdx]		= 0;
// 		}
// 	}
// 
// 	void IncreaseOutputCount()
// 	{
// 		++dwOutputCnt;
// 	};
// 
// 	void AddCycleTime(__in DWORD dwCycleTime)
// 	{
// 		//++dwOutputCnt;
// 
// 		dwCumulativeCT += dwCycleTime;
// 
// 		if (0 != dwOutputCnt)
// 			dAverageCT = (DOUBLE)dwCumulativeCT / (DOUBLE)dwOutputCnt;
// 		else
// 			dAverageCT = 0.0f;
// 
// 		// Min
// 		dwMinCT = __min(dwCycleTime, dwMinCT);
// 
// 		// Max
// 		dwMaxCT = __max(dwCycleTime, dwMaxCT);
// 	};
// 
// 	void AddInspectionTime(__in UINT nSockIdx, __in DWORD dwTestTime)
// 	{
// 		if (nSockIdx < MAX_SITE_CNT)
// 		{
// 			++dwPassCnt_Sock[nSockIdx];
// 
// 			dwCumulativeTT_Sock[nSockIdx] += dwTestTime;
// 			dAverageTT_Sock[nSockIdx] = (DOUBLE)dwCumulativeTT_Sock[nSockIdx] / (DOUBLE)dwPassCnt_Sock[nSockIdx];
// 
// 			// Min
// 			dwMinTT_Sock[nSockIdx] = __min(dwTestTime, dwMinTT_Sock[nSockIdx]);
// 
// 			// Max
// 			dwMaxTT_Sock[nSockIdx] = __max(dwTestTime, dwMaxTT_Sock[nSockIdx]);
// 		}
// 	};
// 
// 	void AddTactTime(__in DWORD dwInputTactTime)
// 	{
// 		dwCumulativeTactTm += dwInputTactTime;
// 
// 		if (0 != dwOutputCnt)
// 			dAveTactTime = (DOUBLE)dwCumulativeTactTm / (DOUBLE)dwOutputCnt;
// 		else
// 			dAveTactTime = 0.0f;
// 
// 		// Min
// 		dwMinTactTime = __min(dwInputTactTime, dwMinTactTime);
// 
// 		// Max
// 		dwMaxTactTime = __max(dwInputTactTime, dwMaxTactTime);
// 	};
// 
// 	void AddTestTimeSite(__in UINT nSiteIdx, __in DWORD dwTestTime)
// 	{
// 		++dwTTCount_Site[nSiteIdx];
// 		dwCumulativeTT_Site[nSiteIdx] += dwTestTime;
// 
// 		if (0 != dwTTCount_Site[nSiteIdx])
// 			dAverageTT_Site[nSiteIdx] = (DOUBLE)dwCumulativeTT_Site[nSiteIdx] / (DOUBLE)dwTTCount_Site[nSiteIdx];
// 		else
// 			dAverageTT_Site[nSiteIdx] = 0.0f;
// 
// 		// Min
// 		dwMinTT_Site[nSiteIdx] = __min(dwTestTime, dwMinTT_Site[nSiteIdx]);
// 
// 		// Max
// 		dwMaxTT_Site[nSiteIdx] = __max(dwTestTime, dwMaxTT_Site[nSiteIdx]);
// 	};
// 
// 	void LoadPreviousCT(__in DWORD dwPrevCumulativeCT, __in DWORD dwPrevOutputCnt)
// 	{
// 		dwCumulativeCT	= dwPrevCumulativeCT;
// 		dwOutputCnt		= dwPrevOutputCnt;
// 
// 		if (0 != dwPrevOutputCnt)
// 		{
// 			dAverageCT = (DOUBLE)dwCumulativeCT / (DOUBLE)dwOutputCnt;
// 		}
// 		else
// 		{
// 			dwCumulativeCT	= 0;
// 			dAverageCT		= 0.0f;
// 		}	
// 	};
// 
// 	void LoadPreviousCT(__in _tag_CycleTime* pstCycleTime)
// 	{
// 		dwCumulativeCT = pstCycleTime->dwCumulativeCT;
// 		dwOutputCnt = pstCycleTime->dwOutputCnt;
// 
// 		if (0 != pstCycleTime->dwOutputCnt)
// 		{
// 			dAverageCT = (DOUBLE)dwCumulativeCT / (DOUBLE)dwOutputCnt;
// 
// 			for (UINT nIdx = 0; nIdx < MAX_SITE_CNT; nIdx++)
// 			{
// 				dwCumulativeTT_Sock[nIdx] = pstCycleTime->dwCumulativeTT_Sock[nIdx];
// 				dAverageTT_Sock[nIdx] = (DOUBLE)dwCumulativeTT_Sock[nIdx] / (DOUBLE)dwOutputCnt;
// 			}
// 		}
// 		else
// 		{
// 			dwCumulativeCT = 0;
// 			dAverageCT = 0.0f;
// 
// 			for (UINT nIdx = 0; nIdx < MAX_SITE_CNT; nIdx++)
// 			{
// 				dwCumulativeTT_Sock[nIdx] = 0;
// 				dAverageTT_Sock[nIdx] = 0.0f;
// 			}
// 		}
// 	};
// }ST_CycleTime, *PST_CycleTime;

//---------------------------------------------------------
// 에러 코드 처리용 구조체
//---------------------------------------------------------
typedef struct _tag_ErrorInfo
{
	SYSTEMTIME	tmTime;
	long		lCode;
	UINT		nType;
	CString		szDesc;

	_tag_ErrorInfo()
	{
		Reset();
	};

	void Reset()
	{
		ZeroMemory(&tmTime, sizeof(SYSTEMTIME));
		lCode = 0;
		nType = 0;
		szDesc.Empty();
	};

	_tag_ErrorInfo& operator= (_tag_ErrorInfo& ref)
	{		
		memcpy(&tmTime, &ref.tmTime, sizeof(SYSTEMTIME));
		lCode	= ref.lCode;
		nType	= ref.nType;
		szDesc	= ref.szDesc;

		return *this;
	};
}ST_ErrorInfo, *PST_ErrorInfo;



//---------------------------------------------------------
// 프로그램 경로 구조체
//---------------------------------------------------------
typedef struct _tag_ProgramPath_Base
{
	CString		szProgram;		// 프로그램 시작 경로
	CString		szLog;			// LOG 경로
	CString		szReport;		// 검사 결과 Report 경로
	CString		szRecipe;		// Recipe 설정 파일 경로
	
}ST_ProgramPath_Base, *PST_ProgramPath_Base;

//---------------------------------------------------------
// 검사 진행 시간 체크용 구조체
//---------------------------------------------------------
 typedef struct _tag_TestTime_Base
 {
 	// 전체 검사 시작 시각
 	SYSTEMTIME		tmStart_All;
 	// 전체 검사 종료 시각
 	SYSTEMTIME		tmEnd_All;
 
 	// 개별 검사 시작 시각
 	SYSTEMTIME		tmStart_Unit[MAX_MODULE_CNT];
 	// 개별 검사 종료 시각
 	SYSTEMTIME		tmEnd_Unit[MAX_MODULE_CNT];
 
 	// Site별 검사 시간 측정 용도 변수
 	DWORD			dwStart_All;				// 전체 검사 시작 체크용 변수
 	DWORD			dwDuration_All;				// 전체 검사 진행 시간
 	DWORD			dwStart[MAX_MODULE_CNT];	// 개별 검사 시작 체크용 변수
 	DWORD			dwDuration[MAX_MODULE_CNT];	// 개별 검사 진행 시간
 
 	_tag_TestTime_Base()
 	{
 		ZeroMemory(&tmStart_All, sizeof(SYSTEMTIME));
 		ZeroMemory(&tmEnd_All, sizeof(SYSTEMTIME));
 
 		for (UINT nIdx = 0; nIdx < MAX_MODULE_CNT; nIdx++)
 		{
 			ZeroMemory(&tmStart_Unit[nIdx], sizeof(SYSTEMTIME));
 			ZeroMemory(&tmEnd_Unit[nIdx], sizeof(SYSTEMTIME));
 		}
 
 		Reset();
 	};
 
 	void Reset()
 	{
 		dwStart_All		= 0;
 		dwDuration_All	= 0;
 		ZeroMemory(dwStart, sizeof(DWORD) * MAX_MODULE_CNT);
 		ZeroMemory(dwDuration, sizeof(DWORD) * MAX_MODULE_CNT);
 	};
 
 	void Reset_Unit()
 	{
 		ZeroMemory(dwStart, sizeof(DWORD) * MAX_MODULE_CNT);
 		ZeroMemory(dwDuration, sizeof(DWORD) * MAX_MODULE_CNT);
 	};
 }ST_TestTime_Base, *PST_TestTime_Base;

//---------------------------------------------------------
// 전체 검사에 관련된 데이터 기록용 구조체
//---------------------------------------------------------
typedef struct _tag_SystemInfo_Base
{	
	CCycleTime			CycleTime;			// 검사 진행 시간

	enPermissionMode	PermissionMode;		// 검사 모드 (작업자, 관리자, )
	CString				Login_UserID;
	CString				Login_UserName;

	CString				szEquipmentID;		// 검사기 고유 ID (MES)

	DWORD				dwPrevOutputTime;	// 이전 제품 배출 시간 (Unloader 기준)
	DWORD				dwTactTime;			// 현재 Tact Time

	_tag_SystemInfo_Base()
	{
		PermissionMode		= Permission_Operator;

		dwPrevOutputTime	= 0;
		dwTactTime			= 0;
	};

	void Initialize()
	{

	};

	// Tact 타임 설정
	DWORD SetTactTime(__in DWORD dwOutputTime)
	{
		if (0 != dwPrevOutputTime)
		{
			if (dwPrevOutputTime < dwOutputTime)
			{
				dwTactTime = dwOutputTime - dwPrevOutputTime;
			}
			else
			{
				dwTactTime = 0xFFFFFFFF - dwPrevOutputTime + dwOutputTime;
			}

			dwPrevOutputTime = dwOutputTime;
		}
		else
		{
			dwTactTime = 0;
			dwPrevOutputTime = dwOutputTime;
		}

		return dwTactTime;
	};

}ST_SystemInfo_Base, *PST_InspectionInfo_Base;




#pragma pack (pop)

#endif // Def_DataStruct_Cm_h__
