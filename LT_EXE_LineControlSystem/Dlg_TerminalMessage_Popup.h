//*****************************************************************************
// Filename	: 	Dlg_TerminalMessage_Popup.h
// Created	:	
// Modified	:	2023.04.20 - 15:00
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#ifndef Dlg_TerminalMessage_Popup_h__
#define Dlg_TerminalMessage_Popup_h__

#pragma once

#include "resource.h"
#include "VGStatic.h"
#include "Def_Constant.h"
#include "Server.h"
#include "LogOutput.h"

//=============================================================================
// CDlg_TerminalMessage_Popup 대화 상자입니다.
//=============================================================================
class CDlg_TerminalMessage_Popup : public CDialogEx
{
	DECLARE_DYNAMIC(CDlg_TerminalMessage_Popup)

public:
	CDlg_TerminalMessage_Popup(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlg_TerminalMessage_Popup();

	afx_msg int		OnCreate				(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnSize					(UINT nType, int cx, int cy);

	enum { IDD = IDD_DLG_POPUP };

protected:
	virtual void	DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL	OnInitDialog();
	virtual void	OnOK();
	virtual void	OnCancel();
	afx_msg void	OnClose();

	DECLARE_MESSAGE_MAP()

	CFont			m_font_Default;

	CVGStatic		m_st_Title;
	CVGStatic		m_st_TerminalMessage;

	// 설비 제어 명령어 버튼
	CButton			m_bn_OK;	

	CString			m_szSvrID;						// 설비 id
	uint8_t			m_nSvrOrder				= 0;	// 라인에서의 설비 순서
	CString			m_TerminalMessage;
	CServer *		m_Svr;
public:
	// 타이틀 명 설정
	void			Set_TerminalMessage		(__in LPCTSTR IN_DATA);
	
	void			Set_ServerID			(__in LPCTSTR IN_DATA, __in uint8_t IN_nDATA);

	void			Set_Server				(__in CServer * IN_Svr);
	//2023.05.02a No Use
	//void			Refresh_UI				();
	//void			OnAdd_Log				(LPCTSTR lpszLog, COLORREF clrTextColor = RGB(0, 0, 0));
	//void			Set_TerminalLog			(__in std::vector<ST_AlarmStatus> IN_Svr);
	//2023.05.01a uhkim
	//std::vector<ST_TerminalLog>		m_nTerminalLog;
	//CLogOutput						m_ed_Log;
};

#endif // Dlg_TerminalMessage_Popup_h__
