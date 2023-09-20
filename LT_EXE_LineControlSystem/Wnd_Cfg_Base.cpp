//*****************************************************************************
// Filename	: 	Wnd_Cfg_Base.cpp
// Created	:	2016/3/14 - 10:57
// Modified	:	2016/3/14 - 10:57
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
// Wnd_Cfg_Base.cpp : implementation file
//

#include "stdafx.h"
#include "Wnd_Cfg_Base.h"



//-----------------------------------------------------------------------------
// CWnd_Cfg_Base
//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CWnd_Cfg_Base, CWnd_BaseView)

CWnd_Cfg_Base::CWnd_Cfg_Base()
{
	m_sizeCaption.SetSize(120, 20);
	m_sizeData.SetSize(160, 20);
	m_Margin.left	= 0;
	m_Margin.top	= 0;
	m_Margin.right	= 0;
	m_Margin.bottom	= 0;
	m_nSpacing		= 5;
	m_nCateSpacing	= 10;

	m_nFontSize_Caption.SetSize(0, 13);
	m_nFontSize_Data.SetSize(0, 13);

	//Create_Font();
}

CWnd_Cfg_Base::~CWnd_Cfg_Base()
{
	Release_Font();

}

BEGIN_MESSAGE_MAP(CWnd_Cfg_Base, CWnd_BaseView)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CWnd_Cfg_Base message handlers
//=============================================================================
// Method		: OnCreate
// Access		: public  
// Returns		: int
// Parameter	: LPCREATESTRUCT lpCreateStruct
// Qualifier	:
// Last Update	: 2016/3/17 - 10:04
// Desc.		:
//=============================================================================
int CWnd_Cfg_Base::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;

// 	DWORD dwStyle = WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
// 	CRect rectDummy;
// 	rectDummy.SetRectEmpty();

	Create_Font();


	return 0;
}

//=============================================================================
// Method		: OnSize
// Access		: public  
// Returns		: void
// Parameter	: UINT nType
// Parameter	: int cx
// Parameter	: int cy
// Qualifier	:
// Last Update	: 2016/3/17 - 10:04
// Desc.		:
//=============================================================================
void CWnd_Cfg_Base::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

}

//=============================================================================
// Method		: PreCreateWindow
// Access		: virtual public  
// Returns		: BOOL
// Parameter	: CREATESTRUCT & cs
// Qualifier	:
// Last Update	: 2016/3/17 - 10:04
// Desc.		:
//=============================================================================
BOOL CWnd_Cfg_Base::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1), NULL);

	return __super::PreCreateWindow(cs);
}

//=============================================================================
// Method		: PreTranslateMessage
// Access		: virtual public  
// Returns		: BOOL
// Parameter	: MSG * pMsg
// Qualifier	:
// Last Update	: 2016/3/17 - 10:04
// Desc.		:
//=============================================================================
BOOL CWnd_Cfg_Base::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	return __super::PreTranslateMessage(pMsg);
}

//=============================================================================
// Method		: Create_Font
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/3/4 - 16:49
// Desc.		:
//=============================================================================
void CWnd_Cfg_Base::Create_Font()
{
	VERIFY(m_font_Default.CreateFont(
		m_nFontSize_Caption.cy,	// nHeight
		m_nFontSize_Caption.cx,	// nWidth
		0,						// nEscapement
		0,						// nOrientation
		FW_NORMAL,				// nWeight
		FALSE,					// bItalic
		FALSE,					// bUnderline
		0,						// cStrikeOut
		DEFAULT_CHARSET,		// nCharSet
		OUT_DEFAULT_PRECIS,		// nOutPrecision
		CLIP_DEFAULT_PRECIS,	// nClipPrecision
		DEFAULT_QUALITY,		// nQuality
		DEFAULT_PITCH,			// nPitchAndFamily
		_T("Tahoma")));			// lpszFacename

	VERIFY(m_font_Data.CreateFont(
		m_nFontSize_Data.cy,	// nHeight
		m_nFontSize_Data.cx,	// nWidth
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
		_T("Tahoma")));			// lpszFacename
	
}

//=============================================================================
// Method		: Release_Font
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/3/4 - 16:49
// Desc.		:
//=============================================================================
void CWnd_Cfg_Base::Release_Font()
{
	m_font_Default.DeleteObject();
	m_font_Data.DeleteObject();
}

//=============================================================================
// Method		: Set_CaptionSize
// Access		: public  
// Returns		: void
// Parameter	: __in UINT nWidth
// Parameter	: __in UINT nHeight
// Qualifier	:
// Last Update	: 2021/3/4 - 16:47
// Desc.		:
//=============================================================================
void CWnd_Cfg_Base::Set_CaptionSize(__in UINT nWidth, __in UINT nHeight)
{
	m_sizeCaption.SetSize(nWidth, nHeight);
}

//=============================================================================
// Method		: Set_DataSize
// Access		: public  
// Returns		: void
// Parameter	: __in UINT nWidth
// Parameter	: __in UINT nHeight
// Qualifier	:
// Last Update	: 2021/3/12 - 14:10
// Desc.		:
//=============================================================================
void CWnd_Cfg_Base::Set_DataSize(__in UINT nWidth, __in UINT nHeight)
{
	m_sizeData.SetSize(nWidth, nHeight);
}

//=============================================================================
// Method		: Set_Margin
// Access		: public  
// Returns		: void
// Parameter	: __in UINT nLeft
// Parameter	: __in UINT nTop
// Parameter	: __in UINT nRight
// Parameter	: __in UINT nBottom
// Qualifier	:
// Last Update	: 2021/3/12 - 14:10
// Desc.		:
//=============================================================================
void CWnd_Cfg_Base::Set_Margin(__in UINT nLeft, __in UINT nTop, __in UINT nRight, __in UINT nBottom)
{
	m_Margin.left	= nLeft;
	m_Margin.top	= nTop;
	m_Margin.right	= nRight;
	m_Margin.bottom = nBottom;
}

//=============================================================================
// Method		: Set_Spacing
// Access		: public  
// Returns		: void
// Parameter	: __in UINT nSpacing
// Parameter	: __in UINT nCateSpacing
// Qualifier	:
// Last Update	: 2021/3/12 - 14:11
// Desc.		:
//=============================================================================
void CWnd_Cfg_Base::Set_Spacing(__in UINT nSpacing, __in UINT nCateSpacing)
{
	m_nSpacing = nSpacing;
	m_nCateSpacing = nCateSpacing;
}

//=============================================================================
// Method		: Set_FontSize_Caption
// Access		: public  
// Returns		: void
// Parameter	: __in UINT nWidth
// Parameter	: __in UINT nHeghit
// Qualifier	:
// Last Update	: 2021/3/12 - 14:11
// Desc.		:
//=============================================================================
void CWnd_Cfg_Base::Set_FontSize_Caption(__in UINT nWidth, __in UINT nHeghit)
{
	m_nFontSize_Caption.SetSize(nWidth, nHeghit);
}

//=============================================================================
// Method		: Set_FontSize_Data
// Access		: public  
// Returns		: void
// Parameter	: __in UINT nWidth
// Parameter	: __in UINT nHeghit
// Qualifier	:
// Last Update	: 2021/3/12 - 14:11
// Desc.		:
//=============================================================================
void CWnd_Cfg_Base::Set_FontSize_Data(__in UINT nWidth, __in UINT nHeghit)
{
	m_nFontSize_Data.SetSize(nWidth, nHeghit);
}

//=============================================================================
// Method		: Enable_ControlEvent
// Access		: public  
// Returns		: void
// Parameter	: __in bool bEnable
// Qualifier	:
// Last Update	: 2021/3/12 - 14:11
// Desc.		:
//=============================================================================
void CWnd_Cfg_Base::Enable_ControlEvent(__in bool bEnable)
{
	m_bUse_ControlEvent = bEnable;
}

//=============================================================================
// Method		: Set_UseFixedSize
// Access		: public  
// Returns		: void
// Parameter	: __in bool bUse
// Qualifier	:
// Last Update	: 2021/4/22 - 15:19
// Desc.		:
//=============================================================================
void CWnd_Cfg_Base::Set_UseFixedSize(__in bool bUse)
{
	m_bUseFixedSize = bUse;
}
