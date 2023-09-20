//*****************************************************************************
// Filename	: 	TestManager_EQP.cpp
// Created	:	2016/5/9 - 13:32
// Modified	:	2016/08/10
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
#include "stdafx.h"
#include "TestManager_EQP.h"
#include "CommonFunction.h"
#include "Def_Test.h"
#include <strsafe.h>

#include <Mmsystem.h>
#pragma comment (lib,"winmm.lib")

CTestManager_EQP::CTestManager_EQP()
{
	OnInitialize();
}

CTestManager_EQP::~CTestManager_EQP()
{
	TRACE(_T("<<< Start ~CTestManager_EQP >>> \n"));

	this->OnFinalize();

	TRACE(_T("<<< End ~CTestManager_EQP >>> \n"));
}

//=============================================================================
// Method		: OnLoadOption
// Access		: virtual protected  
// Returns		: BOOL
// Qualifier	:
// Last Update	: 2016/9/28 - 20:04
// Desc.		:
//=============================================================================
BOOL CTestManager_EQP::OnLoad_Option()
{
	BOOL bReturn = __super::OnLoad_Option();

	if (!m_stOption.Inspector.szPath_Log.IsEmpty())
		m_stInspInfo.Path.szLog = m_stOption.Inspector.szPath_Log + _T("\\");
	
	if (!m_stOption.Inspector.szPath_Report.IsEmpty())
		m_stInspInfo.Path.szReport = m_stOption.Inspector.szPath_Report + _T("\\");
	
	if (!m_stOption.Inspector.szPath_Recipe.IsEmpty())
		m_stInspInfo.Path.szRecipe = m_stOption.Inspector.szPath_Recipe + _T("\\");
 	
	if (!m_stOption.Inspector.szPath_Socket.IsEmpty())
 		m_stInspInfo.Path.szSocket = m_stOption.Inspector.szPath_Socket + _T("\\");
	
	if (!m_stOption.Inspector.szPath_Shared.IsEmpty())
		m_stInspInfo.Path.szShared = m_stOption.Inspector.szPath_Shared + _T("\\");
	
	if (!m_stOption.Inspector.szPath_FailInfo.IsEmpty())
		m_stInspInfo.Path.szFailInfo = m_stOption.Inspector.szPath_FailInfo + _T("\\");


	return bReturn;
}

//=============================================================================
// Method		: OnVerify_RecipeInfo
// Access		: virtual protected  
// Returns		: LRESULT
// Qualifier	:
// Last Update	: 2022/7/18 - 15:28
// Desc.		:
//=============================================================================
LRESULT CTestManager_EQP::OnVerify_RecipeInfo()
{
	if (Get_SettingInfo().szRecipe_FullPath.IsEmpty())
	{
		//OnLog_Err(_T("모델 설정이 되지 않았습니다."));
		//LT_MessageBox(_T("모델 설정이 되지 않았습니다."), MB_SYSTEMMODAL);
		//LT_MessageBox(g_szMessageBox_T[MB_Line_NoConfig][m_nLanguage]);

		return RC_Recipe_Err;
	}

	return RC_OK;
}

//=============================================================================
// Method		: OnLog_EquipmentAlarm
// Access		: virtual protected  
// Returns		: void
// Parameter	: __in uint8_t IN_nEqpOder
// Parameter	: __in const ST_AlarmStatus * IN_pAlarm
// Qualifier	:
// Last Update	: 2022/2/18 - 15:27
// Desc.		:
//=============================================================================
void CTestManager_EQP::OnLog_EquipmentAlarm(__in uint8_t IN_nEqpOder, __in const ST_AlarmStatus* IN_pAlarm)
{
	CString szLog;
	szLog.Format(_T("eqp_%02d(%s) code: %d, info: %s"), IN_nEqpOder, Get_EquipmentID(IN_nEqpOder), IN_pAlarm->m_nAlarmCode, IN_pAlarm->m_szAlarmInfo);

	m_log_EqpAlarm.LogWrite(szLog.GetBuffer());
}

//=============================================================================
// Method		: OnLog_SocketEvent
// Access		: virtual protected  
// Returns		: void
// Parameter	: __in LPCTSTR IN_szRFID
// Parameter	: __in LPCTSTR IN_szLog
// Qualifier	:
// Last Update	: 2022/2/18 - 15:30
// Desc.		:
//=============================================================================
void CTestManager_EQP::OnLog_SocketEvent(__in LPCTSTR IN_szRFID, __in LPCTSTR IN_szLog)
{
	m_log_Socket.LogWrite(IN_szLog, IN_szRFID);
}

void CTestManager_EQP::OnLog_SocketEvent(__in LPCTSTR IN_szRFID, __in LPCTSTR IN_szLog, ...)
{
	__try
	{
		TCHAR szBuffer[4096] = { 0, };

		size_t cb = 0;
		va_list args;
		va_start(args, IN_szLog);
		::StringCchVPrintfEx(szBuffer, 4096, NULL, &cb, 0, IN_szLog, args);
		va_end(args);

		m_log_Socket.LogWrite(szBuffer, IN_szRFID);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		TRACE(_T("*** Exception Error : %s\n"), _T(__FUNCTION__));
	}
}

//=============================================================================
// Method		: OnLog_SocketTestResult
// Access		: virtual protected  
// Returns		: void
// Parameter	: __in uint8_t IN_FromEqp
// Parameter	: __in LPCTSTR IN_szRFID
// Qualifier	:
// Last Update	: 2022/2/18 - 16:57
// Desc.		:
//=============================================================================
void CTestManager_EQP::OnLog_SocketTestResult(__in uint8_t IN_FromEqp, __in LPCTSTR IN_szRFID)
{
	OnLog(_T("[eqp %02d] Test Result => rfid: %s, ng code: %d, eqp type: %s, eqp id: %s, para: %d"), 
		IN_FromEqp, 
		IN_szRFID,
		Get_SocketTestResult(IN_szRFID).m_nNG_Code,	
		g_szEqpTypeName[Get_SocketTestResult(IN_szRFID).m_nNG_EqpType],
		Get_SocketTestResult(IN_szRFID).m_szNG_EqpID,
		Get_SocketTestResult(IN_szRFID).m_nNG_Para);

	OnLog_SocketEvent(IN_szRFID, _T("[eqp %02d] Test Result => rfid: %s, ng code: %d, eqp type: %s, eqp id: %s, para: %d"),
		IN_FromEqp,
		IN_szRFID,
		Get_SocketTestResult(IN_szRFID).m_nNG_Code,
		g_szEqpTypeName[Get_SocketTestResult(IN_szRFID).m_nNG_EqpType],
		Get_SocketTestResult(IN_szRFID).m_szNG_EqpID,
		Get_SocketTestResult(IN_szRFID).m_nNG_Para);

	// 불량이면, log를 따로 남긴다.
	if ((0 != Get_SocketTestResult(IN_szRFID).m_nNG_Code) &&
		(Get_MES_ReworkCode() != Get_SocketTestResult(IN_szRFID).m_nNG_Code))
	{
		auto NG_Code = Get_Socket(IN_szRFID).m_stTestResult.m_nNG_Code;
		auto NG_Para = Get_Socket(IN_szRFID).m_stTestResult.m_nNG_Para;

		if (Get_FailInfo().m_EqpList.at(IN_FromEqp).IsExist(NG_Code))
		{
			OnLog_NG_History(_T("[Eqp %02d] Eqp id: %s, ng code: %d, count: %d, ng para: %s, para_count: %d, rfid: %s, barcode: %s"),
				IN_FromEqp,
				Get_EquipmentID(IN_FromEqp),
				NG_Code,
				Get_FailInfo().m_EqpList.at(IN_FromEqp).Get_NGCount(NG_Code).m_nCount_All,
				g_szEqpPara_Mini[NG_Para],
				Get_FailInfo().m_EqpList.at(IN_FromEqp).Get_NGCount(NG_Code).m_nCount_Para[NG_Para],
				IN_szRFID,
				Get_Socket(IN_szRFID).m_szBarcode
			);
		}
	}
}

//=============================================================================
// Method		: OnLog_NG_History
// Access		: virtual protected  
// Returns		: void
// Parameter	: __in LPCTSTR IN_szLog
// Parameter	: ...
// Qualifier	:
// Last Update	: 2022/7/18 - 16:00
// Desc.		:
//=============================================================================
void CTestManager_EQP::OnLog_NG_History(__in LPCTSTR IN_szLog, ...)
{
	__try
	{
		TCHAR szBuffer[4096] = { 0, };

		size_t cb = 0;
		va_list args;
		va_start(args, IN_szLog);
		::StringCchVPrintfEx(szBuffer, 4096, NULL, &cb, 0, IN_szLog, args);
		va_end(args);

		m_log_NG_History.LogWrite(szBuffer);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		TRACE(_T("*** Exception Error : %s\n"), _T(__FUNCTION__));
	}
}

//=============================================================================
// Method		: OnLogFile_SocketResult
// Access		: protected  
// Returns		: bool
// Parameter	: __in LPCTSTR IN_szRFID
// Parameter	: __in LPCTSTR IN_szPath
// Qualifier	:
// Last Update	: 2022/2/18 - 17:56
// Desc.		:
//=============================================================================
bool CTestManager_EQP::OnLogFile_SocketResult(__in LPCTSTR IN_szRFID, __in LPCTSTR IN_szPath)
{
	ST_TestResult* pRe = &Get_Socket(IN_szRFID).m_stTestResult;

	CString szUnicode, szIn, szOut;
	CStringA szANSI;
	szIn.Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), Get_Socket(IN_szRFID).m_tmLoad.wYear, Get_Socket(IN_szRFID).m_tmLoad.wMonth, Get_Socket(IN_szRFID).m_tmLoad.wDay,
													Get_Socket(IN_szRFID).m_tmLoad.wHour, Get_Socket(IN_szRFID).m_tmLoad.wMinute, Get_Socket(IN_szRFID).m_tmLoad.wSecond);

	szOut.Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), Get_Socket(IN_szRFID).m_tmUnload.wYear, Get_Socket(IN_szRFID).m_tmUnload.wMonth, Get_Socket(IN_szRFID).m_tmUnload.wDay,
													Get_Socket(IN_szRFID).m_tmUnload.wHour, Get_Socket(IN_szRFID).m_tmUnload.wMinute, Get_Socket(IN_szRFID).m_tmUnload.wSecond);


	szUnicode.Format(_T(",%s,%s,%s,%s,%d,%s,%d,%s,%s,%.1f,%.1f\r\n"),
									szIn.GetBuffer(), 
									szOut.GetBuffer(),
									IN_szRFID,
									Get_Socket(IN_szRFID).m_szBarcode,
									pRe->m_nNG_Code,
									g_szEqpTypeName[Get_SocketTestResult(IN_szRFID).m_nNG_EqpType],
									pRe->m_nNG_EqpOrder,
									pRe->m_szNG_EqpID,
									g_szEqpPara_Mini[pRe->m_nNG_Para],
									Get_Socket(IN_szRFID).Get_CycleTime(),
									Get_Socket(IN_szRFID).Get_Tacttime()
									);

	CFile File;
	CFileException e;

	if (!PathFileExists(IN_szPath))
	{
		if (!File.Open(IN_szPath, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite, &e))
			return false;

		// 헤더 추가
		// , Load Time, Unload Time, RFID, Serial Number, NG Code, Eqp Type, Eqp No, Eqp ID, Eqp Para, Cycle Time, Tacttime
		CString szHeader = _T(",Load Time,Unload Time,RFID,Serial Number,NG Code,Eqp Type,Eqp No,Eqp ID,Eqp Para,Cycle Time,Tacttime\r\n");

		szUnicode = szHeader + szUnicode;
	}
	else
	{
		if (!File.Open(IN_szPath, CFile::modeWrite | CFile::shareDenyWrite, &e))
			return false;
	}

	USES_CONVERSION;
	szANSI = CT2A(szUnicode.GetBuffer());

	File.SeekToEnd();
	File.Write(szANSI.GetBuffer(0), szANSI.GetLength());
	File.Flush();
	File.Close();

	return true;
}

//=============================================================================
// Method		: OnMonitor_TimeCheck
// Access		: virtual protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2016/5/30 - 13:28
// Desc.		:
//=============================================================================
void CTestManager_EQP::OnMonitor_TimeCheck()
{
	m_dwTimeCheck = timeGetTime();

	// 개별 채널 검사 시간 체크
//	for (UINT nEquipmentIdx; nEquipmentIdx < USE_CHANNEL_CNT; nEquipmentIdx++)
//	{
// 		if (TP_Testing == m_stInspInfo.ProductInfo[nEquipmentIdx].nProgressStatus)
// 		{
// 			if (m_stInspInfo.Time.dwStart[nEquipmentIdx] < m_dwTimeCheck)
// 				m_stInspInfo.Time.dwDuration[nEquipmentIdx] = m_dwTimeCheck - m_stInspInfo.Time.dwStart[nEquipmentIdx];
// 			else
// 				m_stInspInfo.Time.dwDuration[nEquipmentIdx] = 0xFFFFFFFF - m_stInspInfo.Time.dwStart[nEquipmentIdx] + m_dwTimeCheck;
// 
// 			OnUpdateElapsedTime_Unit(nEquipmentIdx);
// 		}
//	}

	//OnUpdateElapsedTime_All();
}

//=============================================================================
// Method		: OnMonitor_UpdateUI
// Access		: virtual protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2016/5/30 - 21:19
// Desc.		: Digital In 신호 체크
//=============================================================================
void CTestManager_EQP::OnMonitor_UpdateUI()
{

}

//=============================================================================
// Method		: OnSetStatus_Production
// Access		: virtual protected  
// Returns		: void
// Parameter	: __in bool IN_bEnd
// Qualifier	:
// Last Update	: 2022/2/21 - 17:40
// Desc.		:
//=============================================================================
void CTestManager_EQP::OnSetStatus_Production(__in bool IN_bEnd)
{
	Get_LineInfo().Set_EndOfProduction(IN_bEnd);
}

//=============================================================================
// Method		: OnUpdate_ElapsedTime_Unit
// Access		: virtual protected  
// Returns		: void
// Parameter	: __in UINT nEquipmentIdx
// Parameter	: __in uint8_t IN_nPara
// Qualifier	:
// Last Update	: 2021/12/30 - 17:44
// Desc.		:
//=============================================================================
void CTestManager_EQP::OnUpdate_ElapsedTime_Unit(__in UINT nEquipmentIdx, __in uint8_t IN_nPara)
{

}

//=============================================================================
// Method		: OnInitialize
// Access		: virtual public  
// Returns		: void
// Qualifier	:
// Last Update	: 2020/8/4 - 14:37
// Desc.		:
//=============================================================================
void CTestManager_EQP::OnInitialize()
{
	//__super::OnInitialize();	

#if (SET_INSPECTOR == SYS_ICS_RIVIAN_LINE)
	// 설비들의 소켓 유/무 체크하여 라인의 가동/비가동 상태 설정 (1초마다 체크)
	OnCreateTimer_TimeCheck(10000, 1000);
#endif

	// Production End 체크 (10초마다 체크)
	OnCreateTimer_UpdateUI(30000, 10000);
}

//=============================================================================
// Method		: OnFinalize
// Access		: virtual public  
// Returns		: void
// Qualifier	:
// Last Update	: 2020/8/4 - 14:37
// Desc.		:
//=============================================================================
void CTestManager_EQP::OnFinalize()
{
	//__super::OnFinalize();

	//OnDeleteTimer_TimeCheck();
	OnDeleteTimer_UpdateUI();
}

//=============================================================================
// Method		: IsManagerMode
// Access		: public  
// Returns		: BOOL
// Qualifier	:
// Last Update	: 2016/1/13 - 10:32
// Desc.		:
//=============================================================================
BOOL CTestManager_EQP::IsManagerMode()
{
	if (Permission_Engineer == m_stInspInfo.PermissionMode)
		return TRUE;
	else
		return FALSE;
}

//=============================================================================
// Method		: GetPermissionMode
// Access		: public  
// Returns		: enPermissionMode
// Qualifier	:
// Last Update	: 2016/11/9 - 18:52
// Desc.		:
//=============================================================================
enPermissionMode CTestManager_EQP::GetPermissionMode()
{
	return m_stInspInfo.PermissionMode;
}

//=============================================================================
// Method		: OnSet_PermissionMode
// Access		: public  
// Returns		: void
// Parameter	: __in enPermissionMode nAcessMode
// Qualifier	:
// Last Update	: 2016/11/9 - 18:52
// Desc.		:
//=============================================================================
void CTestManager_EQP::OnSet_PermissionMode(__in enPermissionMode nAcessMode)
{
	m_stInspInfo.PermissionMode = nAcessMode;
}
