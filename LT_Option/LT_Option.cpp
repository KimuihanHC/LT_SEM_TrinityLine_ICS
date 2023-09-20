//*****************************************************************************
// Filename	: LT_Option.cpp
// Created	: 2016/03/09
// Modified	: 2016/03/09
//
// Author	: PiRing
//	
// Purpose	: 
//*****************************************************************************
#include "stdafx.h"
#include "LT_Option.h"
#include "Define_OptionDescription.h"
#include "Registry.h"

#define		REG_PATH_OPTION_DEFALT	_T("Software\\Luritech\\Environment\\Option")

//=============================================================================
//
//=============================================================================
CLT_Option::CLT_Option()
{	
	m_strRegPath_Base = REG_PATH_OPTION_DEFALT;

	m_InsptrType	= enInsptrSysType::Sys_Ics_Trinity_Line;
}

//=============================================================================
//
//=============================================================================
CLT_Option::~CLT_Option()
{

}

//=============================================================================
// Method		: CLT_Option::SetRegistryPath
// Access		: public 
// Returns		: void
// Parameter	: LPCTSTR lpszRegPath
// Qualifier	:
// Last Update	: 2016/03/09
// Desc.		: 옵션이 저장될 레지스트리의 경로 설정
//=============================================================================
void CLT_Option::SetRegistryPath( LPCTSTR lpszRegPath )
{
	if (NULL == lpszRegPath)
		AfxMessageBox(_T("lpszRegPath is NULL"));

	m_strRegPath_Base = lpszRegPath;
}

//=============================================================================
// Method		: CLT_Option::SaveOption_Inspector
// Access		: public 
// Returns		: void
// Parameter	: stOpt_Insp stOption
// Qualifier	:
// Last Update	: 2016/03/09
// Desc.		:
//=============================================================================
void CLT_Option::SaveOption_Inspector( stOpt_Insp stOption )
{
	CRegistry	reg;
	CString		strRegPath = m_strRegPath_Base + _T("\\Inspector");
	CString		strValue;
	
	m_stOption.Inspector = stOption;

	if (!reg.VerifyKey(HKEY_CURRENT_USER, strRegPath))
	{
		reg.CreateKey(HKEY_CURRENT_USER, strRegPath);
	}

	if (reg.Open(HKEY_CURRENT_USER, strRegPath))
	{
		// 언어 설정
		reg.WriteDWORD(_T("Language"), (DWORD)m_stOption.Inspector.nLanguage);

		// Auto 모드에서  Manual 모드로 전환하는데 필요한 대기 시간 (Wait time required to switch from Auto mode to Manual mode.)
		reg.WriteDWORD(_T("Auto_to_Manual_WaitTime"), (DWORD)m_stOption.Inspector.nAutoModeDuration);

		// Auto 모드에서 설비의 통신 끊김 알람
		//reg.WriteDWORD(_T("Alarm_EqpDiscon_AutoMode"), (DWORD)m_stOption.Inspector.bAlarm_EqpDiscon_AutoMode);

		// Log 경로
		reg.WriteString(_T("Path_Log"), m_stOption.Inspector.szPath_Log);
		// Report 경로
		reg.WriteString(_T("Path_Report"), m_stOption.Inspector.szPath_Report);
		// Setting 경로
		reg.WriteString(_T("Path_Model"), m_stOption.Inspector.szPath_Recipe);
		// Socket 경로
		reg.WriteString(_T("Path_Socket"), m_stOption.Inspector.szPath_Socket);
		// NGCode 경로
		reg.WriteString(_T("Path_Shared"), m_stOption.Inspector.szPath_Shared);
		// FailInfo 경로
		reg.WriteString(_T("Path_FailInfo"), m_stOption.Inspector.szPath_FailInfo);
		
		// 자동 재 실행 사용 여부
		reg.WriteDWORD(OPT_KEY_USE_AUTO_RESTART, (DWORD)m_stOption.Inspector.UseAutoRestart);
	}

	reg.Close();
}

//=============================================================================
// Method		: CLT_Option::LoadOption_Inspector
// Access		: public 
// Returns		: BOOL
// Parameter	: stOpt_Insp & stOption
// Qualifier	:
// Last Update	: 2016/03/09
// Desc.		:
//=============================================================================
BOOL CLT_Option::LoadOption_Inspector(stOpt_Insp& stOption)
{
	CRegistry	reg;
	CString		strRegPath = m_strRegPath_Base + _T("\\Inspector");
	DWORD		dwValue		= 0;
	CString		strValue;

	if (reg.Open(HKEY_CURRENT_USER, strRegPath))
	{
		// 언어 설정
		if (reg.ReadDWORD(_T("Language"), dwValue))
			m_stOption.Inspector.nLanguage = (uint8_t)dwValue;

		// Auto 모드에서  Manual 모드로 전환하는데 필요한 대기 시간 (Wait time required to switch from Auto mode to Manual mode.)
		if (reg.ReadDWORD(_T("Auto_to_Manual_WaitTime"), dwValue))
			m_stOption.Inspector.nAutoModeDuration = (uint16_t)dwValue;

		// Auto 모드에서 설비의 통신 끊김 알람
// 		if (reg.ReadDWORD(_T("Alarm_EqpDiscon_AutoMode"), dwValue))
// 			m_stOption.Inspector.bAlarm_EqpDiscon_AutoMode = (uint8_t)dwValue;

		// Log 경로
		reg.ReadString(_T("Path_Log"), m_stOption.Inspector.szPath_Log);
		// Report 경로
		reg.ReadString(_T("Path_Report"), m_stOption.Inspector.szPath_Report);
		// Setting 경로
		reg.ReadString(_T("Path_Model"), m_stOption.Inspector.szPath_Recipe);
		// Socket JIG 경로
		reg.ReadString(_T("Path_Socket"), m_stOption.Inspector.szPath_Socket);
		// NGCode 경로
		reg.ReadString(_T("Path_Shared"), m_stOption.Inspector.szPath_Shared);
		// FailInfo 경로
		reg.ReadString(_T("Path_FailInfo"), m_stOption.Inspector.szPath_FailInfo);
		
		// 자동 재 실행 사용 여부
		if (reg.ReadDWORD(OPT_KEY_USE_AUTO_RESTART, dwValue))
			m_stOption.Inspector.UseAutoRestart = (BOOL)dwValue;
	}
	else
	{
		return FALSE;
	}

	reg.Close();

	stOption = m_stOption.Inspector;

	return TRUE;
}

//=============================================================================
// Method		: SaveOption_Server
// Access		: public  
// Returns		: void
// Parameter	: stOpt_Server stOption
// Qualifier	:
// Last Update	: 2016/5/18 - 15:50
// Desc.		:
//=============================================================================
void CLT_Option::SaveOption_Server(stOpt_Server stOption)
{
	CRegistry	reg;
	CString		strRegPath = m_strRegPath_Base + _T("\\Server");
	CString		strValue;

	m_stOption.Server = stOption;

	if (!reg.VerifyKey(HKEY_CURRENT_USER, strRegPath))
	{
		reg.CreateKey(HKEY_CURRENT_USER, strRegPath);
	}

	if (reg.Open(HKEY_CURRENT_USER, strRegPath))
	{
		// 설비 코드
		reg.WriteString(_T("EquipmentID"), m_stOption.Server.szEquipmentID);

		// 서버 주소
		reg.WriteDWORD(OPT_KEY_IP_ADDRESS, m_stOption.Server.Address.dwAddress);
		reg.WriteDWORD(OPT_KEY_IP_PORT, m_stOption.Server.Address.dwPort);

		// MES 경로
		reg.WriteString(_T("Path_Log"), m_stOption.Server.szPath_Log);
	}

	reg.Close();
}

//=============================================================================
// Method		: LoadOption_Server
// Access		: public  
// Returns		: BOOL
// Parameter	: stOpt_Server & stOption
// Qualifier	:
// Last Update	: 2016/5/18 - 15:50
// Desc.		:
//=============================================================================
BOOL CLT_Option::LoadOption_Server(stOpt_Server& stOption)
{
	CRegistry	reg;
	CString		strRegPath = m_strRegPath_Base + _T("\\Server");
	DWORD		dwValue = 0;
	CString		strValue;

	if (reg.Open(HKEY_CURRENT_USER, strRegPath))
	{
		// 설비 코드
		reg.ReadString(_T("EquipmentID"), m_stOption.Server.szEquipmentID);

		// 서버 주소
 		if (reg.ReadDWORD(OPT_KEY_IP_ADDRESS, dwValue))
 			m_stOption.Server.Address.dwAddress = dwValue;
 		if (reg.ReadDWORD(OPT_KEY_IP_PORT, dwValue))
 			m_stOption.Server.Address.dwPort = dwValue;

		// MES 경로
		if (FALSE == reg.ReadString(_T("Path_Log"), m_stOption.Server.szPath_Log))
		{
			m_stOption.Server.szPath_Log = _T("C:\\Server_Log\\");
		}
	}
	else
	{
		return FALSE;
	}

	reg.Close();

	stOption = m_stOption.Server;

	return TRUE;
}

//=============================================================================
// Method		: SaveOption_Misc
// Access		: public  
// Returns		: void
// Parameter	: stOpt_Misc stOption
// Qualifier	:
// Last Update	: 2016/9/26 - 16:12
// Desc.		:
//=============================================================================
void CLT_Option::SaveOption_Misc(stOpt_Misc stOption)
{
// 	CRegistry	reg;
// 	CString		strRegPath = m_strRegPath_Base + _T("\\Misc");
// 	CString		strValue;
// 
// 	m_stOption.Misc = stOption;
// 
// 	if (!reg.VerifyKey(HKEY_CURRENT_USER, strRegPath))
// 	{
// 		reg.CreateKey(HKEY_CURRENT_USER, strRegPath);
// 	}
// 
// 	CString szkey;
// 	if (reg.Open(HKEY_CURRENT_USER, strRegPath))
// 	{
// 
// 	}
// 
// 	reg.Close();
}

//=============================================================================
// Method		: LoadOption_Misc
// Access		: public  
// Returns		: BOOL
// Parameter	: stOpt_Misc & stOption
// Qualifier	:
// Last Update	: 2016/9/26 - 16:12
// Desc.		:
//=============================================================================
BOOL CLT_Option::LoadOption_Misc(stOpt_Misc& stOption)
{
// 	CRegistry	reg;
// 	CString		strRegPath = m_strRegPath_Base + _T("\\Misc");
// 	DWORD		dwValue = 0;
// 	CString		strValue;
// 
// 	CString szkey;
// 	if (reg.Open(HKEY_CURRENT_USER, strRegPath))
// 	{
// 
// 	}
// 	else
// 	{
// 		return FALSE;
// 	}
// 
// 	reg.Close();
// 
// 	stOption = m_stOption.Misc;

	return TRUE;
}

//=============================================================================
// Method		: SaveOption_All
// Access		: public  
// Returns		: void
// Parameter	: stLT_Option stOption
// Qualifier	:
// Last Update	: 2016/5/18 - 16:01
// Desc.		:
//=============================================================================
void CLT_Option::SaveOption_All( stLT_Option stOption )
{
	SaveOption_Inspector	(stOption.Inspector);
	SaveOption_Server		(stOption.Server);
	SaveOption_Misc			(stOption.Misc);
}

//=============================================================================
// Method		: LoadOption_All
// Access		: public  
// Returns		: BOOL
// Parameter	: stLT_Option & stOption
// Qualifier	:
// Last Update	: 2016/5/18 - 16:00
// Desc.		:
//=============================================================================
BOOL CLT_Option::LoadOption_All( stLT_Option& stOption )
{
	BOOL bReturn = TRUE;

	bReturn &= LoadOption_Inspector		(stOption.Inspector);
	bReturn &= LoadOption_Server		(stOption.Server);
	bReturn &= LoadOption_Misc			(stOption.Misc);

	return bReturn;
}

//=============================================================================
// Method		: SetInspectorType
// Access		: public  
// Returns		: void
// Parameter	: __in enInsptrSysType nInsptrType
// Qualifier	:
// Last Update	: 2016/9/26 - 18:01
// Desc.		:
//=============================================================================
void CLT_Option::SetInspectorType(__in enInsptrSysType nInsptrType)
{
	m_InsptrType = nInsptrType;
}
