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
#include "Equipment.h"
#include "Def_Equipment_Type.h"
#include "CommonFunction.h"
#include "RegEquipment.h"

CEquipment::CEquipment()
{
	m_pRegEqp = new CRegEquipment();
}

CEquipment::~CEquipment()
{
	delete m_pRegEqp;
}

CEquipment& CEquipment::operator=(const CConfig_Eqp& ref)
{
	CConfig_Eqp::operator=(ref);

	return *this;
}

CEquipment& CEquipment::operator=(const CEquipment& ref)
{
	CConfig_Eqp::operator=(ref);

	m_bEnable			= ref.m_bEnable;
	m_bSkip				= ref.m_bSkip;
	m_nConnection		= ref.m_nConnection;
	m_bTimeSync			= ref.m_bTimeSync;
	m_bVerifyID			= ref.m_bVerifyID;
	m_nOperMode			= ref.m_nOperMode;
	m_nProcessStatus	= ref.m_nProcessStatus;
	m_bLED_Status		= ref.m_bLED_Status;
	m_nLanguage			= ref.m_nLanguage;
	m_nModelType		= ref.m_nModelType;

	m_nReservedPortCnt	= ref.m_nReservedPortCnt;

	//m_nAlarmCode		= ref.m_nAlarmCode;
	//m_szAlarmInfo		= ref.m_szAlarmInfo;

	m_nPortStatus.clear();
	m_nPortStatus		= ref.m_nPortStatus;
	m_nConveyorStatus.clear();
	m_nConveyorStatus	= ref.m_nConveyorStatus;
	m_nAlarmStatus.clear();
	m_nAlarmStatus		= ref.m_nAlarmStatus;
	
	m_Yield_Day			= ref.m_Yield_Day;
	m_Yield_Cumulative	= ref.m_Yield_Cumulative;

	memcpy(&m_tm_CheckShift, &ref.m_tm_CheckShift, sizeof(SYSTEMTIME));

	m_pSocketInfo		= ref.m_pSocketInfo;

	return *this;
}

#if defined(EES_XML)//20231003
CEquipment& CEquipment::operator=(const CCommonModule& ref)
{
	CCommonModule::operator=(ref);
	return *this;
}
#endif

void CEquipment::Init_EquipmentType_UI()
{
	__super::Init_EquipmentType_UI();
}

// void CEquipment::Init_EquipmentType()
// {
// 	__super::Init_EquipmentType();
// }

//=============================================================================
// Method		: WM_Notify_Equipment
// Access		: protected  
// Returns		: void
// Parameter	: __in WM_Eqp_Nofity_Para IN_wParam
// Parameter	: __in LPARAM IN_lParam
// Qualifier	:
// Last Update	: 2022/1/27 - 10:09
// Desc.		:
//=============================================================================
void CEquipment::WM_Notify_Equipment(__in WM_Eqp_Nofity_Para IN_wParam, __in LPARAM IN_lParam)
{
	if (m_hWndGUI)
	{
		TRACE(_T("WM_Notify_Equipment() => WM: %d \n"), IN_wParam);
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
void CEquipment::WM_Event_Equipment(__in UINT IN_nWM_Event, __in LPCTSTR IN_szRFID /*= nullptr*/)
{
	if (m_hWndOwner)
	{
		if ((nullptr != IN_szRFID) && (0 != _tcsclen(IN_szRFID)) && (m_pSocketInfo->Is_ExistSocket(IN_szRFID)))
		{
			::SendNotifyMessage(m_hWndOwner, IN_nWM_Event, (WPARAM)m_nEqpOrder, (LPARAM)m_pSocketInfo->GetAt(IN_szRFID).szRFID.GetBuffer());

			// 쓰레드로 처리하자.
			//::SendMessage(m_hWndOwner, IN_nWM_Event, (WPARAM)m_nEqpOrder, (LPARAM)m_pSocketInfo->GetAt(IN_szRFID).szRFID.GetBuffer());
		}
		else
		{
			::SendNotifyMessage(m_hWndOwner, IN_nWM_Event, (WPARAM)m_nEqpOrder, (LPARAM)0);
		}
	}
}

void CEquipment::WM_Event_Equipment(__in UINT IN_nWM_Event, __in LPARAM IN_lParam)
{
	if (m_hWndOwner)
	{
		TRACE(_T("WM_Event_Equipment() => event: %d \n"), IN_nWM_Event);
		::SendNotifyMessage(m_hWndOwner, IN_nWM_Event, (WPARAM)m_nEqpOrder, IN_lParam);
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
bool CEquipment::Is_ChangeShift(__in const SYSTEMTIME* IN_ptmCurrent)
{
	bool bReturn = false;
// 	SYSTEMTIME tmLocal;
// 	GetLocalTime(&tmLocal);

	// 오전 8시 SHIFT 변경 됨.
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

		// 하루 이상 차이가 나는가??
		if (86400.0f < DiffTime)
		{
			bReturn = true;
		}
	}

	// 체크시간 변경
	//memcpy(&m_tm_CheckShift, &tmLocal, sizeof(SYSTEMTIME));

	return bReturn;
}

bool CEquipment::Is_ChangeShift()
{
	SYSTEMTIME tmLocal;
	GetLocalTime(&tmLocal);

	bool bReturn = Is_ChangeShift(&tmLocal);

	Set_CheckShiftTime(&tmLocal);
#ifdef USE_EQP_TACTTIME
	m_Tacttime.Set_UnloadTime(&m_tm_CheckShift);
	Save_Equipment_Tacttime();
#endif

	return bReturn;
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
void CEquipment::Update_SocketLocation(__in uint8_t IN_nPortIndex, __in ST_PortStatus* IN_pstPort)
{
	// Port 상태가 PtS_Exist_Socket으로 바뀌면 소켓 정보를 갱신한다.
	if (enPortStatus::PtS_Exist_Socket == IN_pstPort->nStatus)
	{
		if (m_pSocketInfo)
		{
			// IN_pstPort->szBarcode;
			if (m_pSocketInfo->Is_ExistSocket(IN_pstPort->szRfid))
			{
				m_pSocketInfo->Set_SocketLocation(IN_pstPort->szRfid, m_szEquipmentId, IN_nPortIndex);
			}
		}
	}
}

void CEquipment::Save_Equipment_Skip()
{
	m_pRegEqp->Set_Equipment_Skip(this);
}

void CEquipment::Save_Equipment_Reserve()
{
	m_pRegEqp->Set_Equipment_Reserve(this);
}

void CEquipment::Save_Equipment_ReserveQueue()
{
	m_pRegEqp->Set_Equipment_ReserveQueue(this);
}

void CEquipment::Save_Equipment_EndProduction()
{
	m_pRegEqp->Set_Equipment_EndProduction(this);
}

void CEquipment::Save_Equipment_Shift()
{
	m_pRegEqp->Set_Equipment_Shift(this);
}

void CEquipment::Save_Equipment_Port(uint8_t IN_nPortIndex)
{
	m_pRegEqp->Set_Equipment_Port(this, IN_nPortIndex);
}

void CEquipment::Save_Equipment_Conveyor(uint8_t IN_ConveyorIndex)
{
	m_pRegEqp->Set_Equipment_Conveyor(this, IN_ConveyorIndex);
}

void CEquipment::Save_Equipment_Alarm()
{
	m_pRegEqp->Set_Equipment_Alarm(this);
}

void CEquipment::Save_Equipment_Yield()
{
	m_pRegEqp->Set_Equipment_Yield(this);
}

void CEquipment::Save_Equipment_Tacttime()
{
#ifdef USE_EQP_TACTTIME
	//CString szRegPath;
	//szRegPath.Format(_T("%s\\%s"), m_pRegEqp->Get_RegistryPath().GetBuffer(), Get_EquipmentId());

	//m_Tacttime.Save_Tacttime(szRegPath);
#endif
}

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
uint8_t CEquipment::Check_AvablePortCnt(__in uint8_t IN_OldStatus, __in uint8_t IN_NewStatus)
{
	bool bCheck = false;

	// Tester만 사용
	if (Is_Tester())
	{
		// 포트 Disable, Alarm 체크
		switch (IN_OldStatus)
		{
		case enPortStatus::PtS_Disable:
		case enPortStatus::PtS_Alarm:
			switch (IN_NewStatus)
			{
			case enPortStatus::PtS_Empty:
			case enPortStatus::PtS_Exist_Socket:
			case enPortStatus::PtS_Wait_Out:
				// 포트 사용 불가 -> 포트 사용 가능 상태로 변경됨
				bCheck = true;
				break;
			}
			break;

		default:
			switch (IN_NewStatus)
			{
			case enPortStatus::PtS_Disable:
			case enPortStatus::PtS_Alarm:
				// 포트 사용 가능 -> 포트 사용 불가 상태로 변경됨
				bCheck = true;
				break;
			}
			break;
		}

		if (bCheck)
		{
			uint8_t nCount = 0;
			//for (uint8_t nIdx = enPortIndex_Tester::PtI_T_Buffer; nIdx <= enPortIndex_Tester::PtI_T_Test_R; ++nIdx)
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

			//m_nAvablePortCnt = __min(nCount, MAX_RESERVE_COUNT_TESTER);
			m_nAvablePortCnt = __min(nCount, m_nReservablePortCount);
		}
	}


	return m_nAvablePortCnt;
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
uint8_t CEquipment::PortIndex2TestPara(uint8_t IN_nPortIndex)
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

uint8_t CEquipment::TestPara2PortIndex(__in uint8_t IN_nTestPara)
{
	uint8_t OUT_nPortIndex = PtI_T_Test_L;
	switch (IN_nTestPara)
	{
	case Para_Left:
		OUT_nPortIndex = PtI_T_Test_L;
		break;

	case Para_Right:
		OUT_nPortIndex = PtI_T_Test_R;
		break;

	case Para_Center:
		OUT_nPortIndex = PtI_T_Test_C;
		break;
	}

	return OUT_nPortIndex;
}

//=============================================================================
// Method		: Set_OwnerHWND
// Access		: public  
// Returns		: void
// Parameter	: __in HWND IN_hWnd
// Qualifier	:
// Last Update	: 2022/1/13 - 19:49
// Desc.		:
//=============================================================================
void CEquipment::Set_OwnerHWND(__in HWND IN_hWnd)
{
	m_hWndOwner = IN_hWnd;
}

void CEquipment::Set_GUI_HWND(__in HWND IN_hWnd)
{
	m_hWndGUI = IN_hWnd;
}

void CEquipment::Set_WinMsg_Notify(__in UINT IN_nWM_Notify)
{
	m_nWM_Notify = IN_nWM_Notify;
}

//=============================================================================
// Method		: SetPtr_SocketInfo
// Access		: public  
// Returns		: void
// Parameter	: __in CSocketMonitoring * IN_pSocketInfo
// Qualifier	:
// Last Update	: 2022/1/25 - 20:16
// Desc.		:
//=============================================================================
void CEquipment::SetPtr_SocketInfo(__in CSocketMonitoring* IN_pSocketInfo)
{
	m_pSocketInfo = IN_pSocketInfo;
}

//=============================================================================
// Method		: Is_Teseter
// Access		: virtual public  
// Returns		: bool
// Qualifier	: const
// Last Update	: 2022/1/13 - 19:49
// Desc.		:
//=============================================================================
bool CEquipment::Is_Tester() const
{
	//return ((enEquipmentType::Eqp_Loader != m_nEquipmentType) && (enEquipmentType::Eqp_Returner != m_nEquipmentType)) ? true : false;
	//return IsTester_byEqpType(m_nEquipmentType);
	return IsTester_byEqpType(Get_EquipmentType());
}

bool CEquipment::Is_Loader() const
{
	//return (enEquipmentType::Eqp_Loader == m_nEquipmentType) ? true : false;
	return (enEquipmentType::Eqp_Loader == Get_EquipmentType()) ? true : false;
}

bool CEquipment::Is_Returner() const
{
	//return (enEquipmentType::Eqp_Returner == m_nEquipmentType) ? true : false;
	return (enEquipmentType::Eqp_Returner == Get_EquipmentType()) ? true : false;
}

bool CEquipment::Is_Handler() const
{
	//return (enEquipmentType::Eqp_Handler == m_nEquipmentType) ? true : false;
	return (enEquipmentType::Eqp_Handler == Get_EquipmentType()) ? true : false;
}

//=============================================================================
// Method		: Get_EqpOrder
// Access		: public  
// Returns		: uint8_t
// Qualifier	:
// Last Update	: 2022/1/24 - 18:17
// Desc.		:
//=============================================================================
uint8_t CEquipment::Get_EqpOrder()
{
	return m_nEqpOrder;
}

const uint8_t CEquipment::Get_EqpOrder() const
{
	return m_nEqpOrder;
}

void CEquipment::Set_EqpOrder(__in uint8_t IN_nEqpOrder)
{
	m_nEqpOrder = IN_nEqpOrder;
}

size_t CEquipment::Get_PortCount() const
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
void CEquipment::Set_PortClear(__in uint8_t IN_nPortIndex)
{
	if (IN_nPortIndex < m_nPortStatus.size())
	{
		m_nPortStatus.at(IN_nPortIndex).nStatus = 0;

		WM_Notify_Equipment(WM_EqpNotify_PortClear, (LPARAM)IN_nPortIndex);
	}
	else if (PtI_L_All == IN_nPortIndex)
	{
		// 전체 Port 리셋
		for (auto nIdx = 0; nIdx < m_nPortStatus.size(); ++nIdx)
		{
			m_nPortStatus.at(nIdx).nStatus = 0;

			WM_Notify_Equipment(WM_EqpNotify_PortClear, (LPARAM)nIdx);
		}
	}
}

std::vector<ST_PortStatus>& CEquipment::Get_PortStatus()
{
	return m_nPortStatus;
}

const ST_PortStatus& CEquipment::Get_PortStatus(__in uint8_t IN_nPortIndex) const
{
	return m_nPortStatus.at(IN_nPortIndex);
}

const ST_PortStatus& CEquipment::Get_PortStatus_byTestPara(__in uint8_t IN_nTestPara)
{
	return m_nPortStatus.at(TestPara2PortIndex(IN_nTestPara));
}

void CEquipment::Set_PortStatus(__in uint8_t IN_nPortIndex, __in uint8_t IN_nStatus, __in LPCTSTR IN_szRFID, __in LPCTSTR IN_szBarcode, __in bool IN_bSave /*= true*/)
{
#if defined(EES_XML)//20231003
	if (IN_nPortIndex < Get_mEES_PortSubStatusCount()){
		//Get_mEES_PortSubStatus(IN_nPortIndex).Set_nOldPortStatus(Get_mEES_PortSubStatus(IN_nPortIndex).Get_nPortStatus());
		Get_mEES_PortSubStatus(IN_nPortIndex).Set_nPortStatus(IN_nStatus);
		Get_mEES_PortSubStatus(IN_nPortIndex).Set_szRfid(IN_szRFID);
		Get_mEES_PortSubStatus(IN_nPortIndex).Set_szBarcode(IN_szBarcode);
		WM_Event_Equipment(WM_EVENT_EQUIPMENT_REPORT_EQUIPMENT_STATE, (LPARAM)NULL);

	}
#endif
	if (IN_nPortIndex < m_nPortStatus.size())
	{
		// 로더의 버퍼3의 상태가 없음->있음으로 바뀌면 배출 승인 이벤트 처리
		//WM_Event_Equipment(WM_EVENT_EQUIPMENT_PORT_STATUS, IN_szRFID);
		if (Is_Loader())
		{
			uint8_t nOld_PortStatus = m_nPortStatus.at(IN_nPortIndex).nStatus;
			m_nPortStatus.at(IN_nPortIndex).nStatus		= IN_nStatus;
			m_nPortStatus.at(IN_nPortIndex).szRfid		= IN_szRFID;
			m_nPortStatus.at(IN_nPortIndex).szBarcode	= IN_szBarcode;

			if (IN_bSave)
			{
				Save_Equipment_Port(IN_nPortIndex);
			}

			WM_Notify_Equipment(WM_EqpNotify_PortStatus, MAKELPARAM(IN_nPortIndex, IN_nStatus));

			// 로더의 버퍼3의 상태가 없음->있음으로 바뀌면 배출 승인 이벤트 처리
			if (PtI_L_Buffer_3 == IN_nPortIndex)
			{
				if ((enPortStatus::PtS_Empty == nOld_PortStatus) &&
					(enPortStatus::PtS_Exist_Socket == IN_nStatus))
				{
					// RFID 정보가 없으면 오류
					WM_Event_Equipment(WM_EVENT_LOADER_CHEKCK_TRACKOUT, IN_szRFID);
				}
			}
		}
		else if (Is_Tester())
		{
			m_nPortStatus.at(IN_nPortIndex).nStatus		= IN_nStatus;
			m_nPortStatus.at(IN_nPortIndex).szRfid		= IN_szRFID;
			m_nPortStatus.at(IN_nPortIndex).szBarcode	= IN_szBarcode;

			if (IN_bSave)
			{
				Save_Equipment_Port(IN_nPortIndex);
			}

			WM_Notify_Equipment(WM_EqpNotify_PortStatus, MAKELPARAM(IN_nPortIndex, IN_nStatus));
			WM_Event_Equipment(WM_EVENT_EQUIPMENT_PORT_STATUS, (LPARAM)IN_nPortIndex);

			// Tester의 L/R 파라에 소켓이 투입되면 시간 체크한다.
			if ((PtS_Exist_Socket == IN_nStatus) && 
				((PtI_T_Test_L == IN_nPortIndex) || (PtI_T_Test_R == IN_nPortIndex) || (PtI_T_Test_C == IN_nPortIndex)))
			{
				//uint8_t nPara = (PtI_T_Test_L == IN_nPortIndex) ? Para_Left : Para_Right;
				uint8_t nPara = PortIndex2TestPara(IN_nPortIndex);

				Set_Time_InputPara(nPara);
			}
		}
		else // Returner
		{
			m_nPortStatus.at(IN_nPortIndex).nStatus		= IN_nStatus;
			m_nPortStatus.at(IN_nPortIndex).szRfid		= IN_szRFID;
			m_nPortStatus.at(IN_nPortIndex).szBarcode	= IN_szBarcode;

			if (IN_bSave)
			{
				Save_Equipment_Port(IN_nPortIndex);
			}

			WM_Notify_Equipment(WM_EqpNotify_PortStatus, MAKELPARAM(IN_nPortIndex, IN_nStatus));
		}

		// 소켓 정보 갱신 (RFID정보가 없으면, 오류)
		Update_SocketLocation(IN_nPortIndex, &m_nPortStatus.at(IN_nPortIndex));
	}
	//else if (IN_nPortIndex == m_nPortStatus.size()) // 99로 변경.
	else if (PtI_T_All == IN_nPortIndex) // 99로 변경.
	{
		// 전체 Port 설정

		// 

	}
}

size_t CEquipment::Get_ConveyorCount() const
{
	return m_nConveyorStatus.size();
}

std::vector<ST_ConveyorStatus>& CEquipment::Get_ConveyorStatus()
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
const ST_ConveyorStatus& CEquipment::Get_ConveyorStatus(__in uint8_t IN_nConveyorIndex) const
{
	return m_nConveyorStatus.at(IN_nConveyorIndex);
}

void CEquipment::Set_ConveyorStatus(__in uint8_t IN_nConveyorIndex, __in uint8_t IN_nStatus, __in uint8_t IN_nExistSocket, __in LPCTSTR IN_szRFID, __in LPCTSTR IN_szBarcode, __in bool IN_bSave /*= true*/)
{
	if (IN_nConveyorIndex < m_nConveyorStatus.size())
	{
		if (Is_Loader())
		{
			m_nConveyorStatus.at(IN_nConveyorIndex).nStatus			= IN_nStatus;
			m_nConveyorStatus.at(IN_nConveyorIndex).nExistSocket	= IN_nExistSocket;
			m_nConveyorStatus.at(IN_nConveyorIndex).szRfid			= IN_szRFID;
			m_nConveyorStatus.at(IN_nConveyorIndex).szBarcode		= IN_szBarcode;

			if (IN_bSave)
			{
				Save_Equipment_Conveyor(IN_nConveyorIndex);
			}

			WM_Notify_Equipment(WM_EqpNotify_ConveyorStatus, MAKELPARAM(IN_nConveyorIndex, MAKEWORD(IN_nStatus, IN_nExistSocket)));
		}
		else if (Is_Tester())
		{
			m_nConveyorStatus.at(IN_nConveyorIndex).nStatus			= IN_nStatus;
			m_nConveyorStatus.at(IN_nConveyorIndex).nExistSocket	= IN_nExistSocket;
			m_nConveyorStatus.at(IN_nConveyorIndex).szRfid			= IN_szRFID;
			m_nConveyorStatus.at(IN_nConveyorIndex).szBarcode		= IN_szBarcode;

			if (IN_bSave)
			{
				Save_Equipment_Conveyor(IN_nConveyorIndex);
			}

			WM_Notify_Equipment(WM_EqpNotify_ConveyorStatus, MAKELPARAM(IN_nConveyorIndex, MAKEWORD(IN_nStatus, IN_nExistSocket)));
			WM_Event_Equipment(WM_EVENT_EQUIPMENT_CONVEYOR_STATUS, (LPARAM)IN_nConveyorIndex);
		}
		else // Returner
		{
			m_nConveyorStatus.at(IN_nConveyorIndex).nStatus			= IN_nStatus;
			m_nConveyorStatus.at(IN_nConveyorIndex).nExistSocket	= IN_nExistSocket;
			m_nConveyorStatus.at(IN_nConveyorIndex).szRfid			= IN_szRFID;
			m_nConveyorStatus.at(IN_nConveyorIndex).szBarcode		= IN_szBarcode;

			if (IN_bSave)
			{
				Save_Equipment_Conveyor(IN_nConveyorIndex);
			}

			WM_Notify_Equipment(WM_EqpNotify_ConveyorStatus, MAKELPARAM(IN_nConveyorIndex, MAKEWORD(IN_nStatus, IN_nExistSocket)));
		}
	}
}

//=============================================================================
// Method		: Get_Yield_Day
// Access		: public  
// Returns		: const CYield_Equipment&
// Qualifier	: const
// Last Update	: 2022/1/27 - 12:01
// Desc.		:
//=============================================================================
const CYield_Equipment& CEquipment::Get_Yield_Day() const
{
	return m_Yield_Day;
}

const CYield_Equipment& CEquipment::Get_Yield_Cumulative() const
{
	return m_Yield_Cumulative;
}

void CEquipment::Set_Yield_Day(CYield_Equipment * IN_pYield)
{
	m_Yield_Day = *IN_pYield;

	// GUI에 표시
	WM_Notify_Equipment(WM_EqpNotify_Yield, (LPARAM)&m_Yield_Day);
}

void CEquipment::Set_Yield_Cumulative(CYield_Equipment * IN_pYield)
{
	m_Yield_Cumulative = *IN_pYield;

	// GUI에 표시

}

void CEquipment::Reset_Yield_Day()
{
	m_Yield_Day.Reset();
#ifdef USE_EQP_TACTTIME
	m_Tacttime.Reset();
	Save_Equipment_Tacttime();
#endif

	Save_Equipment_Yield();

	WM_Notify_Equipment(WM_EqpNotify_Yield, (LPARAM)&m_Yield_Day);
}

void CEquipment::Reset_Yield_Cumulative()
{
	m_Yield_Cumulative.Reset();


}

void CEquipment::Increase_Yield_Pass(__in uint8_t IN_nPara)
{
	// 시프트 체크
	// 오전 8시 이전 -> 오전 8시 이후이면 수율 초기화

	m_Yield_Day.IncreasePass(IN_nPara);
	m_Yield_Cumulative.IncreasePass(IN_nPara);

	Save_Equipment_Yield();

	WM_Notify_Equipment(WM_EqpNotify_Yield, (LPARAM)&m_Yield_Day);
}

void CEquipment::Increase_Yield_Fail(__in uint8_t IN_nPara)
{
	// 시프트 체크
	// 오전 8시 이전 -> 오전 8시 이후이면 수율 초기화

	m_Yield_Day.IncreaseFail(IN_nPara);
	m_Yield_Cumulative.IncreaseFail(IN_nPara);

	Save_Equipment_Yield();

	WM_Notify_Equipment(WM_EqpNotify_Yield, (LPARAM)&m_Yield_Day);
}

//=============================================================================
// Method		: Get_EnableEquipment
// Access		: public  
// Returns		: bool
// Qualifier	: const
// Last Update	: 2022/1/13 - 19:50
// Desc.		:
//=============================================================================
bool CEquipment::Get_EnableEquipment() const
{
	return m_bEnable;
}

void CEquipment::Set_EnableEquipment(__in bool IN_bEnable)
{
	m_bEnable = IN_bEnable;
}

//=============================================================================
// Method		: Get_ClientConnection
// Access		: public  
// Returns		: uint8_t
// Qualifier	: const
// Last Update	: 2022/1/13 - 19:50
// Desc.		:
//=============================================================================
uint8_t CEquipment::Get_ClientConnection() const
{
	return m_nConnection;
}

void CEquipment::Set_ClientConnection(__in uint8_t IN_nConStatus)
{
	m_nConnection = IN_nConStatus;

	WM_Notify_Equipment(WM_EqpNotify_ClientConnection, (LPARAM)m_nConnection);
	WM_Event_Equipment(WM_EVENT_EQUIPMENT_CONNECTION, (LPARAM)m_nConnection);
#if defined(EES_XML) //20231003
	WM_Event_Equipment(WM_EVENT_EQUIPMENT_REPORT_EQUIPMENT_STATE, (LPARAM)NULL); 
	if (!m_nConnection) {
	}
#endif
}

//=============================================================================
// Method		: Get_VerifyEqpConnection
// Access		: public  
// Returns		: bool
// Qualifier	: const
// Last Update	: 2022/1/13 - 19:52
// Desc.		:
//=============================================================================
bool CEquipment::Get_VerifyEqpConnection() const
{
	return m_bVerifyID;
}

void CEquipment::Set_VerifyEqpConnection(__in bool bVerified)
{
	m_bVerifyID = bVerified;

	WM_Notify_Equipment(WM_EqpNotify_VerifyEqpConnection, (LPARAM)m_bVerifyID);
}

//=============================================================================
// Method		: Get_OperatingMode
// Access		: public  
// Returns		: uint8_t
// Qualifier	: const
// Last Update	: 2022/1/13 - 19:50
// Desc.		:
//=============================================================================
uint8_t CEquipment::Get_OperatingMode() const
{
	return m_nOperMode;
}

void CEquipment::Set_OperatingMode(__in uint8_t IN_nOperMode)
{
	uint8_t nOld_OperMode = m_nOperMode;

	m_nOperMode = IN_nOperMode;

	WM_Notify_Equipment(WM_EqpNotify_OperatingMode, (LPARAM)m_nOperMode);

	if ((nOld_OperMode != enEqpOperatingMode::EOM_Auto) &&
		(IN_nOperMode == enEqpOperatingMode::EOM_Auto))
	{
		WM_Event_Equipment(WM_EVENT_EQUIPMENT_AUTO_MODE);
	}

#if defined(EES_XML)//20231003
	for (int i = 0; i < Get_mEES_PortSubStatusCount(); i++) {
		Get_mEES_PortSubStatus(i).Set_nOperMode(IN_nOperMode);
	}
	WM_Event_Equipment(WM_EVENT_EQUIPMENT_REPORT_EQUIPMENT_STATE, (LPARAM)NULL); // alarm on
#endif 
}

//=============================================================================
// Method		: Get_ProcessStatus
// Access		: public  
// Returns		: uint8_t
// Qualifier	: const
// Last Update	: 2022/1/13 - 19:50
// Desc.		:
//=============================================================================
uint8_t CEquipment::Get_ProcessStatus() const
{
	return m_nProcessStatus;
}

void CEquipment::Set_ProcessStatus(__in uint8_t IN_nStatus)
{
	m_nProcessStatus = IN_nStatus;

	WM_Notify_Equipment(WM_EqpNotify_ProcessStatus, (LPARAM)m_nProcessStatus);
	WM_Event_Equipment(WM_EVENT_EQUIPMENT_CONNECTION, (LPARAM)m_nProcessStatus);

}

void CEquipment::Set_ProcessStatus(__in uint8_t IN_nStatus, __in uint32_t IN_nAlarmCode, __in LPCTSTR IN_szAlarmInfo)
{
	uint8_t Old_Status	= m_nProcessStatus;
	m_nProcessStatus	= IN_nStatus;

	// 알람 누적(????)
	//m_nAlarmCode	= IN_nAlarmCode;
	//m_szAlarmInfo	= IN_szAlarmInfo;

	WM_Notify_Equipment(WM_EqpNotify_ProcessStatus, (LPARAM)m_nProcessStatus);

	// 정상 => Alarm
	if (enEqpProcessStatus::EPC_Alarm == IN_nStatus)
	{
		ST_AlarmStatus stAlarm;
		stAlarm.m_nAlarmCode = IN_nAlarmCode;
		stAlarm.m_szAlarmInfo = IN_szAlarmInfo;

		m_nAlarmStatus.push_back(stAlarm);

#if defined(EES_XML)//20231003
		CAlarmStatus stEESAlarm;
		stEESAlarm.Set_nAlarmCode(IN_nAlarmCode);
		stEESAlarm.Set_szAlarmInfo(IN_szAlarmInfo);
		stEESAlarm.Set_nAlarmSet(ALARMSET_SET);
		Set_mAlarmStatus(stEESAlarm);
#endif
		WM_Event_Equipment(WM_EVENT_EQUIPMENT_ALARM, (LPARAM)m_nProcessStatus); // alarm on
	}
	// Alarm => 정상
	else if ((enEqpProcessStatus::EPC_Alarm == Old_Status) && (enEqpProcessStatus::EPC_Alarm != IN_nStatus))
	{
		WM_Event_Equipment(WM_EVENT_EQUIPMENT_ALARM, (LPARAM)m_nProcessStatus); // alarm off
	}
#if defined(EES_XML)//20231003
	for (int i = 0; i < Get_mEES_PortSubStatusCount() ; i++) {
		Get_mEES_PortSubStatus(i).Set_nProcessStatus(IN_nStatus);		
	}	
	if (0 < Get_mEES_PortSubStatusCount()) {
		WM_Event_Equipment(WM_EVENT_EQUIPMENT_REPORT_EQUIPMENT_STATE, (LPARAM)NULL);
	}	
#endif 	
}

//=============================================================================
// Method		: Get_AlarmCount
// Access		: public  
// Returns		: size_t
// Qualifier	: const
// Last Update	: 2022/2/16 - 17:40
// Desc.		:
//=============================================================================
size_t CEquipment::Get_AlarmCount() const
{
	return m_nAlarmStatus.size();
}

const ST_AlarmStatus& CEquipment::Get_AlarmStatus_Last() const
{
	return m_nAlarmStatus.back();
}

std::vector<ST_AlarmStatus>& CEquipment::Get_AlarmStatus()
{
	return m_nAlarmStatus;
}

const ST_AlarmStatus & CEquipment::Get_AlarmStatus(uint32_t IN_nIndex) const
{
	return m_nAlarmStatus.at(IN_nIndex);
}

//void CEquipment::Set_AlarmStatus_Last(uint32_t IN_nAlarmCode, LPCTSTR IN_szAlarmInfo)
//{
//	//m_nAlarmCode = IN_nAlarmCode;
//	//m_szAlarmInfo = IN_szAlarmInfo;
//}

//=============================================================================
// Method		: Get_Status_LED
// Access		: public  
// Returns		: uint8_t
// Qualifier	: const
// Last Update	: 2022/1/13 - 19:51
// Desc.		:
//=============================================================================
uint8_t CEquipment::Get_Status_LED() const
{
	return m_bLED_Status;
}

void CEquipment::Set_Status_LED(__in uint8_t IN_nLED_Status)
{
	m_bLED_Status = IN_nLED_Status;

	WM_Notify_Equipment(WM_EqpNotify_ProcessStatus, (LPARAM)m_bLED_Status);
}

//=============================================================================
// Method		: Get_Skip
// Access		: public  
// Returns		: bool
// Qualifier	: const
// Last Update	: 2022/1/13 - 19:55
// Desc.		:
//=============================================================================
bool CEquipment::Get_Skip() const
{
	return m_bSkip;
}

void CEquipment::Set_Skip(__in bool IN_bSkip, __in bool IN_bSave /*= true*/)
{
	m_bSkip = IN_bSkip;

	if (IN_bSave)
	{
		Save_Equipment_Skip();
	}

	WM_Notify_Equipment(WM_EqpNotify_Skip, (LPARAM)m_bSkip);
}

//=============================================================================
// Method		: Get_TimeSync
// Access		: public  
// Returns		: bool
// Qualifier	: const
// Last Update	: 2022/1/13 - 19:55
// Desc.		:
//=============================================================================
bool CEquipment::Get_TimeSync() const
{
	return m_bTimeSync;
}

void CEquipment::Set_TimeSync(__in bool IN_bTimeSync)
{
	m_bTimeSync = IN_bTimeSync;

	WM_Notify_Equipment(WM_EqpNotify_TimeSync, (LPARAM)m_bTimeSync);
}

//=============================================================================
// Method		: Reset_ReservedPortInfo
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2023/2/27 - 19:51
// Desc.		:
//=============================================================================
void CEquipment::Reset_ReservedPortInfo()
{
	m_vReservePort.clear();
	Set_ReservedOverCnt(0);
	Set_ReservedPortCnt(0);
}

//=============================================================================
// Method		: Get_ReservedPortCnt
// Access		: public  
// Returns		: uint8_t
// Qualifier	: const
// Last Update	: 2022/5/12 - 15:30
// Desc.		:
//=============================================================================
uint8_t CEquipment::Get_ReservedPortCnt() const
{
	return m_nReservedPortCnt;
}

void CEquipment::Set_ReservedPortCnt(uint8_t IN_nCount, __in bool IN_bSave /*= true*/)
{
	m_nReservedPortCnt = IN_nCount;

	//레지스트리에 저장
	if (IN_bSave)
	{
		Save_Equipment_Reserve();
	}
}

//=============================================================================
// Method		: Increase_ReservedPort
// Access		: public  
// Returns		: bool
// Qualifier	:
// Last Update	: 2022/5/12 - 15:06
// Desc.		: 소켓 투입 예약 (테스터 only)
//=============================================================================
bool CEquipment::Increase_ReservedPort()
{
	bool bReturn = false;
	if (Is_Tester())
	{
		// 예약 가능하면 증가한다. (m_nAvablePortCnt - Get_UsingPortCount())
		if (m_nReservedPortCnt < m_nAvablePortCnt)
		{
			++m_nReservedPortCnt;

			bReturn = true;
		}
		else // 예약 불가능 하면..
		{
			++m_nReservedOvered;
		}

		Save_Equipment_Reserve();
	}

	return bReturn;
}

bool CEquipment::Increase_ReservedPort(__in LPCTSTR IN_szRFID)
{
	bool bReturn = false;
	if (Is_Tester())
	{
		// 예약된 소켓들 체크
		uint8_t nRemovedCnt = Check_ReservedSocket();

		// queue에서 rfid 검색..  있으면 기존거 제거하고 새로 추가
		for (auto nIter = m_vReservePort.begin(); nIter != m_vReservePort.end(); )
		{
			if (0 == (*nIter).szRfid.Compare(IN_szRFID))
			{
				CString szText;
				szText.Format(_T("[eqp %02d] (%s) : Removing reserved sockets due to duplicate RFIDs. => rfid: %s"),
							m_nEqpOrder,
							_T(__FUNCTION__),
							(*nIter).szRfid);
				AfxGetApp()->GetMainWnd()->SendMessage(WM_LOGMSG, (WPARAM)szText.GetBuffer(), MAKELPARAM(LOGTYPE_NONE, 0));

				// 제거
				nIter = m_vReservePort.erase(nIter);

				++nRemovedCnt;
			}
			else
			{
				++nIter;
			}
		}

		// 예약 정보 추가
		ST_ReservedSocket socket;
		socket.szRfid = IN_szRFID;
		GetLocalTime(&socket.time);
		m_vReservePort.push_back(socket);

		// 예약 포트 갯수 구하기
		m_nReservedPortCnt = static_cast<uint8_t>(m_vReservePort.size());
		if (m_nAvablePortCnt < m_nReservedPortCnt)
		{
			m_nReservedOvered = m_nReservedPortCnt - m_nAvablePortCnt;
			m_nReservedPortCnt = m_nAvablePortCnt;
		}
		else
		{
			bReturn = true;
		}

		//Save_Equipment_ReserveQueue();
		Save_Equipment_Reserve();
	}

	return bReturn;
}

void CEquipment::Decrease_ReservedPort()
{
	if (Is_Tester())
	{
		if (0 < m_nReservedOvered)
		{
			--m_nReservedOvered;
		}
		else if (0 < m_nReservedPortCnt)
		{
			--m_nReservedPortCnt;
		}

		Save_Equipment_Reserve();
	}
}

void CEquipment::Decrease_ReservedPort(__in LPCTSTR IN_szRFID)
{
	if (Is_Tester())
	{
		// 예약된 소켓들 체크
		uint8_t nRemovedCnt = Check_ReservedSocket();

		// queue에서 rfid 검색..  있으면 기존거 제거
		for (auto nIter = m_vReservePort.begin(); nIter != m_vReservePort.end(); )
		{
			if (0 == (*nIter).szRfid.Compare(IN_szRFID))
			{
// 				CString szText;
// 				szText.Format(_T("[eqp %02d] (%s) : Remove reserved sockets. => rfid: %s"),
// 								m_nEqpOrder,
// 								_T(__FUNCTION__),
// 								(*nIter).szRfid);
// 				AfxGetApp()->GetMainWnd()->SendMessage(WM_LOGMSG, (WPARAM)szText.GetBuffer(), MAKELPARAM(LOGTYPE_NONE, 0));

				// 제거
				nIter = m_vReservePort.erase(nIter);

				++nRemovedCnt;
			}
			else
			{
				++nIter;
			}
		}

		// 예약 포트 갯수 구하기
		m_nReservedPortCnt = static_cast<uint8_t>(m_vReservePort.size());
		if (m_nAvablePortCnt < m_nReservedPortCnt)
		{
			m_nReservedOvered = m_nReservedPortCnt - m_nAvablePortCnt;
			m_nReservedPortCnt = m_nAvablePortCnt;
		}

		//Save_Equipment_ReserveQueue();
		Save_Equipment_Reserve();
	}
}

//=============================================================================
// Method		: Get_ReservedOverCnt
// Access		: public  
// Returns		: uint8_t
// Qualifier	: const
// Last Update	: 2022/5/25 - 14:32
// Desc.		:
//=============================================================================
uint8_t CEquipment::Get_ReservedOverCnt() const
{
	return m_nReservedOvered;
}

void CEquipment::Set_ReservedOverCnt(__in uint8_t IN_nCount)
{
	m_nReservedOvered = IN_nCount;
}

//=============================================================================
// Method		: Check_ReservedSocket
// Access		: public  
// Returns		: uint8_t
// Qualifier	:
// Last Update	: 2023/2/27 - 19:58
// Desc.		: 예약된 소켓 체크
//=============================================================================
uint8_t CEquipment::Check_ReservedSocket()
{
	uint8_t nRemovedCnt = 0;

	if (Is_Tester())
	{
		// 1. 예약된 시간이 오래되었으면 제거 (중간에 라인 멈춘거 고려 해야함. 오래된것은 중간에 소켓 제거로 판단)
		SYSTEMTIME tmLocal;
		GetLocalTime(&tmLocal);

		for (auto nIter = m_vReservePort.begin(); nIter != m_vReservePort.end(); )
		{
			// 시간 비교 (설정된 시간 보다 차이가 크면 삭제)
			auto tmDiffer = abs(CompareSystemTime(&tmLocal, &(*nIter).time));

			if (m_dReservedTimeoutSec <= tmDiffer)
			{
				CString szText;
				szText.Format(_T("[eqp %02d] (%s) : Removed sockets reserved to time out => rfid: %s, time: %02d:%02d:%02d"), 
									m_nEqpOrder,
									_T(__FUNCTION__), 
									(*nIter).szRfid, 
									(*nIter).time.wHour, 
									(*nIter).time.wMinute, 
									(*nIter).time.wSecond);
				AfxGetApp()->GetMainWnd()->SendMessage(WM_LOGMSG, (WPARAM)szText.GetBuffer(), MAKELPARAM(LOGTYPE_NONE, 0));

				// 제거			
				nIter = m_vReservePort.erase(nIter);

				++nRemovedCnt;
			}
			else
			{
				//++nIter;
				break;
			}
		}

		// 2. 예약된 RFID의 "타겟"이 현재 설비가 아니면 제거 (중간에 빠졌다 재투입으로 판단)
		for (auto nIter = m_vReservePort.begin(); nIter != m_vReservePort.end(); )
		{
			auto socket = m_pSocketInfo->GetAt((*nIter).szRfid);
			if (Get_EqpOrder() != socket.m_nTargetEqpOrder)
			{
				CString szText;
				szText.Format(_T("[eqp %02d] (%s) : Remove the reserved socket because the socket's destination has changed. => rfid: %s, target eqp: %02d"), 
									m_nEqpOrder,
									_T(__FUNCTION__),
									(*nIter).szRfid, 
									socket.m_nTargetEqpOrder);
				AfxGetApp()->GetMainWnd()->SendMessage(WM_LOGMSG, (WPARAM)szText.GetBuffer(), MAKELPARAM(LOGTYPE_NONE, 0));

				// 제거
				nIter = m_vReservePort.erase(nIter);

				++nRemovedCnt;
			}
			else
			{
				++nIter;
			}
		}
	}

	return nRemovedCnt;
}

//=============================================================================
// Method		: Get_ReservedInfo
// Access		: public  
// Returns		: std::vector<ST_ReservedSocket>&
// Qualifier	:
// Last Update	: 2023/2/27 - 19:58
// Desc.		:
//=============================================================================
std::vector<ST_ReservedSocket>& CEquipment::Get_ReservedInfo()
{
	return m_vReservePort;
}

//=============================================================================
// Method		: Get_CheckShiftTime
// Access		: public  
// Returns		: const SYSTEMTIME &
// Qualifier	: const
// Last Update	: 2022/5/12 - 15:29
// Desc.		:
//=============================================================================
const SYSTEMTIME & CEquipment::Get_CheckShiftTime() const
{
	return m_tm_CheckShift;
}

void CEquipment::Set_CheckShiftTime(SYSTEMTIME* IN_ptmCheck, bool IN_bSave /*= true*/)
{
	memcpy(&m_tm_CheckShift, IN_ptmCheck, sizeof(SYSTEMTIME));

	//레지스트리에 저장
	if (IN_bSave)
	{
		Save_Equipment_Shift();
	}
}

#ifdef USE_EQP_TACTTIME
const CTactTime & CEquipment::Get_Tacttime() const
{
	return m_Tacttime;
}
#endif

//=============================================================================
// Method		: IsAlarm
// Access		: public  
// Returns		: bool
// Qualifier	:
// Last Update	: 2022/2/10 - 16:54
// Desc.		:
//=============================================================================
bool CEquipment::IsAlarm()
{
	return (enEqpProcessStatus::EPC_Alarm == m_nProcessStatus);
}

//=============================================================================
// Method		: IsEmpty_Equipment
// Access		: public  
// Returns		: bool
// Parameter	: __in bool bIgnore_EmptySocket
// Qualifier	:
// Last Update	: 2022/2/19 - 16:50
// Desc.		: 설비에 소켓의 존재 유/무 판단
//=============================================================================
bool CEquipment::IsEmpty_Equipment(__in bool bIgnore_EmptySocket /*= true*/)
{
	auto nStartIdx = 0;
	auto nEndIdx = m_nPortStatus.size();

	if (Is_Loader())
	{
		nStartIdx	= enPortIndex_Loader::PtI_L_RFID;
		nEndIdx		= enPortIndex_Loader::PtI_L_Buffer_3 + 1;
	}

	// 포트가 비어 있나?
	for (auto nIdx = nStartIdx; nIdx < nEndIdx; ++nIdx)
	{
		// PtS_Exist_Socket,	// 1 : 제품 있음	
		// PtS_Wait_Out,		// 2 : 배출대기

		// 포트에 소켓 유/무 판단
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
				// 소켓이 제품이 없는 빈 소켓인가?
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
			// 알람 상태에서는 소켓 유/무 판단을 어떻게 해야 하는가??????
			break;

		default:
			break;
		}
	}

	// 컨베이어가 비어 있나?
	for (auto nIdx = 0; nIdx < m_nConveyorStatus.size(); ++nIdx)
	{
		// 컨베이어위에 소켓 유/무 판단
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

	return true;
}

bool CEquipment::IsEmpty_Equipment_AnySocket()
{
	auto nStartIdx = 0;
	auto nEndIdx = m_nPortStatus.size();

// 	if (Is_Loader())
// 	{
// 		nStartIdx = enPortIndex_Loader::PtI_L_RFID;
// 		nEndIdx = enPortIndex_Loader::PtI_L_Buffer_3 + 1;
// 	}

	// 포트가 비어 있나?
	for (auto nIdx = nStartIdx; nIdx < nEndIdx; ++nIdx)
	{
		// 포트에 소켓 유/무 판단
		switch (m_nPortStatus.at(nIdx).nStatus)
		{
		case PtS_Empty:
		case PtS_Disable:
			break;

		case PtS_Exist_Socket:
		case PtS_Wait_Out:
		{
			return false;
		}
		break;

		case PtS_Alarm:
			// 알람 상태에서는 소켓 유/무 판단을 어떻게 해야 하는가??????
			break;

		default:
			break;
		}
	}

	// 컨베이어가 비어 있나?
	for (auto nIdx = 0; nIdx < m_nConveyorStatus.size(); ++nIdx)
	{
		// 컨베이어위에 소켓 유/무 판단
		if (enConveyorStatus_Exist::CoSE_Exist == m_nConveyorStatus.at(nIdx).nExistSocket)
		{
			return false;
		}
	}

	return true;
}

//=============================================================================
// Method		: IsLastSocket_onTestPort
// Access		: public  
// Returns		: bool
// Qualifier	:
// Last Update	: 2022/4/12 - 14:41
// Desc.		:
//=============================================================================
bool CEquipment::IsLastSocket_onTestPort()
{
	bool bReturn = false;

	if (Is_Tester())
	{
		// 검사 중이거나 대기 중인 제품 카운트.
		uint8_t nTestCount = 0;

		if ((PtS_Exist_Socket == m_nPortStatus.at(enPortIndex_Tester::PtI_T_RFID).nStatus) ||
			(PtS_Wait_Out == m_nPortStatus.at(enPortIndex_Tester::PtI_T_RFID).nStatus))
		{
			return false;
		}

		// 포트가 비어 있나?
		//for (uint8_t nIdx = enPortIndex_Tester::PtI_T_Buffer; nIdx <= enPortIndex_Tester::PtI_T_Test_R; ++nIdx)
		for (uint8_t nIdx = enPortIndex_Tester::PtI_T_Buffer; nIdx < m_nPortStatus.size(); ++nIdx)
		{
			// 포트에 소켓 유/무 판단
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

//=============================================================================
// Method		: Get_EmptyPortCount
// Access		: virtual public  
// Returns		: uint8_t
// Parameter	: __in bool bOnlyTestPort
// Qualifier	:
// Last Update	: 2022/2/19 - 14:22
// Desc.		:
//=============================================================================
uint8_t CEquipment::Get_EmptyPortCount(__in bool bOnlyTestPort /*= false*/)
{
	uint8_t nCount = 0;

	if (Is_Tester())
	{
		uint8_t nStartPort = bOnlyTestPort ? enPortIndex_Tester::PtI_T_Test_L : enPortIndex_Tester::PtI_T_Buffer;

		//for (uint8_t nIdx = enPortIndex_Tester::PtI_T_Buffer; nIdx < m_nPortStatus.size(); ++nIdx)
		for (uint8_t nIdx = nStartPort; nIdx < m_nPortStatus.size(); ++nIdx)
		{
			if (enPortStatus::PtS_Empty == m_nPortStatus.at(nIdx).nStatus)
			{
				++nCount;
			}
		}

		// PtI_T_RFID : 카운트 하지 않음 (검사와 관련된 Port만 체크)

		// 사용 예약 중인가?
		if (0 < m_nReservedPortCnt)
		{
			// 실제 비어있는 포트에서 예약된 포트 갯수를 제외한다.
			nCount = (m_nReservedPortCnt < nCount) ? (nCount - m_nReservedPortCnt) : 0;
		}
	}
	else if (Is_Returner()) // 예약 없음
	{
		// 버퍼1, 환승, 버퍼2
		for (auto nIdx = 0; nIdx < m_nPortStatus.size(); ++nIdx)
		{
			if (enPortStatus::PtS_Empty == m_nPortStatus.at(nIdx).nStatus)
			{
				++nCount;
			}
		}
	}
	else // 예약 없음, 체크 필요 없음
	{

	}

	return nCount;
}

//=============================================================================
// Method		: Get_EmptyConveyorCount
// Access		: virtual public  
// Returns		: uint8_t
// Qualifier	:
// Last Update	: 2022/8/1 - 19:30
// Desc.		:
//=============================================================================
uint8_t CEquipment::Get_EmptyConveyorCount()
{
	uint8_t nCount = 0;

	if (Is_Tester())
	{
		for (uint8_t nIdx = CvI_T_RFID; nIdx <= CvI_T_Test; ++nIdx)
		{
			if (enConveyorStatus_Exist::CoSE_Empty == m_nConveyorStatus.at(nIdx).nExistSocket)
			{
				++nCount;
			}
		}
	}
	else if (Is_Returner()) // 예약 없음
	{
		for (auto nIdx = 0; nIdx < m_nConveyorStatus.size(); ++nIdx)
		{
			if (enConveyorStatus_Exist::CoSE_Empty == m_nConveyorStatus.at(nIdx).nExistSocket)
			{
				++nCount;
			}
		}
	}

	return nCount;
}

uint8_t CEquipment::Get_UsingPortCount()
{
	uint8_t nCount = 0;

	if (Is_Tester())
	{
		for (uint8_t nIdx = enPortIndex_Tester::PtI_T_Buffer; nIdx < m_nPortStatus.size(); ++nIdx)
		{
			if (enPortStatus::PtS_Exist_Socket == m_nPortStatus.at(nIdx).nStatus)
			{
				++nCount;
			}
		}
	}
	else if (Is_Returner()) // 예약 없음
	{
		// 버퍼1, 환승, 버퍼2
		for (auto nIdx = 0; nIdx < m_nPortStatus.size(); ++nIdx)
		{
			if (enPortStatus::PtS_Exist_Socket == m_nPortStatus.at(nIdx).nStatus)
			{
				++nCount;
			}
		}
	}
	else // 예약 없음, 체크 필요 없음
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
uint8_t CEquipment::Get_TestingCount()
{
	if (Is_Tester())
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

		/*if ((enPortStatus::PtS_Exist_Socket == m_nPortStatus.at(PtI_T_Test_L).nStatus) ||
			(enPortStatus::PtS_Wait_Out == m_nPortStatus.at(PtI_T_Test_L).nStatus))
		{
			++nCount;
		}

		if ((enPortStatus::PtS_Exist_Socket == m_nPortStatus.at(PtI_T_Test_R).nStatus) ||
			(enPortStatus::PtS_Wait_Out == m_nPortStatus.at(PtI_T_Test_R).nStatus))
		{
			++nCount;
		}*/

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
uint8_t CEquipment::Get_WaitOutCount()
{
	if (Is_Tester())
	{
		uint8_t nCount = 0;

		for (uint8_t nPortIdx = enPortIndex_Tester::PtI_T_Test_L; nPortIdx < m_nPortStatus.size(); ++nPortIdx)
		{
			if (enPortStatus::PtS_Wait_Out == m_nPortStatus.at(nPortIdx).nStatus)
			{
				++nCount;
			}
		}

		/*if (enPortStatus::PtS_Wait_Out == m_nPortStatus.at(PtI_T_Test_L).nStatus)
		{
			++nCount;
		}

		if (enPortStatus::PtS_Wait_Out == m_nPortStatus.at(PtI_T_Test_R).nStatus)
		{
			++nCount;
		}*/

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
uint8_t CEquipment::Get_SocketCount()
{
	// 설비 내에 존재하는 소켓 갯수
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

uint8_t CEquipment::Get_ProductCount()
{
	// 설비 내에 존재하는 제품이 실린 소켓 갯수
	uint8_t nRetCount = 0;

	auto nStartIdx	= 0;
	auto nEndIdx	= m_nPortStatus.size();

	if (Is_Loader())
	{
		nStartIdx	= enPortIndex_Loader::PtI_L_RFID;
		nEndIdx		= enPortIndex_Loader::PtI_L_Buffer_3 + 1;
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
bool CEquipment::Check_EndProduction()
{

	return false;
}

//=============================================================================
// Method		: Get_InputAvailabilityStatus
// Access		: virtual public  
// Returns		: uint8_t
// 					-> IAS_NoInput,		// 전체 사용 불가	
// 					-> IAS_Bypass,		// Bypass 가능
// 					-> IAS_Test,		// 검사 가능	
// Qualifier	:
// Last Update	: 2022/2/14 - 16:00
// Desc.		:
//=============================================================================
uint8_t CEquipment::Get_InputAvailabilityStatus()
{
	// 검사 설비 ?
	if (Is_Tester())
	{
		//-----------------------------------------------------------
		// 설비 알람 ?
		if (IsAlarm())
		{
			TRACE(_T("[eqp %02d] Get_InputAvailabilityStatus() => Alarm \n"), m_nEqpOrder);
			return enInputAvailabilityStatus::IAS_NoInput;
		}

		//-----------------------------------------------------------
		// 컨베이어 상태 체크??
		if ((enConveyorStatus::CoS_Stop == m_nConveyorStatus.at(CvI_T_Test).nStatus) &&
			(enConveyorStatus_Exist::CoSE_Exist == m_nConveyorStatus.at(CvI_T_Test).nExistSocket)) // 0: Empty, 1: Exist
		{
			TRACE(_T("[eqp %02d] Get_InputAvailabilityStatus() => not usable conveyor \n"), m_nEqpOrder);
			return enInputAvailabilityStatus::IAS_NoInput;
		}

		//-----------------------------------------------------------
		// 설비 Skip ?
		if (Get_Skip())
		{
			return enInputAvailabilityStatus::IAS_Bypass;
		}

		//-----------------------------------------------------------
		// 포트 체크 (PtI_T_RFID, PtI_T_Buffer, PtI_T_Test_L, PtI_T_Test_R)
		// 검사 파라 및 버퍼 투입 가능.
		bool bEmpty = false;
		for (uint8_t nIdx = enPortIndex_Tester::PtI_T_Buffer; nIdx < m_nPortStatus.size(); ++nIdx)
		{
			if (enPortStatus::PtS_Empty == m_nPortStatus.at(nIdx).nStatus)
			{
				bEmpty = true;
				break;
			}
		}

		/*if ((enPortStatus::PtS_Empty == m_nPortStatus.at(PtI_T_Test_L).nStatus) ||
			(enPortStatus::PtS_Empty == m_nPortStatus.at(PtI_T_Test_R).nStatus) ||
			(enPortStatus::PtS_Empty == m_nPortStatus.at(PtI_T_Buffer).nStatus))*/
		if (bEmpty)
		{
			// 사용 예약 중인가?
			if (0 < Get_EmptyPortCount())
			{
				TRACE(_T("[eqp %02d] Get_InputAvailabilityStatus() => testable \n"), m_nEqpOrder);
				return enInputAvailabilityStatus::IAS_Test;
			}
		}

		//-----------------------------------------------------------
		// RFID
		if (enPortStatus::PtS_Empty == m_nPortStatus.at(PtI_T_RFID).nStatus)
		{
			TRACE(_T("[eqp %02d] Get_InputAvailabilityStatus() => bypass : rfid empty \n"), m_nEqpOrder);

			return enInputAvailabilityStatus::IAS_Bypass;
		}
	}
	else if (Is_Returner())
	{
		// 버퍼1, 환승, 버퍼2 (버퍼 1이 비어 있으면 투입 가능)
		if (0 < Get_EmptyPortCount())
		{
			TRACE(_T("[returner] Get_InputAvailabilityStatus() => bypass \n"), m_nEqpOrder);
			return enInputAvailabilityStatus::IAS_Test;
		}
	}
	else // 로더 : 체크 필요 없음
	{		
		return enInputAvailabilityStatus::IAS_Test;
	}

	TRACE(_T("[returner] Get_InputAvailabilityStatus() => Impossible to put into the next equipment \n"), m_nEqpOrder);
	return enInputAvailabilityStatus::IAS_NoInput;
}

//=============================================================================
// Method		: Get_ElapsedTime_InputPara
// Access		: virtual public  
// Returns		: uint32_t
// Parameter	: __in uint8_t IN_nPara
// Qualifier	:
// Last Update	: 2022/5/12 - 17:27
// Desc.		:
//=============================================================================
uint32_t CEquipment::Get_ElapsedTime_InputPara(__in uint8_t IN_nPara)
{
	return 0;
}

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
bool CEquipment::Recv_RegisterSocket(__in LPCTSTR IN_szRFID, __in LPCTSTR IN_szBarcode)
{
	// 로더에서 소켓에 제품을 장착하여, RFID 읽어서 서버에 전송함

	if (Is_Loader())
	{
		if (m_pSocketInfo)
		{
			// 제품 있음 / 제품 없음 ?
			if (0 < _tcslen(IN_szBarcode))
			{

			}

			// 바코드에서 \r \n 제거
			/*CString szBarcode = IN_szBarcode;
			szBarcode.Remove(_T('\r'));
			szBarcode.Remove(_T('\n'));*/

			if (m_pSocketInfo->Is_ExistSocket(IN_szRFID))
			{
				bool bReturn = m_pSocketInfo->Register_Socket(m_szEquipmentId, IN_szRFID, IN_szBarcode);

				WM_Event_Equipment(WM_EVENT_LOADER_RESISTER_SOCKET, IN_szRFID);

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
bool CEquipment::Recv_ReqAcceptSocket(__in LPCTSTR IN_szRFID)
{
	if (m_hWndOwner)
	{
		// 설비에 소켓 도착하여 투입 가능하지 판단

		// 설비가 알람?

		// 설비가 Skip?

		if (m_pSocketInfo)
		{
			// 현재 모델 설정에 맞는 RFID인가?
			//if (m_pSocketInfo->Is_ExistSocket(IN_szRFID))
			if (m_pSocketInfo->Verify_Socket(IN_szRFID))
			{
				//Set_TrackInRequestSocket(IN_szRFID); // 2023.08.07

				WM_Event_Equipment(WM_EVENT_TESTER_TRACKIN, IN_szRFID);

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
bool CEquipment::Recv_NotifyTestResult(__in LPCTSTR IN_szRFID, __in int16_t IN_nNGCode, __in uint8_t IN_nPara)
{
	if (Is_Tester())
	{
		// Shift 변경 체크 & Tacttime 체크
		if (Is_ChangeShift())
		{
			Report_Yield_Day();

			if (m_bAutoReset_Yield)
			{
				Reset_Yield_Day();
			}
		}

		// 양/불 체크
		if (0 == IN_nNGCode) // MES Rework NG Code 이면...???
		{
#ifndef USE_EQP_YIELD_UPDATE_AT_UNLOAD
			Increase_Yield_Pass(IN_nPara);
#endif //USE_EQP_YIELD_UPDATE_AT_UNLOAD
		}
		else if (m_pSocketInfo->Get_MES_ReworkCode() == IN_nNGCode)// MES Rework NG Code 이면...???
		{
			;
		}
		else
		{
#ifndef USE_EQP_YIELD_UPDATE_AT_UNLOAD
			Increase_Yield_Fail(IN_nPara);
#endif //USE_EQP_YIELD_UPDATE_AT_UNLOAD

#ifndef USE_NG_CODE_UPDATE_AT_UNLOAD
			// Log : 불량이 발생한 제품 정보 기록 (제품 바코드, NG 발생 검사: Pass된 검사)
			IncreaseFailInfo(IN_nNGCode, IN_nPara);
#endif // USE_NG_CODE_UPDATE_AT_UNLOAD
		}

		if (m_pSocketInfo)
		{
			if (m_pSocketInfo->Set_TestResult(m_szEquipmentId, IN_szRFID, IN_nNGCode, IN_nPara, m_nEqpOrder, Get_EquipmentType(), (m_pSocketInfo->Get_MES_ReworkCode() == IN_nNGCode)))
			{
				if (m_pSocketInfo->Is_ExistSocket(IN_szRFID))
				{
					// 검사 결과를 받으면 배출 승인 여부를 판단해서 서버->설비로 알려줘야 한다.
					WM_Event_Equipment(WM_EVENT_TESTER_END_INSPECTION, IN_szRFID);

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
bool CEquipment::Recv_ReqTestResult(__in LPCTSTR IN_szRFID, __out ST_TestResult& OUT_stResult)
{
	if (Is_Loader())
	{
#ifdef USE_EQP_TACTTIME
		GetLocalTime(&m_tm_CheckShift);
		m_Tacttime.Set_UnloadTime(&m_tm_CheckShift);
		Save_Equipment_Tacttime();

		WM_Notify_Equipment(WM_EqpNotify_Yield, (LPARAM)NULL);
#endif
		WM_Event_Equipment(WM_EVENT_UNLOAD_REQ_TEST_RESULT, IN_szRFID);

		// 검사 결과를 전송한다.
		if (m_pSocketInfo)
		{
			// 제품 있음 : 양품
			// 제품 있음 : 불량
			// 제품 없음 : 양품
			// 배출 시간 설정
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
bool CEquipment::Recv_UnregisterSocket(__in LPCTSTR IN_szRFID)
{
	bool bReturn = false;

	if (Is_Loader())
	{
		// 제품이 있는가?
		if (m_pSocketInfo->GetAt(IN_szRFID).IsEmpty_Barcode())
		{
			;
		}

		if (m_pSocketInfo->Is_ExistSocket(IN_szRFID))
		{
			// 최종 수율 업데이트
			ST_TestResult stResult;
			m_pSocketInfo->Get_TestResult(IN_szRFID, stResult);
			if (0 == stResult.m_nNG_Code)
			{
				Increase_Yield_Pass(0); // 양품은 Para 구분 불가 (양품 판정 검사 설비가 여러대)
			}
			else if (m_pSocketInfo->Get_MES_ReworkCode() == stResult.m_nNG_Code)// MES Rework NG Code 이면...???
			{
				// 오류 상황......
				Increase_Yield_Fail(stResult.m_nNG_Para);
			}
			else if (-2 == stResult.m_nNG_Code)
			{
				; // -2 (빈 소켓)은 카운트 처리 안함 (2023.06.08)
			}
			else
			{
				Increase_Yield_Fail(stResult.m_nNG_Para);
			}

			// 소켓 등록 해제
			if (m_pSocketInfo)
			{
				bReturn = m_pSocketInfo->Unregister_Socket(m_szEquipmentId, IN_szRFID);
			}

			WM_Event_Equipment(WM_EVENT_UNLOAD_UNREGISTER_SOCKET, IN_szRFID);
		}
	}

	return bReturn;
}

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
void CEquipment::Set_Path(__in LPCTSTR IN_szLog, __in LPCTSTR IN_szReport)
{
	m_szPath_Log = IN_szLog;
	m_szPath_Report = IN_szReport;
}

//=============================================================================
// Method		: Report_Yield_Day
// Access		: public  
// Returns		: bool
// Qualifier	:
// Last Update	: 2022/5/25 - 12:07
// Desc.		:
//=============================================================================
bool CEquipment::Report_Yield_Day()
{
	if (m_szPath_Report.IsEmpty())
	{
		TRACE(_T("[%s] LogPath is Empty\n"), _T(__FUNCTION__));
		return false;
	}

	// 년/월/eqpid_yield.csv

	CString szPath;
	CString szFullPath;

	SYSTEMTIME tmLocal;
	GetLocalTime(&tmLocal);

	// Report\\Year\\Month\\ 
	szPath.Format(_T("%s\\%04d\\%02d\\"), m_szPath_Report, tmLocal.wYear, tmLocal.wMonth);
	MakeDirectory(szPath);
	szFullPath.Format(_T("%sYield_%s_%04d_%02d.csv"), szPath, m_szEquipmentId, tmLocal.wYear, tmLocal.wMonth);

	// 년/월/일, equipment no, equpment id, T, P, F, L(T, P, F), R(T, P, F)
	CString szUnicode, szTime;
	CStringA szANSI;
	szTime.Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), tmLocal.wYear, tmLocal.wMonth, tmLocal.wDay, tmLocal.wHour, tmLocal.wMinute, tmLocal.wSecond);

	szUnicode.Format(_T(",%s,%d,%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\r\n"),
		szTime.GetBuffer(),
		m_nEqpOrder,
		m_szEquipmentId,
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
	/*szUnicode.Format(_T(",%s,%d,%s,%d,%d,%d,%d,%d,%d,%d,%d,%d\r\n"),
		szTime.GetBuffer(),
		m_nEqpOrder,
		m_szEquipmentId,
		m_Yield_Day.dwTotal,
		m_Yield_Day.dwPass,
		m_Yield_Day.dwFail,
		m_Yield_Day.m_Para[Para_Left].dwTotal,
		m_Yield_Day.m_Para[Para_Left].dwPass,
		m_Yield_Day.m_Para[Para_Left].dwFail,
		m_Yield_Day.m_Para[Para_Right].dwTotal,
		m_Yield_Day.m_Para[Para_Right].dwPass,
		m_Yield_Day.m_Para[Para_Right].dwFail*/
	);

	CFile File;
	CFileException e;

	if (!PathFileExists(szFullPath.GetBuffer()))
	{
		if (!File.Open(szFullPath.GetBuffer(), CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite, &e))
			return false;

		// 헤더 추가
		// 년/월/일, equipment no, equpment id, T, P, F, L(T, P, F), R(T, P, F)
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

void CEquipment::SetPtr_FailInfo_Eqp(CFailInfo_Eqp * IN_pFailInfo_Eqp)
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
void CEquipment::IncreaseFailInfo(int16_t IN_nNGCode, uint8_t IN_nPara)
{
	if (m_pFailInfo_Eqp)
	{
		m_pFailInfo_Eqp->IncreaseCount(IN_nNGCode, IN_nPara);
	}
}

//=============================================================================
// Method		: Set_ReservedTimeout_Second
// Access		: public  
// Returns		: void
// Parameter	: __in double IN_dSecond
// Qualifier	:
// Last Update	: 2023/2/27 - 20:11
// Desc.		:
//=============================================================================
void CEquipment::Set_ReservedTimeout_Second(__in double IN_dSecond)
{
	m_dReservedTimeoutSec = IN_dSecond;
}

//=============================================================================
// Method		: Set_TrackInRequestSocket
// Access		: protected  
// Returns		: void
// Parameter	: __in LPCTSTR IN_szRFID
// Qualifier	:
// Last Update	: 2023/8/7 - 11:33
// Desc.		:
//=============================================================================
void CEquipment::Set_TrackInRequestSocket(__in LPCTSTR IN_szRFID)
{
	m_szLastTrackIn_RFID = IN_szRFID;
	m_nLastTrackIn_Time = timeGetTime();
	GetLocalTime(&m_tmLastTrackIn);
}

//=============================================================================
// Method		: IsTrackInRequest_Socket
// Access		: public  
// Returns		: bool
// Parameter	: __in LPCTSTR IN_szRFID
// Parameter	: __in uint32_t IN_nCheckTime
// Qualifier	:
// Last Update	: 2023/8/7 - 11:35
// Desc.		:
//=============================================================================
bool CEquipment::IsTrackInRequest_Socket(__in LPCTSTR IN_szRFID, __in uint32_t IN_nCheckTime /*= 900*/)
{
	if (0 == m_szLastTrackIn_RFID.Compare(IN_szRFID))
	{
// 		SYSTEMTIME tmLc;
// 		GetLocalTime(&tmLc);
// 
// 		if (CompareSystemTime(&tmLc, &m_tmLastTrackIn) < IN_nCheckTime)
// 		{
// 			return true;
// 		}

		if (0 < m_nLastTrackIn_Time)
		{
			DWORD dwElapsedTime = 0;
			DWORD dwCurrentTime = timeGetTime();

			if (dwCurrentTime < m_nLastTrackIn_Time)
			{
				dwElapsedTime = 0xFFFFFFFF - m_nLastTrackIn_Time + dwCurrentTime;
			}
			else
			{
				dwElapsedTime = dwCurrentTime - m_nLastTrackIn_Time;
			}

			return (dwElapsedTime < IN_nCheckTime);
		}
	}


	return false;
}

#if defined(EES_XML)//20231003
void CEquipment::Set_Notify_EquipmentState(__in lt::Report_Equipment_State_Args::Args *IN_DATA) {
	LPARAM IN_PARA = (LPARAM)IN_DATA;
	WM_Notify_Equipment(WM_EqpNotify_EQUIPMENTSTATE, IN_PARA);
}
void CEquipment::Set_DEFINEDATA(CEquipment & Data) {
	CString			szTemp;
	Get_DEFINEDATA().Set_EQUIPMENTID(lt::ToMultiByte(Data.Get_EquipmentId()));
	DWORD dwAddress = ntohl(Data.Get_IP_Address());
	szTemp.Format(_T("%d.%d.%d.%d"), FOURTH_IPADDRESS(dwAddress), THIRD_IPADDRESS(dwAddress), SECOND_IPADDRESS(dwAddress), FIRST_IPADDRESS(dwAddress));
	Get_DEFINEDATA().Set_IPADDRESS(lt::ToMultiByte(szTemp));
	Get_DEFINEDATA().Set_SUBEQPID(lt::ToMultiByte(Data.Get_SubEqpID()));
}
#endif