//*****************************************************************************
// Filename	: Dlg_TransferProgress.h
// Created	: 2011/1/6
// Modified	: 2011/1/6 - 10:36
//
// Author	: PiRing
//	
// Purpose	: 
//*****************************************************************************
#ifndef Dlg_TransferProgress_h__
#define Dlg_TransferProgress_h__

#pragma once

#include "resource.h"

//=============================================================================
// CDlg_TransferProgress 대화 상자입니다.
//=============================================================================
class CDlg_TransferProgress : public CDialogEx
{
	DECLARE_DYNAMIC(CDlg_TransferProgress)

public:
	CDlg_TransferProgress(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlg_TransferProgress();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_MODULE_PROGRESS };

protected:
	virtual void	DoDataExchange		(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

	virtual BOOL	OnInitDialog		();
	virtual BOOL	PreTranslateMessage	(MSG* pMsg);
	afx_msg HBRUSH	OnCtlColor			(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	CFont			m_font_Default;

	CStatic			m_st_State;	
	CListBox		m_list_Msg;

	CStringList		m_strMessage;

public:
	void			SetInit				();
	void			SetStatus			(LPCTSTR lpszStatus);
	void			AddMessage			(LPCTSTR lpszMsg);
	void			AddMessageList		(CStringList& strMessage);
};

#endif // Dlg_TransferProgress_h__
