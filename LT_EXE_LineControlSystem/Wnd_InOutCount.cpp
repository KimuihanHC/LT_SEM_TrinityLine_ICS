//*****************************************************************************
// Filename	: 	Wnd_InOutCount.cpp
// Created	:	2021/12/22 - 10:39
// Modified	:	2021/12/22 - 10:39
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
// Wnd_InOutCount.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Wnd_InOutCount.h"
#include "resource.h"


#define		IDC_LC_SOCKETLIST_FST	1000

#define		IDC_ED_BARCOE			1001
#define		IDC_BN_FIND				1002

#define		IDC_BN_SCROLL_UP		1007
#define		IDC_BN_SCROLL_DOWN		1008


//=============================================================================
// CWnd_InOutCount
//=============================================================================
IMPLEMENT_DYNAMIC(CWnd_InOutCount, CWnd_BaseView)

CWnd_InOutCount::CWnd_InOutCount()
{
	VERIFY(m_font_Default.CreateFont(
		20,						// nHeight
		0,						// nWidth
		0,						// nEscapement
		0,						// nOrientation
		FW_SEMIBOLD,			// nWeight
		FALSE,					// bItalic
		FALSE,					// bUnderline
		0,						// cStrikeOut
		ANSI_CHARSET,			// nCharSet
		OUT_DEFAULT_PRECIS,		// nOutPrecision
		CLIP_DEFAULT_PRECIS,	// nClipPrecision
		ANTIALIASED_QUALITY,	// nQuality
		DEFAULT_PITCH,			// nPitchAndFamily
		_T("Arial")));			// lpszFacename
	
	VERIFY(m_Font.CreateFont(
		28,						// nHeight
		0,						// nWidth
		0,						// nEscapement
		0,						// nOrientation
		FW_BOLD,				// nWeight
		FALSE,					// bItalic
		FALSE,					// bUnderline
		0,						// cStrikeOut
		ANSI_CHARSET,			// nCharSet
		OUT_DEFAULT_PRECIS,		// nOutPrecision
		CLIP_DEFAULT_PRECIS,	// nClipPrecision
		ANTIALIASED_QUALITY,	// nQuality
		DEFAULT_PITCH,			// nPitchAndFamily
		_T("Arial")));			// lpszFacename

}

CWnd_InOutCount::~CWnd_InOutCount()
{
	TRACE(_T("<<< Start ~CWnd_InOutCount >>> \n"));
	
	m_font_Default.DeleteObject();
	m_Font.DeleteObject();
}

BEGIN_MESSAGE_MAP(CWnd_InOutCount, CWnd_BaseView)
	ON_WM_CREATE()
	ON_WM_SIZE	()
END_MESSAGE_MAP()


//=============================================================================
// CWnd_InOutCount 메시지 처리기입니다.
//=============================================================================
//=============================================================================
// Method		: OnCreate
// Access		: protected  
// Returns		: int
// Parameter	: LPCREATESTRUCT lpCreateStruct
// Qualifier	:
// Last Update	: 2021/12/22 - 10:39
// Desc.		:
//=============================================================================
int CWnd_InOutCount::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd_BaseView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	//SetBackgroundColor(RGB(0, 0, 0));

	DWORD dwStyle = WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	CRect rectDummy;
	rectDummy.SetRectEmpty();
	
	/*m_Font.CreateStockObject(DEFAULT_GUI_FONT);*/

	UINT nWindowsID = 1;

	LPCTSTR szInOutCount[] =
	{
		_T("In"),		// SCnt_In,
		_T("Out"),		// SCnt_Out,
		_T("Ongoing"),	// SCnt_Ongoing,
		_T("Remove"),	// SCnt_Remove,
	};

	for (auto nIdx = 0; nIdx < SCnt_MaxCount; ++nIdx)
	{
		m_st_Caption[nIdx].SetStaticStyle(CVGStatic::StaticStyle_Title_Alt);
		m_st_Caption[nIdx].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_st_Caption[nIdx].SetFont_Gdip(L"Arial", 16.0F);
		m_st_Caption[nIdx].Create(szInOutCount[nIdx], dwStyle | SS_CENTER | SS_CENTERIMAGE, rectDummy, this, IDC_STATIC);

		m_st_Data[nIdx].SetStaticStyle(CVGStatic::StaticStyle_Data);
		m_st_Data[nIdx].SetFont_Gdip(L"Arial", 16.0F);
		m_st_Data[nIdx].Create(_T(""), dwStyle | SS_CENTER | SS_CENTERIMAGE, rectDummy, this, IDC_STATIC);
	}

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
// Last Update	: 2021/12/22 - 10:39
// Desc.		:
//=============================================================================
void CWnd_InOutCount::OnSize(UINT nType, int cx, int cy)
{
	CWnd_BaseView::OnSize(nType, cx, cy);

	if ((cx == 0) && (cy == 0))
		return;

	int iMargin			= 5;
 	int iSpacing		= 5;
 	int iCateSpacing	= 5;

	int iLeft			= iMargin;
	int iTop			= iMargin;
	int iWidth			= cx - iMargin - iMargin;
	int iHeight			= cy - iMargin - iMargin;	
	int iCtrlWidth		= iWidth / 2;
	int iCtrlHeight		= 50;
	int iLeft_Sub		= iLeft + iCtrlWidth;

	for (auto nIdx = 0; nIdx < SCnt_MaxCount; ++nIdx)
	{
		m_st_Caption[nIdx].MoveWindow(iLeft, iTop, iCtrlWidth, iCtrlHeight);
		m_st_Data[nIdx].MoveWindow(iLeft_Sub, iTop, iCtrlWidth, iCtrlHeight);

		iTop += iCtrlHeight + iSpacing;
	}
}

//=============================================================================
// Method		: Init
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/12/22 - 10:39
// Desc.		:
//=============================================================================
void CWnd_InOutCount::Init()
{
	if (m_pstCountInfo)
	{
		
	}
}

//=============================================================================
// Method		: Update_CountInfo
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/12/22 - 10:39
// Desc.		:
//=============================================================================
void CWnd_InOutCount::Update_CountInfo()
{
	
}

void CWnd_InOutCount::Update_CountInfo(__in const ST_InOutCount* IN_pstCountInfo)
{
	// 중복 동작 방지

	CString szText;

	szText.Format(_T("%d"), IN_pstCountInfo->nIn);
	m_st_Data[SCnt_In].SetText(szText.GetBuffer());

	szText.Format(_T("%d"), IN_pstCountInfo->nOut);
	m_st_Data[SCnt_Out].SetText(szText.GetBuffer());

	szText.Format(_T("%d"), IN_pstCountInfo->nOngoing);
	m_st_Data[SCnt_Ongoing].SetText(szText.GetBuffer());

	szText.Format(_T("%d"), IN_pstCountInfo->nRemove);
	m_st_Data[SCnt_Remove].SetText(szText.GetBuffer());
}
