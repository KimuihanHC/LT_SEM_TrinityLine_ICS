//*****************************************************************************
// Filename	: 	Def_Test.h
// Created	:	2016/06/30
// Modified	:	2016/08/08
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
#ifndef Def_Test_h__
#define Def_Test_h__

#include "Def_Enum.h"
#include "Def_Test_Cm.h"

//msec 측정 라이브러리 추가
#include <Mmsystem.h>
#pragma comment (lib,"winmm.lib")

// 결과 코드 채널 개별 검사
typedef enum enResultCode
{
	RC_UnknownError = 0,
	RC_OK,
	RC_Fail,
	RC_TestSkip,
	RC_Invalid_Handle,
	RC_Invalid_Point,
	RC_Exception,
	RC_Safty_Err,
	RC_DeviceComm_Err,
	RC_Wrong_InspectionSystem,
	RC_Recipe_Err,
	RC_NoBarcode,	// 바코드 없음
	RC_NoPallet_ID,
	RC_LimitPogoCnt,
	RC_NotReadyTest,
	RC_AlreadyLoadUnload_OtherPara,
	RC_AlreadyTesting,
	RC_AlreadyTestItem_Thread,
	RC_Forced_Stoped,
	RC_WaitEndTest_TimeOut,
	RC_WaitEndTest_Wait_Faild,
	RC_WaitPLCOut_TimeOut,
	RC_WaitPLCOut_Wait_Faild,
	RC_LoadingFailed,

	RC_NotConnect_ChBrd,
	RC_NotConnect_CtrlBrd,
	RC_Comm_Err_ChBrd,
	RC_Comm_Err_CtrlBrd,
	RC_Semaphore_Locked_ChBrd,
	RC_Semaphore_Locked_CtrlBrd,

	RC_Max,
}enResultCode;

static LPCTSTR g_szResultCode[] =
{
	_T("UnknownError"),						// RC_UnknownError = 0,
	_T("OK"),								// RC_OK,
	_T("Test Fail"),						// RC_Fail,
	_T("TestSkip"),							// RC_TestSkip,
	_T("Invalid_Handle"),					// RC_Invalid_Handle,
	_T("Invalid_Point"),					// RC_Invalid_Point,
	_T("Exception"),						// RC_Exception,
	_T("Safty_Err"),						// RC_Safty_Err,
	_T("DeviceComm_Err"),					// RC_DeviceComm_Err,
	_T("Wrong_InspectionSystem"),			// RC_Wrong_InspectionSystem,
	_T("Recipe_Err"),						// RC_Recipe_Err,
	_T("No Barcode"),						// RC_NoBarcode,	// 바코드 없음
	_T("NoPallet_ID"),						// RC_NoPallet_ID,
	_T("LimitPogoCnt"),						// RC_LimitPogoCnt,
	_T("NotReadyTest"),				  		// RC_NotReadyTest,
	_T("AlreadyLoadUnload_OtherPara"),		// RC_AlreadyLoadUnload_OtherPara,
	_T("AlreadyTest_OtherPara"),	  		// RC_AlreadyTesting,
	_T("AlreadyTestItem_Thread"),	  		// RC_AlreadyTestItem_Thread,
	_T("Forced_Stoped"),			  		// RC_Forced_Stoped,
	_T("WaitEndTest_TimeOut"),		  		// RC_WaitEndTest_TimeOut,
	_T("WaitEndTest_Wait_Faild"),	  		// RC_WaitEndTest_Wait_Faild,
	_T("WaitPLCOut_TimeOut"),		  		// RC_WaitPLCOut_TimeOut,
	_T("WaitPLCOut_Wait_Faild"),	  		// RC_WaitPLCOut_Wait_Faild,
	_T("LoadingFailed"),					// RC_LoadingFailed,

	_T("Not Connect ChBrd"),				// RC_NotConnect_ChBrd,
	_T("Not Connect CtrlBrd"),				// RC_NotConnect_CtrlBrd,
	_T("Comm Err ChBrd "),					// RC_Comm_Err_ChBrd,
	_T("Comm Err CtrlBrd"),					// RC_Comm_Err_CtrlBrd,
	_T("Semaphore Locked ChBrd"),			// RC_Semaphore_Locked_ChBrd,
	_T("Semaphore Locked CtrlBrd"),			// RC_Semaphore_Locked_CtrlBrd,

	
	NULL
};

//-----------------------------------------------------------------------------
// 카메라 검사 정보 구조체
//-----------------------------------------------------------------------------
typedef struct _tag_CamInfo
{
	CString				szLotID;			// Lot ID	
	CString				szRecipeFileTitle;	// 모델 이름
	CString				szOperatorName;		// 작업자 이름
	CString				szBarcode;			// Barcode

	SYSTEMTIME			tmInputTime;		// 제품 투입시간
	DWORD				dwInputTime;		// CycleTime 계산용 제품 투입시간
	SYSTEMTIME			tmOutputTime;		// 제품 배출시간
	DWORD				dwOutputTime;		// CycleTime 계산용 제품 배출시간
	DWORD				dwCycleTime;		// 제품 투입에서 배출까지 시간
	DWORD				dwTactTime;			// Tact Time
	CString				szDay;				// ex)20161008
	CString				szTime;				// ex)180000

	UINT				nSocketIndex;		// 테이블 위치 (Socket)
	enTestProcess		nProgressStatus;	// 각 카메라의 검사 진행 상태
	enTestResult		nJudgment;			// 최종 결과, 제품 유무
	LRESULT				ResultCode;			// 결과 코드 (오류 코드)	
	
	_tag_CamInfo()
	{
		ZeroMemory(&tmInputTime, sizeof(SYSTEMTIME));
		ZeroMemory(&tmOutputTime, sizeof(SYSTEMTIME));
		dwInputTime		= 0;
		dwOutputTime	= 0;
		dwCycleTime		= 0;
		dwTactTime		= 0;

		nSocketIndex	= 0;
		nProgressStatus = enTestProcess::TP_Ready;
		nJudgment		= enTestResult::TR_Empty;
		ResultCode		= RC_OK;
	};

	void Reset()
	{
		szLotID.Empty();
		szRecipeFileTitle.Empty();
		szOperatorName.Empty();
		szBarcode.Empty();

		ZeroMemory(&tmInputTime, sizeof(SYSTEMTIME));
		ZeroMemory(&tmOutputTime, sizeof(SYSTEMTIME));
		dwInputTime		= 0;
		dwOutputTime	= 0;
		dwCycleTime		= 0;
		dwTactTime		= 0;

		szDay.Empty();
		szTime.Empty();
		nProgressStatus = enTestProcess::TP_Ready;
		nJudgment		= enTestResult::TR_Empty;
		ResultCode		= RC_OK;
	};

	_tag_CamInfo& operator= (_tag_CamInfo& ref)
	{
		szLotID			= ref.szLotID;		
		szRecipeFileTitle		= ref.szRecipeFileTitle;
		szOperatorName	= ref.szOperatorName;
		szBarcode		= ref.szBarcode;

		memcpy(&tmInputTime, &ref.tmInputTime, sizeof(SYSTEMTIME));
		memcpy(&tmOutputTime, &ref.tmOutputTime, sizeof(SYSTEMTIME));
		dwInputTime		= ref.dwInputTime;
		dwOutputTime	= ref.dwOutputTime;
		dwCycleTime		= ref.dwCycleTime;
		dwTactTime		= ref.dwTactTime;
		szDay			= ref.szDay;
		szTime			= ref.szTime;
		nSocketIndex	= ref.nSocketIndex;
		nProgressStatus = ref.nProgressStatus;
		nJudgment		= ref.nJudgment;
		ResultCode		= ref.ResultCode;

		return *this;
	};

	void SetTestProgress(__in enTestProcess nProcess)
	{
		nProgressStatus = nProcess;
	};

	void SetInformation(__in LPCTSTR szIn_LotName, __in LPCTSTR szIn_Barcode, __in LPCTSTR szIn_ModelName, __in LPCTSTR szIn_OperatorName)
	{
		szLotID			= szIn_LotName;
		szBarcode		= szIn_Barcode;
		szRecipeFileTitle		= szIn_ModelName;		// 모델 이름
		szOperatorName	= szIn_OperatorName;
	};

	void SetInputTime()
	{		
		GetLocalTime(&tmInputTime);
		dwInputTime = timeGetTime();
		szDay.Format(_T("%04d/%02d/%02d"), tmInputTime.wYear, tmInputTime.wMonth, tmInputTime.wDay);
		szTime.Format(_T("%02d:%02d:%02d"), tmInputTime.wHour, tmInputTime.wMinute, tmInputTime.wSecond);
	};

	void SetInputTime(__in SYSTEMTIME tmIn, __in DWORD dwIn)
	{
		memcpy(&tmInputTime, &tmIn, sizeof(SYSTEMTIME));
		dwInputTime = dwIn;
		szDay.Format(_T("%04d/%02d/%02d"), tmInputTime.wYear, tmInputTime.wMonth, tmInputTime.wDay);
		szTime.Format(_T("%02d:%02d:%02d"), tmInputTime.wHour, tmInputTime.wMinute, tmInputTime.wSecond);
	};

 	void SetOutputTime()
 	{
 		GetLocalTime(&tmOutputTime);
		dwOutputTime = timeGetTime();

		if (dwOutputTime < dwInputTime)
		{
			dwCycleTime = 0xFFFFFFFF - dwInputTime + dwOutputTime;
		}
		else
		{
			dwCycleTime = dwOutputTime - dwInputTime;
		}
 	};

	void SetOutputTime(__in SYSTEMTIME tmOut, __in DWORD dwOut)
	{
		memcpy(&tmOutputTime, &tmOut, sizeof(SYSTEMTIME));
		dwOutputTime = dwOut;

		if (dwOutputTime < dwInputTime)
		{
			dwCycleTime = 0xFFFFFFFF - dwInputTime + dwOutputTime;
		}
		else
		{
			dwCycleTime = dwOutputTime - dwInputTime;
		}
	};

	void SetSocketIndex(__in UINT nIdx)
	{
		nSocketIndex = nIdx;
	};

}ST_CamInfo;


#endif // Def_Test_h__
