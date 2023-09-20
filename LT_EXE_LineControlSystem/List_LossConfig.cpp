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
#include "List_LossConfig.h"
#include "CommonFunction.h"
#include "Def_Language_Message.h"


typedef enum
{
	TSH_NO,
	TSH_NAMELOSS_TEM,
	TSH_TEM,
	TSH_CODELOSS,
	TSH_NAMELOSS_TEM_ENG,
	TSH_EXPLAINLOSS,
	TSH_LOSS_MaxCol,
}enEESConfigHeader;

static const TCHAR* g_lpszHeader_T[Lang_MaxCount][TSH_LOSS_MaxCol] =
{
	// 한국어
	{
		_T("번호"),		// TSH_NO
		_T("로스명칭"),		// TSH_NO
		_T("템"),		// TSH_EQUIPMENT_STATE
		_T("코드"),		// TSH_EQUIPMENT_STATE_NAME_KOR
		_T("로스명칭(E)"),	// TSH_EQUIPMENT_STATE_NAME_ENG
		_T("설명"),			// TSH_EXPLANATION
	},

	// 영어
	{
		_T("번호"),		// TSH_NO
		_T("로스명칭"),		// TSH_NO
		_T("템"),		// TSH_EQUIPMENT_STATE
		_T("코드"),		// TSH_EQUIPMENT_STATE_NAME_KOR
		_T("로스명칭(E)"),	// TSH_EQUIPMENT_STATE_NAME_ENG
		_T("설명"),			// TSH_EXPLANATION
	},

	// 베트남어
	{
		_T("번호"),		// TSH_NO
		_T("로스명칭"),		// TSH_NO
		_T("템"),		// TSH_EQUIPMENT_STATE
		_T("코드"),		// TSH_EQUIPMENT_STATE_NAME_KOR
		_T("로스명칭(E)"),	// TSH_EQUIPMENT_STATE_NAME_ENG
		_T("설명"),			// TSH_EXPLANATION
	},

	// 중국어
	{
		_T("번호"),		// TSH_NO
		_T("로스명칭"),		// TSH_NO
		_T("템"),		// TSH_EQUIPMENT_STATE
		_T("코드"),		// TSH_EQUIPMENT_STATE_NAME_KOR
		_T("로스명칭(E)"),	// TSH_EQUIPMENT_STATE_NAME_ENG
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
};

// 540 기준
const int	iHeaderWidth[] =
{
	40, 	// TSH_NO
	100,	// TSH_NAMELOSS_TEM
	40,		// TSH_TEM
	60,		// TSH_CODELOSS
	150,	// TSH_NAMELOSS_TEM_ENG
	500,	// TSH_EXPLAINLOSS
};

const bool	bUse[] =
{
	true, 	// TSH_No
	true,	// TSH_SID
	true,	// TSH_SID_TYPE
	true,	// TSH_SID_NAME
	true,	// TSH_SID_TX
	true,	// TSH_SV_TYPE
};

IMPLEMENT_DYNAMIC(CList_LossConfig, CListCtrl)

CList_LossConfig::CList_LossConfig()
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
CList_LossConfig::~CList_LossConfig()
{
	m_Font.DeleteObject();
}
BEGIN_MESSAGE_MAP(CList_LossConfig, CListCtrl)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_NOTIFY_REFLECT(NM_CLICK,	&CList_LossConfig::OnNMClick)
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
int CList_LossConfig::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetFont(&m_Font);

	SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER);

	InitLossHeader();

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
void CList_LossConfig::OnSize(UINT nType, int cx, int cy)
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
BOOL CList_LossConfig::PreCreateWindow(CREATESTRUCT& cs)
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
void CList_LossConfig::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	NM_LISTVIEW* pNMView = (NM_LISTVIEW*)pNMHDR;
	int index = pNMView->iItem;

	*pResult = 0;
}

//=============================================================================
// Method		: InitLossHeader
// Access		: virtual protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2023.04.21 - 11:00
// Desc.		:
//=============================================================================
void CList_LossConfig::InitLossHeader()
{
	//int iMaxCol = m_bUseExtraData ? TSH_MaxCol : TSH_ZoneCnt;

	if (FALSE == m_bIntiHeader)
	{
		m_bIntiHeader = TRUE;

		for (int nCol = 0; nCol < TSH_LOSS_MaxCol; nCol++)
		{
			InsertColumn(nCol, g_lpszHeader_T[m_nLanguage][nCol], iListAglin[nCol], iHeaderWidth[nCol]);
		}
	}

	for (int nCol = 0; nCol < TSH_LOSS_MaxCol; nCol++)
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
void CList_LossConfig::Reset_Number()
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
void CList_LossConfig::Select_SelectItem(__in int nItem)
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
// Method		: Set_Loss_Data
// Access		: protected  
// Returns		: void
// Parameter	: __in int nItem
// Parameter	: __in const CConfig_EES * IN_pData
// Qualifier	:
// Last Update	: 2023.04.21 - 11:00
// Desc.		:
//=============================================================================
void CList_LossConfig::Set_Loss_Data(__in int nItem, __in const CConfig_Loss* IN_pData)
{
	ASSERT(GetSafeHwnd());
	if (NULL == IN_pData)
		return;

	if (GetItemCount() <= nItem)
		return;

	CString szText;

	szText.Format(_T("%d"), nItem + 1);
	SetItemText(nItem, TSH_NO, szText);

	szText.Format(_T("%s"), IN_pData->Get_NameLoss());
	SetItemText(nItem, TSH_NAMELOSS_TEM, (szText));

	szText.Format(_T("%s"), IN_pData->Get_Tem());
	SetItemText(nItem, TSH_TEM, (szText));

	szText.Format(_T("%s"), IN_pData->Get_CodeLoss());
	SetItemText(nItem, TSH_CODELOSS, (szText));

	szText.Format(_T("%s"), IN_pData->Get_NameLossEng());
	SetItemText(nItem, TSH_NAMELOSS_TEM_ENG, (szText));

	szText.Format(_T("%s"), IN_pData->Get_Explain());
	SetItemText(nItem, TSH_EXPLAINLOSS, (szText));
}

//=============================================================================
// Method		: Insert_Loss
// Access		: protected  
// Returns		: void
// Parameter	: __in int nItem
// Parameter	: __in const CConfig_EES* IN_pData
// Qualifier	:
// Last Update	: 2023.04.21 - 11:00
// Desc.		:
//=============================================================================
void CList_LossConfig::Insert_Loss(__in int nItem, __in const CConfig_Loss* IN_pData)
{
	ASSERT(GetSafeHwnd());
	if (NULL == IN_pData)
		return;

	if (GetItemCount() <= nItem)
		return;

	int iNewCount = nItem;

	InsertItem(iNewCount, _T(""));

	Set_Loss_Data(iNewCount, IN_pData);

	// 번호 재부여
	Reset_Number();

	// 화면에 보이게 하기
	EnsureVisible(iNewCount, TRUE);
	ListView_SetItemState(GetSafeHwnd(), iNewCount, LVIS_FOCUSED | LVIS_SELECTED, 0x000F);
}

//=============================================================================
// Method		: Add_Loss
// Access		: protected  
// Returns		: void
// Parameter	: const __in CConfig_EES* IN_pData
// Qualifier	:
// Last Update	: 2023.04.21 - 11:00
// Desc.		:
//=============================================================================
void CList_LossConfig::Add_Loss(const __in CConfig_Loss* IN_pData)
{
	ASSERT(GetSafeHwnd());
	if (NULL == IN_pData)
		return;

	int iNewCount = GetItemCount();

	InsertItem(iNewCount, _T(""));

	Set_Loss_Data(iNewCount, IN_pData);

	// 화면에 보이게 하기
	EnsureVisible(iNewCount, TRUE);
	ListView_SetItemState(GetSafeHwnd(), iNewCount, LVIS_FOCUSED | LVIS_SELECTED, 0x000F);
}

//=============================================================================
// Method		: Modify_Loss
// Access		: protected  
// Returns		: void
// Parameter	: __in int nItem
// Parameter	: __in const CConfig_EES* IN_pData
// Qualifier	:
// Last Update	: 2023.04.21 - 11:00
// Desc.		:
//=============================================================================
void CList_LossConfig::Modify_Loss(__in int nItem, __in const CConfig_Loss* IN_pData)
{
	ASSERT(GetSafeHwnd());
	if (NULL == IN_pData)
		return;

	if (GetItemCount() <= nItem)
		return;

	Set_Loss_Data(nItem, IN_pData);

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
void CList_LossConfig::OnLanguage(__in uint8_t IN_nLanguage)
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
// Method		: Set_LossInfo
// Access		: public  
// Returns		: void
// Parameter	: __in const CConfig_SV_LIST* IN_pSVInfo
// Qualifier	:
// Last Update	: 2023.04.21 - 11:00
// Desc.		:
//=============================================================================
void CList_LossConfig::Set_LossInfo(__in const CConfig_Loss_LIST* IN_pInfo)
{
	m_stEES_Info.RemoveAll();
	DeleteAllItems();

	if (NULL != IN_pInfo)
	{
		m_stEES_Info = *IN_pInfo;

		for (UINT nIdx = 0; nIdx < IN_pInfo->Get_Loss_Count(); nIdx++)
		{
			Add_Loss(&(IN_pInfo->GetAt(nIdx)));
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
void CList_LossConfig::Get_LossInfo(__out CConfig_Loss_LIST& OUT_Info)
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
void CList_LossConfig::Item_Add(__in CConfig_Loss& IN_Data)
{
	POSITION posSel = GetFirstSelectedItemPosition();
	// 데이터 정상인가 확인
	if (m_stEES_Info.Get_Loss_Count() != GetItemCount())
	{
		// 에러
		//LT_MessageBox(_T("CList_LineConfig::Item_Add() Data Count Error"));
		LT_MessageBox(g_szMessageBox_T[MB_Line_DataCount_Error][m_nLanguage]);
		return;
	}
	m_stEES_Info.Loss_Add(IN_Data);
	Add_Loss(&IN_Data);
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
void CList_LossConfig::Item_Insert(__in CConfig_Loss& IN_Data)
{
	if (0 < GetSelectedCount())
	{
		POSITION nPos = GetFirstSelectedItemPosition();
		int iIndex = GetNextSelectedItem(nPos);

		m_stEES_Info.Loss_Insert(iIndex, IN_Data);

		Insert_Loss(iIndex, &IN_Data);
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
void CList_LossConfig::Item_Remove()
{
	if (0 < GetSelectedCount())
	{
		POSITION nPos = GetFirstSelectedItemPosition();
		int iSelIdx = GetNextSelectedItem(nPos);

		DeleteItem(iSelIdx);
		Reset_Number();

		m_stEES_Info.Loss_Remove(iSelIdx);

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
void CList_LossConfig::Item_Up()
{
	if (0 < GetSelectedCount())
	{
		POSITION nPos = GetFirstSelectedItemPosition();
		int iIndex = GetNextSelectedItem(nPos);

		// 0번 인덱스는 위로 이동 불가
		if ((0 < iIndex) && (1 < GetItemCount()))
		{
			CConfig_Loss stStep = m_stEES_Info.GetAt(iIndex);

			DeleteItem(iIndex);
			Insert_Loss(iIndex - 1, &stStep);

			m_stEES_Info.Loss_Remove(iIndex);
			m_stEES_Info.Loss_Insert(iIndex - 1, stStep);

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
void CList_LossConfig::Item_Down()
{
	if (0 < GetSelectedCount())
	{
		POSITION nPos = GetFirstSelectedItemPosition();
		int iIndex = GetNextSelectedItem(nPos);

		// 마지막 인덱스는 아래로 이동 불가
		if ((iIndex < (GetItemCount() - 1)) && (1 < GetItemCount()))
		{
			CConfig_Loss stStep = m_stEES_Info.GetAt(iIndex);

			DeleteItem(iIndex);


			m_stEES_Info.Loss_Remove(iIndex);

			// 변경되는 위치가 최하단이면, Insert 대신 Add 사용
			if ((iIndex + 1) < (GetItemCount()))
			{
				Insert_Loss(iIndex + 1, &stStep);
				m_stEES_Info.Loss_Insert(iIndex + 1, stStep);
			}
			else
			{
				Add_Loss(&stStep);
				m_stEES_Info.Loss_Add(stStep);
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
void CList_LossConfig::Item_Modify(__in CConfig_Loss& IN_Data)
{
	if (0 < GetSelectedCount())
	{
		POSITION nPos = GetFirstSelectedItemPosition();
		int iIndex = GetNextSelectedItem(nPos);

		// 수정
		m_stEES_Info.Loss_Modify(iIndex, IN_Data);
		// 삭제, 추가
		Modify_Loss(iIndex, &IN_Data);


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
bool CList_LossConfig::Get_SelectedLoss(__out CConfig_Loss& OUT_Data)
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
void CList_LossConfig::Add_LossInfo(__in const CConfig_Loss_LIST* IN_pInfo)
{
	if (NULL != IN_pInfo)
	{
		m_stEES_Info = *IN_pInfo;

		for (UINT nIdx = 0; nIdx < IN_pInfo->Get_Loss_Count(); nIdx++)
		{
			Add_Loss(&(IN_pInfo->GetAt(nIdx)));
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
void CList_LossConfig::Insert_LossInfo(__in const CConfig_Loss_LIST* IN_pInfo)
{

	if (0 < GetSelectedCount())
	{
		POSITION nPos = GetFirstSelectedItemPosition();
		int iIndex = GetNextSelectedItem(nPos);

		for (UINT nIdx = 0; nIdx < IN_pInfo->Get_Loss_Count(); nIdx++)
		{
			m_stEES_Info.Loss_Insert(iIndex, IN_pInfo->GetAt(nIdx));

			Insert_Loss(iIndex, &(IN_pInfo->GetAt(nIdx)));
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
void CList_LossConfig::Clear_LossInfo()
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
bool CList_LossConfig::Check_LossCode()
{
	for (auto nIdx = 0; nIdx < m_stEES_Info.Get_Loss_Count(); ++nIdx)
	{

		for (auto nComp = nIdx + 1; nComp < m_stEES_Info.Get_Loss_Count(); ++nComp)
		{
			if (0 >= m_stEES_Info.GetAt(nComp).Get_CodeLoss())
			{
				return false;
			}
			if (m_stEES_Info.GetAt(nIdx).Get_CodeLoss() == m_stEES_Info.GetAt(nComp).Get_CodeLoss())
			{
				return false;
			}
		}
	}

	return true;
}
bool CList_LossConfig::Check_LossCode(__in LPCTSTR IN_Data)
{
	for (auto nIdx = 0; nIdx < m_stEES_Info.Get_Loss_Count(); ++nIdx)
	{
		if (0 == m_stEES_Info.GetAt(nIdx).Get_CodeLoss().Compare(IN_Data))
		{
			return false;
		}
	}
	return true;
}

