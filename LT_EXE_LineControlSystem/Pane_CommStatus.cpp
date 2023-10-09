//*****************************************************************************
// Filename	: 	Pane_CommStatus.cpp
// Created	:	2014/7/5 - 10:24
// Modified	:	2015/12/12 - 0:07
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
#include "stdafx.h"
#include "resource.h"
#include "Pane_CommStatus.h"
#include "CommonFunction.h"
#include "Dlg_UserLogin.h"
#include "Def_PopupMenu.h"
#include "Def_Language.h"

#if defined(EES_XML)//20231003
#include "TestManager_EQP_ICS.h"
#include "TestManager_EQP.h"

#include "Dlg_EES_RMS.h"
#include "Dlg_Server.h"
CPane_CommStatus* mPane_CommStatus;
#endif

static LPCTSTR g_szDeviceName_T[Lang_MaxCount][CPane_CommStatus::DevSI_MaxCount] =
{
	// 한국어
	{
		_T(""),				// DevSI_PermissionMode	
		_T("사용자"),		// DevSI_User	
		_T("생산"),			// DevSI_Production
#if defined(EES_XML)//20231003
		_T("RMS"),		// DevSI_Dummy_2
		_T("EES"),		// DevSI_Dummy_3
#else		
		_T("Dummy 2"),		// DevSI_Dummy_2
		_T("Dummy 3"),		// DevSI_Dummy_3
#endif
		_T("Dummy 4"),		// DevSI_Dummy_4
		_T("Dummy 5"),		// DevSI_Dummy_5
		_T("Dummy 6"),		// DevSI_Dummy_6
		_T("Dummy 7"),		// DevSI_Dummy_7
		_T("Dummy 8"),		// DevSI_Dummy_8
	},

	// 영어
	{
		_T(""),				// DevSI_PermissionMode	
		_T("User"),			// DevSI_User	
		_T("Production"),	// DevSI_Production
#if defined(EES_XML)//20231003
		_T("RMS"),		// DevSI_Dummy_2
		_T("EES"),		// DevSI_Dummy_3
#else		
		_T("Dummy 2"),		// DevSI_Dummy_2
		_T("Dummy 3"),		// DevSI_Dummy_3
#endif
		_T("Dummy 4"),		// DevSI_Dummy_4
		_T("Dummy 5"),		// DevSI_Dummy_5
		_T("Dummy 6"),		// DevSI_Dummy_6
		_T("Dummy 7"),		// DevSI_Dummy_7
		_T("Dummy 8"),		// DevSI_Dummy_8
	},

	// 베트남어
	{
		_T(""),				// DevSI_PermissionMode	
		_T("User"),			// DevSI_User	
		_T("生产"),			// DevSI_Production
#if defined(EES_XML)//20231003
		_T("RMS"),		// DevSI_Dummy_2
		_T("EES"),		// DevSI_Dummy_3
#else		
		_T("Dummy 2"),		// DevSI_Dummy_2
		_T("Dummy 3"),		// DevSI_Dummy_3
#endif
		_T("Dummy 4"),		// DevSI_Dummy_4
		_T("Dummy 5"),		// DevSI_Dummy_5
		_T("Dummy 6"),		// DevSI_Dummy_6
		_T("Dummy 7"),		// DevSI_Dummy_7
		_T("Dummy 8"),		// DevSI_Dummy_8
	},

	// 중국어
	{
		_T(""),				// DevSI_PermissionMode	
		_T("用户"),			// DevSI_User	
		_T("Production"),	// DevSI_Production
#if defined(EES_XML)//20231003
		_T("RMS"),		// DevSI_Dummy_2
		_T("EES"),		// DevSI_Dummy_3
#else		
		_T("Dummy 2"),		// DevSI_Dummy_2
		_T("Dummy 3"),		// DevSI_Dummy_3
#endif
		_T("Dummy 4"),		// DevSI_Dummy_4
		_T("Dummy 5"),		// DevSI_Dummy_5
		_T("Dummy 6"),		// DevSI_Dummy_6
		_T("Dummy 7"),		// DevSI_Dummy_7
		_T("Dummy 8"),		// DevSI_Dummy_8
	},
};

static LPCTSTR g_szBnDeviceName_T[Lang_MaxCount][CPane_CommStatus::DEV_BN_MaxCount] =
{
	// 한국어
	{
		_T("로그인"),			// DEV_BN_PermissionChange
#if defined(EES_XML)//20231003
		_T("RMS"),
		_T("EES"),
#endif		
		_T("Init"),				// DEV_BN_Init
		_T("Reset"),			// DEV_BN_Reset
		_T("Run\n(Auto)"),		// DEV_BN_Run
		_T("Stop\n(Manual)"),	// DEV_BN_Stop
		_T("Show"),				// DEV_BN_Show
		_T("Hide"),				// DEV_BN_Hide
		_T("키보드"),			// DEV_BN_Keyboard
	},

	// 영어
	{
		_T("Login"),			// DEV_BN_PermissionChange
#if defined(EES_XML)//20231003
		_T("RMS"),
		_T("EES"),
#endif		
		_T("Init"),				// DEV_BN_Init
		_T("Reset"),			// DEV_BN_Reset
		_T("Run\n(Auto)"),		// DEV_BN_Run
		_T("Stop\n(Manual)"),	// DEV_BN_Stop
		_T("Show"),				// DEV_BN_Show
		_T("Hide"),				// DEV_BN_Hide
		_T("Keyboard"),			// DEV_BN_Keyboard
	},

	// 베트남어
	{
		_T("Login"),			// DEV_BN_PermissionChange
#if defined(EES_XML)//20231003
		_T("RMS"),
		_T("EES"),
#endif				
		_T("Init"),				// DEV_BN_Init
		_T("Reset"),			// DEV_BN_Reset
		_T("Run\n(Auto)"),		// DEV_BN_Run
		_T("Stop\n(Manual)"),	// DEV_BN_Stop
		_T("Show"),				// DEV_BN_Show
		_T("Hide"),				// DEV_BN_Hide
		_T("Keyboard"),			// DEV_BN_Keyboard
	},

	// 중국어
	{
		_T("登录"),					// DEV_BN_PermissionChange
#if defined(EES_XML)//20231003
		_T("RMS"),
		_T("EES"),
#endif				
		_T("设备原点\n(Init)"),		// DEV_BN_Init
		_T("数据初始化\n(Reset)"),	// DEV_BN_Reset
		_T("启动\n(自动 Auto)"),		// DEV_BN_Run
		_T("停止\n(手动 Manual)"),	// DEV_BN_Stop
		_T("显示\n(Show)"),			// DEV_BN_Show
		_T("隐藏\n(Hide)"),			// DEV_BN_Hide
		_T("键盘"),					// DEV_BN_Keyboard
	},
};

#define		IDC_BN_TEST_S		1001
#define		IDC_BN_TEST_E		IDC_BN_TEST_S + TEST_BUTTON_CNT - 1

#define		IDC_PRG_TESTING		1020

#define		IDC_BN_DEVICE_S		1100
#define		IDC_BN_DEVICE_E		IDC_BN_DEVICE_S + DEV_BN_MaxCount - 1

#define		IDC_BN_VIDEO_S		1200
#define		IDC_BN_VIDEO_E		IDC_BN_VIDEO_S + MAX_DAQ_BOARD - 1

//=============================================================================
// CPane_CommStatus
//=============================================================================
IMPLEMENT_DYNAMIC(CPane_CommStatus, CMFCTasksPane)

//=============================================================================
//
//=============================================================================
CPane_CommStatus::CPane_CommStatus()
{
	VERIFY(m_Font.CreateFont(
		22,						// nHeight
		0,						// nWidth
		0,						// nEscapement
		0,						// nOrientation
		FW_SEMIBOLD,			// nWeight
		FALSE,					// bItalic
		FALSE,					// bUnderline
		0,						// cStrikeOut
		ANSI_CHARSET,			// nCharSet (ANSI_CHARSET -> CHINESEBIG5_CHARSET)
		OUT_DEFAULT_PRECIS,		// nOutPrecision
		CLIP_DEFAULT_PRECIS,	// nClipPrecision
		ANTIALIASED_QUALITY,	// nQuality
		DEFAULT_PITCH,			// nPitchAndFamily
		_T("Arial")));			// lpszFacename

	VERIFY(m_Font_S.CreateFont(
		16,						// nHeight
		0,						// nWidth
		0,						// nEscapement
		0,						// nOrientation
		FW_BOLD,				// nWeight
		FALSE,					// bItalic
		FALSE,					// bUnderline
		0,						// cStrikeOut
		ANSI_CHARSET,			// nCharSet (ANSI_CHARSET -> CHINESEBIG5_CHARSET)
		OUT_DEFAULT_PRECIS,		// nOutPrecision
		CLIP_DEFAULT_PRECIS,	// nClipPrecision
		ANTIALIASED_QUALITY,	// nQuality
		FIXED_PITCH,			// nPitchAndFamily
		_T("Arial")));			// lpszFacename
#if defined(EES_XML)//20231003
	mPane_CommStatus = this;
#endif		
}

//=============================================================================
//
//=============================================================================
CPane_CommStatus::~CPane_CommStatus()
{
	TRACE(_T("<<< Start ~CPane_CommStatus >>> \n"));

	m_Font.DeleteObject();
	m_Font_S.DeleteObject();

	TRACE(_T("<<< End ~CPane_CommStatus >>> \n"));
}


BEGIN_MESSAGE_MAP(CPane_CommStatus, CMFCTasksPane)
	ON_WM_CREATE()	
	ON_WM_SIZE()
	ON_COMMAND_RANGE			(IDC_BN_TEST_S, IDC_BN_TEST_E,		OnBnClickedTest)
	ON_UPDATE_COMMAND_UI_RANGE	(IDC_BN_TEST_S, IDC_BN_TEST_E,		OnUpdateCmdUI_Test)
	ON_COMMAND_RANGE			(IDC_BN_DEVICE_S, IDC_BN_DEVICE_E,	OnBnClicked_Dev)
	ON_UPDATE_COMMAND_UI_RANGE	(IDC_BN_DEVICE_S, IDC_BN_DEVICE_E,	OnUpdateCmdUI_Dev)
END_MESSAGE_MAP()

//=============================================================================
// CPane_CommStatus 메시지 처리기입니다.
//=============================================================================

//=============================================================================
// Method		: CPane_CommStatus::OnCreate
// Access		: public 
// Returns		: int
// Parameter	: LPCREATESTRUCT lpCreateStruct
// Qualifier	:
// Last Update	: 2015/12/12 - 0:07
// Desc.		:
//=============================================================================
int CPane_CommStatus::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMFCTasksPane::OnCreate(lpCreateStruct) == -1)
		return -1;

	DWORD dwStyle = WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN;
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	for (UINT nIdx = 0; nIdx < DevSI_MaxCount; nIdx++)
	{
		m_st_Device[nIdx].SetStaticStyle(CVGStatic::StaticStyle_Title);
		m_st_Device[nIdx].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_st_Device[nIdx].SetFont_Gdip(L"Arial", 8.0F);
		m_st_Device[nIdx].Create(g_szDeviceName_T[m_nLanguage][nIdx], dwStyle, rectDummy, this, IDC_STATIC);
	}

	for (UINT nIdx = 0; nIdx < DEV_BN_MaxCount; nIdx++)
	{
		m_bn_Device[nIdx].Create(g_szBnDeviceName_T[m_nLanguage][nIdx], dwStyle | BS_PUSHBUTTON, rectDummy, this, IDC_BN_DEVICE_S + nIdx);
		m_bn_Device[nIdx].SetFont(&m_Font);
	}
#if defined(EES_XML)//20231003
	m_st_RMS.SetStaticStyle(CVGStatic::StaticStyle_Title);
	m_st_RMS.SetColorStyle(CVGStatic::ColorStyle_DarkGray);
	m_st_RMS.SetFont_Gdip(L"Arial", 8.0F);
	m_st_RMS.Create(g_sEES_Mode_UI[EES_OFFLINE], dwStyle, rectDummy, this, IDC_STATIC);
	m_st_EES.SetStaticStyle(CVGStatic::StaticStyle_Title);
	m_st_EES.SetColorStyle(CVGStatic::ColorStyle_DarkGray);
	m_st_EES.SetFont_Gdip(L"Arial", 8.0F);
	m_st_EES.Create(g_szOnLine_StateUI[ONLINESTATE_OFFLINE], dwStyle, rectDummy, this, IDC_STATIC);
	m_bn_Device[DEV_BN_RMS]		.SetTextHotColor(RGB(255, 192, 0));
	m_bn_Device[DEV_BN_SERVER]	.SetTextHotColor(RGB(255, 192, 0));
#endif 

	m_bn_Device[DEV_BN_Init]	.SetTextHotColor(RGB(255, 192, 0));
	m_bn_Device[DEV_BN_Reset]	.SetTextHotColor(RGB(255, 192, 0));
	m_bn_Device[DEV_BN_Run]		.SetTextHotColor(RGB(0, 192, 0));
	m_bn_Device[DEV_BN_Stop]	.SetTextHotColor(RGB(255, 0, 0));
	m_bn_Device[DEV_BN_Show]	.SetTextHotColor(RGB(255, 50, 255));
	m_bn_Device[DEV_BN_Hide]	.SetTextHotColor(RGB(255, 50, 255));
	m_bn_Device[DEV_BN_Keyboard].SetFont(&m_Font_S);
	HICON DSN_ICON = (HICON)::LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_KEYBOARD), IMAGE_ICON, 32, 32, 0);
	m_bn_Device[DEV_BN_Keyboard].SetImage(DSN_ICON);
	
	static LPCTSTR m_szBnTestName[] =
	{
		_T("Test 01"),
		_T("Test 02"),
		_T("Test 03"),
		_T("Test 04"),
		_T("Test 05"),
		_T("Test 06"),
		NULL
	};


	for (int iCnt = 0; iCnt < TEST_BUTTON_CNT; iCnt++)
	{		
		m_bn_Test[iCnt].Create(m_szBnTestName[iCnt], WS_VISIBLE | WS_CHILD | BS_CENTER , CRect (0, 0, 0, 0), this, IDC_BN_TEST_S + iCnt);
		m_bn_Test[iCnt].SetFont(&m_Font_S);
	}
	
	

	//HINSTANCE hInstance = AfxFindResourceHandle (MAKEINTRESOURCE (nID), RT_GROUP_ICON);
	//HICON hIcon = (HICON)::LoadImage (hInstance, MAKEINTRESOURCE (nID), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
	
	// Pane 옵션 설정
	EnableGroupCollapse (FALSE);
	EnableWrapLabels (TRUE);
	EnableOffsetCustomControls (FALSE);

	// 그룹 추가
	Add_PermissionMode();
#if defined(EES_XML)//20231003
	Add_TCPIP();
#endif
	Add_Command();

#ifdef _DEBUG
	Add_TestCtrl();
#endif
	
	//Add_Utilities();
	Add_SystemInfo ();	

	// 초기값
#ifndef _DEBUG
	SetStatus_PermissionMode(enPermissionMode::Permission_Operator);
#endif

	return 0;
}

//=============================================================================
// Method		: OnSize
// Access		: public  
// Returns		: void
// Parameter	: UINT nType
// Parameter	: int cx
// Parameter	: int cy
// Qualifier	:
// Last Update	: 2021/3/31 - 14:20
// Desc.		:
//=============================================================================
void CPane_CommStatus::OnSize(UINT nType, int cx, int cy)
{
	CMFCTasksPane::OnSize(nType, cx, cy);

	for (auto nIdx = 0; nIdx < DevSI_MaxCount; ++nIdx)
	{
		m_st_Device[nIdx].RedrawWindow();
	}
}

//=============================================================================
// Method		: OnLanguage
// Access		: public  
// Returns		: void
// Parameter	: uint8_t IN_nLanguage
// Qualifier	:
// Last Update	: 2022/7/20 - 15:16
// Desc.		:
//=============================================================================
void CPane_CommStatus::OnLanguage(uint8_t IN_nLanguage)
{
	if (m_nLanguage != IN_nLanguage)
	{
		m_nLanguage = IN_nLanguage;

		// CharSet 변경
		LOGFONT lf;
		m_Font.GetLogFont(&lf);
		lf.lfCharSet = Get_CharSet(m_nLanguage);
		m_Font.DeleteObject();
		m_Font.CreateFontIndirect(&lf);

		LOGFONT lf_s;
		m_Font_S.GetLogFont(&lf_s);
		lf_s.lfCharSet = Get_CharSet(m_nLanguage);
		m_Font_S.DeleteObject();
		m_Font_S.CreateFontIndirect(&lf_s);

		for (UINT nIdx = 0; nIdx < DEV_BN_MaxCount; nIdx++)
		{
			m_bn_Device[nIdx].SetFont(&m_Font);
		}
		m_bn_Device[DEV_BN_Keyboard].SetFont(&m_Font_S);
		for (int iCnt = 0; iCnt < TEST_BUTTON_CNT; iCnt++)
		{
			m_bn_Test[iCnt].SetFont(&m_Font_S);
		}

		if (GetSafeHwnd())
		{
			// UI 변경
			for (UINT nIdx = 0; nIdx < DevSI_MaxCount; nIdx++)
			{
				m_st_Device[nIdx].SetText(g_szDeviceName_T[m_nLanguage][nIdx]);
			}
			
			for (UINT nIdx = 0; nIdx < DEV_BN_MaxCount; nIdx++)
			{
				m_bn_Device[nIdx].SetWindowText(g_szBnDeviceName_T[m_nLanguage][nIdx]);
			}
		}
	}
}

//=============================================================================
// Method		: Set_Width
// Access		: public  
// Returns		: void
// Parameter	: __in int IN_nWidth
// Qualifier	:
// Last Update	: 2021/3/31 - 14:25
// Desc.		:
//=============================================================================
void CPane_CommStatus::Set_Width(__in int IN_nWidth)
{
	m_nWidth = IN_nWidth;
}

//=============================================================================
// Method		: CPane_CommStatus::CalcFixedLayout
// Access		: public 
// Returns		: CSize
// Parameter	: BOOL
// Parameter	: BOOL
// Qualifier	:
// Last Update	: 2010/11/25 - 15:49
// Desc.		: Pane의 가로 크기 변경
//=============================================================================
CSize CPane_CommStatus::CalcFixedLayout (BOOL, BOOL)
{
	ASSERT_VALID(this);

	//return CSize (140, 32767);
	return CSize (m_nWidth, 32767);
}

//=============================================================================
// Method		: CPane_CommStatus::OnUpdateCmdUI_Test
// Access		: virtual protected 
// Returns		: void
// Parameter	: CCmdUI * pCmdUI
// Qualifier	:
// Last Update	: 2013/6/11 - 19:53
// Desc.		:
//=============================================================================
void CPane_CommStatus::OnUpdateCmdUI_Test( CCmdUI* pCmdUI )
{
	if ((IDC_BN_TEST_S <= pCmdUI->m_nID) && (pCmdUI->m_nID <= IDC_BN_TEST_E))
	{
		UINT nBnIndex = pCmdUI->m_nID - IDC_BN_TEST_S;

		m_bn_Test[nBnIndex].EnableWindow(TRUE);
	}
}

//=============================================================================
// Method		: CPane_CommStatus::OnBnClickedTest
// Access		: protected 
// Returns		: void
// Parameter	: UINT nID
// Qualifier	:
// Last Update	: 2013/6/11 - 19:42
// Desc.		:
//=============================================================================
void CPane_CommStatus::OnBnClickedTest(UINT nID)
{
	if ((IDC_BN_TEST_S <= nID) && (nID <= IDC_BN_TEST_E))
	{
		UINT nTestNo = nID - IDC_BN_TEST_S;

		(AfxGetApp()->GetMainWnd())->SendMessage(WM_TEST_FUNCTION, (WPARAM)nTestNo, 0);
	}
}

//=============================================================================
// Method		: OnUpdateCmdUI_Dev
// Access		: protected  
// Returns		: void
// Parameter	: CCmdUI * pCmdUI
// Qualifier	:
// Last Update	: 2016/9/21 - 18:10
// Desc.		:
//=============================================================================
void CPane_CommStatus::OnUpdateCmdUI_Dev(CCmdUI* pCmdUI)
{
	if ((IDC_BN_DEVICE_S <= pCmdUI->m_nID) && (pCmdUI->m_nID <= IDC_BN_DEVICE_E))
	{
		UINT nBnIndex = pCmdUI->m_nID - IDC_BN_DEVICE_S;

		m_bn_Device[nBnIndex].EnableWindow(TRUE);
	}
}

//=============================================================================
// Method		: OnBnClicked_Dev
// Access		: protected  
// Returns		: void
// Parameter	: UINT nID
// Qualifier	:
// Last Update	: 2016/9/21 - 18:10
// Desc.		:
//=============================================================================
void CPane_CommStatus::OnBnClicked_Dev(UINT nID)
{
	if ((IDC_BN_DEVICE_S <= nID) && (nID <= IDC_BN_DEVICE_E))
	{
		UINT nTestNo = nID - IDC_BN_DEVICE_S;

		switch (nTestNo)
		{
		case DEV_BN_LogIn:
		{			
			CDlg_UserLogin	dlgMode;
			dlgMode.OnLanguage(m_nLanguage);
			if (IDCANCEL != dlgMode.DoModal())
			{
				ST_User_Unit User = dlgMode.Get_Login_User();

				if (User.szID.IsEmpty())
				{
					SetStatus_Login((enPermissionMode)User.nAuthority, nullptr);
				}
				else
				{
					SetStatus_Login((enPermissionMode)User.nAuthority, User.szID);
				}
			}
		}
			break;
#if defined(EES_XML)//20231003
		case DEV_BN_RMS:
		{
			CDlg_EES_RMS	dlgMode;			
			if (IDCANCEL != dlgMode.DoModal())			
			{
				uint8_t EES  = dlgMode.Get_EES();				
				SetStatus_EES(EES);
			}						
		}
			break;
		case DEV_BN_SERVER:
		{
			CDlg_Server	dlg;
			dlg.Set_Wnd_Status_Server(mPane_CommStatus->m_pwndServerList);
			//dlg.Set_Wnd_Status_Server();
			if (IDCANCEL != dlg.DoModal())
			{
				//mTestManager_Device->m_pIcsServer;
			}
		}
			break;		
#endif

		case DEV_BN_Init:
			AfxGetMainWnd()->PostMessage(WM_LINE_CTRLCMD, (WPARAM)enConrolCode::CC_Init, (LPARAM)0);
			break;
		case DEV_BN_Reset:
			AfxGetMainWnd()->PostMessage(WM_LINE_CTRLCMD, (WPARAM)enConrolCode::CC_Reset, (LPARAM)0);
			break;
		case DEV_BN_Run:
			AfxGetMainWnd()->PostMessage(WM_LINE_CTRLCMD, (WPARAM)enConrolCode::CC_Run, (LPARAM)0);
			break;
		case DEV_BN_Stop:
			AfxGetMainWnd()->PostMessage(WM_LINE_CTRLCMD, (WPARAM)enConrolCode::CC_Stop, (LPARAM)0);
			break;
		case DEV_BN_Show:
			AfxGetMainWnd()->PostMessage(WM_LINE_CTRLCMD, (WPARAM)enConrolCode::CC_Show, (LPARAM)0);
			break;
		case DEV_BN_Hide:
			AfxGetMainWnd()->PostMessage(WM_LINE_CTRLCMD, (WPARAM)enConrolCode::CC_Hide, (LPARAM)0);
			break;
		case DEV_BN_Keyboard:
		{
			RunTouchKeyboard();
		}
			break;
		}
	}
}

void CPane_CommStatus::ScrollChild(HWND /*hwndTask*/, int /*nScrollValue*/)
{
	LT_MessageBox(_T("Dummy"));
}

//=============================================================================
// Method		: CPane_CommStatus::Add_SystemInfo
// Access		: protected 
// Returns		: void
// Qualifier	:
// Last Update	: 2010/11/25 - 15:49
// Desc.		:
//=============================================================================
void CPane_CommStatus::Add_SystemInfo()
{
	HICON hIcon = (HICON)LoadImage(AfxGetInstanceHandle (), MAKEINTRESOURCE (IDI_ICON_Luritech), IMAGE_ICON, 16, 16, LR_SHARED); 

	int nGroupSysInfo = AddGroup (_T("Version"), TRUE, FALSE, hIcon);
	//AddLabel (nGroupSysInfo, GetVersionInfo(_T("CompanyName")), -1, TRUE);
	
	CString strText;

	strText.Format(_T("ver: %s"), GetVersionInfo(_T("ProductVersion")));
	AddLabel (nGroupSysInfo, strText, -1, TRUE);	
	strText.Format(_T("date: %s"), GetVersionInfo(_T("FileVersion")));
	AddLabel(nGroupSysInfo, strText, -1, TRUE);

	COleDateTime now = COleDateTime::GetCurrentTime ();	
	CString strDate = now.Format (_T("Exec :%Y/%m/%d %H:%M:%S"));

	AddLabel (nGroupSysInfo, _T("Execution: "), -1, TRUE);
	strDate = now.Format (_T("%Y/%m/%d %H:%M:%S"));
	AddLabel (nGroupSysInfo, strDate, -1, TRUE);

	// 키보드
	AddSeparator(nGroupSysInfo);
	AddWindow(nGroupSysInfo, m_bn_Device[DEV_BN_Keyboard].GetSafeHwnd(), 50);
}

//=============================================================================
// Method		: Add_PermissionMode
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2016/9/21 - 17:12
// Desc.		:
//=============================================================================
void CPane_CommStatus::Add_PermissionMode()
{
	HICON hIcon = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_TSP), IMAGE_ICON, 16, 16, LR_SHARED);

	int nGroupCommStatus = AddGroup(_T("Log-In"), FALSE, TRUE, hIcon);

	//AddLabel(nGroupCommStatus, _T(""), -1, TRUE);
	AddWindow(nGroupCommStatus, m_st_Device[DevSI_User].GetSafeHwnd(), 20);
	AddWindow(nGroupCommStatus, m_st_Device[DevSI_PermissionMode].GetSafeHwnd(), 20);
	AddWindow(nGroupCommStatus, m_bn_Device[DEV_BN_LogIn].GetSafeHwnd(), 40);

}

//=============================================================================
// Method		: Add_Production
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2022/2/21 - 17:28
// Desc.		:
//=============================================================================
void CPane_CommStatus::Add_Production()
{

}

//=============================================================================
// Method		: Add_Command
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2016/3/25 - 10:36
// Desc.		:
//=============================================================================
void CPane_CommStatus::Add_Command()
{
	HICON hIcon = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_TSP), IMAGE_ICON, 16, 16, LR_SHARED);

	int nGroupCommStatus = AddGroup(_T("Command"), FALSE, TRUE, hIcon);
	//int nGroupCommStatus = AddGroup(_T(""), FALSE, TRUE);

	//AddLabel(nGroupCommStatus, _T("Control"), -1, TRUE);
	AddWindow(nGroupCommStatus, m_bn_Device[DEV_BN_Init].GetSafeHwnd(), 60);
	AddWindow(nGroupCommStatus, m_bn_Device[DEV_BN_Reset].GetSafeHwnd(), 60);

	//AddLabel(nGroupCommStatus, _T("Process"), -1, TRUE);	
	AddLabel(nGroupCommStatus, _T(""), -1, TRUE);	
	AddWindow(nGroupCommStatus, m_bn_Device[DEV_BN_Run].GetSafeHwnd(), 60);
	AddWindow(nGroupCommStatus, m_bn_Device[DEV_BN_Stop].GetSafeHwnd(), 60);

	//AddLabel(nGroupCommStatus, _T("UI"), -1, TRUE);
	AddLabel(nGroupCommStatus, _T(""), -1, TRUE);
	AddWindow(nGroupCommStatus, m_bn_Device[DEV_BN_Show].GetSafeHwnd(), 60);
	AddWindow(nGroupCommStatus, m_bn_Device[DEV_BN_Hide].GetSafeHwnd(), 60);
}

//=============================================================================
// Method		: Add_TCPIP
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2016/5/13 - 17:05
// Desc.		:
//=============================================================================
void CPane_CommStatus::Add_TCPIP()
{
	HICON hIcon = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_TSP), IMAGE_ICON, 16, 16, LR_SHARED);

#if defined(EES_XML)//20231003
	int nGroupCommStatus = AddGroup(_T("EES"), FALSE, TRUE, hIcon);
	AddWindow(nGroupCommStatus, m_st_RMS.GetSafeHwnd(), 20);
	//AddWindow(nGroupCommStatus, m_bn_Device[DEV_BN_RMS].GetSafeHwnd(), 20);
	AddWindow(nGroupCommStatus, m_st_EES.GetSafeHwnd(), 20);
	AddWindow(nGroupCommStatus, m_bn_Device[DEV_BN_SERVER].GetSafeHwnd(), 20);	
#else
	int nGroupCommStatus = AddGroup(_T("MES"), FALSE, TRUE, hIcon);
#endif
	//AddLabel(nGroupCommStatus, _T("MES"), -1, TRUE);
	//AddWindow(nGroupCommStatus, m_st_Device[DevSI_MES].GetSafeHwnd(), 20);
}

void CPane_CommStatus::Add_Dummy()
{
	HICON hIcon = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_TSP), IMAGE_ICON, 16, 16, LR_SHARED);

	int nGroupCommStatus = AddGroup(_T("Dummy"), FALSE, TRUE, hIcon);

	AddLabel(nGroupCommStatus, _T("Test"), -1, TRUE);

	for (UINT nIdx = DevSI_Dummy_2; nIdx <= DevSI_Dummy_8; nIdx++)
	{
		AddWindow(nGroupCommStatus, m_st_Device[nIdx].GetSafeHwnd(), 20);
	}
}

//=============================================================================
// Method		: Add_Utilities
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2016/10/27 - 10:08
// Desc.		:
//=============================================================================
void CPane_CommStatus::Add_Utilities()
{
	HICON hIcon = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_TSP), IMAGE_ICON, 16, 16, LR_SHARED);
	int nGroupCommStatus = AddGroup(_T("Utilities"), TRUE, FALSE, hIcon);
	//AddLabel(nGroupCommStatus, _T("Keyboard"), -1, TRUE);
	AddWindow(nGroupCommStatus, m_bn_Device[DEV_BN_Keyboard].GetSafeHwnd(), 35);
}

//=============================================================================
// Method		: CPane_CommStatus::AddMarkingStatus
// Access		: protected 
// Returns		: void
// Qualifier	:
// Last Update	: 2013/2/4 - 11:14
// Desc.		:
//=============================================================================
#ifdef USE_LOG_WND
void CPane_CommStatus::Add_WarningStatus()
{
	HICON hIcon = (HICON)LoadImage(AfxGetInstanceHandle (), MAKEINTRESOURCE (IDI_ICON_CONNECT), IMAGE_ICON, 16, 16, LR_SHARED);

	int nLogMessage = AddGroup (_T("Log 메세지"), FALSE, TRUE, hIcon);
	
	//AddWindow (nGroupPLCStatus, m_st_WarningEvent.GetSafeHwnd (), 20);
	AddWindow (nLogMessage, m_ed_WarningLog.GetSafeHwnd (), 100/*250*/);
}
#endif

//=============================================================================
// Method		: CPane_CommStatus::Add_TestCtrl
// Access		: protected 
// Returns		: void
// Qualifier	:
// Last Update	: 2013/6/11 - 19:43
// Desc.		:
//=============================================================================
void CPane_CommStatus::Add_TestCtrl()
{
	HICON hIcon = (HICON)LoadImage(AfxGetInstanceHandle (), MAKEINTRESOURCE (IDI_ICON_CONNECT), IMAGE_ICON, 16, 16, LR_SHARED);

	int nGroupTestCtrl = AddGroup (_T("TEST"), FALSE, TRUE, hIcon);

	for (UINT iCnt = 0; iCnt < TEST_BUTTON_CNT; iCnt++)
		AddWindow (nGroupTestCtrl, m_bn_Test[iCnt].GetSafeHwnd (), 25);
}

//=============================================================================
// Method		: OnSet_SystemType
// Access		: public  
// Returns		: void
// Parameter	: __in enInsptrSysType nSysType
// Qualifier	:
// Last Update	: 2016/9/21 - 20:55
// Desc.		:
//=============================================================================
void CPane_CommStatus::OnSet_SystemType(__in enInsptrSysType nSysType)
{

}

//=============================================================================
// Method		: SetStatus_PermissionMode
// Access		: public  
// Returns		: void
// Parameter	: __in enPermissionMode InspMode
// Qualifier	:
// Last Update	: 2016/9/21 - 20:56
// Desc.		:
//=============================================================================
void CPane_CommStatus::SetStatus_PermissionMode(__in enPermissionMode InspMode)
{
	m_st_Device[DevSI_PermissionMode].SetText(g_szPermissionMode[InspMode]);

	switch (InspMode)
	{
	case Permission_Operator:
		m_st_Device[DevSI_PermissionMode].SetColorStyle(CVGStatic::ColorStyle_Green);
		break;

	case Permission_Engineer:
		m_st_Device[DevSI_PermissionMode].SetColorStyle(CVGStatic::ColorStyle_Yellow);
		break;

	case Permission_Administrator:
		m_st_Device[DevSI_PermissionMode].SetColorStyle(CVGStatic::ColorStyle_Red);
		break;

	default:
		break;
	}

	int nGroup_Cmd = 1;

	switch (InspMode)
	{
	case Permission_Operator:		
		SetGroupName(nGroup_Cmd, _T(""));
		for (auto nIdx = 0; nIdx < 8; ++nIdx)
		{
			ShowTask(nGroup_Cmd, nIdx, FALSE);
		}
		break;

	case Permission_Engineer:
	case Permission_Administrator:		
		SetGroupName(nGroup_Cmd, _T("Command"));
		for (auto nIdx = 0; nIdx < 8; ++nIdx)
		{
			ShowTask(nGroup_Cmd, nIdx, TRUE);
		}
		break;

	default:
		break;
	}

}

//=============================================================================
// Method		: SetStatus_Login
// Access		: public  
// Returns		: void
// Parameter	: __in enPermissionMode IN_nAuthority
// Parameter	: __in LPCTSTR IN_szUserID
// Qualifier	:
// Last Update	: 2022/1/6 - 17:59
// Desc.		:
//=============================================================================
void CPane_CommStatus::SetStatus_Login(__in enPermissionMode IN_nAuthority, __in LPCTSTR IN_szUserID)
{
	SetStatus_PermissionMode(IN_nAuthority);

	if (nullptr != IN_szUserID)
	{
		m_st_Device[DevSI_User].SetText(IN_szUserID);
		m_st_Device[DevSI_User].SetColorStyle(CVGStatic::ColorStyle_Green);
	}
	else
	{
		m_st_Device[DevSI_User].SetText(_T(""));
		m_st_Device[DevSI_User].SetColorStyle(CVGStatic::ColorStyle_DeepDarkGray);
	}
}

//=============================================================================
// Method		: SetStatus_Production
// Access		: public  
// Returns		: void
// Parameter	: __in bool bEnd
// Qualifier	:
// Last Update	: 2022/2/21 - 17:31
// Desc.		:
//=============================================================================
void CPane_CommStatus::SetStatus_Production(__in bool bEnd)
{
	if (bEnd)
	{
		m_st_Device[DevSI_Production].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_st_Device[DevSI_Production].SetText(_T("End Production"));
	}
	else
	{
		m_st_Device[DevSI_Production].SetColorStyle(CVGStatic::ColorStyle_Green);
		m_st_Device[DevSI_Production].SetText(_T("In Production"));
	}
}

#if defined(EES_XML)//20231003
void CPane_CommStatus::SetStatus_EES(__in uint8_t IN_EES)
{
	//m_bn_Device[DEV_BN_RMS].SetWindowText((g_sEES_Mode_UI[IN_EES]));
	m_st_RMS.SetWindowText((g_sEES_Mode_UI[IN_EES]));
}
void CPane_CommStatus::EES_Test()
{
	CRect rtClient;
	GetClientRect(&rtClient);
	ClientToScreen(rtClient);	
}
#endif