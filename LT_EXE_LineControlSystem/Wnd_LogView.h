//*****************************************************************************
// Filename	: Wnd_LogView.h
// Created	: 2010/12/2
// Modified	: 2015/12/22
//
// Author	: PiRing
//	
// Purpose	: 
//*****************************************************************************
#ifndef Wnd_LogView_h__
#define Wnd_LogView_h__

#pragma once

#include "Wnd_BaseView.h"
#include "VGStatic.h"
#include "LogOutput.h"
#include <vector>
#include "Wnd_InOutCount.h"

//=============================================================================
// CWnd_LogView
//=============================================================================
class CWnd_LogView : public CWnd_BaseView
{
	DECLARE_DYNAMIC(CWnd_LogView)

public:
	CWnd_LogView();
	virtual ~CWnd_LogView();

	enum enTab
	{
		Tab_Alarm = 0,
		Tab_Load,
		Tab_Unload,
#ifdef USE_IN_OUT_COUNT_UI
		Tab_InOut_Count,
#endif
		//2023.05.01a uhkim
		Tab_TerminalMessage,
		Tab_Opcall,

		Tab_MaxCount,
	};

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg int		OnCreate			(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnSize				(UINT nType, int cx, int cy);
	virtual BOOL	PreCreateWindow		(CREATESTRUCT& cs);
	
	void			MoveWindow_Tab				(int x, int y, int nWidth, int nHeight);

	afx_msg void	OnTabSelect					(UINT nID);
	afx_msg void	OnBnClicked_Keyboard		();

	CFont			m_font_Default;
	CFont			m_font_Data;

	

	int						m_nTabWidth		= 140;
	CMFCButton				m_rb_Tab[Tab_MaxCount];
	CVGStatic				m_frame_Tab;
	std::vector<CWnd*>		m_pwndTab;


 	CStatic			m_st_Log_All;
 	CStatic			m_st_Log_Error;
 	CLogOutput		m_ed_Log_All;
 	CLogOutput		m_ed_Log_Error;

	CStatic			m_st_Log_Tab;
	CLogOutput		m_ed_Log_Alarm;
	CLogOutput		m_ed_Log_Load;
	CLogOutput		m_ed_Log_Unload;
	//CLogOutput		m_ed_Log_NGInfo;

	// 투입 / 배출 / 진행 / 소켓제거 카운트
	CWnd_InOutCount	m_wnd_InOutCount;

	//2023.05.02a uhkim
	CLogOutput		m_ed_Log_TerminalMessage;
	CLogOutput		m_ed_Log_OpCall;
	//

	CMFCButton				m_bn_Keyboard;

	void	Set_TabWindow		(__in uint8_t IN_nTabIndex);

public:
	
	virtual void	OnLanguage	(__in uint8_t IN_nLanguage);

	// 권한 설정
	void	Set_PermissionMode(__in enPermissionMode IN_PermissionMode);

	void	OnAdd_Log			(LPCTSTR lpszLog, BOOL bError /*= FALSE*/, UINT nLogType /*= 0*/, COLORREF clrTextColor /*= RGB(0, 0, 0)*/);
	void	OnAdd_Log			(LPCTSTR lpszLog, UINT nLogType);

	void	Refresh_UI			();

	// Error
	// Equipment Alarm
	// Socket Load
	// Socket Unload 

	void	Log_EqpAlarm		(__in LPCTSTR lpszLog, __in bool bError = false);
	void	Log_LoadSocket		(__in LPCTSTR lpszLog, __in bool bError = false);
	void	Log_UnloadSocket	(__in LPCTSTR lpszLog, __in bool bError = false);
	void	Log_NGInfo			(__in LPCTSTR lpszLog);

	// 투입/배출 수량 카운트
	void	Set_InOutCount(__in const ST_InOutCount* IN_pCount);

	//2023.05.01a
	void	Log_TerminalMessage	(__in LPCTSTR lpszLog);
	void	Log_OpCall			(__in LPCTSTR lpszLog);
};

#endif // Wnd_LogView_h__

