//*****************************************************************************
// Filename	: 	SocketMonitoring.cpp
// Created	:	2021/11/18 - 14:21
// Modified	:	2021/11/18 - 14:21
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#include "stdafx.h"
#include "SocketMonitoring.h"
#include "Def_Constant.h"
#include "LineInfo.h"
#include "Rework_Info.h"

 
//=============================================================================
//
//=============================================================================
CSocketMonitoring::CSocketMonitoring()
{

}

CSocketMonitoring::~CSocketMonitoring()
{

}

//=============================================================================
//
//=============================================================================
CSocketMonitoring& CSocketMonitoring::operator=(const CSocketList& ref)
{
	m_Sockets.clear();

	auto eIter = ref.SocketList.end();
	auto Iter = ref.SocketList.begin();
	while (Iter != eIter)
	{
		CSocketInfo_Unit SocketUnit;
		SocketUnit = Iter->second;

		Socket_Insert(SocketUnit);

		Iter++;
	}

	return *this;
}

CSocketMonitoring& CSocketMonitoring::operator=(const CSocketMonitoring& ref)
{
	m_Sockets.clear();
	m_Sockets = ref.m_Sockets;

	return *this;
}


//=============================================================================
// Method		: WM_Update_GUI_Data
// Access		: protected  
// Returns		: void
// Parameter	: __in LPCTSTR IN_szRFID
// Parameter	: __in uint16_t IN_nFlag
// Qualifier	:
// Last Update	: 2022/1/26 - 20:45
// Desc.		:
//=============================================================================
void CSocketMonitoring::WM_Update_GUI_Data(__in LPCTSTR IN_szRFID, __in uint16_t IN_nFlag)
{
	if (Is_ExistSocket(IN_szRFID))
	{
		if (m_hWndOwner)
		{			
			::SendNotifyMessage(m_hWndOwner, WM_UPDATE_SOCKET_DATA, (WPARAM)IN_nFlag, (LPARAM)m_Sockets.at(IN_szRFID).szRFID.GetBuffer());

			// 쓰레드로 처리하자.
			//::SendMessage(m_hWndOwner, WM_UPDATE_SOCKET_DATA, (WPARAM)IN_nFlag, (LPARAM)m_Sockets.at(IN_szRFID).szRFID.GetBuffer());
		}
	}
}

void CSocketMonitoring::WM_Update_GUI_Data_All(__in uint16_t IN_nFlag)
{
	if (m_hWndOwner)
	{
		::SendNotifyMessage(m_hWndOwner, WM_UPDATE_SOCKET_DATA_ALL, (WPARAM)IN_nFlag, (LPARAM)0);
	}
}

//=============================================================================
// Method		: Save_SocketInfo
// Access		: public  
// Returns		: bool
// Parameter	: __in LPCTSTR IN_szRFID
// Qualifier	:
// Last Update	: 2022/1/10 - 15:04
// Desc.		:
//=============================================================================
bool CSocketMonitoring::Save_SocketInfo(__in LPCTSTR IN_szRFID)
{
	return m_regSocket.Set_SocketInfo(IN_szRFID, &m_Sockets.at(IN_szRFID));
}

bool CSocketMonitoring::Save_SocketInfo_Barcode(__in LPCTSTR IN_szRFID)
{
	return m_regSocket.Set_SocketInfo_Barcode(IN_szRFID, &m_Sockets.at(IN_szRFID));
}

bool CSocketMonitoring::Save_SocketInfo_Status(__in LPCTSTR IN_szRFID)
{
	return m_regSocket.Set_SocketInfo_Status(IN_szRFID, &m_Sockets.at(IN_szRFID));
}

bool CSocketMonitoring::Save_SocketInfo_Equipment(__in LPCTSTR IN_szRFID)
{
	return m_regSocket.Set_SocketInfo_Equipment(IN_szRFID, &m_Sockets.at(IN_szRFID));
}

bool CSocketMonitoring::Save_SocketInfo_Location(__in LPCTSTR IN_szRFID)
{
	return m_regSocket.Set_SocketInfo_Location(IN_szRFID, &m_Sockets.at(IN_szRFID));
}

bool CSocketMonitoring::Save_SocketInfo_Target(__in LPCTSTR IN_szRFID)
{
	return m_regSocket.Set_SocketInfo_Target(IN_szRFID, &m_Sockets.at(IN_szRFID));
}

bool CSocketMonitoring::Save_SocketInfo_Yield(__in LPCTSTR IN_szRFID)
{
	return m_regSocket.Set_SocketInfo_Yield(IN_szRFID, &m_Sockets.at(IN_szRFID));
}

bool CSocketMonitoring::Save_SocketInfo_TestResult(__in LPCTSTR IN_szRFID)
{
	return m_regSocket.Set_SocketInfo_TestResult(IN_szRFID, &m_Sockets.at(IN_szRFID));
}

bool CSocketMonitoring::Save_SocketInfo_InputTime(__in LPCTSTR IN_szRFID)
{
	return m_regSocket.Set_SocketInfo_InputTime(IN_szRFID, &m_Sockets.at(IN_szRFID));
}

bool CSocketMonitoring::Save_SocketInfo_OutputTime(__in LPCTSTR IN_szRFID)
{
	return m_regSocket.Set_SocketInfo_OutputTime(IN_szRFID, &m_Sockets.at(IN_szRFID));
}

//=============================================================================
// Method		: Load_SocketStatus
// Access		: public  
// Returns		: bool
// Parameter	: __in LPCTSTR IN_szRFID
// Qualifier	:
// Last Update	: 2021/12/23 - 16:00
// Desc.		: 레지스트리에 저장된 소켓 상태 불러오기
//=============================================================================
bool CSocketMonitoring::Load_SocketInfo(__in LPCTSTR IN_szRFID)
{
	CSocketInfo_Unit stSocket;

	if (m_regSocket.Get_SocketInfo(IN_szRFID, stSocket))
	{
		auto result_sock = m_Sockets.find(IN_szRFID);
		if (result_sock != m_Sockets.end())
		{
			// 데이터 갱신
			m_Sockets.at(IN_szRFID) = stSocket;
		}
		else
		{
			// 데이터 추가 삽입
			m_Sockets.insert({ IN_szRFID, stSocket });
		}

		return true;
	}
	else
	{
		return false;
	}
}

bool CSocketMonitoring::Write_File(LPCTSTR IN_szPathFile, LPCTSTR IN_szData, LPCTSTR IN_szHeader)
{
	CString szFullPath = IN_szPathFile;
	CString szUnicode = IN_szData;
	CStringA szANSI;

	CFile File;
	CFileException e;

	if (!PathFileExists(szFullPath.GetBuffer()))
	{
		if (!File.Open(szFullPath.GetBuffer(), CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite, &e))
			return false;

		// 헤더 추가
		CString szHeader = IN_szHeader;
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

//=============================================================================
// Method		: Reset
// Access		: virtual public  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/12/23 - 10:38
// Desc.		: 전체 데이터  초기화
//=============================================================================
void CSocketMonitoring::Reset()
{
	for (auto Iter = m_Sockets.begin(); Iter != m_Sockets.end(); Iter++)
	{
		Iter->second.Reset();
	}
};

// 특정 소켓 데이터 초기화.
void CSocketMonitoring::Reset_Socket(__in LPCTSTR szRFID)
{
	//m_Sockets.at(szRFID).Reset();
	auto result_sock = m_Sockets.find(szRFID);
	if (result_sock != m_Sockets.end())
	{
		m_Sockets.at(szRFID).Reset();
	}
};

//=============================================================================
// Method		: Init_Sockets
// Access		: public  
// Returns		: void
// Parameter	: __in const std::vector<std::wstring> * IN_RFIDs
// Qualifier	:
// Last Update	: 2022/2/20 - 13:48
// Desc.		: 소켓 목록 업데이트 (소켓 데이터 추가)
//=============================================================================
#ifdef _UNICODE
void CSocketMonitoring::Init_Sockets(__in const std::vector<std::wstring>* IN_RFIDs)
#else
void CSocketMonitoring::Init_Sockets(__in const std::vector<std::string>* IN_RFIDs)
#endif
{
	for (auto const &element : *IN_RFIDs)
	{
		auto search = m_Sockets.find(element);
		if (search == m_Sockets.end())
		{
			CSocketInfo_Unit newData;
			m_Sockets.insert({ element, newData });
		}
	}
};

// Increase (equipment id, socket rfid, pass/fail)
//=============================================================================
// Method		: IncreasePass_byEqp
// Access		: virtual public  
// Returns		: void
// Parameter	: __in LPCTSTR szEqpID
// Parameter	: __in LPCTSTR szRFID
// Parameter	: __in uint8_t IN_nPara
// Qualifier	:
// Last Update	: 2022/2/9 - 16:56
// Desc.		:
//=============================================================================
void CSocketMonitoring::IncreasePass_byEqp(__in LPCTSTR szEqpID, __in LPCTSTR szRFID, __in uint8_t IN_nPara)
{
	auto result_sock = m_Sockets.find(szRFID);
	if (result_sock != m_Sockets.end())
	{
		m_Sockets.at(szRFID).IncreasePass_byEqp(szEqpID, IN_nPara);
	}
};

void CSocketMonitoring::IncreaseFail_byEqp(__in LPCTSTR szEqpID, __in LPCTSTR szRFID, __in uint8_t IN_nPara)
{
	auto result_sock = m_Sockets.find(szRFID);
	if (result_sock != m_Sockets.end())
	{
		m_Sockets.at(szRFID).IncreaseFail_byEqp(szEqpID, IN_nPara);
	}
};

void CSocketMonitoring::IncreaseYield_Final(__in LPCTSTR szRFID)
{
	auto result_sock = m_Sockets.find(szRFID);
	if (result_sock != m_Sockets.end())
	{
		if (0 == m_Sockets.at(szRFID).m_stTestResult.m_nNG_Code)
		{
			m_Sockets.at(szRFID).IncreasePass();
		}
		else
		{
			m_Sockets.at(szRFID).IncreaseFail();
		}
	}
}

// 소켓 갯수
//=============================================================================
// Method		: GetCount
// Access		: public  
// Returns		: size_t
// Qualifier	: const
// Last Update	: 2022/2/9 - 16:56
// Desc.		:
//=============================================================================
size_t CSocketMonitoring::GetCount() const
{
	return m_Sockets.size();
};

// 모든 소켓 삭제
void CSocketMonitoring::RemoveAll()
{
	m_Sockets.clear();
};

// 소켓 삽입
bool CSocketMonitoring::Socket_Insert(__in CSocketInfo_Unit& IN_stSocket)
{
	auto Ret = m_Sockets.insert({ IN_stSocket.szRFID.GetBuffer(), IN_stSocket });

	return Ret.second;
};

// 소켓 삭제
void CSocketMonitoring::Socket_Remove(__in LPCTSTR szRFID)
{
	if (0 < m_Sockets.size())
	{
		m_Sockets.erase(szRFID);
	}
};

// 소켓 수정
void CSocketMonitoring::Socket_Modify(__in CSocketInfo_Unit* IN_pstSocket)
{
	if (0 < m_Sockets.size())
	{
		//m_Sockets.at(IN_stSocket.szRFID.GetBuffer()) = IN_stSocket;
		auto result_sock = m_Sockets.find(IN_pstSocket->szRFID.GetBuffer());
		if (result_sock != m_Sockets.end())
		{
			m_Sockets.at(IN_pstSocket->szRFID.GetBuffer()) = *IN_pstSocket;
		}
	}
};

CSocketInfo_Unit& CSocketMonitoring::GetAt(__in LPCTSTR szRFID)
{
	return m_Sockets.at(szRFID);
}

const CSocketInfo_Unit& CSocketMonitoring::GetAt(__in LPCTSTR szRFID) const
{
	return m_Sockets.at(szRFID);
}

//=============================================================================
// Method		: Set_OwnerHWND
// Access		: public  
// Returns		: void
// Parameter	: __in HWND IN_hWnd
// Qualifier	:
// Last Update	: 2022/1/25 - 20:11
// Desc.		:
//=============================================================================
void CSocketMonitoring::Set_OwnerHWND(__in HWND IN_hWnd)
{
	m_hWndOwner = IN_hWnd;
}

void CSocketMonitoring::Set_WinMsg_UpdateUI(__in UINT IN_nWM_UpdateUI)
{
	m_nWM_Notify = IN_nWM_UpdateUI;
}

//=============================================================================
// Method		: Set_LineSocketJIG_Type
// Access		: public  
// Returns		: void
// Parameter	: __in uint8_t IN_nSocketType
// Qualifier	:
// Last Update	: 2022/1/25 - 15:02
// Desc.		:
//=============================================================================
void CSocketMonitoring::Set_LineSocketJIG_Type(__in uint8_t IN_nSocketType)
{
	m_nLineSocketJIG_Type = IN_nSocketType;
}

uint8_t CSocketMonitoring::Get_LineSocketJIG_Type() const
{
	return m_nLineSocketJIG_Type;
}

//=============================================================================
// Method		: Set_SocketConfig
// Access		: public  
// Returns		: void
// Parameter	: __in const CSocketList * IN_pSocketList
// Qualifier	:
// Last Update	: 2021/12/21 - 17:14
// Desc.		:
//=============================================================================
void CSocketMonitoring::Set_SocketConfig(__in const CSocketList* IN_pSocketList)
{
	//m_Sockets.clear();

	auto eIter = IN_pSocketList->SocketList.end();
	auto Iter = IN_pSocketList->SocketList.begin();
	while (Iter != eIter)
	{
		CSocketInfo_Unit SocketUnit;
		SocketUnit = Iter->second;

		Socket_Insert(SocketUnit);

		Iter++;
	}
}

void CSocketMonitoring::Set_SocketInfo(__in LPCTSTR IN_szRFID, __in const CSocketInfo_Unit* IN_pSocketInfo)
{
	auto result_sock = m_Sockets.find(IN_szRFID);
	if (result_sock != m_Sockets.end())
	{
		// 데이터 갱신
		m_Sockets.at(IN_szRFID) = *IN_pSocketInfo;

		// 레지스트리 갱신(??)
		Save_SocketInfo(IN_szRFID);
	}
}

void CSocketMonitoring::Set_Stored_SocketInfo(__in LPCTSTR IN_szRFID, __in const CSocketInfo_Unit* IN_pSocketInfo)
{
	auto result_sock = m_Sockets.find(IN_szRFID);
	if (result_sock != m_Sockets.end())
	{
		CConfig_Socket CfgSocket = m_Sockets.at(IN_szRFID);

		// 레지스트리에 저장된 소켓 데이터 설정
		m_Sockets.at(IN_szRFID) = *IN_pSocketInfo;

		// 설정된 소켓 정보 (Config Socket)를 다시 설정
		m_Sockets.at(IN_szRFID) = CfgSocket;
	}
}

//=============================================================================
// Method		: SetPtr_LineInfo
// Access		: public  
// Returns		: void
// Parameter	: __in const CLineInfo * IN_pConfigLine
// Qualifier	:
// Last Update	: 2022/1/26 - 19:36
// Desc.		:
//=============================================================================
void CSocketMonitoring::SetPtr_LineInfo(__in const CLineInfo* IN_pConfigLine)
{
	m_pLineInfo = IN_pConfigLine;
}

void CSocketMonitoring::SetPtr_DebugMode(const ST_DebugInfo * IN_pDebugInfo)
{
	m_pDebugInfo = IN_pDebugInfo;
}

//=============================================================================
// Method		: Set_MES_ReworkCode
// Access		: protected  
// Returns		: void
// Parameter	: __in int16_t IN_nReworkCode
// Qualifier	:
// Last Update	: 2022/5/25 - 11:44
// Desc.		:
//=============================================================================
void CSocketMonitoring::Set_MES_ReworkCode(__in int16_t IN_nReworkCode)
{
	m_nMES_ReworkCode = IN_nReworkCode;
}

int16_t CSocketMonitoring::Get_MES_ReworkCode() const
{
	return m_nMES_ReworkCode;
}

//=============================================================================
// Method		: Set_InputTime
// Access		: protected  
// Returns		: void
// Parameter	: __in LPCTSTR IN_szRFID
// Qualifier	:
// Last Update	: 2022/1/26 - 20:12
// Desc.		:
//=============================================================================
void CSocketMonitoring::Set_InputTime(__in LPCTSTR IN_szRFID)
{
	auto result_sock = m_Sockets.find(IN_szRFID);
	if (result_sock != m_Sockets.end())
	{
		// 데이터 갱신
		m_Sockets.at(IN_szRFID).Set_InputTime();

		// 레지스트리에 저장(?)
		Save_SocketInfo_InputTime(IN_szRFID);
	}
}

void CSocketMonitoring::Set_OutputTime(__in LPCTSTR IN_szRFID)
{
	auto result_sock = m_Sockets.find(IN_szRFID);
	if (result_sock != m_Sockets.end())
	{
		// 데이터 갱신
		m_Sockets.at(IN_szRFID).Set_OutputTime();

		// 레지스트리에 저장(?)
		Save_SocketInfo_OutputTime(IN_szRFID);
	}
}

//=============================================================================
// Method		: Set_Barcode
// Access		: protected  
// Returns		: void
// Parameter	: __in LPCTSTR IN_szRFID
// Parameter	: __in LPCTSTR IN_szBarcode
// Parameter	: __in bool IN_bRework
// Qualifier	:
// Last Update	: 2022/2/9 - 18:13
// Desc.		:
//=============================================================================
void CSocketMonitoring::Set_Barcode(__in LPCTSTR IN_szRFID, __in LPCTSTR IN_szBarcode, __in bool IN_bRework/* = false*/)
{
	auto result_sock = m_Sockets.find(IN_szRFID);
	if (result_sock != m_Sockets.end())
	{
		// 데이터 갱신
		m_Sockets.at(IN_szRFID).Set_Barcode(IN_szBarcode, IN_bRework);

		// 레지스트리에 저장(?)
		Save_SocketInfo_Barcode(IN_szRFID);
	}
}

void CSocketMonitoring::Set_Status(__in LPCTSTR IN_szRFID, __in uint8_t IN_nStatus)
{
	auto result_sock = m_Sockets.find(IN_szRFID);
	if (result_sock != m_Sockets.end())
	{
		// 데이터 갱신
		m_Sockets.at(IN_szRFID).Set_Status(IN_nStatus);

		// 레지스트리에 저장(?)
		Save_SocketInfo_Status(IN_szRFID);
	}
}

void CSocketMonitoring::Set_Judgment_Final(__in LPCTSTR IN_szRFID, __in uint8_t IN_nJudge)
{
	// 최종 검사 진행 후....


	// 목적지 -> 리터너
	
	// Status 업데이트
	
	// 전체 수율 업데이트


}

void CSocketMonitoring::Set_Equipment(__in LPCTSTR IN_szRFID, __in LPCTSTR IN_szEqpID)
{
	auto result_sock = m_Sockets.find(IN_szRFID);
	if (result_sock != m_Sockets.end())
	{
		// 데이터 갱신
		m_Sockets.at(IN_szRFID).Set_Equipment(IN_szEqpID);

		// 레지스트리에 저장(?)
		Save_SocketInfo_Equipment(IN_szRFID);
	}
}

void CSocketMonitoring::Set_EquipmentTrackIn(__in LPCTSTR IN_szRFID, __in uint8_t IN_nEqpOrder, __in LPCTSTR IN_szEqpID)
{
	auto result_sock = m_Sockets.find(IN_szRFID);
	if (result_sock != m_Sockets.end())
	{
		// 데이터 갱신
		m_Sockets.at(IN_szRFID).Set_TrackInTime();
		m_Sockets.at(IN_szRFID).Set_Equipment(IN_szEqpID);
		m_Sockets.at(IN_szRFID).Set_EquipmentOrder(IN_nEqpOrder);

		// 레지스트리에 저장(?)
		Save_SocketInfo_Equipment(IN_szRFID);
	}
}

void CSocketMonitoring::Set_Location(__in LPCTSTR IN_szRFID, __in LPCTSTR IN_szEqpID, __in uint8_t IN_nLocation)
{
	auto result_sock = m_Sockets.find(IN_szRFID);
	if (result_sock != m_Sockets.end())
	{
		// 데이터 갱신
		m_Sockets.at(IN_szRFID).Set_Location_inEqp(IN_szEqpID, IN_nLocation);

		// 레지스트리에 저장(?)
		Save_SocketInfo_Location(IN_szRFID);
	}
}

void CSocketMonitoring::Set_TargetEquipment(__in LPCTSTR IN_szRFID, __in uint8_t IN_nEqpOrder, __in LPCTSTR IN_szEqpID)
{
	auto result_sock = m_Sockets.find(IN_szRFID);
	if (result_sock != m_Sockets.end())
	{
		// 데이터 갱신
		m_Sockets.at(IN_szRFID).Set_TargetEqpOrder(IN_nEqpOrder);
		m_Sockets.at(IN_szRFID).Set_TargetEquipment(IN_szEqpID);

		// 레지스트리에 저장(?)
		Save_SocketInfo_Target(IN_szRFID);
	}
}

void CSocketMonitoring::Set_Judgment_Tester(__in LPCTSTR IN_szRFID, __in uint8_t IN_nTester, __in uint8_t IN_nJudge)
{
	auto result_sock = m_Sockets.find(IN_szRFID);
	if (result_sock != m_Sockets.end())
	{
		// 데이터 갱신
		m_Sockets.at(IN_szRFID).Set_Judgment_Tester(IN_nTester, IN_nJudge);


		// 수율 계산


		// 레지스트리에 저장(?)
		//Save_ProductInfo(IN_szRFID);
	}
}

//=============================================================================
// Method		: Reset_Socket_Yield
// Access		: public  
// Returns		: void
// Parameter	: __in LPCTSTR szRFID
// Qualifier	:
// Last Update	: 2022/2/20 - 13:46
// Desc.		:
//=============================================================================
void CSocketMonitoring::Reset_Socket_Yield(__in LPCTSTR szRFID)
{
	auto result_sock = m_Sockets.find(szRFID);
	if (result_sock != m_Sockets.end())
	{
		m_Sockets.at(szRFID).Reset_Yield();

		Save_SocketInfo_Yield(szRFID);
	}
}

void CSocketMonitoring::Reset_Socket_Yield_All()
{
	for (auto Iter = m_Sockets.begin(); Iter != m_Sockets.end(); Iter++)
	{
		Iter->second.Reset_Yield();

		Save_SocketInfo_Yield(Iter->second.szRFID);
	}
}

//=============================================================================
// Method		: Is_ExistSocket
// Access		: public  
// Returns		: bool
// Parameter	: __in LPCTSTR IN_szRFID
// Qualifier	:
// Last Update	: 2022/1/24 - 20:16
// Desc.		:
//=============================================================================
bool CSocketMonitoring::Is_ExistSocket(__in LPCTSTR IN_szRFID)
{
	auto result_sock = m_Sockets.find(IN_szRFID);
	if (result_sock != m_Sockets.end())
	{
		return true;
	}
	else
	{
#if defined(EES_XML)//20231003
		int i = 0;
		wstring RFID(IN_szRFID);
		for (auto Iter = m_Sockets.begin(); Iter != m_Sockets.end(); Iter++) {
			if (RFID == Iter->first) {
				return true;
			}
		}
#endif
		return false;
	}
}

bool CSocketMonitoring::Verify_Socket(LPCTSTR IN_szRFID)
{
	if (Is_ExistSocket(IN_szRFID))
	{
		if (m_nLineSocketJIG_Type != m_Sockets.at(IN_szRFID).nSocketType)
		{
			return false;
		}
	}
	else
	{
		return false;
	}

	return true;
}

//=============================================================================
// Method		: Register_Socket
// Access		: public  
// Returns		: bool
// Parameter	: __in LPCTSTR IN_FromEqpID
// Parameter	: __in LPCTSTR IN_szRFID
// Parameter	: __in LPCTSTR IN_szBarcode
// Qualifier	:
// Last Update	: 2022/1/26 - 18:09
// Desc.		:
//=============================================================================
bool CSocketMonitoring::Register_Socket(__in LPCTSTR IN_FromEqpID, __in LPCTSTR IN_szRFID, __in LPCTSTR IN_szBarcode)
{
	// 로더 -> 서버로 통신

	// 정상이면 (등록되어있나?, 검사가 종료 상태 인가?, Socket Type이 현재 설정과 같은가?
	auto result_sock = m_Sockets.find(IN_szRFID);
	if (result_sock != m_Sockets.end())
	{
		// 현재 설정된 소켓 타입과 로더에서 등록하려는 소켓 타입이 다름
		if (m_nLineSocketJIG_Type != m_Sockets.at(IN_szRFID).nSocketType)
		{
			return false;
		}

		// Unregister 된 상태인가?
		if (enSocketStatus::SoS_Ready == m_Sockets.at(IN_szRFID).m_nStatus)
		{
			;
		}
		if (m_Sockets.at(IN_szRFID).IsEmpty_Barcode())
		{
			;// empty ?
		}


		// 소켓 데이터 초기화 (?)
		Reset_RegisterData(IN_szRFID);

		// 투입 시간 설정
		Set_InputTime(IN_szRFID);

		// 소켓 및 제품 등록
#ifdef USE_DEBUG_MODE

		Set_Barcode(IN_szRFID, IN_szBarcode);

#else
		Set_Barcode(IN_szRFID, IN_szBarcode);
#endif // USE_DEBUG_MODE

		// 소켓 상태 : 정상 (제품 없으면 Bypass)
		if (0 < _tcslen(IN_szBarcode))
		{
			Set_Status(IN_szRFID, (uint8_t)enSocketStatus::SoS_Pass);
		}
		else
		{
#ifdef USE_DEBUG_MODE
			if (m_pDebugInfo->Is_Use_NotBypass_Empty())
			{
				Set_Status(IN_szRFID, (uint8_t)enSocketStatus::SoS_Pass);
			}
			else
			{
				Set_Status(IN_szRFID, (uint8_t)enSocketStatus::SoS_Bypass);
			}
#else
			Set_Status(IN_szRFID, (uint8_t)enSocketStatus::SoS_Bypass);
#endif // USE_DEBUG_MODE
		}
		
		// 소켓이 있는 설비 설정
		Set_Equipment(IN_szRFID, IN_FromEqpID);
		
		// GUI 업데이트
		//uint16_t nFlag = WM_Socket_Barcode | WM_Socket_Status | WM_Socket_Equipment;
		WM_Update_GUI_Data(IN_szRFID, WM_Socket_Barcode | WM_Socket_Status | WM_Socket_Equipment);

		return true;
	}

	return false;
}

//=============================================================================
// Method		: Unregister_Socket
// Access		: public  
// Returns		: bool
// Parameter	: __in LPCTSTR IN_FromEqpID
// Parameter	: __in LPCTSTR IN_szRFID
// Qualifier	:
// Last Update	: 2022/2/9 - 16:57
// Desc.		:
//=============================================================================
bool CSocketMonitoring::Unregister_Socket(__in LPCTSTR IN_FromEqpID, __in LPCTSTR IN_szRFID)
{
	// 로더(언로드) -> 서버로 통신

	// 정상이면 (등록?, 검사? 불량?)
	auto result_sock = m_Sockets.find(IN_szRFID);
	if (result_sock != m_Sockets.end())
	{
		// 배출 시간 설정
		Set_OutputTime(IN_szRFID);

		// 최종 수율 업데이트 (소켓 기준)
		if (0 == m_Sockets.at(IN_szRFID).m_stTestResult.m_nNG_Code)
		{	// Pass
			IncreasePass_byEqp(IN_FromEqpID, IN_szRFID, Para_Left);
		}
		else
		{	// Fail
			IncreaseFail_byEqp(IN_FromEqpID, IN_szRFID, m_Sockets.at(IN_szRFID).m_stTestResult.m_nNG_Para);
		}

		IncreaseYield_Final(IN_szRFID);
		WM_Update_GUI_Data(IN_szRFID, WM_Socket_Yield);

		// Log 파일 작성

		// 불량인가? (불량 제품 이력 처리)



		// 설비 : 로더
		// 설비 위치 : 초기화
		// 타겟 초기화
		// Test Result 초기화
//		Reset_RegisterData(IN_szRFID);

		// GUI 업데이트
// 		uint16_t nFlag = WM_Socket_Barcode | WM_Socket_Status | WM_Socket_Target | WM_Socket_TestResult;
// 		WM_Update_GUI_Data(IN_szRFID, nFlag);

		return true;
	}

	return false;
}

//=============================================================================
// Method		: Get_TestResult
// Access		: public  
// Returns		: bool
// Parameter	: __in LPCTSTR IN_szRFID
// Parameter	: __out ST_TestResult & OUT_stTestResult
// Qualifier	:
// Last Update	: 2022/1/26 - 18:09
// Desc.		:
//=============================================================================
bool CSocketMonitoring::Get_TestResult(__in LPCTSTR IN_szRFID, __out ST_TestResult& OUT_stTestResult)
{
	auto result_sock = m_Sockets.find(IN_szRFID);
	if (result_sock != m_Sockets.end())
	{
		// 데이터 반환
		OUT_stTestResult = m_Sockets.at(IN_szRFID).m_stTestResult;

		return true;
	}

	return false;
}

//=============================================================================
// Method		: Check_UnloadTime
// Access		: public  
// Returns		: void
// Parameter	: __in LPCTSTR IN_szRFID
// Qualifier	:
// Last Update	: 2022/2/18 - 19:18
// Desc.		:
//=============================================================================
void CSocketMonitoring::Check_UnloadTime(__in LPCTSTR IN_szRFID)
{
	// 배출 시간 설정
	Set_OutputTime(IN_szRFID);
}

//=============================================================================
// Method		: Set_TestResult
// Access		: public  
// Returns		: bool
// Parameter	: __in LPCTSTR IN_FromEqpID
// Parameter	: __in LPCTSTR IN_szRFID
// Parameter	: __in int16_t IN_nNGCode
// Parameter	: __in uint8_t IN_nPara
// Parameter	: __in uint8_t IN_nNG_EqpOrder
// Parameter	: __in uint8_t IN_nNG_EqpType
// Parameter	: __in bool IN_bRework
// Qualifier	:
// Last Update	: 2022/7/20 - 20:01
// Desc.		:
//=============================================================================
bool CSocketMonitoring::Set_TestResult(__in LPCTSTR IN_FromEqpID, __in LPCTSTR IN_szRFID, __in int16_t IN_nNGCode, __in uint8_t IN_nPara, __in uint8_t IN_nNG_EqpOrder, __in uint8_t IN_nNG_EqpType, __in bool IN_bRework /*= false*/)
{
	auto result_sock = m_Sockets.find(IN_szRFID);
	if (result_sock != m_Sockets.end())
	{
		// 데이터 갱신
		m_Sockets.at(IN_szRFID).Set_TestResult(IN_nNGCode, IN_nPara, IN_nNG_EqpOrder, IN_FromEqpID, IN_nNG_EqpType);

		Save_SocketInfo_TestResult(IN_szRFID);

		// 수율 업데이트
		if (0 == IN_nNGCode) // 양품
		{
			IncreasePass_byEqp(IN_FromEqpID, IN_szRFID, IN_nPara);
		}
		else // 불량
		{
			if (false == IN_bRework)
			{
				IncreaseFail_byEqp(IN_FromEqpID, IN_szRFID, IN_nPara);
			}
		}

		Save_SocketInfo_Yield(IN_szRFID);

		uint16_t nFlag = WM_Socket_TestResult | WM_Socket_Yield;

		// 불량이고, 재검사 제품이 아니면 fail 처리
		if ((0 != IN_nNGCode) && (false == IN_bRework))
		{
#ifdef USE_DEBUG_MODE
			if (m_pDebugInfo->Is_NotBypass_NG())
			{
				Set_Status(IN_szRFID, (uint8_t)enSocketStatus::SoS_Pass);
			}
			else
			{
				Set_Status(IN_szRFID, (uint8_t)enSocketStatus::SoS_Fail);
			}
#else
			Set_Status(IN_szRFID, (uint8_t)enSocketStatus::SoS_Fail);
#endif

			//목적지 : 리터너
			//Set_TargetEquipment(IN_szRFID, );

			// 불량 제품 정보 저장 (DB or 레지스트리)

			nFlag |= (WM_Socket_Status);
		}

		// GUI 업데이트
		WM_Update_GUI_Data(IN_szRFID, nFlag);

		return true;
	}

	return false;
}

bool CSocketMonitoring::Set_TestResult(__in LPCTSTR IN_FromEqpID, __in LPCTSTR IN_szRFID, __in const ST_TestResult* IN_pstTestResult, __in bool IN_bRework /*= false*/)
{
	return Set_TestResult(IN_FromEqpID, IN_szRFID, IN_pstTestResult->m_nNG_Code, IN_pstTestResult->m_nNG_Para, IN_pstTestResult->m_nNG_EqpOrder, IN_pstTestResult->m_nNG_EqpType, IN_bRework);
}

//=============================================================================
// Method		: Set_SocketLocation
// Access		: public  
// Returns		: void
// Parameter	: __in LPCTSTR IN_szRFID
// Parameter	: __in LPCTSTR IN_szEqpID
// Parameter	: __in uint8_t IN_nLocation
// Qualifier	:
// Last Update	: 2022/2/16 - 19:01
// Desc.		:
//=============================================================================
void CSocketMonitoring::Set_SocketLocation(__in LPCTSTR IN_szRFID, __in LPCTSTR IN_szEqpID, __in uint8_t IN_nLocation)
{
	TRACE(_T("Set_SocketLocation() => rfid: %s, eqp id: %s, port: %d \n"), IN_szRFID, IN_szEqpID, IN_nLocation);
	Set_Equipment(IN_szRFID, IN_szEqpID);
	Set_Location(IN_szRFID, IN_szEqpID, IN_nLocation);

	//uint16_t nFlag = WM_Socket_Equipment | WM_Socket_Location;
	WM_Update_GUI_Data(IN_szRFID, WM_Socket_Equipment | WM_Socket_Location);
}

//=============================================================================
// Method		: Set_SocketTarget
// Access		: public  
// Returns		: void
// Parameter	: __in LPCTSTR IN_szRFID
// Parameter	: __in uint8_t IN_nEqpOrder
// Parameter	: __in LPCTSTR IN_szEqpID
// Qualifier	:
// Last Update	: 2022/2/16 - 19:35
// Desc.		:
//=============================================================================
void CSocketMonitoring::Set_SocketTarget(__in LPCTSTR IN_szRFID, __in uint8_t IN_nEqpOrder, __in LPCTSTR IN_szEqpID)
{
	Set_TargetEquipment(IN_szRFID, IN_nEqpOrder, IN_szEqpID);

	//uint16_t nFlag = WM_Socket_Target;
	WM_Update_GUI_Data(IN_szRFID, WM_Socket_Target);
}

//=============================================================================
// Method		: Set_SocketTrackIn
// Access		: public  
// Returns		: void
// Parameter	: __in LPCTSTR IN_szRFID
// Parameter	: __in uint8_t IN_nEqpOrder
// Parameter	: __in LPCTSTR IN_szEqpID
// Qualifier	:
// Last Update	: 2023/7/25 - 21:30
// Desc.		:
//=============================================================================
void CSocketMonitoring::Set_SocketTrackIn(__in LPCTSTR IN_szRFID, __in uint8_t IN_nEqpOrder, __in LPCTSTR IN_szEqpID)
{
	Set_EquipmentTrackIn(IN_szRFID, IN_nEqpOrder, IN_szEqpID);

	WM_Update_GUI_Data(IN_szRFID, WM_Socket_Equipment);
}

//=============================================================================
// Method		: Reset_RegisterData
// Access		: protected  
// Returns		: void
// Parameter	: __in LPCTSTR IN_szRFID
// Qualifier	:
// Last Update	: 2022/2/9 - 17:00
// Desc.		:
//=============================================================================
void CSocketMonitoring::Reset_RegisterData(__in LPCTSTR IN_szRFID)
{
	auto result_sock = m_Sockets.find(IN_szRFID);
	if (result_sock != m_Sockets.end())
	{
		m_Sockets.at(IN_szRFID).Reset_Barcode();
		m_Sockets.at(IN_szRFID).Set_Status((uint8_t)enSocketStatus::SoS_Ready);
		//m_Sockets.at(IN_szRFID).Set_Equipment(m_pLineInfo->Get_Loader().Get_EquipmentId());
		m_Sockets.at(IN_szRFID).Set_Location_inEqp(m_pLineInfo->Get_Loader().Get_EquipmentId(), 0);
		m_Sockets.at(IN_szRFID).Set_TargetEqpOrder(0);
		m_Sockets.at(IN_szRFID).Set_TargetEquipment(_T(""));
		m_Sockets.at(IN_szRFID).Reset_TestResult();
		//m_Sockets.at(IN_szRFID).Reset_TestTime();

		// 레지스트리 저장
		Save_SocketInfo(IN_szRFID);

#if defined(EES_XML)//20231003
		m_Sockets.at(IN_szRFID).Set_Lot(_T(""));
#endif

		// 설비 : 로더
		// 설비 위치 : 초기화
		// 타겟 초기화
		// Test Result 초기화
		// GUI 업데이트
		//uint16_t nFlag = WM_Socket_Barcode | WM_Socket_Status | WM_Socket_Target | WM_Socket_TestResult;
		WM_Update_GUI_Data(IN_szRFID, WM_Socket_Barcode | WM_Socket_Equipment | WM_Socket_Status | WM_Socket_Target | WM_Socket_TestResult);
	}
}

void CSocketMonitoring::Reset_RegisterData_All()
{
	for (auto Iter = m_Sockets.begin(); Iter != m_Sockets.end(); Iter++)
	{
		Iter->second.Reset_Barcode();
		Iter->second.Set_Status((uint8_t)enSocketStatus::SoS_Ready);
		//Iter->second.Set_Equipment(m_pLineInfo->Get_Loader().Get_EquipmentId());
		Iter->second.Set_Location_inEqp(m_pLineInfo->Get_Loader().Get_EquipmentId(), 0);
		Iter->second.Set_TargetEqpOrder(0);
		Iter->second.Set_TargetEquipment(_T(""));
		Iter->second.Reset_TestResult();
		//Iter->second.Reset_TestTime();

#if defined(EES_XML)//20231003
		Iter->second.Set_Lot(_T(""));
#endif

		// 레지스트리 저장
		Save_SocketInfo(Iter->second.szRFID);
	}

	//WM_Update_GUI_Data_All(WM_Socket_Barcode | WM_Socket_Equipment | WM_Socket_Status | WM_Socket_Target | WM_Socket_TestResult);
	WM_Update_GUI_Data_All(WM_Socket_All);
}

//=============================================================================
// Method		: Reset_TestResult
// Access		: public  
// Returns		: void
// Parameter	: __in LPCTSTR IN_szRFID
// Qualifier	:
// Last Update	: 2022/2/16 - 19:46
// Desc.		:
//=============================================================================
void CSocketMonitoring::Reset_TestResult(__in LPCTSTR IN_szRFID)
{


	Save_SocketInfo_TestResult(IN_szRFID);

	WM_Update_GUI_Data(IN_szRFID, WM_Socket_TestResult);
}

//=============================================================================
// Method		: Reset_Yield
// Access		: public  
// Returns		: void
// Parameter	: __in LPCTSTR IN_szRFID
// Qualifier	:
// Last Update	: 2022/2/20 - 13:51
// Desc.		:
//=============================================================================
void CSocketMonitoring::Reset_Yield(__in LPCTSTR IN_szRFID)
{
	Reset_Socket_Yield(IN_szRFID);

	WM_Update_GUI_Data(IN_szRFID, WM_Socket_Yield);
}

//=============================================================================
// Method		: Reset_Yield_All
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2022/2/20 - 13:51
// Desc.		:
//=============================================================================
void CSocketMonitoring::Reset_Yield_All()
{
	Reset_Socket_Yield_All();

	WM_Update_GUI_Data_All(WM_Socket_Yield);
}
#define CSV_HEADER_SOCKETINFO _T("RFID,All,Pass,NG\r\n")
bool CSocketMonitoring::Write_CSV_File(LPCTSTR IN_szPath)
{
	SYSTEMTIME tmLc;
	GetLocalTime(&tmLc);

	CString szFile;

	szFile.Format(_T("%s\\SocketInfo_%04d%02d%02d_%02d%02d%02d.csv"),
		IN_szPath,
		tmLc.wYear, tmLc.wMonth, tmLc.wDay,
		tmLc.wHour, tmLc.wMinute, tmLc.wSecond
	);

	CString szData;
	CString szLine;

	for (auto Iter = m_Sockets.begin(); Iter != m_Sockets.end(); Iter++)
	{
		//Iter->first;
		szLine.Format(_T("%s,%d,%d,%d\r\n"), 
			Iter->first.c_str(),
			Iter->second.m_Yield.dwTotal,
			Iter->second.m_Yield.dwPass,
			Iter->second.m_Yield.dwFail);

		szData += szLine;
	}

	return Write_File(szFile, szData, CSV_HEADER_SOCKETINFO);
}

//=============================================================================
// Method		: Is_TrackInSocket
// Access		: public  
// Returns		: bool
// Parameter	: __in LPCTSTR IN_szRFID
// Parameter	: __in uint8_t IN_nEqpOrder
// Qualifier	:
// Last Update	: 2023/7/25 - 21:38
// Desc.		:
//=============================================================================
bool CSocketMonitoring::Is_TrackInSocket(__in LPCTSTR IN_szRFID, __in uint8_t IN_nEqpOrder)
{
	auto result_sock = m_Sockets.find(IN_szRFID);
	if (result_sock != m_Sockets.end())
	{
		if (IN_nEqpOrder == m_Sockets.at(IN_szRFID).m_nEqpOrder)
			return true;
	}

	return false;
}

//=============================================================================
// Method		: Get_ElapsedTime_TrackIn
// Access		: public  
// Returns		: uint32_t
// Parameter	: __in LPCTSTR IN_szRFID
// Qualifier	:
// Last Update	: 2023/7/27 - 22:02
// Desc.		:
//=============================================================================
uint32_t CSocketMonitoring::Get_ElapsedTime_TrackIn(__in LPCTSTR IN_szRFID)
{
	auto result_sock = m_Sockets.find(IN_szRFID);
	if (result_sock != m_Sockets.end())
	{
		return m_Sockets.at(IN_szRFID).Get_ElapsedTime_TrackIn();
	}

	return 0;
}

#if defined(EES_XML)//20231003
void CSocketMonitoring::Set_LOTID(__in LPCTSTR IN_szRFID, __in LPCTSTR IN_szData){
	auto result_sock = m_Sockets.find(IN_szRFID);
	if (result_sock != m_Sockets.end())
	{
		m_Sockets.at(IN_szRFID).Set_Lot(IN_szData);
		Save_SocketInfo_LOTID(IN_szRFID);
	}
}
bool CSocketMonitoring::Save_SocketInfo_LOTID(__in LPCTSTR IN_szRFID){
	return m_regSocket.Set_SocketInfo_LOTID(IN_szRFID, &m_Sockets.at(IN_szRFID));
}
bool CSocketMonitoring::Register_SocketLotID(LPCTSTR IN_szRFID, __in uint32_t IN_TYPE, __in LPCTSTR IN_szData){
	switch (IN_TYPE) {
	case 0:
	{
		auto result_sock = m_Sockets.find(IN_szRFID);
		if (result_sock != m_Sockets.end())
		{
			// ?�재 ?�정???�켓 ?�?�과 로더?�서 ?�록?�려???�켓 ?�?�이 ?�름
			if (m_nLineSocketJIG_Type != m_Sockets.at(IN_szRFID).nSocketType)
			{
				return false;
			}
			// Unregister ???�태?��??
			if (enSocketStatus::SoS_Ready == m_Sockets.at(IN_szRFID).m_nStatus)
			{
			}
			if (m_Sockets.at(IN_szRFID).IsEmpty_Barcode())
			{
				;// empty ?
			}
			Set_LOTID(IN_szRFID, IN_szData);
			WM_Update_GUI_Data(IN_szRFID, WM_Socket_LOTID);
			return true;
		}
	}
	break;
	case 1:
	{
		CString BARCODE(IN_szRFID);
		for (auto Iter = m_Sockets.begin(); Iter != m_Sockets.end(); Iter++) {
			if (BARCODE == Iter->second.m_szBarcode) {
				auto RFID = Iter->first;
				Set_LOTID(RFID.c_str(), IN_szData);
				WM_Update_GUI_Data(RFID.c_str(), WM_Socket_LOTID);
			}
		}
	}
	break;
	}
	return false;
}
bool CSocketMonitoring::Is_ExistBarcode(__in LPCTSTR IN_szBarcode)
{
	CString BARCODE(IN_szBarcode);
	for (auto Iter = m_Sockets.begin(); Iter != m_Sockets.end(); Iter++) {
		if (BARCODE == Iter->second.m_szBarcode) {
			return true;
		}
	}
	return false;
}
#endif