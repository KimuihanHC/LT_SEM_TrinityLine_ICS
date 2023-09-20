//*****************************************************************************
// Filename	: 	List_NGCode.cpp
// Created	:	2021/11/1 - 15:16
// Modified	:	2021/11/1 - 15:16
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#include "stdafx.h"
#include "List_NGCode.h"
#include "Def_Constant.h"
#include "Def_Language.h"


typedef enum 
{
	TSH_No,
	TSH_NG_Code,			// NG Code
	TSH_NG_Section,			// NG 구분 
	TSH_Retry_Mode,			// Retry Mode
	TSH_RetryCnt,			// Retry Cnt
	TSH_Move_Retry,			// 이동 Retry
	TSH_Reset,				// Reset
	TSH_AlarmCnt,			// 연속 NG 알람 수
	TSH_Description,		// Description

	TSH_MaxCol,
}enTestStepHeader;

// 헤더
static const TCHAR* g_lpszHeader_T[Lang_MaxCount][TSH_MaxCol] =
{
	// 한국어
	{
		_T("번호"),				// TSH_No
		_T("코드"),				// TSH_NG_Code
		_T("섹션"),				// TSH_NG_Section
		_T("재시도 방법"),		// TSH_Retry_Mode
		_T("재검사 횟수"),		// TSH_RetryCnt
		_T("이동 후 재시도"),		// TSH_Move_Retry
		_T("초기화 종류"),		// TSH_Reset
		_T("연속 NG 알람 횟수"),	// TSH_AlarmCnt
		_T("설명"),				// TSH_Description
	},

	// 영어
	{
		_T("No"),				// TSH_No
		_T("NG Code"),			// TSH_NG_Code
		_T("Section"),			// TSH_NG_Section
		_T("Retry Mode"),		// TSH_Retry_Mode
		_T("Retry Cnt"),		// TSH_RetryCnt
		_T("Move Retry"),		// TSH_Move_Retry
		_T("Reset"),			// TSH_Reset
		_T("Alarm Cnt"),		// TSH_AlarmCnt
		_T("Description"),		// TSH_Description
	},

	// 베트남어
	{
		_T("No"),				// TSH_No
		_T("NG Code"),			// TSH_NG_Code
		_T("Section"),			// TSH_NG_Section
		_T("Retry Mode"),		// TSH_Retry_Mode
		_T("Retry Cnt"),		// TSH_RetryCnt
		_T("Move Retry"),		// TSH_Move_Retry
		_T("Reset"),			// TSH_Reset
		_T("Alarm Cnt"),		// TSH_AlarmCnt
		_T("Description"),		// TSH_Description
	},

	// 중국어
	{
		_T("号码"),				// TSH_No
		_T("NG 代码"),			// TSH_NG_Code
		_T("科室"),				// TSH_NG_Section
		_T("重试方法"),			// TSH_Retry_Mode
		_T("重试数量"),			// TSH_RetryCnt
		_T("移动后重试方法"),		// TSH_Move_Retry
		_T("初始化类型"),		// TSH_Reset
		_T("连续报警数量"),		// TSH_AlarmCnt
		_T("说明"),				// TSH_Description
	},
};

const int	iListAglin[] =
{
	LVCFMT_LEFT,	 // TSH_No
	LVCFMT_CENTER,	 // TSH_NG_Code
	LVCFMT_CENTER,	 // TSH_NG_Section
	LVCFMT_CENTER,	 // TSH_Retry_Mode
	LVCFMT_CENTER,	 // TSH_RetryCnt
	LVCFMT_CENTER,	 // TSH_Move_Retry
	LVCFMT_CENTER,	 // TSH_Reset
	LVCFMT_CENTER,	 // TSH_AlarmCnt
	LVCFMT_LEFT,	 // TSH_Description
};

// 540 기준
const int	iHeaderWidth[] =
{
	 40, 	// TSH_No
	 80,	// TSH_NG_Code
#ifdef NG_CODE_MINI_VERSION
	  0,	// TSH_NG_Section
	 80,	// TSH_Retry_Mode
	 80,	// TSH_RetryCnt
	  0,	// TSH_Move_Retry
	  0,	// TSH_Reset
	 80,	// TSH_AlarmCnt
	300,	// TSH_Description
#else
	80,		// TSH_NG_Section
	80,		// TSH_Retry_Mode
	80,		// TSH_RetryCnt
	80,		// TSH_Move_Retry
	80,		// TSH_Reset
	80,		// TSH_AlarmCnt
	200,	// TSH_Description
#endif
};

IMPLEMENT_DYNAMIC(CList_NGCode, CListCtrl)

CList_NGCode::CList_NGCode()
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

CList_NGCode::~CList_NGCode()
{
	m_Font.DeleteObject();
}


BEGIN_MESSAGE_MAP(CList_NGCode, CListCtrl)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_NOTIFY_REFLECT(NM_CLICK,	&CList_NGCode::OnNMClick)
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()



// CList_NGCode message handlers
//=============================================================================
// Method		: OnCreate
// Access		: protected  
// Returns		: int
// Parameter	: LPCREATESTRUCT lpCreateStruct
// Qualifier	:
// Last Update	: 2021/11/2 - 14:28
// Desc.		:
//=============================================================================
int CList_NGCode::OnCreate(LPCREATESTRUCT lpCreateStruct)
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
void CList_NGCode::OnSize(UINT nType, int cx, int cy)
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
BOOL CList_NGCode::PreCreateWindow(CREATESTRUCT& cs)
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
// Last Update	: 2021/11/2 - 14:29
// Desc.		:
//=============================================================================
void CList_NGCode::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
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
void CList_NGCode::InitHeader()
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
// Method		: Find_Row
// Access		: protected  
// Returns		: int
// Parameter	: __in int16_t IN_nNGCode
// Qualifier	:
// Last Update	: 2021/12/22 - 11:45
// Desc.		:
//=============================================================================
int CList_NGCode::Find_Row(__in int16_t IN_nNGCode)
{
	CString szText;
	int nRetRow = -1;
	for (int nRow = 0; nRow < GetItemCount(); nRow++)
	{
		szText = GetItemText(nRow, TSH_NG_Code);

		if (IN_nNGCode == _ttoi(szText.GetBuffer()))
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
void CList_NGCode::Reset_Number()
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
void CList_NGCode::Select_SelectItem(__in int nItem)
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
// Method		: Set_NG_Info
// Access		: protected  
// Returns		: void
// Parameter	: __in int nItem
// Parameter	: __in const ST_NGCode * IN_pNG_Code
// Qualifier	:
// Last Update	: 2021/11/2 - 14:29
// Desc.		:
//=============================================================================
void CList_NGCode::Set_NG_Info(__in int nItem, __in const ST_NGCode* IN_pNG_Code)
{
	ASSERT(GetSafeHwnd());
	if (NULL == IN_pNG_Code)
		return;

 	if (GetItemCount() <= nItem)
 		return;

	CString szText;

	// TSH_No
	szText.Format(_T("%d"), nItem + 1);
	SetItemText(nItem, TSH_No, szText);

	// NG Code
	szText.Format(_T("%d"), IN_pNG_Code->m_nNG_Code);
	SetItemText(nItem, TSH_NG_Code, szText);

	// NG 구분 
	szText.Format(_T("%d"), IN_pNG_Code->m_nNG_Section);
	SetItemText(nItem, TSH_NG_Section, szText);

	// Retry Mode
	szText.Format(_T("%d"), IN_pNG_Code->m_nRetry_Mode);
	SetItemText(nItem, TSH_Retry_Mode, szText);
	
	// Retry Cnt
	szText.Format(_T("%d"), IN_pNG_Code->m_nRetryCnt);
	SetItemText(nItem, TSH_RetryCnt, szText);

	// 이동 Retry
	szText.Format(_T("%d"), IN_pNG_Code->m_nMove_Retry);
	SetItemText(nItem, TSH_Move_Retry, szText);

	// Reset
	szText.Format(_T("%d"), IN_pNG_Code->m_nReset);
	SetItemText(nItem, TSH_Reset, szText);

	// 연속 NG 알람 수
	szText.Format(_T("%d"), IN_pNG_Code->m_nAlarmCnt);
	SetItemText(nItem, TSH_AlarmCnt, szText);

	// Description
	szText = IN_pNG_Code->m_szDescription;
	SetItemText(nItem, TSH_Description, szText);

}

//=============================================================================
// Method		: Add_Item
// Access		: protected  
// Returns		: void
// Parameter	: const __in ST_NGCode * IN_pNG_Code
// Qualifier	:
// Last Update	: 2021/11/2 - 14:29
// Desc.		:
//=============================================================================
void CList_NGCode::Add_Item(const __in ST_NGCode* IN_pNG_Code)
{
	ASSERT(GetSafeHwnd());
	if (NULL == IN_pNG_Code)
		return;

	int iNewCount = GetItemCount();

	// CListCtrl
	InsertItem(iNewCount, _T(""));

	// CListCtrl
	//AddItem(_T(""));
	//InsertItem(iNewCount, _T(""));

	Set_NG_Info(iNewCount, IN_pNG_Code);

	// 화면에 보이게 하기
	EnsureVisible(iNewCount, TRUE);
	ListView_SetItemState(GetSafeHwnd(), iNewCount, LVIS_FOCUSED | LVIS_SELECTED, 0x000F);
}

void CList_NGCode::Modify_Item(int nItem, const ST_NGCode * IN_pNG_Code)
{
	ASSERT(GetSafeHwnd());
	if (NULL == IN_pNG_Code)
		return;

	if (GetItemCount() <= nItem)
		return;

	Set_NG_Info(nItem, IN_pNG_Code);

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
// Last Update	: 2022/7/20 - 9:46
// Desc.		:
//=============================================================================
void CList_NGCode::OnLanguage(__in uint8_t IN_nLanguage)
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
// Method		: Set_ItemList
// Access		: public  
// Returns		: void
// Parameter	: __in const CNGCode_Info * IN_pNGCodeList
// Qualifier	:
// Last Update	: 2021/11/2 - 14:30
// Desc.		:
//=============================================================================
void CList_NGCode::Set_ItemList(__in const CNGCode_Info* IN_pNGCodeList)
{
	m_NGCodeInfo.m_NG_Info.clear();
	DeleteAllItems();

	if (NULL != IN_pNGCodeList)
	{
		m_NGCodeInfo = *IN_pNGCodeList;

 		auto eIter = IN_pNGCodeList->m_NG_Info.end();
 		auto Iter  = IN_pNGCodeList->m_NG_Info.begin();
 		while (Iter != eIter)
 		{
 			Add_Item(&Iter->second);
 
 			Iter++;
 		}
	}
}

//=============================================================================
// Method		: Get_ItemList
// Access		: public  
// Returns		: void
// Parameter	: __out CNGCode_Info & OUT_NGCodeList
// Qualifier	:
// Last Update	: 2021/11/2 - 14:30
// Desc.		:
//=============================================================================
void CList_NGCode::Get_ItemList(__out CNGCode_Info& OUT_NGCodeList)
{
	OUT_NGCodeList.m_NG_Info.clear();
	OUT_NGCodeList = m_NGCodeInfo;
}

bool CList_NGCode::Item_Insert(ST_NGCode & IN_NGCode)
{
	if (m_NGCodeInfo.Insert(&IN_NGCode))
	{
		Add_Item(&IN_NGCode);

		return true;
	}
	else
	{
		return false;
	}
}

void CList_NGCode::Item_Remove()
{
	if (0 < GetSelectedCount())
	{
		POSITION nPos = GetFirstSelectedItemPosition();
		int iSelIdx = GetNextSelectedItem(nPos);

		CString szNGCode = GetItemText(iSelIdx, TSH_NG_Code);
		int16_t nNGCode = _ttoi(szNGCode.GetBuffer());

		DeleteItem(iSelIdx);
		Reset_Number();

		m_NGCodeInfo.Remove(nNGCode);

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

void CList_NGCode::Item_Modify(ST_NGCode & IN_NGCode)
{
	if (0 < GetSelectedCount())
	{
		POSITION nPos = GetFirstSelectedItemPosition();
		int iIndex = GetNextSelectedItem(nPos);

		// 수정
		m_NGCodeInfo.Modify(&IN_NGCode);

		// 삭제, 추가
		Modify_Item(iIndex, &IN_NGCode);

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
// Method		: Get_SelectedItem
// Access		: public  
// Returns		: bool
// Parameter	: __out ST_NGCode & OUT_NGCode
// Qualifier	:
// Last Update	: 2021/11/2 - 14:31
// Desc.		:
//=============================================================================
bool CList_NGCode::Get_SelectedItem(__out ST_NGCode& OUT_NGCode)
{
	if (0 < GetSelectedCount())
	{
		POSITION nPos = GetFirstSelectedItemPosition();
		int iIndex = GetNextSelectedItem(nPos);

		CString szNGCode = GetItemText(iIndex, TSH_NG_Code);
		uint16_t nNGCode = _ttoi(szNGCode.GetBuffer());

		OUT_NGCode = m_NGCodeInfo.m_NG_Info.at(nNGCode);

		return true;
	}
	else
	{
		return false;
	}
}

//=============================================================================
// Method		: Clear_ItemList
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/11/2 - 14:31
// Desc.		:
//=============================================================================
void CList_NGCode::Clear_ItemList()
{
	m_NGCodeInfo.Clear();
	DeleteAllItems();
}

//=============================================================================
// Method		: Refresh_Item
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/11/17 - 15:50
// Desc.		:
//=============================================================================
void CList_NGCode::Refresh_ItemList()
{
	DeleteAllItems();

 	auto eIter = m_NGCodeInfo.m_NG_Info.end();
 	auto Iter = m_NGCodeInfo.m_NG_Info.begin();
 	while (Iter != eIter)
 	{
 		Add_Item(&Iter->second);
 
 		Iter++;
 	}
}

//=============================================================================
// Method		: Update_Item
// Access		: protected  
// Returns		: void
// Parameter	: __in int nRow
// Parameter	: __in const ST_NGCode * IN_pNG_Code
// Qualifier	:
// Last Update	: 2021/11/2 - 14:30
// Desc.		:
//=============================================================================
void CList_NGCode::Update_Item(__in int nRow, __in const ST_NGCode* IN_pNG_Code)
{
	ASSERT(GetSafeHwnd());
	if (NULL == IN_pNG_Code)
		return;

	if (GetItemCount() <= nRow)
		return;

	Set_NG_Info(nRow, IN_pNG_Code);

	// 화면에 보이게 하기
	//EnsureVisible(nRow, TRUE);
	//ListView_SetItemState(GetSafeHwnd(), nRow, LVIS_FOCUSED | LVIS_SELECTED, 0x000F);
}

void CList_NGCode::Update_Item(__in int16_t IN_nNGCode, __in const ST_NGCode* IN_pNG_Code)
{
	int nRow = Find_Row(IN_nNGCode);

	if (0 <= nRow)
	{
		Update_Item(nRow, IN_pNG_Code);
	}
}

//=============================================================================
// Method		: Find_Item
// Access		: public  
// Returns		: int
// Parameter	: __in int16_t IN_nNGCode
// Qualifier	:
// Last Update	: 2021/12/22 - 17:31
// Desc.		:
//=============================================================================
int CList_NGCode::Find_Item(__in int16_t IN_nNGCode)
{
	int nRow = Find_Row(IN_nNGCode);

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
