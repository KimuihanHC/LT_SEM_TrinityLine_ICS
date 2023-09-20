//*****************************************************************************
// Filename	: 	Wnd_MaintenanceView.h
// Created	:	2016/5/13 - 11:39
// Modified	:	2016/5/13 - 11:39
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
#ifndef Wnd_MaintenanceView_h__
#define Wnd_MaintenanceView_h__

#pragma once


#include "Wnd_BaseView.h"
#include "VGStatic.h"

#include "Def_DataStruct.h"

//-----------------------------------------------------------------------------
// CWnd_MaintenanceView
//-----------------------------------------------------------------------------
class CWnd_MaintenanceView : public CWnd_BaseView
{
	DECLARE_DYNAMIC(CWnd_MaintenanceView)

public:
	CWnd_MaintenanceView();
	virtual ~CWnd_MaintenanceView();

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg int		OnCreate				(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnSize					(UINT nType, int cx, int cy);

	void			MoveWindow_Initial		(int x, int y, int nWidth, int nHeight, BOOL bRepaint = TRUE);
	void			MoveWindow_Capture		(int x, int y, int nWidth, int nHeight, BOOL bRepaint = TRUE);
	void			MoveWindow_Final		(int x, int y, int nWidth, int nHeight, BOOL bRepaint = TRUE);

	CFont			m_font_Default;	
	CFont			m_Font;

	// UI
// 	CVGStatic			m_st_frameDevice;
// 	CVGStatic			m_st_frameStepInfo;

	// 데이터
	enPermissionMode	m_InspMode		= enPermissionMode::Permission_Operator;
	ST_SystemInfo*	m_pstSysInfo	= nullptr;


public:


	void	SetPtr_SystemInfo		(__in ST_SystemInfo* m_pstSysInfo)
	{
		m_pstSysInfo = m_pstSysInfo;
	};

	// 검사 모드 설정
	void	Set_PermissionMode			(__in enPermissionMode InspMode);

};

#endif // Wnd_MaintenanceView_h__


