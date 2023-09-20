//*****************************************************************************
// Filename	: 	Rework_Info.h
// Created	:	2022/2/9 - 19:35
// Modified	:	2022/2/9 - 19:35
//
// Author	:	piring
//	
// Purpose	:	
//*****************************************************************************
#ifndef Rework_Info_h__
#define Rework_Info_h__

#pragma once

#include <map>
#include "Def_ProductInfo.h"

class  CRegRework;

//-----------------------------------------------------------------------------
// CRework_Info
//-----------------------------------------------------------------------------
class CRework_Info
{
public:
	CRework_Info();
	~CRework_Info();

protected:

	CRegRework*		m_pReg		= nullptr;
	
public:
	// Fail 제품의 불량 이력
#ifdef _UNICODE
	std::map<std::wstring, ST_ReworkProduct>	m_ReworkInfo;
#else	
	std::map<std::string, ST_ReworkProduct>		m_ReworkInfo;
#endif

protected:
	bool	Save_ReworkProduct				(__in LPCTSTR IN_szBarcode, __in ST_ReworkProduct* IN_pFailProduct);
	bool	Load_ReworkProduct				(__in LPCTSTR IN_szBarcode, __out ST_ReworkProduct& OUT_FailProduct);
	bool	Remove_ReworkProduct			(__in LPCTSTR IN_szBarcode);
	
	bool	Save_ReworkInfo					();
	bool	Load_ReworkInfo					();
	bool	Remove_ReworkInfo				();
	
	// 제품 갯수 제한, 오래된 정보 제한 (프로그램 시작시 or 하루마다)
	bool	Remove_ReworkInfo_Old			();

public:

	bool					Is_Rework			(__in LPCTSTR IN_szBarcode);

	ST_ReworkProduct&		Get_ReworkProduct	(__in LPCTSTR IN_szBarcode);
	const ST_ReworkProduct&	Get_ReworkProduct	(__in LPCTSTR IN_szBarcode) const;

	bool	Add_ReworkProduct					(__in LPCTSTR IN_szBarcode, __in uint8_t IN_nEqpType, __in uint16_t IN_nNG_Code, __in SYSTEMTIME* IN_pInputTime);
	bool	Add_ReworkProduct					(__in LPCTSTR IN_szBarcode, __in ST_ReworkProduct* IN_pFailProduct);
	bool	Delete_ReworkProduct				(__in LPCTSTR IN_szBarcode);
	bool	Delete_ReworkInfo					();

	
	

};

#endif // Rework_Info_h__
