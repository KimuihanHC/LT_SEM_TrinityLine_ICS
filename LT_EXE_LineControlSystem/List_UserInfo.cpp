//*****************************************************************************
// Filename	: 	List_UserInfo.cpp
// Created	:	2021/11/1 - 15:16
// Modified	:	2021/11/1 - 15:16
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#include "stdafx.h"
#include "List_UserInfo.h"
#include "Def_Constant.h"
#include "Def_Language.h"

typedef enum 
{
	TSH_No,
	TSH_ID,			// ID
	TSH_Name,		// Name
	TSH_Authority,	// Authority
	TSH_Password,	// Password
	TSH_MaxCol,
}enTestStepHeader;

// 헤더
// static const TCHAR*	g_lpszHeader[] =
// {
// 	_T("No"),			// TSH_No
// 	_T("ID"),			// TSH_ID
// 	_T("Name"),			// TSH_Name
// 	_T("Authority"),	// TSH_Authority
// 	_T("Password"),		// TSH_Password
// 	NULL
// };

static const TCHAR* g_lpszHeader_T[Lang_MaxCount][TSH_MaxCol] =
{
	// 한국어
	{
		_T("번호"),			// TSH_No
		_T("ID"),			// TSH_ID
		_T("이름"),			// TSH_Name
		_T("권한"),			// TSH_Authority
		_T("비밀번호"),		// TSH_Password
	},

	// 영어
	{
		_T("No"),			// TSH_No
		_T("ID"),			// TSH_ID
		_T("Name"),			// TSH_Name
		_T("Authority"),	// TSH_Authority
		_T("Password"),		// TSH_Password
	},

	// 베트남어
	{
		_T("No"),			// TSH_No
		_T("ID"),			// TSH_ID
		_T("Name"),			// TSH_Name
		_T("Authority"),	// TSH_Authority
		_T("Password"),		// TSH_Password
	},

	// 중국어
	{
		_T("号码"),			// TSH_No
		_T("ID"),			// TSH_ID
		_T("名字"),			// TSH_Name
		_T("权限"),			// TSH_Authority
		_T("密码"),			// TSH_Password
	},
};

const int	iListAglin[] =
{
	LVCFMT_LEFT,	 // TSH_No
	LVCFMT_LEFT,	 // TSH_ID
	LVCFMT_LEFT,	 // TSH_Name
	LVCFMT_LEFT,	 // TSH_Authority
	LVCFMT_LEFT,	 // TSH_Password
};

// 540 기준
const int	iHeaderWidth[] =
{
	40, 	// TSH_No
	150,	// TSH_ID
	200,	// TSH_Name
	200,	// TSH_Authority
	0,		// TSH_Password
};

IMPLEMENT_DYNAMIC(CList_UserInfo, CListCtrl)

CList_UserInfo::CList_UserInfo()
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

CList_UserInfo::~CList_UserInfo()
{
	m_Font.DeleteObject();
}


BEGIN_MESSAGE_MAP(CList_UserInfo, CListCtrl)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_NOTIFY_REFLECT(NM_CLICK,	&CList_UserInfo::OnNMClick)
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()



// CList_UserInfo message handlers
//=============================================================================
// Method		: OnCreate
// Access		: protected  
// Returns		: int
// Parameter	: LPCREATESTRUCT lpCreateStruct
// Qualifier	:
// Last Update	: 2021/11/2 - 14:28
// Desc.		:
//=============================================================================
int CList_UserInfo::OnCreate(LPCREATESTRUCT lpCreateStruct)
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
void CList_UserInfo::OnSize(UINT nType, int cx, int cy)
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
BOOL CList_UserInfo::PreCreateWindow(CREATESTRUCT& cs)
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
// int CList_UserInfo::OnCompareItems(LPARAM lParam1, LPARAM lParam2, int iColumn)
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
void CList_UserInfo::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
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
void CList_UserInfo::InitHeader()
{
	if (FALSE == m_bIntiHeader)
	{
		m_bIntiHeader = TRUE;

		for (int nCol = 0; nCol < TSH_MaxCol; nCol++)
		{
			InsertColumn(nCol, g_lpszHeader_T[m_nLanguage][nCol], iListAglin[nCol], iHeaderWidth[nCol]);
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
void CList_UserInfo::Reset_Number()
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
// Method		: Set_ItemData
// Access		: protected  
// Returns		: void
// Parameter	: __in int nItem
// Parameter	: __in const ST_User_Unit * IN_pItem
// Qualifier	:
// Last Update	: 2022/1/5 - 11:13
// Desc.		:
//=============================================================================
void CList_UserInfo::Set_ItemData(__in int nItem, __in const ST_User_Unit* IN_pItem)
{
	ASSERT(GetSafeHwnd());
	if (NULL == IN_pItem)
		return;

 	if (GetItemCount() <= nItem)
 		return;

	CString szText;

	// TSH_No
	szText.Format(_T("%d"), nItem + 1);
	SetItemText(nItem, TSH_No, szText);

	// TSH_ID
	SetItemText(nItem, TSH_ID, IN_pItem->szID);

	// 	TSH_Name
	SetItemText(nItem, TSH_Name, IN_pItem->szName);

	// 	TSH_Authority
	uint8_t nValue = __min(IN_pItem->nAuthority, Permission_MaxCount - 1);
	szText.Format(_T("%s"), g_szPermissionMode[nValue]);
	SetItemText(nItem, TSH_Authority, szText);

	// 	TSH_Password
	//SetItemText(nItem, TSH_Password, IN_pItem->szPassword);

}

//=============================================================================
// Method		: Add_Item
// Access		: protected  
// Returns		: void
// Parameter	: __in const ST_User_Unit * IN_pItem
// Qualifier	:
// Last Update	: 2022/1/5 - 11:13
// Desc.		:
//=============================================================================
void CList_UserInfo::Add_Item(__in const ST_User_Unit* IN_pItem)
{
	ASSERT(GetSafeHwnd());
	if (NULL == IN_pItem)
		return;

	int iNewCount = GetItemCount();

	// CListCtrl
	InsertItem(iNewCount, _T(""));

	// CListCtrl
	//AddItem(_T(""));
	//InsertItem(iNewCount, _T(""));

	Set_ItemData(iNewCount, IN_pItem);

	// 화면에 보이게 하기
	EnsureVisible(iNewCount, TRUE);
	ListView_SetItemState(GetSafeHwnd(), iNewCount, LVIS_FOCUSED | LVIS_SELECTED, 0x000F);
}

//=============================================================================
// Method		: Modify_Item
// Access		: protected  
// Returns		: void
// Parameter	: __in int nItem
// Parameter	: __in const ST_User_Unit * IN_pItem
// Qualifier	:
// Last Update	: 2022/1/7 - 13:50
// Desc.		:
//=============================================================================
void CList_UserInfo::Modify_Item(__in int nItem, __in const ST_User_Unit* IN_pItem)
{
	ASSERT(GetSafeHwnd());

	if (NULL == IN_pItem)
		return;

	if (GetItemCount() <= nItem)
		return;

	Set_ItemData(nItem, IN_pItem);

	// 화면에 보이게 하기
	EnsureVisible(nItem, TRUE);
	ListView_SetItemState(GetSafeHwnd(), nItem, LVIS_FOCUSED | LVIS_SELECTED, 0x000F);
}

//=============================================================================
// Method		: OnLanguage
// Access		: public  
// Returns		: void
// Parameter	: __in uint8_t IN_nLanguage
// Qualifier	:
// Last Update	: 2022/7/19 - 19:42
// Desc.		:
//=============================================================================
void CList_UserInfo::OnLanguage(__in uint8_t IN_nLanguage)
{
#ifdef USE_LISTCTRL_LANGUAGE
	if (m_nLanguage != IN_nLanguage)
	{
		m_nLanguage = IN_nLanguage;

		// CharSet 변경
		LOGFONT lf;
		m_Font.GetLogFont(&lf);
		lf.lfCharSet = Get_CharSet(m_nLanguage);
		//lf.lfHeight = (enLanguage::Lang_Chinese == m_nLanguage) ? 24 : 18;
		m_Font.DeleteObject();
		m_Font.CreateFontIndirect(&lf);

		SetFont(&m_Font);

		if (GetSafeHwnd())
		{
			for (int nCol = 0; nCol < TSH_MaxCol; nCol++)
			{
				LVCOLUMN column;
				if (GetColumn(nCol, &column))
				{
					column.mask = LVCF_TEXT;
					column.pszText = (LPTSTR)g_lpszHeader_T[m_nLanguage][nCol];

					SetColumn(nCol, &column);
				}
			}
		}
	}
#endif
}

//=============================================================================
// Method		: Set_ItemList
// Access		: public  
// Returns		: void
// Parameter	: __in const ST_UserInfo * IN_pItemList
// Qualifier	:
// Last Update	: 2022/1/5 - 11:12
// Desc.		:
//=============================================================================
void CList_UserInfo::Set_ItemList(__in const ST_UserInfo* IN_pItemList)
{
	m_stItemList.Reset();
	DeleteAllItems();

	if (NULL != IN_pItemList)
	{
		m_stItemList = *IN_pItemList;

		for (UINT nIdx = 0; nIdx < m_stItemList.m_Users.size(); nIdx++)
		{
			Add_Item(&(m_stItemList.m_Users.at(nIdx)));
		}

	}
}


//=============================================================================
// Method		: Get_ItemList
// Access		: public  
// Returns		: void
// Parameter	: __out ST_UserInfo & OUT_stItemList
// Qualifier	:
// Last Update	: 2022/1/5 - 11:12
// Desc.		:
//=============================================================================
void CList_UserInfo::Get_ItemList(__out ST_UserInfo& OUT_stItemList)
{
	OUT_stItemList.m_Users.clear();
	OUT_stItemList = m_stItemList;
}

//=============================================================================
// Method		: Item_Insert
// Access		: public  
// Returns		: void
// Parameter	: __in ST_User_Unit & IN_Item
// Qualifier	:
// Last Update	: 2022/1/6 - 11:45
// Desc.		:
//=============================================================================
void CList_UserInfo::Item_Insert(__in ST_User_Unit& IN_Item)
{
	m_stItemList.m_Users.push_back(IN_Item);

	Add_Item(&IN_Item);
}

//=============================================================================
// Method		: Item_Remove
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2022/1/6 - 11:43
// Desc.		:
//=============================================================================
void CList_UserInfo::Item_Remove()
{
	if (0 < GetSelectedCount())
	{
		POSITION nPos = GetFirstSelectedItemPosition();
		int iSelIdx = GetNextSelectedItem(nPos);

		DeleteItem(iSelIdx);
		Reset_Number();

		m_stItemList.m_Users.erase(m_stItemList.m_Users.begin( ) + iSelIdx);

		// 아이템 선택 활성화
		if (iSelIdx < GetItemCount())
		{
			Select_Item(iSelIdx);
		}
		else
		{
			Select_Item(iSelIdx - 1);
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
// Parameter	: __in const ST_User_Unit * IN_pItem
// Qualifier	:
// Last Update	: 2022/1/7 - 13:51
// Desc.		:
//=============================================================================
void CList_UserInfo::Item_Modify(__in const ST_User_Unit* IN_pItem)
{
	if (0 < GetSelectedCount())
	{
		POSITION nPos = GetFirstSelectedItemPosition();
		int iIndex = GetNextSelectedItem(nPos);

		// 수정
		if (0 < m_stItemList.m_Users.size())
		{
			m_stItemList.m_Users.at(iIndex) = *IN_pItem;
		}

		// 삭제, 추가
		Modify_Item(iIndex, IN_pItem);
		
		// 아이템 선택 활성화
		if (iIndex < GetItemCount())
		{
			Select_Item(iIndex);
		}
		else
		{
			Select_Item(iIndex - 1);
		}
	}
	else
	{
		// 항목을 선택 하세요.
	}
}

//=============================================================================
// Method		: Select_Item
// Access		: protected  
// Returns		: void
// Parameter	: __in int nItem
// Qualifier	:
// Last Update	: 2021/11/2 - 14:29
// Desc.		:
//=============================================================================
void CList_UserInfo::Select_Item(__in int nItem)
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
// Method		: Get_SelectedItem
// Access		: public  
// Returns		: bool
// Parameter	: __out ST_User_Unit & OUT_Item
// Qualifier	:
// Last Update	: 2022/1/5 - 11:12
// Desc.		:
//=============================================================================
bool CList_UserInfo::Get_SelectedItem(__out ST_User_Unit& OUT_Item)
{
	if (0 < GetSelectedCount())
	{
		POSITION nPos = GetFirstSelectedItemPosition();
		int iIndex = GetNextSelectedItem(nPos);

		OUT_Item = m_stItemList.m_Users.at(iIndex);

		return true;
	}
	else
	{
		return false;
	}
}

//=============================================================================
// Method		: Clear_List
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2022/1/5 - 11:12
// Desc.		:
//=============================================================================
void CList_UserInfo::Clear_List()
{
	m_stItemList.Reset();
	DeleteAllItems();
}

//=============================================================================
// Method		: Refresh_List
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2022/1/5 - 11:12
// Desc.		:
//=============================================================================
void CList_UserInfo::Refresh_List()
{
	DeleteAllItems();

	for (UINT nIdx = 0; nIdx < m_stItemList.m_Users.size(); nIdx++)
	{
		Add_Item(&(m_stItemList.m_Users.at(nIdx)));
	}
}

//=============================================================================
// Method		: Update_Item
// Access		: public  
// Returns		: void
// Parameter	: __in int nRow
// Parameter	: __in const ST_User_Unit * IN_pItem
// Qualifier	:
// Last Update	: 2022/1/5 - 11:12
// Desc.		:
//=============================================================================
void CList_UserInfo::Update_Item(__in int nRow, __in const ST_User_Unit* IN_pItem)
{
	ASSERT(GetSafeHwnd());
	if (NULL == IN_pItem)
		return;

	if (GetItemCount() <= nRow)
		return;

	Set_ItemData(nRow, IN_pItem);

	// 화면에 보이게 하기
	//EnsureVisible(nRow, TRUE);
	//ListView_SetItemState(GetSafeHwnd(), nRow, LVIS_FOCUSED | LVIS_SELECTED, 0x000F);
}

//=============================================================================
// Method		: IsExist_Name
// Access		: public  
// Returns		: bool
// Parameter	: __in LPCTSTR IN_szName
// Qualifier	:
// Last Update	: 2022/1/6 - 15:51
// Desc.		:
//=============================================================================
bool CList_UserInfo::IsExist_Name(__in LPCTSTR IN_szName)
{
	for (UINT nIdx = 0; nIdx < m_stItemList.m_Users.size(); nIdx++)
	{
		if (0 == m_stItemList.m_Users.at(nIdx).szName.Compare(IN_szName))
		{
			return true;
		}
	}

	return false;
}

//=============================================================================
// Method		: IsExist_ID
// Access		: public  
// Returns		: bool
// Parameter	: __in LPCTSTR IN_szID
// Qualifier	:
// Last Update	: 2022/1/6 - 15:52
// Desc.		:
//=============================================================================
bool CList_UserInfo::IsExist_ID(__in LPCTSTR IN_szID)
{
	for (UINT nIdx = 0; nIdx < m_stItemList.m_Users.size(); nIdx++)
	{
		if (0 == m_stItemList.m_Users.at(nIdx).szID.Compare(IN_szID))
		{
			return true;
		}
	}

	return false;
}
