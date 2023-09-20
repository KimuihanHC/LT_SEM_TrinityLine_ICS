//*****************************************************************************
// Filename	: 	List_SocketRFID.h
// Created	:	2021/11/1 - 15:16
// Modified	:	2021/11/1 - 15:16
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#ifndef List_SocketRFID_h__
#define List_SocketRFID_h__

#pragma once
//#include <afxcmn.h>
#include <afxlistctrl.h>

//#include "SpeedListCtrl.h"
#include "Def_Socket_Config.h"


//=============================================================================
// CList_SocketRFID
//=============================================================================
class CList_SocketRFID : public CListCtrl
{
	DECLARE_DYNAMIC(CList_SocketRFID)

public:
	CList_SocketRFID();
	virtual ~CList_SocketRFID();

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

	// 라인의 설비 구성 정보 구조체
	CSocketList		m_stSocketList;

	uint8_t			m_nLanguage = 1;

	// 넘버링 다시 설정
	void		Reset_Number				();

	// 아이템 선택
	void		Select_SelectItem			(__in int nItem);

	// 설비 데이터 리스트에 설정
	void		Set_SocketData				(__in int nItem, __in const CConfig_Socket* IN_pSocket);

	// 아이템 추가
	void		Add_Socket					(__in const CConfig_Socket* IN_pSocket);
	// 아이템 수정
	void		Modify_Socket				(__in int nItem, __in const CConfig_Socket* IN_pSocket);

public:

	void		OnLanguage					(__in uint8_t IN_nLanguage);

	// 새로운 라인 정보를 리스트에 넣음
	void		Set_SocketList				(__in const CSocketList* IN_pSoscketList);
	void		Get_SocketList				(__out CSocketList& OUT_Socket);

	// 설비 항목 추가
	bool		Item_Insert					(__in CConfig_Socket& IN_Socket);
	// 설비 항목 삭제
	void		Item_Remove					();
	
	// 설비 항목 선택된 위치의 내용을 변경
	void		Item_Modify					(__in CConfig_Socket& IN_Socket);
	// 선택한 항목의 데이터 반환
	bool		Get_SelectedSocket			(__out CConfig_Socket& OUT_Socket);

	// 리스트에 설비 리스트 추가
	void		Add_Socket					(__in const CSocketList* IN_pSoscketList);
	// 리스트 컨트롤의 선택된 위치에 설비	리스트 삽입
	void		Insert_Socket				(__in const CSocketList* IN_pSoscketList);
	// 리스트의 라인 구성 정보 모두 제거
	void		Clear_SocketList			();	

	// 설비 리스트에서 해당 설비 타입의 총 갯수
	UINT		Get_SocketCount				(__in UINT nSocketType);



	// 아이템 정렬하여 리스트에 다시 표시
	void		Refresh_Socket				();

};

#endif // List_SocketRFID_h__
