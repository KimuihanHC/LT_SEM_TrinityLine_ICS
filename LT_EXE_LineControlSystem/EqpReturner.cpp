//*****************************************************************************
// Filename	: 	EqpReturner.cpp
// Created	:	2021/11/19 - 10:35
// Modified	:	2021/11/19 - 10:35
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#include "stdafx.h"
#include "EqpReturner.h"

CEqpReturner::CEqpReturner()
{
	//m_nPortCount = PtI_R_MaxCount;
	//m_nConveyorCount = CvI_R_MaxCount;

	m_nPortStatus.assign(PtI_R_MaxCount, { 0, _T("") });
	m_nConveyorStatus.assign(CvI_R_MaxCount, { 0, 0, _T("") });

#if (USE_XML)
	assign_mEES_PortSubStatus(PtI_T_MaxCount, {});
	assign_EquipmentIDStatus(PtI_T_MaxCount, {});
#endif
#if SOCKET
	m_nOldPortStatus.assign(PtI_R_MaxCount, { 0, _T("") });
	m_nEquipmentID.assign(PtI_R_MaxCount, { _T("") , _T("") });
#endif
}

CEqpReturner::~CEqpReturner()
{

}
