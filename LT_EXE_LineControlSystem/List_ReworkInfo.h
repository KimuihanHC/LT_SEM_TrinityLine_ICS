//*****************************************************************************
// Filename	: 	List_ReworkInfo.h
// Created	:	2021/11/1 - 15:16
// Modified	:	2021/11/1 - 15:16
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#ifndef List_ReworkInfo_h__
#define List_ReworkInfo_h__

#pragma once
//#include <afxcmn.h>
#include <afxlistctrl.h>

//#include "SpeedListCtrl.h"
#include "Rework_Info.h"


//=============================================================================
// CList_ReworkInfo
//=============================================================================
class CList_ReworkInfo : public CListCtrl
{
	DECLARE_DYNAMIC(CList_ReworkInfo)

public:
	CList_ReworkInfo();
	virtual ~CList_ReworkInfo();

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

	// 라인의 아이템 구성 정보 구조체
	CRework_Info		m_stReworkList;

	// 넘버링 다시 설정
	void		Reset_Number				();

	// 아이템 선택
	void		Select_SelectItem			(__in int nItem);

	// 아이템 데이터 리스트에 설정
	void		Set_ItemData				(__in int nItem, __in LPCTSTR IN_szBarcode, __in const ST_ReworkProduct* IN_pRework);

	// 아이템 추가
	void		Add_Item					(__in LPCTSTR IN_szBarcode, __in const ST_ReworkProduct* IN_pRework);
	// 아이템 수정
	void		Modify_Item					(__in int nItem, __in LPCTSTR IN_szBarcode, __in const ST_ReworkProduct* IN_pRework);

public:

	// 새로운 라인 정보를 리스트에 넣음
	void		Set_ItemList				(__in const CRework_Info* IN_pReworkList);
	//void		Get_ReworkList				(__out CReworkList& OUT_Rework);

	// 아이템 추가
	bool		Item_Insert					(__in LPCTSTR IN_szBarcode, __in ST_ReworkProduct& IN_Rework);
	// 아이템 삭제
	void		Item_Remove					();
	
	// 선택된 아이템의 내용을 변경
	void		Item_Modify					(__in LPCTSTR IN_szBarcode, __in ST_ReworkProduct& IN_Rework);
	// 선택한 아이템의 데이터 반환
	bool		Get_SelectedItem			(__out ST_ReworkProduct& OUT_Rework);

	// 리스트에 아이템 리스트 추가
	void		Add_Item					(__in const CRework_Info* IN_pReworkList);
	// 리스트 컨트롤의 선택된 위치에 아이템	리스트 삽입
	//void		Insert_Item					(__in const CRework_Info* IN_pReworkList);
	// 리스트의 라인 구성 정보 모두 제거
	void		Clear_ItemList				();	

	
	// 아이템 정렬하여 리스트에 다시 표시
	void		Refresh_ItemList			();

};

#endif // List_ReworkInfo_h__
