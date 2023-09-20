//*****************************************************************************
// Filename	: VGButton.cpp
// Created	: 2013/3/7 - 12:56
// Modified	: 2013/3/7 - 12:56
//
// Author	: PiRing
//	
// Purpose	: 
//*****************************************************************************
#include "stdafx.h"
#include "VGButton.h"

//=============================================================================
// CVGButton
//=============================================================================
IMPLEMENT_DYNAMIC(CVGButton, CButton)

CVGButton::CVGButton()
{
	m_nChecked		= BST_UNCHECKED;
	m_bHighlight	= FALSE;

	m_bTracking		= FALSE;

	m_pCachedBitmap = NULL;
}

CVGButton::~CVGButton()
{
	if (NULL != m_pCachedBitmap)
		delete m_pCachedBitmap;
}


BEGIN_MESSAGE_MAP(CVGButton, CButton)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_MESSAGE(BM_CLICK,	OnBMClick)
	ON_MESSAGE(BM_SETCHECK,	OnBMSetCheck)
	ON_MESSAGE(BM_GETCHECK,	OnBMGetCheck)
	ON_MESSAGE(BM_SETSTATE,	OnBMSetState)	
	ON_MESSAGE(BM_GETSTATE,	OnBMGetState)	
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	//ON_WM_MOUSEMOVE()
	//ON_WM_MOUSEHOVER()
	//ON_WM_MOUSELEAVE()
END_MESSAGE_MAP()

// CVGButton 메시지 처리기입니다.
//=============================================================================
// Method		: CVGButton::PreSubclassWindow
// Access		: virtual protected 
// Returns		: void
// Qualifier	:
// Last Update	: 2013/2/21 - 13:23
// Desc.		:
//=============================================================================
void CVGButton::PreSubclassWindow()
{
	//ModifyStyle(0, BS_OWNERDRAW | BS_PUSHLIKE, SWP_FRAMECHANGED);

	CButton::PreSubclassWindow();
}
//=============================================================================
// Method		: CVGButton::OnSize
// Access		: protected 
// Returns		: void
// Parameter	: UINT nType
// Parameter	: int cx
// Parameter	: int cy
// Qualifier	:
// Last Update	: 2013/2/21 - 13:24
// Desc.		:
//=============================================================================
void CVGButton::OnSize(UINT nType, int cx, int cy)
{
	CButton::OnSize(nType, cx, cy);	

	m_bRedrawPanel = TRUE;
}

//=============================================================================
// Method		: CVGButton::ReDrawPanel
// Access		: virtual protected 
// Returns		: void
// Qualifier	:
// Last Update	: 2013/2/21 - 13:24
// Desc.		:
//=============================================================================
void CVGButton::ReDrawPanel()
{
	//CVGBase_Panel::ReDrawPanel();
	m_bRedrawPanel = TRUE;

	if (GetSafeHwnd())
		Invalidate();
}

//=============================================================================
// Method		: CVGButton::OnPaint
// Access		: public 
// Returns		: void
// Qualifier	:
// Last Update	: 2013/2/21 - 13:33
// Desc.		:
//=============================================================================
void CVGButton::OnPaint()
{
	CPaintDC dc(this);
	Graphics graphics(dc.GetSafeHdc());	

	if((m_pCachedBitmap == NULL) || (m_bRedrawPanel))
	{
		CRect rect;
		GetClientRect(&rect);
			
		PaintCachedBitmap (graphics, Rect(rect.left, rect.top, rect.Width(), rect.Height()), m_bHighlight, m_nChecked);
	}	

	graphics.DrawCachedBitmap(m_pCachedBitmap, 0, 0);
	m_bRedrawPanel = FALSE;
}

//=============================================================================
// Method		: CVGButton::OnBMSetCheck
// Access		: protected 
// Returns		: LRESULT
// Parameter	: WPARAM nCheckState
// Parameter	: LPARAM
// Qualifier	:
// Last Update	: 2013/2/22 - 11:40
// Desc.		: 3State 일 경우에는 다르게 수정해야 한다.
//=============================================================================
LRESULT CVGButton::OnBMSetCheck(WPARAM nCheckState, LPARAM)
{	
	if (m_nChecked != nCheckState)
	{
		m_nChecked = (int)nCheckState;
		ReDrawPanel();
	}

	return 0;	
}

//=============================================================================
// Method		: CVGButton::OnBMGetCheck
// Access		: protected 
// Returns		: LRESULT
// Parameter	: WPARAM
// Parameter	: LPARAM
// Qualifier	:
// Last Update	: 2013/2/22 - 11:40
// Desc.		:
//=============================================================================
LRESULT CVGButton::OnBMGetCheck(WPARAM, LPARAM)
{	
	return m_nChecked;
}

//=============================================================================
// Method		: CVGButton::OnBMSetState
// Access		: protected 
// Returns		: LRESULT
// Parameter	: WPARAM state
// Parameter	: LPARAM
// Qualifier	:
// Last Update	: 2013/2/22 - 11:40
// Desc.		:
//=============================================================================
LRESULT CVGButton::OnBMSetState(WPARAM state, LPARAM)
{	
	if (m_bHighlight != (state & 0x0003))
	{
		m_bHighlight = (state & 0x0003);
		ReDrawPanel();
	}

	return 0;
}

//=============================================================================
// Method		: CVGButton::OnBMGetState
// Access		: protected 
// Returns		: LRESULT
// Parameter	: WPARAM
// Parameter	: LPARAM
// Qualifier	:
// Last Update	: 2013/2/22 - 11:40
// Desc.		:
// #define BST_UNCHECKED      0x0000
// #define BST_CHECKED        0x0001
// #define BST_INDETERMINATE  0x0002
// #define BST_PUSHED         0x0004
// #define BST_FOCUS          0x0008
//=============================================================================
LRESULT CVGButton::OnBMGetState(WPARAM, LPARAM)
{
	return m_bHighlight;
}

//=============================================================================
// Method		: CVGButton::OnBMClick
// Access		: protected 
// Returns		: LRESULT
// Parameter	: WPARAM
// Parameter	: LPARAM
// Qualifier	:
// Last Update	: 2013/2/22 - 15:48
// Desc.		:
//=============================================================================
LRESULT CVGButton::OnBMClick( WPARAM,LPARAM )
{
	BOOL bChanged = FALSE;

	switch (GetButtonStyle())
	{
	case BS_AUTORADIOBUTTON:
	case BS_RADIOBUTTON:
		if (!m_nChecked)
		{
			SetCheck(BST_CHECKED);
			bChanged = TRUE;
		}
		break;

	case BS_AUTOCHECKBOX:
	case BS_CHECKBOX:
		if (m_nChecked)
			SetCheck(BST_UNCHECKED);
		else
			SetCheck(BST_CHECKED);
		bChanged = TRUE;
		break;

	case BS_AUTO3STATE:
	case BS_3STATE:
		break;
	}

	if (bChanged)
	{
		CWnd* pParent	= GetParent();
		int nControlID	= GetDlgCtrlID();
		HWND hWnd		= GetSafeHwnd();

		if((pParent != NULL) && (hWnd != NULL))
			pParent->SendNotifyMessage(WM_COMMAND, MAKEWPARAM(nControlID, BN_CLICKED), (LPARAM)hWnd);
	}
	
	return 0;
}

//=============================================================================
// Method		: CVGButton::OnLButtonDown
// Access		: public 
// Returns		: void
// Parameter	: UINT nFlags
// Parameter	: CPoint point
// Qualifier	:
// Last Update	: 2013/2/22 - 15:45
// Desc.		:
//=============================================================================
void CVGButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	__super::OnLButtonDown(nFlags, point);
}

//=============================================================================
// Method		: CVGButton::OnLButtonUp
// Access		: public 
// Returns		: void
// Parameter	: UINT nFlags
// Parameter	: CPoint point
// Qualifier	:
// Last Update	: 2013/2/22 - 15:45
// Desc.		:
//=============================================================================
void CVGButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	CRect	rClient;
	GetClientRect(&rClient);

	if (rClient.PtInRect(point))
		OnBMClick (0, 0);	

	__super::OnLButtonUp(nFlags, point);
}

//=============================================================================
// Method		: CVGButton::OnLButtonDblClk
// Access		: public 
// Returns		: void
// Parameter	: UINT nFlags
// Parameter	: CPoint point
// Qualifier	:
// Last Update	: 2013/2/22 - 15:45
// Desc.		: BS_OWNERDRAW 일때는 이메세지 처리하지 않아도 된다.
//=============================================================================
void CVGButton::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CWnd* pParent	= GetParent();
	int nControlID	= GetDlgCtrlID();
	HWND hWnd		= GetSafeHwnd();

	if((pParent != NULL) && (hWnd != NULL))
		pParent->SendNotifyMessage(WM_COMMAND, MAKEWPARAM(nControlID, BN_DOUBLECLICKED), (LPARAM)hWnd);

	__super::OnLButtonDblClk(nFlags, point);
}

//=============================================================================
// Method		: CVGButton::OnMouseMove
// Access		: public 
// Returns		: void
// Parameter	: UINT nFlags
// Parameter	: CPoint point
// Qualifier	:
// Last Update	: 2013/3/8 - 9:36
// Desc.		:
//=============================================================================
void CVGButton::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize		= sizeof(tme);
		tme.dwFlags		= TME_HOVER | TME_LEAVE;
		tme.hwndTrack	= GetSafeHwnd();
		tme.dwHoverTime = 40;//0x00000001; //400;
		m_bTracking		= _TrackMouseEvent(&tme);
	}

	//__super::OnMouseMove(nFlags, point);
}

//=============================================================================
// Method		: CVGButton::OnMouseHover
// Access		: public 
// Returns		: void
// Parameter	: UINT nFlags
// Parameter	: CPoint point
// Qualifier	:
// Last Update	: 2013/3/7 - 17:01
// Desc.		:
//=============================================================================
void CVGButton::OnMouseHover(UINT nFlags, CPoint point)
{
 	if (FALSE == m_bHover)
 	{
 		m_bHover = TRUE;
 		
		if (!m_bChecked)
			ReDrawPanel();
 	}

	//__super::OnMouseHover(nFlags, point);
}

//=============================================================================
// Method		: CVGButton::OnMouseLeave
// Access		: public 
// Returns		: void
// Qualifier	:
// Last Update	: 2013/3/7 - 17:02
// Desc.		:
//=============================================================================
void CVGButton::OnMouseLeave()
{
 	if (TRUE == m_bHover)
 	{
 		m_bHover	= FALSE;
		m_bTracking	= FALSE;

		if (!m_bChecked)
 			ReDrawPanel();
 	}

	//__super::OnMouseLeave();
}

//=============================================================================
// Method		: CVGButton::PaintCachedBitmap
// Access		: virtual protected 
// Returns		: void
// Parameter	: Graphics & gfx
// Parameter	: Rect & argRect
// Parameter	: BOOL bSelected
// Parameter	: int nChecked
// Qualifier	:
// Last Update	: 2013/3/8 - 9:50
// Desc.		:
//=============================================================================
void CVGButton::PaintCachedBitmap( Graphics& gfx, Rect& argRect, BOOL bSelected /*= FALSE*/, int nChecked /*= BST_UNCHECKED*/ )
{
	Bitmap	*pBit	= new Bitmap(argRect.GetRight(), argRect.GetBottom(), &gfx);
	Graphics *memG	= new Graphics(pBit);	

	m_PanelColors.InitColor_Default();
	m_PanelColors.BackColor = Color::LightGray;
	//m_PanelStyle = PanelStyle_Default;
	m_PanelStyle = PanelStyle_Office2007;
	//m_CaptionStyle	= CaptionStyle_Flat;

	if (bSelected)
		m_bPressed = TRUE;
	else
		m_bPressed = FALSE;

	if (BST_CHECKED == nChecked)
		m_bChecked = true;
	else
		m_bChecked = false;	

	//DrawPanelAndBackground (*memG, argRect);
	DrawPanelWithContent (*memG, argRect);	// 컨텐츠를 사용하면..

	if (NULL != m_pCachedBitmap) 
	{
		delete m_pCachedBitmap;
		m_pCachedBitmap = NULL;
	}

	m_pCachedBitmap = new CachedBitmap(pBit, &gfx);
	delete pBit;
	delete memG;
}

//=============================================================================
// Method		: CVGButton::InitContent
// Access		: protected 
// Returns		: void
// Parameter	: UINT nCount
// Qualifier	:
// Last Update	: 2013/3/12 - 10:13
// Desc.		:
//=============================================================================
void CVGButton::InitContent( UINT nCount /*= 1*/ )
{
	//m_bContentsVertical = TRUE;

	Content	newContent;
	
	newContent.UseBackground	= TRUE;
	newContent.UseBorder		= TRUE;
	newContent.BackColor		= Color::White;
	newContent.BorderColor		= Color::Black;
	newContent.BorderTickness	= 1.0f;

	newContent.FontName			= L"Tahoma";
	newContent.Text				= L"000.0";	
	newContent.fontStyle		= FontStyleRegular;
	newContent.fontSize			= 10.0f;
	newContent.TextAlignment	= StringAlignmentCenter;
	newContent.TextColor		= Color::Black;

	for (UINT nCnt = 0; nCnt < nCount; nCnt++)
	{
		AddContent(newContent);
	}
}

//=============================================================================
// Method		: CVGButton::SetContentBackColorRedraw
// Access		: virtual public 
// Returns		: void
// Parameter	: Color clrBack
// Parameter	: UINT nContentIndex
// Qualifier	:
// Last Update	: 2013/6/18 - 15:43
// Desc.		:
//=============================================================================
void CVGButton::SetContentBackColorRedraw( Color clrBack, UINT nContentIndex /*= 0*/ )
{
	if ((int)nContentIndex < m_arrContent.GetCount ())
	{
		Content* pContent = (Content*)m_arrContent.GetData();
		pContent += nContentIndex;

		if (pContent->BackColor.GetValue() != clrBack.GetValue())
		{
			pContent->BackColor = clrBack;

			ReDrawPanel();
		}		
	}
}