//*****************************************************************************
// Filename	: Dlg_ChangePassword.h
// Created	: 2013/6/13
// Modified	: 2013/6/13 - 11:17
//
// Author	: PiRing
//	
// Purpose	: 
//*****************************************************************************
#ifndef Dlg_ChangePassword_h__
#define Dlg_ChangePassword_h__

#include "afxwin.h"
#include "resource.h"

#pragma once

//=============================================================================
// CDlg_ChangePassword 대화 상자입니다.
//=============================================================================
class CDlg_ChangePassword : public CDialogEx
{
	DECLARE_DYNAMIC(CDlg_ChangePassword)

public:
	CDlg_ChangePassword(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlg_ChangePassword();

	enum { IDD = IDD_DLG_CHANGE_PASSWORD };

protected:
	afx_msg int		OnCreate			(LPCREATESTRUCT lpCreateStruct);
	afx_msg HBRUSH	OnCtlColor			(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual void	DoDataExchange		(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL	PreCreateWindow		(CREATESTRUCT& cs);
	virtual BOOL	PreTranslateMessage	(MSG* pMsg);
	virtual BOOL	OnInitDialog();
	virtual void	OnCancel();
	virtual void	OnOK();

	DECLARE_MESSAGE_MAP()

	CFont			m_font_Default;

	CStatic			m_st_OldPassword;
	CStatic			m_st_NewPassword;
	CStatic			m_st_ConfirmPassword;
	CEdit			m_ed_OldPassword;
	CEdit			m_ed_NewPassword;
	CEdit			m_ed_ConfirmPassword;

	CString			m_szRegBase;

	BOOL			CheckPassword		();

public:
	
	void			SetRegistryPath		(__in LPCTSTR szRegPath)
	{
		m_szRegBase = szRegPath;
	};
};

#endif // Dlg_ChangePassword_h__
