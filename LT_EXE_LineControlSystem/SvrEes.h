//*****************************************************************************
// Filename	: 	EqpEes.h
// Created	:	2022/06/25 - 10:28
// Modified	:	2022/06/25 - 10:28
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#ifndef SvrEes_h__
#define SvrEes_h__


#include "Server.h"

#pragma once

//-----------------------------------------------------------------------------
// CSvrEes
//-----------------------------------------------------------------------------
class CSvrEes : public CServer
{
public:
	CSvrEes();
	virtual ~CSvrEes();

	CSvrEes& operator= (const CConfig_Svr& ref)
	{
		CConfig_Svr::operator=(ref);

		return *this;
	};

	CSvrEes& operator= (const CServer& ref)
	{
		CServer::operator=(ref);

		return *this;
	};

	CSvrEes& operator= (const CSvrEes& ref)
	{
		CServer::operator=(ref);

		return *this;
	};

public:

};

#endif // EqpHandler_h__
