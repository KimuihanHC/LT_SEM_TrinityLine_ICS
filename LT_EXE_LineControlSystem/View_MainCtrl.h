//*****************************************************************************
// Filename	: View_MainCtrl.h
// Created	: 2010/11/26
// Modified	: 2016/06/07
//
// Author	: PiRing
//	
// Purpose	: 
//*****************************************************************************
#ifndef View_MainCtrl_h__
#define View_MainCtrl_h__

#pragma once

#include "TestManager_EQP.h"
// UI
#include "Wnd_MainView.h"
#include "Wnd_SettingView.h"
#include "Wnd_SocketView.h"
#include "Wnd_LogView.h"
#include "View_MonitoringData.h"
#include "Define_MonitorInfoFunc.h"

typedef enum _enumSubView
{
	SUBVIEW_MAIN = 0,
	SUBVIEW_SOCKET,
	SUBVIEW_RECIPE,
	SUBVIEW_LOG,
	SUBVIEW_MAX,
}enumSubView;

//=============================================================================
// CView_MainCtrl â
//=============================================================================
class CView_MainCtrl : public CWnd
{
	// �����Դϴ�.
public:
	CView_MainCtrl();
	virtual ~CView_MainCtrl();

protected:

	afx_msg int		OnCreate				(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnSize					(UINT nType, int cx, int cy);
	afx_msg void	OnPaint					();
	afx_msg BOOL	OnEraseBkgnd			(CDC* pDC);
	afx_msg HBRUSH	OnCtlColor				(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void	OnTimer					(UINT_PTR nIDEvent);

	DECLARE_MESSAGE_MAP()
	
	virtual BOOL	PreCreateWindow			(CREATESTRUCT& cs);

	// ���� ó����
	CBrush				m_brBkgr;

	//-----------------------------------------------------
	// ���ϵ� ������ ���п�
	//-----------------------------------------------------	
	UINT					m_nWndIndex;
	CWnd*					m_pWndPtr[SUBVIEW_MAX];

	CWnd_MainView			m_wndMainView;
	CWnd_SocketView			m_wndSocketView;
	CWnd_SettingView		m_wndSettingView;
	CWnd_LogView			m_wndLogView;

	// ��� ���� ǥ�� Pane�� ������
	CWnd*					m_pwndCommPane				= nullptr;

	CView_MonitoringData	m_ViewSub;			// Sub ������

	ST_MONITORINFO			m_infoMonitor;		// ����� ���� ����ü

	uint8_t					m_nLanguage					= 1;

	//-----------------------------------------------------
	// �ʱ� ���� ����
	//-----------------------------------------------------
	// �����ڿ��� �ʱ�ȭ �� ����
	virtual void	OnInit_ConstructionSetting			(){};
	// Window ���� �� ����
	virtual void	OnInit_UISetting					(){};
	// �ֺ���ġ�� �⺻ ����
	virtual void	OnInit_DeviceSetting				(){};
	
	//-----------------------------------------------------
	// Sub ������ ���� ����
	//-----------------------------------------------------
	void			Create_SubWindows			();
	virtual void	ShowWindow_SubMonitoring	(__in BOOL bShow);

	void			Update_Language				(__in uint8_t IN_nLanguage);

//=============================================================================
public: 
//=============================================================================
	
	// ������ ���� ������ �Լ�
	void			SetBackgroundColor			(__in COLORREF color, __in BOOL bRepaint = TRUE);

	// ���ϵ� ������ ��ȯ �� ���
	virtual UINT	SwitchWindow				(__in UINT nIndex);
	// ��ġ ��� ���� ǥ�� ������ ������ ����
	virtual void	SetCommPanePtr				(__in CWnd* pwndCommPane);
	
	// �ɼ��� ���� �Ǿ��� ��� �ٽ� UI�� �����͸� �����ϱ� ���� �Լ�
	virtual void	ReloadOption				(){};

	// ���α׷� �ε� ���� �� �ڵ� ó���� ���� �Լ�
	virtual void	InitStartProgress			(){};	
	// ���α׷� ����� ó���ؾ� �� ��ɵ��� ó���ϴ� �Լ�
	virtual void	FinalExitProgress			(){};
	
	//--------------------- TEST --------------------------
	virtual void	Test_Process				(__in UINT nTestNo) = 0;
	//--------------------- TEST --------------------------
	

};

#endif // View_MainCtrl_h__


