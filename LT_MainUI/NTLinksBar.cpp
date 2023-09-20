//*****************************************************************************
// Filename	: 	NTLinksBar.cpp
//
// Created	:	2010/11/25 - 11:27
// Modified	:	2010/11/25 - 11:27
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
// CNTLinksBar.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NTLinksBar.h"


// CNTLinksBar

IMPLEMENT_DYNCREATE(CNTLinksBar, CMFCToolBar)

CNTLinksBar::CNTLinksBar ()
{
	m_CurrentCommand = 0;
}

CNTLinksBar::~CNTLinksBar ()
{
}

BEGIN_MESSAGE_MAP(CNTLinksBar, CMFCToolBar)
END_MESSAGE_MAP()

CSize CNTLinksBar::CalcFixedLayout (BOOL, BOOL)
{
	ASSERT_VALID(this);

	return CSize (32767, 10);
}