//*****************************************************************************
// Filename	: 	Dlg_OpCall_Popup.h
// Created	:	
// Modified	:	2023.04.20 - 15:00
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#ifndef Dlg_OpCall_Popup_h__
#define Dlg_OpCall_Popup_h__

#pragma once

#include "resource.h"
#include "VGStatic.h"
#include "Def_Constant.h"
#include "Server.h"
//=============================================================================
// Dlg_OpCall_Popup 대화 상자입니다.
//=============================================================================
class CDlg_OpCall_Popup : public CDialogEx
{
	DECLARE_DYNAMIC(CDlg_OpCall_Popup)

public:
	CDlg_OpCall_Popup(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlg_OpCall_Popup();

	afx_msg int		OnCreate				(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnSize					(UINT nType, int cx, int cy);
	afx_msg void	OnWindowPosChanging		(WINDOWPOS* lpwndpos);
	afx_msg void	OnTimer					(UINT_PTR nIDEvent);
// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_POPUP };

protected:
	virtual void	DoDataExchange	(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL	OnInitDialog	();
	virtual void	OnOK			();

	DECLARE_MESSAGE_MAP()

	CFont			m_font_Default;

	CVGStatic		m_st_Title;
	CVGStatic		m_st_IP_Adress;
	CVGStatic		m_st_SubEqpID;
	CVGStatic		m_st_Message;
	CVGStatic		m_st_BuzzerName;
	CVGStatic		m_st_Buzzer;
	CVGStatic		m_st_TowerLampName;
	CVGStatic		m_st_TowerLamp;

	//제어 버튼.
	CButton			m_bn_OK;

	CString			m_szTitle;						// 타이틀 명
	CString			m_transactionId;				// ID
	CString			m_szIP_Adress;
	CString			m_szSubEqpID;					// ID
	uint8_t			m_nSvrOrder = 0;				// 라인에서의 설비 순서
	CString			m_szMessage;
	CString			m_szBuzzer;
	CString			m_szTowerLamp;
	CServer			*m_Svr;

	
public:

	void			Set_Title				(__in LPCTSTR IN_DATA);
	void			Set_IP_Adress			(__in LPCTSTR IN_DATA);
	void			Set_ServerID			(__in LPCTSTR IN_DATA, __in uint8_t IN_nDATA);
	void			Set_Message				(__in LPCTSTR IN_DATA);
	void			Set_Buzzer				(__in LPCTSTR IN_DATA);
	void			Set_TowerLamp			(__in LPCTSTR IN_DATA);
	void			Set_Server				(__in CServer * IN_DATA);
	void			Set_transactionId		(__in LPCTSTR IN_DATA);
};

#endif // Dlg_OpCall_Popup_h__
