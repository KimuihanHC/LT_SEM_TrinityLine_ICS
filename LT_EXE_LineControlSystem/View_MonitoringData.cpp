//*****************************************************************************
// Filename	: 	View_MonitoringData.cpp
// Created	:	2021/12/22 - 10:39
// Modified	:	2021/12/22 - 10:39
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
// Wnd_SocketView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "View_MonitoringData.h"
#include "resource.h"
#include "Def_Language.h"

enum enCtrlText
{
	Txt_Type,
	Txt_Find,
	Txt_Find_Bn,
	Txt_ResetYield,
	Txt_ResetYield_All,
	Txt_Init_Data,
	Txt_Init_Data_All,
	Txt_SaveYield,

	Txt_MaxCount,
};

static LPCTSTR g_szTabName_T[Lang_MaxCount][Txt_MaxCount] =
{
	// 한국어
	{
		_T("소켓 현황"),		// Tab_Socket
		_T("NG 현황"),		// Tab_FailInfo
	},

	// 영어
	{
		_T("Socket"),		// Tab_Socket
		_T("NG Count"),		// Tab_FailInfo
	},

	// 베트남어
	{
		_T("Socket"),		// Tab_Socket
		_T("NG Count"),		// Tab_FailInfo
	},

	// 중국어
	{
		_T("插座夹具"),		// Tab_Socket
		_T("NG计数"),		// Tab_FailInfo
	},
};


#define		IDC_RB_FIRST		1001
#define		IDC_RB_LAST			IDC_RB_FIRST + Tab_MaxCount - 1

//=============================================================================
// CView_MonitoringData
//=============================================================================
IMPLEMENT_DYNAMIC(CView_MonitoringData, CWnd/*_BaseView*/)

CView_MonitoringData::CView_MonitoringData()
{
	VERIFY(m_font_Default.CreateFont(
		20,						// nHeight
		0,						// nWidth
		0,						// nEscapement
		0,						// nOrientation
		FW_SEMIBOLD,			// nWeight
		FALSE,					// bItalic
		FALSE,					// bUnderline
		0,						// cStrikeOut
		ANSI_CHARSET,			// nCharSet
		OUT_DEFAULT_PRECIS,		// nOutPrecision
		CLIP_DEFAULT_PRECIS,	// nClipPrecision
		ANTIALIASED_QUALITY,	// nQuality
		DEFAULT_PITCH,			// nPitchAndFamily
		_T("Arial")));			// lpszFacename
	
	VERIFY(m_Font.CreateFont(
		28,						// nHeight
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

	m_InspMode		= Permission_Operator;
}

CView_MonitoringData::~CView_MonitoringData()
{
	TRACE(_T("<<< Start ~CView_MonitoringData >>> \n"));
	
	m_font_Default.DeleteObject();
	m_Font.DeleteObject();
}

BEGIN_MESSAGE_MAP(CView_MonitoringData, CWnd/*_BaseView*/)
	ON_WM_CREATE()
	ON_WM_SIZE	()
	ON_WM_GETMINMAXINFO()
	ON_COMMAND_RANGE(IDC_RB_FIRST, IDC_RB_LAST, OnTabSelect)
END_MESSAGE_MAP()


//=============================================================================
// CView_MonitoringData 메시지 처리기입니다.
//=============================================================================
//=============================================================================
// Method		: OnCreate
// Access		: protected  
// Returns		: int
// Parameter	: LPCREATESTRUCT lpCreateStruct
// Qualifier	:
// Last Update	: 2021/12/22 - 10:39
// Desc.		:
//=============================================================================
int CView_MonitoringData::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	//SetBackgroundColor(RGB(0, 0, 0));
	// 현재 설정된 스타일을 얻는다.
	DWORD old_style = GetClassLong(m_hWnd, GCL_STYLE);
	SetClassLong(m_hWnd, GCL_STYLE, old_style | CS_NOCLOSE);

	SetWindowText(_T("Socket Monitoring"));


	DWORD dwStyle = WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	CRect rectDummy;
	rectDummy.SetRectEmpty();
	
	/*m_Font.CreateStockObject(DEFAULT_GUI_FONT);*/

	UINT nWindowsID = 100;

	m_wndSocket.Create(NULL, _T("Socket Monitoring"), dwStyle, rectDummy, this, nWindowsID++);
	m_wndFailInfo.Create(NULL, _T("NG Code Count"), dwStyle, rectDummy, this, nWindowsID++);

	// 검사 결과
	m_wndJudge.Create(NULL, _T("Test Result"), dwStyle, rectDummy, this, nWindowsID++);

	// 탭 윈도우
	UINT nTabIndex = 0;

	m_pwndTab.push_back(&m_wndSocket);
	m_pwndTab.push_back(&m_wndFailInfo);

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


	return 0;
}

//=============================================================================
// Method		: OnSize
// Access		: protected  
// Returns		: void
// Parameter	: UINT nType
// Parameter	: int cx
// Parameter	: int cy
// Qualifier	:
// Last Update	: 2021/12/22 - 10:39
// Desc.		:
//=============================================================================
void CView_MonitoringData::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	if ((cx == 0) && (cy == 0))
		return;

	int iMargin			= 5;
 	int iSpacing		= 5;
 	int iCateSpacing	= 10;

	int iLeft			= iMargin;
	int iTop			= iMargin;
	int iWidth			= cx - iMargin - iMargin;
	int iHeight			= cy - iMargin - iMargin;	
	int iCtrlWidth		= 200;
	int iCtrlHeight		= 35;
	int iJudgeHeight	= 300;

	//-------------------------------------------
	// 모니터링 데이터
	//-------------------------------------------
	MoveWindow_Tab(0, 0, cx, m_nTabHeight);

	iHeight = iHeight - iJudgeHeight - iSpacing - m_nTabHeight - iSpacing;

	iTop += m_nTabHeight + iSpacing;
	m_wndSocket.MoveWindow(iLeft, iTop, iWidth, iHeight);
	m_wndFailInfo.MoveWindow(iLeft, iTop, iWidth, iHeight);
	
	//-------------------------------------------
	// 소켓 결과 표시
	//-------------------------------------------
	iLeft	= iMargin;
	iTop	= cy - iMargin - iJudgeHeight;
	iWidth	= cx - iMargin - iMargin;
	m_wndJudge.MoveWindow(iLeft, iTop, iWidth, iJudgeHeight);

}

//=============================================================================
// Method		: OnEraseBkgnd
// Access		: protected  
// Returns		: BOOL
// Parameter	: CDC * pDC
// Qualifier	:
// Last Update	: 2022/7/20 - 14:45
// Desc.		:
//=============================================================================
BOOL CView_MonitoringData::OnEraseBkgnd(CDC * pDC)
{
	if (m_brBkgr.GetSafeHandle() == NULL)
	{
		return CWnd::OnEraseBkgnd(pDC);
	}

	ASSERT_VALID(pDC);

	CRect rectClient;
	GetClientRect(rectClient);

	if (m_brBkgr.GetSafeHandle() != NULL)
	{
		pDC->FillRect(rectClient, &m_brBkgr);
	}
	else
	{
		CWnd::OnEraseBkgnd(pDC);
	}

	return TRUE;
}

//=============================================================================
// Method		: OnCtlColor
// Access		: protected  
// Returns		: HBRUSH
// Parameter	: CDC * pDC
// Parameter	: CWnd * pWnd
// Parameter	: UINT nCtlColor
// Qualifier	:
// Last Update	: 2022/7/20 - 14:45
// Desc.		:
//=============================================================================
HBRUSH CView_MonitoringData::OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor)
{
	if (m_brBkgr.GetSafeHandle() != NULL)
	{
#define AFX_MAX_CLASS_NAME 255
#define AFX_STATIC_CLASS _T("Static")
#define AFX_BUTTON_CLASS _T("Button")

		if (nCtlColor == CTLCOLOR_STATIC)
		{
			TCHAR lpszClassName[AFX_MAX_CLASS_NAME + 1];

			::GetClassName(pWnd->GetSafeHwnd(), lpszClassName, AFX_MAX_CLASS_NAME);
			CString strClass = lpszClassName;

			if (strClass == AFX_STATIC_CLASS)
			{
				pDC->SetBkMode(TRANSPARENT);
				return(HBRUSH) ::GetStockObject(HOLLOW_BRUSH);
			}

			if (strClass == AFX_BUTTON_CLASS)
			{
				if ((pWnd->GetStyle() & BS_GROUPBOX) == 0)
				{
					pDC->SetBkMode(TRANSPARENT);
				}

				return(HBRUSH) ::GetStockObject(HOLLOW_BRUSH);
			}
		}
	}

	return CWnd::OnCtlColor(pDC, pWnd, nCtlColor);
}

//=============================================================================
// Method		: OnClose
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2022/2/15 - 16:40
// Desc.		:
//=============================================================================
void CView_MonitoringData::OnClose()
{
	ShowWindow(SW_HIDE);
	return;
	__super::OnClose();
}

//=============================================================================
// Method		: OnGetMinMaxInfo
// Access		: protected  
// Returns		: void
// Parameter	: MINMAXINFO * lpMMI
// Qualifier	:
// Last Update	: 2022/2/15 - 16:39
// Desc.		:
//=============================================================================
void CView_MonitoringData::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	__super::OnGetMinMaxInfo(lpMMI);

	lpMMI->ptMinTrackSize.x = 1920;
	lpMMI->ptMinTrackSize.y = 1080;

	lpMMI->ptMaxTrackSize.x = 2560;
	lpMMI->ptMaxTrackSize.y = 1600;
}

//=============================================================================
// Method		: PreCreateWindow
// Access		: virtual protected  
// Returns		: BOOL
// Parameter	: CREATESTRUCT & cs
// Qualifier	:
// Last Update	: 2022/2/15 - 17:07
// Desc.		:
//=============================================================================
BOOL CView_MonitoringData::PreCreateWindow(CREATESTRUCT& cs)
{
	return __super::PreCreateWindow(cs);

	// WS_MINIMIZEBOX
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
// Last Update	: 2022/7/20 - 14:28
// Desc.		:
//=============================================================================
void CView_MonitoringData::MoveWindow_Tab(int x, int y, int nWidth, int nHeight)
{
	int nMagrin		= 10;
	int nSpacing	= 20;
	int nLeft		= x + nMagrin;
	int nTop		= y + nMagrin;;
	int nUnitWidth	= 240;
	int nUnitHeight = 30;

	m_frame_Tab.MoveWindow(x, y, nWidth, nHeight);

	for (auto nIdx = 0; nIdx < Tab_MaxCount; nIdx++)
	{
		m_rb_Tab[nIdx].MoveWindow(nLeft, nTop, nUnitWidth, nUnitHeight);

		nLeft += nUnitWidth + nSpacing;
	}
}

//=============================================================================
// Method		: OnTabSelect
// Access		: protected  
// Returns		: void
// Parameter	: UINT nID
// Qualifier	:
// Last Update	: 2022/7/20 - 14:28
// Desc.		:
//=============================================================================
void CView_MonitoringData::OnTabSelect(UINT nID)
{
	UINT nTabIndex = nID - IDC_RB_FIRST;

	Set_TabWindow(nTabIndex);
}

//=============================================================================
// Method		: Set_TabWindow
// Access		: protected  
// Returns		: void
// Parameter	: uint8_t IN_nTabIndex
// Qualifier	:
// Last Update	: 2022/7/20 - 14:28
// Desc.		:
//=============================================================================
void CView_MonitoringData::Set_TabWindow(uint8_t IN_nTabIndex)
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
// Last Update	: 2022/7/20 - 14:27
// Desc.		:
//=============================================================================
void CView_MonitoringData::OnLanguage(uint8_t IN_nLanguage)
{
	if (m_nLanguage != IN_nLanguage)
	{
		m_nLanguage = IN_nLanguage;

		// CharSet 변경
		LOGFONT lf;
		m_font_Default.GetLogFont(&lf);
		lf.lfCharSet = Get_CharSet(m_nLanguage);
		m_font_Default.DeleteObject();
		m_font_Default.CreateFontIndirect(&lf);

		for (auto nIdx = 0; nIdx < Tab_MaxCount; nIdx++)
		{
			m_rb_Tab[nIdx].SetFont(&m_font_Default);
		}

		// UI 변경
		m_wndSocket.OnLanguage(m_nLanguage);
		m_wndFailInfo.OnLanguage(m_nLanguage);
		m_wndJudge.OnLanguage(m_nLanguage);

		if (GetSafeHwnd())
		{
			for (auto nIdx = 0; nIdx < Tab_MaxCount; nIdx++)
			{
				m_rb_Tab[nIdx].SetWindowText(g_szTabName_T[m_nLanguage][nIdx]);
			}
		}
	}
}

//=============================================================================
// Method		: SetBackgroundColor
// Access		: public  
// Returns		: void
// Parameter	: COLORREF color
// Parameter	: BOOL bRepaint
// Qualifier	:
// Last Update	: 2022/7/20 - 14:27
// Desc.		:
//=============================================================================
void CView_MonitoringData::SetBackgroundColor(COLORREF color, BOOL bRepaint)
{
	if (m_brBkgr.GetSafeHandle() != NULL)
	{
		m_brBkgr.DeleteObject();
	}

	if (color != (COLORREF)-1)
	{
		m_brBkgr.CreateSolidBrush(color);
	}

	if (bRepaint && GetSafeHwnd() != NULL)
	{
		Invalidate();
		UpdateWindow();
	}
}

//=============================================================================
// Method		: SetPtr_SocketInfo
// Access		: public  
// Returns		: void
// Parameter	: CSocketMonitoring * IN_pstSocketInfo
// Qualifier	:
// Last Update	: 2022/7/20 - 14:28
// Desc.		:
//=============================================================================
void CView_MonitoringData::SetPtr_SocketInfo(CSocketMonitoring * IN_pstSocketInfo)
{
	m_wndSocket.SetPtr_SocketInfo(IN_pstSocketInfo);
}

//=============================================================================
// Method		: Set_SocketType
// Access		: public  
// Returns		: void
// Parameter	: uint8_t IN_nSocketType
// Qualifier	:
// Last Update	: 2022/7/20 - 14:29
// Desc.		:
//=============================================================================
void CView_MonitoringData::Set_SocketType(uint8_t IN_nSocketType)
{
	m_wndSocket.Set_SocketType(IN_nSocketType);
}

//=============================================================================
// Method		: Set_PermissionMode
// Access		: public  
// Returns		: void
// Parameter	: enPermissionMode InspMode
// Qualifier	:
// Last Update	: 2021/12/22 - 10:39
// Desc.		:
//=============================================================================
void CView_MonitoringData::Set_PermissionMode(enPermissionMode InspMode)
{
	m_wndSocket.Set_PermissionMode(InspMode);
	m_wndFailInfo.Set_PermissionMode(InspMode);

	switch (InspMode)
	{
	case Permission_Operator:
		m_InspMode = Permission_Operator;
		break;

	case Permission_Engineer:
	case Permission_Administrator:
		m_InspMode = Permission_Administrator;
		break;

	default:
		break;
	}
}

//=============================================================================
// Method		: Update_SocketInfo
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/12/22 - 10:39
// Desc.		:
//=============================================================================
void CView_MonitoringData::Update_SocketInfo()
{
	m_wndSocket.Update_SocketInfo();
}

//=============================================================================
// Method		: Update_SocketStatus
// Access		: public  
// Returns		: void
// Parameter	: __in LPCTSTR IN_szRFID
// Qualifier	:
// Last Update	: 2021/12/22 - 11:46
// Desc.		:
//=============================================================================
void CView_MonitoringData::Update_SocketStatus(__in LPCTSTR IN_szRFID)
{
	m_wndSocket.Update_SocketStatus(IN_szRFID);
}

void CView_MonitoringData::Update_SocketStatus_All()
{
	m_wndSocket.Update_SocketStatus_All();
}

void CView_MonitoringData::Update_SocketStatus_Barcode(__in LPCTSTR IN_szRFID)
{
	m_wndSocket.Update_SocketStatus_Barcode(IN_szRFID);
}

void CView_MonitoringData::Update_SocketStatus_Status(__in LPCTSTR IN_szRFID)
{
	m_wndSocket.Update_SocketStatus_Status(IN_szRFID);
}

void CView_MonitoringData::Update_SocketStatus_Equipment(__in LPCTSTR IN_szRFID)
{
	m_wndSocket.Update_SocketStatus_Equipment(IN_szRFID);
}

void CView_MonitoringData::Update_SocketStatus_Location(__in LPCTSTR IN_szRFID)
{
	m_wndSocket.Update_SocketStatus_Location(IN_szRFID);
}

void CView_MonitoringData::Update_SocketStatus_Target(__in LPCTSTR IN_szRFID)
{
	m_wndSocket.Update_SocketStatus_Target(IN_szRFID);
}

void CView_MonitoringData::Update_SocketStatus_Yield(__in LPCTSTR IN_szRFID)
{
	m_wndSocket.Update_SocketStatus_Yield(IN_szRFID);
}

void CView_MonitoringData::Update_SocketStatus_Yield_All()
{
	m_wndSocket.Update_SocketStatus_Yield_All();
}

void CView_MonitoringData::Update_SocketStatus_TestResult(__in LPCTSTR IN_szRFID)
{
	m_wndSocket.Update_SocketStatus_TestResult(IN_szRFID);
}

//=============================================================================
// Method		: OnUpdate_SocketData
// Access		: public  
// Returns		: void
// Parameter	: __in uint16_t nFlag
// Parameter	: __in LPCTSTR szRFID
// Qualifier	:
// Last Update	: 2022/2/15 - 17:00
// Desc.		:
//=============================================================================
void CView_MonitoringData::OnUpdate_SocketData(__in uint16_t nFlag, __in LPCTSTR szRFID)
{
	m_wndSocket.OnUpdate_SocketData(nFlag, szRFID);
}

void CView_MonitoringData::OnUpdate_SocketData_All(__in uint16_t nFlag)
{
	m_wndSocket.OnUpdate_SocketData_All(nFlag);
}

//=============================================================================
// Method		: Set_TestResult
// Access		: public  
// Returns		: void
// Parameter	: __in LPCTSTR IN_szRFID
// Parameter	: __in LPCTSTR IN_szBarcode
// Parameter	: __in ST_TestResult * IN_pstResult
// Qualifier	:
// Last Update	: 2022/2/15 - 19:28
// Desc.		:
//=============================================================================
void CView_MonitoringData::Set_TestResult(__in LPCTSTR IN_szRFID, __in LPCTSTR IN_szBarcode, __in ST_TestResult* IN_pstResult)
{
	m_wndJudge.Set_TestResult(IN_szRFID, IN_szBarcode, IN_pstResult);
}

void CView_MonitoringData::Set_TestResult(const CSocketInfo_Unit * IN_Socket)
{
	m_wndJudge.Set_TestResult(IN_Socket);
}

void CView_MonitoringData::SetPtr_FailInfo(CFailInfo * IN_pstFailInfo)
{
	m_wndFailInfo.SetPtr_FailInfo(IN_pstFailInfo);
}

void CView_MonitoringData::Update_FailInfo()
{
	m_wndFailInfo.Update_FailInfo();
}

//=============================================================================
// Method		: SetPath_Report
// Access		: public  
// Returns		: void
// Parameter	: __in LPCTSTR IN_szReportPath
// Qualifier	:
// Last Update	: 2022/11/10 - 11:27
// Desc.		:
//=============================================================================
void CView_MonitoringData::SetPath_Report(__in LPCTSTR IN_szReportPath)
{
	m_wndSocket.SetPath_Report(IN_szReportPath);
}

//=============================================================================
// Method		: Reset_NGCount_All
// Access		: public  
// Returns		: bool
// Qualifier	:
// Last Update	: 2023/7/18 - 14:46
// Desc.		:
//=============================================================================
bool CView_MonitoringData::Reset_NGCount_All()
{
	return m_wndFailInfo.Reset_NGCount_All();
}
