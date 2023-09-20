//*****************************************************************************
// Filename	: 	Dlg_AcessMode.h
// Created	:	2016/11/6 - 19:05
// Modified	:	2016/11/6 - 19:05
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
#ifndef Dlg_AcessMode_h__
#define Dlg_AcessMode_h__

#pragma once

#include "resource.h"
#include "Def_Enum_Cm.h"
#include "VGStatic.h"
#include "afxwin.h"
#include "Reg_Management.h"


//-----------------------------------------------------------------------------
// CDlg_AcessMode dialog
//-----------------------------------------------------------------------------
class CDlg_AcessMode : public CDialogEx
{
	DECLARE_DYNAMIC(CDlg_AcessMode)

public:
	CDlg_AcessMode(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg_AcessMode();

// Dialog Data
	enum { IDD = IDD_DLG_ACESS_MODE };

protected:
	virtual void	DoDataExchange		(CDataExchange* pDX);    // DDX/DDV support
	afx_msg int		OnCreate			(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnSize				(UINT nType, int cx, int cy);
	virtual BOOL	PreTranslateMessage	(MSG* pMsg);
	virtual BOOL	OnInitDialog		();

	afx_msg void	OnEnChangeEdPassword();
	afx_msg void	OnBnClickedRbOperMode();
	afx_msg void	OnBnClickedRbManagerMode();

	virtual void	OnOK();
	virtual void	OnCancel();



	DECLARE_MESSAGE_MAP()

	CFont			m_font_Large;
	CFont			m_font_Default;

	// User
	// Password
	// LogIn, LogOut

	CVGStatic			m_st_Title;

	CMFCButton			m_rb_OperMode;
	CMFCButton			m_rb_AdminMode;

	CVGStatic			m_st_Password;
	CEdit				m_ed_Password;

	CButton				m_bn_OK;
	CButton				m_bn_Cancel;

	enPermissionMode	m_AcessMode;
	CReg_Management		m_regManagement;
	CString				m_szUserName;

	void		LoadAcessMode			();
	void		SaveAcessMode			();

	BOOL		CheckPassword			();

public:

	void		SetAcessMode			(__in enPermissionMode nAcessMode);

	enPermissionMode	GetAcessMode()
	{
		return m_AcessMode;
	};
	
	CString		GetUserName()
	{
		return m_szUserName;
	};
	
};

#endif // Dlg_AcessMode_h__
