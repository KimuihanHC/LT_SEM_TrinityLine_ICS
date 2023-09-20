//*****************************************************************************
// Filename	: 	Wnd_Status_EES.cpp
// Created	:	2023/03/20 - 10:30
// Modified	:	2023/03/27 - 
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
// Wnd_Status_EES.cpp : implementation file
//

#include "stdafx.h"
#include "Wnd_Status_EES.h"
#include "Def_Server_Type.h"

//-----------------------------------------------------------------------------
// CWnd_Status_EES
//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CWnd_Status_EES, CUI_Server)
CWnd_Status_EES::CWnd_Status_EES()
{
	m_nPortCount		= PtI_T_MaxCount;
	m_nConveyorCount	= CvI_T_MaxCount;
}
CWnd_Status_EES::~CWnd_Status_EES()
{

}
BEGIN_MESSAGE_MAP(CWnd_Status_EES, CUI_Server)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CWnd_Status_EES message handlers
//=============================================================================
// Method		: OnCreate
// Access		: public  
// Returns		: int
// Parameter	: LPCREATESTRUCT lpCreateStruct
// Qualifier	:
// Last Update	: 
// Desc.		:
//=============================================================================
int CWnd_Status_EES::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
 	DWORD dwStyle = WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
 	CRect rectDummy;
 	rectDummy.SetRectEmpty();

	LPCTSTR szESSName[] =
	{
		_T("EquipMentState"),
		//_T("EquipMentNameKor"),
		//_T("EquipMentNameEng"),
		//_T("LossCode"),
		_T("StateColor"),
		//_T("Use"),
		//_T("Explanation"),
	};

	//for (auto nIdx = 0; nIdx < EES_MaxCount; ++nIdx)
	//{
	//	m_st_EES[nIdx].SetFont_Gdip(L"Arial", 7.0F);
	//	m_st_EES[nIdx].Create(szESSName[nIdx], dwStyle /*| WS_BORDER*/ | SS_CENTER, rectDummy, this, IDC_STATIC);
	//}


	m_st_SubEqpID.SetFont_Gdip(L"Arial", 7.0F);
	m_st_SubEqpID.Create(_T(" "), dwStyle /*| WS_BORDER*/ | SS_CENTER, rectDummy, this, IDC_STATIC);
	for (auto nIdx = 0; nIdx < PtI_EES_MaxCount; ++nIdx)
	{
		m_st_EquipmentID[nIdx].SetFont_Gdip(L"Arial", 7.0F);
		m_st_EquipmentID[nIdx].Create(_T(" "), dwStyle /*| WS_BORDER*/ | SS_CENTER, rectDummy, this, IDC_STATIC);
		m_st_EquipMentState[nIdx].SetFont_Gdip(L"Arial", 7.0F);
		m_st_EquipMentState[nIdx].Create(_T(" ") , dwStyle /*| WS_BORDER*/ | SS_CENTER, rectDummy, this, IDC_STATIC);
		m_st_EquipMentStateDisplay[nIdx].SetFont_Gdip(L"Arial", 7.0F);
		m_st_EquipMentStateDisplay[nIdx].Create(_T(" "), dwStyle /*| WS_BORDER*/ | SS_CENTER, rectDummy, this, IDC_STATIC);
	}

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
void CWnd_Status_EES::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	int iMargin		= 5;
	int iSpacing	= 5;
	int iLeft		= 5;
	int iTop		= m_nTop_Status + iMargin; // 타이틀 바, LED, 설비 Config 제외 위치
	int iWidth		= cx - iMargin - iMargin;
	int iHeight		= cy - iMargin - iMargin;

	MoveWindow_EES	(iLeft, iTop, iWidth, iHeight);

	//MoveWindow_Tester(5, iTop, cx - iSpacing - iSpacing, m_nDefInfo_Height - iSpacing - iTop);

	// 수율 (로더, 검사 설비만 사용?)
	iTop = cy - iSpacing - m_nCtrl_Height;

#ifdef USE_EQP_TACTTIME
	// TactTime
	int iStWidth = 40;
	int iDataWidth = cx - 10 - iStWidth + 1;
	iLeft = 5;
	int iLeftSub = iLeft + iStWidth - 1;
	m_st_Tacttime[TT_Curr].MoveWindow(iLeft, iTop, iStWidth, m_nCtrl_Height);
	m_st_Tacttime[TT_Curr_Data].MoveWindow(iLeftSub, iTop, iDataWidth, m_nCtrl_Height);

	iTop -= (m_nCtrl_Height - 1);
	m_st_Tacttime[TT_Ave].MoveWindow(iLeft, iTop, iStWidth, m_nCtrl_Height);
	m_st_Tacttime[TT_Ave_Data].MoveWindow(iLeftSub, iTop, iDataWidth, m_nCtrl_Height);
#endif

	iTop -= (m_nYield_Height + iSpacing);
	//m_wnd_Yield.MoveWindow(5, iTop, cx - 10, m_nYield_Height);

}
//=============================================================================
// Method		: MoveWindow_EES
// Access		: protected  
// Returns		: void
// Parameter	: int x
// Parameter	: int y
// Parameter	: int nWidth
// Parameter	: int nHeight
// Qualifier	:
// Last Update	: 
// Desc.		:
//=============================================================================
void CWnd_Status_EES::MoveWindow_EES(int x, int y, int nWidth, int nHeight)
{
	int iMargin = 5;
	int iSpacing = 5;
	int iLeft   = iMargin;
	int iTop	= y;
	int iWidth = nWidth;
	int iHeight = nHeight;


	iLeft = iSpacing;
	iTop = (y + iSpacing);
	m_st_SubEqpID.MoveWindow(iLeft, iTop, nWidth, m_nCtrl_Height);
	iTop += (m_nCtrl_Height - 1);
	m_st_EquipmentID[PtI_EES_0].MoveWindow(iLeft, iTop, nWidth, m_nCtrl_Height);

	
	iLeft = iMargin;
	iTop += (m_nCtrl_Height - 1);
	for (auto nIdx = 0; nIdx < PtI_EES_MaxCount; ++nIdx) {
		m_st_EquipMentState[nIdx].MoveWindow(iLeft, iTop, iWidth, m_nCtrl_Height);
		iTop += m_nCtrl_Height - 1;
		m_st_EquipMentStateDisplay[nIdx].MoveWindow(iLeft, iTop, iWidth, m_nCtrl_Height);
		iTop += m_nCtrl_Height - 1;
	}

	m_nTop_Status = iTop;

}

//=============================================================================
// Method		: Init_Tester
// Access		: virtual protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2022/7/27 - 17:34
// Desc.		:
//=============================================================================
void CWnd_Status_EES::Init_Tester()
{

}
//void CWnd_Status_EES::Popup_SubMenu_EquipMentState()
//{
//	__super::Popup_SubMenu_EquipMentState();
//}
void CWnd_Status_EES::Popup_SubMenu_EESMode()
{
	__super::Popup_SubMenu_EESMode();
}

void CWnd_Status_EES::Popup_SubMenu_LossWindow()
{
	__super::Popup_SubMenu_LossWindow();
}

#if ADD_SOCKET_EES_XML
//=============================================================================
// Method		: Set_Verify
// Access		: virtual protected  
// Returns		: void
// Qualifier	:
// Last Update	: 
// Desc.		:
//=============================================================================
void CWnd_Status_EES::Set_VerifyRequestLinkTest(__in lt::Request_Link_Test_Args::Args & lParam)
{
}
void CWnd_Status_EES::Set_VerifyRequestUserCommand(__in lt::Request_User_Cpmmand_Args::Args & lParam)
{
}
void CWnd_Status_EES::Set_VerifyRequestEquipmentStateDisplay(__in lt::Request_Equipment_State_Display_Args::Args & lParam)
{
}
void CWnd_Status_EES::Set_VerifyRequestLossWindow(__in lt::Request_Loss_Window_Args::Args & lParam)
{
	lParam;
}
void CWnd_Status_EES::Set_VerifyRequestAlarmList(__in lt::Request_Alarm_List_Args::Args & lParam)
{
	lParam;
}
void CWnd_Status_EES::Set_VerifyRequestSetDateTime(__in lt::Request_Set_DateTime_Args::Args & lParam)
{
	lParam;
}
void CWnd_Status_EES::Set_VerifyRequestTerminalMessage(__in lt::Request_Terminal_Message_Args::Args & lParam)
{
	lParam;
}
void CWnd_Status_EES::Set_VerifyRequestOpCall(__in lt::Request_Opcall_Args::Args & lParam)
{
	lParam;
}


//=============================================================================
// Method		: SetUI_EquipMentStateDisplay
// Access		: virtual public  
// Returns		: void
// Parameter	: __in bool IN_bEnable
// Qualifier	:
// Last Update	: 2022/1/3 - 11:47
// Desc.		:
//=============================================================================
void CWnd_Status_EES::SetUI_EquipMentStateDisplay(__in CVGStatic* IN_pwndEquipMentState, __in uint8_t IN_nStatus) 
{
	if (IN_nStatus)
	{
		switch (IN_nStatus)
		{
		case PtS_Empty: // 하양
			IN_pwndEquipMentState->SetBackColor_COLORREF(RGB(255, 255, 255));
			break;

		case PtS_Exist_Socket: // 초록
			IN_pwndEquipMentState->SetBackColor_COLORREF(RGB(0, 255, 0));
			break;

		case PtS_Wait_Out: // 노랑
			IN_pwndEquipMentState->SetBackColor_COLORREF(RGB(255, 232, 00));
			break;

		case PtS_Disable: // 회색
			IN_pwndEquipMentState->SetBackColor_COLORREF(RGB(180, 180, 180));
			break;

		case PtS_Alarm:	// 빨강
			IN_pwndEquipMentState->SetBackColor_COLORREF(RGB(255, 80, 80));
			break;

		default:
			break;
		}

		CString szText;
		szText.Format(_T("%d"), IN_nStatus);
		IN_pwndEquipMentState->SetText(szText.GetBuffer());
	}
}
#endif
/*
//=============================================================================
// Method		: Set_EnableEquipment
// Access		: virtual public  
// Returns		: void
// Parameter	: __in bool IN_bEnable
// Qualifier	:
// Last Update	: 2022/1/3 - 11:47
// Desc.		:
//=============================================================================
void CWnd_Status_EES::Set_EnableEquipment(__in bool IN_bEnable)
{
	__super::Set_EnableEquipment(IN_bEnable);

	if (IN_bEnable)
	{
		if (m_pEquipmentStatus->Get_Skip())
		{

		}
		else
		{
			for (auto nIdx = 0; nIdx < Zone_MaxCount; ++nIdx)
			{
				m_st_Zone[nIdx].SetBackColor_COLORREF(RGB(255, 255, 255));
			}

			for (auto nIdx = 0; nIdx < PtI_T_MaxCount; ++nIdx)
			{
				m_st_Port[nIdx].SetBackColor_COLORREF(RGB(255, 255, 255));
			}

#ifdef USE_EQP_TACTTIME
			// TactTime
			for (auto nIdx = 0; nIdx < TT_MaxCount; ++nIdx)
			{
				m_st_Tacttime[nIdx].SetBackColor_COLORREF(RGB(255, 255, 255));
			}
#endif
		}
	}
	else
	{
		m_st_Border.SetBackColor_COLORREF(RGB(100, 100, 100));
		for (auto nIdx = 0; nIdx < Zone_MaxCount; ++nIdx)
		{
			m_st_Zone[nIdx].SetBackColor_COLORREF(RGB(100, 100, 100));
		}

		for (auto nIdx = 0; nIdx < PtI_T_MaxCount; ++nIdx)
		{
			m_st_Port[nIdx].SetBackColor_COLORREF(RGB(100, 100, 100));
		}

#ifdef USE_EQP_TACTTIME
		// TactTime
		for (auto nIdx = 0; nIdx < TT_MaxCount; ++nIdx)
		{
			m_st_Tacttime[nIdx].SetBackColor_COLORREF(RGB(100, 100, 100));
		}
#endif
	}
}

//=============================================================================
// Method		: Set_Skip
// Access		: virtual public  
// Returns		: void
// Parameter	: __in bool IN_bSkip
// Qualifier	:
// Last Update	: 2022/7/27 - 17:34
// Desc.		:
//=============================================================================
void CWnd_Status_EES::Set_Skip(__in bool IN_bSkip)
{
	__super::Set_Skip(IN_bSkip);

	if (m_pEquipmentStatus->Get_EnableEquipment())
	{
		if (IN_bSkip)
		{
			m_st_Border.SetBackColor_COLORREF(RGB(130, 100, 100));
			for (auto nIdx = 0; nIdx < Zone_MaxCount; ++nIdx)
			{
				m_st_Zone[nIdx].SetBackColor_COLORREF(RGB(130, 100, 100));
			}

			for (auto nIdx = 0; nIdx < PtI_T_MaxCount; ++nIdx)
			{
				m_st_Port[nIdx].SetBackColor_COLORREF(RGB(130, 100, 100));
			}

#ifdef USE_EQP_TACTTIME
			// TactTime
			for (auto nIdx = 0; nIdx < TT_MaxCount; ++nIdx)
			{
				m_st_Tacttime[nIdx].SetBackColor_COLORREF(RGB(130, 100, 100));
			}
#endif
		}
		else
		{
			for (auto nIdx = 0; nIdx < Zone_MaxCount; ++nIdx)
			{
				m_st_Zone[nIdx].SetBackColor_COLORREF(RGB(255, 255, 255));
			}

			for (auto nIdx = 0; nIdx < PtI_T_MaxCount; ++nIdx)
			{
				m_st_Port[nIdx].SetBackColor_COLORREF(RGB(255, 255, 255));
			}

#ifdef USE_EQP_TACTTIME
			// TactTime
			for (auto nIdx = 0; nIdx < TT_MaxCount; ++nIdx)
			{
				m_st_Tacttime[nIdx].SetBackColor_COLORREF(RGB(255, 255, 255));
			}
#endif
		}
	}
	else
	{
		// 아무것도 안한다.
	}
}

//=============================================================================
// Method		: Set_PortClear
// Access		: virtual public  
// Returns		: void
// Parameter	: __in uint8_t IN_nPortIndex
// Qualifier	:
// Last Update	: 2022/1/13 - 11:30
// Desc.		:
//=============================================================================
void CWnd_Status_EES::Set_PortClear(__in uint8_t IN_nPortIndex)
{
	Set_PortStatus(IN_nPortIndex, (uint8_t)0);
}

//=============================================================================
// Method		: Set_PortStatus
// Access		: virtual public  
// Returns		: void
// Parameter	: __in uint8_t IN_nPortIndex
// Parameter	: __in uint8_t IN_nStatus
// Qualifier	:
// Last Update	: 2021/12/24 - 14:43
// Desc.		:
//=============================================================================
void CWnd_Status_EES::Set_PortStatus(__in uint8_t IN_nPortIndex, __in uint8_t IN_nStatus)
{
	if (IN_nPortIndex < m_nPortCount)
	{
		SetUI_PortStatus(&m_st_Port[IN_nPortIndex], IN_nStatus);
	}
	else if (PtI_T_All == IN_nPortIndex) // Port All
	{
		for (auto nIdx = 0; nIdx < m_nPortCount; nIdx++)
		{
			SetUI_PortStatus(&m_st_Port[nIdx], IN_nStatus);
		}
	}
	else // 오류
	{

	}
}

void CWnd_Status_EES::Set_PortStatus(__in uint8_t IN_nPortIndex, __in const ST_PortStatus* IN_nStatus)
{
	if (IN_nPortIndex < m_nPortCount)
	{
		SetUI_PortStatus(&m_st_Port[IN_nPortIndex], IN_nStatus);
	}
	else if (PtI_T_All == IN_nPortIndex) // Port All
	{
		for (auto nIdx = 0; nIdx < m_nPortCount; nIdx++)
		{
			SetUI_PortStatus(&m_st_Port[nIdx], IN_nStatus);
		}
	}
	else // 오류
	{

	}
}

//=============================================================================
// Method		: Set_ConveyorStatus
// Access		: virtual public  
// Returns		: void
// Parameter	: __in uint8_t IN_nConveyorIndex
// Parameter	: __in uint8_t IN_nStatus
// Parameter	: __in uint8_t IN_nExistSocket
// Qualifier	:
// Last Update	: 2022/7/27 - 17:35
// Desc.		:
//=============================================================================
void CWnd_Status_EES::Set_ConveyorStatus(__in uint8_t IN_nConveyorIndex, __in uint8_t IN_nStatus, __in uint8_t IN_nExistSocket)
{
	if (IN_nConveyorIndex < m_nConveyorCount)
	{
		SetUI_ConveyorStatus(&m_st_Conveyor[IN_nConveyorIndex], IN_nStatus, IN_nExistSocket);
	}
	else // 오류
	{

	}
}

void CWnd_Status_EES::Set_ConveyorStatus(__in uint8_t IN_nConveyorIndex, __in const ST_ConveyorStatus* IN_nStatus)
{
	if (IN_nConveyorIndex < m_nConveyorCount)
	{
		SetUI_ConveyorStatus(&m_st_Conveyor[IN_nConveyorIndex], IN_nStatus);
	}
	else // 오류
	{

	}
}
*/

void CWnd_Status_EES::Set_EquipmentID(__in CServer* IN_Data)
{
	__super::Set_EquipmentID(IN_Data);
	m_st_SubEqpID.SetText(IN_Data->Get_SubEqpID());
	m_st_EquipmentID[PtI_EES_0].SetText(IN_Data->Get_EquipmentIDStatus(PtI_EES_0).Get_EQUIPMENTID());
}

#if ADD_SOCKET_EES_XML
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


void CWnd_Status_EES::Set_SvrNotify_EQUIPMENTSTATE(__in lt::Report_Equipment_State_Args::Args&  IN_DATA)
{
	__super::Set_SvrNotify_EQUIPMENTSTATE(IN_DATA);
	CString EQUIPMENTID;
	EQUIPMENTID.Format(_T("%s"),IN_DATA.Body.Get_EQUIPMENTID().c_str());
	CString EQUIPMENTSTATE;
	EQUIPMENTSTATE.Format(_T("%s"), IN_DATA.Body.Get_EQUIPMENTSTATE().c_str());
	CString SUBEQPID;		
	SUBEQPID.Format(_T("%s"), IN_DATA.Body.Get_SUBEQPID().c_str());

	for (int nPORTID = 0; nPORTID < m_pEquipmentStatus->Get_EquipmentIDCount(); nPORTID++) {
		//if (m_pEquipmentStatus->Get_EquipmentIDStatus(nPORTID).szPortID == PORTID){//2023.06.22
		if (m_pEquipmentStatus->Get_EquipmentIDStatus(nPORTID).szEquipID == EQUIPMENTID) {
			CVGStatic * staic = &m_st_EquipMentState[nPORTID];
			SetUI_EQUIPMENTSTATE(staic, EQUIPMENTSTATE);
		}
	}
}
void CWnd_Status_EES::Set_SvrNotify_EQUIPMENTSTATEDISPLAY(__in lt::Reply_Equipment_State_Display_Args::Args& IN_DATA)
{
	__super::Set_SvrNotify_EQUIPMENTSTATEDISPLAY(IN_DATA);
	CString EQUIPMENTID;
	EQUIPMENTID.Format(_T("%s"), IN_DATA.Body.Get_EQUIPMENTID().c_str());
	CString SUBEQPID;
	SUBEQPID.Format(_T("%s"), IN_DATA.Body.Get_SUBEQPID().c_str());
	CString EQUIPMENTSTATEDISPLAY;
	EQUIPMENTSTATEDISPLAY.Format(_T("%s"), IN_DATA.Body.Get_EQUIPMENTSTATEDISPLAY().c_str());
	CString RGBDISPLAY;
	RGBDISPLAY.Format(_T("%s"), IN_DATA.Body.Get_RGBDISPLAY().c_str());
	//CString LOSSCODE;
	//LOSSCODE.Format(_T("%s"), IN_DATA.Body.Get_RGBDISPLAY().c_str());
	//CString LOSSCODE(IN_DATA->EES->Get_LossCode());
	for (int nPORTID = 0; nPORTID < m_pEquipmentStatus->Get_EquipmentIDCount(); nPORTID++) {
		if (m_pEquipmentStatus->Get_EquipmentIDStatus(nPORTID).szEquipID == EQUIPMENTID) {
		//if (m_pEquipmentStatus->Get_EquipmentIDStatus(nPORTID).szPortID == PORTID) {
			SetUI_EQUIPMENTSTATEDISPLAY(&m_st_EquipMentStateDisplay[nPORTID], EQUIPMENTSTATEDISPLAY);
			//SetUI_EQUIPMENTSTATEDISPLAY(&m_st_LossCode[nPORTID], LOSSCODE);
			SetUI_RGBDISPLAY(&m_st_EquipMentStateDisplay[nPORTID], RGBDISPLAY);
		}
	}
}
#endif