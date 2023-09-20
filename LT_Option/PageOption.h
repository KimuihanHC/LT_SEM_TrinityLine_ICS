//*****************************************************************************
// Filename	: PageOption.h
// Created	: 2010/8/27
// Modified	: 2010/8/27 - 10:21
//
// Author	: PiRing
//	
// Purpose	: 옵션 - 기본 페이지
//*****************************************************************************
#pragma once

#include "LT_Option.h"
#include "Define_Option.h"
#include <memory>
#include "Def_Enum_Cm.h"

//=============================================================================
// CPageOption
//=============================================================================
class CPageOption : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CPageOption)

public:
	CPageOption( LPCTSTR lpszCaption = NULL );	
	CPageOption(UINT nIDTemplate, UINT nIDCaption = 0);

	virtual ~CPageOption();

protected:

	afx_msg int		OnCreate			(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnSize				(UINT nType, int cx, int cy);
	afx_msg void	OnSetFocus			(CWnd* pOldWnd);
	afx_msg void	OnSettingChange		(UINT uFlags, LPCTSTR lpszSection);

	afx_msg LRESULT OnPropertyChanged	(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

	CMFCPropertyGridCtrl	m_wndPropList;
	CFont					m_fntPropList;

	CLT_Option*				m_pLT_Option;

	virtual BOOL	OnInitDialog		();

	virtual void	AdjustLayout		();
	virtual void	SetPropListFont		();
	virtual void	InitPropList		();

	void			InitPropList_Comport(std::auto_ptr<CMFCPropertyGridProperty> &apGroup_ComPort);
	void			Get_Option_ComPort	(CMFCPropertyGridProperty* pPropertyGroup, stOpt_Serial &stOption);
	void			Set_Option_ComPort	(CMFCPropertyGridProperty* pPropertyGroup, stOpt_Serial stOption);

	// 다이얼로그 템플릿 처리용
	DLGTEMPLATE		m_dlgTemplate;
	HLOCAL			hLocal;

	CStringW		m_strCaption;
	CStringW		m_strFontName;
	UINT			m_nFontSize;

	void			SetDlgTemplate		();
	void			InitTemplate		();

	enInsptrSysType		m_InsptrType;
public:

	virtual void	SaveOption			();
	virtual void	LoadOption			();

	void			SetLT_Option		(__in CLT_Option* pOption)
	{
		m_pLT_Option = pOption;
	}

	// 검사기 구분
	void			SetInspectorType	(__in enInsptrSysType nInsptrType);
};


