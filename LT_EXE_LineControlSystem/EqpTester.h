//*****************************************************************************
// Filename	: 	EqpTester.h
// Created	:	2021/11/19 - 10:28
// Modified	:	2021/11/19 - 10:28
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#ifndef EqpTester_h__
#define EqpTester_h__

#pragma once

#include "Equipment.h"

//-----------------------------------------------------------------------------
// CEqpTester
//-----------------------------------------------------------------------------
class CEqpTester : public CEquipment
{
public:
	CEqpTester();
	virtual ~CEqpTester();

	CEqpTester& operator= (const CConfig_Eqp& ref)
	{
		CConfig_Eqp::operator=(ref);

		return *this;
	};

	CEqpTester& operator= (const CEquipment& ref)
	{
		CEquipment::operator=(ref);

		return *this;
	};

	CEqpTester& operator= (const CEqpTester& ref)
	{
		CEquipment::operator=(ref);

		return *this;
	};
#if defined (EES_XML)
	CEqpTester& operator= (const CCommonModule& ref)
	{
		CCommonModule::operator=(ref);

		return *this;
	};
#endif
protected:

	virtual void Init_EquipmentType_UI();
	//virtual void Init_EquipmentType();


// 	uint8_t		m_PortStatus[PtI_T_MaxCount];
// 	uint8_t		m_ConveyorStatus[CvI_T_MaxCount];

	SYSTEMTIME	m_tmInput_Para[Para_MaxCount];

	virtual void Set_Time_InputPara		(__in uint8_t IN_nPara);

public:

	//virtual void Set_PortStatus			(__in uint8_t IN_nPortIndex, __in uint8_t IN_nStatus);
	virtual void Set_PortStatus			(__in uint8_t IN_nPortIndex, __in uint8_t IN_nStatus, __in LPCTSTR IN_szRFID, __in LPCTSTR IN_szBarcode, __in bool IN_bSave = true);

	//virtual void Set_ConveyorStatus		(__in uint8_t IN_nConveyorIndex, __in uint8_t IN_nStatus, __in uint8_t IN_nExistSocket);
	virtual void Set_ConveyorStatus		(__in uint8_t IN_nConveyorIndex, __in uint8_t IN_nStatus, __in uint8_t IN_nExistSocket, __in LPCTSTR IN_szRFID, __in LPCTSTR IN_szBarcode, __in bool IN_bSave = true);
	//-----------------------------------------------------
	// Tester
	//-----------------------------------------------------
	// 	PtI_T_RFID,
	// 	PtI_T_Buffer,
	// 	PtI_T_Test_L,
	// 	PtI_T_Test_R,

	// 	CvI_T_RFID,
	// 	CvI_T_Test,
	// 	CvI_T_Return,

	virtual uint32_t Get_ElapsedTime_InputPara(__in uint8_t IN_nPara);

	void Update_DataSize();
};

#endif // EqpTester_h__
