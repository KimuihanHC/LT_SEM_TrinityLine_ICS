//*****************************************************************************
// Filename	: 	List_Alarm.cpp
// Created	:	2021/11/1 - 15:16
// Modified	:	2021/11/1 - 15:16
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#include "stdafx.h"
#include "List_Alarm.h"
#include "Def_Constant.h"

typedef enum 
{
	TSH_No,
	TSH_RFID,			// RFID
	TSH_SocketType,		// Socket Type
	TSH_Comment,		// Comment
	TSH_Barcode,		// Barcode
	TSH_Status,			// Status
	TSH_Equipment,		// Equipment
	TSH_Location,		// Location
	TSH_Target,			// Target Equipment
	TSH_Yield_Total,	// Total
	TSH_Yield_Pass,		// Pass 
	TSH_Yield_Fail,		// Fail

	TSH_MaxCol,
}enTestStepHeader;

// 헤더
static const TCHAR*	g_lpszHeader[] =
{
	_T("No"),					// TSH_No
	_T("RFID"),					// TSH_RFID
	_T("Socket Type"),			// TSH_SocketType
	_T("Comment"),				// TSH_Comment
	_T("Barcode"),				// TSH_Barcode
	_T("Status"),				// TSH_Status
	_T("Equipment"),			// TSH_Equipment
	_T("Location in Eqp."),		// TSH_Location
	_T("Target Eqp."),			// TSH_Target
	_T("Total"),				// TSH_Cnt_Total
	_T("Pass "),				// TSH_Cnt_Pass
	_T("Fail"),					// TSH_Cnt_Fail

	NULL
};

const int	iListAglin[] =
{
	LVCFMT_LEFT,	 // TSH_No
	LVCFMT_LEFT,	 // TSH_RFID
	LVCFMT_LEFT,	 // TSH_SocketType
	LVCFMT_LEFT,	 // TSH_Comment
	LVCFMT_LEFT,	 // TSH_Barcode
	LVCFMT_LEFT,	 // TSH_Status
	LVCFMT_LEFT,	 // TSH_Equipment
	LVCFMT_LEFT,	 // TSH_Location
	LVCFMT_LEFT,	 // TSH_Target
	LVCFMT_LEFT,	 // TSH_Cnt_Total
	LVCFMT_LEFT,	 // TSH_Cnt_Pass
	LVCFMT_LEFT,	 // TSH_Cnt_Fail
};

// 540 기준
const int	iHeaderWidth[] =
{
	40, 	// TSH_No
	200,	// TSH_RFID
	150,	// TSH_SocketType
	0,		// TSH_Comment
	200,	// TSH_Barcode
	100,	// TSH_Status
	150,	// TSH_Equipment
	150,	// TSH_Location
	150,	// TSH_Target
	150,	// TSH_Cnt_Total
	150,	// TSH_Cnt_Pass
	150,	// TSH_Cnt_Fail
};

IMPLEMENT_DYNAMIC(CList_Alarm, CListCtrl)

CList_Alarm::CList_Alarm()
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

CList_Alarm::~CList_Alarm()
{
	m_Font.DeleteObject();
}


BEGIN_MESSAGE_MAP(CList_Alarm, CListCtrl)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_NOTIFY_REFLECT(NM_CLICK,	&CList_Alarm::OnNMClick)
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()



// CList_Alarm message handlers
//=============================================================================
// Method		: OnCreate
// Access		: protected  
// Returns		: int
// Parameter	: LPCREATESTRUCT lpCreateStruct
// Qualifier	:
// Last Update	: 2021/11/2 - 14:28
// Desc.		:
//=============================================================================
int CList_Alarm::OnCreate(LPCREATESTRUCT lpCreateStruct)
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
void CList_Alarm::OnSize(UINT nType, int cx, int cy)
{
	CListCtrl::OnSize(nType, cx, cy);
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
BOOL CList_Alarm::PreCreateWindow(CREATESTRUCT& cs)
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
// int CList_Alarm::OnCompareItems(LPARAM lParam1, LPARAM lParam2, int iColumn)
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
void CList_Alarm::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
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
void CList_Alarm::InitHeader()
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
// Method		: Find_Row
// Access		: protected  
// Returns		: int
// Parameter	: __in LPCTSTR IN_szRFID
// Qualifier	:
// Last Update	: 2021/12/22 - 11:45
// Desc.		:
//=============================================================================
int CList_Alarm::Find_Row(__in LPCTSTR IN_szRFID)
{
	CString szText;
	int nRetRow = -1;
	for (int nRow = 0; nRow < GetItemCount(); nRow++)
	{
		szText = GetItemText(nRow, TSH_RFID);

		if (0 == szText.Compare(IN_szRFID))
		{
			nRetRow = nRow;
			break;
		}
	}

	return nRetRow;
}

//=============================================================================
// Method		: Reset_Number
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/11/2 - 14:29
// Desc.		:
//=============================================================================
void CList_Alarm::Reset_Number()
{
	CString szText;

	for (int nRow = 0; nRow < GetItemCount(); nRow++)
	{
		// TSH_No
		szText.Format(_T("%d"), nRow + 1);
		SetItemText(nRow, TSH_No, szText);
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
void CList_Alarm::Select_SelectItem(__in int nItem)
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
void CList_Alarm::Set_SocketData(__in int nItem, __in const CSocketInfo_Unit* IN_pSocket)
{
	ASSERT(GetSafeHwnd());
	if (NULL == IN_pSocket)
		return;

 	if (GetItemCount() <= nItem)
 		return;

	CString szText;

	// TSH_No
	szText.Format(_T("%d"), nItem + 1);
	SetItemText(nItem, TSH_No, szText);

	// RFID
	SetItemText(nItem, TSH_RFID, IN_pSocket->szRFID);

	// Socket Type
	SetItemText(nItem, TSH_SocketType, g_szSocketTypeName[IN_pSocket->nSocketType]);

	// Comment
	SetItemText(nItem, TSH_Comment, IN_pSocket->szComment);
	
	// Barcode
	SetItemText(nItem, TSH_Barcode, IN_pSocket->m_szBarcode);

	// Status
	SetItemText(nItem, TSH_Status, g_szSocketStatus[IN_pSocket->m_nStatus]);

	// Equipment
	SetItemText(nItem, TSH_Equipment, IN_pSocket->m_szEquipmentID);

	// Location

	// Target Equipment
	SetItemText(nItem, TSH_Target, IN_pSocket->m_szTargetEqpID);

	// Total
	szText.Format(_T("%d"), IN_pSocket->m_Yield.dwTotal);
	SetItemText(nItem, TSH_Yield_Total, szText);
	// Pass 
	szText.Format(_T("%d"), IN_pSocket->m_Yield.dwPass);
	SetItemText(nItem, TSH_Yield_Pass, szText);
	// Fail
	szText.Format(_T("%d"), IN_pSocket->m_Yield.dwFail);
	SetItemText(nItem, TSH_Yield_Fail, szText);

}

void CList_Alarm::Set_SocketData_Location(__in int nItem, __in const CSocketInfo_Unit* IN_pSocket)
{
	ASSERT(GetSafeHwnd());
	if (NULL == IN_pSocket)
		return;

	if (GetItemCount() <= nItem)
		return;

	CString szText;

	// Equipment
	SetItemText(nItem, TSH_Equipment, IN_pSocket->m_szEquipmentID);

	// Location

	// Target Equipment
	SetItemText(nItem, TSH_Target, IN_pSocket->m_szTargetEqpID);

	
}

void CList_Alarm::Set_SocketData_Product(__in int nItem, __in const CSocketInfo_Unit* IN_pSocket)
{
	ASSERT(GetSafeHwnd());
	if (NULL == IN_pSocket)
		return;

	if (GetItemCount() <= nItem)
		return;

	CString szText;
	
	// Barcode
	SetItemText(nItem, TSH_Barcode, IN_pSocket->m_szBarcode);

	// Status
	SetItemText(nItem, TSH_Status, g_szSocketStatus[IN_pSocket->m_nStatus]);
}

void CList_Alarm::Set_SocketData_Yield(__in int nItem, __in const CSocketInfo_Unit* IN_pSocket)
{
	ASSERT(GetSafeHwnd());
	if (NULL == IN_pSocket)
		return;

	if (GetItemCount() <= nItem)
		return;

	CString szText;

	// Total
	szText.Format(_T("%d"), IN_pSocket->m_Yield.dwTotal);
	SetItemText(nItem, TSH_Yield_Total, szText);
	// Pass 
	szText.Format(_T("%d"), IN_pSocket->m_Yield.dwPass);
	SetItemText(nItem, TSH_Yield_Pass, szText);
	// Fail
	szText.Format(_T("%d"), IN_pSocket->m_Yield.dwFail);
	SetItemText(nItem, TSH_Yield_Fail, szText);
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
void CList_Alarm::Add_Socket(const __in CSocketInfo_Unit* IN_pSocket)
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

//=============================================================================
// Method		: Set_SocketList
// Access		: public  
// Returns		: void
// Parameter	: __in const CSocketList * IN_pSoscketList
// Qualifier	:
// Last Update	: 2021/11/2 - 14:30
// Desc.		:
//=============================================================================
void CList_Alarm::Set_SocketList(__in const CSocketMonitoring* IN_pSoscketList)
{
	m_stSocketList.RemoveAll();
	DeleteAllItems();

	if (NULL != IN_pSoscketList)
	{
		m_stSocketList = *IN_pSoscketList;

// 		auto eIter = IN_pSoscketList->m_Sockets.end();
// 		auto Iter  = IN_pSoscketList->m_Sockets.begin();
// 		while (Iter != eIter)
// 		{
// 			Add_Socket(&Iter->second);
// 
// 			Iter++;
// 		}
	}
}

//=============================================================================
// Method		: Get_SocketList
// Access		: public  
// Returns		: void
// Parameter	: __out CSocketList & OUT_stSocketList
// Qualifier	:
// Last Update	: 2021/11/2 - 14:30
// Desc.		:
//=============================================================================
void CList_Alarm::Get_SocketList(__out CSocketMonitoring& OUT_stSocketList)
{
	OUT_stSocketList.RemoveAll();
	OUT_stSocketList = m_stSocketList;
}

//=============================================================================
// Method		: Get_SelectedSocket
// Access		: public  
// Returns		: bool
// Parameter	: __out CConfig_Socket & OUT_Socket
// Qualifier	:
// Last Update	: 2021/11/2 - 14:31
// Desc.		:
//=============================================================================
bool CList_Alarm::Get_SelectedSocket(__out CSocketInfo_Unit& OUT_Socket)
{
	if (0 < GetSelectedCount())
	{
		POSITION nPos = GetFirstSelectedItemPosition();
		int iIndex = GetNextSelectedItem(nPos);

		CString szRFID = GetItemText(iIndex, TSH_RFID);

		OUT_Socket = m_stSocketList.GetAt(szRFID.GetBuffer());

		return true;
	}
	else
	{
		return false;
	}
}

//=============================================================================
// Method		: Clear_SocketList
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/11/2 - 14:31
// Desc.		:
//=============================================================================
void CList_Alarm::Clear_SocketList()
{
	m_stSocketList.RemoveAll();
	DeleteAllItems();
}

//=============================================================================
// Method		: Refresh_Socket
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/11/17 - 15:50
// Desc.		:
//=============================================================================
void CList_Alarm::Refresh_Socket()
{
	DeleteAllItems();

// 	auto eIter = m_stSocketList.m_Sockets.end();
// 	auto Iter = m_stSocketList.m_Sockets.begin();
// 	while (Iter != eIter)
// 	{
// 		Add_Socket(&Iter->second);
// 
// 		Iter++;
// 	}
}

//=============================================================================
// Method		: Update_Socket
// Access		: protected  
// Returns		: void
// Parameter	: __in int nRow
// Parameter	: __in const CSocketInfo_Unit * IN_pSocket
// Qualifier	:
// Last Update	: 2021/11/2 - 14:30
// Desc.		:
//=============================================================================
void CList_Alarm::Update_Socket(__in int nRow, __in const CSocketInfo_Unit* IN_pSocket)
{
	ASSERT(GetSafeHwnd());
	if (NULL == IN_pSocket)
		return;

	if (GetItemCount() <= nRow)
		return;

	Set_SocketData(nRow, IN_pSocket);

	// 화면에 보이게 하기
	//EnsureVisible(nRow, TRUE);
	//ListView_SetItemState(GetSafeHwnd(), nRow, LVIS_FOCUSED | LVIS_SELECTED, 0x000F);
}

void CList_Alarm::Update_Socket(__in LPCTSTR IN_szRFID, __in const CSocketInfo_Unit* IN_pSocket)
{
	int nRow = Find_Row(IN_szRFID);

	if (0 <= nRow)
	{
		Update_Socket(nRow, IN_pSocket);
	}
}

void CList_Alarm::Update_Socket_Location(__in int nRow, __in const CSocketInfo_Unit* IN_pSocket)
{
	ASSERT(GetSafeHwnd());
	if (NULL == IN_pSocket)
		return;

	if (GetItemCount() <= nRow)
		return;

	Set_SocketData_Location(nRow, IN_pSocket);
}

void CList_Alarm::Update_Socket_Product(__in int nRow, __in const CSocketInfo_Unit* IN_pSocket)
{
	ASSERT(GetSafeHwnd());
	if (NULL == IN_pSocket)
		return;

	if (GetItemCount() <= nRow)
		return;

	Set_SocketData_Product(nRow, IN_pSocket);
}

void CList_Alarm::Update_Socket_Yield(__in int nRow, __in const CSocketInfo_Unit* IN_pSocket)
{
	ASSERT(GetSafeHwnd());
	if (NULL == IN_pSocket)
		return;

	if (GetItemCount() <= nRow)
		return;

	Set_SocketData_Yield(nRow, IN_pSocket);
}

//=============================================================================
// Method		: Find_Socket
// Access		: public  
// Returns		: int
// Parameter	: __in LPCTSTR IN_szRFID
// Qualifier	:
// Last Update	: 2021/12/22 - 17:31
// Desc.		:
//=============================================================================
int CList_Alarm::Find_Socket(__in LPCTSTR IN_szRFID)
{
	int nRow = Find_Row(IN_szRFID);

	if (0 <= nRow)
	{
		// 선택 보이게 하기
		EnsureVisible(nRow, TRUE);
		ListView_SetItemState(GetSafeHwnd(), nRow, LVIS_FOCUSED | LVIS_SELECTED, 0x000F);
	}
	else
	{
		int nIndex = GetNextItem(-1, LVNI_SELECTED);
		if (nIndex >= 0)
		{
			SetItemState(nIndex, 0);
		}
	}

	return nRow;
}
