//*****************************************************************************
// Filename	: MainFrm.h
// Created	: 2010/11/22
// Modified	: 2010/11/22 - 17:59
//
// Author	: PiRing
//	
// Purpose	: 
//*****************************************************************************
// MainFrm.h : CMainFrame 클래스의 인터페이스
//

#pragma once

#if (SET_INSPECTOR == SYS_ICS_TRINITY_LINE) || (SET_INSPECTOR == SYS_ICS_RIVIAN_LINE)
#include "View_MainCtrl_ICS.h"
#else
#include "View_MainCtrl.h"
#endif

#include "NTCaptionBar.h"
//#include "NTBannerBar.h"
#include "BannerBar_SEM.h"
#include "NTTabViewBar.h"
#include "NTLinksBar.h"
#include "Pane_CommStatus.h"

#include "Define_MonitorInfoFunc.h"

#define CFrameWnd CFrameWndEx

#define USE_BANNER_WINDOW

//=============================================================================
//
//=============================================================================
class CMainFrame : public CFrameWndEx
{

public:
	CMainFrame();
protected: 
	DECLARE_DYNAMIC(CMainFrame)

	// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

	// 구현입니다.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 컨트롤 모음이 포함된 멤버입니다.

#if (SET_INSPECTOR == SYS_ICS_TRINITY_LINE) || (SET_INSPECTOR == SYS_ICS_RIVIAN_LINE)
	CView_MainCtrl_ICS	m_wndView_MainCtrl;
#else
	CView_MainCtrl		m_wndView_MainCtrl;
#endif

	ST_MONITORINFO		m_infoMonitor;

	// 생성된 메시지 맵 함수
protected:
	afx_msg int		OnCreate			(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnSetFocus			(CWnd *pOldWnd);

	afx_msg void	OnClose				();
	afx_msg void	OnActivate			(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void	OnSize				(UINT nType, int cx, int cy);
	afx_msg void	OnGetMinMaxInfo		(MINMAXINFO FAR* lpMMI);
	afx_msg void	OnSettingChange		(UINT uFlags, LPCTSTR lpszSection);

	afx_msg void	OnTabView			(UINT nID);
	afx_msg void	OnUpdateTabView		(CCmdUI* pCmdUI);

	afx_msg	LRESULT	OnWM_LoadComplete		(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWM_Option				(WPARAM wParam, LPARAM lParam);
	afx_msg	LRESULT	OnWM_LogMsg				(WPARAM wParam, LPARAM lParam);

	afx_msg	LRESULT	OnWM_WatchExtProcess	(WPARAM wParam, LPARAM lParam);
	afx_msg	LRESULT	OnWM_OptionChanged		(WPARAM wParam, LPARAM lParam);

	afx_msg	LRESULT	OnWM_TestFunction		(WPARAM wParam, LPARAM lParam);
	afx_msg	LRESULT	OnWM_ChangeView			(WPARAM wParam, LPARAM lParam);

	afx_msg	LRESULT	OnWM_PermissionMode		(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnWM_LineCtrlCmd		(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnWM_EqpCtrlCmd			(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

	int				MakeTabViewBar		();	
	void			OnAdd_Log				(LPCTSTR lpszLog, BOOL bError = FALSE, BYTE bySubLog = 0);
	void			AddLogProgramInfo	();


	HANDLE			m_hThreadStartSetting;
	static UINT WINAPI	ThreadStartSetting	(LPVOID lParam);
	void			LoadStartSetting	();

	BOOL			m_bShowDeviceInfoPane;
	void			ShowPaneByTabIndex	(UINT nTabIndex);

private:

	CNTCaptionBar		m_wndCaptionBar;
#ifdef USE_BANNER_WINDOW
	//CNTBannerBar		m_wndBannerBar;	
	CBannerBar_SEM		m_wndBannerBar;	
#endif
	CNTTabViewBar		m_wndTabViewBar;
	CNTLinksBar			m_wndLinksBar;
	CPane_CommStatus	m_wndDeviceStatus;

	void			SetupMemoryBitmapSize(const CSize& sz);

	UINT				m_nTabView;

	CString				m_strExecutedAppTime;
	// 프로그램 시작 할 때
	void			InitProgram			();

	// 프로그램 종료 할 때
	void			ExitProgram			();

	// 외부의 비정상 종료 감시 프로그램을 실행 시킴.	
	PROCESS_INFORMATION	m_ProcessInfo;
	BOOL			RunWatchProgram		();
	void			CloseWatchProgram	();


	void			ShowWindow_2ndView	(__in BOOL bShow);

	void			OnSet_PermissionMode(__in enPermissionMode nAcessMode, __in bool bInit = false);

public:
	
};


