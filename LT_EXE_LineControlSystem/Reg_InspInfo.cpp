//*****************************************************************************
// Filename	: 	Reg_InspInfo.cpp
// Created	:	2016/3/31 - 16:33
// Modified	:	2016/3/31 - 16:33
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
#include "stdafx.h"
#include "Reg_InspInfo.h"
#include "Registry.h"


CReg_InspInfo::CReg_InspInfo()
{
	m_szRegPath.Format(_T("%s\\InspInfo"), REG_PATH_APP_BASE);
}

CReg_InspInfo::CReg_InspInfo(__in LPCTSTR lpszRegPath)
{
	m_szRegPath = lpszRegPath;
}

CReg_InspInfo::~CReg_InspInfo()
{
}

//=============================================================================
// Method		: SavePassword
// Access		: public  
// Returns		: BOOL
// Parameter	: __in UINT nIndex
// Qualifier	:
// Last Update	: 2016/6/25 - 15:58
// Desc.		:
//=============================================================================
BOOL CReg_InspInfo::SavePassword(__in UINT nIndex, __in CString szPassword)
{
	CRegistry	reg;
	if (reg.Open(HKEY_CURRENT_USER, REG_PATH_OPTION_BASE))
	{
		//Password
		if (0 == nIndex)
			reg.WriteString(_T("Password"), szPassword);
		else
			reg.WriteString(_T("Password_2"), szPassword);

		reg.Close();

		return TRUE;
	}

	return FALSE;
}

//=============================================================================
// Method		: LoadPassword
// Access		: public  
// Returns		: CString
// Parameter	: __in UINT nIndex
// Qualifier	:
// Last Update	: 2016/6/25 - 16:03
// Desc.		:
//=============================================================================
CString CReg_InspInfo::LoadPassword(__in UINT nIndex)
{
	CRegistry	reg;
	CString		strValue;
	if (reg.Open(HKEY_CURRENT_USER, REG_PATH_OPTION_BASE))
	{
		if (0 == nIndex)
			reg.ReadString(_T("Password"), strValue);
		else
			reg.ReadString(_T("Password_2"), strValue);

		reg.Close();
	}

	return strValue;
}

//=============================================================================
// Method		: Set_ModelTypeInfo
// Access		: public  
// Returns		: bool
// Parameter	: __in const CConfig_Model * IN_pModelTypeInfo
// Qualifier	:
// Last Update	: 2022/1/6 - 15:35
// Desc.		:
//=============================================================================
bool CReg_InspInfo::Set_ModelTypeInfo(__in const CConfig_Model* IN_pModelTypeInfo)
{
	CRegistry* m_pReg = new CRegistry(HKEY_CURRENT_USER);
	CString		szRegPath;
	CString		szValue;
	DWORD		dwValue = 0;
	szRegPath.Format(_T("%s\\ModelInfo"), REG_PATH_APP_BASE);

	if (!m_pReg->VerifyKey(szRegPath))
	{
		m_pReg->CreateKey(HKEY_CURRENT_USER, szRegPath);
	}
	m_pReg->Close();

	if (m_pReg->Open(HKEY_CURRENT_USER, szRegPath))
	{
		// Alias
		szValue = IN_pModelTypeInfo->m_szAlias;
		m_pReg->WriteString(_T("Alias"), szValue.GetBuffer());

		// Socket Type
		m_pReg->WriteDWORD(_T("Socket_Type"), IN_pModelTypeInfo->m_nSocketType);

		// Model Type
		m_pReg->WriteDWORD(_T("Model_Type"), IN_pModelTypeInfo->m_nModelType);

		// Tester 사용여부
		m_pReg->WriteDWORD(_T("Count"), Max_TesterCount);
		for (size_t nIdx = 0; nIdx < Max_TesterCount; nIdx++)
		{
			dwValue = IN_pModelTypeInfo->m_bEnable_TestType[nIdx] ? 1 : 0;
			m_pReg->WriteDWORD(g_szTesterTypeName[nIdx], dwValue);
		}
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

bool CReg_InspInfo::Get_ModelTypeInfo(__out CConfig_Model& OUT_ModelTypeInto)
{
	CRegistry* m_pReg = new CRegistry(HKEY_CURRENT_USER);
	CString		szRegPath;
	CString		szValue;
	DWORD		dwValue = 0;
	szRegPath.Format(_T("%s\\ModelInfo"), REG_PATH_APP_BASE);

	if (m_pReg->Open(HKEY_CURRENT_USER, szRegPath))
	{
		// Alias
		if (m_pReg->ReadString(_T("Alias"), szValue))
		{
			OUT_ModelTypeInto.m_szAlias = szValue.GetBuffer();
		}
		else
		{
			OUT_ModelTypeInto.m_szAlias.Empty();
		}
		
		// Socket Type
		if (m_pReg->ReadDWORD(_T("Socket_Type"), dwValue))
		{
			OUT_ModelTypeInto.m_nSocketType = static_cast<uint8_t>(dwValue);
		}
		else
		{
			OUT_ModelTypeInto.m_nSocketType = 0;
			// fail
		}

		// Model Type
		if (m_pReg->ReadDWORD(_T("Model_Type"), dwValue))
		{
			OUT_ModelTypeInto.m_nModelType = static_cast<uint8_t>(dwValue);
		}
		else
		{
			OUT_ModelTypeInto.m_nModelType = 0;
			// fail
		}

		// Tester 사용여부
		DWORD iItemCount = Max_TesterCount;
		if (m_pReg->ReadDWORD(_T("Count"), dwValue))
		{
			iItemCount = __min(dwValue, Max_TesterCount);
		}
		
		for (DWORD nIdx = 0; nIdx < iItemCount; nIdx++)
		{
			if (m_pReg->ReadDWORD(g_szTesterTypeName[nIdx], dwValue))
			{
				OUT_ModelTypeInto.m_bEnable_TestType[nIdx] = (0 < dwValue) ? true : false;
			}
			else
			{
				OUT_ModelTypeInto.m_bEnable_TestType[nIdx] = false;
			}
		}

		m_pReg->Close();
	}

	delete m_pReg;
	return true;
}

//=============================================================================
// Method		: Set_UserInfo
// Access		: public  
// Returns		: bool
// Parameter	: __in const ST_UserInfo * IN_pUserInfo
// Qualifier	:
// Last Update	: 2022/1/6 - 15:35
// Desc.		:
//=============================================================================
bool CReg_InspInfo::Set_UserInfo(__in const ST_UserInfo* IN_pUserInfo)
{
	CRegistry* m_pReg = new CRegistry(HKEY_CURRENT_USER);
	CString		szRegPath;
	CString		szValue;
	DWORD		dwValue = 0;
	szRegPath.Format(_T("%s\\UserInfo"), REG_PATH_APP_BASE);

	if (!m_pReg->VerifyKey(szRegPath))
	{
		m_pReg->CreateKey(HKEY_CURRENT_USER, szRegPath);
	}
	m_pReg->Close();

	if (m_pReg->Open(HKEY_CURRENT_USER, szRegPath))
	{
		// User Count
		dwValue = static_cast<DWORD>(IN_pUserInfo->m_Users.size());
		m_pReg->WriteDWORD(_T("User_Count"), dwValue);

		for (auto nIdx = 0; nIdx < IN_pUserInfo->m_Users.size(); ++nIdx)
		{
			CRegistry	SubReg;
			szRegPath.Format(_T("%s\\UserInfo\\%03d"), REG_PATH_APP_BASE, nIdx);

			if (!SubReg.VerifyKey(HKEY_CURRENT_USER, szRegPath))
			{
				SubReg.CreateKey(HKEY_CURRENT_USER, szRegPath);
			}

			if (SubReg.Open(HKEY_CURRENT_USER, szRegPath))
			{
				// ID
				szValue = IN_pUserInfo->m_Users.at(nIdx).szID;
				SubReg.WriteString(_T("ID"), szValue.GetBuffer());

				// Name
				szValue = IN_pUserInfo->m_Users.at(nIdx).szName;
				SubReg.WriteString(_T("Name"), szValue.GetBuffer());

				// Authority
				dwValue = IN_pUserInfo->m_Users.at(nIdx).nAuthority;
				SubReg.WriteDWORD(_T("Authority"), dwValue);

				// Password
				szValue = IN_pUserInfo->m_Users.at(nIdx).szPassword;
				SubReg.WriteString(_T("Password"), szValue.GetBuffer());

				// Password Length
				dwValue = IN_pUserInfo->m_Users.at(nIdx).szPassword.GetLength();
				SubReg.WriteDWORD(_T("Password_Leng"), dwValue);
			}

			SubReg.Close();
		}	
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

bool CReg_InspInfo::Get_UserInfo(__out ST_UserInfo& OUT_UserInto)
{
	CRegistry* m_pReg = new CRegistry(HKEY_CURRENT_USER);
	CString		szRegPath;
	CString		szValue;
	DWORD		dwValue = 0;
	szRegPath.Format(_T("%s\\UserInfo"), REG_PATH_APP_BASE);

	if (m_pReg->Open(HKEY_CURRENT_USER, szRegPath))
	{
		DWORD dwUserCount = 0;
		m_pReg->ReadDWORD(_T("User_Count"), dwUserCount);
		
		for (DWORD nIdx = 0; nIdx < dwUserCount; ++nIdx)
		{
			CRegistry	SubReg;
			szRegPath.Format(_T("%s\\UserInfo\\%03d"), REG_PATH_APP_BASE, nIdx);

			if (SubReg.Open(HKEY_CURRENT_USER, szRegPath))
			{
				ST_User_Unit stUser;

				// ID
				if (SubReg.ReadString(_T("ID"), szValue))
				{
					stUser.szID = szValue.GetBuffer();
				}
				else
				{
					stUser.szID.Empty();
				}

				// Name
				if (SubReg.ReadString(_T("Name"), szValue))
				{
					stUser.szName = szValue.GetBuffer();
				}
				else
				{
					stUser.szName.Empty();
				}

				// Authority
				if (SubReg.ReadDWORD(_T("Authority"), dwValue))
				{
					stUser.nAuthority = static_cast<uint8_t>(dwValue);
				}
				else
				{
					stUser.nAuthority = 0;
				}

				// Password Length
				DWORD dwLen = 0;				
				SubReg.ReadDWORD(_T("Password_Leng"), dwLen);

				// Password
				if (SubReg.ReadString(_T("Password"), szValue))
				{
					stUser.szPassword = szValue.GetBuffer();
				}
				else
				{
					stUser.szPassword.Empty();
				}

				// 데이터 추가
 				OUT_UserInto.m_Users.push_back(stUser);
			}

			SubReg.Close();
		}

		m_pReg->Close();
	}
	
	// *** 기본 유저 정보를 추가한다.
	OUT_UserInto.Set_Default_Users();

	delete m_pReg;
	return true;
}

//=============================================================================
// Method		: Set_LogIn_User
// Access		: public  
// Returns		: bool
// Parameter	: __in const ST_User_Unit IN_pUser
// Qualifier	:
// Last Update	: 2022/5/13 - 15:20
// Desc.		:
//=============================================================================
bool CReg_InspInfo::Set_LogIn_User(__in const ST_User_Unit IN_pUser)
{
	CRegistry* m_pReg = new CRegistry(HKEY_CURRENT_USER);
	CString		szRegPath;
	CString		szValue;
	DWORD		dwValue = 0;
	szRegPath.Format(_T("%s\\UserInfo"), REG_PATH_APP_BASE);

	if (!m_pReg->VerifyKey(szRegPath))
	{
		m_pReg->CreateKey(HKEY_CURRENT_USER, szRegPath);
	}
	m_pReg->Close();

	if (m_pReg->Open(HKEY_CURRENT_USER, szRegPath))
	{
		// LogIn
		m_pReg->WriteDWORD(_T("LogIn"), TRUE);

		// ID
		szValue = IN_pUser.szID;
		m_pReg->WriteString(_T("LogIn_ID"), szValue.GetBuffer());

		// Name
		szValue = IN_pUser.szName;
		m_pReg->WriteString(_T("LogIn_Name"), szValue.GetBuffer());

		// Authority
		dwValue = IN_pUser.nAuthority;
		m_pReg->WriteDWORD(_T("LogIn_Authority"), dwValue);
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

bool CReg_InspInfo::Get_LogIn_User(__out ST_User_Unit& OUT_stUser)
{
	CRegistry* m_pReg = new CRegistry(HKEY_CURRENT_USER);
	CString		szRegPath;
	CString		szValue;
	DWORD		dwValue = 0;
	szRegPath.Format(_T("%s\\UserInfo"), REG_PATH_APP_BASE);

	if (m_pReg->Open(HKEY_CURRENT_USER, szRegPath))
	{
		// LogIn
		bool bLogIn = FALSE;
		if (m_pReg->ReadDWORD(_T("LogIn"), dwValue))
		{
			bLogIn = (0 < dwValue) ? true : false;
		}
		
		if (false == bLogIn)
		{
			m_pReg->Close();
			delete m_pReg;
			return false;
		}

		// ID
		if (m_pReg->ReadString(_T("LogIn_ID"), szValue))
		{
			OUT_stUser.szID = szValue.GetBuffer();
		}
		else
		{
			OUT_stUser.szID.Empty();
		}

		// Name
		if (m_pReg->ReadString(_T("LogIn_Name"), szValue))
		{
			OUT_stUser.szName = szValue.GetBuffer();
		}
		else
		{
			OUT_stUser.szName.Empty();
		}

		// Authority
		if (m_pReg->ReadDWORD(_T("LogIn_Authority"), dwValue))
		{
			OUT_stUser.nAuthority = static_cast<uint8_t>(dwValue);
		}
		else
		{
			OUT_stUser.nAuthority = 0;
		}

		m_pReg->Close();
	}

	delete m_pReg;
	return true;
}

bool CReg_InspInfo::Set_LogOut_User()
{
	CRegistry* m_pReg = new CRegistry(HKEY_CURRENT_USER);
	CString		szRegPath;
	szRegPath.Format(_T("%s\\UserInfo"), REG_PATH_APP_BASE);

	if (!m_pReg->VerifyKey(szRegPath))
	{
		m_pReg->CreateKey(HKEY_CURRENT_USER, szRegPath);
	}
	m_pReg->Close();

	if (m_pReg->Open(HKEY_CURRENT_USER, szRegPath))
	{
		// LogIn
		m_pReg->WriteDWORD(_T("LogIn"), FALSE);

		// ID
		m_pReg->WriteString(_T("LogIn_ID"), _T(""));

		// Name
		m_pReg->WriteString(_T("LogIn_Name"), _T(""));

		// Authority
		m_pReg->WriteDWORD(_T("LogIn_Authority"), 0);
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
// Method		: Get_Permission
// Access		: public  
// Returns		: uint8_t
// Qualifier	:
// Last Update	: 2022/2/19 - 13:16
// Desc.		:
//=============================================================================
uint8_t CReg_InspInfo::Get_Permission()
{
	CRegistry* m_pReg = new CRegistry(HKEY_CURRENT_USER);
	CString		szRegPath;
	DWORD		dwValue = 0;
	szRegPath.Format(_T("%s\\UserInfo"), REG_PATH_APP_BASE);

	uint8_t		nPermission = 0;

	if (m_pReg->Open(HKEY_CURRENT_USER, szRegPath))
	{
		// LogIn
		bool bLogIn = FALSE;
		if (m_pReg->ReadDWORD(_T("LogIn"), dwValue))
		{
			bLogIn = (0 < dwValue) ? true : false;
		}

		if (bLogIn)
		{
			// Authority
			if (m_pReg->ReadDWORD(_T("Authority"), dwValue))
			{
				nPermission = static_cast<uint8_t>(dwValue);
			}
		}

		m_pReg->Close();
	}

	delete m_pReg;
	return nPermission;
}

//=============================================================================
// Method		: Is_ProgramCrash
// Access		: public  
// Returns		: bool
// Qualifier	:
// Last Update	: 2022/2/19 - 13:36
// Desc.		:
//=============================================================================
bool CReg_InspInfo::Is_ProgramCrash()
{
	CRegistry* m_pReg = new CRegistry(HKEY_CURRENT_USER);
	DWORD		dwValue = 0;
	bool	bReturn = false;

	if (m_pReg->Open(HKEY_CURRENT_USER, REG_PATH_APP_BASE))
	{
		// LogIn
		if (m_pReg->ReadDWORD(_T("Program_Executed"), dwValue))
		{
			if (1 == dwValue)
			{
				bReturn = true;
			}
		}

		m_pReg->Close();

	}

	delete m_pReg;
	return bReturn;
}


//=============================================================================
// Method		: Set_ProgramLoading
// Access		: public  
// Returns		: bool
// Parameter	: __in bool IN_bStart
// Qualifier	:
// Last Update	: 2022/2/19 - 13:40
// Desc.		:
//=============================================================================
bool CReg_InspInfo::Set_ProgramLoading(__in bool IN_bStart)
{
	CRegistry* m_pReg = new CRegistry(HKEY_CURRENT_USER);
	if (!m_pReg->VerifyKey(REG_PATH_APP_BASE))
	{
		m_pReg->CreateKey(HKEY_CURRENT_USER, REG_PATH_APP_BASE);
	}
	m_pReg->Close();

	if (m_pReg->Open(HKEY_CURRENT_USER, REG_PATH_APP_BASE))
	{
		// LogIn
		m_pReg->WriteDWORD(_T("Program_Executed"), IN_bStart ? 1: 0);
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

