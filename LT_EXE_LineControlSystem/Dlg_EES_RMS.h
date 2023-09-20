//*****************************************************************************
// Filename	: 	Dlg_UserLogin.h
// Created	:	2021/11/6 - 19:05
// Modified	:	2021/11/6 - 19:05
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
#ifndef Dlg_EES_RMS_h__
#define Dlg_EES_RMS_h__

#pragma once

#include <afxwin.h>
#include "resource.h"
#include "VGStatic.h"
#include "Def_ConstantServer.h"

#define USE_NEW_CODE	// 리스트 분류

//-----------------------------------------------------------------------------
// CDlg_UserLogin dialog
//-----------------------------------------------------------------------------
class CDlg_EES_RMS : public CDialogEx
{
	DECLARE_DYNAMIC(CDlg_EES_RMS)

public:
	CDlg_EES_RMS(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg_EES_RMS();

// Dialog Data
	enum { IDD = IDD_DLG_ACESS_MODE };	

protected:
	virtual void	DoDataExchange		(CDataExchange* pDX);    // DDX/DDV support
	afx_msg int		OnCreate			(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnSize				(UINT nType, int cx, int cy);
	virtual BOOL	PreTranslateMessage	(MSG* pMsg);
	virtual BOOL	OnInitDialog		();

	afx_msg void	OnAuthoritySelect(UINT nID);


	DECLARE_MESSAGE_MAP()

	CFont			m_font_Large;
	CFont			m_font_Default;
	CMFCButton		m_bn_Keyboard;

	CVGStatic		m_st_Title;

	CMFCButton		m_bn_CtrlCmd[enEES_Mode::EES_MAX];

	uint8_t			m_nLanguage		= 1;

	enEES_Mode		m_nEES;
public:
	uint8_t			Get_EES();
};

#endif // Dlg_EES_RMS_h__
