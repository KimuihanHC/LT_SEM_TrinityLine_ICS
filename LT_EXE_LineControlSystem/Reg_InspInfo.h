//*****************************************************************************
// Filename	: 	Reg_InspInfo.h
// Created	:	2016/3/31 - 16:33
// Modified	:	2016/3/31 - 16:33
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
#ifndef Reg_InspInfo_h__
#define Reg_InspInfo_h__

#pragma once

#include "Def_ModelConfig.h"
#include "Def_UserInfo.h"

class CReg_InspInfo
{
public:
	CReg_InspInfo();
	CReg_InspInfo(__in LPCTSTR lpszRegPath);
	~CReg_InspInfo();

protected:
	

	CString		m_szRegPath;

public:
	void		Set_RegitryPath(__in LPCTSTR lpszRegPath)
	{
		m_szRegPath = lpszRegPath;
	}

	// Password 2차
	BOOL	SavePassword				(__in UINT nIndex, __in CString szPassword);
	CString LoadPassword				(__in UINT nIndex);

	// 검사기 선택
	bool	Set_ModelTypeInfo			(__in const CConfig_Model* IN_pModelTypeInfo);
	bool	Get_ModelTypeInfo			(__out CConfig_Model& OUT_ModelTypeInto);

	// 사용자 정보
	bool	Set_UserInfo				(__in const ST_UserInfo* IN_pUserInfo);
	bool	Get_UserInfo				(__out ST_UserInfo& OUT_UserInto);

	bool	Set_LogIn_User				(__in const ST_User_Unit IN_pUser);
	bool	Get_LogIn_User				(__out ST_User_Unit& OUT_stUser);
	bool	Set_LogOut_User				();
	uint8_t	Get_Permission				();


	// 이전 프로그램 실행시 비정상 종료 체크 (1이면  비정상 종료)
	bool	Is_ProgramCrash				();
	// 프로그램 시작 Flag 설정 (1: 시작, 0: 종료)
	bool	Set_ProgramLoading			(__in bool IN_bStart);
	

};

#endif // Reg_InspInfo_h__
