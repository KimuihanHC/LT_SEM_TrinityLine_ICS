//*****************************************************************************
// Filename	: 	List_NGCodeView.h
// Created	:	2021/12/20 - 14:58
// Modified	:	2021/12/20 - 14:58
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#ifndef List_NGCodeView_h__
#define List_NGCodeView_h__

#pragma once

#include <afxlistctrl.h>
#include "NGCode_Info.h"

//=============================================================================
// CList_NGCodeView
//=============================================================================
class CList_NGCodeView : public CListCtrl
{
	DECLARE_DYNAMIC(CList_NGCodeView)

public:
	CList_NGCodeView();
	virtual ~CList_NGCodeView();

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg int		OnCreate			(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnSize				(UINT nType, int cx, int cy);
	afx_msg void	OnNMClick			(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL	PreCreateWindow		(CREATESTRUCT& cs);

	CFont			m_Font;

	virtual void	InitHeader			();

	BOOL			m_bIntiHeader		= FALSE;
	const int*		m_pHeadWidth		= NULL;

	int			Find_Row					(__in LPCTSTR IN_szRFID);

	// 넘버링 다시 설정
	void		Reset_Number				();

	// 아이템 선택
	void		Select_SelectItem			(__in int nItem);

	// 설비 데이터 리스트에 설정
	void		Set_NG_Info					(__in int nItem, __in const ST_NGCode* IN_pNG_Code);
	
	// 아이템 추가
	void		Add_Item					(__in const ST_NGCode* IN_pNG_Code);

public:

	// 새로운 라인 정보를 리스트에 넣음
	void		Set_ItemList				(__in const CNGCode_Info* IN_pNGCodeList);

};

#endif // List_NGCodeView_h__
