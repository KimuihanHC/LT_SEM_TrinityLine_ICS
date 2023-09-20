//*****************************************************************************
// Filename	: 	EqpHandler.cpp
// Created	:	2022/8/12 - 12:13
// Modified	:	2022/8/12 - 12:13
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#include "stdafx.h"
#include "EqpHandler.h"

CEqpHandler::CEqpHandler()
{
	//m_nPortCount = PtI_L_MaxCount;
	//m_nConveyorCount = CvI_L_MaxCount;

	m_nPortStatus.assign(PtI_L_MaxCount, { 0, _T("") });
	m_nConveyorStatus.assign(CvI_L_MaxCount, { 0, 0, _T("") });

#if (USE_XML)
	assign_mEES_PortSubStatus(PtI_T_MaxCount, {});
	assign_EquipmentIDStatus(PtI_T_MaxCount, {});
#endif
#if SOCKET
	m_nOldPortStatus.assign(PtI_L_MaxCount, { 0, _T("") });
	m_nEquipmentID.assign(PtI_L_MaxCount, { _T("") , _T("") });
#endif//SOCKET
}

CEqpHandler::~CEqpHandler()
{
}
