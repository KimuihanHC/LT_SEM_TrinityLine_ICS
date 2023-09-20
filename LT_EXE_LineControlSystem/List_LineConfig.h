//*****************************************************************************
// Filename	: 	List_LineConfig.h
// Created	:	2021/11/1 - 15:16
// Modified	:	2021/11/1 - 15:16
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#ifndef List_LineConfig_h__
#define List_LineConfig_h__

#pragma once
#include <afxcmn.h>

#include "Def_ConfigLine.h"


//=============================================================================
// CList_LineConfig
//=============================================================================
class CList_LineConfig : public CListCtrl
{
	DECLARE_DYNAMIC(CList_LineConfig)

public:
	CList_LineConfig();
	virtual ~CList_LineConfig();

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
	CConfig_Line	m_stLine_Info;

	uint8_t			m_nLanguage			= 1;

	// 넘버링 다시 설정
	void		Reset_Number			();

	// 아이템 선택
	void		Select_SelectItem			(__in int nItem);

	// 설비 데이터 리스트에 설정
	void		Set_EquipmentData			(__in int nItem, __in const CConfig_Eqp* IN_pEquipment);

	// 원하는 위치에 아이템 삽입
	void		Insert_Equipment			(__in int nItem, __in const CConfig_Eqp* IN_pEquipment);
	// 아이템 추가
	void		Add_Equipment				(__in const CConfig_Eqp* IN_pEquipment);
	// 아이템 수정
	void		Modify_Equipment			(__in int nItem, __in const CConfig_Eqp* IN_pEquipment);

public:

	void		OnLanguage					(__in uint8_t IN_nLanguage);

	// 새로운 라인 정보를 리스트에 넣음
	void		Set_LineInfo				(__in const CConfig_Line* IN_pLineInfo);	
	void		Get_LineInfo				(__out CConfig_Line& OUT_LineInfo);

	// 설비 항목 추가
	void		Item_Add					(__in CConfig_Eqp& IN_Equipment);
	// 설비 항목 선택된 위치에 삽입
	void		Item_Insert					(__in CConfig_Eqp& IN_Equipment);
	// 설비 항목 삭제
	void		Item_Remove					();
	// 설비 항목 위로 이동
	void		Item_Up						();
	// 설비 항목 아래로 이동
	void		Item_Down					();
	// 설비 항목 선택된 위치의 내용을 변경
	void		Item_Modify					(__in CConfig_Eqp& IN_Equipment);
	// 선택한 항목의 데이터 반환
	bool		Get_SelectedEquipment		(__out CConfig_Eqp& OUT_Equipment);

	// 리스트에 설비 리스트 추가
	void		Add_LineInfo				(__in const CConfig_Line* IN_pLineInfo);
	// 리스트 컨트롤의 선택된 위치에 설비	리스트 삽입
	void		Insert_LineInfo				(__in const CConfig_Line* IN_pLineInfo);
	// 리스트의 라인 구성 정보 모두 제거
	void		Clear_LineInfo				();	

	// 설비 리스트에서 해당 설비 타입의 총 갯수
	UINT		Get_EquipmentCount			(__in UINT nEquipmentType);

	// 설비 리스트에서 설비 타입에 따른 설비 Alias 재설정
	//void		Check_EqpType_UpdateAlias	(__in UINT nEquipmentType, __in bool bUpdateListCtrl = true);
	// 설비 리스트의 설비 Alias 재설정
	//void		Check_EqpType_UpdateAlias	();

	// 설비 리스트에서 설비 타입에 따른 설비 Alias 재설정
	void		Check_EqpTypeUI_UpdateAlias	(__in UINT nEqpTypeUI, __in bool bUpdateListCtrl = true);
	// 설비 리스트의 설비 Alias 재설정
	void		Check_EqpTypeUI_UpdateAlias	();

	// Check EQP ID 중복
	bool		Check_EquipmentID			();
	bool		Check_EquipmentID			(__in LPCTSTR IN_szEquipmentID);
	// Check ip address 중복
	bool		Check_IPAddress				();
	bool		Check_IPAddress				(__in DWORD IN_nIPAddress);

	// 설비 중 검사 그룹 체크
	void		Set_Group_Equipment			();

	// *** Loader, Unloader, Returner x 2
	// 고정 설비로 추가

};

#endif // List_LineConfig_h__
