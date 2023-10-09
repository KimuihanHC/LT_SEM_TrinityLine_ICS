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
#include "List_LineConfig.h"
#include "CommonFunction.h"
#include "Def_Equipment_Type.h"
#include "Def_Language_Message.h"

typedef enum 
{
	TSH_No,
	TSH_ItemAlias,			// Eqp Type + Number
	TSH_EqpType,			// Equipmnet Type
	TSH_EqpId,				// Equipment id
	TSH_IpAddr,				// ip address
// 	TSH_ZoneCnt,			// Test Zone Count
// 	TSH_PortCnt,			// Buffer Count
// 	TSH_ConvCnt,			// Conveyor Count
// 	TSH_ReturnCnt,			// Return Conveyor Count

	TSH_MaxCol,
}enLineConfigHeader;

// 헤더
// static const TCHAR*	g_lpszHeader[] =
// {
// 	_T("No"),				// TSH_No
// 	_T("Alias"),			// TSH_ItemName
// 	_T("EQP Type"),			// TSH_Data_1
// 	_T("EQP ID"),			// TSH_Data_2
// 	_T("IP Addr"),			// TSH_Data_3
// // 	_T("Test Zone"),		// TSH_Data_4
// // 	_T("Buffer"),			// TSH_Data_5
// // 	_T("Conveyor"),			// TSH_Data_6
// // 	_T("Return"),			// TSH_Data_7
// 
// 	NULL
// };


static const TCHAR* g_lpszHeader_T[Lang_MaxCount][TSH_MaxCol] =
{
	// 한국어
	{
		_T("번호"),				// TSH_No
		_T("설비 별칭"),			// TSH_ItemName
		_T("설비 종류"),			// TSH_Data_1
		_T("설비 ID"),			// TSH_Data_2
		_T("IP 주소"),			// TSH_Data_3
	},

	// 영어
	{
		_T("No"),				// TSH_No
		_T("Alias"),			// TSH_ItemName
		_T("EQP Type"),			// TSH_Data_1
		_T("EQP ID"),			// TSH_Data_2
		_T("IP Addr"),			// TSH_Data_3
	},

	// 베트남어
	{		
		_T("No"),				// TSH_No
		_T("Alias"),			// TSH_ItemName
		_T("EQP Type"),			// TSH_Data_1
		_T("EQP ID"),			// TSH_Data_2
		_T("IP Addr"),			// TSH_Data_3
	},

	// 중국어
	{
		_T("号码"),				// TSH_No
		_T("别称"),				// TSH_ItemName
		_T("设备类型"),			// TSH_Data_1
		_T("设备ID"),			// TSH_Data_2
		_T("IP地址"),			// TSH_Data_3
	},
};

const int	iListAglin[] =
{
	LVCFMT_LEFT,	 // TSH_No
	LVCFMT_LEFT,	 // TSH_ItemName
	LVCFMT_LEFT,	 // TSH_Data_1	
	LVCFMT_LEFT,	 // TSH_Data_1
	LVCFMT_LEFT,	 // TSH_Data_3
// 	LVCFMT_LEFT,	 // TSH_Data_4
// 	LVCFMT_LEFT,	 // TSH_Data_5
// 	LVCFMT_LEFT,	 // TSH_Data_6
// 	LVCFMT_LEFT,	 // TSH_Data_7
};

// 540 기준
const int	iHeaderWidth[] =
{
	40, 	// TSH_No
	200,	// TSH_ItemName
	150,	// TSH_Data_1
	200,	// TSH_Data_2
	200,	// TSH_Data_3
// 	80,		// TSH_Data_4
// 	80,		// TSH_Data_5
// 	80,		// TSH_Data_6
// 	80,		// TSH_Data_7
};

IMPLEMENT_DYNAMIC(CList_LineConfig, CListCtrl)

CList_LineConfig::CList_LineConfig()
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

CList_LineConfig::~CList_LineConfig()
{
	m_Font.DeleteObject();
}


BEGIN_MESSAGE_MAP(CList_LineConfig, CListCtrl)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_NOTIFY_REFLECT(NM_CLICK,	&CList_LineConfig::OnNMClick)
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()



// CList_LineConfig message handlers
//=============================================================================
// Method		: OnCreate
// Access		: protected  
// Returns		: int
// Parameter	: LPCREATESTRUCT lpCreateStruct
// Qualifier	:
// Last Update	: 2021/11/2 - 14:28
// Desc.		:
//=============================================================================
int CList_LineConfig::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetFont(&m_Font);

	SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER);

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
void CList_LineConfig::OnSize(UINT nType, int cx, int cy)
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
BOOL CList_LineConfig::PreCreateWindow(CREATESTRUCT& cs)
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
void CList_LineConfig::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
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
void CList_LineConfig::InitHeader()
{
	//int iMaxCol = m_bUseExtraData ? TSH_MaxCol : TSH_ZoneCnt;

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
void CList_LineConfig::Reset_Number()
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
// Last Update	: 2021/11/2 - 14:29
// Desc.		:
//=============================================================================
void CList_LineConfig::Select_SelectItem(__in int nItem)
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
// Method		: Set_EquipmentData
// Access		: protected  
// Returns		: void
// Parameter	: __in int nItem
// Parameter	: __in const CConfig_Eqp * IN_pEquipment
// Qualifier	:
// Last Update	: 2021/11/2 - 14:29
// Desc.		:
//=============================================================================
void CList_LineConfig::Set_EquipmentData(__in int nItem, __in const CConfig_Eqp* IN_pEquipment)
{
	ASSERT(GetSafeHwnd());
	if (NULL == IN_pEquipment)
		return;

	if (GetItemCount() <= nItem)
		return;

	CString szText;

	// TSH_No
	szText.Format(_T("%d"), nItem + 1);
	SetItemText(nItem, TSH_No, szText);

	// Alias (Eqp Type + Number)
	SetItemText(nItem, TSH_ItemAlias, IN_pEquipment->Get_Alias());

	// Equipmnet Type
	//SetItemText(nItem, TSH_EqpType, g_szEqpTypeName[IN_pEquipment->Get_EquipmentType()]);
	SetItemText(nItem, TSH_EqpType, g_szEqpTypeName_UI[IN_pEquipment->Get_EqpType_UI()]);

	// Equipment id
	SetItemText(nItem, TSH_EqpId, IN_pEquipment->Get_EquipmentId());

	// Equipment IP Address
	szText = ConvIPAddrToString(IN_pEquipment->Get_IP_Address());
	SetItemText(nItem, TSH_IpAddr, szText.GetBuffer());

}

//=============================================================================
// Method		: Insert_Equipment
// Access		: protected  
// Returns		: void
// Parameter	: __in int nItem
// Parameter	: __in const CConfig_Eqp * IN_pEquipment
// Qualifier	:
// Last Update	: 2021/11/2 - 14:29
// Desc.		:
//=============================================================================
void CList_LineConfig::Insert_Equipment(__in int nItem, __in const CConfig_Eqp* IN_pEquipment)
{
	ASSERT(GetSafeHwnd());
	if (NULL == IN_pEquipment)
		return;

	if (GetItemCount() <= nItem)
		return;

	int iNewCount = nItem;

	InsertItem(iNewCount, _T(""));

	Set_EquipmentData(iNewCount, IN_pEquipment);

	// 번호 재부여
	Reset_Number();

	// 화면에 보이게 하기
	EnsureVisible(iNewCount, TRUE);
	ListView_SetItemState(GetSafeHwnd(), iNewCount, LVIS_FOCUSED | LVIS_SELECTED, 0x000F);
}

//=============================================================================
// Method		: Add_Equipment
// Access		: protected  
// Returns		: void
// Parameter	: const __in CConfig_Eqp * IN_pEquipment
// Qualifier	:
// Last Update	: 2021/11/2 - 14:29
// Desc.		:
//=============================================================================
void CList_LineConfig::Add_Equipment(const __in CConfig_Eqp* IN_pEquipment)
{
	ASSERT(GetSafeHwnd());
	if (NULL == IN_pEquipment)
		return;

	int iNewCount = GetItemCount();

	InsertItem(iNewCount, _T(""));

	Set_EquipmentData(iNewCount, IN_pEquipment);

	// 화면에 보이게 하기
	EnsureVisible(iNewCount, TRUE);
	ListView_SetItemState(GetSafeHwnd(), iNewCount, LVIS_FOCUSED | LVIS_SELECTED, 0x000F);
}
//=============================================================================
// Method		: Modify_Equipment
// Access		: protected  
// Returns		: void
// Parameter	: __in int nItem
// Parameter	: __in const CConfig_Eqp * IN_pEquipment
// Qualifier	:
// Last Update	: 2021/11/2 - 14:30
// Desc.		:
//=============================================================================
void CList_LineConfig::Modify_Equipment(__in int nItem, __in const CConfig_Eqp* IN_pEquipment)
{
	ASSERT(GetSafeHwnd());
	if (NULL == IN_pEquipment)
		return;

	if (GetItemCount() <= nItem)
		return;

	Set_EquipmentData(nItem, IN_pEquipment);

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
// Last Update	: 2022/7/19 - 17:45
// Desc.		:
//=============================================================================
void CList_LineConfig::OnLanguage(__in uint8_t IN_nLanguage)
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
// Method		: Set_LineInfo
// Access		: public  
// Returns		: void
// Parameter	: __in const CConfig_Line * IN_pLineInfo
// Qualifier	:
// Last Update	: 2021/11/2 - 14:30
// Desc.		:
//=============================================================================
void CList_LineConfig::Set_LineInfo(__in const CConfig_Line* IN_pLineInfo)
{
	m_stLine_Info.RemoveAll();
	DeleteAllItems();

	if (NULL != IN_pLineInfo)
	{
		m_stLine_Info = *IN_pLineInfo;

		for (UINT nIdx = 0; nIdx < IN_pLineInfo->GetCount(); nIdx++)
		{
			Add_Equipment(&(IN_pLineInfo->GetAt(nIdx)));
		}
	}
}

//=============================================================================
// Method		: Get_LineInfo
// Access		: public  
// Returns		: void
// Parameter	: __out CConfig_Line & OUT_stLineInfo
// Qualifier	:
// Last Update	: 2021/11/2 - 14:30
// Desc.		:
//=============================================================================
void CList_LineConfig::Get_LineInfo(__out CConfig_Line& OUT_stLineInfo)
{
	OUT_stLineInfo.RemoveAll();
	OUT_stLineInfo = m_stLine_Info;
}

//=============================================================================
// Method		: Item_Add
// Access		: public  
// Returns		: void
// Parameter	: __in CConfig_Eqp & IN_Equipment
// Qualifier	:
// Last Update	: 2021/11/2 - 14:30
// Desc.		:
//=============================================================================
void CList_LineConfig::Item_Add(__in CConfig_Eqp& IN_Equipment)
{
	POSITION posSel = GetFirstSelectedItemPosition();

	// 데이터 정상인가 확인
	if (m_stLine_Info.GetCount() != GetItemCount())
	{
		// 에러
		//LT_MessageBox(_T("CList_LineConfig::Item_Add() Data Count Error"));
		LT_MessageBox(g_szMessageBox_T[MB_Line_DataCount_Error][m_nLanguage]);
		return;
	}

	m_stLine_Info.Eqp_Add(IN_Equipment);
	Add_Equipment(&IN_Equipment);

	// UI 갱신
	//Check_EqpType_UpdateAlias(IN_Equipment.Get_EquipmentType());
	Check_EqpTypeUI_UpdateAlias(IN_Equipment.Get_EqpType_UI());
	Set_Group_Equipment();
}

//=============================================================================
// Method		: Item_Insert
// Access		: public  
// Returns		: void
// Parameter	: __in CConfig_Eqp & IN_Equipment
// Qualifier	:
// Last Update	: 2021/11/2 - 14:30
// Desc.		:
//=============================================================================
void CList_LineConfig::Item_Insert(__in CConfig_Eqp& IN_Equipment)
{
	if (0 < GetSelectedCount())
	{
		POSITION nPos = GetFirstSelectedItemPosition();
		int iIndex = GetNextSelectedItem(nPos);

		m_stLine_Info.Eqp_Insert(iIndex, IN_Equipment);

		Insert_Equipment(iIndex, &IN_Equipment);

		// UI 갱신
		//Check_EqpType_UpdateAlias(IN_Equipment.Get_EquipmentType());
		Check_EqpTypeUI_UpdateAlias(IN_Equipment.Get_EqpType_UI());
		Set_Group_Equipment();
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
// Last Update	: 2021/11/2 - 14:31
// Desc.		:
//=============================================================================
void CList_LineConfig::Item_Remove()
{
	if (0 < GetSelectedCount())
	{
		POSITION nPos = GetFirstSelectedItemPosition();
		int iSelIdx = GetNextSelectedItem(nPos);

		DeleteItem(iSelIdx);
		Reset_Number();


		// Equipment Alias 체크
		//UINT stSelEqpType = m_stLine_Info.GetAt(iSelIdx).Get_EquipmentType();
		UINT stSelEqpTypeUI = m_stLine_Info.GetAt(iSelIdx).Get_EqpType_UI();


		m_stLine_Info.Eqp_Remove(iSelIdx);


		// UI 갱신
		//Check_EqpType_UpdateAlias(stSelEqpType);
		Check_EqpTypeUI_UpdateAlias(stSelEqpTypeUI);
		Set_Group_Equipment();

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
// Last Update	: 2021/11/2 - 14:31
// Desc.		:
//=============================================================================
void CList_LineConfig::Item_Up()
{
	if (0 < GetSelectedCount())
	{
		POSITION nPos = GetFirstSelectedItemPosition();
		int iIndex = GetNextSelectedItem(nPos);

		// 0번 인덱스는 위로 이동 불가
		if ((0 < iIndex) && (1 < GetItemCount()))
		{
			CConfig_Eqp stStep = m_stLine_Info.GetAt(iIndex);

			DeleteItem(iIndex);
			Insert_Equipment(iIndex - 1, &stStep);

			// Equipment Alias 체크
			//UINT stSelEqpType = m_stLine_Info.GetAt(iIndex).Get_EquipmentType();
			UINT stSelEqpTypeUI = m_stLine_Info.GetAt(iIndex).Get_EqpType_UI();

			m_stLine_Info.Eqp_Remove(iIndex);
			m_stLine_Info.Eqp_Insert(iIndex - 1, stStep);


			// UI 갱신
			//Check_EqpType_UpdateAlias(stSelEqpType);
			Check_EqpTypeUI_UpdateAlias(stSelEqpTypeUI);
			Set_Group_Equipment();

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
// Last Update	: 2021/11/2 - 14:31
// Desc.		:
//=============================================================================
void CList_LineConfig::Item_Down()
{
	if (0 < GetSelectedCount())
	{
		POSITION nPos = GetFirstSelectedItemPosition();
		int iIndex = GetNextSelectedItem(nPos);

		// 마지막 인덱스는 아래로 이동 불가
		if ((iIndex < (GetItemCount() - 1)) && (1 < GetItemCount()))
		{
			CConfig_Eqp stStep = m_stLine_Info.GetAt(iIndex);

			DeleteItem(iIndex);

			// Equipment Alias 체크
			//UINT stSelEqpType = m_stLine_Info.GetAt(iIndex).Get_EquipmentType();
			UINT stSelEqpTypeUI = m_stLine_Info.GetAt(iIndex).Get_EqpType_UI();

			m_stLine_Info.Eqp_Remove(iIndex);

			// 변경되는 위치가 최하단이면, Insert 대신 Add 사용
			if ((iIndex + 1) < (GetItemCount()))
			{
				Insert_Equipment(iIndex + 1, &stStep);
				m_stLine_Info.Eqp_Insert(iIndex + 1, stStep);
			}
			else
			{
				Add_Equipment(&stStep);
				m_stLine_Info.Eqp_Add(stStep);
			}

			// UI 갱신
			//Check_EqpType_UpdateAlias(stSelEqpType);
			Check_EqpTypeUI_UpdateAlias(stSelEqpTypeUI);
			Set_Group_Equipment();

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
// Parameter	: __in CConfig_Eqp & IN_Equipment
// Qualifier	:
// Last Update	: 2021/11/2 - 14:31
// Desc.		:
//=============================================================================
void CList_LineConfig::Item_Modify(__in CConfig_Eqp& IN_Equipment)
{
	if (0 < GetSelectedCount())
	{
		POSITION nPos = GetFirstSelectedItemPosition();
		int iIndex = GetNextSelectedItem(nPos);

		// 수정
		m_stLine_Info.Eqp_Modify(iIndex, IN_Equipment);
		// 삭제, 추가
		Modify_Equipment(iIndex, &IN_Equipment);

		// UI 갱신
		//Check_EqpType_UpdateAlias();
		Check_EqpTypeUI_UpdateAlias();

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
// Method		: Get_SelectedEquipment
// Access		: public  
// Returns		: bool
// Parameter	: __out CConfig_Eqp & OUT_Equipment
// Qualifier	:
// Last Update	: 2021/11/2 - 14:31
// Desc.		:
//=============================================================================
bool CList_LineConfig::Get_SelectedEquipment(__out CConfig_Eqp& OUT_Equipment)
{
	if (0 < GetSelectedCount())
	{
		POSITION nPos = GetFirstSelectedItemPosition();
		int iIndex = GetNextSelectedItem(nPos);

		OUT_Equipment = m_stLine_Info.GetAt(iIndex);

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
// Parameter	: __in const CConfig_Line * IN_pLineInfo
// Qualifier	:
// Last Update	: 2021/11/2 - 14:31
// Desc.		:
//=============================================================================
void CList_LineConfig::Add_LineInfo(__in const CConfig_Line* IN_pLineInfo)
{
	if (NULL != IN_pLineInfo)
	{
		m_stLine_Info = *IN_pLineInfo;

		for (UINT nIdx = 0; nIdx < IN_pLineInfo->GetCount(); nIdx++)
		{
			Add_Equipment(&(IN_pLineInfo->GetAt(nIdx)));
		}
	}
}

//=============================================================================
// Method		: Insert_LineInfo
// Access		: public  
// Returns		: void
// Parameter	: __in const CConfig_Line * IN_pLineInfo
// Qualifier	:
// Last Update	: 2021/11/2 - 14:31
// Desc.		:
//=============================================================================
void CList_LineConfig::Insert_LineInfo(__in const CConfig_Line* IN_pLineInfo)
{

	if (0 < GetSelectedCount())
	{
		POSITION nPos = GetFirstSelectedItemPosition();
		int iIndex = GetNextSelectedItem(nPos);

		for (UINT nIdx = 0; nIdx < IN_pLineInfo->GetCount(); nIdx++)
		{
			m_stLine_Info.Eqp_Insert(iIndex, IN_pLineInfo->GetAt(nIdx));

			Insert_Equipment(iIndex, &(IN_pLineInfo->GetAt(nIdx)));
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
void CList_LineConfig::Clear_LineInfo()
{
	m_stLine_Info.RemoveAll();
	DeleteAllItems();
}

//=============================================================================
// Method		: Get_EquipmentCount
// Access		: public  
// Returns		: UINT
// Parameter	: __in UINT nEquipmentType
// Qualifier	:
// Last Update	: 2021/11/9 - 14:41
// Desc.		:
//=============================================================================
UINT CList_LineConfig::Get_EquipmentCount(__in UINT nEquipmentType)
{
	return m_stLine_Info.Get_EquipmentCount(nEquipmentType);
}

//=============================================================================
// Method		: Check_EquipmentType
// Access		: public  
// Returns		: bool
// Parameter	: __in UINT nEquipmentType
// Qualifier	:
// Last Update	: 2021/11/9 - 17:23
// Desc.		:
//=============================================================================
// void CList_LineConfig::Check_EqpType_UpdateAlias(__in UINT nEquipmentType, __in bool bUpdateListCtrl /*= true*/)
// {
// 	bool bReturn = true;
// 
// 	// 해당 설비 타입을 검색해서 Alias를 재 설정
// 
// 	UINT nCount = 0;
// 	CString szValue;
// 
// 	for (auto nIdx = 0; nIdx < m_stLine_Info.EqpList.size(); ++nIdx)
// 	{
// 		// 설비 순서 설정
// 		m_stLine_Info.EqpList.at(nIdx).Set_EqpOrder(nIdx);
// 
// 		if (nEquipmentType == m_stLine_Info.EqpList.at(nIdx).Get_EquipmentType())
// 		{
// 			++nCount;
// 
// 			szValue.Format(_T("%s #%02d"), g_szEqpTypeName[nEquipmentType], nCount);
// 
// 			m_stLine_Info.EqpList.at(nIdx).Set_Alias(szValue);
// 
// 			// 검사 그룹내 설비 인덱스
// 			m_stLine_Info.EqpList.at(nIdx).Set_GroupIndex (nCount - 1);
// 		}
// 	}
// 
// 	// UI상의 Alias 갱신 
// 	if (bUpdateListCtrl)
// 	{
// 		if (0 < nCount)
// 		{
// 			for (auto nIdx = 0; nIdx < GetItemCount(); ++nIdx)
// 			{
// 				SetItemText(nIdx, TSH_ItemAlias, m_stLine_Info.GetAt(nIdx).Get_Alias().GetBuffer());
// 			}
// 		}
// 	}
// }

//=============================================================================
// Method		: Check_EqpType_UpdateAlias
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/11/10 - 11:33
// Desc.		:
//=============================================================================
// void CList_LineConfig::Check_EqpType_UpdateAlias()
// {
// 	// 모든 설비 타입을 검색해서 Alias를 재 설정
// 	for (auto nIdx = 0; nIdx < Max_EqpTypeCount; ++nIdx)
// 	{
// 		Check_EqpType_UpdateAlias(nIdx, false);
// 	}
// 	Set_Group_Equipment();
// 
// 	for (auto nIdx = 0; nIdx < GetItemCount(); ++nIdx)
// 	{
// 		SetItemText(nIdx, TSH_ItemAlias, m_stLine_Info.GetAt(nIdx).Get_Alias().GetBuffer());
// 	}
// }

//=============================================================================
// Method		: Check_EqpTypeUI_UpdateAlias
// Access		: public  
// Returns		: void
// Parameter	: __in UINT nEqpTypeUI
// Parameter	: __in bool bUpdateListCtrl
// Qualifier	:
// Last Update	: 2022/10/21 - 13:46
// Desc.		:
//=============================================================================
void CList_LineConfig::Check_EqpTypeUI_UpdateAlias(__in UINT nEqpTypeUI, __in bool bUpdateListCtrl /*= true*/)
{
	bool bReturn = true;

	// 해당 설비 타입을 검색해서 Alias를 재 설정

	UINT nCount = 0;
	CString szValue;

	for (auto nIdx = 0; nIdx < m_stLine_Info.EqpList.size(); ++nIdx)
	{
		// 설비 순서 설정
		m_stLine_Info.EqpList.at(nIdx).Set_EqpOrder(nIdx);

		if (nEqpTypeUI == m_stLine_Info.EqpList.at(nIdx).Get_EqpType_UI())
		{
			++nCount;

			szValue.Format(_T("%s #%02d"), g_szEqpTypeName_UI[nEqpTypeUI], nCount);

			m_stLine_Info.EqpList.at(nIdx).Set_Alias(szValue);

			// 검사 그룹내 설비 인덱스
			m_stLine_Info.EqpList.at(nIdx).Set_GroupIndex(nCount - 1);
		}
	}

	// UI상의 Alias 갱신 
	if (bUpdateListCtrl)
	{
		if (0 < nCount)
		{
			for (auto nIdx = 0; nIdx < GetItemCount(); ++nIdx)
			{
				SetItemText(nIdx, TSH_ItemAlias, m_stLine_Info.GetAt(nIdx).Get_Alias().GetBuffer());
			}
		}
	}
}

void CList_LineConfig::Check_EqpTypeUI_UpdateAlias()
{
	// 모든 설비 타입을 검색해서 Alias를 재 설정
	for (auto nIdx = 0; nIdx < Max_EqpTypeUICount; ++nIdx)
	{
		Check_EqpTypeUI_UpdateAlias(nIdx, false);
	}
	Set_Group_Equipment();

	for (auto nIdx = 0; nIdx < GetItemCount(); ++nIdx)
	{
		SetItemText(nIdx, TSH_ItemAlias, m_stLine_Info.GetAt(nIdx).Get_Alias().GetBuffer());
	}
}

//=============================================================================
// Method		: Check_EquipmentID
// Access		: public  
// Returns		: bool
// Qualifier	:
// Last Update	: 2021/11/10 - 17:58
// Desc.		: 리스트에서 설비 ID 중복 체크
//=============================================================================
bool CList_LineConfig::Check_EquipmentID()
{
	for (auto nIdx = 0; nIdx < m_stLine_Info.GetCount(); ++nIdx)
	{
		for (auto nComp = nIdx + 1; nComp < m_stLine_Info.GetCount(); ++nComp)
		{
			if (0 == m_stLine_Info.GetAt(nIdx).Get_EquipmentId().Compare(m_stLine_Info.GetAt(nComp).Get_EquipmentId()))
			{
				return false;
			}
		}
	}

	return true;
}

bool CList_LineConfig::Check_EquipmentID(__in LPCTSTR IN_szEquipmentID)
{
	for (auto nIdx = 0; nIdx < m_stLine_Info.GetCount(); ++nIdx)
	{
		if (0 == m_stLine_Info.GetAt(nIdx).Get_EquipmentId().Compare(IN_szEquipmentID))
		{
			return false;
		}
	}

	return true;
}

//=============================================================================
// Method		: Check_IPAddress
// Access		: public  
// Returns		: bool
// Qualifier	:
// Last Update	: 2021/11/10 - 17:59
// Desc.		: 리스트에서 설비 IP Address 중복 체크
//=============================================================================
bool CList_LineConfig::Check_IPAddress()
{
	for (auto nIdx = 0; nIdx < m_stLine_Info.GetCount(); ++nIdx)
	{
		for (auto nComp = nIdx + 1; nComp < m_stLine_Info.GetCount(); ++nComp)
		{
			if (m_stLine_Info.GetAt(nIdx).Get_IP_Address() == m_stLine_Info.GetAt(nComp).Get_IP_Address())
			{
				return false;
			}
		}
	}

	return true;
}

bool CList_LineConfig::Check_IPAddress(__in DWORD IN_nIPAddress)
{
	for (auto nIdx = 0; nIdx < m_stLine_Info.GetCount(); ++nIdx)
	{
		if (m_stLine_Info.GetAt(nIdx).Get_IP_Address() == IN_nIPAddress)
		{
			return false;
		}
	}

	return true;
}

//=============================================================================
// Method		: Set_Group_Equipment
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2022/1/28 - 15:50
// Desc.		: 설비 중 같은 검사 그룹 체크
//=============================================================================
void CList_LineConfig::Set_Group_Equipment()
{
	// 설비 타입
	uint8_t nGoupCount = 0;
	uint8_t nGoupIndex = 0;

	// 그룹 정보 초기화
	for (auto nIdx = 0; nIdx < m_stLine_Info.GetCount(); ++nIdx)
	{
		m_stLine_Info.GetAt(nIdx).Set_Group(false);
		m_stLine_Info.GetAt(nIdx).Set_GroupIndex(0);
	}

	for (uint8_t nEqpType = Eqp_Tester_First; nEqpType <= Eqp_Tester_Last; ++nEqpType)
	{
		nGoupCount = 0;

		// 그룹인가?
		for (auto nIdx = 0; nIdx < m_stLine_Info.GetCount(); ++nIdx)
		{
			if (nEqpType == m_stLine_Info.GetAt(nIdx).Get_EquipmentType())
			{
				++nGoupCount;
			}
		}

		// 그룹이면....
		if (1 < nGoupCount)
		{
			nGoupIndex = 0;
			for (auto nIdx = 0; nIdx < m_stLine_Info.GetCount(); ++nIdx)
			{
				if (nEqpType == m_stLine_Info.GetAt(nIdx).Get_EquipmentType())
				{
					m_stLine_Info.GetAt(nIdx).Set_Group(true);
					m_stLine_Info.GetAt(nIdx).Set_GroupIndex(nGoupIndex++);
				}
			}
		}
	}
}

#if defined(EES_XML)//20231003
void CList_LineConfig::Set_ServerData(__in int nItem, __in const CConfig_Eqp* IN_pEquipment)
{
	ASSERT(GetSafeHwnd());
	if (NULL == IN_pEquipment)
		return;
	if (GetItemCount() <= nItem)
		return;
	CString szText;
	// TSH_No
	szText.Format(_T("%d"), nItem + 1);
	SetItemText(nItem, TSH_No, szText);
	// Alias (Eqp Type + Number)
	SetItemText(nItem, TSH_ItemAlias, IN_pEquipment->Get_Alias());
	// Equipmnet Type
	//SetItemText(nItem, TSH_EqpType, g_szEqpTypeName[IN_pEquipment->Get_EquipmentType()]);
	SetItemText(nItem, TSH_EqpType, g_szEqpTypeName_UI[IN_pEquipment->Get_EqpType_UI()]);
	// Equipment id
	SetItemText(nItem, TSH_EqpId, IN_pEquipment->Get_EquipmentId());
	// Equipment IP Address
	szText = ConvIPAddrToString(IN_pEquipment->Get_IP_Address());
	SetItemText(nItem, TSH_IpAddr, szText.GetBuffer());
}
void CList_LineConfig::Insert_Server(__in int nItem, __in const CConfig_Eqp* IN_pEquipment)
{
	ASSERT(GetSafeHwnd());
	if (NULL == IN_pEquipment)
		return;
	if (GetItemCount() <= nItem)
		return;
	int iNewCount = nItem;
	InsertItem(iNewCount, _T(""));
	Set_ServerData(iNewCount, IN_pEquipment);
	// 번호 재부여
	Reset_Number();
	// 화면에 보이게 하기
	EnsureVisible(iNewCount, TRUE);
	ListView_SetItemState(GetSafeHwnd(), iNewCount, LVIS_FOCUSED | LVIS_SELECTED, 0x000F);
}
void CList_LineConfig::Add_Server(const __in CConfig_Eqp* IN_pEquipment)
{
	ASSERT(GetSafeHwnd());
	if (NULL == IN_pEquipment)
		return;
	int iNewCount = GetItemCount();
	InsertItem(iNewCount, _T(""));
	Set_ServerData(iNewCount, IN_pEquipment);
	// 화면에 보이게 하기
	EnsureVisible(iNewCount, TRUE);
	ListView_SetItemState(GetSafeHwnd(), iNewCount, LVIS_FOCUSED | LVIS_SELECTED, 0x000F);
}
void CList_LineConfig::Modify_Server(__in int nItem, __in const CConfig_Eqp* IN_pEquipment)
{
	ASSERT(GetSafeHwnd());
	if (NULL == IN_pEquipment)
		return;
	if (GetItemCount() <= nItem)
		return;
	Set_ServerData(nItem, IN_pEquipment);
	// 화면에 보이게 하기
	EnsureVisible(nItem, TRUE);
	ListView_SetItemState(GetSafeHwnd(), nItem, LVIS_FOCUSED | LVIS_SELECTED, 0x000F);
}
#endif