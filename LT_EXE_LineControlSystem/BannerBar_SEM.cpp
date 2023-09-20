
//*****************************************************************************
// Filename	: 	BannerBar_SEM.cpp
// Created	:	2021/11/16 - 17:22
// Modified	:	2021/11/16 - 17:22
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#include "stdafx.h"
#include "BannerBar_SEM.h"

IMPLEMENT_DYNAMIC(CBannerBar_SEM, CNTBannerBar)

//=============================================================================
// 생성자
//=============================================================================
CBannerBar_SEM::CBannerBar_SEM()
{

}

//=============================================================================
// 소멸자
//=============================================================================
CBannerBar_SEM::~CBannerBar_SEM()
{

}


BEGIN_MESSAGE_MAP(CBannerBar_SEM, CNTBannerBar)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_BN_CLICKED(SC_CLOSE, &CBannerBar_SEM::OnExitProgram)
	ON_BN_CLICKED(SC_OPTION, &CBannerBar_SEM::OnOption)
END_MESSAGE_MAP()


//=============================================================================
// Method		: CreateEx
// Access		: virtual protected  
// Returns		: BOOL
// Parameter	: CWnd * pParentWnd
// Parameter	: UINT nID
// Qualifier	:
// Last Update	: 2021/11/16 - 17:26
// Desc.		:
//=============================================================================
BOOL CBannerBar_SEM::CreateEx(CWnd* pParentWnd, UINT nID /*= uiBannerBarID*/)
{
	if (FALSE == CNTBannerBar::CreateEx(pParentWnd, nID))
	{
		return FALSE;
	}




	return TRUE;
}

//=============================================================================
// Method		: OnSize
// Access		: protected  
// Returns		: void
// Parameter	: UINT nType
// Parameter	: int cx
// Parameter	: int cy
// Qualifier	:
// Last Update	: 2021/11/16 - 17:28
// Desc.		:
//=============================================================================
void CBannerBar_SEM::OnSize(UINT nType, int cx, int cy)
{
	CNTBannerBar::OnSize(nType, cx, cy);




}

void CBannerBar_SEM::OnExitProgram()
{

}

void CBannerBar_SEM::OnOption()
{

}

