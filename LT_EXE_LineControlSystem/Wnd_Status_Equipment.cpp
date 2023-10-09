//*****************************************************************************
// Filename	: 	Wnd_Status_Equipment.cpp
// Created	:	2016/3/14 - 10:57
// Modified	:	2016/3/14 - 10:57
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
// Wnd_Status_Equipment.cpp : implementation file
//

#include "stdafx.h"
#include "Wnd_Status_Equipment.h"
#include "Def_Equipment_Type.h"
#include "Dlg_PopupMenu.h"

#define		IDC_RB_STATUS		1001
#define		IDC_RB_SOCKET		1002
#define		IDC_BN_POPUPMENU	1003

//-----------------------------------------------------------------------------
// CWnd_Status_Equipment
//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CWnd_Status_Equipment, CWnd_Status_Base)

CWnd_Status_Equipment::CWnd_Status_Equipment()
{
// 	m_sizeCaption.SetSize(120, 20);
// 	m_sizeData.SetSize(160, 20);
// 	m_Margin.left	= 0;
// 	m_Margin.top	= 0;
// 	m_Margin.right	= 0;
// 	m_Margin.bottom	= 0;
// 	m_nSpacing		= 5;
// 	m_nCateSpacing	= 10;
// 
 	m_nFontSize_Caption.SetSize(0, 11);
 	m_nFontSize_Data.SetSize(0, 11);

	//Create_Font();
}

CWnd_Status_Equipment::~CWnd_Status_Equipment()
{
	Release_Font();

}

BEGIN_MESSAGE_MAP(CWnd_Status_Equipment, CWnd_Status_Base)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BN_POPUPMENU, OnBnClicked_Control)
	ON_MESSAGE(WM_EQUIPMENT_NOTIFY, OnWmEquipmentNotify)
END_MESSAGE_MAP()


// CWnd_Status_Equipment message handlers
//=============================================================================
// Method		: OnCreate
// Access		: public  
// Returns		: int
// Parameter	: LPCREATESTRUCT lpCreateStruct
// Qualifier	:
// Last Update	: 2016/3/17 - 10:04
// Desc.		:
//=============================================================================
int CWnd_Status_Equipment::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
 	DWORD dwStyle = WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
 	CRect rectDummy;
 	rectDummy.SetRectEmpty();

	// 설비 제어 버튼 ----------------------------
	m_bn_Control.Create(_T("Ctrl"), WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | BS_PUSHBUTTON, rectDummy, this, IDC_BN_POPUPMENU);
	
	// 경광등 ------------------------------------
	for (auto nIdx = 0; nIdx < enLED_Type::LED_MaxCount; ++nIdx)
	{
		m_st_LED[nIdx].Create(_T(""), dwStyle, rectDummy, this, IDC_STATIC);
	}

	Set_LED_Status(LED_Off, 0);

	// 설비 상태 정보 ----------------------------
	m_st_EqpName		.SetFont_Gdip(L"Arial", 8.0F);
	m_st_EqpID			.SetFont_Gdip(L"Arial", 8.0F);
	m_st_IpAddress		.SetFont_Gdip(L"Arial", 8.0F);
	m_st_OperMode		.SetFont_Gdip(L"Arial", 8.0F);

	m_st_EqpName		.Create(_T(""), dwStyle /*| WS_BORDER*/ | SS_CENTER, rectDummy, this, IDC_STATIC);
	m_st_EqpID			.Create(_T(""), dwStyle /*| WS_BORDER*/ | SS_CENTER, rectDummy, this, IDC_STATIC);
	m_st_IpAddress		.Create(_T(""), dwStyle /*| WS_BORDER*/ | SS_CENTER, rectDummy, this, IDC_STATIC);
	m_st_OperMode		.Create(_T("Auto"), dwStyle /*| WS_BORDER*/ | SS_CENTER, rectDummy, this, IDC_STATIC);

#ifdef USE_EQP_TACTTIME
	for (auto nIdx = 0; nIdx < TT_MaxCount; ++nIdx)
	{
		m_st_Tacttime[nIdx].SetFont_Gdip(L"Arial", 8.0F);
		m_st_Tacttime[nIdx].Create(_T(""), dwStyle /*| WS_BORDER*/ | SS_CENTER, rectDummy, this, IDC_STATIC);
	}

	m_st_Tacttime[TT_Curr].SetText(_T("Time"));
	m_st_Tacttime[TT_Ave].SetText(_T("T Ave"));
#endif

	// 수율
	m_wnd_Yield.Create(NULL, _T(""), dwStyle, rectDummy, this, 100);

	// 타이틀바, 외곽선
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;

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
// Last Update	: 2016/3/17 - 10:04
// Desc.		:
//=============================================================================
void CWnd_Status_Equipment::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);


	int iMargin			= 5;
	int iSpacing		= 5;
	int iLeft			= 5;
	int iTop			= m_nTitle_Height + iMargin;
	int iWidth			= cx - iMargin - iMargin;
	int iHeight			= cy - iMargin - iMargin;
	int iBnHeight		= m_nTitle_Height - 6;
	int iBnWidth		= iBnHeight;

	// 설비 제어 버튼 ----------------------------
	iTop = 3;
	iLeft = cx - iMargin - iBnWidth;
	m_bn_Control.MoveWindow(iLeft, iTop, iBnWidth, iBnHeight);

	iTop = m_nTitle_Height + iSpacing;
	MoveWindow_Status(iLeft, iTop, iWidth, iHeight);

}

//=============================================================================
// Method		: OnWmEquipmentNotify
// Access		: protected  
// Returns		: LRESULT
// Parameter	: WPARAM wParam
// Parameter	: LPARAM lParam
// Qualifier	:
// Last Update	: 2022/1/12 - 20:11
// Desc.		:
//=============================================================================
LRESULT CWnd_Status_Equipment::OnWmEquipmentNotify(WPARAM wParam, LPARAM lParam)
{
// 	typedef enum
// 	{
// 		WM_EqpNotify_ClientConnection,		// 
// 		WM_EqpNotify_VerifyEqpConnection,	// 
// 		WM_EqpNotify_PortClear,				// 
// 		WM_EqpNotify_PortStatus,			// 
// 		WM_EqpNotify_ConveyorStatus,		// 
// 		WM_EqpNotify_OperatingMode,			// 
// 		WM_EqpNotify_ProcessStatus,			// 
// 		WM_EqpNotify_SocketRequest,			// 
// 	}WM_Eqp_Nofity_Para;

	WM_Eqp_Nofity_Para nPara = (WM_Eqp_Nofity_Para)wParam;

	switch (nPara)
	{
	case WM_EqpNotify_ClientConnection:
		Set_ClientConnection((uint8_t)lParam);
		break;

	case WM_EqpNotify_VerifyEqpConnection:
		Set_VerifyEqpConnection((bool)lParam);
		break;

	case WM_EqpNotify_PortClear:
		Set_PortClear((uint8_t)lParam);
		break;

	case WM_EqpNotify_PortStatus:
		Set_PortStatus((uint8_t)LOWORD(lParam), (uint8_t)HIWORD(lParam));
		break;

	case WM_EqpNotify_ConveyorStatus:
		Set_ConveyorStatus((uint8_t)LOWORD(lParam), (uint8_t)LOBYTE(HIWORD(lParam)), (bool)HIBYTE(HIWORD(lParam)));
		break;

	case WM_EqpNotify_OperatingMode:
		Set_OperatingMode((uint8_t)lParam);
		break;

	case WM_EqpNotify_ProcessStatus:
		Set_ProcessStatus((uint8_t)lParam);
		break;

	case WM_EqpNotify_Skip:
		Set_Skip((bool)lParam);
		break;

	case WM_EqpNotify_TimeSync:
		Set_TimeSync((bool)lParam);
		break;

	case WM_EqpNotify_Yield:
		Set_Yield((const CYield_Equipment*)lParam);
		break;

	case WM_EqpNotify_Production:
		Set_Production((bool)LOWORD(lParam), (uint8_t)HIWORD(lParam));
		break;
#if defined(EES_XML)//20231003
	case WM_EqpNotify_EQUIPMENTSTATE:
		Set_EqpNotify_EQUIPMENTSTATE((lt::Report_Equipment_State_Args::Args*)lParam);
		break;
	case WM_EqpNotify_EQUIPMENTSTATEDISPLAY:
		Set_EqpNotify_EQUIPMENTSTATEDISPLAY((CCommonModule *)lParam);
		break;
	case WM_EqpNotify_RGBDISPLAY:
		Set_EqpNotify_RGBDISPLAY((lt::Request_Equipment_State_Display_Args::Args&)lParam);
		break;
#endif	//ADD_SOCKET_EES_XML
	default:
		break;
	}


	return 1;
}

//=============================================================================
// Method		: MoveWindow_Status
// Access		: protected  
// Returns		: void
// Parameter	: int x
// Parameter	: int y
// Parameter	: int nWidth
// Parameter	: int nHeight
// Qualifier	:
// Last Update	: 2021/12/16 - 17:54
// Desc.		:
//=============================================================================
void CWnd_Status_Equipment::MoveWindow_Status(int x, int y, int nWidth, int nHeight)
{
	int iMargin			= 5;
	int iSpacing		= 5;
	int iLeft			= iMargin;
	int iTop			= m_nTitle_Height + iMargin;
	int iWidth			= nWidth;
	int iHeight			= nHeight;
	int iLedWidth		= (iWidth - (iSpacing * (enLED_Type::LED_MaxCount - 1))) / enLED_Type::LED_MaxCount;
	
	// 경광등 ------------------------------------
	iLeft = iMargin;
	iTop = m_nTitle_Height + iMargin;
	for (auto nIdx = 0; nIdx < enLED_Type::LED_MaxCount; ++nIdx)
	{
		m_st_LED[nIdx].MoveWindow(iLeft, iTop, iLedWidth, m_nCtrl_Height);
		iLeft += iLedWidth + iSpacing;
	}

	iTop += m_nCtrl_Height + iSpacing;

	// 설비 상태 정보 ----------------------------
	iLeft = iMargin;
	m_st_EqpName.MoveWindow(iLeft, iTop, iWidth, m_nCtrl_Height);
	iTop += m_nCtrl_Height - 1;
// 	m_st_EqpID.MoveWindow(iLeft, iTop, iWidth, m_nCtrl_Height);
// 	iTop += m_nCtrl_Height - 1;
	m_st_IpAddress.MoveWindow(iLeft, iTop, iWidth, m_nCtrl_Height);
	iTop += m_nCtrl_Height - 1;
	m_st_OperMode.MoveWindow(iLeft, iTop, iWidth, m_nCtrl_Height);
#if defined(EES_XML)//20231003
	iTop += (m_nCtrl_Height/2) + iSpacing;
#else	
	iTop += m_nCtrl_Height + iSpacing;
#endif	
	m_nTop_Status = iTop;
}

//=============================================================================
// Method		: MoveWindow_Tester
// Access		: protected  
// Returns		: void
// Parameter	: int x
// Parameter	: int y
// Parameter	: int nWidth
// Parameter	: int nHeight
// Qualifier	:
// Last Update	: 2021/12/2 - 16:48
// Desc.		:
//=============================================================================
void CWnd_Status_Equipment::MoveWindow_Tester(int x, int y, int nWidth, int nHeight)
{
	

}

//=============================================================================
// Method		: MoveWindow_Loader
// Access		: protected  
// Returns		: void
// Parameter	: int x
// Parameter	: int y
// Parameter	: int nWidth
// Parameter	: int nHeight
// Qualifier	:
// Last Update	: 2021/12/2 - 16:48
// Desc.		:
//=============================================================================
void CWnd_Status_Equipment::MoveWindow_Loader(int x, int y, int nWidth, int nHeight)
{
	
}

//=============================================================================
// Method		: MoveWindow_Unloader
// Access		: protected  
// Returns		: void
// Parameter	: int x
// Parameter	: int y
// Parameter	: int nWidth
// Parameter	: int nHeight
// Qualifier	:
// Last Update	: 2021/12/2 - 16:48
// Desc.		:
//=============================================================================
void CWnd_Status_Equipment::MoveWindow_Unloader(int x, int y, int nWidth, int nHeight)
{
	
}

//=============================================================================
// Method		: MoveWindow_Returner
// Access		: protected  
// Returns		: void
// Parameter	: int x
// Parameter	: int y
// Parameter	: int nWidth
// Parameter	: int nHeight
// Qualifier	:
// Last Update	: 2021/12/2 - 16:48
// Desc.		:
//=============================================================================
void CWnd_Status_Equipment::MoveWindow_Returner(int x, int y, int nWidth, int nHeight)
{

}

//=============================================================================
// Method		: Create_Font
// Access		: protected  
// Returns		: virtual void
// Qualifier	:
// Last Update	: 2021/3/4 - 16:49
// Desc.		:
//=============================================================================
void CWnd_Status_Equipment::Create_Font()
{
	VERIFY(m_font_Default.CreateFont(
		m_nFontSize_Caption.cy,	// nHeight
		m_nFontSize_Caption.cx,	// nWidth
		0,						// nEscapement
		0,						// nOrientation
		FW_NORMAL,				// nWeight
		FALSE,					// bItalic
		FALSE,					// bUnderline
		0,						// cStrikeOut
		DEFAULT_CHARSET,		// nCharSet
		OUT_DEFAULT_PRECIS,		// nOutPrecision
		CLIP_DEFAULT_PRECIS,	// nClipPrecision
		DEFAULT_QUALITY,		// nQuality
		DEFAULT_PITCH,			// nPitchAndFamily
		_T("Tahoma")));			// lpszFacename

	VERIFY(m_font_Data.CreateFont(
		m_nFontSize_Data.cy,	// nHeight
		m_nFontSize_Data.cx,	// nWidth
		0,						// nEscapement
		0,						// nOrientation
		FW_NORMAL,				// nWeight
		FALSE,					// bItalic
		FALSE,					// bUnderline
		0,						// cStrikeOut
		ANSI_CHARSET,			// nCharSet
		OUT_DEFAULT_PRECIS,		// nOutPrecision
		CLIP_DEFAULT_PRECIS,	// nClipPrecision
		ANTIALIASED_QUALITY,	// nQuality
		DEFAULT_PITCH,			// nPitchAndFamily
		_T("Tahoma")));			// lpszFacename
	
}

//=============================================================================
// Method		: Release_Font
// Access		: protected  
// Returns		: virtual void
// Qualifier	:
// Last Update	: 2021/3/4 - 16:49
// Desc.		:
//=============================================================================
void CWnd_Status_Equipment::Release_Font()
{
	m_font_Default.DeleteObject();
	m_font_Data.DeleteObject();
}

//=============================================================================
// Method		: OnBnClicked_Control
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2022/1/17 - 18:03
// Desc.		:
//=============================================================================
void CWnd_Status_Equipment::OnBnClicked_Control()
{
	Popup_SubMenu();
}

//=============================================================================
// Method		: Popup_SubMenu
// Access		: virtual protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2022/1/17 - 18:03
// Desc.		:
//=============================================================================
void CWnd_Status_Equipment::Popup_SubMenu()
{
	CDlg_PopupMenu dlgMenu(this);	

	CRect rtClient;
	GetClientRect(&rtClient);
	ClientToScreen(rtClient);

	dlgMenu.Set_InitPos(rtClient.left + 50, rtClient.top + 50);
	if (m_pEquipmentStatus)
	{
		dlgMenu.Set_Title(m_pEquipmentStatus->Get_Alias());
		dlgMenu.Set_EquipmentID(m_pEquipmentStatus->Get_EquipmentId(), m_nEqpOrder);		
		dlgMenu.Set_Loader(m_pEquipmentStatus->Is_Loader());
	}
	dlgMenu.DoModal();
}

//=============================================================================
// Method		: SetUI_PortStatus
// Access		: protected  
// Returns		: void
// Parameter	: __in CVGStatic * IN_pwndPort
// Parameter	: __in uint8_t IN_nStatus
// Qualifier	:
// Last Update	: 2021/12/24 - 15:06
// Desc.		:
//=============================================================================
void CWnd_Status_Equipment::SetUI_PortStatus(__in CVGStatic* IN_pwndPort, __in uint8_t IN_nStatus)
{
	if (IN_pwndPort)
	{
		switch (IN_nStatus)
		{
		case PtS_Empty: // 하양
			IN_pwndPort->SetBackColor_COLORREF(RGB(255, 255, 255));
			break;

		case PtS_Exist_Socket: // 초록
			IN_pwndPort->SetBackColor_COLORREF(RGB(0, 255, 0));
			break;

		case PtS_Wait_Out: // 노랑
			IN_pwndPort->SetBackColor_COLORREF(RGB(255, 232, 00));
			break;

		case PtS_Disable: // 회색
			IN_pwndPort->SetBackColor_COLORREF(RGB(180, 180, 180));
			break;

		case PtS_Alarm:	// 빨강
			IN_pwndPort->SetBackColor_COLORREF(RGB(255, 80, 80));
			break;

		default:
			break;
		}

		CString szText;
		szText.Format(_T("%d"), IN_nStatus);
		IN_pwndPort->SetText(szText.GetBuffer());
	}
}

void CWnd_Status_Equipment::SetUI_PortStatus(__in CVGStatic* IN_pwndPort, __in const ST_PortStatus* IN_nStatus)
{
	SetUI_PortStatus(IN_pwndPort, IN_nStatus->nStatus);

	IN_nStatus->szRfid;
	IN_nStatus->szBarcode;
}

//=============================================================================
// Method		: SetUI_ConveyorStatus
// Access		: protected  
// Returns		: void
// Parameter	: __in CVGStatic * IN_pwndConveyor
// Parameter	: __in uint8_t IN_nStatus
// Parameter	: __in uint8_t IN_nExistSocket
// Qualifier	:
// Last Update	: 2022/1/13 - 14:47
// Desc.		:
//=============================================================================
void CWnd_Status_Equipment::SetUI_ConveyorStatus(__in CVGStatic* IN_pwndConveyor, __in uint8_t IN_nStatus, __in uint8_t IN_nExistSocket)
{
	if (IN_pwndConveyor)
	{
		switch (IN_nStatus)
		{
		case CoS_Stop: // 회색
			IN_pwndConveyor->SetBackColor_COLORREF(RGB(230, 230, 230));
			break;

		case CoS_Run:	// 초록
			IN_pwndConveyor->SetBackColor_COLORREF(RGB(146, 208, 80));
			break;

		case CoS_Alarm:	// 빨강
			IN_pwndConveyor->SetBackColor_COLORREF(RGB(255, 80, 80));
			break;

		default:
			break;
		}

		CString szText;
		szText.Format(_T("%d : %d"), IN_nStatus, IN_nExistSocket);
		IN_pwndConveyor->SetText(szText.GetBuffer());
	}	
}

void CWnd_Status_Equipment::SetUI_ConveyorStatus(__in CVGStatic* IN_pwndConveyor, __in const ST_ConveyorStatus* IN_nStatus)
{
	SetUI_ConveyorStatus(IN_pwndConveyor, IN_nStatus->nStatus, IN_nStatus->nExistSocket);

	IN_nStatus->szRfid;
	IN_nStatus->szBarcode;
}

//=============================================================================
// Method		: Set_PermissionMode
// Access		: public  
// Returns		: void
// Parameter	: __in enPermissionMode IN_PermissionMode
// Qualifier	:
// Last Update	: 2022/1/17 - 16:29
// Desc.		:
//=============================================================================
void CWnd_Status_Equipment::Set_PermissionMode(__in enPermissionMode IN_PermissionMode)
{
	switch (IN_PermissionMode)
	{
	case enPermissionMode::Permission_Operator:
		m_bn_Control.ShowWindow(SW_HIDE);
		break;

	case enPermissionMode::Permission_Engineer:
		m_bn_Control.ShowWindow(SW_SHOW);
		break;

	case enPermissionMode::Permission_Administrator:
		m_bn_Control.ShowWindow(SW_SHOW);
		break;

	default:
		break;
	}
}

//=============================================================================
// Method		: SetPtr_EquipmentInfo
// Access		: public  
// Returns		: void
// Parameter	: __in uint8_t IN_nEqpNo
// Parameter	: const CEquipment * IN_pEquipment
// Qualifier	:
// Last Update	: 2021/12/29 - 14:02
// Desc.		:
//=============================================================================
void CWnd_Status_Equipment::SetPtr_EquipmentInfo(__in uint8_t IN_nEqpNo, const CEquipment* IN_pEquipment)
{
	m_pEquipmentStatus = IN_pEquipment;

	if (m_pEquipmentStatus->Is_Loader())
	{
		m_wnd_Yield.Set_EqpLoader(true);
	}
#if defined(EES_XML)//20231003
	auto test = m_pEquipmentStatus->Get_mEES_PortSubStatus();
#endif
	// 설비 순서
	m_nEqpOrder = IN_nEqpNo;

	// 사용하는 Port 갯수
	m_nPortCount	 = static_cast<uint8_t>(m_pEquipmentStatus->Get_PortCount());
	m_nConveyorCount = static_cast<uint8_t>(m_pEquipmentStatus->Get_ConveyorCount());

	// 검사 파라 개수 설정
	m_wnd_Yield.Set_TestParaCount(m_pEquipmentStatus->Get_TestPortCount());

	if (GetSafeHwnd())
	{
		// 설비 순서
		CString szText;
		szText.Format(_T("No. %02d"), m_nEqpOrder);
		SetText_Title(szText.GetBuffer());

		// 설비 명칭
		m_st_EqpName.SetText(g_szEqpTypeName[IN_pEquipment->Get_EquipmentType()]);

		// IP Address
		DWORD dwAddress = ntohl(IN_pEquipment->Get_IP_Address());
		CString szTemp;
		szTemp.Format(_T("%d.%d.%d.%d"), FOURTH_IPADDRESS(dwAddress), THIRD_IPADDRESS(dwAddress), SECOND_IPADDRESS(dwAddress), FIRST_IPADDRESS(dwAddress));
		m_st_IpAddress.SetWindowText(szTemp.GetBuffer());

		// Equipment ID
		m_st_EqpID.SetWindowText(IN_pEquipment->Get_EquipmentId());

		// UI 다시 그리기
		CRect rc;
		GetClientRect(rc);
		SendMessage(WM_SIZE, (WPARAM)SIZE_RESTORED, MAKELPARAM(rc.Width(), rc.Height()));
	}
}

//=============================================================================
// Method		: Set_EnableEquipment
// Access		: public  
// Returns		: void
// Parameter	: __in bool IN_bEnable
// Qualifier	:
// Last Update	: 2021/12/27 - 20:21
// Desc.		:
//=============================================================================
void CWnd_Status_Equipment::Set_EnableEquipment(__in bool IN_bEnable)
{
	if (IN_bEnable)
	{
		if (m_pEquipmentStatus->Get_Skip())
		{
			Set_Skip(true);
		}
		else
		{
			m_st_Border.SetBackColor_COLORREF(RGB(255, 255, 255));

			m_wnd_Yield.Set_EnableWindow(IN_bEnable);
		}
	}
	else
	{
		m_st_Border.SetBackColor_COLORREF(RGB(100, 100, 100));

		m_wnd_Yield.Set_EnableWindow(IN_bEnable);
	}
}

//=============================================================================
// Method		: Set_Skip
// Access		: public  
// Returns		: void
// Parameter	: __in bool IN_bSkip
// Qualifier	:
// Last Update	: 2022/1/17 - 15:45
// Desc.		:
//=============================================================================
void CWnd_Status_Equipment::Set_Skip(__in bool IN_bSkip)
{
	if (m_pEquipmentStatus->Get_EnableEquipment())
	{
		if (IN_bSkip)
		{
			// 설비 Enable, Skip 상태
			m_st_Border.SetBackColor_COLORREF(RGB(130, 100, 100));
		}
		else
		{
			Set_EnableEquipment(true);
		}

		m_wnd_Yield.Set_SkipWindow(IN_bSkip);
	}
	else
	{

	}
}

//=============================================================================
// Method		: Set_ClientConnection
// Access		: public  
// Returns		: void
// Parameter	: __in uint8_t IN_nConStatus
// Qualifier	:
// Last Update	: 2021/12/27 - 10:21
// Desc.		:
//=============================================================================
void CWnd_Status_Equipment::Set_ClientConnection(__in uint8_t IN_nConStatus)
{
	//m_st_IpAddress.SetBackColor_COLORREF(RGB(255, 255, 255));
	switch (IN_nConStatus)
	{
	case 0: // 연결 안됨 (빨강)
		m_st_Title.SetBackColor_COLORREF(RGB(192, 0, 0));
		//m_st_Title.SetColorStyle(CVGStatic::ColorStyle_Red);
		m_st_EqpName.SetBackColor_COLORREF(RGB(255, 255, 255));
		break;

	case 1: // 연결 (녹색)
		m_st_Title.SetBackColor_COLORREF(RGB(112, 173, 71));
		//m_st_Title.SetColorStyle(CVGStatic::ColorStyle_Green);
		break;

	default:
		break;
	}
}

//=============================================================================
// Method		: Set_VerifyEqpConnection
// Access		: public  
// Returns		: void
// Parameter	: __in bool bVerified
// Qualifier	:
// Last Update	: 2022/1/13 - 10:48
// Desc.		:
//=============================================================================
void CWnd_Status_Equipment::Set_VerifyEqpConnection(__in bool bVerified)
{
#ifdef USE_NO_VERIFY_EQPID
	return;
#endif

	if (bVerified)
	{
		//m_st_EqpName.SetBackColor_COLORREF(RGB(197, 224, 180));
		m_st_EqpName.SetBackColor_COLORREF(RGB(150, 255, 150));
	}
	else
	{
		m_st_EqpName.SetBackColor_COLORREF(RGB(255, 50, 0));
	}
}

//=============================================================================
// Method		: Set_OperatingMode
// Access		: public  
// Returns		: void
// Parameter	: __in uint8_t IN_OperMode
// Qualifier	:
// Last Update	: 2021/12/27 - 20:03
// Desc.		:
//=============================================================================
void CWnd_Status_Equipment::Set_OperatingMode(__in uint8_t IN_OperMode)
{
	switch (IN_OperMode)
	{
	case EOM_Auto:			// RGB(  0, 176,  80) 녹색
		m_st_OperMode.SetBackColor_COLORREF(RGB(150, 255, 150));
		break;

	case EOM_Manual:		// RGB(255, 192,   0)
		m_st_OperMode.SetBackColor_COLORREF(RGB(255, 255, 150));
		break;

	case EOM_SemiAuto:		// RGB(237, 125, 49) 주황
		m_st_OperMode.SetBackColor_COLORREF(RGB(255, 200, 150));
		break;

	case EOM_Commencement:	// RGB(  0, 122, 204) 파랑
		m_st_OperMode.SetBackColor_COLORREF(RGB(150, 200, 255));
		break;

	case EOM_DryRun:		// RGB(192, 192, 255) 보라
		m_st_OperMode.SetBackColor_COLORREF(RGB(200, 150, 255));
		break;

	default:
		break;
	}

	if (IN_OperMode < enEqpOperatingMode::EOM_MaxCount)
	{
		m_st_OperMode.SetText(g_szOperatingMode[IN_OperMode]);
	}
}

//=============================================================================
// Method		: Set_ProcessStatus
// Access		: public  
// Returns		: void
// Parameter	: __in uint8_t IN_nStatus
// Qualifier	:
// Last Update	: 2021/12/27 - 20:03
// Desc.		:
//=============================================================================
void CWnd_Status_Equipment::Set_ProcessStatus(__in uint8_t IN_nStatus)
{
	switch (IN_nStatus)
	{
	case EPC_None:	  // 초기화 이전		(적)
		Set_LED_Status(LED_On_Red, false);
		break;

	case EPC_Init:	  // 원점 수행 중	(황, 점멸)
		Set_LED_Status(LED_On_Yellow_Blink, false);
		break;

	case EPC_Idle:	  // 대기, 준비		(황)
		Set_LED_Status(LED_On_Yellow, false);
		break;

	case EPC_Run:	  // 동작 중, 시작	(녹)
		Set_LED_Status(LED_On_Green, false);
		break;

	case EPC_Alarm:	  // 알람			(적, 부저)
		Set_LED_Status(LED_On_Red_Buzzer, true);
		break;

	default:
		break;
	}

	//if (IN_nStatus < enEqpProcessStatus::EPC_MaxCount)
	//{
		//m_st_ProcessStatus.SetText(g_szProcessStatus[IN_nStatus]);
		//m_st_ProcessStatus.SetBackColor_COLORREF(RGB(255, 255, 255));
	//}
}

//=============================================================================
// Method		: Set_Status_LED
// Access		: public  
// Returns		: void
// Parameter	: __in uint8_t IN_nLED_Status
// Parameter	: __in bool IN_bBuzzer
// Qualifier	:
// Last Update	: 2021/12/27 - 20:01
// Desc.		:
//=============================================================================
void CWnd_Status_Equipment::Set_LED_Status(__in uint8_t IN_nLED_Status, __in bool IN_bBuzzer)
{
	switch (IN_nLED_Status)
	{
	case LED_Off:
		m_st_LED[LED_Red].SetBackColor_COLORREF(RGB(55, 0, 0));
		m_st_LED[LED_Green].SetBackColor_COLORREF(RGB(0, 55, 0));
		m_st_LED[LED_Yellow].SetBackColor_COLORREF(RGB(55, 55, 0));		
		break;

	case LED_On_Red:
	case LED_On_Red_Blink:
	case LED_On_Red_Buzzer:
		m_st_LED[LED_Red].SetBackColor_COLORREF(RGB(255, 0, 0));
		m_st_LED[LED_Green].SetBackColor_COLORREF(RGB(0, 55, 0));
		m_st_LED[LED_Yellow].SetBackColor_COLORREF(RGB(55, 55, 0));
		m_st_LED[LED_Yellow].SetBlink(FALSE);
		break;

	case LED_On_Green:
	case LED_On_Green_Blink:
		m_st_LED[LED_Red].SetBackColor_COLORREF(RGB(55, 0, 0));
		m_st_LED[LED_Green].SetBackColor_COLORREF(RGB(112, 255, 71));
		m_st_LED[LED_Yellow].SetBackColor_COLORREF(RGB(55, 55, 0));
		m_st_LED[LED_Yellow].SetBlink(FALSE);
		break;

	case LED_On_Yellow:
		m_st_LED[LED_Red].SetBackColor_COLORREF(RGB(55, 0, 0));
		m_st_LED[LED_Green].SetBackColor_COLORREF(RGB(0, 55, 0));
		m_st_LED[LED_Yellow].SetBackColor_COLORREF(RGB(255, 240, 0));
		m_st_LED[LED_Yellow].SetBlink(FALSE);
		break;

	case LED_On_Yellow_Blink:
		m_st_LED[LED_Red].SetBackColor_COLORREF(RGB(55, 0, 0));
		m_st_LED[LED_Green].SetBackColor_COLORREF(RGB(0, 55, 0));
		m_st_LED[LED_Yellow].SetBackColor_COLORREF(RGB(255, 240, 0));
		m_st_LED[LED_Yellow].SetBlink(TRUE);
		break;

	default:
		break;
	}

	if ((IN_bBuzzer) || (LED_On_Red_Buzzer == IN_nLED_Status))
	{
		//m_st_LED[LED_Buzzer].SetBackColor_COLORREF(RGB(0, 0, 255));
		m_st_LED[LED_Buzzer].SetBackColor_COLORREF(RGB(0, 176, 240));
	}
	else
	{
		m_st_LED[LED_Buzzer].SetBackColor_COLORREF(RGB(0, 0, 55));
	}
}

//=============================================================================
// Method		: Set_ShowHandler
// Access		: public  
// Returns		: void
// Parameter	: __in bool IN_bShow
// Qualifier	:
// Last Update	: 2022/1/13 - 11:35
// Desc.		:
//=============================================================================
void CWnd_Status_Equipment::Set_ShowHandler(__in bool IN_bShow)
{

}

//=============================================================================
// Method		: Set_Alarm
// Access		: public  
// Returns		: void
// Parameter	: __in uint8_t IN_nAlarmCode
// Parameter	: __in LPCTSTR IN_szMessage
// Qualifier	:
// Last Update	: 2021/12/27 - 20:03
// Desc.		:
//=============================================================================
void CWnd_Status_Equipment::Set_Alarm(__in uint8_t IN_nAlarmCode, __in LPCTSTR IN_szMessage)
{

}

//=============================================================================
// Method		: Set_TimeSync
// Access		: public  
// Returns		: void
// Parameter	: __in bool IN_bTimeSync
// Qualifier	:
// Last Update	: 2022/1/17 - 15:45
// Desc.		:
//=============================================================================
void CWnd_Status_Equipment::Set_TimeSync(__in bool IN_bTimeSync)
{
	if (IN_bTimeSync)
	{

	}
	else
	{

	}
}

//=============================================================================
// Method		: Set_Yield
// Access		: public  
// Returns		: void
// Parameter	: __in const CYield_Equipment * IN_pYield
// Qualifier	:
// Last Update	: 2022/1/27 - 14:01
// Desc.		:
//=============================================================================
void CWnd_Status_Equipment::Set_Yield(__in const CYield_Equipment* IN_pYield)
{
	if ((m_pEquipmentStatus->Is_Loader()) || (m_pEquipmentStatus->Is_Tester()))
	{
		if (NULL != IN_pYield)
		{
			m_wnd_Yield.Set_Yield(&m_pEquipmentStatus->Get_Yield_Day());
		}

#ifdef USE_EQP_TACTTIME
		CString szText;

		szText.Format(_T("%.1f"), m_pEquipmentStatus->Get_Tacttime().Get_TactTime());
		m_st_Tacttime[TT_Curr_Data].SetText(szText.GetBuffer());

		szText.Format(_T("%.2f"), m_pEquipmentStatus->Get_Tacttime().Get_TactTime_Ave());
		m_st_Tacttime[TT_Ave_Data].SetText(szText.GetBuffer());
#endif
	}	
}

//=============================================================================
// Method		: Set_Production
// Access		: public  
// Returns		: void
// Parameter	: __in bool IN_bEnd
// Parameter	: __in uint8_t IN_nChecking
// Qualifier	:
// Last Update	: 2022/2/21 - 19:43
// Desc.		:
//=============================================================================
void CWnd_Status_Equipment::Set_Production(__in bool IN_bEnd, __in uint8_t IN_nChecking)
{

}

//=============================================================================
// Method		: Set_PortClear
// Access		: virtual public  
// Returns		: void
// Parameter	: __in uint8_t IN_nPortIndex
// Qualifier	:
// Last Update	: 2022/1/13 - 11:07
// Desc.		:
//=============================================================================
void CWnd_Status_Equipment::Set_PortClear(__in uint8_t IN_nPortIndex)
{
	if (IN_nPortIndex < m_nPortCount)
	{

	}
	else if (PtI_T_All == IN_nPortIndex) // Port All
	{

	}
	else // 오류
	{

	}
}

//=============================================================================
// Method		: Set_PortStatus
// Access		: virtual public  
// Returns		: void
// Parameter	: __in uint8_t IN_nPortIndex
// Parameter	: __in uint8_t IN_nStatus
// Qualifier	:
// Last Update	: 2021/12/24 - 14:27
// Desc.		:
//=============================================================================
void CWnd_Status_Equipment::Set_PortStatus(__in uint8_t IN_nPortIndex, __in uint8_t IN_nStatus)
{
	if (IN_nPortIndex < m_nPortCount)
	{

	}
	else if (PtI_T_All == IN_nPortIndex) // Port All
	{

	}
	else // 오류
	{

	}
}

void CWnd_Status_Equipment::Set_PortStatus(__in uint8_t IN_nPortIndex, __in const ST_PortStatus* IN_nStatus)
{

}

//=============================================================================
// Method		: Set_ConveyorStatus
// Access		: virtual public  
// Returns		: void
// Parameter	: __in uint8_t IN_nConveyorIndex
// Parameter	: __in uint8_t IN_nStatus
// Parameter	: __in uint8_t IN_nExistSocket
// Qualifier	:
// Last Update	: 2022/1/13 - 14:46
// Desc.		:
//=============================================================================
void CWnd_Status_Equipment::Set_ConveyorStatus(__in uint8_t IN_nConveyorIndex, __in uint8_t IN_nStatus, __in uint8_t IN_nExistSocket)
{
	if (IN_nConveyorIndex < m_nConveyorCount)
	{

	}
	else // 오류
	{

	}
}

void CWnd_Status_Equipment::Set_ConveyorStatus(__in uint8_t IN_nConveyorIndex, __in const ST_ConveyorStatus* IN_nStatus)
{

}

#if defined(EES_XML)//20231003
void CWnd_Status_Equipment::SetUI_EQUIPMENTSTATE(__in CVGStatic* IN_nPort, LPCTSTR IN_DATA)
{
	if (m_pEquipmentStatus->Is_Loader())	{
	}
	if (m_pEquipmentStatus->Is_Tester())	{
		CString EQUIPMENTSTATE;
		EQUIPMENTSTATE.Format(_T("%s"), IN_DATA);
		IN_nPort->SetText(EQUIPMENTSTATE.GetBuffer());
		for (int i = 0; i < ES_MaxCount; i++) {
			if (g_szEquipment_State[i] == EQUIPMENTSTATE) {
				switch (i) {
				case ES_IDLE:
					IN_nPort->SetBackColor_COLORREF(RGB(255, 255, 0));
					break;
				case ES_RUN:
					IN_nPort->SetBackColor_COLORREF(RGB(0, 255, 0));
					break;
				case ES_STOP:
					IN_nPort->SetBackColor_COLORREF(RGB(255, 200, 0));
					break;
				case ES_SUDDENSTOP:
					IN_nPort->SetBackColor_COLORREF(RGB(255, 0, 0));
					break;
				}
			}
		}
	}
}
void CWnd_Status_Equipment::SetUI_EQUIPMENTSTATEDISPLAY(__in CVGStatic* IN_nPort, LPCTSTR IN_DATA)
{
	if (m_pEquipmentStatus->Is_Loader())	{
	}
	if (m_pEquipmentStatus->Is_Tester())	{
		CString EQUIPMENTSTATEDISPLAY;
		EQUIPMENTSTATEDISPLAY.Format(_T("%s"), IN_DATA);
		IN_nPort->SetText(EQUIPMENTSTATEDISPLAY);
	}
}
void CWnd_Status_Equipment::SetUI_RGBDISPLAY(__in CVGStatic* IN_nPort, LPCTSTR IN_DATA)
{
	if (m_pEquipmentStatus->Is_Loader())	{
	}
	if (m_pEquipmentStatus->Is_Tester())	{
		CString EQUIPMENTSTATEDISPLAY;
		EQUIPMENTSTATEDISPLAY.Format(_T("%s"), IN_DATA);
		COLORREF dw = Get_Color(EQUIPMENTSTATEDISPLAY);
		IN_nPort->SetBackColor_COLORREF(dw);
	}
}
void CWnd_Status_Equipment::Set_EqpNotify_EQUIPMENTSTATE(__in lt::Report_Equipment_State_Args::Args * IN_DATA) 
{
}
COLORREF CWnd_Status_Equipment::Get_Color(__in CString lParam) {
	COLORREF clr;
	CString csRet;
	int nRgb[3];
	lParam.TrimLeft();
	lParam.TrimRight();
	//1
	int iPos = lParam.Find(',');
	csRet.Format(_T("%s."), lParam.Left(iPos));
	nRgb[0] = _ttoi(csRet);
	lParam = lParam.Mid(iPos + 1);
	lParam.TrimLeft();
	//2
	iPos = lParam.Find(',');
	csRet.Format(_T("%s."), lParam.Left(iPos));
	nRgb[1] = _ttoi(csRet);
	lParam = lParam.Mid(iPos + 1);
	lParam.TrimLeft();
	iPos = lParam.GetLength();
	csRet.Format(_T("%s."), lParam.Left(iPos));
	nRgb[2] = _ttoi(csRet);
	clr = RGB(nRgb[0], nRgb[1], nRgb[2]);
	return clr;
}
void CWnd_Status_Equipment::Set_EqpNotify_EQUIPMENTSTATEDISPLAY(CCommonModule * N_DATA){
}
void CWnd_Status_Equipment::Set_EqpNotify_RGBDISPLAY(__in  lt::Request_Equipment_State_Display_Args::Args& IN_DATA){
}
void CWnd_Status_Equipment::Set_EquipmentID(__in CEquipment* IN_Data) {
}
#endif