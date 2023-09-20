//*****************************************************************************
// Filename	: 	Wnd_Yield_Eqp.cpp
// Created	:	2021/11/19 - 11:14
// Modified	:	2021/11/19 - 11:14
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************

#include "stdafx.h"
#include "Wnd_Yield_Eqp.h"

#define RGB_PASS		RGB(200, 255, 200)
#define RGB_FAIL		RGB(255, 200, 200)

#define RGB_ENABLE		RGB(255, 255, 255)
#define RGB_DISABLE		RGB(100, 100, 100)
#define RGB_SKIP		RGB(130, 100, 100)

//-----------------------------------------------------------------------------
// CWnd_Yield_Eqp
//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CWnd_Yield_Eqp, CWnd)

CWnd_Yield_Eqp::CWnd_Yield_Eqp()
{
	m_Margin.left	= 0;
	m_Margin.top	= 0;
	m_Margin.right	= 0;
	m_Margin.bottom	= 0;
	m_nSpacing		= 5;
	m_nCateSpacing	= 10;

	m_nFontSize_Caption.SetSize(0, 13);
	m_nFontSize_Data.SetSize(0, 13);
}

CWnd_Yield_Eqp::~CWnd_Yield_Eqp()
{
	Release_Font();

}

BEGIN_MESSAGE_MAP(CWnd_Yield_Eqp, CWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CWnd_Yield_Eqp message handlers
//=============================================================================
// Method		: OnCreate
// Access		: protected  
// Returns		: int
// Parameter	: LPCREATESTRUCT lpCreateStruct
// Qualifier	:
// Last Update	: 2021/11/19 - 11:14
// Desc.		:
//=============================================================================
int CWnd_Yield_Eqp::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	Set_Background (RGB(0xFF, 0xFF, 0xFF));
	Create_Font();

 	DWORD dwStyle = WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
 	CRect rectDummy;
 	rectDummy.SetRectEmpty();


	//m_st_Title.SetFont_Gdip(L"Arial", 8.0F);
	m_st_Title.SetBorderTickness(1);
	//m_st_Title.SetTextAlignment(StringAlignmentNear);
	m_st_Title.Create(_T(""), dwStyle, rectDummy, this, IDC_STATIC);

	LPCTSTR szHeader_Judge[] =
	{
		_T("P"),
		_T("F"),
		_T("T"),
		NULL
	};

	LPCTSTR szHeader_Para[] =
	{
		_T("L"),
		_T("C"),
		_T("R"),
		_T("A"),
		NULL
	};

	m_st_Head_Judge[HJ_Pass].SetBackColor_COLORREF(RGB_PASS);
	m_st_Head_Judge[HJ_Fail].SetBackColor_COLORREF(RGB_FAIL);

	for (auto nJudIdx = 0; nJudIdx < HJ_MaxCount; nJudIdx++)
	{
		m_st_Head_Judge[nJudIdx].Create(szHeader_Judge[nJudIdx], dwStyle | SS_CENTER | SS_CENTERIMAGE, rectDummy, this, IDC_STATIC);
	}

	for (auto nParIdx = 0; nParIdx < HP_MaxCount; nParIdx++)
	{
		m_st_Head_Para[nParIdx].Create(szHeader_Para[nParIdx], dwStyle | SS_CENTER | SS_CENTERIMAGE, rectDummy, this, IDC_STATIC);
	}

	for (auto nJudIdx = 0; nJudIdx < HJ_MaxCount; nJudIdx++)
	{
		for (auto nParIdx = 0; nParIdx < HP_MaxCount; nParIdx++)
		{
			m_st_Data[nJudIdx][nParIdx].SetFont_Gdip(L"Arial", 6.0F);
			m_st_Data[nJudIdx][nParIdx].Create(_T(""), dwStyle | SS_CENTER | SS_CENTERIMAGE, rectDummy, this, IDC_STATIC);
		}
	}


	// 초기값 (테스트 용도)
	for (auto nJudIdx = 0; nJudIdx < HJ_MaxCount; nJudIdx++)
	{
		for (auto nParIdx = 0; nParIdx < HP_MaxCount; nParIdx++)
		{
			m_st_Data[nJudIdx][nParIdx].SetText(_T("0"));
		}
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
// Last Update	: 2021/11/19 - 11:14
// Desc.		:
//=============================================================================
void CWnd_Yield_Eqp::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	if ((cx == 0) && (cy == 0))
		return;

	int iHead_Width		= 14;
	int iHead_Height	= 16;
	int iLeft			= 0;
	int iTop			= 0;
	int iColCount		= m_nParaCount + 1; // = HP_MaxCount;
	int iWidth			= (cx - iHead_Width) / iColCount;
	int iHeight			= (cy - iHead_Height) / HJ_MaxCount;
	iHead_Width			= cx - (iWidth * iColCount) + iColCount;
	iHead_Height		= cy - (iHeight * HJ_MaxCount) + HJ_MaxCount;


	if (m_bLoader)
	{
		iHead_Width		= 100;
		iHead_Height	= 30;

		iWidth			= cx - iHead_Width;
		iHeight			= (cy - iHead_Height) / HJ_MaxCount;
		iHead_Height	= cy - (iHeight * HJ_MaxCount) + HJ_MaxCount;

		// 숨기기
		m_st_Title.MoveWindow(0, 0, 0, 0);
		for (auto nParIdx = 0; nParIdx < HP_MaxCount; nParIdx++)
		{
			m_st_Head_Para[nParIdx].MoveWindow(0, 0, 0, 0);
		}

		for (auto nJudIdx = 0; nJudIdx < HJ_MaxCount; nJudIdx++)
		{
			m_st_Head_Judge[nJudIdx].MoveWindow(0, 0, 0, 0);
			for (auto nParIdx = 0; nParIdx < HP_MaxCount; nParIdx++)
			{
				m_st_Data[nJudIdx][nParIdx].MoveWindow(0, 0, 0, 0);
			}
		}

		// 필요한것만
		m_st_Title.MoveWindow(0, iTop, iHead_Width, iHead_Height);

		iLeft = (iHead_Width - 1);
		m_st_Head_Para[HP_All].MoveWindow(iLeft, iTop, iWidth, iHead_Height);

		iTop += (iHead_Height - 1);
		for (auto nJudIdx = 0; nJudIdx < HJ_MaxCount; nJudIdx++)
		{
			iLeft = 0;
			m_st_Head_Judge[nJudIdx].MoveWindow(iLeft, iTop, iHead_Width, iHeight);

			iLeft += (iHead_Width - 1);
			m_st_Data[nJudIdx][HP_All].MoveWindow(iLeft, iTop, iWidth, iHeight);

			iTop += (iHeight - 1);
		}
	}
	else
	{
		m_st_Title.MoveWindow(0, iTop, iHead_Width, iHead_Height);
		//m_st_Border.MoveWindow(0, iTop, iWidth, iHeight);

		iLeft = (iHead_Width - 1);
		if (2 == m_nParaCount)
		{
			m_st_Head_Para[HP_Left].MoveWindow(iLeft, iTop, iWidth, iHead_Height);

			iLeft += (iWidth - 1);
			m_st_Head_Para[HP_Right].MoveWindow(iLeft, iTop, iWidth, iHead_Height);

			iLeft += (iWidth - 1);
			m_st_Head_Para[HP_All].MoveWindow(iLeft, iTop, iWidth, iHead_Height);

			iTop += (iHead_Height - 1);
			for (auto nJudIdx = 0; nJudIdx < HJ_MaxCount; nJudIdx++)
			{
				iLeft = 0;
				m_st_Head_Judge[nJudIdx].MoveWindow(iLeft, iTop, iHead_Width, iHeight);

				iLeft += (iHead_Width - 1);
				{
					m_st_Data[nJudIdx][HP_Left].MoveWindow(iLeft, iTop, iWidth, iHeight);

					iLeft += (iWidth - 1);
					m_st_Data[nJudIdx][HP_Right].MoveWindow(iLeft, iTop, iWidth, iHeight);

					iLeft += (iWidth - 1);
					m_st_Data[nJudIdx][HP_All].MoveWindow(iLeft, iTop, iWidth, iHeight);
				}

				iTop += (iHeight - 1);
			}
		}
		else // 3 Para
		{
			for (auto nParIdx = 0; nParIdx < HP_MaxCount; nParIdx++)
			{
				m_st_Head_Para[nParIdx].MoveWindow(iLeft, iTop, iWidth, iHead_Height);

				iLeft += (iWidth - 1);
			}

			iTop += (iHead_Height - 1);
			for (auto nJudIdx = 0; nJudIdx < HJ_MaxCount; nJudIdx++)
			{
				iLeft = 0;
				m_st_Head_Judge[nJudIdx].MoveWindow(iLeft, iTop, iHead_Width, iHeight);

				iLeft += (iHead_Width - 1);
				for (auto nParIdx = 0; nParIdx < HP_MaxCount; nParIdx++)
				{
					m_st_Data[nJudIdx][nParIdx].MoveWindow(iLeft, iTop, iWidth, iHeight);

					iLeft += (iWidth - 1);
				}

				iTop += (iHeight - 1);
			}
		}
	}
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
BOOL CWnd_Yield_Eqp::OnEraseBkgnd(CDC* pDC)
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
HBRUSH CWnd_Yield_Eqp::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
BOOL CWnd_Yield_Eqp::PreCreateWindow(CREATESTRUCT& cs)
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
BOOL CWnd_Yield_Eqp::PreTranslateMessage(MSG* pMsg)
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
void CWnd_Yield_Eqp::Create_Font()
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
void CWnd_Yield_Eqp::Release_Font()
{
	m_font_Default.DeleteObject();
	m_font_Data.DeleteObject();
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
void CWnd_Yield_Eqp::Set_Background(__in COLORREF color, __in BOOL bRepaint /*= TRUE*/)
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
void CWnd_Yield_Eqp::Set_Margin(__in UINT nLeft, __in UINT nTop, __in UINT nRight, __in UINT nBottom)
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
void CWnd_Yield_Eqp::Set_Spacing(__in UINT nSpacing, __in UINT nCateSpacing)
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
void CWnd_Yield_Eqp::Set_FontSize_Caption(__in UINT nWidth, __in UINT nHeghit)
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
void CWnd_Yield_Eqp::Set_FontSize_Data(__in UINT nWidth, __in UINT nHeghit)
{
	m_nFontSize_Data.SetSize(nWidth, nHeghit);
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
void CWnd_Yield_Eqp::SetText_Title(__in LPCTSTR szTitle)
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
void CWnd_Yield_Eqp::SetColor_Title(__in COLORREF clrTitle)
{
	m_st_Title.SetBackColor_COLORREF(clrTitle);
}

//=============================================================================
// Method		: Set_EnableWindow
// Access		: public  
// Returns		: void
// Parameter	: __in bool IN_bEnable
// Qualifier	:
// Last Update	: 2022/1/27 - 17:39
// Desc.		:
//=============================================================================
void CWnd_Yield_Eqp::Set_EnableWindow(__in bool IN_bEnable)
{
	if (IN_bEnable)
	{
		m_st_Title.SetBackColor_COLORREF(RGB_ENABLE);
		m_st_Head_Judge[HJ_Pass].SetBackColor_COLORREF(RGB_PASS);
		m_st_Head_Judge[HJ_Fail].SetBackColor_COLORREF(RGB_FAIL);
		m_st_Head_Judge[HJ_Total].SetBackColor_COLORREF(RGB_ENABLE);

		for (auto nParIdx = 0; nParIdx < HP_MaxCount; nParIdx++)
		{
			m_st_Head_Para[nParIdx].SetBackColor_COLORREF(RGB_ENABLE);
		}

		for (auto nJudIdx = 0; nJudIdx < HJ_MaxCount; nJudIdx++)
		{
			for (auto nParIdx = 0; nParIdx < HP_MaxCount; nParIdx++)
			{
				m_st_Data[nJudIdx][nParIdx].SetBackColor_COLORREF(RGB_ENABLE);
			}
		}
	}
	else
	{
		m_st_Title.SetBackColor_COLORREF(RGB_DISABLE);
		for (auto nParIdx = 0; nParIdx < HP_MaxCount; nParIdx++)
		{
			m_st_Head_Para[nParIdx].SetBackColor_COLORREF(RGB_DISABLE);
		}

		for (auto nJudIdx = 0; nJudIdx < HJ_MaxCount; nJudIdx++)
		{
			m_st_Head_Judge[nJudIdx].SetBackColor_COLORREF(RGB_DISABLE);

			for (auto nParIdx = 0; nParIdx < HP_MaxCount; nParIdx++)
			{
				m_st_Data[nJudIdx][nParIdx].SetBackColor_COLORREF(RGB_DISABLE);
			}
		}
	}
}

//=============================================================================
// Method		: Set_SkipWindow
// Access		: public  
// Returns		: void
// Parameter	: __in bool IN_bSkip
// Qualifier	:
// Last Update	: 2022/5/12 - 14:26
// Desc.		:
//=============================================================================
void CWnd_Yield_Eqp::Set_SkipWindow(__in bool IN_bSkip)
{
	if (IN_bSkip)
	{
		m_st_Title.SetBackColor_COLORREF(RGB_SKIP);
		for (auto nParIdx = 0; nParIdx < HP_MaxCount; nParIdx++)
		{
			m_st_Head_Para[nParIdx].SetBackColor_COLORREF(RGB_SKIP);
		}

		for (auto nJudIdx = 0; nJudIdx < HJ_MaxCount; nJudIdx++)
		{
			m_st_Head_Judge[nJudIdx].SetBackColor_COLORREF(RGB_SKIP);

			for (auto nParIdx = 0; nParIdx < HP_MaxCount; nParIdx++)
			{
				m_st_Data[nJudIdx][nParIdx].SetBackColor_COLORREF(RGB_SKIP);
			}
		}
	}
	else
	{
		m_st_Title.SetBackColor_COLORREF(RGB_ENABLE);
		m_st_Head_Judge[HJ_Pass].SetBackColor_COLORREF(RGB_PASS);
		m_st_Head_Judge[HJ_Fail].SetBackColor_COLORREF(RGB_FAIL);
		m_st_Head_Judge[HJ_Total].SetBackColor_COLORREF(RGB_ENABLE);

		for (auto nParIdx = 0; nParIdx < HP_MaxCount; nParIdx++)
		{
			m_st_Head_Para[nParIdx].SetBackColor_COLORREF(RGB_ENABLE);
		}

		for (auto nJudIdx = 0; nJudIdx < HJ_MaxCount; nJudIdx++)
		{
			for (auto nParIdx = 0; nParIdx < HP_MaxCount; nParIdx++)
			{
				m_st_Data[nJudIdx][nParIdx].SetBackColor_COLORREF(RGB_ENABLE);
			}
		}
	}
	
}

//=============================================================================
// Method		: Set_EqpLoader
// Access		: public  
// Returns		: void
// Parameter	: __in bool IN_bLoader
// Qualifier	:
// Last Update	: 2022/1/27 - 19:07
// Desc.		:
//=============================================================================
void CWnd_Yield_Eqp::Set_EqpLoader(__in bool IN_bLoader)
{
	m_bLoader = IN_bLoader;

	if (GetSafeHwnd())
	{
		if (m_bLoader)
		{
			m_st_Title.SetText(_T("Yield"));
			m_st_Head_Para[HP_All].SetText(_T(""));
			m_st_Head_Judge[HJ_Pass].SetText(_T("Pass"));
			m_st_Head_Judge[HJ_Fail].SetText(_T("Fail"));
			m_st_Head_Judge[HJ_Total].SetText(_T("Total"));

			for (auto nJudIdx = 0; nJudIdx < HJ_MaxCount; nJudIdx++)
			{
				for (auto nParIdx = 0; nParIdx < HP_MaxCount; nParIdx++)
				{
					m_st_Data[nJudIdx][nParIdx].SetFont_Gdip(L"Arial", 10.0F);
				}
			}

			for (auto nParIdx = 0; nParIdx <= HJ_Fail; nParIdx++)
			{
				m_st_Head_Para[nParIdx].ShowWindow(SW_HIDE);
			}

			for (auto nJudIdx = 0; nJudIdx < HJ_MaxCount; nJudIdx++)
			{
				for (auto nParIdx = 0; nParIdx <= HP_Right; nParIdx++)
				{
					m_st_Data[nJudIdx][nParIdx].ShowWindow(SW_HIDE);
				}
			}
		}
		else
		{
			m_st_Title.SetText(_T(""));
			m_st_Head_Para[HP_All].SetText(_T("A"));
			m_st_Head_Judge[HJ_Pass].SetText(_T("P"));
			m_st_Head_Judge[HJ_Fail].SetText(_T("F"));
			m_st_Head_Judge[HJ_Total].SetText(_T("T"));

			for (auto nJudIdx = 0; nJudIdx < HJ_MaxCount; nJudIdx++)
			{
				for (auto nParIdx = 0; nParIdx < HP_MaxCount; nParIdx++)
				{
					m_st_Data[nJudIdx][nParIdx].SetFont_Gdip(L"Arial", 6.0F);
				}
			}

			for (auto nParIdx = 0; nParIdx <= HJ_Fail; nParIdx++)
			{
				m_st_Head_Para[nParIdx].ShowWindow(SW_SHOW);
			}

			for (auto nJudIdx = 0; nJudIdx < HJ_MaxCount; nJudIdx++)
			{
				for (auto nParIdx = 0; nParIdx <= HP_Right; nParIdx++)
				{
					m_st_Data[nJudIdx][nParIdx].ShowWindow(SW_SHOW);
				}
			}
		}

		CRect rc;
		GetClientRect(rc);
		SendMessage(WM_SIZE, (WPARAM)SIZE_RESTORED, MAKELPARAM(rc.Width(), rc.Height()));
	}
}

//=============================================================================
// Method		: Set_Yield
// Access		: public  
// Returns		: void
// Parameter	: __in const CYield_Equipment * IN_pYield
// Qualifier	:
// Last Update	: 2022/1/27 - 18:13
// Desc.		:
//=============================================================================
void CWnd_Yield_Eqp::Set_Yield(__in const CYield_Equipment* IN_pYield)
{
	CString szText;

	szText.Format(_T("%d"), IN_pYield->dwTotal);
	m_st_Data[HJ_Total][HP_All].SetText(szText.GetBuffer());

	szText.Format(_T("%d"), IN_pYield->dwPass);
	m_st_Data[HJ_Pass][HP_All].SetText(szText.GetBuffer());

	szText.Format(_T("%d"), IN_pYield->dwFail);
	m_st_Data[HJ_Fail][HP_All].SetText(szText.GetBuffer());

	uint8_t nDataIdx = 0;
	for (auto nParIdx = 0; nParIdx <= HP_Right; ++nParIdx)
	{
		switch(nParIdx)
		{
		case HP_Left:
			nDataIdx = enEqpPara::Para_Left;
			break;

		case HP_Right:
			nDataIdx = enEqpPara::Para_Right;
			break;

		case HP_Center:
			nDataIdx = enEqpPara::Para_Center;
			break;
		}

		szText.Format(_T("%d"), IN_pYield->m_Para[nDataIdx].dwTotal);
		m_st_Data[HJ_Total][nParIdx].SetText(szText.GetBuffer());

		// 로더에서 양품은 L/R 구분이 없다.
		if (m_bLoader)
		{
			szText = _T("X");
		}
		else
		{
			szText.Format(_T("%d"), IN_pYield->m_Para[nDataIdx].dwPass);
		}
		m_st_Data[HJ_Pass][nParIdx].SetText(szText.GetBuffer());

		szText.Format(_T("%d"), IN_pYield->m_Para[nDataIdx].dwFail);
		m_st_Data[HJ_Fail][nParIdx].SetText(szText.GetBuffer());
	}

	if (m_bLoader)
	{
		if ((0 != IN_pYield->dwPass) && (0 != IN_pYield->dwTotal))
		{
			double dYieldRate = static_cast<double>(IN_pYield->dwPass) / static_cast<double>(IN_pYield->dwTotal) * 100.0f;

			CString szRate;
			szRate.Format(_T("%.2f %%"), dYieldRate);
			m_st_Head_Para[HP_All].SetText(szRate.GetBuffer());
		}
		else
		{
			m_st_Head_Para[HP_All].SetText(_T("0.00 %"));
		}
	}


// 	IN_pYield->dwTotal;
// 	IN_pYield->dwPass;
// 	IN_pYield->dwFail;
// 
// 	IN_pYield->m_Para[Para_Left].dwTotal;
// 	IN_pYield->m_Para[Para_Left].dwPass;
// 	IN_pYield->m_Para[Para_Left].dwFail;
// 
// 	IN_pYield->m_Para[Para_Right].dwTotal;
// 	IN_pYield->m_Para[Para_Right].dwPass;
// 	IN_pYield->m_Para[Para_Right].dwFail;
}

//=============================================================================
// Method		: Set_TestParaCount
// Access		: public  
// Returns		: void
// Parameter	: uint8_t IN_nCount
// Qualifier	:
// Last Update	: 2022/7/27 - 18:03
// Desc.		:
//=============================================================================
void CWnd_Yield_Eqp::Set_TestParaCount(uint8_t IN_nCount)
{
	if (2 < IN_nCount)
	{
		if (m_nParaCount != IN_nCount)
		{
			m_nParaCount = IN_nCount;

			if (GetSafeHwnd())
			{
				CRect rc;
				GetClientRect(rc);
				SendMessage(WM_SIZE, (WPARAM)SIZE_RESTORED, MAKELPARAM(rc.Width(), rc.Height()));
			}
		}
	}

}
