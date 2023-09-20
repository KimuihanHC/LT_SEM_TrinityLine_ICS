//*****************************************************************************
// Filename	: 	BannerBar_SEM.h
// Created	:	2021/11/16 - 17:21
// Modified	:	2021/11/16 - 17:21
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************

#pragma once

#include "NTBannerBar.h"

//-----------------------------------------------------------------------------
// CBannerBar_SEM
//-----------------------------------------------------------------------------
class CBannerBar_SEM : public CNTBannerBar
{
	DECLARE_DYNAMIC(CBannerBar_SEM)

public:
	CBannerBar_SEM();
	virtual ~CBannerBar_SEM();

	virtual BOOL	CreateEx				(CWnd* pParentWnd, UINT nID = uiBannerBarID);

protected:
	afx_msg void	OnSize					(UINT nType, int cx, int cy);
	afx_msg void	OnExitProgram			();
	afx_msg void	OnOption				();

	DECLARE_MESSAGE_MAP()

	

	// Login Button
	// User
	// Operator / Administrator


public:




};

