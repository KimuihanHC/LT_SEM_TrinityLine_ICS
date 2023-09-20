//*****************************************************************************
// Filename	: 	Dlg_ModifyEES.h
// Created	:	
// Modified	:	2023.04.20
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#ifndef Dlg_ModifyEES_h__
#define Dlg_ModifyEES_h__

#pragma once

#include "Wnd_Cfg_EES.h"

// CDlg_ModifySV 대화 상자

class CDlg_ModifyEES : public CDialogEx
{
	DECLARE_DYNAMIC(CDlg_ModifyEES)

public:
	CDlg_ModifyEES(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlg_ModifyEES();

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

	CVGStatic		m_st_ItemData[CWnd_Cfg_EES::EID_MaxEnum];

	CEdit			m_ed_EquipmentState;
	CEdit			m_ed_EquipmentStateNameKor;
	CEdit			m_ed_EquipmentStateNameEng;
	CEdit			m_ed_LossCode;
	CEdit			m_ed_StateColor;
	CComboBox		m_cb_Use;
	CEdit			m_ed_Explanation;

	uint8_t			m_nLanguage = 1;
public:
	BOOL			Get_EESData_UI(__out CConfig_EES& OUT_stData);
	BOOL			Set_EESData_UI(__in CConfig_EES* IN_pstData);
protected:
	void			Verify_Equipment(__inout CConfig_EES& OUT_stDta);
public:
	void			OnLanguage		(__in uint8_t IN_nLanguage);


	CConfig_EES		m_stEES;
	void			Set_EESData		(__in const CConfig_EES* IN_pstData);
	CConfig_EES&	Get_EESData		();
		
	afx_msg void OnBnClickedOk();
};

#endif // Dlg_ModifyEES_h__