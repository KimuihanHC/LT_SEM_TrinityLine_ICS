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

	//2023.04.27a uhkim


	for (auto nIdx = 0; nIdx < Para_MaxCount; ++nIdx)
	{
		memset(&m_tmInput_Para[nIdx], 0, sizeof(SYSTEMTIME));
	}
#if (USE_XML)
	assign_mEES_PortSubStatus(PtI_T_MaxCount, {});
	assign_EquipmentIDStatus(PtI_T_MaxCount, {});
#endif

#if SOCKET
	m_nOldPortStatus.assign(PtI_T_MaxCount, { 0, _T("") });
	m_nEquipmentID.assign(PtI_T_MaxCount, { _T("") , _T("") });
#endif 
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

#if (USE_XML)
	if (IN_nPortIndex < Get_mEES_PortSubStatusCount()) {
		Get_mEES_PortSubStatus(IN_nPortIndex).Set_nPortStatus(IN_nStatus);
		Get_mEES_PortSubStatus(IN_nPortIndex).Set_szRfid(IN_szRFID);
		Get_mEES_PortSubStatus(IN_nPortIndex).Set_szBarcode(IN_szBarcode);
		WM_Event_Equipment(WM_EVENT_EQUIPMENT_REPORT_EQUIPMENT_STATE, (LPARAM)NULL);
		if ((IN_nStatus == PtS_RUN) ||
			(IN_nStatus == PtS_STOP) ||
			(IN_nStatus == PtS_IDLE)) {
			return;
		}
	}
#endif

	if (IN_nPortIndex < m_nPortStatus.size())
	{
		uint8_t oldStatus = m_nPortStatus.at(IN_nPortIndex).nStatus;

		m_nPortStatus.at(IN_nPortIndex).nStatus = IN_nStatus;
		m_nPortStatus.at(IN_nPortIndex).szRfid = IN_szRFID;
		m_nPortStatus.at(IN_nPortIndex).szBarcode = IN_szBarcode;

		if (IN_bSave)
		{
			Save_Equipment_Port(IN_nPortIndex);
		}

		// ?¨Ìä∏ Disable, Alarm Ï≤¥ÌÅ¨
		switch (oldStatus)
		{
		case enPortStatus::PtS_Disable:
		case enPortStatus::PtS_Alarm:
			switch (IN_nStatus)
			{
			case enPortStatus::PtS_Empty:
			case enPortStatus::PtS_Exist_Socket:
			case enPortStatus::PtS_Wait_Out:
				// ?¨Ìä∏ ?¨Ïö© Î∂àÍ? -> ?¨Ìä∏ ?¨Ïö© Í∞Ä???ÅÌÉúÎ°?Î≥ÄÍ≤ΩÎê®
				break;
			}
			break;

		default:
			switch (IN_nStatus)
			{
			case enPortStatus::PtS_Disable:
			case enPortStatus::PtS_Alarm:
				// ?¨Ìä∏ ?¨Ïö© Í∞Ä??-> ?¨Ìä∏ ?¨Ïö© Î∂àÍ? ?ÅÌÉúÎ°?Î≥ÄÍ≤ΩÎê®
				break;
			}
			break;
		}

		// ?àÏïΩ Ï≤¥ÌÅ¨
		if ((enPortStatus::PtS_Empty == oldStatus) && (enPortStatus::PtS_Exist_Socket == IN_nStatus))
		{
			switch (IN_nPortIndex)
			{
			case PtI_T_Buffer:
			case PtI_T_Test_L:
			case PtI_T_Test_R:
			case PtI_T_Test_C:
				Decrease_ReservedPort();
				break;
			}

			// Tester??L/R ?åÎùº???åÏºì???¨ÏûÖ?òÎ©¥ ?úÍ∞Ñ Ï≤¥ÌÅ¨?úÎã§.//
			if ((PtI_T_Test_L == IN_nPortIndex) || (PtI_T_Test_R == IN_nPortIndex) || (PtI_T_Test_C == IN_nPortIndex))
			{
				uint8_t nPara = PortIndex2TestPara(IN_nPortIndex);

				Set_Time_InputPara(nPara);
			}
		}

		// GUI Í∞±Ïã†
		WM_Notify_Equipment(WM_EqpNotify_PortStatus, MAKELPARAM(IN_nPortIndex, IN_nStatus));
#if SOCKET
		//2024.04.26a uhkim
		CString szRFID(IN_szRFID);
		//if (m_pSocketInfo->Is_ExistSocket(szRFID)) 
		{
			CString LOTID(Get_FromPortToLOTID(IN_nPortIndex));
			CString EQUIPMENTID(Get_EquipmentIDStatus(IN_nPortIndex).szEquipID);
			CString SUBEQPID(Get_SubEqpID());
			CString PORTID(Get_EquipmentIDStatus(IN_nPortIndex).szPortID);
			//CString LOTID(m_pSocketInfo->GetAt(szRFID).m_LotID);
			//CString SUBEQPID(Get_EquipmentId());
			//CString PORTID(g_szEES_PORTID[IN_nPortIndex]);
#if TEST
			if (IN_nStatus == PtS_Exist_Socket) {
				Set_REPORT_START_PROCESS(
					GetXmlEes().Set_ReportStartProcessParameter(
						EQUIPMENTID,
						SUBEQPID,
						PORTID,
						LOTID));
			}
			if (IN_nStatus == PtS_Wait_Out) {
				Set_REPORT_END_PROCESS(
					GetXmlEes().Set_ReportEndProcessParameter(
						EQUIPMENTID,
						SUBEQPID,
						PORTID,
						LOTID));
			}
#endif
			
		}	
#endif
		//============================================================
		// ?¥Ï†Ñ ?ÅÌÉú : ?úÌíà ?àÏùå or Î∞∞Ï∂ú ?ÄÍ∏?=> ?ÑÏû¨ ?ÅÌÉú : ÎπÑÏñ¥ ?àÏùå
		if (((enPortStatus::PtS_Exist_Socket == oldStatus) || 
			(enPortStatus::PtS_Wait_Out == oldStatus)) &&
			(enPortStatus::PtS_Empty == IN_nStatus))
		{
			// ?¨Ìä∏ ?ÅÌÉú Î≥ÄÍ≤??¥Î≤§??			WM_Event_Equipment(WM_EVENT_EQUIPMENT_PORT_STATUS, (LPARAM)IN_nPortIndex);
		}

		// ?åÏºì ?ïÎ≥¥ Í∞±Ïã†
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

		// ?¥Ï†Ñ ?ÅÌÉú : Ï§ëÏ? & ?úÌíà ?àÏùå => ?ÑÏû¨ ?ÅÌÉú : nc, ?úÌíà ?ÜÏùå
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
				// ?§ÎπÑ?¥Ïóê ?úÌíà???®ÏïÑ ?àÎäîÍ∞Ä?
				/*if (IsEmpty_Equipment())
				{
					// ?ùÏÇ∞ Ï¢ÖÎ£åÏ≤òÎ¶¨ ?ÑÎ£å??					WM_Event_Equipment(WM_EVENT_EQUIPMENT_EMPTY_EQP, (LPARAM)0); //WM_EVENT_EQUIPMENT_EMPTY_EQP
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

#if (USE_XML)
		assign_mEES_PortSubStatus(m_nReservablePortCount + 1, {});
#endif
	}

}
