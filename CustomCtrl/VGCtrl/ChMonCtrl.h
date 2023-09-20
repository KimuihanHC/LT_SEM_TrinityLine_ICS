//*****************************************************************************
// Filename	: ChMonCtrl.h
// Created	: 2013/7/1
// Modified	: 2013/7/1 - 19:49
//
// Author	: PiRing
//	
// Purpose	: 
//*****************************************************************************
#ifndef ChMonCtrl_h__
#define ChMonCtrl_h__

#pragma once

#include "Define_ColorTable.h"
#include "Define_VGBase.h"

//=============================================================================
// CChMonCtrl
//=============================================================================
class CChMonCtrl : public CButton
{
	DECLARE_DYNAMIC(CChMonCtrl)

public:
	CChMonCtrl();
	virtual ~CChMonCtrl();

protected:
	DECLARE_MESSAGE_MAP()

	virtual void	PreSubclassWindow		();
	afx_msg void	OnSize					(UINT nType, int cx, int cy);
	afx_msg void	OnPaint					();
	afx_msg void	OnLButtonDown			(UINT nFlags, CPoint point);
	afx_msg void	OnLButtonUp				(UINT nFlags, CPoint point);		

	LRESULT			OnBMClick				(WPARAM,LPARAM);
	LRESULT			OnBMSetCheck			(WPARAM wParam,LPARAM lParam);	
	LRESULT			OnBMGetCheck			(WPARAM,LPARAM);
	LRESULT			OnBMSetState			(WPARAM wParam,LPARAM lParam);
	LRESULT			OnBMGetState			(WPARAM,LPARAM);

public:

	enum DockStyle
	{
		// 요약: The control is not docked.
		DockStyle_None		= 0,
		// 요약: The control's top edge is docked to the top of its containing control.
		DockStyle_Top		= 1,
		// 요약: The control's bottom edge is docked to the bottom of its containing control.
		DockStyle_Bottom	= 2,
		// 요약: The control's left edge is docked to the left edge of its containing control.
		DockStyle_Left		= 3,
		// 요약: The control's right edge is docked to the right edge of its containing control.
		DockStyle_Right		= 4,
		// 요약: All the control's edges are docked to the all edges of its containing control and sized appropriately.
		DockStyle_Fill		= 5,
	};

	enum PanelStyle
	{
		PanelStyle_Default,
		PanelStyle_Office2007,
	};

	enum CaptionStyle
	{
		CaptionStyle_Normal,
		CaptionStyle_Flat
	};

	enum ColorStyle
	{
		ColorStyle_Default	= 0,
		ColorStyle_Sky,
		ColorStyle_SkyBlue,
		ColorStyle_Blue,
		ColorStyle_Blue2007,
		ColorStyle_Black,
		ColorStyle_Black2007,
		ColorStyle_Red,
		ColorStyle_Orange,
		ColorStyle_Green,
	};

protected:

	CachedBitmap*	m_pCachedBitmap;	
	virtual void	ReDrawPanel				();	
	virtual void	PaintCachedBitmap		(Graphics& gfx, Rect& argRect, BOOL bSelected = FALSE, int nChecked = BST_UNCHECKED);

	int				m_nChecked;
	BOOL			m_bHighlight; // Highlight 상태	

	Content			m_stContent;

	int				m_iContentSpacing;// 컨텐츠 사이 간격 Spacing
	int				m_iContentPadding;// 보더와 컨텐츠 사이 간격 Padding		

	int				m_iVgSpacing;
	CaptionStyle	m_CaptionStyle;
	int				m_iCaptionHeight;
	int				m_iCaptionWidth;

	Gdiplus::Font*	m_pFontCaption;
	CStringW		m_strCaptionText;

	FontStyle		m_FontStyle;
	REAL			m_FontSize;
	StringAlignment	m_CaptionAlignment;

	Image*			m_pImage;		// Caption에 표시될 아이콘 이미지
	Gdiplus::Rect	m_rectImage;	// Rectangle(CaptionSpacing, CaptionHeight, m_imageSize.Width, m_imageSize.Height);

	SmoothingMode	m_SmoothingMode;
	INT				m_BorderTickness;
	PanelColors		m_PanelColors;
	PanelStyle		m_PanelStyle;
	
	BOOL			m_bChecked;
	BOOL			m_bPressed;

	Rect			m_rectCaption;	// Rect (0, 0, this.rectClient.Width, m_iCaptionHeight);	
	Rect			m_rectContent;

	DockStyle		m_DockStyle;

	BOOL			m_bRedrawPanel;

	/*BOOL			m_bRedraw_CaptionBack;
	BOOL			m_bRedraw_CaptionText;
	BOOL			m_bRedraw_Border;
	BOOL			m_bRedraw_ContentBack;
	BOOL			m_bRedraw_ContentText;
	Image*			m_img_CaptionBack;
	Image*			m_img_CaptionText;
	Image*			m_img_Border;
	Image*			m_img_ContentBack;
	Image*			m_img_ContentText;*/

	//-------------------------------------------------------------------------
	// GDI+ 그리기 함수
	//-------------------------------------------------------------------------
	void DrawString						(Graphics& graphics, RectF& layoutRectangle, Gdiplus::Font* font, Color fontColor, WCHAR* strText, StringAlignment stringAlignment);
	void DrawImagesAndText				(Graphics& graphics, Rect& captionRectangle, int iSpacing, Rect imageRectangle, Image* image,  Gdiplus::Font* fontCaption, Color captionForeColor, WCHAR* strCaptionText, StringAlignment TextAlignment);	
	void RenderFlatButtonBackground		(Graphics& graphics, Rect& bounds, Color colorGradientBegin, Color colorGradientEnd);
	void RenderDoubleBackgroundGradient	(Graphics& graphics, Rect& bounds, Color beginColor, Color middleColor, Color endColor, LinearGradientMode linearGradientMode, bool flipHorizontal);
	void RenderDoubleBackgroundGradient	(Graphics& graphics, Rect& bounds, Color beginColor, Color middleColor, Color endColor, int firstGradientWidth, int secondGradientWidth, LinearGradientMode mode, bool flipHorizontal);
	void RenderButtonBackground			(Graphics& graphics, Rect& bounds, Color colorGradientBegin, Color colorGradientMiddle, Color colorGradientEnd);	
	bool IsZeroWidthOrHeight			(Rect& rectangle);
	//-------------------------------------------------------------------------
	// Panel 그리기 함수
	//-------------------------------------------------------------------------
	void	DrawCaptionbar				(Graphics& graphics, PanelStyle panelStyle, Rect rectClient);
	void	DrawCaptionText				(Graphics& graphics);
	void	DrawBorders					(Graphics& graphics, Rect rectClient);	
	void	DrawInnerBorders			(Graphics& graphics, Rect rectClient, DockStyle dockStyle = DockStyle_Fill, int PaddingAll = 0);
	void	DrawContent					(Graphics& graphics, Rect rectContent, INT iBorderTickness);

	void	CalculatePanelHeights		(Rect rectClient);
	void	DrawBackground				(Graphics& graphics, Rect rectClient);
	void	DrawPanel					(Graphics& graphics, Rect rectClient);		

	virtual void DrawPanelAndBackground	(Graphics& graphics, Rect rectClient);
	virtual void DrawPanelWithContent	(Graphics& graphics, Rect rectClient);	

	// Content 부분
	void			InitContent				();

public:
	virtual void	SetContentText				(LPCTSTR szText);
	virtual	void	SetContentBackColor			(Color clrBack);
	virtual void	SetContentRect				(RectF rectContent);
	virtual	void	SetContentBackColorRedraw	(Color clrBack, UINT nContentIndex = 0);

	//-------------------------------------------------------------------------
	// 멤버 변수 설정 함수
	//-------------------------------------------------------------------------
	void	SetDockStyle				(DockStyle dockStyle, BOOL bReDraw = TRUE);
	void	ChangeColorStyle			(ColorStyle nColorStyle);

	// Caption Text
	void	SetCaptionFont				(IN const WCHAR * familyName, REAL emSize = 8.0, FontStyle fontStyle = FontStyleBold);
	void	GetCaptionText				(OUT CStringW& szValue);
	void	SetCaptionText				(IN const WCHAR* szValue);
	void	SetCaptionTextAlignment		(StringAlignment CationTextAlignment);
	void	SetCaptionTextColor			(Color TextColor, Color SelectedTextColor = Color::White, Color CollapsedTextColor = Color::White);
	void	SetCaptionHeight			(int iHeight);
	
	

};

#endif // ChMonCtrl_h__


