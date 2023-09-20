//*****************************************************************************
// Filename	: 	Dlg_UserLogin.h
// Created	:	2021/11/6 - 19:05
// Modified	:	2021/11/6 - 19:05
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
#ifndef Dlg_UserLogin_h__
#define Dlg_UserLogin_h__

#pragma once

#include <afxwin.h>
#include "resource.h"
#include "VGStatic.h"
#include "List_UserInfo.h"

#define USE_NEW_CODE	// 리스트 분류

//-----------------------------------------------------------------------------
// CDlg_UserLogin dialog
//-----------------------------------------------------------------------------
class CDlg_UserLogin : public CDialogEx
{
	DECLARE_DYNAMIC(CDlg_UserLogin)

public:
	CDlg_UserLogin(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg_UserLogin();

// Dialog Data
	enum { IDD = IDD_DLG_ACESS_MODE };	

protected:
	virtual void	DoDataExchange		(CDataExchange* pDX);    // DDX/DDV support
	afx_msg int		OnCreate			(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnSize				(UINT nType, int cx, int cy);
	virtual BOOL	PreTranslateMessage	(MSG* pMsg);
	virtual BOOL	OnInitDialog		();

	afx_msg void	OnEnChange_Password();
	afx_msg void	OnBnClicked_LogIn();
	afx_msg void	OnBnClicked_LogOut();
	afx_msg void	OnBnClicked_Keyboard();
	afx_msg void	OnAuthoritySelect(UINT nID);
	afx_msg void	OnLbnSelchangeList_User_Admin	(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void	OnLbnSelchangeList_User_Engineer(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void	OnLbnSelchangeList_User_Oper	(NMHDR* pNMHDR, LRESULT* pResult);

	virtual void	OnOK();
	virtual void	OnCancel();



	DECLARE_MESSAGE_MAP()

	CFont			m_font_Large;
	CFont			m_font_Default;


	CMFCButton			m_bn_Keyboard;

	// 사용자 리스트
	CVGStatic			m_st_Title;
	CList_UserInfo		m_lc_UserList[Permission_MaxCount];	// 권한별로 분류 (Permission_MaxCount)

	CMFCButton			m_rb_Authority[Permission_MaxCount];

	CVGStatic			m_st_ID;
	CComboBox			m_cb_ID;

	CVGStatic			m_st_Password;
	CEdit				m_ed_Password;

	CButton				m_bn_LogIn;
	CButton				m_bn_LogOut;

	// 암호 변경

	ST_UserInfo			m_stUserList;

	bool				m_bLogIn		= false;
	ST_User_Unit		m_stLogin;

	enPermissionMode	m_AcessMode		= Permission_Operator;

	uint8_t				m_nLanguage		= 1;

	bool		Load_UserList			();
	bool		CheckPassword			();

public:

	void		OnLanguage				(__in uint8_t IN_nLanguage);

	// 현재 로그인 정보
	ST_User_Unit&	Get_Login_User		();


	void		SetAcessMode			(__in enPermissionMode nAcessMode);

	enPermissionMode GetAcessMode		()
	{
		return m_AcessMode;
	};
	
};

#endif // Dlg_UserLogin_h__
