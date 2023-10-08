//*****************************************************************************
// Filename	: 	TestManager_EQP_ICS.cpp
// Created	:	2021/1/27 - 14:07
// Modified	:	2021/1/27 - 14:07
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#include "stdafx.h"
#include "TestManager_EQP_ICS.h"
#include "Def_Test.h"

#if (USE_XML)
#include "xmlArgs.h"
#include "Util/StringUtil.hpp"
#endif 

#include <sysinfoapi.h>


CTestManager_EQP_ICS::CTestManager_EQP_ICS()
{

}

CTestManager_EQP_ICS::~CTestManager_EQP_ICS()
{

}

//=============================================================================
// Method		: OnLoad_Option
// Access		: virtual protected  
// Returns		: BOOL
// Qualifier	:
// Last Update	: 2021/6/8 - 10:17
// Desc.		:
//=============================================================================
BOOL CTestManager_EQP_ICS::OnLoad_Option()
{
	BOOL bReturn = CTestManager_EQP::OnLoad_Option();



	return bReturn;
}

//=============================================================================
// Method		: OnInit_Devicez
// Access		: virtual protected  
// Returns		: void
// Parameter	: __in HWND hWndOwner
// Qualifier	:
// Last Update	: 2022/8/10 - 10:24
// Desc.		:
//=============================================================================
void CTestManager_EQP_ICS::OnInit_Devicez(__in HWND hWndOwner /*= NULL*/)
{
	CTestManager_EQP::OnInit_Devicez(hWndOwner);
}

//=============================================================================
// Method		: OnMatchingEquipment
// Access		: virtual protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2022/1/12 - 17:37
// Desc.		:
//=============================================================================
void CTestManager_EQP_ICS::OnMatchingEquipment()
{
	auto nEqpCnt = Get_EquipmentCount();
	if (0 < nEqpCnt)
	{
		for (auto nIdx = 0; nIdx < nEqpCnt; ++nIdx)
		{
			switch (Get_EquipmentType(nIdx))
			{
			case enEquipmentType::Eqp_Handler:
			{
 				if (false == m_pIcsComm->CreateRemote((CEqpHandler&)Get_Equipment(nIdx)))
 				{
 					OnLog_Err(_T("CreateRemote(CEqpHandler) failed"));
 				}
			}
			break;

			case enEquipmentType::Eqp_Loader:
			{
				if (false == m_pIcsComm->CreateRemote((CEqpLoader&)Get_Equipment(nIdx)))
				{
					OnLog_Err(_T("CreateRemote(CEqpLoader) failed"));
				}
			}
			break;

			case enEquipmentType::Eqp_Returner:
			{
				if (false == m_pIcsComm->CreateRemote((CEqpReturner&)Get_Equipment(nIdx)))
				{
					OnLog_Err(_T("CreateRemote(CEqpReturner) failed"));
				}
			}
			break;
			default: // Tester
			{
				if (false == m_pIcsComm->CreateRemote((CEqpTester&)Get_Equipment(nIdx)))
				{
					OnLog_Err(_T("CreateRemote(CEqpTester) failed"));
				}
			}
			break;
			}
		}
	}
}


//=============================================================================
// Method		: OnEvent_EquipmentConnection
// Access		: protected  
// Returns		: void
// Parameter	: __in uint8_t IN_FromEqp
// Parameter	: __in bool bConnect
// Qualifier	:
// Last Update	: 2022/1/28 - 17:36
// Desc.		:
//=============================================================================
void CTestManager_EQP_ICS::OnEvent_EquipmentConnection(__in uint8_t IN_FromEqp, __in bool bConnect)
{
	if (COMM_STATUS_CONNECT == Get_Equipment(IN_FromEqp).Get_ClientConnection())
	{
		OnLog(_T("[eqp %02d] tcp/ip connected"), IN_FromEqp);

		// 시간 동기화
		OnSend_TimeSync(IN_FromEqp);

		// 언어 설정
		//OnSend_Language(IN_FromEqp, Get_LineInfo().Get_Language());
		OnSend_Language(IN_FromEqp, m_stOption.Inspector.nLanguage);

		// 모델 정보
		OnSend_Model(IN_FromEqp);
	}
	else
	{
		OnLog(_T("[eqp %02d] tcp/ip disconnected"), IN_FromEqp);
	}
	
}

//=============================================================================
// Method		: OnEvent_AutoMode_Notify
// Access		: protected  
// Returns		: void
// Parameter	: __in uint8_t IN_FromEqp
// Qualifier	:
// Last Update	: 2022/2/15 - 20:28
// Desc.		:
//=============================================================================
void CTestManager_EQP_ICS::OnEvent_AutoMode_Notify(__in uint8_t IN_FromEqp)
{
	//enEqpOperatingMode::EOM_Auto 다른 모드에서 Auto로 변경됨
	OnLog(_T("[eqp %02d] changed to Auto Mode"), IN_FromEqp);

	// 모델 정보
	OnSend_Model(IN_FromEqp);

	// 사용자 정보
	ST_User_Unit stLogin;
	if (m_regInspInfo.Get_LogIn_User(stLogin))
	{
		//OnLog(_T("User Login Info => id: %s, name: %s, level: %d"), stLogin.szID, stLogin.szName, stLogin.nAuthority);

		// 설비들에게 사용자 권한 통보		
		OnSend_UserLevel(IN_FromEqp, m_stInspInfo.PermissionMode, stLogin.szID.GetBuffer());
	}
	else
	{
		//OnLog(_T("User Login Info => Logout"));

		// 설비들에게 사용자 권한 통보
		OnSend_UserLevel(IN_FromEqp, enPermissionMode::Permission_Operator, _T(""));
	}
}

//=============================================================================
// Method		: OnEvent_EquipmentAlarm
// Access		: protected  
// Returns		: void
// Parameter	: __in uint8_t IN_FromEqp
// Parameter	: __in uint8_t IN_nProcessStatus
// Qualifier	:
// Last Update	: 2022/2/16 - 16:42
// Desc.		:
//=============================================================================
void CTestManager_EQP_ICS::OnEvent_EquipmentAlarm(__in uint8_t IN_FromEqp, __in uint8_t IN_nProcessStatus)
{
	// 설비상태가 알람
	if (enEqpProcessStatus::EPC_Alarm == IN_nProcessStatus)
	{
		if (0 < Get_Equipment(IN_FromEqp).Get_AlarmCount())
		{
			OnLog_Err(_T("[eqp %02d] Alarm => code: %d, info: %s"), IN_FromEqp, Get_EqpAlarm(IN_FromEqp).m_nAlarmCode, Get_EqpAlarm(IN_FromEqp).m_szAlarmInfo);

			// file log
			OnLog_EquipmentAlarm(IN_FromEqp, &Get_EqpAlarm(IN_FromEqp));
		}
		else
		{
			OnLog_Err(_T("[eqp %02d] Alarm => eqp status: %d"), IN_FromEqp, IN_nProcessStatus);
		}
	}
	else
	{
		OnLog(_T("[eqp %02d] Alarm Off => eqp status: %d"), IN_FromEqp, IN_nProcessStatus);
	}
}

//=============================================================================
// Method		: OnEvent_Tester_PortStatus
// Access		: protected  
// Returns		: void
// Parameter	: __in uint8_t IN_FromEqp
// Parameter	: __in uint8_t IN_nPortIndex
// Qualifier	:
// Last Update	: 2022/2/17 - 21:26
// Desc.		:
//=============================================================================
void CTestManager_EQP_ICS::OnEvent_Tester_PortStatus(__in uint8_t IN_FromEqp, __in uint8_t IN_nPortIndex)
{
#ifndef USE_FORCED_TRACK_OUT_SEQ
	// 1. 바로 전단계 설비에 배출 대기 중인 소켓이 있나?
// 	if (1 < IN_FromEqp)
// 	{
// 		uint8_t nPrevEqp = IN_FromEqp - 1;
// 
// 		if (enPortStatus::PtS_Wait_Out == Get_EqpPort(nPrevEqp, enPortIndex_Tester::PtI_T_Test_L).nStatus)
// 		{
// 			CString szRFID = Get_EqpPort(nPrevEqp, enPortIndex_Tester::PtI_T_Test_L).szRfid;
// 
// 			Proc_IsPossible_TrackOut(nPrevEqp, szRFID.GetBuffer(), Para_Left);
// 		}
// 
// 		if (enPortStatus::PtS_Wait_Out == Get_EqpPort(nPrevEqp, enPortIndex_Tester::PtI_T_Test_R).nStatus)

	// 2. 이전 설비들 중에서 목적 설비가 현재 설비인 소켓이 있는가? (배출 대기 중인) ?
	std::vector<CString> nSocketz;
	uint8_t nSocketCount = Get_LineInfo().Get_FindSocketz_SameTarget(IN_FromEqp, nSocketz);
	bool bExist = false;

	for (auto nSockIdx = 0; nSockIdx < nSocketCount; ++nSockIdx)
	{
		bExist = false;
		for (uint8_t nPortIdx = enPortIndex_Tester::PtI_T_Test_L; nPortIdx < Get_Equipment(IN_FromEqp).Get_PortCount(); ++nPortIdx)
		{
			if ((Get_Socket(nSocketz.at(nSockIdx)).m_nPort_inEqp == nPortIdx))
			{
				bExist = true;
				break;
			}
		}

		// 소켓이 검사 파라에 있다면?
		/*if ((Get_Socket(nSocketz.at(nSockIdx)).m_nPort_inEqp == enPortIndex_Tester::PtI_T_Test_L) ||
			(Get_Socket(nSocketz.at(nSockIdx)).m_nPort_inEqp == enPortIndex_Tester::PtI_T_Test_R))*/
		if (bExist)
		{
			// 배출 가능 여부?
			if (Proc_IsPossible_TrackOut_byEvent(Get_Socket(nSocketz.at(nSockIdx)).m_nEqpOrder, nSocketz.at(nSockIdx).GetBuffer()))
			{
				// 서버->설비로 배출 승인 여부 통보
				OnSend_Accept_SocketTrackOut(Get_Socket(nSocketz.at(nSockIdx)).m_nEqpOrder, nSocketz.at(nSockIdx).GetBuffer());

				OnLog(_T("[eqp %02d] possible track out => rfid: %s (OnEvent_Tester_PortStatus)"), Get_Socket(nSocketz.at(nSockIdx)).m_nEqpOrder, nSocketz.at(nSockIdx).GetBuffer());
				OnLog_SocketEvent(nSocketz.at(nSockIdx).GetBuffer(), _T("[eqp %02d] possible track out => rfid: %s (Event Port)"), 
																Get_Socket(nSocketz.at(nSockIdx)).m_nEqpOrder, 
																nSocketz.at(nSockIdx).GetBuffer());
			}
		}
	}
#endif // USE_FORCED_TRACK_OUT_SEQ

	// 로더 다음 설비이면?
	//if (1 == IN_FromEqp)
	if ((Get_Loader().Get_EqpOrder() + 1) == IN_FromEqp)
	{
		// 로더에서 배출 대기인가?
		if (enPortStatus::PtS_Wait_Out == Get_Loader().Get_PortStatus(enPortIndex_Loader::PtI_L_Buffer_3).nStatus)
		{
			CString szRFID = Get_Loader().Get_PortStatus(enPortIndex_Loader::PtI_L_Buffer_3).szRfid;

			if (Proc_IsPossible_TrackOut_byLoader(0, szRFID.GetBuffer()))
			{
				// 서버->설비로 배출 승인 여부 통보
				OnSend_Accept_SocketTrackOut(Get_Loader().Get_EqpOrder(), szRFID.GetBuffer());

				OnLog(_T("[Loader] possible track out => rfid: %s (OnEvent_Tester_PortStatus)"), szRFID.GetBuffer());
				OnLog_SocketEvent(szRFID.GetBuffer(), _T("[Loader] possible track out => rfid: %s (Event Port)"), szRFID.GetBuffer());
			}
		}
	}
}

//=============================================================================
// Method		: OnEvent_Tester_CoveyorStatus
// Access		: protected  
// Returns		: void
// Parameter	: __in uint8_t IN_FromEqp
// Parameter	: __in uint8_t IN_nConveyorIndex
// Qualifier	:
// Last Update	: 2022/2/17 - 21:26
// Desc.		:
//=============================================================================
void CTestManager_EQP_ICS::OnEvent_Tester_CoveyorStatus(__in uint8_t IN_FromEqp, __in uint8_t IN_nConveyorIndex)
{
#ifndef USE_FORCED_TRACK_OUT_SEQ
	// 이전 설비에 배출 대기 중인 소켓이 있나?
	std::vector<CString> nSocketz;
	uint8_t nSocketCount = Get_LineInfo().Get_FindSocketz_SameTarget(IN_FromEqp, nSocketz);
	bool bExist = false;

	for (auto nSockIdx = 0; nSockIdx < nSocketCount; ++nSockIdx)
	{
		bExist = false;
		for (uint8_t nPortIdx = enPortIndex_Tester::PtI_T_Test_L; nPortIdx < Get_Equipment(IN_FromEqp).Get_PortCount(); ++nPortIdx)
		{
			if ((Get_Socket(nSocketz.at(nSockIdx)).m_nPort_inEqp == nPortIdx))
			{
				bExist = true;
				break;
			}
		}

		/*if ((Get_Socket(nSocketz.at(nSockIdx)).m_nPort_inEqp == enPortIndex_Tester::PtI_T_Test_L) ||
			(Get_Socket(nSocketz.at(nSockIdx)).m_nPort_inEqp == enPortIndex_Tester::PtI_T_Test_R))*/
		if (bExist)
		{
			// 배출 가능 여부?
			if (Proc_IsPossible_TrackOut_byEvent(Get_Socket(nSocketz.at(nSockIdx)).m_nEqpOrder, nSocketz.at(nSockIdx).GetBuffer()))
			{
				// 서버->설비로 배출 승인 여부 통보
				OnSend_Accept_SocketTrackOut(Get_Socket(nSocketz.at(nSockIdx)).m_nEqpOrder, nSocketz.at(nSockIdx).GetBuffer());

				OnLog(_T("[eqp %02d] possible track out => rfid: %s (OnEvent_Tester_CoveyorStatus)"), Get_Socket(nSocketz.at(nSockIdx)).m_nEqpOrder, nSocketz.at(nSockIdx).GetBuffer());
				OnLog_SocketEvent(nSocketz.at(nSockIdx).GetBuffer(), _T("[eqp %02d] possible track out => rfid: %s (Event Conveyor)"),
																Get_Socket(nSocketz.at(nSockIdx)).m_nEqpOrder,
																nSocketz.at(nSockIdx).GetBuffer());
			}
		}
	}
#endif // USE_FORCED_TRACK_OUT_SEQ

	//if (1 == IN_FromEqp)
	if ((Get_Loader().Get_EqpOrder() + 1) == IN_FromEqp)
	{
		// 로더에서 배출 대기인가?
		if (enPortStatus::PtS_Wait_Out == Get_Loader().Get_PortStatus(enPortIndex_Loader::PtI_L_Buffer_3).nStatus)
		{
			CString szRFID = Get_Loader().Get_PortStatus(enPortIndex_Loader::PtI_L_Buffer_3).szRfid;

			if (Proc_IsPossible_TrackOut_byLoader(0, szRFID.GetBuffer()))
			{
				// 서버->설비로 배출 승인 여부 통보
				OnSend_Accept_SocketTrackOut(Get_Loader().Get_EqpOrder(), szRFID.GetBuffer());

				OnLog(_T("[Loader] possible track out => rfid: %s (OnEvent_Tester_CoveyorStatus)"), szRFID.GetBuffer());
				OnLog_SocketEvent(szRFID.GetBuffer(), _T("[Loader] possible track out => rfid: %s (Event Conveyor)"), szRFID.GetBuffer());
			}
		}
	}
}

//=============================================================================
// Method		: OnEvent_Eqp_EndOfProduction
// Access		: protected  
// Returns		: void
// Parameter	: __in uint8_t IN_FromEqp
// Qualifier	:
// Last Update	: 2022/2/21 - 16:48
// Desc.		:
//=============================================================================
void CTestManager_EQP_ICS::OnEvent_Eqp_EndOfProduction(__in uint8_t IN_FromEqp)
{

}

//=============================================================================
// Method		: OnEvent_CheckEmptyEquipment
// Access		: protected  
// Returns		: void
// Parameter	: __in uint8_t IN_FromEqp
// Qualifier	:
// Last Update	: 2022/4/12 - 14:54
// Desc.		:
//=============================================================================
void CTestManager_EQP_ICS::OnEvent_CheckEmptyEquipment(__in uint8_t IN_FromEqp)
{
	// 이전 설비에 제품이 없다. (기본: 2 설비 체크)
	static const uint8_t nCheckEqpCount = 2;


	// 테스터
	if (Get_Equipment(IN_FromEqp).Is_Tester())
	{
		OnLog(_T("[eqp %02d] Check the empty state of equipment !!"), IN_FromEqp);

		// 현재 검사 설비가 한쪽 파라에서만 배출 대기 중인가?
		if (Get_Equipment(IN_FromEqp).IsLastSocket_onTestPort())
		{
			OnLog(_T("[eqp %02d] The last socket is in the equipment's inspection para!!"), IN_FromEqp);
			// 현재 설비에서 로더 방향으로 이동하면서
			// 설비가 비어 있나 체크 한다.

			// 전 설비가 Loader 이면 1, 아니면 2
			uint8_t nCheckCount = __min(IN_FromEqp, nCheckEqpCount);
			uint8_t nEmptyCount = 0;

			for (int8_t nEqpIdx = (IN_FromEqp - 1); 0 <= nEqpIdx; --nEqpIdx)
			{
				if (Get_Equipment(nEqpIdx).IsEmpty_Equipment())
				{
					++nEmptyCount;
				}
				else if (Get_Equipment(nEqpIdx).Is_Tester() && Get_Equipment(nEqpIdx).IsLastSocket_onTestPort())
				{
					//OnEvent_CheckEmptyEquipment(nEqpIdx);					
					++nEmptyCount;
				}
				else
				{
					// 비어 있지 않다.. 체크 끝
					break;
				}

				if (nCheckCount <= nEmptyCount)
					break;
			}

			if (nCheckCount <= nEmptyCount)
			{
				// 현재 설비로 생산 종료 메세지 전송
				OnSend_EndOfProduction(IN_FromEqp, true);
			}
		}
 		else if (Get_Equipment(IN_FromEqp).IsEmpty_Equipment())
 		{
 			OnLog(_T("[eqp %02d] Equipment is Empty!!"), IN_FromEqp);
 		}
	}
}

//=============================================================================
// Method		: OnEvent_Loader_RegisterSocket
// Access		: protected  
// Returns		: void
// Parameter	: __in LPCTSTR IN_szRFID
// Qualifier	:
// Last Update	: 2022/2/3 - 14:27
// Desc.		:
//=============================================================================
void CTestManager_EQP_ICS::OnEvent_Loader_RegisterSocket(__in LPCTSTR IN_szRFID)
{
	// log (소켓, 제품 바코드)
	OnLog(_T("[Loader] Register Socket => rfid: %s,  barcode: %s, socket type: %s"),
		IN_szRFID,
		Get_SocketBarcode(IN_szRFID).GetBuffer(),
		g_szSocketTypeName[Get_SocketType(IN_szRFID)]);

	OnLog_SocketEvent(IN_szRFID, _T("[Loader] Register Socket => rfid: %s,  barcode: %s, socket type: %s"),
		IN_szRFID,
		Get_SocketBarcode(IN_szRFID).GetBuffer(),
		g_szSocketTypeName[Get_SocketType(IN_szRFID)]);

	OnLog_RegisterSocket(IN_szRFID);
}

//=============================================================================
// Method		: OnEvent_Loader_CheckTrackOut
// Access		: protected  
// Returns		: void
// Parameter	: __in LPCTSTR IN_szRFID
// Qualifier	:
// Last Update	: 2022/2/4 - 16:16
// Desc.		:
//=============================================================================
void CTestManager_EQP_ICS::OnEvent_Loader_CheckTrackOut(__in LPCTSTR IN_szRFID)
{
	// 로더 투입전 버퍼3의 상태가 없음 -> 제품 있음으로 바뀌면, 배출 승인을 알려준다.
	OnLog(_T("[Loader] Check Track Out => rfid: %s"),IN_szRFID);
	OnLog_SocketEvent(IN_szRFID, _T("[Loader] Check Track Out => rfid: %s"), IN_szRFID);

	// 목적 설비 설정
	if (false == Proc_SetTagetEquipment(Get_Loader().Get_EqpOrder(), IN_szRFID))
	{
		;
	}

	// 배출 가능 여부 ?
	if (Proc_IsPossible_TrackOut_byLoader(Get_Loader().Get_EqpOrder(), IN_szRFID))
	{
		// 서버->설비로 배출 승인 여부 통보
		if ((nullptr != IN_szRFID) && (0 != _tcsclen(IN_szRFID)))
		{
			OnSend_Accept_SocketTrackOut(Get_Loader().Get_EqpOrder(), IN_szRFID);
		}
		else
		{
			OnSend_Accept_SocketTrackOut(Get_Loader().Get_EqpOrder(), _T(""));
		}

		OnLog(_T("[Loader] possible track out => rfid: %s (OnEvent_Loader_CheckTrackOut)"), IN_szRFID);
		OnLog_SocketEvent(IN_szRFID, _T("[Loader] possible track out => rfid: %s (Event Loader Check Track Out)"), IN_szRFID);
	}
	else
	{
		// 배출 불가
#ifdef USE_DEBUG_MODE
		if (Get_DebugInfo().Is_NotBypass_NG())
		{
			if ((nullptr != IN_szRFID) && (0 != _tcsclen(IN_szRFID)))
			{
				OnSend_Accept_SocketTrackOut(Get_Loader().Get_EqpOrder(), IN_szRFID);
			}
			else
			{
				OnSend_Accept_SocketTrackOut(Get_Loader().Get_EqpOrder(), _T(""));
			}
		}
		else
		{
			OnLog(_T("[Loader] impossible track out => rfid: %s (OnEvent_Loader_CheckTrackOut)"), IN_szRFID);
			OnLog_SocketEvent(IN_szRFID, _T("[Loader] impossible track out => rfid: %s (Event Loader Check Track Out)"), IN_szRFID);
		}
#else // USE_DEBUG_MODE
		OnLog(_T("[Loader] impossible track out => rfid: %s (OnEvent_Loader_CheckTrackOut)"), IN_szRFID);
		OnLog_SocketEvent(IN_szRFID, _T("[Loader] impossible track out => rfid: %s (Event Loader Check Track Out)"), IN_szRFID);
#endif // USE_DEBUG_MODE
	}
	
}

//=============================================================================
// Method		: OnEvent_Loader_TrackOut
// Access		: protected  
// Returns		: void
// Parameter	: __in LPCTSTR IN_szRFID
// Qualifier	:
// Last Update	: 2022/1/28 - 16:59
// Desc.		:
//=============================================================================
void CTestManager_EQP_ICS::OnEvent_Loader_TrackOut(__in LPCTSTR IN_szRFID)
{
	OnLog(_T("[Loader] Track Out => rfid: %s"), IN_szRFID);
	OnLog_SocketEvent(IN_szRFID, _T("[Loader] Track Out => rfid: %s"), IN_szRFID);

}

//=============================================================================
// Method		: OnEvent_Tester_TrackIn
// Access		: protected  
// Returns		: void
// Parameter	: __in uint8_t IN_FromEqp
// Parameter	: __in LPCTSTR IN_szRFID
// Qualifier	:
// Last Update	: 2022/1/28 - 16:56
// Desc.		:
//=============================================================================
void CTestManager_EQP_ICS::OnEvent_Tester_TrackIn(__in uint8_t IN_FromEqp, __in LPCTSTR IN_szRFID)
{
	// 검사 설비에 소켓이 도착하여, RFID로 투입 승인 요청
	OnLog(_T("[eqp %02d] Track In (Request Socket) => rfid: %s"), IN_FromEqp, IN_szRFID);
	OnLog_SocketEvent(IN_szRFID, _T("[eqp %02d] Track In (Request Socket) => rfid: %s"), IN_FromEqp, IN_szRFID);

	// 0: Error
	// 1: 승인
	// 2: 승인 (마지막 소켓)
	// 3: Bypass


	// 설비가 알람?


	// 설비가 Skip?


#ifdef USE_TARGET_EQP_DISTANCE
	// 설비가 그룹 내 첫 설비 이고, 소켓이 far away 상태?  => 그룹 내 목적지 재 설정
	if (Get_Socket(IN_szRFID).Get_FarAway_TargetEqp())
	{
		if (Get_LineInfo().Is_FirstEquipment_inGroup(IN_FromEqp))
		{
			// 예약 1 감소
			Get_Equipment(IN_FromEqp).Decrease_ReservedPort();

			// 목적 설비 재 설정
			Proc_SetTagetEquipment_Group(IN_szRFID, Get_EquipmentType(IN_FromEqp));
		}
	}
#endif


	Get_Equipment(IN_FromEqp);
	Get_Socket(IN_szRFID);

	// 소켓 상태 판단
	uint8_t nFlag = enAcceptSocket::AS_Error;
	if (Get_SocketInfo().Is_ExistSocket(IN_szRFID))
	{
		switch (Get_Socket(IN_szRFID).m_nStatus)
		{
		case enSocketStatus::SoS_Ready:
		case enSocketStatus::SoS_Pass:
			// 소켓의 목적 설비 체크 (현재 설비와 목적 설비가 같은가?)
			if (IN_FromEqp == Get_Socket(IN_szRFID).m_nTargetEqpOrder)
			{
				// 이 설비에서 검사 진행
				nFlag = enAcceptSocket::AS_Accept;
			}
			else
			{
				// 다음 설비에서 검사 진행 : Bypass
#ifdef USE_DEBUG_MODE
				if (Get_DebugInfo().Is_NotBypass_TestSkip())
				{
					nFlag = enAcceptSocket::AS_Accept;
				}
				else if ((Get_Equipment(IN_FromEqp).Get_Group()) &&
					(Get_Equipment(IN_FromEqp).Get_EquipmentType() == Get_Socket(IN_szRFID).m_stTestResult.m_nNG_EqpType))
				{
					nFlag = enAcceptSocket::AS_Bypass_SameTest;
				}	
				else
				{
					nFlag = enAcceptSocket::AS_Bypass;
				}
#else
				if ((Get_Equipment(IN_FromEqp).Get_Group()) &&
					(Get_Equipment(IN_FromEqp).Get_EquipmentType() == Get_Socket(IN_szRFID).m_stTestResult.m_nNG_EqpType))
				{
					nFlag = enAcceptSocket::AS_Bypass_SameTest;
				}
				else
				{
					nFlag = enAcceptSocket::AS_Bypass;
				}
#endif
			}
			break;

		case enSocketStatus::SoS_Fail:
		case enSocketStatus::SoS_Bypass:
			{
				// 목적 설비가 Returner
#ifdef USE_DEBUG_MODE
				if (Get_DebugInfo().Is_NotBypass_TestSkip())
				{
					nFlag = enAcceptSocket::AS_Accept;
				}
				else
				{
					nFlag = enAcceptSocket::AS_Bypass;
				}
#else
				nFlag = enAcceptSocket::AS_Bypass;
#endif
			}
			break;

		case enSocketStatus::SoS_Error:
			nFlag = enAcceptSocket::AS_Error;
			break;

		}
	}

	// 서버->설비로 투입 승인 여부 통보
	OnSend_Accept_SocketTrackIn(IN_FromEqp, IN_szRFID, nFlag, Get_SocketBarcode(IN_szRFID).GetBuffer());

	OnLog(_T("[eqp %02d] Accept Socket Track In => rfid: %s, flag: %d (%s)"), IN_FromEqp, IN_szRFID, nFlag, g_szAcceptSocket[nFlag]);
	OnLog_SocketEvent(IN_szRFID, _T("[eqp %02d] Accept Socket Track In => rfid: %s, flag: %d (%s)"), IN_FromEqp, IN_szRFID, nFlag, g_szAcceptSocket[nFlag]);
}

//=============================================================================
// Method		: OnEvent_Tester_EndInspection
// Access		: protected  
// Returns		: void
// Parameter	: __in uint8_t IN_FromEqp
// Parameter	: __in LPCTSTR IN_szRFID
// Qualifier	:
// Last Update	: 2022/2/10 - 15:18
// Desc.		:
//=============================================================================
void CTestManager_EQP_ICS::OnEvent_Tester_EndInspection(__in uint8_t IN_FromEqp, __in LPCTSTR IN_szRFID)
{
	// 검사 설비에서 검사 결과 통지
	OnLog(_T("[eqp %02d] End Inspection (Notify Test Result) => rfid: %s"), IN_FromEqp, IN_szRFID);
	OnLog_SocketEvent(IN_szRFID, _T("[eqp %02d] End Inspection (Notify Test Result) => rfid: %s"), IN_FromEqp, IN_szRFID);

	// 검사 결과 출력
	OnLog_SocketTestResult(IN_FromEqp, IN_szRFID);

	// 목적 설비 선택
	if (false == Proc_SetTagetEquipment(IN_FromEqp, IN_szRFID))
	{
		;
	}


	// 배출 가능 여부 판단
	if (Proc_IsPossible_TrackOut(IN_FromEqp, IN_szRFID, Get_SocketTestResult(IN_szRFID).m_nNG_Para))
	{
		// 서버->설비로 배출 승인 여부 통보
		OnSend_Accept_SocketTrackOut(IN_FromEqp, IN_szRFID);

		OnLog(_T("[eqp %02d] possible track out => rfid: %s (OnEvent_Tester_EndInspection)"), IN_FromEqp, IN_szRFID);
		OnLog_SocketEvent(IN_szRFID, _T("[eqp %02d] possible track out => rfid: %s (Event End Inspection)"), IN_FromEqp, IN_szRFID);
	}
	else
	{
#ifdef USE_DEBUG_MODE
		if (Get_DebugInfo().Is_NotBypass_NG())
		{
			OnSend_Accept_SocketTrackOut(IN_FromEqp, IN_szRFID);
		}
		else
		{
#ifdef USE_FORCED_TRACK_OUT_SEQ
			// 현재 문제가 있어서 배출 승인을 무조건 보낸다.
			OnSend_Accept_SocketTrackOut(IN_FromEqp, IN_szRFID);
#endif // USE_FORCED_TRACK_OUT_SEQ

			OnLog(_T("[eqp %02d] impossible track out => rfid: %s (OnEvent_Tester_EndInspection)"), IN_FromEqp, IN_szRFID);
			OnLog_SocketEvent(IN_szRFID, _T("[eqp %02d] impossible track out => rfid: %s (Event End Inspection)"), IN_FromEqp, IN_szRFID);
		}
#else // USE_DEBUG_MODE

#ifdef USE_FORCED_TRACK_OUT_SEQ
		// 현재 문제가 있어서 배출 승인을 무조건 보낸다.
		OnSend_Accept_SocketTrackOut(IN_FromEqp, IN_szRFID);
#endif // USE_FORCED_TRACK_OUT_SEQ

		OnLog(_T("[eqp %02d] impossible track out => rfid: %s (OnEvent_Tester_EndInspection)"), IN_FromEqp, IN_szRFID);
		OnLog_SocketEvent(IN_szRFID, _T("[eqp %02d] impossible track out => rfid: %s (Event End Inspection)"), IN_FromEqp, IN_szRFID);
#endif // USE_DEBUG_MODE

	}
}

//=============================================================================
// Method		: OnEvent_Unloader_TrackIn
// Access		: protected  
// Returns		: void
// Parameter	: __in LPCTSTR IN_szRFID
// Qualifier	:
// Last Update	: 2022/2/15 - 19:48
// Desc.		:
//=============================================================================
void CTestManager_EQP_ICS::OnEvent_Unloader_TrackIn(__in LPCTSTR IN_szRFID)
{
	OnLog(_T("[Unload] Track In (Request Test Result) => rfid: %s"), IN_szRFID);
	OnLog_SocketEvent(IN_szRFID, _T("[Unload] Track In (Request Test Result) => rfid: %s"), IN_szRFID);

	// Tacttime 체크
	double dTacttime = m_stInspInfo.Tacttime.Set_UnloadTime(&Get_Socket(IN_szRFID).m_tmUnload);

	Get_Socket(IN_szRFID).Set_Tacttime(dTacttime);

	OnReport_Socket(IN_szRFID);
}

//=============================================================================
// Method		: OnEvent_Unloader_NG_Info
// Access		: protected  
// Returns		: void
// Parameter	: __in LPCTSTR IN_szRFID
// Qualifier	:
// Last Update	: 2022/4/12 - 11:53
// Desc.		:
//=============================================================================
void CTestManager_EQP_ICS::OnEvent_Unloader_NG_Info(__in LPCTSTR IN_szRFID)
{
	OnReport_Socket_NG(IN_szRFID);
}

//=============================================================================
// Method		: OnEvent_Unloader_UnregisterSocket
// Access		: protected  
// Returns		: void
// Parameter	: __in LPCTSTR IN_szRFID
// Qualifier	:
// Last Update	: 2022/2/9 - 14:46
// Desc.		:
//=============================================================================
void CTestManager_EQP_ICS::OnEvent_Unloader_UnregisterSocket(__in LPCTSTR IN_szRFID)
{
	OnLog(_T("[Unload] Unregister Socket => rfid: %s"), IN_szRFID);
	OnLog_SocketEvent(IN_szRFID, _T("[Unload] Unregister Socket => rfid: %s"), IN_szRFID);

	// 설비 기준 Log 파일 작성

	// 소켓 최종 수율 업데이트

	// 소켓 Log 파일 작성

	// 제품이 불량인가? (불량이 발생한 검사 설비 ?)

	// Test Result 초기화
	Get_SocketInfo().Reset_RegisterData(IN_szRFID);
	
}





//=============================================================================
// Method		: Proc_SetSocketTargetEquipment
// Access		: protected  
// Returns		: void
// Parameter	: __in LPCTSTR IN_szRFID
// Parameter	: __in uint8_t IN_nTargetEqpOrder
// Qualifier	:
// Last Update	: 2022/5/13 - 17:46
// Desc.		:
//=============================================================================
void CTestManager_EQP_ICS::Proc_SetSocketTargetEquipment(__in LPCTSTR IN_szRFID, __in uint8_t IN_nTargetEqpOrder)
{
	// 소켓의 목적 설비 설정
	Get_SocketInfo().Set_SocketTarget(IN_szRFID, IN_nTargetEqpOrder, Get_EquipmentID(IN_nTargetEqpOrder).GetBuffer());

	// *** 소켓의 목적 설비를 설정하면 포트 사용 예약을 설정해야 한다.
	if (false == Get_Equipment(IN_nTargetEqpOrder).Increase_ReservedPort())
	{
		;
	}
}

//=============================================================================
// Method		: Proc_SetTagetEquipment_Group
// Access		: protected  
// Returns		: void
// Parameter	: __in LPCTSTR IN_szRFID
// Parameter	: __in uint8_t IN_FromEqp
// Qualifier	:
// Last Update	: 2022/6/3 - 18:02
// Desc.		:
//=============================================================================
void CTestManager_EQP_ICS::Proc_SetTagetEquipment_Group(__in LPCTSTR IN_szRFID, __in uint8_t IN_FromEqp)
{
	uint8_t IN_nTargetEqpOrder = Get_LineInfo().Get_TargetEquipment_inGroup((enEquipmentType)Get_Equipment(IN_FromEqp).Get_EquipmentType());

	Get_SocketInfo().Set_SocketTarget(IN_szRFID, IN_nTargetEqpOrder, Get_EquipmentID(IN_nTargetEqpOrder).GetBuffer());
}

//=============================================================================
// Method		: Proc_SetTagetEquipment
// Access		: protected  
// Returns		: bool
// Parameter	: __in uint8_t IN_FromEqp
// Parameter	: __in LPCTSTR IN_szRFID
// Qualifier	:
// Last Update	: 2022/2/4 - 16:51
// Desc.		:
//=============================================================================
bool CTestManager_EQP_ICS::Proc_SetTagetEquipment(__in uint8_t IN_FromEqp, __in LPCTSTR IN_szRFID)
{
	// 목적 설비 설정
	uint8_t nTargetEqpOrder = 0;
	if (0 == Get_SocketTestResult(IN_szRFID).m_nNG_Code)
	{	// 양품 : 다음 목적 설비 구하기
		nTargetEqpOrder = Get_LineInfo().Get_TargetEquipment(IN_FromEqp);
	}
	else if (Get_SocketInfo().Get_MES_ReworkCode() == Get_SocketTestResult(IN_szRFID).m_nNG_Code)// MES Rework NG Code 이면...???
	{
		nTargetEqpOrder = Get_LineInfo().Get_TargetEquipment(IN_FromEqp);
	}
	else
	{	// 불량 : 리터너
#ifdef USE_DEBUG_MODE
		if (Get_DebugInfo().Is_NotBypass_NG())
		{
			nTargetEqpOrder = Get_LineInfo().Get_TargetEquipment(IN_FromEqp);
		}
		else
		{
			// 불량 : Returner
			nTargetEqpOrder = Get_Returner().Get_EqpOrder();
		}
#else // USE_DEBUG_MODE
		// 불량 : Returner
		nTargetEqpOrder = Get_Returner().Get_EqpOrder();
#endif // USE_DEBUG_MODE
	}

	// *** nTargetEqpOrder이 0 이면 오류이다.
	if (0 == nTargetEqpOrder)
	{
		//if (0 == IN_FromEqp)
		if (Get_Loader().Get_EqpOrder() == IN_FromEqp)
		{
			OnLog_Err(_T("[Loader] Error! Set Target Eqp => rfid: %s, target: [eqp %02d]"), IN_szRFID, nTargetEqpOrder);
		}
		else
		{
			OnLog_Err(_T("[eqp %02d] Error! Set Target Eqp => rfid: %s, target: [eqp %02d]"), IN_FromEqp, IN_szRFID, nTargetEqpOrder);
		}

		return false;
	}

#ifdef USE_TARGET_EQP_DISTANCE
	// IN_FromEqp ---> nTargetEqpOrder  거리 측정   (Check_TargetDistance)
	bool bFar = false;
	if (Get_Equipment(nTargetEqpOrder).Get_Group())
	{
		uint8_t nEqpType = Get_Equipment(nTargetEqpOrder).Get_EquipmentType();
		uint8_t nTestTarget_1stEqp = Get_LineInfo().Get_FirstEquipment((enEquipmentType)nEqpType).Get_EqpOrder();

		bFar = Proc_IsFarAway_TargetEqp(IN_FromEqp, nTestTarget_1stEqp);
	}

	Get_Socket(IN_szRFID).Set_FarAway_TargetEqp(bFar); // 소켓에 설정
#endif // USE_TARGET_EQP_DISTANCE



	// 소켓의 목적 설비 설정
	Proc_SetSocketTargetEquipment(IN_szRFID, nTargetEqpOrder);

	// 목적 설비의 포트 상태 표시
	OnLog(_T("[eqp %02d] Target Equipment(eqp %02d) Status => port: %d, using: %d, empty: %d, reserved: %d, reserved_over: %d"),
		IN_FromEqp,
		nTargetEqpOrder,
		Get_Equipment(nTargetEqpOrder).Get_PortCount(),
		//Get_Equipment(nTargetEqpOrder).Check_AvablePortCnt(),
		Get_Equipment(nTargetEqpOrder).Get_UsingPortCount(),
		Get_Equipment(nTargetEqpOrder).Get_EmptyPortCount(),
		Get_Equipment(nTargetEqpOrder).Get_ReservedPortCnt(),
		Get_Equipment(nTargetEqpOrder).Get_ReservedOverCnt()
	);


	//if (0 == IN_FromEqp)
	if (Get_Loader().Get_EqpOrder() == IN_FromEqp)
	{
		OnLog(_T("[Loader] Set Target Eqp => rfid: %s, target: [eqp %02d]"), IN_szRFID, nTargetEqpOrder);
	}
	else
	{
		OnLog(_T("[eqp %02d] Set Target Eqp => rfid: %s, target: [eqp %02d]"), IN_FromEqp, IN_szRFID, nTargetEqpOrder);
	}
	OnLog_SocketEvent(IN_szRFID, _T("[eqp %02d] Set Target Eqp => rfid: %s, target: [eqp %02d]"), IN_FromEqp, IN_szRFID, nTargetEqpOrder);

	return true;
}

//=============================================================================
// Method		: Proc_IsPossible_TrackOut_byLoader
// Access		: protected  
// Returns		: bool
// Parameter	: __in uint8_t IN_FromEqp
// Parameter	: __in LPCTSTR IN_szRFID
// Qualifier	:
// Last Update	: 2022/2/4 - 16:51
// Desc.		:
//=============================================================================
bool CTestManager_EQP_ICS::Proc_IsPossible_TrackOut_byLoader(__in uint8_t IN_FromEqp, __in LPCTSTR IN_szRFID)
{
#ifdef USE_DEBUG_MODE
	if (Get_DebugInfo().Is_NotBypass_TestSkip())
	{
		return true;
	}
#endif // USE_DEBUG_MODE

	// 다음 설비 : Bypass
	// 다음 설비 : 검사 진행

	// 해당 소켓의 타겟 설비 (IN_FromEqp < TargetEqp)

	
	// 1. 목적 설비로 소켓 배출 가능 ?
	// => 사용 중 및 예약 체크
	if (Get_Socket(IN_szRFID).m_nTargetEqpOrder < Get_EquipmentCount())
	{
		// 검사 가능
		uint8_t nStatus = Get_Equipment(Get_SocketTarget(IN_szRFID)).Get_InputAvailabilityStatus();
		if (enInputAvailabilityStatus::IAS_Test != nStatus)
		{
			OnLog(_T("[eqp %02d] IsPossible_TrackOut_byLoader() => staus: %d, target eqp: %02d, not testable"), IN_FromEqp, nStatus, Get_SocketTarget(IN_szRFID));

			return false;
		}
	}

	// 2. 현재 설비의 바로 다음에 있는 설비로 소켓 배출 가능 ? (bypass로 지나 갈 경우에만)
	// => rfid 사용 여부 체크
	Get_EquipmentCount();
	uint8_t nNextEqp = IN_FromEqp + 1;
	if (nNextEqp < Get_EquipmentCount())
	{
		// 컨베이어 상태 체크, RFID 상태 체크
		uint8_t nStatus = Get_Equipment(nNextEqp).Get_InputAvailabilityStatus();
		if (enInputAvailabilityStatus::IAS_NoInput == nStatus)
		{
			OnLog(_T("[eqp %02d] IsPossible_TrackOut_byLoader() => staus: %d, next eqp: %02d, Impossible to put into the next equipment"), IN_FromEqp, nStatus, nNextEqp);

			return false;
		}

		// RFID 포트 체크
// 		if (enPortStatus::PtS_Empty != Get_EqpPort(nNextEqp, enPortIndex_Tester::PtI_T_RFID).nStatus)
// 		{
// 			return false;
// 		}
	}

	return true;
}

bool CTestManager_EQP_ICS::Proc_IsPossible_TrackOut(__in uint8_t IN_FromEqp, __in LPCTSTR IN_szRFID, __in uint8_t IN_nPara)
{
#ifdef USE_DEBUG_MODE
	if (Get_DebugInfo().Is_NotBypass_TestSkip())
	{
		return true;
	}
#endif // USE_DEBUG_MODE

	// 테스터 : 
	// 	enInputAvailabilityStatus::IAS_NoInput;		// 전체 사용 불가	
	// 	enInputAvailabilityStatus::IAS_Bypass;		// Bypass 가능
	// 	enInputAvailabilityStatus::IAS_Test;		// 검사 가능	

	// 다음 설비 : Bypass
	// 다음 설비 : 검사 진행

	// 해당 소켓의 타겟 설비 (IN_FromEqp < TargetEqp)
	// 1. 목적 설비로 소켓 배출 가능 ?
	if (Get_Returner().Get_EqpOrder() == Get_SocketTarget(IN_szRFID))
	{
		// 목적지가 Returner (Bypas)

	}
	else
	{
		// 검사 가능 체크
		uint8_t nStatus = Get_Equipment(Get_SocketTarget(IN_szRFID)).Get_InputAvailabilityStatus();
		if (enInputAvailabilityStatus::IAS_Test != nStatus)
		{
			OnLog(_T("[eqp %02d] IsPossible_TrackOut_byLoader() => staus: %d, target eqp: %02d, not testable"), IN_FromEqp, nStatus, Get_SocketTarget(IN_szRFID));

			return false;
		}
	}

	// 2. 현재 설비의 바로 다음에 있는 설비로 소켓 배출 가능 ? (bypass로 지나 갈 경우에만)
	// => rfid 사용 여부 체크
	Get_EquipmentCount();
	uint8_t nNextEqp = IN_FromEqp + 1;
	if (nNextEqp < Get_EquipmentCount())
	{
		// 다음 설비가 리터너이면 배출
		if (Get_Returner().Get_EqpOrder() != nNextEqp)
		{
			// 컨베이어 상태 체크, RFID 상태 체크
			uint8_t nStatus = Get_Equipment(nNextEqp).Get_InputAvailabilityStatus();
			if (enInputAvailabilityStatus::IAS_NoInput == nStatus)
			{
				OnLog(_T("[eqp %02d] IsPossible_TrackOut_byLoader() => staus: %d, next eqp: %02d, Impossible to put into the next equipment"), IN_FromEqp, nStatus, nNextEqp);

				return false;
			}
		}
	}





/*
	// 소켓 배출 가능 여부 판단 (pass / fail)
	if (0 != Get_Socket(IN_szRFID).m_stTestResult.m_nNG_Code)
	{
		// Bypass -> 목적지 : 리터너



		// 현재 설비의 다른 Para 상태 체크
// 		uint8_t nOtherParaIndex = (IN_nPara == Para_Left) ? PtI_T_Test_R : PtI_T_Test_L;
// 		if (enPortStatus::PtS_Wait_Out == Get_Equipment(IN_FromEqp).Get_PortStatus(nOtherParaIndex).nStatus)
// 		{
// 			Get_Socket(Get_Equipment(IN_FromEqp).Get_PortStatus(nOtherParaIndex).szRfid).m_stTestResult.m_nNG_Code;
// 
// 			// 반대편 Para가 배출 대기 중이고, 양품
// 			// 반대편 Para가 배출 대기 중이고, 불량
// 
// 			
// 		}
	}




	// 1. 현재 검사가 그룹
	Get_EquipmentCount();
	if (Get_Equipment(IN_FromEqp).Get_Group())
	{

	}

	// 2. 다음 검사(설비 유형) 선택

	// 3. 선택 검사(설비 유형)의 설비 선택

	// 4. 선택 설비의로 배출 가능 ?
*/


	// 1. 목적 설비로 소켓 배출 가능 ?
	// 2. 현재 설비의 바로 다음에 있는 설비로 소켓 배출 가능 ? (bypass로 지나 갈 경우에만)

	return true;
}

bool CTestManager_EQP_ICS::Proc_IsPossible_TrackOut_byEvent(__in uint8_t IN_FromEqp, __in LPCTSTR IN_szRFID)
{
	// 해당 소켓의 타겟 설비 (IN_FromEqp < TargetEqp)
	// 1. 목적 설비로 소켓 배출 가능 ?
	if (Get_Returner().Get_EqpOrder() == Get_SocketTarget(IN_szRFID))
	{
		// 목적지가 Returner (Bypas)


	}
	else
	{
		// 검사 가능 체크
		uint8_t nStatus = Get_Equipment(Get_SocketTarget(IN_szRFID)).Get_InputAvailabilityStatus();
		if (enInputAvailabilityStatus::IAS_Test != nStatus)
		{
			OnLog(_T("[eqp %02d] IsPossible_TrackOut_byLoader() => staus: %d, target eqp: %02d, not testable"), IN_FromEqp, nStatus, Get_SocketTarget(IN_szRFID));

			return false;
		}
	}

	// 2. 현재 설비의 바로 다음에 있는 설비로 소켓 배출 가능 ? (bypass로 지나 갈 경우에만)
	// => rfid 사용 여부 체크
	Get_EquipmentCount();
	uint8_t nNextEqp = IN_FromEqp + 1;
	if (nNextEqp < Get_EquipmentCount())
	{
		// 컨베이어 상태 체크, RFID 상태 체크
		uint8_t nStatus = Get_Equipment(nNextEqp).Get_InputAvailabilityStatus();
		if (enInputAvailabilityStatus::IAS_NoInput == nStatus)
		{
			OnLog(_T("[eqp %02d] IsPossible_TrackOut_byLoader() => staus: %d, next eqp: %02d, Impossible to put into the next equipment"), IN_FromEqp, nStatus, nNextEqp);

			return false;
		}
	}



	// RFID 포트 변경인가? => 이전 설비에서 bypass 배출 대기 ?
	// Para L/R, Buffer 포트 변경인가? => 


	// 이전 설비 중에 배출 대기 중인 소켓 : 목적지가 현재 설비
	// 앞 설비가 bypass로 현재 설비로 배출 대기




	// 현재 설비가 목적설비인 설비들 검색



	return true;
}

//=============================================================================
// Method		: Proc_EndOfProduction
// Access		: protected  
// Returns		: bool
// Parameter	: bool bEnd
// Qualifier	:
// Last Update	: 2022/4/13 - 10:13
// Desc.		:
//=============================================================================
bool CTestManager_EQP_ICS::Proc_EndOfProduction(bool bEnd)
{
	bool bResult = true;

	return bResult;
}

//=============================================================================
// Method		: Proc_IsFarAway_TargetEqp
// Access		: protected  
// Returns		: bool
// Parameter	: __in uint8_t IN_nFromEqp
// Parameter	: __in uint8_t IN_nTargetEqp
// Qualifier	:
// Last Update	: 2022/5/25 - 15:56
// Desc.		:
//=============================================================================
bool CTestManager_EQP_ICS::Proc_IsFarAway_TargetEqp(__in uint8_t IN_nFromEqp, __in uint8_t IN_nTargetEqp)
{
	int8_t nDistance = IN_nTargetEqp - IN_nFromEqp;

	OnLog(_T("[eqp %02d] Target Eqp(%d) Distance : %d"), IN_nTargetEqp, nDistance);

	if (3 < nDistance)
	{
		return true;
	}

	return false;
}

//=============================================================================
// Method		: OnSend_TimeSync
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2022/1/17 - 19:51
// Desc.		:
//=============================================================================
void CTestManager_EQP_ICS::OnSend_TimeSync()
{
	OnLog(_T("[eqp all] OnSend_TimeSync()"));

	if (m_pIcsComm->SendTimeSync())
	{
		OnLog(_T("OnSend_TimeSync() All Equipment : comm succeed"));
	}
	else
	{
		OnLog_Err(_T("OnSend_TimeSync() All Equipment : comm failed"));
	}
}

void CTestManager_EQP_ICS::OnSend_TimeSync(__in uint8_t IN_nEqpOrder)
{
	OnLog(_T("[eqp %02d] OnSend_TimeSync()"), IN_nEqpOrder);

	if (IN_nEqpOrder < Get_EquipmentCount())
	{
		if (m_pIcsComm->SendTimeSync(Get_EquipmentID(IN_nEqpOrder).GetBuffer()))
		{
			OnLog(_T("[eqp %02d] OnSend_TimeSync() : comm succeed"), IN_nEqpOrder);
		}
		else
		{
			OnLog_Err(_T("[eqp %02d] OnSend_TimeSync() : comm failed"), IN_nEqpOrder);
		}
	}
}

//=============================================================================
// Method		: OnSend_UserLevel
// Access		: protected  
// Returns		: void
// Parameter	: __in enPermissionMode IN_nLevel
// Parameter	: __in LPCTSTR IN_szUserId
// Qualifier	:
// Last Update	: 2022/5/18 - 18:33
// Desc.		:
//=============================================================================
void CTestManager_EQP_ICS::OnSend_UserLevel(__in enPermissionMode IN_nLevel, __in LPCTSTR IN_szUserId)
{
	OnLog(_T("[eqp all] OnSend_UserLevel() => level: %d, user id: %s"), IN_nLevel, IN_szUserId);
	if (m_pIcsComm->SendUserLevel(IN_nLevel, IN_szUserId))
	{
		OnLog(_T("OnSend_UserLevel() All Equipment : comm succeed"));
	}
	else
	{
		OnLog_Err(_T("OnSend_UserLevel() All Equipment : comm failed"));
	}
}

void CTestManager_EQP_ICS::OnSend_UserLevel(__in uint8_t IN_nEqpOrder, __in enPermissionMode IN_nLevel, __in LPCTSTR IN_szUserId)
{
	OnLog(_T("[eqp %02d] OnSend_UserLevel() => level: %d, user id: %s"), IN_nEqpOrder, IN_nLevel, IN_szUserId);

	if (IN_nEqpOrder < Get_EquipmentCount())
	{
		if (m_pIcsComm->SendUserLevel(Get_EquipmentID(IN_nEqpOrder).GetBuffer(), IN_nLevel, IN_szUserId))
		{
			OnLog(_T("[eqp %02d] OnSend_UserLevel() => comm succeed"), IN_nEqpOrder);
		}
		else
		{
			OnLog_Err(_T("[eqp %02d] OnSend_UserLevel() => comm failed"), IN_nEqpOrder);
		}
	}
}


void CTestManager_EQP_ICS::OnSend_UserLevel(__in _ICS_SERVER_Type In_Type,__in enPermissionMode IN_nLevel, __in LPCTSTR IN_szUserId)
{

	CString USERID(IN_szUserId);
	OnLog(_T("[eqp all] OnSend_UserLevel() => level: %d, user id: %s"), IN_nLevel, IN_szUserId);
	switch (In_Type) {
		case ICS_SERVER_MODULE:
			if (m_pIcsComm->SendUserLevel(IN_nLevel, IN_szUserId))
			{
				OnLog(_T("OnSend_UserLevel() All Equipment : comm succeed"));
			}
			else
			{
				OnLog_Err(_T("OnSend_UserLevel() All Equipment : comm failed"));
			}
			break;
		case ICS_SERVER_EES:	
#if (USE_XML)
			for (int i = 0; i < Get_ServerCount(); i++) 
			{				
				if (Get_Server(i).Get_ClientConnection()) {
					for (int IN_From = 0; IN_From < Get_EquipmentCount(); IN_From++) {
						Get_Equipment(IN_From).Get_DEFINEDATA().Set_USERID(lt::ToMultiByte(USERID));
					}
					Get_Server(i).Get_DEFINEDATA().Set_USERID(lt::ToMultiByte(USERID));

					
					auto svr = m_pIcsServer->GetRemote(Get_ServerID(i).GetBuffer());
					lt::Report_User_Change_Args::Args * Args = Get_Server(i).Create_Report_User_ChangeArgs(nullptr);
					lt::XUUID ID = Args->Hd.Get_transactionId();					
					svr->GetRemoteEes().CreateUserCommandProcedure(ID);
					svr->GetRemoteEes().AddeUserCommandProcedure(ID, *Args);
					delete Args;

					auto cntr = svr->GetRemoteEes().UserCommandCtrl(ID);
					OnSend_REPORT_USER_CHANGE(i, cntr.REPORT);
				}
			}		
#endif
			break;
	}

}
//=============================================================================
// Method		: OnSend_Language
// Access		: protected  
// Returns		: void
// Parameter	: __in uint8_t IN_nLanguage
// Qualifier	:
// Last Update	: 2022/1/17 - 20:07
// Desc.		:
//=============================================================================
void CTestManager_EQP_ICS::OnSend_Language(__in uint8_t IN_nLanguage)
{
	OnLog(_T("[eqp all] OnSend_UserLevel() => language: %d"), IN_nLanguage);
	if (m_pIcsComm->SendLanguage(IN_nLanguage))
	{
		OnLog(_T("OnSend_Language() => comm succeed"));
	}
	else
	{
		OnLog_Err(_T("OnSend_Language() => comm failed"));
	}
}

void CTestManager_EQP_ICS::OnSend_Language(__in uint8_t IN_nEqpOrder, __in uint8_t IN_nLanguage)
{
	OnLog(_T("[eqp %02d] OnSend_Language() => language: %d"), IN_nEqpOrder, IN_nLanguage);

	if (IN_nEqpOrder < Get_EquipmentCount())
	{
		if (m_pIcsComm->SendLanguage(Get_EquipmentID(IN_nEqpOrder).GetBuffer(), IN_nLanguage))
		{
			OnLog(_T("[eqp %02d] OnSend_Language() => comm succeed"), IN_nEqpOrder);
		}
		else
		{
			OnLog_Err(_T("[eqp %02d] OnSend_Language() => comm failed"), IN_nEqpOrder);
		}
	}
}

//=============================================================================
// Method		: OnSend_Model
// Access		: protected  
// Returns		: void
// Parameter	: __in uint8_t IN_nEqpOrder
// Qualifier	:
// Last Update	: 2022/1/17 - 20:07
// Desc.		:
//=============================================================================
void CTestManager_EQP_ICS::OnSend_Model(__in uint8_t IN_nEqpOrder)
{
	OnLog(_T("[eqp %02d] OnSend_Model() => model type: %d, socket type: %d"), IN_nEqpOrder, Get_LineInfo().m_nModelType, Get_LineInfo().m_nSocketType);

	if (IN_nEqpOrder < Get_EquipmentCount())
	{
		if (m_pIcsComm->SendModel(Get_EquipmentID(IN_nEqpOrder).GetBuffer(), Get_LineInfo().m_nModelType, Get_LineInfo().m_nSocketType))
		{
			OnLog(_T("[eqp %02d] OnSend_Model() => comm succeed"), IN_nEqpOrder);
		}
		else
		{
			OnLog_Err(_T("[eqp %02d] OnSend_Model() => comm failed"), IN_nEqpOrder);
		}
	}
}

//=============================================================================
// Method		: OnSend_Accept_SocketTrackIn
// Access		: protected  
// Returns		: void
// Parameter	: __in uint8_t IN_nEqpOrder
// Parameter	: __in LPCTSTR IN_szRFID
// Parameter	: __in uint8_t IN_nFlag
// Parameter	: __in LPCTSTR IN_szBarcode
// Qualifier	:
// Last Update	: 2022/1/17 - 20:07
// Desc.		:
//=============================================================================
void CTestManager_EQP_ICS::OnSend_Accept_SocketTrackIn(__in uint8_t IN_nEqpOrder, __in LPCTSTR IN_szRFID, __in uint8_t IN_nFlag, __in LPCTSTR IN_szBarcode)
{
	OnLog(_T("[eqp %02d] OnSend_Accept_SocketTrackIn() => rfid: %s, flag: %d, barcode: %s"), IN_nEqpOrder, IN_szRFID, IN_nFlag, IN_szBarcode);

	if (IN_nEqpOrder < Get_EquipmentCount())
	{
		if (m_pIcsComm->SendSocketAccept(Get_EquipmentID(IN_nEqpOrder).GetBuffer(), IN_szRFID, IN_nFlag, IN_szBarcode))
		{
			OnLog(_T("[eqp %02d] OnSend_Accept_SocketTrackIn() => comm succeed"), IN_nEqpOrder);
		}
		else
		{
			OnLog_Err(_T("[eqp %02d] OnSend_Accept_SocketTrackIn() => comm failed"), IN_nEqpOrder);
		}
	}
}

//=============================================================================
// Method		: OnSend_Accept_SocketTrackOut
// Access		: protected  
// Returns		: void
// Parameter	: __in uint8_t IN_nEqpOrder
// Parameter	: __in LPCTSTR IN_szRFID
// Qualifier	:
// Last Update	: 2022/1/17 - 20:07
// Desc.		:
//=============================================================================
void CTestManager_EQP_ICS::OnSend_Accept_SocketTrackOut(__in uint8_t IN_nEqpOrder, __in LPCTSTR IN_szRFID)
{
	OnLog(_T("[eqp %02d] OnSend_Accept_SocketTrackOut() => rfid: %s"), IN_nEqpOrder, IN_szRFID);

	if (IN_nEqpOrder < Get_EquipmentCount())
	{
		if (m_pIcsComm->SendEjectAccept(Get_EquipmentID(IN_nEqpOrder).GetBuffer(), IN_szRFID))
		{
			OnLog(_T("[eqp %02d] OnSend_Accept_SocketTrackOut() => comm succeed"), IN_nEqpOrder);
		}
		else
		{
			OnLog_Err(_T("[eqp %02d] OnSend_Accept_SocketTrackOut() => comm failed"), IN_nEqpOrder);
		}
	}
}

//=============================================================================
// Method		: OnSend_Forced_SocketTrackOut
// Access		: protected  
// Returns		: bool
// Parameter	: __in bool IN_bOnOff
// Qualifier	:
// Last Update	: 2022/1/17 - 20:08
// Desc.		:
//=============================================================================
bool CTestManager_EQP_ICS::OnSend_Forced_SocketTrackOut(__in bool IN_bOnOff)
{
	OnLog(_T("[eqp all] OnSend_Forced_SocketTrackOut() => OnOff: %d"), IN_bOnOff ? 1 : 0);
	if (m_pIcsComm->SendForceEject(IN_bOnOff))
	{
		OnLog(_T("OnSend_Forced_SocketTrackOut() => comm succeed"));
		return true;
	}
	else
	{
		OnLog_Err(_T("OnSend_Forced_SocketTrackOut() => comm failed"));
		return false;
	}
}

bool CTestManager_EQP_ICS::OnSend_Forced_SocketTrackOut(__in uint8_t IN_nEqpOrder, __in bool IN_bOnOff)
{
	OnLog(_T("[eqp %02d] OnSend_Forced_SocketTrackOut() => OnOff: %d"), IN_nEqpOrder, IN_bOnOff ? 1 : 0);

	if (IN_nEqpOrder < Get_EquipmentCount())
	{
		if (m_pIcsComm->SendForceEject(Get_EquipmentID(IN_nEqpOrder).GetBuffer(), IN_bOnOff))
		{
			OnLog(_T("[eqp %02d] OnSend_Forced_SocketTrackOut() => comm succeed"), IN_nEqpOrder);
			return true;
		}
		else
		{
			OnLog_Err(_T("[eqp %02d] OnSend_Forced_SocketTrackOut() => comm failed"), IN_nEqpOrder);
			return false;
		}
	}

	return false;
}

//=============================================================================
// Method		: OnSend_EndOfProduction
// Access		: protected  
// Returns		: bool
// Qualifier	:
// Last Update	: 2022/2/21 - 16:59
// Desc.		:
//=============================================================================
bool CTestManager_EQP_ICS::OnSend_EndOfProduction(__in bool bEnd /*= true*/)
{
	OnLog(_T("[eqp all] OnSend_EndOfProduction() => %s"), bEnd ? _T("End") : _T("Begin"));

	// 생산 종료 모드로 설정
	OnSetStatus_Production(bEnd);

	bool bResult = false;
	if (bEnd) // 생산 종료
	{
		// 로더로 전송
		if (m_pIcsComm->SendProductionStartEnd(Get_Loader().Get_EquipmentId().GetBuffer(), 1))
		{
			OnLog(_T("[Loader] OnSend_EndOfProduction() => comm succeed"));
			bResult = true;
		}
		else
		{
			OnLog_Err(_T("[Loader] OnSend_EndOfProduction() => comm failed"));
			bResult = false;
		}
	}
	else // 생산 시작
	{
		// 전체로 전송
		if (m_pIcsComm->SendProductionStartEnd(0))
		{
			OnLog(_T("[eqp all] OnSend_EndOfProduction() => comm succeed"));
			bResult = true;
		}
		else
		{
			OnLog_Err(_T("[eqp all] OnSend_EndOfProduction() => comm failed"));
			bResult = false;
		}
	}

	return bResult;
}

bool CTestManager_EQP_ICS::OnSend_EndOfProduction(__in uint8_t IN_nEqpOrder, __in bool bEnd /*= true*/)
{
	OnLog(_T("[eqp %02d] OnSend_EndOfProduction() => %s"), IN_nEqpOrder, bEnd ? _T("End") : _T("Begin"));

	if (IN_nEqpOrder < Get_EquipmentCount())
	{
		if (m_pIcsComm->SendProductionStartEnd(Get_EquipmentID(IN_nEqpOrder).GetBuffer(), bEnd ? 1 : 0))
		{
			OnLog(_T("[eqp %02d] OnSend_EndOfProduction() => comm succeed"), IN_nEqpOrder);
			return true;
		}
		else
		{
			OnLog_Err(_T("[eqp %02d] OnSend_EndOfProduction() => comm failed"), IN_nEqpOrder);
			return false;
		}
	}

	return false;
}


//=============================================================================
// Method		: OnSend_Init
// Access		: protected  
// Returns		: bool
// Qualifier	:
// Last Update	: 2022/2/19 - 14:12
// Desc.		:
//=============================================================================
bool CTestManager_EQP_ICS::OnSend_Init()
{
	OnLog(_T("[eqp all] OnSend_Init()"));
	if (m_pIcsComm->SendInitialization())
	{
		OnLog(_T("[eqp all] OnSend_Init() => comm succeed"));
		return true;
	}
	else
	{
		OnLog_Err(_T("[eqp all] OnSend_Init() => comm failed"));
		return false;
	}
}

bool CTestManager_EQP_ICS::OnSend_Init(__in uint8_t IN_nEqpOrder)
{
	OnLog(_T("[eqp %02d] OnSend_Init()"), IN_nEqpOrder);
	if (m_pIcsComm->SendInitialization(Get_EquipmentID(IN_nEqpOrder).GetBuffer()))
	{
		OnLog(_T("[eqp %02d] OnSend_Init() => comm succeed"), IN_nEqpOrder);
		return true;
	}
	else
	{
		OnLog_Err(_T("[eqp %02d] OnSend_Init() => comm failed"), IN_nEqpOrder);
		return false;
	}
}

//=============================================================================
// Method		: OnSend_Reset
// Access		: protected  
// Returns		: bool
// Qualifier	:
// Last Update	: 2022/7/19 - 10:06
// Desc.		:
//=============================================================================
bool CTestManager_EQP_ICS::OnSend_Reset()
{
	OnLog(_T("[eqp all] OnSend_Reset()"));
	if (m_pIcsComm->SendReset())
	{
		OnLog(_T("[eqp all] OnSend_Reset() => comm succeed"));
		return true;
	}
	else
	{
		OnLog_Err(_T("[eqp all] OnSend_Reset() => comm failed"));
		return false;
	}
}

bool CTestManager_EQP_ICS::OnSend_Reset(__in uint8_t IN_nEqpOrder)
{
	OnLog(_T("[eqp %02d] OnSend_Reset()"), IN_nEqpOrder);
	if (m_pIcsComm->SendReset(Get_EquipmentID(IN_nEqpOrder).GetBuffer()))
	{
		OnLog(_T("[eqp %02d] OnSend_Reset() => comm succeed"), IN_nEqpOrder);
		return true;
	}
	else
	{
		OnLog_Err(_T("[eqp %02d] OnSend_Reset() => comm failed"), IN_nEqpOrder);
		return false;
	}
}

//=============================================================================
// Method		: OnSend_Run
// Access		: protected  
// Returns		: bool
// Qualifier	:
// Last Update	: 2022/8/10 - 10:26
// Desc.		:
//=============================================================================
bool CTestManager_EQP_ICS::OnSend_Run()
{
	OnLog(_T("[eqp all] OnSend_Run()"));
	if (m_pIcsComm->SendRun())
	{
		OnLog(_T("[eqp all] OnSend_Run() => comm succeed"));
		return true;
	}
	else
	{
		OnLog_Err(_T("[eqp all] OnSend_Run() => comm failed"));
		return false;
	}
}

bool CTestManager_EQP_ICS::OnSend_Run(__in uint8_t IN_nEqpOrder)
{
	OnLog(_T("[eqp %02d] OnSend_Run()"), IN_nEqpOrder);
	if (m_pIcsComm->SendRun(Get_EquipmentID(IN_nEqpOrder).GetBuffer()))
	{
		OnLog(_T("[eqp %02d] OnSend_Run() => comm succeed"), IN_nEqpOrder);
		return true;
	}
	else
	{
		OnLog_Err(_T("[eqp %02d] OnSend_Run() => comm failed"), IN_nEqpOrder);
		return false;
	}
}

//=============================================================================
// Method		: OnSend_Stop
// Access		: protected  
// Returns		: bool
// Qualifier	:
// Last Update	: 2022/8/10 - 10:26
// Desc.		:
//=============================================================================
bool CTestManager_EQP_ICS::OnSend_Stop()
{
	OnLog(_T("[eqp all] OnSend_Stop()"));
	if (m_pIcsComm->SendStop())
	{
		OnLog(_T("[eqp all] OnSend_Stop() => comm succeed"));
		return true;
	}
	else
	{
		OnLog_Err(_T("[eqp all] OnSend_Stop() => comm failed"));
		return false;
	}
}

bool CTestManager_EQP_ICS::OnSend_Stop(__in uint8_t IN_nEqpOrder)
{
	OnLog(_T("[eqp %02d] OnSend_Stop()"), IN_nEqpOrder);
	if (m_pIcsComm->SendStop(Get_EquipmentID(IN_nEqpOrder).GetBuffer()))
	{
		OnLog(_T("[eqp %02d] OnSend_Stop() => comm succeed"), IN_nEqpOrder);
		return true;
	}
	else
	{
		OnLog_Err(_T("[eqp %02d] OnSend_Stop() => comm failed"), IN_nEqpOrder);
		return false;
	}
}

//=============================================================================
// Method		: OnSend_BuzzerOff
// Access		: protected  
// Returns		: bool
// Qualifier	:
// Last Update	: 2022/8/10 - 10:26
// Desc.		:
//=============================================================================
bool CTestManager_EQP_ICS::OnSend_BuzzerOff()
{
	OnLog(_T("[eqp all] OnSend_BuzzerOff()"));
	if (m_pIcsComm->SendBuzzerOff())
	{
		OnLog(_T("[eqp all] OnSend_BuzzerOff() => comm succeed"));
		return true;
	}
	else
	{
		OnLog_Err(_T("[eqp all] OnSend_BuzzerOff() => comm failed"));
		return false;
	}
}

bool CTestManager_EQP_ICS::OnSend_BuzzerOff(__in uint8_t IN_nEqpOrder)
{
	OnLog(_T("[eqp %02d] OnSend_BuzzerOff()"), IN_nEqpOrder);
	if (m_pIcsComm->SendBuzzerOff(Get_EquipmentID(IN_nEqpOrder).GetBuffer()))
	{
		OnLog(_T("[eqp %02d] OnSend_BuzzerOff() => comm succeed"), IN_nEqpOrder);
		return true;
	}
	else
	{
		OnLog_Err(_T("[eqp %02d] OnSend_BuzzerOff() => comm failed"), IN_nEqpOrder);
		return false;
	}
}

//=============================================================================
// Method		: OnSend_ShowHide
// Access		: protected  
// Returns		: bool
// Parameter	: __in bool bShow
// Qualifier	:
// Last Update	: 2022/8/10 - 10:26
// Desc.		:
//=============================================================================
bool CTestManager_EQP_ICS::OnSend_ShowHide(__in bool bShow)
{
	OnLog(_T("[eqp all] OnSend_ShowHide()"));
	if (m_pIcsComm->SendUiVisible(bShow ? SW_SHOW : SW_HIDE))
	{
		OnLog(_T("[eqp all] OnSend_ShowHide() => comm succeed"));
		return true;
	}
	else
	{
		OnLog_Err(_T("[eqp all] OnSend_ShowHide() => comm failed"));
		return false;
	}
}

bool CTestManager_EQP_ICS::OnSend_ShowHide(__in uint8_t IN_nEqpOrder, __in bool bShow)
{
	OnLog(_T("[eqp %02d] OnSend_ShowHide() => %s"), IN_nEqpOrder, bShow ? _T("Show") : _T("Hide"));
	if (m_pIcsComm->SendUiVisible(Get_EquipmentID(IN_nEqpOrder).GetBuffer(), bShow ? SW_SHOW : SW_HIDE))
	{
		OnLog(_T("[eqp %02d] OnSend_ShowHide() => comm succeed"), IN_nEqpOrder);
		return true;
	}
	else
	{
		OnLog_Err(_T("[eqp %02d] OnSend_ShowHide() => comm failed"), IN_nEqpOrder);
		return false;
	}

}

//=============================================================================
// Method		: OnVerify_RecipeInfo
// Access		: virtual protected  
// Returns		: LRESULT
// Qualifier	:
// Last Update	: 2021/12/10 - 14:30
// Desc.		:
//=============================================================================
LRESULT CTestManager_EQP_ICS::OnVerify_RecipeInfo()
{
	LRESULT lResult = RC_OK;

	lResult = CTestManager_EQP::OnVerify_RecipeInfo();

	return lResult;
}

//=============================================================================
// Method		: OnReport_Socket
// Access		: virtual protected  
// Returns		: void
// Parameter	: __in LPCTSTR IN_szRFID
// Qualifier	:
// Last Update	: 2021/12/30 - 19:44
// Desc.		:
//=============================================================================
void CTestManager_EQP_ICS::OnReport_Socket(__in LPCTSTR IN_szRFID)
{
	ST_TestResult* pRe = &Get_Socket(IN_szRFID).m_stTestResult;

	if (0 == pRe->m_nNG_Code)
	{
		OnLog(_T("[Unload] Report Socket => rfid: %s, barcode: %s, pass"), IN_szRFID, Get_Socket(IN_szRFID).m_szBarcode);

		OnLog_SocketEvent(IN_szRFID, _T("[Unload] Report Socket => rfid: %s, barcode: %s, pass"), IN_szRFID, Get_Socket(IN_szRFID).m_szBarcode);
	}
	else
	{
		OnLog(_T("[Unload] Report Socket => rfid: %s, barcode: %s, fail (ng code: %d, ng eqp type: %s, ng eqp: %d, ng para: %s)"), 
			IN_szRFID, 
			Get_Socket(IN_szRFID).m_szBarcode,
			pRe->m_nNG_Code,
			g_szEqpTypeName[Get_SocketTestResult(IN_szRFID).m_nNG_EqpType],//pRe->m_nNG_EqpType,
			pRe->m_nNG_EqpOrder,
			g_szEqpPara_Mini[pRe->m_nNG_Para] //(Para_Left == pRe->m_nNG_Para) ? _T("L") : _T("R")
		);

		OnLog_SocketEvent(IN_szRFID, _T("[Unload] Report Socket => rfid: %s, barcode: %s, fail (ng code: %d, ng eqp type: %s, ng eqp: %d, ng para: %s)"),
			IN_szRFID,
			Get_Socket(IN_szRFID).m_szBarcode,
			pRe->m_nNG_Code,
			g_szEqpTypeName[Get_SocketTestResult(IN_szRFID).m_nNG_EqpType],//pRe->m_nNG_EqpType,
			pRe->m_nNG_EqpOrder,
			g_szEqpPara_Mini[pRe->m_nNG_Para] //(Para_Left == pRe->m_nNG_Para) ? _T("L") : _T("R")
		);
	}

	// 파일로 저장 (Load Time, Unload Time, RFID, Serial Number, NG Code, Eqp Type, Eqp No, Eqp ID, Eqp Para)
	CString szPath;
	CString szFullPath;

	SYSTEMTIME tmLocal;
	GetLocalTime(&tmLocal);

	// Report\\Year\\Month\\ 
	szPath.Format(_T("%s\\%04d\\%02d\\"), m_stInspInfo.Path.szReport, tmLocal.wYear, tmLocal.wMonth);
	MakeDirectory(szPath);

	szFullPath.Format(_T("%sReport_%04d_%02d%02d.csv"), szPath, tmLocal.wYear, tmLocal.wMonth, tmLocal.wDay);

	OnLogFile_SocketResult(IN_szRFID, szFullPath.GetBuffer());
	

	// 수율 표시 ?????

}

//=============================================================================
// Method		: OnCreateTimer_UpdateUI
// Access		: virtual protected  
// Returns		: void
// Parameter	: __in DWORD DueTime
// Parameter	: __in DWORD Period
// Qualifier	:
// Last Update	: 2021/6/7 - 12:28
// Desc.		:
//=============================================================================
void CTestManager_EQP_ICS::OnCreateTimer_UpdateUI(__in DWORD DueTime /*= 5000*/, __in DWORD Period /*= 250*/)
{
	__super::OnCreateTimer_UpdateUI(DueTime, Period);
}

void CTestManager_EQP_ICS::OnDeleteTimer_UpdateUI()
{
	__super::OnDeleteTimer_UpdateUI();
}

void CTestManager_EQP_ICS::OnMonitor_TimeCheck()
{
	__super::OnMonitor_TimeCheck();
}

//=============================================================================
// Method		: OnMonitor_UpdateUI
// Access		: virtual protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2022/4/12 - 16:26
// Desc.		:
//=============================================================================
void CTestManager_EQP_ICS::OnMonitor_UpdateUI()
{
	//__super::OnMonitor_UpdateUI();

	
	// 서버가 open 되어 있는 경우에만 사용
	if (m_pIcsComm->IsOpened())
	{
		// 검사 설비의 비어 있음을 체크하여   컨베이어 사용 제어
		//OnLog(_T("Check Empty Equipment"));

		static const uint8_t nCheckEqpCount = 2;

		// 로더 다음 설비에서 리터너 전 설비까지
		uint8_t nEnd_eqp = Get_Returner().Get_EqpOrder() - 1;

		uint8_t nEmptyCount = 0;
		uint8_t nCheckCount = nCheckEqpCount;
		for (auto nEqpIdx = 0; nEqpIdx < nEnd_eqp; ++nEqpIdx)
		{
			nCheckCount = __min(nEqpIdx, nCheckEqpCount);

			// 설비가 비어 있는가?
			if (Get_Equipment(nEqpIdx).IsEmpty_Equipment())
			{
				++nEmptyCount;
			}
			else
			{
				if (Get_Equipment(nEqpIdx).Is_Tester() && Get_Equipment(nEqpIdx).IsLastSocket_onTestPort())
				{
					if (nCheckCount <= nEmptyCount)
					{
						// 현재 설비로 생산 종료 메세지 전송
						OnSend_EndOfProduction(nEqpIdx, true);
					}
				}

				nEmptyCount = 0;
			}
		}
	}
	
}

//=============================================================================
// Method		: OnInitialize
// Access		: virtual public  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/6/7 - 12:28
// Desc.		:
//=============================================================================
void CTestManager_EQP_ICS::OnInitialize()
{
	__super::OnInitialize();
}

void CTestManager_EQP_ICS::OnFinalize()
{
	__super::OnFinalize();
}

void CTestManager_EQP_ICS::OnSet_PermissionMode(__in enPermissionMode nAcessMode)
{
	__super::OnSet_PermissionMode(nAcessMode);
}

#if (USE_XML)
void CTestManager_EQP_ICS::OnMatchingServer() {
	auto nSvrCnt = Get_ServerCount();
	if (0 < nSvrCnt)	{
		for (auto nIdx = 0; nIdx < nSvrCnt; ++nIdx)		{
			switch (Get_ServerType(nIdx))			{
			case enServerType::SERVER_EES:
				if (false == m_pIcsServer->CreateRemote((CSvrEes&)Get_Server(nIdx)))				{
					OnLog_Err(_T("CreateRemote(CSvrEes) failed"));
				}
				break;
			}
		}
	}
}
void CTestManager_EQP_ICS::OnEvent_ServerConnection(__in uint8_t IN_From, __in LPARAM IN_Data)
{	
	lt::Report_Online_State_Args::Args * Args = Get_Server(IN_From).Create_Report_Online_StateArgs(nullptr);
	auto cntr = Add_Online_StateArgs(Get_ServerID(IN_From), *Args);
	OnSend_REPORT_ONLINE_STATE(IN_From, cntr.REPORT);
	
	if (Get_Server(IN_From).Get_ClientConnection()) {		
		for (int n_Eqp = 0; n_Eqp < Get_EquipmentCount(); n_Eqp++) {
			if (Get_Equipment(n_Eqp).Get_ClientConnection()) {
				auto EquipArgs = Get_Server(IN_From).Create_Report_Equipment_StateArgs(nullptr);
				auto Equipcntr = Add_Equipment_StateArgs(Get_ServerID(IN_From), *EquipArgs);
				for (int n_Eqp = 0; n_Eqp < Get_Equipment(n_Eqp).Get_mEES_PortSubStatusCount(); n_Eqp++) {
					Equipcntr.REPORT.Body.Set_EQUIPMENTSTATE(lt::ToMultiByte(g_szPortStatus[Get_Equipment(n_Eqp).Get_mEES_PortSubStatus(n_Eqp).Get_nProcessStatus()]));
					OnSend_REPORT_EQUIPMENT_STATE(IN_From, Equipcntr.REPORT);
				}
			} else {
				for (int n_Eqp = 0; n_Eqp < Get_Equipment(n_Eqp).Get_mEES_PortSubStatusCount(); n_Eqp++) {
					Get_Equipment(n_Eqp).Get_mEES_PortSubStatus(n_Eqp).Set_nProcessStatus(PtS_Empty);
				}
			}
		}
	}
}
void CTestManager_EQP_ICS::OnEvent_ServerREQUEST_LINK_TEST(__in uint8_t IN_From, __in LPARAM IN_Data)
{	
	lt::Request_Link_Test_Args::Args * Args = (lt::Request_Link_Test_Args::Args *) IN_Data;
	auto cntr = Add_Link_TestArgs(Get_ServerID(IN_From), *Args);
	cntr.RequestToReply();
	OnSend_REPLY_LINK_TEST(IN_From, cntr.REPLY);
}
void CTestManager_EQP_ICS::OnEvent_ServerREPLY_LINK_TEST(__in uint8_t IN_From, __in LPARAM IN_Data)
{
}
void CTestManager_EQP_ICS::OnEvent_ServerREPORT_ONLINE_STATE(__in uint8_t IN_From, __in LPARAM IN_Data)
{
	auto Args = Get_Server(IN_From).Create_Report_Online_StateArgs((lt::Report_Online_State_Args::Args *) IN_Data);
	auto cntr = Add_Online_StateArgs(Get_ServerID(IN_From), *Args);
	OnSend_REPORT_ONLINE_STATE(IN_From, cntr.REPORT);
}
void CTestManager_EQP_ICS::OnEvent_ServerREPORT_USER_CHANGE(__in uint8_t IN_From, __in LPARAM IN_Data)
{
	auto Args = Get_Server(IN_From).Create_Report_User_ChangeArgs((lt::Report_User_Change_Args::Args *) IN_Data);
	auto cntr = Add_User_CommandArgs(Get_ServerID(IN_From), *Args);
	OnSend_REPORT_USER_CHANGE(IN_From, cntr.REPORT);
}
void CTestManager_EQP_ICS::OnEvent_ServerREQUEST_USER_CHANGE(__in uint8_t IN_From, __in LPARAM IN_Data)
{
	lt::XUUID ID;
	lt::Request_User_Cpmmand_Args::Args * Args = (lt::Request_User_Cpmmand_Args::Args *) IN_Data;
	auto cntr = Add_User_CommandArgs(Get_ServerID(IN_From), *Args);
	cntr.RequestToReply();
	OnSend_REPLY_USER_COMMAND(IN_From, cntr.REPLY);	
}

void CTestManager_EQP_ICS::OnEvent_ServerREPLY_USER_COMMAND(__in uint8_t IN_From, __in LPARAM IN_Data)
{
	lt::Reply_User_Change_Args::Args * Args = (lt::Reply_User_Change_Args::Args *) IN_Data;
	auto cntr = Add_User_CommandArgs(Get_ServerID(IN_From), *Args);
	OnSend_REPLY_USER_COMMAND(IN_From, cntr.REPLY);
}
void CTestManager_EQP_ICS::OnEvent_ServerREPORT_EQUIPMENT_STATE(__in uint8_t IN_From, __in LPARAM IN_Data)
{
	//lt::Report_Equipment_State_Args::Args * Args = (lt::Report_Equipment_State_Args::Args *) IN_Data;
	auto Args = Get_Server(IN_From).Create_Report_Equipment_StateArgs((lt::Report_Equipment_State_Args::Args *) IN_Data);
	auto cntr = Add_Equipment_StateArgs(Get_ServerID(IN_From), *Args);
	OnSend_REPORT_EQUIPMENT_STATE(IN_From, cntr.REPORT);
#if TEST
	ST_xml_REPORT_EQUIPMENT_STATE* IN_PARA = (ST_xml_REPORT_EQUIPMENT_STATE*)IN_Data;
	/*
	CString EQUIPMENTID(IN_PARA->Body.EQUIPMENTID);
	CString SUBEQPID(IN_PARA->Body.SUBEQPID);
	CString EQUIPMENTSTATE(IN_PARA->Body.EQUIPMENTSTATE);
	CString PORTID(IN_PARA->Body.PORTID);
	CString LOTID(IN_PARA->Body.LOTID);
	*/
	auto bConnectionEvent = bGetClientConnectionEvent(IN_From);
	if (IN_From < Get_ServerCount()){		
		//if (bConnectionEvent && Get_Server(IN_From).Get_ClientConnection()) {
		//	for (int n_SvrPort = 0; n_SvrPort < Get_Server(IN_From).Get_EquipmentIDCount(); n_SvrPort++) {
		//		
		//	}
		//}
		if (Get_Server(IN_From).Get_ClientConnection()) {
			for (int n_SvrPort = 0; n_SvrPort < Get_Server(IN_From).Get_EquipmentIDCount(); n_SvrPort++) {
				int State = ES_IDLE;
				bool bSet = false;
				for (int n_Eqp = 0; n_Eqp < Get_EquipmentCount(); n_Eqp++) {
					if (Get_Equipment(n_Eqp).Get_ClientConnection()) {
						if (bSet) {
							continue;
						}
						for (int n_EqpPort = 0; n_EqpPort < Get_Equipment(n_Eqp).Get_PortCount(); n_EqpPort++) {
							if (!Get_EquipmentTypeEvent(n_Eqp, n_EqpPort)) {
								continue;
							}
							if (bSet) {
								continue;
							}
							if (Get_Equipment(n_Eqp).Get_PortStatus(n_EqpPort).nEquipmentState == ES_SUDDENSTOP) {
								State = ES_SUDDENSTOP;
								bSet = true;								
							}
							else if (bSet == false){
								if (Get_Equipment(n_Eqp).Get_PortStatus(n_EqpPort).nEquipmentState == ES_STOP) {
									State = ES_STOP;
									bSet = true;
								}
								else if (bSet == false) {
									if (Get_Equipment(n_Eqp).Get_PortStatus(n_EqpPort).nEquipmentState == ES_RUN) {
										State = ES_RUN;
										bSet = true;
									}
									else {
										State = ES_IDLE;
										//bSet = true;
									}
								}
							}
						}
					}
				}
				Get_Server(IN_From).Set_PortStatusEquipmentStateEvent(n_SvrPort, State);
				if (bGetSvrEquipmentStateEvent(IN_From, n_SvrPort)) {
					CString EQUIPMENTID(Get_Server(IN_From).Get_EquipmentIDStatus(n_SvrPort).szEquipID);
					CString SUBEQPID(Get_Server(IN_From).Get_SubEqpID());
					CString PORTID(Get_Server(IN_From).Get_EquipmentIDStatus(n_SvrPort).szPortID);
					CString EQUIPMENTSTATE(g_szEquipment_State[Get_Server(IN_From).Get_PortStatus(n_SvrPort).nEquipmentState]);
					CString LOTID;
					auto m_p = Get_Server(IN_From).GetXmlEes().Set_ReportEquipmentStateParameter(
						EQUIPMENTID,
						SUBEQPID,
						PORTID,
						EQUIPMENTSTATE,
						LOTID);
					Get_Server(IN_From).Set_Notify_EquipmentState(m_p);
					OnSend_REPORT_EQUIPMENT_STATE(
						IN_From,
						m_p);
					Get_Server(IN_From).Set_OldPortStatusEquipmentStateEvent(n_SvrPort, State);
				}
			}
		}
	}
	/*
	if (IN_From < Get_ServerCount()) {
		if (Get_Server(IN_From).Get_ClientConnection()) {
			for (int n_Port = 0; n_Port < Get_Server(IN_From).Get_EquipmentIDCount(); n_Port++) {
				if (Get_Server(IN_From).Get_EquipmentIDStatus(n_Port).szEquipID == EQUIPMENTID) {
					OnSend_REPORT_EQUIPMENT_STATE(
						IN_From,
						IN_PARA);
					//Get_Server(IN_From).GetXmlEes().Set_ReportEquipmentStateParameter(
					//	SUBEQPID,
					//	PORTID,
					//	EQUIPMENTSTATE,
					//	LOTID));
					Get_Server(IN_From).Set_Notify_EquipmentState(EQUIPMENTSTATE);
				}
			}
		}
	}*/
	//2023.06.28a
	if (MainFrame->FindLPARAM(IN_Data)) {
		delete IN_PARA;
		IN_PARA = nullptr;
		MainFrame->RemoveLPARAM(IN_Data);
	}
#endif
}
void CTestManager_EQP_ICS::OnEvent_ServerREQUEST_EQUIPMENT_STATE_DISPLAY(__in uint8_t IN_From, __in LPARAM IN_Data)
{
	lt::XUUID ID;
	lt::Reply_Equipment_State_Display_Args::Args * Args = (lt::Reply_Equipment_State_Display_Args::Args *) IN_Data;
	auto svr = m_pIcsServer->GetRemote(Get_ServerID(IN_From));
	if (Args != nullptr) {
		ID = Args->Hd.Get_transactionId();
		svr->GetRemoteEes().AddEquipmentStateProcedure(ID, *Args);
		delete Args;
	}
	else {
		ID = svr->GetRemoteEes().EquipmentStateCtrl();
	}
	auto cntr = svr->GetRemoteEes().EquipmentStateCtrl(ID);
	cntr.RequestToReply();
	CString EQUIPMENTID = lt::ToWideChar(cntr.REQUEST.Body.Get_EQUIPMENTID()).c_str();
	
	if (IN_From < Get_ServerCount()) {
		if (Get_Server(IN_From).Get_ClientConnection()) {
			/*
			for (int n_Port = 0; n_Port < Get_Server(IN_From).Get_EquipmentIDCount(); n_Port++) {
				if (Get_Server(IN_From).Get_EquipmentIDStatus(n_Port).Get_EQUIPMENTID() == EQUIPMENTID) {
				CString EQUIPMENTID = lt::ToWideChar(cntr.REQUEST.Body.Get_EQUIPMENTID()).c_str();
					CString EQUIPMENTID(Get_Server(IN_From).Get_EquipmentIDStatus(n_Port).Get_EQUIPMENTID());
					CString SUBEQPID(Get_Server(IN_From).Get_SubEqpID());
					CString PORTID(Get_Server(IN_From).Get_EquipmentIDStatus(n_Port).Get_PORTID());
					CString EQUIPMENTSTATE(g_szEquipment_State[Get_Server(IN_From).Get_mEES_PortSubStatus(n_Port).Get_nEquipmentStatus()]);
					//CString LOTID(Get_Equipment(i).Get_FromPortToLOTID(n_Port));
					OnSend_REPLY_EQUIPMENT_STATE_DISPLAY(
						IN_From,
						cntr.REPLY);
					
					for (UINT nIdx = 0; nIdx < Get_EESCount(); nIdx++)
					{
						if (0 == Get_EESEqpCode(nIdx).Compare(EQUIPMENTSTATEDISPLAY)) {
							dsp.Dsp = IN_PARA;
							dsp.EES = &Get_EES(nIdx);
							Get_Server(IN_From).Set_Notify_EquipmentStateDisplay(&dsp);
						}
					}
				}
			}*/
			for (int i = 0; i < Get_EquipmentCount(); i++) {
				if (Get_Equipment(i).Get_ClientConnection()) {
					for (int n_Port = 0; n_Port < Get_Equipment(i).Get_EquipmentIDCount(); n_Port++) {
						if (Get_Equipment(i).Get_EquipmentIDStatus(n_Port).Get_EQUIPMENTID() == EQUIPMENTID) {
							
							//CString EQUIPMENTID(Get_Equipment(i).Get_EquipmentIDStatus(n_Port).Get_EQUIPMENTID);
							//2023.06.28a uhkim
							//CString SUBEQPID(Get_Equipment(i).Get_SubEqpID());
							CString SUBEQPID(Get_Server(IN_From).Get_SubEqpID());
							CString PORTID(Get_Equipment(i).Get_EquipmentIDStatus(n_Port).Get_PORTID());
							CString EQUIPMENTSTATE(g_szEquipment_State[Get_Equipment(i).Get_mEES_PortSubStatus(n_Port).Get_nProcessStatus()]);
							//CString LOTID(Get_Equipment(i).Get_FromPortToLOTID(n_Port));
							OnSend_REPLY_EQUIPMENT_STATE_DISPLAY(
								IN_From,
								cntr.REPLY);
							/*
							for (UINT nIdx = 0; nIdx < Get_EESCount(); nIdx++)
							{
								if (0 == Get_EESEqpCode(nIdx).Compare(EQUIPMENTSTATEDISPLAY)) {
									dsp.Dsp = IN_PARA;
									dsp.EES = &Get_EES(nIdx);
									Get_Equipment(i).Set_Notify_EquipmentStateDisplay(&dsp);
								}
							}*/
						}
					}
				}

			}
		}
	}
#if TEST
	CString temp;
	ST_EquipmentStateDisplay dsp;
	ST_xml_REQUEST_EQUIPMENT_STATE_DISPLAY* IN_PARA = (ST_xml_REQUEST_EQUIPMENT_STATE_DISPLAY*)IN_Data;
	
	CString transactionId(IN_PARA->Hd.transactionId);
	CString EQUIPMENTID(IN_PARA->Body.EQUIPMENTID);
	CString SUBEQPID(IN_PARA->Body.SUBEQPID);
	//CString PORTID(IN_PARA->Body.PORTID);
	//int nPORTID = _ttoi(PORTID);
	CString EQUIPMENTSTATEDISPLAY(IN_PARA->Body.EQUIPMENTSTATEDISPLAY);
	CString RGBDISPLAY(IN_PARA->Body.RGBDISPLAY);


	if (IN_From < Get_ServerCount()) {
		if (Get_Server(IN_From).Get_ClientConnection()) {
			for (int n_Port = 0; n_Port < Get_Server(IN_From).Get_EquipmentIDCount(); n_Port++) {
				if (Get_Server(IN_From).Get_EquipmentIDStatus(n_Port).szEquipID == EQUIPMENTID) {
					CString EQUIPMENTID(Get_Server(IN_From).Get_EquipmentIDStatus(n_Port).szEquipID);
					CString SUBEQPID(Get_Server(IN_From).Get_SubEqpID());
					CString PORTID(Get_Server(IN_From).Get_EquipmentIDStatus(n_Port).szPortID);
					CString EQUIPMENTSTATE(g_szEquipment_State[Get_Server(IN_From).Get_PortStatus(n_Port).nEquipmentState]);
					//CString LOTID(Get_Equipment(i).Get_FromPortToLOTID(n_Port));
					OnSend_REPLY_EQUIPMENT_STATE_DISPLAY(
						IN_From,
						Get_Server(IN_From).GetXmlEes().Set_ReplyEquipMentStateDisplayParameter(
							transactionId,
							EQUIPMENTID,
							SUBEQPID,
							PORTID,
							EQUIPMENTSTATEDISPLAY,//EQUIPMENTSTATE,
							RGBDISPLAY));

					for (UINT nIdx = 0; nIdx < Get_EESCount(); nIdx++)
					{
						if (0 == Get_EESEqpCode(nIdx).Compare(EQUIPMENTSTATEDISPLAY)) {
							dsp.Dsp = IN_PARA;
							dsp.EES = &Get_EES(nIdx);
							Get_Server(IN_From).Set_Notify_EquipmentStateDisplay(&dsp);
						}
					}
				}
			}
			for (int i = 0; i < Get_EquipmentCount(); i++) {
				if (Get_Equipment(i).Get_ClientConnection()) {
					for (int n_Port = 0; n_Port < Get_Equipment(i).Get_EquipmentIDCount(); n_Port++) {
						if (Get_Equipment(i).Get_EquipmentIDStatus(n_Port).szEquipID == EQUIPMENTID) {

							CString EQUIPMENTID(Get_Equipment(i).Get_EquipmentIDStatus(n_Port).szEquipID);
							//2023.06.28a uhkim
							//CString SUBEQPID(Get_Equipment(i).Get_SubEqpID());
							CString SUBEQPID(Get_Server(IN_From).Get_SubEqpID());
							CString PORTID(Get_Equipment(i).Get_EquipmentIDStatus(n_Port).szPortID);
							CString EQUIPMENTSTATE(g_szEquipment_State[Get_Equipment(i).Get_PortStatus(n_Port).nEquipmentState]);
							CString LOTID(Get_Equipment(i).Get_FromPortToLOTID(n_Port));
							OnSend_REPLY_EQUIPMENT_STATE_DISPLAY(
								IN_From,
								Get_Equipment(IN_From).GetXmlEes().Set_ReplyEquipMentStateDisplayParameter(
									transactionId,
									EQUIPMENTID,
									SUBEQPID,
									PORTID,
									EQUIPMENTSTATEDISPLAY,//EQUIPMENTSTATE,
									RGBDISPLAY));

							for (UINT nIdx = 0; nIdx < Get_EESCount(); nIdx++)
							{
								if (0 == Get_EESEqpCode(nIdx).Compare(EQUIPMENTSTATEDISPLAY)) {
									dsp.Dsp = IN_PARA;
									dsp.EES = &Get_EES(nIdx);
									Get_Equipment(i).Set_Notify_EquipmentStateDisplay(&dsp);
								}
							}
						}
					}
				}
				
			}
		}
	}

	/*
	if (Get_ServerID(IN_From).GetBuffer() == EQUIPMENTID) {
		if (Get_ServerID(IN_From).GetBuffer() == SUBEQPID) {
			if (Get_Server(IN_From).Get_ClientConnection()) {
				if (IN_From < Get_ServerCount())
				{

				}
			}
		}
		//
		for (int i = 0; i < Get_EquipmentCount(); i++) {
			if (Get_Equipment(i).Get_Alias() == SUBEQPID) {
				if (Get_Equipment(i).Get_ClientConnection()) {
					OnSend_REPLY_EQUIPMENT_STATE_DISPLAY(
						IN_From,
						Get_Server(IN_From).GetXmlEes().Set_ReplyEquipMentStateDisplayParameter(
							transactionId,
							SUBEQPID,
							PORTID,
							EQUIPMENTSTATEDISPLAY,
							RGBDISPLAY));

					for (UINT nIdx = 0; nIdx < Get_EESCount(); nIdx++)
					{
						if (0 == Get_EESEqpCode(nIdx).Compare(EQUIPMENTSTATEDISPLAY)) {
							dsp.Dsp = IN_PARA;
							dsp.EES = &Get_EES(nIdx);
							Get_Equipment(i).Set_Notify_EquipmentStateDisplay(&dsp);
						}
					}
				}
			}
		}
	}*/
	//2023.06.28a
	if (MainFrame->FindLPARAM(IN_Data)) {
		delete IN_PARA;
		IN_PARA = nullptr;
		MainFrame->RemoveLPARAM(IN_Data);
	}
#endif
}
void CTestManager_EQP_ICS::OnEvent_ServerREPLY_EQUIPMENT_STATE_DISPLAY(__in uint8_t IN_From, __in LPARAM IN_Data)
{
#if TEST
	ST_xml_REPLY_EQUIPMENT_STATE_DISPLAY* IN_PARA = (ST_xml_REPLY_EQUIPMENT_STATE_DISPLAY*)IN_Data;
	//서버 이벤트 처리.	
	//CString SUBEQPID(IN_PARA->Body.SUBEQPID);
	//CString EQUIPMENTSTATEDISPLAY(IN_PARA->Body.EQUIPMENTSTATE);
	//CString RGBDISPLAY(IN_PARA->Body.LOTID);
	CString SUBEQPID(IN_PARA->Body.SUBEQPID);
	if (Get_ServerID(IN_From).GetBuffer() == SUBEQPID) {
		if (Get_Server(IN_From).Get_ClientConnection()) {
			if (IN_From < Get_ServerCount())
			{
				/*
				OnSend_REPLY_EQUIPMENT_STATE_DISPLAY(
					IN_From,
					Svr.Set_ReplyEquipMentStateDisplayParameter(
						SUBEQPID,
						EQUIPMENTSTATEDISPLAY,
						RGBDISPLAY));
				*/
			}
		}
	}
	//설비 이벤트 처리
	for (int i = 0; i < Get_EquipmentCount(); i++)
	{
		if (Get_EquipmentID(i).GetBuffer() == SUBEQPID) {
			if (Get_Equipment(i).Get_ClientConnection()) {

			}
		}
	}
	//2023.06.28a
	if (MainFrame->FindLPARAM(IN_Data)) {
		delete IN_PARA;
		IN_PARA = nullptr;
		MainFrame->RemoveLPARAM(IN_Data);
	}
#endif
}
void CTestManager_EQP_ICS::OnEvent_ServerREPORT_LOSS_STATE(__in uint8_t IN_From, __in LPARAM IN_Data)
{
#if TEST
	ST_xml_REPORT_LOSS_STATE* IN_PARA = (ST_xml_REPORT_LOSS_STATE*)IN_Data;
	//서버 이벤트 처리.	
	CString SUBEQPID(IN_PARA->Body.SUBEQPID);
	CString EQUIPMENTSTATE(IN_PARA->Body.EQUIPMENTSTATE);
	CString LOTID(IN_PARA->Body.LOTID);
	CString LOSSSTATE(IN_PARA->Body.LOSSSTATE);
	CString LOSSCOUNT(IN_PARA->Body.LOSSCOUNT);

	if (Get_ServerID(IN_From).GetBuffer() == SUBEQPID) {
		if (Get_Server(IN_From).Get_ClientConnection()) {
			if (IN_From < Get_ServerCount())
			{
				OnSend_REPORT_LOSS_STATE(
					IN_From,
					IN_PARA);
			}
		}
	}
	//설비 이벤트 처리
	for (int i = 0; i < Get_EquipmentCount(); i++)
	{
		if (Get_EquipmentID(i).GetBuffer() == SUBEQPID) {
			if (Get_Equipment(i).Get_ClientConnection()) {

			}
		}
	}
	//2023.06.28a
	if (MainFrame->FindLPARAM(IN_Data)) {
		delete IN_PARA;
		IN_PARA = nullptr;
		MainFrame->RemoveLPARAM(IN_Data);
	}
#endif
}
void CTestManager_EQP_ICS::OnEvent_ServerREQUEST_LOSS_WINDOW(__in uint8_t IN_From, __in LPARAM IN_Data)
{
#if TEST
	ST_xml_REQUEST_LOSS_WINDOW* IN_PARA = (ST_xml_REQUEST_LOSS_WINDOW*)IN_Data;

	CString SUBEQPID(IN_PARA->Body.SUBEQPID);
	CString LOSSWINDOW(IN_PARA->Body.LOSSWINDOW);

	//서버 이벤트 처리.	
	if (Get_ServerID(IN_From).GetBuffer() == SUBEQPID) {
		if (Get_Server(IN_From).Get_ClientConnection()) {
			if (IN_From < Get_ServerCount())
			{
				//서버 보고 이벤트
				OnSend_REPLY_LOSS_WINDOW(
					IN_From,
					Get_Server(IN_From).GetXmlEes().Set_ReplyLossWindowParameter(
						SUBEQPID,
						LOSSWINDOW));
				//Data 및 이벤트 처리.
				Get_Server(IN_From).Set_Notify_LOSSWINDOW(LOSSWINDOW);
			}
		}
	}
	//설비 이벤트 처리
	for (int i = 0; i < Get_EquipmentCount(); i++)
	{
		if (Get_EquipmentID(i).GetBuffer() == SUBEQPID) {
			if (Get_Equipment(i).Get_ClientConnection()) {

			}
		}
	}
	//2023.06.28a
	if (MainFrame->FindLPARAM(IN_Data)) {
		delete IN_PARA;
		IN_PARA = nullptr;
		MainFrame->RemoveLPARAM(IN_Data);
	}
#endif
}
void CTestManager_EQP_ICS::OnEvent_ServerREPLY_LOSS_WINDOW(__in uint8_t IN_From, __in LPARAM IN_Data)
{
#if TEST
	CServer Svr;
	ST_xml_REPLY_LOSS_WINDOW* IN_PARA = (ST_xml_REPLY_LOSS_WINDOW*)IN_Data;

	CString SUBEQPID(IN_PARA->Body.SUBEQPID);
	CString LOSSWINDOW(IN_PARA->Body.LOSSWINDOW);

	//서버 이벤트 처리.	
	if (Get_ServerID(IN_From).GetBuffer() == SUBEQPID) {
		if (Get_Server(IN_From).Get_ClientConnection()) {
			if (IN_From < Get_ServerCount())
			{

			}
		}
	}
	//설비 이벤트 처리
	for (int i = 0; i < Get_EquipmentCount(); i++)
	{
		if (Get_EquipmentID(i).GetBuffer() == SUBEQPID) {
			if (Get_Equipment(i).Get_ClientConnection()) {

			}
		}
	}
	//2023.06.28a
	if (MainFrame->FindLPARAM(IN_Data)) {
		delete IN_PARA;
		IN_PARA = nullptr;
		MainFrame->RemoveLPARAM(IN_Data);
	}
#endif
}
void CTestManager_EQP_ICS::OnEvent_ServerREPORT_ALARM_STATE(__in uint8_t IN_From, __in LPARAM IN_Data)
{
#if TEST
	CServer Svr;
	ST_xml_REPORT_ALARM_STATE* IN_PARA = (ST_xml_REPORT_ALARM_STATE*)IN_Data;
	//서버 이벤트 처리.	
	CString LOTID(IN_PARA->Body.LOTID);
	CString SUBEQPID(IN_PARA->Body.SUBEQPID);
	CString ALARMCODE(IN_PARA->Body.ALARMCODE);
	CString ALARMID(IN_PARA->Body.ALARMID);
	CString ALARMMESSAGE(IN_PARA->Body.ALARMMESSAGE);	

	/*
	if (IN_From < Get_ServerCount()) {
		if (Get_Server(IN_From).Get_ClientConnection()) {
			for (int n_Port = 0; n_Port < Get_Server(IN_From).Get_EquipmentIDCount(); n_Port++) {
				if (Get_Server(IN_From).Get_EquipmentIDStatus(n_Port).szEquipID == EQUIPMENTID) {

				}
			}
			for (int i = 0; i < Get_EquipmentCount(); i++) {
				if (Get_Equipment(i).Get_ClientConnection()) {
					for (int n_Port = 0; n_Port < Get_Equipment(i).Get_EquipmentIDCount(); n_Port++) {
						if (Get_Equipment(i).Get_EquipmentIDStatus(n_Port).szEquipID == EQUIPMENTID) {

							CString EQUIPMENTID(Get_Equipment(i).Get_EquipmentIDStatus(n_Port).szEquipID);
							CString SUBEQPID(Get_Equipment(i).Get_SubEqpID());
							CString PORTID(Get_Equipment(i).Get_EquipmentIDStatus(n_Port).szPortID);
							CString EQUIPMENTSTATE(g_szEquipment_State[Get_Equipment(i).Get_PortStatus(n_Port).nEquipmentState]);
							CString LOTID(Get_Equipment(i).Get_FromPortToLOTID(n_Port));
							OnSend_REPLY_EQUIPMENT_STATE_DISPLAY(
								IN_From,
								Get_Server(IN_From).GetXmlEes().Set_ReplyEquipMentStateDisplayParameter(
									transactionId,
									EQUIPMENTID,
									SUBEQPID,
									PORTID,
									EQUIPMENTSTATE,
									RGBDISPLAY));

							for (UINT nIdx = 0; nIdx < Get_EESCount(); nIdx++)
							{
								if (0 == Get_EESEqpCode(nIdx).Compare(EQUIPMENTSTATEDISPLAY)) {
									dsp.Dsp = IN_PARA;
									dsp.EES = &Get_EES(nIdx);
									Get_Equipment(i).Set_Notify_EquipmentStateDisplay(&dsp);
								}
							}
						}
					}
				}

			}
		}
	}

	if (Get_ServerID(IN_From).GetBuffer() == SUBEQPID) {
		if (Get_Server(IN_From).Get_ClientConnection()) {
			if (IN_From < Get_ServerCount())
			{
				OnSend_REPORT_ALARM_STATE(
					IN_From,
					IN_PARA);
			}
		}
	}
	//설비 이벤트 처리
	for (int i = 0; i < Get_EquipmentCount(); i++)
	{
		if (Get_EquipmentID(i).GetBuffer() == SUBEQPID) {
			if (Get_Equipment(i).Get_ClientConnection()) {

			}
		}
	}*/
	//2023.06.28a
	if (MainFrame->FindLPARAM(IN_Data)) {
		delete IN_PARA;
		IN_PARA = nullptr;
		MainFrame->RemoveLPARAM(IN_Data);
	}
#endif
}
void CTestManager_EQP_ICS::OnEvent_ServerREQUEST_ALARM_LIST(__in uint8_t IN_From, __in LPARAM IN_Data)
{
#if TEST
	ST_xml_REQUEST_ALARM_LIST* IN_PARA = (ST_xml_REQUEST_ALARM_LIST*)IN_Data;

	CString transactionId(IN_PARA->Hd.transactionId);
	CString EQUIPMENTID(IN_PARA->Body.EQUIPMENTID);
	CString SUBEQPID(IN_PARA->Body.SUBEQPID);
	CString IPADDRESS(IN_PARA->Body.IPADDRESS);
	CString returnCode;
	CString returnMessage;

	if (IN_From < Get_ServerCount()) {
		if (Get_Server(IN_From).Get_ClientConnection()) {
			for (int n_Port = 0; n_Port < Get_Server(IN_From).Get_EquipmentIDCount(); n_Port++) {
				if (Get_Server(IN_From).Get_EquipmentIDStatus(n_Port).szEquipID == EQUIPMENTID) {
					CString EQUIPMENTID(Get_Server(IN_From).Get_EquipmentIDStatus(n_Port).szEquipID);
					CString SUBEQPID(Get_Server(IN_From).Get_SubEqpID());
					CString PORTID(Get_Server(IN_From).Get_EquipmentIDStatus(n_Port).szPortID);
					CString EQUIPMENTSTATE(g_szEquipment_State[Get_Server(IN_From).Get_PortStatus(n_Port).nEquipmentState]);
					//CString LOTID(Get_Equipment(i).Get_FromPortToLOTID(n_Port));
					OnSend_REPLY_ALARM_LIST(
						IN_From,
						Get_Server(IN_From).GetXmlEes().Set_ReplyAlarmListParameter(
							transactionId,
							EQUIPMENTID,
							SUBEQPID,
							IPADDRESS,
							returnCode,//EQUIPMENTSTATE,
							returnMessage));
				}
			}
		}
	}

	//서버 이벤트 처리.	
	/*
	if (Get_ServerID(IN_From).GetBuffer() == SUBEQPID) {
		if (Get_Server(IN_From).Get_ClientConnection()) {
			if (IN_From < Get_ServerCount())
			{
				//서버 보고 이벤트
				OnSend_REPLY_ALARM_LIST(
					IN_From,
					Svr.Set_ReplyAlarmListParameter(
						SUBEQPID));
			}
		}
	}
	//설비 이벤트 처리
	for (int i = 0; i < Get_EquipmentCount(); i++)
	{
		if (Get_EquipmentID(i).GetBuffer() == SUBEQPID) {
			if (Get_Equipment(i).Get_ClientConnection()) {

			}
		}
	}*/
	//2023.06.28a
	if (MainFrame->FindLPARAM(IN_Data)) {
		delete IN_PARA;
		IN_PARA = nullptr;
		MainFrame->RemoveLPARAM(IN_Data);
	}
#endif
}
void CTestManager_EQP_ICS::OnEvent_ServerREPLY_ALARM_LIST(__in uint8_t IN_From, __in LPARAM IN_Data)
{
#if TEST
	ST_xml_REPLY_ALARM_LIST* IN_PARA = (ST_xml_REPLY_ALARM_LIST*)IN_Data;

	CString SUBEQPID(IN_PARA->Body.SUBEQPID);

	//서버 이벤트 처리.	
	if (Get_ServerID(IN_From).GetBuffer() == SUBEQPID) {
		if (Get_Server(IN_From).Get_ClientConnection()) {
			if (IN_From < Get_ServerCount())
			{

			}
		}
	}
	//설비 이벤트 처리
	for (int i = 0; i < Get_EquipmentCount(); i++)
	{
		if (Get_EquipmentID(i).GetBuffer() == SUBEQPID) {
			if (Get_Equipment(i).Get_ClientConnection()) {

			}
		}
	}
	//2023.06.28a
	if (MainFrame->FindLPARAM(IN_Data)) {
		delete IN_PARA;
		IN_PARA = nullptr;
		MainFrame->RemoveLPARAM(IN_Data);
	}
#endif
}



//void CTestManager_EQP_ICS::OnSend_EESMode( __in enEES_Mode IN_nLevel)
//{

//}

void CTestManager_EQP_ICS::OnEvent_ServerREPORT_RMS_MODE(__in uint8_t IN_From, __in LPARAM IN_Data)
{
#if TEST
	ST_xml_REPORT_RMS_MODE* IN_PARA = (ST_xml_REPORT_RMS_MODE*)IN_Data;
	
	CString EQUIPMENTID(IN_PARA->Body.EQUIPMENTID);
	CString LOTID(IN_PARA->Body.LOTID);
	CString SUBEQPID(IN_PARA->Body.SUBEQPID);
	CString EESMODE(IN_PARA->Body.EESMODE);
	
	if (0 < Get_ServerCount()) {
		for (int IN_From = 0; IN_From < Get_ServerCount(); IN_From++) {
			if (Get_Server(IN_From).Get_ClientConnection()) {
				for (int n_Port = 0; n_Port < Get_Server(IN_From).Get_EquipmentIDCount(); n_Port++) {
					OnSend_REPORT_RMS_MODE(
						IN_From,
						IN_PARA);
					Get_Server(IN_From).Set_Notify_RMS(EESMODE);
					Get_Server(IN_From).GetXmlEes().SetBaseDataPARA(SV_EES_MODE, EESMODE);
				}
			}
		}
	}
	for (int IN_From = 0; IN_From < Get_EquipmentCount(); IN_From++) {
		Get_Equipment(IN_From).GetXmlEes().SetBaseDataPARA(SV_EES_MODE, EESMODE);
	}
	//2023.06.28a
	if (MainFrame->FindLPARAM(IN_Data)) {
		delete IN_PARA;
		IN_PARA = nullptr;
		MainFrame->RemoveLPARAM(IN_Data);
	}
#endif
}


void CTestManager_EQP_ICS::OnEvent_ServerREQUEST_SET_DATETIME(__in uint8_t IN_From, __in LPARAM IN_Data)
{
#if TEST
	ST_xml_REQUEST_SET_DATETIME* IN_PARA = (ST_xml_REQUEST_SET_DATETIME*)IN_Data;


	CString transactionId(IN_PARA->Hd.transactionId);	
	CString EQUIPMENTID(IN_PARA->Body.EQUIPMENTID);
	CString IPADDRESS(IN_PARA->Body.IPADDRESS);
	CString DATETIME(IN_PARA->Body.DATETIME);

	CString szTemp;
	SYSTEMTIME time;
	SYSTEMTIME tmLocal;
	GetLocalTime(&tmLocal);

	if (0 < Get_ServerCount()) {
		for (int IN_From = 0; IN_From < Get_ServerCount(); IN_From++) {
			if (Get_Server(IN_From).Get_ClientConnection()) {
				for (int n_Port = 0; n_Port < Get_Server(IN_From).Get_EquipmentIDCount(); n_Port++) {
					if (GetNtPrivilege()) {

						time.wYear = _ttoi(DATETIME.Mid(0, 4));
						time.wMonth = _ttoi(DATETIME.Mid(4, 2));

						time.wDayOfWeek = tmLocal.wDayOfWeek;

						time.wDay = _ttoi(DATETIME.Mid(6, 2));
						time.wHour = _ttoi(DATETIME.Mid(8, 2));
						time.wMinute = _ttoi(DATETIME.Mid(10, 2));
						time.wSecond = _ttoi(DATETIME.Mid(12, 2));
						//time.wMilliseconds = _ttoi(DATETIME.Mid(14, 3));
						time.wMilliseconds = 0;

						SetLocalTime(&time);
						//Get_Server(IN_From).Set_TimeSync(true);
						OnSend_REPLY_SET_DATETIME(
							IN_From,
							Get_Server(IN_From).GetXmlEes().Set_ReplySetDateTimeParameter(
								transactionId,
								DATETIME));
					}
				}
			}
		}
	}
	//2023.06.28a
	if (MainFrame->FindLPARAM(IN_Data)) {
		delete IN_PARA;
		IN_PARA = nullptr;
		MainFrame->RemoveLPARAM(IN_Data);
	}
#endif
}
void CTestManager_EQP_ICS::OnEvent_ServerREPLY_SET_DATETIME(__in uint8_t IN_From, __in LPARAM IN_Data)
{
#if TEST
	ST_xml_REPLY_SET_DATETIME* IN_PARA = (ST_xml_REPLY_SET_DATETIME*)IN_Data;
	CString transactionId(IN_PARA->Hd.transactionId);

	CString EQUIPMENTID(IN_PARA->Body.EQUIPMENTID);
	CString IPADDRESS(IN_PARA->Body.IPADDRESS);
	CString DATETIME(IN_PARA->Body.DATETIME);

	CString szTemp;
	SYSTEMTIME time;
	SYSTEMTIME tmLocal;
	GetLocalTime(&tmLocal);

	if (0 < Get_ServerCount()) {
		for (int IN_From = 0; IN_From < Get_ServerCount(); IN_From++) {
			if (Get_Server(IN_From).Get_ClientConnection()) {
				for (int n_Port = 0; n_Port < Get_Server(IN_From).Get_EquipmentIDCount(); n_Port++) {
					if (GetNtPrivilege()) {

						time.wYear = _ttoi(DATETIME.Mid(0, 4));
						time.wMonth = _ttoi(DATETIME.Mid(4, 2));

						time.wDayOfWeek = tmLocal.wDayOfWeek;

						time.wDay = _ttoi(DATETIME.Mid(6, 2));
						time.wHour = _ttoi(DATETIME.Mid(8, 2));
						time.wMinute = _ttoi(DATETIME.Mid(10, 2));
						time.wSecond = _ttoi(DATETIME.Mid(12, 2));
						//time.wMilliseconds = _ttoi(DATETIME.Mid(14, 3));
						time.wMilliseconds = 0;

						SetLocalTime(&time);
						//Get_Server(IN_From).Set_TimeSync(true);
						OnSend_REPLY_SET_DATETIME(
							IN_From,
							IN_PARA);
							//Get_Server(IN_From).GetXmlEes().Set_ReplySetDateTimeParameter(
							//	transactionId,
							//	DATETIME));
					}
				}
			}
		}
	}
	/*
	for (int i = 0; i < Get_EquipmentCount(); i++)
	{
		if (Get_Equipment(i).Get_Alias().GetBuffer() == SUBEQPID) {
			if (Get_Equipment(i).Get_ClientConnection()) {
			}
		}
	}*/
	//2023.06.28a
	if (MainFrame->FindLPARAM(IN_Data)) {
		delete IN_PARA;
		IN_PARA = nullptr;
		MainFrame->RemoveLPARAM(IN_Data);
	}
#endif
}

void CTestManager_EQP_ICS::OnEvent_ServerREQUEST_TERMINAL_MESSAGE(__in uint8_t IN_From, __in LPARAM IN_Data)
{	
#if TEST
	ST_xml_REQUEST_TERMINAL_MESSAGE* IN_PARA = (ST_xml_REQUEST_TERMINAL_MESSAGE*)IN_Data;
	CString transactionId(IN_PARA->Hd.transactionId);

	CString EQUIPMENTID(IN_PARA->Body.EQUIPMENTID);
	CString SUBEQPID(IN_PARA->Body.SUBEQPID);
	CString IPADDRESS(IN_PARA->Body.IPADDRESS);
	CString TERMINALMESSAGE(IN_PARA->Body.TERMINALMESSAGE);

	if (0 < Get_ServerCount()) {
		for (int IN_From = 0; IN_From < Get_ServerCount(); IN_From++) {
			if (Get_Server(IN_From).Get_ClientConnection()) {
				for (int n_Port = 0; n_Port < Get_Server(IN_From).Get_EquipmentIDCount(); n_Port++) {
					OnReport_TerminalMessage(IN_PARA);
					OnSend_REPLY_TERMINAL_MESSAGE(
						IN_From,
						Get_Server(IN_From).GetXmlEes().Set_ReplyTerminalMessageParameter(
							transactionId,
							SUBEQPID,
							TERMINALMESSAGE));
					Get_Server(IN_From).Set_Notify_TERMINAL_MESSAGE(IN_PARA);
				}
			}
		}
	}
	/*
	if (Get_ServerID(IN_From).GetBuffer() == EQUIPMENTID) {
		if (Get_ServerAlias(IN_From).GetBuffer() == SUBEQPID) {
			if (Get_Server(IN_From).Get_ClientConnection()) {
				if (IN_From < Get_ServerCount())
				{
					OnReport_TerminalMessage(IN_PARA);
					OnSend_REPLY_TERMINAL_MESSAGE(
						IN_From,
						Get_Server(IN_From).GetXmlEes().Set_ReplyTerminalMessageParameter(
							transactionId,
							SUBEQPID,
							TERMINALMESSAGE));
					Get_Server(IN_From).Set_Notify_TERMINAL_MESSAGE(IN_PARA);
				}
			}
		}
		for (int i = 0; i < Get_EquipmentCount(); i++)
		{
			if (Get_Equipment(i).Get_Alias().GetBuffer() == SUBEQPID) {
				if (Get_Equipment(i).Get_ClientConnection()) {
				}
			}
		}
	}*/
	//2023.06.28a
	if (MainFrame->FindLPARAM(IN_Data)) {
		delete IN_PARA;
		IN_PARA = nullptr;
		MainFrame->RemoveLPARAM(IN_Data);
	}
#endif
}
void CTestManager_EQP_ICS::OnEvent_ServerREPLY_TERMINAL_MESSAGE(__in uint8_t IN_From, __in LPARAM IN_Data)
{
#if TEST
	ST_xml_REPLY_TERMINAL_MESSAGE* IN_PARA = (ST_xml_REPLY_TERMINAL_MESSAGE*)IN_Data;

	CString EQUIPMENTID(IN_PARA->Body.EQUIPMENTID);
	CString SUBEQPID(IN_PARA->Body.SUBEQPID);

	if (0 < Get_ServerCount()) {
		for (int IN_From = 0; IN_From < Get_ServerCount(); IN_From++) {
			if (Get_Server(IN_From).Get_ClientConnection()) {
				for (int n_Port = 0; n_Port < Get_Server(IN_From).Get_EquipmentIDCount(); n_Port++) {
					OnSend_REPLY_TERMINAL_MESSAGE(
						IN_From,
						IN_PARA);
				}
			}
		}
	}
	//2023.06.28a
	if (MainFrame->FindLPARAM(IN_Data)) {
		delete IN_PARA;
		IN_PARA = nullptr;
		MainFrame->RemoveLPARAM(IN_Data);
	}
#endif
}

void CTestManager_EQP_ICS::OnEvent_ServerREQUEST_OPCALL_MESSAGE(__in uint8_t IN_From, __in LPARAM IN_Data)
{
#if TEST
	ST_xml_REQUEST_OPCALL* IN_PARA = (ST_xml_REQUEST_OPCALL*)IN_Data;

	CString EQUIPMENTID(IN_PARA->Body.EQUIPMENTID);
	CString SUBEQPID(IN_PARA->Body.SUBEQPID);
	CString TERMINALMESSAGE(IN_PARA->Body.MESSAGE);
	CString BUZZER(IN_PARA->Body.BUZZER);
	CString TOWERLAMP(IN_PARA->Body.TOWERLAMP);

	if (0 < Get_ServerCount()) {
		for (int IN_From = 0; IN_From < Get_ServerCount(); IN_From++) {
			if (Get_Server(IN_From).Get_ClientConnection()) {
				for (int n_Port = 0; n_Port < Get_Server(IN_From).Get_EquipmentIDCount(); n_Port++) {
					OnReport_OpCall(IN_PARA);
					Get_Server(IN_From).Set_Notify_OPCALL(IN_PARA);
				}
			}
		}
	}
	//2023.06.28a
	if (MainFrame->FindLPARAM(IN_Data)) {
		delete IN_PARA;
		IN_PARA = nullptr;
		MainFrame->RemoveLPARAM(IN_Data);
	}
#endif
}

void CTestManager_EQP_ICS::OnEvent_ServerREPLY_OPCALL_MESSAGE(__in uint8_t IN_From, __in LPARAM IN_Data)
{
#if TEST
	ST_xml_REPLY_OPCALL* IN_PARA = (ST_xml_REPLY_OPCALL*)IN_Data;
	CString transactionId(IN_PARA->Hd.transactionId);

	CString EQUIPMENTID(IN_PARA->Body.EQUIPMENTID);
	CString SUBEQPID(IN_PARA->Body.SUBEQPID);
	CString TERMINALMESSAGE(IN_PARA->Body.MESSAGE);
	CString BUZZER(IN_PARA->Body.BUZZER);
	CString TOWERLAMP(IN_PARA->Body.TOWERLAMP);

	if (0 < Get_ServerCount()) {
		for (int IN_From = 0; IN_From < Get_ServerCount(); IN_From++) {
			if (Get_Server(IN_From).Get_ClientConnection()) {
				for (int n_Port = 0; n_Port < Get_Server(IN_From).Get_EquipmentIDCount(); n_Port++) {
					OnSend_REPLY_OPCALL(
						IN_From,
						IN_PARA);
				}
			}
		}
	}
	/*
	if (Get_ServerID(IN_From).GetBuffer() == EQUIPMENTID) {
		if (Get_ServerAlias(IN_From).GetBuffer() == SUBEQPID) {
			if (Get_Server(IN_From).Get_ClientConnection()) {
				if (IN_From < Get_ServerCount())
				{
					OnSend_REPLY_OPCALL(
						IN_From,
						Get_Server(IN_From).GetXmlEes().Set_ReplyOpCallParameter(
							transactionId,
							SUBEQPID,
							TERMINALMESSAGE,
							BUZZER,
							TOWERLAMP));
				}
			}
		}

		for (int i = 0; i < Get_EquipmentCount(); i++)
		{
			if (Get_Equipment(i).Get_Alias().GetBuffer() == SUBEQPID) {
				if (Get_Equipment(i).Get_ClientConnection()) {

				}
			}
		}
	}*/
	//2023.06.28a
	if (MainFrame->FindLPARAM(IN_Data)) {
		delete IN_PARA;
		IN_PARA = nullptr;
		MainFrame->RemoveLPARAM(IN_Data);
	}
#endif
}

//=============================================================================
// Method		: OnEvent_Equipment
// Access		:   
// Returns		: void
// Parameter	: 
// Qualifier	:
// Last Update	: 2022/2/9 - 14:46
// Desc.		: 개별 Equipment를 이벤트에 대한 데이터 처리.
//=============================================================================
void CTestManager_EQP_ICS::OnEvent_EquipmentREPORT_ONLINE_STATE(__in uint8_t IN_From, __in LPARAM IN_Data)
{
}
void CTestManager_EQP_ICS::OnEvent_EquipmentREPORT_USER_CHANGE(__in uint8_t IN_From, __in LPARAM IN_Data)
{
}
void CTestManager_EQP_ICS::OnEvent_EquipmentREPLY_USER_COMMAND(__in uint8_t IN_From, __in LPARAM IN_Data)
{
}
//PtS_Empty = 0,	// 0 : 비어 있음
//PtS_Exist_Socket,			// 1 : 제품 있음	
//PtS_Wait_Out,				// 2 : 배출대기
//PtS_Disable,				// 3 : 비활성
//PtS_Alarm,					// 4 : Alarm
void CTestManager_EQP_ICS::OnEvent_EquipmentREPORT_EQUIPMENT_STATE(__in uint8_t IN_From, __in LPARAM IN_Data)
{
	//UI 처리
	if (!Get_Equipment(IN_From).Get_ClientConnection())
		return;
	for (int nSvr = 0; nSvr < Get_ServerCount(); nSvr++) {

		
		for (int nPort = 0; nPort < Get_Equipment(IN_From).Get_mEES_PortSubStatusCount(); nPort++) {
			/*
			auto a = Get_Equipment(IN_From).Get_mEES_PortSubStatus(nPort).Get_nOldOperMode();
			auto b = Get_Equipment(IN_From).Get_mEES_PortSubStatus(nPort).Get_nOldProcessStatus();
			auto c = Get_Equipment(IN_From).Get_mEES_PortSubStatus(nPort).Get_nOldPortStatus();
			auto d = Get_Equipment(IN_From).Get_mEES_PortSubStatus(nPort).Get_nOperMode();
			auto e = Get_Equipment(IN_From).Get_mEES_PortSubStatus(nPort).Get_nProcessStatus();
			auto f = Get_Equipment(IN_From).Get_mEES_PortSubStatus(nPort).Get_nPortStatus();
			if (Get_Equipment(IN_From).Get_mEES_PortSubStatus(nPort).Get_nOldOperMode() == Get_Equipment(IN_From).Get_mEES_PortSubStatus(nPort).Get_nOperMode() &&	//작업 동작.
				Get_Equipment(IN_From).Get_mEES_PortSubStatus(nPort).Get_nOldProcessStatus() == Get_Equipment(IN_From).Get_mEES_PortSubStatus(nPort).Get_nProcessStatus() &&
				Get_Equipment(IN_From).Get_mEES_PortSubStatus(nPort).Get_nOldPortStatus() == Get_Equipment(IN_From).Get_mEES_PortSubStatus(nPort).Get_nPortStatus()) {
				continue;
			}*/
			auto nPortStatus = Get_Equipment(IN_From).Get_mEES_PortSubStatus(nPort).Get_nPortStatus();
			auto nProcessStatus = Get_Equipment(IN_From).Get_mEES_PortSubStatus(nPort).Get_nProcessStatus();
			switch (nProcessStatus)
			{
			case enEqpProcessStatus::EPC_None:
				if (nPortStatus == PtS_Exist_Socket) {
					Get_Equipment(IN_From).Get_mEES_PortSubStatus(nPort).Set_nEquipmentStatus(ES_STOP);
				}	else {
					Get_Equipment(IN_From).Get_mEES_PortSubStatus(nPort).Set_nEquipmentStatus(ES_IDLE);
				}
				break;
			case enEqpProcessStatus::EPC_Init:
				if (nPortStatus == PtS_Exist_Socket) {
					Get_Equipment(IN_From).Get_mEES_PortSubStatus(nPort).Set_nEquipmentStatus(ES_STOP);
				}
				else {
					Get_Equipment(IN_From).Get_mEES_PortSubStatus(nPort).Set_nEquipmentStatus(ES_IDLE);
				}
				break;
			case enEqpProcessStatus::EPC_Idle:
				if (nPortStatus == PtS_Exist_Socket) {
					Get_Equipment(IN_From).Get_mEES_PortSubStatus(nPort).Set_nEquipmentStatus(ES_STOP);
				}
				else {
					Get_Equipment(IN_From).Get_mEES_PortSubStatus(nPort).Set_nEquipmentStatus(ES_IDLE);
				}
				break;
			case enEqpProcessStatus::EPC_Run:
				if (nPortStatus == PtS_RUN) {
					Get_Equipment(IN_From).Get_mEES_PortSubStatus(nPort).Set_nEquipmentStatus(ES_RUN);
				}
				else {
					Get_Equipment(IN_From).Get_mEES_PortSubStatus(nPort).Set_nEquipmentStatus(ES_IDLE);
				}
				break;
			case enEqpProcessStatus::EPC_Alarm:
				Get_Equipment(IN_From).Get_mEES_PortSubStatus(nPort).Set_nEquipmentStatus(ES_SUDDENSTOP);
				break;
			}
			//if (Get_Equipment(IN_From).Get_mEES_PortSubStatus(nPort).Get_nOldEquipmentStatus() != Get_Equipment(IN_From).Get_mEES_PortSubStatus(nPort).Get_nEquipmentStatus()) 
			{
				auto Args = Get_Server(nSvr).Create_Report_Equipment_StateArgs(nullptr);
				auto cntr = Add_Equipment_StateArgs(Get_ServerID(nSvr), *Args);
				cntr.REPORT.Body.Set_EQUIPMENTID(lt::ToMultiByte(Get_Equipment(IN_From).Get_EquipmentIDStatus(nPort).Get_EQUIPMENTID()));
				cntr.REPORT.Body.Set_EQUIPMENTSTATE(lt::ToMultiByte(g_szEquipment_State[Get_Equipment(IN_From).Get_mEES_PortSubStatus(nPort).Get_nOldEquipmentStatus()]));
				Get_Equipment(IN_From).Set_Notify_EquipmentState(&cntr.REPORT);

				if (!Get_Server(nSvr).Get_ClientConnection()) {
					RemoveEquipmentStateProcedure(Get_ServerID(nSvr).GetBuffer(), cntr.REPORT.Hd.Get_transactionId());
					continue;
				}
				OnSend_REPORT_EQUIPMENT_STATE(nSvr, cntr.REPORT);
			}
			Get_Equipment(IN_From).Get_mEES_PortSubStatus(nPort).Set_nOldEquipmentStatus(Get_Equipment(IN_From).Get_mEES_PortSubStatus(nPort).Get_nEquipmentStatus());
		}
		if (IN_Data == enEqpProcessStatus::EPC_Alarm) {
			OnEvent_EquipmentREPORT_ALARM_STATE(IN_From, ALARMSET_SET);
		}
		else {
			OnEvent_EquipmentREPORT_ALARM_STATE(IN_From, ALARMSET_RESET);
		}
	}
	
#if TEST
	bool bOut = false;

	if (!Get_Equipment(IN_From).Get_ClientConnection())
		return;

	//설비 상태.
	for (int nSvr = 0; nSvr < Get_ServerCount(); nSvr++) {
		if (!Get_Server(nSvr).Get_ClientConnection()) {
			continue;
		}
		
		//설비 LOT ID가져오기.
		auto nProcessStatus = Get_Equipment(IN_From).Get_ProcessStatus();
		auto nOperatingMode = Get_Equipment(IN_From).Get_OperatingMode();

		auto bProcessEvent = bGetProcessStatusEvent(IN_From);
		auto bOperModeEvent = bGetOperatingModeEvent(IN_From);		
		auto bConnectionEvent = bGetClientConnectionEvent(IN_From);

		if (bConnectionEvent) {
			for (int nPort = 0; nPort < Get_Equipment(IN_From).Get_PortCount(); nPort++)
			{
				auto bPortEvent = bGetPortStatusEvent(IN_From, nPort);
				if (!bPortEvent) {
					continue;
				}
				if (!Get_EquipmentTypeEvent(IN_From, nPort)) {
					continue;
				}
				auto bPortStatus = Get_Equipment(IN_From).Get_PortStatus(nPort).nStatus;
				//if (bGetPortLOTIDUseEvent(IN_From, nPort)) 
				{
					if (nProcessStatus == enEqpProcessStatus::EPC_None) {
						if (bPortStatus == PtS_Exist_Socket) {
							Get_Equipment(IN_From).Set_PortStatusEquipmentStateEvent(nPort, ES_STOP);
						}
						else {
							Get_Equipment(IN_From).Set_PortStatusEquipmentStateEvent(nPort, ES_IDLE);
						}
					}
					else if (nProcessStatus == enEqpProcessStatus::EPC_Init) {
						if (bPortStatus == PtS_Exist_Socket) {
							Get_Equipment(IN_From).Set_PortStatusEquipmentStateEvent(nPort, ES_STOP);
						}
						else {
							Get_Equipment(IN_From).Set_PortStatusEquipmentStateEvent(nPort, ES_IDLE);
						}
					}
					else if (nProcessStatus == enEqpProcessStatus::EPC_Idle) {
						if (bPortStatus == PtS_Exist_Socket) {
							Get_Equipment(IN_From).Set_PortStatusEquipmentStateEvent(nPort, ES_STOP);
						}
						else {
							Get_Equipment(IN_From).Set_PortStatusEquipmentStateEvent(nPort, ES_IDLE);
						}
					}
					else if (nProcessStatus == enEqpProcessStatus::EPC_Run) {
						if (bPortStatus == PtS_Exist_Socket){
							Get_Equipment(IN_From).Set_PortStatusEquipmentStateEvent(nPort, ES_RUN);
						}
						else {
							Get_Equipment(IN_From).Set_PortStatusEquipmentStateEvent(nPort, ES_IDLE);
						}
					}
					else if (nProcessStatus == enEqpProcessStatus::EPC_Alarm)
					{
						Get_Equipment(IN_From).Set_PortStatusEquipmentStateEvent(nPort, ES_SUDDENSTOP);
					}
				}
				/*
				else {
					if (nProcessStatus == enEqpProcessStatus::EPC_Alarm) {
						Get_Equipment(IN_From).Set_PortStatusEquipmentStateEvent(nPort, ES_SUDDENSTOP);
					}
					else if (nProcessStatus == enEqpProcessStatus::EPC_Run)
					{
						if (bPortStatus == PtS_Exist_Socket)
						{
							Get_Equipment(IN_From).Set_PortStatusEquipmentStateEvent(nPort, ES_RUN);
						}
						else
						{
							Get_Equipment(IN_From).Set_PortStatusEquipmentStateEvent(nPort, ES_STOP);
						}
					}
					else {
						if (bPortStatus == PtS_Exist_Socket) {
							Get_Equipment(IN_From).Set_PortStatusEquipmentStateEvent(nPort, ES_STOP);
						}
						else {
							Get_Equipment(IN_From).Set_PortStatusEquipmentStateEvent(nPort, ES_IDLE);
						}					
					}
				}*/
				if (bGetPortEquipmentStateEvent(IN_From, nPort)) {
					CString EQUIPMENTID(Get_Equipment(IN_From).Get_EquipmentIDStatus(nPort).szEquipID);	
					//2023.06.28a uhkim
					//CString SUBEQPID(Get_Equipment(IN_From).Get_SubEqpID());
					CString SUBEQPID(Get_Server(nSvr).Get_SubEqpID());
					CString PORTID(Get_Equipment(IN_From).Get_EquipmentIDStatus(nPort).szPortID);
					CString EQUIPMENTSTATE(g_szEquipment_State[Get_Equipment(IN_From).Get_PortStatus(nPort).nEquipmentState]);
					CString LOTID(Get_Equipment(IN_From).Get_FromPortToLOTID(nPort));
					auto m_p = Get_Equipment(IN_From).GetXmlEes().Set_ReportEquipmentStateParameter(
						EQUIPMENTID,
						SUBEQPID,
						PORTID,
						EQUIPMENTSTATE,
						LOTID);
					//if (0 < LOTID.GetLength())
					{
						m_pIcsServer->SendReportEquipmentStateMassage(
							Get_ServerID(nSvr),
							m_p);
					}
					Get_Equipment(IN_From).Set_Notify_EquipmentState(
						m_p);

					Get_Equipment(IN_From).Set_OldPortStatusEquipmentStateEvent(nPort, Get_Equipment(IN_From).Get_PortStatus(nPort).nEquipmentState);
					Get_Equipment(IN_From).Set_OldPortStatus(nPort,
						Get_Equipment(IN_From).Get_PortStatus(nPort).nStatus,
						Get_Equipment(IN_From).Get_PortStatus(nPort).szRfid,
						Get_Equipment(IN_From).Get_PortStatus(nPort).szBarcode);
				}
			}
			Get_Equipment(IN_From).Set_OldClientConnection(Get_Equipment(IN_From).Get_ClientConnection());
		}

		if (!bProcessEvent && !bOperModeEvent) {
			//Equipment는 그대로고 Port상태만 바뀐 경우
			for (int nPort = 0; nPort < Get_Equipment(IN_From).Get_PortCount(); nPort++)
			{
				auto bPortEvent = bGetPortStatusEvent(IN_From, nPort);
				if (!bPortEvent) {
					continue;
				}
				if (!Get_EquipmentTypeEvent(IN_From, nPort)) {
					continue;
				}
				auto bPortStatus = Get_Equipment(IN_From).Get_PortStatus(nPort).nStatus;
				//if (bGetPortLOTIDUseEvent(IN_From, nPort)) 
				{
					if (nProcessStatus == enEqpProcessStatus::EPC_None) {
						if (bPortStatus == PtS_Exist_Socket) {
							Get_Equipment(IN_From).Set_PortStatusEquipmentStateEvent(nPort, ES_STOP);
						}
						else {
							Get_Equipment(IN_From).Set_PortStatusEquipmentStateEvent(nPort, ES_IDLE);
						}
					}
					else if (nProcessStatus == enEqpProcessStatus::EPC_Init) {
						if (bPortStatus == PtS_Exist_Socket) {
							Get_Equipment(IN_From).Set_PortStatusEquipmentStateEvent(nPort, ES_STOP);
						}
						else {
							Get_Equipment(IN_From).Set_PortStatusEquipmentStateEvent(nPort, ES_IDLE);
						}
					}
					else if (nProcessStatus == enEqpProcessStatus::EPC_Idle) {
						if (bPortStatus == PtS_Exist_Socket) {
							Get_Equipment(IN_From).Set_PortStatusEquipmentStateEvent(nPort, ES_STOP);
						}
						else {
							Get_Equipment(IN_From).Set_PortStatusEquipmentStateEvent(nPort, ES_IDLE);
						}
					}
					else if (nProcessStatus == enEqpProcessStatus::EPC_Run) {
						if (bPortStatus == PtS_Exist_Socket) {
							Get_Equipment(IN_From).Set_PortStatusEquipmentStateEvent(nPort, ES_RUN);
						}
						else {
							Get_Equipment(IN_From).Set_PortStatusEquipmentStateEvent(nPort, ES_IDLE);
						}
					}
					else if (nProcessStatus == enEqpProcessStatus::EPC_Alarm) 
					{
						Get_Equipment(IN_From).Set_PortStatusEquipmentStateEvent(nPort, ES_SUDDENSTOP);
					}
				}
				/*
				else {
					if (nProcessStatus == enEqpProcessStatus::EPC_Alarm) {
						Get_Equipment(IN_From).Set_PortStatusEquipmentStateEvent(nPort, ES_SUDDENSTOP);
					}
					else if (nProcessStatus == enEqpProcessStatus::EPC_Run)
					{
						if (bPortStatus == PtS_Exist_Socket)
						{
							Get_Equipment(IN_From).Set_PortStatusEquipmentStateEvent(nPort, ES_RUN);
						}
						else
						{
							Get_Equipment(IN_From).Set_PortStatusEquipmentStateEvent(nPort, ES_STOP);
						}
					}
					else {
						if (bPortStatus == PtS_Exist_Socket) {
							Get_Equipment(IN_From).Set_PortStatusEquipmentStateEvent(nPort, ES_STOP);
						}
						else {
							Get_Equipment(IN_From).Set_PortStatusEquipmentStateEvent(nPort, ES_IDLE);
						}
					}
				}*/
				if (bGetPortEquipmentStateEvent(IN_From, nPort)) {					
					CString EQUIPMENTID(Get_Equipment(IN_From).Get_EquipmentIDStatus(nPort).szEquipID);
					//2023.06.28a uhkim
					//CString SUBEQPID(Get_Equipment(IN_From).Get_SubEqpID());
					CString SUBEQPID(Get_Server(nSvr).Get_SubEqpID());
					CString PORTID(Get_Equipment(IN_From).Get_EquipmentIDStatus(nPort).szPortID);
					CString EQUIPMENTSTATE(g_szEquipment_State[Get_Equipment(IN_From).Get_PortStatus(nPort).nEquipmentState]);
					CString LOTID(Get_Equipment(IN_From).Get_FromPortToLOTID(nPort));
					auto m_p = Get_Equipment(IN_From).GetXmlEes().Set_ReportEquipmentStateParameter(
						EQUIPMENTID,
						SUBEQPID,
						PORTID,
						EQUIPMENTSTATE,
						LOTID);
					//if (0 < LOTID.GetLength())
					{
						m_pIcsServer->SendReportEquipmentStateMassage(
							Get_ServerID(nSvr),
							m_p);
					}
					Get_Equipment(IN_From).Set_Notify_EquipmentState(
						m_p);
					Get_Equipment(IN_From).Set_OldPortStatusEquipmentStateEvent(nPort, Get_Equipment(IN_From).Get_PortStatus(nPort).nEquipmentState);
					Get_Equipment(IN_From).Set_OldPortStatus(nPort,
						Get_Equipment(IN_From).Get_PortStatus(nPort).nStatus,
						Get_Equipment(IN_From).Get_PortStatus(nPort).szRfid,
						Get_Equipment(IN_From).Get_PortStatus(nPort).szBarcode);
				}
			}
			//바뀔 PorcessState가 없을 경우
			//continue;
		}
		//EquipmentState가 변경시.
		else {
			//Alarm 이벤트
			//
			for (int nPort = 0; nPort < Get_Equipment(IN_From).Get_PortCount(); nPort++) 
			{
				if (!Get_EquipmentTypeEvent(IN_From, nPort)) {
					continue;
				}
				auto bPortStatus = Get_Equipment(IN_From).Get_PortStatus(nPort).nStatus;
				//if (bGetPortLOTIDUseEvent(IN_From, nPort)) 
				{
					if (nProcessStatus == enEqpProcessStatus::EPC_None) {
						if (bPortStatus == PtS_Exist_Socket) {
							Get_Equipment(IN_From).Set_PortStatusEquipmentStateEvent(nPort, ES_STOP);
						}
						else {
							Get_Equipment(IN_From).Set_PortStatusEquipmentStateEvent(nPort, ES_IDLE);
						}
					}
					else if (nProcessStatus == enEqpProcessStatus::EPC_Init) {
						if (bPortStatus == PtS_Exist_Socket) {
							Get_Equipment(IN_From).Set_PortStatusEquipmentStateEvent(nPort, ES_STOP);
						}
						else {
							Get_Equipment(IN_From).Set_PortStatusEquipmentStateEvent(nPort, ES_IDLE);
						}
					}
					else if (nProcessStatus == enEqpProcessStatus::EPC_Idle) {
						if (bPortStatus == PtS_Exist_Socket) {
							Get_Equipment(IN_From).Set_PortStatusEquipmentStateEvent(nPort, ES_STOP);
						}
						else {
							Get_Equipment(IN_From).Set_PortStatusEquipmentStateEvent(nPort, ES_IDLE);
						}
					}
					else if (nProcessStatus == enEqpProcessStatus::EPC_Run) {
						if (bPortStatus == PtS_Exist_Socket) {
							Get_Equipment(IN_From).Set_PortStatusEquipmentStateEvent(nPort, ES_RUN);
						}
						else {
							Get_Equipment(IN_From).Set_PortStatusEquipmentStateEvent(nPort, ES_IDLE);
						}
					}
					else if (nProcessStatus == enEqpProcessStatus::EPC_Alarm) {
						Get_Equipment(IN_From).Set_PortStatusEquipmentStateEvent(nPort, ES_SUDDENSTOP);
					}
				}				/*
				else {
					if (nProcessStatus == enEqpProcessStatus::EPC_Alarm) 
					{
						Get_Equipment(IN_From).Set_PortStatusEquipmentStateEvent(nPort, ES_SUDDENSTOP);
					}
					else if (nProcessStatus == enEqpProcessStatus::EPC_Run) 
					{
						if (bPortStatus == PtS_Exist_Socket)
						{
							Get_Equipment(IN_From).Set_PortStatusEquipmentStateEvent(nPort, ES_RUN);
						}
						else 
						{
							Get_Equipment(IN_From).Set_PortStatusEquipmentStateEvent(nPort, ES_IDLE);
						}
					} 
					else 
					{
						Get_Equipment(IN_From).Set_PortStatusEquipmentStateEvent(nPort, ES_IDLE);
					}
				}*/
				if (bGetPortEquipmentStateEvent(IN_From, nPort)) {
					CString EQUIPMENTID(Get_Equipment(IN_From).Get_EquipmentIDStatus(nPort).szEquipID);
					//2023.06.28a uhkim
					//CString SUBEQPID(Get_Equipment(IN_From).Get_SubEqpID());
					CString SUBEQPID(Get_Server(nSvr).Get_SubEqpID());
					CString PORTID(Get_Equipment(IN_From).Get_EquipmentIDStatus(nPort).szPortID);
					CString EQUIPMENTSTATE(g_szEquipment_State[Get_Equipment(IN_From).Get_PortStatus(nPort).nEquipmentState]);
					CString LOTID(Get_Equipment(IN_From).Get_FromPortToLOTID(nPort));
					auto m_p = Get_Equipment(IN_From).GetXmlEes().Set_ReportEquipmentStateParameter(
						EQUIPMENTID,
						SUBEQPID,
						PORTID,
						EQUIPMENTSTATE,
						LOTID);
					//if (0 < LOTID.GetLength())
					{
						m_pIcsServer->SendReportEquipmentStateMassage(
							Get_ServerID(nSvr),
							m_p);
					}				
					Get_Equipment(IN_From).Set_Notify_EquipmentState(
						m_p);
					Get_Equipment(IN_From).Set_OldPortStatusEquipmentStateEvent(nPort, Get_Equipment(IN_From).Get_PortStatus(nPort).nEquipmentState);
					Get_Equipment(IN_From).Set_OldPortStatus(nPort,
						Get_Equipment(IN_From).Get_PortStatus(nPort).nStatus,
						Get_Equipment(IN_From).Get_PortStatus(nPort).szRfid,
						Get_Equipment(IN_From).Get_PortStatus(nPort).szBarcode);
				}
			}

		}

		if (bProcessEvent)
		{
			m_stInspInfo.LineInfo.GetAt(IN_From).Set_OldProcessStatus(nProcessStatus);
		}
		if (bOperModeEvent)
		{
			m_stInspInfo.LineInfo.GetAt(IN_From).Set_OldOperatingMode(nOperatingMode);
		}

		OnEvent_ServerREPORT_EQUIPMENT_STATE(nSvr, NULL);

		if (nProcessStatus == enEqpProcessStatus::EPC_Alarm) {
			OnEvent_EquipmentREPORT_ALARM_STATE(IN_From, ALARMSET_SET);
		}
		else {
			OnEvent_EquipmentREPORT_ALARM_STATE(IN_From, ALARMSET_RESET);
		}
	}
#endif
}

void CTestManager_EQP_ICS::OnEvent_EquipmentREPLY_EQUIPMENT_STATE_DISPLAY(__in uint8_t IN_From, __in LPARAM IN_Data)
{
}
void CTestManager_EQP_ICS::OnEvent_EquipmentREPORT_LOSS_STATE(__in uint8_t IN_From, __in LPARAM IN_Data)
{
}
void CTestManager_EQP_ICS::OnEvent_EquipmentREPLY_LOSS_WINDOW(__in uint8_t IN_From, __in LPARAM IN_Data)
{
}
void CTestManager_EQP_ICS::OnEvent_EquipmentREPORT_ALARM_STATE(__in uint8_t IN_From, __in LPARAM IN_Data)
{
	uint32_t IN_PARA = (uint32_t )IN_Data;
	CString temp;
	if (enALARMSET::ALARMSET_SET == IN_PARA)
	{
		for (int i = 0; i < Get_EqpAlarmCount(IN_From); i++) {
			if (Get_ptAlarmStatus(IN_From, i).Get_nAlarmSet() == ALARMSET_NULL) {				
				Get_ptAlarmStatus(IN_From, i).Set_nAlarmSet(ALARMSET_SET);
				for (int nPort = 0; nPort < Get_Equipment(IN_From).Get_PortCount(); nPort++) {
					if (!Get_EquipmentTypeEvent(IN_From, nPort)) {
						continue;	}

					for (int nSvr = 0; nSvr < Get_ServerCount(); nSvr++) {
						for (int n = 0; n < Get_Server(nSvr).Get_mAlarmStatusCount(); n++)
						{ 
				

							lt::XUUID ID;
							lt::Report_Alarm_State_Args::Args * Args = (lt::Report_Alarm_State_Args::Args *) IN_Data;
							auto svr = m_pIcsServer->GetRemote(Get_ServerID(IN_From));
							if (Args != nullptr) {
								ID = Args->Hd.Get_transactionId();
								svr->GetRemoteEes().AddAlarmStateProcedure(ID, *Args);
								delete Args;
								//CString EQUIPMENTSTATE = lt::ToWideChar(Get_Server(nSvr).Get_mEES_PortSubStatus(n).Get_nEquipmentStatus()).c_str();
								CString SUBEQPID(Get_Server(nSvr).Get_SubEqpID());
								CString IPADDRESS = lt::ToWideChar( Get_Server(nSvr).Get_DEFINEDATA().Get_IPADDRESS()).c_str();
								CString EESMODE = lt::ToWideChar(Get_Server(nSvr).Get_DEFINEDATA().Get_EESMODE()).c_str();
								CString ALARMCODE(g_sALARMSET[ALARMSET_SET]);
								temp.Format(_T("%03d%s%02d"), Get_ptAlarmStatus(IN_From, i).Get_nAlarmCode(), SUBEQPID, nPort);
								CString ALARMID(temp);
								CString ALARMMESSAGE(Get_ptAlarmStatus(IN_From, i).Get_szAlarmInfo());

								auto cntr = svr->GetRemoteEes().AlarmStateCtrl(ID);
								//cntr.REPORT.Hd.Set_timeStamp(cntr.REPORT.Hd.Get_timeStamp());
								//cntr.REPORT.Body.Set_EESMODE(EESMODE);
								//cntr.REPORT.Body.Set_ALARMCODE(ALARMCODE);
								//cntr.REPORT.Body.Set_ALARMID(ALARMID);
								//cntr.REPORT.Body.Set_ALARMMESSAGE(ALARMMESSAGE);
								m_pIcsServer->SendReportAlarmStateMassage(
									Get_ServerID(nSvr),
									cntr.REPORT);
								svr->GetRemoteEes().RemoveOnlineStateProcedure(ID);
							}
						}
					}
				}				
			}
		}
	}
	else if (enALARMSET::ALARMSET_RESET == IN_PARA) {
		for (int i = 0; i < Get_EqpAlarmCount(IN_From); i++) {
			if (Get_ptAlarmStatus(IN_From, i).Get_nAlarmSet() == ALARMSET_SET) {
				Get_ptAlarmStatus(IN_From, i).Set_nAlarmSet(ALARMSET_RESET);
				for (int nPort = 0; nPort < Get_Equipment(IN_From).Get_PortCount(); nPort++) {
					if (!Get_EquipmentTypeEvent(IN_From, nPort)) {
						continue;
					}
					for (int nSvr = 0; nSvr < Get_ServerCount(); nSvr++) {
						for (int n = 0; n < Get_Server(nSvr).Get_mAlarmStatusCount(); n++)
						{
							lt::XUUID ID;
							lt::Report_Alarm_State_Args::Args * Args = (lt::Report_Alarm_State_Args::Args *) IN_Data;
							auto svr = m_pIcsServer->GetRemote(Get_ServerID(IN_From));
							if (Args != nullptr) {
								ID = Args->Hd.Get_transactionId();
								svr->GetRemoteEes().AddAlarmStateProcedure(ID, *Args);
								delete Args;

								CString SUBEQPID(Get_Server(nSvr).Get_SubEqpID());
								CString IPADDRESS = lt::ToWideChar(Get_Server(nSvr).Get_DEFINEDATA().Get_IPADDRESS()).c_str();
								CString EESMODE = lt::ToWideChar(Get_Server(nSvr).Get_DEFINEDATA().Get_EESMODE()).c_str();
								CString ALARMCODE(g_sALARMSET[ALARMSET_RESET]);
								temp.Format(_T("%03d%s%02d"), Get_ptAlarmStatus(IN_From, i).Get_nAlarmCode(), SUBEQPID, nPort);
								CString ALARMID(temp);
								CString ALARMMESSAGE(Get_ptAlarmStatus(IN_From, i).Get_szAlarmInfo());

								auto cntr = svr->GetRemoteEes().AlarmStateCtrl(ID);
								cntr.REPORT.Hd.Set_timeStamp(cntr.REPORT.Hd.Get_timeStamp());
								//cntr.REPORT.Body.Set_EESMODE(EESMODE);
								//cntr.REPORT.Body.Set_ALARMCODE(ALARMCODE);
								//cntr.REPORT.Body.Set_ALARMID(ALARMID);
								//cntr.REPORT.Body.Set_ALARMMESSAGE(ALARMMESSAGE);
								m_pIcsServer->SendReportAlarmStateMassage(
									Get_ServerID(nSvr),
									cntr.REPORT);
								svr->GetRemoteEes().RemoveOnlineStateProcedure(ID);
							}
						}
					}
				}
			}
		}
	}

}
void CTestManager_EQP_ICS::OnEvent_EquipmentREPORT_RMS_MODE(__in uint8_t IN_From, __in LPARAM IN_Data)
{
}
void CTestManager_EQP_ICS::OnEvent_EquipmentREPLY_OPCALL(__in uint8_t IN_From, __in LPARAM IN_Data)
{
}

void CTestManager_EQP_ICS::OnSend_REPLY_LINK_TEST(__in uint8_t IN_nSvrOrder, __in lt::Reply_Link_Test_Args::Args & IN_LPARAM)
{
	OnLog(_T("[Svr %03d] OnSend_REPLY_LINK_TEST(transactionId %s)"), 
		IN_nSvrOrder,
		IN_LPARAM.Hd.Get_transactionId_XuuidToStr());
	if (m_pIcsServer->SendReplyLinkTestMassage(Get_ServerID(IN_nSvrOrder).GetBuffer(),
		IN_LPARAM))
	{
		OnLog(_T("[Svr %03d] OnSend_REPLY_LINK_TEST() : comm succeed"), IN_nSvrOrder);
	}
	else
	{
		OnLog_Err(_T("[Svr %03d] OnSend_REPLY_LINK_TEST() : comm failed"), IN_nSvrOrder);
	}
	RemoveLinkTestProcedure(Get_ServerID(IN_nSvrOrder).GetBuffer(), IN_LPARAM.Hd.Get_transactionId());
}
void CTestManager_EQP_ICS::OnSend_REPORT_ONLINE_STATE(__in uint8_t IN_nSvrOrder, __in lt::Report_Online_State_Args::Args& 	IN_LPARAM)
{
	OnLog(_T("[Svr %03d] OnSend_REPORT_ONLINE_STATE(transactionId %s, ONLINESTATE %s, EESMODE %s)"), 
		IN_nSvrOrder,
		IN_LPARAM.Hd.Get_transactionId(), 
		IN_LPARAM.Body.Get_ONLINESTATE(),
		IN_LPARAM.Body.Get_EESMODE());
	if (m_pIcsServer->SendReportOnlineStateMassage(Get_ServerID(IN_nSvrOrder).GetBuffer(), 
		IN_LPARAM))	{
		OnLog(_T("[Svr %03d] OnSend_REPORT_ONLINE_STATE() : comm succeed"), IN_nSvrOrder);
	}else{
		OnLog_Err(_T("[Svr %03d] OnSend_REPORT_ONLINE_STATE() : comm failed"), IN_nSvrOrder);
	}
	RemoveOnlineStateProcedure(Get_ServerID(IN_nSvrOrder).GetBuffer(), IN_LPARAM.Hd.Get_transactionId());
}
void CTestManager_EQP_ICS::OnSend_REPORT_USER_CHANGE(__in uint8_t IN_nSvrOrder, __in lt::Report_User_Change_Args::Args&	 IN_LPARAM)
{
	OnLog(_T("[Svr %03d] OnSend_REPORT_USER_CHANGE(transactionId %s USERID %s)"), 
		IN_nSvrOrder,
		IN_LPARAM.Hd.Get_transactionId_XuuidToStr(),
		IN_LPARAM.Body.Get_USERID());
	if (m_pIcsServer->SendReportUserChangeMassage(Get_ServerID(IN_nSvrOrder).GetBuffer(), 
		IN_LPARAM))	{
		OnLog(_T("[Svr %03d] OnSend_REPORT_USER_CHANGE() : comm succeed"), IN_nSvrOrder);
	}else{
		OnLog_Err(_T("[Svr %03d] OnSend_REPORT_USER_CHANGE() : comm failed"), IN_nSvrOrder);
	}
}
void CTestManager_EQP_ICS::OnSend_REPLY_USER_COMMAND(__in uint8_t IN_nSvrOrder, __in lt::Reply_User_Change_Args::Args & IN_LPARAM)
{
	OnLog(_T("[Svr %03d] OnSend_REPLY_USER_COMMAND(transactionId %s USERID %s)"), 
		IN_nSvrOrder,
		IN_LPARAM.Hd.Get_transactionId_XuuidToStr(),
		IN_LPARAM.Body.Get_USERID());
	if (m_pIcsServer->SendReplyUserCommandMassage(Get_ServerID(IN_nSvrOrder).GetBuffer(), 
		IN_LPARAM))
	{
		OnLog(_T("[Svr %03d] OnSend_REPLY_USER_COMMAND() : comm succeed"), IN_nSvrOrder);
	}
	else
	{
		OnLog_Err(_T("[Svr %03d] OnSend_REPLY_USER_COMMAND() : comm failed"), IN_nSvrOrder);
	}
	RemoveUserCommandProcedure(Get_ServerID(IN_nSvrOrder).GetBuffer(), IN_LPARAM.Hd.Get_transactionId());
}
void CTestManager_EQP_ICS::OnSend_REPORT_EQUIPMENT_STATE(__in uint8_t IN_nSvrOrder, __in lt::Report_Equipment_State_Args::Args& IN_LPARAM)
{
	OnLog(_T("[Svr %03d] OnSend_REPORT_EQUIPMENT_STATE(transactionId %s  SUBEQPID %s, EQUIPMENTSTATE %s)"), 
		IN_nSvrOrder, 
		IN_LPARAM.Hd.Get_transactionId_XuuidToStr(),
		IN_LPARAM.Body.Get_SUBEQPID(),
		IN_LPARAM.Body.Get_EQUIPMENTSTATE());
	if (m_pIcsServer->SendReportEquipmentStateMassage(Get_ServerID(IN_nSvrOrder).GetBuffer(), 
		IN_LPARAM))
	{
		OnLog(_T("[Svr %03d] OnSend_REPORT_EQUIPMENT_STATE() : comm succeed"), IN_nSvrOrder);
	}
	else
	{
		OnLog_Err(_T("[Svr %03d] OnSend_REPORT_EQUIPMENT_STATE() : comm failed"), IN_nSvrOrder);
	}	
}
void CTestManager_EQP_ICS::OnSend_REPLY_EQUIPMENT_STATE_DISPLAY(__in uint8_t IN_nSvrOrder, __in lt::Reply_Equipment_State_Display_Args::Args& IN_LPARAM)
{	
	OnLog(_T("[Svr %03d] OnSend_REPLY_EQUIPMENT_STATE_DISPLAY(transactionId %s )"), 
		IN_nSvrOrder,
		IN_LPARAM.Hd.Get_transactionId_XuuidToStr());
	if (m_pIcsServer->SendReplyEquipmentStateDisplayMassage(Get_ServerID(IN_nSvrOrder).GetBuffer(), 
		IN_LPARAM))
	{
		OnLog(_T("[Svr %03d] OnSend_REPLY_EQUIPMENT_STATE_DISPLAY() : comm succeed"), IN_nSvrOrder);
	}
	else
	{
		OnLog_Err(_T("[Svr %03d] OnSend_REPLY_EQUIPMENT_STATE_DISPLAY() : comm failed"), IN_nSvrOrder);
	}
	RemoveEquipmentStateProcedure(Get_ServerID(IN_nSvrOrder).GetBuffer(), IN_LPARAM.Hd.Get_transactionId());
}
void CTestManager_EQP_ICS::OnSend_REPORT_LOSS_STATE(__in uint8_t IN_nSvrOrder, __in lt::Report_Loss_State_Args::Args&	IN_LPARAM)
{
	OnLog(_T("[Svr %03d] OnSend_REPORT_LOSS_STATE()"), IN_nSvrOrder);
	if (m_pIcsServer->SendReportLossStateMassage(Get_ServerID(IN_nSvrOrder).GetBuffer(), 
		IN_LPARAM))
	{
		OnLog(_T("[Svr %03d] OnSend_REPORT_LOSS_STATE() : comm succeed"), IN_nSvrOrder);
	}
	else
	{
		OnLog_Err(_T("[Svr %03d] OnSend_REPORT_LOSS_STATE() : comm failed"), IN_nSvrOrder);
	}
}
void CTestManager_EQP_ICS::OnSend_REPLY_LOSS_WINDOW(__in uint8_t IN_nSvrOrder, __in lt::Reply_Loss_Window_Args::Args& IN_LPARAM)
{
	OnLog(_T("[Svr %03d] OnSend_REPLY_LOSS_WINDOW()"), IN_nSvrOrder);
	if (m_pIcsServer->SendReplyLossWindowMassage(Get_ServerID(IN_nSvrOrder).GetBuffer(), 
		IN_LPARAM))
	{
		OnLog(_T("[Svr %03d] OnSend_REPLY_LOSS_WINDOW() : comm succeed"), IN_nSvrOrder);
	}
	else
	{
		OnLog_Err(_T("[Svr %03d] OnSend_REPLY_LOSS_WINDOW() : comm failed"), IN_nSvrOrder);
	}
}
void CTestManager_EQP_ICS::OnSend_REPORT_ALARM_STATE(__in uint8_t IN_nSvrOrder, __in lt::Report_Alarm_State_Args::Args& IN_LPARAM)
{
	OnLog(_T("[Svr %03d] OnSend_REPORT_ALARM_STATE()"), IN_nSvrOrder);
	if (m_pIcsServer->SendReportAlarmStateMassage(Get_ServerID(IN_nSvrOrder).GetBuffer(), 
		IN_LPARAM))
	{
		OnLog(_T("[Svr %03d] OnSend_REPORT_ALARM_STATE() : comm succeed"), IN_nSvrOrder);
	}
	else
	{
		OnLog_Err(_T("[Svr %03d] OnSend_REPORT_ALARM_STATE() : comm failed"), IN_nSvrOrder);
	}
	RemoveAlarmStateProcedure(Get_ServerID(IN_nSvrOrder).GetBuffer(), IN_LPARAM.Hd.Get_transactionId());
}
void CTestManager_EQP_ICS::OnSend_REPLY_ALARM_LIST(__in uint8_t IN_nSvrOrder, __in lt::Reply_Alarm_List_Args::Args& IN_LPARAM)
{
	OnLog(_T("[Svr %03d] OnSend_REPLY_ALARM_LIST()"), IN_nSvrOrder);
	if (m_pIcsServer->SendReplyAlarmListMassage(Get_ServerID(IN_nSvrOrder).GetBuffer(),
		IN_LPARAM))
	{
		OnLog(_T("[Svr %03d] OnSend_REPLY_ALARM_LIST() : comm succeed"), IN_nSvrOrder);
	}
	else
	{
		OnLog_Err(_T("[Svr %03d] OnSend_REPLY_ALARM_LIST() : comm failed"), IN_nSvrOrder);
	}
	RemoveAlarmListProcedure(Get_ServerID(IN_nSvrOrder).GetBuffer(), IN_LPARAM.Hd.Get_transactionId());
}
void CTestManager_EQP_ICS::OnSend_REPORT_RMS_MODE(__in uint8_t IN_nSvrOrder, __in lt::Report_Rms_Mode_Args::Args& IN_LPARAM)
{
	OnLog(_T("[Svr %03d] OnSend_REPORT_RMS_MODE(transactionId %s)"), 
		IN_nSvrOrder,
		IN_LPARAM.Hd.Get_transactionId_XuuidToStr());
	if (m_pIcsServer->SendReportRmsModeMassage(Get_ServerID(IN_nSvrOrder).GetBuffer(),
		IN_LPARAM))
	{
		OnLog(_T("[Svr %03d] OnSend_REPORT_RMS_MODE() : comm succeed"), IN_nSvrOrder);
	}
	else
	{
		OnLog_Err(_T("[Svr %03d] OnSend_REPORT_RMS_MODE() : comm failed"), IN_nSvrOrder);
	}
}
void CTestManager_EQP_ICS::OnSend_REPLY_SET_DATETIME(__in uint8_t IN_nSvrOrder, __in lt::Reply_Set_DateTime_Args::Args& IN_LPARAM)
{
	OnLog(_T("[Svr %03d] OnSend_REPLY_SET_DATETIME()"), IN_nSvrOrder);
	if (m_pIcsServer->SendReplySetDateTimeMassage(Get_ServerID(IN_nSvrOrder).GetBuffer(),
		IN_LPARAM))	{
		OnLog(_T("[Svr %03d] OnSend_REPLY_SET_DATETIME() : comm succeed"), IN_nSvrOrder);
	}	else	{
		OnLog_Err(_T("[Svr %03d] OnSend_REPLY_SET_DATETIME() : comm failed"), IN_nSvrOrder);
	}
	RemoveSetDateTimeProcedure(Get_ServerID(IN_nSvrOrder).GetBuffer(), IN_LPARAM.Hd.Get_transactionId());
}
void CTestManager_EQP_ICS::OnSend_REPLY_TERMINAL_MESSAGE(__in uint8_t IN_nSvrOrder, __in lt::Reply_Terminal_Message_Args::Args& IN_LPARAM)
{

	OnLog(_T("[Svr %03d] OnSend_REPLY_TERMINAL_MESSAGE()"), IN_nSvrOrder);
	if (m_pIcsServer->SendReplyTerminalMessageMassage(Get_ServerID(IN_nSvrOrder).GetBuffer(),
		IN_LPARAM))
	{
		OnLog(_T("[Svr %03d] OnSend_REPLY_TERMINAL_MESSAGE() : comm succeed"), IN_nSvrOrder);
	}else{
		OnLog_Err(_T("[Svr %03d] OnSend_REPLY_TERMINAL_MESSAGE() : comm failed"), IN_nSvrOrder);
	}

}
void CTestManager_EQP_ICS::OnSend_REPLY_OPCALL(__in uint8_t IN_nSvrOrder, __in lt::Reply_Opcall_Args::Args&	IN_LPARAM)
{
	OnLog(_T("[Svr %03d] OnSend_REPLY_OPCALL()"), IN_nSvrOrder);
	if (m_pIcsServer->SendReplyOpCallMassage(Get_ServerID(IN_nSvrOrder).GetBuffer(),
		IN_LPARAM))	{
		OnLog(_T("[Svr %03d] OnSend_REPLY_OPCALL() : comm succeed"), IN_nSvrOrder);
	}else{
		OnLog_Err(_T("[Svr %03d] OnSend_REPLY_OPCALL() : comm failed"), IN_nSvrOrder);
	}
}
void CTestManager_EQP_ICS::OnSet_EESMode(__in enEES_Mode nAcessMode)
{
	__super::OnSet_EESMode(nAcessMode);
}

bool CTestManager_EQP_ICS::Get_EquipmentTypeEvent(
	__in uint8_t  IN_From,
	__in uint8_t  IN_nPortStatus) {
	bool bReturn = true;
	auto IN_nEqpType = Get_EquipmentType(IN_From);
	switch (IN_nEqpType) {
	case enEquipmentType::Eqp_Handler:
		bReturn = false;
		break;
	case enEquipmentType::Eqp_Loader:
		bReturn = false;
		break;
	case enEquipmentType::Eqp_Returner:
		break;
	default:
		if ((IN_nPortStatus == PtI_T_Buffer) ||
			(IN_nPortStatus == PtI_T_RFID)) {
			bReturn = false;
		}
		break;
	}
	return bReturn;
}
#endif 



#if SOCKET
bool CTestManager_EQP_ICS::bGetClientConnectionEvent(__in uint8_t IN_From) {
	auto nPara = Get_Equipment(IN_From).Get_ClientConnection();
	auto nOldPara = Get_Equipment(IN_From).Get_OldClientConnection();

	bool bEvent = false;
	if ((nOldPara != nPara)) {
		bEvent = true;
	}
	return bEvent;
}


bool CTestManager_EQP_ICS::bGetProcessStatusEvent(__in uint8_t IN_From) {
	auto nPara = Get_Equipment(IN_From).Get_OldProcessStatus();
	auto nOldPara = Get_Equipment(IN_From).Get_ProcessStatus();

	bool bEvent = false;
	if ((nOldPara != nPara)) {
		bEvent = true;
	}
	return bEvent;
}

bool CTestManager_EQP_ICS::bGetOperatingModeEvent(__in uint8_t IN_From) {
	auto nPara = Get_Equipment(IN_From).Get_OldOperatingMode();
	auto nOldPara = Get_Equipment(IN_From).Get_OperatingMode();

	bool bEvent = false;
	if ((nOldPara != nPara)) {
		bEvent = true;
	}
	return bEvent;
}

bool CTestManager_EQP_ICS::bGetPortStatusEvent(
	__in uint8_t IN_From, 
	__in uint8_t  IN_nPortStatus) 
{
	auto nPara = Get_Equipment(IN_From).Get_PortStatus(IN_nPortStatus).nStatus;
	auto nOldPara = Get_Equipment(IN_From).Get_OldPortStatus(IN_nPortStatus).nStatus;
	bool bEvent = false;
	if ((nOldPara != nPara)) {
		bEvent = true;
	}
	return bEvent;
}


bool CTestManager_EQP_ICS::bGetPortEquipmentStateEvent(
	__in uint8_t IN_From,
	__in uint8_t  IN_nPortStatus)
{
	auto nPara = Get_Equipment(IN_From).Get_PortStatus(IN_nPortStatus).nEquipmentState;
	auto nOldPara = Get_Equipment(IN_From).Get_OldPortStatus(IN_nPortStatus).nEquipmentState;
	bool bEvent = false;
	if ((nOldPara != nPara)) {
		bEvent = true;
	}
	return bEvent;
}

bool CTestManager_EQP_ICS::bGetPortLOTIDUseEvent(
	__in uint8_t IN_From)
{
	bool bExist = false;
	for (int nPort = 0; nPort < Get_Equipment(IN_From).Get_PortCount(); nPort++)
	{
		auto szRFID = Get_Equipment(IN_From).Get_PortStatus(nPort).szRfid;
		if (0 < szRFID.GetLength()) {
			bExist = true;
		}
		//if (0 < m_stInspInfo.SocketInfo.GetAt(szRFID.GetBuffer()).Get_Lot().GetLength());
	}
	return bExist;	
}

bool CTestManager_EQP_ICS::bGetPortLOTIDUseEvent(
	__in uint8_t  IN_From,
	__in uint8_t  IN_nPortStatus)
{
	bool bExist = false;
	
	auto szRFID = Get_Equipment(IN_From).Get_PortStatus(IN_nPortStatus).szRfid;
	if (0 < szRFID.GetLength()) {
		bExist = true;
	}
	
	return bExist;
}



bool CTestManager_EQP_ICS::Get_ServerTypeEvent(
	__in uint8_t  IN_From,
	__in uint8_t  IN_nPortStatus) {
	bool bReturn = true;
	auto IN_nEqpType = Get_ServerType(IN_From);
	switch (IN_nEqpType) {
	case enServerType::SERVER_EES:		
		//if ((IN_nPortStatus == PtI_EES_0)) {
		//	bReturn = false;
		//}
		break;
	}
	return bReturn;
}

bool CTestManager_EQP_ICS::bGetSvrEquipmentStateEvent(
	__in uint8_t IN_From,
	__in uint8_t IN_nPortStatus) {

	auto nPara = Get_Server(IN_From).Get_PortStatus(IN_nPortStatus).nEquipmentState;
	auto nOldPara = Get_Server(IN_From).Get_OldPortStatus(IN_nPortStatus).nEquipmentState;

	bool bEvent = false;
	if ((nOldPara != nPara)) {
		bEvent = true;
	}
	return bEvent;
}
//=============================================================================
// Method		: OnReport_TerminalMessage
// Access		: virtual protected  
// Returns		: void
// Parameter	: __in LPCTSTR IN_szRFID
// Qualifier	:
// Last Update	: 2023.05.02
// Desc.		:
//=============================================================================
void CTestManager_EQP_ICS::OnReport_TerminalMessage(__in lt::Request_Terminal_Message_Args::Args&  IN_szData)
{
	// 파일로 저장 (Load Time, Unload Time, RFID, Serial Number, NG Code, Eqp Type, Eqp No, Eqp ID, Eqp Para)
	CString szPath;
	CString szFullPath;

	SYSTEMTIME tmLocal;
	GetLocalTime(&tmLocal);

	// Report\\Year\\Month\\ 
	szPath.Format(_T("%s\\%04d\\%02d\\"), m_stInspInfo.Path.szReport, tmLocal.wYear, tmLocal.wMonth);
	MakeDirectory(szPath);

	szFullPath.Format(_T("%sTermanalMessage_%04d_%02d%02d.csv"), szPath, tmLocal.wYear, tmLocal.wMonth, tmLocal.wDay);

	OnLogFile_TerminalMessageResult(IN_szData, szFullPath.GetBuffer());
}

//=============================================================================
// Method		: OnReport_OpCall
// Access		: virtual protected  
// Returns		: void
// Parameter	: __in LPCTSTR IN_szRFID
// Qualifier	:
// Last Update	: 2023.05.02
// Desc.		:
//=============================================================================
void CTestManager_EQP_ICS::OnReport_OpCall(__in lt::Request_Opcall_Args::Args& IN_szData)
{
	// 파일로 저장 (Load Time, Unload Time, RFID, Serial Number, NG Code, Eqp Type, Eqp No, Eqp ID, Eqp Para)
	CString szPath;
	CString szFullPath;

	SYSTEMTIME tmLocal;
	GetLocalTime(&tmLocal);

	// Report\\Year\\Month\\ 
	szPath.Format(_T("%s\\%04d\\%02d\\"), m_stInspInfo.Path.szReport, tmLocal.wYear, tmLocal.wMonth);
	MakeDirectory(szPath);

	szFullPath.Format(_T("%sOpCall_%04d_%02d%02d.csv"), szPath, tmLocal.wYear, tmLocal.wMonth, tmLocal.wDay);

	OnLogFile_OpCallResult(IN_szData, szFullPath.GetBuffer());
	
}
//=============================================================================
// Method		: GetNtPrivilege
// Access		: virtual protected  
// Returns		: void
// Parameter	: __in LPCTSTR IN_szRFID
// Qualifier	:
// Last Update	: 2023.05.02
// Desc.		:
//=============================================================================
BOOL CTestManager_EQP_ICS::GetNtPrivilege()
{
	HANDLE h_token;
	TOKEN_PRIVILEGES privilege_info;
	// 현재 프로세스의 권한과 관련된 정보를 변경하기 위해 토큰정보를 연다.
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &h_token))
	{
		// 권한과 관련된 정보 접근에 실패함..
		return FALSE;
	}

	// 현재 프로세스가 SE_SHUTDOWN_NAME 권한을 사용할수 있도록 설정한다.
	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &privilege_info.Privileges[0].Luid);
	privilege_info.PrivilegeCount = 1;
	privilege_info.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	// 지정한 값으로 권한을 조정한다.
	AdjustTokenPrivileges(h_token, FALSE, &privilege_info, 0, (PTOKEN_PRIVILEGES)NULL, 0);
	if (GetLastError() != ERROR_SUCCESS)
	{
		// 권한 조정에 실패한 경우...
		return FALSE;
	}

	return TRUE;
}
#endif	//SOCKET


#if TEST
void CTestManager_EQP_ICS::OnEvent_ServerUNITID_READ(__in uint8_t IN_From, __in LPARAM IN_Data)
{
	ST_xml_UNITID_READ* IN_PARA = (ST_xml_UNITID_READ*)IN_Data;

	CString PORTID(IN_PARA->Body.PORTID);
	CString PRODUCTID(IN_PARA->Body.PRODUCTID);
	//
	if (Get_Server(IN_From).Get_ClientConnection()) {
		if (IN_From < Get_ServerCount())
		{

		}
	}
	//
	if (Get_Equipment(IN_From).Get_ClientConnection()) {
		if (0 < Get_EquipmentCount())
		{
			if (Get_EquipmentID(IN_From).Compare(Get_Loader().Get_EquipmentId())) {

			}
		}
	}
	//2023.06.28a
	if (MainFrame->FindLPARAM(IN_Data)) {
		delete IN_PARA;
		IN_PARA = nullptr;
		MainFrame->RemoveLPARAM(IN_Data);
	}
}
void CTestManager_EQP_ICS::OnEvent_ServerREQUEST_UNITID_READ(__in uint8_t IN_From, __in LPARAM IN_Data)
{
	ST_xml_REQUEST_UNITID_READ* IN_PARA = (ST_xml_REQUEST_UNITID_READ*)IN_Data;

	CString transactionId(IN_PARA->Hd.transactionId);

	CString EQUIPMENTID(IN_PARA->Body.EQUIPMENTID);
	CString PRODUCTID(IN_PARA->Body.PRODUCTID);
	CString PORTID(IN_PARA->Body.PORTID);
	CString LOTID(IN_PARA->Body.LOTID);
	CString SUBEQPID;
	CString CARRIERID;
	CString MODULEID;

	//서버 이벤트 처리.	
	if (0 < Get_ServerCount()) {
		for (int IN_From = 0; IN_From < Get_ServerCount(); IN_From++) {
			if (Get_Server(IN_From).Get_ClientConnection()) {
				for (int n_Port = 0; n_Port < Get_Server(IN_From).Get_EquipmentIDCount(); n_Port++) {

				}
			}
			for (int i = 0; i < Get_EquipmentCount(); i++) {
				if (Get_Equipment(i).Get_ClientConnection()) {
					for (int n_Port = 0; n_Port < Get_Equipment(i).Get_EquipmentIDCount(); n_Port++) {
						if (Get_Equipment(i).Get_EquipmentIDStatus(n_Port).szEquipID == EQUIPMENTID) {

							CString SUBEQPID(Get_Equipment(i).Get_SubEqpID());
							auto Get = MainFrame->GetTransactionID(IN_PARA->Hd.transactionId);
							ST_xml_UNITID_READ a;
							Get_Server(IN_From).GetXmlEes().ParseXML_UNITID_READ(Get->ReportMsg, &a);
							MODULEID = a.Body.MODULEIDLIST->MODULEID.at(0).c_str();
							CARRIERID = a.Body.CARRIERID;

							OnSend_REPLY_UNITID_READ(
								IN_From,
								Get_Server(IN_From).GetXmlEes().Set_ReplyUnitReadParameter(
									transactionId,
									EQUIPMENTID,
									PORTID,
									LOTID));

							OnSend_REPORT_START_LOT(
								IN_From,
								Get_Server(IN_From).GetXmlEes().Set_ReportStartLotParameter(
									EQUIPMENTID,
									SUBEQPID,
									PORTID,
									LOTID));

							auto GetID = Get_EquipmentID(i);
#if SOCKET_LOTID
							if (GetID == Get_Loader().Get_EquipmentId()) {
								//Get_Equipment(i).Recv_RegisterSocketLOT(CARRIERID, 0, LOTID);
								Get_Equipment(i).Recv_RegisterSocketLOT(MODULEID, 1, LOTID);
							}
#endif
						}
					}
				}

			}
		}
	}
	/*
	if (Get_ServerID(IN_From).GetBuffer() == EQUIPMENTID) {
		if (Get_Server(IN_From).Get_ClientConnection()) {
			if (IN_From < Get_ServerCount())
			{
				if (MainFrame->bGetTransactionID(IN_PARA->Hd.transactionId)) {
					auto Get = MainFrame->GetTransactionID(IN_PARA->Hd.transactionId);
					ST_xml_UNITID_READ a;
					Get_Server(IN_From).GetXmlEes().ParseXML_UNITID_READ(Get->ReportMsg, &a);
					MODULEID	= a.Body.MODULEIDLIST->MODULEID.at(0).c_str();
					CARRIERID	= a.Body.CARRIERID;

					OnSend_REPLY_UNITID_READ(
						IN_From,
						Get_Server(IN_From).GetXmlEes().Set_ReplyUnitReadParameter(
							transactionId,
							PORTID,
							LOTID));

					OnSend_REPORT_START_LOT(
						IN_From,
						Get_Server(IN_From).GetXmlEes().Set_ReportStartLotParameter(
							SUBEQPID,
							PORTID,
							LOTID));
				}
			}
		}
		//설비 이벤트 처리
		for (int i = 0; i < Get_EquipmentCount(); i++) {
			if (0 < Get_EquipmentCount())
			{
				auto GetID = Get_EquipmentID(i);
				if (GetID == Get_Loader().Get_EquipmentId()) {
					//Get_Equipment(i).Recv_RegisterSocketLOT(CARRIERID, 0, LOTID);
					Get_Equipment(i).Recv_RegisterSocketLOT(MODULEID, 1, LOTID);
				}
			}
		}
	}*/
	//2023.06.28a
	if (MainFrame->FindLPARAM(IN_Data)) {
		delete IN_PARA;
		IN_PARA = nullptr;
		MainFrame->RemoveLPARAM(IN_Data);
	}
}
void CTestManager_EQP_ICS::OnEvent_ServerREPLY_UNITID_READ(__in uint8_t IN_From, __in LPARAM IN_Data)
{
	CServer Svr;
	ST_xml_REPLY_UNITID_READ* IN_PARA = (ST_xml_REPLY_UNITID_READ*)IN_Data;


	CString PORTID(IN_PARA->Body.PORTID);
	CString LOTID(IN_PARA->Body.LOTID);


	//서버 이벤트 처리.	
	if (Get_Server(IN_From).Get_ClientConnection()) {
		if (IN_From < Get_ServerCount())
		{
			//서버 보고 이벤트

		}
	}
	//설비 이벤트 처리
	for (int i = 0; i < Get_EquipmentCount(); i++)
	{
		if (0 < Get_EquipmentCount())
		{
			if (Get_EquipmentID(i).Compare(Get_Loader().Get_EquipmentId())) {

			}
		}
	}
	//2023.06.28a
	if (MainFrame->FindLPARAM(IN_Data)) {
		delete IN_PARA;
		IN_PARA = nullptr;
		MainFrame->RemoveLPARAM(IN_Data);
	}
}
void CTestManager_EQP_ICS::OnEvent_ServerREPORT_START_PROCESS(__in uint8_t IN_From, __in LPARAM IN_Data)
{
	CServer Svr;
	ST_xml_REPORT_START_PROCESS* IN_PARA = (ST_xml_REPORT_START_PROCESS*)IN_Data;

	CString PORTID(IN_PARA->Body.PORTID);
	CString SUBEQPID(IN_PARA->Body.SUBEQPID);
	CString LOTID(IN_PARA->Body.LOTLIST->LOTID);
	//
	if (Get_ServerID(IN_From).GetBuffer() == SUBEQPID) {
		if (Get_Server(IN_From).Get_ClientConnection()) {
			if (IN_From < Get_ServerCount())
			{
				//서버 보고 이벤트
				OnSend_REPORT_START_PROCESS(
					IN_From,
					IN_PARA);
				//Svr.Set_ReportStartProcessParameter(
					//SUBEQPID,
					//PORTID,
					//LOTID));
			}
		}
	}
	//
	for (int i = 0; i < Get_EquipmentCount(); i++)
	{
		if (Get_EquipmentID(i).GetBuffer() == SUBEQPID) {
			if (Get_Equipment(i).Get_ClientConnection()) {

			}
		}
	}
	//2023.06.28a
	if (MainFrame->FindLPARAM(IN_Data)) {
		delete IN_PARA;
		IN_PARA = nullptr;
		MainFrame->RemoveLPARAM(IN_Data);
	}
}
void CTestManager_EQP_ICS::OnEvent_ServerREPORT_START_LOT(__in uint8_t IN_From, __in LPARAM IN_Data)
{
	ST_xml_REPORT_START_LOT* IN_PARA = (ST_xml_REPORT_START_LOT*)IN_Data;

	CString PORTID(IN_PARA->Body.PORTID);
	CString SUBEQPID(IN_PARA->Body.SUBEQPID);
	CString LOTID(IN_PARA->Body.LOTLIST->LOTID);

	//서버 이벤트 처리.	
	if (Get_ServerID(IN_From).GetBuffer() == SUBEQPID) {
		if (Get_Server(IN_From).Get_ClientConnection()) {
			if (IN_From < Get_ServerCount())
			{
				//서버 보고 이벤트
				//2023.05.17a 
				//OnSend_REPORT_START_LOT(
					//IN_From,
					//IN_PARA);
					//Svr.Set_ReportStartLotParameter(
					//	SUBEQPID,
					//	PORTID,
					//	LOTID));
			}
		}
	}
	//설비 이벤트 처리
	for (int i = 0; i < Get_EquipmentCount(); i++)
	{
		if (Get_EquipmentID(i).GetBuffer() == SUBEQPID) {
			if (Get_Equipment(i).Get_ClientConnection()) {

			}
		}
	}
	//2023.06.28a
	if (MainFrame->FindLPARAM(IN_Data)) {
		delete IN_PARA;
		IN_PARA = nullptr;
		MainFrame->RemoveLPARAM(IN_Data);
	}
}
void CTestManager_EQP_ICS::OnEvent_ServerREPORT_END_EVENT(__in uint8_t IN_From, __in LPARAM IN_Data)
{
	CServer Svr;
	ST_xml_REPORT_END_EVENT* IN_PARA = (ST_xml_REPORT_END_EVENT*)IN_Data;

	CString PORTID(IN_PARA->Body.PORTID);
	CString SUBEQPID(IN_PARA->Body.SUBEQPID);
	CString LOTID(IN_PARA->Body.LOTLIST->LOTID);

	//서버 이벤트 처리.	
	if (Get_ServerID(IN_From).GetBuffer() == SUBEQPID) {
		if (Get_Server(IN_From).Get_ClientConnection()) {
			if (IN_From < Get_ServerCount())
			{
				//서버 보고 이벤트
				OnSend_REPORT_END_EVENT(
					IN_From,
					IN_PARA);
			}
		}
	}
	//설비 이벤트 처리
	for (int i = 0; i < Get_EquipmentCount(); i++)
	{
		if (Get_EquipmentID(i).GetBuffer() == SUBEQPID) {
			if (Get_Equipment(i).Get_ClientConnection()) {

			}
		}
	}
	//2023.06.28a
	if (MainFrame->FindLPARAM(IN_Data)) {
		delete IN_PARA;
		IN_PARA = nullptr;
		MainFrame->RemoveLPARAM(IN_Data);
	}
}
void CTestManager_EQP_ICS::OnEvent_ServerREPORT_END_PROCESS(__in uint8_t IN_From, __in LPARAM IN_Data)
{
	CServer Svr;
	ST_xml_REPORT_END_PROCESS* IN_PARA = (ST_xml_REPORT_END_PROCESS*)IN_Data;

	CString PORTID(IN_PARA->Body.PORTID);
	CString SUBEQPID(IN_PARA->Body.SUBEQPID);
	CString LOTID(IN_PARA->Body.LOTLIST->LOTID);
	//Test용도.

	//서버 이벤트 처리.	
	if (Get_ServerID(IN_From).GetBuffer() == SUBEQPID) {
		if (Get_Server(IN_From).Get_ClientConnection()) {
			if (IN_From < Get_ServerCount())
			{
				//서버 보고 이벤트
				OnSend_REPORT_END_PROCESS(
					IN_From,
					IN_PARA);
				//Svr.Set_ReportEndProcessParameter(
				//	SUBEQPID,
				//	PORTID,
				//	LOTID));
			}
		}
	}
	//설비 이벤트 처리
	for (int i = 0; i < Get_EquipmentCount(); i++)
	{
		if (Get_EquipmentID(i).GetBuffer() == SUBEQPID) {
			if (Get_Equipment(i).Get_ClientConnection()) {

			}
		}
	}
	//2023.06.28a
	if (MainFrame->FindLPARAM(IN_Data)) {
		delete IN_PARA;
		IN_PARA = nullptr;
		MainFrame->RemoveLPARAM(IN_Data);
	}
}

void CTestManager_EQP_ICS::OnEvent_EquipmentUNITID_READ(__in uint8_t IN_From, __in LPARAM IN_Data)
{
	ST_xml_UNITID_READ * IN_PARA = (ST_xml_UNITID_READ*)IN_Data;

	for (int i = 0; i < m_stInspInfo.ServerInfo.GetCount(); i++) {
		if (m_stInspInfo.ServerInfo.GetAt(i).Get_ClientConnection()) {
			//auto Svr = &Get_Server(i).GetXmlEes().GetBaseData();
			//In_Para->Body.EQUIPMENTID = Svr->EQUIPMENTID;

			m_pIcsServer->SendUnitReadMassage(
				Get_ServerID(i),
				IN_PARA);
		}
	}
	//2023.06.28a
	if (MainFrame->FindLPARAM(IN_Data)) {
		delete IN_PARA;
		IN_PARA = nullptr;
		MainFrame->RemoveLPARAM(IN_Data);
	}
}
void CTestManager_EQP_ICS::OnEvent_EquipmentREPLY_UNITID_READ(__in uint8_t IN_From, __in LPARAM IN_Data)
{
}
void CTestManager_EQP_ICS::OnEvent_EquipmentREPORT_START_PROCESS(__in uint8_t IN_From, __in LPARAM IN_Data)
{
	ST_xml_REPORT_START_PROCESS * IN_PARA = (ST_xml_REPORT_START_PROCESS *)IN_Data;
	CString PORTID(IN_PARA->Body.PORTID);
	CString SUBEQPID(IN_PARA->Body.SUBEQPID);
	CString LOTID(IN_PARA->Body.LOTLIST->LOTID);
	for (int i = 0; i < m_stInspInfo.ServerInfo.GetCount(); i++) {
		if (m_stInspInfo.ServerInfo.GetAt(i).Get_ClientConnection()) {
			m_pIcsServer->SendReportStartProcessMassage(
				Get_ServerID(i),
				IN_PARA);
		}
	}
	//2023.06.28a
	if (MainFrame->FindLPARAM(IN_Data)) {
		delete IN_PARA;
		IN_PARA = nullptr;
		MainFrame->RemoveLPARAM(IN_Data);
	}
}
void CTestManager_EQP_ICS::OnEvent_EquipmentREPORT_START_LOT(__in uint8_t IN_From, __in LPARAM IN_Data)
{
	ST_xml_REPORT_START_LOT * IN_PARA = (ST_xml_REPORT_START_LOT *)IN_Data;
	CString PORTID(IN_PARA->Body.PORTID);
	CString SUBEQPID(IN_PARA->Body.SUBEQPID);
	CString LOTID(IN_PARA->Body.LOTLIST->LOTID);

	for (int i = 0; i < m_stInspInfo.ServerInfo.GetCount(); i++) {
		if (m_stInspInfo.ServerInfo.GetAt(i).Get_ClientConnection()) {
			IN_PARA->Body.USERID = Get_Server(i).GetXmlEes().GetBaseData().USERID;
			m_pIcsServer->SendReportStartLotMassage(
				Get_ServerID(i),
				IN_PARA);
		}
	}
	//2023.06.28a
	if (MainFrame->FindLPARAM(IN_Data)) {
		delete IN_PARA;
		IN_PARA = nullptr;
		MainFrame->RemoveLPARAM(IN_Data);
	}
}
void CTestManager_EQP_ICS::OnEvent_EquipmentREPORT_END_EVENT(__in uint8_t IN_From, __in LPARAM IN_Data)
{
	ST_xml_REPORT_END_EVENT * IN_PARA = (ST_xml_REPORT_END_EVENT *)IN_Data;
	CString PORTID(IN_PARA->Body.PORTID);
	CString SUBEQPID(IN_PARA->Body.SUBEQPID);
	CString LOTID(IN_PARA->Body.LOTLIST->LOTID);

	for (int i = 0; i < m_stInspInfo.ServerInfo.GetCount(); i++) {
		if (m_stInspInfo.ServerInfo.GetAt(i).Get_ClientConnection()) {
			//IN_Para->Body.EQUIPMENTID = Get_ServerID(i);
			IN_PARA->Body.USERID = Get_Server(i).GetXmlEes().GetBaseData().USERID;
			IN_PARA->Body.EESMODE = Get_Server(i).GetXmlEes().GetBaseData().EESMODE;
			m_pIcsServer->SendReportEndEventMassage(
				Get_ServerID(i),
				IN_PARA);
		}
	}
	//2023.06.28a
	if (MainFrame->FindLPARAM(IN_Data)) {
		delete IN_PARA;
		IN_PARA = nullptr;
		MainFrame->RemoveLPARAM(IN_Data);
	}
}
void CTestManager_EQP_ICS::OnEvent_EquipmentREPORT_END_PROCESS(__in uint8_t IN_From, __in LPARAM IN_Data)
{

	ST_xml_REPORT_END_PROCESS * IN_PARA = (ST_xml_REPORT_END_PROCESS *)IN_Data;
	CString PORTID(IN_PARA->Body.PORTID);
	CString SUBEQPID(IN_PARA->Body.SUBEQPID);
	CString LOTID(IN_PARA->Body.LOTLIST->LOTID);

	for (int i = 0; i < m_stInspInfo.ServerInfo.GetCount(); i++) {
		if (m_stInspInfo.ServerInfo.GetAt(i).Get_ClientConnection()) {
			m_pIcsServer->SendReportEndProcessMassage(
				Get_ServerID(i),
				IN_PARA);
		}
	}
	//2023.06.28a
	if (MainFrame->FindLPARAM(IN_Data)) {
		delete IN_PARA;
		IN_PARA = nullptr;
		MainFrame->RemoveLPARAM(IN_Data);
	}
}

void CTestManager_EQP_ICS::OnSend_UNITID_READ(__in uint8_t IN_nSvrOrder, __in ST_xml_UNITID_READ * IN_LPARAM)
{

	ST_xml_UNITID_READ * IN_Data = (ST_xml_UNITID_READ *)IN_LPARAM;
	OnLog(_T("[Svr %03d] OnSend_UNITID_READ()"), IN_nSvrOrder);
	if (m_pIcsServer->SendUnitReadMassage(Get_ServerID(IN_nSvrOrder).GetBuffer(),
		IN_Data))
	{
		OnLog(_T("[Svr %03d] OnSend_UNITID_READ() : comm succeed"), IN_nSvrOrder);
	}
	else
	{
		OnLog_Err(_T("[Svr %03d] OnSend_UNITID_READ() : comm failed"), IN_nSvrOrder);
	}
}
void CTestManager_EQP_ICS::OnSend_REPLY_UNITID_READ(__in uint8_t IN_nSvrOrder, __in ST_xml_REPLY_UNITID_READ * IN_LPARAM)
{
	ST_xml_REPLY_UNITID_READ * IN_Data = (ST_xml_REPLY_UNITID_READ *)IN_LPARAM;
	OnLog(_T("[Svr %03d] OnSend_REPLY_UNITID_READ()"), IN_nSvrOrder);
	if (m_pIcsServer->SendReplyUnitReadMassage(Get_ServerID(IN_nSvrOrder).GetBuffer(),
		IN_Data))
	{
		OnLog(_T("[Svr %03d] OnSend_REPLY_UNITID_READ() : comm succeed"), IN_nSvrOrder);
	}
	else
	{
		OnLog_Err(_T("[Svr %03d] OnSend_REPLY_UNITID_READ() : comm failed"), IN_nSvrOrder);
	}
}
//=============================================================================
// Method		: OnSend_REPORT_START_LOT
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2023/03/22 - 19:51
// Desc.		:
//=============================================================================
void CTestManager_EQP_ICS::OnSend_REPORT_START_PROCESS(__in uint8_t IN_nSvrOrder, __in ST_xml_REPORT_START_PROCESS * IN_LPARAM)
{
	ST_xml_REPORT_START_PROCESS * IN_Data = (ST_xml_REPORT_START_PROCESS *)IN_LPARAM;
	OnLog(_T("[Svr %03d] OnSend_REPORT_START_PROCESS()"), IN_nSvrOrder);
	if (m_pIcsServer->SendReportStartProcessMassage(Get_ServerID(IN_nSvrOrder).GetBuffer(),
		IN_Data))
	{
		OnLog(_T("[Svr %03d] OnSend_REPORT_START_PROCESS() : comm succeed"), IN_nSvrOrder);
	}
	else
	{
		OnLog_Err(_T("[Svr %03d] OnSend_REPORT_START_PROCESS() : comm failed"), IN_nSvrOrder);
	}
}
//=============================================================================
// Method		: OnSend_REPORT_START_LOT
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2023/03/22 - 19:51
// Desc.		:
//=============================================================================
void CTestManager_EQP_ICS::OnSend_REPORT_START_LOT(__in uint8_t IN_nSvrOrder, __in ST_xml_REPORT_START_LOT * IN_LPARAM)
{
	ST_xml_REPORT_START_LOT * IN_Data = (ST_xml_REPORT_START_LOT *)IN_LPARAM;
	OnLog(_T("[Svr %03d] OnSend_REPORT_START_LOT()"), IN_nSvrOrder);
	if (m_pIcsServer->SendReportStartLotMassage(Get_ServerID(IN_nSvrOrder).GetBuffer(),
		IN_Data))
	{
		OnLog(_T("[Svr %03d] OnSend_REPORT_START_LOT() : comm succeed"), IN_nSvrOrder);
	}
	else
	{
		OnLog_Err(_T("[Svr %03d] OnSend_REPORT_START_LOT() : comm failed"), IN_nSvrOrder);
	}
}
//=============================================================================
// Method		: OnSend_REPORT_START_PROCESS
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2023/03/22 - 19:51
// Desc.		:
//=============================================================================
void CTestManager_EQP_ICS::OnSend_REPORT_END_EVENT(__in uint8_t IN_nSvrOrder, __in ST_xml_REPORT_END_EVENT * IN_LPARAM)
{
	ST_xml_REPORT_END_EVENT * IN_Data = (ST_xml_REPORT_END_EVENT *)IN_LPARAM;
	OnLog(_T("[Svr %03d] OnSend_REPORT_END_EVENT()"), IN_nSvrOrder);
	if (m_pIcsServer->SendReportEndEventMassage(Get_ServerID(IN_nSvrOrder).GetBuffer(),
		IN_Data))
	{
		OnLog(_T("[Svr %03d] OnSend_REPORT_END_EVENT() : comm succeed"), IN_nSvrOrder);
	}
	else
	{
		OnLog_Err(_T("[Svr %03d] OnSend_REPORT_END_EVENT() : comm failed"), IN_nSvrOrder);
	}
}
//=============================================================================
// Method		: OnSend_REPORT_START_PROCESS
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2023/03/22 - 19:51
// Desc.		:
//=============================================================================
void CTestManager_EQP_ICS::OnSend_REPORT_END_PROCESS(__in uint8_t IN_nSvrOrder, __in ST_xml_REPORT_END_PROCESS * IN_LPARAM)
{
	ST_xml_REPORT_END_PROCESS * IN_Data = (ST_xml_REPORT_END_PROCESS *)IN_LPARAM;
	OnLog(_T("[Svr %03d] OnSend_REPORT_END_PROCESS()"), IN_nSvrOrder);
	if (m_pIcsServer->SendReportEndProcessMassage(Get_ServerID(IN_nSvrOrder).GetBuffer(),
		IN_Data))
	{
		OnLog(_T("[Svr %03d] OnSend_REPORT_END_PROCESS() : comm succeed"), IN_nSvrOrder);
	}
	else
	{
		OnLog_Err(_T("[Svr %03d] OnSend_REPORT_END_PROCESS() : comm failed"), IN_nSvrOrder);
	}

}
#endif