//*****************************************************************************
// Filename	: 	Wnd_SocketView.cpp
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
#include "Wnd_SocketView.h"
#include "resource.h"
#include "Def_Language_Message.h"

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

static LPCTSTR g_szCtrlText_T[Lang_MaxCount][Txt_MaxCount] =
{
	// 한국어
	{
		_T("종류"),							// Txt_Type
		_T("RFID 입력"),						// Txt_Find
		_T("소켓 지그 찾기"),					// Txt_Find_Bn
		_T("선택 소켓 지그 수율 초기화"),		// Txt_ResetYield
		_T("전체 소켓 지그 수율 초기화"),		// Txt_ResetYield_All
		_T("선택 소켓 지그 데이터 초기화"),	// Txt_Init_Data
		_T("전체 소켓 지그 데이터 초기화"),	// Txt_Init_Data_All
		_T("수율 저장 (CSV)"),				// Txt_SaveYield
	},

	// 영어
	{
		_T("Type"),							// Txt_Type
		_T("Input RFID"),					// Txt_Find
		_T("Find Socket JIG"),				// Txt_Find_Bn
		_T("Reset Socket JIG Yield"),		// Txt_ResetYield
		_T("Reset All Socket JIG Yield"),	// Txt_ResetYield_All
		_T("Init. Socket JIG Data"),		// Txt_Init_Data
		_T("Init. All Socket JIG Data"),	// Txt_Init_Data_All
		_T("Save Yield (CSV)"),				// Txt_SaveYield
	},

	// 베트남어
	{
		_T("Type"),							// Txt_Type
		_T("Input RFID"),					// Txt_Find
		_T("Find Socket JIG"),				// Txt_Find_Bn
		_T("Reset Socket JIG Yield"),		// Txt_ResetYield
		_T("Reset All Socket JIG Yield"),	// Txt_ResetYield_All
		_T("Init. Socket JIG Data"),		// Txt_Init_Data
		_T("Init. All Socket JIG Data"),	// Txt_Init_Data_All
		_T("Save Yield (CSV)"),				// Txt_SaveYield
	},

	// 중국어
	{
		_T("类别"),							// Txt_Type
		_T("请输入RFID"),					// Txt_Find
		_T("插口夹具搜索"),					// Txt_Find_Bn
		_T("重置选中的插口夹具收率"),			// Txt_ResetYield
		_T("初始化全部插口夹具收率"),			// Txt_ResetYield_All
		_T("重置所选插口夹具数据"),			// Txt_Init_Data
		_T("重置全部插口夹具数据"),			// Txt_Init_Data_All
		_T("保存收率到文件(.CSV))"),			// Txt_SaveYield
	},
};


#define		IDC_LC_SOCKETLIST_FST	1000
#define		IDC_LC_SOCKETLIST_LST	IDC_LC_SOCKETLIST_FST + Max_SocketTypeCount - 1

#define		IDC_ED_RFID				1001
#define		IDC_BN_FIND				1002

#define		IDC_BN_RESET_YIELD		1003
#define		IDC_BN_RESET_YIELD_A	1004
#define		IDC_BN_UNREGISTER		1005
#define		IDC_BN_UNREGISTER_A		1006

#define		IDC_BN_SCROLL_UP		1007
#define		IDC_BN_SCROLL_DOWN		1008

#define		IDC_CB_SOCKETTYPE		1008
#define		IDC_BN_SAVE_CSV			1009

//=============================================================================
// CWnd_SocketView
//=============================================================================
IMPLEMENT_DYNAMIC(CWnd_SocketView, CWnd_BaseView)

CWnd_SocketView::CWnd_SocketView()
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

CWnd_SocketView::~CWnd_SocketView()
{
	TRACE(_T("<<< Start ~CWnd_SocketView >>> \n"));
	
	m_font_Default.DeleteObject();
	m_Font.DeleteObject();
}

BEGIN_MESSAGE_MAP(CWnd_SocketView, CWnd_BaseView)
	ON_WM_CREATE()
	ON_WM_SIZE	()
	ON_BN_CLICKED(IDC_BN_FIND,			OnBnClicked_Find)
	ON_BN_CLICKED(IDC_BN_RESET_YIELD,	OnBnClicked_ResetYield)
	ON_BN_CLICKED(IDC_BN_RESET_YIELD_A, OnBnClicked_ResetYield_A)
	ON_BN_CLICKED(IDC_BN_UNREGISTER,	OnBnClicked_Unregister)
	ON_BN_CLICKED(IDC_BN_UNREGISTER_A,	OnBnClicked_Unregister_A)
	ON_BN_CLICKED(IDC_BN_SCROLL_UP,		OnBnClicked_ScrollUp)
	ON_BN_CLICKED(IDC_BN_SCROLL_DOWN,	OnBnClicked_ScrollDown)
	ON_BN_CLICKED(IDC_BN_SAVE_CSV,		OnBnClicked_SaveCSV)
	ON_CBN_SELENDOK(IDC_CB_SOCKETTYPE,	OnCbnSelendokSocketType)
	ON_MESSAGE	 (WM_UPDATE_SOCKET_DATA, OnWM_UpdateSocketData)
END_MESSAGE_MAP()


//=============================================================================
// CWnd_SocketView 메시지 처리기입니다.
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
int CWnd_SocketView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd_BaseView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	//SetBackgroundColor(RGB(0, 0, 0));

	DWORD dwStyle = WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	CRect rectDummy;
	rectDummy.SetRectEmpty();
	
	/*m_Font.CreateStockObject(DEFAULT_GUI_FONT);*/

	UINT nWindowsID = 1;

	m_st_SocketType.SetStaticStyle(CVGStatic::StaticStyle_Title_Alt);
	m_st_SocketType.SetColorStyle(CVGStatic::ColorStyle_Black);
	m_st_SocketType.SetFont_Gdip(L"Arial", 12.0F);
	m_st_SocketType.Create(g_szCtrlText_T[m_nLanguage][Txt_Type], dwStyle | SS_CENTER | SS_CENTERIMAGE, rectDummy, this, IDC_STATIC);
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
	m_st_Find.Create(g_szCtrlText_T[m_nLanguage][Txt_Find], dwStyle | SS_CENTER | SS_CENTERIMAGE, rectDummy, this, IDC_STATIC);
	m_ed_RFID.Create(dwStyle | ES_CENTER | WS_BORDER, rectDummy, this, IDC_ED_RFID);
	m_bn_Find.Create(g_szCtrlText_T[m_nLanguage][Txt_Find_Bn], dwStyle | BS_PUSHBUTTON, rectDummy, this, IDC_BN_FIND);

	m_bn_ResetYield.Create(g_szCtrlText_T[m_nLanguage][Txt_ResetYield], dwStyle | BS_PUSHBUTTON, rectDummy, this, IDC_BN_RESET_YIELD);
	m_bn_ResetYield_All.Create(g_szCtrlText_T[m_nLanguage][Txt_ResetYield_All], dwStyle | BS_PUSHBUTTON, rectDummy, this, IDC_BN_RESET_YIELD_A);

	m_bn_Unregister.Create(g_szCtrlText_T[m_nLanguage][Txt_Init_Data], dwStyle | BS_PUSHBUTTON, rectDummy, this, IDC_BN_UNREGISTER);
	m_bn_Unregister_All.Create(g_szCtrlText_T[m_nLanguage][Txt_Init_Data_All], dwStyle | BS_PUSHBUTTON, rectDummy, this, IDC_BN_UNREGISTER_A);

	m_bn_Save_CSV.Create(g_szCtrlText_T[m_nLanguage][Txt_SaveYield], dwStyle | BS_PUSHBUTTON, rectDummy, this, IDC_BN_SAVE_CSV);

	m_ed_RFID.SetFont(&m_Font);
	m_bn_Find.SetFont(&m_font_Default);

	m_ed_RFID.EnableMask(_T("A-AAAAAA-ddd"), _T("_-______-___"), _T('_'));
	m_ed_RFID.SetValidChars(NULL);
	m_ed_RFID.EnableGetMaskedCharsOnly(FALSE);
#if (SET_INSPECTOR == SYS_ICS_RIVIAN_LINE)
	m_ed_RFID.SetWindowText(_T("A-230114-"));
#else
	m_ed_RFID.SetWindowText(_T("H-220215-001"));
#endif

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
void CWnd_SocketView::OnSize(UINT nType, int cx, int cy)
{
	CWnd_BaseView::OnSize(nType, cx, cy);

	if ((cx == 0) && (cy == 0))
		return;

	int iMargin			= 5;
 	int iSpacing		= 5;
 	int iCateSpacing	= 5;

	int iLeft			= iMargin;
	int iTop			= iMargin;
	int iWidth			= cx - iMargin - iMargin;
	int iHeight			= cy - iMargin - iMargin;	
	int iCtrlWidth		= 180;
	int iCtrlHeight		= 35;


	m_st_SocketType.MoveWindow(iLeft, iTop, 60, iCtrlHeight);
	iLeft += 60 + iSpacing;
	m_cb_SocketType.MoveWindow(iLeft, iTop, 120, iCtrlHeight);
	iLeft += 120 + iSpacing;

	m_st_Find.MoveWindow(iLeft, iTop, 120, iCtrlHeight);
	iLeft += 120 + iSpacing;
	m_ed_RFID.MoveWindow(iLeft, iTop, iCtrlWidth, iCtrlHeight);
	iLeft += iCtrlWidth + iSpacing;
	m_bn_Find.MoveWindow(iLeft, iTop, iCtrlWidth, iCtrlHeight);


	iLeft = cx - iMargin - iCtrlWidth;
	m_bn_Save_CSV.MoveWindow(iLeft, iTop, iCtrlWidth, iCtrlHeight);
	iLeft -= (iCtrlWidth + iSpacing);
	m_bn_ResetYield_All.MoveWindow(iLeft, iTop, iCtrlWidth, iCtrlHeight);
	iLeft -= (iCtrlWidth + iSpacing);
	m_bn_ResetYield.MoveWindow(iLeft, iTop, iCtrlWidth, iCtrlHeight);

	iLeft -= (iCtrlWidth + iSpacing);
	m_bn_Unregister_All.MoveWindow(iLeft, iTop, iCtrlWidth, iCtrlHeight);
	iLeft -= (iCtrlWidth + iSpacing);
	m_bn_Unregister.MoveWindow(iLeft, iTop, iCtrlWidth, iCtrlHeight);


	
	
	iLeft = iMargin;
	iTop += iCtrlHeight + iSpacing;
	iHeight = iHeight - iCtrlHeight - iSpacing;
	iWidth -= (40 + iSpacing);
	for (auto nIdx = 0; nIdx < Max_SocketTypeCount; ++nIdx)
	{
		m_lc_SocketList[nIdx].MoveWindow(iLeft, iTop, iWidth, iHeight);
	}

	iCtrlWidth = 40;
	iCtrlHeight = 160;

	iLeft = cx - iMargin - iCtrlWidth;
	m_bn_PgUp.MoveWindow(iLeft, iTop, iCtrlWidth, iCtrlHeight);
	iTop = cy - iMargin - iCtrlHeight;
	m_bn_PgDn.MoveWindow(iLeft, iTop, iCtrlWidth, iCtrlHeight);
}

//=============================================================================
// Method		: OnBnClicked_Find
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/12/22 - 18:21
// Desc.		:
//=============================================================================
void CWnd_SocketView::OnBnClicked_Find()
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

//=============================================================================
// Method		: OnBnClicked_ResetYield
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2022/2/19 - 18:03
// Desc.		:
//=============================================================================
void CWnd_SocketView::OnBnClicked_ResetYield()
{
	Reset_Yield_SelectedSocket();
}

void CWnd_SocketView::OnBnClicked_ResetYield_A()
{
	Reset_Yield_All();
}

void CWnd_SocketView::OnBnClicked_Unregister()
{
	Unregister_SocketInfo();
}

void CWnd_SocketView::OnBnClicked_Unregister_A()
{
	Unregister_SocketInfo_All();
}

void CWnd_SocketView::OnBnClicked_ScrollUp()
{
	// 리스트 page up
	int iSelType = m_cb_SocketType.GetCurSel();
	if (0 <= iSelType)
	{
		m_lc_SocketList[iSelType].SendMessage(WM_VSCROLL, MAKEWPARAM(SB_PAGEUP, 0), (LPARAM)0);
	}
}

void CWnd_SocketView::OnBnClicked_ScrollDown()
{
	// 리스트 page down
	int iSelType = m_cb_SocketType.GetCurSel();
	if (0 <= iSelType)
	{
		m_lc_SocketList[iSelType].SendMessage(WM_VSCROLL, MAKEWPARAM(SB_PAGEDOWN, 0), (LPARAM)0);
	}
}

void CWnd_SocketView::OnBnClicked_SaveCSV()
{
	if (m_pstSocketInfo)
	{
		// 저장 할 경로를 선택하세요.
		CString szInitPath = _T("C:\\");
		CFolderPickerDialog Picker(szInitPath, OFN_FILEMUSTEXIST, NULL, 0);
		if (Picker.DoModal() == IDOK)
		{
			CString szFolderPath = Picker.GetPathName();

			if (m_pstSocketInfo->Write_CSV_File(szFolderPath))
			{
				// 저장에 성공
			}
			else
			{
				// 저장에 실패
			}
		}
	}
	else
	{
		//LT_MessageBox(_T("SocketInfo Pointer is null !!"));
		LT_MessageBox(g_szMessageBox_T[MB_Socket_PointerIsNull][m_nLanguage]);
	}
}

void CWnd_SocketView::OnCbnSelendokSocketType()
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
		m_ed_RFID.SetWindowText(_T("A-230114-001"));
		break;

	case enSocketType::Socket_180_FOV:
		m_ed_RFID.SetWindowText(_T("B-230114-001"));
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
		m_ed_RFID.SetWindowText(_T("L-220215-001"));
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
LRESULT CWnd_SocketView::OnWM_UpdateSocketData(WPARAM wParam, LPARAM lParam)
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
void CWnd_SocketView::Init_SocketUI_List()
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
int CWnd_SocketView::Find_SocketOrder(__in LPCTSTR IN_szRFID)
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

//=============================================================================
// Method		: Reset_Yield_SelectedSocket
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2022/2/19 - 21:13
// Desc.		:
//=============================================================================
void CWnd_SocketView::Reset_Yield_SelectedSocket()
{
	//if (IDYES == LT_MessageBox(_T("Reset yield of selected Socket JIG?"), MB_YESNO))
	if (IDYES == LT_MessageBox(g_szMessageBox_T[MB_Socket_Reset_Yield_Selected][m_nLanguage], MB_YESNO))
	{
		if (m_pstSocketInfo)
		{
			int iSelType = m_cb_SocketType.GetCurSel();
			if (0 <= iSelType)
			{
				CSocketInfo_Unit selSocket;
				m_lc_SocketList[iSelType].Get_SelectedSocket(selSocket);

				m_pstSocketInfo->Reset_Yield(selSocket.szRFID);
			}
		}
	}
}

void CWnd_SocketView::Reset_Yield_All()
{
	//if (IDYES == LT_MessageBox(_T("Reset the yield of all socket JIG?"), MB_YESNO))
	if (IDYES == LT_MessageBox(g_szMessageBox_T[MB_Socket_Reset_Yield_All][m_nLanguage], MB_YESNO))
	{
		if (m_pstSocketInfo)
		{
			if (FALSE == m_szReportPath.IsEmpty())
			{
				if (false == m_pstSocketInfo->Write_CSV_File(m_szReportPath.GetBuffer()))
				{
					// 저장에 실패
					CString szText;
					szText.Format(_T("Socket JIG Yield File write failed!"));
					AfxGetApp()->GetMainWnd()->SendMessage(WM_LOGMSG, (WPARAM)szText.GetBuffer(), MAKELPARAM(LOGTYPE_ERROR, 0));
				}
			}

			m_pstSocketInfo->Reset_Yield_All();
		}
	}
}

//=============================================================================
// Method		: Unregister_SocketInfo
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2022/2/20 - 14:28
// Desc.		:
//=============================================================================
void CWnd_SocketView::Unregister_SocketInfo()
{
	//if (IDYES == LT_MessageBox(_T("Initialize data of selected socket JIG?"), MB_YESNO))
	if (IDYES == LT_MessageBox(g_szMessageBox_T[MB_Socket_Reset_Data_Selected][m_nLanguage], MB_YESNO))
	{
		if (m_pstSocketInfo)
		{
			int iSelType = m_cb_SocketType.GetCurSel();
			if (0 <= iSelType)
			{
				CSocketInfo_Unit selSocket;
				m_lc_SocketList[iSelType].Get_SelectedSocket(selSocket);

				m_pstSocketInfo->Reset_RegisterData(selSocket.szRFID);
			}
		}
	}
}

void CWnd_SocketView::Unregister_SocketInfo_All()
{
	//if (IDYES == LT_MessageBox(_T("Initialize data of All socket JIG?"), MB_YESNO))
	if (IDYES == LT_MessageBox(g_szMessageBox_T[MB_Socket_Reset_Data_All][m_nLanguage], MB_YESNO))
	{
		if (m_pstSocketInfo)
		{
			m_pstSocketInfo->Reset_RegisterData_All();
		}
	}
}

//=============================================================================
// Method		: OnLanguage
// Access		: virtual public  
// Returns		: void
// Parameter	: __in uint8_t IN_nLanguage
// Qualifier	:
// Last Update	: 2022/7/18 - 13:40
// Desc.		:
//=============================================================================
void CWnd_SocketView::OnLanguage(__in uint8_t IN_nLanguage)
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
	m_Font.GetLogFont(&lf);
	lf.lfCharSet = Get_CharSet(m_nLanguage);
	m_Font.DeleteObject();
	m_Font.CreateFontIndirect(&lf);

	m_cb_SocketType.SetFont(&m_Font);
	m_ed_RFID.SetFont(&m_Font);
	m_bn_Find.SetFont(&m_font_Default);

	for (auto nIdx = 0; nIdx < Max_SocketTypeCount; ++nIdx)
	{
		m_lc_SocketList[nIdx].OnLanguage(IN_nLanguage);
	}

	if (GetSafeHwnd())
	{
		m_st_SocketType.SetText(g_szCtrlText_T[m_nLanguage][Txt_Type]);
		m_st_Find.SetText(g_szCtrlText_T[m_nLanguage][Txt_Find]);
		m_bn_Find.SetWindowText(g_szCtrlText_T[m_nLanguage][Txt_Find_Bn]);
		m_bn_ResetYield.SetWindowText(g_szCtrlText_T[m_nLanguage][Txt_ResetYield]);
		m_bn_ResetYield_All.SetWindowText(g_szCtrlText_T[m_nLanguage][Txt_ResetYield_All]);
		m_bn_Unregister.SetWindowText(g_szCtrlText_T[m_nLanguage][Txt_Init_Data]);
		m_bn_Unregister_All.SetWindowText(g_szCtrlText_T[m_nLanguage][Txt_Init_Data_All]);
		m_bn_Save_CSV.SetWindowText(g_szCtrlText_T[m_nLanguage][Txt_SaveYield]);
	}
}

//=============================================================================
// Method		: Set_SocketType
// Access		: public  
// Returns		: void
// Parameter	: uint8_t IN_nSocketType
// Qualifier	:
// Last Update	: 2022/7/18 - 14:02
// Desc.		:
//=============================================================================
void CWnd_SocketView::Set_SocketType(uint8_t IN_nSocketType)
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
void CWnd_SocketView::Set_PermissionMode(enPermissionMode InspMode)
{
	switch (InspMode)
	{
	case Permission_Operator:
		m_InspMode = Permission_Operator;

		m_bn_ResetYield.EnableWindow(FALSE);
		m_bn_ResetYield_All.EnableWindow(FALSE);
		m_bn_Unregister.EnableWindow(FALSE);
		m_bn_Unregister_All.EnableWindow(FALSE);
		m_bn_Save_CSV.EnableWindow(FALSE);
		break;

	case Permission_Engineer:
		m_bn_ResetYield.EnableWindow(TRUE);
		m_bn_ResetYield_All.EnableWindow(TRUE);
		m_bn_Unregister.EnableWindow(FALSE);
		m_bn_Unregister_All.EnableWindow(FALSE);
		m_bn_Save_CSV.EnableWindow(TRUE);
		break;

	case Permission_Administrator:
		m_InspMode = Permission_Administrator;
		m_bn_ResetYield.EnableWindow(TRUE);
		m_bn_ResetYield_All.EnableWindow(TRUE);
		m_bn_Unregister.EnableWindow(TRUE);
		m_bn_Unregister_All.EnableWindow(TRUE);
		m_bn_Save_CSV.EnableWindow(TRUE);
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
void CWnd_SocketView::Update_SocketInfo()
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
void CWnd_SocketView::Update_SocketStatus(__in LPCTSTR IN_szRFID)
{
	// RFID로 소켓 순서 찾아서 List의 내용 업데이트

	if (m_pstSocketInfo)
	{
		//m_lc_SocketList.Update_Socket(IN_szRFID, &m_pstSocketInfo->GetAt(IN_szRFID));
		/*int nRow = Find_SocketOrder(IN_szRFID);
		if (0 <= nRow)
		{
			uint8_t nType = m_pstSocketInfo->GetAt(IN_szRFID).nSocketType;
			m_lc_SocketList[nType].Update_Socket(nRow, &m_pstSocketInfo->GetAt(IN_szRFID));
		}*/

		uint8_t nType = m_pstSocketInfo->GetAt(IN_szRFID).nSocketType;
		m_lc_SocketList[nType].Update_Socket(IN_szRFID, &m_pstSocketInfo->GetAt(IN_szRFID));
	}
}

void CWnd_SocketView::Update_SocketStatus_All()
{
	if (m_pstSocketInfo)
	{
		for (auto Iter = m_pstSocketInfo->m_Sockets.begin(); Iter != m_pstSocketInfo->m_Sockets.end(); Iter++)
		{
 			/*int nRow = Find_SocketOrder(Iter->second.szRFID);
 			if (0 <= nRow)
 			{
 				uint8_t nType = Iter->second.nSocketType;
 				m_lc_SocketList[nType].Update_Socket(nRow, &Iter->second);
 			}*/

 			uint8_t nType = Iter->second.nSocketType;
 			m_lc_SocketList[nType].Update_Socket(Iter->second.szRFID, &Iter->second);

		}
	}
}

void CWnd_SocketView::Update_SocketStatus_Barcode(__in LPCTSTR IN_szRFID)
{
	if (m_pstSocketInfo)
	{
		//m_lc_SocketList.Update_Socket(IN_szRFID, &m_pstSocketInfo->GetAt(IN_szRFID));
		/*int nRow = Find_SocketOrder(IN_szRFID);
		if (0 <= nRow)
		{
			uint8_t nType = m_pstSocketInfo->GetAt(IN_szRFID).nSocketType;
			int nRow = m_lc_SocketList[nType].Find_Socket(IN_szRFID);
			m_lc_SocketList[nType].Update_Socket_Barcode(nRow, &m_pstSocketInfo->GetAt(IN_szRFID));
		}*/

		uint8_t nType = m_pstSocketInfo->GetAt(IN_szRFID).nSocketType;
		int nRow = m_lc_SocketList[nType].Find_Socket(IN_szRFID);
		if (0 <= nRow)
		{
			m_lc_SocketList[nType].Update_Socket_Barcode(nRow, &m_pstSocketInfo->GetAt(IN_szRFID));
		}
	}
}

void CWnd_SocketView::Update_SocketStatus_Status(__in LPCTSTR IN_szRFID)
{
	if (m_pstSocketInfo)
	{
		//m_lc_SocketList.Update_Socket(IN_szRFID, &m_pstSocketInfo->GetAt(IN_szRFID));
		/*int nRow = Find_SocketOrder(IN_szRFID);
		if (0 <= nRow)
		{
			uint8_t nType = m_pstSocketInfo->GetAt(IN_szRFID).nSocketType;
			m_lc_SocketList[nType].Update_Socket_Status(nRow, &m_pstSocketInfo->GetAt(IN_szRFID));
		}*/

		uint8_t nType = m_pstSocketInfo->GetAt(IN_szRFID).nSocketType;
		int nRow = m_lc_SocketList[nType].Find_Socket(IN_szRFID);
		if (0 <= nRow)
		{
			m_lc_SocketList[nType].Update_Socket_Status(nRow, &m_pstSocketInfo->GetAt(IN_szRFID));
		}
	}
}

void CWnd_SocketView::Update_SocketStatus_Equipment(__in LPCTSTR IN_szRFID)
{
	if (m_pstSocketInfo)
	{
		//m_lc_SocketList.Update_Socket(IN_szRFID, &m_pstSocketInfo->GetAt(IN_szRFID));
		/*int nRow = Find_SocketOrder(IN_szRFID);
		if (0 <= nRow)
		{
			uint8_t nType = m_pstSocketInfo->GetAt(IN_szRFID).nSocketType;
			m_lc_SocketList[nType].Update_Socket_Equipment(nRow, &m_pstSocketInfo->GetAt(IN_szRFID));
		}*/

		uint8_t nType = m_pstSocketInfo->GetAt(IN_szRFID).nSocketType;
		int nRow = m_lc_SocketList[nType].Find_Socket(IN_szRFID);
		if (0 <= nRow)
		{
			m_lc_SocketList[nType].Update_Socket_Equipment(nRow, &m_pstSocketInfo->GetAt(IN_szRFID));
		}
	}
}

void CWnd_SocketView::Update_SocketStatus_Location(__in LPCTSTR IN_szRFID)
{
	if (m_pstSocketInfo)
	{
		//m_lc_SocketList.Update_Socket(IN_szRFID, &m_pstSocketInfo->GetAt(IN_szRFID));
		/*int nRow = Find_SocketOrder(IN_szRFID);
		if (0 <= nRow)
		{
			uint8_t nType = m_pstSocketInfo->GetAt(IN_szRFID).nSocketType;
			m_lc_SocketList[nType].Update_Socket_Location(nRow, &m_pstSocketInfo->GetAt(IN_szRFID));
		}*/

		uint8_t nType = m_pstSocketInfo->GetAt(IN_szRFID).nSocketType;
		int nRow = m_lc_SocketList[nType].Find_Socket(IN_szRFID);
		if (0 <= nRow)
		{
			m_lc_SocketList[nType].Update_Socket_Location(nRow, &m_pstSocketInfo->GetAt(IN_szRFID));
		}
	}
}

void CWnd_SocketView::Update_SocketStatus_Target(__in LPCTSTR IN_szRFID)
{
	if (m_pstSocketInfo)
	{
		//m_lc_SocketList.Update_Socket(IN_szRFID, &m_pstSocketInfo->GetAt(IN_szRFID));
		/*int nRow = Find_SocketOrder(IN_szRFID);
		if (0 <= nRow)
		{
			uint8_t nType = m_pstSocketInfo->GetAt(IN_szRFID).nSocketType;
			m_lc_SocketList[nType].Update_Socket_Target(nRow, &m_pstSocketInfo->GetAt(IN_szRFID));
		}*/

		uint8_t nType = m_pstSocketInfo->GetAt(IN_szRFID).nSocketType;
		int nRow = m_lc_SocketList[nType].Find_Socket(IN_szRFID);
		if (0 <= nRow)
		{
			m_lc_SocketList[nType].Update_Socket_Target(nRow, &m_pstSocketInfo->GetAt(IN_szRFID));
		}
	}
}

void CWnd_SocketView::Update_SocketStatus_Yield(__in LPCTSTR IN_szRFID)
{
	if (m_pstSocketInfo)
	{
		//m_lc_SocketList.Update_Socket(IN_szRFID, &m_pstSocketInfo->GetAt(IN_szRFID));
		/*int nRow = Find_SocketOrder(IN_szRFID);
		if (0 <= nRow)
		{
			uint8_t nType = m_pstSocketInfo->GetAt(IN_szRFID).nSocketType;
			m_lc_SocketList[nType].Update_Socket_Yield(nRow, &m_pstSocketInfo->GetAt(IN_szRFID));
		}*/

		uint8_t nType = m_pstSocketInfo->GetAt(IN_szRFID).nSocketType;
		int nRow = m_lc_SocketList[nType].Find_Socket(IN_szRFID);
		if (0 <= nRow)
		{
			m_lc_SocketList[nType].Update_Socket_Yield(nRow, &m_pstSocketInfo->GetAt(IN_szRFID));
		}
	}
}

void CWnd_SocketView::Update_SocketStatus_Yield_All()
{
	if (m_pstSocketInfo)
	{
		for (auto Iter = m_pstSocketInfo->m_Sockets.begin(); Iter != m_pstSocketInfo->m_Sockets.end(); Iter++)
		{
			/*int nRow = Find_SocketOrder(Iter->second.szRFID);
			if (0 <= nRow)
			{
				uint8_t nType = Iter->second.nSocketType;
				m_lc_SocketList[nType].Update_Socket_Yield(nRow, &Iter->second);
			}*/

			uint8_t nType = m_pstSocketInfo->GetAt(Iter->second.szRFID).nSocketType;
			int nRow = m_lc_SocketList[nType].Find_Socket(Iter->second.szRFID);
			if (0 <= nRow)
			{
				m_lc_SocketList[nType].Update_Socket_Yield(nRow, &m_pstSocketInfo->GetAt(Iter->second.szRFID));
			}
		}
	}
}

void CWnd_SocketView::Update_SocketStatus_TestResult(__in LPCTSTR IN_szRFID)
{
	if (m_pstSocketInfo)
	{
		//m_lc_SocketList.Update_Socket(IN_szRFID, &m_pstSocketInfo->GetAt(IN_szRFID));
		/*int nRow = Find_SocketOrder(IN_szRFID);
		if (0 <= nRow)
		{
			uint8_t nType = m_pstSocketInfo->GetAt(IN_szRFID).nSocketType;
			m_lc_SocketList[nType].Update_Socket_TestResult(nRow, &m_pstSocketInfo->GetAt(IN_szRFID));
		}*/

		uint8_t nType = m_pstSocketInfo->GetAt(IN_szRFID).nSocketType;
		int nRow = m_lc_SocketList[nType].Find_Socket(IN_szRFID);
		if (0 <= nRow)
		{
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
// Last Update	: 2022/2/15 - 16:58
// Desc.		:
//=============================================================================
void CWnd_SocketView::OnUpdate_SocketData(__in uint16_t nFlag, __in LPCTSTR szRFID)
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
#if (USE_XML)
	if (0 < (nFlag & WM_Socket_LOTID))
		Update_SocketStatus_LOTID(szRFID);
#endif
}

void CWnd_SocketView::OnUpdate_SocketData_All(__in uint16_t nFlag)
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
// Method		: SetPath_Report
// Access		: public  
// Returns		: void
// Parameter	: __in LPCTSTR IN_szReportPath
// Qualifier	:
// Last Update	: 2022/11/10 - 11:23
// Desc.		:
//=============================================================================
void CWnd_SocketView::SetPath_Report(__in LPCTSTR IN_szReportPath)
{
	m_szReportPath = IN_szReportPath;
}


#if (USE_XML)
void CWnd_SocketView::Update_SocketStatus_LOTID(__in LPCTSTR IN_szRFID){
	if (m_pstSocketInfo)	{
		uint8_t nType = m_pstSocketInfo->GetAt(IN_szRFID).nSocketType;
		int nRow = m_lc_SocketList[nType].Find_Socket(IN_szRFID);
		if (0 <= nRow)		{
			m_lc_SocketList[nType].Update_Socket_LOTID(nRow, &m_pstSocketInfo->GetAt(IN_szRFID));
		}
	}
}
#endif