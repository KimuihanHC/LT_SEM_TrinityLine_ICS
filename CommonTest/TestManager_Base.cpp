//*****************************************************************************
// Filename	: 	TestManager_Base.cpp
// Created	:	2016/5/16 - 17:02
// Modified	:	2016/5/16 - 17:02
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
#include "stdafx.h"
#include "TestManager_Base.h"
#include <strsafe.h>

CTestManager_Base::CTestManager_Base()
{
	// Event 처리 관련
	m_bExitFlag				= FALSE;
	m_hEvent_ProgramExit	= NULL;
	m_hEvent_ForcedStop		= NULL;

	// 동기화 처리 관련
	m_dwSemaphoreWaitTime	= 100;
	m_hSemaphore			= NULL;

	// 타이머 관련
	m_hTimer_TimeCheck		= NULL;
	m_hTimer_Update_UI		= NULL;
	m_hTimerQueue			= NULL;

	OnInitialize();

}


CTestManager_Base::~CTestManager_Base()
{
	TRACE(_T("<<< Start ~CTestManager_Base >>> \n"));

	OnFinalize();

	TRACE(_T("<<< End ~CTestManager_Base >>> \n"));
}

//=============================================================================
// Method		: SetEvent_ForcedStop
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2017/9/19 - 21:44
// Desc.		:
//=============================================================================
void CTestManager_Base::SetEvent_ForcedStop()
{
	if (FALSE == SetEvent(m_hEvent_ForcedStop))
	{
		TRACE(_T("Set Forced Stop Event 실패!!\n"));
	}
}

//=============================================================================
// Method		: LockSemaphore
// Access		: protected  
// Returns		: DWORD
// Qualifier	:
// Last Update	: 2016/5/31 - 22:10
// Desc.		:
//=============================================================================
DWORD CTestManager_Base::LockSemaphore()
{
	DWORD	dwResult = WaitForSingleObject(m_hSemaphore, m_dwSemaphoreWaitTime);
	if (WAIT_OBJECT_0 != dwResult)
	{
		TRACE(_T("------------** Error : Semaphore is Alive **------------\n"));
	}
	else
	{
		//TRACE (_T("------------** Enter Semaphore **------------\n"));
	}

	return dwResult;
}

//=============================================================================
// Method		: UnlockSemaphore
// Access		: protected  
// Returns		: BOOL
// Qualifier	:
// Last Update	: 2016/5/31 - 22:10
// Desc.		:
//=============================================================================
BOOL CTestManager_Base::UnlockSemaphore()
{
	BOOL bResult = ReleaseSemaphore (m_hSemaphore, 1, NULL);

	/*if (!bResult) 
	{ 
	TRACE (_T("------------** Error : ReleaseSemaphore() **------------\n"));
	} 
	TRACE (_T("------------** ReleaseSemaphore() **------------\n"));*/

	return bResult;
}

//=============================================================================
// Method		: WaitSemaphore
// Access		: protected  
// Returns		: BOOL
// Parameter	: __in DWORD dwTimeout
// Qualifier	:
// Last Update	: 2016/5/31 - 22:10
// Desc.		:
//=============================================================================
BOOL CTestManager_Base::WaitSemaphore(__in DWORD dwTimeout /*= 2000*/)
{
	BOOL bResult = FALSE;
	DWORD	dwResult = WaitForSingleObject(m_hSemaphore, dwTimeout);

	switch (dwResult)
	{
	case WAIT_OBJECT_0:
		//TRACE (_T("------------** WaitSemaphore() : Success **------------\n"));
		bResult = TRUE;
		break;

	case WAIT_TIMEOUT:
		//TRACE (_T("------------** WaitSemaphore() : Timeout **------------\n"));
		bResult = FALSE;
		break;

	case WAIT_ABANDONED:
		//TRACE (_T("------------** WaitSemaphore() : Abandoned **------------\n"));
		bResult = FALSE;
		break;
	}

	return bResult;
}

//=============================================================================
// Method		: OnInitLogFolder
// Access		: virtual protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2016/9/23 - 17:25
// Desc.		:
//=============================================================================
void CTestManager_Base::OnInit_LogFolder()
{
	// 로그 처리
	m_logFile.SetPath(_T("C:\\Log\\"), _T("Inspector"));
	m_Log_ErrLog.SetPath(_T("C:\\Log\\"), _T("Error"));
	m_Log_ErrLog.SetLogFileName_Prefix(_T("Err"));


	// 	m_logFile.SetDefPath(_T("C:\\Log\\"));
	// 	m_logFile.SetSubDirName(_T("Inspector"));
	// 
	// 	m_Log_ErrLog.SetDefPath(_T("C:\\Log\\"));
	// 	m_Log_ErrLog.SetSubDirName(_T("Error"));
	// 	m_Log_ErrLog.SetLogName(_T("Err_"));
}

//=============================================================================
// Method		: OnLog
// Access		: virtual protected  
// Returns		: void
// Parameter	: __in LPCTSTR szLog
// Parameter	: ...
// Qualifier	:
// Last Update	: 2016/9/23 - 17:27
// Desc.		:
//=============================================================================
void CTestManager_Base::OnLog(__in LPCTSTR szLog, ...)
{
	__try
	{
		//TCHAR *lpszBuffer = new TCHAR[4096];
		TCHAR szBuffer[4096] = { 0, };

		size_t cb = 0;
		va_list args;
		va_start(args, szLog);
		::StringCchVPrintfEx(szBuffer, 4096, NULL, &cb, 0, szLog, args);
		va_end(args);

		OnAdd_Log(szBuffer, FALSE);

		//delete[] lpszBuffer;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		TRACE(_T("*** Exception Error : CTestManager_Base::OnLog()\n"));
	}
}

void CTestManager_Base::OnLog(__in UINT nLogType, __in LPCTSTR szLog, ...)
{
	try
	{
		TCHAR szBuffer[4096] = { 0, };

		size_t cb = 0;
		va_list args;
		va_start(args, szLog);
		::StringCchVPrintfEx(szBuffer, 4096, NULL, &cb, 0, szLog, args);
		va_end(args);

		OnAdd_Log(szBuffer, nLogType);
	}
	catch (...)
	{
		TRACE(_T("*** Exception Error : CTestManager_Base::OnLog()\n"));
	}
}

//=============================================================================
// Method		: OnLog_Err
// Access		: virtual protected  
// Returns		: void
// Parameter	: __in LPCTSTR szLog
// Parameter	: ...
// Qualifier	:
// Last Update	: 2016/9/23 - 17:27
// Desc.		:
//=============================================================================
void CTestManager_Base::OnLog_Err(__in LPCTSTR szLog, ...)
{
	__try
	{
		TCHAR *lpszBuffer = new TCHAR[4096];

		size_t cb = 0;
		va_list args;
		va_start(args, szLog);
		::StringCchVPrintfEx(lpszBuffer, 4096, NULL, &cb, 0, szLog, args);
		va_end(args);

		OnAdd_Log(lpszBuffer, LOGTYPE_ERROR);
		//TRACE(_T("%s\n"), lpszBuffer);

		delete[] lpszBuffer;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		TRACE(_T("*** Exception Error : CTestManager_Base::OnLog_Err()\n"));
	}
}

//=============================================================================
// Method		: CreateSplashScreen
// Access		: protected  
// Returns		: void
// Parameter	: __in CWnd * pWndParent
// Parameter	: __in UINT nBitmapID
// Qualifier	:
// Last Update	: 2016/9/23 - 17:38
// Desc.		:
//=============================================================================
void CTestManager_Base::CreateSplashScreen(__in CWnd* pWndParent, __in UINT nBitmapID)
{
	m_wndSplash.Create(pWndParent, _T("CSplashScreenEx dynamic text:"), 0, CSS_FADE | CSS_CENTERSCREEN | CSS_SHADOW);
	m_wndSplash.SetBitmap(nBitmapID, 255, 255, 255);
	m_wndSplash.SetTextFont(_T("Arial"), 160, CSS_TEXT_BOLD);
	m_wndSplash.SetTextRect(CRect(400, 0, 850, 100));
	m_wndSplash.SetTextColor(RGB(0, 0, 0));
	m_wndSplash.SetTextFormat(DT_SINGLELINE | DT_LEFT | DT_VCENTER);
}

//=============================================================================
// Method		: DeleteSplashScreen
// Access		: virtual protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2016/9/23 - 17:42
// Desc.		:
//=============================================================================
void CTestManager_Base::DeleteSplashScreen()
{
	m_wndSplash.DestroyWindow();
}

//=============================================================================
// Method		: ShowWindow_SplashScreen
// Access		: protected  
// Returns		: void
// Parameter	: __in BOOL bShow
// Qualifier	:
// Last Update	: 2016/9/23 - 17:42
// Desc.		:
//=============================================================================
void CTestManager_Base::ShowWindow_SplashScreen(__in BOOL bShow /*= TRUE*/)
{	
	if (bShow)
	{
		m_wndSplash.CenterWindow();
		m_wndSplash.Show();
		//GetParent()->EnableWindow(FALSE);
		AfxGetApp()->GetMainWnd()->EnableWindow(FALSE);
	}
	else
	{
		//GetParent()->EnableWindow(TRUE);
		AfxGetApp()->GetMainWnd()->EnableWindow(TRUE);
		m_wndSplash.ShowWindow(SW_HIDE);
	}
}

//=============================================================================
// Method		: OnShow_SplashScreen
// Access		: virtual protected  
// Returns		: void
// Parameter	: __in BOOL bShow
// Parameter	: __in LPCTSTR szText
// Qualifier	:
// Last Update	: 2016/9/23 - 19:36
// Desc.		:
//=============================================================================
void CTestManager_Base::OnShow_SplashScreen(__in BOOL bShow, __in LPCTSTR szText /*= NULL*/)
{
	// #ifdef _DEBUG
	// 	return;
	// #endif

	if (bShow)
	{
		if (FALSE == m_wndSplash.IsWindowVisible())
		{
			ShowWindow_SplashScreen();
		}

		if (NULL != szText)
			m_wndSplash.SetText(szText);
	}
	else
	{
		ShowWindow_SplashScreen(FALSE);
	}
}

//=============================================================================
// Method		: TimerRoutine_TimeCheck
// Access		: protected static  
// Returns		: VOID CALLBACK
// Parameter	: __in PVOID lpParam
// Parameter	: __in BOOLEAN TimerOrWaitFired
// Qualifier	:
// Last Update	: 2016/5/31 - 22:10
// Desc.		:
//=============================================================================
VOID CALLBACK CTestManager_Base::TimerRoutine_TimeCheck(__in PVOID lpParam, __in BOOLEAN TimerOrWaitFired)
{
	CTestManager_Base* pThis = (CTestManager_Base*)lpParam;

	pThis->OnMonitor_TimeCheck();
}

//=============================================================================
// Method		: TimerRoutine_UpdateUI
// Access		: protected static  
// Returns		: VOID CALLBACK
// Parameter	: __in PVOID lpParam
// Parameter	: __in BOOLEAN TimerOrWaitFired
// Qualifier	:
// Last Update	: 2016/5/31 - 22:10
// Desc.		:
//=============================================================================
VOID CALLBACK CTestManager_Base::TimerRoutine_UpdateUI(__in PVOID lpParam, __in BOOLEAN TimerOrWaitFired)
{
	CTestManager_Base* pThis = (CTestManager_Base*)lpParam;

	pThis->OnMonitor_UpdateUI();
}

//=============================================================================
// Method		: CreateTimerQueue_Mon
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2016/5/31 - 22:10
// Desc.		:
//=============================================================================
void CTestManager_Base::CreateTimerQueue_Mon()
{
	__try
	{
		// Create the timer queue.
		m_hTimerQueue = CreateTimerQueue();
		if (NULL == m_hTimerQueue)
		{
			TRACE(_T("CreateTimerQueue failed (%d)\n"), GetLastError());
			return;
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		TRACE(_T("*** Exception Error : CreateTimerQueue_Mon ()"));
	}
}

//=============================================================================
// Method		: DeleteTimerQueue_Mon
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2016/5/31 - 22:10
// Desc.		:
//=============================================================================
void CTestManager_Base::DeleteTimerQueue_Mon()
{
	__try
	{
		//OnDeleteTimer_TimeCheck();
		//OnDeleteTimer_UpdateUI();

		if (!DeleteTimerQueue(m_hTimerQueue))
			TRACE(_T("DeleteTimerQueue failed (%d)\n"), GetLastError());
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		TRACE(_T("*** Exception Error : CTestManager_Base::DeleteTimerQueue_Mon()\n"));
	}

	TRACE(_T("Ended Timer : CTestManager_Base::DeleteTimerQueue_Mon()\n"));
}

//=============================================================================
// Method		: OnCreateTimer_TimeCheck
// Access		: virtual protected  
// Returns		: void
// Parameter	: __in DWORD DueTime
// Parameter	: __in DWORD Period
// Qualifier	:
// Last Update	: 2017/8/24 - 19:56
// Desc.		:
//=============================================================================
void CTestManager_Base::OnCreateTimer_TimeCheck(__in DWORD DueTime /*= 2000*/, __in DWORD Period /*= 153*/)
{
	__try
	{
		// Time Check Timer
		if (NULL == m_hTimer_TimeCheck)
			if (!CreateTimerQueueTimer(&m_hTimer_TimeCheck, m_hTimerQueue, (WAITORTIMERCALLBACK)TimerRoutine_TimeCheck, (PVOID)this, DueTime, Period, WT_EXECUTEDEFAULT))
			{
				TRACE(_T("CreateTimerQueueTimer failed (%d)\n"), GetLastError());
			}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		TRACE(_T("*** Exception Error : CTestManager_Base::OnCreateTimer_TimeCheck()\n"));
	}
}

//=============================================================================
// Method		: OnDeleteTimer_TimeCheck
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2016/5/31 - 22:10
// Desc.		:
//=============================================================================
void CTestManager_Base::OnDeleteTimer_TimeCheck()
{
	__try
	{
		if (NULL != m_hTimer_TimeCheck)
		{
			if (DeleteTimerQueueTimer(m_hTimerQueue, m_hTimer_TimeCheck, NULL))
			{
				m_hTimer_TimeCheck = NULL;
			}
			else
			{
				TRACE(_T("DeleteTimerQueueTimer : m_hTimer_TimeCheck_Mon failed (%d)\n"), GetLastError());
			}
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		TRACE(_T("*** Exception Error : CTestManager_Base::OnDeleteTimer_TimeCheck()\n"));
	}

	TRACE(_T("Ended Timer : CTestManager_Base::OnDeleteTimer_TimeCheck()\n"));
}

//=============================================================================
// Method		: OnCreateTimer_UpdateUI
// Access		: virtual protected  
// Returns		: void
// Parameter	: __in DWORD DueTime
// Parameter	: __in DWORD Period
// Qualifier	:
// Last Update	: 2017/8/24 - 19:57
// Desc.		:
//=============================================================================
void CTestManager_Base::OnCreateTimer_UpdateUI(__in DWORD DueTime /*= 5000*/, __in DWORD Period /*= 250*/)
{
	__try
	{
		if (NULL == m_hTimer_Update_UI)
			if (!CreateTimerQueueTimer(&m_hTimer_Update_UI, m_hTimerQueue, (WAITORTIMERCALLBACK)TimerRoutine_UpdateUI, (PVOID)this, DueTime, Period, WT_EXECUTEDEFAULT))
			{
				TRACE(_T("CreateTimerQueueTimer failed (%d)\n"), GetLastError());
			}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		TRACE(_T("*** Exception Error : CTestManager_Base::OnCreateTimer_UpdateUI()\n"));
	}
}

//=============================================================================
// Method		: OnDeleteTimer_UpdateUI
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2016/5/31 - 22:10
// Desc.		:
//=============================================================================
void CTestManager_Base::OnDeleteTimer_UpdateUI()
{
	__try
	{
		if (NULL != m_hTimer_Update_UI)
		{
			if (DeleteTimerQueueTimer(m_hTimerQueue, m_hTimer_Update_UI, NULL))
			{
				m_hTimer_Update_UI = NULL;
			}
			else
			{
				TRACE(_T("DeleteTimerQueueTimer : m_hTimer_Update_UI failed (%d)\n"), GetLastError());
			}
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		TRACE(_T("*** Exception Error : CTestManager_Base::OnDeleteTimer_UpdateUI()\n"));
	}

	TRACE(_T("타이머 종료 : CTestManager_Base::OnDeleteTimer_UpdateUI()\n"));
}

//=============================================================================
// Method		: OnMonitor_TimeCheck
// Access		: virtual protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2016/5/31 - 22:10
// Desc.		:
//=============================================================================
void CTestManager_Base::OnMonitor_TimeCheck()
{

}

//=============================================================================
// Method		: OnMonitor_UpdateUI
// Access		: virtual protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2016/5/31 - 22:10
// Desc.		:
//=============================================================================
void CTestManager_Base::OnMonitor_UpdateUI()
{
	//  if (WAIT_OBJECT_0 != LockSemaphore())
	//  	return;
	//	UnlockSemaphore();
}


//=============================================================================
// Method		: OnSet_SystemType
// Access		: virtual public  
// Returns		: void
// Parameter	: __in enInsptrSysType nSysType
// Qualifier	:
// Last Update	: 2017/10/11 - 15:52
// Desc.		:
//=============================================================================
void CTestManager_Base::OnSet_SystemType(__in enInsptrSysType nSysType)
{
	m_InspectionType = nSysType;
}

//=============================================================================
// Method		: OnInitialize
// Access		: virtual public  
// Returns		: void
// Qualifier	:
// Last Update	: 2016/5/31 - 22:10
// Desc.		:
//=============================================================================
void CTestManager_Base::OnInitialize()
{
	m_hEvent_ProgramExit	= CreateEvent(NULL, TRUE, FALSE, NULL/*_T("Exit Program Event")*/);
	m_hEvent_ForcedStop		= CreateEvent(NULL, TRUE, FALSE, NULL/*_T("Forced Stop Event")*/);

	m_hSemaphore = CreateSemaphore(NULL, 1, 1, _T("Test Manager Base"));	

	CreateTimerQueue_Mon();	

	OnInit_LogFolder();
}

//=============================================================================
// Method		: OnFinalize
// Access		: virtual public  
// Returns		: void
// Qualifier	:
// Last Update	: 2016/5/31 - 22:11
// Desc.		:
//=============================================================================
void CTestManager_Base::OnFinalize()
{
	DeleteTimerQueue_Mon();

	m_bExitFlag = TRUE;

	if (FALSE == SetEvent(m_hEvent_ProgramExit))
	{
		TRACE(_T("Set Exit Program External Event 실패!!\n"));
	}	

	if (NULL != m_hSemaphore)
		CloseHandle(m_hSemaphore);

	if (NULL != m_hEvent_ForcedStop)
		CloseHandle(m_hEvent_ForcedStop);

	if (NULL != m_hEvent_ProgramExit)
		CloseHandle(m_hEvent_ProgramExit);
}

//=============================================================================
// Method		: OnAdd_Log
// Access		: virtual public  
// Returns		: void
// Parameter	: __in LPCTSTR lpszLog
// Parameter	: __in UINT nLogType
// Qualifier	:
// Last Update	: 2021/6/3 - 17:18
// Desc.		:
//=============================================================================
void CTestManager_Base::OnAdd_Log(__in LPCTSTR lpszLog, __in UINT nLogType /*= LOGTYPE_NONE*/)
{
	if (NULL == lpszLog)
		return;

	try
	{
		TCHAR		strTime[255] = { 0 };
		UINT_PTR	nLogSize = _tcslen(lpszLog) + 255;
		LPTSTR		lpszOutLog = new TCHAR[nLogSize];
		SYSTEMTIME	LocalTime;

		// **** 시간 추가 ****
		GetLocalTime(&LocalTime);
		StringCbPrintf(strTime, sizeof(strTime), _T("[%02d:%02d:%02d] "), LocalTime.wHour, LocalTime.wMinute, LocalTime.wSecond);

		StringCbPrintf(lpszOutLog, nLogSize, _T("%s%s \r\n"), strTime, lpszLog);

		if (LOGTYPE_ERROR == nLogType)
		{
			m_Log_ErrLog.LogWrite(lpszOutLog);
		}

		m_logFile.LogWrite(lpszOutLog);

		delete[] lpszOutLog;
	}
	catch (...)
	{
	}
}

//=============================================================================
// Method		: OnAdd_Log
// Access		: virtual public  
// Returns		: void
// Parameter	: __in LPCTSTR lpszLog
// Parameter	: __in BOOL bError
// Parameter	: __in UINT nLogType
// Parameter	: __in BOOL bOnlyLogType
// Qualifier	:
// Last Update	: 2016/9/23 - 19:36
// Desc.		:
//=============================================================================
// void CTestManager_Base::OnAdd_Log(__in LPCTSTR lpszLog, __in BOOL bError /*= FALSE*/, __in UINT nLogType /*= LOGTYPE_NORMAL*/, __in BOOL bOnlyLogType /*= FALSE*/)
// {
// 	if (NULL == lpszLog)
// 		return;
// 
// 	__try
// 	{
// 		TCHAR		strTime[255] = { 0 };
// 		UINT_PTR	nLogSize = _tcslen(lpszLog) + 255;
// 		LPTSTR		lpszOutLog = new TCHAR[nLogSize];
// 		SYSTEMTIME	LocalTime;
// 
// 		// **** 시간 추가 ****
// 		GetLocalTime(&LocalTime);
// 		StringCbPrintf(strTime, sizeof(strTime), _T("[%02d:%02d:%02d] "), LocalTime.wHour, LocalTime.wMinute, LocalTime.wSecond);
// 
// 		StringCbPrintf(lpszOutLog, nLogSize, _T("%s%s \r\n"), strTime, lpszLog);
// 
// 		if (bError)
// 			m_Log_ErrLog.LogWrite(lpszOutLog);
// 
// 		m_logFile.LogWrite(lpszOutLog);
// 
// 		// UI 처리 --------------------------------------------------
// 		OnSetLog_UI(lpszOutLog, bError, nLogType, RGB(0, 0, 0));
// 
// 		delete[] lpszOutLog;
// 	}
// 	__except (EXCEPTION_EXECUTE_HANDLER)
// 	{
// 		TRACE(_T("*** Exception Error : CTestManager_Base::OnAdd_Log () \n"));
// 	}
// }

//=============================================================================
// Method		: OnAdd_Log_F
// Access		: virtual public  
// Returns		: void
// Parameter	: LPCTSTR lpszLog
// Parameter	: ...
// Qualifier	:
// Last Update	: 2016/9/23 - 19:36
// Desc.		:
//=============================================================================
void CTestManager_Base::OnAdd_Log_F(LPCTSTR lpszLog, ...)
{
	__try
	{
		TCHAR *lpszBuffer = new TCHAR[4096];

		size_t cb = 0;
		va_list args;
		va_start(args, lpszLog);
		::StringCchVPrintfEx(lpszBuffer, 4096, NULL, &cb, 0, lpszLog, args);
		va_end(args);

		OnAdd_Log(lpszBuffer, FALSE);

		delete[] lpszBuffer;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		TRACE(_T("*** Exception Error : CTestManager_Base::OnAdd_Log()\n"));
	}
}