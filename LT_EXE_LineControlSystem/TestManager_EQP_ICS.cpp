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
#include "Def_Language_Message.h"

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
		if (m_bFlag_CheckEqpConnection)
		{
			// Auto 상태인데, 통신이 끊기면 알람을 팝업한다 (2023.03.20)
			if (enEqpOperatingMode::EOM_Auto == Get_Equipment(IN_FromEqp).Get_OperatingMode())
			{
				// 설비와의 통신이 끊겼습니다.
				CString szMsg;
				szMsg.Format(_T("%s\r\n Eqp_%02d: %s(%s)"), g_szMessageBox_T[MB_Eqp_Comm_Disconnected][m_stOption.Inspector.nLanguage],
					IN_FromEqp,
					Get_Equipment(IN_FromEqp).Get_Alias(),
					Get_Equipment(IN_FromEqp).Get_EquipmentId());

				LT_MessageBox(szMsg.GetBuffer());
			}
		}

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
#if (SET_INSPECTOR == SYS_ICS_RIVIAN_LINE)
				// 검사 포트가 2개 이상인 경우에만 메세지 전송
				//if (1 < Get_Equipment(IN_FromEqp).Get_TestPortCount())
				if (Get_EquipmentType(nEmptyCount) == enEquipmentType::Eqp_Distortion)
				{
					// 현재 설비로 생산 종료 메세지 전송
					OnSend_EndOfProduction(IN_FromEqp, true);
				}
#else
				//if (1 < Get_Equipment(IN_FromEqp).Get_TestPortCount())
				if (Get_Equipment(nEmptyCount).Get_EqpType_UI() != enEquipmentType_UI::EqpUI_HotPixel3port)
				{
					// 현재 설비로 생산 종료 메세지 전송
					OnSend_EndOfProduction(IN_FromEqp, true);
				}
#endif // (SET_INSPECTOR == SYS_ICS_RIVIAN_LINE)
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
#ifdef USE_AUTO_TO_MANUAL_AUTOCHANGE

	if (false == m_stInspInfo.bCheckedRegister)
	{
		OnLog(_T("Begin Socket Register Time Check"));
	}
	m_stInspInfo.Set_LastRegisterTime();

#endif // USE_AUTO_TO_MANUAL_AUTOCHANGE

	// 라인 중간에서 수동으로 제거된 소켓인지 판단
	auto nEqpIdx = Get_Socket(IN_szRFID).m_nTargetEqpOrder;
	if (Get_Equipment(nEqpIdx).Is_Tester())
	{
		Get_Equipment(nEqpIdx).Decrease_ReservedPort(IN_szRFID);

		OnLog(_T("[Loader] Register a socket that has been manually removed. (rfid: %s)"), IN_szRFID);
	}

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
	if (NULL == IN_szRFID)
	{
		OnLog_Err(_T("[Loader] Check Track Out => RFID is NULL"));
		return;
	}

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
	//** 이미 트랙인 처리된 소켓이면 무시한다.
	if (true == Get_SocketInfo().Is_TrackInSocket(IN_szRFID, IN_FromEqp))
	{
		auto nTime = Get_SocketInfo().Get_ElapsedTime_TrackIn(IN_szRFID);
		if ((0 < nTime) && (nTime <= 900))
		{
			OnLog(_T("[eqp %02d] Skip Track In : Already Track In Completed => rfid: %s"), IN_FromEqp, IN_szRFID);
			return;
		}
	}

	// 설비에서 이미 Track In 요청된 소켓이면.. 무시
	if (true == Get_Equipment(IN_FromEqp).IsTrackInRequest_Socket(IN_szRFID, 900))
	{
		OnLog(_T("[eqp %02d] Skip Track In : Already Track In Requested => rfid: %s"), IN_FromEqp, IN_szRFID);
		return;
	}
	else
	{
		Get_Equipment(IN_FromEqp).Set_TrackInRequestSocket(IN_szRFID);
	}

	// 검사 설비에 소켓이 도착하여, RFID로 투입 승인 요청
	OnLog(_T("[eqp %02d] Track In (Request Socket) => rfid: %s"), IN_FromEqp, IN_szRFID);
	OnLog_SocketEvent(IN_szRFID, _T("[eqp %02d] Track In (Request Socket) => rfid: %s"), IN_FromEqp, IN_szRFID);

	// 0: Error
	// 1: 승인
	// 2: 승인 (마지막 소켓)
	// 3: Bypass

	// 설비가 알람?
	// 설비가 Skip?


	// 목적 설비가 현재 설비보다 이전 설비이면 오류 상황.. (2023.08.01)
	if (Get_Socket(IN_szRFID).m_nTargetEqpOrder < IN_FromEqp)
	{
		OnLog_Err(_T("[eqp %02d] Target Equipment Error!!! => rfid: %s, Target eqp: %d, Current eqp: %d"), 
						IN_FromEqp, 
						IN_szRFID, 
						Get_Socket(IN_szRFID).m_nTargetEqpOrder, 
						IN_FromEqp);

		Get_Socket(IN_szRFID).Set_TestResult(-3, 0, IN_FromEqp, Get_EquipmentID(IN_FromEqp));

		Proc_SetSocketTargetEquipment(IN_szRFID, Get_Returner().Get_EqpOrder());

		// 리턴
		// 서버->설비로 투입 승인 여부 통보
		OnSend_Accept_SocketTrackIn(IN_FromEqp, IN_szRFID, enAcceptSocket::AS_Bypass, Get_SocketBarcode(IN_szRFID).GetBuffer());

		OnLog(_T("[eqp %02d] Accept Socket Track In (Error) => rfid: %s, flag: %d (%s)"), IN_FromEqp, IN_szRFID, enAcceptSocket::AS_Bypass, g_szAcceptSocket[enAcceptSocket::AS_Bypass]);
		OnLog_SocketEvent(IN_szRFID, _T("[eqp %02d] Accept Socket Track In (Error) => rfid: %s, flag: %d (%s)"), IN_FromEqp, IN_szRFID, enAcceptSocket::AS_Bypass, g_szAcceptSocket[enAcceptSocket::AS_Bypass]);
		return;
	}


#ifdef USE_SET_TARGET_EQP_GROUP_TRACK_IN // 2023.03.23

	if (Get_Equipment(IN_FromEqp).Get_EnableEquipment())
	{
		// 현재 설비가 스킵인가? // 2023.05.10
		if (false == Get_Equipment(IN_FromEqp).Get_Skip())
		{
			Proc_SetTagetEquipment_Group(IN_szRFID, IN_FromEqp);
		}
	}

#endif // USE_SET_TARGET_EQP_GROUP_TRACK_IN


#ifdef USE_TARGET_EQP_DISTANCE
	// 설비가 그룹 내 첫 설비 이고, 소켓이 far away 상태?  => 그룹 내 목적지 재 설정
	if (Get_Socket(IN_szRFID).Get_FarAway_TargetEqp())
	{
		if (Get_LineInfo().Is_FirstEquipment_inGroup(IN_FromEqp))
		{
			// 예약 1 감소
			//Get_Equipment(IN_FromEqp).Decrease_ReservedPort();
			Get_Equipment(IN_FromEqp).Decrease_ReservedPort(IN_szRFID);

			// 목적 설비 재 설정
			Proc_SetTagetEquipment_Group(IN_szRFID, Get_EquipmentType(IN_FromEqp));
		}
	}
#endif


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

#ifdef USE_NG_CODE_UPDATE_AT_UNLOAD

	// 불량 정보 업데이트 
	ST_TestResult* pRe = &Get_Socket(IN_szRFID).m_stTestResult;

	// Log : 불량이 발생한 제품 정보 기록 (제품 바코드, NG 발생 검사: Pass된 검사)	
	Get_Equipment(pRe->m_nNG_EqpOrder).IncreaseFailInfo(pRe->m_nNG_Code, pRe->m_nNG_Para);

#endif // USE_NG_CODE_UPDATE_AT_UNLOAD

#ifdef USE_EQP_YIELD_UPDATE_AT_UNLOAD
	
#ifndef USE_NG_CODE_UPDATE_AT_UNLOAD
	ST_TestResult* pRe = &Get_Socket(IN_szRFID).m_stTestResult;
#endif

	// 설비별 양품 결과 업데이트
	for (auto const &element : pRe->m_Eqp_Result)
	{
		// 검사가 진행된 설비 체크
		if (1 == element.m_bTest)
		{
			if (0 == element.m_nNGCode)
			{
				Get_Equipment(element.m_nEqpOrder).Increase_Yield_Pass(element.m_nPara);
			}
			else if (Get_SocketInfo().Get_MES_ReworkCode() == element.m_nNGCode)
			{
				continue; // MES rework은 검사를 안해서 검사 결과가 없는게 정상 동작이다.
			}
			else // 불량
			{
				break;
			}
		}
	}

// 	for (auto nIdx = 0; pRe->m_Eqp_Result.size(); ++nIdx)
// 	{
// 		auto eqp = pRe->m_Eqp_Result.at(nIdx);
// 
// 		// 검사가 진행된 설비 체크
// 		if (1 == eqp.m_bTest)
// 		{
// 			if (0 == eqp.m_nNGCode)
// 			{
// 				Get_Equipment(eqp.m_nEqpOrder).Increase_Yield_Pass(eqp.m_nPara);
// 			}
// 			else if (Get_SocketInfo().Get_MES_ReworkCode() == eqp.m_nNGCode)
// 			{
// 				continue; // MES rework은 검사를 안해서 검사 결과가 없는게 정상 동작이다.
// 			}
// 			else // 불량
// 			{
// 				break;
// 			}			
// 		}
// 	}

	// 양/불 체크
	if (0 == pRe->m_nNG_Code)
	{
		// 양품은 설비에서 바로 업데이트
	}
	else if (Get_SocketInfo().Get_MES_ReworkCode() == pRe->m_nNG_Code)// MES Rework NG Code 이면...???
	{
		;
	}
	else
	{
		Get_Equipment(pRe->m_nNG_EqpOrder).Increase_Yield_Fail(pRe->m_nNG_Para);
	}

#endif //USE_EQP_YIELD_UPDATE_AT_UNLOAD


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
	//if (false == Get_Equipment(IN_nTargetEqpOrder).Increase_ReservedPort())
	if (false == Get_Equipment(IN_nTargetEqpOrder).Increase_ReservedPort(IN_szRFID))
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
#ifdef USE_TARGET_EQP_DISTANCE
	//uint8_t IN_nTargetEqpOrder = Get_LineInfo().Get_TargetEquipment_inGroup((enEquipmentType)Get_Equipment(IN_FromEqp).Get_EquipmentType());

	//Get_SocketInfo().Set_SocketTarget(IN_szRFID, IN_nTargetEqpOrder, Get_EquipmentID(IN_nTargetEqpOrder).GetBuffer());
#endif // USE_TARGET_EQP_DISTANCE


	// 설비가 그룹이고 설비가 그룹에서 첫번째 설비?
	if (Get_LineInfo().Is_FirstEquipment_inGroup(IN_FromEqp))
	{
		// 현재 소켓의 대상 설비
		auto nTargetEqp = Get_Socket(IN_szRFID).m_nTargetEqpOrder;

		// 대상 설비가 현재 설비의 검사와 동일한가?
		if (Get_EquipmentType(IN_FromEqp) == Get_EquipmentType(nTargetEqp))
		{
			// 이전 대상 설비 예약 1 감소
			Get_Equipment(nTargetEqp).Decrease_ReservedPort(IN_szRFID);

			// 새로운 목적 설비 구하기
			uint8_t nNew_TargetEqpOrder = Get_LineInfo().Get_TargetEquipment_inGroup((enEquipmentType)Get_Equipment(IN_FromEqp).Get_EquipmentType());

			if (nTargetEqp != nNew_TargetEqpOrder)
			{
				// 목적 설비 재 설정
				Get_SocketInfo().Set_SocketTarget(IN_szRFID, nNew_TargetEqpOrder, Get_EquipmentID(nNew_TargetEqpOrder).GetBuffer());
			}

			// 새 대상 설비 예약 1 증가
			if (false == Get_Equipment(nNew_TargetEqpOrder).Increase_ReservedPort(IN_szRFID))
			{
				;
			}

			// 목적 설비의 포트 상태 표시
			OnLog(_T("[eqp %02d] Group Target Equipment(eqp %02d) Status => port: %d, using: %d, empty: %d, reserved: %d, reserved_over: %d"),
				IN_FromEqp,
				nNew_TargetEqpOrder,
				Get_Equipment(nNew_TargetEqpOrder).Get_PortCount(),
				//Get_Equipment(nNew_TargetEqpOrder).Check_AvablePortCnt(),
				Get_Equipment(nNew_TargetEqpOrder).Get_UsingPortCount(),
				Get_Equipment(nNew_TargetEqpOrder).Get_EmptyPortCount(),
				Get_Equipment(nNew_TargetEqpOrder).Get_ReservedPortCnt(),
				Get_Equipment(nNew_TargetEqpOrder).Get_ReservedOverCnt()
			);

			OnLog(_T("[eqp %02d] Group Set Target Eqp => rfid: %s, target: [eqp %02d]"), IN_FromEqp, IN_szRFID, nNew_TargetEqpOrder);
			OnLog_SocketEvent(IN_szRFID, _T("[eqp %02d] Group Set Target Eqp => rfid: %s, target: [eqp %02d]"), IN_FromEqp, IN_szRFID, nNew_TargetEqpOrder);
		}
	}
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
// Method		: OnSend_OperationActiveStatus
// Access		: protected  
// Returns		: void
// Parameter	: __in uint8_t IN_nStatus
// Qualifier	:
// Last Update	: 2023/3/7 - 14:32
// Desc.		:
//=============================================================================
void CTestManager_EQP_ICS::OnSend_OperationActiveStatus(__in uint8_t IN_nStatus)
{
	OnLog(_T("[eqp all] OnSend_OperationActiveStatus() => status: %d"), IN_nStatus);

	if (m_pIcsComm->SendOperationActiveStatus(IN_nStatus))
	{
		OnLog(_T("OnSend_OperationActiveStatus() => comm succeed"));
	}
	else
	{
		OnLog_Err(_T("OnSend_OperationActiveStatus() => comm failed"));
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

			Get_SocketInfo().Set_SocketTrackIn(IN_szRFID, IN_nEqpOrder, Get_EquipmentID(IN_nEqpOrder).GetBuffer());
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

#ifdef USE_AUTO_TO_MANUAL_AUTOCHANGE

		if (false == m_stInspInfo.bCheckedRegister)
		{
			OnLog(_T("Begin Socket Register Time Check"));
		}
		m_stInspInfo.Set_LastRegisterTime();

#endif // USE_AUTO_TO_MANUAL_AUTOCHANGE

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

//=============================================================================
// Method		: OnMonitor_TimeCheck
// Access		: virtual protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2023/3/7 - 15:32
// Desc.		:
//=============================================================================
void CTestManager_EQP_ICS::OnMonitor_TimeCheck()
{
	if (false == m_mTimeMutex.try_lock())
		return;

	//__super::OnMonitor_TimeCheck();

	// 인라인 가동/비가동 상태 체크 (전체 설비에 소켓이 없으면 비가동, 있으면 가동)

	// 라인 설정이 안되어 있으면 사용하면 안됨 (프로그램 비정상 종료 됨)
	if (Get_LineInfo().GetCount() <= 0)
	{
		m_mTimeMutex.unlock();
		return;
	}

	// 서버가 open 되어 있는 경우에만 사용
	if (false == m_pIcsComm->IsOpened())
	{
		m_mTimeMutex.unlock();
		return;
	}

	// 로더 에서 리터너 까지 소켓 유/무 체크
	uint8_t nBegin_eqp	= Get_Loader().Get_EqpOrder();
	uint8_t nEnd_eqp	= Get_Returner().Get_EqpOrder();

	uint8_t nActived = enOperationActiveStatus::OAS_Inactive;
	for (auto nEqpIdx = nBegin_eqp; nEqpIdx < nEnd_eqp; ++nEqpIdx)
	{
		// 설비가 비어 있는가?
		if (false == Get_Equipment(nEqpIdx).IsEmpty_Equipment_AnySocket())
		{
			nActived = enOperationActiveStatus::OAS_Active;
			break;
		}
	}


	// 활성화 <==> 비활성화 상태가 변경되면  통신 메세지를 전체 설비로 보낸다.
	if (nActived != Get_LineInfo().Get_ActiveStatus())
	{
		Get_LineInfo().Set_ActiveStatus(nActived);

		// 전체 설비에 메세지 전송
		OnSend_OperationActiveStatus(nActived);
	}

#ifdef USE_AUTO_TO_MANUAL_AUTOCHANGE

	// 현재 Auto Mode인가?? (로더만 체크??? 소켓 투입모드는 auto?)
	if (enEqpOperatingMode::EOM_Auto == Get_Loader().Get_OperatingMode())
	{
		// 소켓 투입 시간이 체크되었나?
		if (m_stInspInfo.bCheckedRegister)
		{
			// 현재 시간 - 제품 마지막으로 투입된 시간 (분 단위)
			if ((m_stOption.Inspector.nAutoModeDuration * 60.0) <= m_stInspInfo.Get_LastRegister_ElapsedTime())
			{
				OnSend_Stop();  // Manual Mode로 전환

				// 체크 시간 초기화
				m_stInspInfo.Reset_LastRegisterTime();
				OnLog(_T("Reset Last Socket Rgister Time"));
			}
		}
	}
	
#endif // USE_AUTO_TO_MANUAL_AUTOCHANGE

	m_mTimeMutex.unlock();
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
	if (false == m_mUiMutex.try_lock())
		return;

	//__super::OnMonitor_UpdateUI();

	// 라인 설정이 안되어 있으면 사용하면 안됨 (프로그램 비정상 종료 됨)
	if (0 < Get_LineInfo().GetCount())
	{
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
							//OnSend_EndOfProduction(nEqpIdx, true);

							// 설비 구분하여 신호 처리
							// Trinity : HotPixel 3para
							// Rivian : Distortion;
#if (SET_INSPECTOR == SYS_ICS_RIVIAN_LINE)
							// 검사 포트가 2개 이상인 경우에만 메세지 전송
							//if (1 < Get_Equipment(IN_FromEqp).Get_TestPortCount())
							if (Get_EquipmentType(nEmptyCount) == enEquipmentType::Eqp_Distortion)
							{
								// 현재 설비로 생산 종료 메세지 전송
								OnSend_EndOfProduction(nEqpIdx, true);
							}
#else
							if (Get_Equipment(nEmptyCount).Get_EqpType_UI() != enEquipmentType_UI::EqpUI_HotPixel3port)
							{
								// 현재 설비로 생산 종료 메세지 전송
								OnSend_EndOfProduction(nEqpIdx, true);
							}
#endif // (SET_INSPECTOR == SYS_ICS_RIVIAN_LINE)
						}
					}

					nEmptyCount = 0;
				}
			}
		}
	}

	m_mUiMutex.unlock();
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
