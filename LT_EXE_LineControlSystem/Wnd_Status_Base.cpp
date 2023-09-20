//*****************************************************************************
// Filename	: 	Wnd_Status_Base.cpp
// Created	:	2021/11/19 - 11:14
// Modified	:	2021/11/19 - 11:14
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************

#include "stdafx.h"
#include "Wnd_Status_Base.h"

//-----------------------------------------------------------------------------
// CWnd_Status_Base
//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CWnd_Status_Base, CWnd)

CWnd_Status_Base::CWnd_Status_Base()
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

CWnd_Status_Base::~CWnd_Status_Base()
{
	Release_Font();

}

BEGIN_MESSAGE_MAP(CWnd_Status_Base, CWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CWnd_Status_Base message handlers
//=============================================================================
// Method		: OnCreate
// Access		: protected  
// Returns		: int
// Parameter	: LPCREATESTRUCT lpCreateStruct
// Qualifier	:
// Last Update	: 2021/11/19 - 11:14
// Desc.		:
//=============================================================================
int CWnd_Status_Base::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	Set_Background (RGB(0xFF, 0xFF, 0xFF));
	Create_Font();

 	DWORD dwStyle = WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
 	CRect rectDummy;
 	rectDummy.SetRectEmpty();


	//m_st_Title.SetStaticStyle(CVGStatic::StaticStyle_Title_Alt);
	//m_st_Title.SetColorStyle(CVGStatic::ColorStyle_Black);
	m_st_Title.SetFont_Gdip(L"Arial", 8.0F);
	m_st_Title.SetBackColor_COLORREF(RGB(0, 0, 0));
	m_st_Title.SetTextColor_COLORREF(RGB(255, 255, 255), RGB(255, 255, 255), RGB(255, 255, 255));
	m_st_Title.SetBorderTickness(1);
	m_st_Title.SetBorderColor_COLORREF(RGB(255, 255, 255));
	m_st_Title.SetTextAlignment(StringAlignmentNear);

	m_st_Border.SetBorderColor_COLORREF(RGB(255, 255, 255));


	m_st_Title.Create(_T(""), dwStyle, rectDummy, this, IDC_STATIC);
	m_st_Border.Create(_T(""), dwStyle | SS_BLACKFRAME, rectDummy, this, IDC_STATIC);	


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
// Last Update	: 2021/11/19 - 11:14
// Desc.		:
//=============================================================================
void CWnd_Status_Base::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	if ((cx == 0) && (cy == 0))
		return;

	int iTop			= 0;
	int iWidth			= cx;
	int iHeight			= m_nTitle_Height;


	m_st_Title.MoveWindow(0, iTop, iWidth, iHeight);
	iTop += iHeight - 1;
	iHeight = cy - iTop;
	m_st_Border.MoveWindow(0, iTop, iWidth, iHeight);

}

//=============================================================================
// Method		: OnEraseBkgnd
// Access		: protected  
// Returns		: BOOL
// Parameter	: CDC * pDC
// Qualifier	:
// Last Update	: 2021/11/19 - 11:15
// Desc.		:
//=============================================================================
BOOL CWnd_Status_Base::OnEraseBkgnd(CDC* pDC)
{
	if (m_brBkgr.GetSafeHandle() == NULL)
	{
		return CWnd::OnEraseBkgnd(pDC);
	}

	ASSERT_VALID(pDC);

	CRect rectClient;
	GetClientRect(rectClient);

	if (m_brBkgr.GetSafeHandle() != NULL)
	{
		pDC->FillRect(rectClient, &m_brBkgr);
	}
	else
	{
		CWnd::OnEraseBkgnd(pDC);
	}

	return TRUE;
}

//=============================================================================
// Method		: OnCtlColor
// Access		: protected  
// Returns		: HBRUSH
// Parameter	: CDC * pDC
// Parameter	: CWnd * pWnd
// Parameter	: UINT nCtlColor
// Qualifier	:
// Last Update	: 2021/11/19 - 11:15
// Desc.		:
//=============================================================================
HBRUSH CWnd_Status_Base::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (m_brBkgr.GetSafeHandle() != NULL)
	{
#define AFX_MAX_CLASS_NAME 255
#define AFX_STATIC_CLASS _T("Static")
#define AFX_BUTTON_CLASS _T("Button")

		if (nCtlColor == CTLCOLOR_STATIC)
		{
			TCHAR lpszClassName[AFX_MAX_CLASS_NAME + 1];

			::GetClassName(pWnd->GetSafeHwnd(), lpszClassName, AFX_MAX_CLASS_NAME);
			CString strClass = lpszClassName;

			if (strClass == AFX_STATIC_CLASS)
			{
				pDC->SetBkMode(TRANSPARENT);
				return(HBRUSH) ::GetStockObject(HOLLOW_BRUSH);
			}

			if (strClass == AFX_BUTTON_CLASS)
			{
				if ((pWnd->GetStyle() & BS_GROUPBOX) == 0)
				{
					pDC->SetBkMode(TRANSPARENT);
				}

				return(HBRUSH) ::GetStockObject(HOLLOW_BRUSH);
			}
		}
	}

	return CWnd::OnCtlColor(pDC, pWnd, nCtlColor);
}

//=============================================================================
// Method		: PreCreateWindow
// Access		: virtual protected  
// Returns		: BOOL
// Parameter	: CREATESTRUCT & cs
// Qualifier	:
// Last Update	: 2021/11/19 - 11:15
// Desc.		:
//=============================================================================
BOOL CWnd_Status_Base::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	//cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1), NULL);

	return TRUE;
}

//=============================================================================
// Method		: PreTranslateMessage
// Access		: virtual protected  
// Returns		: BOOL
// Parameter	: MSG * pMsg
// Qualifier	:
// Last Update	: 2021/11/19 - 11:15
// Desc.		:
//=============================================================================
BOOL CWnd_Status_Base::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	return __super::PreTranslateMessage(pMsg);
}

//=============================================================================
// Method		: Create_Font
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/11/19 - 11:15
// Desc.		:
//=============================================================================
void CWnd_Status_Base::Create_Font()
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
// Last Update	: 2021/11/19 - 11:15
// Desc.		:
//=============================================================================
void CWnd_Status_Base::Release_Font()
{
	m_font_Default.DeleteObject();
	m_font_Data.DeleteObject();
}

//=============================================================================
// Method		: OnLanguage
// Access		: virtual public  
// Returns		: void
// Parameter	: __in uint8_t IN_nLanguage
// Qualifier	:
// Last Update	: 2022/7/20 - 16:50
// Desc.		:
//=============================================================================
void CWnd_Status_Base::OnLanguage(__in uint8_t IN_nLanguage)
{
	if (m_nLanguage != IN_nLanguage)
	{
		m_nLanguage = IN_nLanguage;
	}
}

//=============================================================================
// Method		: Set_Border
// Access		: virtual public  
// Returns		: void
// Parameter	: __in uint8_t nThickness
// Parameter	: __in COLORREF clrBorder
// Qualifier	:
// Last Update	: 2021/11/19 - 15:28
// Desc.		:
//=============================================================================
void CWnd_Status_Base::Set_Border(__in uint8_t nThickness, __in COLORREF clrBorder)
{
	m_nBorderThickness	= nThickness;
	m_clrBorder			= clrBorder;
}

//=============================================================================
// Method		: Set_TitleBar
// Access		: virtual public  
// Returns		: void
// Parameter	: __in uint8_t nHeight
// Parameter	: __in COLORREF clrBack
// Qualifier	:
// Last Update	: 2021/11/19 - 15:28
// Desc.		:
//=============================================================================
void CWnd_Status_Base::Set_TitleBar(__in uint8_t nHeight, __in COLORREF clrBack)
{
	m_nTitle_Height = nHeight;
	m_clrTitleBk = clrBack;
}

//=============================================================================
// Method		: Set_Background
// Access		: virtual public  
// Returns		: void
// Parameter	: __in COLORREF color
// Parameter	: __in BOOL bRepaint
// Qualifier	:
// Last Update	: 2021/11/19 - 15:28
// Desc.		:
//=============================================================================
void CWnd_Status_Base::Set_Background(__in COLORREF color, __in BOOL bRepaint /*= TRUE*/)
{
	if (m_brBkgr.GetSafeHandle() != NULL)
	{
		m_brBkgr.DeleteObject();
	}

	if (color != (COLORREF)-1)
	{
		m_brBkgr.CreateSolidBrush(color);
	}

	if (bRepaint && GetSafeHwnd() != NULL)
	{
		Invalidate();
		UpdateWindow();
	}
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
void CWnd_Status_Base::Set_CaptionSize(__in UINT nWidth, __in UINT nHeight)
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
void CWnd_Status_Base::Set_DataSize(__in UINT nWidth, __in UINT nHeight)
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
void CWnd_Status_Base::Set_Margin(__in UINT nLeft, __in UINT nTop, __in UINT nRight, __in UINT nBottom)
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
void CWnd_Status_Base::Set_Spacing(__in UINT nSpacing, __in UINT nCateSpacing)
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
void CWnd_Status_Base::Set_FontSize_Caption(__in UINT nWidth, __in UINT nHeghit)
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
void CWnd_Status_Base::Set_FontSize_Data(__in UINT nWidth, __in UINT nHeghit)
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
void CWnd_Status_Base::Enable_ControlEvent(__in bool bEnable)
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
void CWnd_Status_Base::Set_UseFixedSize(__in bool bUse)
{
	m_bUseFixedSize = bUse;
}

//=============================================================================
// Method		: SetText_Title
// Access		: public  
// Returns		: void
// Parameter	: __in LPCTSTR szTitle
// Qualifier	:
// Last Update	: 2021/11/26 - 12:23
// Desc.		:
//=============================================================================
void CWnd_Status_Base::SetText_Title(__in LPCTSTR szTitle)
{
	if (::IsWindow(GetSafeHwnd()))
	{
		m_st_Title.SetText(szTitle);
	}
}

//=============================================================================
// Method		: SetColor_Title
// Access		: public  
// Returns		: void
// Parameter	: __in COLORREF clrTitle
// Qualifier	:
// Last Update	: 2021/11/26 - 12:23
// Desc.		:
//=============================================================================
void CWnd_Status_Base::SetColor_Title(__in COLORREF clrTitle)
{
	m_st_Title.SetBackColor_COLORREF(clrTitle);
}
