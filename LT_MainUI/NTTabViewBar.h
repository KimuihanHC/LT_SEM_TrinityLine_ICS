//*****************************************************************************
// Filename	: 	NTTabViewBar.h
//
// Created	:	
// Modified	:	2010/04/01 - 15:47
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
#ifndef __NTTabViewBar_H__
#define __NTTabViewBar_H__

#pragma once

//=============================================================================
// CNTTabViewBar
//=============================================================================
class CNTTabViewBar : public CMFCToolBar
{
	DECLARE_DYNAMIC(CNTTabViewBar)

public:
	CNTTabViewBar();
	virtual ~CNTTabViewBar();

	virtual BOOL	LoadToolBar			(	UINT uiResID, 
											UINT uiColdResID = 0, 
											UINT uiMenuResID = 0, 
											BOOL bLocked = FALSE,
											UINT uiDisabledResID = 0, 
											UINT uiMenuDisabledResID = 0,
											UINT uiHotResID = 0, 
											UINT uiPressedResID = 0);

	virtual BOOL	RestoreOriginalstate ();
	virtual int		GetRowHeight		() const
	{
		return 1;
	}

protected:
	afx_msg int		OnCreate			(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnSysColorChange	();

	DECLARE_MESSAGE_MAP()

	virtual BOOL	DrawButton			(	CDC* pDC, 
											CMFCToolBarButton* pButton, 
											CMFCToolBarImages* pImages, 
											BOOL bHighlighted, 
											BOOL bDrawDisabledImages);

	static CMFCToolBarImages m_PressedImages; // Shared pressed images

	CList<UINT, UINT>		m_list_TabID;
	CStringList				m_list_TabName;

	int						m_nButtonWidth = 280;

public:
	void	AddTabID				(LPCTSTR lpszTabName, UINT nTabID)
	{	
		m_list_TabName.AddTail(lpszTabName);
		m_list_TabID.AddTail(nTabID);
	};

	void	ShowButton_Exclusive	(__in UINT nTabIndex, __in BOOL bShow);
	void	ShowButton_Exclusive	(__in UINT* pnTabIndexz, __in UINT nTabCount, __in BOOL bShow);

	// 버튼 너비 설정
	void	Set_ButtonWidth			(__in int IN_nWidth);

	// 버튼 텍스트 변경
	void	Button_SetWindowText	(__in UINT IN_nTabIndex, __in LPCTSTR IN_szText);
};

#endif // __NTTabViewBar_H__


