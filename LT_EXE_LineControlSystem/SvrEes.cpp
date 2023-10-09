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
#include "SvrEes.h"

CSvrEes::CSvrEes()
{
#if defined(EES_XML)//20231003
	assign_mEES_PortSubStatus(PtI_T_MaxCount, {});
	assign_EquipmentIDStatus(PtI_T_MaxCount, {});
#endif
#if SOCKET
	m_nPortStatus.assign(PtI_EES_MaxCount, { 0, _T("") });
	m_nConveyorStatus.assign(PtI_EES_MaxCount, { 0, 0, _T("") });
	m_nOldPortStatus.assign(PtI_EES_MaxCount, { 0, _T("") });
	m_nEquipmentID.assign(PtI_EES_MaxCount, { _T("") , _T("") });
#endif
}
CSvrEes::~CSvrEes()
{
}
