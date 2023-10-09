//*****************************************************************************
// Filename	: 	Wnd_TesterSelection.h
// Created	:	2021/1/26 - 14:20
// Modified	:	2021/1/26 - 14:20
//
// Author	:	piring
//	
// Purpose	:	
//*****************************************************************************
#ifndef Wnd_TesterSelection_h__
#define Wnd_TesterSelection_h__

#pragma once
#include <afxwin.h>
#include "Wnd_BaseView.h"
#include "Def_ModelConfig.h"
#include "Def_ConfigLine.h"
#include "VGStatic.h"

#if defined(EES_XML)//20231003
#include "Def_ConfigLineServer.h"
#endif

//-----------------------------------------------------------------------------
// CWnd_TesterSelection
//-----------------------------------------------------------------------------
class CWnd_TesterSelection : public CWnd_BaseView
{
	DECLARE_DYNAMIC(CWnd_TesterSelection)

public:
	CWnd_TesterSelection();
	virtual ~CWnd_TesterSelection();

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg int		OnCreate			(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnSize				(UINT nType, int cx, int cy);
	virtual BOOL	PreCreateWindow		(CREATESTRUCT& cs);
	virtual BOOL	PreTranslateMessage	(MSG* pMsg);

	afx_msg void	OnCbnSelendokSocketType	();
	afx_msg void	OnCbnSelendokModelType	();
	afx_msg void	OnBnClickedBnApply		();

private:
	CFont			m_font;
	CFont			m_font_Data;

	
	CVGStatic		m_st_Title;

	CVGStatic		m_st_ModelType;
	CComboBox		m_cb_ModelType;

	CVGStatic		m_st_SocketType;
	CComboBox		m_cb_SocketType;
	CMFCButton		m_chk_TesterType[Max_TesterCount];

	CMFCButton		m_bn_Apply;


	enPermissionMode m_PermissionMode	= Permission_Operator;

	// 설정 클래스
	CConfig_Model*	m_pConfigModel		= nullptr;

	// 설정된 검사 순서 => <Equipment Type> : Loader, Returner 제외한 검사의 순서
	std::vector<uint8_t>	m_TestOrder;
	int		m_iTest_Height				= 50;
	int		m_iTest_Width				= 100;
	int		m_nTop_TestButton			= 0;
	void	Rebatch_TestButton			();

	// 설정 데이터를 UI에 표시
	void	SetUI_Configuration			(__in const CConfig_Model* IN_pModelConfig);

	void	Update_SocketType			(__in bool IN_bUpdateModel);
	void	Update_ModelType			(__in uint8_t IN_nSocketType);

public:

	virtual void	OnLanguage			(__in uint8_t IN_nLanguage);

	void	Set_Config_Line				(__in const CConfig_Line* IN_pConfig_Line);

	// 모델 데이터 설정
	void	Set_Configuration			(__in CConfig_Model& IN_ModelConfig);	

	// 검사 진행 여부 조건 얻기
	void	Get_Condition				(__out bool* OUT_pbCondition, __out uint8_t& OUT_nDataCount);

	// 권한 설정
	void	Set_PermissionMode			(__in enPermissionMode IN_PermissionMode);

#if defined(EES_XML)//20231003
	void	Set_Config_Server			(__in const CConfig_Server * IN_pConfig_Server);
#endif
};

#endif // Wnd_TesterSelection_h__
