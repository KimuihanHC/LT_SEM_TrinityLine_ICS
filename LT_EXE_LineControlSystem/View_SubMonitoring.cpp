//*****************************************************************************
// Filename	: 	View_SubMonitoring.cpp
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
#include "View_SubMonitoring.h"
#include "resource.h"
#include "Def_Language_Message.h"


#define		IDC_LC_SOCKETLIST_FST	1000
#define		IDC_LC_SOCKETLIST_LST	IDC_LC_SOCKETLIST_FST + Max_SocketTypeCount - 1

#define		IDC_ED_RFID				1001
#define		IDC_BN_FIND				1002

#define		IDC_BN_SCROLL_UP		1007
#define		IDC_BN_SCROLL_DOWN		1008

#define		IDC_CB_SOCKETTYPE		1008

//=============================================================================
// CView_SubMonitoring
//=============================================================================
IMPLEMENT_DYNAMIC(CView_SubMonitoring, CWnd_BaseView)

CView_SubMonitoring::CView_SubMonitoring()
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

CView_SubMonitoring::~CView_SubMonitoring()
{
	TRACE(_T("<<< Start ~CView_SubMonitoring >>> \n"));
	
	m_font_Default.DeleteObject();
	m_Font.DeleteObject();
}

BEGIN_MESSAGE_MAP(CView_SubMonitoring, CWnd_BaseView)
	ON_WM_CREATE()
	ON_WM_SIZE	()
	ON_WM_GETMINMAXINFO()
	ON_BN_CLICKED(IDC_BN_FIND, OnBnClicked_Find)
	ON_BN_CLICKED(IDC_BN_SCROLL_UP,		OnBnClicked_ScrollUp)
	ON_BN_CLICKED(IDC_BN_SCROLL_DOWN,	OnBnClicked_ScrollDown)
	ON_CBN_SELENDOK(IDC_CB_SOCKETTYPE,	OnCbnSelendokSocketType)
	ON_MESSAGE	 (WM_UPDATE_SOCKET_DATA, OnWM_UpdateSocketData)
END_MESSAGE_MAP()


//=============================================================================
// CView_SubMonitoring 메시지 처리기입니다.
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
int CView_SubMonitoring::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd_BaseView::OnCreate(lpCreateStruct) == -1)
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

	UINT nWindowsID = 1;

	m_st_SocketType.SetStaticStyle(CVGStatic::StaticStyle_Title_Alt);
	m_st_SocketType.SetColorStyle(CVGStatic::ColorStyle_Black);
	m_st_SocketType.SetFont_Gdip(L"Arial", 12.0F);
	m_st_SocketType.Create(_T("Type"), dwStyle | SS_CENTER | SS_CENTERIMAGE, rectDummy, this, IDC_STATIC);
	m_cb_SocketType.Create(dwStyle | WS_BORDER | CBS_DROPDOWNLIST, rectDummy, this, IDC_CB_SOCKETTYPE);
	m_cb_SocketType.SetFont(&m_Font);
	for (auto nIdx = 0; nIdx < Max_SocketTypeCount; ++nIdx)
	{
		m_cb_SocketType.AddString(g_szSocketTypeName[nIdx]);
	}
	m_cb_SocketType.SetCurSel(0);

	for (auto nIdx = 0; nIdx < Max_SocketTypeCount; ++nIdx)
	{
		m_lc_SocketList[nIdx].Create(dwStyle | WS_VSCROLL, rectDummy, this, IDC_LC_SOCKETLIST_FST + nIdx);
	}

	m_bn_PgUp.SetImage(IDB_ARROW_UP, IDB_ARROW_UP);
	m_bn_PgDn.SetImage(IDB_ARROW_DOWN, IDB_ARROW_DOWN);
	m_bn_PgUp.Create(_T(""), dwStyle | BS_PUSHBUTTON, rectDummy, this, IDC_BN_SCROLL_UP);
	m_bn_PgDn.Create(_T(""), dwStyle | BS_PUSHBUTTON, rectDummy, this, IDC_BN_SCROLL_DOWN);

	m_st_Find.SetStaticStyle(CVGStatic::StaticStyle_Title_Alt);
	m_st_Find.SetColorStyle(CVGStatic::ColorStyle_Black);
	m_st_Find.SetFont_Gdip(L"Arial", 12.0F);
	m_st_Find.Create(_T("Input RFID"), dwStyle | SS_CENTER | SS_CENTERIMAGE, rectDummy, this, IDC_STATIC);
	m_ed_RFID.Create(dwStyle | ES_CENTER | WS_BORDER, rectDummy, this, IDC_ED_RFID);
	m_bn_Find.Create(_T("Find Socket JIG"), dwStyle | BS_PUSHBUTTON, rectDummy, this, IDC_BN_FIND);

	m_ed_RFID.SetFont(&m_Font);
	m_bn_Find.SetFont(&m_font_Default);

	m_ed_RFID.EnableMask(_T("A-AAAAAA-ddd"), _T("_-______-___"), _T('_'));
	m_ed_RFID.SetValidChars(NULL);
	m_ed_RFID.EnableGetMaskedCharsOnly(FALSE);
	m_ed_RFID.SetWindowText (_T("H-220215-001"));

	// 
	m_wndJudge.Create(NULL, _T("Socket Monitoring"), dwStyle, rectDummy, this, IDC_STATIC);



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
void CView_SubMonitoring::OnSize(UINT nType, int cx, int cy)
{
	CWnd_BaseView::OnSize(nType, cx, cy);

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

	m_st_SocketType.MoveWindow(iLeft, iTop, 60, iCtrlHeight);
	iLeft += 60 + iSpacing;
	m_cb_SocketType.MoveWindow(iLeft, iTop, 120, iCtrlHeight);
	iLeft += 120 + iSpacing;

	m_st_Find.MoveWindow(iLeft, iTop, 120, iCtrlHeight);
	iLeft += 120 + iSpacing;
	m_ed_RFID.MoveWindow(iLeft, iTop, iCtrlWidth, iCtrlHeight);
	iLeft += iCtrlWidth + iSpacing;
	m_bn_Find.MoveWindow(iLeft, iTop, iCtrlWidth, iCtrlHeight);

	iLeft = iMargin;
	iTop += iCtrlHeight + iSpacing;
	iHeight = iHeight - iCtrlHeight - iSpacing - iJudgeHeight - iSpacing;
	iWidth -= (40 + iSpacing);
	for (auto nIdx = 0; nIdx < Max_SocketTypeCount; ++nIdx)
	{
		m_lc_SocketList[nIdx].MoveWindow(iLeft, iTop, iWidth, iHeight);
	}
	
	iCtrlWidth = 40;
	iCtrlHeight = 160;

	iLeft = cx - iMargin - iCtrlWidth;
	m_bn_PgUp.MoveWindow(iLeft, iTop, iCtrlWidth, iCtrlHeight);
	iTop = cy - iMargin - iJudgeHeight - iSpacing - iCtrlHeight;
	m_bn_PgDn.MoveWindow(iLeft, iTop, iCtrlWidth, iCtrlHeight);
	
	// 소켓 결과 표시
	iLeft = iMargin;
	iTop = cy - iMargin - iJudgeHeight;
	iWidth = cx - iMargin - iMargin;
	m_wndJudge.MoveWindow(iLeft, iTop, iWidth, iJudgeHeight);

}

//=============================================================================
// Method		: OnClose
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2022/2/15 - 16:40
// Desc.		:
//=============================================================================
void CView_SubMonitoring::OnClose()
{
	ShowWindow(SW_HIDE);
	return;
	CWnd_BaseView::OnClose();
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
void CView_SubMonitoring::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	CWnd_BaseView::OnGetMinMaxInfo(lpMMI);

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
BOOL CView_SubMonitoring::PreCreateWindow(CREATESTRUCT& cs)
{
	return CWnd_BaseView::PreCreateWindow(cs);

	// WS_MINIMIZEBOX
}

//=============================================================================
// Method		: OnBnClicked_Find
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/12/22 - 18:21
// Desc.		:
//=============================================================================
void CView_SubMonitoring::OnBnClicked_Find()
{
	CString szText;

	int iSelType = m_cb_SocketType.GetCurSel();
	if (0 <= iSelType)
	{
		m_ed_RFID.GetWindowText(szText);

		if (m_lc_SocketList[iSelType].Find_Socket(szText) < 0)
		{
			//LT_MessageBox(_T("No socket found."));
			LT_MessageBox(g_szMessageBox_T[MB_Socket_NoFound][m_nLanguage]);
		}
	}
}

void CView_SubMonitoring::OnBnClicked_ScrollUp()
{
	// 리스트 page up
	int iSelType = m_cb_SocketType.GetCurSel();
	if (0 <= iSelType)
	{
		m_lc_SocketList[iSelType].SendMessage(WM_VSCROLL, MAKEWPARAM(SB_PAGEUP, 0), (LPARAM)0);
	}
}

void CView_SubMonitoring::OnBnClicked_ScrollDown()
{
	// 리스트 page down
	int iSelType = m_cb_SocketType.GetCurSel();
	if (0 <= iSelType)
	{
		m_lc_SocketList[iSelType].SendMessage(WM_VSCROLL, MAKEWPARAM(SB_PAGEDOWN, 0), (LPARAM)0);
	}
}

void CView_SubMonitoring::OnCbnSelendokSocketType()
{
	int iSelType = m_cb_SocketType.GetCurSel();
	if (0 <= iSelType)
	{
		for (auto nIdx = 0; nIdx < Max_SocketTypeCount; ++nIdx)
		{
			if (nIdx == iSelType)
			{
				m_lc_SocketList[nIdx].ShowWindow(SW_SHOW);
			}
			else
			{
				m_lc_SocketList[nIdx].ShowWindow(SW_HIDE);
			}
		}
	}
#if (SET_INSPECTOR == SYS_ICS_RIVIAN_LINE)
	switch (iSelType)
	{
	case enSocketType::Socket_30_FOV:
		m_ed_RFID.SetWindowText(_T("N-221225-001"));
		break;

	case enSocketType::Socket_180_FOV:
		m_ed_RFID.SetWindowText(_T("W-221225-001"));
		break;
	}
#else
	switch (iSelType)
	{
	case enSocketType::Socket_H:
		m_ed_RFID.SetWindowText(_T("H-220215-001"));
		break;

	case enSocketType::Socket_V:
		m_ed_RFID.SetWindowText(_T("V-220215-001"));
		break;

	case enSocketType::Socket_Highland:
		m_ed_RFID.SetWindowText(_T("HL-220215-001"));
		break;
	}
#endif // (SET_INSPECTOR == SYS_ICS_RIVIAN_LINE)
}

//=============================================================================
// Method		: OnWM_UpdateSocketData
// Access		: protected  
// Returns		: LRESULT
// Parameter	: WPARAM wParam
// Parameter	: LPARAM lParam
// Qualifier	:
// Last Update	: 2022/1/26 - 17:38
// Desc.		:
//=============================================================================
LRESULT CView_SubMonitoring::OnWM_UpdateSocketData(WPARAM wParam, LPARAM lParam)
{
	uint16_t nFlag = (uint16_t)wParam;
	CString szRFID = (LPCTSTR)lParam;

	OnUpdate_SocketData(nFlag, szRFID.GetBuffer());


	return 1;
}

//=============================================================================
// Method		: Init_SocketUI_List
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/12/22 - 10:39
// Desc.		:
//=============================================================================
void CView_SubMonitoring::Init_SocketUI_List()
{
	if (m_pstSocketInfo)
	{
		for (auto nTypeIdx = 0; nTypeIdx < Max_SocketTypeCount; ++nTypeIdx)
		{
			m_lc_SocketList[nTypeIdx].Clear_SocketList();

			size_t nCnt = m_pstSocketInfo->GetCount();

			if (0 < nCnt)
			{
				//m_lc_SocketList[nIdx].Set_SocketList(m_pstSocketInfo);
				m_lc_SocketList[nTypeIdx].Set_SocketList_bySocketType(m_pstSocketInfo, nTypeIdx);
			}
		}
	}
}

//=============================================================================
// Method		: Find_SocketOrder
// Access		: protected  
// Returns		: int
// Parameter	: __in LPCTSTR IN_szRFID
// Qualifier	:
// Last Update	: 2021/12/22 - 11:43
// Desc.		:
//=============================================================================
int CView_SubMonitoring::Find_SocketOrder(__in LPCTSTR IN_szRFID)
{
	int nRetIndex = -1;

	if (m_pstSocketInfo)
	{
		int nCount = 0;

		auto eIter = m_pstSocketInfo->m_Sockets.end();
		auto Iter = m_pstSocketInfo->m_Sockets.begin();
		while (Iter != eIter)
		{
			if (0 == Iter->first.compare(IN_szRFID))
			{
				nRetIndex = nCount;
				break;
			}

			nCount++;
			Iter++;
		}
	}

	return nRetIndex;
}

void CView_SubMonitoring::Set_SocketType(uint8_t IN_nSocketType)
{
	if (IN_nSocketType < enSocketType::Max_SocketTypeCount)
	{
		m_cb_SocketType.SetCurSel(IN_nSocketType);

		OnCbnSelendokSocketType();
	}
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
void CView_SubMonitoring::Set_PermissionMode(enPermissionMode InspMode)
{
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
void CView_SubMonitoring::Update_SocketInfo()
{
 	Init_SocketUI_List();
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
void CView_SubMonitoring::Update_SocketStatus(__in LPCTSTR IN_szRFID)
{
	// RFID로 소켓 순서 찾아서 List의 내용 업데이트

	if (m_pstSocketInfo)
	{
		//m_lc_SocketList.Update_Socket(IN_szRFID, &m_pstSocketInfo->GetAt(IN_szRFID));
		int nRow = Find_SocketOrder(IN_szRFID);
		if (0 <= nRow)
		{
			uint8_t nType = m_pstSocketInfo->GetAt(IN_szRFID).nSocketType;
			m_lc_SocketList[nType].Update_Socket(nRow, &m_pstSocketInfo->GetAt(IN_szRFID));
		}
	}
}

void CView_SubMonitoring::Update_SocketStatus_All()
{
	if (m_pstSocketInfo)
	{
		for (auto Iter = m_pstSocketInfo->m_Sockets.begin(); Iter != m_pstSocketInfo->m_Sockets.end(); Iter++)
		{
			int nRow = Find_SocketOrder(Iter->second.szRFID);
			if (0 <= nRow)
			{
				uint8_t nType = Iter->second.nSocketType;
				m_lc_SocketList[nType].Update_Socket(nRow, &Iter->second);
			}
		}
	}
}

void CView_SubMonitoring::Update_SocketStatus_Barcode(__in LPCTSTR IN_szRFID)
{
	if (m_pstSocketInfo)
	{
		//m_lc_SocketList.Update_Socket(IN_szRFID, &m_pstSocketInfo->GetAt(IN_szRFID));
		int nRow = Find_SocketOrder(IN_szRFID);
		if (0 <= nRow)
		{
			uint8_t nType = m_pstSocketInfo->GetAt(IN_szRFID).nSocketType;
			m_lc_SocketList[nType].Update_Socket_Barcode(nRow, &m_pstSocketInfo->GetAt(IN_szRFID));
		}
	}
}

void CView_SubMonitoring::Update_SocketStatus_Status(__in LPCTSTR IN_szRFID)
{
	if (m_pstSocketInfo)
	{
		//m_lc_SocketList.Update_Socket(IN_szRFID, &m_pstSocketInfo->GetAt(IN_szRFID));
		int nRow = Find_SocketOrder(IN_szRFID);
		if (0 <= nRow)
		{
			uint8_t nType = m_pstSocketInfo->GetAt(IN_szRFID).nSocketType;
			m_lc_SocketList[nType].Update_Socket_Status(nRow, &m_pstSocketInfo->GetAt(IN_szRFID));
		}
	}
}

void CView_SubMonitoring::Update_SocketStatus_Equipment(__in LPCTSTR IN_szRFID)
{
	if (m_pstSocketInfo)
	{
		//m_lc_SocketList.Update_Socket(IN_szRFID, &m_pstSocketInfo->GetAt(IN_szRFID));
		int nRow = Find_SocketOrder(IN_szRFID);
		if (0 <= nRow)
		{
			uint8_t nType = m_pstSocketInfo->GetAt(IN_szRFID).nSocketType;
			m_lc_SocketList[nType].Update_Socket_Equipment(nRow, &m_pstSocketInfo->GetAt(IN_szRFID));
		}
	}
}

void CView_SubMonitoring::Update_SocketStatus_Location(__in LPCTSTR IN_szRFID)
{
	if (m_pstSocketInfo)
	{
		//m_lc_SocketList.Update_Socket(IN_szRFID, &m_pstSocketInfo->GetAt(IN_szRFID));
		int nRow = Find_SocketOrder(IN_szRFID);
		if (0 <= nRow)
		{
			uint8_t nType = m_pstSocketInfo->GetAt(IN_szRFID).nSocketType;
			m_lc_SocketList[nType].Update_Socket_Location(nRow, &m_pstSocketInfo->GetAt(IN_szRFID));
		}
	}
}

void CView_SubMonitoring::Update_SocketStatus_Target(__in LPCTSTR IN_szRFID)
{
	if (m_pstSocketInfo)
	{
		//m_lc_SocketList.Update_Socket(IN_szRFID, &m_pstSocketInfo->GetAt(IN_szRFID));
		int nRow = Find_SocketOrder(IN_szRFID);
		if (0 <= nRow)
		{
			uint8_t nType = m_pstSocketInfo->GetAt(IN_szRFID).nSocketType;
			m_lc_SocketList[nType].Update_Socket_Target(nRow, &m_pstSocketInfo->GetAt(IN_szRFID));
		}
	}
}

void CView_SubMonitoring::Update_SocketStatus_Yield(__in LPCTSTR IN_szRFID)
{
	if (m_pstSocketInfo)
	{
		//m_lc_SocketList.Update_Socket(IN_szRFID, &m_pstSocketInfo->GetAt(IN_szRFID));
		int nRow = Find_SocketOrder(IN_szRFID);
		if (0 <= nRow)
		{
			uint8_t nType = m_pstSocketInfo->GetAt(IN_szRFID).nSocketType;
			m_lc_SocketList[nType].Update_Socket_Yield(nRow, &m_pstSocketInfo->GetAt(IN_szRFID));
		}
	}
}

void CView_SubMonitoring::Update_SocketStatus_Yield_All()
{
	if (m_pstSocketInfo)
	{
		for (auto Iter = m_pstSocketInfo->m_Sockets.begin(); Iter != m_pstSocketInfo->m_Sockets.end(); Iter++)
		{
			int nRow = Find_SocketOrder(Iter->second.szRFID);
			if (0 <= nRow)
			{
				uint8_t nType = Iter->second.nSocketType;
				m_lc_SocketList[nType].Update_Socket_Yield(nRow, &Iter->second);
			}
		}
	}
}

void CView_SubMonitoring::Update_SocketStatus_TestResult(__in LPCTSTR IN_szRFID)
{
	if (m_pstSocketInfo)
	{
		//m_lc_SocketList.Update_Socket(IN_szRFID, &m_pstSocketInfo->GetAt(IN_szRFID));
		int nRow = Find_SocketOrder(IN_szRFID);
		if (0 <= nRow)
		{
			uint8_t nType = m_pstSocketInfo->GetAt(IN_szRFID).nSocketType;
			m_lc_SocketList[nType].Update_Socket_TestResult(nRow, &m_pstSocketInfo->GetAt(IN_szRFID));
		}
	}
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
void CView_SubMonitoring::OnUpdate_SocketData(__in uint16_t nFlag, __in LPCTSTR szRFID)
{
	// 	typedef enum 
// 	{
// 		WM_Socket_All			= 0xFFFF,
// 		WM_Socket_Barcode		= 0x0001,
// 		WM_Socket_Status		= 0x0002,
// 		WM_Socket_Equipment		= 0x0004,
// 		WM_Socket_Location		= 0x0008,
// 		WM_Socket_Target		= 0x0010,
// 		WM_Socket_Yield			= 0x0020,
// 		WM_Socket_TestResult	= 0x0040,
// 	}WM_Socket_Update_Para;

	if (nFlag == WM_Socket_All)
	{
		Update_SocketStatus(szRFID);

		return;
	}

	if (0 < (nFlag & WM_Socket_Barcode))
		Update_SocketStatus_Barcode(szRFID);

	if (0 < (nFlag & WM_Socket_Status))
		Update_SocketStatus_Status(szRFID);

	if (0 < (nFlag & WM_Socket_Equipment))
		Update_SocketStatus_Equipment(szRFID);

	if (0 < (nFlag & WM_Socket_Location))
		Update_SocketStatus_Location(szRFID);

	if (0 < (nFlag & WM_Socket_Target))
		Update_SocketStatus_Target(szRFID);

	if (0 < (nFlag & WM_Socket_Yield))
		Update_SocketStatus_Yield(szRFID);

	if (0 < (nFlag & WM_Socket_TestResult))
		Update_SocketStatus_TestResult(szRFID);
}

void CView_SubMonitoring::OnUpdate_SocketData_All(__in uint16_t nFlag)
{
	if (nFlag == WM_Socket_All)
	{
		Update_SocketStatus_All();

		return;
	}

	if (0 < (nFlag & WM_Socket_Yield))
	{
		Update_SocketStatus_Yield_All();
	}
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
void CView_SubMonitoring::Set_TestResult(__in LPCTSTR IN_szRFID, __in LPCTSTR IN_szBarcode, __in ST_TestResult* IN_pstResult)
{
	m_wndJudge.Set_TestResult(IN_szRFID, IN_szBarcode, IN_pstResult);
}

void CView_SubMonitoring::Set_TestResult(const CSocketInfo_Unit * IN_Socket)
{
	m_wndJudge.Set_TestResult(IN_Socket);
}
