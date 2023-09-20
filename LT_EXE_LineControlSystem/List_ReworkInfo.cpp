//*****************************************************************************
// Filename	: 	List_ReworkInfo.cpp
// Created	:	2021/11/1 - 15:16
// Modified	:	2021/11/1 - 15:16
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#include "stdafx.h"
#include "List_ReworkInfo.h"
#include "Def_Equipment_Type.h"
#include "CommonFunction.h"

typedef enum 
{
	TSH_No,
	TSH_Barcode,		// 제품 S/N
	TSH_NG_EqpType,		// NG_EqpType;	// Fail 처리된 검사 유형
	TSH_NG_Code,		// NG_Code;		// Fail 사유 ?
	TSH_Time,			// InputTime;	// 제품 투입된 시간

	TSH_MaxCol,
}enTestStepHeader;

// 헤더
static const TCHAR*	g_lpszHeader[] =
{
	_T("No"),			// TSH_No
	_T("Barcode"),		// TSH_Barcode
	_T("NG Eqp Type"),	// TSH_NG_EqpType
	_T("NG Code"),		// TSH_NG_Code
	_T("Time"),			// TSH_Time

	NULL
};

const int	iListAglin[] =
{
	LVCFMT_LEFT,	 // TSH_No
	LVCFMT_LEFT,	 // TSH_Barcode
	LVCFMT_LEFT,	 // TSH_NG_EqpType
	LVCFMT_LEFT,	 // TSH_NG_Code
	LVCFMT_LEFT,	 // TSH_Time
};

// 540 기준
const int	iHeaderWidth[] =
{
	40, 	// TSH_No
	200, 	// TSH_Barcode
	100,	// TSH_NG_EqpType
	100,	// TSH_NG_Code
	200,	// TSH_Time
};

IMPLEMENT_DYNAMIC(CList_ReworkInfo, CListCtrl)

CList_ReworkInfo::CList_ReworkInfo()
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

CList_ReworkInfo::~CList_ReworkInfo()
{
	m_Font.DeleteObject();
}


BEGIN_MESSAGE_MAP(CList_ReworkInfo, CListCtrl)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_NOTIFY_REFLECT(NM_CLICK,	&CList_ReworkInfo::OnNMClick)
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()



// CList_ReworkInfo message handlers
//=============================================================================
// Method		: OnCreate
// Access		: protected  
// Returns		: int
// Parameter	: LPCREATESTRUCT lpCreateStruct
// Qualifier	:
// Last Update	: 2021/11/2 - 14:28
// Desc.		:
//=============================================================================
int CList_ReworkInfo::OnCreate(LPCREATESTRUCT lpCreateStruct)
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
void CList_ReworkInfo::OnSize(UINT nType, int cx, int cy)
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
BOOL CList_ReworkInfo::PreCreateWindow(CREATESTRUCT& cs)
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
// int CList_ReworkInfo::OnCompareItems(LPARAM lParam1, LPARAM lParam2, int iColumn)
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
void CList_ReworkInfo::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
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
void CList_ReworkInfo::InitHeader()
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
// Method		: Reset_Number
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/11/2 - 14:29
// Desc.		:
//=============================================================================
void CList_ReworkInfo::Reset_Number()
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
void CList_ReworkInfo::Select_SelectItem(__in int nItem)
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
// Method		: Set_ItemData
// Access		: protected  
// Returns		: void
// Parameter	: __in int nItem
// Parameter	: __in const ST_ReworkProduct * IN_pRework
// Qualifier	:
// Last Update	: 2021/11/2 - 14:29
// Desc.		:
//=============================================================================
void CList_ReworkInfo::Set_ItemData(__in int nItem, __in LPCTSTR IN_szBarcode, __in const ST_ReworkProduct* IN_pRework)
{
	ASSERT(GetSafeHwnd());
	if (NULL == IN_pRework)
		return;

 	if (GetItemCount() <= nItem)
 		return;

	CString szText;

	// TSH_No
	szText.Format(_T("%d"), nItem + 1);
	SetItemText(nItem, TSH_No, szText);

	// TSH_Barcode,
	SetItemText(nItem, TSH_Barcode, IN_szBarcode);

	// TSH_NG_EqpType
	szText.Format(_T("%s"), g_szEqpTypeName[IN_pRework->nNG_EqpType]);
	SetItemText(nItem, TSH_NG_EqpType, szText);

	// TSH_NG_Code,
	szText.Format(_T("%d"), IN_pRework->nNG_Code);
	SetItemText(nItem, TSH_NG_Code, szText);

	// TSH_Time
	szText = SystemTimeToFormatString(IN_pRework->InputTime);
	SetItemText(nItem, TSH_Time, szText);

}

//=============================================================================
// Method		: Add_Item
// Access		: protected  
// Returns		: void
// Parameter	: const __in ST_ReworkProduct * IN_pRework
// Qualifier	:
// Last Update	: 2021/11/2 - 14:29
// Desc.		:
//=============================================================================
void CList_ReworkInfo::Add_Item(__in LPCTSTR IN_szBarcode, const __in ST_ReworkProduct* IN_pRework)
{
	ASSERT(GetSafeHwnd());
	if (NULL == IN_pRework)
		return;

	int iNewCount = GetItemCount();

	// CListCtrl
	InsertItem(iNewCount, _T(""));

	// CListCtrl
	//AddItem(_T(""));
	//InsertItem(iNewCount, _T(""));

	Set_ItemData(iNewCount, IN_szBarcode, IN_pRework);

	// 화면에 보이게 하기
	EnsureVisible(iNewCount, TRUE);
	ListView_SetItemState(GetSafeHwnd(), iNewCount, LVIS_FOCUSED | LVIS_SELECTED, 0x000F);
}

//=============================================================================
// Method		: Modify_Item
// Access		: protected  
// Returns		: void
// Parameter	: __in int nItem
// Parameter	: __in const ST_ReworkProduct * IN_pRework
// Qualifier	:
// Last Update	: 2021/11/2 - 14:30
// Desc.		:
//=============================================================================
void CList_ReworkInfo::Modify_Item(__in int nItem, __in LPCTSTR IN_szBarcode, __in const ST_ReworkProduct* IN_pRework)
{
	ASSERT(GetSafeHwnd());
	if (NULL == IN_pRework)
		return;

	if (GetItemCount() <= nItem)
		return;

	Set_ItemData(nItem, IN_szBarcode, IN_pRework);

	// 화면에 보이게 하기
	EnsureVisible(nItem, TRUE);
	ListView_SetItemState(GetSafeHwnd(), nItem, LVIS_FOCUSED | LVIS_SELECTED, 0x000F);
}

//=============================================================================
// Method		: Set_ItemList
// Access		: public  
// Returns		: void
// Parameter	: __in const CRework_Info * IN_pReworkList
// Qualifier	:
// Last Update	: 2021/11/2 - 14:30
// Desc.		:
//=============================================================================
void CList_ReworkInfo::Set_ItemList(__in const CRework_Info* IN_pReworkList)
{
	m_stReworkList.m_ReworkInfo.clear();
	DeleteAllItems();

	if (NULL != IN_pReworkList)
	{
		m_stReworkList = *IN_pReworkList;

		auto eIter = IN_pReworkList->m_ReworkInfo.end();
		auto Iter  = IN_pReworkList->m_ReworkInfo.begin();
		while (Iter != eIter)
		{
			Add_Item(Iter->first.c_str(), &Iter->second);

			Iter++;
		}
	}
}

//=============================================================================
// Method		: Item_Insert
// Access		: public  
// Returns		: bool
// Parameter	: __in ST_ReworkProduct & IN_Rework
// Qualifier	:
// Last Update	: 2021/11/2 - 14:30
// Desc.		:
//=============================================================================
bool CList_ReworkInfo::Item_Insert(__in LPCTSTR IN_szBarcode, __in ST_ReworkProduct& IN_Rework)
{
	auto Ret = m_stReworkList.m_ReworkInfo.insert({ IN_szBarcode, IN_Rework });
	if (Ret.second)
	{
		Add_Item(IN_szBarcode, &IN_Rework);

		return true;
	}
	else
	{
		return false;
	}
}

//=============================================================================
// Method		: Item_Remove
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/11/2 - 14:31
// Desc.		:
//=============================================================================
void CList_ReworkInfo::Item_Remove()
{
	if (0 < GetSelectedCount())
	{
		POSITION nPos = GetFirstSelectedItemPosition();
		int iSelIdx = GetNextSelectedItem(nPos);

		CString szBarcode = GetItemText(iSelIdx, TSH_NG_EqpType);

		DeleteItem(iSelIdx);
		Reset_Number();

		m_stReworkList.Delete_ReworkProduct(szBarcode.GetBuffer());

		// 아이템 선택 활성화
		if (iSelIdx < GetItemCount())
		{
			Select_SelectItem(iSelIdx);
		}
		else
		{
			Select_SelectItem(iSelIdx - 1);
		}
	}
	else
	{
		// 항목을 선택 하세요.
	}
}

//=============================================================================
// Method		: Item_Modify
// Access		: public  
// Returns		: void
// Parameter	: __in ST_ReworkProduct & IN_Rework
// Qualifier	:
// Last Update	: 2021/11/2 - 14:31
// Desc.		:
//=============================================================================
void CList_ReworkInfo::Item_Modify(__in LPCTSTR IN_szBarcode, __in ST_ReworkProduct& IN_Rework)
{
	if (0 < GetSelectedCount())
	{
		POSITION nPos = GetFirstSelectedItemPosition();
		int iIndex = GetNextSelectedItem(nPos);

		// 수정
		m_stReworkList.Add_ReworkProduct(IN_szBarcode, &IN_Rework);

		// 삭제, 추가
		Modify_Item(iIndex, IN_szBarcode, &IN_Rework);

		// 아이템 선택 활성화
		if (iIndex < GetItemCount())
		{
			Select_SelectItem(iIndex);
		}
		else
		{
			Select_SelectItem(iIndex - 1);
		}
	}
	else
	{
		// 항목을 선택 하세요.
	}
}

//=============================================================================
// Method		: Get_SelectedRework
// Access		: public  
// Returns		: bool
// Parameter	: __out ST_ReworkProduct & OUT_Rework
// Qualifier	:
// Last Update	: 2021/11/2 - 14:31
// Desc.		:
//=============================================================================
bool CList_ReworkInfo::Get_SelectedItem(__out ST_ReworkProduct& OUT_Rework)
{
	if (0 < GetSelectedCount())
	{
		POSITION nPos = GetFirstSelectedItemPosition();
		int iIndex = GetNextSelectedItem(nPos);

		CString szRFID = GetItemText(iIndex, TSH_NG_EqpType);

		OUT_Rework = m_stReworkList.m_ReworkInfo.at(szRFID.GetBuffer());

		return true;
	}
	else
	{
		return false;
	}
}

//=============================================================================
// Method		: Add_Item
// Access		: public  
// Returns		: void
// Parameter	: __in const CRework_Info * IN_pReworkList
// Qualifier	:
// Last Update	: 2021/11/2 - 14:31
// Desc.		:
//=============================================================================
void CList_ReworkInfo::Add_Item(__in const CRework_Info* IN_pReworkList)
{
	if (NULL != IN_pReworkList)
	{
		m_stReworkList = *IN_pReworkList;

		auto eIter = IN_pReworkList->m_ReworkInfo.end();
		auto Iter = IN_pReworkList->m_ReworkInfo.begin();
		while (Iter != eIter)
		{
			Add_Item(Iter->first.c_str(), &Iter->second);

			Iter++;
		}
	}
}

//=============================================================================
// Method		: Insert_Item
// Access		: public  
// Returns		: void
// Parameter	: __in const CRework_Info * IN_pReworkList
// Qualifier	:
// Last Update	: 2021/11/2 - 14:31
// Desc.		:
//=============================================================================
//void CList_ReworkInfo::Insert_Item(__in const CRework_Info* IN_pReworkList)
//{
//	auto eIter = IN_pReworkList->m_ReworkInfo.end();
//	auto Iter = IN_pReworkList->m_ReworkInfo.begin();
//	while (Iter != eIter)
//	{
//		m_stReworkList.m_ReworkInfo.insert({Iter->first.c_str(), Iter->second });
//
//		Add_Item(Iter->first.c_str(), &Iter->second);
//
//		Iter++;
//	}
//}

//=============================================================================
// Method		: Clear_ItemList
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/11/2 - 14:31
// Desc.		:
//=============================================================================
void CList_ReworkInfo::Clear_ItemList()
{
	m_stReworkList.m_ReworkInfo.clear();
	DeleteAllItems();
}

//=============================================================================
// Method		: Refresh_ItemList
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/11/17 - 15:50
// Desc.		:
//=============================================================================
void CList_ReworkInfo::Refresh_ItemList()
{
	DeleteAllItems();

	auto eIter = m_stReworkList.m_ReworkInfo.end();
	auto Iter = m_stReworkList.m_ReworkInfo.begin();
	while (Iter != eIter)
	{
		Add_Item(Iter->first.c_str(), &Iter->second);

		Iter++;
	}
}
