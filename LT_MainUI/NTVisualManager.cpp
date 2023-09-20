//*****************************************************************************
// Filename	: 	NTVisualManager.cpp
//
// Created	:	
// Modified	:	2010/03/10 - 10:51
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
// NTVisualManager.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NTVisualManager.h"
#include "NTCaptionBar.h"
#include "NTBannerBar.h"
#include "NTTabViewBar.h"
#include "NTTabViewBarButton.h"
#include "NTLinksBar.h"
#include "afxtagmanager.h "
#include "NTStyle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WP_MINBUTTON		15
#define WP_MAXBUTTON		17
#define WP_CLOSEBUTTON		18
#define WP_RESTOREBUTTON	21

#define CBS_NORMAL			1
#define CBS_HOT				2
#define CBS_PUSHED			3
#define	CBS_DISABLED		4

IMPLEMENT_DYNCREATE(CNTVisualManager, CMFCVisualManagerOffice2007)

//=============================================================================
// Method		: _StretchBitmap
// Access		: public 
// Returns		: BOOL
// Parameter	: CDC * pDC
// Parameter	: const CRect & rectClient
// Parameter	: CBitmap & bmp
// Qualifier	:
// Last Update	: 2010/03/10 - 11:20
// Desc.		: 
//=============================================================================
BOOL _StretchBitmap (CDC* pDC, const CRect& rectClient, CBitmap& bmp)
{
	ASSERT_VALID (pDC);

	if (bmp.GetSafeHandle () != NULL)
	{
		BITMAP bm;
		ZeroMemory (&bm, sizeof (BITMAP));

		bmp.GetBitmap (&bm);

		CDC dc;
		dc.CreateCompatibleDC (pDC);

		// Select the bitmap into the in-memory DC
		CBitmap* pOldBitmap = dc.SelectObject (&bmp);

		int nOldStretchBltMode = pDC->SetStretchBltMode (COLORONCOLOR);

		pDC->StretchBlt(0, 0, rectClient.Width (), rectClient.Height (), &dc, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

		dc.SelectObject (pOldBitmap);

		pDC->SetStretchBltMode (nOldStretchBltMode);

		return true;
	}

	return false;
}

//=============================================================================
// Method		: _DrawReflectedGradient
// Access		: public 
// Returns		: void
// Parameter	: CDC * pDC
// Parameter	: COLORREF clrS
// Parameter	: COLORREF clrM
// Parameter	: COLORREF clrF
// Parameter	: const CRect & rect
// Parameter	: BOOL bHorz
// Qualifier	:
// Last Update	: 2010/03/10 - 11:20
// Desc.		: 
//=============================================================================
void _DrawReflectedGradient (CDC* pDC, COLORREF clrS, COLORREF clrM, COLORREF clrF, const CRect& rect, BOOL bHorz)
{
	ASSERT_VALID (pDC);

	CDrawingManager dm (*pDC);

	long x1 = rect.left;
	long x2 = rect.right;
	long y1 = rect.top;
	long y2 = rect.bottom;

	if (bHorz)
	{
		CRect rt (x1, y1, x2, (y1 + y2) / 2);
		dm.FillGradient (rt, clrM, clrS, TRUE);
		rt.top = rt.bottom;
		rt.bottom = y2;
		dm.FillGradient (rt, clrF, clrM, TRUE);
	}
	else
	{
		CRect rt (x1, y1, (x1 + x2) / 2, y2);
		dm.FillGradient (rt, clrS, clrM, FALSE);
		rt.left  = rt.right;
		rt.right = x2;
		dm.FillGradient (rt, clrM, clrF, FALSE);
	}
}

// CNTVisualManager
//=============================================================================
// 생성자
//=============================================================================
CNTVisualManager::CNTVisualManager()
{
	SetMenuShadowDepth  (0);

	m_MemoryDC.m_hDC	= NULL;
	m_MemoryFilled		= false;
	m_MemorySize		= CSize (0, 0);

	m_CaptionButtonSize = CSize (0, 0);
	m_MenuButtonSize	= CSize (0, 0);

	m_clrToolBarButtonTextRegular       = RGB (255, 255, 255);
	m_clrToolBarButtonTextPressed       = RGB (255, 223, 127);
	m_clrToolBarButtonTextHighlighted   = RGB (255, 223, 127);

	m_pfDrawThemeBackground = NULL;

	// Office 2007 Style
	m_clrTabViewBarButtonTextRegular	= RGB (232, 243, 253);
	m_clrTabViewBarButtonTextPressed	= RGB (  0,   0,   0);	
	m_clrTabViewBarButtonTextHighlighted= RGB (230,   0,   0);

	m_clrTabViewBarButtonBorder			= RGB (159, 124,  53);
	m_clrTabViewBarButtonBorderM		= RGB (231, 219, 191);
	m_clrTabViewBarButtonLine			= RGB (254, 217, 169);
	m_clrTabViewBarButtonF				= RGB (193, 157,  77);
	m_clrTabViewBarButtonL				= RGB (144, 104,  40);

	m_clrToolBarSeparatorS				= RGB(119, 162, 211);
	m_clrToolBarSeparatorM				= RGB(255, 255, 255);
	m_clrToolBarSeparatorF				= RGB(85, 136, 198);

#if (UI_COLOR_SET == UI_COLOR_SKYBLUE)

	m_brLinkBarBack.CreateSolidBrush (RGB ( 49,  85, 153)); // Blue

	m_clrTaskPaneGroupCaptionHighSpecDark	= RGB(  0,   0,   0);
	m_clrTaskPaneGroupCaptionHighSpecLight	= RGB(255, 255, 255);
	m_clrTaskPaneGroupCaptionSpecDark		= RGB(  0,   0,   0);
	m_clrTaskPaneGroupCaptionSpecLight		= RGB(255, 255, 255);
	m_clrTaskPaneGroupCaptionHighDark		= RGB(  0,   0,   0);
	m_clrTaskPaneGroupCaptionHighLight		= RGB(255, 255, 255);
	m_clrTaskPaneGroupCaptionDark			= RGB(  0,   0,   0);
	m_clrTaskPaneGroupCaptionLight			= RGB(255, 255, 255);

	SetStyle(Office2007_LunaBlue);

#else

	m_brLinkBarBack.CreateSolidBrush (RGB ( 30,  30, 30)); // Gray

	SetStyle(Office2007_ObsidianBlack);

#endif

	
	
}

//=============================================================================
// 소멸자
//=============================================================================
CNTVisualManager::~CNTVisualManager()
{
}

//=============================================================================
// CNTVisualManager 멤버 함수
//=============================================================================
//=============================================================================
// Method		: CNTVisualManager::LoadNTCaptionButtonsIcons
// Access		: public 
// Returns		: BOOL
// Parameter	: LPCTSTR lpszID
// Qualifier	:
// Last Update	: 2010/03/18 - 14:42
// Desc.		: 
//=============================================================================
BOOL CNTVisualManager::LoadNTCaptionButtonsIcons (LPCTSTR lpszID)
{
	CBitmap bmp;

	m_CaptionButtonSize = CSize (0, 0);

	if (bmp.LoadBitmap (lpszID))
	{
		BITMAP bm;
		ZeroMemory (&bm, sizeof (BITMAP));

		bmp.GetBitmap (&bm);

		if (m_CaptionButtonIconst.GetSafeHandle() != NULL && m_CaptionButtonIconst.GetImageCount () > 0)
		{
			m_CaptionButtonIconst.DeleteImageList ();
		}

		UINT nFlags = ILC_MASK;

		switch (bm.bmBitsPixel)
		{
		case 4 :
			nFlags |= ILC_COLOR4;
			break;
		case 8 :
			nFlags |= ILC_COLOR8;
			break;
		case 16:
			nFlags |= ILC_COLOR16;
			break;
		case 24:
			nFlags |= ILC_COLOR24;
			break;
		case 32:
			nFlags |= ILC_COLOR32;
			break;
		default:
			ASSERT (FALSE);
		}

		//m_CaptionButtonSize.cx = bm.bmWidth / 12;
		m_CaptionButtonSize.cx = bm.bmWidth / 15;
		m_CaptionButtonSize.cy = bm.bmHeight;

		//m_CaptionButtonIconst.Create (m_CaptionButtonSize.cx, m_CaptionButtonSize.cy, nFlags, 0, 3);
		m_CaptionButtonIconst.Create (m_CaptionButtonSize.cx, m_CaptionButtonSize.cy, nFlags, 0, 4);
		m_CaptionButtonIconst.Add (&bmp, RGB (255, 0, 255));

		return true;
	}

	return false;
}

//=============================================================================
// Method		: CNTVisualManager::NTDrawCaptionButton
// Access		: public 
// Returns		: void
// Parameter	: CDC * pDC
// Parameter	: CRect rect
// Parameter	: AFX_BUTTON_STATE state
// Parameter	: UINT id
// Qualifier	:
// Last Update	: 2010/03/18 - 14:42
// Desc.		: 
//=============================================================================
void CNTVisualManager::NTDrawCaptionButton (CDC* pDC, CRect rect, AFX_BUTTON_STATE state, UINT id)
{
	int nState	= CBS_NORMAL;
	int nPart	= 0;

	//if (m_CaptionButtonIconst.GetSafeHandle () != NULL && m_CaptionButtonIconst.GetImageCount () >= 12)
	if (m_CaptionButtonIconst.GetSafeHandle () != NULL && m_CaptionButtonIconst.GetImageCount () >= 15)
	{
		long index = 0;

		switch (id)
		{
		case SC_CLOSE:
			index = 9;
			break;
		case SC_MINIMIZE:
			index = 0;
			break;
		case SC_MAXIMIZE:
			index = 3;
			break;
		case SC_RESTORE:
			index = 6;
			break;
		case SC_OPTION:
			index = 12;
			break;
		default:
			ASSERT (FALSE);
		}

		switch (state)
		{
		case ButtonsIsRegular:	
			break;

		case ButtonsIsHighlighted:
			index += 1;
			break;
		case ButtonsIsPressed:
			index += 2;
			break;
		}

		m_CaptionButtonIconst.Draw (pDC, index, rect.TopLeft (), ILD_NORMAL);

		return;
	}

	if (m_pfDrawThemeBackground == NULL || m_hThemeWindow == NULL)
	{
		switch (state)
		{
		case ButtonsIsRegular:
		case ButtonsIsHighlighted:
			nState = 0;
			break;
		case ButtonsIsPressed:
			nState = DFCS_PUSHED;
			break;
		}

		switch (id)
		{
		case SC_CLOSE:
			nPart = DFCS_CAPTIONCLOSE;
			break;
		case SC_MINIMIZE:
			nPart = DFCS_CAPTIONMIN;
			break;
		case SC_MAXIMIZE:
			nPart = DFCS_CAPTIONMAX;
			break;
		case SC_RESTORE:
			nPart = DFCS_CAPTIONRESTORE;
			break;
		case SC_OPTION:
			break;
		default:
			ASSERT (FALSE);
		}

		pDC->DrawFrameControl (rect, DFC_CAPTION, nPart | nState);
		return;
	}

	switch (state)
	{
	case ButtonsIsRegular:
		nState = CBS_NORMAL;
		break;
	case ButtonsIsPressed:
		nState = CBS_PUSHED;
		break;
	case ButtonsIsHighlighted:
		nState = CBS_HOT;
		break;
	}

	switch (id)
	{
	case SC_CLOSE:
		nPart = WP_CLOSEBUTTON;
		break;
	case SC_MINIMIZE:
		nPart = WP_MINBUTTON;
		break;
	case SC_MAXIMIZE:
		nPart = WP_MAXBUTTON;
		break;
	case SC_RESTORE:
		nPart = WP_RESTOREBUTTON;
		break;
	case SC_OPTION:
		break;
	default:
		ASSERT (FALSE);
	}

#if (_MSC_VER >= 1700)
	if (NULL != m_pfDrawThemeBackground)
		(*m_pfDrawThemeBackground) (m_hThemeWindow, pDC->GetSafeHdc(), nPart, nState, &rect, 0);
#else
	(*m_pfDrawThemeBackground) (m_hThemeWindow, pDC->GetSafeHdc(), nPart, nState, &rect, 0);
#endif	
	//typedef HRESULT (__stdcall * DRAWTHEMEBACKGROUND)(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, const RECT *pRect, OPTIONAL const RECT *pClipRect);
}

//=============================================================================
// Method		: CNTVisualManager::OnUpdateSystemColors
// Access		: virtual protected 
// Returns		: void
// Qualifier	:
// Last Update	: 2010/03/18 - 14:42
// Desc.		: 
//=============================================================================
void CNTVisualManager::OnUpdateSystemColors ()
{
	CMFCVisualManagerOffice2007::OnUpdateSystemColors();

	m_clrMenuLight						= RGB (210, 232, 253);
	m_brMenuLight.DeleteObject ();
	m_brMenuLight.CreateSolidBrush (m_clrMenuLight);

	m_clrHighlight						= RGB (152, 192, 235);
	m_brHighlight.DeleteObject ();
	m_brHighlight.CreateSolidBrush (m_clrHighlight);

	m_clrHighlightDn					= RGB (152, 192, 235);
	m_brHighlightDn.DeleteObject ();
	m_brHighlightDn.CreateSolidBrush (m_clrHighlightDn);

	m_clrHighlightChecked				= RGB (152, 192, 235);
	m_brHighlightChecked.DeleteObject ();
	m_brHighlightChecked.CreateSolidBrush (m_clrHighlightChecked);

	m_clrMenuItemBorder 				= RGB (152, 192, 235);
	m_clrMenuBorder 					= RGB (128, 128, 128);
}

//=============================================================================
// Method		: CNTVisualManager::SetupMemoryBitmapSize
// Access		: public 
// Returns		: void
// Parameter	: CDC * pDC
// Parameter	: const CSize & size
// Qualifier	:
// Last Update	: 2010/03/18 - 14:42
// Desc.		: 
//=============================================================================
void CNTVisualManager::SetupMemoryBitmapSize (CDC* pDC, const CSize& size)
{
	if (pDC == NULL && size == CSize (0, 0))
	{
		return;
	}

	if (m_MemoryDC.GetSafeHdc () != NULL)
	{
		if (pDC && pDC->GetDeviceCaps (BITSPIXEL) != m_MemoryDC.GetDeviceCaps (BITSPIXEL))
		{
			m_MemoryBitmap.DeleteObject ();
			m_MemoryDC.DeleteDC ();
		}
	}

	if (m_MemoryDC.GetSafeHdc () == NULL)
	{
		m_MemoryDC.CreateCompatibleDC (pDC);
	}

	if (m_MemorySize != size || m_MemoryBitmap.GetSafeHandle () == NULL)
	{
		m_MemorySize = size;

		if (m_MemoryBitmap.GetSafeHandle () != NULL)
		{
			m_MemoryBitmap.DeleteObject ();
		}

		m_MemoryBitmap.CreateCompatibleBitmap (pDC, m_MemorySize.cx, m_MemorySize.cy);
		m_MemoryDC.SelectObject (m_MemoryBitmap);

		m_MemoryFilled = false;
	}

	if (m_NTFrameBitmap.GetSafeHandle () != NULL)
	{
		m_MemoryFilled = _StretchBitmap (&m_MemoryDC, CRect (CPoint (0, 0), m_MemorySize), m_NTFrameBitmap);
	}
}

//=============================================================================
// Method		: CNTVisualManager::OnFillBarBackground
// Access		: virtual protected 
// Returns		: void
// Parameter	: CDC * pDC
// Parameter	: CBasePane * pBar
// Parameter	: CRect rectClient
// Parameter	: CRect rectClip
// Parameter	: BOOL
// Qualifier	:
// Last Update	: 2010/03/18 - 14:42
// Desc.		: 
//=============================================================================
void CNTVisualManager::OnFillBarBackground (CDC* pDC, CBasePane* pBar,	CRect rectClient, CRect rectClip, BOOL /*bNCArea*/)
{
	if (m_NTFrameBitmap.GetSafeHandle () == NULL)
	{
#if (UI_COLOR_SET == UI_COLOR_SKYBLUE)
		m_NTFrameBitmap.LoadBitmap (_T("IDB_NT_FRAME_BKGND_B"));
#else
		m_NTFrameBitmap.LoadBitmap (_T("IDB_NT_FRAME_BKGND"));
#endif

		if (!m_MemoryFilled)
		{
			m_MemoryFilled = _StretchBitmap (&m_MemoryDC, CRect (CPoint (0, 0), m_MemorySize), m_NTFrameBitmap);
		}
	}

	ASSERT_VALID (pDC);
	ASSERT_VALID (pBar);

	if (rectClip.IsRectEmpty ())
	{
		rectClip = rectClient;
	}

	CRuntimeClass* pBarClass = pBar->GetRuntimeClass ();

	if (pBarClass == NULL || pBarClass->IsDerivedFrom (RUNTIME_CLASS (CMFCPopupMenuBar)))
	{
		pDC->FillRect (rectClip, &m_brMenuLight);

		return;
	}

	BOOL bDefault = true;

	if (pBarClass != NULL &&
		(pBarClass->IsDerivedFrom (RUNTIME_CLASS (CNTCaptionBar))
		|| pBarClass->IsDerivedFrom (RUNTIME_CLASS (CNTBannerBar))  
		|| pBarClass->IsDerivedFrom (RUNTIME_CLASS (CNTLinksBar)) 
		/*|| pBarClass->IsDerivedFrom (RUNTIME_CLASS (CNTTabViewBar))*/
		))
	{		
		if (!pBar->IsFloating ())
		{
			CRect rt (rectClient);

			CWnd* pWnd = pBar->GetTopLevelFrame ();
			if (pWnd != NULL)
			{
				CRect rtW;
				pBar->GetWindowRect (rtW);
				pWnd->ScreenToClient (rtW);

				rt.OffsetRect (rtW.TopLeft ());
			}

			int nOldStretchBltMode = pDC->SetStretchBltMode (COLORONCOLOR);

			pDC->BitBlt(rectClient.left, rectClient.top,
				rectClient.Width (), rectClient.Height (), 
				&m_MemoryDC, 
				rt.left, rt.top, 
				SRCCOPY);

			pDC->SetStretchBltMode (nOldStretchBltMode);

			bDefault = false;
						
			/*if (pBarClass->IsDerivedFrom(RUNTIME_CLASS(CNTTabViewBar)))
			{
				CDrawingManager dm(*pDC);

				CRect rect(rectClient);
				rect.top += 3;

				rect.bottom = rect.top + 1;
				dm.HighlightRect(rect, 15, (COLORREF)-1, 0, RGB(255, 255, 255));

				rect.top++;
				rect.bottom++;
				dm.HighlightRect(rect, 40, (COLORREF)-1, 0, RGB(255, 255, 255));

				rect.top++;
				rect.bottom = rectClient.bottom - 1;
				dm.HighlightRect(rect, 30, (COLORREF)-1, 0, RGB(255, 255, 255));

				rect.top = rect.bottom;
				rect.bottom++;
				dm.HighlightRect(rect, 10, (COLORREF)-1, 0, RGB(255, 255, 255));
			}
			else*/ if (pBarClass->IsDerivedFrom(RUNTIME_CLASS(CNTLinksBar)))
			{
				pDC->FillRect (rectClip, &m_brLinkBarBack);
				return;
			}
		}
	}
	else if (pBarClass != NULL && pBarClass->IsDerivedFrom(RUNTIME_CLASS(CNTTabViewBar)))
	{
		if (!pBar->IsFloating())
		{
			bDefault = false;

			pDC->FillRect(rectClient, &m_brLinkBarBack);

			CDrawingManager dm(*pDC);

			CRect rect(rectClient);
			rect.top += 3;

			rect.bottom = rect.top + 1;
			dm.HighlightRect(rect, 15, (COLORREF)-1, 0, RGB(150, 150, 150));

			rect.top++;
			rect.bottom++;
			dm.HighlightRect(rect, 40, (COLORREF)-1, 0, RGB(150, 150, 150));

			rect.top++;
			rect.bottom = rectClient.bottom - 1;
			dm.HighlightRect(rect, 30, (COLORREF)-1, 0, RGB(150, 150, 150));

			rect.top = rect.bottom;
			rect.bottom++;
			dm.HighlightRect(rect, 10, (COLORREF)-1, 0, RGB(150, 150, 150));
		}
	}

	if (bDefault)
	{
		//CMFCVisualManagerOffice2007::OnFillBarBackground (pDC, pBar, rectClient, rectClip);
		pDC->FillRect(rectClient, &m_brLinkBarBack);
	}
}

//=============================================================================
// Method		: CNTVisualManager::GetToolbarButtonTextColor
// Access		: virtual protected 
// Returns		: COLORREF
// Parameter	: CMFCToolBarButton * pButton
// Parameter	: CMFCVisualManager::AFX_BUTTON_STATE state
// Qualifier	:
// Last Update	: 2010/4/9 - 14:22
// Desc.		:
//=============================================================================
COLORREF CNTVisualManager::GetToolbarButtonTextColor (CMFCToolBarButton* pButton, CMFCVisualManager::AFX_BUTTON_STATE state)
{
	COLORREF clr = COLORREF (-1);

	if (pButton->IsKindOf (RUNTIME_CLASS (CNTTabViewBarButton)))
	{
		switch (state)
		{
		case ButtonsIsRegular:
			clr = m_clrTabViewBarButtonTextRegular;
			break;
		case ButtonsIsPressed:
			clr = m_clrTabViewBarButtonTextPressed;
			break;
		case ButtonsIsHighlighted:
			clr = m_clrTabViewBarButtonTextHighlighted;
			break;
		}
	}
	else
	{
		switch (state)
		{
		case ButtonsIsRegular:
			clr = m_clrToolBarButtonTextRegular;
			break;
		case ButtonsIsPressed:
			clr = m_clrToolBarButtonTextPressed;
			break;
		case ButtonsIsHighlighted:
			clr = m_clrToolBarButtonTextHighlighted;
			break;
		}
	}

	if (clr == COLORREF (-1))
	{
		clr = CMFCVisualManagerOffice2007::GetToolbarButtonTextColor (pButton, state);		
	}

	return clr;
}

//=============================================================================
// Method		: CNTVisualManager::OnFillHighlightedArea
// Access		: virtual protected 
// Returns		: void
// Parameter	: CDC * pDC
// Parameter	: CRect rect
// Parameter	: CBrush * pBrush
// Parameter	: CMFCToolBarButton * pButton
// Qualifier	:
// Last Update	: 2010/03/18 - 14:41
// Desc.		: 
//=============================================================================
void CNTVisualManager::OnFillHighlightedArea (CDC* pDC, CRect rect, CBrush* pBrush, CMFCToolBarButton* pButton)
{
	if (pButton == NULL)
	{
		CMFCVisualManagerOffice2007::OnFillHighlightedArea (pDC, rect, pBrush, pButton);
		return;
	}

	ASSERT_VALID (pDC);
	ASSERT_VALID (pBrush);

	CBrush* pBr = pBrush;

	BOOL bDefault = false;

	if (bDefault)
	{
		CMFCVisualManagerOfficeXP::OnFillHighlightedArea (pDC, rect, pBr, pButton);
		return;
	}
	else if (pButton->IsKindOf (RUNTIME_CLASS (CNTTabViewBarButton)))
	{
		if ((pButton->m_nStyle & TBBS_CHECKED) != 0)
		{
			CDrawingManager dm(*pDC);

			dm.FillGradient (rect, m_clrTabViewBarButtonL, m_clrTabViewBarButtonF, TRUE);
		}
	}
	else
	{
		CMFCVisualManagerOffice2007::OnFillHighlightedArea (pDC, rect, pBr, pButton);
	}
}

//=============================================================================
// Method		: CNTVisualManager::OnDrawButtonBorder
// Access		: virtual protected 
// Returns		: void
// Parameter	: CDC * pDC
// Parameter	: CMFCToolBarButton * pButton
// Parameter	: CRect rect
// Parameter	: AFX_BUTTON_STATE state
// Qualifier	:
// Last Update	: 2010/03/18 - 14:41
// Desc.		: 
//=============================================================================
void CNTVisualManager::OnDrawButtonBorder (CDC* pDC, CMFCToolBarButton* pButton, CRect rect, AFX_BUTTON_STATE state)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);

	if ((state != ButtonsIsPressed) && (state != ButtonsIsHighlighted))
	{
		ASSERT ((pButton->m_nStyle & TBBS_CHECKED) == 0);
		return;
	}

	if (pButton->IsKindOf (RUNTIME_CLASS (CNTTabViewBarButton)))
	{
		if (((CNTTabViewBarButton*)pButton)->Get_Enable())
		{
			if ((pButton->m_nStyle & TBBS_CHECKED) != 0)
			{
				_DrawReflectedGradient(pDC,
					m_clrTabViewBarButtonBorder,
					m_clrTabViewBarButtonBorderM,
					m_clrTabViewBarButtonBorder,
					CRect(rect.left, rect.top, rect.right, rect.top + 1),
					false
				);

				CDrawingManager dm(*pDC);

				dm.FillGradient(CRect(rect.left, rect.top, rect.left + 1, rect.bottom), m_clrTabViewBarButtonL, m_clrTabViewBarButtonBorder, TRUE);
				dm.FillGradient(CRect(rect.right - 1, rect.top, rect.right, rect.bottom), m_clrTabViewBarButtonL, m_clrTabViewBarButtonBorder, TRUE);

				{
					CPen pen(PS_SOLID, 0, m_clrTabViewBarButtonLine);

					CPen* pOldPen = pDC->SelectObject(&pen);

					pDC->MoveTo(rect.left + 1, rect.top + 1);
					pDC->LineTo(rect.right - 1, rect.top + 1);

					pDC->SelectObject(pOldPen);
				}

				{
					CPen pen(PS_SOLID, 0, m_clrTabViewBarButtonL);

					CPen* pOldPen = pDC->SelectObject(&pen);

					pDC->MoveTo(rect.left + 1, rect.bottom - 1);
					pDC->LineTo(rect.right - 1, rect.bottom - 1);

					pDC->SelectObject(pOldPen);
				}
			}
		}
	}
	else
	{
		CMFCVisualManagerOffice2007::OnDrawButtonBorder (pDC, pButton, rect, state);		
	}
}

//=============================================================================
// Method		: CNTVisualManager::CheckColor
// Access		: public 
// Returns		: CString
// Qualifier	:
// Last Update	: 2010/12/4 - 10:36
// Desc.		: 색상 체크용도
//=============================================================================
CString CNTVisualManager::CheckColor()
{	
	DWORD dwColorTable[] = 
	{
		m_clrBarGradientDark,
		m_clrBarGradientLight,
		m_clrToolBarGradientDark,
		m_clrToolBarGradientLight,
		m_clrToolbarDisabled,
		m_clrToolBarGradientVertLight,
		m_clrToolBarGradientVertDark,
		m_clrCustomizeButtonGradientDark,
		m_clrCustomizeButtonGradientLight,
		m_clrToolBarBottomLine,
		m_colorToolBarCornerTop,
		m_colorToolBarCornerBottom,
	};

	LPCTSTR lpszColorTable[] = 
	{
		_T("m_clrBarGradientDark"),
		_T("m_clrBarGradientLight"),
		_T("m_clrToolBarGradientDark"),
		_T("m_clrToolBarGradientLight"),
		_T("m_clrToolbarDisabled"),
		_T("m_clrToolBarGradientVertLight"),
		_T("m_clrToolBarGradientVertDark"),
		_T("m_clrCustomizeButtonGradientDark"),
		_T("m_clrCustomizeButtonGradientLight"),
		_T("m_clrToolBarBottomLine"),
		_T("m_colorToolBarCornerTop"),
		_T("m_colorToolBarCornerBottom"),
		NULL
	};

	CString strColor;
	CString strReturn;
	for (int iIndex = 0; iIndex < 12; iIndex++)
	{
		strColor.Format (_T("%s = RGB(%d, %d, %d)\r\n"),	lpszColorTable[iIndex],
			GetRValue (dwColorTable[iIndex]), 
			GetGValue (dwColorTable[iIndex]), 
			GetBValue (dwColorTable[iIndex]));

		strReturn += strColor;
	}

	return strReturn;
}

//=============================================================================
// Method		: CNTVisualManager::OnDrawSeparator
// Access		: virtual protected 
// Returns		: void
// Parameter	: CDC * pDC
// Parameter	: CBasePane * pBar
// Parameter	: CRect rect
// Parameter	: BOOL bHorz
// Qualifier	:
// Last Update	: 2011/6/3 - 9:54
// Desc.		:
//=============================================================================
void CNTVisualManager::OnDrawSeparator (CDC* pDC, CBasePane* pBar,CRect rect, BOOL bHorz)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pBar);

	if (pBar->IsDialogControl () || 
		pBar->IsKindOf (RUNTIME_CLASS (CMFCPopupMenuBar)) || 
		pBar->IsKindOf (RUNTIME_CLASS (CMFCColorBar)))
	{
		CMFCVisualManagerOffice2007::OnDrawSeparator (pDC, pBar, rect, bHorz);
		return;
	}


	CRect rectSeparator = rect;

	int x1, x2;
	int y1, y2;

	if (bHorz)
	{
		x1 = x2 = (rect.left + rect.right) / 2;
		y1 = rect.top;
		y2 = rect.bottom - 1;
	}
	else
	{
		y1 = y2 = (rect.top + rect.bottom) / 2;
		x1 = rect.left;
		x2 = rect.right;

		if (pBar->IsKindOf (RUNTIME_CLASS (CMFCPopupMenuBar)) &&
			!pBar->IsKindOf (RUNTIME_CLASS (CMFCColorBar)))
		{

			x1 = rect.left + CMFCToolBar::GetMenuImageSize ().cx + GetMenuImageMargin () + 1;

			CRect rectBar;
			pBar->GetClientRect (rectBar);

			if (rectBar.right - x2 < 50) // Last item in row
			{
				x2 = rectBar.right;
			}

			if (((CMFCPopupMenuBar*) pBar)->m_bDisableSideBarInXPMode)
			{
				x1 = 0;
			}

			//---------------------------------
			//	Maybe Quick Customize separator
			//---------------------------------
			if (pBar->IsKindOf (RUNTIME_CLASS (CMFCPopupMenuBar)))
			{
				CWnd* pWnd = pBar->GetParent();
				if (pWnd != NULL && pWnd->IsKindOf (RUNTIME_CLASS (CMFCPopupMenu)))
				{
					CMFCPopupMenu* pMenu = (CMFCPopupMenu*)pWnd;
					if (pMenu->IsCustomizePane())
					{
						x1 = rect.left + 2*CMFCToolBar::GetMenuImageSize ().cx + 3*GetMenuImageMargin () + 2;
					}
				}
			}
		}
	}

	if (x1 == x2)
	{
		x2++;
	}

	if (y1 == y2)
	{
		y2++;
	}

	_DrawReflectedGradient (pDC, m_clrToolBarSeparatorS, m_clrToolBarSeparatorM, m_clrToolBarSeparatorF, CRect (x1, y1, x2, y2), !!bHorz);
}

//=============================================================================
// Method		: CNTVisualManager::OnFillTasksPaneBackground
// Access		: virtual protected 
// Returns		: void
// Parameter	: CDC * pDC
// Parameter	: CRect rectWorkArea
// Qualifier	:
// Last Update	: 2010/03/18 - 14:40
// Desc.		: 
//=============================================================================
void CNTVisualManager::OnFillTasksPaneBackground(CDC* pDC, CRect rectWorkArea)
{
	CDrawingManager dm (*pDC);

	// Right ------------------------------------------------------------------
#if (UI_COLOR_SET == UI_COLOR_SKYBLUE)
	dm.FillGradient (rectWorkArea, RGB (226, 242, 254), RGB (203, 228, 252), FALSE);  // Skyblue
	CPen penLeft (PS_SOLID, 1, RGB (188, 218, 247));
#else
	dm.FillGradient(rectWorkArea, RGB(66, 66, 66), RGB(33, 33, 33), FALSE); // Gray
	CPen penLeft(PS_SOLID, 1, RGB(20, 20, 20)); // Gray
#endif
	CPen* pOldPen = pDC->SelectObject (&penLeft);
	pDC->MoveTo (rectWorkArea.left + 1, rectWorkArea.top);
	pDC->LineTo (rectWorkArea.left + 1, rectWorkArea.bottom);


	// Left -------------------------------------------------------------------
/*
#if (UI_COLOR_SET == UI_COLOR_SKYBLUE)
	dm.FillGradient (rectWorkArea, RGB (203, 228, 252), RGB (226, 242, 254), FALSE); // Skyblue
	CPen penLeft (PS_SOLID, 1, RGB (188, 218, 247)); // Skyblue
#else
	dm.FillGradient (rectWorkArea, RGB (53, 53, 53), RGB (96, 96, 96), FALSE); // Gray
	CPen penLeft (PS_SOLID, 1, RGB (20, 20, 20)); // Gray
#endif
	CPen* pOldPen = pDC->SelectObject (&penLeft);
	pDC->MoveTo (rectWorkArea.right - 1, rectWorkArea.top);
	pDC->LineTo (rectWorkArea.right - 1, rectWorkArea.bottom);
*/


	pDC->SelectObject (pOldPen);
}

//=============================================================================
// Method		: CNTVisualManager::OnDrawTask
// Access		: virtual protected 
// Returns		: void
// Parameter	: CDC * pDC
// Parameter	: CMFCTasksPaneTask * pTask
// Parameter	: CImageList * pIcons
// Parameter	: BOOL bIsHighlighted
// Parameter	: BOOL bIsSelected
// Qualifier	:
// Last Update	: 2010/03/18 - 14:40
// Desc.		: 
//=============================================================================
void CNTVisualManager::OnDrawTask(CDC* pDC, CMFCTasksPaneTask* pTask, CImageList* pIcons, BOOL bIsHighlighted, BOOL bIsSelected)
{
	if (pTask != NULL)
	{
#if (UI_COLOR_SET == UI_COLOR_SKYBLUE)
		pTask->m_clrText = pTask->m_clrTextHot = RGB (16, 37, 127);
#else
		pTask->m_clrText = pTask->m_clrTextHot = RGB (255, 255, 255);
#endif
	}

	CMFCVisualManagerOffice2007::OnDrawTask(pDC, pTask, pIcons, bIsHighlighted, bIsSelected);
}


//=============================================================================
// Method		: OnDrawTasksGroupCaption
// Access		: virtual protected  
// Returns		: void
// Parameter	: CDC * pDC
// Parameter	: CMFCTasksPaneTaskGroup * pGroup
// Parameter	: BOOL bIsHighlighted
// Parameter	: BOOL bIsSelected
// Parameter	: BOOL bCanCollapse
// Qualifier	:
// Last Update	: 2013/11/26 - 17:28
// Desc.		:
//=============================================================================
void CNTVisualManager::OnDrawTasksGroupCaption( CDC* pDC, CMFCTasksPaneTaskGroup* pGroup, BOOL bIsHighlighted /*= FALSE*/, BOOL bIsSelected /*= FALSE*/, BOOL bCanCollapse /*= FALSE*/ )
{
	if (pGroup != NULL)
	{
#if (UI_COLOR_SET == UI_COLOR_SKYBLUE)
		pGroup->m_clrText = pGroup->m_clrTextHot = RGB (55, 55, 255);
#else
		pGroup->m_clrText = pGroup->m_clrTextHot = RGB (255, 255, 0);
#endif
	}

/*
	CMFCVisualManagerOffice2007::OnDrawTasksGroupCaption(pDC, pGroup, bIsHighlighted, bIsSelected, bCanCollapse);
*/


	ASSERT_VALID(pDC);
	ASSERT_VALID(pGroup);
	ASSERT_VALID(pGroup->m_pPage);

	if (!CanDrawImage())
	{
		CMFCVisualManagerOffice2007::OnDrawTasksGroupCaption(pDC, pGroup, bIsHighlighted, bIsSelected, bCanCollapse);
		return;
	}

	CRect rectGroup = pGroup->m_rect;

	// -----------------------
	// Draw caption background
	// -----------------------
	/*CDrawingManager dm(*pDC);

	if (pGroup->m_bIsSpecial)
	{
		if (IsBeta())
		{
			dm.FillGradient(pGroup->m_rect, bIsHighlighted ? m_clrTaskPaneGroupCaptionHighSpecLight : m_clrTaskPaneGroupCaptionSpecLight,
											bIsHighlighted ? m_clrTaskPaneGroupCaptionHighSpecDark : m_clrTaskPaneGroupCaptionSpecDark, TRUE);
		}
		else
		{
			dm.Fill4ColorsGradient(pGroup->m_rect,	bIsHighlighted ? m_clrTaskPaneGroupCaptionHighSpecDark  : m_clrTaskPaneGroupCaptionSpecDark,
													bIsHighlighted ? m_clrTaskPaneGroupCaptionHighSpecLight : m_clrTaskPaneGroupCaptionSpecLight,
													bIsHighlighted ? m_clrTaskPaneGroupCaptionHighSpecLight : m_clrTaskPaneGroupCaptionSpecLight,
													bIsHighlighted ? m_clrTaskPaneGroupCaptionHighSpecDark  : m_clrTaskPaneGroupCaptionSpecDark, FALSE);
		}
	}
	else
	{
		if (IsBeta())
		{
			dm.FillGradient(pGroup->m_rect, bIsHighlighted ? m_clrTaskPaneGroupCaptionHighLight : m_clrTaskPaneGroupCaptionLight,
											bIsHighlighted ? m_clrTaskPaneGroupCaptionHighDark : m_clrTaskPaneGroupCaptionDark, TRUE);
		}
		else
		{
			dm.Fill4ColorsGradient(pGroup->m_rect,	bIsHighlighted ? m_clrTaskPaneGroupCaptionHighDark  : m_clrTaskPaneGroupCaptionDark,
													bIsHighlighted ? m_clrTaskPaneGroupCaptionHighLight : m_clrTaskPaneGroupCaptionLight,
													bIsHighlighted ? m_clrTaskPaneGroupCaptionHighLight : m_clrTaskPaneGroupCaptionLight,
													bIsHighlighted ? m_clrTaskPaneGroupCaptionHighDark  : m_clrTaskPaneGroupCaptionDark, FALSE);
		}
	}*/

	// ---------------------------
	// Draw an icon if it presents
	// ---------------------------
	BOOL bShowIcon = (pGroup->m_hIcon != NULL && pGroup->m_sizeIcon.cx < rectGroup.Width() - rectGroup.Height());
	if (bShowIcon)
	{
		OnDrawTasksGroupIcon(pDC, pGroup, 5, bIsHighlighted, bIsSelected, bCanCollapse);
	}

	// -----------------------
	// Draw group caption text
	// -----------------------
	CFont* pFontOld = pDC->SelectObject(&(GetGlobalData()->fontBold));
	COLORREF clrTextOld = pDC->GetTextColor();

	if (bCanCollapse && bIsHighlighted)
	{
		pDC->SetTextColor(pGroup->m_clrTextHot == (COLORREF)-1 ?
			(pGroup->m_bIsSpecial ? m_clrTaskPaneGroupCaptionTextHighSpec : m_clrTaskPaneGroupCaptionTextHigh) : pGroup->m_clrTextHot);
	}
	else
	{
		pDC->SetTextColor(pGroup->m_clrText == (COLORREF)-1 ?
			(pGroup->m_bIsSpecial ? m_clrTaskPaneGroupCaptionTextSpec : m_clrTaskPaneGroupCaptionText) : pGroup->m_clrText);
	}

	int nBkModeOld = pDC->SetBkMode(TRANSPARENT);

	int nTaskPaneHOffset = pGroup->m_pPage->m_pTaskPane->GetGroupCaptionHorzOffset();
	int nTaskPaneVOffset = pGroup->m_pPage->m_pTaskPane->GetGroupCaptionVertOffset();
	int nCaptionHOffset = (nTaskPaneHOffset != -1 ? nTaskPaneHOffset : m_nGroupCaptionHorzOffset);

	CRect rectText = rectGroup;
	rectText.left += (bShowIcon ? pGroup->m_sizeIcon.cx + 15: nCaptionHOffset);
	rectText.top += (nTaskPaneVOffset != -1 ? nTaskPaneVOffset : m_nGroupCaptionVertOffset);
	rectText.right = max(rectText.left, rectText.right -(bCanCollapse ? rectGroup.Height() : nCaptionHOffset));

	pDC->DrawText(pGroup->m_strName, rectText, DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS);

	pDC->SetBkMode(nBkModeOld);
	pDC->SelectObject(pFontOld);
	pDC->SetTextColor(clrTextOld);

	// -------------------------
	// Draw group caption button
	// -------------------------
	/*if (bCanCollapse && !pGroup->m_strName.IsEmpty())
	{
		CSize sizeButton = CMenuImages::Size();
		CRect rectButton = rectGroup;
		rectButton.left = max(rectButton.left, rectButton.right -(rectButton.Height() + 1) / 2 -(sizeButton.cx + 1) / 2);
		rectButton.top = max(rectButton.top, rectButton.bottom -(rectButton.Height() + 1) / 2 -(sizeButton.cy + 1) / 2);
		rectButton.right = rectButton.left + sizeButton.cx;
		rectButton.bottom = rectButton.top + sizeButton.cy;

		if (rectButton.right <= rectGroup.right && rectButton.bottom <= rectGroup.bottom)
		{
			if (bIsHighlighted)
			{
				// Draw button frame
				CBrush* pBrushOld = (CBrush*) pDC->SelectObject(&(GetGlobalData()->brBarFace));
				COLORREF clrBckOld = pDC->GetBkColor();

				pDC->Draw3dRect(&rectButton, GetGlobalData()->clrWindow, GetGlobalData()->clrBarShadow);

				pDC->SetBkColor(clrBckOld);
				pDC->SelectObject(pBrushOld);
			}

			CMenuImages::Draw(pDC, pGroup->m_bIsCollapsed ? CMenuImages::IdArrowDown : CMenuImages::IdArrowUp, rectButton.TopLeft(), CMenuImages::ImageBlack);
		}
	}*/
}

//=============================================================================
// Method		: OnDrawTasksGroupIcon
// Access		: virtual protected  
// Returns		: void
// Parameter	: CDC * pDC
// Parameter	: CMFCTasksPaneTaskGroup * pGroup
// Parameter	: int nIconHOffset
// Parameter	: BOOL bIsHighlighted
// Parameter	: BOOL bIsSelected
// Parameter	: BOOL bCanCollapse
// Qualifier	:
// Last Update	: 2013/11/27 - 0:48
// Desc.		:
//=============================================================================
void CNTVisualManager::OnDrawTasksGroupIcon( CDC* pDC, CMFCTasksPaneTaskGroup* pGroup, int nIconHOffset /*= 5*/, BOOL bIsHighlighted /*= FALSE*/, BOOL bIsSelected /*= FALSE*/, BOOL bCanCollapse /*= FALSE*/ )
{
	CMFCVisualManagerOffice2007::OnDrawTasksGroupIcon(pDC, pGroup, nIconHOffset, bIsHighlighted, bIsSelected, bCanCollapse);
}
