//*****************************************************************************
// Filename	: 	List_LineConfig.cpp
// Created	:	2021/11/1 - 15:16
// Modified	:	2021/11/1 - 15:16
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#include "stdafx.h"
#include "List_ALID_ListConfig.h"
#include "CommonFunction.h"
#include "Def_ID_Type.h"
#include "Def_Language_Message.h"


typedef enum
{
	TSH_No,
	TSH_ALID,
	TSH_ALCD,
	TSH_ALTX,
	TSH_AL_MaxCol,
}enALConfigHeader;

static const TCHAR* g_lpszHeader_T[Lang_MaxCount][TSH_AL_MaxCol] =
{
	// 한국어
	{
		_T("번호"),	// TSH_No
		_T("ID"),	// TSH_SID
		_T("CD"),	// TSH_SID_TYPE
		_T("TX"),	// TSH_SID_TX
	},

	// 영어
	{
		_T("No"),		// TSH_No
		_T("ID"),	// TSH_SID
		_T("CD"),	// TSH_SID_TYPE
		_T("TX"),	// TSH_SID_TX
	},

	// 베트남어
	{
		_T("No"),		// TSH_No
		_T("ID"),	// TSH_SID
		_T("CD"),	// TSH_SID_TYPE
		_T("TX"),	// TSH_SID_TX


	},

	// 중국어
	{
		_T("No"),			// TSH_No
		_T("ID"),	// TSH_SID
		_T("CD"),	// TSH_SID_TYPE
		_T("TX"),	// TSH_SID_TX
	},
};

const int	iListAglin[] =
{
	LVCFMT_LEFT,	 // TSH_No
	LVCFMT_LEFT,	 // TSH_ALID
	LVCFMT_LEFT,	 // TSH_ALCD
	LVCFMT_LEFT,	 // TSH_ALTX
};

// 540 기준
const int	iHeaderWidth[] =
{
	40, 	// TSH_No
	40,		// TSH_ALID
	40,		// TSH_ALCD
	500,	// TSH_ALTX
};

IMPLEMENT_DYNAMIC(CList_AL_ListConfig, CListCtrl)

CList_AL_ListConfig::CList_AL_ListConfig()
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

CList_AL_ListConfig::~CList_AL_ListConfig()
{
	m_Font.DeleteObject();
}


BEGIN_MESSAGE_MAP(CList_AL_ListConfig, CListCtrl)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_NOTIFY_REFLECT(NM_CLICK,	&CList_AL_ListConfig::OnNMClick)
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()



// CList_LineConfig message handlers
//=============================================================================
// Method		: OnCreate
// Access		: protected  
// Returns		: int
// Parameter	: LPCREATESTRUCT lpCreateStruct
// Qualifier	:
// Last Update	: 2023/02/21 - 15:26
// Desc.		:
//=============================================================================
int CList_AL_ListConfig::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetFont(&m_Font);

	SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER);

	InitSVHeader();

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
// Last Update	: 2023/02/21 - 15:26
// Desc.		:
//=============================================================================
void CList_AL_ListConfig::OnSize(UINT nType, int cx, int cy)
{
	CListCtrl::OnSize(nType, cx, cy);
}

//=============================================================================
// Method		: PreCreateWindow
// Access		: virtual protected  
// Returns		: BOOL
// Parameter	: CREATESTRUCT & cs
// Qualifier	:
// Last Update	: 2023/02/21 - 15:26
// Desc.		:
//=============================================================================
BOOL CList_AL_ListConfig::PreCreateWindow(CREATESTRUCT& cs)
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
// Last Update	: 2023/02/21 - 15:26
// Desc.		:
//=============================================================================
void CList_AL_ListConfig::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
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
// Last Update	: 2023/02/21 - 15:26
// Desc.		:
//=============================================================================
void CList_AL_ListConfig::InitSVHeader()
{
	//int iMaxCol = m_bUseExtraData ? TSH_MaxCol : TSH_ZoneCnt;

	if (FALSE == m_bIntiHeader)
	{
		m_bIntiHeader = TRUE;

		for (int nCol = 0; nCol < TSH_AL_MaxCol; nCol++)
		{
			InsertColumn(nCol, g_lpszHeader_T[m_nLanguage][nCol], iListAglin[nCol], iHeaderWidth[nCol]);
		}
	}

	for (int nCol = 0; nCol < TSH_AL_MaxCol; nCol++)
	{
		SetColumnWidth(nCol, m_pHeadWidth[nCol]);
	}
}

//=============================================================================
// Method		: Reset_Number
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2023/02/21 - 15:26
// Desc.		:
//=============================================================================
void CList_AL_ListConfig::Reset_Number()
{
	CString szText;

	for (int nIdx = 0; nIdx < GetItemCount(); nIdx++)
	{
		// TSH_No
		szText.Format(_T("%d"), nIdx + 1);
		SetItemText(nIdx, TSH_No, szText);
	}
}

//=============================================================================
// Method		: Select_SelectItem
// Access		: protected  
// Returns		: void
// Parameter	: __in int nItem
// Qualifier	:
// Last Update	: 2023/02/21 - 15:26
// Desc.		:
//=============================================================================
void CList_AL_ListConfig::Select_SelectItem(__in int nItem)
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
// Method		: Set_SV_Data
// Access		: protected  
// Returns		: void
// Parameter	: __in int nItem
// Parameter	: __in const CConfig_SV * IN_pSV
// Qualifier	:
// Last Update	: 2023/02/21 - 15:26
// Desc.		:
//=============================================================================
void CList_AL_ListConfig::Set_AL_Data(__in int nItem, __in const CConfig_AL* IN_pSV)
{
	ASSERT(GetSafeHwnd());
	if (NULL == IN_pSV)
		return;

	if (GetItemCount() <= nItem)
		return;

	CString szText;

	// TSH_No,
	szText.Format(_T("%d"), nItem + 1);
	SetItemText(nItem, TSH_No, szText);
	// TSH_SID
	szText.Format(_T("%d"), IN_pSV->Get_ALID());
	SetItemText(nItem, TSH_ALID, (szText));
	// TSH_ALCD
	//szText.Format(_T("%d"), IN_pSV->Get_ALCD());
	SetItemText(nItem, TSH_ALCD, (g_szALCD[IN_pSV->Get_ALCD()]));
	// TSH_ALTX
	SetItemText(nItem, TSH_ALTX, IN_pSV->Get_ALTX());
}

//=============================================================================
// Method		: Insert_SV
// Access		: protected  
// Returns		: void
// Parameter	: __in int nItem
// Parameter	: __in const CConfig_SV* IN_pSV
// Qualifier	:
// Last Update	: 2023/02/21 - 15:30
// Desc.		:
//=============================================================================
void CList_AL_ListConfig::Insert_AL(__in int nItem, __in const CConfig_AL* IN_pSV)
{
	ASSERT(GetSafeHwnd());
	if (NULL == IN_pSV)
		return;

	if (GetItemCount() <= nItem)
		return;

	int iNewCount = nItem;

	InsertItem(iNewCount, _T(""));

	Set_AL_Data(iNewCount, IN_pSV);

	// 번호 재부여
	Reset_Number();

	// 화면에 보이게 하기
	EnsureVisible(iNewCount, TRUE);
	ListView_SetItemState(GetSafeHwnd(), iNewCount, LVIS_FOCUSED | LVIS_SELECTED, 0x000F);
}

//=============================================================================
// Method		: Add_SV
// Access		: protected  
// Returns		: void
// Parameter	: const __in CConfig_SV* IN_pSV
// Qualifier	:
// Last Update	: 2023/02/21 - 15:50
// Desc.		:
//=============================================================================
void CList_AL_ListConfig::Add_AL(const __in CConfig_AL* IN_pSV)
{
	ASSERT(GetSafeHwnd());
	if (NULL == IN_pSV)
		return;

	int iNewCount = GetItemCount();

	InsertItem(iNewCount, _T(""));

	Set_AL_Data(iNewCount, IN_pSV);

	// 화면에 보이게 하기
	EnsureVisible(iNewCount, TRUE);
	ListView_SetItemState(GetSafeHwnd(), iNewCount, LVIS_FOCUSED | LVIS_SELECTED, 0x000F);
}

//=============================================================================
// Method		: Modify_SV
// Access		: protected  
// Returns		: void
// Parameter	: __in int nItem
// Parameter	: __in const CConfig_SV* IN_pSV
// Qualifier	:
// Last Update	: 2023/02/21 - 15:51
// Desc.		:
//=============================================================================
void CList_AL_ListConfig::Modify_AL(__in int nItem, __in const CConfig_AL* IN_pSV)
{
	ASSERT(GetSafeHwnd());
	if (NULL == IN_pSV)
		return;

	if (GetItemCount() <= nItem)
		return;

	Set_AL_Data(nItem, IN_pSV);

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
// Last Update	: 2023/02/21 - 15:51
// Desc.		:
//=============================================================================
void CList_AL_ListConfig::OnLanguage(__in uint8_t IN_nLanguage)
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
// Method		: Set_SVInfo
// Access		: public  
// Returns		: void
// Parameter	: __in const CConfig_SV_LIST* IN_pSVInfo
// Qualifier	:
// Last Update	: 2023/02/21 - 15:51
// Desc.		:
//=============================================================================
void CList_AL_ListConfig::Set_ALInfo(__in const CConfig_AL_LIST* IN_pSVInfo)
{
	m_stAL_Info.RemoveAll();
	DeleteAllItems();

	if (NULL != IN_pSVInfo)
	{
		m_stAL_Info = *IN_pSVInfo;

		for (UINT nIdx = 0; nIdx < IN_pSVInfo->Get_AL_Count(); nIdx++)
		{
			Add_AL(&(IN_pSVInfo->GetAt(nIdx)));
		}
	}
}

//=============================================================================
// Method		: Get_SVInfo
// Access		: public  
// Returns		: void
// Parameter	: __out CConfig_SV_LIST& OUT_SVInfo
// Qualifier	:
// Last Update	: 2023/02/21 - 15:53
// Desc.		:
//=============================================================================
void CList_AL_ListConfig::Get_ALInfo(__out CConfig_AL_LIST& OUT_SVInfo)
{
	OUT_SVInfo.RemoveAll();
	OUT_SVInfo = m_stAL_Info;
}

//=============================================================================
// Method		: Item_SV
// Access		: public  
// Returns		: void
// Parameter	: __in CConfig_SV& IN_SV
// Qualifier	:
// Last Update	: 2023/02/21 - 16:09
// Desc.		:
//=============================================================================
void CList_AL_ListConfig::Item_Add(__in CConfig_AL& IN_SV)
{
	POSITION posSel = GetFirstSelectedItemPosition();

	// 데이터 정상인가 확인
	if (m_stAL_Info.Get_AL_Count() != GetItemCount())
	{
		// 에러
		//LT_MessageBox(_T("CList_LineConfig::Item_Add() Data Count Error"));
		LT_MessageBox(g_szMessageBox_T[MB_Line_DataCount_Error][m_nLanguage]);
		return;
	}

	m_stAL_Info.AL_Add(IN_SV);
	Add_AL(&IN_SV);

}

//=============================================================================
// Method		: Item_Insert
// Access		: public  
// Returns		: void
// Parameter	: __in CConfig_Eqp & IN_Equipment
// Qualifier	:
// Last Update	: 2023/02/21 - 16:15
// Desc.		:
//=============================================================================
void CList_AL_ListConfig::Item_Insert(__in CConfig_AL& IN_SV)
{
	if (0 < GetSelectedCount())
	{
		POSITION nPos = GetFirstSelectedItemPosition();
		int iIndex = GetNextSelectedItem(nPos);

		m_stAL_Info.AL_Insert(iIndex, IN_SV);

		Insert_AL(iIndex, &IN_SV);
	}
	else
	{
		// 항목을 선택 하세요.
	}
}

//=============================================================================
// Method		: Item_Remove
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2023/02/21 - 16:15
// Desc.		:
//=============================================================================
void CList_AL_ListConfig::Item_Remove()
{
	if (0 < GetSelectedCount())
	{
		POSITION nPos = GetFirstSelectedItemPosition();
		int iSelIdx = GetNextSelectedItem(nPos);

		DeleteItem(iSelIdx);
		Reset_Number();

		m_stAL_Info.AL_Remove(iSelIdx);

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
// Method		: Item_Up
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2023/02/21 - 16:18
// Desc.		:
//=============================================================================
void CList_AL_ListConfig::Item_Up()
{
	if (0 < GetSelectedCount())
	{
		POSITION nPos = GetFirstSelectedItemPosition();
		int iIndex = GetNextSelectedItem(nPos);

		// 0번 인덱스는 위로 이동 불가
		if ((0 < iIndex) && (1 < GetItemCount()))
		{
			CConfig_AL stStep = m_stAL_Info.GetAt(iIndex);

			DeleteItem(iIndex);
			Insert_AL(iIndex - 1, &stStep);

			m_stAL_Info.AL_Remove(iIndex);
			m_stAL_Info.AL_Insert(iIndex - 1, stStep);

			// 아이템 선택 활성화
			Select_SelectItem(iIndex - 1);
		}
	}
	else
	{
		// 항목을 선택 하세요.
	}
}

//=============================================================================
// Method		: Item_Down
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2023/02/21 - 16:21
// Desc.		:
//=============================================================================
void CList_AL_ListConfig::Item_Down()
{
	if (0 < GetSelectedCount())
	{
		POSITION nPos = GetFirstSelectedItemPosition();
		int iIndex = GetNextSelectedItem(nPos);

		// 마지막 인덱스는 아래로 이동 불가
		if ((iIndex < (GetItemCount() - 1)) && (1 < GetItemCount()))
		{
			CConfig_AL stStep = m_stAL_Info.GetAt(iIndex);

			DeleteItem(iIndex);


			m_stAL_Info.AL_Remove(iIndex);

			// 변경되는 위치가 최하단이면, Insert 대신 Add 사용
			if ((iIndex + 1) < (GetItemCount()))
			{
				Insert_AL(iIndex + 1, &stStep);
				m_stAL_Info.AL_Insert(iIndex + 1, stStep);
			}
			else
			{
				Add_AL(&stStep);
				m_stAL_Info.AL_Add(stStep);
			}

			// 아이템 선택 활성화
			Select_SelectItem(iIndex + 1);
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
// Parameter	: __in CConfig_SV& IN_SV
// Qualifier	:
// Last Update	: 2023/02/21 - 16:22
// Desc.		:
//=============================================================================
void CList_AL_ListConfig::Item_Modify(__in CConfig_AL& IN_SV)
{
	if (0 < GetSelectedCount())
	{
		POSITION nPos = GetFirstSelectedItemPosition();
		int iIndex = GetNextSelectedItem(nPos);

		// 수정
		m_stAL_Info.AL_Modify(iIndex, IN_SV);
		// 삭제, 추가
		Modify_AL(iIndex, &IN_SV);


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
// Method		: Get_SelectedSV
// Access		: public  
// Returns		: bool
// Parameter	: __out CConfig_SV& OUT_SV
// Qualifier	:
// Last Update	: 2023/02/21 - 16:24
// Desc.		:
//=============================================================================
bool CList_AL_ListConfig::Get_SelectedAL(__out CConfig_AL& OUT_SV)
{
	if (0 < GetSelectedCount())
	{
		POSITION nPos = GetFirstSelectedItemPosition();
		int iIndex = GetNextSelectedItem(nPos);

		OUT_SV = m_stAL_Info.GetAt(iIndex);

		return true;
	}
	else
	{
		return false;
	}
}
//=============================================================================
// Method		: Add_LineInfo
// Access		: public  
// Returns		: void
// Parameter	: __in const CConfig_SV_LIST* IN_pSVInfo
// Qualifier	:
// Last Update	: 2023/02/21 - 16:26
// Desc.		:
//=============================================================================
void CList_AL_ListConfig::Add_ALInfo(__in const CConfig_AL_LIST* IN_pSVInfo)
{
	if (NULL != IN_pSVInfo)
	{
		m_stAL_Info = *IN_pSVInfo;

		for (UINT nIdx = 0; nIdx < IN_pSVInfo->Get_AL_Count(); nIdx++)
		{
			Add_AL(&(IN_pSVInfo->GetAt(nIdx)));
		}
	}
}


//=============================================================================
// Method		: Insert_LineInfo
// Access		: public  
// Returns		: void
// Parameter	: __in const CConfig_SV_LIST * IN_pSVInfo
// Qualifier	:
// Last Update	: 2023/02/21 - 16:29
// Desc.		:
//=============================================================================
void CList_AL_ListConfig::Insert_ALInfo(__in const CConfig_AL_LIST* IN_pSVInfo)
{

	if (0 < GetSelectedCount())
	{
		POSITION nPos = GetFirstSelectedItemPosition();
		int iIndex = GetNextSelectedItem(nPos);

		for (UINT nIdx = 0; nIdx < IN_pSVInfo->Get_AL_Count(); nIdx++)
		{
			m_stAL_Info.AL_Insert(iIndex, IN_pSVInfo->GetAt(nIdx));

			Insert_AL(iIndex, &(IN_pSVInfo->GetAt(nIdx)));
		}
	}
	else
	{
		// 항목을 선택 하세요.
	}
}

//=============================================================================
// Method		: Clear_LineInfo
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/11/2 - 14:31
// Desc.		:
//=============================================================================
void CList_AL_ListConfig::Clear_ALInfo()
{
	m_stAL_Info.RemoveAll();
	DeleteAllItems();
}
/*
//=============================================================================
// Method		: Check_SVUI_Update
// Access		: public  
// Returns		: bool
// Qualifier	:
// Last Update	: 2021/11/10 - 17:58
// Desc.		: 리스트에서 설비 ID 중복 체크
//=============================================================================
bool CList_SV_ListConfig::Check_SV_UI_Update()
{
	for (auto nIdx = 0; nIdx < GetItemCount(); ++nIdx)
	{
		SetItemText(nIdx, TSH_ItemAlias, m_stLine_Info.GetAt(nIdx).Get_Alias().GetBuffer());
	}
}*/

//=============================================================================
// Method		: Check_EquipmentID
// Access		: public  
// Returns		: bool
// Qualifier	:
// Last Update	: 2021/11/10 - 17:58
// Desc.		: 리스트에서 설비 ID 중복 체크
//=============================================================================
bool CList_AL_ListConfig::Check_ALID()
{
	for (auto nIdx = 0; nIdx < m_stAL_Info.Get_AL_Count(); ++nIdx)
	{

		for (auto nComp = nIdx + 1; nComp < m_stAL_Info.Get_AL_Count(); ++nComp)
		{
			if (0 >= m_stAL_Info.GetAt(nComp).Get_ALID())
			{
				return false;
			}
			if (m_stAL_Info.GetAt(nIdx).Get_ALID() == m_stAL_Info.GetAt(nComp).Get_ALID())
			{
				return false;
			}
		}
	}

	return true;
}
bool CList_AL_ListConfig::Check_ALID(__in uint32_t IN_nSID)
{
	for (auto nIdx = 0; nIdx < m_stAL_Info.Get_AL_Count(); ++nIdx)
	{
		if (0 >= IN_nSID)
		{
			return false;
		}
		if (m_stAL_Info.GetAt(nIdx).Get_ALID() == IN_nSID)
		{
			return false;
		}
	}

	return true;
}

