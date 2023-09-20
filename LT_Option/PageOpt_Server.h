//*****************************************************************************
// Filename	: PageOpt_Server.h
// Created	: 2010/9/16
// Modified	: 2010/9/16 - 18:05
//
// Author	: PiRing
//	
// Purpose	: 
//*****************************************************************************
#ifndef PageOpt_Server_h__
#define PageOpt_Server_h__

#pragma once
#include "PageOption.h"
#include "Define_Option.h"

using namespace Luritech_Option;

//=============================================================================
//
//=============================================================================
class CPageOpt_Server : public CPageOption
{
	DECLARE_DYNAMIC(CPageOpt_Server)

public:
	CPageOpt_Server						(void);
	CPageOpt_Server						(UINT nIDTemplate, UINT nIDCaption = 0);
	virtual ~CPageOpt_Server			(void);

protected:

	DECLARE_MESSAGE_MAP()

	virtual void			AdjustLayout		();
	virtual void			SetPropListFont		();
	virtual void			InitPropList		();

	//2023.01.26a uhkim 추가 서버
	stOpt_Server				m_stOption[ICS_SERVER_MAX];
	//stOpt_Server				m_stOption;
	//2023.01.26a uhkim 추가 서버
	//stOpt_Server				Get_Option			();
	void						Get_Option();
	//void						Set_Option			(stOpt_Server stOption);
	void						Set_Option			();

public:

	virtual void			SaveOption			();
	virtual void			LoadOption			();
};

#endif // PageOpt_Server_h__
