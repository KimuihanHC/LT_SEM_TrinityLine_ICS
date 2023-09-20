//*****************************************************************************
// Filename	: 	EqpLoader.h
// Created	:	2021/11/19 - 10:28
// Modified	:	2021/11/19 - 10:28
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#ifndef EqpLoader_h__
#define EqpLoader_h__

#pragma once

#include "Equipment.h"

//-----------------------------------------------------------------------------
// CEqpLoader
//-----------------------------------------------------------------------------
class CEqpLoader : public CEquipment
{
public:
	CEqpLoader();
	virtual ~CEqpLoader();

	CEqpLoader& operator= (const CConfig_Eqp& ref)
	{
		CConfig_Eqp::operator=(ref);

		return *this;
	};

	CEqpLoader& operator= (const CEquipment& ref)
	{
		CEquipment::operator=(ref);

		return *this;
	};

	CEqpLoader& operator= (const CEqpLoader& ref)
	{
		CEquipment::operator=(ref);

		return *this;
	};

protected:
// 	uint8_t		m_PortStatus[PtI_L_MaxCount];
// 	uint8_t		m_ConveyorStatus[CvI_L_MaxCount];

public:

	virtual void	Set_PortStatus		(__in uint8_t IN_nPortIndex, __in uint8_t IN_nStatus, __in LPCTSTR IN_szRFID, __in LPCTSTR IN_szBarcode, __in bool IN_bSave = true);
	//-----------------------------------------------------
	// Loader
	//-----------------------------------------------------
	// 	PtI_L_Load,
	// 	PtI_L_RFID,
	// 	PtI_L_Transfer,
	// 	PtI_L_Buffer_1,
	// 	PtI_L_Buffer_2,
	// 	PtI_L_Buffer_3,
	// 	PtI_L_Un_RFID,
	// 	PtI_L_Un_NG,
	// 	PtI_L_Un_OK_Wait,
	// 	PtI_L_Un_OK,
	// 	PtI_L_Un_Clean,

	// 	CvI_L_Load,
	// 	CvI_L_RFID,
	// 	CvI_L_Transfer,
	// 	CvI_L_Buffer_1,
	// 	CvI_L_Buffer_2,
	// 	CvI_L_Buffer_3,
	// 	CvI_L_Un_RFID,
	// 	CvI_L_Un_NG,
	// 	CvI_L_Un_OK,
	// 	CvI_L_Un_Clean,

};

#endif // EqpLoader_h__
