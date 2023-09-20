//*****************************************************************************
// Filename	: 	RegDebugInfo.h
// Created	:	2021/12/21 - 13:45
// Modified	:	2021/12/21 - 13:45
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#ifndef RegDebugInfo_h__
#define RegDebugInfo_h__


#pragma once

#include "Def_DebugInfo.h"


//-----------------------------------------------------------------------------
// CRegDebugInfo
//-----------------------------------------------------------------------------
class CRegDebugInfo
{
public:
	CRegDebugInfo();
	CRegDebugInfo(__in LPCTSTR lpszRegPath);
	~CRegDebugInfo();

protected:
	CString		m_szRegPath;

public:
	void	Set_RegitryPath				(__in LPCTSTR lpszRegPath);

	// 레지스트리에 디버그 정보 저장
	bool	Set_DebugInfo				(__in const ST_DebugInfo* IN_pReworkPrd);
	
	// 레지스트리에 저장된 디버그 정보 가져오기
	bool	Get_DebugInfo				(__out ST_DebugInfo& OUT_ReworkPrd);

};


#endif // RegDebugInfo_h__
