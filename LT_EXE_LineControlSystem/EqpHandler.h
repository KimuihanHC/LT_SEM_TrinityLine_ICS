//*****************************************************************************
// Filename	: 	EqpHandler.h
// Created	:	2022/06/25 - 10:28
// Modified	:	2022/06/25 - 10:28
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#ifndef EqpHandler_h__
#define EqpHandler_h__


#include "Equipment.h"

#pragma once

//-----------------------------------------------------------------------------
// CEqpHandler
//-----------------------------------------------------------------------------
class CEqpHandler : public CEquipment
{
public:
	CEqpHandler();
	virtual ~CEqpHandler();

	CEqpHandler& operator= (const CConfig_Eqp& ref)
	{
		CConfig_Eqp::operator=(ref);

		return *this;
	};

	CEqpHandler& operator= (const CEquipment& ref)
	{
		CEquipment::operator=(ref);

		return *this;
	};

	CEqpHandler& operator= (const CEqpHandler& ref)
	{
		CEquipment::operator=(ref);

		return *this;
	};

public:
	

};

#endif // EqpHandler_h__
