//*****************************************************************************
// Filename	: 	EqpTester.cpp
// Created	:	2021/11/19 - 10:35
// Modified	:	2021/11/19 - 10:35
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#include "stdafx.h"
#include "EqpTester.h"

CEqpTester::CEqpTester()
{
	//m_nPortCount = PtI_T_MaxCount;
	//m_nConveyorCount = CvI_T_MaxCount;

	m_nPortStatus.assign(PtI_T_MaxCount, { 0, _T("") });
	m_nConveyorStatus.assign(CvI_T_MaxCount, { 0, 0, _T("") });

	for (auto nIdx = 0; nIdx < Para_MaxCount; ++nIdx)
	{
		memset(&m_tmInput_Para[nIdx], 0, sizeof(SYSTEMTIME));
	}
}

CEqpTester::~CEqpTester()
{

}

void CEqpTester::Init_EquipmentType_UI()
{
	__super::Init_EquipmentType_UI();

	Update_DataSize();
}

// void CEqpTester::Init_EquipmentType()
// {
// 	__super::Init_EquipmentType();
// 
// 	Update_DataSize();
// }

//=============================================================================
// Method		: Set_Time_InputPara
// Access		: virtual protected  
// Returns		: void
// Parameter	: __in uint8_t IN_nPara
// Qualifier	:
// Last Update	: 2022/2/3 - 20:21
// Desc.		:
//=============================================================================
void CEqpTester::Set_Time_InputPara(__in uint8_t IN_nPara)
{
	if (IN_nPara < Para_MaxCount)
	{
		GetLocalTime(&m_tmInput_Para[IN_nPara]);
	}
}

//=============================================================================
// Method		: Set_PortStatus
// Access		: virtual public  
// Returns		: void
// Parameter	: __in uint8_t IN_nPortIndex
// Parameter	: __in uint8_t IN_nStatus
// Parameter	: __in LPCTSTR IN_szRFID
// Parameter	: __in LPCTSTR IN_szBarcode
// Qualifier	:
// Last Update	: 2022/2/3 - 11:57
// Desc.		:
//=============================================================================
void CEqpTester::Set_PortStatus(__in uint8_t IN_nPortIndex, __in uint8_t IN_nStatus, __in LPCTSTR IN_szRFID, __in LPCTSTR IN_szBarcode, __in bool IN_bSave /*= true*/)
{
	//__super::Set_PortStatus(IN_nPortIndex, IN_nStatus, IN_szRFID, IN_szBarcode);

	if (IN_nPortIndex < m_nPortStatus.size())
	{
		uint8_t oldStatus = m_nPortStatus.at(IN_nPortIndex).nStatus;

		m_nPortStatus.at(IN_nPortIndex).nStatus		= IN_nStatus;
		m_nPortStatus.at(IN_nPortIndex).szRfid		= IN_szRFID;
		m_nPortStatus.at(IN_nPortIndex).szBarcode	= IN_szBarcode;

		if (IN_bSave)
		{
			Save_Equipment_Port(IN_nPortIndex);
		}

		// 포트 Disable, Alarm 체크
		switch (oldStatus)
		{
		case enPortStatus::PtS_Disable:
		case enPortStatus::PtS_Alarm:
			switch (IN_nStatus)
			{
			case enPortStatus::PtS_Empty:
			case enPortStatus::PtS_Exist_Socket:
			case enPortStatus::PtS_Wait_Out:
				// 포트 사용 불가 -> 포트 사용 가능 상태로 변경됨
				break;
			}
			break;

		default:
			switch (IN_nStatus)
			{
			case enPortStatus::PtS_Disable:
			case enPortStatus::PtS_Alarm:
				// 포트 사용 가능 -> 포트 사용 불가 상태로 변경됨
				break;
			}
			break;
		}

		// 예약 체크
		if ((enPortStatus::PtS_Empty == oldStatus) && (enPortStatus::PtS_Exist_Socket == IN_nStatus))
		{
			switch (IN_nPortIndex)
			{
			case PtI_T_Buffer:
			case PtI_T_Test_L:
			case PtI_T_Test_R:
			case PtI_T_Test_C:
				//Decrease_ReservedPort();
				Decrease_ReservedPort(IN_szRFID);
				break;
			}

			// Tester의 L/R 파라에 소켓이 투입되면 시간 체크한다.
			if ((PtI_T_Test_L == IN_nPortIndex) || (PtI_T_Test_R == IN_nPortIndex) || (PtI_T_Test_C == IN_nPortIndex))
			{
				uint8_t nPara = PortIndex2TestPara(IN_nPortIndex);

				Set_Time_InputPara(nPara);
			}
		}

		// GUI 갱신
		WM_Notify_Equipment(WM_EqpNotify_PortStatus, MAKELPARAM(IN_nPortIndex, IN_nStatus));

		// 이전 상태 : 제품 있음 or 배출 대기 => 현재 상태 : 비어 있음
		if (((enPortStatus::PtS_Exist_Socket == oldStatus) || 
			(enPortStatus::PtS_Wait_Out == oldStatus)) &&
			(enPortStatus::PtS_Empty == IN_nStatus))
		{
			// 포트 상태 변경 이벤트
			WM_Event_Equipment(WM_EVENT_EQUIPMENT_PORT_STATUS, (LPARAM)IN_nPortIndex);
		}

		// 소켓 정보 갱신
		Update_SocketLocation(IN_nPortIndex, &m_nPortStatus.at(IN_nPortIndex));
	}
}

//=============================================================================
// Method		: Set_ConveyorStatus
// Access		: virtual public  
// Returns		: void
// Parameter	: __in uint8_t IN_nConveyorIndex
// Parameter	: __in uint8_t IN_nStatus
// Parameter	: __in uint8_t IN_nExistSocket
// Parameter	: __in LPCTSTR IN_szRFID
// Parameter	: __in LPCTSTR IN_szBarcode
// Parameter	: __in bool IN_bSave
// Qualifier	:
// Last Update	: 2022/4/12 - 14:47
// Desc.		:
//=============================================================================
void CEqpTester::Set_ConveyorStatus(__in uint8_t IN_nConveyorIndex, __in uint8_t IN_nStatus, __in uint8_t IN_nExistSocket, __in LPCTSTR IN_szRFID, __in LPCTSTR IN_szBarcode, __in bool IN_bSave /*= true*/)
{
	//__super::Set_ConveyorStatus(IN_nPortIndex, IN_nStatus, IN_szRFID, IN_szBarcode);

	if (IN_nConveyorIndex < m_nConveyorStatus.size())
	{
		uint8_t oldStatus		= m_nConveyorStatus.at(IN_nConveyorIndex).nStatus;
		uint8_t oldExistStatus	= m_nConveyorStatus.at(IN_nConveyorIndex).nExistSocket;

		m_nConveyorStatus.at(IN_nConveyorIndex).nStatus			= IN_nStatus;
		m_nConveyorStatus.at(IN_nConveyorIndex).nExistSocket	= IN_nExistSocket;
		m_nConveyorStatus.at(IN_nConveyorIndex).szRfid			= IN_szRFID;
		m_nConveyorStatus.at(IN_nConveyorIndex).szBarcode		= IN_szBarcode;

		if (IN_bSave)
		{
			Save_Equipment_Conveyor(IN_nConveyorIndex);
		}

		WM_Notify_Equipment(WM_EqpNotify_ConveyorStatus, MAKELPARAM(IN_nConveyorIndex, MAKEWORD(IN_nStatus, IN_nExistSocket)));

		// 이전 상태 : 중지 & 제품 있음 => 현재 상태 : nc, 제품 없음
		if ((enConveyorStatus::CoS_Stop == oldStatus) &&
			(enConveyorStatus_Exist::CoSE_Exist == oldExistStatus) &&
			(enConveyorStatus_Exist::CoSE_Empty == IN_nExistSocket))
		{
			WM_Event_Equipment(WM_EVENT_EQUIPMENT_CONVEYOR_STATUS, (LPARAM)IN_nConveyorIndex);
		}

		if (enConveyorIndex_Tester::CvI_T_Test == IN_nConveyorIndex)
		{
			if ((enConveyorStatus_Exist::CoSE_Exist == oldExistStatus) &&
				(enConveyorStatus_Exist::CoSE_Empty == IN_nExistSocket))
			{
				// 설비내에 제품이 남아 있는가?
				/*if (IsEmpty_Equipment())
				{
					// 생산 종료처리 완료됨
					WM_Event_Equipment(WM_EVENT_EQUIPMENT_EMPTY_EQP, (LPARAM)0); //WM_EVENT_EQUIPMENT_EMPTY_EQP
				}
				else */
				if (IsLastSocket_onTestPort())
				{					
					WM_Event_Equipment(WM_EVENT_EQUIPMENT_EMPTY_EQP, (LPARAM)0);
				}
			}
		}
	}
}

//=============================================================================
// Method		: Get_ElapsedTime_InputPara
// Access		: virtual public  
// Returns		: uint32_t
// Parameter	: __in uint8_t IN_nPara
// Qualifier	:
// Last Update	: 2022/2/3 - 20:22
// Desc.		:
//=============================================================================
uint32_t CEqpTester::Get_ElapsedTime_InputPara(__in uint8_t IN_nPara)
{
	if (IN_nPara < Para_MaxCount)
	{
		SYSTEMTIME tmLocal;
		GetLocalTime(&tmLocal);

		auto difftime = CompareSystemTime(&tmLocal, &m_tmInput_Para[IN_nPara]);

		return static_cast<uint32_t>(difftime);
	}

	return 0;
}

void CEqpTester::Update_DataSize()
{
	if ((m_nReservablePortCount + 1) != m_nPortStatus.size())
	{
		if (0 < m_nPortStatus.size())
			m_nPortStatus.clear();

		m_nPortStatus.assign(m_nReservablePortCount + 1, { 0, _T("") });
	}
}
