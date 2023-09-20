//*****************************************************************************
// Filename	: 	Wnd_Cfg_NGCode.h
// Created	:	2021/10/21 - 15:15
// Modified	:	2021/10/21 - 15:15
//
// Author	:	piring
//	
// Purpose	:	
//*****************************************************************************
#ifndef Wnd_Cfg_NGCode_h__
#define Wnd_Cfg_NGCode_h__

#pragma once
#include "Wnd_BaseView.h"
#include "VGStatic.h"
#include "List_NGCode.h"

//-----------------------------------------------------------------------------
// CWnd_Cfg_NGCode
//-----------------------------------------------------------------------------
class CWnd_Cfg_NGCode : public CWnd_BaseView
{
	DECLARE_DYNAMIC(CWnd_Cfg_NGCode)

public:
	CWnd_Cfg_NGCode();
	virtual ~CWnd_Cfg_NGCode();

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
		Item_MES_NGCode,	// MES Rework NG Code
		Item_MaxCount,
	};

	enum enCtrlText
	{
		Txt_Add,
		Txt_Remove,
		Txt_Modify,
		Txt_Apply,

		Txt_MaxCount,
	};

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg int		OnCreate				(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnSize					(UINT nType, int cx, int cy);
	virtual BOOL	PreCreateWindow			(CREATESTRUCT& cs);
	virtual BOOL	PreTranslateMessage		(MSG* pMsg);

	afx_msg void	OnBnClicked_Add			();
	afx_msg void	OnBnClicked_Remove		();
	afx_msg void	OnBnClicked_Modify		();
	afx_msg void	OnBnClicked_Apply		();
	afx_msg void	OnBnClicked_SharePath	();


	CFont			m_font_Default;
	CFont			m_font_Data;

	CVGStatic		m_st_Item[Item_MaxCount];
	CMFCMaskedEdit	m_ed_Item[Item_MaxCount];

	// 저장 경로
	CVGStatic		m_st_Path_Save;
	CEdit			m_ed_Path_Save;

	// NG Code 리스트
	CList_NGCode	m_lc_NGCodeList;
	
	// NGCode 추가, 제거 (리스트 자동 정렬)
	CMFCButton		m_bn_Add;
	CMFCButton		m_bn_Remove;	// 삭제 확인 팝업 창 사용
	CMFCButton		m_bn_Modify;
	CMFCButton		m_bn_Apply;

	// 버튼 제어 함수
	void		Item_Insert				();
	void		Item_Remove				();
	void		Item_Modify				();

	// file 전체 경로
	CString		m_szSavePath;			// 저장 파일 전체 경로
	CString		m_szSharedPath;			// 설비쪽 공유 파일 전체 경로
	
	bool		Save_IniFile			();
	bool		Load_IniFile			();

	// UI에 세팅 된 데이터 -> 구조체
	void		Get_UIData				(__out ST_NGCode& OUT_stNGCode);
	// 구조체 -> UI에 세팅
	void		Set_UIData				(__in const ST_NGCode* IN_pNGCode);

public:

	virtual void	OnLanguage			(__in uint8_t IN_nLanguage);

	// 파일 저장 경로, 공유 파일 저장 경로
	void		Set_Path				(__in LPCTSTR IN_szSavePath);
	void		Set_PathShared			(__in LPCTSTR IN_szSharedPath);
	void		Set_Path				(__in LPCTSTR IN_szSavePath, __in LPCTSTR IN_szSharePath);
	
	// 권한 설정
	void		Set_PermissionMode		(__in enPermissionMode IN_PermissionMode);

	// UI에 표시된 데이터의 구조체 반환	
	void		Get_Configuration		(__out CNGCode_Info& OUT_stNGCodeList);

	// 데이터를 UI에 표시
	void		Set_Configuration		(__in const CNGCode_Info* IN_pNGCodeList);
	
};

#endif // Wnd_Cfg_NGCode_h__


