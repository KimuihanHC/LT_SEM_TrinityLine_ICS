//*****************************************************************************
// Filename	: 	List_SocketInfo.cpp
// Created	:	2021/11/1 - 15:16
// Modified	:	2021/11/1 - 15:16
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#include "stdafx.h"
#include "List_SocketInfo.h"
#include "Def_Constant.h"
#include "Def_Language.h"

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
	TSH_NG_Code,		// NG Code
	TSH_NG_Para,		// NG Para
	TSH_NG_Eqp,			// NG Equipment
	TSH_Yield_Total,	// Total
	TSH_Yield_Pass,		// Pass 
	TSH_Yield_Fail,		// Fail
#if defined(EES_XML)//202310
	TSH_LOTID,			// LOTID
#endif
	TSH_MaxCol,
}enTestStepHeader;

// 헤더
static const TCHAR* g_lpszHeader_T[Lang_MaxCount][TSH_MaxCol] =
{
	// 한국어
	{
		_T("번호"),					// TSH_No
		_T("RFID"),					// TSH_RFID
		_T("소켓 종류"),				// TSH_SocketType
		_T("설명"),					// TSH_Comment
		_T("바코드"),				// TSH_Barcode
		_T("상태"),					// TSH_Status
		_T("설비"),					// TSH_Equipment
		_T("설비 포트"),				// TSH_Location
		_T("목적 설비"),				// TSH_Target
		_T("NG 코드"),				// TSH_NG_Code
		_T("검사 파라"),				// TSH_NG_Para
		_T("최종 검사 설비"),			// TSH_NG_Eqp
		_T("전체"),					// TSH_Cnt_Total
		_T("양품"),					// TSH_Cnt_Pass
		_T("불량"),					// TSH_Cnt_Fail
#if defined(EES_XML)//202310		
		_T("LOT"),					// TSH_LOTID
#endif		
	},

	// 영어
	{
		_T("No"),					// TSH_No
		_T("RFID"),					// TSH_RFID
		_T("Socket Type"),			// TSH_SocketType
		_T("Comment"),				// TSH_Comment
		_T("Barcode"),				// TSH_Barcode
		_T("Status"),				// TSH_Status
		_T("Equipment"),			// TSH_Equipment
		_T("Port in Eqp."),			// TSH_Location
		_T("Target Eqp."),			// TSH_Target
		_T("NG Code"),				// TSH_NG_Code
		_T("Test Para"),			// TSH_NG_Para
		_T("Test Eqp."),			// TSH_NG_Eqp
		_T("Total"),				// TSH_Cnt_Total
		_T("Pass "),				// TSH_Cnt_Pass
		_T("Fail"),					// TSH_Cnt_Fail
#if defined(EES_XML)//202310		
		_T("LOT"),					// TSH_LOTID
#endif			
	},

	// 베트남어
	{
		_T("No"),					// TSH_No
		_T("RFID"),					// TSH_RFID
		_T("Socket Type"),			// TSH_SocketType
		_T("Comment"),				// TSH_Comment
		_T("Barcode"),				// TSH_Barcode
		_T("Status"),				// TSH_Status
		_T("Equipment"),			// TSH_Equipment
		_T("Port in Eqp."),			// TSH_Location
		_T("Target Eqp."),			// TSH_Target
		_T("NG Code"),				// TSH_NG_Code
		_T("Test Para"),			// TSH_NG_Para
		_T("Test Eqp."),			// TSH_NG_Eqp
		_T("Total"),				// TSH_Cnt_Total
		_T("Pass "),				// TSH_Cnt_Pass
		_T("Fail"),					// TSH_Cnt_Fail
#if defined(EES_XML)//202310		
		_T("LOT"),					// TSH_LOTID
#endif			
	},

	// 중국어
	{
		_T("号码"),					// TSH_No
		_T("RFID"),					// TSH_RFID
		_T("插座类型"),				// TSH_SocketType
		_T("评语"),					// TSH_Comment
		_T("序列号"),				// TSH_Barcode
		_T("地位"),					// TSH_Status
		_T("设备"),					// TSH_Equipment
		_T("设备端口"),				// TSH_Location
		_T("目标设备"),				// TSH_Target
		_T("NG码"),					// TSH_NG_Code
		_T("试验地点"),				// TSH_NG_Para
		_T("试验设备"),				// TSH_NG_Eqp
		_T("总计"),					// TSH_Cnt_Total
		_T("过关 "),					// TSH_Cnt_Pass
		_T("失败"),					// TSH_Cnt_Fail
#if defined(EES_XML)//202310		
		_T("LOT"),					// TSH_LOTID
#endif			
	},
};

const int	iListAglin[] =
{
	LVCFMT_CENTER,	 // TSH_No
	LVCFMT_CENTER,	 // TSH_RFID
	LVCFMT_CENTER,	 // TSH_SocketType
	LVCFMT_CENTER,	 // TSH_Comment
	LVCFMT_CENTER,	 // TSH_Barcode
	LVCFMT_CENTER,	 // TSH_Status
	LVCFMT_CENTER,	 // TSH_Equipment
	LVCFMT_CENTER,	 // TSH_Location
	LVCFMT_CENTER,	 // TSH_Target
	LVCFMT_CENTER,	 // TSH_NG_Code
	LVCFMT_CENTER,	 // TSH_NG_Para
	LVCFMT_CENTER,	 // TSH_NG_Eqp
	LVCFMT_CENTER,	 // TSH_Cnt_Total
	LVCFMT_CENTER,	 // TSH_Cnt_Pass
	LVCFMT_CENTER,	 // TSH_Cnt_Fail
#if defined(EES_XML)//202310		
	LVCFMT_CENTER,	 // TSH_LOTID
#endif		
};

// 540 기준
const int	iHeaderWidth[] =
{
	 40, 	// TSH_No
	120,	// TSH_RFID
	 90,	// TSH_SocketType
	  0,	// TSH_Comment
	200,	// TSH_Barcode
	 80,	// TSH_Status
	120,	// TSH_Equipment
	 80,	// TSH_Location
	120,	// TSH_Target
	 80,	// TSH_NG_Code
	 80,	// TSH_NG_Para
	120,	// TSH_NG_Eqp
	 80,	// TSH_Cnt_Total
	 80,	// TSH_Cnt_Pass
	 80,	// TSH_Cnt_Fail
#if defined(EES_XML)//202310		
	 80,	// TSH_LOTID
#endif		 
};

IMPLEMENT_DYNAMIC(CList_SocketInfo, CListCtrl)

CList_SocketInfo::CList_SocketInfo()
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

CList_SocketInfo::~CList_SocketInfo()
{
	m_Font.DeleteObject();
}


BEGIN_MESSAGE_MAP(CList_SocketInfo, CListCtrl)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_NOTIFY_REFLECT(NM_CLICK,	&CList_SocketInfo::OnNMClick)
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()



// CList_SocketInfo message handlers
//=============================================================================
// Method		: OnCreate
// Access		: protected  
// Returns		: int
// Parameter	: LPCREATESTRUCT lpCreateStruct
// Qualifier	:
// Last Update	: 2021/11/2 - 14:28
// Desc.		:
//=============================================================================
int CList_SocketInfo::OnCreate(LPCREATESTRUCT lpCreateStruct)
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
void CList_SocketInfo::OnSize(UINT nType, int cx, int cy)
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
BOOL CList_SocketInfo::PreCreateWindow(CREATESTRUCT& cs)
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
// int CList_SocketInfo::OnCompareItems(LPARAM lParam1, LPARAM lParam2, int iColumn)
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
void CList_SocketInfo::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
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
void CList_SocketInfo::InitHeader()
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
// Parameter	: __in LPCTSTR IN_szRFID
// Qualifier	:
// Last Update	: 2021/12/22 - 11:45
// Desc.		:
//=============================================================================
int CList_SocketInfo::Find_Row(__in LPCTSTR IN_szRFID)
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
void CList_SocketInfo::Reset_Number()
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
void CList_SocketInfo::Select_SelectItem(__in int nItem)
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
void CList_SocketInfo::Set_SocketData(__in int nItem, __in const CSocketInfo_Unit* IN_pSocket)
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

	// Location (Port, Conveyor)]
	szText.Format(_T("%d"), IN_pSocket->m_nPort_inEqp);
	SetItemText(nItem, TSH_Location, szText);

	// Target Equipment
	SetItemText(nItem, TSH_Target, IN_pSocket->m_szTargetEqpID);

	// NG Code
	szText.Format(_T("%d"), IN_pSocket->m_stTestResult.m_nNG_Code);
	SetItemText(nItem, TSH_NG_Code, szText);
	
	// NG Para
	if ((0 <= IN_pSocket->m_stTestResult.m_nNG_Para) && (IN_pSocket->m_stTestResult.m_nNG_Para < Para_MaxCount))
	{
		// szText.Format(_T("%s"), (Para_Left == IN_pSocket->m_stTestResult.m_nNG_Para) ? _T("L") : _T("R"));
		szText = g_szEqpPara_Mini[IN_pSocket->m_stTestResult.m_nNG_Para];
		SetItemText(nItem, TSH_NG_Para, szText);
	}
	else
	{
		SetItemText(nItem, TSH_NG_Para, _T("X"));
	}
	
	// NG Equipment
// 	if (0 == IN_pSocket->m_stTestResult.m_nNG_Code)
// 	{
// 		SetItemText(nItem, TSH_NG_Eqp, _T(""));
// 	}
// 	else
// 	{
		SetItemText(nItem, TSH_NG_Eqp, IN_pSocket->m_stTestResult.m_szNG_EqpID);
//	}

	// Total
	szText.Format(_T("%d"), IN_pSocket->m_Yield.dwTotal);
	SetItemText(nItem, TSH_Yield_Total, szText);
	// Pass 
	szText.Format(_T("%d"), IN_pSocket->m_Yield.dwPass);
	SetItemText(nItem, TSH_Yield_Pass, szText);
	// Fail
	szText.Format(_T("%d"), IN_pSocket->m_Yield.dwFail);
	SetItemText(nItem, TSH_Yield_Fail, szText);
#if defined(EES_XML)//20231003
	szText.Format(_T("%s"), IN_pSocket->m_LotID);
	SetItemText(nItem, TSH_LOTID, szText);
#endif
}

void CList_SocketInfo::Set_SocketData_Barcode(__in int nItem, __in const CSocketInfo_Unit* IN_pSocket)
{
	ASSERT(GetSafeHwnd());
	if (NULL == IN_pSocket)
		return;

	if (GetItemCount() <= nItem)
		return;

	// Barcode
	SetItemText(nItem, TSH_Barcode, IN_pSocket->m_szBarcode);
}

void CList_SocketInfo::Set_SocketData_Status(__in int nItem, __in const CSocketInfo_Unit* IN_pSocket)
{
	ASSERT(GetSafeHwnd());
	if (NULL == IN_pSocket)
		return;

	if (GetItemCount() <= nItem)
		return;

	// Status
	SetItemText(nItem, TSH_Status, g_szSocketStatus[IN_pSocket->m_nStatus]);
}

void CList_SocketInfo::Set_SocketData_Equipment(__in int nItem, __in const CSocketInfo_Unit* IN_pSocket)
{
	ASSERT(GetSafeHwnd());
	if (NULL == IN_pSocket)
		return;

	if (GetItemCount() <= nItem)
		return;

	// Equipment
	SetItemText(nItem, TSH_Equipment, IN_pSocket->m_szEquipmentID);
}

void CList_SocketInfo::Set_SocketData_Location(__in int nItem, __in const CSocketInfo_Unit* IN_pSocket)
{
	ASSERT(GetSafeHwnd());
	if (NULL == IN_pSocket)
		return;

	if (GetItemCount() <= nItem)
		return;

	CString szText;

	// Location
	szText.Format(_T("%d"), IN_pSocket->m_nPort_inEqp);
	SetItemText(nItem, TSH_Location, szText);
	
}

void CList_SocketInfo::Set_SocketData_Target(__in int nItem, __in const CSocketInfo_Unit* IN_pSocket)
{
	ASSERT(GetSafeHwnd());
	if (NULL == IN_pSocket)
		return;

	if (GetItemCount() <= nItem)
		return;

	// Target Equipment
	SetItemText(nItem, TSH_Target, IN_pSocket->m_szTargetEqpID);
}

void CList_SocketInfo::Set_SocketData_Yield(__in int nItem, __in const CSocketInfo_Unit* IN_pSocket)
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

void CList_SocketInfo::Set_SocketData_TestResult(__in int nItem, __in const CSocketInfo_Unit* IN_pSocket)
{
	ASSERT(GetSafeHwnd());
	if (NULL == IN_pSocket)
		return;

	if (GetItemCount() <= nItem)
		return;

	CString szText;

	// NG Code
	szText.Format(_T("%d"), IN_pSocket->m_stTestResult.m_nNG_Code);
	SetItemText(nItem, TSH_NG_Code, szText);

	// NG Para
	if ((0 <= IN_pSocket->m_stTestResult.m_nNG_Para) && (IN_pSocket->m_stTestResult.m_nNG_Para < Para_MaxCount))
	{
		//szText.Format(_T("%s"), (Para_Left == IN_pSocket->m_stTestResult.m_nNG_Para) ? _T("L") : _T("R"));
		szText = g_szEqpPara_Mini[IN_pSocket->m_stTestResult.m_nNG_Para];
		SetItemText(nItem, TSH_NG_Para, szText);
	}
	else
	{
		SetItemText(nItem, TSH_NG_Para, _T("X"));
	}

	// NG Equipment
// 	if (0 == IN_pSocket->m_stTestResult.m_nNG_Code)
// 	{
// 		SetItemText(nItem, TSH_NG_Eqp, _T(""));
// 	}
// 	else
// 	{
		SetItemText(nItem, TSH_NG_Eqp, IN_pSocket->m_stTestResult.m_szNG_EqpID);
//	}

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
void CList_SocketInfo::Add_Socket(const __in CSocketInfo_Unit* IN_pSocket)
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
// Method		: OnLanguage
// Access		: public  
// Returns		: void
// Parameter	: __in uint8_t IN_nLanguage
// Qualifier	:
// Last Update	: 2022/7/20 - 13:58
// Desc.		:
//=============================================================================
void CList_SocketInfo::OnLanguage(__in uint8_t IN_nLanguage)
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
			int nCnt = GetItemCount();
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
void CList_SocketInfo::Set_SocketList(__in const CSocketMonitoring* IN_pSoscketList)
{
	m_stSocketList.RemoveAll();
	DeleteAllItems();

	if (NULL != IN_pSoscketList)
	{
		m_stSocketList = *IN_pSoscketList;

		auto eIter = IN_pSoscketList->m_Sockets.end();
		auto Iter  = IN_pSoscketList->m_Sockets.begin();
		while (Iter != eIter)
		{
			//Iter->second.nSocketType;
			Add_Socket(&Iter->second);

			Iter++;
		}
	}
}

void CList_SocketInfo::Set_SocketList_bySocketType(const CSocketMonitoring * IN_pSoscketList, uint8_t IN_nType)
{
	m_stSocketList.RemoveAll();
	DeleteAllItems();

	if (NULL != IN_pSoscketList)
	{
		m_stSocketList = *IN_pSoscketList;

		auto eIter = IN_pSoscketList->m_Sockets.end();
		auto Iter = IN_pSoscketList->m_Sockets.begin();
		while (Iter != eIter)
		{
			if (IN_nType == Iter->second.nSocketType)
			{
				Add_Socket(&Iter->second);
			}

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
void CList_SocketInfo::Get_SocketList(__out CSocketMonitoring& OUT_stSocketList)
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
bool CList_SocketInfo::Get_SelectedSocket(__out CSocketInfo_Unit& OUT_Socket)
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
void CList_SocketInfo::Clear_SocketList()
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
void CList_SocketInfo::Refresh_Socket()
{
	DeleteAllItems();

	auto eIter = m_stSocketList.m_Sockets.end();
	auto Iter = m_stSocketList.m_Sockets.begin();
	while (Iter != eIter)
	{
		Add_Socket(&Iter->second);

		Iter++;
	}
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
void CList_SocketInfo::Update_Socket(__in int nRow, __in const CSocketInfo_Unit* IN_pSocket)
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

void CList_SocketInfo::Update_Socket(__in LPCTSTR IN_szRFID, __in const CSocketInfo_Unit* IN_pSocket)
{
	int nRow = Find_Row(IN_szRFID);

	if (0 <= nRow)
	{
		Update_Socket(nRow, IN_pSocket);
	}
}

void CList_SocketInfo::Update_Socket_Barcode(__in int nRow, __in const CSocketInfo_Unit* IN_pSocket)
{
	Set_SocketData_Barcode(nRow, IN_pSocket);
}

void CList_SocketInfo::Update_Socket_Status(__in int nRow, __in const CSocketInfo_Unit* IN_pSocket)
{
	Set_SocketData_Status(nRow, IN_pSocket);
}

void CList_SocketInfo::Update_Socket_Equipment(__in int nRow, __in const CSocketInfo_Unit* IN_pSocket)
{
	Set_SocketData_Equipment(nRow, IN_pSocket);
}

void CList_SocketInfo::Update_Socket_Location(__in int nRow, __in const CSocketInfo_Unit* IN_pSocket)
{
	Set_SocketData_Location(nRow, IN_pSocket);
}

void CList_SocketInfo::Update_Socket_Target(__in int nRow, __in const CSocketInfo_Unit* IN_pSocket)
{
	Set_SocketData_Target(nRow, IN_pSocket);
}

void CList_SocketInfo::Update_Socket_Yield(__in int nRow, __in const CSocketInfo_Unit* IN_pSocket)
{
	Set_SocketData_Yield(nRow, IN_pSocket);
}

void CList_SocketInfo::Update_Socket_TestResult(__in int nRow, __in const CSocketInfo_Unit* IN_pSocket)
{
	Set_SocketData_TestResult(nRow, IN_pSocket);
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
int CList_SocketInfo::Find_Socket(__in LPCTSTR IN_szRFID)
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
#if defined(EES_XML)//20231003
void CList_SocketInfo::Update_Socket_LOTID(__in int nRow, __in const CSocketInfo_Unit* IN_pSocket)
{
	Set_SocketData_LOTID(nRow, IN_pSocket);
}
void CList_SocketInfo::Set_SocketData_LOTID(__in int nItem, __in const CSocketInfo_Unit* IN_pSocket)
{
	ASSERT(GetSafeHwnd());
	if (NULL == IN_pSocket)
		return;
	if (GetItemCount() <= nItem)
		return;
	CString szText;
	
	szText.Format(_T("%s"), IN_pSocket->m_LotID);
	SetItemText(nItem, TSH_LOTID, szText);
}
#endif