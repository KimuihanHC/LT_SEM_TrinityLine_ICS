//*****************************************************************************
// Filename	: 	Grid_Yield.cpp
// Created	:	2016/3/30 - 18:36
// Modified	:	2016/3/30 - 18:36
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
#include "stdafx.h"
#include "Grid_Yield.h"

static LPCTSTR lpszRowHeader[] =
{
	_T("TOTAL"),//_T("총계"),
	_T("PASS"),//_T("정상"),
	_T("FAIL"),//_T("불량"),
	_T("YIELD"),//_T("수율"),
	_T("White, Dark"),
	_T("35cm"),
	_T("5m"),
	_T("15cm"),
	_T("7cm"),
	NULL
};

typedef enum
{
	IDX_Y_Header = 0,
	IDX_Y_Total = IDX_Y_Header,
	IDX_Y_Pass,
	IDX_Y_Fail,
	IDX_Y_Yield,
	IDX_Y_Particle,
	IDX_Y_35Cm,
	IDX_Y_5M,
	IDX_Y_15Cm,
	IDX_Y_7Cm,
	IDX_ROW_MAX,
}enumRowHeaderYield;

static const COLORREF clrRowHeader[] =
{
	RGB(0, 150, 0),
	RGB(50, 50, 200),
	RGB(200, 50, 50),
	RGB(86, 86, 86),
	RGB(63, 101,  169),
	RGB(63, 101,  169),
	RGB(63, 101,  169),
	RGB(63, 101,  169),
	RGB(63, 101,  169),
};

static LPCTSTR lpszColHeader[] =
{
	_T(""),
	_T(""),
	NULL
};

typedef enum
{
	IDX_X_Header = 0,	
	IDX_X_Item = IDX_X_Header,
	IDX_X_Data,
	IDX_COL_MAX,
}enumColHeaderYield;


#define		RGB_BLACK		RGB(0x00, 0x00, 0x00)
#define		RGB_WHITE		RGB(0xFF, 0xFF, 0xFF)
#define		RGB_YELLOW		RGB(0xFF, 0xFF, 0x00)
#define		RGB_ROW_HEADER	RGB(63, 101,  169)
#define		RGB_BK_ID		RGB(135, 169, 213)
#define		RGB_COL_HEADER	RGB(0xFF, 200, 100)
#define		RGB_TITLE		RGB(150, 200, 0xFF)

#define		RGB_BIT_SET		RGB(123, 255,  75) //RGB(112, 173, 71)
#define		RGB_BIT_CLEAR	RGB(100,  10,  10) //RGB(237, 125, 49)
//#define		RGB_SELECT		RGB(0xFF, 200, 100) //RGB(150, 200, 0xFF)

//=============================================================================
//
//=============================================================================
CGrid_Yield::CGrid_Yield()
{
	SetRowColCount(IDX_ROW_MAX, IDX_COL_MAX);

	//setup the fonts
	m_font_Header.CreateFont(20, 0, 0, 0, 900, 0, 0, 0, 0, 0, 0, ANTIALIASED_QUALITY, 0, _T("Arial"));
	m_font_Data.CreateFont(20, 0, 0, 0, 900, 0, 0, 0, 0, 0, 0, ANTIALIASED_QUALITY, 0, _T("Arial"));
}

//=============================================================================
//
//=============================================================================
CGrid_Yield::~CGrid_Yield()
{
	m_font_Header.DeleteObject();
	m_font_Data.DeleteObject();
}

//=============================================================================
// Method		: OnSetup
// Access		: virtual protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2015/12/14 - 14:30
// Desc.		:
//=============================================================================
void CGrid_Yield::OnSetup()
{
	__super::OnSetup();
}

//=============================================================================
// Method		: DrawGridOutline
// Access		: virtual public  
// Returns		: void
// Qualifier	:
// Last Update	: 2015/12/11 - 13:27
// Desc.		:
//=============================================================================
void CGrid_Yield::DrawGridOutline()
{
	CGrid_Base::DrawGridOutline();

	SetDefFont(&m_font_Data);

	// 헤더를 설정한다.
	InitHeader();
}

//=============================================================================
// Method		: CGrid_Yield::CalGridOutline
// Access		: public 
// Returns		: void
// Qualifier	:
// Last Update	: 2015/12/11 - 13:27
// Desc.		:
//=============================================================================
void CGrid_Yield::CalGridOutline()
{
	//CGrid_Base::CalGridOutline();

	// 윈도우 면적에 따라서 열의 너비를 결정
	CRect rect;
	GetWindowRect(&rect);
	int nWidth = rect.Width();
	int nHeight = rect.Height();

	if ((nWidth <= 0) || (nHeight <= 0))
		return;

	// 기본 열 추가 ---------------------------------------
	int iUnitWidth = nWidth / m_nMaxCols;
	int iMisc = nWidth - (iUnitWidth * (m_nMaxCols - 1));
	SetColWidth(IDX_X_Header, iMisc);
	for (UINT iCol = IDX_X_Header + 1; iCol < m_nMaxCols; iCol++)
	{
		SetColWidth(iCol, iUnitWidth);
	}

	// 패턴 행 헤더 추가 ----------------------------------		
	UINT nUnitHeight = nHeight / m_nMaxRows;
	UINT nRemindHeight = nHeight - (nUnitHeight * m_nMaxRows);

	// Height 계산하고 남거나 모자르는 공간 계산하여 헤더 Height 추가 처리
	for (UINT iRow = 0; iRow < nRemindHeight; iRow++)
	{
		SetRowHeight(iRow, nUnitHeight + 1);
	}

	for (UINT iRow = nRemindHeight; iRow < m_nMaxRows; iRow++)
	{
		SetRowHeight(iRow, nUnitHeight);
	}
}

//=============================================================================
// Method		: CGrid_Yield::InitRowHeader
// Access		: protected 
// Returns		: void
// Qualifier	:
// Last Update	: 2015/12/11 - 13:27
// Desc.		:
//=============================================================================
void CGrid_Yield::InitHeader()
{
	for (UINT iRow = 0; iRow < m_nMaxRows; iRow++)
	{
		QuickSetFont		(IDX_X_Item, iRow, &m_font_Header);
		QuickSetBackColor	(IDX_X_Item, iRow, clrRowHeader[iRow]);
		QuickSetTextColor	(IDX_X_Item, iRow, RGB_WHITE);
		QuickSetText		(IDX_X_Item, iRow, lpszRowHeader[iRow]);

		QuickSetFont		(IDX_X_Data, iRow, &m_font_Data);
		QuickSetBackColor	(IDX_X_Data, iRow, RGB_WHITE);
		QuickSetTextColor	(IDX_X_Data, iRow, clrRowHeader[iRow]);
		QuickSetText		(IDX_X_Data, iRow, _T("0"));	
	}	

	QuickSetText		(IDX_X_Data, IDX_Y_Yield, _T("0.00 %"));

	for (UINT iRow = IDX_Y_Particle; iRow < m_nMaxRows; iRow++)
	{
		QuickSetText(IDX_X_Data, iRow, _T("0.00 %"));
	}
}

//=============================================================================
// Method		: OnHint
// Access		: virtual protected  
// Returns		: int
// Parameter	: int col
// Parameter	: long row
// Parameter	: int section
// Parameter	: CString * string
// Qualifier	:
// Last Update	: 2015/12/11 - 13:27
// Desc.		:
//=============================================================================
int CGrid_Yield::OnHint(int col, long row, int section, CString *string)
{
	return FALSE;
}

//=============================================================================
// Method		: CGrid_Yield::OnGetCell
// Access		: virtual protected 
// Returns		: void
// Parameter	: int col
// Parameter	: long row
// Parameter	: CUGCell * cell
// Qualifier	:
// Last Update	: 2015/12/10 - 23:28
// Desc.		:
//=============================================================================
void CGrid_Yield::OnGetCell(int col, long row, CUGCell *cell)
{
	CGrid_Base::OnGetCell(col, row, cell);

//   	switch (row)
//   	{
//   	case IDX_Y_Header:
//   		if (IDX_X_Total == col)
//   			cell->SetBorder(cell->GetBorder() | UG_BDR_RMEDIUM | UG_BDR_BMEDIUM);
//   		break;
//   	}
}

//=============================================================================
// Method		: OnDrawFocusRect
// Access		: virtual protected  
// Returns		: void
// Parameter	: CDC * dc
// Parameter	: RECT * rect
// Qualifier	:
// Last Update	: 2015/12/11 - 13:27
// Desc.		:
//=============================================================================
void CGrid_Yield::OnDrawFocusRect(CDC *dc, RECT *rect)
{

}

//=============================================================================
// Method		: SetYield
// Access		: public  
// Returns		: void
// Parameter	: __in const ST_ProgressTable * pstYield
// Qualifier	:
// Last Update	: 2016/3/31 - 18:04
// Desc.		:
//=============================================================================
 void CGrid_Yield::SetYield(__in const ST_Yield* pstYield)
 {
 	CString strText;
 
 	QuickSetNumber(IDX_X_Data, IDX_Y_Total, pstYield->dwTotal);
 	QuickSetNumber(IDX_X_Data, IDX_Y_Pass, pstYield->dwPass);
 	QuickSetNumber(IDX_X_Data, IDX_Y_Fail, pstYield->dwFail);
 
 	strText.Format(_T("%.2f %%"), pstYield->fYield);
 	QuickSetText(IDX_X_Data, IDX_Y_Yield, strText);
 
	// 불량 항목 불량율	
	strText.Format(_T("%.2f %%"), pstYield->fFailYield[Site_B]);
	QuickSetText(IDX_X_Data, IDX_Y_Particle, strText);

	strText.Format(_T("%.2f %%"), pstYield->fFailYield[Site_C]);
	QuickSetText(IDX_X_Data, IDX_Y_35Cm, strText);

	strText.Format(_T("%.2f %%"), pstYield->fFailYield[Site_D]);
	QuickSetText(IDX_X_Data, IDX_Y_5M, strText);

	strText.Format(_T("%.2f %%"), pstYield->fFailYield[Site_E]);
	QuickSetText(IDX_X_Data, IDX_Y_15Cm, strText);

	strText.Format(_T("%.2f %%"), pstYield->fFailYield[Site_F]);
	QuickSetText(IDX_X_Data, IDX_Y_7Cm, strText);
	
 	RedrawCol(IDX_X_Data);
 }


 //=============================================================================
 // Method		: ShowResetButton
 // Access		: public  
 // Returns		: void
 // Parameter	: __in BOOL bShow
 // Qualifier	:
 // Last Update	: 2016/12/6 - 17:42
 // Desc.		:
 //=============================================================================
 void CGrid_Yield::ShowResetButton(__in BOOL bShow)
 {

 }
