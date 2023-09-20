//*****************************************************************************
// Filename	: 	Wnd_SettingView.h
// Created	:	2021/11/4 - 17:55
// Modified	:	2021/11/4 - 17:55
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#ifndef Wnd_SettingView_h__
#define Wnd_SettingView_h__

#pragma once

#include "Wnd_BaseView.h"
#include "VGStatic.h"

#include "Wnd_Cfg_Line.h"
#include "Wnd_Cfg_Socket.h"
#include "Wnd_Cfg_User.h"
#include "Wnd_Cfg_NGCode.h"
//#include "Wnd_Cfg_Alarm.h"
#include "Wnd_Cfg_Debug.h"

//=============================================================================
// CWnd_SettingView
//=============================================================================
class CWnd_SettingView : public CWnd_BaseView
{
	DECLARE_DYNAMIC(CWnd_SettingView)

public:
	CWnd_SettingView();
	virtual ~CWnd_SettingView();

	enum enTab
	{
		Tab_Line = 0,
		Tab_Socket,
		Tab_User,
		Tab_NGCode,
		//Tab_Alarm,
#ifdef USE_DEBUG_MODE
		Tab_DebugMode,
#endif
		Tab_MaxCount,
	};

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg int		OnCreate					(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnSize						(UINT nType, int cx, int cy);
	afx_msg void	OnShowWindow				(BOOL bShow, UINT nStatus);
	virtual BOOL	PreCreateWindow				(CREATESTRUCT& cs);

	void			MoveWindow_Tab				(int x, int y, int nWidth, int nHeight);

	afx_msg void	OnTabSelect					(UINT nID);
	afx_msg void	OnBnClicked_Keyboard		();

	CFont			m_font_Default;
	CFont			m_font_Data;

	

	int						m_nTabWidth		= 140;
	CMFCButton				m_rb_Tab[Tab_MaxCount];
	CVGStatic				m_frame_Tab;

	std::vector<CWnd*>		m_pwndTab;

	// Tab Windows
	CWnd_Cfg_Line			m_wnd_LineCfg;
	CWnd_Cfg_Socket			m_wnd_Socket;
	CWnd_Cfg_User			m_wnd_User;
	CWnd_Cfg_NGCode			m_wnd_NGCode;
	//CWnd_Cfg_Alarm			m_wnd_Alarm;
	CWnd_Cfg_Debug			m_wnd_Debug;
	
	CMFCButton				m_bn_Keyboard;

	CString			m_szLineInfoPath;
	CString			m_szSocketPath;

	void		Set_TabWindow				(__in uint8_t IN_nTabIndex);

public:

	virtual void	OnLanguage				(__in uint8_t IN_nLanguage);

	// 권한 설정
	void		Set_PermissionMode			(__in enPermissionMode IN_PermissionMode);

	void		SetPath_LineInfo			(__in LPCTSTR szLineInfoPath);
	void		SetPath_Socket				(__in LPCTSTR szSocketPath);
	void		SetPath_Shared				(__in LPCTSTR szSharedPath);
	
	void		UpdateUI_LineInfo			(__in const CConfig_Line* IN_pConfigLine);

	void		UpdateUI_SocketInfo			();
	void		UpdateUI_UserInfo			();
	void		UpdateUI_AlarmInfo			();
	void		UpdateUI_NGCodeInfo			();


	void		Init_RecipeView				();

};

#endif // Wnd_SettingView_h__

