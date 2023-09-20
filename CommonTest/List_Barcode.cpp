//*****************************************************************************
// Filename	: 	List_Barcode.cpp
// Created	:	2016/10/22
// Modified	:	2016/10/22
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
// List_Barcode.cpp : implementation file
//

#include "stdafx.h"
#include "List_Barcode.h"

// CList_Barcode

typedef enum enBarcodeHeader
{
	BLH_Time,
	BLH_Barcode,
	BLH_TableIdx,
	BLH_MaxCol,
}enBarcodeHeader;

// 헤더
static const TCHAR*	g_lpszHeader[] = 
{
	_T("Time"),			// BLH_Time
	_T("Barcode"),		// BLH_Barcode
	_T("Sk"),			// BLH_TableIdx, Socket
	NULL 
};

const int	iListAglin[] = 
{ 
	LVCFMT_LEFT,	 // BLH_Time
	LVCFMT_LEFT,	 // BLH_Barcode
	LVCFMT_LEFT,	 // BLH_TableIdx
};

const int	iHeaderWidth[] = 
{
	57, 	// BLH_Time
	88,	// BLH_Barcode
	18,		// BLH_TableIdx
};


IMPLEMENT_DYNAMIC(CList_Barcode, CListCtrl)

CList_Barcode::CList_Barcode()
{	
	VERIFY(m_Font.CreateFont(
		14,						// nHeight
		0,						// nWidth
		0,						// nEscapement
		0,						// nOrientation
		FW_BOLD,				// nWeight
		FALSE,					// bItalic
		FALSE,					// bUnderline
		0,						// cStrikeOut
		ANSI_CHARSET,			// nCharSet
		OUT_DEFAULT_PRECIS,		// nOutPrecision
		CLIP_DEFAULT_PRECIS,	// nClipPrecision
		ANTIALIASED_QUALITY,	// nQuality
		FIXED_PITCH,			// nPitchAndFamily
		_T("CONSOLAS")));			// lpszFacename
}

CList_Barcode::~CList_Barcode()
{
	m_Font.DeleteObject();
}

BEGIN_MESSAGE_MAP(CList_Barcode, CListCtrl)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//ON_NOTIFY_REFLECT(NM_CLICK,	&CList_Barcode::OnNMClick)
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()


// CList_Barcode message handlers
//=============================================================================
// Method		: OnCreate
// Access		: public  
// Returns		: int
// Parameter	: LPCREATESTRUCT lpCreateStruct
// Qualifier	:
// Last Update	: 2016/3/21 - 9:52
// Desc.		:
//=============================================================================
int CList_Barcode::OnCreate(LPCREATESTRUCT lpCreateStruct)
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
// Access		: public  
// Returns		: void
// Parameter	: UINT nType
// Parameter	: int cx
// Parameter	: int cy
// Qualifier	:
// Last Update	: 2016/3/21 - 9:52
// Desc.		:
//=============================================================================
void CList_Barcode::OnSize(UINT nType, int cx, int cy)
{
	CListCtrl::OnSize(nType, cx, cy);

	if ((cx == 0) && (cy == 0))
		return;

	int iColWidth[BLH_MaxCol] = { 0, };
	int iColDivide = 0;
	int iUnitWidth = 0;
	int iMisc = 0;

	for (int nCol = 0; nCol < BLH_MaxCol; nCol++)
	{
		iColDivide += iHeaderWidth[nCol];
	}

	CRect rectClient;
	GetClientRect(rectClient);

	for (int nCol = 0; nCol < BLH_MaxCol; nCol++)
	{
		iUnitWidth = (rectClient.Width() * iHeaderWidth[nCol]) / iColDivide;
		iMisc += iUnitWidth;
		SetColumnWidth(nCol, iUnitWidth);
	}

	iUnitWidth = ((rectClient.Width() * iHeaderWidth[BLH_Time]) / iColDivide) + (rectClient.Width() - iMisc);
	SetColumnWidth(BLH_Time, iUnitWidth);
}

//=============================================================================
// Method		: PreCreateWindow
// Access		: virtual public  
// Returns		: BOOL
// Parameter	: CREATESTRUCT & cs
// Qualifier	:
// Last Update	: 2016/3/21 - 9:52
// Desc.		:
//=============================================================================
BOOL CList_Barcode::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL | WS_BORDER | WS_TABSTOP;
	cs.dwExStyle &= LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;

	return CListCtrl::PreCreateWindow(cs);
}

//=============================================================================
// Method		: OnNMClick
// Access		: public  
// Returns		: void
// Parameter	: NMHDR * pNMHDR
// Parameter	: LRESULT * pResult
// Qualifier	:
// Last Update	: 2016/3/21 - 9:52
// Desc.		:
//=============================================================================
void CList_Barcode::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
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
// Last Update	: 2016/3/21 - 16:11
// Desc.		:
//=============================================================================
void CList_Barcode::InitHeader()
{
	for (int nCol = 0; nCol < BLH_MaxCol; nCol++)
	{
		InsertColumn(nCol, g_lpszHeader[nCol], iListAglin[nCol], iHeaderWidth[nCol]);
	}
}

//=============================================================================
// Method		: ClearWorklist
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2016/8/12 - 17:13
// Desc.		:
//=============================================================================
void CList_Barcode::ClearWorklist()
{
	this->DeleteAllItems();
}

//=============================================================================
// Method		: InsertBarcode
// Access		: public  
// Returns		: void
// Parameter	: __in LPCTSTR szBarcode
// Qualifier	:
// Last Update	: 2016/10/22 - 16:51
// Desc.		:
//=============================================================================
void CList_Barcode::InsertBarcode(__in LPCTSTR szBarcode)
{
	ASSERT(GetSafeHwnd());

	int iNewCount = GetItemCount();

	CString szTime;
	SYSTEMTIME tmLocal;
	GetLocalTime(&tmLocal);

	szTime.Format(_T("%02d:%02d:%02d.%03d"), tmLocal.wHour, tmLocal.wMinute, tmLocal.wSecond, tmLocal.wMilliseconds);

	InsertItem(iNewCount, _T(""));
	
	SetItemText(iNewCount, BLH_Time, szTime);

	SetItemText(iNewCount, BLH_Barcode, szBarcode);

	// 화면에 보이게 하기
	EnsureVisible(iNewCount, TRUE);
	ListView_SetItemState(GetSafeHwnd(), iNewCount, LVIS_FOCUSED | LVIS_SELECTED, 0x000F);
}

//=============================================================================
// Method		: InsertBarcode
// Access		: public  
// Returns		: void
// Parameter	: __in LPCTSTR szBarcode
// Parameter	: __in UINT nTableIdx
// Qualifier	:
// Last Update	: 2016/10/22 - 19:39
// Desc.		:
//=============================================================================
void CList_Barcode::InsertBarcode(__in LPCTSTR szBarcode, __in UINT nTableIdx)
{
	ASSERT(GetSafeHwnd());

	int iNewCount = GetItemCount();

	CString szItem;
	SYSTEMTIME tmLocal;
	GetLocalTime(&tmLocal);

	szItem.Format(_T("%02d:%02d:%02d.%1d"), tmLocal.wHour, tmLocal.wMinute, tmLocal.wSecond, tmLocal.wMilliseconds);

	InsertItem(iNewCount, _T(""));

	SetItemText(iNewCount, BLH_Time, szItem);

	SetItemText(iNewCount, BLH_Barcode, szBarcode);

	szItem.Format(_T("%d"), nTableIdx);
	SetItemText(iNewCount, BLH_TableIdx, szItem);

	// 화면에 보이게 하기
	EnsureVisible(iNewCount, TRUE);
	ListView_SetItemState(GetSafeHwnd(), iNewCount, LVIS_FOCUSED | LVIS_SELECTED, 0x000F);
}

