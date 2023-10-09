//*****************************************************************************
// Filename	: PageOpt_Server.cpp
// Created	: 2010/9/16
// Modified	: 2010/9/16 - 15:33
//
// Author	: PiRing
//	
// Purpose	: 
//*****************************************************************************
#include "StdAfx.h"
#include "PageOpt_Server.h"
#include "Define_OptionItem.h"
#include "Define_OptionDescription.h"
#include <memory>
#include "MFCPropertyGridProperties.h"

#define ID_PROPGRID_IPADDR (21)

IMPLEMENT_DYNAMIC(CPageOpt_Server, CPageOption)

//=============================================================================
// 생성자
//=============================================================================
CPageOpt_Server::CPageOpt_Server(void)
{
}

CPageOpt_Server::CPageOpt_Server(UINT nIDTemplate, UINT nIDCaption /*= 0*/) : CPageOption(nIDTemplate, nIDCaption)
{

}

//=============================================================================
// 소멸자
//=============================================================================
CPageOpt_Server::~CPageOpt_Server(void)
{
}

BEGIN_MESSAGE_MAP(CPageOpt_Server, CPageOption)	
END_MESSAGE_MAP()

//=============================================================================
// CPageOpt_Server 메시지 처리기입니다.
//=============================================================================
//=============================================================================
// Method		: CPageOpt_Server::AdjustLayout
// Access		: virtual protected 
// Returns		: void
// Qualifier	:
// Last Update	: 2010/8/30 - 11:12
// Desc.		:
//=============================================================================
void CPageOpt_Server::AdjustLayout()
{
	CPageOption::AdjustLayout();
}

//=============================================================================
// Method		: CPageOpt_Server::SetPropListFont
// Access		: virtual protected 
// Returns		: void
// Qualifier	:
// Last Update	: 2010/8/30 - 11:12
// Desc.		:
//=============================================================================
void CPageOpt_Server::SetPropListFont()
{
	CPageOption::SetPropListFont();
}

//=============================================================================
// Method		: CPageOpt_Server::InitPropList
// Access		: virtual protected 
// Returns		: void
// Qualifier	:
// Last Update	: 2010/8/30 - 11:12
// Desc.		:
//=============================================================================
void CPageOpt_Server::InitPropList()
{
	CPageOption::InitPropList();

	//--------------------------------------------------------
	// 통신 설정
	//--------------------------------------------------------
	std::auto_ptr<CMFCPropertyGridProperty> apGroup_Comm(new CMFCPropertyGridProperty(_T("Server Setting")));

	CMFCPropertyGridProperty* pProp = NULL;

	// 설비코드
	//apGroup_Comm->AddSubItem(new CMFCPropertyGridProperty(_T("Equipment ID"), (COleVariant)_T(""), _T("Equipment ID")));

	// IP Address	
 	in_addr addr;
 	addr.s_addr = inet_addr("192.168.0.1");
 	pProp = new CMFCPropertyGridIPAdressProperty(_T("NIC IP Address"), addr, OPT_DESC_IP_ADDRESS, ID_PROPGRID_IPADDR);
 	apGroup_Comm->AddSubItem(pProp);

	// Port
 	pProp = new CMFCPropertyGridProperty(_T("Port"), (_variant_t)1296l, OPT_DESC_IP_PORT);
 	pProp->EnableSpinControl(TRUE, 0, 9999);
 	apGroup_Comm->AddSubItem(pProp);

#if defined(EES_XML)//20231003
	pProp = new CMFCPropertyGridIPAdressProperty(_T("EES IP Address2"), addr, OPT_DESC_IP_ADDRESS, ID_PROPGRID_IPADDR);
	apGroup_Comm->AddSubItem(pProp);
	//추가 포트
	pProp = new CMFCPropertyGridProperty(_T("EES Port"), (_variant_t)1296l, OPT_DESC_IP_PORT);
	pProp->EnableSpinControl(TRUE, 0, 9999);
	apGroup_Comm->AddSubItem(pProp);
#endif
	// Log 저장 경로
	//apGroup_Comm->AddSubItem(new CMFCPropertyGridFileProperty(_T("Log Path"), _T("C:\\Server_Log\\")));

	m_wndPropList.AddProperty(apGroup_Comm.release());
}

//=============================================================================
// Method		: CPageOpt_Server::SaveOption
// Access		: virtual protected 
// Returns		: void
// Qualifier	:
// Last Update	: 2010/9/6 - 15:20
// Desc.		:
//=============================================================================
void CPageOpt_Server::SaveOption()
{
	CPageOption::SaveOption();
#if defined(EES_XML)//20231003
	Get_Option(ICS_SERVER_MAX);
	for (int i = 0; i < ICS_SERVER_MAX;i++) {
		m_pLT_Option->SaveOption_Server(i, m_stOption[i]);
	}
#else
	m_stOption	= Get_Option ();
	m_pLT_Option->SaveOption_Server(m_stOption);
#endif
}

//=============================================================================
// Method		: CPageOpt_Server::LoadOption
// Access		: virtual protected 
// Returns		: void
// Qualifier	:
// Last Update	: 2010/9/6 - 15:20
// Desc.		:
//=============================================================================
void CPageOpt_Server::LoadOption()
{
	CPageOption::LoadOption();
#if defined(EES_XML)//20231003
	Get_Option(ICS_SERVER_MAX);
	for (int i = 0; i < ICS_SERVER_MAX;i++) {
		m_pLT_Option->SaveOption_Server(i, m_stOption[i]);
	}
#else
	if (m_pLT_Option->LoadOption_Server(m_stOption))
		Set_Option(m_stOption);
#endif
}

#if defined(EES_XML)//20231003
void CPageOpt_Server::Get_Option(int nIn)
{
	UINT nGroupIndex	= 0;
	UINT nSubItemIndex	= 0;
	UINT nIndex			= 0;
	COleVariant rVariant;
	VARIANT		varData;
	CString		strValue;
	//---------------------------------------------------------------
	// 그룹 1 
	//---------------------------------------------------------------
	int iCount = m_wndPropList.GetPropertyCount();
	CMFCPropertyGridProperty* pPropertyGroup = m_wndPropList.GetProperty(nGroupIndex++);
	int iSubItemCount = pPropertyGroup->GetSubItemsCount();
	USES_CONVERSION;
	// 설비 코드
	// 서버 IP Address ---------------------------
	//2023.01.26a uhkim Server Option 추가 IP 변경
	m_stOption[ICS_SERVER_MODULE].Address.dwAddress = (pPropertyGroup->GetSubItem(nSubItemIndex++))->GetValue().ulVal;
	// 서버 IP Port
	rVariant = (pPropertyGroup->GetSubItem(nSubItemIndex++))->GetValue();
	varData = rVariant.Detach();
	ASSERT(varData.vt == VT_I4);
	m_stOption[ICS_SERVER_MODULE].Address.dwPort = varData.intVal;
	m_stOption[ICS_SERVER_EES].Address.dwAddress = (pPropertyGroup->GetSubItem(nSubItemIndex++))->GetValue().ulVal;
	// 서버 IP Port
	rVariant = (pPropertyGroup->GetSubItem(nSubItemIndex++))->GetValue();
	varData = rVariant.Detach();
	ASSERT(varData.vt == VT_I4);
	m_stOption[ICS_SERVER_EES].Address.dwPort = varData.intVal;
}
//=============================================================================
// Method		: Set_Option
// Access		: protected  
// Returns		: void
// Parameter	: stOpt_Server stOption
// Qualifier	:
// Last Update	: 2016/5/18 - 16:22
// Desc.		:
//=============================================================================
void CPageOpt_Server::Set_Option()
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
	//Equipment ID
	// 서버 IP Address ----------------------
	(pPropertyGroup->GetSubItem(nSubItemIndex++))->SetValue(ULONG_VARIANT(m_stOption[ICS_SERVER_MODULE].Address.dwAddress));
	// 서버 IP Port
	(pPropertyGroup->GetSubItem(nSubItemIndex++))->SetValue((_variant_t)(long int)m_stOption[ICS_SERVER_MODULE].Address.dwPort);
	(pPropertyGroup->GetSubItem(nSubItemIndex++))->SetValue(ULONG_VARIANT(m_stOption[ICS_SERVER_EES].Address.dwAddress));
	(pPropertyGroup->GetSubItem(nSubItemIndex++))->SetValue((_variant_t)(long int)m_stOption[ICS_SERVER_EES].Address.dwPort);
}
#else
//=============================================================================
// Method		: Get_Option
// Access		: protected  
// Returns		: Luritech_Option::stOpt_Server
// Qualifier	:
// Last Update	: 2016/5/18 - 16:22
// Desc.		:
//=============================================================================
Luritech_Option::stOpt_Server CPageOpt_Server::Get_Option()
{
	UINT nGroupIndex	= 0;
	UINT nSubItemIndex	= 0;
	UINT nIndex			= 0;

	COleVariant rVariant;
	VARIANT		varData;
	CString		strValue;

	//---------------------------------------------------------------
	// 그룹 1 
	//---------------------------------------------------------------
	int iCount = m_wndPropList.GetPropertyCount();
	CMFCPropertyGridProperty* pPropertyGroup = m_wndPropList.GetProperty(nGroupIndex++);
	int iSubItemCount = pPropertyGroup->GetSubItemsCount();

	USES_CONVERSION;

	// 설비 코드
// 	rVariant = (pPropertyGroup->GetSubItem(nSubItemIndex++))->GetValue();
// 	varData = rVariant.Detach();
// 	ASSERT(varData.vt == VT_BSTR);
// 	strValue = OLE2A(varData.bstrVal);
// 	m_stOption.szEquipmentID = strValue;

	// 서버 IP Address ---------------------------
	m_stOption.Address.dwAddress = (pPropertyGroup->GetSubItem(nSubItemIndex++))->GetValue().ulVal;

	// 서버 IP Port
	rVariant = (pPropertyGroup->GetSubItem(nSubItemIndex++))->GetValue();
	varData = rVariant.Detach();
	ASSERT(varData.vt == VT_I4);
	m_stOption.Address.dwPort = varData.intVal;

	// MES 저장 경로
// 	rVariant = (pPropertyGroup->GetSubItem(nSubItemIndex++))->GetValue();
// 	varData = rVariant.Detach();
// 	ASSERT(varData.vt == VT_BSTR);
// 	strValue = OLE2A(varData.bstrVal);
// 	m_stOption.szPath_Log = strValue;

	return m_stOption;
}


//=============================================================================
// Method		: Set_Option
// Access		: protected  
// Returns		: void
// Parameter	: stOpt_Server stOption
// Qualifier	:
// Last Update	: 2016/5/18 - 16:22
// Desc.		:
//=============================================================================
void CPageOpt_Server::Set_Option( stOpt_Server stOption )
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

	//Equipment ID
	//(pPropertyGroup->GetSubItem(nSubItemIndex++))->SetValue(m_stOption.szEquipmentID);

	// 서버 IP Address ----------------------
	(pPropertyGroup->GetSubItem(nSubItemIndex++))->SetValue(ULONG_VARIANT(m_stOption.Address.dwAddress));
	
	// 서버 IP Port
	(pPropertyGroup->GetSubItem(nSubItemIndex++))->SetValue((_variant_t)(long int)m_stOption.Address.dwPort);

	// MES 저장 경로
	//(pPropertyGroup->GetSubItem(nSubItemIndex++))->SetValue(m_stOption.szPath_Log);

}

#endif