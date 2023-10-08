//*****************************************************************************
// Filename	: PageOpt_Insp.cpp
// Created	: 2010/9/6
// Modified	: 2010/9/6 - 15:51
//
// Author	: PiRing
//	
// Purpose	: 
//*****************************************************************************
#include "StdAfx.h"
#include "PageOpt_Insp.h"
#include "Define_OptionItem.h"
#include "Define_OptionDescription.h"

#include <memory>
#include "CustomProperties.h"

IMPLEMENT_DYNAMIC(CPageOpt_Insp, CPageOption)

//=============================================================================
//
//=============================================================================
CPageOpt_Insp::CPageOpt_Insp(LPCTSTR lpszCaption /*= NULL*/) : CPageOption (lpszCaption)
{
	
}

CPageOpt_Insp::CPageOpt_Insp(UINT nIDTemplate, UINT nIDCaption /*= 0*/) : CPageOption(nIDTemplate, nIDCaption)
{
	
}

//=============================================================================
//
//=============================================================================
CPageOpt_Insp::~CPageOpt_Insp(void)
{
}

BEGIN_MESSAGE_MAP(CPageOpt_Insp, CPageOption)	
END_MESSAGE_MAP()

// CPageOpt_Insp 메시지 처리기입니다.

//=============================================================================
// Method		: CPageOpt_Insp::AdjustLayout
// Access		: virtual protected 
// Returns		: void
// Qualifier	:
// Last Update	: 2010/8/30 - 11:12
// Desc.		:
//=============================================================================
void CPageOpt_Insp::AdjustLayout()
{
	CPageOption::AdjustLayout();
}

//=============================================================================
// Method		: CPageOpt_Insp::SetPropListFont
// Access		: virtual protected 
// Returns		: void
// Qualifier	:
// Last Update	: 2010/8/30 - 11:12
// Desc.		:
//=============================================================================
void CPageOpt_Insp::SetPropListFont()
{
	CPageOption::SetPropListFont();
}

//=============================================================================
// Method		: CPageOpt_Insp::InitPropList
// Access		: virtual protected 
// Returns		: void
// Qualifier	:
// Last Update	: 2010/8/30 - 11:12
// Desc.		:
//=============================================================================
void CPageOpt_Insp::InitPropList()
{
	CPageOption::InitPropList();

	CMFCPropertyGridProperty* pProp = NULL;

	//-----------------------------------------------------
	// 기타 설정
	//-----------------------------------------------------
	std::auto_ptr<CMFCPropertyGridProperty> apGroup_Etc(new CMFCPropertyGridProperty(_T("System Setting")));

	pProp = new CMFCPropertyGridProperty(_T("Language"), g_szOptLanguage[OptLang_English], _T("Select Language"));
	pProp->AddOption(g_szOptLanguage[0]); // 한국어
	pProp->AddOption(g_szOptLanguage[1]); // 영어
	pProp->AddOption(g_szOptLanguage[2]); // 베트남어
	pProp->AddOption(g_szOptLanguage[3]); // 중국어
	pProp->AllowEdit(FALSE);
	apGroup_Etc->AddSubItem(pProp);
	m_wndPropList.AddProperty(apGroup_Etc.release());


	//-----------------------------------------------------
	// 경로 설정
	//-----------------------------------------------------
	std::auto_ptr<CMFCPropertyGridProperty> apGroup_Path(new CMFCPropertyGridProperty(_T("Path Setting")));

	apGroup_Path->AddSubItem(new CMFCPropertyGridFileProperty(_T("Log Path"), _T("C:\\Log")));
	apGroup_Path->AddSubItem(new CMFCPropertyGridFileProperty(_T("Report Path"), _T("C:\\Report")));
	apGroup_Path->AddSubItem(new CMFCPropertyGridFileProperty(_T("Line Setting Path"), _T("C:\\Recipe")));
	apGroup_Path->AddSubItem(new CMFCPropertyGridFileProperty(_T("Socket JIG Path"), _T("C:\\Socket")));
	apGroup_Path->AddSubItem(new CMFCPropertyGridFileProperty(_T("Shared Path (NG Code, User)"), _T("C:\\Shared")));
	apGroup_Path->AddSubItem(new CMFCPropertyGridFileProperty(_T("Fail Info Path (NG Count)"), _T("C:\\FailInfo")));

	m_wndPropList.AddProperty(apGroup_Path.release());
	
#ifdef _DEBUG
	//-----------------------------------------------------
	// 프로그램 운영 설정
	//-----------------------------------------------------
	std::auto_ptr<CMFCPropertyGridProperty> apGroup_Comm(new CMFCPropertyGridProperty(_T("Operation Setting")));

	// 자동 재 실행 사용 여부
	pProp = new CMFCPropertyGridProperty(_T("비정상 종료 시 자동 재 실행 사용 여부"), lpszUsableTable[0], OPT_DESC_USE_AUTO_RESTART);
	pProp->AddOption(lpszUsableTable[0]);
	pProp->AddOption(lpszUsableTable[1]);
	pProp->AllowEdit(FALSE);
	apGroup_Comm->AddSubItem(pProp);
	
	m_wndPropList.AddProperty(apGroup_Comm.release());
#endif

	
}

//=============================================================================
// Method		: CPageOpt_Insp::SaveOption
// Access		: virtual protected 
// Returns		: void
// Qualifier	:
// Last Update	: 2010/9/6 - 15:20
// Desc.		:
//=============================================================================
void CPageOpt_Insp::SaveOption()
{
	CPageOption::SaveOption();

	m_stOption	= Get_Option ();

	m_pLT_Option->SaveOption_Inspector(m_stOption);
}

//=============================================================================
// Method		: CPageOpt_Insp::LoadOption
// Access		: virtual protected 
// Returns		: void
// Qualifier	:
// Last Update	: 2010/9/6 - 15:20
// Desc.		:
//=============================================================================
void CPageOpt_Insp::LoadOption()
{
	CPageOption::LoadOption();

	if (m_pLT_Option->LoadOption_Inspector(m_stOption))
		Set_Option(m_stOption);
}

//=============================================================================
// Method		: CPageOpt_Insp::Get_Option
// Access		: protected 
// Returns		: Luritech_Option::stOption_Inspector
// Qualifier	:
// Last Update	: 2010/9/10 - 16:07
// Desc.		:
//=============================================================================
Luritech_Option::stOpt_Insp CPageOpt_Insp::Get_Option()
{
	UINT nGroupIndex	= 0;
	UINT nSubItemIndex	= 0;
	UINT nIndex			= 0;

	COleVariant rVariant;
	VARIANT		varData;
	CString		strValue;

	//---------------------------------------------------------------
	// 기본 검사 설정
	//---------------------------------------------------------------
	int iCount = m_wndPropList.GetPropertyCount();
	CMFCPropertyGridProperty* pPropertyGroup = NULL;
	int iSubItemCount = 0;

	USES_CONVERSION;

	//---------------------------------------------------------------
	// 기타 설정
	//---------------------------------------------------------------
	pPropertyGroup = m_wndPropList.GetProperty(nGroupIndex++);
	iSubItemCount = pPropertyGroup->GetSubItemsCount();
	nSubItemIndex = 0;

	// Language -----------------------------
	rVariant = (pPropertyGroup->GetSubItem(nSubItemIndex++))->GetValue();
	varData = rVariant.Detach();
	ASSERT(varData.vt == VT_BSTR);
	strValue = OLE2T(varData.bstrVal);

	for (nIndex = 0; NULL != g_szOptLanguage[nIndex]; nIndex++)
	{
		if (g_szOptLanguage[nIndex] == strValue)
			break;
	}
	m_stOption.nLanguage = (nIndex < OptLang_MaxCount) ? (uint8_t)nIndex : 1;


	//-----------------------------------------------------
	// 경로 설정
	//-----------------------------------------------------
	pPropertyGroup = m_wndPropList.GetProperty(nGroupIndex++);
	iSubItemCount = pPropertyGroup->GetSubItemsCount();
	nSubItemIndex = 0;

	rVariant = (pPropertyGroup->GetSubItem(nSubItemIndex++))->GetValue();
	varData = rVariant.Detach();
	ASSERT(varData.vt == VT_BSTR);
	strValue = OLE2T(varData.bstrVal);
	m_stOption.szPath_Log = strValue;

 	rVariant = (pPropertyGroup->GetSubItem(nSubItemIndex++))->GetValue();
 	varData = rVariant.Detach();
 	ASSERT(varData.vt == VT_BSTR);
 	strValue = OLE2T(varData.bstrVal);
 	m_stOption.szPath_Report = strValue;

	rVariant = (pPropertyGroup->GetSubItem(nSubItemIndex++))->GetValue();
	varData = rVariant.Detach();
	ASSERT(varData.vt == VT_BSTR);
	strValue = OLE2T(varData.bstrVal);
	m_stOption.szPath_Recipe = strValue;

	rVariant = (pPropertyGroup->GetSubItem(nSubItemIndex++))->GetValue();
	varData = rVariant.Detach();
	ASSERT(varData.vt == VT_BSTR);
	strValue = OLE2T(varData.bstrVal);
	m_stOption.szPath_Socket = strValue;

	rVariant = (pPropertyGroup->GetSubItem(nSubItemIndex++))->GetValue();
	varData = rVariant.Detach();
	ASSERT(varData.vt == VT_BSTR);
	strValue = OLE2T(varData.bstrVal);
	m_stOption.szPath_Shared = strValue;

	rVariant = (pPropertyGroup->GetSubItem(nSubItemIndex++))->GetValue();
	varData = rVariant.Detach();
	ASSERT(varData.vt == VT_BSTR);
	strValue = OLE2T(varData.bstrVal);
	m_stOption.szPath_FailInfo = strValue;

#ifdef _DEBUG
	//---------------------------------------------------------------
	// 프로그램 운영 설정
	//---------------------------------------------------------------
	pPropertyGroup = m_wndPropList.GetProperty(nGroupIndex++);
	iSubItemCount = pPropertyGroup->GetSubItemsCount();
	nSubItemIndex = 0;

	// 자동 재 실행 사용 여부
	rVariant = (pPropertyGroup->GetSubItem(nSubItemIndex++))->GetValue();
	varData = rVariant.Detach();
	ASSERT(varData.vt == VT_BSTR);
	strValue = OLE2T(varData.bstrVal);

	for (nIndex = 0; NULL != lpszUsableTable[nIndex]; nIndex++)
	{
		if (lpszUsableTable[nIndex] == strValue)
			break;
	}
	m_stOption.UseAutoRestart = (BOOL)nIndex;

#endif

	return m_stOption;
}

//=============================================================================
// Method		: CPageOpt_Insp::Set_Option
// Access		: protected 
// Returns		: void
// Parameter	: stOption_Inspector stOption
// Qualifier	:
// Last Update	: 2010/9/10 - 16:07
// Desc.		:
//=============================================================================
void CPageOpt_Insp::Set_Option( stOpt_Insp stOption )
{
	UINT nGroupIndex	= 0;
	UINT nSubItemIndex	= 0;
	UINT nIndex			= 0;

	//---------------------------------------------------------------
	// 기본 검사 설정
	//---------------------------------------------------------------
	int iCount = m_wndPropList.GetPropertyCount();
	CMFCPropertyGridProperty* pPropertyGroup = NULL;
	int iSubItemCount = 0;

	//---------------------------------------------------------------
	// 기타 설정
	//---------------------------------------------------------------
	pPropertyGroup = m_wndPropList.GetProperty(nGroupIndex++);
	iSubItemCount = pPropertyGroup->GetSubItemsCount();
	nSubItemIndex = 0;

	// Password -----------------------------
	(pPropertyGroup->GetSubItem(nSubItemIndex++))->SetValue(g_szOptLanguage[m_stOption.nLanguage]);


	//-----------------------------------------------------
	// 경로 설정
	//-----------------------------------------------------
	pPropertyGroup = m_wndPropList.GetProperty(nGroupIndex++);
	iSubItemCount = pPropertyGroup->GetSubItemsCount();
	nSubItemIndex = 0;

	(pPropertyGroup->GetSubItem(nSubItemIndex++))->SetValue(m_stOption.szPath_Log);
	(pPropertyGroup->GetSubItem(nSubItemIndex++))->SetValue(m_stOption.szPath_Report);
	(pPropertyGroup->GetSubItem(nSubItemIndex++))->SetValue(m_stOption.szPath_Recipe);
	(pPropertyGroup->GetSubItem(nSubItemIndex++))->SetValue(m_stOption.szPath_Socket);
	(pPropertyGroup->GetSubItem(nSubItemIndex++))->SetValue(m_stOption.szPath_Shared);
	(pPropertyGroup->GetSubItem(nSubItemIndex++))->SetValue(m_stOption.szPath_FailInfo);

#ifdef _DEBUG
	//---------------------------------------------------------------
	// 프로그램 운영 설정
	//---------------------------------------------------------------
	pPropertyGroup = m_wndPropList.GetProperty(nGroupIndex++);
	iSubItemCount = pPropertyGroup->GetSubItemsCount();
	nSubItemIndex = 0;

	// 자동 재 실행 사용 여부
	(pPropertyGroup->GetSubItem(nSubItemIndex++))->SetValue(lpszUsableTable[m_stOption.UseAutoRestart]);
#endif
	
}