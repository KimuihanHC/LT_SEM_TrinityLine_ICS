//*****************************************************************************
// Filename	: 	NGCode_Info.h
// Created	:	2021/10/21 - 15:15
// Modified	:	2021/10/21 - 15:15
//
// Author	:	piring
//	
// Purpose	:	
//*****************************************************************************
#ifndef NGCode_Info_h__
#define NGCode_Info_h__

#pragma once

#include <map>
#include "Def_NGCode.h"

//-----------------------------------------------------------------------------
// CNGCode_Info
//-----------------------------------------------------------------------------
class CNGCode_Info
{
public:
	CNGCode_Info();
	virtual ~CNGCode_Info();

	// NG Code 데이터.
	std::map<int16_t, ST_NGCode>	m_NG_Info;

	// MES rework NG Code
	int16_t		m_nMES_ReworkNGCode = -2;

private:
	void		MakeDirectory		(__in LPCTSTR szPath);
	BOOL		Backup_File			(__in LPCTSTR szPath);

public:

	bool		Save_IniFile_MES	(__in LPCTSTR szPath, __in int16_t IN_nMES_ReworkCode);
	bool		Load_IniFile_MES	(__in LPCTSTR szPath, __out int16_t& OUT_nMES_ReworkCode);

	bool		Save_IniFile		(__in LPCTSTR szPath, __in const std::map<int16_t, ST_NGCode>* IN_pNGCode_Info);
	bool		Save_IniFile		(__in LPCTSTR szPath);
	bool		Load_IniFile		(__in LPCTSTR szPath, __out std::map<int16_t, ST_NGCode>& OUT_NGCode_Info);
	bool		Load_IniFile		(__in LPCTSTR szPath);

	// 버튼 제어 함수
	bool		Insert				(__in ST_NGCode* IN_pNGCode);
	bool		Remove				(__in int16_t IN_nNG_Code);
	bool		Modify				(__in ST_NGCode* IN_pNGCode);

	void		Clear				();

public:

	bool				IsExist		(__in int16_t IN_nNG_Code);
	ST_NGCode&			Get_NGCode	(__in int16_t IN_nNG_Code);
	const ST_NGCode&	Get_NGCode	(__in int16_t IN_nNG_Code) const;
	
};

#endif // NGCode_Info_h__


