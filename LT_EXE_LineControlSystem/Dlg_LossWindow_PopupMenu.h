//*****************************************************************************
// Filename	: 	Dlg_LossWindow_PopupMenu.h
// Created	:	
// Modified	:	2023.04.20 - 15:00
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#ifndef Dlg_LossWindow_PopupMenu_h__
#define Dlg_LossWindow_PopupMenu_h__

#pragma once

#include "resource.h"
#include "VGStatic.h"
#include "Def_Constant.h"
#include "XmlLossConfig.h"
#include "Server.h"
//=============================================================================
// CDlg_LossWindow_PopupMenu 대화 상자입니다.
//=============================================================================
class CDlg_LossWindow_PopupMenu : public CDialogEx
{
	DECLARE_DYNAMIC(CDlg_LossWindow_PopupMenu)

public:
	CDlg_LossWindow_PopupMenu(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlg_LossWindow_PopupMenu();

	afx_msg int		OnCreate				(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnSize					(UINT nType, int cx, int cy);
	afx_msg void	OnWindowPosChanging		(WINDOWPOS* lpwndpos);
	afx_msg void	OnBnClicked_LossWindow	(UINT nID);
	afx_msg void	OnClose();
// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_POPUP };

protected:
	virtual void	DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL	OnInitDialog();
	virtual void	OnCancel();
	virtual void	OnOK();
	DECLARE_MESSAGE_MAP()

	CFont			m_font_Default;

	// 외곽 보더라인
	CVGStatic		m_st_frame;

	// 설비 제어 명령어 버튼
	CMFCButton		m_bn_CtrlCmd[100];
	CMFCButton		m_bn_OK;

	long			nCount[100];
	CPoint			m_nInitPos;						// 팝업 초기 위치
	CString			m_szTitle;						// 타이틀 명
	CString			m_szEqpID;						// 설비 id
	uint8_t			m_nEqpOrder				= 0;	// 라인에서의 설비 순서
	CServer *		m_Svr;

	CXmlLossConfig		StepSeq;
	CConfig_Loss_LIST	Info;
	UINT				nBnIndex = 0;

public:

	// 다이얼로그 초기 표시 위치 설정
	void			Set_InitPos				(__in int x, __in int y);
	// 타이틀 명 설정
	void			Set_Title				(__in LPCTSTR IN_szTitle);
	
	void			Set_EquipmentID			(__in LPCTSTR IN_szEqpID, __in uint8_t IN_nEqpOrder);

	void			Set_Server				(__in CServer * IN_Svr);

};

#endif // Dlg_LossWindow_PopupMenu_h__
