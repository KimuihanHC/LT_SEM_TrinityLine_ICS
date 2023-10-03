//*****************************************************************************
// Filename	: 	SocketMonitoring.h
// Created	:	2021/11/18 - 14:20
// Modified	:	2021/11/18 - 14:20
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#ifndef SocketMonitoring_h__
#define SocketMonitoring_h__


#pragma once

#include <map>
#include "Def_Socket_Info.h"
#include "Def_Yield.h"
#include "RegSocket.h"
#include "Def_DebugInfo.h"


class CLineInfo;
class CRework_Info;

//-----------------------------------------------------------------------------
// CSocketMonitoring
//-----------------------------------------------------------------------------
class CSocketMonitoring
{
public:
	CSocketMonitoring();
	~CSocketMonitoring();

	CSocketMonitoring& operator= (const CSocketList& ref);
	CSocketMonitoring& operator= (const CSocketMonitoring& ref);

//protected:

	// 소켓 정보 (key: RFID)
#ifdef _UNICODE
	std::map<std::wstring, CSocketInfo_Unit> m_Sockets;
#else
	std::map<std::string, CSocketInfo_Unit> m_Sockets;
#endif

protected:
	// 레지스트리에 데이터 저장용도
	CRegSocket		m_regSocket;

	uint8_t			m_nLineSocketJIG_Type	= 0;	// H, V

	HWND			m_hWndOwner				= nullptr;
	UINT			m_nWM_Notify			= NULL;
	inline void		WM_Update_GUI_Data		(__in LPCTSTR IN_szRFID, __in uint16_t IN_nFlag);
	inline void		WM_Update_GUI_Data_All	(__in uint16_t IN_nFlag);

	// 라인 정보
	const CLineInfo*	m_pLineInfo			= nullptr;

	// 디버그 정보
	const ST_DebugInfo*	m_pDebugInfo		= nullptr;

	// MES Rework NG Code
	int16_t				m_nMES_ReworkCode	= 0;	// REWORK 제품 MES 결과 코드: 설비에서 해당코드로 리턴 할 경우 REWORK 제품으로 양품 처리

	// 파일 or 레지스트리 처리
	bool	Save_SocketInfo				(__in LPCTSTR IN_szRFID);
	bool	Save_SocketInfo_Barcode		(__in LPCTSTR IN_szRFID);
	bool	Save_SocketInfo_Status		(__in LPCTSTR IN_szRFID);
	bool	Save_SocketInfo_Equipment	(__in LPCTSTR IN_szRFID);
	bool	Save_SocketInfo_Location	(__in LPCTSTR IN_szRFID);
	bool	Save_SocketInfo_Target		(__in LPCTSTR IN_szRFID);
	bool	Save_SocketInfo_Yield		(__in LPCTSTR IN_szRFID);
	bool	Save_SocketInfo_TestResult	(__in LPCTSTR IN_szRFID);
	bool	Save_SocketInfo_InputTime	(__in LPCTSTR IN_szRFID);
	bool	Save_SocketInfo_OutputTime	(__in LPCTSTR IN_szRFID);

	bool	Load_SocketInfo				(__in LPCTSTR IN_szRFID);

	//bool	Save_ReworkInfo				(__in LPCTSTR IN_szRFID);
	//bool	Load_ReworkInfo				(__in LPCTSTR IN_szBarcode, __in LPCTSTR IN_szRFID);
	//bool	Remove_ReworkInfo			(__in LPCTSTR IN_szBarcode);

	bool	Write_File					(__in LPCTSTR IN_szPathFile, __in LPCTSTR IN_szData, __in LPCTSTR IN_szHeader);

public:

	// 전체 데이터 초기화
	void	Reset						();	

	// 특정 소켓 데이터 초기화
	void	Reset_Socket				(__in LPCTSTR szRFID);	

	// 소켓 목록 업데이트 (소켓 데이터 추가)
#ifdef _UNICODE
	void	Init_Sockets				(__in const std::vector<std::wstring>* IN_RFIDs);
#else
	void	Init_Sockets				(__in const std::vector<std::string>* IN_RFIDs);
#endif

	// 생산수량 증가 (equipment id, socket rfid, pass/fail)
	void	IncreasePass_byEqp			(__in LPCTSTR szEqpID, __in LPCTSTR szRFID, __in uint8_t IN_nPara);
	void	IncreaseFail_byEqp			(__in LPCTSTR szEqpID, __in LPCTSTR szRFID, __in uint8_t IN_nPara);
	// Unload시 Unregister 할 때 처리
	void	IncreaseYield_Final			(__in LPCTSTR szRFID);

	// 소켓 갯수
	size_t	GetCount					() const;

	// 모든 소켓 삭제
	void	RemoveAll					();

	// 소켓 삽입
	bool	Socket_Insert				(__in CSocketInfo_Unit& IN_stSocket);

	// 소켓 삭제
	void	Socket_Remove				(__in LPCTSTR szRFID);

	// 소켓 수정
	void	Socket_Modify				(__in CSocketInfo_Unit* IN_pstSocket);

	// RFID 정보로 소켓 정보 얻기
	CSocketInfo_Unit&		GetAt		(__in LPCTSTR szRFID);
	const CSocketInfo_Unit& GetAt		(__in LPCTSTR szRFID) const;


public:

	// Data를 GUI에 표시하기 위해 사용
	void	Set_OwnerHWND				(__in HWND IN_hWnd);
	// Notify Windows Message 설정
	void	Set_WinMsg_UpdateUI			(__in UINT IN_nWM_UpdateUI);


	// 현재 사용 설정된 소켓 JIG Type
	void	Set_LineSocketJIG_Type		(__in uint8_t IN_nSocketType);
	uint8_t	Get_LineSocketJIG_Type		() const;

	// 소켓 RFID 기준으로 Socket Type과 Comment가 변경된 경우
	void	Set_SocketConfig			(__in const CSocketList* IN_pSocketList);
	// 소켓 정보 설정
	void	Set_SocketInfo				(__in LPCTSTR IN_szRFID, __in const CSocketInfo_Unit* IN_pSocketInfo);
	// 프로그램 시작 시 저장된 소켓 정보 설정
	void    Set_Stored_SocketInfo		(__in LPCTSTR IN_szRFID, __in const CSocketInfo_Unit* IN_pSocketInfo);

	void	SetPtr_LineInfo				(__in const CLineInfo* IN_pConfigLine);

	void	SetPtr_DebugMode			(__in const ST_DebugInfo* IN_pDebugInfo);

	// MES Rework NG Code
	void	Set_MES_ReworkCode			(__in int16_t IN_nReworkCode);
	int16_t	Get_MES_ReworkCode			() const;

protected:

	// 로더 투입시간
	void	Set_InputTime				(__in LPCTSTR IN_szRFID);
	// 언로더 배출 시간
	void	Set_OutputTime				(__in LPCTSTR IN_szRFID);

	// 소켓의 제품 바코드 설정
	void	Set_Barcode					(__in LPCTSTR IN_szRFID, __in LPCTSTR IN_szBarcode, __in bool IN_bRework = false);	
	
	// 소켓의 상태 설정
	void	Set_Status					(__in LPCTSTR IN_szRFID, __in uint8_t IN_nStatus);
	
	// 제품의 최종 양/불 판정
	void	Set_Judgment_Final			(__in LPCTSTR IN_szRFID, __in uint8_t IN_nJudge);

	// 소켓이 있는 설비 설정
	void	Set_Equipment				(__in LPCTSTR IN_szRFID, __in LPCTSTR IN_szEqpID);
	void	Set_EquipmentTrackIn(__in LPCTSTR IN_szRFID, __in uint8_t IN_nEqpOrder, __in LPCTSTR IN_szEqpID);

	// 설비내의 소켓 위치 (Port, Conveyor) 설정 가능하면..
	void	Set_Location				(__in LPCTSTR IN_szRFID, __in LPCTSTR IN_szEqpID, __in uint8_t IN_nLocation);
	
	// 소켓의 목적 설비 설정
	void	Set_TargetEquipment			(__in LPCTSTR IN_szRFID, __in uint8_t IN_nEqpOrder, __in LPCTSTR IN_szEqpID);
	
	// 제품의 검사별 결과 설정
	void	Set_Judgment_Tester			(__in LPCTSTR IN_szRFID, __in uint8_t IN_nTester, __in uint8_t IN_nJudge);

	void	Reset_Socket_Yield			(__in LPCTSTR szRFID);
	void	Reset_Socket_Yield_All		();

	

public:
	// 통신 이벤트 처리 --------------------------------------------------------
	// 소켓 투입 가능 ?
	bool	Is_ExistSocket				(__in LPCTSTR IN_szRFID);
	bool	Verify_Socket				(__in LPCTSTR IN_szRFID);
	bool	Register_Socket				(__in LPCTSTR IN_FromEqpID, __in LPCTSTR IN_szRFID, __in LPCTSTR IN_szBarcode);
	bool	Unregister_Socket			(__in LPCTSTR IN_FromEqpID, __in LPCTSTR IN_szRFID);
	bool	Get_TestResult				(__in LPCTSTR IN_szRFID, __out ST_TestResult& OUT_stTestResult);
	void	Check_UnloadTime			(__in LPCTSTR IN_szRFID);
	//bool	Get_TestResult				(__in LPCTSTR IN_FromEqpID, __in LPCTSTR IN_szRFID, __out ST_TestResult& OUT_stTestResult);	
	bool	Set_TestResult				(__in LPCTSTR IN_FromEqpID, __in LPCTSTR IN_szRFID, __in int16_t IN_nNGCode, __in uint8_t IN_nPara, __in uint8_t IN_nNG_EqpOrder, __in uint8_t IN_nNG_EqpType, __in bool IN_bRework = false);
	bool	Set_TestResult				(__in LPCTSTR IN_FromEqpID, __in LPCTSTR IN_szRFID, __in const ST_TestResult* IN_pstTestResult, __in bool IN_bRework = false);
	
	void	Set_SocketLocation			(__in LPCTSTR IN_szRFID, __in LPCTSTR IN_szEqpID, __in uint8_t IN_nLocation);
	void	Set_SocketTarget			(__in LPCTSTR IN_szRFID, __in uint8_t IN_nEqpOrder, __in LPCTSTR IN_szEqpID);
	void	Set_SocketTrackIn			(__in LPCTSTR IN_szRFID, __in uint8_t IN_nEqpOrder, __in LPCTSTR IN_szEqpID);

	// 소켓 데이터 초기화CTestManager_Device::CTestManager_Device()
	void	Reset_RegisterData			(__in LPCTSTR IN_szRFID);
	void	Reset_RegisterData_All		();
	void	Reset_TestResult			(__in LPCTSTR IN_szRFID);
	void	Reset_Yield					(__in LPCTSTR IN_szRFID);
	void	Reset_Yield_All				();
	
	// 소켓 결과 Log
	bool	Write_CSV_File				(__in LPCTSTR IN_szPath);
	
	// 해당 설비에 소켓이 트랙인 처리가 되어 있는가?
	bool	Is_TrackInSocket(__in LPCTSTR IN_szRFID, __in uint8_t IN_nEqpOrder);
	uint32_t Get_ElapsedTime_TrackIn(__in LPCTSTR IN_szRFID);

#if (USE_XML)
protected:
	void	Set_LOTID					(__in LPCTSTR IN_szRFID, __in LPCTSTR IN_szData);
	bool	Save_SocketInfo_LOTID		(__in LPCTSTR IN_szRFID);
public:
	bool	Is_ExistBarcode				(__in LPCTSTR IN_szBarcode);
	bool	Register_SocketLotID		(__in LPCTSTR IN_szRFID, __in uint32_t IN_TYPE, __in LPCTSTR IN_szData);
#endif
};

#endif // SocketMonitoring_h__
