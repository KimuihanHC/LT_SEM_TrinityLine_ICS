//*****************************************************************************
// Filename	: 	TestManager_Device.cpp
// Created	:	2016/10/6 - 13:46
// Modified	:	2016/10/6 - 13:46
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
#include "stdafx.h"
#include "TestManager_Device.h"
#include "CommonFunction.h"

#if defined(EES_XML)//20231003
#include "Pane_CommStatus.h"
#endif

CTestManager_Device::CTestManager_Device()
{
	//OnInitialize();
	m_pIcsComm = &CIcsCommunicator::GetInstance();
#if defined(EES_XML)//20231003
	m_pIcsServer = &CIcsCommunicatorEes::GetInstance();
#endif
}


CTestManager_Device::~CTestManager_Device()
{
	TRACE(_T("<<< Start ~CTestManager_Device >>> \n"));

	//OnFinalize();
	if (m_pIcsComm->IsOpened())
	{
		m_pIcsComm->Close();
	}
#if defined(EES_XML)//20231003
	if (m_pIcsServer->IsOpened())
	{		
		m_pIcsServer->Close();
	}
#endif	
//	delete m_pIcsComm;


	TRACE(_T("<<< End ~CTestManager_Device >>> \n"));
}

//=============================================================================
// Method		: OnLoadOption
// Access		: virtual protected  
// Returns		: BOOL
// Qualifier	:
// Last Update	: 2016/5/18 - 18:58
// Desc.		:
//=============================================================================
BOOL CTestManager_Device::OnLoad_Option()
{
	CLT_Option	stOption;
	stOption.SetInspectorType((enInsptrSysType)SET_INSPECTOR);
	BOOL		bReturn = TRUE;

	stOption.SetRegistryPath(REG_PATH_OPTION_BASE);
	bReturn &= stOption.LoadOption_Inspector(m_stOption.Inspector);
#if defined(EES_XML)//20231003
	for (int i = 0; i < ICS_SERVER_MAX; i++) {		
		bReturn &= stOption.LoadOption_Server(i, m_stOption.Server[i]);
	}
#else		
	bReturn &= stOption.LoadOption_Server(m_stOption.Server);
#endif
	//bReturn &= stOption.LoadOption_Misc(m_stOption.Misc);
	
	return bReturn;
}

//=============================================================================
// Method		: InitDevicez
// Access		: virtual protected  
// Returns		: void
// Parameter	: HWND hWndOwner
// Qualifier	:
// Last Update	: 2016/5/18 - 18:41
// Desc.		: 주변 장치 초기화
//=============================================================================
void CTestManager_Device::OnInit_Devicez(HWND hWndOwner /*= NULL*/)
{
	

}

//=============================================================================
// Method		: OnConnect_Devicez
// Access		: virtual protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/1/29 - 14:39
// Desc.		:
//=============================================================================
#if !(EES_XML)//20231003
void CTestManager_Device::OnConnect_Devicez()
{
	OnShow_SplashScreen(TRUE, _T("Starting Server"));
	
	CString szText;
	szText = ConvIPAddrToString(m_stOption.Server.Address.dwAddress);

	if (m_pIcsComm->Open(szText.GetBuffer(), static_cast<lt::ushort>(m_stOption.Server.Address.dwPort)))
	{
		OnLog(_T("Sever Starting.. (port: %d)"), m_stOption.Server.Address.dwPort);

		OnSetStatus_Server(COMM_STATUS_CONNECT);
	}
	else
	{
		OnLog_Err(_T("Sever Start failed!! (port: %d)"), m_stOption.Server.Address.dwPort);

		OnSetStatus_Server(COMM_STATUS_NOTCONNECTED);
	}

	//OnShow_SplashScreen(FALSE);
}

//=============================================================================
// Method		: OnDisconnect_Devicez
// Access		: virtual protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/1/29 - 14:39
// Desc.		:
//=============================================================================
void CTestManager_Device::OnDisconnect_Devicez()
{
	OnShow_SplashScreen(TRUE, _T("Close Server"));

	OnLog(_T("Sever Closing.. (port: %d)"), m_stOption.Server.Address.dwPort);
	m_pIcsComm->Close();
	

	OnSetStatus_Server(COMM_STATUS_NOTCONNECTED);
	
	//OnShow_SplashScreen(FALSE);
}
#endif
//=============================================================================
// Method		: OnInitialize
// Access		: virtual public  
// Returns		: void
// Qualifier	:
// Last Update	: 2016/9/28 - 20:10
// Desc.		:
//=============================================================================
void CTestManager_Device::OnInitialize()
{
	// 테스트 코드

}

//=============================================================================
// Method		: OnFinalize
// Access		: virtual public  
// Returns		: void
// Qualifier	:
// Last Update	: 2016/9/28 - 20:10
// Desc.		:
//=============================================================================
void CTestManager_Device::OnFinalize()
{

}

#if defined(EES_XML)//20231003
//=============================================================================
// Method		: OnConnect_Devicez
// Access		: virtual protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2023/3/07 - 10:47
// Desc.		:
//=============================================================================
void CTestManager_Device::OnConnect_Devicez(int nIn)
{
	OnShow_SplashScreen(TRUE, _T("Starting Server"));
	CString szText;
	switch (nIn) {
	case ICS_SERVER_MODULE:
		szText = ConvIPAddrToString(m_stOption.Server[ICS_SERVER_MODULE].Address.dwAddress);
		if (m_pIcsComm->Open(szText.GetBuffer(), static_cast<lt::ushort>(m_stOption.Server[ICS_SERVER_MODULE].Address.dwPort)))
		{
			OnLog(_T("Sever Starting.. (port: %d)"), m_stOption.Server[ICS_SERVER_MODULE].Address.dwPort);
			OnSetStatus_Server(COMM_STATUS_CONNECT);
		}
		else
		{
			OnLog_Err(_T("Sever Start failed!! (port: %d)"), m_stOption.Server[ICS_SERVER_MODULE].Address.dwPort);
			OnSetStatus_Server(COMM_STATUS_NOTCONNECTED);
		}
		break;

	case ICS_SERVER_EES:
		szText = ConvIPAddrToString(m_stOption.Server[ICS_SERVER_EES].Address.dwAddress);
		if (m_pIcsServer->Open(szText.GetBuffer(), static_cast<lt::ushort>(m_stOption.Server[ICS_SERVER_EES].Address.dwPort)))
		{		
			mPane_CommStatus->m_st_EES.SetText(g_szSocket_StateUI[SOCKETSTATE_OPEN]);
			mPane_CommStatus->m_st_EES.SetText(g_szSocket_StateUI[SOCKETSTATE_OPEN]);
			OnLog(_T("Sever Starting.. (port: %d)"), m_stOption.Server[ICS_SERVER_EES].Address.dwPort);			
		}
		else
		{
			OnLog_Err(_T("Sever Start failed!! (port: %d)"), m_stOption.Server[ICS_SERVER_EES].Address.dwPort);
		}
		break;
	}
}
//=============================================================================
// Method		: OnDisconnect_Devicez
// Access		: virtual protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2023/03/07 - 10:47
// Desc.		:
//=============================================================================
void CTestManager_Device::OnDisconnect_Devicez(int nIn)
{
	OnShow_SplashScreen(TRUE, _T("Close Server"));
	switch (nIn) {
	case ICS_SERVER_MODULE:
		if (m_pIcsComm != nullptr) {
			OnLog(_T("Server Closing.. (port: %d)"), m_stOption.Server[ICS_SERVER_MODULE].Address.dwPort);
			m_pIcsComm->Close();
		}
		break;
	case ICS_SERVER_EES:
		if (m_pIcsServer != nullptr) {
			OnLog(_T("Server Closing.. (port: %d)"), m_stOption.Server[ICS_SERVER_EES].Address.dwPort);			
			mPane_CommStatus->m_st_EES.SetText(g_szSocket_StateUI[SOCKETSTATE_CLOSE]);
			m_pIcsServer->Close();
		}
		break;
	}
	OnSetStatus_Server(COMM_STATUS_NOTCONNECTED);
}

lt::Link_Test_Args::Args CTestManager_Device::Add_Link_TestArgs(
	__in lt::ConstStringT equipmentId, lt::Request_Link_Test_Args::Args & Args) {
	lt::XUUID ID;
	auto svr = m_pIcsServer->GetRemote(equipmentId);
	if (&Args != nullptr) {
		ID = Args.Hd.Get_transactionId();
		svr->GetRemoteEes().CreateLinkTestProcedure(ID);
		svr->GetRemoteEes().AddLinkTestProcedure(ID, Args);
		delete &Args;
	}
	else {
		ID = svr->GetRemoteEes().LinkTestCtrl();
	}
	auto cntr = svr->GetRemoteEes().LinkTestCtrl(ID);
	return cntr;
}
void CTestManager_Device::RemoveLinkTestProcedure(__in lt::ConstStringT equipmentId, lt::XUUID ID) {
	m_pIcsServer->GetRemote(equipmentId)->GetRemoteEes().RemoveLinkTestProcedure(ID);
}
lt::Online_State_Args::Args CTestManager_Device::Add_Online_StateArgs(
	__in lt::ConstStringT equipmentId, 
	lt::Report_Online_State_Args::Args & Args) {
	lt::XUUID ID;
	auto svr = m_pIcsServer->GetRemote(equipmentId);
	if (&Args != nullptr) {
		ID = Args.Hd.Get_transactionId();
		svr->GetRemoteEes().CreateOnlineStateProcedure(ID);
		svr->GetRemoteEes().AddOnlineStateProcedure(ID, Args);
		delete &Args;
	} else {
		ID = svr->GetRemoteEes().OnlineStateCtrl();
	}
	auto cntr = svr->GetRemoteEes().OnlineStateCtrl(ID);
	return cntr;
}
void CTestManager_Device::RemoveOnlineStateProcedure(__in lt::ConstStringT equipmentId, lt::XUUID ID) {
	m_pIcsServer->GetRemote(equipmentId)->GetRemoteEes().RemoveOnlineStateProcedure(ID);
}
lt::User_Change_Args::Args CTestManager_Device::Add_User_CommandArgs(
	__in lt::ConstStringT equipmentId,
	lt::Report_User_Change_Args::Args & Args) {
	lt::XUUID ID;
	auto svr = m_pIcsServer->GetRemote(equipmentId);
	if (&Args != nullptr) {
		ID = Args.Hd.Get_transactionId();
		svr->GetRemoteEes().CreateUserCommandProcedure(ID);
		svr->GetRemoteEes().AddeUserCommandProcedure(ID, Args);
		delete &Args;
	}
	else {
		ID = svr->GetRemoteEes().UserCommandCtrl();
	}
	auto cntr = svr->GetRemoteEes().UserCommandCtrl(ID);
	return cntr;
}
lt::User_Change_Args::Args CTestManager_Device::Add_User_CommandArgs(
	__in lt::ConstStringT equipmentId,
	lt::Request_User_Cpmmand_Args::Args & Args) {
	lt::XUUID ID;
	auto svr = m_pIcsServer->GetRemote(equipmentId);
	if (&Args != nullptr) {
		ID = Args.Hd.Get_transactionId();
		svr->GetRemoteEes().CreateUserCommandProcedure(ID);
		svr->GetRemoteEes().AddeUserCommandProcedure(ID, Args);
		delete &Args;
	}
	else {
		ID = svr->GetRemoteEes().UserCommandCtrl();
	}
	auto cntr = svr->GetRemoteEes().UserCommandCtrl(ID);
	return cntr;
}
lt::User_Change_Args::Args CTestManager_Device::Add_User_CommandArgs(
	__in lt::ConstStringT equipmentId,
	lt::Reply_User_Change_Args::Args & Args) {
	lt::XUUID ID;
	auto svr = m_pIcsServer->GetRemote(equipmentId);
	if (&Args != nullptr) {
		ID = Args.Hd.Get_transactionId();
		svr->GetRemoteEes().CreateUserCommandProcedure(ID);
		svr->GetRemoteEes().AddeUserCommandProcedure(ID, Args);
		delete &Args;
	}
	else {
		ID = svr->GetRemoteEes().UserCommandCtrl();
	}
	auto cntr = svr->GetRemoteEes().UserCommandCtrl(ID);
	return cntr;
}
void CTestManager_Device::RemoveUserCommandProcedure(__in lt::ConstStringT equipmentId, lt::XUUID ID) {
	m_pIcsServer->GetRemote(equipmentId)->GetRemoteEes().RemoveUserCommandProcedure(ID);
}
lt::Equipment_State_Args::Args CTestManager_Device::Add_Equipment_StateArgs(
	__in lt::ConstStringT equipmentId,
	lt::Report_Equipment_State_Args::Args & Args) {
	lt::XUUID ID;
	auto svr = m_pIcsServer->GetRemote(equipmentId);
	if (&Args != nullptr) {
		ID = Args.Hd.Get_transactionId();
		svr->GetRemoteEes().CreateEquipmentStateProcedure(ID);
		svr->GetRemoteEes().AddEquipmentStateProcedure(ID, Args);
		delete &Args;
	}
	else {
		ID = svr->GetRemoteEes().EquipmentStateCtrl();
	}
	auto cntr = svr->GetRemoteEes().EquipmentStateCtrl(ID);
	return cntr;
}
lt::Equipment_State_Args::Args CTestManager_Device::Add_Equipment_StateArgs(
	__in lt::ConstStringT equipmentId,
	lt::Request_Equipment_State_Display_Args::Args & Args) {
	lt::XUUID ID;
	auto svr = m_pIcsServer->GetRemote(equipmentId);
	if (&Args != nullptr) {
		ID = Args.Hd.Get_transactionId();
		svr->GetRemoteEes().CreateEquipmentStateProcedure(ID);
		svr->GetRemoteEes().AddEquipmentStateProcedure(ID, Args);
		delete &Args;
	}
	else {
		ID = svr->GetRemoteEes().EquipmentStateCtrl();
	}
	auto cntr = svr->GetRemoteEes().EquipmentStateCtrl(ID);
	return cntr;
}
lt::Equipment_State_Args::Args CTestManager_Device::Add_Equipment_StateArgs(
	__in lt::ConstStringT equipmentId,
	lt::Reply_Equipment_State_Display_Args::Args & Args) {
	lt::XUUID ID;
	auto svr = m_pIcsServer->GetRemote(equipmentId);
	if (&Args != nullptr) {
		ID = Args.Hd.Get_transactionId();
		svr->GetRemoteEes().CreateEquipmentStateProcedure(ID);
		svr->GetRemoteEes().AddEquipmentStateProcedure(ID, Args);
		delete &Args;
	}
	else {
		ID = svr->GetRemoteEes().EquipmentStateCtrl();
	}
	auto cntr = svr->GetRemoteEes().EquipmentStateCtrl(ID);
	return cntr;
}
void CTestManager_Device::RemoveEquipmentStateProcedure(__in lt::ConstStringT equipmentId, lt::XUUID ID) {
	m_pIcsServer->GetRemote(equipmentId)->GetRemoteEes().RemoveEquipmentStateProcedure(ID);
}
lt::Alarm_State_Args::Args CTestManager_Device::Add_Alarm_StateArgs(
	__in lt::ConstStringT equipmentId,
	lt::Report_Alarm_State_Args::Args & Args) {
	lt::XUUID ID;
	auto svr = m_pIcsServer->GetRemote(equipmentId);
	if (&Args != nullptr) {
		ID = Args.Hd.Get_transactionId();
		svr->GetRemoteEes().CreateAlarmStateProcedure(ID);
		svr->GetRemoteEes().AddAlarmStateProcedure(ID, Args);
		delete &Args;
	}
	else {
		ID = svr->GetRemoteEes().AlarmStateCtrl();
	}
	auto cntr = svr->GetRemoteEes().AlarmStateCtrl(ID);
	return cntr;
}
void CTestManager_Device::RemoveAlarmStateProcedure(__in lt::ConstStringT equipmentId, lt::XUUID ID) {
	m_pIcsServer->GetRemote(equipmentId)->GetRemoteEes().RemoveAlarmStateProcedure(ID);
}
lt::Alarm_List_Args::Args CTestManager_Device::Add_Alarm_ListArgs(
	__in lt::ConstStringT equipmentId,
	lt::Request_Alarm_List_Args::Args & Args) {
	lt::XUUID ID;
	auto svr = m_pIcsServer->GetRemote(equipmentId);
	if (&Args != nullptr) {
		ID = Args.Hd.Get_transactionId();
		svr->GetRemoteEes().CreateAlarmListProcedure(ID);
		svr->GetRemoteEes().AddAlarmListProcedure(ID, Args);
		delete &Args;
	}
	else {
		ID = svr->GetRemoteEes().AlarmListCtrl();
	}
	auto cntr = svr->GetRemoteEes().AlarmListCtrl(ID);
	return cntr;
}
lt::Alarm_List_Args::Args CTestManager_Device::Add_Alarm_ListArgs(
	__in lt::ConstStringT equipmentId,
	lt::Reply_Alarm_List_Args::Args & Args) {
	lt::XUUID ID;
	auto svr = m_pIcsServer->GetRemote(equipmentId);
	if (&Args != nullptr) {
		ID = Args.Hd.Get_transactionId();
		svr->GetRemoteEes().CreateAlarmListProcedure(ID);
		svr->GetRemoteEes().AddAlarmListProcedure(ID, Args);
		delete &Args;
	}
	else {
		ID = svr->GetRemoteEes().AlarmListCtrl();
	}
	auto cntr = svr->GetRemoteEes().AlarmListCtrl(ID);
	return cntr;
}
void CTestManager_Device::RemoveAlarmListProcedure(__in lt::ConstStringT equipmentId, lt::XUUID ID) {
	m_pIcsServer->GetRemote(equipmentId)->GetRemoteEes().RemoveAlarmListProcedure(ID);
}
lt::Set_DateTime_Args::Args CTestManager_Device::Add_Set_DateTimeArgs(
	__in lt::ConstStringT equipmentId,
	lt::Request_Set_DateTime_Args::Args & Args) {
	lt::XUUID ID;
	auto svr = m_pIcsServer->GetRemote(equipmentId);
	if (&Args != nullptr) {
		ID = Args.Hd.Get_transactionId();
		svr->GetRemoteEes().CreateSetDateTimeProcedure(ID);
		svr->GetRemoteEes().AddSetDateTimeProcedure(ID, Args);
		delete &Args;
	}
	else {
		ID = svr->GetRemoteEes().SetDateTimeCtrl();
	}
	auto cntr = svr->GetRemoteEes().SetDateTimeCtrl(ID);
	return cntr;
}
lt::Set_DateTime_Args::Args CTestManager_Device::Add_Set_DateTimeArgs(
	__in lt::ConstStringT equipmentId,
	lt::Reply_Set_DateTime_Args::Args & Args) {
	lt::XUUID ID;
	auto svr = m_pIcsServer->GetRemote(equipmentId);
	if (&Args != nullptr) {
		ID = Args.Hd.Get_transactionId();
		svr->GetRemoteEes().CreateSetDateTimeProcedure(ID);
		svr->GetRemoteEes().AddSetDateTimeProcedure(ID, Args);
		delete &Args;
	}
	else {
		ID = svr->GetRemoteEes().SetDateTimeCtrl();
	}
	auto cntr = svr->GetRemoteEes().SetDateTimeCtrl(ID);
	return cntr;
}
void CTestManager_Device::RemoveSetDateTimeProcedure(__in lt::ConstStringT equipmentId, lt::XUUID ID) {
	m_pIcsServer->GetRemote(equipmentId)->GetRemoteEes().RemoveSetDateTimeProcedure(ID);
}
#endif