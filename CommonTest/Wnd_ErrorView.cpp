//*****************************************************************************
// Filename	: 	Wnd_ErrorView.cpp
// Created	:	2016/10/29 - 16:48
// Modified	:	2016/10/29 - 16:48
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
// Wnd_ErrorView.cpp : implementation file
//

#include "stdafx.h"
#include "Wnd_ErrorView.h"


#define IDC_LIST_ERROR		1001
#define IDC_BN_DELETE		1002
#define IDC_ED_DESC			1003

// CWnd_ErrorView

IMPLEMENT_DYNAMIC(CWnd_ErrorView, CWnd)

CWnd_ErrorView::CWnd_ErrorView()
{
	VERIFY(m_font_Default.CreateFont(
		32,						// nHeight
		0,						// nWidth
		0,						// nEscapement
		0,						// nOrientation
		FW_HEAVY,				// nWeight
		FALSE,					// bItalic
		FALSE,					// bUnderline
		0,						// cStrikeOut
		ANSI_CHARSET,			// nCharSet
		OUT_DEFAULT_PRECIS,		// nOutPrecision
		CLIP_DEFAULT_PRECIS,	// nClipPrecision
		ANTIALIASED_QUALITY,	// nQuality
		DEFAULT_PITCH,			// nPitchAndFamily
		_T("Arial")));			// lpszFacename

	VERIFY(m_font_Data.CreateFont(
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
		DEFAULT_PITCH,			// nPitchAndFamily
		_T("Arial")));			// lpszFacename

}

CWnd_ErrorView::~CWnd_ErrorView()
{
	m_font_Default.DeleteObject();
	m_font_Data.DeleteObject();
}


BEGIN_MESSAGE_MAP(CWnd_ErrorView, CWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_NOTIFY		(NM_CLICK, IDC_LIST_ERROR, OnNMClickList)
	ON_BN_CLICKED	(IDC_BN_DELETE, OnBnClickedBnDelete)
END_MESSAGE_MAP()


// CWnd_ErrorView message handlers

//=============================================================================
// Method		: OnCreate
// Access		: protected  
// Returns		: int
// Parameter	: LPCREATESTRUCT lpCreateStruct
// Qualifier	:
// Last Update	: 2016/10/29 - 16:50
// Desc.		:
//=============================================================================
int CWnd_ErrorView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	DWORD dwStyle = WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	m_st_Title.SetBorderTickness(3);
	m_st_Title.SetBackColor(RGB(192, 50, 50));
	m_st_Title.SetFont_Gdip(L"Arial", 32.0F);
	m_st_Title.Create(_T("Error List"), dwStyle | SS_CENTER | SS_CENTERIMAGE, rectDummy, this, IDC_STATIC);

	m_st_ErrorCap.Create(_T("에러 발생 정보"), dwStyle | SS_CENTER | SS_CENTERIMAGE, rectDummy, this, IDC_STATIC);
	m_lc_Error.Create(WS_CHILD | WS_VISIBLE, rectDummy, this, IDC_LIST_ERROR);

	m_st_DescCap.Create(_T("조치 방법"), dwStyle | SS_CENTER | SS_CENTERIMAGE, rectDummy, this, IDC_STATIC);
	m_ed_Desc.Create(dwStyle | WS_BORDER, rectDummy, this, IDC_ED_DESC);

	m_bn_Delete.Create(_T("Delete Error Message"), dwStyle, rectDummy, this, IDC_BN_DELETE);

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
// Last Update	: 2016/10/29 - 16:50
// Desc.		:
//=============================================================================
void CWnd_ErrorView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	if ((0 == cx) || (0 == cy))
		return;

	int iMagrin = 10;
	int iSpacing = 5;
	int iCateSpacing = 10;

	int iLeft = iMagrin;
	int iTop = iMagrin;
	int iWidth = cx - iMagrin - iMagrin;
	int iHeight = cy - iMagrin - iMagrin;
	int iCtrlWidth = 300;
	int iCtrlHeight = 50;
// 	int iSubTop = iTop + iCtrlHeight + iSpacing;
// 	int iSubLeft = iLeft;

	int iTempWidth = (iWidth - iCateSpacing) * 2 / 3;
	int iTempHeight = 60;
	m_st_Title.MoveWindow(iLeft, iTop, iWidth, iTempHeight);

	iTop += iTempHeight + iCateSpacing;
	//m_st_ErrorCap.MoveWindow(iLeft, iTop, iTempWidth, iCtrlHeight);


	iTempHeight = iHeight - iTempHeight - iCateSpacing - iCateSpacing - iCtrlHeight;
	m_lc_Error.MoveWindow(iLeft, iTop, iTempWidth, iTempHeight);

	iLeft += iTempWidth + iCateSpacing;
	iTempWidth = iWidth - iCateSpacing - iTempWidth;
	m_ed_Desc.MoveWindow(iLeft, iTop, iTempWidth, iTempHeight);

	iLeft = iMagrin;
	iTop += iTempHeight + iCateSpacing;
	m_bn_Delete.MoveWindow(iLeft, iTop, iCtrlWidth, iCtrlHeight);

}

//=============================================================================
// Method		: PreCreateWindow
// Access		: virtual protected  
// Returns		: BOOL
// Parameter	: CREATESTRUCT & cs
// Qualifier	:
// Last Update	: 2016/10/29 - 16:50
// Desc.		:
//=============================================================================
BOOL CWnd_ErrorView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1), NULL);

	return CWnd::PreCreateWindow(cs);
}

//=============================================================================
// Method		: OnNMClickList
// Access		: protected  
// Returns		: void
// Parameter	: NMHDR * pNMHDR
// Parameter	: LRESULT * result
// Qualifier	:
// Last Update	: 2016/10/29 - 20:47
// Desc.		:
//=============================================================================
void CWnd_ErrorView::OnNMClickList(NMHDR * pNMHDR, LRESULT * result)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	NM_LISTVIEW* pNMView = (NM_LISTVIEW*)pNMHDR;

	int index = pNMView->iItem;

	if (0 <= index)
	{
		ST_ErrorInfo errInfo;
		if (m_lc_Error.GetErrorInfo(index, errInfo))
		{
			errInfo.lCode;
		}
	}

}

//=============================================================================
// Method		: OnBnClickedBnDelete
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2016/10/29 - 20:44
// Desc.		:
//=============================================================================
void CWnd_ErrorView::OnBnClickedBnDelete()
{
	if (IDYES == AfxMessageBox(_T("에러 메세지를 삭제하시겠습니까?"), MB_YESNO | MB_SYSTEMMODAL))
	{
		//m_lc_Error.DeleteError();

		m_lc_Error.ClearErrorlist();
	}

}

//=============================================================================
// Method		: InsertErrorlist
// Access		: public  
// Returns		: void
// Parameter	: __in const ST_ErrorInfo * pstErrorlist
// Qualifier	:
// Last Update	: 2016/10/29 - 20:43
// Desc.		:
//=============================================================================
void CWnd_ErrorView::InsertErrorInfo(__in const ST_ErrorInfo* pstErrorlist)
{
	m_lc_Error.InsertErrorInfo(pstErrorlist);


}
