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
// CView_MainCtrl ì°?//=============================================================================
class CView_MainCtrl : public CWnd
{
// ?ì„±?…ë‹ˆ??
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

	// ë°°ê²½??ì²˜ë¦¬??/
	CBrush				m_brBkgr;

	//-----------------------------------------------------
	// ì°¨ì¼???ˆë„??êµ¬ë¶„??	//-----------------------------------------------------	
	UINT					m_nWndIndex;
	CWnd*					m_pWndPtr[SUBVIEW_MAX];

	CWnd_MainView			m_wndMainView;
	CWnd_SocketView			m_wndSocketView;
	CWnd_SettingView		m_wndSettingView;
	CWnd_LogView			m_wndLogView;

	// ?µì‹  ?íƒœ ?œì‹œ Pane???¬ì¸??/
	CWnd*					m_pwndCommPane				= nullptr;

	CView_MonitoringData	m_ViewSub;			// Sub ?ˆë„??

	ST_MONITORINFO			m_infoMonitor;		// ëª¨ë‹ˆ???•ë³´ êµ¬ì¡°ì²?	
	uint8_t					m_nLanguage					= 1;

	//-----------------------------------------------------
	// ì´ˆê¸° ?¤ì • ê´€??	//-----------------------------------------------------
	// ?ì„±?ì—??ì´ˆê¸°?????¸íŒ…
	virtual void	OnInit_ConstructionSetting			(){};
	// Window ?ì„± ???¸íŒ…
	virtual void	OnInit_UISetting					(){};
	// ì£¼ë??¥ì¹˜??ê¸°ë³¸ ?¤ì •
	virtual void	OnInit_DeviceSetting				(){};
	
	//-----------------------------------------------------
	// Sub ?ˆë„???¤ì • ê´€??	//-----------------------------------------------------
	void			Create_SubWindows			();
	virtual void	ShowWindow_SubMonitoring	(__in BOOL bShow);

	void			Update_Language				(__in uint8_t IN_nLanguage);

//=============================================================================
public: 
//=============================================================================
	
	// ?ˆë„??ë°°ê²½???¤ì •???¨ìˆ˜
	void			SetBackgroundColor			(__in COLORREF color, __in BOOL bRepaint = TRUE);

	// ì°¨ì¼???ˆë„???„í™˜ ???¬ìš©
	virtual UINT	SwitchWindow				(__in UINT nIndex);
	// ?¥ì¹˜ ?µì‹  ?íƒœ ?œì‹œ ?ˆë„???¬ì¸???¤ì •
	virtual void	SetCommPanePtr				(__in CWnd* pwndCommPane);
	
	// ?µì…˜??ë³€ê²??˜ì—ˆ??ê²½ìš° ?¤ì‹œ UI???°ì´?°ë? ?¸íŒ…?˜ê¸° ?„í•œ ?¨ìˆ˜
	virtual void	ReloadOption				(){};

	// ?„ë¡œê·¸ë¨ ë¡œë”© ?ë‚œ ???ë™ ì²˜ë¦¬ë¥??„í•œ ?¨ìˆ˜
	virtual void	InitStartProgress			(){};	
	// ?„ë¡œê·¸ë¨ ì¢…ë£Œ??ì²˜ë¦¬?´ì•¼ ??ê¸°ëŠ¥?¤ì„ ì²˜ë¦¬?˜ëŠ” ?¨ìˆ˜
	virtual void	FinalExitProgress			(){};
	
	//--------------------- TEST --------------------------
	virtual void	Test_Process				(__in UINT nTestNo) = 0;
	//--------------------- TEST --------------------------
	
};

#endif // View_MainCtrl_h__


