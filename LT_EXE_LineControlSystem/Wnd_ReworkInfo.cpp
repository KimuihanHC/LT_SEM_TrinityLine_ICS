//*****************************************************************************
// Filename	: 	Wnd_ReworkInfo.cpp
// Created	:	2021/12/22 - 10:39
// Modified	:	2021/12/22 - 10:39
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
// Wnd_ReworkInfo.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Wnd_ReworkInfo.h"
#include "resource.h"


#define		IDC_LC_SOCKETLIST_FST	1000

#define		IDC_ED_BARCOE			1001
#define		IDC_BN_FIND				1002

#define		IDC_BN_SCROLL_UP		1007
#define		IDC_BN_SCROLL_DOWN		1008


//=============================================================================
// CWnd_ReworkInfo
//=============================================================================
IMPLEMENT_DYNAMIC(CWnd_ReworkInfo, CWnd_BaseView)

CWnd_ReworkInfo::CWnd_ReworkInfo()
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

CWnd_ReworkInfo::~CWnd_ReworkInfo()
{
	TRACE(_T("<<< Start ~CWnd_ReworkInfo >>> \n"));
	
	m_font_Default.DeleteObject();
	m_Font.DeleteObject();
}

BEGIN_MESSAGE_MAP(CWnd_ReworkInfo, CWnd_BaseView)
	ON_WM_CREATE()
	ON_WM_SIZE	()
	ON_BN_CLICKED(IDC_BN_FIND,				OnBnClicked_Find)
	ON_BN_CLICKED(IDC_BN_SCROLL_UP,			OnBnClicked_ScrollUp)
	ON_BN_CLICKED(IDC_BN_SCROLL_DOWN,		OnBnClicked_ScrollDown)
	ON_MESSAGE	 (WM_UPDATE_SOCKET_DATA,	OnWM_UpdateReworkData)
END_MESSAGE_MAP()


//=============================================================================
// CWnd_ReworkInfo 메시지 처리기입니다.
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
int CWnd_ReworkInfo::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd_BaseView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	//SetBackgroundColor(RGB(0, 0, 0));

	DWORD dwStyle = WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	CRect rectDummy;
	rectDummy.SetRectEmpty();
	
	/*m_Font.CreateStockObject(DEFAULT_GUI_FONT);*/

	UINT nWindowsID = 1;

	m_lc_ReworkList.Create(dwStyle | WS_VSCROLL, rectDummy, this, IDC_LC_SOCKETLIST_FST);

	m_bn_PgUp.SetImage(IDB_ARROW_UP, IDB_ARROW_UP);
	m_bn_PgDn.SetImage(IDB_ARROW_DOWN, IDB_ARROW_DOWN);
	m_bn_PgUp.Create(_T(""), dwStyle | BS_PUSHBUTTON, rectDummy, this, IDC_BN_SCROLL_UP);
	m_bn_PgDn.Create(_T(""), dwStyle | BS_PUSHBUTTON, rectDummy, this, IDC_BN_SCROLL_DOWN);

	m_st_Find.SetStaticStyle(CVGStatic::StaticStyle_Title_Alt);
	m_st_Find.SetColorStyle(CVGStatic::ColorStyle_Black);
	m_st_Find.SetFont_Gdip(L"Arial", 12.0F);
	m_st_Find.Create(_T("Input Barcode"), dwStyle | SS_CENTER | SS_CENTERIMAGE, rectDummy, this, IDC_STATIC);
	m_ed_Barcode.Create(dwStyle | ES_CENTER | WS_BORDER, rectDummy, this, IDC_ED_BARCOE);
	m_bn_Find.Create(_T("Find Rework JIG"), dwStyle | BS_PUSHBUTTON, rectDummy, this, IDC_BN_FIND);

	m_ed_Barcode.SetFont(&m_Font);
	m_bn_Find.SetFont(&m_font_Default);

	m_ed_Barcode.SetValidChars(NULL);
	m_ed_Barcode.EnableGetMaskedCharsOnly(FALSE);


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
void CWnd_ReworkInfo::OnSize(UINT nType, int cx, int cy)
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
	int iCtrlWidth		= 200;
	int iCtrlHeight		= 35;

	m_st_Find.MoveWindow(iLeft, iTop, 120, iCtrlHeight);
	iLeft += 120 + iSpacing;
	m_ed_Barcode.MoveWindow(iLeft, iTop, iCtrlWidth, iCtrlHeight);
	iLeft += iCtrlWidth + iSpacing;
	m_bn_Find.MoveWindow(iLeft, iTop, iCtrlWidth, iCtrlHeight);

	iLeft = iMargin;
	iTop += iCtrlHeight + iSpacing;
	iHeight = iHeight - iCtrlHeight - iSpacing;
	iWidth -= (40 + iSpacing);
	m_lc_ReworkList.MoveWindow(iLeft, iTop, iWidth, iHeight);

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
void CWnd_ReworkInfo::OnBnClicked_Find()
{
	/*CString szText;

	int iSelType = m_cb_ReworkType.GetCurSel();
	if (0 <= iSelType)
	{
		m_ed_Barcode.GetWindowText(szText);

		if (m_lc_ReworkList[iSelType].Find_Rework(szText) < 0)
		{
			//LT_MessageBox(_T("No socket found."));
			LT_MessageBox(g_szMessageBox_T[MB_Socket_NoFound][m_nLanguage]);
		}
	}*/
}

void CWnd_ReworkInfo::OnBnClicked_ScrollUp()
{
	// 리스트 page up
	m_lc_ReworkList.SendMessage(WM_VSCROLL, MAKEWPARAM(SB_PAGEUP, 0), (LPARAM)0);
}

void CWnd_ReworkInfo::OnBnClicked_ScrollDown()
{
	// 리스트 page down
	m_lc_ReworkList.SendMessage(WM_VSCROLL, MAKEWPARAM(SB_PAGEDOWN, 0), (LPARAM)0);
}

//=============================================================================
// Method		: OnWM_UpdateReworkData
// Access		: protected  
// Returns		: LRESULT
// Parameter	: WPARAM wParam
// Parameter	: LPARAM lParam
// Qualifier	:
// Last Update	: 2022/1/26 - 17:38
// Desc.		:
//=============================================================================
LRESULT CWnd_ReworkInfo::OnWM_UpdateReworkData(WPARAM wParam, LPARAM lParam)
{
	uint16_t nFlag = (uint16_t)wParam;
	CString szBarcode = (LPCTSTR)lParam;

	OnUpdate_ReworkData(nFlag, szBarcode.GetBuffer());

	return 1;
}

//=============================================================================
// Method		: Init_ReworkUI_List
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/12/22 - 10:39
// Desc.		:
//=============================================================================
void CWnd_ReworkInfo::Init_ReworkUI_List()
{
	if (m_pstReworkInfo)
	{
		m_lc_ReworkList.Clear_ItemList();

		size_t nCnt = m_pstReworkInfo->m_ReworkInfo.size();

		if (0 < nCnt)
		{
			m_lc_ReworkList.Set_ItemList(m_pstReworkInfo);
		}
	}
}

//=============================================================================
// Method		: Find_ReworkOrder
// Access		: protected  
// Returns		: int
// Parameter	: __in LPCTSTR IN_szBarcode
// Qualifier	:
// Last Update	: 2021/12/22 - 11:43
// Desc.		:
//=============================================================================
int CWnd_ReworkInfo::Find_ReworkOrder(__in LPCTSTR IN_szBarcode)
{
	int nRetIndex = -1;

	if (m_pstReworkInfo)
	{
		int nCount = 0;

		auto eIter = m_pstReworkInfo->m_ReworkInfo.end();
		auto Iter = m_pstReworkInfo->m_ReworkInfo.begin();
		while (Iter != eIter)
		{
			if (0 == Iter->first.compare(IN_szBarcode))
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
// Method		: Set_PermissionMode
// Access		: public  
// Returns		: void
// Parameter	: enPermissionMode InspMode
// Qualifier	:
// Last Update	: 2021/12/22 - 10:39
// Desc.		:
//=============================================================================
void CWnd_ReworkInfo::Set_PermissionMode(enPermissionMode InspMode)
{
	switch (InspMode)
	{
	case Permission_Operator:
		m_InspMode = Permission_Operator;
		break;

	case Permission_Engineer:
		break;

	case Permission_Administrator:
		m_InspMode = Permission_Administrator;
		break;

	default:
		break;
	}
}

//=============================================================================
// Method		: Update_ReworkInfo
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/12/22 - 10:39
// Desc.		:
//=============================================================================
void CWnd_ReworkInfo::Update_ReworkInfo()
{
 	Init_ReworkUI_List();
}

//=============================================================================
// Method		: OnUpdate_ReworkData
// Access		: public  
// Returns		: void
// Parameter	: __in uint16_t nFlag
// Parameter	: __in LPCTSTR szBarcode
// Qualifier	:
// Last Update	: 2022/2/15 - 16:58
// Desc.		:
//=============================================================================
void CWnd_ReworkInfo::OnUpdate_ReworkData(__in uint16_t nFlag, __in LPCTSTR szBarcode)
{
	
}

void CWnd_ReworkInfo::OnUpdate_ReworkData_All(__in uint16_t nFlag)
{
	
}
