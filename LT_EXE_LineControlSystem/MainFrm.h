//*****************************************************************************
// Filename	: MainFrm.h
// Created	: 2010/11/22
// Modified	: 2010/11/22 - 17:59
//
// Author	: PiRing
//	
// Purpose	: 
//*****************************************************************************
// MainFrm.h : CMainFrame ?¥Îûò?§Ïùò ?∏ÌÑ∞?òÏù¥??
//
#pragma once

#if (SET_INSPECTOR == SYS_DEV_MOD_1)
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

	// ?¨Ï†ï?òÏûÖ?àÎã§.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

	// Íµ¨ÌòÑ?ÖÎãà??
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // Ïª®Ìä∏Î°?Î™®Ïùå???¨Ìï®??Î©§Î≤Ñ?ÖÎãà??

#if (SET_INSPECTOR == SYS_DEV_MOD_1)
	CView_MainCtrl_ICS	m_wndView_MainCtrl;
#else
	CView_MainCtrl		m_wndView_MainCtrl;
#endif

	ST_MONITORINFO		m_infoMonitor;

	// ?ùÏÑ±??Î©îÏãúÏßÄ Îß??®Ïàò.
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

#if (USE_XML)
protected:
	afx_msg LRESULT	OnSet_CONNECTION		(WPARAM wParam, LPARAM lParam);
protected:
	afx_msg LRESULT	OnWM_REPORT_RMS_MODE	(WPARAM wParam, LPARAM lParam);	
//=============================================================================
// WM_EVENT_SERVER 2023.03.22a uhkim
//=============================================================================
	afx_msg LRESULT OnWM_Svr_REQUEST_LINK_TEST(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnWM_Svr_REPLY_LINK_TEST(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnWM_Svr_REPORT_ONLINE_STATE(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT	OnWM_Svr_REPORT_USER_CHANGE(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnWM_Svr_REQUEST_USER_CHANGE(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnWM_Svr_REPLY_USER_COMMAND(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT	OnWM_Svr_REPORT_EQUIPMENT_STATE(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnWM_Svr_REQUEST_EQUIPMENT_STATE_DISPLAY(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnWM_Svr_REPLY_EQUIPMENT_STATE_DISPLAY(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT	OnWM_Svr_REPORT_LOSS_STATE(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnWM_Svr_REQUEST_LOSS_WINDOW(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnWM_Svr_REPLY_LOSS_WINDOW(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnWM_Svr_REPORT_ALARM_STATE(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWM_Svr_REQUEST_ALARM_LIST(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWM_Svr_REPLY_ALARM_LIST(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnWM_Svr_REPORT_RMS_MODE(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnWM_Svr_REQUEST_SET_DATETIME(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWM_Svr_REPLY_SET_DATETIME(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnWM_Svr_REQUEST_TERMINAL_MESSAGE(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWM_Svr_REPLY_TERMINAL_MESSAGE(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnWM_Svr_REQUEST_OPCALL_MESSAGE(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWM_Svr_REPLY_OPCALL(WPARAM wParam, LPARAM lParam);
#endif
#if TEST
	afx_msg LRESULT OnWM_Svr_UNITID_READ(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWM_Svr_REQUEST_UNITID_READ(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnWM_Svr_REPLY_UNITID_READ(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnWM_Svr_REPORT_START_PROCESS(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnWM_Svr_REPORT_START_LOT(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWM_Svr_REPORT_END_EVENT(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnWM_Svr_REPORT_END_PROCESS(WPARAM wParam, LPARAM lParam);
#endif	//TEST

//=============================================================================

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

	void				SetupMemoryBitmapSize(const CSize& sz);

	UINT				m_nTabView;

	CString				m_strExecutedAppTime;
	// ?ÑÎ°úÍ∑∏Îû® ?úÏûë ????
	void			InitProgram			();

	// ?ÑÎ°úÍ∑∏Îû® Ï¢ÖÎ£å ????
	void			ExitProgram			();

	// ?∏Î???ÎπÑÏ†ï??Ï¢ÖÎ£å Í∞êÏãú ?ÑÎ°úÍ∑∏Îû®???§Ìñâ ?úÌÇ¥.	
	PROCESS_INFORMATION	m_ProcessInfo;
	BOOL			RunWatchProgram		();
	void			CloseWatchProgram	();


	void			ShowWindow_2ndView	(__in BOOL bShow);

	void			OnSet_PermissionMode(__in enPermissionMode nAcessMode, __in bool bInit = false);

//=============================================================================================
//Add
//=============================================================================================
#if (USE_XML)
private:
	void			OnSet_RMSMode(__in enEES_Mode nAcessMode, __in bool bInit = false);
public:
	void SetSystemTimePrivilege();
#endif
#if TEST
	using ssaTransationIDCntr = lt::StdMapContainer<CStringA, ST_TransactionID *>::Map;
	ssaTransationIDCntr * ssaTransationIDBufferCntr = nullptr;
	lt::StdMutex * m_pssaTransationIDBufferCntrMutex = nullptr;

	using LPARAMCntr = lt::StdSequenceContainer<LPARAM >::Vector;
	LPARAMCntr * m_pLPARAMCntr = nullptr;
	lt::StdMutex * m_pLPARAMCntrMutex = nullptr;

public:	

	ST_TransactionID * CreateTransactionID(CStringA cstr);
	CStringA CreateTransactionID();
	void SetTransactionID(CStringA command, ST_TransactionID *data);
	ST_TransactionID * GetTransactionID(CStringA command) const;
	bool bGetTransactionID(CStringA command) const;
	void ClearTransactionID(CStringA command);

	void AddLPARAM(LPARAM PARA);
	void RemoveLPARAM(LPARAM PARA);
	bool FindLPARAM(LPARAM PARA);
#endif

//=============================================================================
};

extern CMainFrame* MainFrame;

