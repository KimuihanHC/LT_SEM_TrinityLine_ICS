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
}

CEqpHandler::~CEqpHandler()
{
}
