//*****************************************************************************
// Filename	: 	List_UserInfo.h
// Created	:	2021/12/20 - 14:58
// Modified	:	2021/12/20 - 14:58
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#ifndef List_UserInfo_h__
#define List_UserInfo_h__

#pragma once
//#include <afxcmn.h>
#include <afxlistctrl.h>

//#include "SpeedListCtrl.h"
#include "Def_UserInfo.h"


//=============================================================================
// CList_UserInfo
//=============================================================================
class CList_UserInfo : public CListCtrl
{
	DECLARE_DYNAMIC(CList_UserInfo)

public:
	CList_UserInfo();
	virtual ~CList_UserInfo();

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg int		OnCreate			(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnSize				(UINT nType, int cx, int cy);
	afx_msg void	OnNMClick			(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL	PreCreateWindow		(CREATESTRUCT& cs);

//	virtual int		OnCompareItems		(LPARAM lParam1, LPARAM lParam2, int iColumn);

	CFont			m_Font;

	virtual void	InitHeader			();

	BOOL			m_bIntiHeader		= FALSE;
	const int*		m_pHeadWidth		= NULL;

	// 사용자 목록 
	ST_UserInfo		m_stItemList;

	uint8_t			m_nLanguage			= 1;

	// 넘버링 다시 설정
	void		Reset_Number			();	

	// 아이템 데이터 리스트에 설정
	void		Set_ItemData			(__in int nItem, __in const ST_User_Unit* IN_pItem);

	// 아이템 추가
	void		Add_Item				(__in const ST_User_Unit* IN_pItem);
	// 아이템 수정
	void		Modify_Item				(__in int nItem, __in const ST_User_Unit* IN_pItem);

public:

	void		OnLanguage				(__in uint8_t IN_nLanguage);

	// 새로운 사용자 정보를 리스트에 넣음
	void		Set_ItemList			(__in const ST_UserInfo* IN_pItemList);
	void		Get_ItemList			(__out ST_UserInfo& OUT_ItemList);
	
	// 사용자 항목 추가
	void		Item_Insert				(__in ST_User_Unit& IN_Item);
	// 사용자 항목 삭제
	void		Item_Remove				();
	// 항목 수정
	void		Item_Modify				(__in const ST_User_Unit* IN_pItem);

	// 아이템 선택
	void		Select_Item				(__in int nItem);

	// 선택한 항목의 데이터 반환
	bool		Get_SelectedItem		(__out ST_User_Unit& OUT_Item);

	// 리스트의 정보 모두 제거
	void		Clear_List				();	

	// 아이템 정렬하여 리스트에 다시 표시
	void		Refresh_List			();

	// 아이템 수정
	void		Update_Item				(__in int nRow, __in const ST_User_Unit* IN_pSocket);
	
	// 중복 아이템 체크
	bool		IsExist_Name			(__in LPCTSTR IN_szName);
	bool		IsExist_ID				(__in LPCTSTR IN_szID);

};

#endif // List_UserInfo_h__
