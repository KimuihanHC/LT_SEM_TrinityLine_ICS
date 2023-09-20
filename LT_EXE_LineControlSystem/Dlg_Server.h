//*****************************************************************************
// Filename	: 	Dlg_UserLogin.h
// Created	:	2021/11/6 - 19:05
// Modified	:	2021/11/6 - 19:05
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
#ifndef Dlg_Server_h__
#define Dlg_Server_h__

#pragma once

#include <afxwin.h>
#include "resource.h"
//#include "VGStatic.h"
//#include "Def_ConstantServer.h"
#include <vector>

#include "Wnd_Status_Server.h"

#define USE_NEW_CODE	// 리스트 분류

//-----------------------------------------------------------------------------
// CDlg_UserLogin dialog
//-----------------------------------------------------------------------------
class CDlg_Server : public CDialogEx
{
	DECLARE_DYNAMIC(CDlg_Server)

public:
	CDlg_Server(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg_Server();

// Dialog Data
	enum { IDD = IDD_DLG_ACESS_MODE };	

protected:
	virtual void	DoDataExchange		(CDataExchange* pDX);    // DDX/DDV support
	afx_msg int		OnCreate			(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnSize				(UINT nType, int cx, int cy);
	virtual BOOL	PreTranslateMessage	(MSG* pMsg);
	virtual BOOL	OnInitDialog		();

	afx_msg void	OnAuthoritySelect(UINT nID);
	afx_msg void	OnAuthorityRMSSelect(UINT nID);


	DECLARE_MESSAGE_MAP()

	CFont			m_font_Large;
	CFont			m_font_Default;

	CString			m_szTitle;
	CVGStatic		m_st_Title;

	std::vector<CWnd_Status_Server*>		m_pwndServerList;
	uint8_t			m_nLanguage		= 1;
	CMFCButton		m_bn_CtrlCmd[enOnlineState::ONLINESTATE_MaxCount];
	CMFCButton		m_bn_CtrlRms[enEES_Mode::EES_MAX];
	enOnlineState	m_nEES;
public:		
	void			OnLanguage(__in uint8_t IN_nLanguage);
	void			Set_Wnd_Status_Server(std::vector<CWnd_Status_Server*> IN_Status_Server);
	HWND			Get_ServerHWND(__in uint8_t IN_nSvrOrder);
};

#endif // Dlg_Server_h__
