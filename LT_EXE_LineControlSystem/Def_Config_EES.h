//*****************************************************************************
// Filename	: 	Def_Config_EES.h
// Created	:	
// Modified	:	2023.04.20
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#ifndef Def_Config_EES_h__
#define Def_Config_EES_h__

#pragma once

#include <afxwin.h>
#include <stdint.h>
#include "Def_ID_Type.h"

class CConfig_EES
{
protected:
	CString		m_szEES_EquipMentStateCode;
	CString		m_szEES_EquipMentStateKorText;
	CString		m_szEES_EquipMentStateEngText;
	CString		m_szEES_LossCode;
	CString		m_szEES_StateColor;
	uint16_t	m_szEES_Use;
	CString		m_szEES_explanation;

public:
	CConfig_EES()	{
		CString		m_szEES_EquipMentStateCode		= _T("");
		CString		m_szEES_EquipMentStateKorText	= _T("");
		CString		m_szEES_EquipMentStateEngText	= _T("");
		CString		m_szEES_LossCode				= _T("");
		CString		m_szEES_StateColor				= _T("");
		uint16_t	m_szEES_Use						= 0;
		CString		m_szEES_explanation				= _T("");
	};
	~CConfig_EES()
	{

	};
	CConfig_EES& operator= (const CConfig_EES& ref)
	{
		m_szEES_EquipMentStateCode = ref.m_szEES_EquipMentStateCode;
		m_szEES_EquipMentStateKorText = ref.m_szEES_EquipMentStateKorText;
		m_szEES_EquipMentStateEngText = ref.m_szEES_EquipMentStateEngText;
		m_szEES_LossCode = ref.m_szEES_LossCode;
		m_szEES_StateColor = ref.m_szEES_StateColor;
		m_szEES_Use = ref.m_szEES_Use;
		m_szEES_explanation = ref.m_szEES_explanation;

		return *this;
	};
	void Reset()
	{
		CString		m_szEES_EquipMentStateCode = _T("");
		CString		m_szEES_EquipMentStateKorText = _T("");
		CString		m_szEES_EquipMentStateEngText = _T("");
		CString		m_szEES_LossCode = _T("");
		CString		m_szEES_StateColor = _T("");
		uint16_t	m_szEES_Use = 0;
		CString		m_szEES_explanation = _T("");
	};
protected:
	virtual void Init_ValueType_UI()
	{

	};

public:
	CString		Get_EquipMentStateCode() const{
		return m_szEES_EquipMentStateCode;};
	CString		Get_EquipMentStateKorText() const{
		return m_szEES_EquipMentStateKorText;};
	CString		Get_EquipMentStateEngText() const{
		return m_szEES_EquipMentStateEngText;};
	CString		Get_LossCode() const{
		return m_szEES_LossCode;};
	CString		Get_StateColor() const{
		return m_szEES_StateColor;};
	uint16_t	Get_Use() const{
		return m_szEES_Use;};
	CString		Get_Explanation() const{
		return m_szEES_explanation;};

	void		Set_EquipMentStateCode(__in LPCTSTR	IN_Data) {
		m_szEES_EquipMentStateCode = IN_Data;		};
	void		Set_EquipMentStateKorText(__in LPCTSTR	IN_Data) {
		m_szEES_EquipMentStateKorText = IN_Data;	};
	void		Set_EquipMentStateEngText(__in LPCTSTR	IN_Data) {
		m_szEES_EquipMentStateEngText = IN_Data;	};
	void		Set_LossCode(__in LPCTSTR	IN_Data)  {
		m_szEES_LossCode = IN_Data;	};
	void		Set_StateColor(__in LPCTSTR		IN_Data)  {
		m_szEES_StateColor = IN_Data;	};
	void		Set_Use(__in uint16_t	IN_Data)  {
		m_szEES_Use	= IN_Data;	};
	void		Set_Explanation(__in LPCTSTR	IN_Data)  {
		m_szEES_explanation = IN_Data;	};
}; // CConfig_ID
#endif // Def_Config_EES_h__
