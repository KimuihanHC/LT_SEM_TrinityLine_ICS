//*****************************************************************************
// Filename	: PageOpt_Misc.cpp
// Created	: 2010/9/16
// Modified	: 2010/9/16 - 15:33
//
// Author	: PiRing
//	
// Purpose	: 
//*****************************************************************************
#include "StdAfx.h"
#include "PageOpt_Misc.h"
#include "Define_OptionItem.h"
#include "Define_OptionDescription.h"
#include <memory>
#include "MFCPropertyGridProperties.h"

#define ID_PROPGRID_IPADDR (21)

IMPLEMENT_DYNAMIC(CPageOpt_Misc, CPageOption)

//=============================================================================
// 생성자
//=============================================================================
CPageOpt_Misc::CPageOpt_Misc(void)
{
}

CPageOpt_Misc::CPageOpt_Misc(UINT nIDTemplate, UINT nIDCaption /*= 0*/) : CPageOption(nIDTemplate, nIDCaption)
{

}

//=============================================================================
// 소멸자
//=============================================================================
CPageOpt_Misc::~CPageOpt_Misc(void)
{
}

BEGIN_MESSAGE_MAP(CPageOpt_Misc, CPageOption)	
END_MESSAGE_MAP()

//=============================================================================
// CPageOpt_Misc 메시지 처리기입니다.
//=============================================================================
//=============================================================================
// Method		: CPageOpt_Misc::AdjustLayout
// Access		: virtual protected 
// Returns		: void
// Qualifier	:
// Last Update	: 2010/8/30 - 11:12
// Desc.		:
//=============================================================================
void CPageOpt_Misc::AdjustLayout()
{
	CPageOption::AdjustLayout();
}

//=============================================================================
// Method		: CPageOpt_Misc::SetPropListFont
// Access		: virtual protected 
// Returns		: void
// Qualifier	:
// Last Update	: 2010/8/30 - 11:12
// Desc.		:
//=============================================================================
void CPageOpt_Misc::SetPropListFont()
{
	CPageOption::SetPropListFont();
}

//=============================================================================
// Method		: CPageOpt_Misc::InitPropList
// Access		: virtual protected 
// Returns		: void
// Qualifier	:
// Last Update	: 2010/8/30 - 11:12
// Desc.		:
//=============================================================================
void CPageOpt_Misc::InitPropList()
{
	CPageOption::InitPropList();

	
	//--------------------------------------------------------
	// 통신 설정
	//--------------------------------------------------------
	std::auto_ptr<CMFCPropertyGridProperty> apGroup_Misc(new CMFCPropertyGridProperty(_T("기타 설정")));

	CMFCPropertyGridProperty* pProp = NULL;
	

	m_wndPropList.AddProperty(apGroup_Misc.release());
}

//=============================================================================
// Method		: CPageOpt_Misc::SaveOption
// Access		: virtual protected 
// Returns		: void
// Qualifier	:
// Last Update	: 2010/9/6 - 15:20
// Desc.		:
//=============================================================================
void CPageOpt_Misc::SaveOption()
{
	CPageOption::SaveOption();

	m_stOption	= Get_Option ();

	m_pLT_Option->SaveOption_Misc(m_stOption);
}

//=============================================================================
// Method		: CPageOpt_Misc::LoadOption
// Access		: virtual protected 
// Returns		: void
// Qualifier	:
// Last Update	: 2010/9/6 - 15:20
// Desc.		:
//=============================================================================
void CPageOpt_Misc::LoadOption()
{
	CPageOption::LoadOption();

	if (m_pLT_Option->LoadOption_Misc(m_stOption))
		Set_Option(m_stOption);
}

//=============================================================================
// Method		: Get_Option
// Access		: protected  
// Returns		: Luritech_Option::stOpt_Misc
// Qualifier	:
// Last Update	: 2016/5/18 - 16:22
// Desc.		:
//=============================================================================
Luritech_Option::stOpt_Misc CPageOpt_Misc::Get_Option()
{
	UINT nGroupIndex	= 0;
	UINT nSubItemIndex	= 0;
	UINT nIndex			= 0;

	COleVariant rVariant;
//	VARIANT		varData;
	CString		strValue;

	//---------------------------------------------------------------
	// 그룹 1 
	//---------------------------------------------------------------
	int iCount = m_wndPropList.GetPropertyCount();
	CMFCPropertyGridProperty* pPropertyGroup = m_wndPropList.GetProperty(nGroupIndex++);
	int iSubItemCount = pPropertyGroup->GetSubItemsCount();

	USES_CONVERSION;

	

	return m_stOption;
}

//=============================================================================
// Method		: Set_Option
// Access		: protected  
// Returns		: void
// Parameter	: stOpt_Misc stOption
// Qualifier	:
// Last Update	: 2016/5/18 - 16:22
// Desc.		:
//=============================================================================
void CPageOpt_Misc::Set_Option( stOpt_Misc stOption )
{
	UINT nGroupIndex	= 0;
	UINT nSubItemIndex	= 0;
	UINT nIndex			= 0;

	//---------------------------------------------------------------
	// 그룹 1 통신 종류
	//---------------------------------------------------------------
	int iCount = m_wndPropList.GetPropertyCount();
	CMFCPropertyGridProperty* pPropertyGroup = m_wndPropList.GetProperty(nGroupIndex++);
	int iSubItemCount = pPropertyGroup->GetSubItemsCount();

	

}
