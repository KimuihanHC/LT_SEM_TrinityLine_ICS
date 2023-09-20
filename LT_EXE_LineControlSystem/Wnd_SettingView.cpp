//*****************************************************************************
// Filename	: Wnd_SettingView.cpp
// Created	: 2016/03/18
// Modified	: 2016/03/18
//
// Author	: PiRing
//	
// Purpose	: 
//*****************************************************************************
#include "stdafx.h"
#include "Wnd_SettingView.h"
#include "Def_Constant.h"
#include "resource.h"
#include "CommonFunction.h"
#include "Def_Language.h"

static LPCTSTR g_szTabName_T[Lang_MaxCount][CWnd_SettingView::Tab_MaxCount] =
{
	// 한국어
	{
		_T("라인"),			//Tab_Line
		_T("소켓 지그"),		//Tab_Socket
		_T("사용자"),		//Tab_User
		_T("NG 코드"),		//Tab_NGCode
		//_T("Alarm"),		//Tab_Alarm
#ifdef USE_DEBUG_MODE
		_T("디버그"),		//Tab_DebugMode
#endif
	},

	// 영어
	{
		_T("Line"),			//Tab_Line
		_T("Socket JIG"),	//Tab_Socket
		_T("User"),			//Tab_User
		_T("NGCode"),		//Tab_NGCode
		//_T("Alarm"),		//Tab_Alarm
#ifdef USE_DEBUG_MODE
		_T("Debug"),		//Tab_DebugMode
#endif
	},

	// 베트남어
	{
		_T("Line"),			//Tab_Line
		_T("Socket JIG"),	//Tab_Socket
		_T("User"),			//Tab_User
		_T("NGCode"),		//Tab_NGCode
		//_T("Alarm"),		//Tab_Alarm
#ifdef USE_DEBUG_MODE
		_T("Debug"),		//Tab_DebugMode
#endif
	},

	// 중국어
	{
		_T("生产线"),		//Tab_Line
		_T("插座夹具"),		//Tab_Socket
		_T("用户"),			//Tab_User
		_T("NG码"),			//Tab_NGCode
		//_T("警报"),		//Tab_Alarm
#ifdef USE_DEBUG_MODE
		_T("调试"),			//Tab_DebugMode
#endif
	},
};

enum enCtrlText
{
	Txt_Keyboard,

	Txt_MaxCount,
};

static LPCTSTR g_szCtrlText_T[Lang_MaxCount][Txt_MaxCount] =
{
	// 한국어
	{
		_T(" 키보드"),		// Txt_Keyboard
	},

	// 영어
	{
		_T(" Keyboard"),	// Txt_Keyboard
	},

	// 베트남어
	{
		_T(" Keyboard"),	// Txt_Keyboard
	},

	// 중국어
	{
		_T(" 键盘"),			// Txt_Keyboard
	},
};



#define		IDC_RB_FIRST		1001
#define		IDC_RB_LAST			IDC_RB_FIRST + Tab_MaxCount - 1
#define		IDC_BN_KEYBOARD		2001

//=============================================================================
// CWnd_SettingView
//=============================================================================
IMPLEMENT_DYNAMIC(CWnd_SettingView, CWnd_BaseView)

//=============================================================================
//
//=============================================================================
CWnd_SettingView::CWnd_SettingView()
{
	VERIFY(m_font_Default.CreateFont(
		20,						// nHeight
		0,						// nWidth
		0,						// nEscapement
		0,						// nOrientation
		FW_SEMIBOLD,				// nWeight
		FALSE,					// bItalic
		FALSE,					// bUnderline
		0,						// cStrikeOut
		ANSI_CHARSET,			// nCharSet
		OUT_DEFAULT_PRECIS,		// nOutPrecision
		CLIP_DEFAULT_PRECIS,	// nClipPrecision
		ANTIALIASED_QUALITY,	// nQuality
		DEFAULT_PITCH,			// nPitchAndFamily
		_T("Arial")));			// lpszFacename

	VERIFY(m_font_Data.CreateFont(
		18,						// nHeight
		0,						// nWidth
		0,						// nEscapement
		0,						// nOrientation
		FW_BOLD,				// nWeight
		FALSE,					// bItalic
		FALSE,					// bUnderline
		0,						// cStrikeOut
		ANSI_CHARSET,			// nCharSet
		OUT_DEFAULT_PRECIS,		// nOutPrecision
		CLIP_DEFAULT_PRECIS,	// nClipPrecision
		ANTIALIASED_QUALITY,	// nQuality
		DEFAULT_PITCH,			// nPitchAndFamily
		_T("Arial")));			// lpszFacename

}

CWnd_SettingView::~CWnd_SettingView()
{
	TRACE(_T("<<< Start ~CWnd_SettingView >>> \n"));
	m_font_Default.DeleteObject();
	m_font_Data.DeleteObject();
}


BEGIN_MESSAGE_MAP(CWnd_SettingView, CWnd_BaseView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_COMMAND_RANGE(IDC_RB_FIRST, IDC_RB_LAST,	OnTabSelect)
	ON_BN_CLICKED(IDC_BN_KEYBOARD, OnBnClicked_Keyboard)
END_MESSAGE_MAP()


//=============================================================================
// CWnd_SettingView 메시지 처리기입니다.
//=============================================================================
//=============================================================================
// Method		: CWnd_SettingView::OnCreate
// Access		: protected 
// Returns		: int
// Parameter	: LPCREATESTRUCT lpCreateStruct
// Qualifier	:
// Last Update	: 2015/12/9 - 0:05
// Desc.		:
//=============================================================================
int CWnd_SettingView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd_BaseView::OnCreate(lpCreateStruct) == -1)
		return -1;

	DWORD dwStyle = WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Keyboard
	m_bn_Keyboard.Create(g_szCtrlText_T[m_nLanguage][Txt_Keyboard], dwStyle | BS_PUSHBUTTON | BS_CENTER, rectDummy, this, IDC_BN_KEYBOARD);
	m_bn_Keyboard.SetFont(&m_font_Data);

	HICON DSN_ICON = (HICON)::LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_KEYBOARD), IMAGE_ICON, 32, 32, 0);
	m_bn_Keyboard.SetImage(DSN_ICON);

	// 탭 윈도우
	UINT nWndID = 1;

	m_wnd_LineCfg.SetOwner(GetOwner());
	m_wnd_LineCfg.Create(NULL, _T("Line"), dwStyle /*| WS_BORDER*/, rectDummy, this, nWndID++);	

	m_wnd_Socket.SetOwner(GetOwner());
	m_wnd_Socket.Create(NULL, _T("Socket JIG"), dwStyle /*| WS_BORDER*/, rectDummy, this, nWndID++);
	
	m_wnd_User.SetOwner(GetOwner());
	m_wnd_User.Create(NULL, _T("User"), dwStyle /*| WS_BORDER*/, rectDummy, this, nWndID++);

	m_wnd_NGCode.SetOwner(GetOwner());
	m_wnd_NGCode.Create(NULL, _T("NGCode"), dwStyle /*| WS_BORDER*/, rectDummy, this, nWndID++);

	//m_wnd_Alarm.SetOwner(GetOwner());
	//m_wnd_Alarm.Create(NULL, _T("Alarm"), dwStyle /*| WS_BORDER*/, rectDummy, this, nWndID++);

	m_wnd_Debug.SetOwner(GetOwner());
	m_wnd_Debug.Create(NULL, _T("Debug Mode"), dwStyle /*| WS_BORDER*/, rectDummy, this, nWndID++);


	m_pwndTab.push_back(&m_wnd_LineCfg);
	m_pwndTab.push_back(&m_wnd_Socket);
	m_pwndTab.push_back(&m_wnd_User);
	m_pwndTab.push_back(&m_wnd_NGCode);
	//m_pwndTab.push_back(&m_wnd_Alarm);
#ifdef USE_DEBUG_MODE
	m_pwndTab.push_back(&m_wnd_Debug);
#endif


	for (auto nIdx = 0; nIdx < Tab_MaxCount; nIdx++)
	{
		m_rb_Tab[nIdx].m_nFlatStyle = CMFCButton::BUTTONSTYLE_SEMIFLAT;
		m_rb_Tab[nIdx].SetImage(IDB_SELECTNO_16);
		m_rb_Tab[nIdx].SetCheckedImage(IDB_SELECT_16);

		m_rb_Tab[nIdx].Create(g_szTabName_T[m_nLanguage][nIdx], dwStyle | BS_AUTORADIOBUTTON, rectDummy, this, IDC_RB_FIRST + nIdx);
		m_rb_Tab[nIdx].SetFont(&m_font_Default);
	}
	m_rb_Tab[0].ModifyStyle(0, WS_GROUP);
	m_rb_Tab[0].SetCheck(BST_CHECKED);

	m_frame_Tab.SetBackColor_COLORREF(RGB(30, 30, 30));
	m_frame_Tab.Create(_T(""), dwStyle, rectDummy, this, IDC_STATIC);


	Set_TabWindow(0);

	//OnLanguage(m_nLanguage);

	return 0;
}

//=============================================================================
// Method		: CWnd_SettingView::OnSize
// Access		: protected 
// Returns		: void
// Parameter	: UINT nType
// Parameter	: int cx
// Parameter	: int cy
// Qualifier	:
// Last Update	: 2015/12/9 - 0:05
// Desc.		:
//=============================================================================
void CWnd_SettingView::OnSize(UINT nType, int cx, int cy)
{
	CWnd_BaseView::OnSize(nType, cx, cy);

	if ((0 == cx) || (0 == cy))
		return;

	int iMagrin = 10;
	int iWidth = cx - iMagrin - iMagrin;
	int iHeight = cy - iMagrin - iMagrin;

	m_wnd_LineCfg.MoveWindow(10, 10, iWidth - m_nTabWidth, iHeight);
	m_wnd_Socket.MoveWindow(10, 10, iWidth - m_nTabWidth, iHeight);
	m_wnd_User.MoveWindow(10, 10, iWidth - m_nTabWidth, iHeight);
	m_wnd_NGCode.MoveWindow(10, 10, iWidth - m_nTabWidth, iHeight);
	//m_wnd_Alarm.MoveWindow(10, 10, iWidth - m_nTabWidth, iHeight);
	m_wnd_Debug.MoveWindow(10, 10, iWidth - m_nTabWidth, iHeight);
	

	MoveWindow_Tab(cx - m_nTabWidth, 0, m_nTabWidth, cy);

	
}

//=============================================================================
// Method		: OnShowWindow
// Access		: protected  
// Returns		: void
// Parameter	: BOOL bShow
// Parameter	: UINT nStatus
// Qualifier	:
// Last Update	: 2016/5/28 - 22:31
// Desc.		:
//=============================================================================
void CWnd_SettingView::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CWnd_BaseView::OnShowWindow(bShow, nStatus);


}

//=============================================================================
// Method		: CWnd_SettingView::PreCreateWindow
// Access		: virtual protected 
// Returns		: BOOL
// Parameter	: CREATESTRUCT & cs
// Qualifier	:
// Last Update	: 2015/12/9 - 0:05
// Desc.		:
//=============================================================================
BOOL CWnd_SettingView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1), NULL);

	return CWnd_BaseView::PreCreateWindow(cs);
}

//=============================================================================
// Method		: MoveWindow_Tab
// Access		: protected  
// Returns		: void
// Parameter	: int x
// Parameter	: int y
// Parameter	: int nWidth
// Parameter	: int nHeight
// Qualifier	:
// Last Update	: 2022/1/6 - 20:02
// Desc.		:
//=============================================================================
void CWnd_SettingView::MoveWindow_Tab(int x, int y, int nWidth, int nHeight)
{
	int nMagrin		= 10;
	int nSpacing	= 20;
	int nLeft		= x + nMagrin;
	int nTop		= 30;
	int nUnitWidth	= nWidth - ( nMagrin * 2);
	int nUnitHeight	= 60;


	m_frame_Tab.MoveWindow(x, y, nWidth, nHeight);

	for (auto nIdx = 0; nIdx < Tab_MaxCount; nIdx++)
	{
		m_rb_Tab[nIdx].MoveWindow(nLeft, nTop, nUnitWidth, nUnitHeight);

		nTop += nUnitHeight + nSpacing;
	}
	

	nTop = y + nHeight - 60;
	m_bn_Keyboard.MoveWindow(nLeft, nTop, nUnitWidth, 50);	

}

//=============================================================================
// Method		: OnTabSelect
// Access		: protected  
// Returns		: void
// Parameter	: UINT nID
// Qualifier	:
// Last Update	: 2022/1/6 - 20:25
// Desc.		:
//=============================================================================
void CWnd_SettingView::OnTabSelect(UINT nID)
{
	UINT nTabIndex = nID - IDC_RB_FIRST;

	Set_TabWindow(nTabIndex);
}

//=============================================================================
// Method		: OnBnClicked_Keyboard
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2022/1/7 - 16:18
// Desc.		:
//=============================================================================
void CWnd_SettingView::OnBnClicked_Keyboard()
{
	RunTouchKeyboard();
}

//=============================================================================
// Method		: Set_TabWindow
// Access		: protected  
// Returns		: void
// Parameter	: __in uint8_t IN_nTabIndex
// Qualifier	:
// Last Update	: 2022/1/6 - 20:22
// Desc.		:
//=============================================================================
void CWnd_SettingView::Set_TabWindow(__in uint8_t IN_nTabIndex)
{
	if (Tab_MaxCount <= m_pwndTab.size())
	{
		for (auto nIdx = 0; nIdx < Tab_MaxCount; nIdx++)
		{
			if (IN_nTabIndex != nIdx)
			{
				m_pwndTab.at(nIdx)->ShowWindow(SW_HIDE);
			}
		}

		m_pwndTab.at(IN_nTabIndex)->ShowWindow(SW_SHOW);
	}
}

//=============================================================================
// Method		: OnLanguage
// Access		: virtual public  
// Returns		: void
// Parameter	: uint8_t IN_nLanguage
// Qualifier	:
// Last Update	: 2022/7/18 - 12:27
// Desc.		:
//=============================================================================
void CWnd_SettingView::OnLanguage(uint8_t IN_nLanguage)
{
	__super::OnLanguage(IN_nLanguage);

	m_wnd_LineCfg.OnLanguage(IN_nLanguage);
	m_wnd_Socket.OnLanguage(IN_nLanguage);
	m_wnd_User.OnLanguage(IN_nLanguage);
	m_wnd_NGCode.OnLanguage(IN_nLanguage);
	//m_wnd_Alarm.OnLanguage(IN_nLanguage);
	m_wnd_Debug.OnLanguage(IN_nLanguage);

	// CharSet 변경
	LOGFONT lf_def;
	m_font_Default.GetLogFont(&lf_def);
	lf_def.lfCharSet = Get_CharSet(m_nLanguage);
	m_font_Default.DeleteObject();
	m_font_Default.CreateFontIndirect(&lf_def);

	// CharSet 변경
	LOGFONT lf;
	m_font_Data.GetLogFont(&lf);
	lf.lfCharSet = Get_CharSet(m_nLanguage);
	lf.lfHeight = (enLanguage::Lang_Chinese == m_nLanguage) ? 24 : 18;
	m_font_Data.DeleteObject();
	m_font_Data.CreateFontIndirect(&lf);

	m_bn_Keyboard.SetFont(&m_font_Data);
	for (auto nIdx = 0; nIdx < Tab_MaxCount; nIdx++)
	{
		m_rb_Tab[nIdx].SetFont(&m_font_Default);
	}

	if (GetSafeHwnd())
	{
		for (auto nIdx = 0; nIdx < Tab_MaxCount; nIdx++)
		{
			m_rb_Tab[nIdx].SetWindowText(g_szTabName_T[m_nLanguage][nIdx]);
		}

		m_bn_Keyboard.SetWindowText(g_szCtrlText_T[m_nLanguage][Txt_Keyboard]);
	}
}

//=============================================================================
// Method		: Set_PermissionMode
// Access		: public  
// Returns		: void
// Parameter	: __in enPermissionMode IN_PermissionMode
// Qualifier	:
// Last Update	: 2022/1/7 - 11:09
// Desc.		:
//=============================================================================
void CWnd_SettingView::Set_PermissionMode(__in enPermissionMode IN_PermissionMode)
{
	m_wnd_LineCfg.Set_PermissionMode(IN_PermissionMode);
	m_wnd_Socket.Set_PermissionMode(IN_PermissionMode);
	m_wnd_User.Set_PermissionMode(IN_PermissionMode);
	m_wnd_NGCode.Set_PermissionMode(IN_PermissionMode);
	//m_wnd_Alarm.Set_PermissionMode(IN_PermissionMode);
	m_wnd_Debug.Set_PermissionMode(IN_PermissionMode);
}

//=============================================================================
// Method		: SetPath
// Access		: public  
// Returns		: void
// Parameter	: __in LPCTSTR szRecipePath
// Qualifier	:
// Last Update	: 2021/11/17 - 18:07
// Desc.		:
//=============================================================================
void CWnd_SettingView::SetPath_LineInfo(__in LPCTSTR szLineInfoPath)
{
	if (NULL != szLineInfoPath)
	{
		m_szLineInfoPath = szLineInfoPath;

		CString szFullPath;
		szFullPath.Format(_T("%s\\%s"), szLineInfoPath, LINE_INFO_FILENAME);
		m_wnd_LineCfg.Set_FileFullPath(szFullPath.GetBuffer());

		// NG Code
		m_wnd_NGCode.Set_Path(szLineInfoPath);
	}
}

//=============================================================================
// Method		: SetPath_Socket
// Access		: public  
// Returns		: void
// Parameter	: __in LPCTSTR szSocketPath
// Qualifier	:
// Last Update	: 2021/12/21 - 16:48
// Desc.		:
//=============================================================================
void CWnd_SettingView::SetPath_Socket(__in LPCTSTR szSocketPath)
{
	if (NULL != szSocketPath)
	{
		m_szSocketPath = szSocketPath;

		CString szFullPath;
		szFullPath.Format(_T("%s\\%s"), szSocketPath, SOCKET_INFO_FILENAME);
		m_wnd_Socket.Set_FileFullPath(szFullPath.GetBuffer());
	}
}

void CWnd_SettingView::SetPath_Shared(LPCTSTR szSharedPath)
{
	if (NULL != szSharedPath)
	{
		// NG Code
		m_wnd_NGCode.Set_PathShared(szSharedPath);
		m_wnd_User.Set_PathShared(szSharedPath);
	}
}

//=============================================================================
// Method		: UpdateUI_LineInfo
// Access		: public  
// Returns		: void
// Parameter	: __in const CConfig_Line * IN_pConfigLine
// Qualifier	:
// Last Update	: 2022/1/5 - 19:06
// Desc.		:
//=============================================================================
void CWnd_SettingView::UpdateUI_LineInfo(__in const CConfig_Line* IN_pConfigLine)
{
	// 자체적으로 파일 불러오기 실행
	m_wnd_LineCfg.Set_Configuration();
}

void CWnd_SettingView::UpdateUI_SocketInfo()
{
	m_wnd_Socket.Set_Configuration();
}

void CWnd_SettingView::UpdateUI_UserInfo()
{

}

void CWnd_SettingView::UpdateUI_AlarmInfo()
{

}

void CWnd_SettingView::UpdateUI_NGCodeInfo()
{

}

//=============================================================================
// Method		: Init_RecipeView
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2016/6/25 - 14:37
// Desc.		:
//=============================================================================
void CWnd_SettingView::Init_RecipeView()
{
	
}
