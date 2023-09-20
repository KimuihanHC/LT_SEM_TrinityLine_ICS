//*****************************************************************************
// Filename	: 	List_UnloadInfo.h
// Created	:	2021/11/1 - 15:16
// Modified	:	2021/11/1 - 15:16
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#ifndef List_UnloadInfo_h__
#define List_UnloadInfo_h__

#pragma once
//#include <afxcmn.h>
#include <afxlistctrl.h>

//#include "SpeedListCtrl.h"
#include "Def_Socket_Info.h"

//=============================================================================
// CList_UnloadInfo
//=============================================================================
class CList_UnloadInfo : public CListCtrl
{
	DECLARE_DYNAMIC(CList_UnloadInfo)

public:
	CList_UnloadInfo();
	virtual ~CList_UnloadInfo();

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

	// 넘버링 다시 설정
	void		Reset_Number				();

	// 아이템 선택
	void		Select_SelectItem			(__in int nItem);

	// 설비 데이터 리스트에 설정
	void		Set_SocketData				(__in int nItem, __in const CSocketInfo_Unit* IN_pSocket);

public:
	void		Add_Socket					(const __in CSocketInfo_Unit* IN_pSocket);


};

#endif // List_UnloadInfo_h__
