//*****************************************************************************
// Filename	: 	Def_UserInfo.h
// Created	:	2022/1/4 - 20:09
// Modified	:	2022/1/4 - 20:09
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#ifndef Def_UserInfo_h__
#define Def_UserInfo_h__


#pragma once

#include <stdint.h>
#include <afxwin.h>
#include <vector>

typedef struct _tag_User_Unit
{
	CString		szID;		// ID
	CString		szName;		// Name
	uint8_t		nAuthority;	// Authority
	CString		szPassword;	// Password

	_tag_User_Unit()
	{
		nAuthority = 0;
	};

	_tag_User_Unit operator= (const _tag_User_Unit& ref)
	{
		szID		= ref.szID;
		szName		= ref.szName;
		nAuthority	= ref.nAuthority;
		szPassword	= ref.szPassword;

		return *this;
	}

	void Reset()
	{
		szID.Empty();
		szName.Empty();
		nAuthority = 0;
		szPassword.Empty();
	};

}ST_User_Unit, *PST_User_Unit;


//#define ST_UserInfo		std::vector<ST_User_Unit>

typedef struct _tag_UserInfo
{
public:
	std::vector<ST_User_Unit>	m_Users;

	_tag_UserInfo()
	{
	};

	_tag_UserInfo operator= (const _tag_UserInfo& ref)
	{
		m_Users.clear();

		m_Users = ref.m_Users;

		return *this;
	}

	void Reset()
	{
		m_Users.clear();
	};

	bool IsExist_Name(__in LPCTSTR IN_szName)
	{
		for (UINT nIdx = 0; nIdx < m_Users.size(); nIdx++)
		{
			if (0 == m_Users.at(nIdx).szName.Compare(IN_szName))
			{
				return true;
			}
		}

		return false;
	}

	bool IsExist_ID(__in LPCTSTR IN_szID)
	{
		for (UINT nIdx = 0; nIdx < m_Users.size(); nIdx++)
		{
			if (0 == m_Users.at(nIdx).szID.Compare(IN_szID))
			{
				return true;
			}
		}

		return false;
	}

	void Set_Default_Users()
	{
		if (false == IsExist_ID(_T("Admin")))
		{
			ST_User_Unit newUser;
		
			newUser.szID		= _T("Admin");				// ID
			newUser.szName		= _T("Administrator");		// Name
			newUser.nAuthority	= enPermissionMode::Permission_Administrator;	// Authority
			newUser.szPassword	= _T("Admin");				// Password
		
			m_Users.push_back(newUser);
		}
	};

}ST_UserInfo, *PST_UserInfo;



// 파일 포맷
typedef struct _tag_User_FileFormat
{
	char	ID[100];		// ID
	char	Name[100];		// Name
	uint8_t	nAuthority;		// Authority
	char	Password[21];	// Password
	uint8_t	PasswordLength;	// Password Length
	                        
}ST_User_FileFormat, *PST_User_FileFormat;


// File 저장


#endif // Def_UserInfo_h__
