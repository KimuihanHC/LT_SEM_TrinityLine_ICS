//*****************************************************************************
// Filename	: 	View_MainCtrl_ICS.cpp
// Created	:	2021/1/27 - 14:00
// Modified	:	2021/1/27 - 14:00
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************

#include "stdafx.h"
#include "View_MainCtrl_ICS.h"
#include <strsafe.h>
#include "Pane_CommStatus.h"
#include "CommonFunction.h"
#include "resource.h"
#include "Def_PopupMenu.h"
#include "Def_Constant.h"

#include "XmlLineConfig.h"
#include "XmlSocket.h"
#include "RegSocket.h"
#include "RegEquipment.h"
#include "RegDebugInfo.h"
#include "Def_Language_Message.h"


// Test Code
#include "FileCrypt.h"
#include "File_UserInfo.h"
//---------------------------------------------------------
#if (USE_XML)
#include "XmlLineServerConfig.h"
#include "XmlEESConfig.h"
#include "XmlALConfig.h"
#include "XmlLossConfig.h"
#include "RegServer.h"
#include "Util/StringUtil.hpp"
//2023.09.03
#include "Pane_CommStatus.h"
CView_MainCtrl_ICS* mView_MainCtrl_ICS;
#endif

#ifdef USE_HW_LOCK_KEY
#include "Rockey.h"
#endif


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
CView_MainCtrl_ICS::CView_MainCtrl_ICS()
{
	OnInit_ConstructionSetting();
#if (USE_XML)
	mView_MainCtrl_ICS = this;//2023.09.03
#endif
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
CView_MainCtrl_ICS::~CView_MainCtrl_ICS()
{

		DeleteSplashScreen();
}

BEGIN_MESSAGE_MAP(CView_MainCtrl_ICS, CView_MainCtrl)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_MESSAGE	(WM_LOGMSG,							OnWM_LogMsg)
	ON_MESSAGE	(WM_LOGMSG_TESTER,					OnWM_LogMsg)
	ON_MESSAGE	(WM_PERMISSION_MODE,				OnWM_SwitchPermissionMode)
	ON_MESSAGE	(WM_CHANGED_RECIPE,					OnWM_ChangeRecipe)
	ON_MESSAGE	(WM_CHANGED_SETTING_INFO,			OnWM_ChangedSetting)
	ON_MESSAGE	(WM_EQP_CTRLCMD,					OnWM_EqpCtrlCmd)
	ON_MESSAGE	(WM_TEST_SELECT,					OnWM_TestSelect)

	ON_MESSAGE	(WM_EVENT_EQUIPMENT_CONNECTION,		OnWM_Eqp_Connection)
	ON_MESSAGE	(WM_EVENT_EQUIPMENT_AUTO_MODE,		OnWM_Eqp_AutoMode)
	ON_MESSAGE	(WM_EVENT_EQUIPMENT_ALARM,			OnWM_Eqp_Alarm)
	ON_MESSAGE	(WM_EVENT_EQUIPMENT_PORT_STATUS,	OnWM_Eqp_PortStatus)
	ON_MESSAGE	(WM_EVENT_EQUIPMENT_CONVEYOR_STATUS,OnWM_Eqp_ConveyorStatus)
	ON_MESSAGE	(WM_EVENT_EQUIPMENT_END_PRODUCTION,	OnWM_Eqp_EndOfProduction)
	ON_MESSAGE	(WM_EVENT_EQUIPMENT_EMPTY_EQP,		OnWM_Eqp_CheckEmptyEqp)

	ON_MESSAGE	(WM_EVENT_LOADER_RESISTER_SOCKET,	OnWM_Loader_RegisterSocket)
	ON_MESSAGE	(WM_EVENT_LOADER_CHEKCK_TRACKOUT,	OnWM_Loader_Check_TrackOut)
	ON_MESSAGE	(WM_EVENT_TESTER_TRACKIN,			OnWM_Tester_TrackIn)
	ON_MESSAGE	(WM_EVENT_TESTER_END_INSPECTION,	OnWM_Tester_EndInspection)
	ON_MESSAGE	(WM_EVENT_UNLOAD_REQ_TEST_RESULT,	OnWM_Unload_ReqTestResult)
	ON_MESSAGE	(WM_EVENT_UNLOAD_NG_INFO,			OnWM_Unload_NG_Info)
	ON_MESSAGE	(WM_EVENT_UNLOAD_UNREGISTER_SOCKET,	OnWM_Unload_UnregisterSocket)

	ON_MESSAGE	(WM_UPDATE_SOCKET_DATA,				OnWM_Update_SocketData)
	ON_MESSAGE	(WM_UPDATE_SOCKET_DATA_ALL,			OnWM_Update_SocketData_All)
#if (USE_XML)
	ON_MESSAGE(WM_EVENT_SERVER_CONNECTION,						OnWM_Svr_Connection)
	ON_MESSAGE(WM_EVENT_SERVER_REQUEST_LINK_TEST,				OnWM_Svr_REQUEST_LINK_TEST)
	ON_MESSAGE(WM_EVENT_SERVER_REPLY_LINK_TEST,					OnWM_Svr_REPLY_LINK_TEST)
	ON_MESSAGE(WM_EVENT_SERVER_REPORT_ONLINE_STATE,				OnWM_Svr_REPORT_ONLINE_STATE)
	ON_MESSAGE(WM_EVENT_SERVER_REPORT_USER_CHANGE,				OnWM_Svr_REPORT_USER_CHANGE)
	ON_MESSAGE(WM_EVENT_SERVER_REQUEST_USER_COMMAND,			OnWM_Svr_REQUEST_USER_CHANGE)	
	ON_MESSAGE(WM_EVENT_SERVER_REPLY_USER_COMMAND,				OnWM_Svr_REPLY_USER_COMMAND)
	ON_MESSAGE(WM_EVENT_SERVER_REPORT_EQUIPMENT_STATE,			OnWM_Svr_REPORT_EQUIPMENT_STATE)
	ON_MESSAGE(WM_EVENT_SERVER_REQUEST_EQUIPMENT_STATE_DISPLAY, OnWM_Svr_REQUEST_EQUIPMENT_STATE_DISPLAY)
	ON_MESSAGE(WM_EVENT_SERVER_REPLY_EQUIPMENT_STATE_DISPLAY,	OnWM_Svr_REPLY_EQUIPMENT_STATE_DISPLAY)
	ON_MESSAGE(WM_EVENT_SERVER_REPORT_LOSS_STATE,				OnWM_Svr_REPORT_LOSS_STATE)
	ON_MESSAGE(WM_EVENT_SERVER_REQUEST_LOSS_WINDOW,				OnWM_Svr_REQUEST_LOSS_WINDOW)
	ON_MESSAGE(WM_EVENT_SERVER_REPLY_LOSS_WINDOW,				OnWM_Svr_REPLY_LOSS_WINDOW)
	ON_MESSAGE(WM_EVENT_SERVER_REPORT_ALARM_STATE,				OnWM_Svr_REPORT_ALARM_STATE)
	ON_MESSAGE(WM_EVENT_SERVER_REQUEST_ALARM_LIST,				OnWM_Svr_REQUEST_ALARM_LIST)
	ON_MESSAGE(WM_EVENT_SERVER_REPLY_ALARM_LIST,				OnWM_Svr_REPLY_ALARM_LIST)
	ON_MESSAGE(WM_EVENT_SERVER_REPORT_RMS_MODE,					OnWM_Svr_REPORT_RMS_MODE)
	ON_MESSAGE(WM_EVENT_SERVER_REQUEST_SET_DATETIME,			OnWM_Svr_REQUEST_SET_DATETIME)
	ON_MESSAGE(WM_EVENT_SERVER_REPLY_SET_DATETIME,				OnWM_Svr_REPLY_SET_DATETIME)
	ON_MESSAGE(WM_EVENT_SERVER_REQUEST_TERMINAL_MESSAGE,		OnWM_Svr_REQUEST_TERMINAL_MESSAGE)
	ON_MESSAGE(WM_EVENT_SERVER_REPLY_TERMINAL_MESSAGE,			OnWM_Svr_REPLY_TERMINAL_MESSAGE)
	ON_MESSAGE(WM_EVENT_SERVER_REQUEST_OPCALL_MESSAGE,			OnWM_Svr_REQUEST_OPCALL_MESSAGE)
	ON_MESSAGE(WM_EVENT_SERVER_REPLY_OPCALL_MESSAGE,			OnWM_Svr_REPLY_OPCALL)

	ON_MESSAGE(WM_EVENT_EQUIPMENT_REPORT_ONLINE_STATE,			OnWM_Eqp_REPORT_ONLINE_STATE)
	ON_MESSAGE(WM_EVENT_EQUIPMENT_REPORT_USER_CHANGE,			OnWM_Eqp_REPORT_USER_CHANGE)
	ON_MESSAGE(WM_EVENT_EQUIPMENT_REPLY_USER_COMMAND,			OnWM_Eqp_REPLY_USER_COMMAND)
	ON_MESSAGE(WM_EVENT_EQUIPMENT_REPORT_EQUIPMENT_STATE,		OnWM_Eqp_REPORT_EQUIPMENT_STATE)
	ON_MESSAGE(WM_EVENT_EQUIPMENT_REPLY_EQUIPMENT_STATE_DISPLAY,OnWM_Eqp_REPLY_EQUIPMENT_STATE_DISPLAY)
	ON_MESSAGE(WM_EVENT_EQUIPMENT_REPORT_LOSS_STATE,			OnWM_Eqp_REPORT_LOSS_STATE)
	ON_MESSAGE(WM_EVENT_EQUIPMENT_REPLY_LOSS_WINDOW,			OnWM_Eqp_REPLY_LOSS_WINDOW)
	ON_MESSAGE(WM_EVENT_EQUIPMENT_REPORT_ALARM_STATE,			OnWM_Eqp_REPORT_ALARM_STATE)
	ON_MESSAGE(WM_EVENT_EQUIPMENT_REPORT_RMS_MODE,				OnWM_Eqp_REPORT_RMS_MODE)
	ON_MESSAGE(WM_EVENT_EQUIPMENT_REPLY_OPCALL_MESSAGE,			OnWM_Eqp_REPLY_OPCALL)
#endif
#if TEST
	ON_MESSAGE(WM_EVENT_SERVER_UNITID_READ,						OnWM_Svr_UNITID_READ)
	ON_MESSAGE(WM_EVENT_SERVER_REQUEST_UNITID_READ,				OnWM_Svr_REQUEST_UNITID_READ)
	ON_MESSAGE(WM_EVENT_SERVER_REPLY_UNITID_READ,				OnWM_Svr_REPLY_UNITID_READ)

	ON_MESSAGE(WM_EVENT_SERVER_REPORT_START_LOT,				OnWM_Svr_REPORT_START_PROCESS)
	ON_MESSAGE(WM_EVENT_SERVER_REPORT_START_PROCESS,			OnWM_Svr_REPORT_START_LOT)
	ON_MESSAGE(WM_EVENT_SERVER_REPORT_END_PROCESS,				OnWM_Svr_REPORT_END_EVENT)
	ON_MESSAGE(WM_EVENT_SERVER_REPORT_END_EVENT,				OnWM_Svr_REPORT_END_PROCESS)

	ON_MESSAGE(WM_EVENT_EQUIPMENT_UNITID_READ,					OnWM_Eqp_UNITID_READ)
	ON_MESSAGE(WM_EVENT_EQUIPMENT_REPLY_UNITID_READ,			OnWM_Eqp_REPLY_UNITID_READ)
	ON_MESSAGE(WM_EVENT_EQUIPMENT_REPORT_START_LOT,				OnWM_Eqp_REPORT_START_LOT)
	ON_MESSAGE(WM_EVENT_EQUIPMENT_REPORT_START_PROCESS,			OnWM_Eqp_REPORT_START_PROCESS)
	ON_MESSAGE(WM_EVENT_EQUIPMENT_REPORT_END_PROCESS,			OnWM_Eqp_REPORT_END_PROCESS)
	ON_MESSAGE(WM_EVENT_EQUIPMENT_REPORT_END_EVENT,				OnWM_Eqp_REPORT_END_EVENT)
#endif
END_MESSAGE_MAP()

//=============================================================================
// Method		: OnCreate
// Access		: protected  
// Returns		: int
// Parameter	: LPCREATESTRUCT lpCreateStruct
// Qualifier	:
// Last Update	: 2021/1/27 - 14:01
// Desc.		:
//=============================================================================
int CView_MainCtrl_ICS::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView_MainCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	// 초기 세팅
	CTestManager_Base::CreateSplashScreen(this, IDB_BITMAP_Luritech);
	OnInit_UISetting();
	OnInit_DeviceSetting();

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
// Last Update	: 2021/6/14 - 16:06
// Desc.		:
//=============================================================================
void CView_MainCtrl_ICS::OnSize(UINT nType, int cx, int cy)
{
	CView_MainCtrl::OnSize(nType, cx, cy);
}

//=============================================================================
// Method		: OnWM_LogMsg
// Access		: protected  
// Returns		: LRESULT
// Parameter	: WPARAM wParam
// Parameter	: LPARAM lParam
// Qualifier	:
// Last Update	: 2021/6/8 - 16:24
// Desc.		:
//=============================================================================
LRESULT CView_MainCtrl_ICS::OnWM_LogMsg(WPARAM wParam, LPARAM lParam)
{
	BOOL	bError = (BOOL)HIWORD(lParam);
	UINT	nType  = LOWORD(lParam);
	
	if (NULL == (LPCTSTR)wParam)
	{
		return FALSE;
	}
	
	OnAdd_Log((LPCTSTR)wParam, bError, nType);

	return 1;
}

//=============================================================================
// Method		: OnWM_SwitchPermissionMode
// Access		: protected  
// Returns		: LRESULT
// Parameter	: WPARAM wParam
// Parameter	: LPARAM lParam
// Qualifier	:
// Last Update	: 2021/6/11 - 15:36
// Desc.		:
//=============================================================================
LRESULT CView_MainCtrl_ICS::OnWM_SwitchPermissionMode(WPARAM wParam, LPARAM lParam)
{
	enPermissionMode InspMode = (enPermissionMode)wParam;
	
	m_stInspInfo.PermissionMode = InspMode;
	
	//OnSet_PermissionMode(InspMode);	
	
	// MainFrm으로 권한 변경 통보
	//GetParent()->SendMessage(WM_PERMISSION_MODE, (WPARAM)InspMode, 0);

	return TRUE;
}

//=============================================================================
// Method		: OnWM_ChangeRecipe
// Access		: protected  
// Returns		: LRESULT
// Parameter	: WPARAM wParam
// Parameter	: LPARAM lParam
// Qualifier	:
// Last Update	: 2021/6/11 - 15:36
// Desc.		:
//=============================================================================
LRESULT CView_MainCtrl_ICS::OnWM_ChangeRecipe(WPARAM wParam, LPARAM lParam)
{
	OnLog(_T("Changed Setting ..."));

	// 모델 파일에서 모델 정보 불러오기
	CString strModel = (LPCTSTR)wParam;
	bool bNotifyModelView = (0 < lParam) ? true : false;

	if (OnLoad_LineInfo(bNotifyModelView))
	{
		
	}

	OnMatchingEquipment();

	return TRUE;
}

//=============================================================================
// Method		: OnWM_ChangedSetting
// Access		: protected  
// Returns		: LRESULT
// Parameter	: WPARAM wParam
// Parameter	: LPARAM lParam
// Qualifier	:
// Last Update	: 2024.04.20 - 16:00
// Desc.		:
//=============================================================================
LRESULT CView_MainCtrl_ICS::OnWM_ChangedSetting(WPARAM wParam, LPARAM lParam)
{
	//GetOwner()->SendNotifyMessage(WM_CHANGED_SETTING_INFO, (WPARAM)m_szPath_Default.GetBuffer(), WM_Setting_Line);

	LPCTSTR szPath			= (LPCTSTR)wParam;
	uint8_t nSettingType	= (uint8_t)lParam;

	switch (nSettingType)
	{
	case WM_Setting_Line:	
		// 라인 설정 변경 : 설비 대수, 설비 ip, 설비 eqp ip 비교해서 서버 재시작 판단.
		m_bFlag_CheckEqpConnection = false;
		OnShow_SplashScreen(TRUE, _T("Changing Line Setting !!"));
		OnLoad_LineInfo(false);
		OnMatchingEquipment();
		if (false == m_pIcsComm->IsOpened())
		{
			OnConnect_Devicez(ICS_SERVER_MODULE);
		}
		OnShow_SplashScreen(FALSE);
		OnLog(_T("Changed Setting : Line Info"));
		m_bFlag_CheckEqpConnection = true;
		break;

	case WM_Setting_Socket:
		OnLoad_SocketInfo(false);
		OnLog(_T("Changed Setting : Socket Info"));
		break;

	case WM_Setting_User:
		OnLoad_UserInfo(false);
		OnLog(_T("Changed Setting : User Info"));
		break;

	case WM_Setting_Alarm:
		//OnLoad_AlarmInfo(false);
		OnLog(_T("Changed Setting : Alarm Info"));
		break;

	case WM_Setting_NGCode:
		OnLoad_NGCodeInfo(false);
		OnLog(_T("Changed Setting : NG Code Info"));
		break;

	case WM_Setting_Debug:
		OnLoad_DebugInfo(false);
		OnLog(_T("Changed Setting : Debug Info"));
		break;
#if (USE_XML)
	case WM_Setting_EES:
		OnLoad_EESInfo(false);
		OnLog(_T("Changed Setting : SV Info"));
		break;
	case WM_Setting_ALARMID:
		OnLoad_ALInfo(false);
		OnLog(_T("Changed Setting : AL ID Info"));
		break;
	case WM_Setting_Sever:
		OnShow_SplashScreen(TRUE, _T("Changing Server Setting !!"));
		OnLoad_ServerInfo(false);
		OnMatchingServer();
		if (false == m_pIcsServer->IsOpened())
		{
			OnConnect_Devicez(ICS_SERVER_EES);
		}
		OnShow_SplashScreen(FALSE);
		OnLog(_T("Changed Setting : Server Info"));
		break;
	case WM_Setting_Loss:
		OnLoad_LossInfo(false);
		OnLog(_T("Changed Setting : Loss Info"));
		break;
#endif
	default:
		break;
	}

	return 1;
}

//=============================================================================
// Method		: OnWM_EqpCtrlCmd
// Access		: protected  
// Returns		: LRESULT
// Parameter	: WPARAM wParam
// Parameter	: LPARAM lParam
// Qualifier	:
// Last Update	: 2021/12/27 - 18:21
// Desc.		:
//=============================================================================
LRESULT CView_MainCtrl_ICS::OnWM_EqpCtrlCmd(WPARAM wParam, LPARAM lParam)
{
	Proc_EquipmentCtrlCmd((uint8_t)wParam, (uint8_t)lParam);

	return 1;
}

//=============================================================================
// Method		: OnWM_TestSelect
// Access		: protected  
// Returns		: LRESULT
// Parameter	: WPARAM wParam
// Parameter	: LPARAM lParam
// Qualifier	:
// Last Update	: 2022/1/3 - 17:12
// Desc.		:
//=============================================================================
LRESULT CView_MainCtrl_ICS::OnWM_TestSelect(WPARAM wParam, LPARAM lParam)
{
	//GetOwner()->SendMessage(WM_TEST_SELECT, (WPARAM)nSockeType, MAKELPARAM(Max_TesterCount, m_arCondition));

	//bool* pCondition	= (bool*)wParam;
	//uint8_t nTestCount	= (uint8_t)LOWORD(lParam);
	//uint8_t nSocketType = (uint8_t)HIWORD(lParam);

	// 설비 정보 설정 및 UI 업데이트
	size_t nCount = Get_LineInfo().GetCount();
	for (auto nIdx = 0; nIdx < nCount; ++nIdx)
	{
		if (Get_Equipment(nIdx).Is_Tester())
		{
			int8_t nTestType = Get_Equipment(nIdx).Get_TesterType();

			if (0 <= nTestType)
			{
				Get_Equipment(nIdx).Set_EnableEquipment(Get_LineInfo().m_bEnable_TestType[nTestType]);
				m_wndMainView.UpdateUI_EnableEquipment(nIdx, Get_LineInfo().m_bEnable_TestType[nTestType]);
			}
		}
	}

	// 현재 설정된 소켓 Type 설정
	Get_SocketInfo().Set_LineSocketJIG_Type(Get_LineInfo().m_nSocketType);

	// 레지스트리에 저장 ?
	m_regInspInfo.Set_ModelTypeInfo(&m_stInspInfo.LineInfo);

	// 소켓 UI에 타입 변경 설정
	m_wndSocketView.Set_SocketType(Get_LineInfo().m_nSocketType);
	m_ViewSub.Set_SocketType(Get_LineInfo().m_nSocketType);

	OnLog(_T("------------ Changed Process Skip Info ------------"));
	OnLog(_T("Socket\t: %s"), g_szSocketTypeName[Get_LineInfo().m_nSocketType]);
	OnLog(_T("Model\t: %s"), g_szModelType[Get_LineInfo().m_nModelType]);

	for (auto nIdx = 0; nIdx < Get_LineInfo().GetCount(); ++nIdx)
	{
		OnLog(_T("[eqp %02d] %s (%s) \t: %s"), nIdx, 
											g_szEqpTypeName[Get_Equipment(nIdx).Get_EquipmentType()], 
											Get_EquipmentID(nIdx).GetBuffer(), 
											Get_Equipment(nIdx).Get_EnableEquipment() ? _T("enable") : _T("disable"));
	}
	OnLog(_T("---------------------------------------------------"));

	return 1;
}


//=============================================================================
// Method		: OnWM_Eqp_Connection
// Access		: protected  
// Returns		: LRESULT
// Parameter	: WPARAM wParam
// Parameter	: LPARAM lParam
// Qualifier	:
// Last Update	: 2022/2/3 - 11:50
// Desc.		:
//=============================================================================
LRESULT CView_MainCtrl_ICS::OnWM_Eqp_Connection(WPARAM wParam, LPARAM lParam)
{
	OnEvent_EquipmentConnection((uint8_t)wParam, (bool)lParam);

	return 1;
}

LRESULT CView_MainCtrl_ICS::OnWM_Eqp_AutoMode(WPARAM wParam, LPARAM lParam)
{
	OnEvent_AutoMode_Notify((uint8_t)wParam);

	return 1;
}

LRESULT CView_MainCtrl_ICS::OnWM_Eqp_Alarm(WPARAM wParam, LPARAM lParam)
{
	OnEvent_EquipmentAlarm((uint8_t)wParam, (uint8_t)lParam);

	return 1;
}

LRESULT CView_MainCtrl_ICS::OnWM_Eqp_PortStatus(WPARAM wParam, LPARAM lParam)
{
	OnEvent_Tester_PortStatus ((uint8_t)wParam, (uint8_t)lParam);
	
	return 1;
}

LRESULT CView_MainCtrl_ICS::OnWM_Eqp_ConveyorStatus(WPARAM wParam, LPARAM lParam)
{
	OnEvent_Tester_CoveyorStatus((uint8_t)wParam, (uint8_t)lParam);

	return 1;
}

LRESULT CView_MainCtrl_ICS::OnWM_Eqp_EndOfProduction(WPARAM wParam, LPARAM lParam)
{
	// 생산 종료 설정된 상태에서, 설비 내의 제품이 모두 배출 됨.
	OnEvent_Eqp_EndOfProduction((uint8_t)wParam);

	return 1;
}

LRESULT CView_MainCtrl_ICS::OnWM_Eqp_CheckEmptyEqp(WPARAM wParam, LPARAM lParam)
{
	// 현재 설비와 이전 설비에 제품이 없는것을 체크한다.
	OnEvent_CheckEmptyEquipment((uint8_t)wParam);

	return 1;
}

//=============================================================================
// Method		: OnWM_Loader_RegisterSocket
// Access		: protected  
// Returns		: LRESULT
// Parameter	: WPARAM wParam
// Parameter	: LPARAM lParam
// Qualifier	:
// Last Update	: 2022/1/28 - 17:05
// Desc.		:
//=============================================================================
LRESULT CView_MainCtrl_ICS::OnWM_Loader_RegisterSocket(WPARAM wParam, LPARAM lParam)
{
	// 소켓 등록 (로딩) (로더 / 언로더)
	OnEvent_Loader_RegisterSocket((LPCTSTR)lParam);

	return 1;
}

LRESULT CView_MainCtrl_ICS::OnWM_Loader_Check_TrackOut(WPARAM wParam, LPARAM lParam)
{
	OnEvent_Loader_CheckTrackOut((LPCTSTR)lParam);

	return 1;
}

LRESULT CView_MainCtrl_ICS::OnWM_Tester_TrackIn(WPARAM wParam, LPARAM lParam)
{
	// 소켓 투입 승인 요청
	//::SendNotifyMessage(m_hWndOwner, WM_EQUIPMENT_REQ_ACCEPT_SOCKET, (WPARAM)m_nEqpOrder, (LPARAM)IN_szRFID);	
	OnEvent_Tester_TrackIn((uint8_t)wParam, (LPCTSTR)lParam);

	return 1;
}

LRESULT CView_MainCtrl_ICS::OnWM_Tester_EndInspection(WPARAM wParam, LPARAM lParam)
{
	// 검사 결과 통지 (테스터)
	uint8_t nFromEqp = (uint8_t)wParam;
	CString szRFID = (LPCTSTR)lParam;
	OnEvent_Tester_EndInspection(nFromEqp, szRFID.GetBuffer());

	return 1;
}

LRESULT CView_MainCtrl_ICS::OnWM_Unload_ReqTestResult(WPARAM wParam, LPARAM lParam)
{
	// 검사 결과 요청 (로더 / 언로더)
	//::SendNotifyMessage(m_hWndOwner, WM_EQUIPMENT_REQ_TEST_RESULT, (WPARAM)m_nEqpOrder, (LPARAM)IN_szRFID);

	if (NULL != lParam)
		OnEvent_Unloader_TrackIn((LPCTSTR)lParam);
	else
		OnLog_Err(_T("%s() => Error : lParam is NULL!!"), _T(__FUNCTION__));

	return 1;
}

LRESULT CView_MainCtrl_ICS::OnWM_Unload_NG_Info(WPARAM wParam, LPARAM lParam)
{
	OnEvent_Unloader_NG_Info((LPCTSTR)lParam);

	return 1;
}

LRESULT CView_MainCtrl_ICS::OnWM_Unload_UnregisterSocket(WPARAM wParam, LPARAM lParam)
{
	// 소켓 등록 해제 (언로딩) (로더 / 언로더)
	//::SendNotifyMessage(m_hWndOwner, WM_EQUIPMENT_UNREGISTER_SOCKET, (WPARAM)m_nEqpOrder, (LPARAM)IN_szRFID);
	OnEvent_Unloader_UnregisterSocket((LPCTSTR)lParam);

	return 1;
}

//=============================================================================
// Method		: OnWM_Update_SocketData
// Access		: protected  
// Returns		: LRESULT
// Parameter	: WPARAM wParam
// Parameter	: LPARAM lParam
// Qualifier	:
// Last Update	: 2022/2/15 - 17:01
// Desc.		:
//=============================================================================
LRESULT CView_MainCtrl_ICS::OnWM_Update_SocketData(WPARAM wParam, LPARAM lParam)
{
	uint16_t nFlag = (uint16_t)wParam;
	CString szRFID = (LPCTSTR)lParam;

	m_wndSocketView.OnUpdate_SocketData(nFlag, szRFID.GetBuffer());
	m_ViewSub.OnUpdate_SocketData(nFlag, szRFID.GetBuffer());

	return 1;
}

LRESULT CView_MainCtrl_ICS::OnWM_Update_SocketData_All(WPARAM wParam, LPARAM lParam)
{
	uint16_t nFlag = (uint16_t)wParam;

	m_wndSocketView.OnUpdate_SocketData_All(nFlag);
	m_ViewSub.OnUpdate_SocketData_All(nFlag);

	return 1;
}
#if (USE_XML)
//=============================================================================
// Method		: OnWM_Svr_Connection
// Access		: protected  
// Returns		: LRESULT
// Parameter	: WPARAM wParam
// Parameter	: LPARAM lParam
// Qualifier	:
// Last Update	: 2022/2/15 - 17:01
// Desc.		:
//=============================================================================
LRESULT CView_MainCtrl_ICS::OnWM_Svr_Connection(WPARAM wParam, LPARAM lParam) {
	OnEvent_ServerConnection((uint8_t)wParam, lParam);
	return 1;
}

LRESULT CView_MainCtrl_ICS::OnWM_Svr_REQUEST_LINK_TEST(WPARAM wParam, LPARAM lParam){
	OnEvent_ServerREQUEST_LINK_TEST(wParam, lParam);
	return 1;
}
LRESULT CView_MainCtrl_ICS::OnWM_Svr_REPLY_LINK_TEST(WPARAM wParam, LPARAM lParam){
	OnEvent_ServerREPLY_LINK_TEST(wParam, lParam);
	return 1;
}
LRESULT CView_MainCtrl_ICS::OnWM_Svr_REPORT_ONLINE_STATE(WPARAM wParam, LPARAM lParam) {
	OnEvent_ServerREPORT_ONLINE_STATE(wParam, lParam);
	return 1;
}
LRESULT CView_MainCtrl_ICS::OnWM_Svr_REPORT_USER_CHANGE(WPARAM wParam, LPARAM lParam){
	OnEvent_ServerREPORT_USER_CHANGE(wParam, lParam);
	return 1;
}
LRESULT CView_MainCtrl_ICS::OnWM_Svr_REQUEST_USER_CHANGE(WPARAM wParam, LPARAM lParam){
	OnEvent_ServerREQUEST_USER_CHANGE(wParam, lParam);
	return 1;
}
LRESULT CView_MainCtrl_ICS::OnWM_Svr_REPLY_USER_COMMAND(WPARAM wParam, LPARAM lParam){
	OnEvent_ServerREPLY_USER_COMMAND(wParam, lParam);
	return 1;
}
LRESULT CView_MainCtrl_ICS::OnWM_Svr_REPORT_EQUIPMENT_STATE(WPARAM wParam, LPARAM lParam){
	OnEvent_ServerREPORT_EQUIPMENT_STATE(wParam, lParam);
	return 1;
}
LRESULT CView_MainCtrl_ICS::OnWM_Svr_REQUEST_EQUIPMENT_STATE_DISPLAY(WPARAM wParam, LPARAM lParam){
	OnEvent_ServerREQUEST_EQUIPMENT_STATE_DISPLAY(wParam, lParam);
	return 1;
}
LRESULT CView_MainCtrl_ICS::OnWM_Svr_REPLY_EQUIPMENT_STATE_DISPLAY(WPARAM wParam, LPARAM lParam){
	OnEvent_ServerREPLY_EQUIPMENT_STATE_DISPLAY(wParam, lParam);
	return 1;
}
LRESULT CView_MainCtrl_ICS::OnWM_Svr_REPORT_LOSS_STATE(WPARAM wParam, LPARAM lParam){
	OnEvent_ServerREPORT_LOSS_STATE(wParam, lParam);
	return 1;
}
LRESULT CView_MainCtrl_ICS::OnWM_Svr_REQUEST_LOSS_WINDOW(WPARAM wParam, LPARAM lParam){
	OnEvent_ServerREQUEST_LOSS_WINDOW(wParam, lParam);
	return 1;
}
LRESULT CView_MainCtrl_ICS::OnWM_Svr_REPLY_LOSS_WINDOW(WPARAM wParam, LPARAM lParam){
	OnEvent_ServerREPLY_LOSS_WINDOW(wParam, lParam);
	return 1;
}
LRESULT CView_MainCtrl_ICS::OnWM_Svr_REPORT_ALARM_STATE(WPARAM wParam, LPARAM lParam){
	OnEvent_ServerREPORT_ALARM_STATE(wParam, lParam);
	return 1;
}
LRESULT CView_MainCtrl_ICS::OnWM_Svr_REQUEST_ALARM_LIST(WPARAM wParam, LPARAM lParam){
	OnEvent_ServerREQUEST_ALARM_LIST(wParam, lParam);
	return 1;
}
LRESULT CView_MainCtrl_ICS::OnWM_Svr_REPLY_ALARM_LIST(WPARAM wParam, LPARAM lParam){
	OnEvent_ServerREPLY_ALARM_LIST(wParam, lParam);
	return 1;
}
LRESULT CView_MainCtrl_ICS::OnWM_Svr_REPORT_RMS_MODE(WPARAM wParam, LPARAM lParam){
	OnEvent_ServerREPORT_RMS_MODE(wParam, lParam);
	return 1;
}
LRESULT CView_MainCtrl_ICS::OnWM_Svr_REQUEST_SET_DATETIME(WPARAM wParam, LPARAM lParam){
	OnEvent_ServerREQUEST_SET_DATETIME(wParam, lParam);
	return 1;
}
LRESULT CView_MainCtrl_ICS::OnWM_Svr_REPLY_SET_DATETIME(WPARAM wParam, LPARAM lParam){
	OnEvent_ServerREPLY_SET_DATETIME(wParam, lParam);
	return 1;
}
LRESULT CView_MainCtrl_ICS::OnWM_Svr_REQUEST_TERMINAL_MESSAGE(WPARAM wParam, LPARAM lParam){
	OnEvent_ServerREQUEST_TERMINAL_MESSAGE(wParam, lParam);
	return 1;
}
LRESULT CView_MainCtrl_ICS::OnWM_Svr_REPLY_TERMINAL_MESSAGE(WPARAM wParam, LPARAM lParam){
	OnEvent_ServerREPLY_TERMINAL_MESSAGE(wParam, lParam);
	return 1;
}
LRESULT CView_MainCtrl_ICS::OnWM_Svr_REQUEST_OPCALL_MESSAGE(WPARAM wParam, LPARAM lParam){
	OnEvent_ServerREQUEST_OPCALL_MESSAGE(wParam, lParam);
	return 1;
}
LRESULT CView_MainCtrl_ICS::OnWM_Svr_REPLY_OPCALL(WPARAM wParam, LPARAM lParam){
	OnEvent_ServerREPLY_OPCALL_MESSAGE(wParam, lParam);
	return 1;
}
#endif	//ADD_SOCKET_EES_XML


#if (USE_XML)
//=============================================================================
// Method		: OnWM_Svr_REPORT_ONLINE_STATE
// Access		: protected  
// Returns		: LRESULT
// Parameter	: WPARAM wParam
// Parameter	: LPARAM lParam
// Qualifier	:
// Last Update	: 2022/2/15 - 17:01
// Desc.		:
//=============================================================================
LRESULT CView_MainCtrl_ICS::OnWM_Eqp_REPORT_ONLINE_STATE(WPARAM wParam, LPARAM lParam){
	OnEvent_EquipmentREPORT_ONLINE_STATE(wParam, lParam);
	return 1;
}
LRESULT CView_MainCtrl_ICS::OnWM_Eqp_REPORT_USER_CHANGE(WPARAM wParam, LPARAM lParam){
	OnEvent_EquipmentREPORT_USER_CHANGE(wParam, lParam);
	return 1;
}
LRESULT CView_MainCtrl_ICS::OnWM_Eqp_REPLY_USER_COMMAND(WPARAM wParam, LPARAM lParam){
	OnEvent_EquipmentREPLY_USER_COMMAND(wParam, lParam);
	return 1;
}
LRESULT CView_MainCtrl_ICS::OnWM_Eqp_REPORT_EQUIPMENT_STATE(WPARAM wParam, LPARAM lParam){
	OnEvent_EquipmentREPORT_EQUIPMENT_STATE(wParam, lParam);
	return 1;
}
LRESULT CView_MainCtrl_ICS::OnWM_Eqp_REPLY_EQUIPMENT_STATE_DISPLAY(WPARAM wParam, LPARAM lParam){
	OnEvent_EquipmentREPLY_EQUIPMENT_STATE_DISPLAY(wParam, lParam);
	return 1;
}
LRESULT CView_MainCtrl_ICS::OnWM_Eqp_REPORT_LOSS_STATE(WPARAM wParam, LPARAM lParam){
	OnEvent_EquipmentREPORT_LOSS_STATE(wParam, lParam);
	return 1;
}
LRESULT CView_MainCtrl_ICS::OnWM_Eqp_REPLY_LOSS_WINDOW(WPARAM wParam, LPARAM lParam){
	OnEvent_EquipmentREPLY_LOSS_WINDOW(wParam, lParam);
	return 1;
}
LRESULT CView_MainCtrl_ICS::OnWM_Eqp_REPORT_ALARM_STATE(WPARAM wParam, LPARAM lParam){
	OnEvent_EquipmentREPORT_ALARM_STATE(wParam, lParam);
	return 1;
}
LRESULT CView_MainCtrl_ICS::OnWM_Eqp_REPORT_RMS_MODE(WPARAM wParam, LPARAM lParam){
	OnEvent_EquipmentREPORT_RMS_MODE(wParam, lParam);
	return 1;
}
LRESULT CView_MainCtrl_ICS::OnWM_Eqp_REPLY_OPCALL(WPARAM wParam, LPARAM lParam){
	OnEvent_EquipmentREPLY_OPCALL(wParam, lParam);
	return 1;
}
#endif
#if TEST
LRESULT CView_MainCtrl_ICS::OnWM_Eqp_UNITID_READ(WPARAM wParam, LPARAM lParam)
{
	OnEvent_EquipmentUNITID_READ(wParam, lParam);
	return 1;
}
LRESULT CView_MainCtrl_ICS::OnWM_Eqp_REPLY_UNITID_READ(WPARAM wParam, LPARAM lParam)
{
	OnEvent_EquipmentREPLY_UNITID_READ(wParam, lParam);
	return 1;
}
LRESULT CView_MainCtrl_ICS::OnWM_Eqp_REPORT_START_PROCESS(WPARAM wParam, LPARAM lParam)
{
	OnEvent_EquipmentREPORT_START_PROCESS(wParam, lParam);
	return 1;
}
LRESULT CView_MainCtrl_ICS::OnWM_Eqp_REPORT_START_LOT(WPARAM wParam, LPARAM lParam)
{
	OnEvent_EquipmentREPORT_START_LOT(wParam, lParam);
	return 1;
}
LRESULT CView_MainCtrl_ICS::OnWM_Eqp_REPORT_END_EVENT(WPARAM wParam, LPARAM lParam)
{
	OnEvent_EquipmentREPORT_END_EVENT(wParam, lParam);
	return 1;
}
LRESULT CView_MainCtrl_ICS::OnWM_Eqp_REPORT_END_PROCESS(WPARAM wParam, LPARAM lParam)
{
	OnEvent_EquipmentREPORT_END_PROCESS(wParam, lParam);
	return 1;
}
#endif	//TEST

//=============================================================================
// Method		: OnInit_LogFolder
// Access		: virtual protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/6/11 - 18:39
// Desc.		:
//=============================================================================
void CView_MainCtrl_ICS::OnInit_LogFolder()
{
	// 로그 처리
	if (!m_stInspInfo.Path.szLog.IsEmpty())
	{
		m_logFile.SetPath(m_stInspInfo.Path.szLog, _T("Inspector"));
	}

	// 에러 처리
	if (!m_stInspInfo.Path.szLog.IsEmpty())
	{
		m_Log_ErrLog.SetPath(m_stInspInfo.Path.szLog, _T("Error"));
	}

	m_Log_ErrLog.SetLogFileName_Prefix(_T("Err"));


	// 설비 알람
	if (!m_stInspInfo.Path.szLog.IsEmpty())
	{
		m_log_EqpAlarm.SetPath(m_stInspInfo.Path.szLog, _T("Alarm"));
	}
	m_log_EqpAlarm.SetUseTimeStamp(TRUE, TRUE, TRUE, TRUE);
	m_log_EqpAlarm.SetLogFileName_Prefix(_T("Alarm"));

	// 소켓
	if (!m_stInspInfo.Path.szLog.IsEmpty())
	{
		m_log_Socket.SetPath(m_stInspInfo.Path.szLog, _T("Socket"));
	}
	m_log_Socket.SetUseTimeStamp(TRUE, TRUE, TRUE, TRUE);
	m_log_Socket.SetLogFileName_Prefix(_T("Socket"));

	// NG 이력
	if (!m_stInspInfo.Path.szLog.IsEmpty())
	{
		m_log_NG_History.SetPath(m_stInspInfo.Path.szLog, _T("NG_History"));
	}
	m_log_NG_History.SetUseTimeStamp(TRUE, TRUE, TRUE, TRUE);
	m_log_NG_History.SetLogFileName_Prefix(_T("NG_History"));
}

//=============================================================================
// Method		: OnInit_ConstructionSetting
// Access		: virtual protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/6/11 - 18:39
// Desc.		:
//=============================================================================
void CView_MainCtrl_ICS::OnInit_ConstructionSetting()
{
	// 프로그램 폴더 구하기
	TCHAR szExePath[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, szExePath, MAX_PATH);

	TCHAR drive[_MAX_DRIVE];
	TCHAR dir[_MAX_DIR];
	TCHAR file[_MAX_FNAME];
	TCHAR ext[_MAX_EXT];
	_tsplitpath_s(szExePath, drive, _MAX_DRIVE, dir, _MAX_DIR, file, _MAX_FNAME, ext, _MAX_EXT);

	m_stInspInfo.Path.szProgram.Format(_T("%s%s"), drive, dir);
	m_stInspInfo.Path.szLog.Format(_T("%s%sLOG\\"), drive, dir);
	m_stInspInfo.Path.szReport.Format(_T("%s%sReport\\"), drive, dir);
	m_stInspInfo.Path.szRecipe.Format(_T("%s%sRecipe\\"), drive, dir);
	m_stInspInfo.Path.szSocket.Format(_T("%s%sSocket\\"), drive, dir);
	m_stInspInfo.Path.szShared.Format(_T("%s%sShared\\"), drive, dir);
	m_stInspInfo.Path.szFailInfo.Format(_T("%s%sFailInfo\\"), drive, dir);

	// 환경 설정
	OnLoad_Option();
	Update_Language(m_stOption.Inspector.nLanguage);

	// 기본 폴더 설정
	OnInit_LogFolder();

	MakeSubDirectory(m_stInspInfo.Path.szReport);
	MakeSubDirectory(m_stInspInfo.Path.szRecipe);
	MakeSubDirectory(m_stInspInfo.Path.szSocket);
	//MakeSubDirectory(m_stInspInfo.Path.szShared); // 네트워크 공유 폴더 설정 필요
	MakeSubDirectory(m_stInspInfo.Path.szFailInfo);

	// Main
	m_wndMainView.SetPtr_SystemInfo(&m_stInspInfo);
	
	// Setting
	m_wndSettingView.SetPath_LineInfo(m_stInspInfo.Path.szRecipe);
	m_wndSettingView.SetPath_Socket(m_stInspInfo.Path.szSocket);
	m_wndSettingView.SetPath_Shared(m_stInspInfo.Path.szShared);

#if (USE_XML)
	m_wndSettingView.SetPath_EES_Info(m_stInspInfo.Path.szRecipe);
	m_wndSettingView.SetPath_ServerInfo(m_stInspInfo.Path.szRecipe);
	m_wndSettingView.SetPath_ALIDInfo(m_stInspInfo.Path.szRecipe);
	m_wndSettingView.SetPath_Loss_Info(m_stInspInfo.Path.szRecipe);
#endif
	// Socket
	m_wndSocketView.SetPtr_SocketInfo(&m_stInspInfo.SocketInfo);
	m_wndSocketView.SetPath_Report(m_stInspInfo.Path.szReport);
	m_ViewSub.SetPtr_SocketInfo(&m_stInspInfo.SocketInfo);
	m_ViewSub.SetPtr_FailInfo(&m_stInspInfo.FailInfo);
	m_ViewSub.SetPath_Report(m_stInspInfo.Path.szReport);

	// Log
	//InOutCount;

	// FailInfo
	Get_FailInfo().Set_Path(m_stInspInfo.Path.szFailInfo);
	Get_FailInfo().Set_Path_Report(m_stInspInfo.Path.szReport);
}

//=============================================================================
// Method		: OnInit_UISetting
// Access		: virtual protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2022/1/26 - 17:53
// Desc.		:
//=============================================================================
void CView_MainCtrl_ICS::OnInit_UISetting()
{
	// ***** 수정이 필요 함 *****
	// Get_SocketInfo().Set_OwnerHWND(m_ViewSub.GetSafeHwnd());
	// Get_SocketInfo().Set_OwnerHWND(m_wndSocketView.GetSafeHwnd());
	Get_SocketInfo().Set_OwnerHWND(GetSafeHwnd());
	Get_SocketInfo().Set_WinMsg_UpdateUI(WM_UPDATE_SOCKET_DATA);
}

//=============================================================================
// Method		: OnInit_DeviceSetting
// Access		: virtual protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/6/11 - 18:39
// Desc.		:
//=============================================================================
void CView_MainCtrl_ICS::OnInit_DeviceSetting()
{
	OnInit_Devicez(GetSafeHwnd());
}

//=============================================================================
// Method		: OnLoad_LineInfo
// Access		: protected  
// Returns		: bool
// Parameter	: __in bool IN_bNotifySettingWnd
// Qualifier	:
// Last Update	: 2023/3/7 - 10:49
// Desc.		:
//=============================================================================
bool CView_MainCtrl_ICS::OnLoad_LineInfo(__in bool IN_bNotifySettingWnd /*= true*/)
{
	// Line Config 불러오기
	CXmlLineConfig xml;

	CString szLog;
	CString szFullPath;
	szFullPath.Format(_T("%s%s"), m_stInspInfo.Path.szRecipe.GetBuffer(), LINE_INFO_FILENAME);

	OnLog(_T("Line Info file load: %s"), szFullPath.GetBuffer());

	// 설비와의 통신 연결 해제
// 	if (false == IN_bNotifySettingWnd)
// 	{
// 		if (m_pIcsComm)
// 		{
// 			// Server Close
// 			OnDisconnect_Devicez();
// 
// 			m_pIcsComm->RemoveRemotes();			
// 		}
// 	}

	// 현재의 라인 설정 백업
	bool bServerRestart = false;
	CConfig_Line Old_Line = Get_SettingInfo().LineInfo;

	if (xml.LoadXML_LineInfo(szFullPath.GetBuffer(), Get_SettingInfo().LineInfo))
	{
		// 예전 설정과 비교
		if (Compare_LineConfig_ServRestart(&Old_Line, &Get_SettingInfo().LineInfo))
		{
			OnLog(_T("Line Cofnig Changed => Server Restart!!"));

			// 설비와의 통신 연결 해제
			if (false == IN_bNotifySettingWnd)
			{
				// Server Close
				OnDisconnect_Devicez(ICS_SERVER_MODULE);	//2023.03.07a uhkim [추가 서버]

				m_pIcsComm->RemoveRemotes();	
			}
		}

		// 레시피의 라인 구성을 메인 데이터에 세팅
		Get_LineInfo().Set_Config_Line(&Get_SettingInfo().LineInfo);

		// 레지스트리 체크
		CRegEquipment*	pRegEqp = new CRegEquipment();
		for (auto nIdx = 0; nIdx < Get_LineInfo().GetCount(); ++nIdx)
		{
			pRegEqp->Check_RegistryPath(Get_Equipment(nIdx).Get_EquipmentId());
#if (USE_XML)
			pRegEqp->Load_Equipment_ID(Get_EquipmentID(nIdx), Get_Equipment(nIdx));
#endif
		}
		delete pRegEqp;

		// 설비에 경로 설정
		Get_LineInfo().Set_Path(m_stInspInfo.Path.szLog, m_stInspInfo.Path.szReport);

		// 레지스트리에서 불러오기로 바꿈
		OnLoad_ModelInfo();

		// Line Config log
		OnLog(_T("------------------- Line Config -------------------"));
		
		OnLog(_T("Socket\t: %s"), g_szSocketTypeName[Get_LineInfo().m_nSocketType]);
		OnLog(_T("Model\t: %s"), g_szModelType[Get_LineInfo().m_nModelType]);

		for (auto nIdx = 0; nIdx < Get_LineInfo().GetCount(); ++nIdx)
		{
			OnLog(_T("[eqp %02d] %s:%s (%s, %s) \t: %s"), nIdx, 
								g_szEqpTypeName[Get_Equipment(nIdx).Get_EquipmentType()],
								g_szEqpTypeName_UI[Get_Equipment(nIdx).Get_EqpType_UI()],
								Get_EquipmentID(nIdx).GetBuffer(), 
								ConvIPAddrToString(Get_Equipment(nIdx).Get_IP_Address()),
								Get_Equipment(nIdx).Get_EnableEquipment() ? _T("enable") : _T("disable"));
		}
		OnLog(_T("---------------------------------------------------"));

		// 레시피 변경에 따른 UI 업데이트
		m_wndMainView.Update_LineInfo();

		// 불량 정보 갱신
		OnLoad_FailInfo();

		// 소켓 UI에 타입 변경 설정
		m_wndSocketView.Set_SocketType(Get_LineInfo().m_nSocketType);
		m_ViewSub.Set_SocketType(Get_LineInfo().m_nSocketType);

		

		// 소켓 정보에 소켓 Config 적용
		//Get_SocketInfo().Set_SocketConfig();

		// 데이터 초기화
		// m_stInspInfo.ResetEquipmentInfo(nChIdx);

		// 프로그램 시작 할 때 세팅 윈도우로 데이터 갱신 요청
		if (IN_bNotifySettingWnd)
		{
			m_wndSettingView.UpdateUI_LineInfo(&Get_SettingInfo().LineInfo);
		}

		// 설비 정보와 설비 UI를 다시 매칭 함
		//OnMatchingEquipment();

		// 모델 정보 불러오기 완료
		OnLog(_T("File load completed. [File: %s]"), szFullPath);

		return true;
	}
	else
	{
		OnLog_Err(_T("File load failed. [File: %s]"), szFullPath);

		return false;
	}
}

//=============================================================================
// Method		: OnLoad_SocketInfo
// Access		: protected  
// Returns		: bool
// Parameter	: __in bool IN_bNotifySettingWnd
// Qualifier	:
// Last Update	: 2022/1/5 - 19:50
// Desc.		:
//=============================================================================
bool CView_MainCtrl_ICS::OnLoad_SocketInfo(__in bool IN_bNotifySettingWnd /*= true*/)
{
	// Socket Config 불러오기
	CXmlSocket xml;
	CSocketList stSocketList;

	CString szFullPath;
	szFullPath.Format(_T("%s%s"), m_stInspInfo.Path.szSocket.GetBuffer(), SOCKET_INFO_FILENAME);

	OnLog(_T("Socket Info file load: %s"), szFullPath.GetBuffer());

	if (xml.LoadXML_Socket_CfgList(szFullPath.GetBuffer(), stSocketList))
	{
		Get_SocketInfo().Set_SocketConfig(&stSocketList);

		// 레지스트리 체크
		CRegSocket*	pRegSocket = new CRegSocket();
		pRegSocket->Check_RegistryPath(&Get_SocketInfo());
		delete pRegSocket;

		// 프로그램 시작 할 때 세팅 윈도우로 데이터 갱신 요청
		if (IN_bNotifySettingWnd)
		{
			m_wndSettingView.UpdateUI_SocketInfo();
		}

		OnLog(_T("------------------ Socket Config ------------------"));
		OnLog(_T("Socket JIG Count : %d"), Get_SocketInfo().GetCount());
		OnLog(_T("---------------------------------------------------"));

		// Socket Info 불러오기
		CRegSocket	regSocket(REG_PATH_APP_BASE);
		if (regSocket.Get_SocketInfo(m_stInspInfo.SocketInfo))
		{
			// 현재 설정된 소켓 Type 설정
			Get_SocketInfo().Set_LineSocketJIG_Type(Get_LineInfo().m_nSocketType);

			// UI 갱신
			m_wndSocketView.Update_SocketInfo();
			m_ViewSub.Update_SocketInfo();

			// MES Rework NG Code 불러오기
			OnLoad_NGCodeInfo();

			return true;
		}
		else
		{
			OnLog_Err(_T("Socket Registry load failed."));
			return false;
		}

		OnLog(_T("File load completed. [File: %s]"), szFullPath);
	}
	else
	{
		OnLog_Err(_T("File load failed. [File: %s]"), szFullPath);

		return false;
	}
}

bool CView_MainCtrl_ICS::OnLoad_UserInfo(__in bool IN_bNotifySettingWnd /*= true*/)
{


	return true;
}

// bool CView_MainCtrl_ICS::OnLoad_AlarmInfo(__in bool IN_bNotifySettingWnd /*= true*/)
// {
// 
// 	return true;
// }

//=============================================================================
// Method		: OnLoad_NGCodeInfo
// Access		: protected  
// Returns		: bool
// Parameter	: __in bool IN_bNotifySettingWnd
// Qualifier	:
// Last Update	: 2022/7/15 - 18:02
// Desc.		:
//=============================================================================
bool CView_MainCtrl_ICS::OnLoad_NGCodeInfo(__in bool IN_bNotifySettingWnd /*= true*/)
{
	// MES Rework NG Code 불러오기
	CNGCode_Info NgCode;
	CString szPath;
	szPath.Format(_T("%s\\%s"), m_stInspInfo.Path.szRecipe, NGCODE_INFO_FILENAME);
	int16_t nMES_ReworkCode = 0;
	if (NgCode.Load_IniFile_MES(szPath, nMES_ReworkCode))
	{
		Get_SocketInfo().Set_MES_ReworkCode(nMES_ReworkCode);
	}

	return true;
}

//=============================================================================
// Method		: OnLoad_DebugInfo
// Access		: protected  
// Returns		: bool
// Parameter	: bool IN_bNotifySettingWnd
// Qualifier	:
// Last Update	: 2022/7/15 - 18:02
// Desc.		:
//=============================================================================
bool CView_MainCtrl_ICS::OnLoad_DebugInfo(bool IN_bNotifySettingWnd)
{
#ifdef USE_DEBUG_MODE
	CRegDebugInfo reg;

	OnLog(_T("Debug Info loading ..."));
	if (reg.Get_DebugInfo(m_stInspInfo.DebugMode))
	{
		OnLog(_T("Use_DebugMode : %d"), Get_DebugInfo().m_bUse_DebugMode ? 1 : 0);
		
		if (Get_DebugInfo().Is_DebugMode())
		{
			OnLog(_T("------------------- Debug Info  -------------------"));
			OnLog(_T("Use_NotBypass_NG			: %d"), Get_DebugInfo().m_bUse_NotBypass_NG);
			OnLog(_T("Use_NotBypass_TestSkip	: %d"), Get_DebugInfo().m_bUse_NotBypass_TestSkip);
			OnLog(_T("Use_NotBypass_Empty		: %d"), Get_DebugInfo().m_bUse_NotBypass_Empty);
			//OnLog(_T("AutoReset_Yield_byShift	: %d"), Get_DebugInfo().m_AutoReset_Yield_byShift);
			//OnLog(_T("SocketTrackOut_Condition	: %d"), Get_DebugInfo().m_nSocketTrackOut_Condition);
			//OnLog(_T("WaitingSocket_Condition	: %d"), Get_DebugInfo().m_nWaitingSocket_Condition);
			//OnLog(_T("MultiTester_SelectMethod	: %d"), Get_DebugInfo().m_nMultiTester_SelectMethod);
			OnLog(_T("---------------------------------------------------"));
		}
	}
	else
	{
		OnLog(_T("Debug Info load failed"));
	}
#else
	m_stInspInfo.DebugMode.m_bUse_DebugMode = false;
#endif

	return true;
}


//=============================================================================
// Method		: OnLoad_ModelInfo
// Access		: protected  
// Returns		: bool
// Qualifier	:
// Last Update	: 2022/1/5 - 19:50
// Desc.		:
//=============================================================================
bool CView_MainCtrl_ICS::OnLoad_ModelInfo()
{
	if (m_regInspInfo.Get_ModelTypeInfo(m_stInspInfo.LineInfo))
	{
		// 설비 Enable/Disable 데이터 설정
		size_t nCount = Get_LineInfo().GetCount();
		for (auto nIdx = 0; nIdx < nCount; ++nIdx)
		{
			if (Get_Equipment(nIdx).Is_Tester())
			{
				//int8_t nTestType = ConvTo_TesterType(static_cast<enEquipmentType>(Get_Equipment(nIdx).m_nEquipmentType));
				int8_t nTestType = Get_Equipment(nIdx).Get_TesterType();

				if (0 <= nTestType)
				{
					Get_Equipment(nIdx).Set_EnableEquipment(Get_LineInfo().m_bEnable_TestType[nTestType]);
				}
			}
			else
			{
				Get_Equipment(nIdx).Set_EnableEquipment(true);
			}
		}

		return true;
	}

	return false;
}

//=============================================================================
// Method		: OnLoad_FailInfo
// Access		: protected  
// Returns		: bool
// Qualifier	:
// Last Update	: 2022/7/18 - 14:56
// Desc.		:
//=============================================================================
bool CView_MainCtrl_ICS::OnLoad_FailInfo()
{
	// 설비가 바뀌었을수 있으니 다시 설정
	Get_FailInfo().Clear_EqpList();

	// 설비 세팅
	size_t nCount = Get_LineInfo().GetCount();
	for (auto nEqpIdx = 0; nEqpIdx < nCount; ++nEqpIdx)
	{
		if (Get_Equipment(nEqpIdx).Is_Tester())
		{
			//Get_FailInfo().Add_Equipment(nEqpIdx, Get_Equipment(nEqpIdx).Get_EquipmentId());
			Get_FailInfo().Add_Equipment(nEqpIdx, 
						Get_Equipment(nEqpIdx).Get_EquipmentId(), 
						Get_Equipment(nEqpIdx).Get_TestPortCount());

			Get_Equipment(nEqpIdx).SetPtr_FailInfo_Eqp(&Get_FailInfo().Get_Equipment(nEqpIdx));
		}
	}

	bool bReturn = Get_FailInfo().Load_IniFile();

	m_ViewSub.Update_FailInfo();

	return bReturn;
}

//=============================================================================
// Method		: OnLoad_Prev_LineInfo
// Access		: protected  
// Returns		: bool
// Qualifier	:
// Last Update	: 2022/7/18 - 14:56
// Desc.		:
//=============================================================================
bool CView_MainCtrl_ICS::OnLoad_Prev_LineInfo()
{
	// 레지스트리에 저장된 이전 라인 설비 정보를 불러온다.
	CRegEquipment	regEquipment;

	size_t nCount = Get_LineInfo().GetCount();
	for (auto nIdx = 0; nIdx < nCount; ++nIdx)
	{
		regEquipment.Load_Equipment(Get_EquipmentID(nIdx), Get_Equipment(nIdx));
	}

	return true;
}

//=============================================================================
// Method		: OnLoad_Prev_LineInfo_OnlyYield
// Access		: protected  
// Returns		: bool
// Qualifier	:
// Last Update	: 2022/7/18 - 14:56
// Desc.		:
//=============================================================================
bool CView_MainCtrl_ICS::OnLoad_Prev_LineInfo_OnlyYield()
{
	// 레지스트리에 저장된 이전 라인 설비 정보를 불러온다.
	CRegEquipment	regEquipment;

	size_t nCount = Get_LineInfo().GetCount();
	for (auto nIdx = 0; nIdx < nCount; ++nIdx)
	{
		regEquipment.Load_Yield(Get_EquipmentID(nIdx), Get_Equipment(nIdx));
	}

	return true;
}

//=============================================================================
// Method		: OnMatchingEquipment
// Access		: virtual protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2022/1/17 - 19:32
// Desc.		:
//=============================================================================
void CView_MainCtrl_ICS::OnMatchingEquipment()
{
	__super::OnMatchingEquipment();

	auto nEqpCnt = Get_LineInfo().GetCount();

	if (0 < nEqpCnt)
	{
		for (auto nIdx = 0; nIdx < nEqpCnt; ++nIdx)
		{
			Get_Equipment(nIdx).Set_OwnerHWND(GetSafeHwnd());
			Get_Equipment(nIdx).Set_GUI_HWND(m_wndMainView.Get_EquipmentHWND(nIdx));
			Get_Equipment(nIdx).Set_WinMsg_Notify(WM_EQUIPMENT_NOTIFY);

			Get_Equipment(nIdx).SetPtr_SocketInfo(&m_stInspInfo.SocketInfo);
		}
	}

	OnLog(_T("Matching Equipments..."));
}

//=============================================================================
// Method		: Compare_LineConfig_ServRestart
// Access		: protected  
// Returns		: bool -> true : server restart 필요
// Parameter	: __in CConfig_Line * IN_Old
// Parameter	: __in CConfig_Line * IN_New
// Qualifier	:
// Last Update	: 2022/9/23 - 11:32
// Desc.		:
//=============================================================================
bool CView_MainCtrl_ICS::Compare_LineConfig_ServRestart(__in CConfig_Line* IN_Old, __in CConfig_Line* IN_New)
{
	// 설비 대수
	if (IN_Old->GetCount() == IN_New->GetCount())
	{
		auto nCount = IN_New->GetCount();

		for (auto nEqpIdx = 0; nEqpIdx < nCount; ++nEqpIdx)
		{
			// 설비 ip
			if (IN_Old->GetAt(nEqpIdx).Get_IP_Address() != IN_New->GetAt(nEqpIdx).Get_IP_Address())
			{
				return true;
			}

			// 설비 eqp id
			if (0 != IN_Old->GetAt(nEqpIdx).Get_EquipmentId().Compare(IN_New->GetAt(nEqpIdx).Get_EquipmentId()))
			{
				return true;
			}
		}

		return false;
	}

	return true;
}



//=============================================================================
// Method		: OnSetStatus_Production
// Access		: virtual protected  
// Returns		: void
// Parameter	: __in bool IN_bEnd
// Qualifier	:
// Last Update	: 2022/2/21 - 17:40
// Desc.		:
//=============================================================================
void CView_MainCtrl_ICS::OnSetStatus_Production(__in bool IN_bEnd)
{
	__super::OnSetStatus_Production(IN_bEnd);
	((CPane_CommStatus*)m_pwndCommPane)->SetStatus_Production(IN_bEnd);
}

//=============================================================================
// Method		: OnLog_EquipmentAlarm
// Access		: virtual protected  
// Returns		: void
// Parameter	: uint8_t IN_nEqpOder
// Parameter	: const ST_AlarmStatus * IN_pAlarm
// Qualifier	:
// Last Update	: 2022/7/18 - 14:50
// Desc.		:
//=============================================================================
void CView_MainCtrl_ICS::OnLog_EquipmentAlarm(uint8_t IN_nEqpOder, const ST_AlarmStatus * IN_pAlarm)
{
	__super::OnLog_EquipmentAlarm(IN_nEqpOder, IN_pAlarm);

	CString		szLog;
	SYSTEMTIME	tmLocal;
	GetLocalTime(&tmLocal);
	szLog.Format(_T("[%02d:%02d:%02d.%03d] eqp_%02d(%s) code: %d, info : %s\r\n"), tmLocal.wHour, tmLocal.wMinute, tmLocal.wSecond, tmLocal.wMilliseconds,
												IN_nEqpOder, 
												Get_EquipmentID(IN_nEqpOder), 
												IN_pAlarm->m_nAlarmCode, 
												IN_pAlarm->m_szAlarmInfo);

	m_wndLogView.Log_EqpAlarm(szLog.GetBuffer());
}

//=============================================================================
// Method		: OnLog_RegisterSocket
// Access		: virtual protected  
// Returns		: void
// Parameter	: LPCTSTR IN_szRFID
// Qualifier	:
// Last Update	: 2022/7/18 - 14:50
// Desc.		:
//=============================================================================
void CView_MainCtrl_ICS::OnLog_RegisterSocket(LPCTSTR IN_szRFID)
{
	CString		szLog;
	CString		szTime;
	SYSTEMTIME	tmLocal;
	GetLocalTime(&tmLocal);
	szTime.Format(_T("[%02d:%02d:%02d.%03d]"), tmLocal.wHour, tmLocal.wMinute, tmLocal.wSecond, tmLocal.wMilliseconds);

	if (false == Get_Socket(IN_szRFID).m_szBarcode.IsEmpty())
	{
		szLog.Format(_T("%s Register Socket => rfid: %s, barcode: %s\r\n"), szTime.GetBuffer(), IN_szRFID, Get_Socket(IN_szRFID).m_szBarcode);
		m_wndLogView.Log_LoadSocket(szLog.GetBuffer(), true);
	}
	else
	{
		szLog.Format(_T("%s Empty Socket => rfid: %s)\r\n"), szTime.GetBuffer(), IN_szRFID);
		m_wndLogView.Log_LoadSocket(szLog.GetBuffer(), false);
	}
}

//=============================================================================
// Method		: OnReport_Socket
// Access		: virtual protected  
// Returns		: void
// Parameter	: __in LPCTSTR IN_szRFID
// Qualifier	:
// Last Update	: 2022/2/15 - 19:45
// Desc.		:
//=============================================================================
void CView_MainCtrl_ICS::OnReport_Socket(__in LPCTSTR IN_szRFID)
{
	__super::OnReport_Socket(IN_szRFID);

	// NG만 출력되도록 코드 수정
	//m_ViewSub.Set_TestResult(&Get_Socket(IN_szRFID));

	CString		szLog;
	CString		szTime;
	SYSTEMTIME	tmLocal;
	GetLocalTime(&tmLocal);
	szTime.Format(_T("[%02d:%02d:%02d.%03d] "), tmLocal.wHour, tmLocal.wMinute, tmLocal.wSecond, tmLocal.wMilliseconds);

	if (0 == Get_Socket(IN_szRFID).m_stTestResult.m_nNG_Code)
	{
		szLog.Format(_T("%s Pass => rfid: %s, barcode: %s\r\n"), szTime.GetBuffer(), IN_szRFID, Get_Socket(IN_szRFID).m_szBarcode);

		m_wndLogView.Log_UnloadSocket(szLog.GetBuffer(), true);
	}
	else
	{
		szLog.Format(_T("%s NG => rfid: %s, barcode: %s(ng code: %d, ng eqp type: %s, ng eqp: %d, ng para: %s)\r\n"),
			szTime.GetBuffer(),
			IN_szRFID,
			Get_Socket(IN_szRFID).m_szBarcode,
			Get_Socket(IN_szRFID).m_stTestResult.m_nNG_Code,
			g_szEqpTypeName[Get_SocketTestResult(IN_szRFID).m_nNG_EqpType],
			Get_Socket(IN_szRFID).m_stTestResult.m_nNG_EqpOrder,
			g_szEqpPara_Mini[Get_Socket(IN_szRFID).m_stTestResult.m_nNG_Para]); // (Para_Left == Get_Socket(IN_szRFID).m_stTestResult.m_nNG_Para) ? _T("L") : _T("R"));

		m_wndLogView.Log_UnloadSocket(szLog.GetBuffer(), false);
	}
}

//=============================================================================
// Method		: OnReport_Socket_NG
// Access		: virtual protected  
// Returns		: void
// Parameter	: __in LPCTSTR IN_szRFID
// Qualifier	:
// Last Update	: 2022/7/18 - 14:50
// Desc.		:
//=============================================================================
void CView_MainCtrl_ICS::OnReport_Socket_NG(__in LPCTSTR IN_szRFID)
{
	if (IN_szRFID)
	{
		m_ViewSub.Set_TestResult(&Get_Socket(IN_szRFID));
	}
	else
	{
		m_ViewSub.Set_TestResult(nullptr);
	}
}

void CView_MainCtrl_ICS::OnUpdate_ElapsedTime_Unit(__in UINT nEquipmentIdx, __in uint8_t IN_nPara)
{
	// 설비 para의 검사 시간 표시 (?)
}

void CView_MainCtrl_ICS::OnUpdate_ElapsedTime_All()
{

}

//=============================================================================
// Method		: SwitchWindow
// Access		: virtual public  
// Returns		: UINT
// Parameter	: __in UINT nIndex
// Qualifier	:
// Last Update	: 2021/7/2 - 13:48
// Desc.		:
//=============================================================================
UINT CView_MainCtrl_ICS::SwitchWindow(__in UINT nIndex)
{
	return CView_MainCtrl::SwitchWindow(nIndex);
}

void CView_MainCtrl_ICS::SetCommPanePtr(__in CWnd* pwndCommPane)
{
	return CView_MainCtrl::SetCommPanePtr(pwndCommPane);
}

//=============================================================================
// Method		: ReloadOption
// Access		: virtual public  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/7/2 - 13:48
// Desc.		:
//=============================================================================
void CView_MainCtrl_ICS::ReloadOption()
{
	uint8_t nCurrentLang = m_stOption.Inspector.nLanguage;
	//__super::ReloadOption();

	// 환경설정이 바뀜 (log 출력)
	OnLoad_Option();
	
	OnLog(_T("----------------  Changed  Option  ----------------"));
	OnLog(_T("Language: %d"), m_stOption.Inspector.nLanguage);
	OnLog(_T("Path Log: %s"), m_stOption.Inspector.szPath_Log);
	OnLog(_T("Path Report: %s"), m_stOption.Inspector.szPath_Report);
	OnLog(_T("Path Recipe: %s"), m_stOption.Inspector.szPath_Recipe);
	OnLog(_T("Path Socket: %s"), m_stOption.Inspector.szPath_Socket);
	OnLog(_T("Path Shared: %s"), m_stOption.Inspector.szPath_Shared);
	OnLog(_T("Path FailInfo: %s"), m_stOption.Inspector.szPath_FailInfo);
	USES_CONVERSION;
	//OnLog(_T("Sever Address: %s"), A2T(inet_ntoa(*(IN_ADDR*)&m_stOption.Server.Address.dwAddress)));
	//OnLog(_T("Sever Port: %d"), m_stOption.Server.Address.dwPort);
	//2023.01.26a uhkim 
	for (int i = 0; i < ICS_SERVER_MAX; i++) {
		OnLog(_T("Sever Address %d : %s"), i, A2T(inet_ntoa(*(IN_ADDR*)&m_stOption.Server[i].Address.dwAddress)));
		OnLog(_T("Sever Port %d : %d"), i, m_stOption.Server[i].Address.dwPort);
	}
	OnLog(_T("---------------------------------------------------"));

	// UI의 언어 설정 변경
	if (nCurrentLang != m_stOption.Inspector.nLanguage)
	{
		Update_Language(m_stOption.Inspector.nLanguage);
	}

}

//=============================================================================
// Method		: InitStartProgress
// Access		: virtual public  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/6/11 - 18:43
// Desc.		:
//=============================================================================
void CView_MainCtrl_ICS::InitStartProgress()
{
	OnShow_SplashScreen(TRUE);
	
	// Setting ----------------------------------
	m_wndSplash.SetText(_T("Loading Settings.."));

	// 생산 시작 설정
	OnSetStatus_Production(false);

	

	// 레지스트리에서 이전 실행시 프로그램 오류로 비정상 종료되었나 체크
	if (m_regInspInfo.Is_ProgramCrash())
	{
		// 이전에 프로그램 실행할때 비정상 종료
		m_stInspInfo.m_bCrash_Executed = true;
	}

	// 레지스트리에 프로그램 시작 설정
	m_regInspInfo.Set_ProgramLoading(true);

	// 라인 설정 불러오기
	OnLoad_LineInfo();
	// 소켓 설정 불러오기
	OnLoad_SocketInfo();

	// 각각의 설비들에 설정 세팅
	OnMatchingEquipment();



	// 프로그램 종료 이전에 가지고 있던 설비 데이터 불러오기
	if (m_stInspInfo.m_bCrash_Executed)
	{
		OnLoad_Prev_LineInfo();
	}
	else
	{
		OnLoad_Prev_LineInfo_OnlyYield();
	}

	// 디버그 모드 불러오기
	OnLoad_DebugInfo();

#if (USE_XML)
	OnLoad_ServerInfo();	
	OnLoad_EESInfo();	
	OnLoad_ALInfo();
	OnLoad_LossInfo();

	OnMatchingServer();
#endif
	

#ifdef _DEBUG
	OnSet_PermissionMode(enPermissionMode::Permission_Administrator);
#else
	// Log Out
	m_regInspInfo.Set_LogOut_User();
 	((CPane_CommStatus*)m_pwndCommPane)->SetStatus_Login(enPermissionMode::Permission_Operator, nullptr);
	OnSet_PermissionMode(enPermissionMode::Permission_Operator);
// 	m_wndMainView.Set_PermissionMode(enPermissionMode::Permission_Operator);
// 	m_wndSettingView.Set_PermissionMode(enPermissionMode::Permission_Operator);
// 	m_wndMaintenanceView.Set_PermissionMode(enPermissionMode::Permission_Operator);
#endif
	


	// Devices ----------------------------------
	m_wndSplash.SetText(_T("Connecting Devices"));
	__try
	{
		//2023.03.07a uhkim [서버 추가]
		OnConnect_Devicez(ICS_SERVER_MODULE);
		OnConnect_Devicez(ICS_SERVER_EES);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		m_bFlag_ReadyTest = FALSE;
		OnLog_Err(_T("Device Connecting Err"));
		OnShow_SplashScreen(FALSE);
		return;
	}

#ifdef USE_AUTO_TO_MANUAL_AUTOCHANGE

	m_stInspInfo.Reset_LastRegisterTime();

	m_stInspInfo.Set_LastRegisterTime();

#endif // USE_AUTO_TO_MANUAL_AUTOCHANGE

	// 2nd Window 활성화
	ShowWindow_SubMonitoring(TRUE);

	// Inspection -------------------------------
	m_wndSplash.SetText(_T("Loading existing inspection information ..."));

	m_bFlag_ReadyTest = InitStartDeviceProgress() ? TRUE : FALSE;

	OnLog(m_bFlag_ReadyTest ? LOGTYPE_NONE : LOGTYPE_ERROR, _T("InitStartProgress::m_bFlag_ReadyTest : %d"), m_bFlag_ReadyTest);

	m_bFlag_CheckEqpConnection = true;

	OnShow_SplashScreen(FALSE);

	OnLog(_T("Init Start Progress : finished"));

}

//=============================================================================
// Method		: InitStartDeviceProgress
// Access		: virtual public  
// Returns		: BOOL
// Qualifier	:
// Last Update	: 2021/6/11 - 18:44
// Desc.		:
//=============================================================================
BOOL CView_MainCtrl_ICS::InitStartDeviceProgress()
{
	// 장치 연결 후 초기 설정




	return TRUE;
}

//=============================================================================
// Method		: FinalExitProgress
// Access		: virtual public  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/6/11 - 18:44
// Desc.		:
//=============================================================================
void CView_MainCtrl_ICS::FinalExitProgress()
{
	m_bFlag_CheckEqpConnection = false;
	m_bFlag_ReadyTest = FALSE;
	m_bExitFlag = TRUE;

	OnShow_SplashScreen(TRUE, _T("Exit Progress"));

	
	// Log Out
	m_regInspInfo.Set_LogOut_User();	



	// thread exit
	SetEvent(m_hEvent_ProgramExit);

	OnShow_SplashScreen(TRUE, _T("Disconnect Divices"));

	Delay(100);
	
	//==========================================================================
	//2023.03.07a uhkim [서버]
	OnDisconnect_Devicez(ICS_SERVER_MODULE);
	m_pIcsComm->RemoveRemotes();

#if (USE_XML)
	OnDisconnect_Devicez(ICS_SERVER_EES);	
	m_pIcsServer->RemoveRemotes();
#endif
	//==========================================================================

	// 레지스트리에 프로그램 종료 설정
	m_regInspInfo.Set_ProgramLoading(false);


	OnShow_SplashScreen(TRUE, _T("-- Quit --"));
	OnShow_SplashScreen(FALSE);
}

//=============================================================================
// Method		: OnSet_SystemType
// Access		: virtual public  
// Returns		: void
// Parameter	: __in enInsptrSysType nSysType
// Qualifier	:
// Last Update	: 2021/6/11 - 18:45
// Desc.		:
//=============================================================================
void CView_MainCtrl_ICS::OnSet_SystemType(__in enInsptrSysType nSysType)
{
	__super::OnSet_SystemType(nSysType);
}

//=============================================================================
// Method		: OnAdd_Log
// Access		: virtual public  
// Returns		: void
// Parameter	: __in LPCTSTR lpszLog
// Parameter	: __in BOOL bError
// Parameter	: __in UINT nLogType
// Parameter	: __in BOOL bOnlyLogType
// Qualifier	:
// Last Update	: 2022/2/15 - 21:09
// Desc.		:
//=============================================================================
void CView_MainCtrl_ICS::OnAdd_Log(__in LPCTSTR lpszLog, __in BOOL bError /*= FALSE*/, __in UINT nLogType /*= LOGTYPE_NONE*/, __in BOOL bOnlyLogType /*= FALSE*/)
{
	if (bError)
	{
		OnLog_Err(lpszLog);
	}
	else
	{
		OnLog(lpszLog);
	}
}

//=============================================================================
// Method		: OnAdd_Log
// Access		: virtual public  
// Returns		: void
// Parameter	: __in LPCTSTR lpszLog
// Parameter	: __in UINT nLogType
// Qualifier	:
// Last Update	: 2021/6/11 - 18:44
// Desc.		:
//=============================================================================
void CView_MainCtrl_ICS::OnAdd_Log(__in LPCTSTR lpszLog, __in UINT nLogType /*= LOGTYPE_NONE*/)
{
	if (!GetSafeHwnd())
		return;

	if (NULL == lpszLog)
		return;

	try
	{
		TCHAR		strTime[255] = { 0 };
		UINT_PTR	nLogSize = _tcslen(lpszLog) + 255;
		LPTSTR		lpszOutLog = new TCHAR[nLogSize];
		SYSTEMTIME	LocalTime;

		GetLocalTime(&LocalTime);
		StringCbPrintf(strTime, sizeof(strTime), _T("[%02d:%02d:%02d.%03d] "), LocalTime.wHour, LocalTime.wMinute, LocalTime.wSecond, LocalTime.wMilliseconds);
		StringCbPrintf(lpszOutLog, nLogSize, _T("%s%s\r\n"), strTime, lpszLog);

		if (LOGTYPE_ERROR == nLogType)
			m_Log_ErrLog.LogWrite(lpszOutLog);

		m_wndLogView.OnAdd_Log(lpszOutLog, nLogType);
		m_logFile.LogWrite(lpszOutLog);

		delete[] lpszOutLog;
	}
	catch (...)
	{
		TRACE(_T("*** Exception Error : CView_MainCtrl_Calibration::AddLog () \n"));
	}
}

//=============================================================================
// Method		: OnInit_PermissionMode
// Access		: virtual public  
// Returns		: void
// Parameter	: __in enPermissionMode nAcessMode
// Qualifier	:
// Last Update	: 2022/10/19 - 19:40
// Desc.		:
//=============================================================================
void CView_MainCtrl_ICS::OnInit_PermissionMode(__in enPermissionMode nAcessMode)
{
	__super::OnSet_PermissionMode(nAcessMode);

	// UI 갱신
	m_wndMainView.Set_PermissionMode(nAcessMode);
	m_wndSocketView.Set_PermissionMode(nAcessMode);
	m_wndSettingView.Set_PermissionMode(nAcessMode);
	m_ViewSub.Set_PermissionMode(nAcessMode);

	//TRACE(_T("\nChanged Permission : %s\n"), g_szPermissionMode[nAcessMode]);
	//OnLog(_T("Changed Permission : %s"), g_szPermissionMode[nAcessMode]);
}

//=============================================================================
// Method		: OnSet_PermissionMode
// Access		: virtual public  
// Returns		: void
// Parameter	: __in enPermissionMode nAcessMode
// Qualifier	:
// Last Update	: 2021/6/11 - 18:45
// Desc.		:
//=============================================================================
void CView_MainCtrl_ICS::OnSet_PermissionMode(__in enPermissionMode nAcessMode)
{
	__super::OnSet_PermissionMode(nAcessMode);

	// UI 갱신
	m_wndMainView.Set_PermissionMode(nAcessMode);
	m_wndSocketView.Set_PermissionMode(nAcessMode);
	m_wndSettingView.Set_PermissionMode(nAcessMode);
	m_ViewSub.Set_PermissionMode(nAcessMode);
	
	TRACE(_T("\nChanged Permission : %s\n"), g_szPermissionMode[nAcessMode]);
	OnLog(_T("Changed Permission : %s"), g_szPermissionMode[nAcessMode]);
	ST_User_Unit stLogin;
	if (m_regInspInfo.Get_LogIn_User(stLogin))
	{
		TRACE(_T("\nUser Login => id: %s, name: %s, level: %d\n"), stLogin.szID, stLogin.szName, stLogin.nAuthority);
		OnLog(_T("User Login => id: %s, name: %s, level: %d"), stLogin.szID, stLogin.szName, stLogin.nAuthority);

		// 설비들에게 사용자 권한 통보
		//2023.01.26a uhkim
		if (m_pIcsComm->IsOpened())
		{
			OnSend_UserLevel(m_stInspInfo.PermissionMode, stLogin.szID.GetBuffer());
			//OnSend_UserLevel((enPermissionMode)stLogin.nAuthority, stLogin.szID.GetBuffer());
		}		
#if (USE_XML)
		if (m_pIcsServer->IsOpened())
		{
			OnSend_UserLevel(ICS_SERVER_EES,
							m_stInspInfo.PermissionMode, 
							stLogin.szID.GetBuffer());
			//OnSend_UserLevel((enPermissionMode)stLogin.nAuthority, stLogin.szID.GetBuffer());
		}
#endif
	}
	else
	{
		TRACE(_T("\nUser Logout\n"));
		OnLog(_T("User Logout"));

		// 설비들에게 사용자 권한 통보
		//2023.01.26a uhkim
		if (m_pIcsComm->IsOpened())
		{
			OnSend_UserLevel(enPermissionMode::Permission_Operator, _T(""));
		}
	}
}

//=============================================================================
// Method		: Proc_LineCtrlCmd
// Access		: public  
// Returns		: void
// Parameter	: __in uint8_t IN_nCmdIndex
// Qualifier	:
// Last Update	: 2021/12/29 - 11:51
// Desc.		: 전체 설비 통신
//=============================================================================
void CView_MainCtrl_ICS::Proc_LineCtrlCmd(__in uint8_t IN_nCmdIndex)
{
	//CString szText;
	//szText.Format(_T("Line Control Command: %d (%s)"), IN_nCmdIndex, g_szCtrlCode[IN_nCmdIndex]);
	//OnLog(_T("Line Control Command: %d (%s)"), IN_nCmdIndex, g_szCtrlCode[IN_nCmdIndex]);
	//LT_MessageBox(szText);

	bool bResult = true;

	CString szText;
	//szText.Format(_T("Do you want to ""%s"" all equipment?"), g_szCtrlCode[IN_nCmdIndex]);
	if (m_nLanguage == enLanguage::Lang_Chinese)
	{
		szText.Format(g_szMessageBox_T[MB_Eqp_AllEqp_Control][m_nLanguage], g_szCtrlCode[IN_nCmdIndex], g_szCtrlCode[IN_nCmdIndex]);
	}
	else
	{
		szText.Format(g_szMessageBox_T[MB_Eqp_AllEqp_Control][m_nLanguage], g_szCtrlCode[IN_nCmdIndex]);
	}

	switch (IN_nCmdIndex)
	{
	case enConrolCode::CC_Init:
		if (IDYES == LT_MessageBox(szText, MB_YESNO))
		{
			bResult = OnSend_Init();
		}
		break;

	case enConrolCode::CC_Reset:
		if (IDYES == LT_MessageBox(szText, MB_YESNO))
		{
			//bResult = OnSend_Reset();
			// 설비 수율, 소켓 수율, NG Count 초기화 용도로 사용

			OnLog(_T("[eqp all] Reset All : Eqp Yield, Socket Yield, NG Count"));

			// 설비 수율 초기화
			if (false == Get_LineInfo().Write_CSV_File(m_stInspInfo.Path.szReport.GetBuffer()))
			{
				// 저장에 실패
				OnLog_Err(_T("Equipment Yield File write failed!"));
			}
			Get_LineInfo().Reset_EquipmentYield_All();

			// 소켓 수율 초기화
			if (false == Get_SocketInfo().Write_CSV_File(m_stInspInfo.Path.szReport.GetBuffer()))
			{
				// 저장에 실패
				OnLog_Err(_T("Socket JIG Yield File write failed!"));
			}
			Get_SocketInfo().Reset_Yield_All();

			// NG Count 초기화
			if (false == m_ViewSub.Reset_NGCount_All())
			{
				// 저장에 실패
				OnLog_Err(_T("NG Count File write failed!"));
				//LT_MessageBox(g_szMessageBox_T[MB_NGCount_Save_Failed_CSV][m_nLanguage]);
			}
			Get_FailInfo().Reset_Count_All();
		}
		break;

	case enConrolCode::CC_Run:
		if (IDYES == LT_MessageBox(szText, MB_YESNO))
		{
			bResult = OnSend_Run();
		}
		break;

	case enConrolCode::CC_Stop:
		if (IDYES == LT_MessageBox(szText, MB_YESNO))
		{
			bResult = OnSend_Stop();
		}
		break;

	case enConrolCode::CC_AlarmOff:
		if (IDYES == LT_MessageBox(szText, MB_YESNO))
		{
			bResult = OnSend_BuzzerOff();
		}
		break;

	case enConrolCode::CC_Skip:
		break;

	case enConrolCode::CC_Show:
		if (IDYES == LT_MessageBox(szText, MB_YESNO))
		{
			bResult = OnSend_ShowHide(true);
		}
		break;

	case enConrolCode::CC_Hide:
		if (IDYES == LT_MessageBox(szText, MB_YESNO))
		{
			bResult = OnSend_ShowHide(false);
		}
		break;

#ifndef HIDE_NOT_USE_EQP_CONTROL
	case enConrolCode::CC_ForcedEject:
		break;
#endif

	case enConrolCode::CC_EndOfProduction:
		if (IDYES == LT_MessageBox(szText, MB_YESNO))
		{
			bResult = Proc_EndOfProduction(true);
		}
		break;

	default:
		break;
	}

	if (false == bResult)
	{
		//LT_MessageBox(_T("A communication error with the handler occurred."));
		LT_MessageBox(g_szMessageBox_T[MB_Eqp_Comm_Error][m_nLanguage]);
	}
}

//=============================================================================
// Method		: Proc_EquipmentCtrlCmd
// Access		: public  
// Returns		: void
// Parameter	: __in uint8_t IN_nCmdIndex
// Parameter	: __in uint8_t IN_nEqpOrder
// Qualifier	:
// Last Update	: 2021/12/27 - 19:03
// Desc.		: 개별 설비 통신
//=============================================================================
void CView_MainCtrl_ICS::Proc_EquipmentCtrlCmd(__in uint8_t IN_nCmdIndex, __in uint8_t IN_nEqpOrder)
{
	bool bResult = true;

	if (IN_nEqpOrder <  Get_LineInfo().GetCount())
	{
		CString szText;
		//szText.Format(_T("Are you sure you want to ""%s"" the selected equipment?"), g_szCtrlCode[IN_nCmdIndex]);
		if (m_nLanguage == enLanguage::Lang_Chinese)
		{
			szText.Format(g_szMessageBox_T[MB_Eqp_SelectEqp_Control][m_nLanguage], g_szCtrlCode[IN_nCmdIndex], g_szCtrlCode[IN_nCmdIndex]);
		}
		else
		{
			szText.Format(g_szMessageBox_T[MB_Eqp_SelectEqp_Control][m_nLanguage], g_szCtrlCode[IN_nCmdIndex]);
		}

		switch (IN_nCmdIndex)
		{
		case enConrolCode::CC_Init:
			if (IDYES == LT_MessageBox(szText, MB_YESNO))
			{
				bResult = OnSend_Init(IN_nEqpOrder);
			}
			
			break;

		case enConrolCode::CC_Reset:
			if (IDYES == LT_MessageBox(szText, MB_YESNO))
			{
				bResult = OnSend_Reset(IN_nEqpOrder);
			}
			break;

		case enConrolCode::CC_Run:
			if (IDYES == LT_MessageBox(szText, MB_YESNO))
			{
				bResult = OnSend_Run(IN_nEqpOrder);
			}
			break;

		case enConrolCode::CC_Stop:
			if (IDYES == LT_MessageBox(szText, MB_YESNO))
			{
				bResult = OnSend_Stop(IN_nEqpOrder);
			}
			break;

		case enConrolCode::CC_AlarmOff:
			if (IDYES == LT_MessageBox(szText, MB_YESNO))
			{
				bResult = OnSend_BuzzerOff(IN_nEqpOrder);
			}
			break;

		case enConrolCode::CC_Skip:
		{
			if (Get_Equipment(IN_nEqpOrder).Is_Tester())
			{
				if (Get_Equipment(IN_nEqpOrder).Get_EnableEquipment())
				{
					if (Get_Equipment(IN_nEqpOrder).Get_Skip())
					{
						// 설비를 사용 상태로 설정 하시겠습니까?
						//if (IDYES == LT_MessageBox(__T("Are you sure you want to set the Equipment to ""Enabled"" status?"), MB_YESNO))
						if (IDYES == LT_MessageBox(g_szMessageBox_T[MB_Eqp_Enable_Status][m_nLanguage], MB_YESNO))
						{
							Get_Equipment(IN_nEqpOrder).Set_Skip(false);

							OnLog(_T("[eqp %02d] Set Skip Equipment : Enable"), IN_nEqpOrder);
						}
					}
					else
					{
						// 설비를 미사용 상태로 설정 하시겠습니까?
						//if (IDYES == LT_MessageBox(_T("Are you sure you want to set the Equipment to ""Disabled"" status?"), MB_YESNO))
						if (IDYES == LT_MessageBox(g_szMessageBox_T[MB_Eqp_Disable_Status][m_nLanguage], MB_YESNO))
						{
							Get_Equipment(IN_nEqpOrder).Set_Skip(true);

							OnLog(_T("[eqp %02d] Set Skip Equipment : Disable"), IN_nEqpOrder);
						}
					}
				}
				else
				{
					// 비활성화된 설비 입니다.
					//LT_MessageBox(_T("Disabled equipment."));
					LT_MessageBox(g_szMessageBox_T[MB_Eqp_Disabled][m_nLanguage]);
				}
			}
// 			else
// 			{
// 				// 검사 설비가 아닙니다.
// 				LT_MessageBox(_T("It is not an inspection equipment."));
//				LT_MessageBox(g_szMessageBox_T[MB_Eqp_NotTester][m_nLanguage]);
// 			}
		}
			break;

		case enConrolCode::CC_Show:
			if (IDYES == LT_MessageBox(szText, MB_YESNO))
			{
				bResult = OnSend_ShowHide(IN_nEqpOrder, true);
			}
			break;

		case enConrolCode::CC_Hide:
			if (IDYES == LT_MessageBox(szText, MB_YESNO))
			{
				bResult = OnSend_ShowHide(IN_nEqpOrder, false);
			}
			break;

#ifndef HIDE_NOT_USE_EQP_CONTROL
		case enConrolCode::CC_ForcedEject:
			if (IDYES == LT_MessageBox(szText, MB_YESNO))
			{
				bResult = OnSend_Forced_SocketTrackOut(IN_nEqpOrder, true);
			}
			break;
#endif

		case enConrolCode::CC_EndOfProduction:
			if (IDYES == LT_MessageBox(szText, MB_YESNO))
			{
				bResult = OnSend_EndOfProduction(IN_nEqpOrder);
			}
			break;

		case enConrolCode::CC_ResetYield:
			if (IDYES == LT_MessageBox(szText, MB_YESNO))
			{
				OnLog(_T("[eqp %02d] Reset Yield Equipment"), IN_nEqpOrder);

				// 수율 초기화
				// 로더에서 초기화하면 전체 설비를 초기화 한다.
				if (IN_nEqpOrder == Get_Loader().Get_EqpOrder())
				{
					if (false == Get_LineInfo().Write_CSV_File(m_stInspInfo.Path.szReport.GetBuffer()))
					{
						// 저장에 실패
						OnLog_Err(_T("Equipment Yield File write failed!"));
					}
					Get_LineInfo().Reset_EquipmentYield_All();

					for (auto nIdx = 0; nIdx < Get_EquipmentCount(); ++nIdx)
					{
						Get_Equipment(nIdx).Reset_Yield_Day();
					}
				}
				else
				{
					Get_Equipment(IN_nEqpOrder).Reset_Yield_Day();
				}
			}
			break;

		case enConrolCode::CC_SaveYield:
			//if(IDYES == LT_MessageBox(_T("Save the yield as a csv file?"), MB_YESNO))
			if (IDYES == LT_MessageBox(g_szMessageBox_T[MB_Eqp_SaveYield_CSV][m_nLanguage], MB_YESNO))
			{
				// 저장 할 경로를 선택하세요.
				CString szInitPath = _T("C:\\");
				CFolderPickerDialog Picker(szInitPath, OFN_FILEMUSTEXIST, NULL, 0);
				if (Picker.DoModal() == IDOK)
				{
					CString szFolderPath = Picker.GetPathName();

					bResult = Get_LineInfo().Write_CSV_File(szFolderPath);

					OnLog(_T("[eqp all] Save the yield (csv file: %s)"), szFolderPath.GetBuffer());
				}
			}
			break;

		//case enConrolCode::CC_Dummy:
			/*if (IDYES == LT_MessageBox(szText, MB_YESNO))
			{
				Get_Equipment(IN_nEqpOrder).Reset_Yield_Cumulative();
			}*/
		//	break;

		default:
			break;
		}
	}

	if (false == bResult)
	{
		//LT_MessageBox(_T("A communication error with the handler occurred."));
		LT_MessageBox(g_szMessageBox_T[MB_Eqp_Comm_Error][m_nLanguage]);
	}
}

//=============================================================================
// Method		: Test_Process
// Access		: virtual public  
// Returns		: void
// Parameter	: __in UINT nTestNo
// Qualifier	:
// Last Update	: 2021/5/10 - 10:09
// Desc.		:
//=============================================================================
void CView_MainCtrl_ICS::Test_Process(__in UINT nTestNo)
{
	switch (nTestNo)
	{
	case 0:
	{
		//::MessageBox((HWND)NULL, _T("MODELESS MessageBox"), _T("Equipment tcp/ip disconnected"), MB_OK);
		LT_MessageBox(g_szMessageBox_T[MB_Eqp_Comm_Error][m_nLanguage]);

		//OnSend_OperationActiveStatus(enOperationActiveStatus::OAS_Active);

		//OnEvent_Loader_CheckTrackOut(_T("H-220215-001"));

		/*auto HWnd = GetSafeHwnd();
		for (auto nIdx = 0; nIdx < 8; ++nIdx)
		{
			::SendNotifyMessage(HWnd, WM_EVENT_EQUIPMENT_AUTO_MODE, (WPARAM)nIdx, (LPARAM)0);
		}*/

		// Loader 투입

		// Tester 투입

		// Tester 배출

		/*Get_Equipment(6).Set_PortClear(2);
		Get_Equipment(6).Set_PortStatus(2, enPortStatus::PtS_Exist_Socket, _T("H-220215-000"), _T("test"));

		Delay(3000);

		auto nTime = Get_Equipment(6).Get_ElapsedTime_InputPara(0);

		CString szText;
		szText.Format(_T("time : %d sec"), nTime);
		AfxMessageBox(szText);*/


		/*srand((unsigned int)time(NULL));
		int num = 0;
		int num2 = 0;

		for (auto nIdx = 1; nIdx < 50; ++nIdx)
		{
			num = rand();
			int16_t nSN = (int16_t)num % 13;
			//nSN = (nSN == 16) ? -1 : nSN;

			num2 = rand();
			Get_Equipment(3).Recv_NotifyTestResult(_T("V-220215-001"), nSN, num2 % 2);

			//Delay(20);
			Delay(((nSN <= 0) ? 1 : nSN)  * 1000);
		}*/

		//Get_Socket(_T("H-220215-001")).m_stTestResult.m_nNG_Code = 32;
		//Get_Socket(_T("H-220215-001")).m_stTestResult.m_nNG_Para = 0;
		//Get_Equipment(0).Recv_UnregisterSocket(_T("H-220215-001"));

	}
	break;

	case 1:
	{
		//OnSend_OperationActiveStatus(enOperationActiveStatus::OAS_Inactive);

		std::thread thr([this]()
		{
			srand((unsigned int)time(NULL));
			int num = 0;
			int num2 = 0;
#if (SET_INSPECTOR == SYS_ICS_TRINITY_LINE)
			CString szRFID = _T("H-221021-002");
#else
			CString szRFID = _T("A-230114-002");
#endif


			// Test
			for (auto nIdx = 1; nIdx < 133; ++nIdx)
			{
				// 설비
				for (auto nEqp = Get_Loader().Get_EqpOrder() + 1; nEqp < Get_Returner().Get_EqpOrder(); ++nEqp)
				{
					num = rand();
					int16_t nSN = (int16_t)num % 32;
					nSN = (nSN == 31) ? -1 : nSN;

					nSN = ((nSN % 7)) ? 0 : nSN;

					num2 = rand();

					Get_Equipment(nEqp).Recv_NotifyTestResult(szRFID.GetBuffer(), nSN, num2 % 2);

					Sleep(20);

					if (0 < nSN)
						break;
				}

				// Unload
				Sleep(20);

				ST_TestResult testResult;
				Get_Loader().Recv_ReqTestResult(szRFID.GetBuffer(), testResult);
				Sleep(200);

				// Reset
				Get_Loader().Recv_UnregisterSocket(szRFID.GetBuffer());
				//Get_Socket(szRFID.GetBuffer()).Reset_TestResult();
			}

			AfxMessageBox(_T("Test is Done!!"));
		});
		thr.detach();
		
		//Get_Equipment(0).Recv_UnregisterSocket(_T("H-220215-002"));
	}
	break;

	case 2:
	{
//		OnSend_EndOfProduction(false);

		//m_stInspInfo.Set_LastRegisterTime();
		//OnEvent_Loader_RegisterSocket(_T("A-230114-011"));

		//H-220823-011

		Proc_SetSocketTargetEquipment(_T("H-220823-011"), 1);

		std::thread thr([this]()
		{
			Get_Equipment(2).Recv_ReqAcceptSocket(_T("H-220823-011"));
		});
		thr.detach();

		//Delay(10);
		std::thread thr2([this]()
		{
			Get_Equipment(2).Recv_ReqAcceptSocket(_T("H-220823-011"));
		});
		thr2.detach();
		//Delay(10);
		Get_Equipment(2).Recv_ReqAcceptSocket(_T("H-220823-011"));
		Delay(50);
		Get_Equipment(2).Recv_ReqAcceptSocket(_T("H-220823-011"));
		Delay(30);
		Get_Equipment(2).Recv_ReqAcceptSocket(_T("H-220823-011"));
		Delay(300);
		Get_Equipment(2).Recv_ReqAcceptSocket(_T("H-220823-011"));
	}
	break;

	case 3:
	{
		Get_Socket(_T("H-220823-001")).Set_TargetEqpOrder(3);
		Get_Socket(_T("H-220823-002")).Set_TargetEqpOrder(3);
		Get_Socket(_T("H-220823-003")).Set_TargetEqpOrder(3);
		Get_Socket(_T("H-220823-004")).Set_TargetEqpOrder(3);

		Get_Equipment(3).Increase_ReservedPort(_T("H-220823-001"));
		Get_Equipment(3).Increase_ReservedPort(_T("H-220823-002"));
		Get_Equipment(3).Increase_ReservedPort(_T("H-220823-003"));
		Get_Equipment(3).Increase_ReservedPort(_T("H-220823-004"));
		Get_Equipment(3).Increase_ReservedPort(_T("H-220823-001"));


		Get_Equipment(3).Decrease_ReservedPort(_T("H-220823-001"));
		Get_Equipment(3).Decrease_ReservedPort(_T("H-220823-002"));


// 		srand((unsigned int)time(NULL));
// 		int num = 0;
// 
// 		CString szRFID, szBarcode;
// 		uint16_t nRFID_idx = 1;
// 		uint16_t nRFID_idx2 = 1;
// 		for (auto nCount = 0; nCount < 10; nCount++)
// 		{
// 			// 로더
// 			for (auto nEqpIdx = 0; nEqpIdx < Get_LineInfo().GetCount(); nEqpIdx++)
// 			{
// 				if (Get_Equipment(nEqpIdx).Is_Loader())
// 				{
// 					szRFID.Format(_T("H-220215-%03d"), nRFID_idx2++);
// 
// 					num = rand();
// 					uint16_t nSN = (uint16_t)num % 1000;
// 					szBarcode.Format(_T("SN_%05d"), nSN);
// 					Get_Equipment(nEqpIdx).Recv_RegisterSocket(szRFID.GetBuffer(), szBarcode.GetBuffer());
// 				}
// 			}
// 		}
// 
// 		for (auto nCount = 0; nCount < 10; nCount++)
// 		{
// 			// 검사 설비
// 			for (auto nEqpIdx = 0; nEqpIdx < Get_LineInfo().GetCount(); nEqpIdx++)
// 			{
// 				if (Get_Equipment(nEqpIdx).Is_Tester())
// 				{
// 					// 파라 L / R 수율 업데이트
// 					szRFID.Format(_T("H-220215-%03d"), nRFID_idx++);
// 
// 					num = rand();
// 					uint16_t nNGcode = (uint8_t)num % 2;
// 					//Delay(11);
// 					num = rand();
// 					uint8_t nPara = (uint8_t)num % 2;
// 
// 					Get_Equipment(nEqpIdx).Recv_NotifyTestResult(szRFID.GetBuffer(), nNGcode, nPara);
// 				}
// 
// 				Delay(47);
// 			}
// 
// 			Delay(100);
// 		}

// 		for (auto nCount = 0; nCount < 10; nCount++)
// 		{
// 			// 언로더
// 			nRFID_idx2 = 1;
// 			for (auto nEqpIdx = 0; nEqpIdx < Get_LineInfo().GetCount(); nEqpIdx++)
// 			{
// 				if (Get_Equipment(nEqpIdx).Is_Loader())
// 				{
// 					szRFID.Format(_T("H-220215-%03d"), nRFID_idx2++);
// 					Get_Equipment(nEqpIdx).Recv_UnregisterSocket(szRFID.GetBuffer());
// 				}
// 			}
// 		}
		
		/*nRFID_idx = 0;
		nRFID_idx2 = 0;
		for (auto nCount = 0; nCount < 10; nCount++)
		{
			// 로더
			for (auto nEqpIdx = 0; nEqpIdx < Get_LineInfo().GetCount(); nEqpIdx++)
			{
				if (Get_Equipment(nEqpIdx).Is_Loader())
				{
					szRFID.Format(_T("H-220215-%03d"), nRFID_idx2++);

					num = rand();
					uint16_t nSN = (uint16_t)num % 1000;
					szBarcode.Format(_T("SN_%05d"), nSN);
					Get_Equipment(nEqpIdx).Recv_RegisterSocket(szRFID.GetBuffer(), szBarcode.GetBuffer());
				}
			}
		}

		for (auto nCount = 0; nCount < 10; nCount++)
		{
			// 검사 설비
			for (auto nEqpIdx = 0; nEqpIdx < Get_LineInfo().GetCount(); nEqpIdx++)
			{
				if (Get_Equipment(nEqpIdx).Is_Tester())
				{
					// 파라 L / R 수율 업데이트
					szRFID.Format(_T("H-220215-%03d"), nRFID_idx++);

					num = rand();
					uint16_t nNGcode = (uint8_t)num % 2;
					//Delay(11);
					num = rand();
					uint8_t nPara = (uint8_t)num % 2;

					Get_Equipment(nEqpIdx).Recv_NotifyTestResult(szRFID.GetBuffer(), nNGcode, nPara);
				}

				Delay(47);
			}

			Delay(100);
		}*/

// 		for (auto nCount = 0; nCount < 10; nCount++)
// 		{
// 			// 언로더
// 			nRFID_idx2 = 1;
// 			for (auto nEqpIdx = 0; nEqpIdx < Get_LineInfo().GetCount(); nEqpIdx++)
// 			{
// 				if (Get_Equipment(nEqpIdx).Is_Loader())
// 				{
// 					szRFID.Format(_T("H-220215-%03d"), nRFID_idx2++);
// 					Get_Equipment(nEqpIdx).Recv_UnregisterSocket(szRFID.GetBuffer());
// 				}
// 			}
// 		}




		//Get_SocketInfo().Set_TestResult(_T("TL01_CC01"), _T("RFID_0001"), 3, 1, 1, _T("TL01_CC01"));


		/*std::list<std::pair<Key, Data> >*/


		/*
		BYTE pDataBuffer[4095] = {0, };
		DWORD dwDataCount = 0;
		DWORD dwBufferLen = 0;
		DWORD	dwBytesRead = 4095;

		CString szPassword = _T("Sork01@#Sork01@#Sork01@#");
		CString szPassword_R;

		dwDataCount = szPassword.GetLength() * sizeof(TCHAR);
		dwBufferLen = dwDataCount;
		memcpy(pDataBuffer, szPassword.GetBuffer(), dwDataCount);

		if (EncryptData(pDataBuffer, &dwDataCount, dwBufferLen))
		{
			TRACE("Enc = %d \n", dwDataCount);

			if (DecryptData(pDataBuffer, dwBufferLen, &dwDataCount))
			{
				pDataBuffer[dwBytesRead] = 0x00;
				szPassword_R = (TCHAR*)pDataBuffer;

				TRACE("Dec = %d : %s \n", dwDataCount, szPassword_R.GetBuffer());
			}
		}
		*/


	}
	break;

	default:
		break;
	}
}


#if (USE_XML)
bool CView_MainCtrl_ICS::OnLoad_ServerInfo(__in bool IN_bNotifySettingWnd /*= true*/)
{
	// Line Config 불러오기
	CXmlLineServerConfig xml;
	CString szLog;
	CString szFullPath;
	szFullPath.Format(_T("%s%s"), m_stInspInfo.Path.szRecipe.GetBuffer(), SERVER_INFO_FILENAME);
	OnLog(_T("Server Info file load: %s")
		, szFullPath.GetBuffer());
	// 현재의 라인 설정 백업
	bool bServerRestart = false;
	CConfig_Server Old_Line = Get_SettingInfo().ServerInfo;
	if (xml.LoadXML_ServerInfo(szFullPath.GetBuffer(), Get_SettingInfo().ServerInfo))
	{
		// 예전 설정과 비교
		if (Compare_ServerConfig_ServRestart(&Old_Line, &Get_SettingInfo().ServerInfo))
		{
			OnLog(_T("Line Cofnig Changed => Server Restart!!"));
			// 설비와의 통신 연결 해제
			if (false == IN_bNotifySettingWnd)
			{
				OnDisconnect_Devicez(ICS_SERVER_EES);
				m_pIcsServer->RemoveRemotes();
			}
		}
		// 레시피의 라인 구성을 메인 데이터에 세팅
		Get_ServerInfo().Set_Config_Line(&Get_SettingInfo().ServerInfo);
		// 레지스트리 체크
		CRegServer*	pRegEqp = new CRegServer();
		for (auto nIdx = 0; nIdx < Get_ServerInfo().GetCount(); ++nIdx)
		{
			pRegEqp->Check_RegistryPath(Get_Server(nIdx).Get_ServerId());
			pRegEqp->Load_Equipment_ID(Get_Server(nIdx).Get_ServerId(),	Get_Server(nIdx));
		}
		delete pRegEqp;

		Get_ServerInfo().Set_Path(m_stInspInfo.Path.szLog, m_stInspInfo.Path.szReport);

		// Line Config log
		OnLog(_T("------------------- Line Config -------------------"));

		OnLog(_T("Socket\t: %s"), g_szSocketTypeName[Get_ServerInfo().m_nSocketType]);
		OnLog(_T("Model\t: %s"), g_szModelType[Get_ServerInfo().m_nModelType]);

		for (auto nIdx = 0; nIdx < Get_ServerInfo().GetCount(); ++nIdx)
		{
			OnLog(_T("[eqp %02d] %s:%s (%s, %s) \t: %s"), nIdx,
				g_szServerTypeName[Get_Server(nIdx).Get_ServerType()],
				g_szServerUIName[Get_Server(nIdx).Get_SvrType_UI()],
				Get_ServerID(nIdx).GetBuffer(),
				ConvIPAddrToString(Get_Server(nIdx).Get_IP_Address()),
				Get_Server(nIdx).Get_EnableServer() ? _T("enable") : _T("disable"));
		}
		OnLog(_T("---------------------------------------------------"));
		// 레시피 변경에 따른 UI 업데이트
		m_wndMainView.Update_ServerInfo();
		// 프로그램 시작 할 때 세팅 윈도우로 데이터 갱신 요청
		if (IN_bNotifySettingWnd)
		{
			m_wndSettingView.UpdateUI_ServerInfo();
		}
		OnMatchingServer();
		// 모델 정보 불러오기 완료
		OnLog(_T("File load completed. [File: %s]"), szFullPath);
		return true;
	}
	else
	{
		OnLog_Err(_T("File load failed. [File: %s]"), szFullPath);
		return false;
	}
}
bool CView_MainCtrl_ICS::OnLoad_EESInfo(__in bool IN_bNotifySettingWnd /*= true*/)
{
	// SV List 불러오기
	CXmlEESConfig xml;
	CString szLog;
	CString szFullPath;
	szFullPath.Format(_T("%s%s"), m_stInspInfo.Path.szRecipe.GetBuffer(), EES_INFO_FILENAME);
	OnLog(_T("SV Info file load: %s"), szFullPath.GetBuffer());

	// 현재의 라인 설정 백업
	bool bServerRestart = false;	
	if (xml.LoadXML_EESInfo(szFullPath.GetBuffer(), Get_SettingInfo().EES_Info))
	{
		if (IN_bNotifySettingWnd)
		{
			m_wndSettingView.UpdateUI_EESInfo();
		}
		// 설비 정보와 설비 UI를 다시 매칭 함
		// 모델 정보 불러오기 완료
		OnLog(_T("File load completed. [File: %s]"), szFullPath);
		return true;
	}
	else
	{
		OnLog_Err(_T("File load failed. [File: %s]"), szFullPath);
		return false;
	}
	return true;
}
bool CView_MainCtrl_ICS::OnLoad_ALInfo(__in bool IN_bNotifySettingWnd /*= true*/)
{
	// SV List 불러오기
	CXmlALConfig xml;
	CString szLog;
	CString szFullPath;
	szFullPath.Format(_T("%s%s"), m_stInspInfo.Path.szRecipe.GetBuffer(), AL_INFO_FILENAME);
	OnLog(_T("AL Info file load: %s"), szFullPath.GetBuffer());
	// 현재의 라인 설정 백업
	bool bServerRestart = false;
	if (xml.LoadXML_ALInfo(szFullPath.GetBuffer(), Get_SettingInfo().AL_Info))
	{
		if (IN_bNotifySettingWnd)
		{
			m_wndSettingView.UpdateUI_ALIDInfo();
		}
		// 모델 정보 불러오기 완료
		OnLog(_T("File load completed. [File: %s]"), szFullPath);
		return true;
	}
	else
	{
		OnLog_Err(_T("File load failed. [File: %s]"), szFullPath);
		return false;
	}
	return true;
}
bool CView_MainCtrl_ICS::OnLoad_LossInfo(__in bool IN_bNotifySettingWnd /*= true*/)
{
	// SV List 불러오기
	CXmlLossConfig xml;
	CString szLog;
	CString szFullPath;
	szFullPath.Format(_T("%s%s"), m_stInspInfo.Path.szRecipe.GetBuffer(), LOSS_INFO_FILENAME);
	OnLog(_T("AL Info file load: %s"), szFullPath.GetBuffer());
	// 현재의 라인 설정 백업
	bool bServerRestart = false;
	if (xml.LoadXML_LossInfo(szFullPath.GetBuffer(), Get_SettingInfo().Loss_Info))
	{
		if (IN_bNotifySettingWnd)
		{
			m_wndSettingView.UpdateUI_LossInfo();
		}
		OnLog(_T("File load completed. [File: %s]"), szFullPath);
		return true;
	}
	else
	{
		OnLog_Err(_T("File load failed. [File: %s]"), szFullPath);
		return false;
	}
	return true;
}
void CView_MainCtrl_ICS::OnMatchingServer()
{
	__super::OnMatchingServer();
	auto nSvrCnt = Get_ServerInfo().GetCount();
	if (0 < nSvrCnt) {
		for (auto nIdx = 0; nIdx < nSvrCnt; ++nIdx)
		{
			Get_Server(nIdx).Set_OwnerHWND(GetSafeHwnd());
			Get_Server(nIdx).Set_GUI_HWND(m_wndMainView.Get_ServerHWND(nIdx));//수정 필요.			
			Get_Server(nIdx).Set_WinMsg_Notify(WM_SERVER_NOTIFY);
			Get_Server(nIdx).SetPtr_SocketInfo(&m_stInspInfo.SocketInfo);
		}
	}
	OnLog(_T("Matching Server..."));
}
bool CView_MainCtrl_ICS::Compare_ServerConfig_ServRestart(__in CConfig_Server* IN_Old, __in CConfig_Server* IN_New)
{
	if (IN_Old->GetCount() == IN_New->GetCount())	
	{
		auto nCount = IN_New->GetCount();
		for (auto nEqpIdx = 0; nEqpIdx < nCount; ++nEqpIdx)		
		{
			if (IN_Old->GetAt(nEqpIdx).Get_IP_Address() != IN_New->GetAt(nEqpIdx).Get_IP_Address())			
			{
				return true;
			}
			if (0 != IN_Old->GetAt(nEqpIdx).Get_ServerId().Compare(IN_New->GetAt(nEqpIdx).Get_ServerId()))
			{
				return true;
			}
		}
		return false;
	}
	return true;
}
void CView_MainCtrl_ICS::OnSet_Connection(__in LPARAM IN_LPARAM) {
	enSocketState state = (enSocketState)IN_LPARAM;
	if (0 < Get_ServerCount()) {
		for (int IN_From = 0; IN_From < Get_ServerCount(); IN_From++) {
			//if (Get_Server(IN_From).Get_ClientConnection()) {
				//for (int n_Port = 0; n_Port < Get_Server(IN_From).Get_EquipmentIDCount(); n_Port++)
				//{
			switch (state) {
			case SOCKETSTATE_CLOSE:
				Get_Server(IN_From).Set_ClientConnection(ONLINESTATE_OFFLINE);
				OnDisconnect_Devicez(ICS_SERVER_EES);
				break;
			case SOCKETSTATE_OPEN:
				OnConnect_Devicez(ICS_SERVER_EES);
				break;
			}
			OnShow_SplashScreen(false);
			//}
		//}
		}
	}

}
void CView_MainCtrl_ICS::Proc_ServerRequestLinkTest(__in WPARAM IN_WPARAM, __in LPARAM IN_LPARAM)
{
	uint8_t IN_nSvrOrder = (uint8_t)IN_WPARAM;
	OnEvent_ServerREQUEST_LINK_TEST(IN_nSvrOrder, IN_LPARAM);
}
void CView_MainCtrl_ICS::Proc_ServerReplyLinkTest(__in WPARAM IN_WPARAM, __in LPARAM IN_LPARAM)
{
	uint8_t IN_nSvrOrder = (uint8_t)IN_WPARAM;
	OnEvent_ServerREPLY_LINK_TEST(IN_nSvrOrder, IN_LPARAM);
}
void CView_MainCtrl_ICS::Proc_ServerReportOnlineState(__in WPARAM IN_WPARAM, __in LPARAM IN_LPARAM)
{
	uint8_t IN_nSvrOrder = (uint8_t)IN_WPARAM;
	OnEvent_ServerREPORT_ONLINE_STATE(IN_nSvrOrder, IN_LPARAM);
}
void CView_MainCtrl_ICS::Proc_ServerReportUserChange(__in WPARAM IN_WPARAM, __in LPARAM IN_LPARAM)
{
	uint8_t IN_nSvrOrder = (uint8_t)IN_WPARAM;
	OnEvent_ServerREPORT_USER_CHANGE(IN_nSvrOrder, IN_LPARAM);
}
void CView_MainCtrl_ICS::Proc_ServerRequestUserChange(__in WPARAM IN_WPARAM, __in LPARAM IN_LPARAM)
{
	uint8_t IN_nSvrOrder = (uint8_t)IN_WPARAM;
	OnEvent_ServerREQUEST_USER_CHANGE(IN_nSvrOrder, IN_LPARAM);
}
void CView_MainCtrl_ICS::Proc_ServerReplyUserCommand(__in WPARAM IN_WPARAM, __in LPARAM IN_LPARAM)
{
	uint8_t IN_nSvrOrder = (uint8_t)IN_WPARAM;
	OnEvent_ServerREPLY_USER_COMMAND(IN_nSvrOrder, IN_LPARAM);
}
void CView_MainCtrl_ICS::Proc_ServerReportEquipmentState(__in WPARAM IN_WPARAM, __in LPARAM IN_LPARAM)
{
	uint8_t IN_nSvrOrder = (uint8_t)IN_WPARAM;
	OnEvent_ServerREPORT_EQUIPMENT_STATE(IN_nSvrOrder, IN_LPARAM);
}
void CView_MainCtrl_ICS::Proc_ServerRequestEquipmentStateDisplay(__in WPARAM IN_WPARAM, __in LPARAM IN_LPARAM)
{
	uint8_t IN_nSvrOrder = (uint8_t)IN_WPARAM;
	OnEvent_ServerREQUEST_EQUIPMENT_STATE_DISPLAY(IN_nSvrOrder, IN_LPARAM);
}
void CView_MainCtrl_ICS::Proc_ServerReplyEquipmentStateDisplay(__in WPARAM IN_WPARAM, __in LPARAM IN_LPARAM)
{
	uint8_t IN_nSvrOrder = (uint8_t)IN_WPARAM;
	OnEvent_ServerREPLY_EQUIPMENT_STATE_DISPLAY(IN_nSvrOrder, IN_LPARAM);
}
void CView_MainCtrl_ICS::Proc_ServerReplyLossState(__in WPARAM IN_WPARAM, __in LPARAM IN_LPARAM)
{
	uint8_t IN_nSvrOrder = (uint8_t)IN_WPARAM;
	OnEvent_ServerREPORT_LOSS_STATE(IN_nSvrOrder, IN_LPARAM);
}
void CView_MainCtrl_ICS::Proc_ServerRequestLossState(__in WPARAM IN_WPARAM, __in LPARAM IN_LPARAM)
{
	uint8_t IN_nSvrOrder = (uint8_t)IN_WPARAM;
	OnEvent_ServerREQUEST_LOSS_WINDOW(IN_nSvrOrder, IN_LPARAM);
}
void CView_MainCtrl_ICS::Proc_ServerReplyLossWindow(__in WPARAM IN_WPARAM, __in LPARAM IN_LPARAM)
{
	uint8_t IN_nSvrOrder = (uint8_t)IN_WPARAM;
	OnEvent_ServerREPLY_LOSS_WINDOW(IN_nSvrOrder, IN_LPARAM);
}
void CView_MainCtrl_ICS::Proc_ServerReportAlarmState(__in WPARAM IN_WPARAM, __in LPARAM IN_LPARAM)
{
	uint8_t IN_nSvrOrder = (uint8_t)IN_WPARAM;
	OnEvent_ServerREPORT_ALARM_STATE(IN_nSvrOrder, IN_LPARAM);
}
void CView_MainCtrl_ICS::Proc_ServerRequestAlarmList(__in WPARAM IN_WPARAM, __in LPARAM IN_LPARAM)
{
	uint8_t IN_nSvrOrder = (uint8_t)IN_WPARAM;
	OnEvent_ServerREQUEST_ALARM_LIST(IN_nSvrOrder, IN_LPARAM);
}
void CView_MainCtrl_ICS::Proc_ServerReplyAlarmList(__in WPARAM IN_WPARAM, __in LPARAM IN_LPARAM)
{
	uint8_t IN_nSvrOrder = (uint8_t)IN_WPARAM;
	OnEvent_ServerREPLY_ALARM_LIST(IN_nSvrOrder, IN_LPARAM);
}

void CView_MainCtrl_ICS::Proc_ServerEESMode(__in WPARAM IN_WPARAM, __in LPARAM IN_LPARAM)
{
	uint8_t IN_nSvrOrder = (uint8_t)IN_WPARAM;
	OnEvent_ServerREPORT_RMS_MODE(IN_nSvrOrder, IN_LPARAM);
}
void CView_MainCtrl_ICS::Proc_ServerRequestSetDateTime(__in WPARAM IN_WPARAM, __in LPARAM IN_LPARAM)
{
	uint8_t IN_nSvrOrder = (uint8_t)IN_WPARAM;
	OnEvent_ServerREQUEST_SET_DATETIME(IN_nSvrOrder, IN_LPARAM);
}
void CView_MainCtrl_ICS::Proc_ServerReplySetDateTime(__in WPARAM IN_WPARAM, __in LPARAM IN_LPARAM)
{
	uint8_t IN_nSvrOrder = (uint8_t)IN_WPARAM;
	OnEvent_ServerREPLY_SET_DATETIME(IN_nSvrOrder, IN_LPARAM);
}
void CView_MainCtrl_ICS::Proc_ServerRequestTerminalMessage(__in WPARAM IN_WPARAM, __in LPARAM IN_LPARAM)
{
	uint8_t IN_nSvrOrder = (uint8_t)IN_WPARAM;
	OnEvent_ServerREQUEST_TERMINAL_MESSAGE(IN_nSvrOrder, IN_LPARAM);
}
void CView_MainCtrl_ICS::Proc_ServerReplyTerminalMessage(__in WPARAM IN_WPARAM, __in LPARAM IN_LPARAM)
{
	uint8_t IN_nSvrOrder = (uint8_t)IN_WPARAM;
	OnEvent_ServerREPLY_TERMINAL_MESSAGE(IN_nSvrOrder, IN_LPARAM);
}
void CView_MainCtrl_ICS::Proc_ServerRequestOpCallMessage(__in WPARAM IN_WPARAM, __in LPARAM IN_LPARAM)
{
	uint8_t IN_nSvrOrder = (uint8_t)IN_WPARAM;
	OnEvent_ServerREQUEST_OPCALL_MESSAGE(IN_nSvrOrder, IN_LPARAM);
}
void CView_MainCtrl_ICS::Proc_ServerReplyOpCall(__in WPARAM IN_WPARAM, __in LPARAM IN_LPARAM)
{
	uint8_t IN_nSvrOrder = (uint8_t)IN_WPARAM;
	OnEvent_ServerREPLY_OPCALL_MESSAGE(IN_nSvrOrder, IN_LPARAM);
}


void CView_MainCtrl_ICS::OnSet_EESMode(__in enEES_Mode nAcessMode)
{
	__super::OnSet_EESMode(nAcessMode);
	if (m_pIcsServer->IsOpened())
	{
		if (0 < Get_ServerCount()) {
			for (int IN_From = 0; IN_From < Get_ServerCount(); IN_From++) {
				if (Get_Server(IN_From).Get_ClientConnection()) {
					//for (int n_Port = 0; n_Port < Get_Server(IN_From).Get_EquipmentIDCount(); n_Port++) 
					{
						//if (!Get_Server(IN_From).Get_EquipmentIDStatus(n_Port).Get_EQUIPMENTID().Compare(_T(""))) 
						//{
						//	continue;
						//}
						Get_Server(IN_From).Get_DEFINEDATA().Set_EESMODE(lt::ToMultiByte(g_sEES_Mode[nAcessMode]));

						Get_Server(IN_From).Get_DEFINEDATA().Get_ONLINESTATE();

						lt::XUUID ID;
						lt::Report_Online_State_Args::Args * Args = new lt::Report_Online_State_Args::Args();
						auto svr = m_pIcsServer->GetRemote(Get_ServerID(IN_From));
						if (Args != nullptr) {
							ID = Args->Hd.CreateUUID();
							svr->GetRemoteEes().AddOnlineStateProcedure(ID, *Args);
							delete Args;
						}
						else {
							ID = svr->GetRemoteEes().LinkTestCtrl();
						}
						auto cntr = svr->GetRemoteEes().OnlineStateCtrl(ID);
						cntr.REPORT.Hd.Set_transactionId(ID);
						cntr.REPORT.Hd.Set_timeStamp(cntr.REPORT.Hd.Get_timeStamp());
						cntr.REPORT.Body.Set_EESMODE(Get_Server(IN_From).Get_DEFINEDATA().Get_EESMODE());
						cntr.REPORT.Body.Set_ONLINESTATE(Get_Server(IN_From).Get_DEFINEDATA().Get_ONLINESTATE());
						cntr.REPORT.Body.Set_EQUIPMENTID(Get_Server(IN_From).Get_DEFINEDATA().Get_EQUIPMENTID());
						cntr.REPORT.Body.Set_IPADDRESS(Get_Server(IN_From).Get_DEFINEDATA().Get_IPADDRESS());
						cntr.REPORT.Body.Set_USERID(Get_Server(IN_From).Get_DEFINEDATA().Get_USERID());
						cntr.REPORT.Body.Set_SUBEQPID(Get_Server(IN_From).Get_DEFINEDATA().Get_SUBEQPID());

						m_pIcsServer->SendReportOnlineStateMassage(Get_ServerID(IN_From), cntr.REPORT);
						svr->GetRemoteEes().RemoveOnlineStateProcedure(ID);

						mPane_CommStatus->m_st_RMS.SetText(g_sEES_Mode_UI[nAcessMode]);

					}
				}
				else {
					mPane_CommStatus->m_st_RMS.SetText(g_sEES_Mode_UI[EES_OFFLINE]);
					Get_Server(IN_From).Get_DEFINEDATA().Set_EESMODE(lt::ToMultiByte(g_sEES_Mode[EES_OFFLINE]).c_str());
				}
			}
		}
	}
	else {
		mPane_CommStatus->m_st_RMS.SetText(g_sEES_Mode_UI[EES_OFFLINE]);
		for (int IN_From = 0; IN_From < Get_ServerCount(); IN_From++) {
			Get_Server(IN_From).Get_DEFINEDATA().Set_EESMODE(lt::ToMultiByte(g_sEES_Mode[EES_OFFLINE]).c_str());
		}
	}
}
#endif	
#if TEST
void CView_MainCtrl_ICS::Proc_ServerUnitIdRead(__in WPARAM IN_WPARAM, __in LPARAM IN_LPARAM)
{
	uint8_t IN_nSvrOrder = (uint8_t)IN_WPARAM;
	OnEvent_ServerUNITID_READ(IN_nSvrOrder, IN_LPARAM);
}
void CView_MainCtrl_ICS::Proc_ServerRequestUnitIdRead(__in WPARAM IN_WPARAM, __in LPARAM IN_LPARAM)
{
	uint8_t IN_nSvrOrder = (uint8_t)IN_WPARAM;
	OnEvent_ServerREQUEST_UNITID_READ(IN_nSvrOrder, IN_LPARAM);
}
void CView_MainCtrl_ICS::Proc_ServerReplytUnitIdRead(__in WPARAM IN_WPARAM, __in LPARAM IN_LPARAM)
{
	uint8_t IN_nSvrOrder = (uint8_t)IN_WPARAM;
	OnEvent_ServerREPLY_UNITID_READ(IN_nSvrOrder, IN_LPARAM);
}
void CView_MainCtrl_ICS::Proc_ServerReportStartProcess(__in WPARAM IN_WPARAM, __in LPARAM IN_LPARAM)
{
	uint8_t IN_nSvrOrder = (uint8_t)IN_WPARAM;
	OnEvent_ServerREPORT_START_PROCESS(IN_nSvrOrder, IN_LPARAM);
}
void CView_MainCtrl_ICS::Proc_ServerReportStartLot(__in WPARAM IN_WPARAM, __in LPARAM IN_LPARAM)
{
	uint8_t IN_nSvrOrder = (uint8_t)IN_WPARAM;
	OnEvent_ServerREPORT_START_LOT(IN_nSvrOrder, IN_LPARAM);
}
void CView_MainCtrl_ICS::Proc_ServerReportEndEvent(__in WPARAM IN_WPARAM, __in LPARAM IN_LPARAM)
{
	uint8_t IN_nSvrOrder = (uint8_t)IN_WPARAM;
	OnEvent_ServerREPORT_END_EVENT(IN_nSvrOrder, IN_LPARAM);
}
void CView_MainCtrl_ICS::Proc_ServerReportEndProcess(__in WPARAM IN_WPARAM, __in LPARAM IN_LPARAM)
{
	uint8_t IN_nSvrOrder = (uint8_t)IN_WPARAM;
	OnEvent_ServerREPORT_END_PROCESS(IN_nSvrOrder, IN_LPARAM);
}

//=============================================================================
// Method		: OnReport_TerminalMessage
// Access		: virtual protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2023.05.02
// Desc.		:
//=============================================================================
void CView_MainCtrl_ICS::OnReport_TerminalMessage(__in lt::Request_Terminal_Message_Args::Args&  IN_Data)
{
	__super::OnReport_TerminalMessage(IN_Data);

	CString		szTime;
	SYSTEMTIME	tmLocal;
	CString TERMINALMESSAGE;
	TERMINALMESSAGE.Format(_T("%s"),IN_Data.Body.Get_TERMINALMESSAGE().c_str());
	CString szLog;
	GetLocalTime(&tmLocal);
	szTime.Format(_T("[%02d:%02d:%02d.%03d]"), tmLocal.wHour, tmLocal.wMinute, tmLocal.wSecond, tmLocal.wMilliseconds);
	szLog.Format(_T("%s TERMINALMESSAGE %s\r\n"),
		szTime, 
		TERMINALMESSAGE);
	
	m_wndLogView.Log_TerminalMessage(szLog);
}
void CView_MainCtrl_ICS::OnReport_OpCall(__in lt::Request_Opcall_Args::Args& IN_Data)
{
	__super::OnReport_OpCall(IN_Data);

	CString		szTime;
	SYSTEMTIME	tmLocal;
	GetLocalTime(&tmLocal);
	szTime.Format(_T("[%02d:%02d:%02d.%03d]"), tmLocal.wHour, tmLocal.wMinute, tmLocal.wSecond, tmLocal.wMilliseconds);

	CString EQUIPMENTID;
	EQUIPMENTID.Format(_T("%s"), IN_Data.Body.Get_EQUIPMENTID().c_str());
	CString MESSAGE;
	MESSAGE.Format(_T("%s"), IN_Data.Body.Get_MESSAGE().c_str());
	CString TOWERLAMP;
	TOWERLAMP.Format(_T("%s"), IN_Data.Body.Get_TOWERLAMP().c_str());
	CString BUZZER;
	BUZZER.Format(_T("%s"), IN_Data.Body.Get_BUZZER().c_str());
	CString szLog;
	szLog.Format(_T("%s Message %s , TOWERLAMP %s , Buzzer %s\r\n"),
		szTime,
		MESSAGE,
		TOWERLAMP,
		BUZZER);	
	m_wndLogView.Log_OpCall(szLog);
}

//=============================================================================
// Method		: GetNtPrivilege
// Access		: virtual protected  
// Returns		: void
// Parameter	: 
// Qualifier	:
// Last Update	: 2023.05.02
// Desc.		:
//=============================================================================
BOOL CView_MainCtrl_ICS::GetNtPrivilege()
{
	BOOL bReturn = __super::GetNtPrivilege();

	return bReturn;
}

LRESULT CView_MainCtrl_ICS::OnWM_Svr_UNITID_READ(WPARAM wParam, LPARAM lParam)
{
	OnEvent_ServerUNITID_READ(wParam, lParam);
	return 1;
}
LRESULT CView_MainCtrl_ICS::OnWM_Svr_REQUEST_UNITID_READ(WPARAM wParam, LPARAM lParam)
{
	OnEvent_ServerREQUEST_UNITID_READ(wParam, lParam);
	return 1;
}
LRESULT CView_MainCtrl_ICS::OnWM_Svr_REPLY_UNITID_READ(WPARAM wParam, LPARAM lParam)
{
	OnEvent_ServerREPLY_UNITID_READ(wParam, lParam);
	return 1;
}
LRESULT CView_MainCtrl_ICS::OnWM_Svr_REPORT_START_PROCESS(WPARAM wParam, LPARAM lParam)
{
	OnEvent_ServerREPORT_START_PROCESS(wParam, lParam);
	return 1;
}
LRESULT CView_MainCtrl_ICS::OnWM_Svr_REPORT_START_LOT(WPARAM wParam, LPARAM lParam)
{
	OnEvent_ServerREPORT_START_LOT(wParam, lParam);
	return 1;
}
LRESULT CView_MainCtrl_ICS::OnWM_Svr_REPORT_END_EVENT(WPARAM wParam, LPARAM lParam)
{
	OnEvent_ServerREPORT_END_EVENT(wParam, lParam);
	return 1;
}
LRESULT CView_MainCtrl_ICS::OnWM_Svr_REPORT_END_PROCESS(WPARAM wParam, LPARAM lParam)
{
	OnEvent_ServerREPORT_END_PROCESS(wParam, lParam);
	return 1;
}
#endif