//*****************************************************************************
// Filename	: 	Wnd_Cfg_User.h
// Created	:	2021/10/21 - 15:15
// Modified	:	2021/10/21 - 15:15
//
// Author	:	piring
//	
// Purpose	:	
//*****************************************************************************
#ifndef Wnd_Cfg_User_h__
#define Wnd_Cfg_User_h__

#pragma once
#include "Wnd_BaseView.h"
#include "VGStatic.h"
#include "List_UserInfo.h"

//-----------------------------------------------------------------------------
// CWnd_Cfg_User
//-----------------------------------------------------------------------------
class CWnd_Cfg_User : public CWnd_BaseView
{
	DECLARE_DYNAMIC(CWnd_Cfg_User)

public:
	CWnd_Cfg_User();
	virtual ~CWnd_Cfg_User();

	enum enUserSetting
	{
		US_Authority = 0,
		US_Name,
		US_ID,
		US_Password,
		US_Password_Confirm, // Confirm Password
		US_MaxCount,
	};

	enum enCtrlText
	{
		Txt_Add,
		Txt_Remove,
		Txt_Modify,

		Txt_MaxCount,
	};

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg int		OnCreate				(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnSize					(UINT nType, int cx, int cy);
	afx_msg void	OnShowWindow			(BOOL bShow, UINT nStatus);
	virtual BOOL	PreCreateWindow			(CREATESTRUCT& cs);
	virtual BOOL	PreTranslateMessage		(MSG* pMsg);

	afx_msg void	OnBnClicked_Add			();
	afx_msg void	OnBnClicked_Remove		();
	afx_msg void	OnBnClicked_Modify		();


	CFont			m_font_Default;
	CFont			m_font_Data;

	// 사용자 리스트
	CList_UserInfo	m_lc_UserList;
	
	// 추가, 제거 (리스트 자동 정렬)
	CMFCButton			m_bn_Add;
	CMFCButton			m_bn_Remove;	// 삭제 확인 팝업 창 사용
	CMFCButton			m_bn_Modify;

	

	CVGStatic			m_st_Caption[US_MaxCount];
	CComboBox			m_cb_Authority;
	CMFCMaskedEdit		m_ed_Name;
	CMFCMaskedEdit		m_ed_ID;
	CMFCMaskedEdit		m_ed_Password;
	CMFCMaskedEdit		m_ed_Password_Confirm;
	

	// 버튼 제어 함수
	void		Item_Insert				();
	void		Item_Remove				();
	void		Item_Modify				();
	void		Item_New				();

	// file 전체 경로
	CString		m_szPath_Default;
	CString		m_szSharedPath;			// 설비쪽 공유 파일 전체 경로
	bool		Save_UserList			();
	bool		Load_UserList			();


	// UI에 세팅 된 데이터 -> 구조체
	bool		Get_UIData				(__out ST_User_Unit& OUT_stUser);	

public:

	virtual void	OnLanguage			(__in uint8_t IN_nLanguage);

	// 권한 설정
	void		Set_PermissionMode		(__in enPermissionMode IN_PermissionMode);

	// 기본 저장 파일 전체 경로
	void		Set_FileFullPath		(__in LPCTSTR IN_szFullPath);
	void		Set_PathShared			(__in LPCTSTR IN_szSharedPath);

	// UI에 표시된 데이터의 구조체 반환	
	void		Get_Configuration		(__out ST_UserInfo& OUT_stUserList);

	// 모델 데이터를 UI에 표시
	void		Set_Configuration		(__in const ST_UserInfo* IN_pUserList);
	
	
};

#endif // Wnd_Cfg_User_h__


