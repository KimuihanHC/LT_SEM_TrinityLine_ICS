//*****************************************************************************
// Filename	: 	Dlg_LossWindow_PopupMenu.cpp
// Created	:	
// Modified	:	2023.04.20 - 15:00
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
// Dlg_LossWindow_PopupMenu.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Dlg_LossWindow_PopupMenu.h"
#include "afxdialogex.h"


#define		IDC_BN_CTRL_FST		1001
#define		IDC_BN_CTRL_LST		IDC_BN_CTRL_FST + 100

// CDlg_PopupMenu 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlg_LossWindow_PopupMenu, CDialogEx)

CDlg_LossWindow_PopupMenu::CDlg_LossWindow_PopupMenu(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlg_LossWindow_PopupMenu::IDD, pParent)
{

	VERIFY(m_font_Default.CreateFont(
		20,						// nHeight
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
		_T("arial")));			// lpszFacename
}

CDlg_LossWindow_PopupMenu::~CDlg_LossWindow_PopupMenu()
{
	// 테마 비활성화가 다른 윈도우에 영향을 주어서 소멸자에서 테마 원복 시킴
	for (auto nIdx = 0; nIdx < enLOSS_WINDOW_Mode::LOSS_WINDOW_MAX; nIdx++)
	{
		m_bn_CtrlCmd[nIdx].EnableWindowsTheming(TRUE);
		m_bn_CtrlCmd[nIdx].m_nFlatStyle = CMFCButton::BUTTONSTYLE_3D;
		m_bn_CtrlCmd[nIdx].m_bTransparent = true;
	}

	m_font_Default.DeleteObject();
}

void CDlg_LossWindow_PopupMenu::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlg_LossWindow_PopupMenu, CDialogEx)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_WINDOWPOSCHANGING()
	ON_COMMAND_RANGE(IDC_BN_CTRL_FST, IDC_BN_CTRL_LST, OnBnClicked_LossWindow)
	ON_WM_TIMER()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CDlg_PopupMenu 메시지 처리기입니다.
//=============================================================================
// Method		: OnCreate
// Access		: public  
// Returns		: int
// Parameter	: LPCREATESTRUCT lpCreateStruct
// Qualifier	:
// Last Update	:	2023.04.20 - 15:00
// Desc.		:
//=============================================================================
int CDlg_LossWindow_PopupMenu::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	//SetBackgroundColor(RGB(0, 0, 0));
	//
	CRect rectDummy;
	DWORD dwStyle = WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	rectDummy.SetRectEmpty();

	m_st_frame.SetBackColor(RGB(255, 255, 255));
	m_st_frame.SetBorderColor_COLORREF(RGB(255, 255, 255));
	m_st_frame.SetBorderTickness(2);
	m_st_frame.Create(_T(""), dwStyle | SS_CENTER | SS_CENTERIMAGE | SS_WHITEFRAME, rectDummy, this, 0);
	m_st_frame.SetFont_Gdip(L"arial", 16.0f, Gdiplus::FontStyleBold);

	CString szFullPath;
	szFullPath.Format(_T("C:\\_System\\Recipe\\%s"), LOSS_INFO_FILENAME);
	UINT nWinID = IDC_BN_CTRL_FST;

	if (StepSeq.LoadXML_LossInfo(szFullPath.GetBuffer(0), Info)) {
		for (auto nIdx = 0; nIdx < Info.Get_Loss_Count(); nIdx++)
		{
			m_bn_CtrlCmd[nIdx].EnableWindowsTheming(FALSE);
			m_bn_CtrlCmd[nIdx].m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT;
			m_bn_CtrlCmd[nIdx].m_bTransparent = false;
			m_bn_CtrlCmd[nIdx].SetFaceColor(RGB(0, 0, 0), true);
			m_bn_CtrlCmd[nIdx].SetTextColor(RGB(255, 255, 255));

			m_bn_CtrlCmd[nIdx].Create(Info.Loss_LIST.at(nIdx).Get_NameLossEng(), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | WS_BORDER, rectDummy, this, nWinID++);
			m_bn_CtrlCmd[nIdx].SetFont(&m_font_Default);
		}
	}

	m_bn_OK.EnableWindowsTheming(FALSE);
	m_bn_OK.m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT;
	m_bn_OK.m_bTransparent = false;
	m_bn_OK.SetFaceColor(RGB(0, 0, 0), true);
	m_bn_OK.SetTextColor(RGB(255, 255, 255));
	m_bn_OK.Create(_T("OK"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | WS_BORDER, rectDummy, this, IDOK);
	m_bn_OK.SetFont(&m_font_Default);

#ifndef USE_DISABLE_NOT_USE_EQUIPMENT_CONTROL
	m_bn_CtrlCmd[CC_Reset].SetTextColor(RGB(255, 255, 0));
#endif
	//m_bn_CtrlCmd[CC_Run].SetTextColor(RGB(0, 200, 0));
	//m_bn_CtrlCmd[CC_Stop].SetTextColor(RGB(200, 0, 0));
	MoveWindow(0, 0, 500, 500);
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
// Last Update	:	2023.04.20 - 15:00
// Desc.		:
//=============================================================================
void CDlg_LossWindow_PopupMenu::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	int nWidthN1 = 2;
	int nAddWidthN1 = 0;
	if ((Info.Get_Loss_Count() % 2) > 0) {
		nAddWidthN1 = 1;
	}
	int iMagrin = 10;
	int iSpacing = 5;
	int iCateSpacing = 10;
	int iCtrlHeight = 40;

	int iLeft = iMagrin;
	int iTop = iMagrin;
	int iWidth = cx - iMagrin - iMagrin;
	int iHeight = cy - iMagrin - iMagrin;
	int iCtrlWidth = (iWidth - iSpacing) / 2;

	int nLeft = 5;
	int nTop = 5;
	int nWidth = cx - 10;
	int nHeight = cy - 10;
	int nBnWidth = (nWidth - 5) / nWidthN1;
	int nLeftSub = nLeft + nBnWidth + 5;

	m_st_frame.MoveWindow(0, 0, cx, cy);


	iCtrlWidth = (iWidth - (iSpacing * 2)) * 2 / 5;
	m_bn_OK.MoveWindow(nLeftSub, nTop, iCtrlWidth, iCtrlHeight);
	
	nTop += iCtrlHeight + iCateSpacing;

	iWidth	 =
	nBnWidth = (nWidth - 5) / nWidthN1;
	nLeftSub = nLeft + nBnWidth + 5;

	int nBnHeight = 
		((nHeight - nTop) - (5 * ((Info.Get_Loss_Count() / nWidthN1) + nAddWidthN1))) / ((Info.Get_Loss_Count() / nWidthN1) + nAddWidthN1);

	
	for (auto nIdx = 0; nIdx < Info.Get_Loss_Count(); ++nIdx)
	{
		m_bn_CtrlCmd[nIdx].MoveWindow(nLeft, nTop, nBnWidth, nBnHeight);

		++nIdx;

		if (Info.Get_Loss_Count() <= nIdx)
			break;

		m_bn_CtrlCmd[nIdx].MoveWindow(nLeftSub, nTop, nBnWidth, nBnHeight);

		nTop += nBnHeight + 5;
	}
}

//=============================================================================
// Method		: OnWindowPosChanging
// Access		: public  
// Returns		: void
// Parameter	: WINDOWPOS * lpwndpos
// Qualifier	:
// Last Update	:	2023.04.20 - 15:00
// Desc.		:
//=============================================================================
void CDlg_LossWindow_PopupMenu::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
	CDialogEx::OnWindowPosChanging(lpwndpos);
}
//=============================================================================
// Method		: OnClose
// Access		: public  
// Returns		: void
// Parameter	:
// Qualifier	:
// Last Update	:	2023.04.20 - 15:00
// Desc.		:
//=============================================================================
void CDlg_LossWindow_PopupMenu::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CDialogEx::OnClose();
}
//=============================================================================
// Method		: OnBnClicked_LossWindow
// Access		: public  
// Returns		: void
// Parameter	: UINT nID
// Qualifier	:
// Last Update	:	2023.04.20 - 15:00
// Desc.		:
//=============================================================================
void CDlg_LossWindow_PopupMenu::OnBnClicked_LossWindow(UINT nID)
{
	nBnIndex = nID;
	nCount[nID - IDC_BN_CTRL_FST]++;
}
//=============================================================================
// Method		: OnInitDialog
// Access		: virtual protected  
// Returns		: BOOL
// Qualifier	:
// Last Update	:	2023.04.20 - 15:00
// Desc.		:
//=============================================================================
BOOL CDlg_LossWindow_PopupMenu::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	int extendedStyle = GetWindowLong(m_hWnd, GWL_EXSTYLE);
	SetWindowLong(m_hWnd, GWL_EXSTYLE, extendedStyle | WS_EX_DLGMODALFRAME);

	// 표시위치 지정
	CenterWindow();

	return TRUE;
}
//=============================================================================
// Method		: OnCancel
// Access		: virtual protected  
// Returns		: void
// Qualifier	:
// Last Update	:	2023.04.20 - 15:00
// Desc.		:
//=============================================================================
void CDlg_LossWindow_PopupMenu::OnCancel()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CDialogEx::OnCancel();
}
//=============================================================================
// Method		: OnOK
// Access		: public
// Returns		: void
// Qualifier	:
// Last Update	:	2023.04.20 - 15:00
// Desc.		:
//=============================================================================
void CDlg_LossWindow_PopupMenu::OnOK()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	static long nCount[100];
	static bool bint = true;
	if (bint) {
		for (auto nIdx = 0; nIdx < Info.Get_Loss_Count(); nIdx++)
		{
			nCount[nIdx] = 0;
		}
		bint = false;
	}
	if (nBnIndex > 0) {
		int nIndex = nBnIndex - IDC_BN_CTRL_FST;
		CString SUBEQPID;
		CString EQUIPMENTSTATE;
		CString LOTID;
		CString LOSSSTATE;
		CString LOSSCOUNT;

		SUBEQPID.Format(_T("%s"), m_Svr->Get_ServerId().GetBuffer());
		//EQUIPMENTSTATE.Format(_T("%s"), ;
		LOTID = "";
		LOSSSTATE.Format(_T("%d"), (nIndex + 1));
		LOSSCOUNT.Format(_T("%d"), ++nCount[nIndex]);
#if TEST
		auto IN_Para = m_Svr->GetXmlEes().Set_ReportLossStateParameter(
			SUBEQPID,
			EQUIPMENTSTATE,
			LOTID,
			LOSSSTATE,
			LOSSCOUNT);
		LPARAM IN_Data = (LPARAM)IN_Para;
		AfxGetMainWnd()->PostMessage(WM_EVENT_SERVER_REPORT_LOSS_STATE, (WPARAM)m_nEqpOrder, (LPARAM)IN_Data);
		m_Svr->Set_Notify_LOSSCODE(Info.Loss_LIST.at(nIndex).Get_CodeLoss());
#endif
		nBnIndex = 0;

		//일단 Clear하도록 함.
		CDialogEx::OnOK();
	}
}

//=============================================================================
// Method		: Set_InitPos
// Access		: public  
// Returns		: void
// Parameter	: __in int x
// Parameter	: __in int y
// Qualifier	:
// Last Update	: 2021/12/27 - 15:59
// Desc.		: 팝업 초기 위치 설정
//=============================================================================
void CDlg_LossWindow_PopupMenu::Set_InitPos(__in int x, __in int y)
{
	m_nInitPos.x = x;
	m_nInitPos.y = y;
}

//=============================================================================
// Method		: Set_Title
// Access		: public  
// Returns		: void
// Parameter	: __in LPCTSTR IN_szTitle
// Qualifier	:
// Last Update	: 2021/12/27 - 16:39
// Desc.		:
//=============================================================================
void CDlg_LossWindow_PopupMenu::Set_Title(__in LPCTSTR IN_szTitle)
{
	m_szTitle = IN_szTitle;
}

//=============================================================================
// Method		: Set_EquipmentID
// Access		: public  
// Returns		: void
// Parameter	: __in LPCTSTR IN_szEqpID
// Parameter	: __in uint8_t IN_nEqpOrder
// Qualifier	:
// Last Update	: 2021/12/27 - 19:00
// Desc.		:
//=============================================================================
void CDlg_LossWindow_PopupMenu::Set_EquipmentID(__in LPCTSTR IN_szEqpID, __in uint8_t IN_nEqpOrder)
{
	m_szEqpID	= IN_szEqpID;
	m_nEqpOrder = IN_nEqpOrder;
}
//=============================================================================
// Method		: Set_ReportRMSMode
// Access		: public  
// Returns		: void
// Parameter	: __in bool IN_bLoader
// Qualifier	:
// Last Update	: 2022/7/27 - 12:02
// Desc.		:
//=============================================================================
void CDlg_LossWindow_PopupMenu::Set_Server(__in CServer * IN_Svr)
{
	m_Svr = IN_Svr;
}



