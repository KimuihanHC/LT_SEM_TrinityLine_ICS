//*****************************************************************************
// Filename	: VGBase_Content.cpp
// Created	: 2013/3/13
// Modified	: 2013/3/13 - 16:13
//
// Author	: PiRing
//	
// Purpose	: 
//*****************************************************************************
#include "StdAfx.h"
#include "VGBase_Content.h"

CVGBase_Content::CVGBase_Content(void)
{
	m_nMaxContents		= 5;
	m_bContentsVertical = FALSE;

	m_iContentSpacing	= 3;
	m_iContentPadding	= 5;
}

CVGBase_Content::~CVGBase_Content(void)
{
}

//=============================================================================
// Method		: CVGBase_Content::DrawContent
// Access		: protected 
// Returns		: void
// Parameter	: Graphics & graphics
// Parameter	: Rect rectContent
// Qualifier	:
// Last Update	: 2013/3/11 - 15:58
// Desc.		:
//=============================================================================
void CVGBase_Content::DrawContent( Graphics& graphics, Rect rectContent, INT iBorderTickness )
{
// 	if (!m_bExpand)
// 		return;

	// Content Style : 1. 배경 + 글자, 2. 글자
	// Content Count : 최대 5개?
	INT_PTR iCount = m_arrContent.GetCount();

	if (iCount < 1)
		return;

	int iSpacing	= m_iContentSpacing;// 컨텐츠 사이 간격 Spacing
	int iPadding	= m_iContentPadding;// 보더와 컨텐츠 사이 간격 Padding
	INT iAdjust		= iBorderTickness / 2;
	RectF rectRealContent((REAL)rectContent.X + iAdjust, (REAL)rectContent.Y + iAdjust, (REAL)rectContent.Width - (iAdjust * 2), (REAL)rectContent.Height - (iAdjust * 2));
	RectF rectUnitContent;

	if (1 < iCount)
	{		
		if (!m_bContentsVertical)	// 가로 배치
		{
			rectUnitContent.Height	= (rectRealContent.Height - (iSpacing * (iCount - 1)) - (iPadding * 2)) / iCount;
			rectUnitContent.Width	= rectRealContent.Width - (iPadding * 2);
		}
		else	// 세로 배치
		{
			rectUnitContent.Height	= rectRealContent.Height - (iPadding * 2);
			rectUnitContent.Width	= (rectRealContent.Width - (iSpacing * (iCount - 1)) - (iPadding * 2)) / iCount;			
		}

		rectUnitContent.X		= rectRealContent.X + iPadding;
		rectUnitContent.Y		= rectRealContent.Y + iPadding;
	}
	else
	{
		rectUnitContent = rectRealContent;
	}

	Gdiplus::Font* pFont = NULL;
	Content		TempContent;
	SolidBrush	backgroundBrush(Color::White);
	Pen			borderPen(Color::Black, 1);

	Matrix	matRect(1, 0, 0, 1, 1, 1);
	PointF  ptRotationCenter;
	RectF	rectText;

	for (int iCnt = 0; iCnt < iCount; iCnt++)
	{
		TempContent = m_arrContent.GetAt(iCnt);

		if (TempContent.UseBackground)
		{
			backgroundBrush.SetColor(TempContent.BackColor);
			graphics.FillRectangle(&backgroundBrush, rectUnitContent);

			borderPen.SetColor(TempContent.BorderColor);
			borderPen.SetWidth(TempContent.BorderTickness);
			graphics.DrawRectangle(&borderPen, rectUnitContent);
		}

		// 컨텐트의 작업 영역 설정 
		SetContentRect (rectUnitContent, iCnt);

		pFont = new Gdiplus::Font (TempContent.FontName.GetBuffer(), TempContent.fontSize, TempContent.fontStyle);

		if (m_bContentsVertical)
		{
			rectText.X = rectUnitContent.X - (rectUnitContent.Height / 2) + (rectUnitContent.Width / 2);
			rectText.Y = rectUnitContent.Y + (rectUnitContent.Height / 2) - (rectUnitContent.Width / 2);
			rectText.Width = rectUnitContent.Height;
			rectText.Height= rectUnitContent.Width;

			ptRotationCenter.X = rectUnitContent.X + (rectUnitContent.Width / 2);
			ptRotationCenter.Y = rectUnitContent.Y + (rectUnitContent.Height / 2);

			matRect.RotateAt(-90.0f, ptRotationCenter);
			graphics.SetTransform(&matRect);

			DrawString(graphics, rectText, pFont, TempContent.TextColor, TempContent.Text.GetBuffer(), RightToLeft_No, TempContent.TextAlignment);

			graphics.ResetTransform();
			matRect.Reset();			

			rectUnitContent.X = rectUnitContent.X + iSpacing + rectUnitContent.Width;
		}
		else
		{
			rectText.X = rectUnitContent.X - iPadding;
			rectText.Y = rectUnitContent.Y;
			rectText.Width = rectUnitContent.Width + (iPadding * 2);
			rectText.Height= rectUnitContent.Height;

			DrawString(graphics, rectText, pFont, TempContent.TextColor, TempContent.Text.GetBuffer(), RightToLeft_No, TempContent.TextAlignment);
			//DrawString(graphics, rectUnitContent, pFont, TempContent.TextColor, TempContent.Text.GetBuffer(), RightToLeft_No, TempContent.TextAlignment);

			rectUnitContent.Y = rectUnitContent.Y + iSpacing + rectUnitContent.Height;
		}

		delete pFont;
		pFont = NULL;
	}
}

//-----------------------------------------------------------------------------
// 컨텐츠 표시 관련
//-----------------------------------------------------------------------------
//=============================================================================
// Method		: CVGBase_Content::AddContent
// Access		: protected 
// Returns		: BOOL
// Parameter	: Content newContent
// Qualifier	:
// Last Update	: 2013/3/12 - 15:36
// Desc.		:
//=============================================================================
BOOL CVGBase_Content::AddContent( Content newContent )
{
	if (m_arrContent.GetCount() < m_nMaxContents)
	{
		m_arrContent.Add (newContent);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//=============================================================================
// Method		: CVGBase_Content::ModifyContent
// Access		: protected 
// Returns		: BOOL
// Parameter	: int Index
// Parameter	: Content newContent
// Qualifier	:
// Last Update	: 2013/3/12 - 15:36
// Desc.		:
//=============================================================================
BOOL CVGBase_Content::ModifyContent( int Index, Content newContent )
{
	if (Index < m_arrContent.GetCount())
	{
		m_arrContent.RemoveAt(Index);
		m_arrContent.InsertAt(Index, newContent);

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//=============================================================================
// Method		: CVGBase_Content::ResetContent
// Access		: protected 
// Returns		: void
// Qualifier	:
// Last Update	: 2013/3/12 - 15:36
// Desc.		:
//=============================================================================
void CVGBase_Content::ResetContent()
{
	m_arrContent.RemoveAll();
}

//=============================================================================
// Method		: CVGBase_Content::SetContentBatchType
// Access		: protected 
// Returns		: void
// Parameter	: BOOL bBatchVertical
// Qualifier	:
// Last Update	: 2013/3/12 - 15:36
// Desc.		:
//=============================================================================
void CVGBase_Content::SetContentBatchType( BOOL bBatchVertical )
{
	if (m_bContentsVertical != bBatchVertical)
	{
		m_bContentsVertical = bBatchVertical;
		//ReDrawPanel();
	}
}

//=============================================================================
// Method		: CVGBase_Content::SetContentText
// Access		: public 
// Returns		: void
// Parameter	: LPCTSTR szText
// Parameter	: UINT nContentIndex
// Qualifier	:
// Last Update	: 2013/3/12 - 16:05
// Desc.		:
// 2013.06.11 -> 메모리가 계속 증가하는 문제 발견
//=============================================================================
void CVGBase_Content::SetContentText( LPCTSTR szText, UINT nContentIndex /*= 0*/ )
{
	if ((int)nContentIndex < m_arrContent.GetCount ())
	{
		Content* pContent = (Content*)m_arrContent.GetData();
		pContent += nContentIndex;

		CStringW strText(szText);
		pContent->Text = strText;

		//ReDrawPanel();
	}
}

//=============================================================================
// Method		: CVGBase_Content::SetContectBackColor
// Access		: virtual public 
// Returns		: void
// Parameter	: Color clrBack
// Parameter	: UINT nContentIndex
// Qualifier	:
// Last Update	: 2013/6/7 - 15:46
// Desc.		:
//=============================================================================
void CVGBase_Content::SetContentBackColor( Color clrBack, UINT nContentIndex /*= 0*/ )
{
	if ((int)nContentIndex < m_arrContent.GetCount ())
	{
		Content* pContent = (Content*)m_arrContent.GetData();
		pContent += nContentIndex;
		
		pContent->BackColor = clrBack;

		//ReDrawPanel();
	}
}

//=============================================================================
// Method		: CVGBase_Content::SetContentRect
// Access		: virtual public 
// Returns		: void
// Parameter	: RectF rectContent
// Parameter	: UINT nContentIndex
// Qualifier	:
// Last Update	: 2013/6/10 - 19:23
// Desc.		:
//=============================================================================
void CVGBase_Content::SetContentRect( RectF rectContent, UINT nContentIndex /*= 0*/ )
{
	if ((int)nContentIndex < m_arrContent.GetCount ())
	{
		Content* pContent = (Content*)m_arrContent.GetData();
		pContent += nContentIndex;

		pContent->rectContent = rectContent;

		//ReDrawPanel();
	}
}