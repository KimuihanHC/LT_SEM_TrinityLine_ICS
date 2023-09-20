//*****************************************************************************
// Filename	: 	List_SocketRFID.cpp
// Created	:	2021/11/1 - 15:16
// Modified	:	2021/11/1 - 15:16
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#include "stdafx.h"
#include "List_SocketRFID.h"
#include "Def_Language.h"

typedef enum 
{
	TSH_No,
	TSH_RFID,			// RFID
	TSH_SocketType,		// Socket Type
	TSH_Comment,		// Comment

	TSH_MaxCol,
}enTestStepHeader;

static const TCHAR* g_lpszHeader_T[Lang_MaxCount][TSH_MaxCol] =
{
	// 한국어
	{
		_T("번호"),				// TSH_No
		_T("RFID"),				// TSH_RFID
		_T("소켓 지그 종류"),		// TSH_SocketType
		_T("설명"),				// TSH_Comment
	},

	// 영어
	{
		_T("No"),				// TSH_No
		_T("RFID"),				// TSH_RFID
		_T("Socket JIG Type"),	// TSH_SocketType
		_T("Comment"),			// TSH_Comment
	},

	// 베트남어
	{
		_T("No"),				// TSH_No
		_T("RFID"),				// TSH_RFID
		_T("Socket JIG Type"),	// TSH_SocketType
		_T("Comment"),			// TSH_Comment
	},

	// 중국어
	{
		_T("号码"),				// TSH_No
		_T("RFID"),				// TSH_RFID
		_T("插口夹具类型"),		// TSH_SocketType
		_T("评语"),				// TSH_Comment
	},
};

const int	iListAglin[] =
{
	LVCFMT_LEFT,	 // TSH_No
	LVCFMT_LEFT,	 // TSH_RFID
	LVCFMT_LEFT,	 // TSH_SocketType
	LVCFMT_LEFT,	 // TSH_Comment
};

// 540 기준
const int	iHeaderWidth[] =
{
	40, 	// TSH_No
	200,	// TSH_RFID
	150,	// TSH_SocketType
	300,	// TSH_Comment
};

IMPLEMENT_DYNAMIC(CList_SocketRFID, CListCtrl)

CList_SocketRFID::CList_SocketRFID()
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

CList_SocketRFID::~CList_SocketRFID()
{
	m_Font.DeleteObject();
}


BEGIN_MESSAGE_MAP(CList_SocketRFID, CListCtrl)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_NOTIFY_REFLECT(NM_CLICK,	&CList_SocketRFID::OnNMClick)
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()



// CList_SocketRFID message handlers
//=============================================================================
// Method		: OnCreate
// Access		: protected  
// Returns		: int
// Parameter	: LPCREATESTRUCT lpCreateStruct
// Qualifier	:
// Last Update	: 2021/11/2 - 14:28
// Desc.		:
//=============================================================================
int CList_SocketRFID::OnCreate(LPCREATESTRUCT lpCreateStruct)
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
void CList_SocketRFID::OnSize(UINT nType, int cx, int cy)
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
BOOL CList_SocketRFID::PreCreateWindow(CREATESTRUCT& cs)
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
// int CList_SocketRFID::OnCompareItems(LPARAM lParam1, LPARAM lParam2, int iColumn)
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
void CList_SocketRFID::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
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
void CList_SocketRFID::InitHeader()
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
void CList_SocketRFID::Reset_Number()
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
void CList_SocketRFID::Select_SelectItem(__in int nItem)
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
// Parameter	: __in const CConfig_Socket * IN_pSocket
// Qualifier	:
// Last Update	: 2021/11/2 - 14:29
// Desc.		:
//=============================================================================
void CList_SocketRFID::Set_SocketData(__in int nItem, __in const CConfig_Socket* IN_pSocket)
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
	
	
}

//=============================================================================
// Method		: Add_Socket
// Access		: protected  
// Returns		: void
// Parameter	: const __in CConfig_Socket * IN_pSocket
// Qualifier	:
// Last Update	: 2021/11/2 - 14:29
// Desc.		:
//=============================================================================
void CList_SocketRFID::Add_Socket(const __in CConfig_Socket* IN_pSocket)
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
// Method		: Modify_Socket
// Access		: protected  
// Returns		: void
// Parameter	: __in int nItem
// Parameter	: __in const CConfig_Socket * IN_pSocket
// Qualifier	:
// Last Update	: 2021/11/2 - 14:30
// Desc.		:
//=============================================================================
void CList_SocketRFID::Modify_Socket(__in int nItem, __in const CConfig_Socket* IN_pSocket)
{
	ASSERT(GetSafeHwnd());
	if (NULL == IN_pSocket)
		return;

	if (GetItemCount() <= nItem)
		return;

	Set_SocketData(nItem, IN_pSocket);

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
// Last Update	: 2022/7/19 - 19:11
// Desc.		:
//=============================================================================
void CList_SocketRFID::OnLanguage(__in uint8_t IN_nLanguage)
{
#ifdef USE_LISTCTRL_LANGUAGE
	if (m_nLanguage != IN_nLanguage)
	{
		m_nLanguage = IN_nLanguage;

		// CharSet 변경
		LOGFONT lf;
		m_Font.GetLogFont(&lf);
		lf.lfCharSet = Get_CharSet(m_nLanguage);
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
// Method		: Set_SocketList
// Access		: public  
// Returns		: void
// Parameter	: __in const CSocketList * IN_pSoscketList
// Qualifier	:
// Last Update	: 2021/11/2 - 14:30
// Desc.		:
//=============================================================================
void CList_SocketRFID::Set_SocketList(__in const CSocketList* IN_pSoscketList)
{
	m_stSocketList.RemoveAll();
	DeleteAllItems();

	if (NULL != IN_pSoscketList)
	{
		m_stSocketList = *IN_pSoscketList;

		auto eIter = IN_pSoscketList->SocketList.end();
		auto Iter  = IN_pSoscketList->SocketList.begin();
		while (Iter != eIter)
		{
			Add_Socket(&Iter->second);

			Iter++;
		}
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
void CList_SocketRFID::Get_SocketList(__out CSocketList& OUT_stSocketList)
{
	OUT_stSocketList.RemoveAll();
	OUT_stSocketList = m_stSocketList;
}

//=============================================================================
// Method		: Item_Insert
// Access		: public  
// Returns		: bool
// Parameter	: __in CConfig_Socket & IN_Socket
// Qualifier	:
// Last Update	: 2021/11/2 - 14:30
// Desc.		:
//=============================================================================
bool CList_SocketRFID::Item_Insert(__in CConfig_Socket& IN_Socket)
{
	if (m_stSocketList.Socket_Insert(IN_Socket))
	{
		Add_Socket(&IN_Socket);

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
void CList_SocketRFID::Item_Remove()
{
	if (0 < GetSelectedCount())
	{
		POSITION nPos = GetFirstSelectedItemPosition();
		int iSelIdx = GetNextSelectedItem(nPos);

		CString szRFID = GetItemText(iSelIdx, TSH_RFID);

		DeleteItem(iSelIdx);
		Reset_Number();

		m_stSocketList.Socket_Remove(szRFID.GetBuffer());

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
// Parameter	: __in CConfig_Socket & IN_Socket
// Qualifier	:
// Last Update	: 2021/11/2 - 14:31
// Desc.		:
//=============================================================================
void CList_SocketRFID::Item_Modify(__in CConfig_Socket& IN_Socket)
{
	if (0 < GetSelectedCount())
	{
		POSITION nPos = GetFirstSelectedItemPosition();
		int iIndex = GetNextSelectedItem(nPos);

		// 수정
		m_stSocketList.Socket_Modify(IN_Socket);

		// 삭제, 추가
		Modify_Socket(iIndex, &IN_Socket);

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
// Method		: Get_SelectedSocket
// Access		: public  
// Returns		: bool
// Parameter	: __out CConfig_Socket & OUT_Socket
// Qualifier	:
// Last Update	: 2021/11/2 - 14:31
// Desc.		:
//=============================================================================
bool CList_SocketRFID::Get_SelectedSocket(__out CConfig_Socket& OUT_Socket)
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
// Method		: Add_Socket
// Access		: public  
// Returns		: void
// Parameter	: __in const CSocketList * IN_pSoscketList
// Qualifier	:
// Last Update	: 2021/11/2 - 14:31
// Desc.		:
//=============================================================================
void CList_SocketRFID::Add_Socket(__in const CSocketList* IN_pSoscketList)
{
	if (NULL != IN_pSoscketList)
	{
		m_stSocketList = *IN_pSoscketList;

		auto eIter = IN_pSoscketList->SocketList.end();
		auto Iter = IN_pSoscketList->SocketList.begin();
		while (Iter != eIter)
		{
			Add_Socket(&Iter->second);

			Iter++;
		}
	}
}

//=============================================================================
// Method		: Insert_Socket
// Access		: public  
// Returns		: void
// Parameter	: __in const CSocketList * IN_pSoscketList
// Qualifier	:
// Last Update	: 2021/11/2 - 14:31
// Desc.		:
//=============================================================================
void CList_SocketRFID::Insert_Socket(__in const CSocketList* IN_pSoscketList)
{
	auto eIter = IN_pSoscketList->SocketList.end();
	auto Iter = IN_pSoscketList->SocketList.begin();
	while (Iter != eIter)
	{
		m_stSocketList.Socket_Insert(Iter->second);

		Add_Socket(&Iter->second);

		Iter++;
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
void CList_SocketRFID::Clear_SocketList()
{
	m_stSocketList.RemoveAll();
	DeleteAllItems();
}

//=============================================================================
// Method		: Get_SocketCount
// Access		: public  
// Returns		: UINT
// Parameter	: __in UINT nSocketType
// Qualifier	:
// Last Update	: 2021/11/9 - 14:41
// Desc.		:
//=============================================================================
UINT CList_SocketRFID::Get_SocketCount(__in UINT nSocketType)
{
	return m_stSocketList.Get_SocketCount(nSocketType);
}

//=============================================================================
// Method		: Refresh_Socket
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/11/17 - 15:50
// Desc.		:
//=============================================================================
void CList_SocketRFID::Refresh_Socket()
{
	DeleteAllItems();

	auto eIter = m_stSocketList.SocketList.end();
	auto Iter = m_stSocketList.SocketList.begin();
	while (Iter != eIter)
	{
		Add_Socket(&Iter->second);

		Iter++;
	}
}
