//*****************************************************************************
// Filename	: VGGroupWnd.cpp
// Created	: 2015/12/5 - 16:34
// Modified	: 2015/12/5 - 16:34
//
// Author	: PiRing
//	
// Purpose	: 
//*****************************************************************************
#include "stdafx.h"
#include "VGGroupWnd.h"

//=============================================================================
// CVGGroupWnd
//=============================================================================
IMPLEMENT_DYNAMIC(CVGGroupWnd, CButton)

CVGGroupWnd::CVGGroupWnd()
{
//  	m_panelColorizer.IsColorizerEnabled			= true;
//  	m_panelColorizer.IsTransparencyEnabled		= true;
//  	m_panelColorizer.Red						= 255;
//  	m_panelColorizer.Green						= 255;
//  	m_panelColorizer.Blue						= 255;
//  	m_panelColorizer.Alpha						= 250;
//  	m_panelColorizer.PaintingStyle				= AllPanel;
//  	m_panelColorizer.TextColor					= Color::White;	

	m_ColorizerHighlighted.IsColorizerEnabled	= true;
	m_ColorizerHighlighted.IsTransparencyEnabled= true;
	m_ColorizerHighlighted.Red					= 100;
	m_ColorizerHighlighted.Green				= 100;
	m_ColorizerHighlighted.Blue					= 150;
	m_ColorizerHighlighted.Alpha				= 150;
	m_ColorizerHighlighted.PaintingStyle		= AllPanel;
	m_ColorizerHighlighted.TextColor			= Color::LightYellow;

	m_ColorizerSelected.IsColorizerEnabled		= true;
	m_ColorizerSelected.IsTransparencyEnabled	= false;
	m_ColorizerSelected.Red						= 250;
	m_ColorizerSelected.Green					= 50;
	m_ColorizerSelected.Blue					= 50;
	m_ColorizerSelected.Alpha					= 50;
	m_ColorizerSelected.PaintingStyle			= AllPanel;	//OnlyCorners;
	m_ColorizerSelected.TextColor				= Color::Yellow;

	m_nChecked			= BST_UNCHECKED;
	m_nHighlight		= 0;


	m_bTimeCheckFlag	= FALSE;
}

CVGGroupWnd::~CVGGroupWnd()
{
}


BEGIN_MESSAGE_MAP(CVGGroupWnd, CButton)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
 	ON_MESSAGE(BM_CLICK,	OnBMClick)
 	ON_MESSAGE(BM_SETCHECK,	OnBMSetCheck)
 	ON_MESSAGE(BM_GETCHECK,	OnBMGetCheck)
 	ON_MESSAGE(BM_SETSTATE,	OnBMSetState)	
 	ON_MESSAGE(BM_GETSTATE,	OnBMGetState)	
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CVGGroupWnd 메시지 처리기입니다.
//=============================================================================
// Method		: CVGGroupWnd::PreSubclassWindow
// Access		: virtual protected 
// Returns		: void
// Qualifier	:
// Last Update	: 2015/12/5 - 16:34
// Desc.		:
//=============================================================================
void CVGGroupWnd::PreSubclassWindow()
{
	ModifyStyle(0, BS_OWNERDRAW | BS_GROUPBOX, SWP_FRAMECHANGED);

	CButton::PreSubclassWindow();
}
//=============================================================================
// Method		: CVGGroupWnd::OnSize
// Access		: protected 
// Returns		: void
// Parameter	: UINT nType
// Parameter	: int cx
// Parameter	: int cy
// Qualifier	:
// Last Update	: 2015/12/5 - 16:34
// Desc.		:
//=============================================================================
void CVGGroupWnd::OnSize(UINT nType, int cx, int cy)
{
	CButton::OnSize(nType, cx, cy);

	m_bRedrawPanel = TRUE;
}

//=============================================================================
// Method		: CVGGroupWnd::ReDrawPanel
// Access		: virtual protected 
// Returns		: void
// Qualifier	:
// Last Update	: 2015/12/5 - 16:34
// Desc.		:
//=============================================================================
void CVGGroupWnd::ReDrawPanel()
{
	CVS2008PanelBase::ReDrawPanel();

	if (GetSafeHwnd())
		Invalidate();
}

//=============================================================================
// Method		: CVGGroupWnd::OnPaint
// Access		: public 
// Returns		: void
// Qualifier	:
// Last Update	: 2015/12/5 - 16:34
// Desc.		:
//=============================================================================
void CVGGroupWnd::OnPaint()
{
	CPaintDC dc(this);
	DrawPanel(&dc);
}

//=============================================================================
// Method		: OnEraseBkgnd
// Access		: protected  
// Returns		: BOOL
// Parameter	: CDC * pDC
// Qualifier	:
// Last Update	: 2016/3/23 - 17:34
// Desc.		:
//=============================================================================
BOOL CVGGroupWnd::OnEraseBkgnd(CDC* pDC)
{
	DrawPanel(pDC);

	BOOL bReturn = __super::OnEraseBkgnd(pDC);

	OnRedrawControl();

	return bReturn;
}

//=============================================================================
// Method		: CVGGroupWnd::OnBMSetCheck
// Access		: protected 
// Returns		: LRESULT
// Parameter	: WPARAM nCheckState
// Parameter	: LPARAM
// Qualifier	:
// Last Update	: 2015/12/5 - 16:34
// Desc.		: 3State 일 경우에는 다르게 수정해야 한다.
//=============================================================================
LRESULT CVGGroupWnd::OnBMSetCheck(WPARAM nCheckState, LPARAM)
{	
	if (m_nChecked != nCheckState)
	{
		m_nChecked = (UINT)nCheckState;
		ReDrawPanel();
	}

	return 0;
}

//=============================================================================
// Method		: CVGGroupWnd::OnBMGetCheck
// Access		: protected 
// Returns		: LRESULT
// Parameter	: WPARAM
// Parameter	: LPARAM
// Qualifier	:
// Last Update	: 2015/12/5 - 16:34
// Desc.		:
//=============================================================================
LRESULT CVGGroupWnd::OnBMGetCheck(WPARAM, LPARAM)
{
	return m_nChecked;
}

//=============================================================================
// Method		: CVGGroupWnd::OnBMSetState
// Access		: protected 
// Returns		: LRESULT
// Parameter	: WPARAM state
// Parameter	: LPARAM
// Qualifier	:
// Last Update	: 2015/12/5 - 16:34
// Desc.		:
//=============================================================================
 LRESULT CVGGroupWnd::OnBMSetState(WPARAM state, LPARAM)
 {	
	BOOL bChanged = FALSE;
 
 	switch (GetButtonStyle())
 	{
 	case BS_AUTORADIOBUTTON:
 	case BS_RADIOBUTTON:
 	case BS_AUTOCHECKBOX:
 	case BS_CHECKBOX:
	case BS_AUTO3STATE:
 	case BS_3STATE:
 		if (m_nHighlight != state)
 		{
 			m_nHighlight = (BOOL)state;
 			ReDrawPanel();
 		}
 		break;
  	
 	case BS_GROUPBOX:
 		break;
 	}

 	return 0;
 }

//=============================================================================
// Method		: CVGGroupWnd::OnBMGetState
// Access		: protected 
// Returns		: LRESULT
// Parameter	: WPARAM
// Parameter	: LPARAM
// Qualifier	:
// Last Update	: 2015/12/5 - 16:34
// Desc.		:
//=============================================================================
 LRESULT CVGGroupWnd::OnBMGetState(WPARAM, LPARAM)
 {
 	return m_nHighlight;
 }

 //=============================================================================
 // Method		: DrawPanel
 // Access		: protected  
 // Returns		: void
 // Parameter	: CDC * pDC
 // Qualifier	:
 // Last Update	: 2016/3/23 - 17:35
 // Desc.		:
 //=============================================================================
 void CVGGroupWnd::DrawPanel(CDC* pDC)
 {
	 Graphics graphics(pDC->GetSafeHdc());

	 if ((m_pCachedBitmap == NULL) || (m_bRedrawPanel))
	 {
		 CRect rect;
		 GetClientRect(&rect);

		 if (m_nChecked)
		 {
			 m_bSelectedPanel = TRUE;
			 PaintCachedBitmap(graphics, Rect(rect.left, rect.top, rect.Width(), rect.Height()), m_ColorizerSelected);
		 }
		 else if (m_nHighlight)
		 {
			 m_bSelectedPanel = FALSE;
			 PaintCachedBitmap(graphics, Rect(rect.left, rect.top, rect.Width(), rect.Height()), m_ColorizerHighlighted);
		 }
		 else
		 {
			 m_bSelectedPanel = FALSE;
			 PaintCachedBitmap(graphics, Rect(rect.left, rect.top, rect.Width(), rect.Height()));
		 }
	 }

	 graphics.DrawCachedBitmap(m_pCachedBitmap, 0, 0);
	 m_bRedrawPanel = FALSE;
 }

 //=============================================================================
// Method		: CVGGroupWnd::OnBMClick
// Access		: protected 
// Returns		: LRESULT
// Parameter	: WPARAM
// Parameter	: LPARAM
// Qualifier	:
// Last Update	: 2015/12/5 - 16:34
// Desc.		:
//=============================================================================
 LRESULT CVGGroupWnd::OnBMClick( WPARAM,LPARAM )
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
 	case BS_GROUPBOX:
 		break;
 	}
 		
 	if (bChanged)
 	{
 		CWnd* pParent	= GetParent();
 		int nControlID	= GetDlgCtrlID();
 		HWND hWnd		= GetSafeHwnd();
 
 		if((pParent != NULL) && (hWnd != NULL))
 			pParent->SendMessage(WM_COMMAND, MAKEWPARAM(nControlID, BN_CLICKED), (LPARAM)hWnd);
 	}
 	
 	return 0;
 }

//=============================================================================
// Method		: CVGGroupWnd::OnLButtonDown
// Access		: public 
// Returns		: void
// Parameter	: UINT nFlags
// Parameter	: CPoint point
// Qualifier	:
// Last Update	: 2015/12/5 - 16:34
// Desc.		:
//=============================================================================
 void CVGGroupWnd::OnLButtonDown(UINT nFlags, CPoint point)
 {
 	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
 
 	__super::OnLButtonDown(nFlags, point);
 }

//=============================================================================
// Method		: CVGGroupWnd::OnLButtonUp
// Access		: public 
// Returns		: void
// Parameter	: UINT nFlags
// Parameter	: CPoint point
// Qualifier	:
// Last Update	: 2015/12/5 - 16:34
// Desc.		:
//=============================================================================
 void CVGGroupWnd::OnLButtonUp(UINT nFlags, CPoint point)
 {
 	CRect	rClient;
 	GetClientRect(&rClient);
 	if (rClient.PtInRect(point))
 		OnBMClick (0, 0);	
 
 	__super::OnLButtonUp(nFlags, point);
 }

//=============================================================================
// Method		: CVGGroupWnd::OnLButtonDblClk
// Access		: public 
// Returns		: void
// Parameter	: UINT nFlags
// Parameter	: CPoint point
// Qualifier	:
// Last Update	: 2015/12/5 - 16:34
// Desc.		: BS_OWNERDRAW 일때는 이메세지 처리하지 않아도 된다.
//=============================================================================
void CVGGroupWnd::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CWnd* pParent	= GetParent();
	int nControlID	= GetDlgCtrlID();
	HWND hWnd		= GetSafeHwnd();

	if((pParent != NULL) && (hWnd != NULL))
		pParent->SendMessage(WM_COMMAND, MAKEWPARAM(nControlID, BN_DOUBLECLICKED), (LPARAM)hWnd);


	__super::OnLButtonDblClk(nFlags, point);
}

//=============================================================================
// Method		: CVGGroupWnd::SetStatusBackColor
// Access		: public 
// Returns		: void
// Parameter	: Color clrBack
// Qualifier	:
// Last Update	: 2015/12/5 - 16:34
// Desc.		:
//=============================================================================
void CVGGroupWnd::SetStatusBackColor( Color clrBack )
{
	SetBackgroundColor(clrBack);
}

//=============================================================================
// Method		: OnRedrawControl
// Access		: virtual protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2016/3/28 - 16:39
// Desc.		:
//=============================================================================
void CVGGroupWnd::OnRedrawControl()
{

}

//=============================================================================
// Method		: CVGGroupWnd::UpdateStatusByTimer
// Access		: protected 
// Returns		: void
// Qualifier	:
// Last Update	: 2015/12/5 - 16:34
// Desc.		:
//=============================================================================
void CVGGroupWnd::UpdateStatusByTimer()
{
		
	ReDrawPanel();
}
