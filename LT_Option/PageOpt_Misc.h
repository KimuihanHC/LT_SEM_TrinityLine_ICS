//*****************************************************************************
// Filename	: PageOpt_Misc.h
// Created	: 2010/9/16
// Modified	: 2010/9/16 - 18:05
//
// Author	: PiRing
//	
// Purpose	: 
//*****************************************************************************
#ifndef PageOpt_Misc_h__
#define PageOpt_Misc_h__

#pragma once
#include "PageOption.h"
#include "Define_Option.h"

using namespace Luritech_Option;

//=============================================================================
//
//=============================================================================
class CPageOpt_Misc : public CPageOption
{
	DECLARE_DYNAMIC(CPageOpt_Misc)

public:
	CPageOpt_Misc						(void);
	CPageOpt_Misc						(UINT nIDTemplate, UINT nIDCaption = 0);
	virtual ~CPageOpt_Misc				(void);

protected:

	DECLARE_MESSAGE_MAP()

	virtual void			AdjustLayout		();
	virtual void			SetPropListFont		();
	virtual void			InitPropList		();

	stOpt_Misc				m_stOption;

	stOpt_Misc				Get_Option			();
	void					Set_Option			(stOpt_Misc stOption);

public:

	virtual void			SaveOption			();
	virtual void			LoadOption			();
};

#endif // PageOpt_Misc_h__
