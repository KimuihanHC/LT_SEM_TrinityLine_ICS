//*****************************************************************************
// Filename	: LT_Option.h
// Created	: 2015/12/16
// Modified	: 2015/12/16
//
// Author	: PiRing
//	
// Purpose	: 옵션 처리를 위한 클래스
//*****************************************************************************
#ifndef LT_Option_h__
#define LT_Option_h__

#pragma once

#include "Define_Option.h"
#include "Def_Enum_Cm.h"

using namespace Luritech_Option;

//=============================================================================
//
//=============================================================================
class CLT_Option
{
public:
	CLT_Option();
	~CLT_Option();

	CLT_Option& operator= (CLT_Option& ref)
	{
		m_stOption			= ref.m_stOption;
		m_strRegPath_Base	= ref.m_strRegPath_Base;
		m_InsptrType		= ref.m_InsptrType;

		return *this;
	};

protected:
	// 옵션 구조체
	stLT_Option			m_stOption;

	// 레지스트리 저장 경로
	CString				m_strRegPath_Base;

	// 검사기 설정
	enInsptrSysType		m_InsptrType;

public:
	void		SetRegistryPath			(__in LPCTSTR lpszRegPath);

	// 기본 검사 설정
	void		SaveOption_Inspector	(__in stOpt_Insp stOption);
	BOOL		LoadOption_Inspector	(__out stOpt_Insp& stOption);

#if (20230905)
	void		SaveOption_Server		(__in unsigned nType, __in  stOpt_Server stOption);
	BOOL		LoadOption_Server		(__in unsigned nType, __out stOpt_Server& stOption);
#endif
	// 기타 설정
	void		SaveOption_Misc			(__in stOpt_Misc stOption);
	BOOL		LoadOption_Misc			(__out stOpt_Misc& stOption);

	// 전체 옵션 저장/불러오기
	void		SaveOption_All			(__in stLT_Option stOption);
	BOOL		LoadOption_All			(__out stLT_Option& stOption);

	// 검사기 구분
	void		SetInspectorType		(__in enInsptrSysType nInsptrType);

};

#endif // LT_Option_h__

