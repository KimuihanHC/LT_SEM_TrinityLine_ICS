//*****************************************************************************
// Filename	: 	Def_Config_ALID.h
// Created	:	
// Modified	:	2023.04.20
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#ifndef Def_Config_ALID_h__
#define Def_Config_ALID_h__

#pragma once

#include <afxwin.h>
#include <stdint.h>
#include "Def_ID_Type.h"

class CConfig_AL
{
protected:
	uint32_t	m_nALID;	//Alarm ID.
	uint32_t	m_nALCD;	//Alarm ?„í—˜??
	CString		m_nALTX;	//Alarm Text.

public:
	CConfig_AL()	{
		uint32_t		m_nALID = 0;
		uint32_t		m_nALCD = 0;
		CString		m_nALTX = _T("");
	};

	~CConfig_AL()	{
	};

	CConfig_AL& operator= (const CConfig_AL& ref){		
		m_nALID = ref.m_nALID;
		m_nALCD = ref.m_nALCD;
		m_nALTX = ref.m_nALTX;
		return *this;
	};

	void Reset(){
		uint32_t		m_nALID = 0;
		uint32_t		m_nALCD = 0;
		CString		m_nALTX = _T("");
	};
public:
	uint32_t		Get_ALID() const{
		return m_nALID;	};
	uint32_t		Get_ALCD() const{
		return m_nALCD;	};
	CString		Get_ALTX() const{
		return m_nALTX;	};


	void		Set_ALID(__in uint32_t	IN_nALID){
		m_nALID = IN_nALID;	};
	void		Set_ALCD(__in uint32_t	IN_nALCD){
		m_nALCD = IN_nALCD;	};
	void		Set_ALTX(__in LPCTSTR	IN_nALTX){
		m_nALTX = IN_nALTX;	};
};
#endif // Def_Config_ALID_h__
