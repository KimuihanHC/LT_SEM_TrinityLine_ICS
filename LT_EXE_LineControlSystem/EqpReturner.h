//*****************************************************************************
// Filename	: 	EqpReturner.h
// Created	:	2021/11/19 - 10:28
// Modified	:	2021/11/19 - 10:28
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#ifndef EqpReturner_h__
#define EqpReturner_h__

#pragma once

#include "Equipment.h"

//-----------------------------------------------------------------------------
// CEqpReturner
//-----------------------------------------------------------------------------
class CEqpReturner : public CEquipment
{
public:
	CEqpReturner();
	virtual ~CEqpReturner();

	CEqpReturner& operator= (const CConfig_Eqp& ref)
	{
		CConfig_Eqp::operator=(ref);

		return *this;
	};

	CEqpReturner& operator= (const CEquipment& ref)
	{
		CEquipment::operator=(ref);

		return *this;
	};

	CEqpReturner& operator= (const CEqpReturner& ref)
	{
		CEquipment::operator=(ref);

		return *this;
	};

protected:
// 	uint8_t		m_PortStatus[PtI_R_MaxCount];
// 	uint8_t		m_ConveyorStatus[CvI_R_MaxCount];

public:

	//-----------------------------------------------------
	// Returner
	//-----------------------------------------------------
	// PtI_R_Buffer_1,
	// PtI_R_Transfer,
	// PtI_R_Buffer_2,

	// CvI_R_Buffer_1,
	// CvI_R_Transfer,
	// CvI_R_Buffer_2,


};

#endif // EqpReturner_h__
