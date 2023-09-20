//*****************************************************************************
// Filename	: 	Wnd_MaintenanceView.cpp
// Created	:	2016/7/5 - 16:18
// Modified	:	2016/7/5 - 16:18
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
// Wnd_MaintenanceView.cpp : implementation file
//

#include "stdafx.h"
#include "Wnd_MaintenanceView.h"
#include "resource.h"
#include "Dlg_ChkPassword.h"


#define IDC_BN_RESORE_RECIPE	1001
#define IDC_BN_START_INIT		2001
#define IDC_BN_START_FINAL		2002
#define IDC_BN_LOAD_INITIAL		2003
#define IDC_BN_LOAD_FINAL		2004

// CWnd_MaintenanceView


IMPLEMENT_DYNAMIC(CWnd_MaintenanceView, CWnd_BaseView)

CWnd_MaintenanceView::CWnd_MaintenanceView()
{
	VERIFY(m_font_Default.CreateFont(
		24,						// nHeight
		0,						// nWidth
		0,						// nEscapement
		0,						// nOrientation
		FW_HEAVY,				// nWeight
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

	m_InspMode = Permission_Operator;
	m_pstSysInfo = NULL;

}

CWnd_MaintenanceView::~CWnd_MaintenanceView()
{
	TRACE(_T("<<< Start ~CWnd_MaintenanceView >>> \n"));

	m_font_Default.DeleteObject();
	m_Font.DeleteObject();
}


BEGIN_MESSAGE_MAP(CWnd_MaintenanceView, CWnd_BaseView)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CWnd_MaintenanceView message handlers
//=============================================================================
// Method		: OnCreate
// Access		: public  
// Returns		: int
// Parameter	: LPCREATESTRUCT lpCreateStruct
// Qualifier	:
// Last Update	: 2016/5/16 - 19:04
// Desc.		:
//=============================================================================
int CWnd_MaintenanceView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd_BaseView::OnCreate(lpCreateStruct) == -1)
		return -1;

	//SetBackgroundColor(RGB(0, 0, 0));

	DWORD dwStyle = WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	/*m_Font.CreateStockObject(DEFAULT_GUI_FONT);*/

	UINT nWindowsID = 1;

// 	m_st_frameDevice.SetColorStyle(CVGStatic::ColorStyle_White);
// 	m_st_frameDevice.Create(_T(""), dwStyle | SS_CENTER | SS_CENTERIMAGE | SS_BLACKFRAME, rectDummy, this, IDC_STATIC);
// 	//m_st_frameChannel.Create(_T(""), dwStyle | SS_CENTER | SS_CENTERIMAGE | SS_BLACKFRAME, rectDummy, this, IDC_STATIC);
// 	m_st_frameStepInfo.Create(_T(""), dwStyle | SS_CENTER | SS_CENTERIMAGE | SS_BLACKFRAME, rectDummy, this, IDC_STATIC);

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
// Last Update	: 2016/5/16 - 19:04
// Desc.		:
//=============================================================================
void CWnd_MaintenanceView::OnSize(UINT nType, int cx, int cy)
{
	CWnd_BaseView::OnSize(nType, cx, cy);


	if ((cx == 0) && (cy == 0))
		return;

	int iMagrin			= 5;
	int iSpacing		= 5;
	int iCateSpacing	= 5;

	int iLeft			= iMagrin;
	int iTop			= iMagrin;
	int iWidth			= cx - iMagrin - iMagrin;
	int iHeight			= cy - iMagrin - iMagrin;

	int iWidth_Ctrl		= 300;
	int iHeight_Ctrl	= 25;
	int iHeight_Capture	= (iHeight_Ctrl * 2) + iSpacing;
	int iHeight_Log		= 100;
	int iHeight_Final	= 240;
	int iHeight_Init	= iHeight - (iSpacing * 4) - iHeight_Ctrl - iHeight_Capture - iHeight_Log - iHeight_Final;
	
	
	MoveWindow_Initial(iLeft, iTop, iWidth, iHeight_Init);
	iTop += iHeight_Init + iSpacing;

	MoveWindow_Capture(iLeft, iTop, iWidth, iHeight_Capture);
	iTop += iHeight_Capture + iSpacing;

	MoveWindow_Final(iLeft, iTop, iWidth, iHeight_Final);
	iTop += iHeight_Final + iSpacing;

}

//=============================================================================
// Method		: MoveWindow_Info
// Access		: protected  
// Returns		: void
// Parameter	: int x
// Parameter	: int y
// Parameter	: int nWidth
// Parameter	: int nHeight
// Parameter	: BOOL bRepaint
// Qualifier	:
// Last Update	: 2016/5/17 - 11:32
// Desc.		:
//=============================================================================
void CWnd_MaintenanceView::MoveWindow_Initial(int x, int y, int nWidth, int nHeight, BOOL bRepaint /*= TRUE*/)
{
	int iSpacing	= 5;
	int iLeft		= x;
	int iTop		= y;
	int iCtrlWidth	= __min(200, (nWidth - iSpacing) / 2);
	int iCtrlHeight = 25;
	int iListHeight = nHeight - iSpacing - iCtrlHeight;
	int iLeft_Sub	= x + nWidth - iCtrlWidth;

	//m_st_Judgment.MoveWindow(iLeft, iTop, iStatusWidth, iStatusHeight);

}

//=============================================================================
// Method		: MoveWindow_Status
// Access		: protected  
// Returns		: void
// Parameter	: int x
// Parameter	: int y
// Parameter	: int nWidth
// Parameter	: int nHeight
// Parameter	: BOOL bRepaint
// Qualifier	:
// Last Update	: 2016/5/17 - 11:32
// Desc.		:
//=============================================================================
void CWnd_MaintenanceView::MoveWindow_Capture(int x, int y, int nWidth, int nHeight, BOOL bRepaint /*= TRUE*/)
{
	int iSpacing	= 5;
	int iLeft		= x;
	int iTop		= y;
	int iCtrlWidth	= __min(200, (nWidth - iSpacing) / 2);
	int iCtrlHeight = 25;
	int iListHeight = nHeight - iSpacing - iCtrlHeight;
	int iLeft_Sub	= iLeft + iCtrlWidth + iSpacing;

}

//=============================================================================
// Method		: MoveWindow_Final
// Access		: protected  
// Returns		: void
// Parameter	: int x
// Parameter	: int y
// Parameter	: int nWidth
// Parameter	: int nHeight
// Parameter	: BOOL bRepaint
// Qualifier	:
// Last Update	: 2021/4/30 - 12:28
// Desc.		:
//=============================================================================
void CWnd_MaintenanceView::MoveWindow_Final(int x, int y, int nWidth, int nHeight, BOOL bRepaint /*= TRUE*/)
{
	int iSpacing	= 5;
	int iLeft		= x;
	int iTop		= y;
	int iCtrlWidth	= __min(200, (nWidth - iSpacing) / 2);
	int iCtrlHeight = 25;
	int iListHeight = nHeight - iSpacing - iCtrlHeight;
	int iLeft_Sub	= x + nWidth - iCtrlWidth;

}

//=============================================================================
// Method		: SetInspectionMode
// Access		: public  
// Returns		: void
// Parameter	: enum_Inspection_Mode InspMode
// Qualifier	:
// Last Update	: 2016/1/13 - 14:18
// Desc.		:
//=============================================================================
void CWnd_MaintenanceView::Set_PermissionMode(enPermissionMode InspMode)
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
