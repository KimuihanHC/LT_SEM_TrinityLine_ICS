﻿//*****************************************************************************
// Filename	: 	Dlg_PopupMenu.h
// Created	:	2021/12/27 - 16:22
// Modified	:	2021/12/27 - 16:22
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#ifndef Dlg_EquipMentState_PopupMenu_h__
#define Dlg_EquipMentState_PopupMenu_h__

#pragma once

#include "resource.h"
#include "VGStatic.h"
//#include "Def_EquipMentState_PopupMenu.h"
#include "Def_Constant.h"

//=============================================================================
// CDlg_PopupMenu 대화 상자입니다.
//=============================================================================
class CDlg_EquipMentState_PopupMenu : public CDialogEx
{
	DECLARE_DYNAMIC(CDlg_EquipMentState_PopupMenu)

public:
	CDlg_EquipMentState_PopupMenu(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlg_EquipMentState_PopupMenu();

	afx_msg int		OnCreate				(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnSize					(UINT nType, int cx, int cy);
	afx_msg void	OnWindowPosChanging		(WINDOWPOS* lpwndpos);
	afx_msg void	OnBnClicked_EquipMentState(UINT nID);


// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_POPUP };

protected:
	virtual void	DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL	OnInitDialog();

	DECLARE_MESSAGE_MAP()

	CFont			m_font_Default;

	// 외곽 보더라인
	CVGStatic		m_st_frame;

	// 설비 제어 명령어 버튼
	CMFCButton		m_bn_CtrlCmd[enEquipmentState::ES_MaxCount];
	
 	bool			m_bActivted	= false;			// 다이얼로그 이동 방지를 위한 플래그
	
	CPoint			m_nInitPos;						// 팝업 초기 위치
	CString			m_szTitle;						// 타이틀 명
	CString			m_szSvrID;						// 설비 id
	uint8_t			m_nSvrOrder				= 0;	// 라인에서의 설비 순서
	bool			m_bLoader				= false;

public:

	// 다이얼로그 초기 표시 위치 설정
	void			Set_InitPos				(__in int x, __in int y);
	// 타이틀 명 설정
	void			Set_Title				(__in LPCTSTR IN_szTitle);
	// 설비 id와 설비 순서 설정
	void			Set_ServerID			(__in LPCTSTR IN_szSvrID, __in uint8_t IN_nEqpOrder);
	// 검사 설비 구분
	void			Set_Loader				(__in bool IN_bLoader);
};

#endif // Dlg_PopupMenu_h__
