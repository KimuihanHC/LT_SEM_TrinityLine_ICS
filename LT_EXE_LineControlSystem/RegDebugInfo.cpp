//*****************************************************************************
// Filename	: 	RegDebugInfo.cpp
// Created	:	2021/12/21 - 13:46
// Modified	:	2021/12/21 - 13:46
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#include "stdafx.h"
#include "RegDebugInfo.h"
#include "Registry.h"
#include "Rework_Info.h"

CRegDebugInfo::CRegDebugInfo()
{
	m_szRegPath.Format(_T("%s\\DebugInfo"), REG_PATH_APP_BASE);
}

CRegDebugInfo::CRegDebugInfo(__in LPCTSTR lpszRegPath)
{
	m_szRegPath.Format(_T("%s\\DebugInfo"), lpszRegPath);
}

CRegDebugInfo::~CRegDebugInfo()
{

}

//=============================================================================
// Method		: Set_RegitryPath
// Access		: public  
// Returns		: void
// Parameter	: __in LPCTSTR lpszRegPath
// Qualifier	:
// Last Update	: 2022/1/10 - 18:10
// Desc.		:
//=============================================================================
void CRegDebugInfo::Set_RegitryPath(__in LPCTSTR lpszRegPath)
{
	m_szRegPath.Format(_T("%s\\DebugInfo"), lpszRegPath);
}

bool CRegDebugInfo::Set_DebugInfo(const ST_DebugInfo * IN_pDebugInfo)
{
	CRegistry	m_Reg;
	CString		szValue;

	if (!m_Reg.VerifyKey(HKEY_CURRENT_USER, m_szRegPath))
	{
		m_Reg.CreateKey(HKEY_CURRENT_USER, m_szRegPath);
	}

	if (m_Reg.Open(HKEY_CURRENT_USER, m_szRegPath))
	{
		// m_bUse_DebugMode;
		m_Reg.WriteDWORD(_T("Use_DebugMode"), IN_pDebugInfo->m_bUse_DebugMode ? 1 : 0);

		// m_bUse_NotBypass_NG;
		m_Reg.WriteDWORD(_T("Use_NotBypass_NG"), IN_pDebugInfo->m_bUse_NotBypass_NG ? 1 : 0);

		// m_bUse_NotBypass_TestSkip;
		m_Reg.WriteDWORD(_T("Use_NotBypass_TestSkip"), IN_pDebugInfo->m_bUse_NotBypass_TestSkip ? 1 : 0);

		// m_bUse_NotBypass_Empty;
		m_Reg.WriteDWORD(_T("Use_NotBypas_Empty"), IN_pDebugInfo->m_bUse_NotBypass_Empty ? 1 : 0);

		// m_AutoReset_Yield_byShift;
		m_Reg.WriteDWORD(_T("AutoReset_Yield_byShift"), IN_pDebugInfo->m_AutoReset_Yield_byShift ? 1 : 0);

		// m_nSocketTrackOut_Condition;
		m_Reg.WriteDWORD(_T("SocketTrackOut_Condition"), IN_pDebugInfo->m_nSocketTrackOut_Condition);

		// m_nWaitingSocket_Condition;
		m_Reg.WriteDWORD(_T("WaitingSocket_Condition"), IN_pDebugInfo->m_nWaitingSocket_Condition);

		// m_nMultiTester_SelectMethod;
		m_Reg.WriteDWORD(_T("MultiTester_SelectMethod"), IN_pDebugInfo->m_nMultiTester_SelectMethod);
	}
	else
	{
		return false;
	}

	m_Reg.Close();

	return true;
}

bool CRegDebugInfo::Get_DebugInfo(ST_DebugInfo & OUT_DebugInfo)
{
	CRegistry	m_Reg;
	CString		szValue;
	DWORD		dwValue = 0;

	if (m_Reg.Open(HKEY_CURRENT_USER, m_szRegPath))
	{
		// m_bUse_DebugMode;
		if (m_Reg.ReadDWORD(_T("Use_DebugMode"), dwValue))
			OUT_DebugInfo.m_bUse_DebugMode = dwValue ? true : false;
		else
			OUT_DebugInfo.m_bUse_DebugMode = false;

		// m_bUse_NotBypass_NG;
		if (m_Reg.ReadDWORD(_T("Use_NotBypass_NG"), dwValue))
			OUT_DebugInfo.m_bUse_NotBypass_NG = dwValue ? true : false;
		else
			OUT_DebugInfo.m_bUse_NotBypass_NG = false;

		// m_bUse_NotBypass_TestSkip;
		if (m_Reg.ReadDWORD(_T("Use_NotBypass_TestSkip"), dwValue))
			OUT_DebugInfo.m_bUse_NotBypass_TestSkip = dwValue ? true : false;
		else
			OUT_DebugInfo.m_bUse_NotBypass_TestSkip = false;

		// m_bUse_NotBypass_Empty;
		if (m_Reg.ReadDWORD(_T("Use_NotBypas_Empty"), dwValue))
			OUT_DebugInfo.m_bUse_NotBypass_Empty = dwValue ? true : false;
		else
			OUT_DebugInfo.m_bUse_NotBypass_Empty = false;

		// m_AutoReset_Yield_byShift;
		if (m_Reg.ReadDWORD(_T("AutoReset_Yield_byShift"), dwValue))
			OUT_DebugInfo.m_AutoReset_Yield_byShift = dwValue ? true : false;
		else
			OUT_DebugInfo.m_AutoReset_Yield_byShift = false;

		// m_nSocketTrackOut_Condition;
		if (m_Reg.ReadDWORD(_T("SocketTrackOut_Condition"), dwValue))
			OUT_DebugInfo.m_nSocketTrackOut_Condition = static_cast<uint8_t>(dwValue);
		else
			OUT_DebugInfo.m_nSocketTrackOut_Condition = 0;

		// m_nWaitingSocket_Condition;
		if (m_Reg.ReadDWORD(_T("WaitingSocket_Condition"), dwValue))
			OUT_DebugInfo.m_nWaitingSocket_Condition = static_cast<uint8_t>(dwValue);
		else
			OUT_DebugInfo.m_nWaitingSocket_Condition = 0;

		// m_nMultiTester_SelectMethod;
		if (m_Reg.ReadDWORD(_T("MultiTester_SelectMethod"), dwValue))
			OUT_DebugInfo.m_nMultiTester_SelectMethod = static_cast<uint8_t>(dwValue);
		else
			OUT_DebugInfo.m_nMultiTester_SelectMethod = 0;
	}
	else
	{
		return false;
	}

	m_Reg.Close();

	return true;
}
