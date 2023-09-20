//*****************************************************************************
// Filename	: MainFrm.cpp
// Created	: 2010/11/23
// Modified	: 2010/11/23 - 10:45
//
// Author	: PiRing
//	
// Purpose	: 
//*****************************************************************************
// MainFrm.cpp : CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "LT MainExecution.h"
#include "resource.h"
#include "MainFrm.h"
#include "NTVisualManager.h"
#include "Def_CompileOption.h"
#include "CommonFunction.h"
#include "SheetOption.h"
#include "Dlg_ChkPassword.h"
#include "Def_Language_Message.h"

enum enTabName
{
	Tab_Operation,
	Tab_SocketJIG,
	Tab_Setting,
	Tab_Log,
	Tab_MaxCount,
};

static LPCTSTR g_szTabName_T[Lang_MaxCount][Tab_MaxCount] =
{
	// 한국어
	{
		_T("설비 운영"),		// Tab_Operation
		_T("소켓 지그"),		// Tab_SocketJIG
		_T("설정"),				// Tab_Setting
		_T("로그"),				// Tab_Log
	},

	// 영어
	{
		_T("Operation"),		// Tab_Operation
		_T("Socket JIG"),		// Tab_SocketJIG
		_T("Setting"),			// Tab_Setting
		_T("Log"),				// Tab_Log
	},

	// 베트남어
	{
		_T("Operation"),		// Tab_Operation
		_T("Socket JIG"),		// Tab_SocketJIG
		_T("Setting"),			// Tab_Setting
		_T("Log"),				// Tab_Log
	},

	// 중국어
	{
		_T("设备运行"),			// Tab_Operation
		_T("插座夹具"),			// Tab_SocketJIG
		_T("设定"),				// Tab_Setting
		_T("日志"),				// Tab_Log
	},
};

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define IDC_STATIS_COMM_01              3010
#define ID_VIEW_OUTPUTWND				3011

//=============================================================================
// CMainFrame
//=============================================================================
IMPLEMENT_DYNAMIC(CMainFrame, CFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_CLOSE()
	ON_WM_ACTIVATE()
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_WM_SETTINGCHANGE()
	ON_COMMAND_RANGE			(ID_TABVIEW_1, ID_TABVIEW_6, OnTabView)
	ON_UPDATE_COMMAND_UI_RANGE	(ID_TABVIEW_1, ID_TABVIEW_6, OnUpdateTabView)
	ON_MESSAGE(WM_OPTION,				OnWM_Option)
	ON_MESSAGE(WM_LOGMSG,				OnWM_LogMsg)
	ON_MESSAGE(WM_LOAD_COMPLETE,		OnWM_LoadComplete)
	ON_MESSAGE(WM_ALIVE_PROCESS,		OnWM_WatchExtProcess)
	ON_MESSAGE(WM_OPTION_CHANGED,		OnWM_OptionChanged)
	ON_MESSAGE(WM_TEST_FUNCTION,		OnWM_TestFunction)
	ON_MESSAGE(WM_CHANGE_VIEW,			OnWM_ChangeView)
	ON_MESSAGE(WM_PERMISSION_MODE,		OnWM_PermissionMode)
	ON_MESSAGE(WM_LINE_CTRLCMD,			OnWM_LineCtrlCmd)
	ON_MESSAGE(WM_EQP_CTRLCMD,			OnWM_EqpCtrlCmd)
#if (USE_XML)
	ON_MESSAGE(WM_EVENT_SERVER_CONNECTION, OnSet_CONNECTION)
	ON_MESSAGE(WM_EVENT_REPORT_RMS_MODE,OnWM_REPORT_RMS_MODE)
#endif
//	ON_COMMAND(ID_CAPTION_DELETE, &CMainFrame::OnCaptionDelete)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 상태 줄 표시기
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

//=============================================================================
// CMainFrame 생성/소멸
//=============================================================================
//=============================================================================
// Method		: CMainFrame::CMainFrame
// Access		: public 
// Returns		: 
// Qualifier	:
// Last Update	: 2010/04/02 - 15:29
// Desc.		: 
//=============================================================================
CMainFrame::CMainFrame()
{
	m_nTabView				= (UINT)-1;
	m_hThreadStartSetting	= NULL;

	m_bShowDeviceInfoPane	= TRUE;

	m_infoMonitor.nMonitor			= 1;
	m_infoMonitor.nWidthVirtual		= 1280;
	m_infoMonitor.nHeightVirtual	= 1024;
	m_infoMonitor.nWidth			= 1280;
	m_infoMonitor.nHeight			= 1024;
	m_infoMonitor.nBitPerPixel		= 16;
	m_infoMonitor.nRefresh			= 60;
	
#if TEST
	ssaTransationIDBufferCntr			= new ssaTransationIDCntr();
	m_pssaTransationIDBufferCntrMutex	= new lt::StdMutex();

	m_pLPARAMCntr						= new LPARAMCntr();
	m_pLPARAMCntrMutex					= new lt::StdMutex();
#endif
}

//=============================================================================
// Method		: CMainFrame::~CMainFrame
// Access		: public 
// Returns		: 
// Qualifier	:
// Last Update	: 2010/04/02 - 15:29
// Desc.		: 
//=============================================================================
CMainFrame::~CMainFrame()
{
	TRACE(_T("<<< Start ~CMainFrame >>> \n"));

	if (NULL != m_hThreadStartSetting)
		CloseHandle(m_hThreadStartSetting);

#if TEST
	delete ssaTransationIDBufferCntr;
	delete m_pssaTransationIDBufferCntrMutex;

	delete m_pLPARAMCntr;
	delete m_pLPARAMCntrMutex;
#endif
	TRACE(_T("<<< End ~CMainFrame >>> \n"));
}

//=============================================================================
// Method		: CMainFrame::OnCreate
// Access		: protected 
// Returns		: int
// Parameter	: LPCREATESTRUCT lpCreateStruct
// Qualifier	:
// Last Update	: 2010/04/02 - 15:29
// Desc.		: 
//=============================================================================
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon, FALSE);

	ModifyStyle		(WS_CAPTION | FWS_ADDTOTITLE, 0);
	ModifyStyleEx	(WS_EX_CLIENTEDGE, 0);

	CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
	CMFCVisualManager::SetDefaultManager (RUNTIME_CLASS (CNTVisualManager));
	CMFCToolBarButton::m_bWrapText = FALSE;
		
	//-------------------------------------------------------------------------
	// Caption Bar
	//-------------------------------------------------------------------------
	m_wndCaptionBar.SetWM_Option(WM_OPTION);
	if (!m_wndCaptionBar.Create(this, IDR_MAINFRAME))
	{
		TRACE0("Failed to create Caption Bar \n");
		return -1;
	}

	if (afxGlobalData.fontRegular.GetSafeHandle () != NULL)
	{
		LOGFONT lf;
		ZeroMemory (&lf, sizeof (LOGFONT));
		afxGlobalData.fontRegular.GetLogFont (&lf);

		m_wndCaptionBar.SetCaptionFont (lf);
	}

	EnableDocking(CBRS_ALIGN_ANY);

#ifdef USE_BANNER_WINDOW
	//-------------------------------------------------------------------------
	// Banner
	//-------------------------------------------------------------------------
	//m_wndBannerBar.Set_UseClock(false);
	m_wndBannerBar.Set_UseClock(true);
	m_wndBannerBar.Set_TextFontSize(30);
	m_wndBannerBar.Set_TextPosition_Adjust(40, 5);
	m_wndBannerBar.Set_IconPosition_Adjust(20, 0);
	m_wndBannerBar.Set_ClockFontSize(14, 22, 14);
	m_wndBannerBar.Set_ClockPosition_Adjust(0, 0, 0, -4, -35, -8);
	if (!m_wndBannerBar.Create(this, IDI_ICON_SEM))	
	{
		TRACE0("Failed to create Banner Bar\n");
		return -1;
	}

//	DockPane(&m_wndBannerBar);

	if (afxGlobalData.fontRegular.GetSafeHandle () != NULL)
	{
		LOGFONT lf;
		ZeroMemory (&lf, sizeof (LOGFONT));
		
		afxGlobalData.fontRegular.GetLogFont (&lf);
		lf.lfHeight = 50;//배너 높이
		lf.lfWeight	= 600;

		m_wndBannerBar.SetCaptionFont (lf);
	}
	
	// 배너에 프로그램 이름 쓰기 --------------------------------------
	CString szAppName;
#ifdef USE_DEBUG_MODE
	szAppName.Format(_T("%s (Debug Mode)"), g_szInsptrSysType[SET_INSPECTOR]);
#else
	szAppName.Format(_T("%s"), g_szInsptrSysType[SET_INSPECTOR]);
#endif
	m_wndBannerBar.SetWindowText (szAppName);
#endif

	//-------------------------------------------------------------------------
	// Links Bar
	//-------------------------------------------------------------------------
	if (!m_wndLinksBar.CreateEx(this, TBSTYLE_FLAT, CBRS_SIZE_DYNAMIC | AFX_DEFAULT_TOOLBAR_STYLE, CRect(1, 1, 1, 1), AFX_IDW_TOOLBAR + 1))
	{
		TRACE0("Failed to create linksbar\n");
		return -1;
	}
	DockPane(&m_wndLinksBar);

	//-------------------------------------------------------------------------
	// TabView Bar
	//-------------------------------------------------------------------------
 	if (!MakeTabViewBar ())
 		return -1;

	//-------------------------------------------------------------------------
	// 장비 정보 표시용 Pane
	//-------------------------------------------------------------------------	
	int nPaneWidth = 140;
	m_wndDeviceStatus.Set_Width(nPaneWidth);
	m_wndDeviceStatus.Create (_T(""), this, CRect (0, 0, nPaneWidth, 200), FALSE, IDR_MAINFRAME + 1, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT, AFX_CBRS_REGULAR_TABS, NULL/*AFX_CBRS_RESIZE | AFX_CBRS_AUTO_ROLLUP*/);
	DockPane(&m_wndDeviceStatus);

	/*CLT_Option	ntOption;
	stOpt_Insp	stOption;
	ntOption.SetInspectorType((enInsptrSysType)SET_INSPECTOR);
	ntOption.SetRegistryPath(REG_PATH_OPTION_BASE);
	ntOption.LoadOption_Inspector(stOption);
	m_bShowDeviceInfoPane = stOption.UseDeviceInfoPane;

	if (!m_bShowDeviceInfoPane)
		m_wndDeviceStatus.ShowPane(FALSE, 0, TRUE);	*/

	//-------------------------------------------------------------------------
	// 프레임의 클라이언트 영역을 차지하는 뷰를 만듭니다.
	//-------------------------------------------------------------------------
	// 주변기기 통신 연결
	m_wndView_MainCtrl.SetCommPanePtr(&m_wndDeviceStatus);
	m_wndView_MainCtrl.SetOwner(this);
	if (!m_wndView_MainCtrl.Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("뷰 창을 만들지 못했습니다.\n");
		return -1;
	}	


	OnTabView (ID_TABVIEW_1);
 	
//#ifndef _DEBUG
//  	//m_wndTabViewBar.ShowButton_Exclusive(0, FALSE);	// **** 배포할때 사용해야 함
 	OnSet_PermissionMode(Permission_Operator, true);
//#endif
#if (USE_XML)
	SetSystemTimePrivilege();
#endif
	return 0;
}

//=============================================================================
// Method		: CMainFrame::MakeTabViewBar
// Access		: protected 
// Returns		: BOOL
// Qualifier	:
// Last Update	: 2010/10/8 - 14:27
// Desc.		:
//=============================================================================
BOOL CMainFrame::MakeTabViewBar()
{
	m_wndTabViewBar.Set_ButtonWidth(220);

	CLT_Option	ntOption;
	stOpt_Insp	stOption;
	ntOption.SetRegistryPath(REG_PATH_OPTION_BASE);
	ntOption.SetInspectorType((enInsptrSysType)SET_INSPECTOR);
	ntOption.LoadOption_Inspector(stOption);
	uint8_t nLang = stOption.nLanguage;

	UINT nID = ID_TABVIEW_1;
	m_wndTabViewBar.AddTabID (g_szTabName_T[nLang][Tab_Operation],	nID++);
	m_wndTabViewBar.AddTabID (g_szTabName_T[nLang][Tab_SocketJIG],	nID++);
	m_wndTabViewBar.AddTabID (g_szTabName_T[nLang][Tab_Setting],	nID++);
	m_wndTabViewBar.AddTabID (g_szTabName_T[nLang][Tab_Log],		nID++);

	if (!m_wndTabViewBar.CreateEx (this, TBSTYLE_FLAT, CBRS_SIZE_FIXED | CBRS_ALIGN_BOTTOM | AFX_DEFAULT_TOOLBAR_STYLE, CRect (0, 0, 0, 0), AFX_IDW_TOOLBAR))
	{
		TRACE0("Failed to create TabView Bar\n");
		return FALSE;      // fail to create
	}

	m_wndTabViewBar.EnableDocking(CBRS_ALIGN_BOTTOM);
	DockPane (&m_wndTabViewBar);

	return TRUE;
}

//=============================================================================
// Method		: CMainFrame::PreCreateWindow
// Access		: public 
// Returns		: BOOL
// Parameter	: CREATESTRUCT & cs
// Qualifier	:
// Last Update	: 2010/04/02 - 15:29
// Desc.		: 
//=============================================================================
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;

	if(cs.hMenu != NULL)
	{
		::DestroyMenu(cs.hMenu);
		cs.hMenu = NULL;
	}

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;	
	cs.lpszClass = AfxRegisterWndClass(0);	


	BOOL bRet = FALSE;
	CString strTitle = _T("");
	bRet = strTitle.LoadString (IDR_MAINFRAME);
		
	CString strVersion = GetVersionInfo(_T("ProductVersion"));	
	CString	strDate = GetVersionInfo(_T("FileVersion"));

	CString strCaption;
	strCaption.Format (_T("%s ( Ver %s : %s)"), g_szInsptrSysType[SET_INSPECTOR], strVersion.GetBuffer(), strDate);
	CFrameWnd::SetTitle (strCaption);

	strTitle.Empty();
	strTitle.ReleaseBuffer();
	strVersion.Empty();
	strVersion.ReleaseBuffer();

	return TRUE;
}

//=============================================================================
// CMainFrame 진단
//=============================================================================
#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG

//=============================================================================
// CMainFrame 메시지 처리기
//=============================================================================
//=============================================================================
// Method		: CMainFrame::OnSetFocus
// Access		: protected 
// Returns		: void
// Parameter	: CWnd *
// Qualifier	:
// Last Update	: 2010/04/02 - 15:28
// Desc.		: 
//=============================================================================
void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// 뷰 창으로 포커스를 이동합니다.	
	m_wndView_MainCtrl.SetFocus();
}

//=============================================================================
// Method		: CMainFrame::OnCmdMsg
// Access		: public 
// Returns		: BOOL
// Parameter	: UINT nID
// Parameter	: int nCode
// Parameter	: void * pExtra
// Parameter	: AFX_CMDHANDLERINFO * pHandlerInfo
// Qualifier	:
// Last Update	: 2010/04/02 - 15:28
// Desc.		: 
//=============================================================================
BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// 뷰에서 첫째 크랙이 해당 명령에 나타나도록 합니다.	
	if (m_wndView_MainCtrl.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// 그렇지 않으면 기본 처리합니다.
	return CFrameWndEx::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

//=============================================================================
// Method		: CMainFrame::OnClose
// Access		: protected 
// Returns		: void
// Qualifier	:
// Last Update	: 2010/04/02 - 15:28
// Desc.		: 
//=============================================================================
void CMainFrame::OnClose() 
{
	CLT_Option	ntOption;
	stLT_Option	stOption;
	ntOption.SetRegistryPath(REG_PATH_OPTION_BASE);
	ntOption.LoadOption_All(stOption);

	//if (IDYES == LT_MessageBox(_T("Are you sure you want to exit the program?"), MB_YESNO))
	if (IDYES == LT_MessageBox(g_szMessageBox_T[MB_Common_ExitProgram][stOption.Inspector.nLanguage], MB_YESNO))
	{
		TRACE(_T("<<< Exit Program >>> \n"));
		m_wndView_MainCtrl.FinalExitProgress();

		CloseWatchProgram();
		ExitProgram();
		CFrameWndEx::OnClose();
	}
}


//=============================================================================
// Method		: CMainFrame::OnActivate
// Access		: protected 
// Returns		: void
// Parameter	: UINT nState
// Parameter	: CWnd * pWndOther
// Parameter	: BOOL bMinimized
// Qualifier	:
// Last Update	: 2010/04/02 - 15:28
// Desc.		: 
//=============================================================================
void CMainFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CFrameWndEx::OnActivate(nState, pWndOther, bMinimized);

	if (m_wndCaptionBar.GetSafeHwnd () != NULL)
	{
		m_wndCaptionBar.SetParentActive (nState != WA_INACTIVE);
	}
}

//=============================================================================
// Method		: CMainFrame::OnSize
// Access		: protected 
// Returns		: void
// Parameter	: UINT nType
// Parameter	: int cx
// Parameter	: int cy
// Qualifier	:
// Last Update	: 2010/04/02 - 15:28
// Desc.		: 
//=============================================================================
void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	SetupMemoryBitmapSize (CSize (cx, cy));

	if (m_wndCaptionBar.GetSafeHwnd () != NULL)
	{
		m_wndCaptionBar.SetParentMaximize (nType == SIZE_MAXIMIZED);
	}

#ifdef USE_BANNER_WINDOW
	if (m_wndBannerBar.GetSafeHwnd () != NULL)
	{
		m_wndBannerBar.SetParentMaximize (nType == SIZE_MAXIMIZED);
	}
#endif

	// 그림이 깨지는 증상 때문에
	Invalidate();

	CFrameWndEx::OnSize(nType, cx, cy);
}

//=============================================================================
// Method		: CMainFrame::OnGetMinMaxInfo
// Access		: protected 
// Returns		: void
// Parameter	: MINMAXINFO FAR * lpMMI
// Qualifier	:
// Last Update	: 2010/04/02 - 15:28
// Desc.		: 
//=============================================================================
void CMainFrame::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	CFrameWndEx::OnGetMinMaxInfo(lpMMI);

	if(m_wndTabViewBar.GetSafeHwnd () != NULL)
	{
		CRect rtRect (CPoint (0, 0), m_wndTabViewBar.CalcSize (FALSE));

		rtRect.bottom = 200;

		CalcWindowRect (rtRect, CWnd::adjustBorder);

		lpMMI->ptMinTrackSize.x = rtRect.Width ();
		lpMMI->ptMinTrackSize.y = rtRect.Height ();		
	}

	lpMMI->ptMinTrackSize.x = 1920;//640;
	lpMMI->ptMinTrackSize.y = 1080;

	lpMMI->ptMaxTrackSize.x = 2560;
	lpMMI->ptMaxTrackSize.y = 1600;

}

//=============================================================================
// Method		: CMainFrame::OnSettingChange
// Access		: protected 
// Returns		: void
// Parameter	: UINT uFlags
// Parameter	: LPCTSTR lpszSection
// Qualifier	:
// Last Update	: 2010/04/02 - 15:28
// Desc.		: 
//=============================================================================
void CMainFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection) 
{
	CFrameWndEx::OnSettingChange(uFlags, lpszSection);

	if (m_wndCaptionBar.GetSafeHwnd () != NULL && afxGlobalData.fontRegular.GetSafeHandle () != NULL)
	{
		LOGFONT lf;
		ZeroMemory (&lf, sizeof (LOGFONT));
		afxGlobalData.fontRegular.GetLogFont (&lf);

		m_wndCaptionBar.SetCaptionFont (lf);
	}

	CRect rt;
	GetClientRect (rt);

	SetupMemoryBitmapSize (rt.Size ());
}

//=============================================================================
// Method		: CMainFrame::SetupMemoryBitmapSize
// Access		: private 
// Returns		: void
// Parameter	: const CSize & sz
// Qualifier	:
// Last Update	: 2010/04/02 - 15:28
// Desc.		: 배경으로 쓰이는 비트맵 세로 크기를 정함
//=============================================================================
void CMainFrame::SetupMemoryBitmapSize (const CSize& sz)
{
	CNTVisualManager* pManager = DYNAMIC_DOWNCAST (CNTVisualManager, CMFCVisualManager::GetInstance ());

	if (pManager != NULL)
	{
		CRect rtRes (0, 0, sz.cx, 0);
		CRect rt;

#ifdef USE_BANNER_WINDOW
		CPane* bars[2] = {&m_wndCaptionBar, &m_wndBannerBar};

		for (long i = 0; i < 2; i++)
		{
			CPane* pBar = bars [i];

			if (pBar != NULL && pBar->GetSafeHwnd () != NULL)
			{
				pBar->GetWindowRect (rt);
				rtRes.bottom += rt.Height ();
			}
		}
#else
		CPane* bars[2] = { &m_wndCaptionBar, &m_wndTabViewBar };

		for (long i = 0; i < 2; i++)
		{
			CPane* pBar = bars[i];

			if (pBar != NULL && pBar->GetSafeHwnd() != NULL)
			{
				pBar->GetWindowRect(rt);
				rtRes.bottom += rt.Height();
			}
		}
#endif

		CDC* pDC = GetDC ();

		pManager->SetupMemoryBitmapSize (pDC, rtRes.Size ());

		ReleaseDC (pDC);
	}
}

//=============================================================================
// Method		: CMainFrame::OnTabView
// Access		: protected 
// Returns		: void
// Parameter	: UINT nID
// Qualifier	:
// Last Update	: 2010/04/02 - 15:27
// Desc.		: 
//=============================================================================
void CMainFrame::OnTabView(UINT nID)
{
	if (m_nTabView == nID)
		return;
		
	UINT nOldView = m_nTabView;
	m_nTabView = nID - ID_TABVIEW_1;
	
	if ((SUBVIEW_RECIPE == m_nTabView) || (SUBVIEW_LOG == m_nTabView))
	{
		m_wndView_MainCtrl.SwitchWindow(m_nTabView);

		// Pane 윈도우 숨김/보이기
		ShowPaneByTabIndex(m_nTabView);
	}
	else if ((SUBVIEW_RECIPE == nOldView) || (SUBVIEW_LOG == nOldView))
	{
		// Pane 윈도우 숨김/보이기
		ShowPaneByTabIndex(m_nTabView);

		m_wndView_MainCtrl.SwitchWindow(m_nTabView);
	}
	else
	{
		m_wndView_MainCtrl.SwitchWindow(m_nTabView);
	}
}

//=============================================================================
// Method		: CMainFrame::OnUpdateTabView
// Access		: protected 
// Returns		: void
// Parameter	: CCmdUI * pCmdUI
// Qualifier	:
// Last Update	: 2010/04/02 - 15:27
// Desc.		: 
//=============================================================================
void CMainFrame::OnUpdateTabView(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable (TRUE);
	pCmdUI->SetCheck (pCmdUI->m_nID == (m_nTabView + ID_TABVIEW_1));
}

//=============================================================================
// Method		: CMainFrame::OnWM_Option
// Access		: protected 
// Returns		: LRESULT
// Parameter	: WPARAM wParam
// Parameter	: LPARAM lParam
// Qualifier	:
// Last Update	: 2010/5/7 - 14:21
// Desc.		: 옵션 처리
//=============================================================================
LRESULT CMainFrame::OnWM_Option( WPARAM wParam, LPARAM lParam )
{
	if (enPermissionMode::Permission_Administrator == m_wndView_MainCtrl.GetPermissionMode())
	{
		// 패스워드 체크
// 		CDlg_ChkPassword	dlgPassword(this);
// 		if (IDCANCEL == dlgPassword.DoModal())
// 			return FALSE;

		CList<UINT, UINT> listPageID;
		UINT nIDCaption = 0;
		for (UINT iCnt = 0; iCnt < OPT_TYPE_MAX; iCnt++) // 최대 옵션 Page 수 : 8
		{
			nIDCaption = IDS_STR_OPT_PAGE_01 + iCnt;	// 캡션 스트링 ID
			listPageID.AddTail(nIDCaption);
		}

		CSheetOption *pPropSheet = new CSheetOption(IDS_STR_OPTION, NULL);
		// 검사기 설정 (*필수 항목)
		pPropSheet->SetInspectorType((enInsptrSysType)SET_INSPECTOR);
		pPropSheet->SetWM_ItemChanged(WM_OPTION_CHANGED);
		pPropSheet->SetRegistryPath(REG_PATH_OPTION_BASE);
		pPropSheet->InitPage(IDD_PROPPAGE_OPTION, listPageID);
		pPropSheet->DoModal();

		delete pPropSheet;
	}
	
	return TRUE;
}

//=============================================================================
// Method		: CMainFrame::OnWM_LogMsg
// Access		: protected 
// Returns		: LRESULT
// Parameter	: WPARAM wParam	-> 메세지 문자열
// Parameter	: LPARAM lParam	
//					-> HIWORD : 오류 메세지 인가?
//					-> LOWORD : 로그 종류 (기본, PLC, 관리PC 등)
// Qualifier	:
// Last Update	: 2010/10/14 - 17:38
// Desc.		: 로그 처리용
//	LOG_TAB_PLC		= 0,
//	LOG_TAB_MANPC,
//	LOG_TAB_IRDA,
//	LOG_TAB_BCR,
//=============================================================================
LRESULT CMainFrame::OnWM_LogMsg( WPARAM wParam, LPARAM lParam )
{
	BOOL	bError = (BOOL)HIWORD(lParam);
	UINT	nType  = LOWORD(lParam);

	CString strLog = (LPCTSTR)wParam;

	OnAdd_Log (strLog, bError, nType);

	//if (NULL != (LPTSTR)wParam)
	//	delete [] (LPTSTR)wParam;

	return TRUE;
}

//=============================================================================
// Method		: CMainFrame::OnWM_LoadComplete
// Access		: protected 
// Returns		: LRESULT
// Parameter	: WPARAM wParam
// Parameter	: LPARAM lParam
// Qualifier	:
// Last Update	: 2010/10/15 - 15:24
// Desc.		:
//=============================================================================
LRESULT CMainFrame::OnWM_LoadComplete( WPARAM wParam, LPARAM lParam )
{
	TRACE (_T("Start OnWM_LoadComplete\n"));

	//-------------------------------------------
	// 옵션 설정 데이터 표시	
	//-------------------------------------------
	m_hThreadStartSetting = HANDLE(_beginthreadex(NULL, 0, ThreadStartSetting, this, 0, NULL));	
	
	TRACE (_T("End OnWM_LoadComplete\n"));

	return 1;
}

//=============================================================================
// Method		: CMainFrame::OnAdd_Log
// Access		: protected 
// Returns		: void
// Parameter	: LPCTSTR lpszLog
// Parameter	: BOOL bError
// Parameter	: BYTE bySubLog
// Qualifier	:
// Last Update	: 2010/10/15 - 15:28
// Desc.		:
//=============================================================================
void CMainFrame::OnAdd_Log( LPCTSTR lpszLog, BOOL bError /*= FALSE*/, BYTE bySubLog /*= 0*/ )
{
	// UI -------------------------------------------------
	if (!m_wndView_MainCtrl.GetSafeHwnd())
		return;

	m_wndView_MainCtrl.OnAdd_Log(lpszLog, bError, bySubLog);
}

//=============================================================================
// Method		: CMainFrame::AddLogProgramInfo
// Access		: protected 
// Returns		: void
// Qualifier	:
// Last Update	: 2010/10/26 - 16:52
// Desc.		: 프로그램, 옵션설정 관련 정보를 로그에 추가합니다.
//=============================================================================
void CMainFrame::AddLogProgramInfo()
{
	CString strLog;

	if (m_strExecutedAppTime.IsEmpty())
	{	
		COleDateTime now = COleDateTime::GetCurrentTime ();	
		m_strExecutedAppTime = now.Format (_T("[%Y/%m/%d %H:%M:%S]"));
	}
	else
	{
		strLog.Format(_T("  Executed Time : %s"), m_strExecutedAppTime);
		OnAdd_Log (strLog);
	}

	strLog = _T("  Program Version : ") + GetVersionInfo(_T("ProductVersion"));
	OnAdd_Log (strLog);
	strLog = _T("  File Version    : ") + GetVersionInfo(_T("FileVersion"));
	OnAdd_Log (strLog);

	//-------------------------------------------
	// 옵션 설정 데이터 표시	.
	//-------------------------------------------
	CLT_Option	ntOption;
	stLT_Option	stOption;
	ntOption.LoadOption_All(stOption);

}

//=============================================================================
// Method		: CMainFrame::InitProgram
// Access		: private 
// Returns		: void
// Qualifier	:
// Last Update	: 2010/10/15 - 14:54
// Desc.		: 프로그램 시작 할 때 처리해야 할 코드.
//=============================================================================
void CMainFrame::InitProgram()
{
	OnAdd_Log(_T("=================================================="));
	OnAdd_Log(_T("            In-Line Control System"));
	OnAdd_Log(_T("--------------------------------------------------"));
	AddLogProgramInfo();
	OnAdd_Log(_T("=================================================="));
}

//=============================================================================
// Method		: CMainFrame::ExitProgram
// Access		: private 
// Returns		: void
// Qualifier	:
// Last Update	: 2010/10/15 - 14:54
// Desc.		: 프로그램 종료 할 때 처리해야 할 코드.
//=============================================================================
void CMainFrame::ExitProgram()
{
	OnAdd_Log(_T("=================================================="));
	OnAdd_Log(_T("                 Exit  Program"));
	OnAdd_Log(_T("--------------------------------------------------"));
	AddLogProgramInfo();
	OnAdd_Log(_T("=================================================="));
}

//=============================================================================
// Method		: CMainFrame::RunWatchProgram
// Access		: private 
// Returns		: BOOL
// Qualifier	:
// Last Update	: 2010/12/28 - 17:54
// Desc.		: 프로그램 비 정상 종료 시 자동 실행 용
//=============================================================================
BOOL CMainFrame::RunWatchProgram()
{
	HWND hwnd = GetSafeHwnd();
	HINSTANCE handle = (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE);
	
	TCHAR szExePath[MAX_PATH] = {0};	
	GetModuleFileName(handle, szExePath, MAX_PATH);

	CString strLog;
	strLog.Format(_T("AppRestarter Execute : %s"), szExePath);
	OnAdd_Log (strLog);


	TCHAR drive[_MAX_DRIVE];
	TCHAR dir[_MAX_DIR];	
	TCHAR file[_MAX_FNAME];
	TCHAR ext[_MAX_EXT];	
	_tsplitpath_s (szExePath, drive, _MAX_DRIVE, dir, _MAX_DIR, file, _MAX_FNAME, ext, _MAX_EXT);

	CString strWindowName;
	GetWindowText(strWindowName);

	CString strTargetPath;
	strTargetPath.Format(_T("%s\\%sAppRestarter.exe"), drive, dir);

	HWND hWnd = ::FindWindow(NULL, _T("AppRestarter"));

	if (NULL == hWnd)
		//if (!ShellExecute( m_hWnd, "open", strTargetPath, szExePath, NULL, SW_SHOWNORMAL))
		if (!ShellExecute( m_hWnd, _T("open"), strTargetPath, strWindowName.GetBuffer(), NULL, SW_SHOWNORMAL))
			return FALSE;


	return TRUE;

	//return NTCreateProcess (m_ProcessInfo, _T("AppRestarter"), szExePath);	
}

//=============================================================================
// Method		: CMainFrame::CloseWatchProgram
// Access		: private 
// Returns		: void
// Qualifier	:
// Last Update	: 2010/12/28 - 19:00
// Desc.		:
//=============================================================================
void CMainFrame::CloseWatchProgram()
{
	//NTExitProcess (m_ProcessInfo);

	OnAdd_Log (_T("Quit AppRestarter"));

	HWND hWnd = ::FindWindow(NULL, _T("AppRestarter"));

	if (NULL != hWnd)
		::PostMessage (hWnd, WM_CLOSE, 0, 0);

	EraseTrayIcon();
}

//=============================================================================
// Method		: ShowWindow_2ndView
// Access		: private  
// Returns		: void
// Parameter	: __in BOOL bShow
// Qualifier	:
// Last Update	: 2016/5/17 - 17:15
// Desc.		:
//=============================================================================
void CMainFrame::ShowWindow_2ndView(__in BOOL bShow)
{
	if (bShow)
	{
		// 모니터 전체 해상도로 출력
		//RECT rcDesktop;
		//HWND hWndDesktop = ::GetDesktopWindow();
		//::GetWindowRect(hWndDesktop, &rcDesktop);

		GetMonitorInformation(m_infoMonitor);

		int iMonCnt = m_infoMonitor.nMonitor;

		int iResWidth = GetSystemMetrics(SM_CXSCREEN);
		int iResHeight = GetSystemMetrics(SM_CYSCREEN);

		if (1 < iMonCnt)
		{
			RECT rt = m_infoMonitor.aryMonitors.GetAt(1).rcMonitor;
			RECT rtWork = m_infoMonitor.aryMonitors.GetAt(1).rcWork;

// 			m_wnd_2ndView.MoveWindow(rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top);
// 			m_wnd_2ndView.ShowWindow(SW_SHOW);
		}
		/*else
		{
		RECT rt = m_infoMonitor.aryMonitors.GetAt(0).rcMonitor;
		RECT rtWork = m_infoMonitor.aryMonitors.GetAt(0).rcWork;

		m_wnd_2ndView.MoveWindow(rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top);
		m_wnd_2ndView.ShowWindow(SW_SHOW);
		}*/


		// 모니터 개수 표시
		// 	CString strDevice;
		// 	for (int iCnt = 0; iCnt < iMonCnt; iCnt++)
		// 	{
		// 		strDevice = m_infoMonitor.aryMonitors.GetAt(iCnt).szDevice;
		// 		strDevice.Remove('\\');
		// 		strDevice.Remove('.');
		// 		
		// 	}	
	}
	else
	{
		//m_wnd_2ndView.ShowWindow(SW_HIDE);
	}
}

//=============================================================================
// Method		: OnSet_PermissionMode
// Access		: private  
// Returns		: void
// Parameter	: __in enPermissionMode nAcessMode
// Parameter	: __in bool bInit
// Qualifier	:
// Last Update	: 2022/10/19 - 19:44
// Desc.		:
//=============================================================================
void CMainFrame::OnSet_PermissionMode(__in enPermissionMode nAcessMode, __in bool bInit /*= false*/)
{
	//if (Permission_Operator == m_wndView_MainCtrl.GetPermissionMode())
	if (Permission_Operator == nAcessMode)
	{
		UINT nTabz[] = { 0, 1, 3 }; // Operation, Socket, Log
		m_wndTabViewBar.ShowButton_Exclusive(nTabz, 3, FALSE);

		m_wndCaptionBar.Enable_OptionButton(false);
	}
	else
	{
		UINT nTabz[] = { 0, 1, 2, 3, 4 }; // Operation, Socket, Setting, Log
		m_wndTabViewBar.ShowButton_Exclusive(nTabz, 0, TRUE);

		if (Permission_Administrator == nAcessMode)
		{
			m_wndCaptionBar.Enable_OptionButton(true);
		}
		else
		{
			m_wndCaptionBar.Enable_OptionButton(false);
		}
	}

	if (bInit)
	{
		m_wndView_MainCtrl.OnInit_PermissionMode(nAcessMode);
	}
	else
	{
		m_wndView_MainCtrl.OnSet_PermissionMode(nAcessMode);
	}
}



//=============================================================================
// Method		: CMainFrame::OnWM_WatchExtProcess
// Access		: private 
// Returns		: LRESULT
// Parameter	: WPARAM wParam
// Parameter	: LPARAM lParam
// Qualifier	:
// Last Update	: 2010/12/29 - 14:00
// Desc.		:
//=============================================================================
LRESULT CMainFrame::OnWM_WatchExtProcess( WPARAM wParam, LPARAM lParam )
{
	//OnAdd_Log(_T("AppRestarter Check"));

	return TRUE;
}

//=============================================================================
// Method		: CMainFrame::LoadStartSetting
// Access		: protected 
// Returns		: void
// Qualifier	:
// Last Update	: 2011/1/11 - 17:15
// Desc.		:
//=============================================================================
void CMainFrame::LoadStartSetting()
{
	//-------------------------------------------
	// 옵션 설정 데이터 표시	
	//-------------------------------------------
	//CLT_Option	ntOption;
	//stOpt_Insp	stOption;
	//ntOption.LoadOption_Inspector(stOption);

	// 초기 로그
	InitProgram();

	m_wndView_MainCtrl.InitStartProgress();

	ShowWindow_2ndView(TRUE);
}

//=============================================================================
// Method		: ShowPaneByTabIndex
// Access		: protected  
// Returns		: void
// Parameter	: UINT nTabIndex
// Qualifier	:
// Last Update	: 2016/10/2 - 18:50
// Desc.		:
//=============================================================================
void CMainFrame::ShowPaneByTabIndex(UINT nTabIndex)
{
	// Pane 윈도우 숨김/보이기
	BOOL bUIChanged = FALSE;
	CLT_Option	ntOption;
	stOpt_Insp	stOption;
	ntOption.SetRegistryPath(REG_PATH_OPTION_BASE);
	ntOption.LoadOption_Inspector(stOption);
	if ((SUBVIEW_RECIPE == (nTabIndex)) || (SUBVIEW_LOG == (nTabIndex)))
	{
		//if (stOption.UseDeviceInfoPane)
		{
			if (m_wndDeviceStatus.IsVisible())
			{
				m_wndDeviceStatus.ShowPane(FALSE, 0, FALSE);
				bUIChanged = TRUE;
			}
		}
	}
	else
	{
		//if (stOption.UseDeviceInfoPane)
		{
			if (FALSE == m_wndDeviceStatus.IsVisible())
			{
				m_wndDeviceStatus.ShowPane(TRUE, 0, FALSE);
				bUIChanged = TRUE;
			}
		}
	}

	// UI가 변경 되었으면 다시 그리기
	if (bUIChanged)
	{
		CRect rc;
		GetClientRect(rc);
		SendMessage(WM_SIZE, (WPARAM)SIZE_RESTORED, MAKELPARAM(rc.Width(), rc.Height()));
	}
}

//=============================================================================
// Method		: CMainFrame::ThreadStartSetting
// Access		: protected static 
// Returns		: UINT WINAPI
// Parameter	: LPVOID lParam
// Qualifier	:
// Last Update	: 2011/1/11 - 17:17
// Desc.		:
//=============================================================================
UINT WINAPI CMainFrame::ThreadStartSetting( LPVOID lParam )
{
	CMainFrame* pThis = (CMainFrame*)lParam;	

	pThis->LoadStartSetting ();

	return TRUE;
}

//=============================================================================
// Method		: CMainFrame::OnWM_OptionChanged
// Access		: protected 
// Returns		: LRESULT
// Parameter	: WPARAM wParam
// Parameter	: LPARAM lParam
// Qualifier	:
// Last Update	: 2013/2/20 - 15:07
// Desc.		:
// m_WMItemChanged -> WPARAM : Redraw Window
//					  LPARAM : Reconnect Network & Comport
//=============================================================================
LRESULT CMainFrame::OnWM_OptionChanged( WPARAM wParam, LPARAM lParam )
{
	// UI가 변경 되었으면 다시 그림?
	BOOL bUIChanged = FALSE;

	CLT_Option	ntOption;
	stLT_Option	stOption;
	ntOption.SetRegistryPath(REG_PATH_OPTION_BASE);
	ntOption.LoadOption_All(stOption);

	for (auto nIdx = 0; nIdx < Tab_MaxCount; ++nIdx)
	{
		m_wndTabViewBar.Button_SetWindowText(nIdx, g_szTabName_T[stOption.Inspector.nLanguage][nIdx]);
	}

	m_wndView_MainCtrl.ReloadOption();

	// UI가 변경 되었으면 다시 그리기
	if (bUIChanged)
	{
		CRect rc;
		GetClientRect(rc);
		SendMessage(WM_SIZE, (WPARAM)SIZE_RESTORED, MAKELPARAM(rc.Width(), rc.Height()));
	}

	return 1;
}

//=============================================================================
// Method		: CMainFrame::OnWM_TestFunction
// Access		: protected 
// Returns		: LRESULT
// Parameter	: WPARAM wParam
// Parameter	: LPARAM lParam
// Qualifier	:
// Last Update	: 2013/6/11 - 20:03
// Desc.		:
//=============================================================================
LRESULT CMainFrame::OnWM_TestFunction( WPARAM wParam, LPARAM lParam )
{
	UINT nTestNo = (UINT)wParam;

	m_wndView_MainCtrl.Test_Process(nTestNo);

	return 1;
}

//=============================================================================
// Method		: CMainFrame::OnWM_ChangeView
// Access		: protected 
// Returns		: LRESULT
// Parameter	: WPARAM wParam
// Parameter	: LPARAM lParam
// Qualifier	:
// Last Update	: 2013/6/18 - 19:13
// Desc.		:
//=============================================================================
LRESULT CMainFrame::OnWM_ChangeView( WPARAM wParam, LPARAM lParam )
{
	UINT nView			= (UINT)wParam + ID_TABVIEW_1;
	UINT nEquipIndex	= (UINT)lParam;	

	OnTabView(nView);	

	return TRUE;
}

//=============================================================================
// Method		: OnWM_PermissionMode
// Access		: protected  
// Returns		: LRESULT
// Parameter	: WPARAM wParam
// Parameter	: LPARAM lParam
// Qualifier	:
// Last Update	: 2016/9/27 - 20:01
// Desc.		: 검사 권한 모드 변경 알림
//=============================================================================
LRESULT CMainFrame::OnWM_PermissionMode(WPARAM wParam, LPARAM lParam)
{
	enPermissionMode nAccessMode = (enPermissionMode)wParam;

	OnSet_PermissionMode(nAccessMode);

// 	if (Permission_Operator == nAccessMode)
// 	{
// 		UINT nTabz[] = {0, 1, 3}; // Operation, Socket, Log
// 		m_wndTabViewBar.ShowButton_Exclusive(nTabz, 3, FALSE);
// 
// 		m_wndCaptionBar.Enable_OptionButton(false);
// 	}
// 	else
// 	{
// 		UINT nTabz[] = { 0, 1, 2, 3, 4 }; // Operation, Socket, Setting, Log
// 		m_wndTabViewBar.ShowButton_Exclusive(nTabz, 0, TRUE);
// 
// 		if (Permission_Administrator == nAccessMode)
// 		{
// 			m_wndCaptionBar.Enable_OptionButton(true);
// 		}
// 		else
// 		{
// 			m_wndCaptionBar.Enable_OptionButton(false);
// 		}
// 	}
// 
// 	m_wndView_MainCtrl.OnSet_PermissionMode(nAccessMode);

	return TRUE;
}

//=============================================================================
// Method		: OnWM_LineCtrlCmd
// Access		: protected  
// Returns		: LRESULT
// Parameter	: WPARAM wParam
// Parameter	: LPARAM lParam
// Qualifier	:
// Last Update	: 2021/12/29 - 11:50
// Desc.		:
//=============================================================================
LRESULT CMainFrame::OnWM_LineCtrlCmd(WPARAM wParam, LPARAM lParam)
{
	m_wndView_MainCtrl.Proc_LineCtrlCmd((uint8_t)wParam);

	return 1;
}

//=============================================================================
// Method		: OnWM_EqpCtrlCmd
// Access		: protected  
// Returns		: LRESULT
// Parameter	: WPARAM wParam
// Parameter	: LPARAM lParam
// Qualifier	:
// Last Update	: 2021/12/27 - 18:24
// Desc.		:
//=============================================================================
LRESULT CMainFrame::OnWM_EqpCtrlCmd(WPARAM wParam, LPARAM lParam)
{
	m_wndView_MainCtrl.Proc_EquipmentCtrlCmd((uint8_t)wParam, (uint8_t)lParam);

	return 1;
}
#if (USE_XML)
LRESULT CMainFrame::OnSet_CONNECTION(WPARAM wParam, LPARAM lParam)
{
	m_wndView_MainCtrl.OnSet_Connection(lParam);
	return 1;
}

//=============================================================================
// Method		: REPORT_RMS_MODE
// Access		: protected  
// Returns		: LRESULT
// Parameter	: WPARAM wParam
// Parameter	: LPARAM lParam
// Qualifier	:
// Last Update	: 2023.07.10 - 19:44
// Desc.		:
//=============================================================================
LRESULT CMainFrame::OnWM_REPORT_RMS_MODE(WPARAM wParam, LPARAM lParam)
{
	enEES_Mode nEESMode = (enEES_Mode)lParam;
	OnSet_RMSMode(nEESMode);
	return 1;
}
LRESULT CMainFrame::OnWM_Svr_REQUEST_LINK_TEST(WPARAM wParam, LPARAM lParam)
{
	m_wndView_MainCtrl.Proc_ServerRequestLinkTest(wParam, lParam);
	return 1;
}
LRESULT CMainFrame::OnWM_Svr_REPLY_LINK_TEST(WPARAM wParam, LPARAM lParam)
{
	m_wndView_MainCtrl.Proc_ServerReplyLinkTest(wParam, lParam);
	return 1;
}
LRESULT CMainFrame::OnWM_Svr_REPORT_ONLINE_STATE(WPARAM wParam, LPARAM lParam)
{
	m_wndView_MainCtrl.Proc_ServerReportOnlineState((WPARAM)wParam, (LPARAM)lParam);
	return 1;
}
LRESULT CMainFrame::OnWM_Svr_REPORT_USER_CHANGE(WPARAM wParam, LPARAM lParam)
{
	m_wndView_MainCtrl.Proc_ServerReportUserChange((WPARAM)wParam, (LPARAM)lParam);
	return 1;
}
LRESULT CMainFrame::OnWM_Svr_REQUEST_USER_CHANGE(WPARAM wParam, LPARAM lParam)
{
	m_wndView_MainCtrl.Proc_ServerRequestUserChange((WPARAM)wParam, (LPARAM)lParam);
	return 1;
}
LRESULT CMainFrame::OnWM_Svr_REPLY_USER_COMMAND(WPARAM wParam, LPARAM lParam)
{
	m_wndView_MainCtrl.Proc_ServerReplyUserCommand((WPARAM)wParam, (LPARAM)lParam);
	return 1;
}
LRESULT CMainFrame::OnWM_Svr_REPORT_EQUIPMENT_STATE(WPARAM wParam, LPARAM lParam)
{
	m_wndView_MainCtrl.Proc_ServerReportEquipmentState((WPARAM)wParam, (LPARAM)lParam);
	return 1;
}
LRESULT CMainFrame::OnWM_Svr_REQUEST_EQUIPMENT_STATE_DISPLAY(WPARAM wParam, LPARAM lParam)
{
	m_wndView_MainCtrl.Proc_ServerRequestEquipmentStateDisplay((WPARAM)wParam, (LPARAM)lParam);
	return 1;
}
LRESULT CMainFrame::OnWM_Svr_REPLY_EQUIPMENT_STATE_DISPLAY(WPARAM wParam, LPARAM lParam)
{
	m_wndView_MainCtrl.Proc_ServerReplyEquipmentStateDisplay((WPARAM)wParam, (LPARAM)lParam);
	return 1;
}
LRESULT CMainFrame::OnWM_Svr_REPORT_LOSS_STATE(WPARAM wParam, LPARAM lParam)
{
	m_wndView_MainCtrl.Proc_ServerReplyLossState((WPARAM)wParam, (LPARAM)lParam);
	return 1;
}
LRESULT CMainFrame::OnWM_Svr_REQUEST_LOSS_WINDOW(WPARAM wParam, LPARAM lParam)
{
	m_wndView_MainCtrl.Proc_ServerRequestLossState((WPARAM)wParam, (LPARAM)lParam);
	return 1;
}
LRESULT CMainFrame::OnWM_Svr_REPLY_LOSS_WINDOW(WPARAM wParam, LPARAM lParam)
{
	m_wndView_MainCtrl.Proc_ServerReplyLossWindow((WPARAM)wParam, (LPARAM)lParam);
	return 1;
}
LRESULT CMainFrame::OnWM_Svr_REPORT_ALARM_STATE(WPARAM wParam, LPARAM lParam)
{
	m_wndView_MainCtrl.Proc_ServerReportAlarmState((WPARAM)wParam, (LPARAM)lParam);
	return 1;
}
LRESULT CMainFrame::OnWM_Svr_REQUEST_ALARM_LIST(WPARAM wParam, LPARAM lParam)
{
	m_wndView_MainCtrl.Proc_ServerRequestAlarmList((WPARAM)wParam, (LPARAM)lParam);
	return 1;
}
LRESULT CMainFrame::OnWM_Svr_REPLY_ALARM_LIST(WPARAM wParam, LPARAM lParam)
{
	m_wndView_MainCtrl.Proc_ServerReplyAlarmList((WPARAM)wParam, (LPARAM)lParam);
	return 1;
}
LRESULT CMainFrame::OnWM_Svr_REPORT_RMS_MODE(WPARAM wParam, LPARAM lParam)
{
	m_wndView_MainCtrl.Proc_ServerEESMode((WPARAM)wParam, (LPARAM)lParam);
	return 1;
}
LRESULT CMainFrame::OnWM_Svr_REQUEST_SET_DATETIME(WPARAM wParam, LPARAM lParam)
{
	m_wndView_MainCtrl.Proc_ServerRequestSetDateTime((WPARAM)wParam, (LPARAM)lParam);
	return 1;
}
LRESULT CMainFrame::OnWM_Svr_REPLY_SET_DATETIME(WPARAM wParam, LPARAM lParam)
{
	m_wndView_MainCtrl.Proc_ServerReplySetDateTime((WPARAM)wParam, (LPARAM)lParam);
	return 1;
}
LRESULT CMainFrame::OnWM_Svr_REQUEST_TERMINAL_MESSAGE(WPARAM wParam, LPARAM lParam)
{
	m_wndView_MainCtrl.Proc_ServerRequestTerminalMessage((WPARAM)wParam, (LPARAM)lParam);
	return 1;
}
LRESULT CMainFrame::OnWM_Svr_REPLY_TERMINAL_MESSAGE(WPARAM wParam, LPARAM lParam)
{
	m_wndView_MainCtrl.Proc_ServerReplyTerminalMessage((WPARAM)wParam, (LPARAM)lParam);
	return 1;
}
LRESULT CMainFrame::OnWM_Svr_REQUEST_OPCALL_MESSAGE(WPARAM wParam, LPARAM lParam)
{
	m_wndView_MainCtrl.Proc_ServerRequestOpCallMessage((WPARAM)wParam, (LPARAM)lParam);
	return 1;
}
LRESULT CMainFrame::OnWM_Svr_REPLY_OPCALL(WPARAM wParam, LPARAM lParam)
{
	m_wndView_MainCtrl.Proc_ServerReplyOpCall((WPARAM)wParam, (LPARAM)lParam);
	return 1;
}
#endif 

#if TEST
LRESULT CMainFrame::OnWM_Svr_UNITID_READ(WPARAM wParam, LPARAM lParam)
{
	m_wndView_MainCtrl.Proc_ServerUnitIdRead((WPARAM)wParam, (LPARAM)lParam);
	return 1;
}
LRESULT CMainFrame::OnWM_Svr_REQUEST_UNITID_READ(WPARAM wParam, LPARAM lParam)
{
	m_wndView_MainCtrl.Proc_ServerRequestUnitIdRead((WPARAM)wParam, (LPARAM)lParam);
	return 1;
}
LRESULT CMainFrame::OnWM_Svr_REPLY_UNITID_READ(WPARAM wParam, LPARAM lParam)
{
	m_wndView_MainCtrl.Proc_ServerReplytUnitIdRead((WPARAM)wParam, (LPARAM)lParam);
	return 1;
}
LRESULT CMainFrame::OnWM_Svr_REPORT_START_PROCESS(WPARAM wParam, LPARAM lParam)
{
	m_wndView_MainCtrl.Proc_ServerReportStartProcess((WPARAM)wParam, (LPARAM)lParam);
	return 1;
}
LRESULT CMainFrame::OnWM_Svr_REPORT_START_LOT(WPARAM wParam, LPARAM lParam)
{
	m_wndView_MainCtrl.Proc_ServerReportStartLot((WPARAM)wParam, (LPARAM)lParam);
	return 1;
}
LRESULT CMainFrame::OnWM_Svr_REPORT_END_EVENT(WPARAM wParam, LPARAM lParam)
{
	m_wndView_MainCtrl.Proc_ServerReportEndEvent((WPARAM)wParam, (LPARAM)lParam);
	return 1;
}
LRESULT CMainFrame::OnWM_Svr_REPORT_END_PROCESS(WPARAM wParam, LPARAM lParam)
{
	m_wndView_MainCtrl.Proc_ServerReportEndProcess((WPARAM)wParam, (LPARAM)lParam);
	return 1;
}

ST_TransactionID * CMainFrame::CreateTransactionID(CStringA cstr)
{
	CStringA szUUID;
	szUUID.Format("%s", cstr);

	ST_TransactionID * TransactionID = NULL;
	TransactionID = new ST_TransactionID();

	SetTransactionID(szUUID, TransactionID);

	return TransactionID;
}
CStringA CMainFrame::CreateTransactionID()
{
	CStringA szUUID;
	RPC_CSTR cstr;
	UUID	uuid;
	UuidCreate(&uuid);
	UuidToStringA(&uuid, &cstr);
	szUUID.Format("%s", cstr);

	ST_TransactionID *TransactionID;
	TransactionID = new ST_TransactionID();

	SetTransactionID(szUUID, TransactionID);

	return szUUID;
}
void CMainFrame::SetTransactionID(CStringA command, ST_TransactionID * data)
{
	SimpleLockPtr(m_pssaTransationIDBufferCntrMutex);

	auto res = ssaTransationIDBufferCntr->emplace(command, std::move(data));

	if (!res.second) {
		(*res.first).second = std::move(data);
	}
}
ST_TransactionID * CMainFrame::GetTransactionID(CStringA command) const
{
	SimpleLockPtr(m_pssaTransationIDBufferCntrMutex);

	auto iter = ssaTransationIDBufferCntr->find(command);

	//if (iter == ssaTransationIDBufferCntr->end())
	//	return false;

	auto data = (*iter).second;

	return data;
}

bool CMainFrame::bGetTransactionID(CStringA command) const
{
	SimpleLockPtr(m_pssaTransationIDBufferCntrMutex);

	auto iter = ssaTransationIDBufferCntr->find(command);

	if (iter == ssaTransationIDBufferCntr->end())
		return false;

	return true;
}
void CMainFrame::ClearTransactionID(CStringA command)
{
	SimpleLockPtr(m_pssaTransationIDBufferCntrMutex);

	auto iter = ssaTransationIDBufferCntr->find(command);

	if (iter != ssaTransationIDBufferCntr->end()) {		
		ST_TransactionID *TransactionID;
		TransactionID = (*iter).second;
		//auto ReportMsg = TransactionID->ReportMsg;
		//auto RequestMsg = TransactionID->RequestMsg;
		//auto ReplyMsg = TransactionID->ReplyMsg;
		//auto nSize = sizeof(RequestMsg);
		delete TransactionID;
		TransactionID = nullptr;
		//nSize = sizeof(RequestMsg);
		//TransactionID->ReplyMsg;


		ssaTransationIDBufferCntr->erase(iter);		
	}		
}

//=============================================================================
// Method		: CMainFrame::New_wnd_IdList
// Access		: public 
// Returns		: Cwnd_IdList
// Qualifier	:
// Last Update	: 2023.06.27
// Desc.		: 
//=============================================================================

void CMainFrame::AddLPARAM(LPARAM PARA)
{
	SimpleLockPtr(m_pLPARAMCntrMutex);

	m_pLPARAMCntr->emplace_back(PARA);
}
void CMainFrame::RemoveLPARAM(LPARAM PARA)
{
	SimpleLockPtr(m_pLPARAMCntrMutex);

	auto iter = std::find(m_pLPARAMCntr->begin(), m_pLPARAMCntr->end(), PARA);

	if (iter != m_pLPARAMCntr->end()){
		m_pLPARAMCntr->erase(iter);
	}
}
bool CMainFrame::FindLPARAM(LPARAM PARA)
{
	SimpleLockPtr(m_pLPARAMCntrMutex);

	auto iter = std::find(m_pLPARAMCntr->begin(), m_pLPARAMCntr->end(), PARA);

	if (iter != m_pLPARAMCntr->end()) {
		return true;
	}
	return false;
}
#endif

#if (USE_XML)
void CMainFrame::OnSet_RMSMode(__in enEES_Mode nAcessMode, __in bool bInit /*= false*/)
{
	m_wndView_MainCtrl.OnSet_EESMode(nAcessMode);
}
void CMainFrame::SetSystemTimePrivilege()
{
	LUID luid;
	TOKEN_PRIVILEGES tp;
	HANDLE hToken;
	OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);

	LookupPrivilegeValue(NULL, SE_SYSTEMTIME_NAME, &luid);
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	AdjustTokenPrivileges(hToken, FALSE, &tp, 0, NULL, NULL);
}
#endif