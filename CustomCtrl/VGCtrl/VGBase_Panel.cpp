//*****************************************************************************
// Filename	: VGBase_Panel.cpp
// Created	: 2013/2/27
// Modified	: 2013/2/27 - 16:51
//
// Author	: PiRing
//	
// Purpose	: 
//*****************************************************************************
#include "StdAfx.h"
#include "VGBase_Panel.h"

//=============================================================================
//
//=============================================================================
CVGBase_Panel::CVGBase_Panel(void)
{	
	m_CaptionStyle		= CaptionStyle_Normal;
	m_iCaptionHeight	= CAPTION_MIN_HEIGHT;
	m_iCaptionWidth		= CAPTION_MIN_WIDTH;

	m_FontSize			= 10.0f;
	m_pFontCaption		= NULL; // Font("Segoe UI", 8F, System.Drawing.FontStyle.Bold)
	m_strCaptionText	= L"";
	m_RightToLeft		= RightToLeft_No;
	m_CaptionAlignment	= StringAlignmentCenter;

	m_pImage			= NULL;	

	m_SmoothingMode		= SmoothingModeAntiAlias;
	m_BorderTickness	= BORDER_THICKNESS;
	m_bShowBorder		= TRUE;
	m_bExpand			= TRUE;
	m_bChecked			= FALSE;
	m_bPressed			= FALSE;
	m_PanelColors;
	m_PanelStyle		= PanelStyle_Default;
	m_bHover			= FALSE;
	
	m_DockStyle			= DockStyle_Top;

	m_bRedrawPanel		= TRUE;

	m_pFontCaption		= new Gdiplus::Font(L"Segoe UI", m_FontSize, FontStyleBold);
}

CVGBase_Panel::~CVGBase_Panel(void)
{
	if (NULL != m_pFontCaption)
		delete m_pFontCaption;
}

//=============================================================================
// Method		: CVGBase_Panel::DrawCaptionbar
// Access		: private 
// Returns		: void
// Parameter	: Graphics & graphics
// Parameter	: BOOL bExpand
// Parameter	: BOOL bShowBorder
// Parameter	: PanelStyle panelStyle
// Parameter	: Rect rectClient
// Qualifier	:
// Last Update	: 2013/3/5 - 14:45
// Desc.		:
//=============================================================================
void CVGBase_Panel::DrawCaptionbar( Graphics& graphics, BOOL bExpand, BOOL bShowBorder, PanelStyle panelStyle, Rect rectClient )
{	
	Color colorGradientBegin	= m_PanelColors.CaptionGradientBegin;
	Color colorGradientEnd		= m_PanelColors.CaptionGradientEnd;
	Color colorGradientMiddle	= m_PanelColors.CaptionGradientMiddle;
	Color colorText				= m_PanelColors.CaptionText;
	Rect  rectCaption			= m_rectCaption;

	if (m_CaptionStyle == CaptionStyle_Normal)
	{
		if (m_bChecked)
		{
			colorGradientBegin	= m_PanelColors.CaptionCheckedGradientBegin;
			colorGradientEnd	= m_PanelColors.CaptionCheckedGradientEnd;
			colorGradientMiddle = m_PanelColors.CaptionCheckedGradientMid;
			colorText			= m_PanelColors.CaptionSelectedText;
		}
		else
		{
			if (m_bPressed)
			{
				colorGradientBegin	= m_PanelColors.CaptionPressedGradientBegin;
				colorGradientEnd	= m_PanelColors.CaptionPressedGradientEnd;
				colorGradientMiddle = m_PanelColors.CaptionPressedGradientMid;
			}
			else if (m_bHover)
			{
				colorGradientBegin	= m_PanelColors.CaptionSelectedGradientBegin;
				colorGradientEnd	= m_PanelColors.CaptionSelectedGradientEnd;
				colorGradientMiddle = m_PanelColors.CaptionSelectedGradientMid;				

				colorText			= m_PanelColors.CaptionSelectedText;
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
		Color colorFlatGradientBegin	= m_PanelColors.CaptionGradientBegin;
		Color colorFlatGradientEnd		= m_PanelColors.CaptionGradientEnd;
		Color colorInnerBorder			= m_PanelColors.InnerBorderColor;
		colorText						= m_PanelColors.CaptionText;

		if (m_bChecked)
		{
			colorFlatGradientBegin	= m_PanelColors.CaptionCheckedGradientBegin;
			colorFlatGradientEnd	= m_PanelColors.CaptionCheckedGradientEnd;			
			colorText				= m_PanelColors.CaptionSelectedText;
		}
		else
		{
			if (m_bHover)
			{
				colorFlatGradientBegin	= m_PanelColors.CaptionSelectedGradientBegin;
				colorFlatGradientEnd	= m_PanelColors.CaptionSelectedGradientEnd;				

				if (m_bPressed)
				{
					colorFlatGradientBegin	= m_PanelColors.CaptionPressedGradientBegin;
					colorFlatGradientEnd	= m_PanelColors.CaptionPressedGradientEnd;					
				}

				colorText			= m_PanelColors.CaptionSelectedText;
			}
		}

		RenderFlatButtonBackground(graphics, rectCaption, colorFlatGradientBegin, colorFlatGradientEnd, m_bHover);
		DrawInnerBorders(graphics, rectClient, DockStyle_Fill, 0);
	}

 	if ((m_DockStyle == DockStyle_Top) && (m_FontSize <= 10))
 	{
 		rectCaption.Y += 3;
 		rectCaption.Height -= 3;
 	}

	DrawImagesAndText(graphics, rectCaption, VG_SPACING, m_rectImage, m_pImage, m_RightToLeft, m_pFontCaption, colorText, m_strCaptionText.GetBuffer(), m_CaptionAlignment);
}

//=============================================================================
// Method		: CVGBase_Panel::DrawBorders
// Access		: private 
// Returns		: void
// Parameter	: Graphics & graphics
// Parameter	: Rect rectClient
// Qualifier	:
// Last Update	: 2013/3/5 - 15:15
// Desc.		:
//=============================================================================
void CVGBase_Panel::DrawBorders( Graphics& graphics, Rect rectClient)
{
	if (false == m_bShowBorder)
		return;

	GraphicsPath	graphicsPath;
	Color BorderColor = m_bChecked ? m_PanelColors.CaptionCheckedGradientMid : m_PanelColors.BorderColor;
	Pen				borderPen(BorderColor, (REAL)m_BorderTickness);

	INT iAdjust = m_BorderTickness / 2;

	if (m_bExpand) // Caption과 Content 사이의 경계선 그리기
	{
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
	}	

	borderPen.SetColor(BorderColor);
	borderPen.SetWidth((REAL)m_BorderTickness);

	INT iAdjust2 = m_BorderTickness;	
	if (SmoothingModeAntiAlias <= graphics.GetSmoothingMode())
		iAdjust2 = ((INT)m_BorderTickness % 2) ? m_BorderTickness : m_BorderTickness + 1;
	
	graphics.DrawRectangle(&borderPen, RectF((REAL)rectClient.X + iAdjust, (REAL)rectClient.Y + iAdjust, (REAL)(rectClient.Width - iAdjust2), (REAL)(rectClient.Height - iAdjust2)));
}

//=============================================================================
// Method		: CVGBase_Panel::DrawInnerBorders
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
void CVGBase_Panel::DrawInnerBorders( Graphics& graphics, Rect rectClient, DockStyle dockStyle /*= DockStyle_Fill*/, int PaddingAll /*= 0*/ )
{
	if (false == m_bShowBorder)
		return;

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
// Method		: CVGBase_Panel::CalculatePanelHeights
// Access		: protected 
// Returns		: void
// Parameter	: Rect rectClient
// Qualifier	:
// Last Update	: 2013/3/8 - 17:24
// Desc.		:
//=============================================================================
void CVGBase_Panel::CalculatePanelHeights( Rect rectClient )
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
// Method		: CVGBase_Panel::DrawBackground
// Access		: private 
// Returns		: void
// Parameter	: Graphics & graphics
// Parameter	: Rect rectClient
// Qualifier	:
// Last Update	: 2013/3/5 - 15:15
// Desc.		:
//=============================================================================
void CVGBase_Panel::DrawBackground( Graphics& graphics, Rect rectClient )
{
	Rect rectangle = m_rectContent;
	SolidBrush backgroundBrush(m_PanelColors.BackColor);

	graphics.FillRectangle(&backgroundBrush, rectangle);	
}

//=============================================================================
// Method		: CVGBase_Panel::DrawPanel
// Access		: private 
// Returns		: void
// Parameter	: Graphics & graphics
// Parameter	: Rect rectClient
// Qualifier	:
// Last Update	: 2013/3/5 - 15:15
// Desc.		:
//=============================================================================
void CVGBase_Panel::DrawPanel( Graphics& graphics, Rect rectClient )
{
	if (IsZeroWidthOrHeight(m_rectCaption) == true)
	{
		return;
	}
	
	graphics.SetSmoothingMode(m_SmoothingMode);
	graphics.SetTextRenderingHint(TextRenderingHintClearTypeGridFit);
	
	DrawCaptionbar(graphics, m_bExpand, m_bShowBorder, m_PanelStyle, rectClient);	
	DrawBorders(graphics, rectClient);
}

//=============================================================================
// Method		: CVGBase_Panel::DrawPanelAndBackground
// Access		: protected 
// Returns		: void
// Parameter	: Graphics & graphics
// Parameter	: Rect rectClient
// Qualifier	:
// Last Update	: 2013/3/8 - 17:01
// Desc.		:
//=============================================================================
void CVGBase_Panel::DrawPanelAndBackground( Graphics& graphics, Rect rectClient )
{
	CalculatePanelHeights (rectClient);
	DrawBackground (graphics, rectClient);
	DrawPanel (graphics, rectClient);
}

//=============================================================================
// Method		: CVGBase_Panel::DrawPanelWithContent
// Access		: virtual protected 
// Returns		: void
// Parameter	: Graphics & graphics
// Parameter	: Rect rectClient
// Qualifier	:
// Last Update	: 2013/3/11 - 10:29
// Desc.		:
//=============================================================================
void CVGBase_Panel::DrawPanelWithContent( Graphics& graphics, Rect rectClient )
{
	if (!m_bExpand)
		return;

	CalculatePanelHeights (rectClient);
	DrawBackground (graphics, rectClient);
	
	DrawContent (graphics, m_rectContent, m_BorderTickness);

	DrawPanel (graphics, rectClient);
}

//=============================================================================
// Method		: CVGBase_Panel::ReDrawPanel
// Access		: virtual protected 
// Returns		: void
// Qualifier	:
// Last Update	: 2013/3/7 - 14:38
// Desc.		:
//=============================================================================
void CVGBase_Panel::ReDrawPanel()
{
	m_bRedrawPanel = TRUE;
}

//-------------------------------------------------------------------------
// 멤버 변수 설정 함수
//-------------------------------------------------------------------------
//=============================================================================
// Method		: CVGBase_Panel::ChangeColorStyle
// Access		: public 
// Returns		: void
// Parameter	: ColorStyle nColorStyle
// Qualifier	:
// Last Update	: 2013/3/12 - 15:45
// Desc.		:
//=============================================================================
void CVGBase_Panel::ChangeColorStyle( ColorStyle nColorStyle )
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
// Method		: CVGBase_Panel::SetFont
// Access		: public 
// Returns		: void
// Parameter	: IN const WCHAR * familyName
// Parameter	: REAL emSize
// Parameter	: FontStyle fontStyle
// Qualifier	:
// Last Update	: 2013/3/12 - 15:45
// Desc.		:
//=============================================================================
void CVGBase_Panel::SetCaptionFont( IN const WCHAR * familyName, REAL emSize /*= 8.0*/, FontStyle fontStyle /*= FontStyleBold*/ )
{
	if (NULL != m_pFontCaption)
	{
		delete m_pFontCaption;
	}

	m_pFontCaption = new Gdiplus::Font(familyName, emSize, fontStyle);

	ReDrawPanel();
}

//=============================================================================
// Method		: CVGBase_Panel::GetCaptionText
// Access		: public 
// Returns		: void
// Parameter	: OUT CStringW & szValue
// Qualifier	:
// Last Update	: 2013/3/12 - 15:45
// Desc.		:
//=============================================================================
void CVGBase_Panel::GetCaptionText( OUT CStringW& szValue )
{
	szValue = m_strCaptionText;

	ReDrawPanel();
}

//=============================================================================
// Method		: CVGBase_Panel::SetCaptionText
// Access		: public 
// Returns		: void
// Parameter	: IN const WCHAR * szValue
// Qualifier	:
// Last Update	: 2013/3/12 - 15:45
// Desc.		:
//=============================================================================
void CVGBase_Panel::SetCaptionText( IN const WCHAR* szValue )
{
	m_strCaptionText = szValue;
	
	ReDrawPanel();
}

//=============================================================================
// Method		: CVGBase_Panel::SetCaptionTextAlignment
// Access		: public 
// Returns		: void
// Parameter	: StringAlignment CationTextAlignment
// Qualifier	:
// Last Update	: 2013/3/12 - 15:45
// Desc.		:
//=============================================================================
void CVGBase_Panel::SetCaptionTextAlignment( StringAlignment CationTextAlignment )
{
	m_CaptionAlignment = CationTextAlignment;

	ReDrawPanel();
}

//=============================================================================
// Method		: CVGBase_Panel::SetCaptionTextColor
// Access		: public 
// Returns		: void
// Parameter	: Color TextColor
// Parameter	: Color SelectedTextColor
// Parameter	: Color CollapsedTextColor
// Qualifier	:
// Last Update	: 2013/3/12 - 15:45
// Desc.		:
//=============================================================================
void CVGBase_Panel::SetCaptionTextColor( Color TextColor, Color SelectedTextColor /*= Color::White*/, Color CollapsedTextColor /*= Color::White*/ )
{
	m_PanelColors.CaptionText			= TextColor;
	m_PanelColors.CaptionSelectedText	= SelectedTextColor;
	m_PanelColors.CollapsedCaptionText	= CollapsedTextColor;
	
	ReDrawPanel();
}

//=============================================================================
// Method		: CVGBase_Panel::SetCaptionHeight
// Access		: public 
// Returns		: void
// Parameter	: int iHeight
// Qualifier	:
// Last Update	: 2013/3/12 - 15:45
// Desc.		:
//=============================================================================
void CVGBase_Panel::SetCaptionHeight( int iHeight )
{
	m_iCaptionHeight = iHeight;

	ReDrawPanel();
}

//=============================================================================
// Method		: CVGBase_Panel::SetDockStyle
// Access		: public 
// Returns		: void
// Parameter	: DockStyle dockStyle
// Parameter	: BOOL bReDraw
// Qualifier	:
// Last Update	: 2013/3/13 - 9:43
// Desc.		:
//=============================================================================
void CVGBase_Panel::SetDockStyle( DockStyle dockStyle, BOOL bReDraw /*= TRUE*/ )
{
	//if (m_DockStyle != dockStyle) // 버그 문제로 주석처리
	{
		m_DockStyle = dockStyle;

		if (bReDraw)
			ReDrawPanel();	
	}
}

//=============================================================================
// Method		: CVGBase_Panel::SetContentBatchType
// Access		: virtual public 
// Returns		: void
// Parameter	: BOOL bBatchVertical
// Qualifier	:
// Last Update	: 2013/3/13 - 16:34
// Desc.		:
//=============================================================================
void CVGBase_Panel::SetContentBatchType( BOOL bBatchVertical )
{
	CVGBase_Content::SetContentBatchType (bBatchVertical);
	ReDrawPanel();
}

//=============================================================================
// Method		: CVGBase_Panel::SetContentText
// Access		: virtual public 
// Returns		: void
// Parameter	: LPCTSTR szText
// Parameter	: UINT nContentIndex
// Qualifier	:
// Last Update	: 2013/3/13 - 16:34
// Desc.		:
//=============================================================================
void CVGBase_Panel::SetContentText( LPCTSTR szText, UINT nContentIndex /*= 0*/ )
{
	CVGBase_Content::SetContentText(szText, nContentIndex);
	ReDrawPanel();
}
