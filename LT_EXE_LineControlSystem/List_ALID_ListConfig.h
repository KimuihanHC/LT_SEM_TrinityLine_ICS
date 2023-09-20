//*****************************************************************************
// Filename	: 	List_LineConfig.h
// Created	:	2021/11/1 - 15:16
// Modified	:	2021/11/1 - 15:16
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#ifndef List_ALID_LISTConfig_h__
#define List_ALID_LISTConfig_h__

#pragma once
#include <afxcmn.h>

#include "Def_Config_ALID_LIST.h"

//=============================================================================
// CList_LineConfig
//=============================================================================
class CList_AL_ListConfig : public CListCtrl
{
	DECLARE_DYNAMIC(CList_AL_ListConfig)

public:
	CList_AL_ListConfig();
	virtual ~CList_AL_ListConfig();

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg int		OnCreate			(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnSize				(UINT nType, int cx, int cy);
	afx_msg void	OnNMClick			(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL	PreCreateWindow		(CREATESTRUCT& cs);

	CFont			m_Font;

	virtual void	InitSVHeader();

	BOOL			m_bIntiHeader		= FALSE;
	const int*		m_pHeadWidth = NULL;

	uint8_t			m_nLanguage			= 1;



	// 넘버링 다시 설정
	void		Reset_Number();

	// 아이템 선택
	void		Select_SelectItem			(__in int nItem);

	// 설비 데이터 리스트에 설정
	void		Set_AL_Data					(__in int nItem, __in const CConfig_AL* IN_pSV);

	// 원하는 위치에 아이템 삽입
	void		Insert_AL					(__in int nItem, __in const CConfig_AL* IN_pSV);

	// 아이템 추가
	void		Add_AL						(__in const CConfig_AL* IN_pSV);

	// 아이템 수정
	void		Modify_AL					(__in int nItem, __in const CConfig_AL* IN_pSV);
public:
	// 라인의 설비 구성 정보 구조체
	CConfig_AL_LIST	m_stAL_Info;

	void		OnLanguage					(__in uint8_t IN_nLanguage);

	// 새로운 라인 정보를 리스트에 넣음
	void		Set_ALInfo					(__in const CConfig_AL_LIST* IN_pSVInfo);
	void		Get_ALInfo					(__out CConfig_AL_LIST& OUT_SVInfo);

	// 설비 항목 추가
	void		Item_Add					(__in CConfig_AL& IN_SV);	
	// 설비 항목 선택된 위치에 삽입
	void		Item_Insert					(__in CConfig_AL& IN_Equipment);
	// 설비 항목 삭제
	void		Item_Remove					();
	// 설비 항목 위로 이동
	void		Item_Up						();
	// 설비 항목 아래로 이동
	void		Item_Down					();
	// 설비 항목 선택된 위치의 내용을 변경
	void		Item_Modify					(__in CConfig_AL& IN_SV);
	// 선택한 항목의 데이터 반환
	bool		Get_SelectedAL				(__out CConfig_AL& OUT_SV);
	// 리스트에 설비 리스트 추가
	void		Add_ALInfo					(__in const CConfig_AL_LIST* IN_pSVInfo);
	// 리스트 컨트롤의 선택된 위치에 설비	리스트 삽입
	void		Insert_ALInfo				(__in const CConfig_AL_LIST* IN_pSVInfo);
	// 리스트의 라인 구성 정보 모두 제거
	void		Clear_ALInfo();
	//  
	//void		Check_SV_UI_Update();
	// Check AL ID 중복
	bool		Check_ALID					();
	bool		Check_ALID					(__in uint32_t IN_nSID);


	// *** Loader, Unloader, Returner x 2
	// 고정 설비로 추가

};

#endif // List_LineConfig_h__
