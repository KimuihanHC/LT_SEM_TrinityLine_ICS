//*****************************************************************************
// Filename	: 	NTVisualManager.h
//
// Created	:	2010/03/10
// Modified	:	2010/03/10 - 10:50
//
// Author	:	PiRing
//	
// Purpose	:	윈도우 비주얼 관리 클래스
//*****************************************************************************
#ifndef __NTVisualManager_H__
#define __NTVisualManager_H__

#pragma once

#define UI_COLOR_DARKGRAY	0	// Office2007_ObsidianBlack
#define UI_COLOR_SKYBLUE	1	// Office2007_LunaBlue
// Office2007_Aqua,
// Office2007_Silver

#define UI_COLOR_SET		UI_COLOR_DARKGRAY

//=============================================================================
// CNTVisualManager 명령 대상입니다.
//=============================================================================
class CNTVisualManager : public CMFCVisualManagerOffice2007
{
	DECLARE_DYNCREATE(CNTVisualManager)

public:
	CNTVisualManager();
	virtual ~CNTVisualManager();

	void				SetupMemoryBitmapSize		(CDC* pDC, const CSize& size);

	virtual void		NTDrawCaptionButton			(CDC* pDC, CRect rect, AFX_BUTTON_STATE state, UINT id);	
	BOOL				LoadNTCaptionButtonsIcons	(LPCTSTR lpszID);
	const CSize&		GetNTCaptionButtonsSize		() const;

	CString				CheckColor					();

protected:

#if (_MSC_VER >= 1700)
	DRAWTHEMEBACKGROUND	m_pfDrawThemeBackground;
#endif

	virtual void		OnUpdateSystemColors		();		
	virtual COLORREF	GetToolbarButtonTextColor	(CMFCToolBarButton* pButton, CMFCVisualManager::AFX_BUTTON_STATE state);
	virtual void		OnFillBarBackground			(CDC* pDC, CBasePane* pBar, CRect rectClient, CRect rectClip, BOOL bNCArea = FALSE);
	virtual void		OnFillHighlightedArea		(CDC* pDC, CRect rect, CBrush* pBrush, CMFCToolBarButton* pButton);	
	virtual void		OnDrawButtonBorder			(CDC* pDC, CMFCToolBarButton* pButton, CRect rect, AFX_BUTTON_STATE state);
	
	// Customization dialog :
	//virtual void		OnDrawMiniFrameBorder		(CDC* pDC, CPaneFrameWnd* pFrameWnd, CRect rectBorder, CRect rectBorderSize){};

	// Tasks pane : 
	virtual void		OnFillTasksPaneBackground	(CDC* pDC, CRect rectWorkArea);
	virtual void		OnDrawTasksGroupCaption		(CDC* pDC, CMFCTasksPaneTaskGroup* pGroup, BOOL bIsHighlighted = FALSE, BOOL bIsSelected = FALSE, BOOL bCanCollapse = FALSE);
	virtual void		OnDrawTasksGroupIcon		(CDC* pDC, CMFCTasksPaneTaskGroup* pGroup, int nIconHOffset = 5, BOOL bIsHighlighted = FALSE, BOOL bIsSelected = FALSE, BOOL bCanCollapse = FALSE);
	virtual void		OnFillTasksGroupInterior	(CDC* pDC, CRect rect, BOOL bSpecial = FALSE) {}
	virtual void		OnDrawTasksGroupAreaBorder	(CDC* pDC, CRect rect, BOOL bSpecial = FALSE, BOOL bNoTitle = FALSE) {}
	virtual void		OnDrawTask					(CDC* pDC, CMFCTasksPaneTask* pTask, CImageList* pIcons, BOOL bIsHighlighted = FALSE, BOOL bIsSelected = FALSE);		

	virtual void		OnDrawSeparator				(CDC* pDC, CBasePane* pBar,CRect rect, BOOL bHorz);

private:
	CSize			m_MemorySize;
	CDC 			m_MemoryDC;
	CBitmap			m_MemoryBitmap;
	BOOL			m_MemoryFilled;

	CImageList		m_CaptionButtonIconst;
	CSize			m_CaptionButtonSize;

	CImageList		m_MenuButtonIconst;
	CSize			m_MenuButtonSize;

	CBitmap			m_NTFrameBitmap;
	CBitmap			m_NTViewBitmap;

	COLORREF		m_clrToolBarButtonTextRegular;
	COLORREF		m_clrToolBarButtonTextPressed;
	COLORREF		m_clrToolBarButtonTextHighlighted;

	COLORREF		m_clrTabViewBarButtonTextRegular;
	COLORREF		m_clrTabViewBarButtonTextPressed;
	COLORREF		m_clrTabViewBarButtonTextHighlighted;	

	COLORREF		m_clrTabViewBarButtonBorder;
	COLORREF		m_clrTabViewBarButtonBorderM;
	COLORREF		m_clrTabViewBarButtonLine;
	COLORREF		m_clrTabViewBarButtonF;
	COLORREF		m_clrTabViewBarButtonL;

	CBrush			m_brLinkBarBack;

	COLORREF   m_clrToolBarSeparatorS;
	COLORREF   m_clrToolBarSeparatorM;
	COLORREF   m_clrToolBarSeparatorF;
};

inline const CSize& CNTVisualManager::GetNTCaptionButtonsSize () const
{
	return m_CaptionButtonSize;
}


#endif // __NTVisualManager_H__

