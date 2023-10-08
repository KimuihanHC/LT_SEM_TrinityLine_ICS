//*****************************************************************************
// Filename	: 	List_FailInfo.h
// Created	:	2021/12/20 - 14:58
// Modified	:	2021/12/20 - 14:58
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#ifndef List_FailInfo_h__
#define List_FailInfo_h__

#pragma once
#include <afxlistctrl.h>
#include "FailInfo_Eqp.h"

//=============================================================================
// CList_FailInfo
//=============================================================================
class CList_FailInfo : public CListCtrl
{
	DECLARE_DYNAMIC(CList_FailInfo)

public:
	CList_FailInfo();
	virtual ~CList_FailInfo();

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg int		OnCreate			(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnSize				(UINT nType, int cx, int cy);
	afx_msg void	OnNMClick			(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL	PreCreateWindow		(CREATESTRUCT& cs);

	afx_msg LRESULT	OnWM_NotifyNGCode	(WPARAM wParam, LPARAM lParam);

	CFont			m_Font;

	virtual void	InitHeader			();

	BOOL			m_bIntiHeader		= FALSE;
	const int*		m_pHeadWidth		= NULL;

	// 라인의 설비 구성 정보 구조체
	const CFailInfo_Eqp*	m_pNGCountList	= nullptr;

	uint8_t			m_nParaCount		= 2;

	uint8_t			m_nLanguage			= 1;

	int			Find_Row				(__in int16_t IN_nNGCode);

	// 아이템 선택
	void		Select_SelectItem		(__in int nItem);

	// 설비 데이터 리스트에 설정
	void		Set_NGCountData			(__in int nItem, __in const ST_NGCode_Count* IN_pNGCount);

	// 아이템 추가
	void		Add_NGCount				(__in const ST_NGCode_Count* IN_pNGCount);

	void		Update_NGCount			(__in int nRow, __in const ST_NGCode_Count* IN_pNGCount);
public:

	void		OnLanguage				(__in uint8_t IN_nLanguage);

	// 새로운 라인 정보를 리스트에 넣음
	void		Set_NGCountList			(__in const CFailInfo_Eqp* IN_pFailInfo_Eqp);
	
	// 리스트의 라인 구성 정보 모두 제거
	void		Clear_NGCountList		();	

	// 아이템 정렬하여 리스트에 다시 표시
	void		Refresh_NGCount			();

	// 아이템 수정
	void		Update_NGCount			(__in int16_t IN_nNGCode, __in const CFailInfo_Eqp* IN_pFailInfo_Eqp);
	void		Update_NGCount			(__in int16_t IN_nNGCode);

	// 검사 파라 설정
	void		Set_ParaCount			(__in uint8_t IN_nCount);

	// 검사 Para 열의 너비 조정
	void		Adjust_ColumnWidth		(__in int IN_nWidth);
};

#endif // List_FailInfo_h__
