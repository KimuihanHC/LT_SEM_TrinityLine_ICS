//*****************************************************************************
// Filename	: 	NTTabViewBar.cpp
//
// Created	:	
// Modified	:	2010/04/01 - 15:47
//
// Author	:	PiRing
//	
// Purpose	:	탭형태의 멀티 뷰 구현을 위한 툴바
//*****************************************************************************
// NTTabViewBar.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NTTabViewBar.h"
//#include "NTVisualManager.h"
#include "NTTabViewBarButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CMFCToolBarImages CNTTabViewBar::m_PressedImages;

//=============================================================================
// CNTTabBar
//=============================================================================

IMPLEMENT_DYNAMIC(CNTTabViewBar, CMFCToolBar)

//=============================================================================
//
//=============================================================================
CNTTabViewBar::CNTTabViewBar()
{
}

//=============================================================================
//
//=============================================================================
CNTTabViewBar::~CNTTabViewBar()
{
}

BEGIN_MESSAGE_MAP(CNTTabViewBar, CMFCToolBar)
	ON_WM_CREATE()
	ON_WM_SYSCOLORCHANGE()
END_MESSAGE_MAP()

//=============================================================================
// CNTTabViewBar 메시지 처리기입니다.
//=============================================================================
//=============================================================================
// Method		: CNTTabViewBar::LoadToolBar
// Access		: virtual public 
// Returns		: BOOL
// Parameter	: UINT uiResID
// Parameter	: UINT uiColdResID
// Parameter	: UINT uiMenuResID
// Parameter	: BOOL bLocked
// Parameter	: UINT uiDisabledResID
// Parameter	: UINT uiMenuDisabledResID
// Parameter	: UINT uiHotResID
// Parameter	: UINT uiPressedResID
// Qualifier	:
// Last Update	: 2010/04/02 - 13:23
// Desc.		: 
//=============================================================================
BOOL CNTTabViewBar::LoadToolBar (	UINT uiResID, UINT uiColdResID, UINT uiMenuResID, 
									BOOL bLocked,
									UINT uiDisabledResID, UINT uiMenuDisabledResID,
									UINT uiHotResID, UINT uiPressedResID)
{
	if (!CMFCToolBar::LoadToolBar (uiResID, uiColdResID, uiMenuResID, bLocked, uiDisabledResID, uiMenuDisabledResID, uiHotResID))
	{
		return FALSE;
	}

	if (uiPressedResID != 0)
	{
		m_PressedImages.SetImageSize (m_sizeImage);

		if (!m_PressedImages.Load (uiPressedResID, AfxGetResourceHandle (), TRUE))
		{
			return FALSE;
		}

		ASSERT (m_Images.GetCount () == m_PressedImages.GetCount ());
	}

	return TRUE;
}

//=============================================================================
// Method		: CNTTabViewBar::OnCreate
// Access		: protected 
// Returns		: int
// Parameter	: LPCREATESTRUCT lpCreateStruct
// Qualifier	:
// Last Update	: 2010/04/02 - 13:23
// Desc.		: 
//=============================================================================
int CNTTabViewBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CMFCToolBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetPaneStyle((GetPaneStyle () & ~(CBRS_GRIPPER | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT)) | CBRS_SIZE_DYNAMIC);
	SetBorders ();

	SetGrayDisabledButtons (FALSE);

	RestoreOriginalstate ();
	
	return 0;
}

//=============================================================================
// Method		: CNTTabViewBar::DrawButton
// Access		: virtual protected 
// Returns		: BOOL
// Parameter	: CDC * pDC
// Parameter	: CMFCToolBarButton * pButton
// Parameter	: CMFCToolBarImages * pImages
// Parameter	: BOOL bHighlighted
// Parameter	: BOOL bDrawDisabledImages
// Qualifier	:
// Last Update	: 2010/04/02 - 13:23
// Desc.		: 
//=============================================================================
BOOL CNTTabViewBar::DrawButton(CDC* pDC, CMFCToolBarButton* pButton, CMFCToolBarImages* pImages, BOOL bHighlighted, BOOL bDrawDisabledImages)
{
	ASSERT_VALID (pDC);
	ASSERT_VALID (pButton);

	CMFCToolBarImages* pNewImages = pImages;

	CAfxDrawState ds;

	if (!m_bMenuMode && (pButton->m_nStyle & (TBBS_PRESSED)) && m_PressedImages.GetCount () > 0)
	{
		pNewImages = &m_PressedImages;

		pNewImages->SetTransparentColor (afxGlobalData.clrBtnFace);

		pNewImages->PrepareDrawImage (ds, GetImageSize (), FALSE);
	}

	if (!CMFCToolBar::DrawButton (pDC, pButton, pNewImages, bHighlighted, bDrawDisabledImages))
	{
		return FALSE;
	}

	if (pNewImages != pImages)
	{
		pNewImages->EndDrawImage (ds);
	}

	return TRUE;
}

//=============================================================================
// Method		: CNTTabViewBar::OnSysColorChange
// Access		: protected 
// Returns		: void
// Qualifier	:
// Last Update	: 2010/04/02 - 13:23
// Desc.		: 
//=============================================================================
void CNTTabViewBar::OnSysColorChange ()
{
	CMFCToolBar::OnSysColorChange ();

	m_PressedImages.OnSysColorChange ();
}

//=============================================================================
// Method		: CNTTabViewBar::RestoreOriginalstate
// Access		: public 
// Returns		: BOOL
// Qualifier	:
// Last Update	: 2010/04/02 - 13:23
// Desc.		: 
//=============================================================================
BOOL CNTTabViewBar::RestoreOriginalstate ()
{
	RemoveAllButtons ();

	INT_PTR iCount = m_list_TabName.GetCount();

	POSITION pos = m_list_TabName.GetHeadPosition();
	POSITION pos2 = m_list_TabID.GetHeadPosition();
	for (int iIndex = 0; iIndex < iCount; iIndex++)
	{
		//InsertButton (CNTTabViewBarButton (m_list_TabName.GetNext(pos), m_list_TabID.GetNext(pos2)));
		InsertButton (CNTTabViewBarButton (m_list_TabName.GetNext(pos), m_list_TabID.GetNext(pos2), m_nButtonWidth));
	}

	AdjustLayout ();
	Invalidate ();

	return TRUE;
}

//=============================================================================
// Method		: ShowButton_Exclusive
// Access		: public  
// Returns		: void
// Parameter	: __in UINT nTabIndex
// Parameter	: __in BOOL bShow
// Qualifier	:
// Last Update	: 2016/9/27 - 19:39
// Desc.		:
//=============================================================================
void CNTTabViewBar::ShowButton_Exclusive(__in UINT nTabIndex, __in BOOL bShow)
{	
	int iCnt = (int)m_list_TabID.GetCount();

	for (int nIdx = 0; nIdx < iCnt; nIdx++)
	{
		if (nTabIndex != nIdx)
		{
			GetButton(nIdx)->EnableWindow(bShow);
			//GetButton(nIdx)->SetVisible(bShow);
			//GetButton(nIdx)->Show(bShow);
			((CNTTabViewBarButton*)GetButton(nIdx))->Set_Enable(bShow);
		}
	}

	Invalidate();
	CRect rc;
	GetClientRect(rc);
	OnSize(SIZE_RESTORED, rc.Width(), rc.Height());
}

void CNTTabViewBar::ShowButton_Exclusive(__in UINT* pnTabIndexz, __in UINT nTabCount, __in BOOL bShow)
{
	int iCnt = (int)m_list_TabID.GetCount();

	bool bComp = true;
	for (int nIdx = 0; nIdx < iCnt; nIdx++)
	{
		bComp = true;
		for (UINT nCnt = 0; nCnt < nTabCount; ++nCnt)
		{
			if (pnTabIndexz[nCnt] == nIdx)
			{
				bComp = false;
				break;
			}
		}

		if (bComp)
		{
			GetButton(nIdx)->EnableWindow(bShow);
			//GetButton(nIdx)->SetVisible(bShow);
			//GetButton(nIdx)->Show(bShow);
			((CNTTabViewBarButton*)GetButton(nIdx))->Set_Enable(bShow);

// 			UINT nStyle = GetButton(nIdx)->m_nStyle;
// 			if (bShow)
// 			{
// 				//GetButton(nIdx)->m_nStyle &= (~TBBS_DISABLED);
// 				nStyle &= (~TBBS_DISABLED);
// 			}
// 			else
// 			{
// 				//GetButton(nIdx)->m_nStyle |= TBBS_DISABLED;
// 				nStyle |= TBBS_DISABLED;
// 			}
// 
// 			GetButton(nIdx)->SetStyle(nStyle);
		}
	}

	Invalidate();
	CRect rc;
	GetClientRect(rc);
	OnSize(SIZE_RESTORED, rc.Width(), rc.Height());
}

//=============================================================================
// Method		: Set_ButtonWidth
// Access		: public  
// Returns		: void
// Parameter	: __in int IN_nWidth
// Qualifier	:
// Last Update	: 2021/4/30 - 11:46
// Desc.		:
//=============================================================================
void CNTTabViewBar::Set_ButtonWidth(__in int IN_nWidth)
{
	m_nButtonWidth = IN_nWidth;
}

//=============================================================================
// Method		: Button_SetWindowText
// Access		: public  
// Returns		: void
// Parameter	: __in UINT IN_nTabIndex
// Parameter	: __in LPCTSTR IN_szText
// Qualifier	:
// Last Update	: 2022/7/20 - 16:16
// Desc.		:
//=============================================================================
void CNTTabViewBar::Button_SetWindowText(__in UINT IN_nTabIndex, __in LPCTSTR IN_szText)
{
	UINT iCnt = (UINT)m_list_TabID.GetCount();

	if (IN_nTabIndex < iCnt)
	{
		GetButton(IN_nTabIndex)->m_strText = IN_szText;

		Invalidate();
	}
}

