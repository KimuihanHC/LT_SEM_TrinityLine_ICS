//*****************************************************************************
// Filename	: 	Dlg_ModifyNGCode.h
// Created	:	2021/1/20 - 12:10
// Modified	:	2021/1/20 - 12:10
//
// Author	:	piring
//	
// Purpose	:	
//*****************************************************************************
#ifndef Dlg_ModifyNGCode_h__
#define Dlg_ModifyNGCode_h__

#pragma once

#include "resource.h"
#include "VGStatic.h"
#include "Def_NGCode.h"

//=============================================================================
// CDlg_ModifyNGCode 대화 상자입니다.
//=============================================================================
class CDlg_ModifyNGCode : public CDialogEx
{
	DECLARE_DYNAMIC(CDlg_ModifyNGCode)

public:
	CDlg_ModifyNGCode(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlg_ModifyNGCode();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_MODIFY_NGCODE };

	enum enItem
	{
		Item_NG_Code,		// Code (NGCode	)
		Item_NG_Section,	// Section (NG구분)	
		Item_Retry_Mode,	// Retry Mode	
		Item_RetryCnt,		// Retry Count	
		Item_Move_Retry,	// Move Retry	
		Item_Reset,			// Reset	
		Item_AlarmCnt,		// Alarm Count(연속NG알람수)
		Item_Description,	// Descript

		Item_MaxCount,
	};

	enum enCtrlText
	{
		Txt_DialogCaption,
		Txt_OK,
		Txt_Cancel,

		Txt_MaxCount,
	};

protected:
	virtual void	DoDataExchange			(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	afx_msg int		OnCreate				(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnSize					(UINT nType, int cx, int cy);
	virtual BOOL	PreTranslateMessage		(MSG* pMsg);
	virtual BOOL	OnInitDialog			();

	virtual void	OnCancel				();
	virtual void	OnOK					();

	DECLARE_MESSAGE_MAP()

	CFont			m_font_Default;
	CFont			m_font_Data;

	CButton			m_bn_OK;
	CButton			m_bn_Cancel;

	CVGStatic			m_st_Item[Item_MaxCount];
	CMFCMaskedEdit		m_ed_Item[Item_MaxCount];

	// 기본 데이터 ------------------------------------------------
	ST_NGCode		m_stNGCode;

	uint8_t			m_nLanguage			= 1;
	
	// UI에 세팅 된 데이터 -> 구조체
	bool			Get_UIData			(__out ST_NGCode& OUT_stNGCode);
	void			Set_UIData			(__in const ST_NGCode* IN_pstIN_pNGCode);

public:

	void			OnLanguage			(__in uint8_t IN_nLanguage);

	void			Set_NGCodeData		(__in const ST_NGCode* IN_pNGCode);
	ST_NGCode&		Get_NGCodeData		();

};

#endif // Dlg_ModifyNGCode_h__
