//*****************************************************************************
// Filename	: 	EqpLoader.cpp
// Created	:	2021/11/19 - 10:35
// Modified	:	2021/11/19 - 10:35
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#include "stdafx.h"
#include "EqpLoader.h"

CEqpLoader::CEqpLoader()
{
	//m_nPortCount = PtI_L_MaxCount;
	//m_nConveyorCount = CvI_L_MaxCount;

	m_nPortStatus.assign(PtI_L_MaxCount, { 0, _T("") });
	m_nConveyorStatus.assign(CvI_L_MaxCount, { 0, 0, _T("") });

#if (USE_XML)
	assign_mEES_PortSubStatus(PtI_T_MaxCount, {});
	assign_EquipmentIDStatus(PtI_T_MaxCount, {});
#endif
#if SOCKET
	m_nOldPortStatus.assign(PtI_L_MaxCount, { 0, _T("") });
	m_nEquipmentID.assign(PtI_L_MaxCount, { _T("") , _T("") });
#endif 
}

CEqpLoader::~CEqpLoader()
{

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
// Last Update	: 2022/1/28 - 11:20
// Desc.		:
//=============================================================================
void CEqpLoader::Set_PortStatus(__in uint8_t IN_nPortIndex, __in uint8_t IN_nStatus, __in LPCTSTR IN_szRFID, __in LPCTSTR IN_szBarcode, __in bool IN_bSave /*= true*/)
{
	if (IN_nPortIndex < m_nPortStatus.size())
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

		//2024.04.26a uhkim
		CString szRFID(IN_szRFID);
		if (m_pSocketInfo->Is_ExistSocket(szRFID)) 
		{
#if SOCKET
			CString LOTID(m_pSocketInfo->GetAt(szRFID).m_LotID);
			CString EQUIPMENTID(Get_EquipmentIDStatus(IN_nPortIndex).szEquipID);
			CString SUBEQPID(Get_SubEqpID());
#endif	//SOCKET
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
#endif	//TEST
		}

		if (PtI_L_Buffer_3 == IN_nPortIndex)
		{			
			// Î°úÎçî??Î≤ÑÌçº3???ÅÌÉúÍ∞Ä ?ÜÏùå->?àÏùå?ºÎ°ú Î∞îÎÄåÎ©¥ Î∞∞Ï∂ú ?πÏù∏ ?¥Î≤§??Ï≤òÎ¶¨
// 			if ((enPortStatus::PtS_Empty == nOld_PortStatus) &&
// 				(enPortStatus::PtS_Exist_Socket == IN_nStatus))
			// Î°úÎçî?êÏÑú ?†Ìò∏Ï≤òÎ¶¨Í∞Ä ?úÎ?Î°??àÎêò??Í≤ΩÏö∞Í∞Ä ?àÏñ¥???òÏ†ï (2022.10.17)
			if (enPortStatus::PtS_Exist_Socket == IN_nStatus)
			{
				WM_Event_Equipment(WM_EVENT_LOADER_CHEKCK_TRACKOUT, IN_szRFID);
			}
		}
		else if (PtI_L_Un_NG == IN_nPortIndex) // NG 
		{
			if ((enPortStatus::PtS_Empty == nOld_PortStatus) && (enPortStatus::PtS_Exist_Socket == IN_nStatus))
			{
				if (m_pSocketInfo->Is_ExistSocket(IN_szRFID))
				{
					WM_Event_Equipment(WM_EVENT_UNLOAD_NG_INFO, IN_szRFID);
				}
				else
				{
					WM_Event_Equipment(WM_EVENT_UNLOAD_NG_INFO, (LPARAM)0);
				}
			}
			/*else if ((enPortStatus::PtS_Exist_Socket == nOld_PortStatus) && (enPortStatus::PtS_Empty == IN_nStatus))
			{
				if (enPortStatus::PtS_Exist_Socket == m_nPortStatus.at(PtI_L_Un_OK).nStatus)
				{
					if (m_pSocketInfo->Is_ExistSocket(m_nPortStatus.at(PtI_L_Un_OK).szRfid))
					{
						if ((0 == m_pSocketInfo->GetAt(m_nPortStatus.at(PtI_L_Un_OK).szRfid).m_stTestResult.m_nNG_Code) &&
							(enSocketStatus::SoS_Ready != m_pSocketInfo->GetAt(m_nPortStatus.at(PtI_L_Un_OK).szRfid).m_nStatus))
						{
							WM_Event_Equipment(WM_EVENT_UNLOAD_NG_INFO, m_nPortStatus.at(PtI_L_Un_OK).szRfid);
						}
					}
				}
			}*/
		}
		else if (PtI_L_Un_OK == IN_nPortIndex) // OK
		{
			if ((enPortStatus::PtS_Empty == nOld_PortStatus) && (enPortStatus::PtS_Exist_Socket == IN_nStatus))
			{
				if (enPortStatus::PtS_Empty == m_nPortStatus.at(PtI_L_Un_NG).nStatus)
				{
					if (m_pSocketInfo->Is_ExistSocket(IN_szRFID))
					{
						if ((0 == m_pSocketInfo->GetAt(IN_szRFID).m_stTestResult.m_nNG_Code) &&
							(enSocketStatus::SoS_Ready != m_pSocketInfo->GetAt(IN_szRFID).m_nStatus))
						{
							// ?ëÌíà??Í≤ΩÏö∞?êÎßå ?úÏãú (Î∂àÎüâ ?úÌíà??ÏßÄ?òÍ?Í∏??åÎ¨∏)
							WM_Event_Equipment(WM_EVENT_UNLOAD_NG_INFO, IN_szRFID);
						}
					}
				}
			}
			else if ((enPortStatus::PtS_Exist_Socket == nOld_PortStatus) && (enPortStatus::PtS_Empty == IN_nStatus))
			{
				if (enPortStatus::PtS_Empty == m_nPortStatus.at(PtI_L_Un_NG).nStatus)
				{
					// OK, NG Î™®Îëê ?åÏºì???ÜÎã§. READYÎ°??úÏãú
					WM_Event_Equipment(WM_EVENT_UNLOAD_NG_INFO, nullptr);
				}
			}
		}

		// ?åÏºì ?ïÎ≥¥ Í∞±Ïã†
		Update_SocketLocation(IN_nPortIndex, &m_nPortStatus.at(IN_nPortIndex));
	}
}
