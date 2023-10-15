//*****************************************************************************
// Filename	: 	Equipment.cpp
// Created	:	2021/11/19 - 10:38
// Modified	:	2021/11/19 - 10:38
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#include "stdafx.h"
#include "Server.h"

#include "Def_Equipment_Type.h"
#include "CommonFunction.h"
#include "RegServer.h"


CServer::CServer()
{
	m_pRegSvr = new CRegServer();
}
CServer::~CServer()
{
	delete m_pRegSvr;
}
CServer& CServer::operator=(const CConfig_Svr& ref)
{
	CConfig_Svr::operator=(ref);
	return *this;
}
CServer& CServer::operator=(const CServer& ref)
{
	CConfig_Svr::operator=(ref);
	CCommonModule::operator=(ref);

	m_bEnable = ref.m_bEnable;
	m_bSkip = ref.m_bSkip;
	m_nConnection = ref.m_nConnection;
	m_bTimeSync = ref.m_bTimeSync;
	m_bVerifyID = ref.m_bVerifyID;
	m_nOperMode = ref.m_nOperMode;
	m_nProcessStatus = ref.m_nProcessStatus;
	m_bLED_Status = ref.m_bLED_Status;
	m_nLanguage = ref.m_nLanguage;
	m_nModelType = ref.m_nModelType;

	m_nReservedPortCnt = ref.m_nReservedPortCnt;
#if TEST
	m_nPortStatus.clear();
	m_nPortStatus = ref.m_nPortStatus;
	m_nConveyorStatus.clear();
	m_nConveyorStatus = ref.m_nConveyorStatus;
	m_nAlarmStatus.clear();
	m_nAlarmStatus = ref.m_nAlarmStatus;
#endif
	//m_Yield_Day = ref.m_Yield_Day;
	//m_Yield_Cumulative = ref.m_Yield_Cumulative;

	memcpy(&m_tm_CheckShift, &ref.m_tm_CheckShift, sizeof(SYSTEMTIME));

	m_pSocketInfo = ref.m_pSocketInfo;

	return *this;
}

CServer& CServer::operator=(const CCommonModule& ref)
{
	CCommonModule::operator=(ref);
	return *this;
}

void CServer::Init_ServerType_UI()
{
	__super::Init_ServerType_UI();
}
//=============================================================================
// Method		: WM_Notify_Server
// Access		: protected  
// Returns		: void
// Parameter	: __in WM_Eqp_Nofity_Para IN_wParam
// Parameter	: __in LPARAM IN_lParam
// Qualifier	:
// Last Update	: 2022/1/27 - 10:09
// Desc.		:
//=============================================================================
void CServer::WM_Notify_Server(__in WM_Svr_Nofity_Para IN_wParam, __in LPARAM IN_lParam)
{
	if (m_hWndGUI)
	{
		//?òÏ†ï ?ÑÏöî
		TRACE(_T("WM_Notify_Server() => WM: %d \n"), IN_wParam);
		::SendNotifyMessage(m_hWndGUI, m_nWM_Notify, (WPARAM)IN_wParam, IN_lParam);
	}
}

//=============================================================================
// Method		: WM_Event_Equipment
// Access		: protected  
// Returns		: void
// Parameter	: __in UINT IN_nWM_Event
// Parameter	: __in LPCTSTR IN_szRFID
// Qualifier	:
// Last Update	: 2022/2/3 - 11:47
// Desc.		:
//=============================================================================
void CServer::WM_Event_Server(__in UINT IN_nWM_Event, __in LPCTSTR IN_szRFID )
{
	if (m_hWndOwner)
	{
		if ((nullptr != IN_szRFID) && (0 != _tcsclen(IN_szRFID)) && (m_pSocketInfo->Is_ExistSocket(IN_szRFID)))
		{
			::SendNotifyMessage(m_hWndOwner, IN_nWM_Event, (WPARAM)m_nSvrOrder, (LPARAM)m_pSocketInfo->GetAt(IN_szRFID).szRFID.GetBuffer());
		}
		else
		{
			::SendNotifyMessage(m_hWndOwner, IN_nWM_Event, (WPARAM)m_nSvrOrder, (LPARAM)0);
		}
	}
}
void CServer::WM_Event_Server(__in UINT IN_nWM_Event, __in LPARAM IN_lParam)
{
	if (m_hWndOwner)
	{
		TRACE(_T("WM_Event_Server() => event: %d \n"), IN_nWM_Event);
		::SendNotifyMessage(m_hWndOwner, IN_nWM_Event, (WPARAM)m_nSvrOrder, IN_lParam);
	}
}

//=============================================================================
// Method		: Is_ChangeShift
// Access		: protected  
// Returns		: bool
// Qualifier	:
// Last Update	: 2022/1/27 - 14:33
// Desc.		:
//=============================================================================
bool CServer::Is_ChangeShift(__in const SYSTEMTIME* IN_ptmCurrent){
	bool bReturn = false;
	if ((m_tm_CheckShift.wHour < 8) && (8 <= IN_ptmCurrent->wHour))
	{
		bReturn = true;
	}
	else if (8 <= IN_ptmCurrent->wHour)
	{
		if ((m_tm_CheckShift.wDay != IN_ptmCurrent->wDay) || (m_tm_CheckShift.wMonth != IN_ptmCurrent->wMonth) || (m_tm_CheckShift.wYear != IN_ptmCurrent->wYear))
		{
			bReturn = true;
		}
	}
	else
	{
		auto DiffTime = CompareSystemTime((SYSTEMTIME*)IN_ptmCurrent, &m_tm_CheckShift);

		if (86400.0f < DiffTime)
		{
			bReturn = true;
		}
	}
	return bReturn;
}
bool CServer::Is_ChangeShift(){
	SYSTEMTIME tmLocal;
	GetLocalTime(&tmLocal);

	bool bReturn = Is_ChangeShift(&tmLocal);
#if SOCKET
	Set_CheckShiftTime(&tmLocal);
#endif	//SOCKET

#ifdef USE_EQP_TACTTIME
	m_Tacttime.Set_UnloadTime(&m_tm_CheckShift);
#if SOCKET
	Save_Equipment_Tacttime();
#endif	//SOCKET
#endif	//USE_EQP_TACTTIME
	return bReturn;
}

//=============================================================================
// Last Update	: 
// Desc.		: Reg
//=============================================================================
#if SOCKET
void CServer::Save_Equipment_Skip(){
	m_pRegSvr->Set_Equipment_Skip(this);
}
#endif //SOCKET
void CServer::Save_Equipment_Reserve(){
	m_pRegSvr->Set_Equipment_Reserve(this);
}
#if SOCKET
void CServer::Save_Equipment_EndProduction(){
	m_pRegSvr->Set_Equipment_EndProduction(this);
}
void CServer::Save_Equipment_Shift(){
	m_pRegSvr->Set_Equipment_Shift(this);
}
void CServer::Save_Equipment_Port(uint8_t IN_nPortIndex){
	m_pRegSvr->Set_Equipment_Port(this, IN_nPortIndex);
}
void CServer::Save_Equipment_Conveyor(uint8_t IN_ConveyorIndex){
	m_pRegSvr->Set_Equipment_Conveyor(this, IN_ConveyorIndex);
}
void CServer::Save_Equipment_Alarm(){
	m_pRegSvr->Set_Equipment_Alarm(this);
}
void CServer::Save_Equipment_Yield(){
	m_pRegSvr->Set_Equipment_Yield(this);
}
void CServer::Save_Equipment_Tacttime(){
#ifdef USE_EQP_TACTTIME
	//CString szRegPath;
	//szRegPath.Format(_T("%s\\%s"), m_pRegEqp->Get_RegistryPath().GetBuffer(), Get_EquipmentId());

	//m_Tacttime.Save_Tacttime(szRegPath);
#endif
}
#endif //SOCKET
//=============================================================================
// Last Update	: 
// Desc.		: HWND
//=============================================================================
void CServer::Set_OwnerHWND(__in HWND IN_hWnd){
	m_hWndOwner = IN_hWnd;}
void CServer::Set_GUI_HWND(__in HWND IN_hWnd){
	m_hWndGUI = IN_hWnd;}
void CServer::Set_WinMsg_Notify(__in UINT IN_nWM_Notify){
	m_nWM_Notify = IN_nWM_Notify;}

void CServer::SetPtr_SocketInfo(__in CSocketMonitoring* IN_pSocketInfo){
	m_pSocketInfo = IN_pSocketInfo;}

//=============================================================================
// Last Update	: 
// Desc.		: SvrOrder
//=============================================================================
uint8_t CServer::Get_SvrOrder(){
	return m_nSvrOrder;}
const uint8_t CServer::Get_SvrOrder() const{
	return m_nSvrOrder;}
void CServer::Set_SvrOrder(__in uint8_t IN_nSvrOrder) {
	m_nSvrOrder = IN_nSvrOrder;}

//=============================================================================
// Last Update	: 
// Desc.		: ClientConnection
//=============================================================================
uint8_t CServer::Get_ClientConnection() const{
	return m_nConnection;}

void CServer::Set_ClientConnection(__in uint8_t IN_nConStatus){
	m_nConnection = IN_nConStatus;	
	WM_Notify_Server(WM_SvrNotify_ClientConnection, (LPARAM)m_nConnection);
#if TEST
	if (!m_nConnection) {
		Set_OldEquipmentStatus(-1);
	}
#endif
}
//=============================================================================
// Last Update	: 
// Desc.		: EnableServer
//=============================================================================
bool CServer::Get_EnableServer() const{
	return m_bEnable;}
void CServer::Set_EnableServer(__in bool IN_bEnable){
	m_bEnable = IN_bEnable;}
//=============================================================================
// Last Update	: 
// Desc.		: EnableServer	:
//=============================================================================
bool CServer::Get_VerifySvrConnection() const{
	return m_bVerifyID;
}
void CServer::Set_VerifySvrConnection(__in bool bVerified){
	m_bVerifyID = bVerified;
	WM_Notify_Server(WM_SvrNotify_VerifyEqpConnection, (LPARAM)m_bVerifyID);
}
//=============================================================================
// Last Update	: 
// Desc.		: OperatingMode	:
//=============================================================================
uint8_t CServer::Get_OperatingMode() const{
	return m_nOperMode;}

void CServer::Set_OperatingMode(__in uint8_t IN_nOperMode){
	uint8_t nOld_OperMode = m_nOperMode;
	m_nOperMode = IN_nOperMode;
	WM_Notify_Server(WM_SvrNotify_OperatingMode, (LPARAM)m_nOperMode);

	if ((nOld_OperMode != enEqpOperatingMode::EOM_Auto) &&
		(IN_nOperMode == enEqpOperatingMode::EOM_Auto))	{
		WM_Event_Server(WM_EVENT_EQUIPMENT_AUTO_MODE);
	}
}
//=============================================================================
// Last Update	: 
// Desc.		: ProcessStatus	:
//=============================================================================
uint8_t CServer::Get_ProcessStatus() const{
	return m_nProcessStatus;}

void CServer::Set_ProcessStatus(__in uint8_t IN_nStatus){
	m_nProcessStatus = IN_nStatus;
	//WM_Notify_Server(WM_SvrNotify_ProcessStatus, (LPARAM)m_nProcessStatus);
}
void CServer::Set_ProcessStatus(__in uint8_t IN_nStatus, __in uint32_t IN_nAlarmCode, __in LPCTSTR IN_szAlarmInfo){
	uint8_t Old_Status = m_nProcessStatus;
	m_nProcessStatus = IN_nStatus;

	//WM_Notify_Server(WM_SvrNotify_ProcessStatus, (LPARAM)m_nProcessStatus);

	if (enEqpProcessStatus::EPC_Alarm == IN_nStatus)
	{
#if TEST
		ST_AlarmStatus stAlarm;
		stAlarm.m_nAlarmCode = IN_nAlarmCode;
		stAlarm.m_szAlarmInfo = IN_szAlarmInfo;

		m_nAlarmStatus.push_back(stAlarm);
#endif
		WM_Event_Server(WM_EVENT_EQUIPMENT_ALARM, (LPARAM)m_nProcessStatus); // alarm on

	}
	// Alarm => ?ïÏÉÅ
	else if ((enEqpProcessStatus::EPC_Alarm == Old_Status) && (enEqpProcessStatus::EPC_Alarm != IN_nStatus))
	{
		WM_Event_Server(WM_EVENT_EQUIPMENT_ALARM, (LPARAM)m_nProcessStatus); // alarm off
	}
}

//=============================================================================
// Last Update	: 
// Desc.		: Status_LED	:
//=============================================================================
uint8_t CServer::Get_Status_LED() const{
	return m_bLED_Status;}

void CServer::Set_Status_LED(__in uint8_t IN_nLED_Status){
	m_bLED_Status = IN_nLED_Status;
	WM_Notify_Server(WM_SvrNotify_Yield, (LPARAM)m_bLED_Status);
}
//=============================================================================
// Last Update	: 
// Desc.		: Skip	:
//=============================================================================
bool CServer::Get_Skip() const{
	return m_bSkip;}

void CServer::Set_Skip(__in bool IN_bSkip, __in bool IN_bSave){
	m_bSkip = IN_bSkip;
	if (IN_bSave)	{
#if SOCKET
		Save_Equipment_Skip();
#endif	//SOCKET
	}
	WM_Notify_Server(WM_SvrNotify_Skip, (LPARAM)m_bSkip);
}
//=============================================================================
// Last Update	: 
// Desc.		: TimeSync
//=============================================================================
bool CServer::Get_TimeSync() const{
	return m_bTimeSync;
}
void CServer::Set_TimeSync(__in bool IN_bTimeSync){
	m_bTimeSync = IN_bTimeSync;
	WM_Notify_Server(WM_SvrNotify_TimeSync, (LPARAM)m_bTimeSync);
}
//=============================================================================
// Last Update	: 
// Desc.		: ReservedPortCnt
//=============================================================================

uint8_t CServer::Get_ReservedPortCnt() const {
	return m_nReservedPortCnt;
}
void CServer::Set_ReservedPortCnt(uint8_t IN_nCount, __in bool IN_bSave) {
	m_nReservedPortCnt = IN_nCount;
	if (IN_bSave) {

		Save_Equipment_Reserve();

	}
}
//=============================================================================
// Last Update	: 
// Desc.		: ReservedPort
//=============================================================================
bool CServer::Increase_ReservedPort(){
	bool bReturn = false;
	if (Is_EES())	{
		if (m_nReservedPortCnt < m_nAvablePortCnt)		{
			++m_nReservedPortCnt;
			bReturn = true;
		}	else		{
			++m_nReservedOvered;
		}
		Save_Equipment_Reserve();
	}
	return bReturn;
}
void CServer::Decrease_ReservedPort()
{
	if (Is_EES())	{
		if (0 < m_nReservedOvered)		{
			--m_nReservedOvered;
		}	else if (0 < m_nReservedPortCnt)	{
			--m_nReservedPortCnt;
		}
		Save_Equipment_Reserve();
	}
}

//=============================================================================
// Last Update	: 
// Desc.		: ReservedOverCnt
//=============================================================================
uint8_t CServer::Get_ReservedOverCnt() const{
	return m_nReservedOvered;
}
void CServer::Set_ReservedOverCnt(__in uint8_t IN_nCount){
	m_nReservedOvered = IN_nCount;
}
#if SOCKET
//=============================================================================
// Last Update	: 
// Desc.		: CheckShiftTime
//=============================================================================
const SYSTEMTIME & CServer::Get_CheckShiftTime() const{
	return m_tm_CheckShift;
}
void CServer::Set_CheckShiftTime(SYSTEMTIME* IN_ptmCheck, bool IN_bSave){
	memcpy(&m_tm_CheckShift, IN_ptmCheck, sizeof(SYSTEMTIME));
	if (IN_bSave)	{
		Save_Equipment_Shift();
	}
}
#endif	//SOCKET
CString CServer::GetDateTime(){
	SYSTEMTIME stime = {};
	::GetLocalTime(&stime);
	CString cstTimeStamp;
	cstTimeStamp.Format(_T("%04d%02d%02d%02d%02d%03d"), stime.wYear,
		stime.wMonth,
		stime.wDay,
		stime.wHour,
		stime.wMinute,
		stime.wSecond);
	return cstTimeStamp;
}

#ifdef USE_EQP_TACTTIME
const CTactTime & CServer::Get_Tacttime() const{
	return m_Tacttime;
}
#endif

//=============================================================================
// Last Update	: 
// Desc.		: Alarm
//=============================================================================
bool CServer::IsAlarm(){
	return (enEqpProcessStatus::EPC_Alarm == m_nProcessStatus);
}


//=============================================================================
// Method		: IsEmpty_Equipment
// Access		: public  
// Returns		: bool
// Parameter	: __in bool bIgnore_EmptySocket
// Qualifier	:
// Last Update	: 2022/2/19 - 16:50
// Desc.		: ?§ÎπÑ???åÏºì??Ï°¥Ïû¨ ??Î¨??êÎã®
//=============================================================================
bool CServer::IsEmpty_Equipment(__in bool bIgnore_EmptySocket)
{
	auto nStartIdx = 0;
#if SOCKET
	auto nEndIdx = m_nPortStatus.size();

	if (Is_EES())
	{
		nStartIdx = enPortIndex_Loader::PtI_L_RFID;
		nEndIdx = enPortIndex_Loader::PtI_L_Buffer_3 + 1;
	}

	// ?¨Ìä∏Í∞Ä ÎπÑÏñ¥ ?àÎÇò?
	for (auto nIdx = nStartIdx; nIdx < nEndIdx; ++nIdx)
	{
		// PtS_Exist_Socket,	// 1 : ?úÌíà ?àÏùå	
		// PtS_Wait_Out,		// 2 : Î∞∞Ï∂ú?ÄÍ∏?

		// ?¨Ìä∏???åÏºì ??Î¨??êÎã®
		switch (m_nPortStatus.at(nIdx).nStatus)
		{
		case PtS_Empty:
		case PtS_Disable:
			break;

		case PtS_Exist_Socket:
		case PtS_Wait_Out:
		{
			if (bIgnore_EmptySocket)
			{
				// ?åÏºì???úÌíà???ÜÎäî Îπ??åÏºì?∏Í??
				if (false == m_nPortStatus.at(nIdx).szBarcode.IsEmpty())
				{
					return false;
				}
				else if (m_pSocketInfo->Is_ExistSocket(m_nPortStatus.at(nIdx).szRfid))
				{
					if (false == m_pSocketInfo->GetAt(m_nPortStatus.at(nIdx).szRfid).IsEmpty_Barcode())
					{
						return false;
					}
				}
			}
			else
			{
				return false;
			}
		}
		break;

		case PtS_Alarm:
			// ?åÎûå ?ÅÌÉú?êÏÑú???åÏºì ??Î¨??êÎã®???¥ÎñªÍ≤??¥Ïïº ?òÎäîÍ∞Ä??????
			break;

		default:
			break;
		}
	}

	for (auto nIdx = 0; nIdx < m_nConveyorStatus.size(); ++nIdx)
	{
		// Ïª®Î≤†?¥Ïñ¥?ÑÏóê ?åÏºì ??Î¨??êÎã®
		if (enConveyorStatus_Exist::CoSE_Exist == m_nConveyorStatus.at(nIdx).nExistSocket)
		{
			if (bIgnore_EmptySocket)
			{
				if (false == m_nConveyorStatus.at(nIdx).szBarcode.IsEmpty())
				{
					return false;
				}
				else if (m_pSocketInfo->Is_ExistSocket(m_nConveyorStatus.at(nIdx).szRfid))
				{
					if (false == m_pSocketInfo->GetAt(m_nConveyorStatus.at(nIdx).szRfid).IsEmpty_Barcode())
					{
						return false;
					}
				}
			}
			else
			{
				return false;
			}
		}
	}
#endif

	return true;
}
#if SOCKET
//=============================================================================
// Method		: IsLastSocket_onTestPort
// Access		: public  
// Returns		: bool
// Qualifier	:
// Last Update	: 2022/4/12 - 14:41
// Desc.		:
//=============================================================================
bool CServer::IsLastSocket_onTestPort()
{
	bool bReturn = false;

	if (Is_EES())
	{
		uint8_t nTestCount = 0;

		if ((PtS_Exist_Socket == m_nPortStatus.at(enPortIndex_Tester::PtI_T_RFID).nStatus) ||
			(PtS_Wait_Out == m_nPortStatus.at(enPortIndex_Tester::PtI_T_RFID).nStatus))
		{
			return false;
		}
		//for (uint8_t nIdx = enPortIndex_Tester::PtI_T_Buffer; nIdx <= enPortIndex_Tester::PtI_T_Test_R; ++nIdx)
		for (uint8_t nIdx = enPortIndex_Tester::PtI_T_Buffer; nIdx < m_nPortStatus.size(); ++nIdx)
		{
			switch (m_nPortStatus.at(nIdx).nStatus)
			{
			case PtS_Exist_Socket:
			case PtS_Wait_Out:
				++nTestCount;
				break;

			default:
				break;
			}
		}

		if (1 == nTestCount)
		{
			bReturn = true;
		}
	}

	return bReturn;
}
#endif

//=============================================================================
// Method		: Get_EmptyPortCount
// Access		: virtual public  
// Returns		: uint8_t
// Parameter	: __in bool bCount_EmptySocket
// Qualifier	:
// Last Update	: 2022/2/19 - 14:22
// Desc.		:
//=============================================================================
uint8_t CServer::Get_EmptyPortCount(__in bool bCount_EmptySocket)
{
	uint8_t nCount = 0;

	if (Is_EES())
	{
		//for (uint8_t nIdx = PtI_T_Buffer; nIdx <= PtI_T_Test_R; ++nIdx)
#if SOCKET
		for (uint8_t nIdx = enPortIndex_Tester::PtI_T_Buffer; nIdx < m_nPortStatus.size(); ++nIdx)
		{
			if (enPortStatus::PtS_Empty == m_nPortStatus.at(nIdx).nStatus)
			{
				++nCount;
			}
		}
#endif
		// PtI_T_RFID : Ïπ¥Ïö¥???òÏ? ?äÏùå (Í≤Ä?¨Ï? Í¥Ä?®Îêú PortÎß?Ï≤¥ÌÅ¨)

		// ?¨Ïö© ?àÏïΩ Ï§ëÏù∏Í∞Ä?
		if (0 < m_nReservedPortCnt)
		{
			// ?§Ï†ú ÎπÑÏñ¥?àÎäî ?¨Ìä∏?êÏÑú ?àÏïΩ???¨Ìä∏ Í∞?àòÎ•??úÏô∏?úÎã§.
			nCount = (m_nReservedPortCnt < nCount) ? (nCount - m_nReservedPortCnt) : 0;
		}
	}
	else // ?àÏïΩ ?ÜÏùå, Ï≤¥ÌÅ¨ ?ÑÏöî ?ÜÏùå
	{

	}

	return nCount;
}
#if SOCKET
//=============================================================================
// Method		: Get_EmptyConveyorCount
// Access		: virtual public  
// Returns		: uint8_t
// Qualifier	:
// Last Update	: 2022/8/1 - 19:30
// Desc.		:
//=============================================================================
uint8_t CServer::Get_EmptyConveyorCount()
{
	uint8_t nCount = 0;

	if (Is_EES())
	{
		for (uint8_t nIdx = CvI_T_RFID; nIdx <= CvI_T_Test; ++nIdx)
		{
			if (enConveyorStatus_Exist::CoSE_Empty == m_nConveyorStatus.at(nIdx).nExistSocket)
			{
				++nCount;
			}
		}
	}
	return nCount;
}

uint8_t CServer::Get_UsingPortCount()
{
	uint8_t nCount = 0;

	if (Is_EES())
	{
		for (uint8_t nIdx = enPortIndex_Tester::PtI_T_Buffer; nIdx < m_nPortStatus.size(); ++nIdx)
		{
			if (enPortStatus::PtS_Exist_Socket == m_nPortStatus.at(nIdx).nStatus)
			{
				++nCount;
			}
		}
	}
	else // ?àÏïΩ ?ÜÏùå, Ï≤¥ÌÅ¨ ?ÑÏöî ?ÜÏùå
	{

	}

	return nCount;
}

//=============================================================================
// Method		: Get_TestingCount
// Access		: public  
// Returns		: uint8_t
// Qualifier	:
// Last Update	: 2022/2/21 - 14:30
// Desc.		:
//=============================================================================
uint8_t CServer::Get_TestingCount()
{
	if (Is_EES())
	{
		uint8_t nCount = 0;

		for (uint8_t nPortIdx = enPortIndex_Tester::PtI_T_Test_L; nPortIdx < m_nPortStatus.size(); ++nPortIdx)
		{
			if ((enPortStatus::PtS_Exist_Socket == m_nPortStatus.at(nPortIdx).nStatus) ||
				(enPortStatus::PtS_Wait_Out == m_nPortStatus.at(nPortIdx).nStatus))
			{
				++nCount;
			}
		}

		return nCount;
	}
	else
	{
		return 0;
	}
}

//=============================================================================
// Method		: Get_WaitOutCount
// Access		: public  
// Returns		: uint8_t
// Qualifier	:
// Last Update	: 2022/2/21 - 14:35
// Desc.		:
//=============================================================================
uint8_t CServer::Get_WaitOutCount()
{
	if (Is_EES())
	{
		uint8_t nCount = 0;

		for (uint8_t nPortIdx = enPortIndex_Tester::PtI_T_Test_L; nPortIdx < m_nPortStatus.size(); ++nPortIdx)
		{
			if (enPortStatus::PtS_Wait_Out == m_nPortStatus.at(nPortIdx).nStatus)
			{
				++nCount;
			}
		}

		return nCount;
	}
	else
	{
		return 0;
	}
}

//=============================================================================
// Method		: Get_ExistSocketCount
// Access		: public  
// Returns		: uint8_t
// Qualifier	:
// Last Update	: 2022/2/10 - 16:50
// Desc.		:
//=============================================================================
uint8_t CServer::Get_SocketCount()
{
	// ?§ÎπÑ ?¥Ïóê Ï°¥Ïû¨?òÎäî ?åÏºì Í∞?àò
	uint8_t nCount = 0;

	for (auto nIdx = 0; nIdx < m_nPortStatus.size(); ++nIdx)
	{
		switch (m_nPortStatus.at(nIdx).nStatus)
		{
		case enPortStatus::PtS_Exist_Socket:
		case enPortStatus::PtS_Wait_Out:
		{
			++nCount;
		}
		break;

		default:
			break;
		}
	}

	return 0;
}

uint8_t CServer::Get_ProductCount()
{
	// ?§ÎπÑ ?¥Ïóê Ï°¥Ïû¨?òÎäî ?úÌíà???§Î¶∞ ?åÏºì Í∞?àò
	uint8_t nRetCount = 0;

	auto nStartIdx = 0;
	auto nEndIdx = m_nPortStatus.size();

	if (Is_EES())
	{
		nStartIdx = enPortIndex_Loader::PtI_L_RFID;
		nEndIdx = enPortIndex_Loader::PtI_L_Buffer_3 + 1;
	}

	for (auto nIdx = nStartIdx; nIdx < nEndIdx; ++nIdx)
	{
		switch (m_nPortStatus.at(nIdx).nStatus)
		{
		case enPortStatus::PtS_Exist_Socket:
		case enPortStatus::PtS_Wait_Out:
		{
			if (false == m_nPortStatus.at(nIdx).szBarcode.IsEmpty())
			{
				++nRetCount;
			}
			else if (m_pSocketInfo->Is_ExistSocket(m_nPortStatus.at(nIdx).szRfid))
			{
				if (false == m_pSocketInfo->GetAt(m_nPortStatus.at(nIdx).szRfid).IsEmpty_Barcode())
				{
					++nRetCount;
				}
			}
		}
		break;

		default:
			break;
		}
	}

	return nRetCount;
}

//=============================================================================
// Method		: Check_EndProduction
// Access		: public  
// Returns		: bool
// Qualifier	:
// Last Update	: 2022/2/21 - 15:31
// Desc.		:
//=============================================================================
bool CServer::Check_EndProduction()
{

	return false;
}

//=============================================================================
// Method		: Get_InputAvailabilityStatus
// Access		: virtual public  
// Returns		: uint8_t
// 					-> IAS_NoInput,		// ?ÑÏ≤¥ ?¨Ïö© Î∂àÍ?	
// 					-> IAS_Bypass,		// Bypass Í∞Ä??
// 					-> IAS_Test,		// Í≤Ä??Í∞Ä??
// Qualifier	:
// Last Update	: 2022/2/14 - 16:00
// Desc.		:
//=============================================================================
uint8_t CServer::Get_InputAvailabilityStatus()
{
	// Í≤Ä???§ÎπÑ ?
	if (Is_EES())
	{
		//-----------------------------------------------------------
		// ?§ÎπÑ ?åÎûå ?
		if (IsAlarm())
		{
			TRACE(_T("[eqp %02d] Get_InputAvailabilityStatus() => Alarm \n"), m_nSvrOrder);
			return enInputAvailabilityStatus::IAS_NoInput;
		}

		//-----------------------------------------------------------
		// Ïª®Î≤†?¥Ïñ¥ ?ÅÌÉú Ï≤¥ÌÅ¨??
		if ((enConveyorStatus::CoS_Stop == m_nConveyorStatus.at(CvI_T_Test).nStatus) &&
			(enConveyorStatus_Exist::CoSE_Exist == m_nConveyorStatus.at(CvI_T_Test).nExistSocket)) // 0: Empty, 1: Exist
		{
			TRACE(_T("[eqp %02d] Get_InputAvailabilityStatus() => not usable conveyor \n"), m_nSvrOrder);
			return enInputAvailabilityStatus::IAS_NoInput;
		}

		//-----------------------------------------------------------
		// ?§ÎπÑ Skip ?//
		if (Get_Skip())
		{
			return enInputAvailabilityStatus::IAS_Bypass;
		}

		//-----------------------------------------------------------
		// ?¨Ìä∏ Ï≤¥ÌÅ¨ (PtI_T_RFID, PtI_T_Buffer, PtI_T_Test_L, PtI_T_Test_R)
		// Í≤Ä???åÎùº Î∞?Î≤ÑÌçº ?¨ÏûÖ Í∞Ä??/
		bool bEmpty = false;
		for (uint8_t nIdx = enPortIndex_Tester::PtI_T_Buffer; nIdx < m_nPortStatus.size(); ++nIdx)
		{
			if (enPortStatus::PtS_Empty == m_nPortStatus.at(nIdx).nStatus)
			{
				bEmpty = true;
				break;
			}
		}

		//if ((enPortStatus::PtS_Empty == m_nPortStatus.at(PtI_T_Test_L).nStatus) ||
		//	(enPortStatus::PtS_Empty == m_nPortStatus.at(PtI_T_Test_R).nStatus) ||
		//	(enPortStatus::PtS_Empty == m_nPortStatus.at(PtI_T_Buffer).nStatus))
		if (bEmpty)
		{
			// ?¨Ïö© ?àÏïΩ Ï§ëÏù∏Í∞Ä?
			if (0 < Get_EmptyPortCount())
			{
				TRACE(_T("[eqp %02d] Get_InputAvailabilityStatus() => testable \n"), m_nSvrOrder);
				return enInputAvailabilityStatus::IAS_Test;
			}
		}

		//-----------------------------------------------------------
		// RFID
		if (enPortStatus::PtS_Empty == m_nPortStatus.at(PtI_T_RFID).nStatus)
		{
			TRACE(_T("[eqp %02d] Get_InputAvailabilityStatus() => bypass : rfid empty \n"), m_nSvrOrder);

			return enInputAvailabilityStatus::IAS_Bypass;
		}
	}
	else // Î°úÎçî : Ï≤¥ÌÅ¨ ?ÑÏöî ?ÜÏùå
	{
		return enInputAvailabilityStatus::IAS_Test;
	}

	TRACE(_T("[returner] Get_InputAvailabilityStatus() => Impossible to put into the next equipment \n"), m_nSvrOrder);
	return enInputAvailabilityStatus::IAS_NoInput;
}
#endif
//=============================================================================
// Method		: Get_ElapsedTime_InputPara
// Access		: virtual public  
// Returns		: uint32_t
// Parameter	: __in uint8_t IN_nPara
// Qualifier	:
// Last Update	: 2022/5/12 - 17:27
// Desc.		:
//=============================================================================
uint32_t CServer::Get_ElapsedTime_InputPara(__in uint8_t IN_nPara)
{
	return 0;
}




#if SOCKET
//=============================================================================
// Method		: Recv_RegisterSocket
// Access		: public  
// Returns		: bool
// Parameter	: __in LPCTSTR IN_szRFID
// Parameter	: __in LPCTSTR IN_szBarcode
// Qualifier	:
// Last Update	: 2022/1/25 - 20:25
// Desc.		: 
//=============================================================================
bool CServer::Recv_RegisterSocket(__in LPCTSTR IN_szRFID, __in LPCTSTR IN_szBarcode)
{
	if (Is_EES())
	{
		if (m_pSocketInfo)
		{
			// ?úÌíà ?àÏùå / ?úÌíà ?ÜÏùå ?
			if (0 < _tcslen(IN_szBarcode))
			{

			}
			if (m_pSocketInfo->Is_ExistSocket(IN_szRFID))
			{
				bool bReturn = m_pSocketInfo->Register_Socket(m_szServerId, IN_szRFID, IN_szBarcode);
				WM_Event_Server(WM_EVENT_LOADER_RESISTER_SOCKET, IN_szRFID);
				return bReturn;
			}
		}
	}
	return false;
}
//=============================================================================
// Method		: Recv_ReqAcceptSocket
// Access		: public  
// Returns		: bool
// Parameter	: __in LPCTSTR IN_szRFID
// Qualifier	:
// Last Update	: 2022/1/24 - 16:28
// Desc.		:
//=============================================================================
bool CServer::Recv_ReqAcceptSocket(__in LPCTSTR IN_szRFID)
{
	if (m_hWndOwner)
	{
		if (m_pSocketInfo)
		{
			//if (m_pSocketInfo->Is_ExistSocket(IN_szRFID))
			if (m_pSocketInfo->Verify_Socket(IN_szRFID))
			{
				WM_Event_Server(WM_EVENT_TESTER_TRACKIN, IN_szRFID);

				return true;
			}
		}
	}

	return false;
}

//=============================================================================
// Method		: Recv_NotifyTestResult
// Access		: public  
// Returns		: bool
// Parameter	: __in LPCTSTR IN_szRFID
// Parameter	: __in int16_t IN_nNGCode
// Parameter	: __in uint8_t IN_nPara
// Qualifier	:
// Last Update	: 2022/1/26 - 12:08
// Desc.		:
//=============================================================================
bool CServer::Recv_NotifyTestResult(__in LPCTSTR IN_szRFID, __in int16_t IN_nNGCode, __in uint8_t IN_nPara)
{
	if (Is_EES())
	{
		// Shift Î≥ÄÍ≤?Ï≤¥ÌÅ¨ & Tacttime Ï≤¥ÌÅ¨
		if (Is_ChangeShift())
		{
			Report_Yield_Day();

			if (m_bAutoReset_Yield)
			{
				Reset_Yield_Day();
			}
		}

		// ??Î∂?Ï≤¥ÌÅ¨
		if (0 == IN_nNGCode) // MES Rework NG Code ?¥Î©¥...???
		{
			Increase_Yield_Pass(IN_nPara);
		}
		else if (m_pSocketInfo->Get_MES_ReworkCode() == IN_nNGCode)// MES Rework NG Code ?¥Î©¥...???
		{
			;
		}
		else
		{
			Increase_Yield_Fail(IN_nPara);

			// Log : Î∂àÎüâ??Î∞úÏÉù???úÌíà ?ïÎ≥¥ Í∏∞Î°ù (?úÌíà Î∞îÏΩî?? NG Î∞úÏÉù Í≤Ä?? Pass??Í≤Ä??
			IncreaseFailInfo(IN_nNGCode, IN_nPara);
		}

		if (m_pSocketInfo)
		{
			//if (m_pSocketInfo->Set_TestResult(m_szEquipmentId, IN_szRFID, IN_nNGCode, IN_nPara, m_nEqpOrder, m_nEquipmentType, (m_pSocketInfo->Get_MES_ReworkCode() == IN_nNGCode)))
			if (m_pSocketInfo->Set_TestResult(m_szServerId, IN_szRFID, IN_nNGCode, IN_nPara, m_nSvrOrder, Get_ServerType(), (m_pSocketInfo->Get_MES_ReworkCode() == IN_nNGCode)))
			{
				if (m_pSocketInfo->Is_ExistSocket(IN_szRFID))
				{
					// Í≤Ä??Í≤∞Í≥ºÎ•?Î∞õÏúºÎ©?Î∞∞Ï∂ú ?πÏù∏ ?¨Î?Î•??êÎã®?¥ÏÑú ?úÎ≤Ñ->?§ÎπÑÎ°??åÎ†§Ï§òÏïº ?úÎã§.
					WM_Event_Server(WM_EVENT_TESTER_END_INSPECTION, IN_szRFID);

					return true;
				}

				return true;
			}
		}
	}

	return false;
}

//=============================================================================
// Method		: Recv_ReqTestResult
// Access		: public  
// Returns		: bool
// Parameter	: __in LPCTSTR IN_szRFID
// Parameter	: __out ST_TestResult & OUT_stResult
// Qualifier	:
// Last Update	: 2022/1/26 - 12:08
// Desc.		:
//=============================================================================
bool CServer::Recv_ReqTestResult(__in LPCTSTR IN_szRFID, __out ST_TestResult& OUT_stResult)
{
	if (Is_EES())
	{
#ifdef USE_EQP_TACTTIME
		GetLocalTime(&m_tm_CheckShift);
		m_Tacttime.Set_UnloadTime(&m_tm_CheckShift);
		Save_Equipment_Tacttime();
#endif
		WM_Event_Server(WM_EVENT_UNLOAD_REQ_TEST_RESULT, IN_szRFID);

		if (m_pSocketInfo)
		{
			m_pSocketInfo->Check_UnloadTime(IN_szRFID);

			return m_pSocketInfo->Get_TestResult(IN_szRFID, OUT_stResult);
		}
	}

	return false;
}

//=============================================================================
// Method		: Recv_UnregisterSocket
// Access		: public  
// Returns		: bool
// Parameter	: __in LPCTSTR IN_szRFID
// Qualifier	:
// Last Update	: 2022/1/26 - 12:08
// Desc.		:
//=============================================================================
bool CServer::Recv_UnregisterSocket(__in LPCTSTR IN_szRFID)
{
	bool bReturn = false;

	if (Is_EES())
	{
		// ?úÌíà???àÎäîÍ∞Ä?
		if (m_pSocketInfo->GetAt(IN_szRFID).IsEmpty_Barcode())
		{
			;
		}

		if (m_pSocketInfo->Is_ExistSocket(IN_szRFID))
		{
			// ÏµúÏ¢Ö ?òÏú® ?ÖÎç∞?¥Ìä∏
			ST_TestResult stResult;
			m_pSocketInfo->Get_TestResult(IN_szRFID, stResult);
			if (0 == stResult.m_nNG_Code)
			{
				Increase_Yield_Pass(0); // ?ëÌíà?Ä Para Íµ¨Î∂Ñ Î∂àÍ? (?ëÌíà ?êÏ†ï Í≤Ä???§ÎπÑÍ∞Ä ?¨Îü¨?Ä)
			}
			else if (m_pSocketInfo->Get_MES_ReworkCode() == stResult.m_nNG_Code)// MES Rework NG Code ?¥Î©¥...???
			{
				// ?§Î•ò ?ÅÌô©......
				Increase_Yield_Fail(stResult.m_nNG_Para);
			}
			else
			{
				Increase_Yield_Fail(stResult.m_nNG_Para);
			}

			// ?åÏºì ?±Î°ù ?¥Ï†ú
			if (m_pSocketInfo)
			{
				bReturn = m_pSocketInfo->Unregister_Socket(m_szServerId, IN_szRFID);
			}

			WM_Event_Server(WM_EVENT_UNLOAD_UNREGISTER_SOCKET, IN_szRFID);
		}
	}

	return bReturn;
}
#endif	//SOCKET



//=============================================================================
// Method		: Set_Path
// Access		: public  
// Returns		: void
// Parameter	: __in LPCTSTR IN_szLog
// Parameter	: __in LPCTSTR IN_szReport
// Qualifier	:
// Last Update	: 2022/5/25 - 12:07
// Desc.		:
//=============================================================================
void CServer::Set_Path(__in LPCTSTR IN_szLog, __in LPCTSTR IN_szReport)
{
	m_szPath_Log = IN_szLog;
	m_szPath_Report = IN_szReport;
}

#if SOCKET
//=============================================================================
// Method		: Report_Yield_Day
// Access		: public  
// Returns		: bool
// Qualifier	:
// Last Update	: 2022/5/25 - 12:07
// Desc.		:
//=============================================================================
bool CServer::Report_Yield_Day()
{
	if (m_szPath_Report.IsEmpty())
	{
		TRACE(_T("[%s] LogPath is Empty\n"), _T(__FUNCTION__));
		return false;
	}

	// ????eqpid_yield.csv

	CString szPath;
	CString szFullPath;

	SYSTEMTIME tmLocal;
	GetLocalTime(&tmLocal);

	// Report\\Year\\Month\\ 
	szPath.Format(_T("%s\\%04d\\%02d\\"), m_szPath_Report, tmLocal.wYear, tmLocal.wMonth);
	MakeDirectory(szPath);
	szFullPath.Format(_T("%sYield_%s_%04d_%02d.csv"), szPath, m_szServerId, tmLocal.wYear, tmLocal.wMonth);

	// ?????? equipment no, equpment id, T, P, F, L(T, P, F), R(T, P, F)
	CString szUnicode, szTime;
	CStringA szANSI;
	szTime.Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), tmLocal.wYear, tmLocal.wMonth, tmLocal.wDay, tmLocal.wHour, tmLocal.wMinute, tmLocal.wSecond);

	szUnicode.Format(_T(",%s,%d,%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\r\n"),
		szTime.GetBuffer(),
		m_nSvrOrder,
		m_szServerId,
		m_Yield_Day.dwTotal,
		m_Yield_Day.dwPass,
		m_Yield_Day.dwFail,
		m_Yield_Day.m_Para[Para_Left].dwTotal,
		m_Yield_Day.m_Para[Para_Left].dwPass,
		m_Yield_Day.m_Para[Para_Left].dwFail,
		m_Yield_Day.m_Para[Para_Right].dwTotal,
		m_Yield_Day.m_Para[Para_Right].dwPass,
		m_Yield_Day.m_Para[Para_Right].dwFail,
		m_Yield_Day.m_Para[Para_Center].dwTotal,
		m_Yield_Day.m_Para[Para_Center].dwPass,
		m_Yield_Day.m_Para[Para_Center].dwFail
		//szUnicode.Format(_T(",%s,%d,%s,%d,%d,%d,%d,%d,%d,%d,%d,%d\r\n"),
		//	szTime.GetBuffer(),
		//	m_nEqpOrder,
		//	m_szEquipmentId,
		//	m_Yield_Day.dwTotal,
		//	m_Yield_Day.dwPass,
		//	m_Yield_Day.dwFail,
		//	m_Yield_Day.m_Para[Para_Left].dwTotal,
		//	m_Yield_Day.m_Para[Para_Left].dwPass,
		//	m_Yield_Day.m_Para[Para_Left].dwFail,
		//	m_Yield_Day.m_Para[Para_Right].dwTotal,
		//	m_Yield_Day.m_Para[Para_Right].dwPass,
		//	m_Yield_Day.m_Para[Para_Right].dwFail
	);

	CFile File;
	CFileException e;

	if (!PathFileExists(szFullPath.GetBuffer()))
	{
		if (!File.Open(szFullPath.GetBuffer(), CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite, &e))
			return false;

		// ?§Îçî Ï∂îÍ?
		// ?????? equipment no, equpment id, T, P, F, L(T, P, F), R(T, P, F)
		//CString szHeader = _T(",Time,Eqp No,Eqp ID,Total,Pass,NG,Left_Total,Left_Pass,Left_NG,Right_Total,Right_Pass,Right_NG\r\n");
		CString szHeader = _T(",Time,Eqp No,Eqp ID,Total,Pass,NG,Left_Total,Left_Pass,Left_NG,Right_Total,Right_Pass,Right_NG,Center_Total,Center_Pass,Center_NG\r\n");

		szUnicode = szHeader + szUnicode;
	}
	else
	{
		if (!File.Open(szFullPath.GetBuffer(), CFile::modeWrite | CFile::shareDenyWrite, &e))
			return false;
	}

	USES_CONVERSION;
	szANSI = CT2A(szUnicode.GetBuffer());

	File.SeekToEnd();
	File.Write(szANSI.GetBuffer(0), szANSI.GetLength());
	File.Flush();
	File.Close();

	return true;
}
#endif	//SOCKET
void CServer::SetPtr_FailInfo_Eqp(CFailInfo_Eqp * IN_pFailInfo_Eqp)
{
	m_pFailInfo_Eqp = IN_pFailInfo_Eqp;
}

//=============================================================================
// Method		: IncreaseFailInfo
// Access		: public  
// Returns		: void
// Parameter	: int16_t IN_nNGCode
// Parameter	: uint8_t IN_nPara
// Qualifier	:
// Last Update	: 2022/5/25 - 12:07
// Desc.		:
//=============================================================================
void CServer::IncreaseFailInfo(int16_t IN_nNGCode, uint8_t IN_nPara)
{
	if (m_pFailInfo_Eqp)
	{
		m_pFailInfo_Eqp->IncreaseCount(IN_nNGCode, IN_nPara);
	}
}


//=============================================================================
// Last Update	: 
// Desc.		: EES
//=============================================================================
bool CServer::Is_EES() const
{
	return (enServerType::SERVER_EES == Get_ServerType());
}
#if defined(EES_XML)//20231003
//=============================================================================
// Method		: Set_RequestLinkTest
// Access		: public  
// Returns		: const SYSTEMTIME &
// Qualifier	: const
// Last Update	: 2022/5/12 - 15:29
// Desc.		:
//=============================================================================

void CServer::Set_SERVER_CONNECTION(__in LPARAM IN_Data)
{
	LPARAM IN_LPARAM = (LPARAM)IN_Data;
	WM_Event_Server(WM_EVENT_SERVER_CONNECTION, (LPARAM)IN_LPARAM);
}
void CServer::Set_REQUEST_LINK_TEST(__in lt::Request_Link_Test_Args::Args * IN_Data)
{
	LPARAM IN_LPARAM = (LPARAM)IN_Data;
	WM_Event_Server(WM_EVENT_SERVER_REQUEST_LINK_TEST, (LPARAM)IN_LPARAM);
}
void CServer::Set_REPLY_LINK_TEST(__in lt::Reply_Link_Test_Args::Args * IN_Data)
{
	LPARAM IN_LPARAM = (LPARAM)IN_Data;
	WM_Event_Server(WM_EVENT_SERVER_REPLY_LINK_TEST, (LPARAM)IN_LPARAM);
}
void CServer::Set_REPORT_ONLINE_STATE(__in lt::Report_Online_State_Args::Args * IN_Data)
{
	LPARAM IN_LPARAM = (LPARAM)IN_Data;
	WM_Event_Server(WM_EVENT_SERVER_REPORT_ONLINE_STATE, (LPARAM)IN_LPARAM);
}
void CServer::Set_REPORT_USER_CHANGE(__in lt::Report_User_Change_Args::Args * IN_Data)
{
	LPARAM IN_LPARAM = (LPARAM)IN_Data;
	WM_Event_Server(WM_EVENT_SERVER_REPORT_USER_CHANGE, (LPARAM)IN_LPARAM);
}
void CServer::Set_REQUEST_USER_COMMAND(__in lt::Request_User_Cpmmand_Args::Args *  IN_Data)
{
	LPARAM IN_LPARAM = (LPARAM)IN_Data;
	WM_Event_Server(WM_EVENT_SERVER_REQUEST_USER_COMMAND, (LPARAM)IN_LPARAM);
}
void CServer::Set_REPLY_USER_COMMAND(__in lt::Reply_User_Change_Args::Args * IN_Data)
{
	LPARAM IN_LPARAM = (LPARAM)IN_Data;
	WM_Event_Server(WM_EVENT_SERVER_REPLY_USER_COMMAND, (LPARAM)IN_LPARAM);
}
void CServer::Set_REPORT_EQUIPMENT_STATE(__in lt::Report_Equipment_State_Args::Args * IN_Data)
{
	LPARAM IN_LPARAM = (LPARAM)IN_Data;
	WM_Event_Server(WM_EVENT_SERVER_REPORT_EQUIPMENT_STATE, (LPARAM)IN_LPARAM);
}
void CServer::Set_REQUEST_EQUIPMENT_STATE_DISPLAY(__in lt::Request_Equipment_State_Display_Args::Args * IN_Data)
{
	LPARAM IN_LPARAM = (LPARAM)IN_Data;
	WM_Event_Server(WM_EVENT_SERVER_REQUEST_EQUIPMENT_STATE_DISPLAY, (LPARAM)IN_LPARAM);
}
void CServer::Set_REPLY_EQUIPMENT_STATE_DISPLAY(__in lt::Reply_Equipment_State_Display_Args::Args * IN_Data)
{
	LPARAM IN_LPARAM = (LPARAM)IN_Data;
	WM_Event_Server(WM_EVENT_SERVER_REPLY_EQUIPMENT_STATE_DISPLAY, (LPARAM)IN_LPARAM);
}
void CServer::Set_REPORT_LOSS_STATE(__in lt::Report_Loss_State_Args::Args *  IN_Data)
{
	LPARAM IN_LPARAM = (LPARAM)IN_Data;
	WM_Event_Server(WM_EVENT_SERVER_REPORT_LOSS_STATE, (LPARAM)IN_LPARAM);
}
void CServer::Set_REQUEST_LOSS_WINDOW(__in lt::Request_Loss_Window_Args::Args * IN_Data)
{
	LPARAM IN_LPARAM = (LPARAM)IN_Data;
	WM_Event_Server(WM_EVENT_SERVER_REQUEST_LOSS_WINDOW, (LPARAM)IN_LPARAM);
}
void CServer::Set_REPLY_LOSS_WINDOW(__in lt::Reply_Loss_Window_Args::Args * IN_Data)
{
	LPARAM IN_LPARAM = (LPARAM)IN_Data;
	WM_Event_Server(WM_EVENT_SERVER_REPLY_LOSS_WINDOW, (LPARAM)IN_LPARAM);
}
void CServer::Set_REPORT_ALARM_STATE(__in lt::Report_Alarm_State_Args::Args * IN_Data)
{
	LPARAM IN_LPARAM = (LPARAM)IN_Data;
	WM_Event_Server(WM_EVENT_SERVER_REPORT_ALARM_STATE, (LPARAM)IN_LPARAM);
}
void CServer::Set_REQUEST_ALARM_LIST(__in lt::Request_Alarm_List_Args::Args * IN_Data)
{
	LPARAM IN_LPARAM = (LPARAM)IN_Data;
	WM_Event_Server(WM_EVENT_SERVER_REQUEST_ALARM_LIST, (LPARAM)IN_LPARAM);
}
void CServer::Set_REPLY_ALARM_LIST(__in lt::Reply_Alarm_List_Args::Args * IN_Data)
{
	LPARAM IN_LPARAM = (LPARAM)IN_Data;
	WM_Event_Server(WM_EVENT_SERVER_REPLY_ALARM_LIST, (LPARAM)IN_LPARAM);
}
void CServer::Set_REPORT_RMS_MODE(__in lt::Report_Rms_Mode_Args::Args * IN_Data)
{
	LPARAM IN_LPARAM = (LPARAM)IN_Data;
	WM_Event_Server(WM_EVENT_SERVER_REPORT_RMS_MODE, (LPARAM)IN_LPARAM);
}
void CServer::Set_REQUEST_SET_DATETIME(__in lt::Request_Set_DateTime_Args::Args * IN_Data)
{
	LPARAM IN_LPARAM = (LPARAM)IN_Data;
	WM_Event_Server(WM_EVENT_SERVER_REQUEST_SET_DATETIME, (LPARAM)IN_LPARAM);
}
void CServer::Set_REPLY_SET_DATETIME(__in lt::Reply_Set_DateTime_Args::Args * IN_Data)
{
	LPARAM IN_LPARAM = (LPARAM)IN_Data;
	WM_Event_Server(WM_EVENT_SERVER_REPLY_SET_DATETIME, (LPARAM)IN_LPARAM);
}
void CServer::Set_REQUEST_TERMINAL_MESSAGE(__in lt::Request_Terminal_Message_Args::Args * IN_Data)
{
	LPARAM IN_LPARAM = (LPARAM)IN_Data;
	WM_Event_Server(WM_EVENT_SERVER_REQUEST_TERMINAL_MESSAGE, (LPARAM)IN_LPARAM);
}
void CServer::Set_REPLY_TERMINAL_MESSAGE(__in lt::Reply_Terminal_Message_Args::Args * IN_Data)
{
	LPARAM IN_LPARAM = (LPARAM)IN_Data;
	WM_Event_Server(WM_EVENT_SERVER_REPLY_TERMINAL_MESSAGE, (LPARAM)IN_LPARAM);
}
void CServer::Set_REQUEST_OPCALL_MESSAGE(__in lt::Request_Opcall_Args::Args * IN_Data)
{
	LPARAM IN_LPARAM = (LPARAM)IN_Data;
	WM_Event_Server(WM_EVENT_SERVER_REQUEST_OPCALL_MESSAGE, (LPARAM)IN_LPARAM);
}
void CServer::Set_REPLY_OPCALL_MESSAGE(__in lt::Reply_Opcall_Args::Args *  IN_Data)
{
	LPARAM IN_LPARAM = (LPARAM)IN_Data;
	WM_Event_Server(WM_EVENT_SERVER_REPLY_OPCALL_MESSAGE, (LPARAM)IN_LPARAM);
}
void CServer::Set_DEFINEDATA(CServer & Data) {
	CString			szTemp;
	lt::StdStringA EQUIPMENTID = lt::ToMultiByte(Data.Get_ServerId());
	Get_DEFINEDATA().Set_EQUIPMENTID(EQUIPMENTID);

	DWORD dwAddress = ntohl(Data.Get_IP_Address());
	szTemp.Format(_T("%d.%d.%d.%d"), FOURTH_IPADDRESS(dwAddress), THIRD_IPADDRESS(dwAddress), SECOND_IPADDRESS(dwAddress), FIRST_IPADDRESS(dwAddress));
	lt::StdStringA IP = lt::ToMultiByte(szTemp);
	Get_DEFINEDATA().Set_IPADDRESS(IP);
	lt::StdStringA SUBEQPID = lt::ToMultiByte(Data.Get_SubEqpID());
	Get_DEFINEDATA().Set_SUBEQPID(SUBEQPID);
}

#endif	//SOCKET
#if TEST
void CServer::Set_UINTID_READ(__in ST_xml_UNITID_READ * IN_Data)
{
	LPARAM IN_LPARAM = (LPARAM)IN_Data;
	WM_Event_Server(WM_EVENT_SERVER_UNITID_READ, (LPARAM)IN_LPARAM);
}
void CServer::Set_REQUEST_UNITID_READ(__in ST_xml_REQUEST_UNITID_READ * IN_Data)
{
	LPARAM IN_LPARAM = (LPARAM)IN_Data;
	WM_Event_Server(WM_EVENT_SERVER_REQUEST_UNITID_READ, (LPARAM)IN_LPARAM);
}
void CServer::Set_REPLY_UNITID_READ(__in ST_xml_REPLY_UNITID_READ * IN_Data)
{
	LPARAM IN_LPARAM = (LPARAM)IN_Data;
	WM_Event_Server(WM_EVENT_SERVER_REPLY_UNITID_READ, (LPARAM)IN_LPARAM);
}
void CServer::Set_REPORT_START_LOT(__in ST_xml_REPORT_START_LOT * IN_Data)
{
	LPARAM IN_LPARAM = (LPARAM)IN_Data;
	WM_Event_Server(WM_EVENT_SERVER_REPORT_START_LOT, (LPARAM)IN_LPARAM);
}
void CServer::Set_REPORT_END_EVENT(__in ST_xml_REPORT_END_EVENT * IN_Data)
{
	LPARAM IN_LPARAM = (LPARAM)IN_Data;
	WM_Event_Server(WM_EVENT_EQUIPMENT_REPORT_END_EVENT, (LPARAM)IN_LPARAM);
}
void CServer::Set_REPORT_START_PROCESS(__in ST_xml_REPORT_START_PROCESS * IN_Data)
{
	LPARAM IN_LPARAM = (LPARAM)IN_Data;
	WM_Event_Server(WM_EVENT_SERVER_REPORT_START_PROCESS, (LPARAM)IN_LPARAM);
}
void CServer::Set_REPORT_END_PROCESS(__in ST_xml_REPORT_END_PROCESS * IN_Data)
{
	LPARAM IN_LPARAM = (LPARAM)IN_Data;
	WM_Event_Server(WM_EVENT_SERVER_REPORT_END_PROCESS, (LPARAM)IN_LPARAM);
}
#endif
#if TESTUI
void CServer::Set_Notify_EquipmentState(ST_xml_REPORT_EQUIPMENT_STATE * IN_DATA)
{
	LPARAM IN_PARA = (LPARAM)IN_DATA;
	WM_Notify_Server(WM_SvrNotify_EQUIPMENTSTATE, (LPARAM)IN_PARA);
}
void CServer::Set_Notify_EquipmentStateDisplay(ST_EquipmentStateDisplay * IN_DATA)
{
	LPARAM IN_PARA2 = (LPARAM)IN_DATA;
	WM_Notify_Server(WM_SvrNotify_EQUIPMENTSTATEDISPLAY, IN_PARA2);
}
void CServer::Set_Notify_RGBDisplay(int nPORTID, LPCTSTR IN_DATA)
{
	LPARAM IN_PARA1 = (LPARAM)nPORTID;
	LPARAM IN_PARA2 = (LPARAM)IN_DATA;
	WM_Notify_Server(WM_SvrNotify_RGBDISPLAY, MAKELPARAM(IN_PARA1, IN_PARA2));
}
void CServer::Set_Notify_LOSSWINDOW(LPCTSTR IN_DATA)
{
	LPARAM IN_PARA = (LPARAM)IN_DATA;
	WM_Notify_Server(WM_SvrNotify_LOSSWINDOW, (LPARAM)IN_PARA);
}
void CServer::Set_Notify_LOSSCODE(LPCTSTR IN_DATA)
{
	LPARAM IN_PARA = (LPARAM)IN_DATA;
	WM_Notify_Server(WM_SvrNotify_LOSSCODE, (LPARAM)IN_PARA);
}
void CServer::Set_Notify_TERMINAL_MESSAGE(ST_xml_REQUEST_TERMINAL_MESSAGE* IN_DATA)
{
	LPARAM IN_PARA = (LPARAM)IN_DATA;
	WM_Notify_Server(WM_SvrNotify_TERMINAL_MESSAGE, (LPARAM)IN_PARA);
}
void CServer::Set_Notify_OPCALL(ST_xml_REQUEST_OPCALL * IN_DATA)
{
	LPARAM IN_PARA = (LPARAM)IN_DATA;
	WM_Notify_Server(WM_SvrNotify_OPCALL, (LPARAM)IN_PARA);
}
void CServer::Set_Notify_RMS(LPCTSTR  IN_DATA)
{
	LPARAM IN_PARA = (LPARAM)IN_DATA;
	WM_Notify_Server(WM_SvrNotify_RMS, (LPARAM)IN_PARA);
}
#endif


#if TEST
//=============================================================================
// Method		: Check_AvablePortCnt
// Access		: virtual protected  
// Returns		: uint8_t
// Parameter	: __in uint8_t IN_OldStatus
// Parameter	: __in uint8_t IN_NewStatus
// Qualifier	:
// Last Update	: 2022/5/18 - 12:26
// Desc.		:
//=============================================================================
uint8_t CServer::Check_AvablePortCnt(__in uint8_t IN_OldStatus, __in uint8_t IN_NewStatus)
{
	bool bCheck = false;

	if (Is_EES())
	{
		// ?¨Ìä∏ Disable, Alarm Ï≤¥ÌÅ¨
		switch (IN_OldStatus)
		{
		case enPortStatus::PtS_Disable:
		case enPortStatus::PtS_Alarm:
			switch (IN_NewStatus)
			{
			case enPortStatus::PtS_Empty:
			case enPortStatus::PtS_Exist_Socket:
			case enPortStatus::PtS_Wait_Out:
				bCheck = true;
				break;
			}
			break;

		default:
			switch (IN_NewStatus)
			{
			case enPortStatus::PtS_Disable:
			case enPortStatus::PtS_Alarm:
				bCheck = true;
				break;
			}
			break;
		}
		if (bCheck)
		{
			uint8_t nCount = 0;
			for (uint8_t nIdx = enPortIndex_Tester::PtI_T_Buffer; nIdx < m_nPortStatus.size(); ++nIdx)
			{
				switch (m_nPortStatus.at(nIdx).nStatus)
				{
				case enPortStatus::PtS_Empty:
				case enPortStatus::PtS_Exist_Socket:
				case enPortStatus::PtS_Wait_Out:
					++nCount;
					break;
				}
			}
			m_nAvablePortCnt = __min(nCount, m_nReservablePortCount);
		}
	}
	return m_nAvablePortCnt;
}

//=============================================================================
// Method		: Update_SocketLocation
// Access		: protected  
// Returns		: void
// Parameter	: __in uint8_t IN_nPortIndex
// Parameter	: __in ST_PortStatus * IN_pstPort
// Qualifier	:
// Last Update	: 2022/2/16 - 18:25
// Desc.		:
//=============================================================================
void CServer::Update_SocketLocation(__in uint8_t IN_nPortIndex, __in ST_PortStatus* IN_pstPort)
{
	// Port ?ÅÌÉúÍ∞Ä PtS_Exist_Socket?ºÎ°ú Î∞îÎÄåÎ©¥ ?åÏºì ?ïÎ≥¥Î•?Í∞±Ïã†?úÎã§.
	if (enPortStatus::PtS_Exist_Socket == IN_pstPort->nStatus)
	{
		if (m_pSocketInfo)
		{
			// IN_pstPort->szBarcode;
			if (m_pSocketInfo->Is_ExistSocket(IN_pstPort->szRfid))
			{
				m_pSocketInfo->Set_SocketLocation(IN_pstPort->szRfid, m_szServerId, IN_nPortIndex);
			}
		}
	}
}

//=============================================================================
// Method		: PortIndex2TestPara
// Access		: protected  
// Returns		: uint8_t
// Parameter	: uint8_t IN_nPortIndex
// Qualifier	:
// Last Update	: 2022/7/28 - 17:07
// Desc.		:
//=============================================================================
uint8_t CServer::PortIndex2TestPara(uint8_t IN_nPortIndex)
{
	uint8_t nPara = Para_Left;
	switch (IN_nPortIndex)
	{
	case PtI_T_Test_L:
		nPara = Para_Left;
		break;

	case PtI_T_Test_R:
		nPara = Para_Right;
		break;

	case PtI_T_Test_C:
		nPara = Para_Center;
		break;
	}

	return nPara;
}


#if TESTTERMINAL
size_t CServer::Get_TerminalCount() const
{
	return m_nTerminalLog.size();
}

const ST_TerminalLog& CServer::Get_TerminalStatus_Last() const
{
	return m_nTerminalLog.back();
}

std::vector<ST_TerminalLog>& CServer::Get_TerminalStatus()
{
	return m_nTerminalLog;
}
const ST_TerminalLog & CServer::Get_TerminalStatus(uint32_t IN_nIndex) const
{
	return m_nTerminalLog.at(IN_nIndex);
}
#endif

//=============================================================================
// Method		: EquipmentID
// Access		: virtual public  
// Returns		: 
// Parameter	: 
// Qualifier	:
// Last Update	: 2023.05.25
// Desc.		:
//=============================================================================
size_t CServer::Get_EquipmentIDCount() const
{
	return m_nEquipmentID.size();
}
std::vector<ST_EquipmentID>& CServer::Get_EquipmentIDStatus()
{
	return m_nEquipmentID;
}
const ST_EquipmentID& CServer::Get_EquipmentIDStatus(__in uint8_t IN_nPortIndex) const
{
	return m_nEquipmentID.at(IN_nPortIndex);
}

void CServer::Set_PortStatusEquipmentIDEvent(__in uint8_t IN_nPortIndex, __in ST_EquipmentID * IN_nData)
{
	if (IN_nPortIndex < m_nEquipmentID.size())
	{
		m_nEquipmentID.at(IN_nPortIndex).szEquipID = IN_nData->szEquipID;
		m_nEquipmentID.at(IN_nPortIndex).szPortID = IN_nData->szPortID;
	}
}
size_t CServer::Get_PortCount() const
{
	return m_nPortStatus.size();
}

//=============================================================================
// Method		: Set_PortClear
// Access		: virtual public  
// Returns		: void
// Parameter	: __in uint8_t IN_nPortIndex
// Qualifier	:
// Last Update	: 2022/1/13 - 19:49
// Desc.		:
//=============================================================================
void CServer::Set_PortClear(__in uint8_t IN_nPortIndex)
{
	if (IN_nPortIndex < m_nPortStatus.size())
	{
		m_nPortStatus.at(IN_nPortIndex).nStatus = 0;
		//2023.03.28
		//WM_Notify_Server(WM_SvrNotify_PortClear, (LPARAM)IN_nPortIndex);
	}
	else if (PtI_L_All == IN_nPortIndex)
	{
		// ?ÑÏ≤¥ Port Î¶¨ÏÖã
		for (auto nIdx = 0; nIdx < m_nPortStatus.size(); ++nIdx)
		{
			m_nPortStatus.at(nIdx).nStatus = 0;
			//2023.03.28
			//WM_Notify_Server(WM_SvrNotify_PortClear, (LPARAM)nIdx);
		}
	}
}

std::vector<ST_PortStatus>& CServer::Get_PortStatus()
{
	return m_nPortStatus;
}

const ST_PortStatus& CServer::Get_PortStatus(__in uint8_t IN_nPortIndex) const
{
	return m_nPortStatus.at(IN_nPortIndex);
}

//2023.04.27a uhkim
void CServer::Set_PortStatusEquipmentStateEvent(__in uint8_t IN_nPortIndex, __in uint8_t IN_nData)
{
	if (IN_nPortIndex < m_nPortStatus.size())
	{
		{
			m_nPortStatus.at(IN_nPortIndex).nEquipmentState = IN_nData;
		}
	}
	else if (PtI_T_All == IN_nPortIndex) // 99Î°?Î≥ÄÍ≤?/
	{
	}
}

void CServer::Set_PortStatus(__in uint8_t IN_nPortIndex, __in uint8_t IN_nStatus, __in LPCTSTR IN_szRFID, __in LPCTSTR IN_szBarcode, __in bool IN_bSave)
{
	if (IN_nPortIndex < m_nPortStatus.size())
	{
		// Î°úÎçî??Î≤ÑÌçº3???ÅÌÉúÍ∞Ä ?ÜÏùå->?àÏùå?ºÎ°ú Î∞îÎÄåÎ©¥ Î∞∞Ï∂ú ?πÏù∏ ?¥Î≤§??Ï≤òÎ¶¨
		//WM_Event_Equipment(WM_EVENT_EQUIPMENT_PORT_STATUS, IN_szRFID);
		if (Is_EES())
		{
			uint8_t nOld_PortStatus = m_nPortStatus.at(IN_nPortIndex).nStatus;
			m_nPortStatus.at(IN_nPortIndex).nStatus = IN_nStatus;
			m_nPortStatus.at(IN_nPortIndex).szRfid = IN_szRFID;
			m_nPortStatus.at(IN_nPortIndex).szBarcode = IN_szBarcode;

			if (IN_bSave)
			{
				Save_Equipment_Port(IN_nPortIndex);
			}
			//2023.03.28
			//WM_Notify_Server(WM_SvrNotify_PortStatus, MAKELPARAM(IN_nPortIndex, IN_nStatus));

			// Î°úÎçî??Î≤ÑÌçº3???ÅÌÉúÍ∞Ä ?ÜÏùå->?àÏùå?ºÎ°ú Î∞îÎÄåÎ©¥ Î∞∞Ï∂ú ?πÏù∏ ?¥Î≤§??Ï≤òÎ¶¨
			if (PtI_L_Buffer_3 == IN_nPortIndex)
			{
				if ((enPortStatus::PtS_Empty == nOld_PortStatus) &&
					(enPortStatus::PtS_Exist_Socket == IN_nStatus))
				{
					// RFID ?ïÎ≥¥Í∞Ä ?ÜÏúºÎ©??§Î•ò
					WM_Event_Server(WM_EVENT_LOADER_CHEKCK_TRACKOUT, IN_szRFID);
				}
			}
		}
		else // Returner
		{
			m_nPortStatus.at(IN_nPortIndex).nStatus = IN_nStatus;
			m_nPortStatus.at(IN_nPortIndex).szRfid = IN_szRFID;
			m_nPortStatus.at(IN_nPortIndex).szBarcode = IN_szBarcode;

			if (IN_bSave)
			{
				Save_Equipment_Port(IN_nPortIndex);
			}
			//2023.03.28
			//WM_Notify_Server(WM_SvrNotify_PortStatus, MAKELPARAM(IN_nPortIndex, IN_nStatus));
		}

		// ?åÏºì ?ïÎ≥¥ Í∞±Ïã† (RFID?ïÎ≥¥Í∞Ä ?ÜÏúºÎ©? ?§Î•ò)
		Update_SocketLocation(IN_nPortIndex, &m_nPortStatus.at(IN_nPortIndex));
	}
	//else if (IN_nPortIndex == m_nPortStatus.size()) // 99Î°?Î≥ÄÍ≤?
	else if (PtI_T_All == IN_nPortIndex) // 99Î°?Î≥ÄÍ≤?/
	{
	}
}


std::vector<ST_PortStatus>&	 CServer::Get_OldPortStatus()
{
	return m_nOldPortStatus;
}
const ST_PortStatus& CServer::Get_OldPortStatus(__in uint8_t IN_nStatus) const
{
	return m_nOldPortStatus.at(IN_nStatus);
}
void CServer::Set_OldPortStatusEquipmentStateEvent(__in uint8_t IN_nPortIndex, __in uint8_t IN_nData)
{
	if (IN_nPortIndex < m_nOldPortStatus.size())
	{
		{
			m_nOldPortStatus.at(IN_nPortIndex).nEquipmentState = IN_nData;
		}
	}
	else if (PtI_T_All == IN_nPortIndex) // 99Î°?Î≥ÄÍ≤?/
	{
	}
}


size_t CServer::Get_ConveyorCount() const
{
	return m_nConveyorStatus.size();
}

std::vector<ST_ConveyorStatus>& CServer::Get_ConveyorStatus()
{
	return m_nConveyorStatus;
}

//=============================================================================
// Method		: Get_ConveyorStatus
// Access		: public  
// Returns		: const ST_ConveyorStatus&
// Parameter	: __in uint8_t IN_nConveyorIndex
// Qualifier	: const
// Last Update	: 2022/1/27 - 12:00
// Desc.		:
//=============================================================================
const ST_ConveyorStatus& CServer::Get_ConveyorStatus(__in uint8_t IN_nConveyorIndex) const
{
	return m_nConveyorStatus.at(IN_nConveyorIndex);
}

void CServer::Set_ConveyorStatus(__in uint8_t IN_nConveyorIndex, __in uint8_t IN_nStatus, __in uint8_t IN_nExistSocket, __in LPCTSTR IN_szRFID, __in LPCTSTR IN_szBarcode, __in bool IN_bSave)
{
	if (IN_nConveyorIndex < m_nConveyorStatus.size())
	{
		if (Is_EES())
		{
			m_nConveyorStatus.at(IN_nConveyorIndex).nStatus = IN_nStatus;
			m_nConveyorStatus.at(IN_nConveyorIndex).nExistSocket = IN_nExistSocket;
			m_nConveyorStatus.at(IN_nConveyorIndex).szRfid = IN_szRFID;
			m_nConveyorStatus.at(IN_nConveyorIndex).szBarcode = IN_szBarcode;

			if (IN_bSave)
			{
				Save_Equipment_Conveyor(IN_nConveyorIndex);
			}
			//2023.03.28
			//WM_Notify_Server(WM_SvrNotify_ConveyorStatus, MAKELPARAM(IN_nConveyorIndex, MAKEWORD(IN_nStatus, IN_nExistSocket)));
		}
		else // Returner
		{
			m_nConveyorStatus.at(IN_nConveyorIndex).nStatus = IN_nStatus;
			m_nConveyorStatus.at(IN_nConveyorIndex).nExistSocket = IN_nExistSocket;
			m_nConveyorStatus.at(IN_nConveyorIndex).szRfid = IN_szRFID;
			m_nConveyorStatus.at(IN_nConveyorIndex).szBarcode = IN_szBarcode;

			if (IN_bSave)
			{
				Save_Equipment_Conveyor(IN_nConveyorIndex);
			}
			//2023.03.28
			//WM_Notify_Server(WM_SvrNotify_ConveyorStatus, MAKELPARAM(IN_nConveyorIndex, MAKEWORD(IN_nStatus, IN_nExistSocket)));
		}
	}
}
//=============================================================================
// Method		: SubEqpID
// Access		: virtual public  
// Returns		: 
// Parameter	: 
// Qualifier	:
// Last Update	: 2023.05.25
// Desc.		:
//=============================================================================
CString CServer::Get_SubEqpID() const
{
	return m_nSubEqpID;
}

void CServer::Set_SubEqpID(__in LPCTSTR IN_DATA)
{
	m_nSubEqpID = IN_DATA;
}

//=============================================================================
// Method		: EquipmentStatus
// Access		: public  
// Returns		: uint8_t
// Qualifier	: const
// Last Update	: 2022/1/13 - 19:50
// Desc.		:
//=============================================================================
uint8_t CServer::Get_EquipmentStatus() const
{
	return m_nEquipmentStatus;
}

void CServer::Set_EquipmentStatus(__in uint8_t IN_nStatus)
{
	m_nEquipmentStatus = IN_nStatus;
	//Set_SValues(SV_EQUIPMENTSTATE, g_szEquipment_State[IN_nStatus]);
}
uint8_t CServer::Get_OldEquipmentStatus() const
{
	return m_nOldEquipmentStatus;
}
void CServer::Set_OldEquipmentStatus(__in uint8_t IN_nStatus)
{
	m_nOldEquipmentStatus = IN_nStatus;
}


//=============================================================================
// Method		: Get_AlarmCount
// Access		: public  
// Returns		: size_t
// Qualifier	: const
// Last Update	: 2022/2/16 - 17:40
// Desc.		:
//=============================================================================
size_t CServer::Get_AlarmCount() const
{
	return m_nAlarmStatus.size();
}

const ST_AlarmStatus& CServer::Get_AlarmStatus_Last() const
{
	return m_nAlarmStatus.back();
}

std::vector<ST_AlarmStatus>& CServer::Get_AlarmStatus()
{
	return m_nAlarmStatus;
}

const ST_AlarmStatus & CServer::Get_AlarmStatus(uint32_t IN_nIndex) const
{
	return m_nAlarmStatus.at(IN_nIndex);
}
//=============================================================================
// Method		: LossWindow
// Access		: public  
// Returns		: size_t
// Qualifier	: const
// Last Update	: 2022/2/16 - 17:40
// Desc.		:
//=============================================================================
size_t CServer::Get_LossWindowCount() const
{
	return m_nLossWindow.size();
}
std::vector<ST_LossWindow>& CServer::Get_LossWindow()
{
	return m_nLossWindow;
}
const ST_LossWindow & CServer::Get_LossWindow(uint32_t IN_nIndex) const
{
	return m_nLossWindow.at(IN_nIndex);
}

#endif	//TEST



/*
//=============================================================================
// Method		: Get_Yield_Day
// Access		: public  
// Returns		: const CYield_Equipment&
// Qualifier	: const
// Last Update	: 2022/1/27 - 12:01
// Desc.		:
//=============================================================================
const CYield_Equipment& CServer::Get_Yield_Day() const
{
	return m_Yield_Day;
}

const CYield_Equipment& CServer::Get_Yield_Cumulative() const
{
	return m_Yield_Cumulative;
}

void CServer::Set_Yield_Day(CYield_Equipment * IN_pYield)
{
	m_Yield_Day = *IN_pYield;

	// GUI???úÏãú //2023.03.28
	//WM_Notify_Server(WM_SvrNotify_Yield, (LPARAM)&m_Yield_Day);
}

void CServer::Set_Yield_Cumulative(CYield_Equipment * IN_pYield)
{
	m_Yield_Cumulative = *IN_pYield;

	// GUI???úÏãú

}

void CServer::Reset_Yield_Day()
{
	m_Yield_Day.Reset();
#ifdef USE_EQP_TACTTIME
	m_Tacttime.Reset();
	Save_Equipment_Tacttime();
#endif

	Save_Equipment_Yield();
	//2023.03.28
	//WM_Notify_Server(WM_SvrNotify_Yield, (LPARAM)&m_Yield_Day);
}

void CServer::Reset_Yield_Cumulative()
{
	m_Yield_Cumulative.Reset();


}

void CServer::Increase_Yield_Pass(__in uint8_t IN_nPara)
{
	// ?úÌîÑ??Ï≤¥ÌÅ¨
	// ?§Ï†Ñ 8???¥Ï†Ñ -> ?§Ï†Ñ 8???¥ÌõÑ?¥Î©¥ ?òÏú® Ï¥àÍ∏∞??

	m_Yield_Day.IncreasePass(IN_nPara);
	m_Yield_Cumulative.IncreasePass(IN_nPara);

	Save_Equipment_Yield();
	//2023.03.28
	//WM_Notify_Server(WM_SvrNotify_Yield, (LPARAM)&m_Yield_Day);
}

void CServer::Increase_Yield_Fail(__in uint8_t IN_nPara)
{
	// ?úÌîÑ??Ï≤¥ÌÅ¨
	// ?§Ï†Ñ 8???¥Ï†Ñ -> ?§Ï†Ñ 8???¥ÌõÑ?¥Î©¥ ?òÏú® Ï¥àÍ∏∞??

	m_Yield_Day.IncreaseFail(IN_nPara);
	m_Yield_Cumulative.IncreaseFail(IN_nPara);

	Save_Equipment_Yield();
	//2023.03.28
	//WM_Notify_Server(WM_SvrNotify_Yield, (LPARAM)&m_Yield_Day);
}*/


