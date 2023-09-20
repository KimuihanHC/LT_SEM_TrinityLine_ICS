#ifndef Dlg_ModifyAL_h__
#define Dlg_ModifyAL_h__

#pragma once

#include "Wnd_Cfg_ALID_List.h"

// CDlg_ModifySV 대화 상자

class CDlg_ModifyAL : public CDialogEx
{
	DECLARE_DYNAMIC(CDlg_ModifyAL)

public:
	CDlg_ModifyAL(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlg_ModifyAL();


	enum enALItemData
	{
		EID_ALID,		// Equipmnet Type
		EID_ALCD,
		EID_ALTX,		// Equipment id			// Buffer Count
		EID_MaxEnum,
	};

	enum enCtrlText
	{
		Txt_DialogCaption,
		Txt_OK,
		Txt_Cancel,

		Txt_MaxCount,
	};

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_MODIFY_AL};
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

	CButton m_bn_Cancel;
	CButton m_bn_OK;

	CVGStatic m_st_SVItemData[CWnd_Cfg_ALID_List::EID_MaxEnum];
	
	CEdit m_ed_nALID;
	CComboBox m_cb_nALCD;
	CEdit m_ed_cstALTX;
	

	uint8_t			m_nLanguage = 1;

public:
	void OnLanguage		(__in uint8_t IN_nLanguage);
	BOOL Get_ALData_UI	(__out CConfig_AL& OUT_stSV);
	BOOL Set_ALData_UI	(__in CConfig_AL* IN_pstSV);

	CConfig_AL		m_stSV;
	void Set_ALData(__in const CConfig_AL* IN_pstSV);
	CConfig_AL& Get_SVData();
		
	afx_msg void OnBnClickedOk();
};

#endif // DialogMessage_h__