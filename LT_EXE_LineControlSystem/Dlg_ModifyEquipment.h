//*****************************************************************************
// Filename	: 	Dlg_ModifyEquipment.h
// Created	:	2021/1/20 - 12:10
// Modified	:	2021/1/20 - 12:10
//
// Author	:	piring
//	
// Purpose	:	
//*****************************************************************************
#ifndef Dlg_ModifyEquipment_h__
#define Dlg_ModifyEquipment_h__

#pragma once

#include "resource.h"
#include "VGStatic.h"
#include "Def_ConfigLine.h"

//=============================================================================
// CDlg_ModifyEquipment 대화 상자입니다.
//=============================================================================
class CDlg_ModifyEquipment : public CDialogEx
{
	DECLARE_DYNAMIC(CDlg_ModifyEquipment)

public:
	CDlg_ModifyEquipment(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlg_ModifyEquipment();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_MODIFY_STEP };

	enum enEqpItemData
	{
		EID_Eqp_Alias = 0,
		EID_Eqp_Type,			// Equipmnet Type
		EID_Eqp_Id,				// Equipment id
		EID_IP_Address,			// ip address
		EID_TestZoneCount,		// Test Zone Count
		EID_BufferCount,		// Buffer Count
		EID_ConveyorCount,		// Conveyor Count
		EID_ReturnConveyorCount,// Return Conveyor Count

		EID_MaxEnum,
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
	afx_msg void	OnCbnSelendokEqpType	();

	DECLARE_MESSAGE_MAP()

	CFont			m_font_Default;
	CFont			m_Font;

	CButton			m_bn_OK;
	CButton			m_bn_Cancel;

	// 기본 데이터 ------------------------------------------------
	CVGStatic		m_st_EqpItemData[EID_MaxEnum];

	CEdit			m_ed_EqpAlias;
	CComboBox		m_cb_EquipmentType;
	CEdit			m_ed_EquipmentId;
	CMFCMaskedEdit	m_ed_IPAddress;

// 	CComboBox		m_cb_TestZoneCount;
// 	CComboBox		m_cb_BufferCount;
// 	CComboBox		m_cb_ConveyorCount;
// 	CComboBox		m_cb_ReturnConveyorCount;


	std::vector<CString>	m_EquipmentTypeList;

	CConfig_Eqp		m_stEquipment;

	uint8_t			m_nLanguage			= 1;

	// UI에 설정된 스텝 데이터 구하기
public:
	BOOL	Get_EquipmentData_UI		(__out CConfig_Eqp& OUT_stEquipment);
	BOOL	Set_EquipmentData_UI		(__in CConfig_Eqp* IN_pstEquipment);

protected:
	void	Verify_Equipment			(__inout CConfig_Eqp& OUT_stEquipment);

	// 스텝 항목 선택에 따른 파라미터 데이터 사용 및 명칭 업데이트
	void	UpdateUI_EquipmentData		(__in int IN_nIndex);
	// ComboBox의 스텝 항목 리스트 업데이트
	void	UpdateUI_EquipmentType		();

public:

	void			OnLanguage				(__in uint8_t IN_nLanguage);

	// 초기 상태로 설정 (New)
	void			Init_DefaultSet			();

	// 사용하는 스텝 항목 목록 추가
	void			Reset_EquipmentTypeList	();
	void			Set_EquipmenTypetList	(__in std::vector<CString>* IN_pEquipmentTypeList);

	void			Set_EquipmentData		(__in const CConfig_Eqp* IN_pstEuquipment);
	CConfig_Eqp&	Get_EquipmentData		();

};

#endif // Dlg_ModifyEquipment_h__
