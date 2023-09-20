//*****************************************************************************
// Filename	: 	RegEquipment.h
// Created	:	2021/12/21 - 13:45
// Modified	:	2021/12/21 - 13:45
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#ifndef RegCommModule_h__
#define RegCommModule_h__
#pragma once

#include <stdint.h>
#include "Equipment.h"
#include "Server.h"

class CEquipment;
class CServer;

//-----------------------------------------------------------------------------
// CRegEquipment
//-----------------------------------------------------------------------------
class CRegCommModule
{
public:
	CRegCommModule();
	CRegCommModule(__in LPCTSTR lpszRegPath);
	~CRegCommModule();

protected:
	CString		m_szRegModulePath;

public:
//#if (USE_XML)
	bool	Save_Equipment_ID(__in const CEquipment* IN_pEquipment);
	bool	Load_Equipment_ID(__in LPCTSTR IN_szEqpID, __out CEquipment& OUT_Equipment);
	bool	Save_Equipment_ID(__in const CServer* IN_pServer);
	bool	Load_Equipment_ID(__in LPCTSTR IN_szSvrID, __out CServer& OUT_Server);

	bool	Save_Equipment_ID(__in LPCTSTR IN_szEqpID, __in uint8_t IN_nPortIndex, __in uint8_t Type, LPCTSTR IN_Data);
	CString	Load_Equipment_ID(__in LPCTSTR IN_szEqpID, __in uint8_t IN_nPortIndex, __in uint8_t Type);
//#endif
};
#endif // RegCommModule_h__
