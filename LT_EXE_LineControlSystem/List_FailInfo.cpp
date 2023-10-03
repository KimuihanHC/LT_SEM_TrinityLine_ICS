//*****************************************************************************
// Filename	: 	List_FailInfo.cpp
// Created	:	2021/11/1 - 15:16
// Modified	:	2021/11/1 - 15:16
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#include "stdafx.h"
#include "List_FailInfo.h"
#include "Def_WindowMessage.h"
#include "Def_Language.h"

typedef enum 
{
	TSH_NG_Code,		// NG Code
	TSH_Count_All,		// Count All
	TSH_Count_Left,		// Count Left
	TSH_Count_Center,	// Count Center
	TSH_Count_Right,	// Count Right
	
	TSH_MaxCol,
}enTestStepHeader;

// 헤더
static const TCHAR* g_lpszHeader_T[Lang_MaxCount][TSH_MaxCol] =
{
	// 한국어
	{
		_T("코드"),		// TSH_NG_Code,	
		_T("전체"),		// TSH_Count_All,	
		_T("L"),		// TSH_Count_Left,
		_T("C"),		// TSH_Count_Center
		_T("R"),		// TSH_Count_Right
	},

	// 영어
	{
		_T("Code"),		// TSH_NG_Code,	
		_T("All"),		// TSH_Count_All,	
		_T("L"),		// TSH_Count_Left,
		_T("C"),		// TSH_Count_Center
		_T("R"),		// TSH_Count_Right
	},

	// 베트남어
	{
		_T("Code"),		// TSH_NG_Code,	
		_T("All"),		// TSH_Count_All,	
		_T("L"),		// TSH_Count_Left,
		_T("C"),		// TSH_Count_Center
		_T("R"),		// TSH_Count_Right
	},

	// 중국어
	{
		_T("代码"),		// TSH_NG_Code,	
		_T("全体"),		// TSH_Count_All,	
		_T("左"),		// TSH_Count_Left,
		_T("中"),		// TSH_Count_Center
		_T("右"),		// TSH_Count_Right
	},
};

const int	iListAglin[] =
{
	LVCFMT_CENTER,	 // TSH_NG_Code,	
	LVCFMT_CENTER,	 // TSH_Count_All,	
	LVCFMT_CENTER,	 // TSH_Count_Left,
	LVCFMT_CENTER,	 // TSH_Count_Center
	LVCFMT_CENTER,	 // TSH_Count_Right
};

// 열의 폭
const int	iHeaderWidth[] =
{
	 38,	// TSH_NG_Code,	
	 44,	// TSH_Count_All,	
	 44,	// TSH_Count_Left,
	  0,	// TSH_Count_Center
	 44,	// TSH_Count_Right
};

const int	iHeaderWidth_Over10[] =
{
	 42,	// TSH_NG_Code,	
	 42,	// TSH_Count_All,	
	 42,	// TSH_Count_Left,
	  0,	// TSH_Count_Center
	 42,	// TSH_Count_Right
};

const int	iHeaderWidth_3Para[] =
{
	 38,	// TSH_NG_Code,	
	 44,	// TSH_Count_All,	
	 36,	// TSH_Count_Left,
	 36,	// TSH_Count_Center
	 36,	// TSH_Count_Right
};

// 3 para에 전체 검사 설비 수가 10개 초과되는 경우
const int	iHeaderWidth_3Para_Over10[] =
{
	 34,	// TSH_NG_Code,	
	 35,	// TSH_Count_All,	
	 33,	// TSH_Count_Left,
	 33,	// TSH_Count_Center
	 33,	// TSH_Count_Right
};

const int	iHeaderWidth_1Para[] =
{
	 55,	// TSH_NG_Code,	
	 55,	// TSH_Count_All,	
	 55,	// TSH_Count_Left,
	 0,		// TSH_Count_Center
	 0,		// TSH_Count_Right
};

IMPLEMENT_DYNAMIC(CList_FailInfo, CListCtrl)

CList_FailInfo::CList_FailInfo()
{
	//m_Font.CreateStockObject(DEFAULT_GUI_FONT);
	VERIFY(m_Font.CreateFont(
	 	12,						// nHeight
	 	0,						// nWidth
	 	0,						// nEscapement
	 	0,						// nOrientation
		FW_NORMAL,				// nWeight
	 	FALSE,					// bItalic
	 	FALSE,					// bUnderline
	 	0,						// cStrikeOut
	 	ANSI_CHARSET,			// nCharSet
	 	OUT_DEFAULT_PRECIS,		// nOutPrecision
	 	CLIP_DEFAULT_PRECIS,	// nClipPrecision
	 	ANTIALIASED_QUALITY,	// nQuality
	 	FIXED_PITCH,			// nPitchAndFamily
	 	_T("Arial")));		// lpszFacename

	//Set_ParaCount(m_nParaCount);
	m_nParaCount = 2;
	m_pHeadWidth = iHeaderWidth;
}

CList_FailInfo::~CList_FailInfo()
{
	m_Font.DeleteObject();
}


BEGIN_MESSAGE_MAP(CList_FailInfo, CListCtrl)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_NOTIFY_REFLECT(NM_CLICK,	&CList_FailInfo::OnNMClick)
	ON_WM_MOUSEWHEEL()
	ON_MESSAGE(WM_NGCODE_NOTIFY, OnWM_NotifyNGCode)
END_MESSAGE_MAP()



// CList_FailInfo message handlers
//=============================================================================
// Method		: OnCreate
// Access		: protected  
// Returns		: int
// Parameter	: LPCREATESTRUCT lpCreateStruct
// Qualifier	:
// Last Update	: 2021/11/2 - 14:28
// Desc.		:
//=============================================================================
int CList_FailInfo::OnCreate(LPCREATESTRUCT lpCreateStruct)
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
void CList_FailInfo::OnSize(UINT nType, int cx, int cy)
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
BOOL CList_FailInfo::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL | WS_BORDER | WS_TABSTOP;
	cs.dwExStyle &= LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;

	return CListCtrl::PreCreateWindow(cs);
}

//=============================================================================
// Method		: OnWM_NotifyNGCode
// Access		: protected  
// Returns		: LRESULT
// Parameter	: WPARAM wParam
// Parameter	: LPARAM lParam
// Qualifier	:
// Last Update	: 2022/7/22 - 17:41
// Desc.		:
//=============================================================================
LRESULT CList_FailInfo::OnWM_NotifyNGCode(WPARAM wParam, LPARAM lParam)
{
	uint8_t IN_nEqpOrder = (uint8_t)wParam;
	int16_t IN_nNGCode = (int16_t)lParam;

	if (0 != IN_nNGCode)
	{
		Update_NGCount(IN_nNGCode);
	}
	else
	{
		Refresh_NGCount();
	}

	return 1;
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
void CList_FailInfo::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
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
void CList_FailInfo::InitHeader()
{
	if (FALSE == m_bIntiHeader)
	{
		m_bIntiHeader = TRUE;

		for (int nCol = 0; nCol < TSH_MaxCol; nCol++)
		{
			InsertColumn(nCol, g_lpszHeader_T[m_nLanguage][nCol], iListAglin[nCol], m_pHeadWidth[nCol]);
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
int CList_FailInfo::Find_Row(__in int16_t IN_nNGCode)
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
// Method		: Select_SelectItem
// Access		: protected  
// Returns		: void
// Parameter	: __in int nItem
// Qualifier	:
// Last Update	: 2021/11/2 - 14:29
// Desc.		:
//=============================================================================
void CList_FailInfo::Select_SelectItem(__in int nItem)
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
// Method		: Set_NGCountData
// Access		: protected  
// Returns		: void
// Parameter	: __in int nItem
// Parameter	: __in const ST_NGCode_Count * IN_pNGCount
// Qualifier	:
// Last Update	: 2021/11/2 - 14:29
// Desc.		:
//=============================================================================
void CList_FailInfo::Set_NGCountData(__in int nItem, __in const ST_NGCode_Count* IN_pNGCount)
{
	ASSERT(GetSafeHwnd());
	if (NULL == IN_pNGCount)
		return;

 	if (GetItemCount() <= nItem)
 		return;

	CString szText;

	// TSH_NG_Code,		// NG Code
	szText.Format(_T("%d"), IN_pNGCount->m_nNGCode);
	SetItemText(nItem, TSH_NG_Code, szText);

	// TSH_Count_All,	// Count All
	szText.Format(_T("%d"), IN_pNGCount->m_nCount_All);
	SetItemText(nItem, TSH_Count_All, szText);

	// TSH_Count_Left,	// Count Left
	szText.Format(_T("%d"), IN_pNGCount->m_nCount_Para[Para_Left]);
	SetItemText(nItem, TSH_Count_Left, szText);

	// TSH_Count_Center,	// Count Center
	szText.Format(_T("%d"), IN_pNGCount->m_nCount_Para[Para_Center]);
	SetItemText(nItem, TSH_Count_Center, szText);

	// TSH_Count_Right,	// Count Right
	szText.Format(_T("%d"), IN_pNGCount->m_nCount_Para[Para_Right]);
	SetItemText(nItem, TSH_Count_Right, szText);
}

//=============================================================================
// Method		: Add_NGCount
// Access		: protected  
// Returns		: void
// Parameter	: const __in ST_NGCode_Count * IN_pNGCount
// Qualifier	:
// Last Update	: 2021/11/2 - 14:29
// Desc.		:
//=============================================================================
void CList_FailInfo::Add_NGCount(const __in ST_NGCode_Count* IN_pNGCount)
{
	ASSERT(GetSafeHwnd());
	if (NULL == IN_pNGCount)
		return;

	int iNewCount = GetItemCount();

	// CListCtrl
	InsertItem(iNewCount, _T(""));

	// CListCtrl
	//AddItem(_T(""));
	//InsertItem(iNewCount, _T(""));

	Set_NGCountData(iNewCount, IN_pNGCount);

	// 화면에 보이게 하기
	EnsureVisible(iNewCount, TRUE);
	ListView_SetItemState(GetSafeHwnd(), iNewCount, LVIS_FOCUSED | LVIS_SELECTED, 0x000F);
}

//=============================================================================
// Method		: OnLanguage
// Access		: public  
// Returns		: void
// Parameter	: __in uint8_t IN_nLanguage
// Qualifier	:
// Last Update	: 2022/7/20 - 14:40
// Desc.		:
//=============================================================================
void CList_FailInfo::OnLanguage(__in uint8_t IN_nLanguage)
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
// Method		: Set_NGCountList
// Access		: public  
// Returns		: void
// Parameter	: __in const CNGCountList * IN_pFailInfo_Eqp
// Qualifier	:
// Last Update	: 2021/11/2 - 14:30
// Desc.		:
//=============================================================================
void CList_FailInfo::Set_NGCountList(__in const CFailInfo_Eqp* IN_pFailInfo_Eqp)
{
	//m_stNGCountList.Clear_NGCode();
	DeleteAllItems();

	if (NULL != IN_pFailInfo_Eqp)
	{
		m_pNGCountList = IN_pFailInfo_Eqp;

		auto eIter = IN_pFailInfo_Eqp->m_NG_Count.end();
		auto Iter  = IN_pFailInfo_Eqp->m_NG_Count.begin();
		while (Iter != eIter)
		{
			Add_NGCount(&Iter->second);

			Iter++;
		}
	}
}

//=============================================================================
// Method		: Clear_NGCountList
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/11/2 - 14:31
// Desc.		:
//=============================================================================
void CList_FailInfo::Clear_NGCountList()
{
	//m_stNGCountList.Clear_NGCode();
	DeleteAllItems();
}

//=============================================================================
// Method		: Refresh_NGCount
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/11/17 - 15:50
// Desc.		:
//=============================================================================
void CList_FailInfo::Refresh_NGCount()
{
	DeleteAllItems();

	auto eIter = m_pNGCountList->m_NG_Count.end();
	auto Iter = m_pNGCountList->m_NG_Count.begin();
	while (Iter != eIter)
	{
		Add_NGCount(&Iter->second);

		Iter++;
	}
}

//=============================================================================
// Method		: Update_NGCount
// Access		: protected  
// Returns		: void
// Parameter	: __in int nRow
// Parameter	: __in const ST_NGCode_Count * IN_pNGCount
// Qualifier	:
// Last Update	: 2021/11/2 - 14:30
// Desc.		:
//=============================================================================
void CList_FailInfo::Update_NGCount(__in int nRow, __in const ST_NGCode_Count* IN_pNGCount)
{
	ASSERT(GetSafeHwnd());
	if (NULL == IN_pNGCount)
		return;

	if (GetItemCount() <= nRow)
		return;

	Set_NGCountData(nRow, IN_pNGCount);

	// 화면에 보이게 하기
	//EnsureVisible(nRow, TRUE);
	//ListView_SetItemState(GetSafeHwnd(), nRow, LVIS_FOCUSED | LVIS_SELECTED, 0x000F);
}

void CList_FailInfo::Update_NGCount(__in int16_t IN_nNGCode, __in const CFailInfo_Eqp* IN_pFailInfo_Eqp)
{
	int nRow = Find_Row(IN_nNGCode);

	if (0 <= nRow)
	{
		if (IN_pFailInfo_Eqp->IsExist(IN_nNGCode))
		{
			Update_NGCount(nRow, &IN_pFailInfo_Eqp->Get_NGCount(IN_nNGCode));
		}
	}
	else
	{
		// 없으면 추가해야 한다.
		if (IN_pFailInfo_Eqp->IsExist(IN_nNGCode))
		{
			Add_NGCount(&IN_pFailInfo_Eqp->Get_NGCount(IN_nNGCode));
		}
	}
}

void CList_FailInfo::Update_NGCount(int16_t IN_nNGCode)
{
	if (m_pNGCountList)
	{
		int nRow = Find_Row(IN_nNGCode);

		if (0 <= nRow)
		{
			if (m_pNGCountList->IsExist(IN_nNGCode))
			{
				Update_NGCount(nRow, &m_pNGCountList->Get_NGCount(IN_nNGCode));
			}
		}
		else
		{
			// 없으면 추가해야 한다.
			if (m_pNGCountList->IsExist(IN_nNGCode))
			{
				Add_NGCount(&m_pNGCountList->Get_NGCount(IN_nNGCode));
			}
		}
	}
}

//=============================================================================
// Method		: Set_ParaCount
// Access		: public  
// Returns		: void
// Parameter	: uint8_t IN_nCount
// Parameter	: __in bool IN_bOver10Eqp
// Qualifier	:
// Last Update	: 2023/5/31 - 10:11
// Desc.		:
//=============================================================================
void CList_FailInfo::Set_ParaCount(uint8_t IN_nCount, __in bool IN_bOver10Eqp /*= false*/)
{
	//if (m_nParaCount != IN_nCount)
	{
		m_nParaCount = IN_nCount;

		switch (IN_nCount)
		{
		case 1:
			m_pHeadWidth = iHeaderWidth_1Para;
			break;

		case 2:
			m_pHeadWidth = IN_bOver10Eqp ? iHeaderWidth_Over10 : iHeaderWidth;
			break;

		case 3:
			m_pHeadWidth = IN_bOver10Eqp ? iHeaderWidth_3Para_Over10 : iHeaderWidth_3Para;
			break;

		default:
			m_pHeadWidth = IN_bOver10Eqp ? iHeaderWidth_Over10 : iHeaderWidth;
			break;
		}

		if (GetSafeHwnd())
		{
			InitHeader();
		}
	}
}

//=============================================================================
// Method		: Adjust_ColumnWidth
// Access		: public  
// Returns		: void
// Parameter	: int IN_nWidth
// Qualifier	:
// Last Update	: 2022/7/27 - 18:05
// Desc.		:
//=============================================================================
void CList_FailInfo::Adjust_ColumnWidth(int IN_nWidth)
{
	int iWidth = IN_nWidth - m_pHeadWidth[TSH_NG_Code] - m_pHeadWidth[TSH_Count_All];

	if (2 == m_nParaCount)
	{
		iWidth = iWidth / 2;

		SetColumnWidth(TSH_Count_Center, 0);
	}
	else
	{
		iWidth = iWidth / 3;

		SetColumnWidth(TSH_Count_Center, iWidth);
	}

	SetColumnWidth(TSH_Count_Left, iWidth);
	SetColumnWidth(TSH_Count_Right, iWidth);
}

