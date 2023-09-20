//*****************************************************************************
// Filename	: 	Wnd_Status_Server.cpp
// Created	:	2023/3/24 - 10:57
// Created	:	2023/3/27 - 10:57
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
// Wnd_Status_Server.cpp : implementation file
//

#include "stdafx.h"
#include "Wnd_Status_Server.h"
#include "Def_Server_Type.h"
#include "Dlg_EESMode_PopupMenu.h"

#if (USE_XML)
#include "Pane_CommStatus.h"
#include "Util/StringUtil.hpp"
#endif

#define		IDC_BN_EESMODE_POPUPMENU	1004
#define		IDC_BN_LOSSWINDOW_POPUPMENU	1005

//-----------------------------------------------------------------------------
// CWnd_Status_Server
//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CWnd_Status_Server, CWnd_Status_Base)

CWnd_Status_Server::CWnd_Status_Server()
{
 
 	m_nFontSize_Caption.SetSize(0, 11);
 	m_nFontSize_Data.SetSize(0, 11);

	//2023.04.13a uhkim
	LossWindow		= new CDlg_LossWindow_PopupMenu(this);	
	TerminalMessage = new CDlg_TerminalMessage_Popup(this);
	OpCall			= new CDlg_OpCall_Popup(this);
}

CWnd_Status_Server::~CWnd_Status_Server()
{
	Release_Font();
	delete LossWindow;
	delete TerminalMessage;
	delete OpCall;
}

BEGIN_MESSAGE_MAP(CWnd_Status_Server, CWnd_Status_Base)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BN_EESMODE_POPUPMENU,		OnBnClicked_EES_Mode_Control)
	ON_BN_CLICKED(IDC_BN_LOSSWINDOW_POPUPMENU,	OnBnClicked_LossWindow_Control)
	ON_MESSAGE(WM_SERVER_NOTIFY,				OnWmServerNotify)
	ON_WM_TIMER()
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
int CWnd_Status_Server::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
 	DWORD dwStyle = WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
 	CRect rectDummy;
 	rectDummy.SetRectEmpty();

	// 설비 제어 버튼 ----------------------------	
	m_bn_RMS.Create(_T("rms"), WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | BS_PUSHBUTTON, rectDummy, this, IDC_BN_EESMODE_POPUPMENU);
	m_bn_LossWindow.Create(_T("Loss"), WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | BS_PUSHBUTTON, rectDummy, this, IDC_BN_LOSSWINDOW_POPUPMENU);
	
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
	/*
	m_st_EquipMentState			.SetFont_Gdip(L"Arial", 8.0F);;	// OFFLINE
	m_st_EquipMentStateDisplay	.SetFont_Gdip(L"Arial", 8.0F);;	// OFFLINE
	m_st_LossCode				.SetFont_Gdip(L"Arial", 8.0F);;	// OFFLINE
	m_st_RMS					.SetFont_Gdip(L"Arial", 8.0F);;	// OFFLINE
	*/
	m_st_EqpName		.Create(_T(""), dwStyle /*| WS_BORDER*/ | SS_CENTER, rectDummy, this, IDC_STATIC);
	m_st_EqpID			.Create(_T(""), dwStyle /*| WS_BORDER*/ | SS_CENTER, rectDummy, this, IDC_STATIC);
	m_st_IpAddress		.Create(_T(""), dwStyle /*| WS_BORDER*/ | SS_CENTER, rectDummy, this, IDC_STATIC);
	m_st_OperMode		.Create(_T(""), dwStyle /*| WS_BORDER*/ | SS_CENTER, rectDummy, this, IDC_STATIC);
	
	//m_st_EquipMentState	.Create(_T(""), dwStyle /*| WS_BORDER*/ | SS_CENTER, rectDummy, this, IDC_STATIC);
	//m_st_EquipMentStateDisplay.Create(_T(""), dwStyle /*| WS_BORDER*/ | SS_CENTER, rectDummy, this, IDC_STATIC);
	//m_st_LossCode		.Create(_T(""), dwStyle /*| WS_BORDER*/ | SS_CENTER, rectDummy, this, IDC_STATIC);
	//m_st_RMS			.Create(_T(""), dwStyle /*| WS_BORDER*/ | SS_CENTER, rectDummy, this, IDC_STATIC);
	
#ifdef USE_EQP_TACTTIME
	for (auto nIdx = 0; nIdx < TT_MaxCount; ++nIdx)
	{
		m_st_Tacttime[nIdx].SetFont_Gdip(L"Arial", 8.0F);
		m_st_Tacttime[nIdx].Create(_T(""), dwStyle | SS_CENTER, rectDummy, this, IDC_STATIC);
	}

	m_st_Tacttime[TT_Curr].SetText(_T("Time"));
	m_st_Tacttime[TT_Ave].SetText(_T("T Ave"));

#endif	
	// 타이틀바, 외곽선
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;

	Start();
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
void CWnd_Status_Server::OnSize(UINT nType, int cx, int cy)
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
	//m_bn_Control.MoveWindow(iLeft, iTop, iBnWidth, iBnHeight);
	m_bn_LossWindow.MoveWindow(iLeft, iTop, iBnWidth, iBnHeight);
	//2023.03.23a uhkim
	//int iRmsBnWidth = iBnWidth + 2;
	//int iRmsLeft = iLeft - iMargin - iRmsBnWidth;
	//m_bn_RMS.MoveWindow(iRmsLeft, iTop, iRmsBnWidth, iBnHeight);

	iTop = m_nTitle_Height + iSpacing;
	MoveWindow_Status(iLeft, iTop, iWidth, iHeight);

}

//=============================================================================
// Method		: OnWmServerNotify
// Access		: protected  
// Returns		: LRESULT
// Parameter	: WPARAM wParam
// Parameter	: LPARAM lParam
// Qualifier	:
// Last Update	: 2022/1/12 - 20:11
// Desc.		:
//=============================================================================
LRESULT CWnd_Status_Server::OnWmServerNotify(WPARAM wParam, LPARAM lParam)
{
	WM_Svr_Nofity_Para nPara = (WM_Svr_Nofity_Para)wParam;
	switch (nPara)
	{
	case WM_SvrNotify_ClientConnection:
		Set_ClientConnection((uint8_t)lParam);
		break;
	case WM_SvrNotify_VerifyEqpConnection:
		Set_VerifySvrConnection((bool)lParam);
		break;
	case WM_SvrNotify_OperatingMode:
		Set_OperatingMode((uint8_t)lParam);
		break;
#if ADD_SOCKET_EES_XML
	case WM_SvrNotify_EQUIPMENTSTATE:
		Set_SvrNotify_EQUIPMENTSTATE((lt::Report_Equipment_State_Args::Args&)lParam);
		break;
	case WM_SvrNotify_EQUIPMENTSTATEDISPLAY:
		Set_SvrNotify_EQUIPMENTSTATEDISPLAY((lt::Reply_Equipment_State_Display_Args::Args&)lParam);
		//Set_SvrNotify_EQUIPMENTSTATEDISPLAY((LPCTSTR)lParam);
		break;
	case WM_SvrNotify_RGBDISPLAY:
		//Set_SvrNotify_RGBDISPLAY((LPCTSTR)lParam);
		break;
	case WM_SvrNotify_LOSSWINDOW:
		Set_SvrNotify_LOSSWINDOW((LPCTSTR)lParam);
		break;
	case WM_SvrNotify_LOSSCODE:
		Set_SvrNotify_LOSSCODE((LPCTSTR)lParam);
		break;
	case WM_SvrNotify_REQUEST_TERMINAL_MESSAGE:
		//Set_SvrNotify_REQUEST_TERMINAL_MESSAGE((ST_xml_REQUEST_TERMINAL_MESSAGE*)lParam);
		break;
	case WM_SvrNotify_TERMINAL_MESSAGE:
		Set_SvrNotify_TERMINAL_MESSAGE((lt::Reply_Terminal_Message_Args::Args&)lParam);
		break;
	case WM_SvrNotify_OPCALL:
		Set_SvrNotify_OPCALL((lt::Reply_Opcall_Args::Args&)lParam);
		break;
	case WM_SvrNotify_RMS:
		Set_SvrNotify_RMS((LPCTSTR)lParam);
		break;
#endif
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
void CWnd_Status_Server::MoveWindow_Status(int x, int y, int nWidth, int nHeight)
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
 	//m_st_EqpID.MoveWindow(iLeft, iTop, iWidth, m_nCtrl_Height);
 	//iTop += m_nCtrl_Height - 1;
	m_st_IpAddress.MoveWindow(iLeft, iTop, iWidth, m_nCtrl_Height);
	iTop += m_nCtrl_Height - 1;
	m_st_OperMode.MoveWindow(iLeft, iTop, iWidth, m_nCtrl_Height);
	//iTop += m_nCtrl_Height + iSpacing;
	//m_st_EquipMentState.MoveWindow(iLeft, iTop, iWidth, m_nCtrl_Height);
	//iTop += m_nCtrl_Height - 1;
	//m_st_EquipMentState.MoveWindow(iLeft, iTop, (iWidth / 2), m_nCtrl_Height);
	//m_st_EquipMentStateDisplay.MoveWindow(((iLeft)+(iWidth / 2)), iTop, (iWidth / 2), m_nCtrl_Height);
	//iTop += m_nCtrl_Height + iSpacing;

	//m_st_RMS.MoveWindow(iLeft, iTop, iWidth, m_nCtrl_Height);
	//iTop += m_nCtrl_Height + iSpacing;
	iTop += (m_nCtrl_Height / 2) + iSpacing;

	m_nTop_Status = iTop;
}

//=============================================================================
// Method		: Create_Font
// Access		: protected  
// Returns		: virtual void
// Qualifier	:
// Last Update	: 2021/3/4 - 16:49
// Desc.		:
//=============================================================================
void CWnd_Status_Server::Create_Font()
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
void CWnd_Status_Server::Release_Font()
{
	m_font_Default.DeleteObject();
	m_font_Data.DeleteObject();
}

COLORREF CWnd_Status_Server::Get_Color(__in CString lParam)
{
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
//=============================================================================
// Method		: OnBnClicked_RMS_Mode_Control
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2023/03/23 - 18:03
// Desc.		:
//=============================================================================
void CWnd_Status_Server::OnBnClicked_EES_Mode_Control()
{
	Popup_SubMenu_EESMode();
}

void CWnd_Status_Server::Popup_SubMenu_EESMode()
{
	CDlg_EESMode_PopupMenu dlgMenu(this);
	CRect rtClient;
	GetClientRect(&rtClient);
	ClientToScreen(rtClient);

	dlgMenu.Set_InitPos(rtClient.left + 50, rtClient.top + 50);
	if (m_pServerStatus)
	{
		dlgMenu.Set_Title(m_pServerStatus->Get_Alias());
		dlgMenu.Set_EquipmentID(m_pServerStatus->Get_ServerId(), m_nEqpOrder);
		dlgMenu.Set_Server(m_pServerStatus);
	}
	dlgMenu.DoModal();
}

//=============================================================================
// Method		: OnBnClicked_RMS_Mode_Control
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2023/03/23 - 18:03
// Desc.		:
//=============================================================================
void CWnd_Status_Server::OnBnClicked_LossWindow_Control()
{
	Popup_SubMenu_LossWindow();
}

void CWnd_Status_Server::Popup_SubMenu_LossWindow()
{
	if (!LossWindow->IsWindowVisible()) {
		CRect rtClient;
		if (m_pServerStatus)
		{
			LossWindow->Set_Title(m_pServerStatus->Get_Alias());
			LossWindow->Set_EquipmentID(m_pServerStatus->Get_ServerId(), m_nEqpOrder);
			LossWindow->Set_Server(m_pServerStatus);
		}
		KillTimer(2);
		LossWindow->DoModal();
		SetTimer(2, 1000, NULL);
	}
}

//=============================================================================
// Method		: OnBnClicked_RMS_Mode_Control
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2023/03/23 - 18:03
// Desc.		:
//=============================================================================
#if TESTTERMINAL
void CWnd_Status_Server::Popup_Terminal_Message(ST_xml_REQUEST_TERMINAL_MESSAGE * IN_DATA)
{

	if (!TerminalMessage->IsWindowVisible()) {
		CRect rtClient;

		CString TERMINALMESSAGE(IN_DATA->Body.TERMINALMESSAGE);
		
		if (m_pEquipmentStatus)
		{
			TerminalMessage->Set_ServerID(m_pEquipmentStatus->Get_ServerId(), m_nEqpOrder);
			TerminalMessage->Set_Server(m_pEquipmentStatus);
			TerminalMessage->Set_TerminalMessage(TERMINALMESSAGE);
			//2023.05.02a Termainal Log	
			/*
			ST_TerminalLog stLog;
			CString szTime;
			SYSTEMTIME tmLocal;
			GetLocalTime(&tmLocal);

			szTime.Format(_T("%02d:%02d:%02d.%03d"), tmLocal.wHour, tmLocal.wMinute, tmLocal.wSecond, tmLocal.wMilliseconds);
			stLog.m_szTime = szTime;
			stLog.m_szText = TERMINALMESSAGE;
			*/
			//m_pEquipmentStatus->Get_TerminalStatus().push_back(stLog);
		}
		TerminalMessage->DoModal();
	}
}
#endif
#if TESTOPCALL
void CWnd_Status_Server::Popup_OPCALL(ST_xml_REQUEST_OPCALL * IN_DATA)
{
	if (!OpCall->IsWindowVisible()) {
		CRect rtClient;
		CString transactionId(IN_DATA->Hd.transactionId);

		CString MESSAGE(IN_DATA->Body.MESSAGE);
		CString BUZZER(IN_DATA->Body.BUZZER);
		CString TOWERLAMP(IN_DATA->Body.TOWERLAMP);
		
		if (m_pEquipmentStatus)
		{
			OpCall->Set_transactionId(transactionId);
			OpCall->Set_ServerID(m_pEquipmentStatus->Get_ServerId(), m_nEqpOrder);
			OpCall->Set_Server(m_pEquipmentStatus);
			OpCall->Set_Message(MESSAGE);
			OpCall->Set_Buzzer(BUZZER);
			OpCall->Set_TowerLamp(TOWERLAMP);
		}
		OpCall->DoModal();
	}
}
#endif
//=============================================================================
// Method		: Set_ClientConnection
// Access		: public  
// Returns		: void
// Parameter	: __in uint8_t IN_nConStatus
// Qualifier	:
// Last Update	: 2021/12/27 - 10:21
// Desc.		:
//=============================================================================
void CWnd_Status_Server::Set_ClientConnection(__in uint8_t IN_nConStatus)
{
	CString szText, szClientConnectionText;
	switch (IN_nConStatus)
	{
	case 0: // 연결 안됨 (빨강)
		m_st_Title.SetBackColor_COLORREF(RGB(192, 0, 0));
		szClientConnectionText = "DISCONNECTED";
		szText.Format(_T("Svr. %s"),  szClientConnectionText);
		SetText_Title(szText.GetBuffer());
		szText.Format(_T("%d"), (IN_nConStatus + 1));
#if (USE_XML)
		m_pServerStatus->Get_DEFINEDATA().Set_ONLINESTATE(lt::ToMultiByte(g_szOnLine_State[ONLINESTATE_OFFLINE]));
		m_pServerStatus->Get_DEFINEDATA().Set_EESMODE(lt::ToMultiByte(g_sEES_Mode[EES_OFFLINE]));
		mPane_CommStatus->m_st_RMS.SetText(g_sEES_Mode_UI[EES_OFFLINE]);
		mPane_CommStatus->m_st_EES.SetColorStyle(CVGStatic::ColorStyle_Red);				
		//m_pServerStatus->Set_REPORT_ONLINE_STATE(m_pServerStatus->Create_Report_Online_StateArgs(nullptr));		
		m_pServerStatus->Set_SERVER_CONNECTION(NULL);
		//OnSend_REPORT_ONLINE_STATE();
#endif
		break;

	case 1: // 연결 (녹색)
		m_st_Title.SetBackColor_COLORREF(RGB(112, 173, 71));
		szClientConnectionText = "CONNECTED";
		szText.Format(_T("Svr. %s"), szClientConnectionText);
		SetText_Title(szText.GetBuffer());
		szText.Format(_T("%d"), (IN_nConStatus + 1));
#if (USE_XML)				
		mPane_CommStatus->m_st_EES.SetColorStyle(CVGStatic::ColorStyle_Green);		
		m_pServerStatus->Get_DEFINEDATA().Set_ONLINESTATE(lt::ToMultiByte(g_szOnLine_State[ONLINESTATE_ONLINE]));

		mPane_CommStatus->m_st_RMS.SetText(g_sEES_Mode_UI[EES_TRACKIN]);
		m_pServerStatus->Get_DEFINEDATA().Set_EESMODE(lt::ToMultiByte(g_sEES_Mode[EES_TRACKIN]));
		
		//m_pServerStatus->Set_REPORT_ONLINE_STATE(m_pServerStatus->Create_Report_Online_StateArgs(nullptr));		
		m_pServerStatus->Set_SERVER_CONNECTION(NULL);
#endif	
		break;

	default:
		break;
	}
}
//=============================================================================
// Method		: Set_VerifySvrConnection
// Access		: public  
// Returns		: void
// Parameter	: __in bool bVerified
// Qualifier	:
// Last Update	: 2022/1/13 - 10:48
// Desc.		:
//=============================================================================
void CWnd_Status_Server::Set_VerifySvrConnection(__in bool bVerified)
{
#ifdef USE_NO_VERIFY_EQPID
	return;
#endif

	if (bVerified)
	{
		m_st_EqpName.SetBackColor_COLORREF(RGB(150, 255, 150));
	}
	else
	{
		m_st_EqpName.SetBackColor_COLORREF(RGB(255, 50, 0));
	}
}
#if SOCKET
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
void CWnd_Status_Server::SetUI_PortStatus(__in CVGStatic* IN_pwndPort, __in uint8_t IN_nStatus)
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

void CWnd_Status_Server::SetUI_PortStatus(__in CVGStatic* IN_pwndPort, __in const ST_PortStatus* IN_nStatus)
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
void CWnd_Status_Server::SetUI_ConveyorStatus(__in CVGStatic* IN_pwndConveyor, __in uint8_t IN_nStatus, __in uint8_t IN_nExistSocket)
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

void CWnd_Status_Server::SetUI_ConveyorStatus(__in CVGStatic* IN_pwndConveyor, __in const ST_ConveyorStatus* IN_nStatus)
{
	SetUI_ConveyorStatus(IN_pwndConveyor, IN_nStatus->nStatus, IN_nStatus->nExistSocket);

	IN_nStatus->szRfid;
	IN_nStatus->szBarcode;
}
#endif //SOCKET
//=============================================================================
// Method		: Set_PermissionMode
// Access		: public  
// Returns		: void
// Parameter	: __in enPermissionMode IN_PermissionMode
// Qualifier	:
// Last Update	: 2022/1/17 - 16:29
// Desc.		:
//=============================================================================
void CWnd_Status_Server::Set_PermissionMode(__in enPermissionMode IN_PermissionMode)
{
	switch (IN_PermissionMode)
	{
	case enPermissionMode::Permission_Operator:
		//m_bn_Control.ShowWindow(SW_HIDE);
		m_bn_RMS.ShowWindow(SW_HIDE);
		m_bn_LossWindow.ShowWindow(SW_HIDE);
		break;

	case enPermissionMode::Permission_Engineer:
		//m_bn_Control.ShowWindow(SW_SHOW);
		m_bn_RMS.ShowWindow(SW_SHOW);
		m_bn_LossWindow.ShowWindow(SW_SHOW);
		break;

	case enPermissionMode::Permission_Administrator:
		//m_bn_Control.ShowWindow(SW_SHOW);
		m_bn_RMS.ShowWindow(SW_SHOW);
		m_bn_LossWindow.ShowWindow(SW_SHOW);
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
void CWnd_Status_Server::SetPtr_EquipmentInfo(__in uint8_t IN_nEqpNo, CServer* IN_pEquipment)
{
	m_pServerStatus = IN_pEquipment;

	//if (m_pEquipmentStatus->Is_EES())
	{
		//m_wnd_Yield.Set_EqpLoader(true);
	}
	
	// 설비 순서
	m_nEqpOrder = IN_nEqpNo;
#if SOCKET
	// 사용하는 Port 갯수
	m_nPortCount	 = static_cast<uint8_t>(m_pEquipmentStatus->Get_PortCount());
	m_nConveyorCount = static_cast<uint8_t>(m_pEquipmentStatus->Get_ConveyorCount());

	// 검사 파라 개수 설정
	//m_wnd_Yield.Set_TestParaCount(m_pEquipmentStatus->Get_TestPortCount());
#endif//SOCKET
	if (GetSafeHwnd())
	{
		// 설비 순서
		CString szText, szClientConnectionText;
		//szText.Format(_T("No. %02d"), m_nEqpOrder);
			//IN_pEquipment->Get_ClientConnection()
		if (m_pServerStatus->Get_ClientConnection()) {
			szClientConnectionText = "CONNECTED";
		}
		else {
			szClientConnectionText = "DISCONNECTED";
		}
#if TEST
		szText.Format(_T("Svr. %s. %s" ), IN_pEquipment->GetXmlEes().GetBaseData().SUBEQPID , szClientConnectionText);
		SetText_Title(szText.GetBuffer());
#endif
		// 설비 명칭
		m_st_EqpName.SetText(g_szServerUIName[IN_pEquipment->Get_ServerType()]);

		// IP Address
		DWORD dwAddress = ntohl(IN_pEquipment->Get_IP_Address());
		CString szTemp;
		szTemp.Format(_T("%d.%d.%d.%d"), FOURTH_IPADDRESS(dwAddress), THIRD_IPADDRESS(dwAddress), SECOND_IPADDRESS(dwAddress), FIRST_IPADDRESS(dwAddress));
		m_st_IpAddress.SetWindowText(szTemp.GetBuffer());

		// Equipment ID
		m_st_EqpID.SetWindowText(IN_pEquipment->Get_ServerId());
		//m_st_EqpID.SetWindowText(m_pEquipmentStatus->Get_SValues(SV_EQUIPMENT_ID));
		
		// UI 다시 그리기
		CRect rc;
		GetClientRect(rc);
		SendMessage(WM_SIZE, (WPARAM)SIZE_RESTORED, MAKELPARAM(rc.Width(), rc.Height()));
	}
}
//=============================================================================
// Method		: SetBaseDataConfig
// Access		: public  
// Returns		: void
// Parameter	: __inOut CServer* IN_pServer
// Qualifier	:
// Last Update	: 2023.05.08 - 17:51
// Desc.		:
//=============================================================================
#if TEST
void CWnd_Status_Server::SetBaseDataConfig(CServer* IN_pServer) {

	CString			szTemp;
	ST_BaseDataID	m_BaseDataID;
	//EqpID
	//szTemp.Format(_T("%s"), IN_pServer->Get_ServerId());
	//IN_pServer->Set_SValues(SV_EQUIPMENT_ID,szTemp);
	//m_BaseDataID.EQUIPMENTID = szTemp;

	//Type
	szTemp.Format(_T("%s"), g_szServerTypeName[IN_pServer->Get_ServerType()]);
	//IN_pServer->Set_SValues(SV_TYPE, szTemp);
	m_BaseDataID.TYPE = szTemp;

	//IP Adress
	DWORD dwAddress = ntohl(IN_pServer->Get_IP_Address());
	szTemp.Format(_T("%d.%d.%d.%d"), FOURTH_IPADDRESS(dwAddress), THIRD_IPADDRESS(dwAddress), SECOND_IPADDRESS(dwAddress), FIRST_IPADDRESS(dwAddress));
	//IN_pServer->Set_SValues(SV_IPADRESS,szTemp);
	m_BaseDataID.IPADDRESS	= szTemp;
	
	//subEqpID
	szTemp.Format(_T("%s"), IN_pServer->Get_SubEqpID());
	//IN_pServer->Set_SValues(SV_SUBEQPID,szTemp);
	m_BaseDataID.SUBEQPID	= szTemp;

	//EES Mode
	szTemp.Format(_T("%s"), g_sEES_Mode[EES_OFFLINE]);
	//IN_pServer->Set_SValues(SV_EES_MODE,szTemp);
	m_BaseDataID.EESMODE	= szTemp;

	//ONLINESTATE
	szTemp.Format(_T("%s"), g_szOnLine_State[ONLINESTATE_OFFLINE]);
	//IN_pServer->Set_SValues(SV_ONLINESTATE,	szTemp);
	m_BaseDataID.ONLINESTATE	= szTemp;

	//Order
	szTemp.Format(_T("%d"), IN_pServer->Get_SvrOrder());
	//IN_pServer->Set_SValues(SV_ORDER,szTemp);
	m_BaseDataID.ORDER = szTemp;

	//ALIAS
	szTemp.Format(_T("%s"), IN_pServer->Get_Alias());
	//IN_pServer->Set_SValues(SV_ALIAS, szTemp);
	m_BaseDataID.ALIAS = szTemp;

	IN_pServer->GetXmlEes().SetBaseData(&m_BaseDataID);
}
#endif
//=============================================================================
// Method		: Set_EnableEquipment
// Access		: public  
// Returns		: void
// Parameter	: __in bool IN_bEnable
// Qualifier	:
// Last Update	: 2021/12/27 - 20:21
// Desc.		:
//=============================================================================
void CWnd_Status_Server::Set_EnableEquipment(__in bool IN_bEnable)
{
	if (IN_bEnable)
	{
		if (m_pServerStatus->Get_Skip())
		{
			Set_Skip(true);
		}
		else
		{
			m_st_Border.SetBackColor_COLORREF(RGB(255, 255, 255));

			//m_wnd_Yield.Set_EnableWindow(IN_bEnable);
		}
	}
	else
	{
		m_st_Border.SetBackColor_COLORREF(RGB(100, 100, 100));

		//m_wnd_Yield.Set_EnableWindow(IN_bEnable);
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
void CWnd_Status_Server::Set_Skip(__in bool IN_bSkip)
{
	if (m_pServerStatus->Get_EnableServer())
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

		//m_wnd_Yield.Set_SkipWindow(IN_bSkip);
	}
	else
	{

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
void CWnd_Status_Server::Set_OperatingMode(__in uint8_t IN_OperMode)
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
void CWnd_Status_Server::Set_ProcessStatus(__in uint8_t IN_nStatus)
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
void CWnd_Status_Server::Set_LED_Status(__in uint8_t IN_nLED_Status, __in bool IN_bBuzzer)
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
void CWnd_Status_Server::Set_ShowHandler(__in bool IN_bShow)
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
void CWnd_Status_Server::Set_Alarm(__in uint8_t IN_nAlarmCode, __in LPCTSTR IN_szMessage)
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
void CWnd_Status_Server::Set_TimeSync(__in bool IN_bTimeSync)
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
/*
void CWnd_Status_Server::Set_Yield(__in const CYield_Equipment* IN_pYield)
{
	if ((m_pEquipmentStatus->Is_EES()) || (m_pEquipmentStatus->Is_EES()))
	{
		if (NULL != IN_pYield)
		{
			//m_wnd_Yield.Set_Yield(&m_pEquipmentStatus->Get_Yield_Day());
		}

#ifdef USE_EQP_TACTTIME
		CString szText;

		szText.Format(_T("%.1f"), m_pEquipmentStatus->Get_Tacttime().Get_TactTime());
		m_st_Tacttime[TT_Curr_Data].SetText(szText.GetBuffer());

		szText.Format(_T("%.2f"), m_pEquipmentStatus->Get_Tacttime().Get_TactTime_Ave());
		m_st_Tacttime[TT_Ave_Data].SetText(szText.GetBuffer());
#endif
	}	
}*/

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
void CWnd_Status_Server::Set_Production(__in bool IN_bEnd, __in uint8_t IN_nChecking)
{

}
#if SOCKET
//=============================================================================
// Method		: Set_PortClear
// Access		: virtual public  
// Returns		: void
// Parameter	: __in uint8_t IN_nPortIndex
// Qualifier	:
// Last Update	: 2022/1/13 - 11:07
// Desc.		:
//=============================================================================
void CWnd_Status_Server::Set_PortClear(__in uint8_t IN_nPortIndex)
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
void CWnd_Status_Server::Set_PortStatus(__in uint8_t IN_nPortIndex, __in uint8_t IN_nStatus)
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

void CWnd_Status_Server::Set_PortStatus(__in uint8_t IN_nPortIndex, __in const ST_PortStatus* IN_nStatus)
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
void CWnd_Status_Server::Set_ConveyorStatus(__in uint8_t IN_nConveyorIndex, __in uint8_t IN_nStatus, __in uint8_t IN_nExistSocket)
{
	if (IN_nConveyorIndex < m_nConveyorCount)
	{

	}
	else // 오류
	{

	}
}

void CWnd_Status_Server::Set_ConveyorStatus(__in uint8_t IN_nConveyorIndex, __in const ST_ConveyorStatus* IN_nStatus)
{

}
#endif//SOCKET
void CWnd_Status_Server::Start()
{
	//SetTimer(2, 1000, NULL);
}
void CWnd_Status_Server::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	static DWORD WaitTime = GetTickCount() + 30000;
	static bool bShow = false;
	if (!LossWindow->IsWindowVisible()) {
		if (WaitTime < GetTickCount()) {
#if SOCKET
			if (ES_IDLE == m_pServerStatus->Get_EquipmentStatus()) {
				bShow = true;
				WaitTime = GetTickCount() + 30000;
			}
			else {
				WaitTime = GetTickCount() + 30000;
				bShow = false;
			}
#endif//SOCKET
		}
		if (bShow) {		
			//Popup_SubMenu_LossWindow();
			bShow = false;
			WaitTime = GetTickCount() + 30000;
		}
	}
	else {
		WaitTime = GetTickCount() + 30000;
		bShow = false;
	}
	CWnd_Status_Base::OnTimer(nIDEvent);
}

#if ADD_SOCKET_EES_XML
//=============================================================================
// Method		: Set_SvrNotify_LOSSWINDOW
// Access		: virtual public  
// Returns		: void
// Parameter	: __in uint8_t IN_nConveyorIndex
// Parameter	: __in uint8_t IN_nStatus
// Parameter	: __in uint8_t IN_nExistSocket
// Qualifier	:
// Last Update	: 2022/1/13 - 14:46
// Desc.		:
//=============================================================================
void CWnd_Status_Server::Set_SvrNotify_EQUIPMENTSTATE(__in lt::Report_Equipment_State_Args::Args&	IN_DATA)
{
	//CString EQUIPMENTSTATE;
	//EQUIPMENTSTATE.Format(_T("%s"), IN_DATA);
	//m_st_EquipMentState.SetText(EQUIPMENTSTATE);
}
void CWnd_Status_Server::Set_SvrNotify_EQUIPMENTSTATEDISPLAY(__in lt::Reply_Equipment_State_Display_Args::Args& IN_DATA)
{
	//CString EQUIPMENTSTATEDISPLAY;
	//EQUIPMENTSTATEDISPLAY.Format(_T("%s"), IN_DATA);
	//m_st_EquipMentStateDisplay.SetText(EQUIPMENTSTATEDISPLAY);
}
void CWnd_Status_Server::Set_SvrNotify_RGBDISPLAY(LPCTSTR IN_DATA)
{
	//CString EQUIPMENTSTATEDISPLAY;
	//EQUIPMENTSTATEDISPLAY.Format(_T("%s"), IN_DATA);
	//COLORREF dw = Get_Color(EQUIPMENTSTATEDISPLAY);
	//m_st_EquipMentStateDisplay.SetBackColor_COLORREF(dw);
}
void CWnd_Status_Server::Set_SvrNotify_LOSSWINDOW(LPCTSTR IN_DATA)
{
	CString LOSSWINDOW;
	LOSSWINDOW.Format(_T("%s"), IN_DATA);
	if (LOSSWINDOW == "1") {
		Popup_SubMenu_LossWindow();
	}
	if (LOSSWINDOW == "2") {
		if (LossWindow->IsWindowVisible()) {
			LossWindow->SendMessage(WM_COMMAND, IDCANCEL);
		}
	}
}
void CWnd_Status_Server::Set_SvrNotify_LOSSCODE(LPCTSTR IN_DATA)
{
	//CString LOSSCODE;
	//LOSSCODE.Format(_T("%s"), IN_DATA);
	//m_st_LossCode.SetText(LOSSCODE);
}

void CWnd_Status_Server::Set_SvrNotify_TERMINAL_MESSAGE(__in lt::Reply_Terminal_Message_Args::Args&  IN_DATA)
{
#if TESTTERMINAL
	Popup_Terminal_Message(IN_DATA);
#endif
}

void CWnd_Status_Server::Set_SvrNotify_OPCALL(__in lt::Reply_Opcall_Args::Args& IN_DATA)
{
#if TESTOPCALL
	Popup_OPCALL(IN_DATA);
#endif
}

void CWnd_Status_Server::Set_SvrNotify_RMS(LPCTSTR IN_DATA)
{
	//CString TEXT;
	//TEXT.Format(_T("%s"), IN_DATA);
	//m_st_RMS.SetText(TEXT);
}
void CWnd_Status_Server::SetUI_EQUIPMENTSTATE(__in CVGStatic* IN_nPort, LPCTSTR IN_DATA)
{
	//if (m_pServerStatus->Is_Tester())
	{
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
void CWnd_Status_Server::SetUI_EQUIPMENTSTATEDISPLAY(__in CVGStatic* IN_nPort, LPCTSTR IN_DATA)
{
	{
		CString EQUIPMENTSTATEDISPLAY;
		EQUIPMENTSTATEDISPLAY.Format(_T("%s"), IN_DATA);
		IN_nPort->SetText(EQUIPMENTSTATEDISPLAY);
	}
}
void CWnd_Status_Server::SetUI_RGBDISPLAY(__in CVGStatic* IN_nPort, LPCTSTR IN_DATA)
{
	{
		EQUIPMENTSTATEDISPLAY.Format(_T("%s"), IN_DATA);
		COLORREF dw = Get_Color(EQUIPMENTSTATEDISPLAY);
		IN_nPort->SetBackColor_COLORREF(dw);
	}
}
		CString EQUIPMENTSTATEDISPLAY;
//=============================================================================
// Method		: MoveWindow_EES
// Access		: protected  
// Returns		: void
// Parameter	: int x
// Parameter	: int y
// Parameter	: int nWidth
// Parameter	: int nHeight
// Qualifier	:
// Last Update	: 2023.05.25
// Desc.		:
//=============================================================================

#endif //ADD_SOCKET_EES_XML

void CWnd_Status_Server::Set_EquipmentID(__in CServer* IN_Data)
{
}