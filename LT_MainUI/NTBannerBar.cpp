//*****************************************************************************
// Filename	: 	NTBannerBar.cpp
//
// Created	:	2010/03/18
// Modified	:	2010/03/18 - 17:10
//
// Author	:	PiRing
//	
// Purpose	:	배너 & 메뉴 버튼 & 시계 출력용 윈도우
//*****************************************************************************
// NTBannerBar.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NTBannerBar.h"
//#include "NTVisualManager.h"
#include "resource.h"

#include "../TextDesigner/OutlineText.h"
#include "../TextDesigner/PngOutlineText.h"

#define			IDC_CLOCK	1

// CNTBannerBar

IMPLEMENT_DYNAMIC(CNTBannerBar, CPane)
//=============================================================================
// 생성자
//=============================================================================
CNTBannerBar::CNTBannerBar() : CPane  ()
	, m_hIcon           (NULL)
	, m_szIcon          (0, 0)
	, m_bParentActive   (false)
	, m_bParentMaximize (false)
{
	m_dwControlBarStyle = 0;

	m_nID_BnExit		= 0;
	m_nID_BnOption		= 1;

	m_wmOption			= NULL;
}

//=============================================================================
// 소멸자
//=============================================================================
CNTBannerBar::~CNTBannerBar()
{
	m_CaptionFont.DeleteObject();
}


BEGIN_MESSAGE_MAP(CNTBannerBar, CPane)
	ON_MESSAGE(WM_SETTEXT, OnSetText)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_BN_CLICKED(SC_CLOSE, &CNTBannerBar::OnExitProgram)
	ON_BN_CLICKED(SC_OPTION, &CNTBannerBar::OnOption)
END_MESSAGE_MAP()

//=============================================================================
// CNTBannerBar 메시지 처리기입니다.
//=============================================================================
// Method		: CNTBannerBar::PreCreateWindow
// Access		: virtual protected 
// Returns		: BOOL
// Parameter	: CREATESTRUCT & cs
// Qualifier	:
// Last Update	: 2010/03/18 - 17:19
// Desc.		: 
//=============================================================================
BOOL CNTBannerBar::PreCreateWindow (CREATESTRUCT& cs)
{
	// in Win4, status bars do not have a border at all, since it is provided by the client area.
	if ((m_dwStyle & (CBRS_ALIGN_ANY | CBRS_BORDER_ANY)) == CBRS_TOP)
	{
		m_dwStyle &= ~(CBRS_BORDER_ANY | CBRS_BORDER_3D);
	}

	return CPane::PreCreateWindow(cs);
}

//=============================================================================
// Method		: CNTBannerBar::Create
// Access		: public 
// Returns		: BOOL
// Parameter	: CWnd * pParentWnd
// Parameter	: UINT nID
// Qualifier	:
// Last Update	: 2010/03/18 - 17:19
// Desc.		: 
//=============================================================================
BOOL CNTBannerBar::Create (CWnd* pParentWnd, UINT nID)
{
	return CreateEx(pParentWnd, nID);
}

//=============================================================================
// Method		: CNTBannerBar::CreateEx
// Access		: public 
// Returns		: BOOL
// Parameter	: CWnd * pParentWnd
// Parameter	: UINT nID
// Qualifier	:
// Last Update	: 2010/03/18 - 17:19
// Desc.		: 
//=============================================================================
BOOL CNTBannerBar::CreateEx (CWnd* pParentWnd, UINT nID)
{
	ASSERT_VALID(pParentWnd);   // must have a parent

	DWORD dwStyle = WS_VISIBLE | WS_CHILD | CBRS_TOP;

	// save the style
	SetPaneAlignment (dwStyle & CBRS_ALL);

	//-------------------------------------------------------------------------
	// create the HWND
	//-------------------------------------------------------------------------
	CRect rect(0, 0, 0, 0);

	LPCTSTR lpszClass = AfxRegisterWndClass(CS_DBLCLKS, ::LoadCursor(NULL, IDC_ARROW), (HBRUSH)(COLOR_BTNFACE+1), NULL);

	m_dwControlBarStyle = 0; // can't float, resize, close, slide

	BOOL bSystemSmall = (pParentWnd->GetExStyle() & WS_EX_TOOLWINDOW) != 0;

	if (!CWnd::Create(lpszClass, NULL, dwStyle | WS_CLIPSIBLINGS, rect, pParentWnd, uiBannerBarID)) //nID))
	{
		return FALSE;
	}

	if (pParentWnd->IsKindOf (RUNTIME_CLASS (CFrameWndEx)))
	{
		((CFrameWndEx*) pParentWnd)->AddPane (this);
	}
	else
	{
		ASSERT (FALSE);
		return FALSE;
	}

	//-------------------------------------------------------------------------
	// Icon Image
	//-------------------------------------------------------------------------
	HINSTANCE hInstance = AfxFindResourceHandle (MAKEINTRESOURCE (nID), RT_GROUP_ICON);

	if (hInstance != NULL)
	{	
		CSize szIcon (::GetSystemMetrics (SM_CXSMICON), ::GetSystemMetrics (SM_CYSMICON));		

		//HICON hIcon = (HICON)::LoadImage (hInstance, MAKEINTRESOURCE (nID), IMAGE_ICON, szIcon.cx, szIcon.cy, LR_DEFAULTCOLOR);
		//HICON hIcon = (HICON)::LoadImage (hInstance, MAKEINTRESOURCE (nID), IMAGE_ICON, 128, 43, LR_DEFAULTCOLOR);
		HICON hIcon = (HICON)::LoadImage (hInstance, MAKEINTRESOURCE (nID), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);

		if (hIcon != NULL)
		{
			SetIcon (hIcon);
		}
	}

	//-------------------------------------------------------------------------
	// Caption
	//-------------------------------------------------------------------------
	CString strCaption;

	pParentWnd->GetWindowText (strCaption);

	if (strCaption.IsEmpty ())
	{
		BOOL bValidString;
		bValidString = strCaption.LoadString (nID);
	}

	if (!strCaption.IsEmpty ())
	{
		SetWindowText (strCaption.GetBuffer());
		strCaption.ReleaseBuffer();
	}

	NONCLIENTMETRICS ncm;
	ZeroMemory (&ncm, sizeof (NONCLIENTMETRICS));
	ncm.cbSize = sizeof (NONCLIENTMETRICS);

	SystemParametersInfo (SPI_GETNONCLIENTMETRICS, ncm.cbSize, &ncm, 0);

	m_SystemHeight  = ::GetSystemMetrics (bSystemSmall ? SM_CYSMCAPTION : SM_CYCAPTION);
	m_CaptionHeight = m_SystemHeight;

	SetCaptionFont (bSystemSmall ? ncm.lfSmCaptionFont : ncm.lfCaptionFont);

	CRect rt (0, 0, bSystemSmall ? ncm.iSmCaptionWidth : ncm.iCaptionWidth, bSystemSmall ? ncm.iSmCaptionHeight : ncm.iCaptionHeight);

	//-------------------------------------------------------------------------
	// 버튼 컨트롤
	//-------------------------------------------------------------------------
	/*CRect rtBtn (0, 0, 48, 64);
	m_szBtnSize = CSize(48, 64);

	m_BtnExit.SetImage(m_nID_BnExit, _T("PNG"), 48, 48);	
	m_BtnExit.Create (_T("종료"), BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE, rtBtn, this, SC_CLOSE);
	m_BtnExit.SetTooltip (_T("프로그램을 종료 합니다."));
	m_BtnExit.InitButton();

	m_BtnOption.SetImage(m_nID_BnOption, _T("PNG"), 48, 48);	
	m_BtnOption.Create (_T("설정"), BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE, rtBtn, this, SC_OPTION); //IDC_OPTION);	
	m_BtnOption.SetTooltip(_T("환경설정"));
	m_BtnOption.InitButton();
	m_BtnOption.SetMouseCursorHand();
	*/

#ifdef USE_CLOCK
	//-------------------------------------------------------------------------
	// 시간 표시
	//-------------------------------------------------------------------------	
	if (m_bUseClock)
	{
		//m_wndClock.SetFontSize (24, 36, 24);
		//m_wndClock.SetFontSize (20, 32, 20);
		//m_wndClock.SetFontSize(15, 24, 15);
		if (!m_wndClock.Create(_T("Clock"), WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, CRect(0, 0, 0, 0), this, IDC_CLOCK))
		{
			AfxMessageBox(_T("시계를 생성하지 못했습니다."));
		}
		m_wndClock.Start();
	}
#endif	

	return TRUE;
}

//=============================================================================
// Method		: CNTBannerBar::OnCreate
// Access		: protected 
// Returns		: int
// Parameter	: LPCREATESTRUCT lpCreateStruct
// Qualifier	:
// Last Update	: 2010/4/9 - 13:51
// Desc.		:
//=============================================================================
int CNTBannerBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CPane::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetPaneStyle((GetPaneStyle () & ~(CBRS_GRIPPER | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT)) | CBRS_SIZE_DYNAMIC);
	SetBorders ();

	return 0;
}


//=============================================================================
// Method		: CNTBannerBar::SetCaptionHeight
// Access		: public 
// Returns		: void
// Parameter	: int nHeight
// Qualifier	:
// Last Update	: 2010/03/18 - 17:18
// Desc.		: 
//=============================================================================
void CNTBannerBar::SetCaptionHeight (int nHeight)
{
	if (m_CaptionHeight != nHeight)
	{
		m_CaptionHeight = nHeight;

		RecalcLayout ();
	}
}

//=============================================================================
// Method		: CNTBannerBar::CalcFixedLayout
// Access		: public 
// Returns		: CSize
// Parameter	: BOOL
// Parameter	: BOOL
// Qualifier	:
// Last Update	: 2010/03/18 - 17:18
// Desc.		: 
//=============================================================================
CSize CNTBannerBar::CalcFixedLayout (BOOL, BOOL)
{
	ASSERT_VALID(this);

	// recalculate based on font height + icon height + borders
	TEXTMETRIC tm;
	{
		CClientDC dcScreen(NULL);
		HFONT hFont = GetCaptionFont ();

		HGDIOBJ hOldFont = dcScreen.SelectObject(hFont);
		VERIFY(dcScreen.GetTextMetrics(&tm));
		dcScreen.SelectObject(hOldFont);
	}
	
	//int nHeight = max ( max (tm.tmHeight, m_szIcon.cy), GetCaptionHeight ()) + 20;
	//int nHeight = max ( max (tm.tmHeight, 64), GetCaptionHeight ()) + 10;
	//int nHeight = max(max(tm.tmHeight, 48), GetCaptionHeight()) + 5;
	int nHeight = max(max(tm.tmHeight, 30), GetCaptionHeight()) + 5; // 2021.02.08

#ifdef USE_BANNER
	return CSize (32767, nHeight);// + CY_BORDER * 4 - rectSize.Height());
#else
	return CSize (32767, 10);
#endif
}

//=============================================================================
// Method		: CNTBannerBar::SetCaptionFont
// Access		: public 
// Returns		: void
// Parameter	: const LOGFONT & lf
// Qualifier	:
// Last Update	: 2010/03/18 - 17:18
// Desc.		: 
//=============================================================================
void CNTBannerBar::SetCaptionFont (const LOGFONT& lf)
{
	if (m_CaptionFont.GetSafeHandle () != NULL)
	{
		m_CaptionFont.DeleteObject ();
	}

	m_CaptionFont.CreateFontIndirect (&lf);
}

//=============================================================================
// Method		: CNTBannerBar::SetIcon
// Access		: public 
// Returns		: void
// Parameter	: HICON hIcon
// Qualifier	:
// Last Update	: 2010/03/18 - 17:18
// Desc.		: 
//=============================================================================
void CNTBannerBar::SetIcon (HICON hIcon)
{
	m_szIcon = CSize (0, 0);

	if (m_hIcon != NULL)
	{
		::DestroyIcon(m_hIcon);
	}

	m_hIcon = hIcon;

	if (m_hIcon != NULL)
	{
		ICONINFO ii;
		ZeroMemory (&ii, sizeof (ICONINFO));

		::GetIconInfo (m_hIcon, &ii);

		if (ii.hbmColor != NULL)
		{
			BITMAP bm;
			ZeroMemory (&bm, sizeof (BITMAP));

			if (::GetObject (ii.hbmColor, sizeof (BITMAP), &bm) != 0)
			{
				m_szIcon.cx = bm.bmWidth;
				m_szIcon.cy = bm.bmHeight;
			}
		}
	}
}

//=============================================================================
// Method		: CNTBannerBar::SetParentActive
// Access		: public 
// Returns		: void
// Parameter	: BOOL bParentActive
// Qualifier	:
// Last Update	: 2010/03/18 - 17:18
// Desc.		: 
//=============================================================================
void CNTBannerBar::SetParentActive (BOOL bParentActive)
{
	if (m_bParentActive != bParentActive)
	{
		m_bParentActive = bParentActive;
		Invalidate ();
	}
}

//=============================================================================
// Method		: CNTBannerBar::SetParentMaximize
// Access		: public 
// Returns		: void
// Parameter	: BOOL bParentMaximize
// Qualifier	:
// Last Update	: 2010/03/18 - 17:18
// Desc.		: 
//=============================================================================
void CNTBannerBar::SetParentMaximize (BOOL bParentMaximize)
{
	if (m_bParentMaximize != bParentMaximize)
	{
		m_bParentMaximize = bParentMaximize;

		Invalidate ();
	}
}

//=============================================================================
// Method		: Set_ClockFontSize
// Access		: public  
// Returns		: void
// Parameter	: __in UINT nDateFontSize
// Parameter	: __in UINT nTimeFontSize
// Parameter	: __in UINT nSecondFontSize
// Qualifier	:
// Last Update	: 2021/5/11 - 12:57
// Desc.		:
//=============================================================================
void CNTBannerBar::Set_ClockFontSize(__in UINT nDateFontSize, __in UINT nTimeFontSize, __in UINT nSecondFontSize)
{
#ifdef USE_CLOCK
	m_wndClock.SetFontSize (nDateFontSize, nTimeFontSize, nSecondFontSize);
#endif
}

//=============================================================================
// Method		: Set_ClockPosition_Adjust
// Access		: public  
// Returns		: void
// Parameter	: __in int nDate_x
// Parameter	: __in int nDate_y
// Parameter	: __in int nTime_x
// Parameter	: __in int nTime_y
// Parameter	: __in int nSec_x
// Parameter	: __in int nSec_y
// Qualifier	:
// Last Update	: 2021/5/11 - 13:31
// Desc.		:
//=============================================================================
void CNTBannerBar::Set_ClockPosition_Adjust(__in int nDate_x, __in int nDate_y, __in int nTime_x, __in int nTime_y, __in int nSec_x, __in int nSec_y)
{
#ifdef USE_CLOCK
	m_wndClock.Set_Adjust_Date		(nDate_x, nDate_y);
	m_wndClock.Set_Adjust_Time		(nTime_x, nTime_y);
	m_wndClock.Set_Adjust_Second	(nSec_x, nSec_y);
#endif
}

//=============================================================================
// Method		: CNTBannerBar::GetCaptionTextColor
// Access		: public 
// Returns		: COLORREF
// Qualifier	: const
// Last Update	: 2010/03/18 - 17:18
// Desc.		: 
//=============================================================================
COLORREF CNTBannerBar::GetCaptionTextColor () const
{
	return IsParentActive () ? afxGlobalData.clrCaptionText : afxGlobalData.clrInactiveCaptionText;
}

//=============================================================================
// Method		: CNTBannerBar::DoPaint
// Access		: virtual protected 
// Returns		: void
// Parameter	: CDC * pDCPaint
// Qualifier	:
// Last Update	: 2010/03/18 - 17:18
// Desc.		: 
//=============================================================================
void CNTBannerBar::DoPaint (CDC* pDCPaint)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pDCPaint);

	CRect rectClip;
	pDCPaint->GetClipBox (rectClip);

	CRect rect;
	GetClientRect(rect);

	CMemDC memDC (*pDCPaint, this);
	CDC* pDC = &memDC.GetDC ();

	CMFCVisualManager::GetInstance ()->OnFillBarBackground (pDC, this, rect, rectClip);

	HGDIOBJ  hOldFont    = pDC->SelectObject (GetCaptionFont());

	int      nOldMode    = pDC->SetBkMode    (TRANSPARENT);
	COLORREF crTextColor = pDC->SetTextColor (GetCaptionTextColor());
	COLORREF crBkColor   = pDC->SetBkColor   (afxGlobalData.clrBtnFace);
	//COLORREF crBkColor   = pDC->SetBkColor   (RGB(0,255,255));

#ifdef USE_BANNER
	//-------------------------------------------------------------------------
	// Icon
	//-------------------------------------------------------------------------
	if (m_hIcon != NULL && m_szIcon != CSize (0, 0))
	{
		CRect rectIcon (0, 0, m_SystemHeight, rect.Height ());

		int x = max (0, (rectIcon.Width ()  - m_szIcon.cx) / 2) + m_nIconOffset_Left; // 20은 마진 설정으로 조정
		int y = max (0, (rectIcon.Height () - m_szIcon.cy) / 2) + m_nIconOffset_Top;

		::DrawIconEx (pDC->GetSafeHdc (), x, y, m_hIcon, m_szIcon.cx, m_szIcon.cy, 0, NULL, DI_NORMAL);

		rect.left  += m_SystemHeight;	
	}

	//-------------------------------------------------------------------------
	// Caption Text
	//-------------------------------------------------------------------------
	if (!m_strCaption.IsEmpty ())
	{
		// 글자 출력 위치 설정
		//rect.left	+= 120; // *** 120 대신에 아이콘의 크기에 맞춰 입력해야함
		//pDC->DrawText (m_strCaption, rect, DT_END_ELLIPSIS | DT_SINGLELINE | DT_VCENTER);

		int iLeftMargin = m_szIcon.cx + m_nTextOffset_Left; // 60
		//---------------------------------------------------------------------

		using namespace Gdiplus;
		using namespace TextDesigner;
		Graphics graphics(pDC->GetSafeHdc());
		graphics.SetSmoothingMode(SmoothingModeAntiAlias);
		graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);

		FontFamily fontFamily(L"Arial Black");		
		StringFormat strformat;

		OutlineText text;
		//text.TextDblOutline(Color(255,255,255), Color(0,128,128), Color(0,255,0), 4, 4); // Green
		//text.TextDblOutline(Color(220,0,0), Color(0,128,128), Color(255,255,255), 3, 3); // Red
		//text.TextDblOutline(Color(220, 0, 0), Color(0, 128, 128), Color(255, 255, 255), 1, 1); // Red
		text.TextDblOutline(m_clrText, m_clrOutline1, m_clrOutline2, m_nThickness1, m_nThickness2); // Red

		text.EnableShadow(true);
		//text.Shadow(Color(128,0,0,0), 8, Point(4,8));
		text.Shadow(Color(128,0,0,0), 8, Point(1,2));
#ifdef _MBCS
		// convert the CString to wchar_t
		size_t cssize = strlen(m_strCaption) + 1;
		size_t wcsize = cssize*sizeof(wchar_t);		
		wchar_t *pwstr = (wchar_t*)malloc(wcsize);
		memset (pwstr, 0, wcsize);
		MultiByteToWideChar(CP_ACP, 0, m_strCaption, (int)cssize, pwstr, (int)wcsize);
		text.DrawString(&graphics, &fontFamily, FontStyleRegular, 44, pwstr, Gdiplus::Point(iLeftMargin, 0), &strformat);
		free(pwstr);
#else // Unicode
		//text.DrawString(&graphics, &fontFamily, FontStyleRegular, 44, m_strCaption, Gdiplus::Point(iLeftMargin, 0), &strformat);
		text.DrawString(&graphics, &fontFamily, FontStyleRegular, m_nTextFontSize, m_strCaption, Gdiplus::Point(iLeftMargin, m_nTextOffset_Top), &strformat); // 2021.02.08
#endif
		//---------------------------------------------------------------------
	}

#endif

	pDC->SetTextColor (crTextColor);
	pDC->SetBkColor   (crBkColor);
	pDC->SetBkMode    (nOldMode);

	pDC->SelectObject (hOldFont);

	ReleaseDC(pDC);
}


/////////////////////////////////////////////////////////////////////////////
// CNTBannerBar message handlers

//=============================================================================
// Method		: CNTBannerBar::OnSetText
// Access		: protected 
// Returns		: LRESULT
// Parameter	: WPARAM
// Parameter	: LPARAM lParam
// Qualifier	:
// Last Update	: 2010/03/18 - 17:18
// Desc.		: 
//=============================================================================
LRESULT CNTBannerBar::OnSetText (WPARAM, LPARAM lParam)
{
	TRY
	{
		m_strCaption.Empty();
		if (lParam != NULL)
		{
			m_strCaption = (LPCTSTR)lParam;
		}

		Invalidate ();
	}
	CATCH_ALL(e)
	{
		// Note: DELETE_EXCEPTION(e) not required
		return FALSE;
	}
	END_CATCH_ALL

		return TRUE;
}

//=============================================================================
// Method		: CNTBannerBar::OnSize
// Access		: protected 
// Returns		: void
// Parameter	: UINT nType
// Parameter	: int cx
// Parameter	: int cy
// Qualifier	:
// Last Update	: 2010/03/18 - 17:18
// Desc.		: 
//=============================================================================
void CNTBannerBar::OnSize (UINT nType, int cx, int cy) 
{
	CPane::OnSize(nType, cx, cy);

	//if (m_BtnOption.GetSafeHwnd () != NULL && m_BtnExit.GetSafeHwnd () != NULL)

	/*int nHeight = cy;

	DWORD dwFlag = SWP_NOOWNERZORDER | SWP_NOZORDER;

	CRect rt;
	m_BtnOption.GetWindowRect (rt);
	rt.OffsetRect (-rt.TopLeft ());

	if (m_szBtnSize == CSize (0, 0))
	{
		m_szBtnSize = rt.Size ();
	}

	if (m_szBtnSize.cy > nHeight)
	{
		rt.right   = m_szBtnSize.cy;
		rt.bottom  = m_szBtnSize.cy;
		rt.DeflateRect (1, 1);
	}
	else
	{
		rt.right   = m_szBtnSize.cx;
		rt.top     = (nHeight - m_szBtnSize.cy) / 2;
		rt.bottom  = rt.top + m_szBtnSize.cy;
	}*/

//	rt.OffsetRect (cx - rt.Width () - 20, -25); // 20은 마진	
//	m_BtnExit.SetWindowPos(&wndTop, rt.left, rt.bottom - 37, rt.Width(), rt.Height() + 16, dwFlag);

//	rt.OffsetRect(-rt.Width() - 1 - 20, 0); // 20은 마진	
//	m_BtnOption.SetWindowPos(&wndTop, rt.left, rt.bottom - 37, rt.Width(), rt.Height() + 16, dwFlag);

#ifdef USE_CLOCK
	if (m_bUseClock)
	{
		DWORD dwFlag = SWP_NOOWNERZORDER | SWP_NOZORDER;
		int	iLeft = cx - 200;
		int iTop = 5;

		if (m_wndClock.GetSafeHwnd())
		{
			//rt.OffsetRect(-220, -15);
			m_wndClock.SetWindowPos(&wndTop, iLeft, iTop, 180, 90, dwFlag);
		}
	}
#endif
}

//=============================================================================
// Method		: CNTBannerBar::OnClose
// Access		: protected 
// Returns		: void
// Qualifier	:
// Last Update	: 2010/4/15 - 17:12
// Desc.		:
//=============================================================================
void CNTBannerBar::OnExitProgram()
{
	CWnd* pParentWnd = GetParent ();

	if (pParentWnd != NULL)
	{
		pParentWnd->PostMessage (WM_SYSCOMMAND, SC_CLOSE);
	}
}

//=============================================================================
// Method		: CNTBannerBar::OnOption
// Access		: protected 
// Returns		: void
// Qualifier	:
// Last Update	: 2010/5/7 - 14:16
// Desc.		:
//=============================================================================
void CNTBannerBar::OnOption()
{
	CWnd* pParentWnd = GetParent ();

	if (pParentWnd != NULL)
	{
		pParentWnd->PostMessage (m_wmOption, NULL);
	}
}

//=============================================================================
// Method		: CNTBannerBar::PreTranslateMessage
// Access		: virtual protected 
// Returns		: BOOL
// Parameter	: MSG * pMsg
// Qualifier	:
// Last Update	: 2010/03/18 - 17:18
// Desc.		: 
//=============================================================================
BOOL CNTBannerBar::PreTranslateMessage (MSG* pMsg) 
{
	if (!CPane::PreTranslateMessage (pMsg))
	{
		return FALSE;
	}

	CWnd* pParentWnd = GetParent ();

	if (pParentWnd != NULL)
	{
		CPoint pt;
		::GetCursorPos (&pt);

		UINT uiHit = HitTest (pt);

		if (uiHit == HTCAPTION || uiHit == HTSYSMENU)
		{
			UINT   message = 0;
			WPARAM wParam  = uiHit;
			LPARAM lParam  = MAKELPARAM (pt.x, pt.y);

			switch (pMsg->message)
			{
			case WM_LBUTTONDOWN:
				message = WM_NCLBUTTONDOWN;
				break;
			case WM_LBUTTONUP:
				message = WM_NCLBUTTONUP;
				break;
			case WM_LBUTTONDBLCLK:
				message = WM_NCLBUTTONDBLCLK;
				break;
			}

			if (message != 0)
			{
				if (message == WM_NCLBUTTONDOWN && uiHit == HTSYSMENU)
				{
				}
				else
				{
					pParentWnd->SendMessage (message, wParam, lParam);
				}
			}
		}
	}

	return TRUE;
}

//=============================================================================
// Method		: CNTBannerBar::HitTest
// Access		: virtual protected 
// Returns		: UINT
// Parameter	: const CPoint & pt
// Qualifier	: const
// Last Update	: 2010/03/18 - 17:18
// Desc.		: 
//=============================================================================
UINT CNTBannerBar::HitTest (const CPoint& pt) const
{
	UINT uiHit = HTNOWHERE;

	CPoint ptHit  (pt);
	ScreenToClient (&ptHit);

	CRect rtClient;
	GetClientRect (rtClient);

	if (rtClient.PtInRect (ptHit))
	{
		uiHit = HTCAPTION;

		CRect rt (0, 0, m_SystemHeight, GetCaptionHeight ());

		if (m_hIcon != NULL && m_szIcon != CSize (0, 0))
		{
			if (rt.PtInRect (ptHit))
			{
				uiHit = HTSYSMENU;
			}
		}
	}

	return uiHit;
}