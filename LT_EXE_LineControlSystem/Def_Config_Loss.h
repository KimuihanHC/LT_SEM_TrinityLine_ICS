//*****************************************************************************
// Filename	: 	Def_Config_Loss.h
// Created	:	
// Modified	:	2023.04.20
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#ifndef Def_Config_Loss_h__
#define Def_Config_Loss_h__

#pragma once

#include <afxwin.h>
#include <stdint.h>
#include "Def_ID_Type.h"
//-----------------------------------------------------------------------------
// EES 기본 구조�?//-----------------------------------------------------------------------------
class CConfig_Loss
{
protected:
	CString		m_szNameLoss;
	CString		m_szTem;
	CString		m_szCodeLoss;
	CString		m_szNameLossKor;
	CString		m_szExplainLoss;

public:
	CConfig_Loss()	{
		CString		m_szNameLoss = _T("");
		CString		m_szTem = _T("");
		CString		m_szCodeLoss = _T("");
		CString		m_szNameLossName = _T("");
		CString		m_szExplainLoss = _T("");
	};
	~CConfig_Loss()
	{

	};
	CConfig_Loss& operator= (const CConfig_Loss& ref)
	{
		m_szNameLoss = ref.m_szNameLoss;
		m_szTem = ref.m_szTem;
		m_szCodeLoss = ref.m_szCodeLoss;
		m_szNameLossKor = ref.m_szNameLossKor;
		m_szExplainLoss = ref.m_szExplainLoss;

		return *this;
	};
	void Reset()
	{
		CString		m_szNameLoss = _T("");
		CString		m_szTem = _T("");
		CString		m_szCodeLoss = _T("");
		CString		m_szNameLossKor = _T("");
		CString		m_szExplainLoss = _T("");
	};
protected:
	virtual void Init_ValueType_UI()	{
	};

public:
	CString		Get_NameLoss() const{
		return m_szNameLoss;};
	CString		Get_Tem() const{
		return m_szTem;};
	CString		Get_CodeLoss() const{
		return m_szCodeLoss;};
	CString		Get_NameLossEng() const{
		return m_szNameLossKor;};
	CString		Get_Explain() const{
		return m_szExplainLoss;};


	void		Set_NameLoss(__in LPCTSTR	IN_Data) {
		m_szNameLoss = IN_Data;		};
	void		Set_Tem(__in LPCTSTR	IN_Data) {
		m_szTem = IN_Data;	};
	void		Set_CodeLoss(__in LPCTSTR	IN_Data) {
		m_szCodeLoss = IN_Data;	};
	void		Set_NameLossEng(__in LPCTSTR	IN_Data)  {
		m_szNameLossKor = IN_Data;	};
	void		Set_Explain(__in LPCTSTR	IN_Data)  {
		m_szExplainLoss = IN_Data;	};
}; // CConfig_ID
#endif // Def_Config_Loss_h__
