//*****************************************************************************
// Filename	: PageOption.cpp
// Created	: 2010/8/27
// Modified	: 2010/8/27 - 10:22
//
// Author	: PiRing
//	
// Purpose	: 옵션 - 기본 페이지
//*****************************************************************************
// PageOption.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "PageOption.h"
#include "Define_OptionItem.h"
#include "Define_OptionDescription.h"

//=============================================================================
// CPageOption
//=============================================================================
IMPLEMENT_DYNAMIC(CPageOption, CMFCPropertyPage)

//=============================================================================
// 생성자
//=============================================================================
CPageOption::CPageOption( LPCTSTR lpszCaption /*= NULL*/ )
{
	if (NULL != lpszCaption)
		m_strCaption	= lpszCaption;

	m_strFontName	= _T("ARIAL");
	m_nFontSize		= 11;

	hLocal = NULL;

	m_InsptrType	= enInsptrSysType::Sys_Ics_Trinity_Line;

	SetDlgTemplate();
	InitTemplate();

}

CPageOption::CPageOption(UINT nIDTemplate, UINT nIDCaption /*= 0*/) : CMFCPropertyPage(nIDTemplate, nIDCaption)
{
	m_strFontName = _T("ARIAL");
	m_nFontSize = 11;

	hLocal = NULL;

	m_InsptrType = enInsptrSysType::Sys_Ics_Trinity_Line;

// 	SetDlgTemplate();
// 	InitTemplate();
}

//=============================================================================
// 소멸자
//=============================================================================
CPageOption::~CPageOption()
{
	m_strCaption.Empty();
	m_strCaption.ReleaseBuffer();

	m_strFontName.Empty();
	m_strFontName.ReleaseBuffer();

	if (NULL != hLocal)
	{
		LocalUnlock(hLocal);
		LocalFree(hLocal);
	}
}


BEGIN_MESSAGE_MAP(CPageOption, CMFCPropertyPage)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChanged)
END_MESSAGE_MAP()


//=============================================================================
// CPageOption 메시지 처리기입니다.
//=============================================================================

//=============================================================================
// Method		: CPageOption::OnCreate
// Access		: protected 
// Returns		: int
// Parameter	: LPCREATESTRUCT lpCreateStruct
// Qualifier	:
// Last Update	: 2010/8/27 - 11:14
// Desc.		:
//=============================================================================
int CPageOption::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMFCPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();
	
	if (!m_wndPropList.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER, rectDummy, this, 1))
	{
		TRACE0("Failed to create Properies Grid \n");
		return -1;      // fail to create
	}

	InitPropList();
	AdjustLayout();

	return 0;
}

//=============================================================================
// Method		: CPageOption::OnSize
// Access		: protected 
// Returns		: void
// Parameter	: UINT nType
// Parameter	: int cx
// Parameter	: int cy
// Qualifier	:
// Last Update	: 2010/8/27 - 11:14
// Desc.		:
//=============================================================================
void CPageOption::OnSize(UINT nType, int cx, int cy)
{
	CMFCPropertyPage::OnSize(nType, cx, cy);

	AdjustLayout();
}

//=============================================================================
// Method		: CPageOption::OnSetFocus
// Access		: protected 
// Returns		: void
// Parameter	: CWnd * pOldWnd
// Qualifier	:
// Last Update	: 2010/8/27 - 11:14
// Desc.		:
//=============================================================================
void CPageOption::OnSetFocus(CWnd* pOldWnd)
{
	CMFCPropertyPage::OnSetFocus(pOldWnd);

	m_wndPropList.SetFocus();
}

//=============================================================================
// Method		: CPageOption::OnSettingChange
// Access		: protected 
// Returns		: void
// Parameter	: UINT uFlags
// Parameter	: LPCTSTR lpszSection
// Qualifier	:
// Last Update	: 2010/8/27 - 11:14
// Desc.		:
//=============================================================================
void CPageOption::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CMFCPropertyPage::OnSettingChange(uFlags, lpszSection);

	SetPropListFont();
}

//=============================================================================
// Method		: CPageOption::OnInitDialog
// Access		: public 
// Returns		: BOOL
// Qualifier	:
// Last Update	: 2010/8/27 - 16:57
// Desc.		:
//=============================================================================
BOOL CPageOption::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();

	LoadOption();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

//=============================================================================
// Method		: CPageOption::OnPropertyChanged
// Access		: protected 
// Returns		: LRESULT
// Parameter	: WPARAM wParam
// Parameter	: LPARAM lParam
// Qualifier	:
// Last Update	: 2010/9/6 - 16:43
// Desc.		:
//=============================================================================
LRESULT CPageOption::OnPropertyChanged( WPARAM wParam, LPARAM lParam )
{
	GetParent()->SendMessage(AFX_WM_PROPERTY_CHANGED, wParam, lParam);

	return TRUE;
}

//=============================================================================
// Method		: CPageOption::AdjustLayout
// Access		: virtual protected 
// Returns		: void
// Qualifier	:
// Last Update	: 2010/8/27 - 11:14
// Desc.		:
//=============================================================================
void CPageOption::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);
	
	int iHeight			= 30;
	int iWidthAdjust	= 0; //159;

	m_wndPropList.MoveWindow(rectClient.left, rectClient.top + iHeight, rectClient.Width() - iWidthAdjust, rectClient.Height() - iHeight);
}

//=============================================================================
// Method		: CPageOption::SetPropListFont
// Access		: virtual protected 
// Returns		: void
// Qualifier	:
// Last Update	: 2010/8/27 - 11:14
// Desc.		:
//=============================================================================
void CPageOption::SetPropListFont()
{
	::DeleteObject (m_fntPropList.Detach ());

	LOGFONT lf;
	afxGlobalData.fontRegular.GetLogFont (&lf);

	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);

	afxGlobalData.GetNonClientMetrics (info);

	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	m_fntPropList.CreateFontIndirect (&lf);

	m_wndPropList.SetFont(&m_fntPropList);
}

//=============================================================================
// Method		: CPageOption::InitPropList
// Access		: virtual protected 
// Returns		: void
// Qualifier	:
// Last Update	: 2010/8/27 - 11:14
// Desc.		:
//=============================================================================
void CPageOption::InitPropList()
{
	SetPropListFont();

	m_wndPropList.EnableHeaderCtrl(TRUE, _T( "Item" ), _T( "Data" ));
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();
}

//=============================================================================
// Method		: CPageOption::SaveOption
// Access		: public 
// Returns		: void
// Qualifier	:
// Last Update	: 2010/9/6 - 15:20
// Desc.		:
//=============================================================================
void CPageOption::SaveOption()
{

}

//=============================================================================
// Method		: CPageOption::LoadOption
// Access		: public 
// Returns		: void
// Qualifier	:
// Last Update	: 2010/9/6 - 15:20
// Desc.		:
//=============================================================================
void CPageOption::LoadOption()
{

}

//=============================================================================
// Method		: CPageOption::InitPropList_Comport
// Access		: protected 
// Returns		: void
// Parameter	: std::auto_ptr<CMFCPropertyGridProperty> apGroup_ComPort
// Qualifier	:
// Last Update	: 2010/10/6 - 10:49
// Desc.		:
//=============================================================================
void CPageOption::InitPropList_Comport( std::auto_ptr<CMFCPropertyGridProperty> &apGroup_ComPort )
{
	CMFCPropertyGridProperty* pProp = NULL;

	// 통신 포트
	pProp = new CMFCPropertyGridProperty(_T("Com Port"), lpszComPort[0], OPT_DESC_COMPORT);
	for (int iIndex = 0; NULL != lpszComPort[iIndex]; iIndex++)
	{
		pProp->AddOption(lpszComPort[iIndex]);
	}	
	pProp->AllowEdit(FALSE);
	apGroup_ComPort->AddSubItem(pProp);

	// BaudRate /* Baudrate at which running       */
	pProp = new CMFCPropertyGridProperty(_T("BaudRate"), lpszBaudRate[0], OPT_DESC_BAUDRATE);
	for (int iIndex = 0; NULL != lpszBaudRate[iIndex]; iIndex++)
	{
		pProp->AddOption(lpszBaudRate[iIndex]);
	}	
	pProp->AllowEdit(FALSE);
	apGroup_ComPort->AddSubItem(pProp);

	// ByteSize /* Number of bits/byte, 4-8        */
	pProp = new CMFCPropertyGridProperty(_T("ByteSize"), (_variant_t) 8l, OPT_DESC_BYTESIZE);
	pProp->EnableSpinControl(TRUE, 4, 8);	
	apGroup_ComPort->AddSubItem(pProp);

	// Parity /* 0-4=None,Odd,Even,Mark,Space    */
	pProp = new CMFCPropertyGridProperty(_T("Parity"), lpszParity[0], OPT_DESC_PARITY);
	for (int iIndex = 0; NULL != lpszParity[iIndex]; iIndex++)
	{
		pProp->AddOption(lpszParity[iIndex]);
	}	
	pProp->AllowEdit(FALSE);
	apGroup_ComPort->AddSubItem(pProp);

	// StopBits /* 0,1,2 = 1, 1.5, 2               */
	pProp = new CMFCPropertyGridProperty(_T("StopBits"), lpszStopBits[0], OPT_DESC_STOPBITS);
	for (int iIndex = 0; NULL != lpszStopBits[iIndex]; iIndex++)
	{
		pProp->AddOption(lpszStopBits[iIndex]);
	}	
	pProp->AllowEdit(FALSE);
	apGroup_ComPort->AddSubItem(pProp);
}

//=============================================================================
// Method		: CPageOption::Get_Option_ComPort
// Access		: protected 
// Returns		: void
// Parameter	: CMFCPropertyGridProperty * pPropertyGroup
// Parameter	: stOption_Serial & stOption
// Qualifier	:
// Last Update	: 2010/10/6 - 10:40
// Desc.		:
//=============================================================================
void CPageOption::Get_Option_ComPort( CMFCPropertyGridProperty* pPropertyGroup, stOpt_Serial &stOption )
{
	ASSERT(NULL != pPropertyGroup );
	UINT nSubItemIndex	= 0;
	UINT nIndex			= 0;

	COleVariant rVariant;
	VARIANT		varData;
	CString		strValue;

	int iSubItemCount = pPropertyGroup->GetSubItemsCount();

	USES_CONVERSION;

	// 통신 포트 --------------------------------
	rVariant = (pPropertyGroup->GetSubItem(nSubItemIndex++))->GetValue();
	varData	 = rVariant.Detach();
	ASSERT (varData.vt == VT_BSTR);	
	strValue = OLE2A(varData.bstrVal);

	for (nIndex = 0; NULL != lpszComPort[nIndex]; nIndex++)
	{
		if (lpszComPort[nIndex] == strValue)
			break;
	}	
	stOption.Port = (BYTE)nIndex + 1;

	// Baud Rate --------------------------------
	rVariant = (pPropertyGroup->GetSubItem(nSubItemIndex++))->GetValue();
	varData	 = rVariant.Detach();
	ASSERT(varData.vt == VT_BSTR);		
	strValue = OLE2A(varData.bstrVal);

	for (nIndex = 0; NULL != lpszBaudRate[nIndex]; nIndex++)
	{
		if (lpszBaudRate[nIndex] == strValue)
			break;
	}	
	stOption.BaudRate = BaudRateTable[nIndex];

	// ByteSize ---------------------------------
	rVariant = (pPropertyGroup->GetSubItem(nSubItemIndex++))->GetValue();
	varData	 = rVariant.Detach();	
	ASSERT (varData.vt == VT_I4);	
	stOption.ByteSize = (BYTE)varData.intVal;

	// Parity -----------------------------------
	rVariant = (pPropertyGroup->GetSubItem(nSubItemIndex++))->GetValue();
	varData	 = rVariant.Detach();
	ASSERT(varData.vt == VT_BSTR);		
	strValue = OLE2A(varData.bstrVal);

	for (nIndex = 0; NULL != lpszParity[nIndex]; nIndex++)
	{
		if (lpszParity[nIndex] == strValue)
			break;
	}	
	stOption.Parity = (BYTE)ParityTable[nIndex];

	// StopBits ---------------------------------
	rVariant = (pPropertyGroup->GetSubItem(nSubItemIndex++))->GetValue();
	varData	 = rVariant.Detach();
	ASSERT(varData.vt == VT_BSTR);		
	strValue = OLE2A(varData.bstrVal);

	for (nIndex = 0; NULL != lpszStopBits[nIndex]; nIndex++)
	{
		if (lpszStopBits[nIndex] == strValue)
			break;
	}	
	stOption.StopBits = (BYTE)StopBitsTable[nIndex];
}

//=============================================================================
// Method		: CPageOption::Set_Option_ComPort
// Access		: protected 
// Returns		: void
// Parameter	: CMFCPropertyGridProperty * pPropertyGroup
// Parameter	: stOption_Serial stOption
// Qualifier	:
// Last Update	: 2010/10/6 - 10:39
// Desc.		:
//=============================================================================
void CPageOption::Set_Option_ComPort( CMFCPropertyGridProperty* pPropertyGroup, stOpt_Serial stOption )
{
	ASSERT(NULL != pPropertyGroup );
	UINT nSubItemIndex	= 0;
	UINT nIndex			= 0;
	int iSubItemCount = pPropertyGroup->GetSubItemsCount();

	// 통신 포트 --------------------------------
	(pPropertyGroup->GetSubItem(nSubItemIndex++))->SetValue(lpszComPort[stOption.Port - 1]);

	// Baud Rate --------------------------------
	for (nIndex = 0; NULL != lpszBaudRate[nIndex]; nIndex++)
	{
		if (BaudRateTable[nIndex] == stOption.BaudRate)
			break;
	}	
	(pPropertyGroup->GetSubItem(nSubItemIndex++))->SetValue(lpszBaudRate[nIndex]);

	// ByteSize ---------------------------------
	(pPropertyGroup->GetSubItem(nSubItemIndex++))->SetValue((_variant_t)(long int)stOption.ByteSize);

	// Parity -----------------------------------
	for (nIndex = 0; NULL != lpszParity[nIndex]; nIndex++)
	{
		if (ParityTable[nIndex] == stOption.Parity)
			break;
	}	
	(pPropertyGroup->GetSubItem(nSubItemIndex++))->SetValue(lpszParity[nIndex]);

	// StopBits ---------------------------------
	for (nIndex = 0; NULL != lpszStopBits[nIndex]; nIndex++)
	{
		if (StopBitsTable[nIndex] == stOption.StopBits)
			break;
	}	
	(pPropertyGroup->GetSubItem(nSubItemIndex++))->SetValue(lpszStopBits[nIndex]);
}

//=============================================================================
// Method		: CPageOption::SetDlgTemplate
// Access		: public 
// Returns		: void
// Qualifier	:
// Last Update	: 2010/12/1 - 18:56
// Desc.		:
//=============================================================================
void CPageOption::SetDlgTemplate()
{
	// now initialize the DLGTEMPLATE structure	
	m_dlgTemplate.cx = 280;  // 4 horizontal units are the width of one character
	m_dlgTemplate.cy = 160;  // 8 vertical units are the height of one character
	m_dlgTemplate.style = WS_CAPTION | WS_VISIBLE | WS_DLGFRAME | WS_POPUPWINDOW | DS_MODALFRAME | DS_SETFONT;
	m_dlgTemplate.dwExtendedStyle = 0;
	m_dlgTemplate.x = 0;
	m_dlgTemplate.y = 0;
	m_dlgTemplate.cdit = 0;
}

//=============================================================================
// Method		: CPageOption::InitTemplate
// Access		: public 
// Returns		: void
// Qualifier	:
// Last Update	: 2010/12/1 - 18:56
// Desc.		:
//=============================================================================
void CPageOption::InitTemplate()
{
	// will first convert the control captions to UNICODE	
	TRY  // catch memory exceptions and don't worry about allocation failures
	{		
		int iSizeCaption	= (m_strCaption.GetLength() + 1) * sizeof(WCHAR);
		int iSizeFontName	= (m_strFontName.GetLength() + 1) * sizeof(WCHAR);
		int nBufferSize =  sizeof(DLGTEMPLATE) + (2 * sizeof(WORD))/*menu and class*/ + iSizeCaption;

		/* font information*/
		nBufferSize += sizeof(WORD) + iSizeFontName;

		// adjust size to make first control DWORD aligned
		nBufferSize = (nBufferSize + 3) & ~3;

		HLOCAL hLocal = LocalAlloc(LHND, nBufferSize);
		if (hLocal == NULL)
			AfxThrowMemoryException();

		BYTE*   pBuffer = (BYTE*)LocalLock(hLocal);
		if (pBuffer == NULL)
		{
			LocalFree(hLocal);
			AfxThrowMemoryException();
		}

		BYTE*   pdest	= pBuffer;
		BYTE* pdestEnd	= pdest + nBufferSize;

		// transfer DLGTEMPLATE structure to the buffer
		memcpy_s(pdest, pdestEnd - pdest, &m_dlgTemplate, sizeof(DLGTEMPLATE));
		pdest += sizeof(DLGTEMPLATE);

		*(WORD*)pdest = 0; // no menu
		*(WORD*)(pdest + sizeof(WORD)) = 0;  // use default window class

		pdest += 2 * sizeof(WORD);
		memcpy_s(pdest, pdestEnd - pdest, m_strCaption.GetBuffer(), iSizeCaption);
		pdest += iSizeCaption;

		*(WORD*)pdest = m_nFontSize;
		pdest += sizeof(WORD);
		memcpy_s(pdest, pdestEnd - pdest, m_strFontName.GetBuffer(), iSizeFontName);
		pdest += iSizeFontName;

		InitModalIndirect((DLGTEMPLATE*)pBuffer);

		//LocalUnlock(hLocal);
		//LocalFree(hLocal);
	}
	CATCH(CMemoryException, e)
	{
		TRACE(_T("Memory allocation for dialog template failed!"));
	}
	END_CATCH
}

//=============================================================================
// Method		: SetInspectorType
// Access		: public  
// Returns		: void
// Parameter	: __in enInsptrSysType nInsptrType
// Qualifier	:
// Last Update	: 2016/9/26 - 19:16
// Desc.		:
//=============================================================================
void CPageOption::SetInspectorType(__in enInsptrSysType nInsptrType)
{
	m_InsptrType = nInsptrType;
}
