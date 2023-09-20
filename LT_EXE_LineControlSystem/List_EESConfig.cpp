//*****************************************************************************
// Filename	: 	List_EESConfig.cpp
// Created	:	
// Modified	:	2023.04.21 - 11:00
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#include "stdafx.h"
#include "List_EESConfig.h"
#include "CommonFunction.h"
//#include "Def_ID_Type.h"
#include "Def_Language_Message.h"


typedef enum
{
	TSH_NO,
	TSH_EQUIPMENT_STATE,
	TSH_EQUIPMENT_STATE_NAME_KOR,
	TSH_EQUIPMENT_STATE_NAME_ENG,
	TSH_LOSSCODE,
	TSH_STATE_COLOR,
	TSH_USE,
	TSH_EXPLANATION,
	TSH_EES_MaxCol,
}enEESConfigHeader;

static const TCHAR* g_lpszHeader_T[Lang_MaxCount][TSH_EES_MaxCol] =
{
	// 한국어
	{
		_T("번호"),			// TSH_NO
		_T("상태"),		// TSH_EQUIPMENT_STATE
		_T("명칭(K)"),	// TSH_EQUIPMENT_STATE_NAME_KOR
		_T("명칭(E)"),	// TSH_EQUIPMENT_STATE_NAME_ENG
		_T("CODE"),	// TSH_LOSSCODE
		_T("색상"),	// TSH_STATE_COLOR
		_T("사용"),	// TSH_USE
		_T("설명"),			// TSH_EXPLANATION
	},

	// 영어
	{
		_T("번호"),			// TSH_NO
		_T("상태"),		// TSH_EQUIPMENT_STATE
		_T("명칭(K)"),	// TSH_EQUIPMENT_STATE_NAME_KOR
		_T("명칭(E)"),	// TSH_EQUIPMENT_STATE_NAME_ENG
		_T("CODE"),	// TSH_LOSSCODE
		_T("색상"),	// TSH_STATE_COLOR
		_T("사용"),	// TSH_USE
		_T("설명"),			// TSH_EXPLANATION
	},

	// 베트남어
	{
		_T("번호"),			// TSH_NO
		_T("상태"),		// TSH_EQUIPMENT_STATE
		_T("명칭(K)"),	// TSH_EQUIPMENT_STATE_NAME_KOR
		_T("명칭(E)"),	// TSH_EQUIPMENT_STATE_NAME_ENG
		_T("CODE"),	// TSH_LOSSCODE
		_T("색상"),	// TSH_STATE_COLOR
		_T("사용"),	// TSH_USE
		_T("설명"),			// TSH_EXPLANATION
	},

	// 중국어
	{
		_T("번호"),			// TSH_NO
		_T("상태"),		// TSH_EQUIPMENT_STATE
		_T("명칭(K)"),	// TSH_EQUIPMENT_STATE_NAME_KOR
		_T("명칭(E)"),	// TSH_EQUIPMENT_STATE_NAME_ENG
		_T("CODE"),	// TSH_LOSSCODE
		_T("색상"),	// TSH_STATE_COLOR
		_T("사용"),	// TSH_USE
		_T("설명"),			// TSH_EXPLANATION
	},
};

const int	iListAglin[] =
{
	LVCFMT_LEFT,	 // TSH_No
	LVCFMT_LEFT,	 // TSH_SID
	LVCFMT_LEFT,	 // TSH_SID_TYPE
	LVCFMT_LEFT,	 // TSH_SID_NAME
	LVCFMT_LEFT,	 // TSH_SID_TX
	LVCFMT_LEFT,	 // TSH_SV_TYPE
	LVCFMT_LEFT,	 // TSH_SV_USE
	LVCFMT_LEFT,	 // TSH_SV

};

// 540 기준
const int	iHeaderWidth[] =
{
	40, 	// TSH_No
	80,		// TSH_SID
	150,	// TSH_SID_TYPE
	150,	// TSH_SID_NAME
	80,		// TSH_SID_TX
	100,		// TSH_SV_TYPE
	60,		// TSH_SV_USE
	150,	// TSH_SV

};

const bool	bUse[] =
{
	true, 	// TSH_No
	true,	// TSH_SID
	true,	// TSH_SID_TYPE
	true,	// TSH_SID_NAME
	true,	// TSH_SID_TX
	true,	// TSH_SV_TYPE
	true,	// TSH_SV
	true,	// TSH_SV_USE
};

IMPLEMENT_DYNAMIC(CList_EESConfig, CListCtrl)

CList_EESConfig::CList_EESConfig()
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

CList_EESConfig::~CList_EESConfig()
{
	m_Font.DeleteObject();
}


BEGIN_MESSAGE_MAP(CList_EESConfig, CListCtrl)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_NOTIFY_REFLECT(NM_CLICK,	&CList_EESConfig::OnNMClick)
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()



// CList_EESConfig message handlers
//=============================================================================
// Method		: OnCreate
// Access		: protected  
// Returns		: int
// Parameter	: LPCREATESTRUCT lpCreateStruct
// Qualifier	:
// Last Update	: 2023.04.21 - 11:00
// Desc.		:
//=============================================================================
int CList_EESConfig::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetFont(&m_Font);

	SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER);

	InitEESHeader();

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
// Last Update	: 2023.04.21 - 11:00
// Desc.		:
//=============================================================================
void CList_EESConfig::OnSize(UINT nType, int cx, int cy)
{
	CListCtrl::OnSize(nType, cx, cy);
}

//=============================================================================
// Method		: PreCreateWindow
// Access		: virtual protected  
// Returns		: BOOL
// Parameter	: CREATESTRUCT & cs
// Qualifier	:
// Last Update	: 2023.04.21 - 11:00
// Desc.		:
//=============================================================================
BOOL CList_EESConfig::PreCreateWindow(CREATESTRUCT& cs)
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
// Last Update	: 2023.04.21 - 11:00
// Desc.		:
//=============================================================================
void CList_EESConfig::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
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
// Last Update	: 2023.04.21 - 11:00
// Desc.		:
//=============================================================================
void CList_EESConfig::InitEESHeader()
{
	//int iMaxCol = m_bUseExtraData ? TSH_MaxCol : TSH_ZoneCnt;

	if (FALSE == m_bIntiHeader)
	{
		m_bIntiHeader = TRUE;

		for (int nCol = 0; nCol < TSH_EES_MaxCol; nCol++)
		{
			InsertColumn(nCol, g_lpszHeader_T[m_nLanguage][nCol], iListAglin[nCol], iHeaderWidth[nCol]);
		}
	}

	for (int nCol = 0; nCol < TSH_EES_MaxCol; nCol++)
	{
		SetColumnWidth(nCol, m_pHeadWidth[nCol]);
	}
}

//=============================================================================
// Method		: Reset_Number
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2023.04.21 - 11:00
// Desc.		:
//=============================================================================
void CList_EESConfig::Reset_Number()
{
	CString szText;

	for (int nIdx = 0; nIdx < GetItemCount(); nIdx++)
	{
		// TSH_No
		szText.Format(_T("%d"), nIdx + 1);
		SetItemText(nIdx, TSH_NO, szText);
	}
}

//=============================================================================
// Method		: Select_SelectItem
// Access		: protected  
// Returns		: void
// Parameter	: __in int nItem
// Qualifier	:
// Last Update	: 2023.04.21 - 11:00
// Desc.		:
//=============================================================================
void CList_EESConfig::Select_SelectItem(__in int nItem)
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
// Method		: Set_EES_Data
// Access		: protected  
// Returns		: void
// Parameter	: __in int nItem
// Parameter	: __in const CConfig_EES * IN_pData
// Qualifier	:
// Last Update	: 2023.04.21 - 11:00
// Desc.		:
//=============================================================================
void CList_EESConfig::Set_EES_Data(__in int nItem, __in const CConfig_EES* IN_pData)
{
	ASSERT(GetSafeHwnd());
	if (NULL == IN_pData)
		return;

	if (GetItemCount() <= nItem)
		return;

	CString szText;

	szText.Format(_T("%d"), nItem + 1);
	SetItemText(nItem, TSH_NO, szText);

	szText.Format(_T("%s"), IN_pData->Get_EquipMentStateCode());
	SetItemText(nItem, TSH_EQUIPMENT_STATE, (szText));

	szText.Format(_T("%s"), IN_pData->Get_EquipMentStateKorText());
	SetItemText(nItem, TSH_EQUIPMENT_STATE_NAME_KOR, (szText));

	szText.Format(_T("%s"), IN_pData->Get_EquipMentStateEngText());
	SetItemText(nItem, TSH_EQUIPMENT_STATE_NAME_ENG, (szText));

	szText.Format(_T("%s"), IN_pData->Get_LossCode());
	SetItemText(nItem, TSH_LOSSCODE, (szText));

	szText.Format(_T("%s"), IN_pData->Get_StateColor());
	SetItemText(nItem, TSH_STATE_COLOR, (szText));

	szText.Format(_T("%s"), g_szEES_USE[IN_pData->Get_Use()]);
	SetItemText(nItem, TSH_USE,  (szText));

	szText.Format(_T("%s"), IN_pData->Get_Explanation());
	SetItemText(nItem, TSH_EXPLANATION, (szText));
}

//=============================================================================
// Method		: Insert_EES
// Access		: protected  
// Returns		: void
// Parameter	: __in int nItem
// Parameter	: __in const CConfig_EES* IN_pData
// Qualifier	:
// Last Update	: 2023.04.21 - 11:00
// Desc.		:
//=============================================================================
void CList_EESConfig::Insert_EES(__in int nItem, __in const CConfig_EES* IN_pData)
{
	ASSERT(GetSafeHwnd());
	if (NULL == IN_pData)
		return;

	if (GetItemCount() <= nItem)
		return;

	int iNewCount = nItem;

	InsertItem(iNewCount, _T(""));

	Set_EES_Data(iNewCount, IN_pData);

	// 번호 재부여
	Reset_Number();

	// 화면에 보이게 하기
	EnsureVisible(iNewCount, TRUE);
	ListView_SetItemState(GetSafeHwnd(), iNewCount, LVIS_FOCUSED | LVIS_SELECTED, 0x000F);
}

//=============================================================================
// Method		: Add_EES
// Access		: protected  
// Returns		: void
// Parameter	: const __in CConfig_EES* IN_pData
// Qualifier	:
// Last Update	: 2023.04.21 - 11:00
// Desc.		:
//=============================================================================
void CList_EESConfig::Add_EES(const __in CConfig_EES* IN_pData)
{
	ASSERT(GetSafeHwnd());
	if (NULL == IN_pData)
		return;

	int iNewCount = GetItemCount();

	InsertItem(iNewCount, _T(""));

	Set_EES_Data(iNewCount, IN_pData);

	// 화면에 보이게 하기
	EnsureVisible(iNewCount, TRUE);
	ListView_SetItemState(GetSafeHwnd(), iNewCount, LVIS_FOCUSED | LVIS_SELECTED, 0x000F);
}

//=============================================================================
// Method		: Modify_EES
// Access		: protected  
// Returns		: void
// Parameter	: __in int nItem
// Parameter	: __in const CConfig_EES* IN_pData
// Qualifier	:
// Last Update	: 2023.04.21 - 11:00
// Desc.		:
//=============================================================================
void CList_EESConfig::Modify_EES(__in int nItem, __in const CConfig_EES* IN_pData)
{
	ASSERT(GetSafeHwnd());
	if (NULL == IN_pData)
		return;

	if (GetItemCount() <= nItem)
		return;

	Set_EES_Data(nItem, IN_pData);

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
// Last Update	: 2023.04.21 - 11:00
// Desc.		:
//=============================================================================
void CList_EESConfig::OnLanguage(__in uint8_t IN_nLanguage)
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
// Method		: Set_EESInfo
// Access		: public  
// Returns		: void
// Parameter	: __in const CConfig_SV_LIST* IN_pSVInfo
// Qualifier	:
// Last Update	: 2023.04.21 - 11:00
// Desc.		:
//=============================================================================
void CList_EESConfig::Set_EESInfo(__in const CConfig_EES_LIST* IN_pInfo)
{
	m_stEES_Info.RemoveAll();
	DeleteAllItems();

	if (NULL != IN_pInfo)
	{
		m_stEES_Info = *IN_pInfo;

		for (UINT nIdx = 0; nIdx < IN_pInfo->Get_EES_Count(); nIdx++)
		{
			Add_EES(&(IN_pInfo->GetAt(nIdx)));
		}
	}
}

//=============================================================================
// Method		: Get_SVInfo
// Access		: public  
// Returns		: void
// Parameter	: __out CConfig_SV_LIST& OUT_Info
// Qualifier	:
// Last Update	: 2023.04.21 - 11:00
// Desc.		:
//=============================================================================
void CList_EESConfig::Get_EESInfo(__out CConfig_EES_LIST& OUT_Info)
{
	OUT_Info.RemoveAll();
	OUT_Info = m_stEES_Info;
}

//=============================================================================
// Method		: Item_Add
// Access		: public  
// Returns		: void
// Parameter	: __in CConfig_SV& IN_Data
// Qualifier	:
// Last Update	: 2023.04.21 - 11:00
// Desc.		:
//=============================================================================
void CList_EESConfig::Item_Add(__in CConfig_EES& IN_Data)
{
	POSITION posSel = GetFirstSelectedItemPosition();
	// 데이터 정상인가 확인
	if (m_stEES_Info.Get_EES_Count() != GetItemCount())
	{
		// 에러
		//LT_MessageBox(_T("CList_LineConfig::Item_Add() Data Count Error"));
		LT_MessageBox(g_szMessageBox_T[MB_Line_DataCount_Error][m_nLanguage]);
		return;
	}
	m_stEES_Info.EES_Add(IN_Data);
	Add_EES(&IN_Data);
}

//=============================================================================
// Method		: Item_Insert
// Access		: public  
// Returns		: void
// Parameter	: __in CConfig_Eqp & IN_Data
// Qualifier	:
// Last Update	: 2023.04.21 - 11:00
// Desc.		:
//=============================================================================
void CList_EESConfig::Item_Insert(__in CConfig_EES& IN_Data)
{
	if (0 < GetSelectedCount())
	{
		POSITION nPos = GetFirstSelectedItemPosition();
		int iIndex = GetNextSelectedItem(nPos);

		m_stEES_Info.EES_Insert(iIndex, IN_Data);

		Insert_EES(iIndex, &IN_Data);
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
// Last Update	: 2023.04.21 - 11:00
// Desc.		:
//=============================================================================
void CList_EESConfig::Item_Remove()
{
	if (0 < GetSelectedCount())
	{
		POSITION nPos = GetFirstSelectedItemPosition();
		int iSelIdx = GetNextSelectedItem(nPos);

		DeleteItem(iSelIdx);
		Reset_Number();

		m_stEES_Info.EES_Remove(iSelIdx);

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
// Last Update	: 2023.04.21 - 11:00
// Desc.		:
//=============================================================================
void CList_EESConfig::Item_Up()
{
	if (0 < GetSelectedCount())
	{
		POSITION nPos = GetFirstSelectedItemPosition();
		int iIndex = GetNextSelectedItem(nPos);

		// 0번 인덱스는 위로 이동 불가
		if ((0 < iIndex) && (1 < GetItemCount()))
		{
			CConfig_EES stStep = m_stEES_Info.GetAt(iIndex);

			DeleteItem(iIndex);
			Insert_EES(iIndex - 1, &stStep);

			m_stEES_Info.EES_Remove(iIndex);
			m_stEES_Info.EES_Insert(iIndex - 1, stStep);

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
// Last Update	: 2023.04.21 - 11:00
// Desc.		:
//=============================================================================
void CList_EESConfig::Item_Down()
{
	if (0 < GetSelectedCount())
	{
		POSITION nPos = GetFirstSelectedItemPosition();
		int iIndex = GetNextSelectedItem(nPos);

		// 마지막 인덱스는 아래로 이동 불가
		if ((iIndex < (GetItemCount() - 1)) && (1 < GetItemCount()))
		{
			CConfig_EES stStep = m_stEES_Info.GetAt(iIndex);

			DeleteItem(iIndex);


			m_stEES_Info.EES_Remove(iIndex);

			// 변경되는 위치가 최하단이면, Insert 대신 Add 사용
			if ((iIndex + 1) < (GetItemCount()))
			{
				Insert_EES(iIndex + 1, &stStep);
				m_stEES_Info.EES_Insert(iIndex + 1, stStep);
			}
			else
			{
				Add_EES(&stStep);
				m_stEES_Info.EES_Add(stStep);
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
// Parameter	: __in CConfig_SV& IN_Data
// Qualifier	:
// Last Update	: 2023.04.21 - 11:00
// Desc.		:
//=============================================================================
void CList_EESConfig::Item_Modify(__in CConfig_EES& IN_Data)
{
	if (0 < GetSelectedCount())
	{
		POSITION nPos = GetFirstSelectedItemPosition();
		int iIndex = GetNextSelectedItem(nPos);

		// 수정
		m_stEES_Info.EES_Modify(iIndex, IN_Data);
		// 삭제, 추가
		Modify_EES(iIndex, &IN_Data);


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
// Method		: Get_SelectedEES
// Access		: public  
// Returns		: bool
// Parameter	: __out CConfig_SV& OUT_Data
// Qualifier	:
// Last Update	: 2023.04.21 - 11:00
// Desc.		:
//=============================================================================
bool CList_EESConfig::Get_SelectedEES(__out CConfig_EES& OUT_Data)
{
	if (0 < GetSelectedCount())
	{
		POSITION nPos = GetFirstSelectedItemPosition();
		int iIndex = GetNextSelectedItem(nPos);

		OUT_Data = m_stEES_Info.GetAt(iIndex);

		return true;
	}
	else
	{
		return false;
	}
}
//=============================================================================
// Method		: Add_EESInfo
// Access		: public  
// Returns		: void
// Parameter	: __in const CConfig_EES_LIST* IN_pInfo
// Qualifier	:
// Last Update	: 2023.04.21 - 11:00
// Desc.		:
//=============================================================================
void CList_EESConfig::Add_EESInfo(__in const CConfig_EES_LIST* IN_pInfo)
{
	if (NULL != IN_pInfo)
	{
		m_stEES_Info = *IN_pInfo;

		for (UINT nIdx = 0; nIdx < IN_pInfo->Get_EES_Count(); nIdx++)
		{
			Add_EES(&(IN_pInfo->GetAt(nIdx)));
		}
	}
}


//=============================================================================
// Method		: Insert_EESInfo
// Access		: public  
// Returns		: void
// Parameter	: __in const CConfig_EES_LIST * IN_pInfo
// Qualifier	:
// Last Update	: 2023.04.21 - 11:00
// Desc.		:
//=============================================================================
void CList_EESConfig::Insert_EESInfo(__in const CConfig_EES_LIST* IN_pInfo)
{

	if (0 < GetSelectedCount())
	{
		POSITION nPos = GetFirstSelectedItemPosition();
		int iIndex = GetNextSelectedItem(nPos);

		for (UINT nIdx = 0; nIdx < IN_pInfo->Get_EES_Count(); nIdx++)
		{
			m_stEES_Info.EES_Insert(iIndex, IN_pInfo->GetAt(nIdx));

			Insert_EES(iIndex, &(IN_pInfo->GetAt(nIdx)));
		}
	}
	else
	{
		// 항목을 선택 하세요.
	}
}

//=============================================================================
// Method		: Clear_EESInfo
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/11/2 - 14:31
// Desc.		:
//=============================================================================
void CList_EESConfig::Clear_EESInfo()
{
	m_stEES_Info.RemoveAll();
	DeleteAllItems();
}

//=============================================================================
// Method		: Check_EESCode
// Access		: public  
// Returns		: bool
// Qualifier	:
// Last Update	: 2021/11/10 - 17:58
// Desc.		: EES Code중복 체크
//=============================================================================
bool CList_EESConfig::Check_EESCode()
{
	for (auto nIdx = 0; nIdx < m_stEES_Info.Get_EES_Count(); ++nIdx)
	{

		for (auto nComp = nIdx + 1; nComp < m_stEES_Info.Get_EES_Count(); ++nComp)
		{
			if (0 >= m_stEES_Info.GetAt(nComp).Get_EquipMentStateCode())
			{
				return false;
			}
			if (m_stEES_Info.GetAt(nIdx).Get_EquipMentStateCode() == m_stEES_Info.GetAt(nComp).Get_EquipMentStateCode())
			{
				return false;
			}
		}
	}

	return true;
}
bool CList_EESConfig::Check_EESCode(__in LPCTSTR IN_Data)
{
	for (auto nIdx = 0; nIdx < m_stEES_Info.Get_EES_Count(); ++nIdx)
	{
		if (0 == m_stEES_Info.GetAt(nIdx).Get_EquipMentStateCode().Compare(IN_Data))
		{
			return false;
		}
	}
	return true;
}

