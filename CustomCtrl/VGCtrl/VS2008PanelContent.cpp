//*****************************************************************************
// Filename	: VS2008PanelContent.cpp
// Created	: 2013/2/21
// Modified	: 2013/2/21 - 11:49
//
// Author	: PiRing
//	
// Purpose	: 
//*****************************************************************************
#include "StdAfx.h"
#include "VS2008PanelContent.h"
#include "resource.h"

// CVS2008PanelContent
//=============================================================================
//
//=============================================================================
CVS2008PanelContent::CVS2008PanelContent()
{
	m_title = "";

	m_ContentColor = Color::White;
	m_BackColor = Color::White;
	m_bUseContentColor = FALSE;

	m_pCachedBitmap = NULL;
	m_bRedrawPanel = TRUE;
	m_titleAlignment = StringAlignmentCenter;

	m_FontFamilyName = L"Tahoma";
	m_FontSize = 13;

	m_bUseSelectionFlag = FALSE;
	m_bSelectedPanel = FALSE;
	m_SelectionColor = Color::Red;
}

CVS2008PanelContent::~CVS2008PanelContent()
{
	if (NULL != m_pCachedBitmap)
	{
		delete m_pCachedBitmap;
		m_pCachedBitmap = NULL;
	}
}

//=============================================================================
// Method		: CVS2008PanelContent::DrawCornersAndText
// Access		: virtual protected 
// Returns		: void
// Parameter	: Graphics & gfx
// Parameter	: Rect argRect
// Parameter	: ColorizerPanel & PanelColor
// Qualifier	:
// Last Update	: 2013/2/21 - 15:12
// Desc.		:
//=============================================================================
void CVS2008PanelContent::DrawCornersAndText(Graphics& gfx, Rect argRect, ColorizerPanel& PanelColor)
{
	Rect	destRect(argRect);

	if (destRect.Width < 28 || destRect.Height < 44)
		return;

	// Create a new empty image for manipulations. If you use this constructor, you get a new Bitmap object that represents a bitmap in memory with a PixelFormat of Format32bppRGB.
	Bitmap overlay(destRect.Width, destRect.Height, PixelFormat32bppRGB);
	{
		// Make an associated Graphics object.
		Graphics gr(&overlay);
		SolidBrush brush(Color::White);
		gr.FillRectangle(&brush, 0, 0, overlay.GetWidth(), overlay.GetHeight());

		/* Draw panel corners. */
		Bitmap* cornerBmp = NULL;
		for (int i = 0; i <= 7; i++)
		{
			switch (i)
			{
			case 0://Corner
			{
				cornerBmp = Bitmap::FromResource(AfxGetInstanceHandle(), (WCHAR *)MAKEINTRESOURCEW(IDB_TOP_LEFT));
				destRect = Rect(0, 0, cornerBmp->GetWidth(), cornerBmp->GetHeight());
				gr.DrawImage(cornerBmp, destRect);
				break;
			}
			case 1:
			{
				cornerBmp = Bitmap::FromResource(AfxGetInstanceHandle(), (WCHAR *)MAKEINTRESOURCEW(IDB_TOP_MIDDLE));
				destRect = Rect(14, 0, overlay.GetWidth() - 28, cornerBmp->GetHeight());
				TextureBrush brush(cornerBmp);
				gr.FillRectangle(&brush, destRect);
				break;
			}
			case 2://Corner
			{
				cornerBmp = Bitmap::FromResource(AfxGetInstanceHandle(), (WCHAR *)MAKEINTRESOURCEW(IDB_TOP_RIGHT));
				destRect = Rect(overlay.GetWidth() - cornerBmp->GetWidth(), 0, cornerBmp->GetWidth(), cornerBmp->GetHeight());
				gr.DrawImage(cornerBmp, destRect);
				break;
			}
			case 3:
			{
				cornerBmp = Bitmap::FromResource(AfxGetInstanceHandle(), (WCHAR *)MAKEINTRESOURCEW(IDB_MIDDLE_LEFT));
				destRect = Rect(0, 30, cornerBmp->GetWidth(), overlay.GetHeight() - 44);
				ImageAttributes attributes;
				attributes.SetWrapMode(WrapModeTileFlipY);
				gr.DrawImage(cornerBmp, destRect, 0, 0, cornerBmp->GetWidth(), cornerBmp->GetHeight(), UnitPixel, &attributes);
				break;
			}
			case 4:
			{
				cornerBmp = Bitmap::FromResource(AfxGetInstanceHandle(), (WCHAR *)MAKEINTRESOURCEW(IDB_MIDDLE_RIGHT));
				destRect = Rect(overlay.GetWidth() - cornerBmp->GetWidth(), 30, cornerBmp->GetWidth(), overlay.GetHeight() - 44);
				ImageAttributes attributes;
				attributes.SetWrapMode(WrapModeTileFlipY);
				gr.DrawImage(cornerBmp, destRect, 0, 0, cornerBmp->GetWidth(), cornerBmp->GetHeight(), UnitPixel, &attributes);
				break;
			}
			case 5://Corner
			{
				cornerBmp = Bitmap::FromResource(AfxGetInstanceHandle(), (WCHAR *)MAKEINTRESOURCEW(IDB_BOTTOM_LEFT));
				destRect = Rect(0, overlay.GetHeight() - cornerBmp->GetHeight(), cornerBmp->GetWidth(), cornerBmp->GetHeight());
				gr.DrawImage(cornerBmp, destRect);
				break;
			}
			case 6:
			{
				cornerBmp = Bitmap::FromResource(AfxGetInstanceHandle(), (WCHAR *)MAKEINTRESOURCEW(IDB_BOTTOM_MIDDLE));
				destRect = Rect(14, overlay.GetHeight() - cornerBmp->GetHeight(), overlay.GetWidth() - 28, cornerBmp->GetHeight());
				ImageAttributes attributes;
				attributes.SetWrapMode(WrapModeTileFlipX);
				gr.DrawImage(cornerBmp, destRect, 0, 0, cornerBmp->GetWidth(), cornerBmp->GetHeight(), UnitPixel, &attributes);
				break;
			}
			case 7://Corner
			{
				cornerBmp = Bitmap::FromResource(AfxGetInstanceHandle(), (WCHAR *)MAKEINTRESOURCEW(IDB_BOTTOM_RIGHT));
				destRect = Rect(overlay.GetWidth() - cornerBmp->GetWidth(), overlay.GetHeight() - cornerBmp->GetHeight(), cornerBmp->GetWidth(), cornerBmp->GetHeight());
				gr.DrawImage(cornerBmp, destRect);
				break;
			}
			}//End switch

			if (NULL != cornerBmp)
			{
				delete cornerBmp;
				cornerBmp = NULL;
			}
		}//End for		

		destRect = argRect;

		/* Create a new color matrix, The value Alpha in row 4, column 4 specifies the alpha value */
		REAL jaggedMatrixAllPanel[5][5] = {
			// Red component   [from 0.0 to 1.0 increase red color component.]
			{ PanelColor.IsColorizerEnabled ? PanelColor.Red / 255.0f : 1.0f, 0.0f, 0.0f, 0.0f, 0.0f },
			// Green component [from 0.0 to 1.0 increase green color component.]
			{ 0.0f, PanelColor.IsColorizerEnabled ? PanelColor.Green / 255.0f : 1.0f, 0.0f, 0.0f, 0.0f },
			// Blue component  [from 0.0 to 1.0 increase blue color component.]
			{ 0.0f, 0.0f, PanelColor.IsColorizerEnabled ? PanelColor.Blue / 255.0f : 1.0f, 0.0f, 0.0f },
			// Alpha component [from 1.0 to 0.0 increase transparency bitmap.]
			{ 0.0f, 0.0f, 0.0f, PanelColor.IsTransparencyEnabled ? PanelColor.Alpha / 255.0f : 1.0f, 0.0f },
			// White component [0.0: goes to Original color, 1.0: goes to white for all color component(Red, Green, Blue.)]
			{ PanelColor.IsColorizerEnabled ? 0.2f : 0.0f, PanelColor.IsColorizerEnabled ? 0.2f : 0.0f, PanelColor.IsColorizerEnabled ? 0.2f : 0.0f, 0.0f, 1.0f }
		};

		REAL jaggedMatrixCorners[5][5] = {
			//Red component   [from 0.0 to 1.0 increase red color component.]
			{ 1.0f, 0.0f, 0.0f, 0.0f, 0.0f },
			// Green component [from 0.0 to 1.0 increase green color component.]
			{ 0.0f, 1.0f, 0.0f, 0.0f, 0.0f },
			// Blue component  [from 0.0 to 1.0 increase blue color component.]
			{ 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
			// Alpha component [from 1.0 to 0.0 increase transparency bitmap.]
			{ 0.0f, 0.0f, 0.0f, PanelColor.IsTransparencyEnabled ? PanelColor.Alpha / 255.0f : 1.0f, 0.0f },
			// White component [0.0: goes to Original color, 1.0: goes to white for all color component(Red, Green, Blue.)]
			{ PanelColor.IsColorizerEnabled ? PanelColor.Red / 255.0f : 0.0f, PanelColor.IsColorizerEnabled ? PanelColor.Green / 255.0f : 0.0f, PanelColor.IsColorizerEnabled ? PanelColor.Blue / 255.0f : 0.0f, 0.0f, 1.0f }
		};

		ColorMatrix colorMatrix;

		if (PanelColor.PaintingStyle == AllPanel)
		{
			memcpy(colorMatrix.m, jaggedMatrixAllPanel, sizeof(ColorMatrix));
		}
		else	// OnlyCorners
		{
			memcpy(colorMatrix.m, jaggedMatrixCorners, sizeof(ColorMatrix));
		}

		// Create an ImageAttributes object and set its color matrix
		ImageAttributes attributes;
		ColorMap		map;
		map.oldColor = Color::Magenta;
		map.newColor = Color::Transparent;

		attributes.SetRemapTable(1, &map);
		attributes.SetColorMatrix(&colorMatrix, ColorMatrixFlagsDefault, ColorAdjustTypeDefault);

		// Background 사용한다면
		SolidBrush BackBrush(m_BackColor);
		gfx.FillRectangle(&BackBrush, destRect);

		// Panel 그리기
		gfx.DrawImage(&overlay, RectF((REAL)destRect.X, (REAL)destRect.Y, (REAL)destRect.Width, (REAL)destRect.Height), 0, 0, (REAL)overlay.GetWidth(), (REAL)overlay.GetHeight(), UnitPixel, &attributes);

		// 컨텐츠 배경색 칠하기
		if (m_bUseContentColor)
		{
			SolidBrush ContentBrush(Color(200, m_ContentColor.GetRed(), m_ContentColor.GetGreen(), m_ContentColor.GetBlue()));
			GraphicsPath gPath;
			AddContentPath(destRect, gPath);
			gfx.FillPath(&ContentBrush, &gPath);
		}

		if ((TRUE == m_bUseSelectionFlag) && (TRUE == m_bSelectedPanel))
		{
			SolidBrush SelectionBrush(m_SelectionColor);
			GraphicsPath gSelPath;
			AddSelectionPath(destRect, gSelPath);
			gfx.FillPath(&SelectionBrush, &gSelPath);
		}
	}

	// Draw String
	destRect.Inflate(-8, -4);

	if (destRect.Width > 0)
	{
		Gdiplus::Font captionFont(m_FontFamilyName, m_FontSize, FontStyleBold, UnitPixel);
		SolidBrush captionTextBrush(PanelColor.TextColor);
		StringFormat captionTextFormat(StringFormatFlagsNoWrap);

		captionTextFormat.SetAlignment(m_titleAlignment);
		captionTextFormat.SetTrimming(StringTrimmingEllipsisCharacter);
		captionTextFormat.SetHotkeyPrefix(HotkeyPrefixHide);

		gfx.SetTextRenderingHint(TextRenderingHintAntiAliasGridFit);
		gfx.DrawString(m_title, -1, &captionFont, RectF((REAL)destRect.X, (REAL)destRect.Y, (REAL)destRect.Width, (REAL)destRect.Height), &captionTextFormat, &captionTextBrush);
	}
}

void CVS2008PanelContent::DrawCornersAndText(Graphics& gfx, Rect argRect)
{
	DrawCornersAndText(gfx, argRect, m_panelColorizer);
}

//=============================================================================
// Method		: CVS2008PanelContent::AddContentPath
// Access		: protected 
// Returns		: void
// Parameter	: IN Rect rectClient
// Parameter	: OUT GraphicsPath & gPath
// Qualifier	:
// Last Update	: 2013/4/8 - 17:39
// Desc.		: 컨텐츠 부분 패스 설정
//=============================================================================
void CVS2008PanelContent::AddContentPath(IN Rect rectClient, OUT GraphicsPath& gPath)
{
	Rect rectP = rectClient;

	// 좌측
	gPath.AddLine(rectP.X + 2, rectP.Y + 4, rectP.X + 2, rectP.GetBottom() - 4);
	gPath.AddLine(rectP.X + 3, rectP.GetBottom() - 4, rectP.X + 3, rectP.GetBottom() - 3);
	// 하단
	gPath.AddLine(rectP.X + 4, rectP.GetBottom() - 2, rectP.GetRight() - 4, rectP.GetBottom() - 2);
	// 우측
	gPath.AddLine(rectP.GetRight() - 3, rectP.GetBottom() - 3, rectP.GetRight() - 3, rectP.GetBottom() - 4);
	gPath.AddLine(rectP.GetRight() - 2, rectP.GetBottom() - 4, rectP.GetRight() - 2, rectP.Y + 4);

	// 상단
	gPath.AddLine(rectP.GetRight() - 5, rectP.Y + 4, rectP.GetRight() - 5, rectP.Y + 21);
	gPath.AddLine(rectP.GetRight() - 6, rectP.Y + 21, rectP.GetRight() - 6, rectP.Y + 22);
	gPath.AddLine(rectP.GetRight() - 7, rectP.Y + 22, rectP.GetRight() - 7, rectP.Y + 23);


	gPath.AddLine(rectP.X + 7, rectP.Y + 23, rectP.X + 7, rectP.Y + 22);
	gPath.AddLine(rectP.X + 6, rectP.Y + 22, rectP.X + 6, rectP.Y + 21);
	gPath.AddLine(rectP.X + 5, rectP.Y + 21, rectP.X + 5, rectP.Y + 4);

	gPath.CloseFigure();
}

//=============================================================================
// Method		: CVS2008PanelContent::AddSelectionPath
// Access		: protected 
// Returns		: void
// Parameter	: IN Rect rectClient
// Parameter	: OUT GraphicsPath & gPath
// Qualifier	:
// Last Update	: 2013/4/9 - 17:00
// Desc.		:
//=============================================================================
void CVS2008PanelContent::AddSelectionPath(IN Rect rectClient, OUT GraphicsPath& gPath)
{
	Rect rectP = rectClient;

	gPath.AddLine(rectP.X, rectP.Y + 2, rectP.X, rectP.GetBottom());
	gPath.AddLine(rectP.GetRight(), rectP.GetBottom(), rectP.GetRight(), rectP.Y + 2);
	gPath.AddLine(rectP.GetRight() - 5, rectP.Y + 2, rectP.GetRight() - 5, rectP.GetBottom() - 5);
	gPath.AddLine(rectP.X + 5, rectP.GetBottom() - 5, rectP.X + 5, rectP.Y + 2);

	gPath.CloseFigure();
}

//=============================================================================
// Method		: CVS2008PanelContent::DrawCachedBitmap
// Access		: virtual protected 
// Returns		: void
// Parameter	: Graphics & gfx
// Parameter	: Rect argRect
// Parameter	: ColorizerPanel & PanelColor
// Qualifier	:
// Last Update	: 2013/2/21 - 15:13
// Desc.		:
//=============================================================================
void CVS2008PanelContent::PaintCachedBitmap(Graphics& gfx, Rect argRect, ColorizerPanel& PanelColor)
{
	/*Bitmap	Bit(argRect.GetRight(), argRect.GetBottom(), &gfx);
	Graphics memG(&Bit);

	DrawPanel (memG, argRect, PanelColor);

	if (NULL != m_pCachedBitmap)
	{
	delete m_pCachedBitmap;
	m_pCachedBitmap = NULL;
	}

	m_pCachedBitmap = new CachedBitmap(&Bit, &gfx);*/

	Bitmap	*pBit = new Bitmap(argRect.GetRight(), argRect.GetBottom(), &gfx);
	Graphics *memG = new Graphics(pBit);

	DrawPanel(*memG, argRect, PanelColor);

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
// Method		: CVS2008PanelContent::DrawCachedBitmap
// Access		: virtual protected 
// Returns		: void
// Parameter	: Graphics & gfx
// Parameter	: Rect argRect
// Qualifier	:
// Last Update	: 2013/2/21 - 11:51
// Desc.		: 더블 버퍼링용
//=============================================================================
void CVS2008PanelContent::PaintCachedBitmap(Graphics& gfx, Rect argRect)
{
	PaintCachedBitmap(gfx, argRect, m_panelColorizer);
}

//=============================================================================
// Method		: CVS2008PanelContent::ReDrawPanel
// Access		: virtual protected 
// Returns		: void
// Qualifier	:
// Last Update	: 2013/2/20 - 17:09
// Desc.		:
//=============================================================================
void CVS2008PanelContent::ReDrawPanel()
{
	m_bRedrawPanel = TRUE;
}

//=============================================================================
// Method		: CVS2008PanelContent::GetTitle
// Access		: public 
// Returns		: CStringW
// Qualifier	:
// Last Update	: 2013/2/20 - 17:29
// Desc.		:
//=============================================================================
CStringW CVS2008PanelContent::GetTitle()
{
	return m_title;
}

//=============================================================================
// Method		: CVS2008PanelContent::SetTitle
// Access		: public 
// Returns		: void
// Parameter	: WCHAR * szValue
// Qualifier	:
// Last Update	: 2013/2/20 - 17:29
// Desc.		:
//=============================================================================
void CVS2008PanelContent::SetTitle(WCHAR* szValue)
{
	if (NULL == szValue)
	{
		m_title.Empty();
	}
	else
	{
		if (szValue != m_title)
		{
			m_title = szValue;

			// Update text on the panel.			
			ReDrawPanel();
		}
	}
}

//=============================================================================
// Method		: CVS2008PanelContent::SetTitleAlignment
// Access		: public 
// Returns		: void
// Parameter	: StringAlignment TitleAlignment
// Qualifier	:
// Last Update	: 2013/2/20 - 17:29
// Desc.		:
//=============================================================================
void CVS2008PanelContent::SetTitleAlignment(StringAlignment TitleAlignment)
{
	if (m_titleAlignment != TitleAlignment)
	{
		m_titleAlignment = TitleAlignment;
		ReDrawPanel();
	}
}

//=============================================================================
// Method		: CVS2008PanelContent::SetTitleColor
// Access		: public 
// Returns		: void
// Parameter	: Color TextColor
// Qualifier	:
// Last Update	: 2013/2/20 - 17:53
// Desc.		:
//=============================================================================
void CVS2008PanelContent::SetTitleColor(Color TextColor)
{
	m_ContentColor = TextColor;
	ReDrawPanel();
}

//=============================================================================
// Method		: CVS2008PanelContent::SetBackgroundColor
// Access		: public 
// Returns		: void
// Parameter	: Color BackColor
// Qualifier	:
// Last Update	: 2013/2/20 - 17:29
// Desc.		:
//=============================================================================
void CVS2008PanelContent::SetBackgroundColor(Color BackColor)
{
	if (m_BackColor.ToCOLORREF() != BackColor.ToCOLORREF())
	{
		m_BackColor = BackColor;

		ReDrawPanel();
	}
}

//=============================================================================
// Method		: CVS2008PanelContent::SetColor
// Access		: public 
// Returns		: void
// Parameter	: byte red
// Parameter	: byte green
// Parameter	: byte blue
// Qualifier	:
// Last Update	: 2013/2/20 - 17:29
// Desc.		:
//=============================================================================
void CVS2008PanelContent::SetColor(byte red, byte green, byte blue)
{
	m_panelColorizer.Red = red;
	m_panelColorizer.Green = green;
	m_panelColorizer.Blue = blue;

	if (m_panelColorizer.IsColorizerEnabled)
		ReDrawPanel();
}

//=============================================================================
// Method		: CVS2008PanelContent::SetColorAlpha
// Access		: public 
// Returns		: void
// Parameter	: byte alpha
// Qualifier	:
// Last Update	: 2013/2/20 - 17:29
// Desc.		:
//=============================================================================
void CVS2008PanelContent::SetColorAlpha(byte alpha)
{
	if (m_panelColorizer.Alpha != alpha)
	{
		m_panelColorizer.Alpha = alpha;

		if (m_panelColorizer.IsTransparencyEnabled)
			ReDrawPanel();
	}
}

//=============================================================================
// Method		: CVS2008PanelContent::SetColorOption
// Access		: public 
// Returns		: void
// Parameter	: bool isColorizerEnabled
// Parameter	: bool isTransparencyEnabled
// Qualifier	:
// Last Update	: 2013/2/20 - 17:29
// Desc.		:
//=============================================================================
void CVS2008PanelContent::SetColorOption(bool isColorizerEnabled, bool isTransparencyEnabled)
{
	m_panelColorizer.IsColorizerEnabled = isColorizerEnabled;
	m_panelColorizer.IsTransparencyEnabled = isTransparencyEnabled;

	ReDrawPanel();
}

//=============================================================================
// Method		: CVS2008PanelContent::SetColorStyle
// Access		: public 
// Returns		: void
// Parameter	: ColorMatrixStyle paintingStyle
// Qualifier	:
// Last Update	: 2013/2/20 - 17:29
// Desc.		:
//=============================================================================
void CVS2008PanelContent::SetColorStyle(ColorMatrixStyle paintingStyle)
{
	if (m_panelColorizer.PaintingStyle != paintingStyle)
	{
		m_panelColorizer.PaintingStyle = paintingStyle;
		ReDrawPanel();
	}
}

//=============================================================================
// Method		: CVS2008PanelContent::SetFont
// Access		: public 
// Returns		: void
// Parameter	: const WCHAR * familyName
// Qualifier	:
// Last Update	: 2013/2/22 - 17:58
// Desc.		:
//=============================================================================
void CVS2008PanelContent::SetFont(const WCHAR * familyName)
{
	if (m_FontFamilyName != familyName)
	{
		m_FontFamilyName = familyName;
		ReDrawPanel();
	}
}

//=============================================================================
// Method		: CVS2008PanelContent::SetFontSize
// Access		: public 
// Returns		: void
// Parameter	: REAL emSize
// Qualifier	:
// Last Update	: 2013/2/22 - 17:58
// Desc.		:
//=============================================================================
void CVS2008PanelContent::SetFontSize(REAL emSize)
{
	if (m_FontSize != emSize)
	{
		m_FontSize = emSize;
		ReDrawPanel();
	}
}

//=============================================================================
// Method		: CVS2008PanelContent::SetContentBatchType
// Access		: virtual public 
// Returns		: void
// Parameter	: BOOL bBatchVertical
// Qualifier	:
// Last Update	: 2013/3/13 - 16:34
// Desc.		:
//=============================================================================
void CVS2008PanelContent::SetContentBatchType(BOOL bBatchVertical)
{
	CVGBase_Content::SetContentBatchType(bBatchVertical);
	ReDrawPanel();
}

//=============================================================================
// Method		: CVS2008PanelContent::SetContentText
// Access		: virtual public 
// Returns		: void
// Parameter	: LPCTSTR szText
// Parameter	: UINT nContentIndex
// Qualifier	:
// Last Update	: 2013/3/13 - 16:34
// Desc.		:
//=============================================================================
void CVS2008PanelContent::SetContentText(LPCTSTR szText, UINT nContentIndex /*= 0*/)
{
	CVGBase_Content::SetContentText(szText, nContentIndex);
	//ReDrawPanel();
}

//=============================================================================
// Method		: CVS2008PanelContent::DrawPanel
// Access		: virtual protected 
// Returns		: void
// Parameter	: Graphics & graphics
// Parameter	: Rect rectClient
// Parameter	: ColorizerPanel & PanelColor
// Qualifier	:
// Last Update	: 2013/3/13 - 16:51
// Desc.		:
//=============================================================================
void CVS2008PanelContent::DrawPanel(Graphics& graphics, Rect rectClient, ColorizerPanel& PanelColor)
{
	DrawCornersAndText(graphics, rectClient, PanelColor);

	// Content
	if (0 < m_arrContent.GetCount())
	{
		Rect rectContent(rectClient.X + 4, rectClient.Y + 24, rectClient.Width - 8, rectClient.Height - 32);

		DrawContent(graphics, rectContent, 1);
	}
}

//=============================================================================
// Method		: CVS2008PanelContent::SetContentBackColor
// Access		: public 
// Returns		: void
// Parameter	: Color BackColor
// Qualifier	:
// Last Update	: 2013/4/8 - 13:00
// Desc.		:
//=============================================================================
void CVS2008PanelContent::SetContentColor(Color BackColor)
{
	if (m_ContentColor.ToCOLORREF() != BackColor.ToCOLORREF())
	{
		m_ContentColor = BackColor;

		ReDrawPanel();
	}
}
