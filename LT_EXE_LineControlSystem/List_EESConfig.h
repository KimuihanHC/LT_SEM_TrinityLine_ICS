//*****************************************************************************
// Filename	: 	List_EESConfig.h
// Created	:	
// Modified	:	2023.04.21 - 11:00
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#ifndef List_EESConfig_h__
#define List_EESConfig_h__

#pragma once
#include <afxcmn.h>

#include "Def_Config_EES_LIST.h"

//=============================================================================
// CList_LineConfig
//=============================================================================
class CList_EESConfig : public CListCtrl
{
	DECLARE_DYNAMIC(CList_EESConfig)

public:
	CList_EESConfig();
	virtual ~CList_EESConfig();

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg int		OnCreate			(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnSize				(UINT nType, int cx, int cy);
	afx_msg void	OnNMClick			(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL	PreCreateWindow		(CREATESTRUCT& cs);

	CFont			m_Font;

	virtual void	InitEESHeader();

	BOOL			m_bIntiHeader		= FALSE;
	const int*		m_pHeadWidth = NULL;

	uint8_t			m_nLanguage			= 1;


	// 넘버링 다시 설정
	void		Reset_Number();
	// 아이템 선택
	void		Select_SelectItem			(__in int nItem);
	// 설비 데이터 리스트에 설정
	void		Set_EES_Data				(__in int nItem, __in const CConfig_EES* IN_pData);
	// 원하는 위치에 아이템 삽입
	void		Insert_EES					(__in int nItem, __in const CConfig_EES* IN_pData);
	// 아이템 추가
	void		Add_EES						(__in const CConfig_EES* IN_pData);
	// 아이템 수정
	void		Modify_EES					(__in int nItem, __in const CConfig_EES* IN_pData);
public:
	// 라인의 설비 구성 정보 구조체
	CConfig_EES_LIST	m_stEES_Info;

	void		OnLanguage					(__in uint8_t IN_nLanguage);

	// 새로운 라인 정보를 리스트에 넣음
	void		Set_EESInfo					(__in const CConfig_EES_LIST* IN_pInfo);
	void		Get_EESInfo					(__out CConfig_EES_LIST& OUT_Info);

	// 설비 항목 추가
	void		Item_Add					(__in CConfig_EES& IN_Data);
	// 설비 항목 선택된 위치에 삽입
	void		Item_Insert					(__in CConfig_EES& IN_Data);
	// 설비 항목 삭제
	void		Item_Remove					();
	// 설비 항목 위로 이동
	void		Item_Up						();
	// 설비 항목 아래로 이동
	void		Item_Down					();
	// 설비 항목 선택된 위치의 내용을 변경
	void		Item_Modify					(__in CConfig_EES& IN_Data);
	// 선택한 항목의 데이터 반환
	bool		Get_SelectedEES				(__out CConfig_EES& OUT_Data);
	// 리스트에 설비 리스트 추가
	void		Add_EESInfo					(__in const CConfig_EES_LIST* IN_pInfo);
	// 리스트 컨트롤의 선택된 위치에 설비	리스트 삽입
	void		Insert_EESInfo				(__in const CConfig_EES_LIST* IN_pInfo);
	// 리스트의 라인 구성 정보 모두 제거
	void		Clear_EESInfo();
	// Check SV ID 중복
	bool		Check_EESCode();
	bool		Check_EESCode(__in LPCTSTR IN_Data);
	// *** Loader, Unloader, Returner x 2
	// 고정 설비로 추가
};

#endif // List_EESConfig_h__
