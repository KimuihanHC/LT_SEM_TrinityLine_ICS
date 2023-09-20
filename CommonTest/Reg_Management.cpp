//*****************************************************************************
// Filename	: 	Reg_Management.cpp
// Created	:	2016/11/7 - 15:55
// Modified	:	2016/11/7 - 15:55
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
#include "stdafx.h"
#include "Reg_Management.h"

#ifndef REG_PATH_APP_BASE
#define		REG_PATH_APP_BASE		_T("Software\\Luritech\\Environment")
#endif

#ifndef REG_PATH_OPTION_BASE
#define		REG_PATH_OPTION_BASE	_T("Software\\Luritech\\Environment\\Option")
#endif

CReg_Management::CReg_Management()
{
	m_szRegPath.Format(_T("%s\\Handler"), REG_PATH_APP_BASE);
}


CReg_Management::~CReg_Management()
{
}

//=============================================================================
// Method		: SavePassword_Mgr
// Access		: public  
// Returns		: BOOL
// Parameter	: __in CString szPassword
// Qualifier	:
// Last Update	: 2016/11/7 - 16:12
// Desc.		:
//=============================================================================
BOOL CReg_Management::SavePassword_Mgr(__in CString szPassword)
{
	CRegistry	reg;

	if (reg.Open(HKEY_CURRENT_USER, REG_PATH_OPTION_BASE))
	{
		//Password
		reg.WriteString(_T("Password_Mgr"), szPassword);

		reg.Close();

		return TRUE;
	}

	return FALSE;
}

//=============================================================================
// Method		: LoadPassword_Mgr
// Access		: public  
// Returns		: CString
// Qualifier	:
// Last Update	: 2016/11/7 - 16:12
// Desc.		:
//=============================================================================
CString CReg_Management::LoadPassword_Mgr()
{
	CRegistry	reg;
	CString		strValue;

	if (reg.Open(HKEY_CURRENT_USER, REG_PATH_OPTION_BASE))
	{
		reg.ReadString(_T("Password_Mgr"), strValue);

		reg.Close();
	}

	return strValue;
}

//=============================================================================
// Method		: SavePassword_Admin
// Access		: public  
// Returns		: BOOL
// Parameter	: __in CString szPassword
// Qualifier	:
// Last Update	: 2016/11/7 - 16:12
// Desc.		:
//=============================================================================
BOOL CReg_Management::SavePassword_Admin(__in CString szPassword)
{
	CRegistry	reg;

	if (reg.Open(HKEY_CURRENT_USER, REG_PATH_OPTION_BASE))
	{
		//Password
		reg.WriteString(_T("Password_Admin"), szPassword);

		reg.Close();

		return TRUE;
	}

	return FALSE;
}

//=============================================================================
// Method		: LoadPassword_Admin
// Access		: public  
// Returns		: CString
// Qualifier	:
// Last Update	: 2016/11/7 - 16:12
// Desc.		:
//=============================================================================
CString CReg_Management::LoadPassword_Admin()
{
	CRegistry	reg;
	CString		strValue;

	if (reg.Open(HKEY_CURRENT_USER, REG_PATH_OPTION_BASE))
	{
		reg.ReadString(_T("Password_Admin"), strValue);

		reg.Close();
	}

	return strValue;
}

//=============================================================================
// Method		: SaveUserName
// Access		: public  
// Returns		: BOOL
// Parameter	: __in CString szUserName
// Qualifier	:
// Last Update	: 2016/11/7 - 17:36
// Desc.		:
//=============================================================================
BOOL CReg_Management::SaveUserName(__in CString szUserName)
{
	CRegistry	reg;

	if (reg.Open(HKEY_CURRENT_USER, REG_PATH_APP_BASE))
	{
		reg.WriteString(_T("OperatorName"), szUserName);

		reg.Close();

		return TRUE;
	}

	return FALSE;
}

CString CReg_Management::LoadUserName()
{
	CRegistry	reg;
	CString		strValue;

	if (reg.Open(HKEY_CURRENT_USER, REG_PATH_APP_BASE))
	{
		reg.ReadString(_T("OperatorName"), strValue);

		reg.Close();
	}

	return strValue;
}
