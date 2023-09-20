//*****************************************************************************
// Filename	: 	Dlg_ModifyUser.h
// Created	:	2021/1/20 - 12:10
// Modified	:	2021/1/20 - 12:10
//
// Author	:	piring
//	
// Purpose	:	
//*****************************************************************************
#ifndef Dlg_ModifyUser_h__
#define Dlg_ModifyUser_h__

#pragma once

#include "resource.h"
#include "VGStatic.h"
#include "Def_UserInfo.h"

//=============================================================================
// CDlg_ModifyUser 대화 상자입니다.
//=============================================================================
class CDlg_ModifyUser : public CDialogEx
{
	DECLARE_DYNAMIC(CDlg_ModifyUser)

public:
	CDlg_ModifyUser(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlg_ModifyUser();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_MODIFY_STEP };

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
		Txt_DialogCaption,
		Txt_OK,
		Txt_Cancel,
		Txt_MaxCount,
	};

protected:
	virtual void	DoDataExchange			(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	afx_msg int		OnCreate				(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnSize					(UINT nType, int cx, int cy);
	virtual BOOL	PreTranslateMessage		(MSG* pMsg);
	virtual BOOL	OnInitDialog			();

	virtual void	OnCancel				();
	virtual void	OnOK					();

	DECLARE_MESSAGE_MAP()

	CFont			m_font_Default;
	CFont			m_Font;

	CButton			m_bn_OK;
	CButton			m_bn_Cancel;
	

	// 기본 데이터 ------------------------------------------------
	CVGStatic			m_st_Caption[US_MaxCount];
	CComboBox			m_cb_Authority;
	CMFCMaskedEdit		m_ed_Name;
	CMFCMaskedEdit		m_ed_ID;
	CMFCMaskedEdit		m_ed_Password;
	CMFCMaskedEdit		m_ed_Password_Confirm;

	ST_User_Unit		m_stNGCode;

	uint8_t				m_nLanguage			= 1;
	
	// UI에 세팅 된 데이터 -> 구조체
	bool			Get_UIData				(__out ST_User_Unit& OUT_stUser);
	void			Set_UIData				(__in const ST_User_Unit* IN_pstUser);

public:

	void			OnLanguage			(__in uint8_t IN_nLanguage);

	void			Set_NGCodeData		(__in const ST_User_Unit* IN_pstUser);
	ST_User_Unit&	Get_NGCodeData		();

};

#endif // Dlg_ModifyUser_h__
