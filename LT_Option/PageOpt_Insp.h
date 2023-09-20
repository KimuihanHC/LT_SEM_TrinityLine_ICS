//*****************************************************************************
// Filename	: PageOpt_Insp.h
// Created	: 2010/9/6
// Modified	: 2010/9/6 - 15:48
//
// Author	: PiRing
//	
// Purpose	: 
//*****************************************************************************
#ifndef PageOpt_Insp_h__
#define PageOpt_Insp_h__

#pragma once

#include "PageOption.h"
#include "Define_Option.h"

using namespace Luritech_Option;

class CPageOpt_Insp : public CPageOption
{
	DECLARE_DYNAMIC(CPageOpt_Insp)

public:
	CPageOpt_Insp			(LPCTSTR lpszCaption = NULL);
	CPageOpt_Insp			(UINT nIDTemplate, UINT nIDCaption = 0);
	virtual ~CPageOpt_Insp	(void);

protected:

	DECLARE_MESSAGE_MAP()

	virtual void		AdjustLayout		();
	virtual void		SetPropListFont		();
	virtual void		InitPropList		();

	stOpt_Insp	m_stOption;

	stOpt_Insp	Get_Option			();
	void				Set_Option			( stOpt_Insp stOption);

public:

	virtual void		SaveOption			();
	virtual void		LoadOption			();
};

#endif // PageOpt_Insp_h__
