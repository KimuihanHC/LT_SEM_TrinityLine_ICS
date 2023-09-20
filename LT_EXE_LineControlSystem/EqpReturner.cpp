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

}

CEqpReturner::~CEqpReturner()
{

}
