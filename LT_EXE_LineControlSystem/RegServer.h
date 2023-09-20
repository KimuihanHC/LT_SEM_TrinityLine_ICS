//*****************************************************************************
// Filename	: 	RegEquipment.h
// Created	:	2021/12/21 - 13:45
// Modified	:	2021/12/21 - 13:45
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#ifndef RegServer_h__
#define RegServer_h__
#pragma once
#include <stdint.h>

#include "RegCommModule.h"
class CServer;

//-----------------------------------------------------------------------------
// CRegEquipment
//-----------------------------------------------------------------------------
class CRegServer : public CRegCommModule
{
public:
	CRegServer();
	CRegServer(__in LPCTSTR lpszRegPath);
	~CRegServer();

protected:
	CString		m_szRegPath;

public:
	void	Set_RegistryPath			(__in LPCTSTR lpszRegPath);
	CString	Get_RegistryPath			() const;

	// ?àÏ??§Ìä∏Î¶??®Ïä§ Ï≤¥ÌÅ¨?òÏó¨ ?ÜÏúºÎ©??ùÏÑ±
	bool	Check_RegistryPath			(__in LPCTSTR IN_szEqpID);
#if SOCKET
	// ?àÏ??§Ìä∏Î¶¨Ïóê ?åÏºì?ïÎ≥¥ ?Ä??/
	bool	Save_Equipment				(__in const CServer* IN_pEquipment);	
	bool	Set_Equipment_Skip			(__in const CServer* IN_pEquipment);
#endif//SOCKET
	bool	Set_Equipment_Reserve		(__in const CServer* IN_pEquipment);
#if SOCKET
	bool	Set_Equipment_EndProduction	(__in const CServer* IN_pEquipment);
	bool	Set_Equipment_Shift			(__in const CServer* IN_pEquipment);

	bool	Set_Equipment_Port			(__in const CServer* IN_pEquipment);
	bool	Set_Equipment_Port			(__in const CServer* IN_pEquipment, __in uint8_t IN_nPortIndex);
	bool	Set_Equipment_Conveyor		(__in const CServer* IN_pEquipment);
	bool	Set_Equipment_Conveyor		(__in const CServer* IN_pEquipment, __in uint8_t IN_ConveyorIndex);
	bool	Set_Equipment_Alarm			(__in const CServer* IN_pEquipment);
	bool	Set_Equipment_Yield			(__in const CServer* IN_pEquipment);


	// ?àÏ??§Ìä∏Î¶¨Ïóê ?Ä?•Îêú ?åÏºì ?ïÎ≥¥ Í∞Ä?∏Ïò§Í∏?/
	bool	Load_Equipment				(__in LPCTSTR IN_szEqpID, __out CServer& OUT_Equipment);
	bool	Load_Yield					(__in LPCTSTR IN_szEqpID, __out CServer& OUT_Equipment);
T
	//-----------------------------------------------------------------------------
	// Add
	//-----------------------------------------------------------------------------	
public:
	bool	Save_Equipment_ID(__in const CServer* IN_pEquipment);
	bool	Load_Equipment_ID(__in LPCTSTR IN_szEqpID, __out CServer& OUT_Equipment);

	bool	Save_Equipment_ID(__in LPCTSTR IN_szEqpID, __in uint8_t IN_nPortIndex, __in uint8_t Type, LPCTSTR IN_Data);
	CString	Load_Equipment_ID(__in LPCTSTR IN_szEqpID, __in uint8_t IN_nPortIndex, __in uint8_t Type);
#endif//SOCKE

};

#endif // RegServer_h__