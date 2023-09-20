//*****************************************************************************
// Filename	: 	Dlg_ModifyLoss.h
// Created	:	
// Modified	:	2023.04.20
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#ifndef Dlg_ModifyLoss_h__
#define Dlg_ModifyLoss_h__

#pragma once

#include "Wnd_Cfg_Loss.h"

// CDlg_ModifySV 대화 상자

class CDlg_ModifyLoss : public CDialogEx
{
	DECLARE_DYNAMIC(CDlg_ModifyLoss)

public:
	CDlg_ModifyLoss(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlg_ModifyLoss();

	enum {	IDD = IDD_DLG_MODIFY_STEP	};
	enum enCtrlText
	{
		Txt_DialogCaption,
		Txt_OK,
		Txt_Cancel,

		Txt_MaxCount,
	};

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	
#endif

protected:
	virtual void	DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	afx_msg int		OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnSize(UINT nType, int cx, int cy);
	virtual BOOL	OnInitDialog();
	virtual BOOL	PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()

	CFont			m_font_Default;
	CFont			m_Font;

	CButton			m_bn_Cancel;
	CButton			m_bn_OK;

	CVGStatic		m_st_ItemData[CWnd_Cfg_Loss::EID_MaxEnum];

	CEdit			m_ed_NameLossTem;
	CEdit			m_ed_Tem;
	CEdit			m_ed_CodeLoss;
	CEdit			m_ed_NameLossTemEng;
	CEdit			m_ed_Explan;

	uint8_t			m_nLanguage = 1;
public:
	BOOL			Get_LossData_UI(__out CConfig_Loss& OUT_stData);
	BOOL			Set_LossData_UI(__in CConfig_Loss* IN_pstData);
protected:
	void			Verify_Equipment(__inout CConfig_Loss& OUT_stDta);
public:
	void			OnLanguage		(__in uint8_t IN_nLanguage);


	CConfig_Loss	m_stLoss;
	void			Set_LossData		(__in const CConfig_Loss* IN_pstData);
	CConfig_Loss&	Get_LossData		();
		
	afx_msg void OnBnClickedOk();
};

#endif // Dlg_ModifyLoss_h__