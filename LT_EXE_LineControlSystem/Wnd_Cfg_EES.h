//*****************************************************************************
// Filename	: 	Wnd_Cfg_EES.h
// Created	:	
// Modified	:	2023.04.21 - 12:00
//
// Author	:	piring
//	
// Purpose	:	
//*****************************************************************************
#ifndef Wnd_Cfg_EES_h__
#define Wnd_Cfg_EES_h__

#pragma once
#include "Wnd_BaseView.h"
#include "VGStatic.h"
#include "List_EESConfig.h"

//-----------------------------------------------------------------------------
// CWnd_Cfg_Line
//-----------------------------------------------------------------------------
class CWnd_Cfg_EES : public CWnd_BaseView
{
	DECLARE_DYNAMIC(CWnd_Cfg_EES)

public:
	CWnd_Cfg_EES();
	virtual ~CWnd_Cfg_EES();
	enum enIDCtrl
	{
		ID_Add,
		ID_Insert,
		ID_Remove,
		ID_Modify,	// 선택한 항목 수정
		ID_Order_Up,
		ID_Order_Down,
		ID_MaxEnum,
	};
	enum enIDItemData
	{
		EID_EquipmentState,
		EID_EquipmentStateNameKor,
		EID_EquipmentStateNameEng,
		EID_LossCode,
		EID_StateColor,
		EID_USE,		
		EID_Explanation,
		EID_MaxEnum,
	};
	enum enXmlCtrl
	{
		XML_Load,
		XML_Save,
		XML_Clear,
		XML_MaxCount,
	};
protected:
	DECLARE_MESSAGE_MAP()

	afx_msg int		OnCreate					(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnSize						(UINT nType, int cx, int cy);
	//void			MoveWindow_EqpCtrl			(int x, int y, int nWidth, int nHeight);
	//void			MoveWindow_Item				(int x, int y, int nWidth, int nHeight);

	afx_msg void	OnBnClickedBnStepCtrl		(UINT nID);
	afx_msg void	OnBnClickedBnListCtrl		(UINT nID);
	afx_msg void	OnBnClickedBnClearList		();
	afx_msg void	OnBnClickedBnVerifyLine		();
	afx_msg void	OnBnClickedBnApply			();

	CFont			m_font_Default;
	CFont			m_Font;

	//2023.02.13 추가 LineInfo
	CList_EESConfig	m_lc_Info;

	// 선택된 라인 설정
	CMFCButton		m_bn_Apply;

	// 설비 목록
	CMFCButton		m_bn_VerifyConfig;

	// 설비 항목 추가/삭제/이동
	CMFCButton		m_bn_ItemCtrl[ID_MaxEnum];

	// 검사기 항목 ---------------------------------------------------
	CVGStatic		m_st_ItemData[EID_MaxEnum];

	CEdit			m_ed_EquipmentState;
	CEdit			m_ed_EquipmentStateNameKor;
	CEdit			m_ed_EquipmentStateNameEng;
	CEdit			m_ed_LossCode;
	CEdit			m_ed_StateColor;
	CComboBox		m_cb_Use;
	CEdit			m_ed_Explanation;
	   
	CMFCButton		m_bn_XmlCtrl[XML_MaxCount];
		
	CString			m_szPath_XML;		// XML 파일 패스
	CString			m_szPath_Default;	// 기본 저장 파일 전체 경로

	// 기타 데이터 사용여부
	bool			m_bUseExtraData		= false;

	// UI에 설정된 설비 구성 데이터 구하기
	BOOL			Get_EESData					(__out	CConfig_EES& OUT_stSV);
	void			Verify_EES					(__inout CConfig_EES& OUT_stSV);

	// 버튼 제어 함수
	void			Item_Add					();
	void			Item_Insert					();
	void			Item_Remove					();
	void			Item_Modify					();
	void			Item_Up						();
	void			Item_Down					();

	// XML 불러오기 / 저장하기
	BOOL			LoadXML_EESInfo				();
	BOOL			LoadXML_EESInfo				(__out CConfig_EES_LIST& OUT_stInfo, __out CString& OUT_FilePath, __out CString& OUT_FileTitle);
	BOOL			LoadXML_EESInfo				(__in LPCTSTR IN_szFilePath, __out CConfig_EES_LIST& OUT_stInfo);

	BOOL			SaveXML_EESInfo				();
	void			Apply_EESInfo				();

	// 루프 begin -> end 구조가 정상인지 판단
	bool			Verify_Sequence				(__in const CConfig_EES_LIST* IN_pInfo);

public:

	virtual void	OnLanguage					(__in uint8_t IN_nLanguage);

	// 권한 설정
	void			Set_PermissionMode			(__in enPermissionMode IN_PermissionMode);

	// 기본 저장 파일 전체 경로
	void			Set_FileFullPath			(__in LPCTSTR IN_szFullPath);

	// 설비 List 저장 파일 경로
	void			Set_XMLFilePath				(__in LPCTSTR szPath_XML);

	// 기타 데이터 사용 여부
	void			Set_UseExtraData			(__in bool bUse);

	// 저장된 라인 구성 정보 데이터 불러오기
	void			Set_Configuration			(__in const CConfig_EES_LIST* IN_SVInfo);
	void			Set_Configuration			();
	void			Get_Configuration			(__out CConfig_EES_LIST& OUT_stLineInfo);

	// 초기 상태로 설정 (New)
	void			Init_DefaultSet				();
};

#endif // Wnd_Cfg_EES_h__


