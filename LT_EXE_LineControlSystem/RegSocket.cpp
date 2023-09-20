//*****************************************************************************
// Filename	: 	RegSocket.cpp
// Created	:	2021/12/21 - 13:46
// Modified	:	2021/12/21 - 13:46
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#include "stdafx.h"
#include "RegSocket.h"
#include "Registry.h"
#include "Def_Socket_Info.h"
#include "SocketMonitoring.h"

//#define USE_VERIFY_KEY_ALWAYS	// 레지스트리에 데이터 기록 할 때마다 레지스트리 경로 체크 및 생성

CRegSocket::CRegSocket()
{
	m_szRegPath.Format(_T("%s\\SocketInfo"), REG_PATH_APP_BASE);
}

CRegSocket::CRegSocket(__in LPCTSTR lpszRegPath)
{
	m_szRegPath.Format(_T("%s\\SocketInfo"), lpszRegPath);
}

CRegSocket::~CRegSocket()
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
void CRegSocket::Set_RegitryPath(__in LPCTSTR lpszRegPath)
{
	m_szRegPath.Format(_T("%s\\SocketInfo"), lpszRegPath);
}

//=============================================================================
// Method		: Check_RegistryPath
// Access		: public  
// Returns		: bool
// Parameter	: __in LPCTSTR IN_szRFID
// Qualifier	:
// Last Update	: 2022/8/12 - 11:28
// Desc.		:
//=============================================================================
bool CRegSocket::Check_RegistryPath(__in LPCTSTR IN_szRFID)
{
	CRegistry* m_pReg = new CRegistry(HKEY_CURRENT_USER);
	CString		szRegPath;
	szRegPath.Format(_T("%s\\%s"), m_szRegPath.GetBuffer(), IN_szRFID);

	if (!m_pReg->VerifyKey(szRegPath))
	{
		m_pReg->CreateKey(HKEY_CURRENT_USER, szRegPath);
	}
	m_pReg->Close();

	delete m_pReg;
	return true;
}

bool CRegSocket::Check_RegistryPath(__in const CSocketMonitoring* IN_SocketInto)
{
	auto eIter = IN_SocketInto->m_Sockets.end();
	auto Iter = IN_SocketInto->m_Sockets.begin();
	while (Iter != eIter)
	{
		Check_RegistryPath(Iter->second.szRFID);

		Iter++;
	}

	return true;
}

//=============================================================================
// Method		: Set_SocketInfo
// Access		: public  
// Returns		: bool
// Parameter	: __in LPCTSTR IN_szRFID
// Parameter	: __in const CSocketInfo_Unit * IN_pSocketInfo
// Qualifier	:
// Last Update	: 2021/12/21 - 16:15
// Desc.		:
//=============================================================================
bool CRegSocket::Set_SocketInfo(__in LPCTSTR IN_szRFID, __in const CSocketInfo_Unit* IN_pSocketInfo)
{
	CRegistry* m_pReg = new CRegistry(HKEY_CURRENT_USER);
	CString		szRegPath;
	CString		szValue;
	szRegPath.Format(_T("%s\\%s"), m_szRegPath.GetBuffer(), IN_szRFID);

#ifdef USE_VERIFY_KEY_ALWAYS
	if (!m_pReg->VerifyKey(szRegPath))
	{
		m_pReg->CreateKey(HKEY_CURRENT_USER, szRegPath);
	}
	m_pReg->Close();
#endif

	if (m_pReg->Open(HKEY_CURRENT_USER, szRegPath))
	{
		//IN_pSocketInfo->szRFID;
		//IN_pSocketInfo->nSocketType;
		//IN_pSocketInfo->szComment;

		// 제품 정보
		szValue = IN_pSocketInfo->m_szBarcode;
		m_pReg->WriteString(_T("Barcode"), szValue.GetBuffer());

		m_pReg->WriteDWORD(_T("Status"), IN_pSocketInfo->m_nStatus);
		//m_pReg->WriteDWORD(_T("Judgment"), IN_pSocketInfo->m_nJudgement);

		// 소켓 위치 정보
		m_pReg->WriteDWORD(_T("Equipment_Order"), IN_pSocketInfo->m_nEqpOrder);

		szValue = IN_pSocketInfo->m_szEquipmentID;
		m_pReg->WriteString(_T("Equipment_ID"), szValue.GetBuffer());

		m_pReg->WriteDWORD(_T("Location_inEqp"), IN_pSocketInfo->m_nPort_inEqp);

		m_pReg->WriteDWORD(_T("Target_EqpOrder"), IN_pSocketInfo->m_nTargetEqpOrder);

		szValue = IN_pSocketInfo->m_szTargetEqpID;
		m_pReg->WriteString(_T("Target_EqpID"), szValue.GetBuffer());

		// 소켓 수율
		m_pReg->WriteDWORD(_T("Yield_Total"), IN_pSocketInfo->m_Yield.dwTotal);

		m_pReg->WriteDWORD(_T("Yield_Pass"), IN_pSocketInfo->m_Yield.dwPass);

		m_pReg->WriteDWORD(_T("Yield_Fail"), IN_pSocketInfo->m_Yield.dwFail);

		// 검사 결과
		//m_pReg->WriteDWORD(_T("Result_NG_Code"), IN_pSocketInfo->m_stTestResult.m_nNG_Code);
		szValue.Format(_T("%d"), IN_pSocketInfo->m_stTestResult.m_nNG_Code);
		m_pReg->WriteString(_T("Result_NG_Code"), szValue.GetBuffer());

		m_pReg->WriteDWORD(_T("Result_NG_Para"), IN_pSocketInfo->m_stTestResult.m_nNG_Para);

		m_pReg->WriteDWORD(_T("Result_NG_EqpOrder"), IN_pSocketInfo->m_stTestResult.m_nNG_EqpOrder);

		szValue = IN_pSocketInfo->m_stTestResult.m_szNG_EqpID;
		m_pReg->WriteString(_T("Result_NG_EqpID"), szValue.GetBuffer());

		m_pReg->WriteDWORD(_T("Result_NG_EqpType"), IN_pSocketInfo->m_stTestResult.m_nNG_EqpType);

		// Rework
		m_pReg->WriteDWORD(_T("Rework"), IN_pSocketInfo->m_bRework ? 1 : 0);
	}
	else
	{
		delete m_pReg;
		return false;
	}

	m_pReg->Close();

	delete m_pReg;
	return true;
}

//=============================================================================
// Method		: Set_SocketInfo_Barcode
// Access		: public  
// Returns		: bool
// Parameter	: __in LPCTSTR IN_szRFID
// Parameter	: __in const CSocketInfo_Unit * IN_pSocketInfo
// Qualifier	:
// Last Update	: 2022/8/12 - 12:05
// Desc.		:
//=============================================================================
bool CRegSocket::Set_SocketInfo_Barcode(__in LPCTSTR IN_szRFID, __in const CSocketInfo_Unit* IN_pSocketInfo)
{
	CRegistry* m_pReg = new CRegistry(HKEY_CURRENT_USER);
	CString		szRegPath;
	CString		szValue;
	szRegPath.Format(_T("%s\\%s"), m_szRegPath.GetBuffer(), IN_szRFID);

//#ifdef USE_VERIFY_KEY_ALWAYS
// 	if (!m_pReg->VerifyKey(szRegPath))
// 	{
// 		m_pReg->CreateKey(HKEY_CURRENT_USER, szRegPath);
// 	}
//	m_pReg->Close();
//#endif

	if (m_pReg->Open(HKEY_CURRENT_USER, szRegPath))
	{
		// 제품 정보
		szValue = IN_pSocketInfo->m_szBarcode;
		m_pReg->WriteString(_T("Barcode"), szValue.GetBuffer());

		// Rework
		m_pReg->WriteDWORD(_T("Rework"), IN_pSocketInfo->m_bRework ? 1 : 0);
	}
	else
	{
		delete m_pReg;
		return false;
	}

	m_pReg->Close();

	delete m_pReg;
	return true;
}

//=============================================================================
// Method		: Set_SocketInfo_Status
// Access		: public  
// Returns		: bool
// Parameter	: __in LPCTSTR IN_szRFID
// Parameter	: __in const CSocketInfo_Unit * IN_pSocketInfo
// Qualifier	:
// Last Update	: 2022/8/12 - 12:05
// Desc.		:
//=============================================================================
bool CRegSocket::Set_SocketInfo_Status(__in LPCTSTR IN_szRFID, __in const CSocketInfo_Unit* IN_pSocketInfo)
{
	CRegistry* m_pReg = new CRegistry(HKEY_CURRENT_USER);
	CString		szRegPath;
	CString		szValue;
	szRegPath.Format(_T("%s\\%s"), m_szRegPath.GetBuffer(), IN_szRFID);

//#ifdef USE_VERIFY_KEY_ALWAYS
// 	if (!m_pReg->VerifyKey(szRegPath))
// 	{
// 		m_pReg->CreateKey(HKEY_CURRENT_USER, szRegPath);
// 	}
//	m_pReg->Close();
//#endif

	if (m_pReg->Open(HKEY_CURRENT_USER, szRegPath))
	{
		m_pReg->WriteDWORD(_T("Status"), IN_pSocketInfo->m_nStatus);
	}
	else
	{
		delete m_pReg;
		return false;
	}

	m_pReg->Close();

	delete m_pReg;
	return true;
}

//=============================================================================
// Method		: Set_SocketInfo_Equipment
// Access		: public  
// Returns		: bool
// Parameter	: __in LPCTSTR IN_szRFID
// Parameter	: __in const CSocketInfo_Unit * IN_pSocketInfo
// Qualifier	:
// Last Update	: 2022/8/12 - 12:05
// Desc.		:
//=============================================================================
bool CRegSocket::Set_SocketInfo_Equipment(__in LPCTSTR IN_szRFID, __in const CSocketInfo_Unit* IN_pSocketInfo)
{
	CRegistry* m_pReg = new CRegistry(HKEY_CURRENT_USER);
	CString		szRegPath;
	CString		szValue;
	szRegPath.Format(_T("%s\\%s"), m_szRegPath.GetBuffer(), IN_szRFID);

//#ifdef USE_VERIFY_KEY_ALWAYS
// 	if (!m_pReg->VerifyKey(szRegPath))
// 	{
// 		m_pReg->CreateKey(HKEY_CURRENT_USER, szRegPath);
// 	}
//	m_pReg->Close();
//#endif

	if (m_pReg->Open(HKEY_CURRENT_USER, szRegPath))
	{
		// 소켓 위치 정보
		m_pReg->WriteDWORD(_T("Equipment_Order"), IN_pSocketInfo->m_nEqpOrder);

		szValue = IN_pSocketInfo->m_szEquipmentID;
		m_pReg->WriteString(_T("Equipment_ID"), szValue.GetBuffer());
	}
	else
	{
		delete m_pReg;
		return false;
	}

	m_pReg->Close();

	delete m_pReg;
	return true;
}

//=============================================================================
// Method		: Set_SocketInfo_Location
// Access		: public  
// Returns		: bool
// Parameter	: __in LPCTSTR IN_szRFID
// Parameter	: __in const CSocketInfo_Unit * IN_pSocketInfo
// Qualifier	:
// Last Update	: 2022/8/12 - 12:05
// Desc.		:
//=============================================================================
bool CRegSocket::Set_SocketInfo_Location(__in LPCTSTR IN_szRFID, __in const CSocketInfo_Unit* IN_pSocketInfo)
{
	CRegistry* m_pReg = new CRegistry(HKEY_CURRENT_USER);
	CString		szRegPath;
	CString		szValue;
	szRegPath.Format(_T("%s\\%s"), m_szRegPath.GetBuffer(), IN_szRFID);

//#ifdef USE_VERIFY_KEY_ALWAYS
// 	if (!m_pReg->VerifyKey(szRegPath))
// 	{
// 		m_pReg->CreateKey(HKEY_CURRENT_USER, szRegPath);
// 	}
//	m_pReg->Close();
//#endif

	if (m_pReg->Open(HKEY_CURRENT_USER, szRegPath))
	{
		// 소켓 위치 정보
		m_pReg->WriteDWORD(_T("Location_inEqp"), IN_pSocketInfo->m_nPort_inEqp);
	}
	else
	{
		delete m_pReg;
		return false;
	}

	m_pReg->Close();

	delete m_pReg;
	return true;
}

//=============================================================================
// Method		: Set_SocketInfo_Target
// Access		: public  
// Returns		: bool
// Parameter	: __in LPCTSTR IN_szRFID
// Parameter	: __in const CSocketInfo_Unit * IN_pSocketInfo
// Qualifier	:
// Last Update	: 2022/8/12 - 12:06
// Desc.		:
//=============================================================================
bool CRegSocket::Set_SocketInfo_Target(__in LPCTSTR IN_szRFID, __in const CSocketInfo_Unit* IN_pSocketInfo)
{
	CRegistry* m_pReg = new CRegistry(HKEY_CURRENT_USER);
	CString		szRegPath;
	CString		szValue;
	szRegPath.Format(_T("%s\\%s"), m_szRegPath.GetBuffer(), IN_szRFID);

//#ifdef USE_VERIFY_KEY_ALWAYS
// 	if (!m_pReg->VerifyKey(szRegPath))
// 	{
// 		m_pReg->CreateKey(HKEY_CURRENT_USER, szRegPath);
// 	}
//	m_pReg->Close();
//#endif

	if (m_pReg->Open(HKEY_CURRENT_USER, szRegPath))
	{
		m_pReg->WriteDWORD(_T("Target_EqpOrder"), IN_pSocketInfo->m_nTargetEqpOrder);

		szValue = IN_pSocketInfo->m_szTargetEqpID;
		m_pReg->WriteString(_T("Target_EqpID"), szValue.GetBuffer());
	}
	else
	{
		delete m_pReg;
		return false;
	}

	m_pReg->Close();

	delete m_pReg;
	return true;
}

//=============================================================================
// Method		: Set_SocketInfo_Yield
// Access		: public  
// Returns		: bool
// Parameter	: __in LPCTSTR IN_szRFID
// Parameter	: __in const CSocketInfo_Unit * IN_pSocketInfo
// Qualifier	:
// Last Update	: 2022/8/12 - 12:06
// Desc.		:
//=============================================================================
bool CRegSocket::Set_SocketInfo_Yield(__in LPCTSTR IN_szRFID, __in const CSocketInfo_Unit* IN_pSocketInfo)
{
	CRegistry* m_pReg = new CRegistry(HKEY_CURRENT_USER);
	CString		szRegPath;
	szRegPath.Format(_T("%s\\%s"), m_szRegPath.GetBuffer(), IN_szRFID);

#ifdef USE_VERIFY_KEY_ALWAYS
	if (!m_pReg->VerifyKey(szRegPath))
	{
		m_pReg->CreateKey(HKEY_CURRENT_USER, szRegPath);
	}
	m_pReg->Close();
#endif

	if (m_pReg->Open(HKEY_CURRENT_USER, szRegPath))
	{
		// 소켓 수율
		m_pReg->WriteDWORD(_T("Yield_Total"), IN_pSocketInfo->m_Yield.dwTotal);

		m_pReg->WriteDWORD(_T("Yield_Pass"), IN_pSocketInfo->m_Yield.dwPass);

		m_pReg->WriteDWORD(_T("Yield_Fail"), IN_pSocketInfo->m_Yield.dwFail);
	}
	else
	{
		delete m_pReg;
		return false;
	}

	m_pReg->Close();

	delete m_pReg;
	return true;
}

//=============================================================================
// Method		: Set_SocketInfo_TestResult
// Access		: public  
// Returns		: bool
// Parameter	: __in LPCTSTR IN_szRFID
// Parameter	: __in const CSocketInfo_Unit * IN_pSocketInfo
// Qualifier	:
// Last Update	: 2022/8/12 - 12:06
// Desc.		:
//=============================================================================
bool CRegSocket::Set_SocketInfo_TestResult(__in LPCTSTR IN_szRFID, __in const CSocketInfo_Unit* IN_pSocketInfo)
{
	CRegistry* m_pReg = new CRegistry(HKEY_CURRENT_USER);
	CString		szRegPath;
	CString		szValue;
	szRegPath.Format(_T("%s\\%s"), m_szRegPath.GetBuffer(), IN_szRFID);

#ifdef USE_VERIFY_KEY_ALWAYS
	if (!m_pReg->VerifyKey(szRegPath))
	{
		m_pReg->CreateKey(HKEY_CURRENT_USER, szRegPath);
	}
	m_pReg->Close();
#endif

	if (m_pReg->Open(HKEY_CURRENT_USER, szRegPath))
	{
		// 검사 결과
		//m_pReg->WriteDWORD(_T("Result_NG_Code"), IN_pSocketInfo->m_stTestResult.m_nNG_Code);
		szValue.Format(_T("%d"), IN_pSocketInfo->m_stTestResult.m_nNG_Code);
		m_pReg->WriteString(_T("Result_NG_Code"), szValue.GetBuffer());

		m_pReg->WriteDWORD(_T("Result_NG_Para"), IN_pSocketInfo->m_stTestResult.m_nNG_Para);

		m_pReg->WriteDWORD(_T("Result_NG_EqpOrder"), IN_pSocketInfo->m_stTestResult.m_nNG_EqpOrder);

		szValue = IN_pSocketInfo->m_stTestResult.m_szNG_EqpID;
		m_pReg->WriteString(_T("Result_NG_EqpID"), szValue.GetBuffer());

		m_pReg->WriteDWORD(_T("Result_NG_EqpType"), IN_pSocketInfo->m_stTestResult.m_nNG_EqpType);
	}
	else
	{
		delete m_pReg;
		return false;
	}

	m_pReg->Close();

	delete m_pReg;
	return true;
}

//=============================================================================
// Method		: Set_SocketInfo_InputTime
// Access		: public  
// Returns		: bool
// Parameter	: __in LPCTSTR IN_szRFID
// Parameter	: __in const CSocketInfo_Unit * IN_pSocketInfo
// Qualifier	:
// Last Update	: 2022/8/12 - 12:06
// Desc.		:
//=============================================================================
bool CRegSocket::Set_SocketInfo_InputTime(__in LPCTSTR IN_szRFID, __in const CSocketInfo_Unit* IN_pSocketInfo)
{

	return true;
}

bool CRegSocket::Set_SocketInfo_OutputTime(__in LPCTSTR IN_szRFID, __in const CSocketInfo_Unit* IN_pSocketInfo)
{

	return true;
}

//=============================================================================
// Method		: Get_SocketInfo
// Access		: public  
// Returns		: bool
// Parameter	: __in LPCTSTR IN_szRFID
// Parameter	: __out CSocketInfo_Unit & OUT_SocketInto
// Qualifier	:
// Last Update	: 2021/12/21 - 16:15
// Desc.		:
//=============================================================================
bool CRegSocket::Get_SocketInfo(__in LPCTSTR IN_szRFID, __out CSocketInfo_Unit& OUT_SocketInto)
{
	CRegistry* m_pReg = new CRegistry(HKEY_CURRENT_USER);
	CString		szRegPath;
	CString		szValue;	
	DWORD		dwValue = 0;
	szRegPath.Format(_T("%s\\%s"), m_szRegPath.GetBuffer(), IN_szRFID);

	if (m_pReg->Open(HKEY_CURRENT_USER, szRegPath))
	{
		//OUT_SocketInto.szRFID;
		//OUT_SocketInto.nSocketType;
		//OUT_SocketInto.szComment;

		// 제품 정보
		if (m_pReg->ReadString(_T("Barcode"), szValue))
			OUT_SocketInto.m_szBarcode = szValue.GetBuffer();
		else
			OUT_SocketInto.m_szBarcode.Empty();

		if (m_pReg->ReadDWORD(_T("Status"), dwValue))
			OUT_SocketInto.m_nStatus = static_cast<uint8_t>(dwValue);
		else
			OUT_SocketInto.m_nStatus = 0;

// 		if (m_pReg->ReadDWORD(_T("Judgment"), dwValue))
// 			OUT_SocketInto.m_nJudgement = static_cast<uint8_t>(dwValue);
// 		else
// 			OUT_SocketInto.m_nJudgement = 0;

		// 소켓 위치 정보
		if (m_pReg->ReadDWORD(_T("Equipment_Order"), dwValue))
			OUT_SocketInto.m_nEqpOrder = static_cast<uint8_t>(dwValue);
		else
			OUT_SocketInto.m_nEqpOrder = 0;

 		if (m_pReg->ReadString(_T("Equipment_ID"), szValue))
			OUT_SocketInto.m_szEquipmentID = szValue.GetBuffer();
 		else
			OUT_SocketInto.m_szEquipmentID.Empty();
		
		if (m_pReg->ReadDWORD(_T("Location_inEqp"), dwValue))
			OUT_SocketInto.m_nPort_inEqp = static_cast<uint8_t>(dwValue);
		else
			OUT_SocketInto.m_nPort_inEqp = 0;

		if (m_pReg->ReadDWORD(_T("Target_EqpOrder"), dwValue))
			OUT_SocketInto.m_nTargetEqpOrder = static_cast<uint8_t>(dwValue);
		else
			OUT_SocketInto.m_nTargetEqpOrder = 0;

		if (m_pReg->ReadString(_T("Target_EqpID"), szValue))
			OUT_SocketInto.m_szTargetEqpID = szValue.GetBuffer();
		else
			OUT_SocketInto.m_szTargetEqpID.Empty();

		// 소켓 수율
		if (m_pReg->ReadDWORD(_T("Yield_Total"), dwValue))
			OUT_SocketInto.m_Yield.dwTotal = static_cast<uint8_t>(dwValue);
		else
			OUT_SocketInto.m_Yield.dwTotal = 0;

		if (m_pReg->ReadDWORD(_T("Yield_Pass"), dwValue))
			OUT_SocketInto.m_Yield.dwPass = static_cast<uint8_t>(dwValue);
		else
			OUT_SocketInto.m_Yield.dwPass = 0;

		if (m_pReg->ReadDWORD(_T("Yield_Fail"), dwValue))
			OUT_SocketInto.m_Yield.dwFail = static_cast<uint8_t>(dwValue);
		else
			OUT_SocketInto.m_Yield.dwFail = 0;

		// 검사 결과
		/*if (m_pReg->ReadDWORD(_T("Result_NG_Code"), dwValue))
			OUT_SocketInto.m_stTestResult.m_nNG_Code = static_cast<uint16_t>(dwValue);*/
		if (m_pReg->ReadString(_T("Result_NG_Code"), szValue))
			OUT_SocketInto.m_stTestResult.m_nNG_Code = _ttoi(szValue.GetBuffer());
		else
			OUT_SocketInto.m_stTestResult.m_nNG_Code = 0;

		if (m_pReg->ReadDWORD(_T("Result_NG_Para"), dwValue))
			OUT_SocketInto.m_stTestResult.m_nNG_Para = static_cast<int8_t>(dwValue);

		if (m_pReg->ReadDWORD(_T("Result_NG_EqpOrder"), dwValue))
			OUT_SocketInto.m_stTestResult.m_nNG_EqpOrder = static_cast<uint8_t>(dwValue);

		if (m_pReg->ReadString(_T("Result_NG_EqpID"), szValue))
			OUT_SocketInto.m_stTestResult.m_szNG_EqpID = szValue.GetBuffer();
		else
			OUT_SocketInto.m_stTestResult.m_szNG_EqpID.Empty();

		if (m_pReg->ReadDWORD(_T("Result_NG_EqpType"), dwValue))
			OUT_SocketInto.m_stTestResult.m_nNG_EqpType = static_cast<uint8_t>(dwValue);

		// Rework
		if (m_pReg->ReadDWORD(_T("Rework"), dwValue))
			OUT_SocketInto.m_bRework = (0 < dwValue) ? true : false;
	}
	else
	{
		delete m_pReg;
		return false;
	}

	m_pReg->Close();

	delete m_pReg;
	return true;
}

//=============================================================================
// Method		: Get_SocketInfo
// Access		: public  
// Returns		: bool
// Parameter	: __out CSocketMonitoring & OUT_SocketInto
// Qualifier	:
// Last Update	: 2021/12/21 - 16:33
// Desc.		:
//=============================================================================
bool CRegSocket::Get_SocketInfo(__out CSocketMonitoring& OUT_SocketInto)
{
	// OUT_SocketInto에 있는 기본 소켓 Config 정보를 이용해서 레지스트리에 저장된 정보 불러옴

	auto eIter = OUT_SocketInto.m_Sockets.end();
	auto Iter = OUT_SocketInto.m_Sockets.begin();
	while (Iter != eIter)
	{
		CSocketInfo_Unit SocketUnit;

		// 레지스트리에 있는 소켓 정보 구하기
		if (Get_SocketInfo (Iter->second.szRFID, SocketUnit))
		{
			OUT_SocketInto.Set_Stored_SocketInfo(Iter->second.szRFID, &SocketUnit);
		}

		Iter++;
	}

	return true;
}
