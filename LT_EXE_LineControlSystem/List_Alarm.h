//*****************************************************************************
// Filename	: 	List_Alarm.h
// Created	:	2021/12/20 - 14:58
// Modified	:	2021/12/20 - 14:58
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#ifndef List_Alarm_h__
#define List_Alarm_h__

#pragma once
//#include <afxcmn.h>
#include <afxlistctrl.h>

//#include "SpeedListCtrl.h"
#include "Def_Socket_Info.h"
#include "SocketMonitoring.h"


//=============================================================================
// CList_Alarm
//=============================================================================
class CList_Alarm : public CListCtrl
{
	DECLARE_DYNAMIC(CList_Alarm)

public:
	CList_Alarm();
	virtual ~CList_Alarm();

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
	CSocketMonitoring		m_stSocketList;

	int			Find_Row					(__in LPCTSTR IN_szRFID);

	// 넘버링 다시 설정
	void		Reset_Number				();

	// 아이템 선택
	void		Select_SelectItem			(__in int nItem);

	// 설비 데이터 리스트에 설정
	void		Set_SocketData				(__in int nItem, __in const CSocketInfo_Unit* IN_pSocket);
	void		Set_SocketData_Location		(__in int nItem, __in const CSocketInfo_Unit* IN_pSocket);
	void		Set_SocketData_Product		(__in int nItem, __in const CSocketInfo_Unit* IN_pSocket);
	void		Set_SocketData_Yield		(__in int nItem, __in const CSocketInfo_Unit* IN_pSocket);

	// 아이템 추가
	void		Add_Socket					(__in const CSocketInfo_Unit* IN_pSocket);

public:

	// 새로운 라인 정보를 리스트에 넣음
	void		Set_SocketList				(__in const CSocketMonitoring* IN_pSoscketList);
	void		Get_SocketList				(__out CSocketMonitoring& OUT_Socket);
	
	// 선택한 항목의 데이터 반환
	bool		Get_SelectedSocket			(__out CSocketInfo_Unit& OUT_Socket);

	// 리스트의 라인 구성 정보 모두 제거
	void		Clear_SocketList			();	

	// 아이템 정렬하여 리스트에 다시 표시
	void		Refresh_Socket				();

	// 아이템 수정
	void		Update_Socket				(__in int nRow, __in const CSocketInfo_Unit* IN_pSocket);
	void		Update_Socket				(__in LPCTSTR IN_szRFID, __in const CSocketInfo_Unit* IN_pSocket);

	void		Update_Socket_Location		(__in int nRow, __in const CSocketInfo_Unit* IN_pSocket);
	void		Update_Socket_Product		(__in int nRow, __in const CSocketInfo_Unit* IN_pSocket);
	void		Update_Socket_Yield			(__in int nRow, __in const CSocketInfo_Unit* IN_pSocket);


	// 아이템 찾기
	int			Find_Socket					(__in LPCTSTR IN_szRFID);

};

#endif // List_Alarm_h__
