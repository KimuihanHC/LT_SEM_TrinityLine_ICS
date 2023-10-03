//*****************************************************************************
// Filename	: SheetOption.cpp
// Created	: 2010/8/27
// Modified	: 2013/2/20 - 15:53
//
// Author	: PiRing
//	
// Purpose	: 
//*****************************************************************************
// SheetOption.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SheetOption.h"
#include "resource.h"

#include "PageOpt_Insp.h"
#include "PageOpt_Server.h"
#include "PageOpt_Misc.h"


//=============================================================================
// CSheetOption
//=============================================================================
IMPLEMENT_DYNAMIC(CSheetOption, CMFCPropertySheet)

//=============================================================================
//
//=============================================================================
CSheetOption::CSheetOption()
{
	SetLook (CMFCPropertySheet::PropSheetLook_List);
	EnablePageHeader (30);

	m_nBarWidth = 120;//160;

	m_WMItemChanged = NULL;
	m_bSavedOption	= FALSE;

	m_InsptrType	= enInsptrSysType::Sys_Ics_Trinity_Line;
}

CSheetOption::CSheetOption(UINT nIDCaption, CWnd* pParentWnd /*= NULL*/) 
	: CMFCPropertySheet(nIDCaption, pParentWnd)
{
	SetLook (CMFCPropertySheet::PropSheetLook_List);
	EnablePageHeader (30);

	//InitPage();
	
	//SetIconsList (uiIconsResID, cxIcon);
	m_nBarWidth = 120;//160;
	m_WMItemChanged = NULL;

	m_InsptrType	= enInsptrSysType::Sys_Ics_Trinity_Line;
}

//=============================================================================
//
//=============================================================================
CSheetOption::~CSheetOption()
{		
// 	if (m_bSavedOption)
// 		AfxGetApp()->GetMainWnd()->SendNotifyMessage(m_WMItemChanged, 0, 0);

 	for (int iIdx = 0; iIdx < m_ptrPage.GetCount(); iIdx++)
 	{
 		delete (CPageOption*)m_ptrPage.ElementAt(iIdx);		
 	}

	m_ptrPage.RemoveAll();
}


BEGIN_MESSAGE_MAP(CSheetOption, CMFCPropertySheet)	
	ON_WM_SIZE()
	ON_BN_CLICKED (ID_APPLY_NOW,	OnBnClickedApplyNow)
	ON_BN_CLICKED (IDOK,			OnOK)
	ON_BN_CLICKED (IDCANCEL,		OnCancel)
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChanged)
END_MESSAGE_MAP()


//=============================================================================
// CSheetOption 메시지 처리기입니다.
//=============================================================================
//=============================================================================
// Method		: CSheetOption::OnSize
// Access		: public 
// Returns		: void
// Parameter	: UINT nType
// Parameter	: int cx
// Parameter	: int cy
// Qualifier	:
// Last Update	: 2010/8/30 - 16:34
// Desc.		:
//=============================================================================
void CSheetOption::OnSize(UINT nType, int cx, int cy)
{
	CMFCPropertySheet::OnSize(nType, cx, cy);

	if (!GetDlgItem(ID_APPLY_NOW)->GetSafeHwnd())
		return;

	CRect	rectApply;

	rectApply.SetRectEmpty();
	GetDlgItem(IDHELP)->GetWindowRect(rectApply);
	ScreenToClient(rectApply);
	GetDlgItem(ID_APPLY_NOW)->MoveWindow(rectApply);
}

//=============================================================================
// Method		: CSheetOption::OnInitDialog
// Access		: public 
// Returns		: BOOL
// Qualifier	:
// Last Update	: 2010/8/30 - 16:34
// Desc.		:
//=============================================================================
BOOL CSheetOption::OnInitDialog()
{
	BOOL bResult = CMFCPropertySheet::OnInitDialog();

	// UI 만들어 놓은것과 충돌을 일으킴
//	CMFCVisualManagerOffice2007::SetStyle( CMFCVisualManagerOffice2007::Office2007_ObsidianBlack );
//	CMFCVisualManager::SetDefaultManager( RUNTIME_CLASS( CMFCVisualManagerOffice2007 ));

	GetDlgItem(IDHELP)->EnableWindow(FALSE);
	GetDlgItem(ID_APPLY_NOW)->EnableWindow(FALSE);
	m_bItemChanged	= FALSE;

	GetDlgItem(IDHELP)->ShowWindow(SW_HIDE);	

	INT_PTR iCount = m_ptrPage.GetCount();
	for (int iIndex = 1; iIndex < iCount; iIndex++)
		SetActivePage (iIndex);
	
	SetActivePage (0);

	return bResult;
}

//=============================================================================
// Method		: CSheetOption::OnDrawPageHeader
// Access		: virtual protected 
// Returns		: void
// Parameter	: CDC * pDC
// Parameter	: int nPage
// Parameter	: CRect rectHeader
// Qualifier	:
// Last Update	: 2010/8/30 - 16:34
// Desc.		:
//=============================================================================
void CSheetOption::OnDrawPageHeader (CDC* pDC, int nPage, CRect rectHeader)
{
	rectHeader.top += 2;
	rectHeader.right -= 2;
	rectHeader.bottom -= 2;

	pDC->FillRect (rectHeader, &afxGlobalData.brBtnFace);
	pDC->Draw3dRect (rectHeader, afxGlobalData.clrBtnShadow, afxGlobalData.clrBtnShadow);

	CDrawingManager dm (*pDC);
	dm.DrawShadow (rectHeader, 2);	

	CString strText;
	if (0 < m_strCategoryName.GetCount())
	{
		strText = m_strCategoryName.GetAt(nPage);
		//strText.Format (_T("Page %d description..."), nPage + 1);
	}

	CRect rectText = rectHeader;
	rectText.DeflateRect (10, 0);

	CFont* pOldFont = pDC->SelectObject (&afxGlobalData.fontBold);
	pDC->SetBkMode (TRANSPARENT);
	pDC->SetTextColor (afxGlobalData.clrBtnText);

	pDC->DrawText (strText, rectText, DT_SINGLELINE | DT_VCENTER);

	pDC->SelectObject (pOldFont);
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
LRESULT CSheetOption::OnPropertyChanged( WPARAM wParam, LPARAM lParam )
{
	GetDlgItem(ID_APPLY_NOW)->EnableWindow(TRUE);
	m_bItemChanged = TRUE;

	return TRUE;
}

//=============================================================================
// Method		: CSheetOption::InitPage
// Access		: public 
// Returns		: void
// Parameter	: CList<stPropertyPage_ID
// Parameter	: stPropertyPage_ID & > listPageID
// Qualifier	:
// Last Update	: 2010/10/11 - 11:37
// Desc.		:
//=============================================================================
void CSheetOption::InitPage( UINT nIDTemplate, CList<UINT, UINT> &listPageID )
{
	POSITION Pos = listPageID.GetHeadPosition();

	// 검사기 기본 설정	
	CPageOpt_Insp* m_pPageInspector	= new CPageOpt_Insp(nIDTemplate, listPageID.GetNext(Pos));
	m_pPageInspector->SetInspectorType(m_InsptrType);
	m_pPageInspector->SetLT_Option(&m_LT_Option);
	AddPage(m_pPageInspector);
	m_strCategoryName.Add(lpszOptionCategory[OPT_INSPECTOR]);
	m_ptrPage.Add ((CPageOption*)m_pPageInspector);

	// Server
 	CPageOpt_Server* m_pPageServer = new CPageOpt_Server(nIDTemplate, listPageID.GetNext(Pos));
	m_pPageServer->SetInspectorType(m_InsptrType);
	m_pPageServer->SetLT_Option(&m_LT_Option);
 	AddPage(m_pPageServer);
 	m_strCategoryName.Add(lpszOptionCategory[OPT_SERVER]);
 	m_ptrPage.Add((CPageOption*)m_pPageServer);


	// 기타
// 	CPageOpt_Misc* m_pPageMisc = new CPageOpt_Misc(nIDTemplate, listPageID.GetNext(Pos));
//	m_pPageMisc->SetInspectorType(m_InsptrType);
//	m_pPageMisc->SetUseEVMS(m_bUseEVMS);
// 	m_pPageMisc->SetLT_Option(&m_LT_Option);
// 	AddPage(m_pPageMisc);
// 	m_strCategoryName.Add(lpszOptionCategory[OPT_MISC]);
// 	m_ptrPage.Add((CPageOption*)m_pPageMisc);

}

//=============================================================================
// Method		: CSheetOption::InitPage
// Access		: public 
// Returns		: void
// Qualifier	:
// Last Update	: 2010/12/13 - 14:24
// Desc.		:
//=============================================================================
void CSheetOption::InitPage()
{
	// 검사기 기본 설정	
	CPageOpt_Insp* m_pPageInspector	= new CPageOpt_Insp(_T("System Setting"));
	m_pPageInspector->SetLT_Option(&m_LT_Option);
	AddPage(m_pPageInspector);
	m_strCategoryName.Add(lpszOptionCategory[OPT_INSPECTOR]);

	m_ptrPage.Add ((CPageOption*)m_pPageInspector);
}

//=============================================================================
// Method		: CSheetOption::OnBnClickedApplyNow
// Access		: protected 
// Returns		: void
// Qualifier	:
// Last Update	: 2010/9/3 - 14:47
// Desc.		:
//=============================================================================
void CSheetOption::OnBnClickedApplyNow()
{
	SaveOption();

	GetDlgItem(ID_APPLY_NOW)->EnableWindow(FALSE);
	m_bItemChanged = FALSE;	
}

//=============================================================================
// Method		: CSheetOption::OnOK
// Access		: protected 
// Returns		: void
// Qualifier	:
// Last Update	: 2010/9/3 - 14:47
// Desc.		:
// m_WMItemChanged -> WPARAM : Redraw Window
//					  LPARAM : Reconnect Network & Comport
//=============================================================================
void CSheetOption::OnOK()
{
	if (m_bItemChanged)	
		SaveOption();
	
	EndDialog (IDOK);
}

//=============================================================================
// Method		: CSheetOption::OnCancel
// Access		: protected 
// Returns		: void
// Qualifier	:
// Last Update	: 2013/2/20 - 15:46
// Desc.		:
//=============================================================================
void CSheetOption::OnCancel()
{
	EndDialog (IDCANCEL);
}

//=============================================================================
// Method		: CSheetOption::SaveOption
// Access		: public 
// Returns		: void
// Qualifier	:
// Last Update	: 2010/9/6 - 17:05
// Desc.		:
//=============================================================================
void CSheetOption::SaveOption()
{
	INT_PTR iCount = m_ptrPage.GetCount();

	for (int iPageIndex = 0; iPageIndex < iCount; iPageIndex++)
		((CPageOption*)m_ptrPage.GetAt(iPageIndex))->SaveOption();

	m_bSavedOption = TRUE;

	AfxGetApp()->GetMainWnd()->SendNotifyMessage(m_WMItemChanged, 0, 0);
}

//=============================================================================
// Method		: CSheetOption::LoadOption
// Access		: public 
// Returns		: void
// Qualifier	:
// Last Update	: 2010/9/6 - 17:05
// Desc.		:
//=============================================================================
void CSheetOption::LoadOption()
{
	INT_PTR iCount = m_ptrPage.GetCount();

	for (int iPageIndex = 0; iPageIndex < iCount; iPageIndex++)
		((CPageOption*)m_ptrPage.GetAt(iPageIndex))->LoadOption();
}

//=============================================================================
// Method		: SetInspectorType
// Access		: public  
// Returns		: void
// Parameter	: __in enInsptrSysType nInsptrType
// Qualifier	:
// Last Update	: 2016/9/26 - 18:00
// Desc.		:
//=============================================================================
void CSheetOption::SetInspectorType(__in enInsptrSysType nInsptrType)
{
	m_InsptrType = nInsptrType;
	m_LT_Option.SetInspectorType(nInsptrType);
}
