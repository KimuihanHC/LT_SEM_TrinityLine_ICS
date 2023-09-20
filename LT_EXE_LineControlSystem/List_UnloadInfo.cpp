//*****************************************************************************
// Filename	: 	List_UnloadInfo.cpp
// Created	:	2021/11/1 - 15:16
// Modified	:	2021/11/1 - 15:16
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#include "stdafx.h"
#include "List_UnloadInfo.h"

typedef enum 
{
	TSH_Time,		// Time (HH:MM::SS)
	TSH_RFID,		// RFID (H-001)
	TSH_SN,			// S/N  (뒤 16자리)  SSMS220420000029, SSMS220080000002
	TSH_NGCode,		// NG Code	(0, -1, 1, )
	TSH_Test,		// NG Test (SC, SMH, SMV, HP, CC, GF, DI)

	TSH_MaxCol,
}enTestStepHeader;

// 헤더
static const TCHAR*	g_lpszHeader[] =
{
	_T("Time"),		// TSH_Time
	_T("RFID"),		// TSH_RFID
	_T("S/N"),		// TSH_SN
	_T("NG Code"),	// TSH_NGCode
	_T("Test"),		// TSH_Test
	NULL
};

const int	iListAglin[] =
{
	LVCFMT_LEFT,	 // TSH_Time
	LVCFMT_LEFT,	 // TSH_RFID
	LVCFMT_LEFT,	 // TSH_SN
	LVCFMT_LEFT,	 // TSH_NGCode
	LVCFMT_LEFT,	 // TSH_Test
};

// 540 기준
const int	iHeaderWidth[] =
{
	100, 	// TSH_Time
	100,	// TSH_RFID
	150,	// TSH_SN
	100,	// TSH_NGCode
	100,	// TSH_Test
};

IMPLEMENT_DYNAMIC(CList_UnloadInfo, CListCtrl)

CList_UnloadInfo::CList_UnloadInfo()
{
	m_Font.CreateStockObject(DEFAULT_GUI_FONT);
	// 	VERIFY(m_Font.CreateFont(
	// 		16,						// nHeight
	// 		0,						// nWidth
	// 		0,						// nEscapement
	// 		0,						// nOrientation
	// 		FW_BOLD,				// nWeight
	// 		FALSE,					// bItalic
	// 		FALSE,					// bUnderline
	// 		0,						// cStrikeOut
	// 		ANSI_CHARSET,			// nCharSet
	// 		OUT_DEFAULT_PRECIS,		// nOutPrecision
	// 		CLIP_DEFAULT_PRECIS,	// nClipPrecision
	// 		ANTIALIASED_QUALITY,	// nQuality
	// 		FIXED_PITCH,			// nPitchAndFamily
	// 		_T("CONSOLAS")));		// lpszFacename

	m_pHeadWidth = iHeaderWidth;
}

CList_UnloadInfo::~CList_UnloadInfo()
{
	m_Font.DeleteObject();
}


BEGIN_MESSAGE_MAP(CList_UnloadInfo, CListCtrl)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_NOTIFY_REFLECT(NM_CLICK,	&CList_UnloadInfo::OnNMClick)
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()



// CList_UnloadInfo message handlers
//=============================================================================
// Method		: OnCreate
// Access		: protected  
// Returns		: int
// Parameter	: LPCREATESTRUCT lpCreateStruct
// Qualifier	:
// Last Update	: 2021/11/2 - 14:28
// Desc.		:
//=============================================================================
int CList_UnloadInfo::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetFont(&m_Font);

	SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER | LVS_EX_LABELTIP);	
	
//	EnableMarkSortedColumn();

	InitHeader();

	//this->GetHeaderCtrl()->EnableWindow(FALSE);	

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
// Last Update	: 2021/11/2 - 14:28
// Desc.		:
//=============================================================================
void CList_UnloadInfo::OnSize(UINT nType, int cx, int cy)
{
	CListCtrl::OnSize(nType, cx, cy);

	// 	if ((cx == 0) && (cy == 0))
	// 		return;

	/*int iColWidth[TSH_MaxCol] = { 0, };
	int iColDivide = 0;
	int iUnitWidth = 0;
	int iMisc = 0;
	int iMaxCol = TSH_MaxCol;

	for (int nCol = 0; nCol < iMaxCol; nCol++)
	{
	iColDivide += iHeaderWidth[nCol];
	}

	//CRect rectClient;
	//GetClientRect(rectClient);

	for (int nCol = 0; nCol < iMaxCol; nCol++)
	{
	iUnitWidth = (cx * m_pHeadWidth[nCol]) / iColDivide;
	iMisc += iUnitWidth;
	SetColumnWidth(nCol, iUnitWidth);
	}

	iUnitWidth = ((cx * m_pHeadWidth[TSH_ItemName]) / iColDivide) + (cx - iMisc);
	SetColumnWidth(TSH_ItemName, iUnitWidth);*/
}

//=============================================================================
// Method		: PreCreateWindow
// Access		: virtual protected  
// Returns		: BOOL
// Parameter	: CREATESTRUCT & cs
// Qualifier	:
// Last Update	: 2021/11/2 - 14:29
// Desc.		:
//=============================================================================
BOOL CList_UnloadInfo::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL | WS_BORDER | WS_TABSTOP;
	cs.dwExStyle &= LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;

	return CListCtrl::PreCreateWindow(cs);
}

//=============================================================================
// Method		: OnCompareItems
// Access		: virtual protected  
// Returns		: int
// Parameter	: LPARAM lParam1
// Parameter	: LPARAM lParam2
// Parameter	: int iColumn
// Qualifier	:
// Last Update	: 2021/11/16 - 10:07
// Desc.		:
//=============================================================================
// int CList_UnloadInfo::OnCompareItems(LPARAM lParam1, LPARAM lParam2, int iColumn)
// {
// 	CString strItem1 = GetItemText((int)(lParam1 < lParam2 ? lParam1 : lParam2), iColumn);
// 	CString strItem2 = GetItemText((int)(lParam1 < lParam2 ? lParam2 : lParam1), iColumn);
// 
// 	if (iColumn == 0)
// 	{
// 		int nItem1 = _ttoi(strItem1);
// 		int nItem2 = _ttoi(strItem2);
// 
// 		return(nItem1 < nItem2 ? -1 : 1);
// 	}
// 	else
// 	{
// 		int iSort = _tcsicmp(strItem1, strItem2);
// 
// 		return(iSort);
// 	}
// }

//=============================================================================
// Method		: OnNMClick
// Access		: protected  
// Returns		: void
// Parameter	: NMHDR * pNMHDR
// Parameter	: LRESULT * pResult
// Qualifier	:
// Last Update	: 2021/11/2 - 14:29
// Desc.		:
//=============================================================================
void CList_UnloadInfo::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
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
// Last Update	: 2021/11/2 - 14:29
// Desc.		:
//=============================================================================
void CList_UnloadInfo::InitHeader()
{
	if (FALSE == m_bIntiHeader)
	{
		m_bIntiHeader = TRUE;

		for (int nCol = 0; nCol < TSH_MaxCol; nCol++)
		{
			InsertColumn(nCol, g_lpszHeader[nCol], iListAglin[nCol], iHeaderWidth[nCol]);
		}
	}

	for (int nCol = 0; nCol < TSH_MaxCol; nCol++)
	{
		SetColumnWidth(nCol, m_pHeadWidth[nCol]);
	}

}

//=============================================================================
// Method		: Select_SelectItem
// Access		: protected  
// Returns		: void
// Parameter	: __in int nItem
// Qualifier	:
// Last Update	: 2021/11/2 - 14:29
// Desc.		:
//=============================================================================
void CList_UnloadInfo::Select_SelectItem(__in int nItem)
{
	if (nItem < GetItemCount())
	{
		SetHotItem(nItem);
		SetItemState(nItem, LVIS_FOCUSED, LVIS_FOCUSED);
		SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED);
		SetFocus();
	}
}

//=============================================================================
// Method		: Set_SocketData
// Access		: protected  
// Returns		: void
// Parameter	: __in int nItem
// Parameter	: __in const CSocketInfo_Unit * IN_pSocket
// Qualifier	:
// Last Update	: 2021/11/2 - 14:29
// Desc.		:
//=============================================================================
void CList_UnloadInfo::Set_SocketData(__in int nItem, __in const CSocketInfo_Unit* IN_pSocket)
{
	ASSERT(GetSafeHwnd());
	if (NULL == IN_pSocket)
		return;

 	if (GetItemCount() <= nItem)
 		return;

	CString szText;

	// TSH_No
	//szText.Format(_T("%d"), nItem + 1);
	//SetItemText(nItem, TSH_No, szText);

	// RFID
	SetItemText(nItem, TSH_RFID, IN_pSocket->szRFID);

	// Socket Type
	//SetItemText(nItem, TSH_SocketType, g_szSocketTypeName[IN_pSocket->nSocketType]);

	// Comment
	//SetItemText(nItem, TSH_Comment, IN_pSocket->szComment);
	
	
}

//=============================================================================
// Method		: Add_Socket
// Access		: protected  
// Returns		: void
// Parameter	: const __in CSocketInfo_Unit * IN_pSocket
// Qualifier	:
// Last Update	: 2021/11/2 - 14:29
// Desc.		:
//=============================================================================
void CList_UnloadInfo::Add_Socket(const __in CSocketInfo_Unit* IN_pSocket)
{
	ASSERT(GetSafeHwnd());
	if (NULL == IN_pSocket)
		return;

	int iNewCount = GetItemCount();

	// CListCtrl
	InsertItem(iNewCount, _T(""));

	// CListCtrl
	//AddItem(_T(""));
	//InsertItem(iNewCount, _T(""));

	Set_SocketData(iNewCount, IN_pSocket);

	// 화면에 보이게 하기
	EnsureVisible(iNewCount, TRUE);
	ListView_SetItemState(GetSafeHwnd(), iNewCount, LVIS_FOCUSED | LVIS_SELECTED, 0x000F);
}
