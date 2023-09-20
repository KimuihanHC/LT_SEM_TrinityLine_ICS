//*****************************************************************************
// Filename	: 	Reg_Management.h
// Created	:	2016/11/7 - 15:53
// Modified	:	2016/11/7 - 15:53
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
#ifndef Reg_Management_h__
#define Reg_Management_h__

#pragma once

#include "Registry.h"

class CReg_Management
{
public:
	CReg_Management();
	~CReg_Management();

protected:
	CRegistry	m_Reg;

	CString		m_szRegPath;

public:
	void		SetRegitryPath(__in LPCTSTR lpszRegPath)
	{
		m_szRegPath = lpszRegPath;
	}

	// User
	// Password
	// Password Admin

	BOOL		SavePassword_Mgr	(__in CString szPassword);
	CString		LoadPassword_Mgr	();

	BOOL		SavePassword_Admin	(__in CString szPassword);
	CString		LoadPassword_Admin	();

	BOOL		SaveUserName		(__in CString szUserName);
	CString		LoadUserName		();


};

#endif // Reg_Management_h__

