//*****************************************************************************
// Filename	: 	List_Error.cpp
// Created	:	2016/10/29 - 17:06
// Modified	:	2016/10/29 - 17:06
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
// List_Error.cpp : implementation file
//

#include "stdafx.h"
#include "List_Error.h"
#include "CommonFunction.h"

// CList_Error

typedef enum enErrorlistHeader
{
	ELH_No,
	ELH_Time,
	ELH_Code,
	ELH_Type,
	ELH_Desc,
	ELH_MaxCol,
}enErrorlistHeader;

// 헤더
static const TCHAR*	g_lpszHeader[] = 
{
	_T("No"),		// ELH_No
	_T("Time"),		// ELH_Time
	_T("Code"),		// ELH_Code
	_T("Type"),		// ELH_Type
	_T("Desc."),	// ELH_Desc
	NULL
};

const int	iListAglin[] = 
{ 
	LVCFMT_LEFT,	 // ELH_No
	LVCFMT_CENTER,	 // ELH_Time
	LVCFMT_CENTER, 	 // ELH_Code
	LVCFMT_CENTER,	 // ELH_Type
	LVCFMT_CENTER,	 // ELH_Desc
};

const int	iHeaderWidth[] = 
{
	50, 	// ELH_No
	150,	// ELH_Time
	100,	// ELH_Code
	150,	// ELH_Type
	600,	// ELH_Desc
};


IMPLEMENT_DYNAMIC(CList_Error, CListCtrl)

CList_Error::CList_Error()
{
	m_Font.CreateStockObject(DEFAULT_GUI_FONT);
}

CList_Error::~CList_Error()
{
	m_Font.DeleteObject();
}

BEGIN_MESSAGE_MAP(CList_Error, CListCtrl)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//ON_NOTIFY_REFLECT(NM_CLICK,	&CList_Error::OnNMClick)
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()


// CList_Error message handlers
//=============================================================================
// Method		: OnCreate
// Access		: protected  
// Returns		: int
// Parameter	: LPCREATESTRUCT lpCreateStruct
// Qualifier	:
// Last Update	: 2016/10/29 - 17:13
// Desc.		:
//=============================================================================
int CList_Error::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetFont(&m_Font);

	SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER);	

	InitHeader();

	return 0;
}

//=============================================================================
// Method		: OnSize
// Access		: protected  
// Returns		: void
// Parameter	: UINT nType
// Parameter	: int cx
// Parameter	: int cy
// Qualifier	:
// Last Update	: 2016/10/29 - 17:13
// Desc.		:
//=============================================================================
void CList_Error::OnSize(UINT nType, int cx, int cy)
{
	CListCtrl::OnSize(nType, cx, cy);

	if ((cx == 0) && (cy == 0))
		return;

	int iColWidth[ELH_MaxCol] = { 0, };
	int iColDivide = 0;
	int iUnitWidth = 0;
	int iMisc = 0;

	for (int nCol = 0; nCol < ELH_MaxCol; nCol++)
	{
		iColDivide += iHeaderWidth[nCol];
	}

	CRect rectClient;
	GetClientRect(rectClient);

	for (int nCol = 0; nCol < ELH_MaxCol; nCol++)
	{
		iUnitWidth = (rectClient.Width() * iHeaderWidth[nCol]) / iColDivide;
		iMisc += iUnitWidth;
		SetColumnWidth(nCol, iUnitWidth);
	}

	iUnitWidth = ((rectClient.Width() * iHeaderWidth[ELH_No]) / iColDivide) + (rectClient.Width() - iMisc);
	SetColumnWidth(ELH_No, iUnitWidth);
}

//=============================================================================
// Method		: PreCreateWindow
// Access		: virtual protected  
// Returns		: BOOL
// Parameter	: CREATESTRUCT & cs
// Qualifier	:
// Last Update	: 2016/10/29 - 17:12
// Desc.		:
//=============================================================================
BOOL CList_Error::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL | WS_BORDER | WS_TABSTOP;
	cs.dwExStyle &= LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;

	return CListCtrl::PreCreateWindow(cs);
}

//=============================================================================
// Method		: OnNMClick
// Access		: protected  
// Returns		: void
// Parameter	: NMHDR * pNMHDR
// Parameter	: LRESULT * pResult
// Qualifier	:
// Last Update	: 2016/10/29 - 17:12
// Desc.		:
//=============================================================================
void CList_Error::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	NM_LISTVIEW* pNMView = (NM_LISTVIEW*)pNMHDR;
	int index = pNMView->iItem;

	*pResult = 0;
}

//=============================================================================
// Method		: InitHeader
// Access		: virtual protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2016/10/29 - 17:12
// Desc.		:
//=============================================================================
void CList_Error::InitHeader()
{
	for (int nCol = 0; nCol < ELH_MaxCol; nCol++)
	{
		InsertColumn(nCol, g_lpszHeader[nCol], iListAglin[nCol], iHeaderWidth[nCol]);
	}
}

//=============================================================================
// Method		: InsertRowItemz
// Access		: virtual protected  
// Returns		: void
// Parameter	: __in const ST_ErrorInfo * pstErrorList
// Qualifier	:
// Last Update	: 2016/10/29 - 17:12
// Desc.		:
//=============================================================================
void CList_Error::InsertRowItemz(__in const ST_ErrorInfo* pstErrorList)
{
	ASSERT(GetSafeHwnd());

	int iNewCount = GetItemCount();

	CString strText;

	InsertItem(iNewCount, _T(""));
	
	SetRowItemz(iNewCount, pstErrorList);

 	strText.Format(_T("%d"), iNewCount + 1);
 	SetItemText(iNewCount, ELH_No, strText);
 
}

//=============================================================================
// Method		: SetRowItemz
// Access		: protected  
// Returns		: void
// Parameter	: UINT nRow
// Parameter	: __in const ST_ErrorInfo * pstErrorList
// Qualifier	:
// Last Update	: 2016/10/29 - 17:12
// Desc.		:
//=============================================================================
void CList_Error::SetRowItemz(UINT nRow, __in const ST_ErrorInfo* pstErrorList)
{
	CString szItem;

	// 에러 발생시간
	szItem = SystemTimeToFormatString(pstErrorList->tmTime);
	SetItemText(nRow, ELH_Time, szItem);

	// 에러 코드	
	szItem.Format(_T("%d"), pstErrorList->lCode);
	SetItemText(nRow, ELH_Code, szItem);

	// 에러 종류
	szItem.Format(_T("%d"), pstErrorList->nType);
	SetItemText(nRow, ELH_Type, szItem);

	// 에러 설명
	SetItemText(nRow, ELH_Desc, pstErrorList->szDesc);

}

//=============================================================================
// Method		: InsertErrorInfo
// Access		: public  
// Returns		: void
// Parameter	: __in const ST_ErrorInfo * pstErrorList
// Qualifier	:
// Last Update	: 2016/10/29 - 17:12
// Desc.		:
//=============================================================================
void CList_Error::InsertErrorInfo(__in const ST_ErrorInfo* pstErrorList)
{
 	m_Errorlist.Add(*(ST_ErrorInfo*)pstErrorList);
 
 	InsertRowItemz(pstErrorList);
 
 	// 500개가 되면 300개 삭제??
//  	int iCount = GetItemCount();
//  	if (500 < iCount)
//  	{
//  		for (int iCnt = 0; iCnt < 300; iCnt++)
//  		{
//  			DeleteItem(0);
//  		}
//  		m_Errorlist.RemoveAt(0, 300);
//  
//  		iCount = GetItemCount();
//  		CString strText;
//  		for (int iIdx = 0; iIdx < iCount; iIdx++)
//  		{
//  			strText.Format(_T("%d"), iIdx + 1);
//  			SetItemText(iIdx, ELH_No, strText);
//  		}
//  	}
}

//=============================================================================
// Method		: ClearWorklist
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2016/10/29 - 17:12
// Desc.		:
//=============================================================================
void CList_Error::ClearErrorlist()
{
	m_Errorlist.RemoveAll();

	this->DeleteAllItems();
}

//=============================================================================
// Method		: GetErrorInfo
// Access		: public  
// Returns		: BOOL
// Parameter	: __in UINT nRowIndex
// Parameter	: __out ST_ErrorInfo & stErrorList
// Qualifier	:
// Last Update	: 2016/10/29 - 17:12
// Desc.		:
//=============================================================================
BOOL CList_Error::GetErrorInfo(__in UINT nRowIndex, __out ST_ErrorInfo& stErrorList)
{
	if (nRowIndex < 0)
		return FALSE;

	INT_PTR iCount = m_Errorlist.GetCount();

	if (iCount <= (int)nRowIndex)
		return FALSE;

	stErrorList = m_Errorlist.GetAt(nRowIndex);

	return TRUE;
}

//=============================================================================
// Method		: DeleteError
// Access		: public  
// Returns		: void
// Parameter	: __in UINT nRowIndex
// Qualifier	:
// Last Update	: 2016/10/29 - 17:17
// Desc.		:
//=============================================================================
void CList_Error::DeleteError(__in UINT nRowIndex)
{
// 	INT_PTR iCount = m_Errorlist.GetCount();
// 
// 	m_Errorlist.RemoveAt(nRowIndex, 1);

	
}
