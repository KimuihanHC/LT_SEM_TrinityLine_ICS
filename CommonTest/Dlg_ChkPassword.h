//*****************************************************************************
// Filename	: Dlg_ChkPassword.h
// Created	: 2010/10/29
// Modified	: 2010/10/29 - 10:15
//
// Author	: PiRing
//	
// Purpose	: 
//*****************************************************************************
#ifndef Dlg_ChkPassword_h__
#define Dlg_ChkPassword_h__

#include "resource.h"

#pragma once

//=============================================================================
// CDlg_ChkPassword 대화 상자입니다.
//=============================================================================
class CDlg_ChkPassword : public CDialogEx
{
	DECLARE_DYNAMIC(CDlg_ChkPassword)

public:
	CDlg_ChkPassword(CWnd* pParent = NULL);   // 표준 생성자입니다.
	CDlg_ChkPassword(UINT nIndex, CWnd* pParent = NULL);
	virtual ~CDlg_ChkPassword();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_CHK_PASSWORD };

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

	CStatic			m_st_Password;
	CEdit			m_ed_Password;
	CButton			m_bn_OK;
	CButton			m_bn_Cancel;

	CFont			m_font_Default;

	UINT			m_nPassIndex;
	CString			m_szRegBase;

	BOOL			CheckPassword		();
	CString			GetPassword			();
public:
	
	void			SetPasswordIndex	(__in UINT nIndex);
	void			SetRegistryPath		(__in LPCTSTR szRegPath)
	{
		m_szRegBase = szRegPath;
	};
};

#endif // Dlg_ChkPassword_h__
