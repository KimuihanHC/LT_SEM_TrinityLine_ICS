//*****************************************************************************
// Filename	: 	Wnd_Cfg_Debug.h
// Created	:	2021/10/21 - 15:15
// Modified	:	2021/10/21 - 15:15
//
// Author	:	piring
//	
// Purpose	:	
//*****************************************************************************
#ifndef Wnd_Cfg_Debug_h__
#define Wnd_Cfg_Debug_h__

#pragma once
#include "Wnd_BaseView.h"
#include "VGStatic.h"
#include "Def_DebugInfo.h"


//-----------------------------------------------------------------------------
// CWnd_Cfg_Debug
//-----------------------------------------------------------------------------
class CWnd_Cfg_Debug : public CWnd_BaseView
{
	DECLARE_DYNAMIC(CWnd_Cfg_Debug)

public:
	CWnd_Cfg_Debug();
	virtual ~CWnd_Cfg_Debug();

	enum enItem
	{
		Item_UseDebugMode,
		Item_NotBypass_NG,
		Item_NotBypass_TestSkip,
		Item_NotBypass_Empty,
		Item_AutoReset_Yield_byShift,
		Item_SocketTrackOut_Condition,
		Item_WaitingSocket_Condition,
		Item_MultiTester_SelectMethod,

		Item_MaxCount,
	};

	enum enCtrlText
	{
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

	afx_msg void	OnBnClicked_Apply		();
	afx_msg void	OnCbnSelEndOk_DebugMode	();

	CFont			m_font_Default;
	CFont			m_font_Data;

	CVGStatic		m_st_Item[Item_MaxCount];
	CComboBox		m_cb_Item[Item_MaxCount];
	
	CMFCButton		m_bn_Apply;

	// UI에 세팅 된 데이터 -> 구조체
	void		Get_UIData			(__out ST_DebugInfo& stDebugInfo);
	// 구조체 -> UI에 세팅
	void		Set_UIData			(__in const ST_DebugInfo* pDebugInfo);

	void		EnableControl		(__in BOOL bEnable);

public:

	virtual void	OnLanguage			(__in uint8_t IN_nLanguage);

	// 권한 설정
	void		Set_PermissionMode		(__in enPermissionMode IN_PermissionMode);

	// 모델 데이터를 UI에 표시
	void		Set_Configuration		(__in const ST_DebugInfo* pDebugInfo);
	// UI에 표시된 데이터의 구조체 반환	
	void		Get_Configuration		(__out ST_DebugInfo& stDebugInfo);


};

#endif // Wnd_Cfg_Debug_h__


