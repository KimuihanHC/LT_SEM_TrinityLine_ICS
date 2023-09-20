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

		
		if (PtI_L_Buffer_3 == IN_nPortIndex)
		{			
			// 로더의 버퍼3의 상태가 없음->있음으로 바뀌면 배출 승인 이벤트 처리
// 			if ((enPortStatus::PtS_Empty == nOld_PortStatus) &&
// 				(enPortStatus::PtS_Exist_Socket == IN_nStatus))
			// 로더에서 신호처리가 제대로 안되는 경우가 있어서 수정 (2022.10.17)
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
							// 양품인 경우에만 표시 (불량 제품도 지나가기 때문)
							WM_Event_Equipment(WM_EVENT_UNLOAD_NG_INFO, IN_szRFID);
						}
					}
				}
			}
			else if ((enPortStatus::PtS_Exist_Socket == nOld_PortStatus) && (enPortStatus::PtS_Empty == IN_nStatus))
			{
				if (enPortStatus::PtS_Empty == m_nPortStatus.at(PtI_L_Un_NG).nStatus)
				{
					// OK, NG 모두 소켓이 없다. READY로 표시
					WM_Event_Equipment(WM_EVENT_UNLOAD_NG_INFO, nullptr);
				}
			}
		}

		// 소켓 정보 갱신
		Update_SocketLocation(IN_nPortIndex, &m_nPortStatus.at(IN_nPortIndex));
	}
}
