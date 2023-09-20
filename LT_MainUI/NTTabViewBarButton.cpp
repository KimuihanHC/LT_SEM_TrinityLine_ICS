//*****************************************************************************
// Filename	: 	NTTabViewBarButton.cpp
//
// Created	:	
// Modified	:	2010/04/02 - 13:19
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
// NTTabViewBarButton.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NTTabViewBarButton.h"
#include "NTVisualManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static const int		nTextMargin		= 3;
static const int		nSeparatorWidth = 8;
static const CString	strDummyAmpSeq	= _T("\001\001");

IMPLEMENT_SERIAL(CNTTabViewBarButton, CMFCToolBarButton, 1)

//=============================================================================
// CNTTabViewBarButton
//=============================================================================

//=============================================================================
// 생성자
//=============================================================================
CNTTabViewBarButton::CNTTabViewBarButton()
{
	Initialize ();
}

// CNTTabViewBarButton::CNTTabViewBarButton(LPCTSTR lpszLabel, UINT nID)
// {
// 	m_strText = lpszLabel;
// 	m_nID = nID;
// 
// 	Initialize();
// }

CNTTabViewBarButton::CNTTabViewBarButton(LPCTSTR lpszLabel, UINT nID, int nWidth)
{
	m_strText	= lpszLabel;
	m_nID		= nID;
	m_nStdWidth = nWidth;

	Initialize();
}

//=============================================================================
// 소멸자
//=============================================================================
CNTTabViewBarButton::~CNTTabViewBarButton()
{
}

//=============================================================================
// Method		: OnClick
// Access		: virtual protected  
// Returns		: BOOL
// Parameter	: CWnd * pWnd
// Parameter	: BOOL bDelay
// Qualifier	:
// Last Update	: 2021/12/22 - 16:47
// Desc.		:
//=============================================================================
BOOL CNTTabViewBarButton::OnClick(CWnd* pWnd, BOOL bDelay /*= TRUE*/)
{
	if (m_bEnable)
	{
		return __super::OnClick(pWnd, bDelay);
	}
	
	return TRUE;
}

//=============================================================================
// CNTTabViewBarButton 메시지 처리기입니다.
//=============================================================================
//=============================================================================
// Method		: CNTTabViewBarButton::Initialize
// Access		: protected 
// Returns		: void
// Qualifier	:
// Last Update	: 2010/04/02 - 13:15
// Desc.		: 
//=============================================================================
void CNTTabViewBarButton::Initialize()
{
	m_bText = TRUE;
}

//=============================================================================
// Method		: CNTTabViewBarButton::OnDraw
// Access		: virtual public 
// Returns		: void
// Parameter	: CDC * pDC
// Parameter	: const CRect & rect
// Parameter	: CMFCToolBarImages * pImages
// Parameter	: BOOL bHorz
// Parameter	: BOOL bCustomizeMode
// Parameter	: BOOL bHighlight
// Parameter	: BOOL bDrawBorder
// Parameter	: BOOL bGrayDisabledButtons
// Qualifier	:
// Last Update	: 2010/04/02 - 13:15
// Desc.		: 
//=============================================================================
void CNTTabViewBarButton::OnDraw(CDC* pDC,
								const CRect& rect,
								CMFCToolBarImages* pImages,
								BOOL bHorz /*= TRUE*/,
								BOOL bCustomizeMode /*= FALSE*/,
								BOOL bHighlight /*= FALSE*/,
								BOOL bDrawBorder /*= TRUE*/,
								BOOL bGrayDisabledButtons /*= TRUE*/)
{
	ASSERT_VALID(pDC);
	ASSERT_VALID(this);

	m_bHorz = bHorz;

	// Disable 상태이면 Highlight가 안되게 처리
	if (FALSE == m_bEnable)
	{
		bHighlight = FALSE;
	}

	//----------------------
	// Fill button interior:
	//----------------------
	FillInterior (pDC, rect, bHighlight);

	CSize sizeImage = (pImages == NULL) ? CSize (0, 0) : pImages->GetImageSize (TRUE);
	CRect rectInternal = rect;

	CFont* pOldFont = (CFont*)(pDC->SelectObject (&afxGlobalData.fontBold)); 

	CFont newFont;
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));

	pOldFont->GetLogFont(&lf);
	lf.lfHeight = 36;
	newFont.CreateFontIndirect(&lf);
	pDC->SelectObject (&newFont); 

	newFont.Detach();

	//--------------------
	// Draw button's text:
	//--------------------
	CMFCVisualManager::AFX_BUTTON_STATE state = CMFCVisualManager::ButtonsIsRegular;

	if (bHighlight)
	{
		state = CMFCVisualManager::ButtonsIsHighlighted;
	}
	else if (m_nStyle & (TBBS_PRESSED | TBBS_CHECKED))
	{
		// Pressed in or checked:
		state = CMFCVisualManager::ButtonsIsPressed;
	}

	// Disable 상태이면 글자색을 어둡게 표시
	COLORREF clrText = m_bEnable ? CMFCVisualManager::GetInstance ()->GetToolbarButtonTextColor (this, state) : RGB(20, 20, 20);

	pDC->SetTextColor (clrText);
	CString		strText		= m_strText;
	CRect		rectText	= rectInternal;

	if (bHorz)
	{
		pDC->DrawText (strText, &rectText, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	}
	
	pDC->SelectObject (pOldFont); 

// Disable 처리가 1회만 반영되고 다시 Enable 상태로 바뀜
// 	if (m_nStyle & TBBS_DISABLED)
// 	{
// 		TRACE(_T("Tab Button is Disabled\n"));
// 	}
// 
// 	if (FALSE == m_bEnable)
// 	{
// 		TRACE(_T("Tab Button is Disabled 2\n"));
// 	}

	//--------------------
	// Draw button border:
	//--------------------
	if (m_bEnable)
	{
		if (!bCustomizeMode && HaveHotBorder() && bDrawBorder)
		{
			if (m_nStyle & (TBBS_PRESSED | TBBS_CHECKED))
			{
				//-----------------------
				// Pressed in or checked:
				//-----------------------
				CMFCVisualManager::GetInstance()->OnDrawButtonBorder(pDC, this, rect, CMFCVisualManager::ButtonsIsPressed);
			}
			else if (bHighlight && !(m_nStyle & TBBS_DISABLED) && !(m_nStyle & (TBBS_CHECKED | TBBS_INDETERMINATE)))
			{
				CMFCVisualManager::GetInstance()->OnDrawButtonBorder(pDC, this, rect, CMFCVisualManager::ButtonsIsHighlighted);
			}
			//else
			//{
			//	TRACE(_T("Tab Button No Border\n"));
			//}
		}
	}

}

//=============================================================================
// Method		: CNTTabViewBarButton::OnCalculateSize
// Access		: virtual protected 
// Returns		: SIZE
// Parameter	: CDC * pDC
// Parameter	: const CSize & sizeDefault
// Parameter	: BOOL bHorz
// Qualifier	:
// Last Update	: 2010/04/02 - 13:15
// Desc.		: 
//=============================================================================
SIZE CNTTabViewBarButton::OnCalculateSize( CDC* pDC, const CSize& sizeDefault, BOOL bHorz )
{
	ASSERT_VALID (pDC);

	CNTVisualManager* pManager = DYNAMIC_DOWNCAST (CNTVisualManager, CMFCVisualManager::GetInstance ());

	if (pManager == NULL)
	{
		return CMFCToolBarButton::OnCalculateSize (pDC, sizeDefault, bHorz);
	}

	if(!IsVisible())
	{
		return CSize(0,0);
	}

	CSize size = sizeDefault;

	if ((m_nStyle & TBBS_SEPARATOR) == 0)
	{
		if (!IsDrawImage () || GetImage () < 0)
		{
			CSize sizeExtra = m_bExtraSize ? CMFCVisualManager::GetInstance ()->GetButtonExtraBorder () : CSize (0, 0);

			if (bHorz)
			{
				size.cx = sizeExtra.cx;
			}
			else
			{
				size.cy = sizeExtra.cy;
			}
		}

		m_sizeText = CSize (0, 0);


// 		if (!m_strText.IsEmpty () && IsDrawText ())
// 		{
// 			CFont* pOldFont = (CFont*)(pDC->SelectObject (&afxGlobalData.fontBold)); 
// 
// 			CRect rectText (0, 0, sizeDefault.cx * 3, sizeDefault.cy);
// 
// 			UINT uiTextFormat = DT_CENTER | DT_CALCRECT;
// 			if (m_bWrapText)
// 			{
// 				uiTextFormat |= DT_WORDBREAK;
// 			}
// 
// 			pDC->DrawText (	m_strText, rectText, uiTextFormat);
// 			m_sizeText = rectText.Size ();
// 			m_sizeText.cx += 2 * nTextMargin;			
// 
// 			//size.cx = max (size.cx, m_sizeText.cx) + 6 * nTextMargin;
// 			size.cx = m_nStdWidth + 6 * nTextMargin;
// 			//size.cx = 210 + 6 * nTextMargin;
// 
// 			//size.cy = m_sizeText.cy + 11;
// 			size.cy = 80;
// 
// 			pDC->SelectObject (pOldFont);
// 		}

		if (!m_strText.IsEmpty() && IsDrawText())
		{
			CFont* pOldFont = (CFont*)(pDC->SelectObject(&afxGlobalData.fontBold));

			//CRect rectText(0, 0, sizeDefault.cx * 3, sizeDefault.cy);
			CRect rectText(0, 0, sizeDefault.cx * 3, 80);

			UINT uiTextFormat = DT_CENTER | DT_VCENTER | DT_CALCRECT;
			if (m_bWrapText)
			{
				uiTextFormat |= DT_WORDBREAK;
			}

			pDC->DrawText(m_strText, rectText, uiTextFormat);
			m_sizeText = rectText.Size();
			m_sizeText.cx += 2 * nTextMargin;

			//size.cx = max (size.cx, m_sizeText.cx) + 6 * nTextMargin;
			size.cx = m_nStdWidth + 6 * nTextMargin;
			//size.cx = 210 + 6 * nTextMargin;

			//size.cy = m_sizeText.cy + 11;
			size.cy = 60;

			pDC->SelectObject(pOldFont);
		}
	}

	return size;
}

//=============================================================================
// Method		: Set_StandardWidth
// Access		: public  
// Returns		: void
// Parameter	: __in int IN_nWidth
// Qualifier	:
// Last Update	: 2021/4/30 - 11:43
// Desc.		:
//=============================================================================
void CNTTabViewBarButton::Set_StandardWidth(__in int IN_nWidth)
{
	m_nStdWidth = IN_nWidth;
}


