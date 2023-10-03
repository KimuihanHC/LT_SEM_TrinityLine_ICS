//*****************************************************************************
// Filename	: Wnd_LogView.cpp
// Created	: 2010/12/2
// Modified	: 2015/12/22
//
// Author	: PiRing
//	
// Purpose	: 
//*****************************************************************************
#include "stdafx.h"
#include "Wnd_LogView.h"
#include "resource.h"
#include "CommonFunction.h"
#include "Def_Language.h"

static LPCTSTR g_szLogType[] =
{
	_T("  Equipment Alarm Log"),	// Tab_Alarm	
	_T("  Socket Load Log"),		// Tab_Load
	_T("  Socket Unload Log"),		// Tab_Unload
	_T("  Socket In, Out Count"),	// Tab_InOut_Count
};

static LPCTSTR g_szLogType_T[Lang_MaxCount][CWnd_LogView::Tab_MaxCount] =
{
	// 한국어
	{
		_T("  설비 알람 로그"),			// Tab_Alarm	
		_T("  소켓 투입 로그"),			// Tab_Load
		_T("  소켓 배출 로그"),			// Tab_Unload
#ifdef USE_IN_OUT_COUNT_UI
		_T("  투입, 배출 집계"),			// Tab_InOut_Count
#endif
	},

	// 영어
	{
		_T("  Equipment Alarm Log"),	// Tab_Alarm	
		_T("  Socket Load Log"),		// Tab_Load
		_T("  Socket Unload Log"),		// Tab_Unload
#ifdef USE_IN_OUT_COUNT_UI
		_T("  In, Out Counting"),		// Tab_InOut_Count
#endif
	},

	// 베트남어
	{
		_T("  Equipment Alarm Log"),	// Tab_Alarm	
		_T("  Socket Load Log"),		// Tab_Load
		_T("  Socket Unload Log"),		// Tab_Unload
#ifdef USE_IN_OUT_COUNT_UI
		_T("  In, Out Counting"),		// Tab_InOut_Count
#endif
	},

	// 중국어
	{
		_T("  设备报警记录"),			// Tab_Alarm	
		_T("  插口投入记录"),			// Tab_Load
		_T("  插口导出记录"),			// Tab_Unload
#ifdef USE_IN_OUT_COUNT_UI
		_T("  In, Out Counting"),		// Tab_InOut_Count
#endif
	}
};

static LPCTSTR g_szTabName_T[Lang_MaxCount][CWnd_LogView::Tab_MaxCount] =
{
	// 한국어
	{
		_T("알람"),		// Tab_Alarm
		_T("로드"),		// Tab_Load
		_T("언로드 "),	// Tab_Unload
#ifdef USE_IN_OUT_COUNT_UI
		_T("투입 집계"),	// Tab_InOut_Count
#endif
		//2023.05.01a
		_T("터미널메세지"),		// Tab_Load
		_T("오피콜"),	// Tab_Unload
	},

	// 영어
	{
		_T("Alarm"),	// Tab_Alarm
		_T("Load"),		// Tab_Load
		_T("Unload"),	// Tab_Unload
#ifdef USE_IN_OUT_COUNT_UI
		_T("In Count"),	// Tab_InOut_Count
#endif
		//2023.05.01a
		_T("Terminal Message"),		// Tab_Load
		_T("Opcall"),	// Tab_Unload
	},

	// 베트남어
	{
		_T("Alarm"),	// Tab_Alarm
		_T("Load"),		// Tab_Load
		_T("Unload"),	// Tab_Unload
#ifdef USE_IN_OUT_COUNT_UI
		_T("In Count"),	// Tab_InOut_Count
#endif
		//2023.05.01a
		_T("Terminal Message"),		// Tab_Load
		_T("Opcall"),	// Tab_Unload
	},

	// 중국어
	{
		_T("警报"),		// Tab_Alarm
		_T("投入"),		// Tab_Load
		_T("导出"),		// Tab_Unload
#ifdef USE_IN_OUT_COUNT_UI
		_T("In Count"),	// Tab_InOut_Count
#endif
		//2023.05.01a
		_T("Terminal Message"),		// Tab_Load
		_T("Opcall"),	// Tab_Unload		
	},
};

enum enLogWindow
{
	Log_All,
	Log_Error,

	Log_MaxCount,
};

static LPCTSTR g_szLogWindow_T[Lang_MaxCount][Log_MaxCount] =
{
	// 한국어
	{
		_T("  전체"),		// Log_All
		_T("  오류"),		// Log_Error
	},

	// 영어
	{
		_T("  All"),		// Log_All
		_T("  Error"),	// Log_Error
	},

	// 베트남어
	{
		_T("  All"),		// Log_All
		_T("  Error"),	// Log_Error
	},

	// 중국어
	{
		_T("  全部"),		// Log_All
		_T("  错误"),		// Log_Error
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
// CWnd_LogView
//=============================================================================
IMPLEMENT_DYNAMIC(CWnd_LogView, CWnd_BaseView)

//=============================================================================
//
//=============================================================================
CWnd_LogView::CWnd_LogView()
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
		_T("Tahoma")));			// lpszFacename

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

CWnd_LogView::~CWnd_LogView()
{
	TRACE(_T("<<< Start ~CWnd_LogView >>> \n"));
	m_font_Default.DeleteObject();
	m_font_Data.DeleteObject();
}


BEGIN_MESSAGE_MAP(CWnd_LogView, CWnd_BaseView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND_RANGE(IDC_RB_FIRST, IDC_RB_LAST, OnTabSelect)
	ON_BN_CLICKED(IDC_BN_KEYBOARD, OnBnClicked_Keyboard)
END_MESSAGE_MAP()


//=============================================================================
// CWnd_LogView 메시지 처리기입니다.
//=============================================================================
//=============================================================================
// Method		: CWnd_LogView::OnCreate
// Access		: protected 
// Returns		: int
// Parameter	: LPCREATESTRUCT lpCreateStruct
// Qualifier	:
// Last Update	: 2015/12/22
// Desc.		:
//=============================================================================
int CWnd_LogView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd_BaseView::OnCreate(lpCreateStruct) == -1)
		return -1;

	DWORD dwStyle = WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	UINT nWndID = 20;

	// Keyboard
	m_bn_Keyboard.Create(g_szCtrlText_T[m_nLanguage][Txt_Keyboard], dwStyle | BS_PUSHBUTTON | BS_CENTER, rectDummy, this, IDC_BN_KEYBOARD);
	m_bn_Keyboard.SetFont(&m_font_Data);

	HICON DSN_ICON = (HICON)::LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_KEYBOARD), IMAGE_ICON, 32, 32, 0);
	m_bn_Keyboard.SetImage(DSN_ICON);

	m_st_Log_All.Create(g_szLogWindow_T[m_nLanguage][Log_All], WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_BORDER | SS_CENTERIMAGE, rectDummy, this, nWndID++);	
	m_st_Log_Error.Create(g_szLogWindow_T[m_nLanguage][Log_Error], WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_BORDER | SS_CENTERIMAGE, rectDummy, this, nWndID++);
	m_st_Log_Tab.Create(g_szLogType_T[m_nLanguage][0], WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_BORDER | SS_CENTERIMAGE, rectDummy, this, nWndID++);
	m_st_Log_All.SetFont(&m_font_Default);
	m_st_Log_Error.SetFont(&m_font_Default);
	m_st_Log_Tab.SetFont(&m_font_Default);

	if (!m_ed_Log_All.Create(WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, rectDummy, this, nWndID++))
	{
		TRACE0("출력 창을 만들지 못했습니다.\n");
		return -1;
	}

	if (!m_ed_Log_Error.Create(WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, rectDummy, this, nWndID++))
	{
		TRACE0("출력 창을 만들지 못했습니다.\n");
		return -1;
	}

	if (!m_ed_Log_Alarm.Create(WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, rectDummy, this, nWndID++))
	{
		TRACE0("출력 창을 만들지 못했습니다.\n");
		return -1;
	}

	if (!m_ed_Log_Load.Create(WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, rectDummy, this, nWndID++))
	{
		TRACE0("출력 창을 만들지 못했습니다.\n");
		return -1;
	}

	if (!m_ed_Log_Unload.Create(WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, rectDummy, this, nWndID++))
	{
		TRACE0("출력 창을 만들지 못했습니다.\n");
		return -1;
	}

	if (!m_wnd_InOutCount.Create(NULL, _T("In Out Count"), dwStyle, rectDummy, this, nWndID++))
	{
		TRACE0("InOutCount 창을 만들지 못했습니다.\n");
		return -1;
	}
	//2023.05.01a uhkim [Log]
	if (!m_ed_Log_TerminalMessage.Create(WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, rectDummy, this, nWndID++))
	{
		TRACE0("출력 창을 만들지 못했습니다.\n");
		return -1;
	}

	if (!m_ed_Log_OpCall.Create(WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, rectDummy, this, nWndID++))
	{
		TRACE0("출력 창을 만들지 못했습니다.\n");
		return -1;
	}
	//

	UINT nTabIndex = 0;

	m_pwndTab.push_back(&m_ed_Log_Alarm);
	m_pwndTab.push_back(&m_ed_Log_Load);
	m_pwndTab.push_back(&m_ed_Log_Unload);
#ifdef USE_IN_OUT_COUNT_UI
	m_pwndTab.push_back(&m_wnd_InOutCount);
#endif
	//2023.05.01a uhkim [Log]
	m_pwndTab.push_back(&m_ed_Log_TerminalMessage);
	m_pwndTab.push_back(&m_ed_Log_OpCall);


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
// Method		: CWnd_LogView::OnSize
// Access		: protected 
// Returns		: void
// Parameter	: UINT nType
// Parameter	: int cx
// Parameter	: int cy
// Qualifier	:
// Last Update	: 2015/12/22
// Desc.		:
//=============================================================================
void CWnd_LogView::OnSize(UINT nType, int cx, int cy)
{
	CWnd_BaseView::OnSize(nType, cx, cy);

	int iMagrin			= 10;
	int iSpacing		= 5;
	int iCateSpacing	= 10;

	int iLeft		= iMagrin;
	int iTop		= iMagrin;
	int iBottom		= cy - iMagrin;
	int iWidth		= cx - iMagrin - iMagrin - m_nTabWidth;
	int iHeight		= cy - iMagrin - iMagrin;

	int iCtrlHeight = 30;
	int iHalfWidth	= (iWidth - iCateSpacing) / 2;
	int iHalfHeight	= (iHeight - (iSpacing * 3) - (iCtrlHeight * 2)) / 2;


	m_st_Log_All.MoveWindow(iLeft, iTop, iHalfWidth, iCtrlHeight);
	iTop += iCtrlHeight + iSpacing;

	m_ed_Log_All.MoveWindow(iLeft, iTop, iHalfWidth, iHalfHeight);
	iTop += iHalfHeight + iSpacing;

	m_st_Log_Error.MoveWindow(iLeft, iTop, iHalfWidth, iCtrlHeight);
	iTop += iCtrlHeight + iSpacing;

	m_ed_Log_Error.MoveWindow(iLeft, iTop, iHalfWidth, iHalfHeight);

	iLeft += iHalfWidth + iCateSpacing;
	iTop = iMagrin;

	m_st_Log_Tab.MoveWindow(iLeft, iTop, iHalfWidth, iCtrlHeight);
	iTop += iCtrlHeight + iSpacing;
	iHalfHeight = cy - iMagrin - iTop;
	m_ed_Log_Alarm.MoveWindow(iLeft, iTop, iHalfWidth, iHalfHeight);
	m_ed_Log_Load.MoveWindow(iLeft, iTop, iHalfWidth, iHalfHeight);
	m_ed_Log_Unload.MoveWindow(iLeft, iTop, iHalfWidth, iHalfHeight);

#ifdef USE_IN_OUT_COUNT_UI
	m_wnd_InOutCount.MoveWindow(iLeft, iTop, iHalfWidth, iHalfHeight);
#endif // USE_IN_OUT_COUNT_UI

	//2023.05.01a 
	m_ed_Log_TerminalMessage.MoveWindow(iLeft, iTop, iHalfWidth, iHalfHeight);
	m_ed_Log_OpCall.MoveWindow(iLeft, iTop, iHalfWidth, iHalfHeight);
	//
	MoveWindow_Tab(cx - m_nTabWidth, 0, m_nTabWidth, cy);
}

//=============================================================================
// Method		: CWnd_LogView::PreCreateWindow
// Access		: virtual protected 
// Returns		: BOOL
// Parameter	: CREATESTRUCT & cs
// Qualifier	:
// Last Update	: 2015/12/22
// Desc.		:
//=============================================================================
BOOL CWnd_LogView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

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
void CWnd_LogView::MoveWindow_Tab(int x, int y, int nWidth, int nHeight)
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
void CWnd_LogView::OnTabSelect(UINT nID)
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
void CWnd_LogView::OnBnClicked_Keyboard()
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
void CWnd_LogView::Set_TabWindow(__in uint8_t IN_nTabIndex)
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

		//m_st_Log_Tab.SetWindowText(g_szLogType[IN_nTabIndex]);
		m_st_Log_Tab.SetWindowText(g_szLogType_T[m_nLanguage][IN_nTabIndex]);
	}
}

//=============================================================================
// Method		: OnLanguage
// Access		: virtual public  
// Returns		: void
// Parameter	: uint8_t IN_nLanguage
// Qualifier	:
// Last Update	: 2022/7/18 - 12:26
// Desc.		:
//=============================================================================
void CWnd_LogView::OnLanguage(uint8_t IN_nLanguage)
{
	__super::OnLanguage(IN_nLanguage);

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
	m_st_Log_All.SetFont(&m_font_Default);
	m_st_Log_Error.SetFont(&m_font_Default);
	m_st_Log_Tab.SetFont(&m_font_Default);
		for (auto nIdx = 0; nIdx < Tab_MaxCount; nIdx++)
	{
		m_rb_Tab[nIdx].SetFont(&m_font_Default);
	}

	// 언어에 따라서 폰트 설정 변경 ??
	if (GetSafeHwnd())
	{
		m_st_Log_All.SetWindowText(g_szLogWindow_T[m_nLanguage][Log_All]);
		m_st_Log_Error.SetWindowText(g_szLogWindow_T[m_nLanguage][Log_Error]);

		for (auto nIdx = 0; nIdx < Tab_MaxCount; nIdx++)
		{
			m_rb_Tab[nIdx].SetWindowText(g_szTabName_T[m_nLanguage][nIdx]);

			if (BST_CHECKED == m_rb_Tab[nIdx].GetCheck())
			{
				m_st_Log_Tab.SetWindowText(g_szLogType_T[m_nLanguage][nIdx]);
			}
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
// Last Update	: 2023/3/15 - 16:50
// Desc.		:
//=============================================================================
void CWnd_LogView::Set_PermissionMode(__in enPermissionMode IN_PermissionMode)
{
	switch (IN_PermissionMode)
	{
	case enPermissionMode::Permission_Operator:
	case enPermissionMode::Permission_Engineer:
	{
#ifdef USE_IN_OUT_COUNT_UI
		// 버튼 감추기	
		m_rb_Tab[Tab_InOut_Count].ShowWindow(SW_HIDE);

		// Log UI 바꾸기
		if (BST_CHECKED == m_rb_Tab[Tab_InOut_Count].GetCheck())
		{
			m_rb_Tab[Tab_InOut_Count].SetCheck(BST_CHECKED);
			OnTabSelect(IDC_RB_FIRST);
		}
#endif
	}
	break;

	case enPermissionMode::Permission_Administrator:
	{
#ifdef USE_IN_OUT_COUNT_UI
		// 버튼 보이기
		m_rb_Tab[Tab_InOut_Count].ShowWindow(SW_SHOW);
#endif
	}
	break;

	default:
		break;
	}
}

//=============================================================================
// Method		: OnAdd_Log
// Access		: public  
// Returns		: void
// Parameter	: LPCTSTR lpszLog
// Parameter	: BOOL bError
// Parameter	: UINT nLogType
// Parameter	: COLORREF clrTextColor
// Qualifier	:
// Last Update	: 2015/12/22 - 18:48
// Desc.		:
//=============================================================================
void CWnd_LogView::OnAdd_Log(LPCTSTR lpszLog, BOOL bError /*= FALSE*/, UINT nLogType /*= 0*/, COLORREF clrTextColor /*= RGB(0, 0, 0)*/)
{
	m_ed_Log_All.AddText(lpszLog, clrTextColor);

	if (bError)
	{
		m_ed_Log_Error.AddText(lpszLog, clrTextColor);
	}
}

void CWnd_LogView::OnAdd_Log(LPCTSTR lpszLog, UINT nLogType)
{
	m_ed_Log_All.AddText(lpszLog, clrLoggerColor[nLogType]);

	if (LOGTYPE_ERROR == nLogType)
	{
		m_ed_Log_Error.AddText(lpszLog, clrLoggerColor[LOGTYPE_NONE]);
	}
}

//=============================================================================
// Method		: Refresh_UI
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2022/2/16 - 17:54
// Desc.		:
//=============================================================================
void CWnd_LogView::Refresh_UI()
{
	m_ed_Log_All.SetSel(0, -1);
	m_ed_Log_All.SetSel(-1, -1);

	m_ed_Log_Error.SetSel(0, -1);
	m_ed_Log_Error.SetSel(-1, -1);
}

//=============================================================================
// Method		: Log_EqpAlarm
// Access		: public  
// Returns		: void
// Parameter	: LPCTSTR lpszLog
// Parameter	: bool bError
// Qualifier	:
// Last Update	: 2022/7/19 - 10:27
// Desc.		:
//=============================================================================
void CWnd_LogView::Log_EqpAlarm(LPCTSTR lpszLog, bool bError)
{
	if (bError)
	{
		m_ed_Log_Alarm.AddText(lpszLog, clrLoggerColor[LOGTYPE_NONE]);
	}
	else
	{
		m_ed_Log_Alarm.AddText(lpszLog, clrLoggerColor[LOGTYPE_ERROR]);
	}
}

void CWnd_LogView::Log_LoadSocket(LPCTSTR lpszLog, bool bError)
{
	if (bError)
	{
		m_ed_Log_Load.AddText(lpszLog, clrLoggerColor[LOGTYPE_NONE]);
	}
	else
	{
		m_ed_Log_Load.AddText(lpszLog, clrLoggerColor[LOGTYPE_ERROR]);
	}
}

void CWnd_LogView::Log_UnloadSocket(LPCTSTR lpszLog, bool bError)
{
	if (bError)
	{
		m_ed_Log_Unload.AddText(lpszLog, clrLoggerColor[LOGTYPE_NONE]);
	}
	else
	{
		m_ed_Log_Unload.AddText(lpszLog, clrLoggerColor[LOGTYPE_ERROR]);
	}
}

void CWnd_LogView::Log_NGInfo(__in LPCTSTR lpszLog)
{
	// m_ed_Log_NGInfo.AddText(lpszLog, clrLoggerColor[LOGTYPE_NONE]);
}

//=============================================================================
// Method		: Set_InOutCount
// Access		: public  
// Returns		: void
// Parameter	: __in const ST_InOutCount * IN_pCount
// Qualifier	:
// Last Update	: 2023/3/15 - 16:40
// Desc.		:
//=============================================================================
void CWnd_LogView::Set_InOutCount(__in const ST_InOutCount* IN_pCount)
{
	m_wnd_InOutCount.Update_CountInfo(IN_pCount);
}

//=============================================================================
// Method		: Log_TerminalMessage
// Access		: public  
// Returns		: void
// Parameter	: LPCTSTR lpszLog
// Parameter	: bool bError
// Qualifier	:
// Last Update	: 2023.05.02
// Desc.		:
//=============================================================================
void CWnd_LogView::Log_TerminalMessage(__in LPCTSTR lpszLog)
{
	m_ed_Log_TerminalMessage.AddText(lpszLog, clrLoggerColor[LOGTYPE_NONE]);
}
void CWnd_LogView::Log_OpCall(__in LPCTSTR lpszLog)
{
	m_ed_Log_OpCall.AddText(lpszLog, clrLoggerColor[LOGTYPE_NONE]);
}