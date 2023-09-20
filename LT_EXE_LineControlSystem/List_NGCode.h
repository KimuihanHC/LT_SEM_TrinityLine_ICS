//*****************************************************************************
// Filename	: 	List_NGCode.h
// Created	:	2021/12/20 - 14:58
// Modified	:	2021/12/20 - 14:58
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#ifndef List_NGCode_h__
#define List_NGCode_h__

#pragma once

#include <afxlistctrl.h>
#include "NGCode_Info.h"


//=============================================================================
// CList_NGCode
//=============================================================================
class CList_NGCode : public CListCtrl
{
	DECLARE_DYNAMIC(CList_NGCode)

public:
	CList_NGCode();
	virtual ~CList_NGCode();

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

	// 라인의 설비 구성 정보 구조체
	CNGCode_Info	m_NGCodeInfo;

	uint8_t			m_nLanguage			= 1;

	int			Find_Row					(__in int16_t IN_nNGCode);

	// 넘버링 다시 설정
	void		Reset_Number				();

	// 아이템 선택
	void		Select_SelectItem			(__in int nItem);

	// 설비 데이터 리스트에 설정
	void		Set_NG_Info					(__in int nItem, __in const ST_NGCode* IN_pNG_Code);
	
	// 아이템 추가
	void		Add_Item					(__in const ST_NGCode* IN_pNG_Code);
	// 아이템 수정
	void		Modify_Item					(__in int nItem, __in const ST_NGCode* IN_pNG_Code);

public:

	void		OnLanguage					(__in uint8_t IN_nLanguage);

	// 새로운 라인 정보를 리스트에 넣음
	void		Set_ItemList				(__in const CNGCode_Info* IN_pNGCodeList);
	void		Get_ItemList				(__out CNGCode_Info& OUT_NGCodeList);
	
	// 항목 추가
	bool		Item_Insert					(__in ST_NGCode& IN_NGCode);
	// 항목 삭제
	void		Item_Remove					();
	// 항목 선택된 위치의 내용을 변경
	void		Item_Modify					(__in ST_NGCode& IN_NGCode);

	// 선택한 항목의 데이터 반환
	bool		Get_SelectedItem			(__out ST_NGCode& OUT_NG_Code);

	// 리스트의 라인 구성 정보 모두 제거
	void		Clear_ItemList				();	

	// 아이템 정렬하여 리스트에 다시 표시
	void		Refresh_ItemList			();

	// 아이템 수정
	void		Update_Item					(__in int nRow, __in const ST_NGCode* IN_pNG_Code);
	void		Update_Item					(__in int16_t IN_nNGCode, __in const ST_NGCode* IN_pNG_Code);

	// 아이템 찾기
	int			Find_Item					(__in int16_t IN_nNGCode);

};

#endif // List_NGCode_h__
