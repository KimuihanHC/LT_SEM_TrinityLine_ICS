//*****************************************************************************
// Filename	: 	RegEquipment.h
// Created	:	2021/12/21 - 13:45
// Modified	:	2021/12/21 - 13:45
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#ifndef RegEquipment_h__
#define RegEquipment_h__


#pragma once

#include <stdint.h>

#if (USE_XML)
#include "RegCommModule.h"
#endif

class CEquipment;

//-----------------------------------------------------------------------------
// CRegEquipment
//-----------------------------------------------------------------------------
#if (USE_XML)
class CRegEquipment : public CRegCommModule
#else 
class CRegEquipment
#endif
{
public:
	CRegEquipment();
	CRegEquipment(__in LPCTSTR lpszRegPath);
	~CRegEquipment();

protected:
	CString		m_szRegPath;

public:
	void	Set_RegistryPath			(__in LPCTSTR lpszRegPath);
	CString	Get_RegistryPath			() const;

	// 레지스트리 패스 체크하여 없으면 생성
	bool	Check_RegistryPath			(__in LPCTSTR IN_szEqpID);

	// 레지스트리에 소켓정보 저장
	bool	Save_Equipment				(__in const CEquipment* IN_pEquipment);
	
	bool	Set_Equipment_Skip			(__in const CEquipment* IN_pEquipment);
	bool	Set_Equipment_Reserve		(__in const CEquipment* IN_pEquipment);
	bool	Set_Equipment_ReserveQueue	(__in const CEquipment* IN_pEquipment);
	bool	Set_Equipment_EndProduction	(__in const CEquipment* IN_pEquipment);
	bool	Set_Equipment_Shift			(__in const CEquipment* IN_pEquipment);

	bool	Set_Equipment_Port			(__in const CEquipment* IN_pEquipment);
	bool	Set_Equipment_Port			(__in const CEquipment* IN_pEquipment, __in uint8_t IN_nPortIndex);
	bool	Set_Equipment_Conveyor		(__in const CEquipment* IN_pEquipment);
	bool	Set_Equipment_Conveyor		(__in const CEquipment* IN_pEquipment, __in uint8_t IN_ConveyorIndex);
	bool	Set_Equipment_Alarm			(__in const CEquipment* IN_pEquipment);
	bool	Set_Equipment_Yield			(__in const CEquipment* IN_pEquipment);


	// 레지스트리에 저장된 소켓 정보 가져오기
	bool	Load_Equipment				(__in LPCTSTR IN_szEqpID, __out CEquipment& OUT_Equipment);
	bool	Load_Yield					(__in LPCTSTR IN_szEqpID, __out CEquipment& OUT_Equipment);


};


#endif // RegEquipment_h__
