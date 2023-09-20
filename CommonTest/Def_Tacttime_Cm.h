//*****************************************************************************
// Filename	: 	Def_Tacttime_Cm.h
// Created	:	2021/1/25 - 10:39
// Modified	:	2021/1/25 - 10:39
//
// Author	:	piring
//	
// Purpose	:	
//*****************************************************************************
#ifndef Def_Tacttime_Cm_h__
#define Def_Tacttime_Cm_h__

#include <afxwin.h>
#include <Mmsystem.h>
#include <vector>

#pragma once

#pragma pack(push, 1)

//#define		MAX_TESTITEM_COUNT		80
//#define		MAX_TESTSOCKET_COUNT	6

//---------------------------------------------------------
// Elapsed Time 설정용 구조체
//---------------------------------------------------------
class CElapTime
{
public:
	DWORD	dwMinTime;			// 최단 Elapsed Time
	DWORD	dwMaxTime;			// 최장 Elapsed Time
	DOUBLE	dAverageTime;		// 장비 Elapsed Time
	DWORD64	dwCumulativeTime;	// 장비 누적 Elapsed Time
	DWORD	dwCount;			// 측정 갯수

	CElapTime()
	{
		Reset();
	};

	CElapTime& operator= (CElapTime& ref)
	{
		dwMinTime			= ref.dwMinTime;
		dwMaxTime			= ref.dwMaxTime;
		dAverageTime		= ref.dAverageTime;
		dwCumulativeTime	= ref.dwCumulativeTime;
		dwCount				= ref.dwCount;
	};

	void Reset()
	{
		dwMinTime			= 0xFFFFFFFF;
		dwMaxTime			= 0;
		dAverageTime		= 0.0f;
		dwCumulativeTime	= 0;
		dwCount				= 0;
	};

	void Add_ElapTime(__in DWORD IN_dwInputElapTime)
	{
		// 카운트 증가
		++dwCount;

		// 누적 타임 
		dwCumulativeTime += IN_dwInputElapTime;

		// 평균
		if (0 != dwCount)
			dAverageTime = (DOUBLE)dwCumulativeTime / (DOUBLE)dwCount;
		else
			dAverageTime = 0.0f;

		// Min
		dwMinTime = __min(IN_dwInputElapTime, dwMinTime);

		// Max
		dwMaxTime = __max(IN_dwInputElapTime, dwMaxTime);
	};


	void Load_PreviousElapTime(__in DWORD IN_dwPrevCumulativeTime, __in DWORD IN_dwPrevOutputCnt)
	{
		dwCumulativeTime = IN_dwPrevCumulativeTime;
		dwCount = IN_dwPrevOutputCnt;

		if (0 != IN_dwPrevOutputCnt)
		{
			dAverageTime = (DOUBLE)dwCumulativeTime / (DOUBLE)dwCount;
		}
		else
		{
			dwCumulativeTime	= 0;
			dAverageTime		= 0.0f;
		}
	};

};

//---------------------------------------------------------
// 검사 진행 시간 구조체
//---------------------------------------------------------
class CCycleTime
{
public:
	CElapTime		CycleTime;					// 검사 시작 -> 검사 종료까지 시간
	CElapTime		TactTime;					// 이전 제품 배출 -> 현재 제품 배출까지 시간

	std::vector<CElapTime>	ItemCycleTime;		// 검사 항목 별 검사 시작 -> 검사 종료까지 시간
	std::vector<CElapTime>	ParaTestTime;		// 검사 시작 -> 검사 종료까지 시간

	UINT			nTestItemCnt;				// 검사 항목 개수
	UINT			nSocketCnt;					// 검사 소켓(파라) 개수

	CCycleTime()
	{
		nTestItemCnt	= 0;
		nSocketCnt		= 0;

		ItemCycleTime.reserve(nTestItemCnt);
		ParaTestTime.reserve(nSocketCnt);
	};

	CCycleTime(__in UINT IN_TestItemCnt, __in UINT IN_SocketCnt)
	{
		nTestItemCnt	= IN_TestItemCnt;
		nSocketCnt		= IN_SocketCnt;

		ItemCycleTime.reserve(nTestItemCnt);
		ParaTestTime.reserve(nSocketCnt);
	};

	CCycleTime& operator= (CCycleTime& ref)
	{
		CycleTime = ref.CycleTime;
		TactTime = ref.TactTime;

		ItemCycleTime.clear();
		ItemCycleTime = ref.ItemCycleTime;

		ParaTestTime.clear();
		ParaTestTime = ref.ParaTestTime;

		return *this;
	};

	void Reset()
	{
		CycleTime.Reset();
		TactTime.Reset();

		for (UINT nIdx = 0; nIdx < ItemCycleTime.size(); nIdx++)
		{
			ItemCycleTime[nIdx].Reset();
		}

		for (UINT nIdx = 0; nIdx < ParaTestTime.size(); nIdx++)
		{
			ParaTestTime[nIdx].Reset();
		}
	};

	void Add_CycleTime(__in DWORD IN_dwCycleTime)
	{
		CycleTime.Add_ElapTime(IN_dwCycleTime);
	};

	void Add_TestTime(__in DWORD IN_dwTestTime, __in UINT IN_nParaIdx = 0)
	{
		if (IN_nParaIdx < ParaTestTime.size())
		{
			ParaTestTime[IN_nParaIdx].Add_ElapTime(IN_dwTestTime);
		}
	};

	void Add_ItemCycleTime(__in DWORD IN_dwCycleTime, __in UINT IN_nTestItemIndex)
	{
		if (IN_nTestItemIndex < ItemCycleTime.size())
		{
			ItemCycleTime[IN_nTestItemIndex].Add_ElapTime(IN_dwCycleTime);
		}
	};

	void Add_TactTime(__in DWORD IN_dwTactTime)
	{
		TactTime.Add_ElapTime(IN_dwTactTime);
	};

};

//---------------------------------------------------------
// 검사 진행 시간 체크용 구조체
//---------------------------------------------------------
class CTestTime_Unit
{
public:
	DWORD	dwStart;		// 시작 시간
	DWORD	dwEnd;			// 종료 시간
	DWORD	dwDuration;		// 진행 시간

	CTestTime_Unit()
	{
		Reset();
	};

	CTestTime_Unit& operator= (CTestTime_Unit& ref)
	{
		dwStart		= ref.dwStart;
		dwEnd		= ref.dwEnd;
		dwDuration	= ref.dwDuration;

		return *this;
	};

	void Reset()
	{
		dwStart		= 0;
		dwEnd		= 0;
		dwDuration	= 0;
	};

	void Set_Start()
	{
		dwStart = timeGetTime();
	};

	// 종료 시간 설정
	void Set_End()
	{
		dwEnd = timeGetTime();

		if (dwEnd < dwStart)
		{
			dwDuration = 0xFFFFFFFF - dwStart + dwEnd;
		}
		else
		{
			dwDuration = dwEnd - dwStart;
		}
	};

	// 시작부터 현재 체크하는 시간까지 걸린 시간 계산
	DWORD Get_Duration(__in DWORD IN_dwTimeCheck)
	{
		if (IN_dwTimeCheck < dwStart)
		{
			dwDuration = 0xFFFFFFFF - dwStart + IN_dwTimeCheck;
		}
		else
		{
			dwDuration = IN_dwTimeCheck - dwStart;
		}

		return dwDuration;
	};
};

class CTestTime
{
public:
	SYSTEMTIME		tmStart_Test;			// 전체 검사 시작 시각
	SYSTEMTIME		tmEnd_Test;				// 전체 검사 종료 시각
	SYSTEMTIME		tmStart_Loading;		// 팔레트 로딩 시작 시각
	SYSTEMTIME		tmEnd_Unloading;		// 팔레트 로딩 종료 시각

	CTestTime_Unit	Test;					// 검사 구간 진행 시간 (총 진행 시간에서 로딩, 언로딩 시간 제외)
	CTestTime_Unit	Cycle;					// 검사 총진행 시간 (제품 투입부터 배출까지)
	std::vector<CTestTime_Unit>	TestItem;	// 검사 항목 별 검사 시간 측정 용도 변수


	CTestTime()
	{
		Reset();

		//TestItem.reserve(MAX_TESTITEM_COUNT);
	};

	CTestTime(__in UINT nTestitemCnt)
	{
		Reset();

		TestItem.reserve(nTestitemCnt);
	};

	CTestTime& operator= (CTestTime& ref)
	{
		memcpy(&tmStart_Test, &ref.tmStart_Test, sizeof(SYSTEMTIME));
		memcpy(&tmEnd_Test, &ref.tmEnd_Test, sizeof(SYSTEMTIME));
		memcpy(&tmStart_Loading, &ref.tmStart_Loading, sizeof(SYSTEMTIME));
		memcpy(&tmEnd_Unloading, &ref.tmEnd_Unloading, sizeof(SYSTEMTIME));

		Test = ref.Test;
		Cycle = ref.Cycle;

		TestItem.clear();
		TestItem = ref.TestItem;

		return *this;
	};

	void Reset()
	{
		Test.Reset();
		Cycle.Reset();

		for (UINT nIdx = 0; nIdx < TestItem.size(); nIdx++)
		{
			TestItem[nIdx].Reset();
		}

		ZeroMemory(&tmStart_Test,	 sizeof(SYSTEMTIME));
		ZeroMemory(&tmEnd_Test,		 sizeof(SYSTEMTIME));
		ZeroMemory(&tmStart_Loading, sizeof(SYSTEMTIME));
		ZeroMemory(&tmEnd_Unloading, sizeof(SYSTEMTIME));
	};

	// 검사 시간 초기화
	void Reset_Test()
	{
		Test.Reset();
		for (UINT nIdx = 0; nIdx < TestItem.size(); nIdx++)
		{
			TestItem[nIdx].Reset();
		}

		ZeroMemory(&tmStart_Test, sizeof(SYSTEMTIME));
		ZeroMemory(&tmEnd_Test, sizeof(SYSTEMTIME));
	};

	// 검사 항목 검사 시간 초기화
	void Reset_TestItem()
	{
		for (UINT nIdx = 0; nIdx < TestItem.size(); nIdx++)
		{
			TestItem[nIdx].Reset();
		}
	};

	// 팔레트 로딩 시간 초기화
	void Reset_Loading()
	{
		Cycle.Reset();

		ZeroMemory(&tmStart_Loading, sizeof(SYSTEMTIME));
		ZeroMemory(&tmEnd_Unloading, sizeof(SYSTEMTIME));
	};

	// 팔레트 로딩 시작 시간 설정
	void Set_StartLoading()
	{
		GetLocalTime(&tmStart_Loading);

		Cycle.Set_Start();
	};

	// 팔레트 로딩 종료 시간 설정
	void Set_EndUnloading()
	{
		GetLocalTime(&tmEnd_Unloading);

		Cycle.Set_End();
	};

	// 시작부터 현재 체크하는 시간까지 걸린 시간 계산
	DWORD Get_Duration_Cycle(__in DWORD IN_dwTimeCheck)
	{
		return Cycle.Get_Duration(IN_dwTimeCheck);
	};

	// 전체 검사 시작 시간 설정
	void Set_StartTest()
	{
		GetLocalTime(&tmStart_Test);

		Test.Set_Start();
	};

	// 전체 검사 종료 시간 설정
	void Set_EndTest()
	{
		GetLocalTime(&tmEnd_Test);
		Test.Set_End();
	};

	// 시작부터 현재 체크하는 시간까지 걸린 시간 계산
	DWORD Get_Duration_Test(__in DWORD IN_dwTimeCheck)
	{
		return Test.Get_Duration(IN_dwTimeCheck);
	};

	// 개별 검사 항목별 검사 시간 설정
	void Set_StartTestItem(__in UINT IN_nItemIndex)
	{
		if (IN_nItemIndex < TestItem.size())
		{
			TestItem[IN_nItemIndex].Set_Start();
		}
	};

	// 개별 검사 항목별 검사 종료 시간 설정
	void Set_EndTestItem(__in UINT IN_nItemIndex)
	{
		if (IN_nItemIndex < TestItem.size())
		{
			TestItem[IN_nItemIndex].Set_End();
		}
	};

	// 검사 항목의 검사 진행 시간 얻기
	DWORD Get_Duration_TestItem(DWORD IN_dwTimeCheck, __in UINT IN_nItemIndex)
	{
		return TestItem[IN_nItemIndex].Get_Duration(IN_dwTimeCheck);
	};

};

#pragma pack (pop)

#endif // Def_Tacttime_Cm_h__
