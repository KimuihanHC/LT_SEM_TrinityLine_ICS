//*****************************************************************************
// Filename	: ChMonCtrl.cpp
// Created	: 2013/7/2
// Modified	: 2013/7/2 - 10:00
//
// Author	: PiRing
//	
// Purpose	: 
//*****************************************************************************
#include "stdafx.h"
#include "ChMonCtrl.h"

//=============================================================================
// CChMonCtrl
//=============================================================================
IMPLEMENT_DYNAMIC(CChMonCtrl, CButton)

CChMonCtrl::CChMonCtrl()
{
	m_iVgSpacing		= 2;//4;
	//---------------------------------------------------------------
	m_iContentSpacing	= 0;//3;
	m_iContentPadding	= 0;//5;
	//---------------------------------------------------------------
	m_CaptionStyle		= CaptionStyle_Flat;//CaptionStyle_Normal;
	m_PanelStyle		= PanelStyle_Office2007;//PanelStyle_Default;
	m_DockStyle			= DockStyle_Top;
	m_iCaptionHeight	= 18;
	m_iCaptionWidth		= 30;//38;

	m_FontSize			= 9.0f;
	m_pFontCaption		= new Gdiplus::Font(L"Segoe UI", m_FontSize, FontStyleBold);
	m_strCaptionText	= L"";
	m_CaptionAlignment	= StringAlignmentCenter;

	m_pImage			= NULL;	

	m_SmoothingMode		= SmoothingModeHighSpeed;//SmoothingModeAntiAlias;
	m_BorderTickness	= 2;
	m_bChecked			= FALSE;
	m_bPressed			= FALSE;
	m_PanelColors.InitColor_Default();
	m_PanelColors.BackColor = Color::LightGray;		

	m_bRedrawPanel		= TRUE;
	
	//---------------------------------------------------------------
	m_nChecked		= BST_UNCHECKED;
	m_bHighlight	= FALSE;
	m_pCachedBitmap = NULL;

	InitContent ();
}

CChMonCtrl::~CChMonCtrl()
{
	if (NULL != m_pFontCaption)
		delete m_pFontCaption;

	if (NULL != m_pCachedBitmap)
		delete m_pCachedBitmap;
}


BEGIN_MESSAGE_MAP(CChMonCtrl, CButton)
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
END_MESSAGE_MAP()

// CChMonCtrl 메시지 처리기입니다.
//=============================================================================
// Method		: CChMonCtrl::PreSubclassWindow
// Access		: virtual protected 
// Returns		: void
// Qualifier	:
// Last Update	: 2013/2/21 - 13:23
// Desc.		:
//=============================================================================
void CChMonCtrl::PreSubclassWindow()
{
	//ModifyStyle(0, BS_OWNERDRAW | BS_PUSHLIKE, SWP_FRAMECHANGED);

	CButton::PreSubclassWindow();
}
//=============================================================================
// Method		: CChMonCtrl::OnSize
// Access		: protected 
// Returns		: void
// Parameter	: UINT nType
// Parameter	: int cx
// Parameter	: int cy
// Qualifier	:
// Last Update	: 2013/2/21 - 13:24
// Desc.		:
//=============================================================================
void CChMonCtrl::OnSize(UINT nType, int cx, int cy)
{
	CButton::OnSize(nType, cx, cy);	

	m_bRedrawPanel = TRUE;
}

//=============================================================================
// Method		: CChMonCtrl::ReDrawPanel
// Access		: virtual protected 
// Returns		: void
// Qualifier	:
// Last Update	: 2013/2/21 - 13:24
// Desc.		:
//=============================================================================
void CChMonCtrl::ReDrawPanel()
{
	m_bRedrawPanel = TRUE;

	if (GetSafeHwnd())
		Invalidate();
}

//=============================================================================
// Method		: CChMonCtrl::OnPaint
// Access		: public 
// Returns		: void
// Qualifier	:
// Last Update	: 2013/2/21 - 13:33
// Desc.		:
//=============================================================================
void CChMonCtrl::OnPaint()
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
// Method		: CChMonCtrl::OnBMSetCheck
// Access		: protected 
// Returns		: LRESULT
// Parameter	: WPARAM nCheckState
// Parameter	: LPARAM
// Qualifier	:
// Last Update	: 2013/2/22 - 11:40
// Desc.		: 3State 일 경우에는 다르게 수정해야 한다.
//=============================================================================
LRESULT CChMonCtrl::OnBMSetCheck(WPARAM nCheckState, LPARAM)
{	
	if (m_nChecked != nCheckState)
	{
		m_nChecked = (int)nCheckState;
		ReDrawPanel();
	}

	return 0;	
}

//=============================================================================
// Method		: CChMonCtrl::OnBMGetCheck
// Access		: protected 
// Returns		: LRESULT
// Parameter	: WPARAM
// Parameter	: LPARAM
// Qualifier	:
// Last Update	: 2013/2/22 - 11:40
// Desc.		:
//=============================================================================
LRESULT CChMonCtrl::OnBMGetCheck(WPARAM, LPARAM)
{	
	return m_nChecked;
}

//=============================================================================
// Method		: CChMonCtrl::OnBMSetState
// Access		: protected 
// Returns		: LRESULT
// Parameter	: WPARAM state
// Parameter	: LPARAM
// Qualifier	:
// Last Update	: 2013/2/22 - 11:40
// Desc.		:
//=============================================================================
LRESULT CChMonCtrl::OnBMSetState(WPARAM state, LPARAM)
{	
	if (m_bHighlight != (state & 0x0003))
	{
		m_bHighlight = (state & 0x0003);
		ReDrawPanel();
	}

	return 0;
}

//=============================================================================
// Method		: CChMonCtrl::OnBMGetState
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
LRESULT CChMonCtrl::OnBMGetState(WPARAM, LPARAM)
{
	return m_bHighlight;
}

//=============================================================================
// Method		: CChMonCtrl::OnBMClick
// Access		: protected 
// Returns		: LRESULT
// Parameter	: WPARAM
// Parameter	: LPARAM
// Qualifier	:
// Last Update	: 2013/2/22 - 15:48
// Desc.		:
//=============================================================================
LRESULT CChMonCtrl::OnBMClick( WPARAM,LPARAM )
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
// Method		: CChMonCtrl::OnLButtonDown
// Access		: public 
// Returns		: void
// Parameter	: UINT nFlags
// Parameter	: CPoint point
// Qualifier	:
// Last Update	: 2013/2/22 - 15:45
// Desc.		:
//=============================================================================
void CChMonCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	__super::OnLButtonDown(nFlags, point);
}

//=============================================================================
// Method		: CChMonCtrl::OnLButtonUp
// Access		: public 
// Returns		: void
// Parameter	: UINT nFlags
// Parameter	: CPoint point
// Qualifier	:
// Last Update	: 2013/2/22 - 15:45
// Desc.		:
//=============================================================================
void CChMonCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	CRect	rClient;
	GetClientRect(&rClient);

	if (rClient.PtInRect(point))
		OnBMClick (0, 0);	

	__super::OnLButtonUp(nFlags, point);
}

//=============================================================================
// Method		: CChMonCtrl::PaintCachedBitmap
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
void CChMonCtrl::PaintCachedBitmap( Graphics& gfx, Rect& argRect, BOOL bSelected /*= FALSE*/, int nChecked /*= BST_UNCHECKED*/ )
{
	Bitmap	*pBit	= new Bitmap(argRect.GetRight(), argRect.GetBottom(), &gfx);
	Graphics *memG	= new Graphics(pBit);	

	if (bSelected)
		m_bPressed = TRUE;
	else
		m_bPressed = FALSE;

	if (BST_CHECKED == nChecked)
		m_bChecked = TRUE;
	else
		m_bChecked = FALSE;	
	
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
// Method		: CChMonCtrl::InitContent
// Access		: protected 
// Returns		: void
// Qualifier	:
// Last Update	: 2013/3/12 - 10:13
// Desc.		:
//=============================================================================
void CChMonCtrl::InitContent()
{
	m_stContent.UseBackground	= TRUE;
	m_stContent.UseBorder		= TRUE;
	m_stContent.BackColor		= Color::White;
	m_stContent.BorderColor		= Color::Black;
	m_stContent.BorderTickness	= 1.0f;

	m_stContent.FontName		= L"Tahoma";
	m_stContent.Text			= L"000.0";	
	m_stContent.fontStyle		= FontStyleRegular;
	m_stContent.fontSize		= 9.0f;
	m_stContent.TextAlignment	= StringAlignmentCenter;
	m_stContent.TextColor		= Color::Black;
}

//=============================================================================
// Method		: CChMonCtrl::SetContentBackColorRedraw
// Access		: virtual public 
// Returns		: void
// Parameter	: Color clrBack
// Parameter	: UINT nContentIndex
// Qualifier	:
// Last Update	: 2013/6/18 - 15:43
// Desc.		:
//=============================================================================
void CChMonCtrl::SetContentBackColorRedraw( Color clrBack, UINT nContentIndex /*= 0*/ )
{
	m_stContent.BackColor = clrBack;

	ReDrawPanel();
}

//=============================================================================
// Method		: CChMonCtrl::DrawCaptionbar
// Access		: protected 
// Returns		: void
// Parameter	: Graphics & graphics
// Parameter	: PanelStyle panelStyle
// Parameter	: Rect rectClient
// Qualifier	:
// Last Update	: 2013/7/16 - 13:57
// Desc.		:
//=============================================================================
void CChMonCtrl::DrawCaptionbar( Graphics& graphics, PanelStyle panelStyle, Rect rectClient )
{	
	Color colorGradientBegin	= m_PanelColors.CaptionGradientBegin;
	Color colorGradientEnd		= m_PanelColors.CaptionGradientEnd;
	Color colorGradientMiddle	= m_PanelColors.CaptionGradientMiddle;
	Rect  rectCaption			= m_rectCaption;

	if (m_CaptionStyle == CaptionStyle_Normal)
	{
		if (m_bChecked)
		{
			colorGradientBegin	= m_PanelColors.CaptionCheckedGradientBegin;
			colorGradientEnd	= m_PanelColors.CaptionCheckedGradientEnd;
			colorGradientMiddle = m_PanelColors.CaptionCheckedGradientMid;
		}
		else
		{
			if (m_bPressed)
			{
				colorGradientBegin	= m_PanelColors.CaptionPressedGradientBegin;
				colorGradientEnd	= m_PanelColors.CaptionPressedGradientEnd;
				colorGradientMiddle = m_PanelColors.CaptionPressedGradientMid;
			}			
		}				

		if (panelStyle == PanelStyle_Office2007)
		{
			RenderButtonBackground(graphics, rectCaption, colorGradientBegin, colorGradientMiddle, colorGradientEnd);
		}		
		else
		{
			if (rectCaption.Height < rectCaption.Width)
				RenderDoubleBackgroundGradient(graphics, rectCaption, colorGradientBegin, colorGradientMiddle, colorGradientEnd, LinearGradientModeVertical, false);
			else
				RenderDoubleBackgroundGradient(graphics, rectCaption, colorGradientBegin, colorGradientMiddle, colorGradientEnd, LinearGradientModeHorizontal, false);
		}
	}
	else //m_captionStyle == CaptionStyle_Flat
	{
		colorGradientBegin	= m_PanelColors.CaptionGradientBegin;
		colorGradientEnd	= m_PanelColors.CaptionGradientEnd;
		
		if (m_bChecked)
		{
			colorGradientBegin	= m_PanelColors.CaptionCheckedGradientBegin;
			colorGradientEnd	= m_PanelColors.CaptionCheckedGradientEnd;			
		}		

		RenderFlatButtonBackground(graphics, rectCaption, colorGradientBegin, colorGradientEnd);

		//Color colorInnerBorder			= m_PanelColors.InnerBorderColor;
		//DrawInnerBorders(graphics, rectClient, DockStyle_Fill, 0);
	}
}

//=============================================================================
// Method		: CChMonCtrl::DrawCaptionText
// Access		: protected 
// Returns		: void
// Parameter	: Graphics & graphics
// Qualifier	:
// Last Update	: 2013/7/2 - 13:03
// Desc.		:
//=============================================================================
void CChMonCtrl::DrawCaptionText( Graphics& graphics)
{	
	Color colorText				= m_PanelColors.CaptionText;
	Rect  rectCaption			= m_rectCaption;

	if (m_bChecked)
		colorText	= m_PanelColors.CaptionSelectedText;
		
	if ((m_DockStyle == DockStyle_Top) && (m_FontSize <= 10))
	{
		rectCaption.Y += 3;
		rectCaption.Height -= 3;
	}

	DrawImagesAndText(graphics, rectCaption, m_iVgSpacing, m_rectImage, m_pImage, m_pFontCaption, colorText, m_strCaptionText.GetBuffer(), m_CaptionAlignment);
}

//=============================================================================
// Method		: CChMonCtrl::DrawBorders
// Access		: private 
// Returns		: void
// Parameter	: Graphics & graphics
// Parameter	: Rect rectClient
// Qualifier	:
// Last Update	: 2013/3/5 - 15:15
// Desc.		:
//=============================================================================
void CChMonCtrl::DrawBorders( Graphics& graphics, Rect rectClient)
{
	GraphicsPath	graphicsPath;
	Color BorderColor = m_bChecked ? m_PanelColors.CaptionCheckedGradientMid : m_PanelColors.BorderColor;
	Pen				borderPen(BorderColor, (REAL)m_BorderTickness);

	INT iAdjust = m_BorderTickness / 2;

	// Caption과 Content 사이의 경계선 그리기
	switch (m_DockStyle)
	{
	case DockStyle_Left:
		graphics.DrawLine(
			&borderPen,
			m_rectCaption.GetRight() + iAdjust,
			m_rectCaption.GetTop(),
			m_rectCaption.GetRight() + iAdjust,
			m_rectCaption.GetTop() + m_rectCaption.Height);
		break;

	case DockStyle_Right:
		graphics.DrawLine(
			&borderPen,
			m_rectCaption.GetLeft() - iAdjust,
			m_rectCaption.GetTop(),
			m_rectCaption.GetLeft() - iAdjust,
			m_rectCaption.GetTop() + m_rectCaption.Height);
		break;

	case DockStyle_Top:
		graphics.DrawLine(
			&borderPen,
			m_rectCaption.GetLeft(),
			m_rectCaption.GetBottom() + iAdjust,
			m_rectCaption.GetLeft() + m_rectCaption.Width,
			m_rectCaption.GetBottom() + iAdjust);
		break;

	case DockStyle_Bottom:
		graphics.DrawLine(
			&borderPen,
			m_rectCaption.GetLeft(),
			m_rectCaption.GetTop() - iAdjust,
			m_rectCaption.GetLeft() + m_rectCaption.Width,
			m_rectCaption.GetTop() - iAdjust);
		break;

	default:	// Top
		graphics.DrawLine(
			&borderPen,
			m_rectCaption.GetLeft(),
			m_rectCaption.GetBottom() + iAdjust,
			m_rectCaption.GetLeft() + m_rectCaption.Width,
			m_rectCaption.GetBottom() + iAdjust);
		break;
	}		

	borderPen.SetColor(BorderColor);
	borderPen.SetWidth((REAL)m_BorderTickness);

	INT iAdjust2 = m_BorderTickness;	
	if (SmoothingModeAntiAlias <= graphics.GetSmoothingMode())
		iAdjust2 = ((INT)m_BorderTickness % 2) ? m_BorderTickness : m_BorderTickness + 1;

	graphics.DrawRectangle(&borderPen, RectF((REAL)rectClient.X + iAdjust, (REAL)rectClient.Y + iAdjust, (REAL)(rectClient.Width - iAdjust2), (REAL)(rectClient.Height - iAdjust2)));
}

//=============================================================================
// Method		: CChMonCtrl::DrawInnerBorders
// Access		: private 
// Returns		: void
// Parameter	: Graphics & graphics
// Parameter	: Rect rectClient
// Parameter	: DockStyle dockStyle
// Parameter	: int PaddingAll
// Qualifier	:
// Last Update	: 2013/3/5 - 15:15
// Desc.		:
//=============================================================================
void CChMonCtrl::DrawInnerBorders( Graphics& graphics, Rect rectClient, DockStyle dockStyle /*= DockStyle_Fill*/, int PaddingAll /*= 0*/ )
{
	GraphicsPath graphicsPath;
	Rect captionRectangle (m_rectCaption);				
	if (dockStyle == DockStyle_Fill)
	{
		if (PaddingAll == 1)
		{
			//Left vertical borderline
			graphicsPath.AddLine(captionRectangle.X, captionRectangle.Y + captionRectangle.Height, captionRectangle.X, captionRectangle.Y + m_BorderTickness);
			if (rectClient.GetTop() == 0)
			{
				//Upper horizontal borderline
				graphicsPath.AddLine(captionRectangle.X, captionRectangle.Y, captionRectangle.X + captionRectangle.Width, captionRectangle.Y);
			}
			else
			{
				//Upper horizontal borderline
				graphicsPath.AddLine(captionRectangle.X, captionRectangle.Y + m_BorderTickness, captionRectangle.X + captionRectangle.Width, captionRectangle.Y + m_BorderTickness);
			}
		}
	}
	else
	{
		//Left vertical borderline
		graphicsPath.AddLine(captionRectangle.X + m_BorderTickness, captionRectangle.Y + captionRectangle.Height, captionRectangle.X + m_BorderTickness, captionRectangle.Y);
		if (rectClient.GetTop() == 0)
		{
			//Upper horizontal borderline
			graphicsPath.AddLine(captionRectangle.X + m_BorderTickness, captionRectangle.Y + m_BorderTickness, captionRectangle.X + captionRectangle.Width - m_BorderTickness, captionRectangle.Y + m_BorderTickness);
		}
		else
		{
			//Upper horizontal borderline
			graphicsPath.AddLine(captionRectangle.X + m_BorderTickness, captionRectangle.Y, captionRectangle.X + captionRectangle.Width - m_BorderTickness, captionRectangle.Y);
		}
	}

	Pen borderPen (m_PanelColors.InnerBorderColor);
	{
		graphics.DrawPath(&borderPen, &graphicsPath);
	}
}

//=============================================================================
// Method		: CChMonCtrl::DrawContent
// Access		: protected 
// Returns		: void
// Parameter	: Graphics & graphics
// Parameter	: Rect rectContent
// Parameter	: INT iBorderTickness
// Qualifier	:
// Last Update	: 2013/7/2 - 13:09
// Desc.		:
//=============================================================================
void CChMonCtrl::DrawContent( Graphics& graphics, Rect rectContent, INT iBorderTickness )
{
	int iCount = 1;

	int iSpacing	= m_iContentSpacing;// 컨텐츠 사이 간격 Spacing
	int iPadding	= m_iContentPadding;// 보더와 컨텐츠 사이 간격 Padding
	INT iAdjust		= 0;//iBorderTickness / 2;
	RectF rectRealContent((REAL)rectContent.X + iAdjust, (REAL)rectContent.Y + iAdjust, (REAL)rectContent.Width - (iAdjust * 2), (REAL)rectContent.Height - (iAdjust * 2));
	RectF rectUnitContent;

	rectUnitContent = rectRealContent;

	Gdiplus::Font* pFont = NULL;
	Content		TempContent;
	SolidBrush	backgroundBrush(Color::White);
	Pen			borderPen(Color::Black, 1);

	Matrix	matRect(1, 0, 0, 1, 1, 1);
	PointF  ptRotationCenter;
	RectF	rectText;

	TempContent = m_stContent;

	if (TempContent.UseBackground)
	{
		backgroundBrush.SetColor(TempContent.BackColor);
		graphics.FillRectangle(&backgroundBrush, rectUnitContent);

		borderPen.SetColor(TempContent.BorderColor);
		borderPen.SetWidth(TempContent.BorderTickness);
		graphics.DrawRectangle(&borderPen, rectUnitContent);
	}

	// 컨텐트의 작업 영역 설정 
	SetContentRect (rectUnitContent);

	pFont = new Gdiplus::Font (TempContent.FontName.GetBuffer(), TempContent.fontSize, TempContent.fontStyle);

	DrawString(graphics, rectUnitContent, pFont, TempContent.TextColor, TempContent.Text.GetBuffer(), TempContent.TextAlignment);

	rectUnitContent.Y = rectUnitContent.Y + iSpacing + rectUnitContent.Height;

	delete pFont;
	pFont = NULL;
}

//=============================================================================
// Method		: CChMonCtrl::CalculatePanelHeights
// Access		: protected 
// Returns		: void
// Parameter	: Rect rectClient
// Qualifier	:
// Last Update	: 2013/3/8 - 17:24
// Desc.		:
//=============================================================================
void CChMonCtrl::CalculatePanelHeights( Rect rectClient )
{	
	int iCaptionWidth = ((rectClient.Width / 2) < m_iCaptionWidth) ? (rectClient.Width / 2) : m_iCaptionWidth;

	switch (m_DockStyle)
	{
	case DockStyle_Left:
		m_rectCaption = Rect (0, 0, iCaptionWidth, rectClient.Height);
		m_rectContent = Rect (iCaptionWidth, 0, rectClient.Width - iCaptionWidth, rectClient.Height);
		break;

	case DockStyle_Right:
		m_rectCaption = Rect (rectClient.Width - iCaptionWidth, 0, iCaptionWidth, rectClient.Height);
		m_rectContent = Rect (0, 0, rectClient.Width - iCaptionWidth, rectClient.Height);
		break;

	case DockStyle_Top:
		m_rectCaption = Rect (0, 0, rectClient.Width, m_iCaptionHeight);
		m_rectContent = Rect (0, m_iCaptionHeight, rectClient.Width, rectClient.Height - m_iCaptionHeight);
		break;

	case DockStyle_Bottom:
		m_rectCaption = Rect (0, rectClient.Height - m_iCaptionHeight, rectClient.Width, m_iCaptionHeight);
		m_rectContent = Rect (0, 0, rectClient.Width, rectClient.Height - m_iCaptionHeight);
		break;

	default:	// Top
		m_rectCaption = Rect (0, 0, rectClient.Width, m_iCaptionHeight);
		m_rectContent = Rect (0, m_iCaptionHeight, rectClient.Width, rectClient.Height - m_iCaptionHeight);
		break;
	}
}

//=============================================================================
// Method		: CChMonCtrl::DrawBackground
// Access		: private 
// Returns		: void
// Parameter	: Graphics & graphics
// Parameter	: Rect rectClient
// Qualifier	:
// Last Update	: 2013/3/5 - 15:15
// Desc.		:
//=============================================================================
void CChMonCtrl::DrawBackground( Graphics& graphics, Rect rectClient )
{
	Rect rectangle = m_rectContent;
	SolidBrush backgroundBrush(m_PanelColors.BackColor);

	graphics.FillRectangle(&backgroundBrush, rectangle);	
}

//=============================================================================
// Method		: CChMonCtrl::DrawPanel
// Access		: private 
// Returns		: void
// Parameter	: Graphics & graphics
// Parameter	: Rect rectClient
// Qualifier	:
// Last Update	: 2013/3/5 - 15:15
// Desc.		:
//=============================================================================
void CChMonCtrl::DrawPanel( Graphics& graphics, Rect rectClient )
{
	if (IsZeroWidthOrHeight(m_rectCaption) == true)
	{
		return;
	}

	graphics.SetSmoothingMode(m_SmoothingMode);
	//graphics.SetTextRenderingHint(TextRenderingHintClearTypeGridFit);	
	graphics.SetTextRenderingHint(TextRenderingHintAntiAliasGridFit);

	DrawCaptionbar(graphics, m_PanelStyle, rectClient);	
	DrawCaptionText (graphics);
	DrawBorders(graphics, rectClient);
}

//=============================================================================
// Method		: CChMonCtrl::DrawPanelAndBackground
// Access		: protected 
// Returns		: void
// Parameter	: Graphics & graphics
// Parameter	: Rect rectClient
// Qualifier	:
// Last Update	: 2013/3/8 - 17:01
// Desc.		:
//=============================================================================
void CChMonCtrl::DrawPanelAndBackground( Graphics& graphics, Rect rectClient )
{
	CalculatePanelHeights (rectClient);
	DrawBackground (graphics, rectClient);
	DrawPanel (graphics, rectClient);
}

//=============================================================================
// Method		: CChMonCtrl::DrawPanelWithContent
// Access		: virtual protected 
// Returns		: void
// Parameter	: Graphics & graphics
// Parameter	: Rect rectClient
// Qualifier	:
// Last Update	: 2013/3/11 - 10:29
// Desc.		:
//=============================================================================
void CChMonCtrl::DrawPanelWithContent( Graphics& graphics, Rect rectClient )
{
	CalculatePanelHeights (rectClient);
	//DrawBackground (graphics, rectClient);

	// 컨텐츠
	DrawContent (graphics, m_rectContent, m_BorderTickness);

	// 캡션 + 캡션 텍스트 + 보더
	if (IsZeroWidthOrHeight(m_rectCaption) == true)
	{
		return;
	}

	graphics.SetSmoothingMode(m_SmoothingMode);
	//graphics.SetTextRenderingHint(TextRenderingHintClearTypeGridFit);	
	graphics.SetTextRenderingHint(TextRenderingHintAntiAliasGridFit);

	DrawCaptionbar(graphics, m_PanelStyle, rectClient);	
	DrawCaptionText (graphics);
	DrawBorders(graphics, rectClient);
}

//-------------------------------------------------------------------------
// 멤버 변수 설정 함수
//-------------------------------------------------------------------------
//=============================================================================
// Method		: CChMonCtrl::ChangeColorStyle
// Access		: public 
// Returns		: void
// Parameter	: ColorStyle nColorStyle
// Qualifier	:
// Last Update	: 2013/3/12 - 15:45
// Desc.		:
//=============================================================================
void CChMonCtrl::ChangeColorStyle( ColorStyle nColorStyle )
{
	switch (nColorStyle)
	{
	case ColorStyle_Default:
	case ColorStyle_Sky:
		m_PanelColors.InitColor_Sky();
		break;
	case ColorStyle_SkyBlue:
		m_PanelColors.InitColor_Skyblue();
		break;
	case ColorStyle_Blue:
		m_PanelColors.InitColor_Blue();
		break;
	case ColorStyle_Blue2007:
		m_PanelColors.InitColor_Blue2007();
		break;
	case ColorStyle_Black:
		m_PanelColors.InitColor_Black();
		break;
	case ColorStyle_Black2007:
		m_PanelColors.InitColor_Black2007();
		break;
	case ColorStyle_Red:
		m_PanelColors.InitColor_Red();
		break;
	case ColorStyle_Orange:
		m_PanelColors.InitColor_Orange();
		break;
	case ColorStyle_Green:
		m_PanelColors.InitColor_Green();
		break;
	}

	ReDrawPanel();
}

//=============================================================================
// Method		: CChMonCtrl::SetFont
// Access		: public 
// Returns		: void
// Parameter	: IN const WCHAR * familyName
// Parameter	: REAL emSize
// Parameter	: FontStyle fontStyle
// Qualifier	:
// Last Update	: 2013/3/12 - 15:45
// Desc.		:
//=============================================================================
void CChMonCtrl::SetCaptionFont( IN const WCHAR * familyName, REAL emSize /*= 8.0*/, FontStyle fontStyle /*= FontStyleBold*/ )
{
	if (NULL != m_pFontCaption)
	{
		delete m_pFontCaption;
	}

	m_pFontCaption = new Gdiplus::Font(familyName, emSize, fontStyle);

	ReDrawPanel();
}

//=============================================================================
// Method		: CChMonCtrl::GetCaptionText
// Access		: public 
// Returns		: void
// Parameter	: OUT CStringW & szValue
// Qualifier	:
// Last Update	: 2013/3/12 - 15:45
// Desc.		:
//=============================================================================
void CChMonCtrl::GetCaptionText( OUT CStringW& szValue )
{
	szValue = m_strCaptionText;

	ReDrawPanel();
}

//=============================================================================
// Method		: CChMonCtrl::SetCaptionText
// Access		: public 
// Returns		: void
// Parameter	: IN const WCHAR * szValue
// Qualifier	:
// Last Update	: 2013/3/12 - 15:45
// Desc.		:
//=============================================================================
void CChMonCtrl::SetCaptionText( IN const WCHAR* szValue )
{
	m_strCaptionText = szValue;

	ReDrawPanel();
}

//=============================================================================
// Method		: CChMonCtrl::SetCaptionTextAlignment
// Access		: public 
// Returns		: void
// Parameter	: StringAlignment CationTextAlignment
// Qualifier	:
// Last Update	: 2013/3/12 - 15:45
// Desc.		:
//=============================================================================
void CChMonCtrl::SetCaptionTextAlignment( StringAlignment CationTextAlignment )
{
	m_CaptionAlignment = CationTextAlignment;

	ReDrawPanel();
}

//=============================================================================
// Method		: CChMonCtrl::SetCaptionTextColor
// Access		: public 
// Returns		: void
// Parameter	: Color TextColor
// Parameter	: Color SelectedTextColor
// Parameter	: Color CollapsedTextColor
// Qualifier	:
// Last Update	: 2013/3/12 - 15:45
// Desc.		:
//=============================================================================
void CChMonCtrl::SetCaptionTextColor( Color TextColor, Color SelectedTextColor /*= Color::White*/, Color CollapsedTextColor /*= Color::White*/ )
{
	m_PanelColors.CaptionText			= TextColor;
	m_PanelColors.CaptionSelectedText	= SelectedTextColor;
	m_PanelColors.CollapsedCaptionText	= CollapsedTextColor;

	ReDrawPanel();
}

//=============================================================================
// Method		: CChMonCtrl::SetCaptionHeight
// Access		: public 
// Returns		: void
// Parameter	: int iHeight
// Qualifier	:
// Last Update	: 2013/3/12 - 15:45
// Desc.		:
//=============================================================================
void CChMonCtrl::SetCaptionHeight( int iHeight )
{
	m_iCaptionHeight = iHeight;

	ReDrawPanel();
}

//=============================================================================
// Method		: CChMonCtrl::SetDockStyle
// Access		: public 
// Returns		: void
// Parameter	: DockStyle dockStyle
// Parameter	: BOOL bReDraw
// Qualifier	:
// Last Update	: 2013/3/13 - 9:43
// Desc.		:
//=============================================================================
void CChMonCtrl::SetDockStyle( DockStyle dockStyle, BOOL bReDraw /*= TRUE*/ )
{
	//if (m_DockStyle != dockStyle) // 버그 문제로 주석처리
	{
		m_DockStyle = dockStyle;

		if (bReDraw)
			ReDrawPanel();	
	}
}

//-----------------------------------------------------------------------------
// 컨텐츠 표시 관련
//-----------------------------------------------------------------------------
//=============================================================================
// Method		: CChMonCtrl::SetContentText
// Access		: public 
// Returns		: void
// Parameter	: LPCTSTR szText
// Qualifier	:
// Last Update	: 2013/3/12 - 16:05
// Desc.		:
// 2013.06.11 -> 메모리가 계속 증가하는 문제 발견
//=============================================================================
void CChMonCtrl::SetContentText( LPCTSTR szText)
{
	CStringW strText(szText);
	m_stContent.Text = strText;

	//ReDrawPanel();
}

//=============================================================================
// Method		: CChMonCtrl::SetContectBackColor
// Access		: virtual public 
// Returns		: void
// Parameter	: Color clrBack
// Qualifier	:
// Last Update	: 2013/6/7 - 15:46
// Desc.		:
//=============================================================================
void CChMonCtrl::SetContentBackColor( Color clrBack)
{
	m_stContent.BackColor = clrBack;

	//ReDrawPanel();
}

//=============================================================================
// Method		: CChMonCtrl::SetContentRect
// Access		: virtual public 
// Returns		: void
// Parameter	: RectF rectContent
// Qualifier	:
// Last Update	: 2013/6/10 - 19:23
// Desc.		:
//=============================================================================
void CChMonCtrl::SetContentRect( RectF rectContent )
{
	m_stContent.rectContent = rectContent;

	//ReDrawPanel();
}

//=============================================================================
// Method		: CChMonCtrl::DrawString
// Access		: protected 
// Returns		: void
// Parameter	: Graphics & graphics
// Parameter	: RectF & layoutRectangle
// Parameter	: Gdiplus::Font * font
// Parameter	: Color fontColor
// Parameter	: WCHAR * strText
// Parameter	: StringAlignment stringAlignment
// Qualifier	:
// Last Update	: 2013/7/2 - 11:17
// Desc.		:
//=============================================================================
void CChMonCtrl::DrawString( Graphics& graphics, RectF& layoutRectangle, Gdiplus::Font* font, Color fontColor, WCHAR* strText, StringAlignment stringAlignment )
{
	SolidBrush		stringBrush(fontColor);
	StringFormat	stringFormat;

	stringFormat.SetFormatFlags(StringFormatFlagsNoWrap);	
	stringFormat.SetTrimming(StringTrimmingEllipsisCharacter);
	stringFormat.SetLineAlignment(StringAlignmentCenter);
	stringFormat.SetAlignment (stringAlignment);
	graphics.DrawString(strText, -1, font, layoutRectangle, &stringFormat, &stringBrush);
}

//=============================================================================
// Method		: CChMonCtrl::DrawImagesAndText
// Access		: protected 
// Returns		: void
// Parameter	: Graphics & graphics
// Parameter	: Rect & captionRectangle
// Parameter	: int iSpacing
// Parameter	: Rect imageRectangle
// Parameter	: Image * image
// Parameter	: Gdiplus::Font * fontCaption
// Parameter	: Color captionForeColor
// Parameter	: WCHAR * strCaptionText
// Parameter	: StringAlignment TextAlignment
// Qualifier	:
// Last Update	: 2013/7/2 - 11:17
// Desc.		:
//=============================================================================
void CChMonCtrl::DrawImagesAndText( Graphics& graphics, Rect& captionRectangle, int iSpacing, 
								Rect imageRectangle, Image* image, Gdiplus::Font* fontCaption, 
								Color captionForeColor, WCHAR* strCaptionText, StringAlignment TextAlignment )
{
	//DrawImages
	int iTextPositionX1 = captionRectangle.GetLeft() + iSpacing;
	int iTextPositionX2 = captionRectangle.GetRight() - iSpacing;

	imageRectangle.Y = (captionRectangle.Height - imageRectangle.Height) / 2;

	if (image != NULL)
	{		
		graphics.DrawImage(image, imageRectangle);
		iTextPositionX1 += imageRectangle.Width + iSpacing;
	}

	// Draw Caption text
	Rect textRectangle	= captionRectangle;
	textRectangle.X		= iTextPositionX1;
	//textRectangle.Width -= (iSpacing + iSpacing);	
	textRectangle.Width = iTextPositionX2 - iTextPositionX1;	

	DrawString(graphics, RectF((REAL)textRectangle.X, (REAL)textRectangle.Y, (REAL)textRectangle.Width, (REAL)textRectangle.Height), fontCaption, captionForeColor, strCaptionText, TextAlignment);
}

//=============================================================================
// Method		: CChMonCtrl::RenderFlatButtonBackground
// Access		: protected 
// Returns		: void
// Parameter	: Graphics & graphics
// Parameter	: Rect & bounds
// Parameter	: Color colorGradientBegin
// Parameter	: Color colorGradientEnd
// Parameter	: BOOL bHover
// Qualifier	:
// Last Update	: 2013/7/2 - 11:17
// Desc.		:
//=============================================================================
void CChMonCtrl::RenderFlatButtonBackground( Graphics& graphics, Rect& bounds, Color colorGradientBegin, Color colorGradientEnd)
{
	/*float Positions[]	= { 0.0F, 0.2F, 0.3F, 0.4F, 0.5F, 0.6F, 0.7F, 0.8F, 1.0F };
	float Factors[]		= { 0.4F, 0.5F, 0.6F, 0.8F, 1.0F, 0.8F, 0.6F, 0.5F, 0.4F };

	LinearGradientBrush gradientBrush(bounds, colorGradientBegin, colorGradientEnd, LinearGradientModeHorizontal);		

	gradientBrush.SetBlend(Factors, Positions, 9);
	graphics.FillRectangle(&gradientBrush, bounds);*/


	// 단색
	SolidBrush solidBrush(colorGradientEnd);
	graphics.FillRectangle(&solidBrush, bounds);
}

//=============================================================================
// Method		: CChMonCtrl::RenderDoubleBackgroundGradient
// Access		: protected 
// Returns		: void
// Parameter	: Graphics & graphics
// Parameter	: Rect & bounds
// Parameter	: Color beginColor
// Parameter	: Color middleColor
// Parameter	: Color endColor
// Parameter	: int firstGradientWidth
// Parameter	: int secondGradientWidth
// Parameter	: LinearGradientMode mode
// Parameter	: bool flipHorizontal
// Qualifier	:
// Last Update	: 2013/7/2 - 11:17
// Desc.		:
//=============================================================================
void CChMonCtrl::RenderDoubleBackgroundGradient( Graphics& graphics, Rect& bounds, Color beginColor, Color middleColor, Color endColor, int firstGradientWidth, int secondGradientWidth, LinearGradientMode mode, bool flipHorizontal )
{
	if ((bounds.Width != 0) && (bounds.Height != 0))
	{
		Rect rectangle1 = bounds;
		Rect rectangle2 = bounds;
		bool flag1 = true;
		if (mode == LinearGradientModeHorizontal)
		{
			if (flipHorizontal)
			{
				Color color1 = endColor;
				endColor = beginColor;
				beginColor = color1;
			}
			rectangle2.Width = firstGradientWidth;
			rectangle1.Width = secondGradientWidth + 1;
			rectangle1.X = bounds.GetRight() - rectangle1.Width;
			flag1 = bounds.Width > (firstGradientWidth + secondGradientWidth);
		}
		else
		{
			rectangle2.Height = firstGradientWidth;
			rectangle1.Height = secondGradientWidth + 1;
			rectangle1.Y = bounds.GetBottom() - rectangle1.Height;
			flag1 = bounds.Height > (firstGradientWidth + secondGradientWidth);
		}

		if (flag1)
		{
			SolidBrush brush1(middleColor);
			{
				graphics.FillRectangle(&brush1, bounds);
			}

			LinearGradientBrush brush2(rectangle2, beginColor, middleColor, mode);
			{
				graphics.FillRectangle(&brush2, rectangle2);
			}

			LinearGradientBrush brush3(rectangle1, middleColor, endColor, mode);
			{
				if (mode == LinearGradientModeHorizontal)
				{
					rectangle1.X++;
					rectangle1.Width--;
				}
				else
				{
					rectangle1.Y++;
					rectangle1.Height--;
				}
				graphics.FillRectangle(&brush3, rectangle1);
				return;
			}
		}

		LinearGradientBrush brush4(bounds, beginColor, endColor, mode);
		{
			graphics.FillRectangle(&brush4, bounds);
		}
	}
}

//=============================================================================
// Method		: CChMonCtrl::RenderDoubleBackgroundGradient
// Access		: protected 
// Returns		: void
// Parameter	: Graphics & graphics
// Parameter	: Rect & bounds
// Parameter	: Color beginColor
// Parameter	: Color middleColor
// Parameter	: Color endColor
// Parameter	: LinearGradientMode linearGradientMode
// Parameter	: bool flipHorizontal
// Qualifier	:
// Last Update	: 2013/7/2 - 11:17
// Desc.		:
//=============================================================================
void CChMonCtrl::RenderDoubleBackgroundGradient( Graphics& graphics, Rect& bounds, Color beginColor, Color middleColor, Color endColor, LinearGradientMode linearGradientMode, bool flipHorizontal )
{
	int iUpperHeight = bounds.Height / 2;
	int iLowerHeight = bounds.Height - iUpperHeight;

	RenderDoubleBackgroundGradient(graphics, bounds, beginColor, middleColor, endColor, iUpperHeight, iLowerHeight, linearGradientMode, flipHorizontal);
}

//=============================================================================
// Method		: CChMonCtrl::RenderButtonBackground
// Access		: protected 
// Returns		: void
// Parameter	: Graphics & graphics
// Parameter	: Rect & bounds
// Parameter	: Color colorGradientBegin
// Parameter	: Color colorGradientMiddle
// Parameter	: Color colorGradientEnd
// Qualifier	:
// Last Update	: 2013/7/2 - 11:17
// Desc.		:
//=============================================================================
void CChMonCtrl::RenderButtonBackground( Graphics& graphics, Rect& bounds, Color colorGradientBegin, Color colorGradientMiddle, Color colorGradientEnd )
{
	if (bounds.Height < bounds.Width) // 가로가 세로보다 길때
	{
		RectF rectUpper ((REAL)bounds.X, (REAL)bounds.Y, (REAL)bounds.Width, (REAL)bounds.Height);
		rectUpper.Height = bounds.Height * 0.4f;		

		LinearGradientBrush upperLinearGradientBrush(rectUpper, colorGradientBegin, colorGradientMiddle, LinearGradientModeVertical);

		float Positions[] = { 0.0F, 1.0F };
		float Factors[] = { 0.0F, 0.6F };
		upperLinearGradientBrush.SetBlend(Factors, Positions, 2);
		graphics.FillRectangle(&upperLinearGradientBrush, rectUpper);

		RectF rectLower ((REAL)bounds.X, (REAL)bounds.Y, (REAL)bounds.Width, (REAL)bounds.Height);
		rectLower.Y = bounds.Y + rectUpper.Height;
		rectLower.Height = bounds.Height - rectUpper.Height;

		LinearGradientBrush lowerLinearGradientBrush(rectLower, colorGradientMiddle, colorGradientEnd, LinearGradientModeVertical);

		graphics.FillRectangle(&lowerLinearGradientBrush, rectLower);

		//At some captionheights there are drawing errors. This is the correction
		RectF correctionRectangle = rectLower;
		correctionRectangle.Y -= 1;
		correctionRectangle.Height = 2;		

		SolidBrush solidBrush(colorGradientMiddle);
		graphics.FillRectangle(&solidBrush, correctionRectangle);
	}
	else // 세로가 가로보다 길때
	{
		RectF rectLeft ((REAL)bounds.X, (REAL)bounds.Y, (REAL)bounds.Width, (REAL)bounds.Height);
		rectLeft.Width = bounds.Width * 0.4f;		

		LinearGradientBrush upperLinearGradientBrush(rectLeft, colorGradientBegin, colorGradientMiddle, LinearGradientModeHorizontal);

		float Positions[] = { 0.0F, 1.0F };
		float Factors[] = { 0.0F, 0.6F };				
		upperLinearGradientBrush.SetBlend(Factors, Positions, 2);
		graphics.FillRectangle(&upperLinearGradientBrush, rectLeft);

		RectF rectRight ((REAL)bounds.X, (REAL)bounds.Y, (REAL)bounds.Width, (REAL)bounds.Height);
		rectRight.X = bounds.X + rectLeft.Width;
		rectRight.Width = bounds.Width - rectLeft.Width;

		LinearGradientBrush lowerLinearGradientBrush(rectRight, colorGradientMiddle, colorGradientEnd, LinearGradientModeHorizontal);

		graphics.FillRectangle(&lowerLinearGradientBrush, rectRight);

		//At some captionheights there are drawing errors. This is the correction
		RectF correctionRectangle = rectRight;
		correctionRectangle.X -= 1;
		correctionRectangle.Width = 2;		

		SolidBrush solidBrush(colorGradientMiddle);
		graphics.FillRectangle(&solidBrush, correctionRectangle);
	}
}

//=============================================================================
// Method		: CChMonCtrl::IsZeroWidthOrHeight
// Access		: protected 
// Returns		: bool
// Parameter	: Rect & rectangle
// Qualifier	:
// Last Update	: 2013/7/2 - 11:17
// Desc.		:
//=============================================================================
bool CChMonCtrl::IsZeroWidthOrHeight( Rect& rectangle )
{
	if (rectangle.Width != 0)
	{
		return (rectangle.Height == 0);
	}

	return true;
}
