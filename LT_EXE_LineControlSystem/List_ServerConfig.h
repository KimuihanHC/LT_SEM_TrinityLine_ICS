//*****************************************************************************
// Filename	: 	List_LineConfig.h
// Created	:	2021/11/1 - 15:16
// Modified	:	2021/11/1 - 15:16
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#ifndef List_ServerConfig_h__
#define List_ServerConfig_h__

#pragma once
#include <afxcmn.h>

#include "Def_ConfigLineServer.h"


//=============================================================================
// CList_LineConfig
//=============================================================================
class CList_ServerConfig : public CListCtrl
{
	DECLARE_DYNAMIC(CList_ServerConfig)

public:
	CList_ServerConfig();
	virtual ~CList_ServerConfig();

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
	CConfig_Server	m_stLine_Info;

	uint8_t			m_nLanguage			= 1;

	// 넘버링 다시 설정
	void		Reset_Number			();

	// 아이템 선택
	void		Select_SelectItem			(__in int nItem);

	// 설비 데이터 리스트에 설정
	void		Set_ServerData				(__in int nItem, __in const CConfig_Svr* IN_pEquipment);

	// 원하는 위치에 아이템 삽입
	//2023.02.13a uhkim [추가 옵션]
	void		Insert_Server				(__in int nItem, __in const CConfig_Svr* IN_pEquipment);
	// 아이템 추가
	void		Add_Server					(__in const CConfig_Svr* IN_pEquipment);	//2023.02.13a uhkim [추가 옵션]
	// 아이템 수정
	void		Modify_Server				(__in int nItem, __in const CConfig_Svr* IN_pEquipment);
public:

	void		OnLanguage					(__in uint8_t IN_nLanguage);

	// 새로운 라인 정보를 리스트에 넣음
	void		Set_LineInfo				(__in const CConfig_Server* IN_pLineInfo);
	void		Get_LineInfo				(__out CConfig_Server& OUT_LineInfo);

	// 설비 항목 추가
	void		Item_Add					(__in CConfig_Svr& IN_Equipment);
	// 설비 항목 선택된 위치에 삽입
	void		Item_Insert					(__in CConfig_Svr& IN_Equipment);
	// 설비 항목 삭제
	void		Item_Remove					();
	// 설비 항목 위로 이동
	void		Item_Up						();
	// 설비 항목 아래로 이동
	void		Item_Down					();
	// 설비 항목 선택된 위치의 내용을 변경
	void		Item_Modify					(__in CConfig_Svr& IN_Equipment);
	// 선택한 항목의 데이터 반환
	bool		Get_Selected				(__out CConfig_Svr& OUT_Equipment);
	// 리스트에 설비 리스트 추가
	void		Add_LineInfo				(__in const CConfig_Server* IN_pLineInfo);
	// 리스트 컨트롤의 선택된 위치에 설비	리스트 삽입
	void		Insert_LineInfo				(__in const CConfig_Server* IN_pLineInfo);
	// 리스트의 라인 구성 정보 모두 제거
	void		Clear_LineInfo				();	

	// 설비 리스트에서 해당 설비 타입의 총 갯수
	UINT		Get_ServerCount				(__in UINT nEquipmentType);

	// 설비 리스트에서 설비 타입에 따른 설비 Alias 재설정
	//void		Check_EqpType_UpdateAlias	(__in UINT nEquipmentType, __in bool bUpdateListCtrl = true);
	// 설비 리스트의 설비 Alias 재설정
	//void		Check_EqpType_UpdateAlias	();

	// 설비 리스트에서 설비 타입에 따른 설비 Alias 재설정
	void		Check_UI_UpdateAlias		(__in UINT nEqpTypeUI, __in bool bUpdateListCtrl = true);
	// 설비 리스트의 설비 Alias 재설정
	void		Check_UI_UpdateAlias		();

	// Check EQP ID 중복
	bool		Check_ServerID				();
	bool		Check_ServerID				(__in LPCTSTR IN_szEquipmentID);
	// Check ip address 중복
	bool		Check_IPAddress				();
	bool		Check_IPAddress				(__in DWORD IN_nIPAddress);

	// 설비 중 검사 그룹 체크
	void		Set_Group_Server			();

	// *** Loader, Unloader, Returner x 2
	// 고정 설비로 추가

};

#endif // List_LineConfig_h__
