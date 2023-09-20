//*****************************************************************************
// Filename	: 	RegRework.h
// Created	:	2021/12/21 - 13:45
// Modified	:	2021/12/21 - 13:45
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#ifndef RegRework_h__
#define RegRework_h__


#pragma once

#include "Def_ProductInfo.h"
#include <map>

class CRework_Info;

//-----------------------------------------------------------------------------
// CRegRework
//-----------------------------------------------------------------------------
class CRegRework
{
public:
	CRegRework();
	CRegRework(__in LPCTSTR lpszRegPath);
	~CRegRework();

protected:
	CString		m_szRegPath;

	bool	Set_ReworkBarcodeList(__in const CRework_Info* IN_pReworkInfo);

public:
	void	Set_RegitryPath				(__in LPCTSTR lpszRegPath);

	// 레지스트리에 소켓정보 저장//
	bool	Set_ReworkProduct			(__in LPCTSTR IN_szBarcode, __in const ST_ReworkProduct* IN_pReworkPrd);
	bool	Set_ReworkInfo				(__in const CRework_Info* IN_pReworkInfo);
	
	// 레지스트리에 저장된 소켓 정보 가져오기//
	bool	Get_ReworkProduct			(__in LPCTSTR IN_szBarcode, __out ST_ReworkProduct& OUT_ReworkPrd);
#ifdef _UNICODE
	bool	Get_ReworkInfo				(__out std::map<std::wstring, ST_ReworkProduct>& OUT_ReworkInfo);
#else
	bool	Get_ReworkInfo				(__out std::map<std::string, ST_ReworkProduct>& OUT_ReworkInfo);
#endif

	//bool	Remove_ReworkProduct		(__in LPCTSTR IN_szBarcode);
	bool	Remove_ReworkProduct		(__in LPCTSTR IN_szBarcode, __in const CRework_Info* IN_pReworkInfo);
	bool	Remove_ReworkInfo			();

};


#endif // RegRework_h__
