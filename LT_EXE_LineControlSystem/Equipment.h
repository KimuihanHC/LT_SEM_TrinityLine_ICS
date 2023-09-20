//*****************************************************************************
// Filename	: 	Equipment.h
// Created	:	2021/11/19 - 10:38
// Modified	:	2021/11/19 - 10:38
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#ifndef Equipment_h__
#define Equipment_h__

#pragma once

#include "Def_ConfigEquipment.h"
#include "Def_Constant.h"
#include <vector>
#include <queue>
#include "SocketMonitoring.h"
#include "FailInfo_Eqp.h"

#ifdef USE_EQP_TACTTIME
#include "TactTime.h"
#endif

#pragma pack(push, 1)

// 포트 상태 구조체
// 컨베이어 상태 구조체
typedef struct _PortStatus
{
	uint8_t nStatus = 0;
	CString szRfid;
	CString szBarcode;

	_PortStatus& operator= (const _PortStatus& ref)
	{
		nStatus		= ref.nStatus;
		szRfid		= ref.szRfid;
		szBarcode	= ref.szBarcode;

		return *this;
	};
}ST_PortStatus;

typedef struct _ConveyorStatus
{
	uint8_t nStatus			= 0;
	uint8_t nExistSocket	= 0;
	CString szRfid;
	CString szBarcode;

	_ConveyorStatus& operator= (const _ConveyorStatus& ref)
	{
		nStatus			= ref.nStatus;
		nExistSocket	= ref.nExistSocket;
		szRfid			= ref.szRfid;
		szBarcode		= ref.szBarcode;

		return *this;
	};
}ST_ConveyorStatus;

typedef struct _AlarmStatus
{
	uint32_t	m_nAlarmCode = 0;
	CString		m_szAlarmInfo;

	_AlarmStatus& operator= (const _AlarmStatus& ref)
	{
		m_nAlarmCode	= ref.m_nAlarmCode;
		m_szAlarmInfo	= ref.m_szAlarmInfo;

		return *this;
	};
}ST_AlarmStatus;

typedef struct _ReservedSocket
{
	CString		szRfid;
	SYSTEMTIME	time;

	_ReservedSocket& operator= (const _ReservedSocket& ref)
	{
		szRfid = ref.szRfid;
		memcpy(&time, &ref.time, sizeof(SYSTEMTIME));

		return *this;
	};
}ST_ReservedSocket;

#pragma pack (pop)

typedef enum 
{
	CP_Init,
	CP_Checking,
	CP_End,

	CP_MaxCount,
}enChkProduction;

typedef enum
{
	OAS_Inactive,
	OAS_Active,
	OAS_InitStatus,

	OAS_MaxCount,
}enOperationActiveStatus;

class CRegEquipment;

//-----------------------------------------------------------------------------
// 설비 모니터링 정보
//-----------------------------------------------------------------------------
class CEquipment : public CConfig_Eqp
{
public:
	CEquipment();
	virtual ~CEquipment();

	CEquipment& operator= (const CConfig_Eqp& ref);
	CEquipment& operator= (const CEquipment& ref);
	

protected:

	CRegEquipment*	m_pRegEqp		= nullptr;
	
	bool		m_bEnable			= false;	// 설비 사용 유/무 (검사항목 진행 여부)
	bool		m_bSkip				= false;	// 설비 Skip 여부
	uint8_t		m_nConnection		= 0;		// tcp/ip connection
	bool		m_bTimeSync			= false;	// time sync
	bool		m_bVerifyID			= false;	// eqp id 인증 (ip address와 eqp id 매칭 검토)

	// Show / Hide

	uint8_t		m_nOperMode			= 0;		// 운영모드
	uint8_t		m_nProcessStatus	= 0;		// 설비 가동 상태
	uint8_t		m_bLED_Status		= 0;		// 경광등, 버저
	uint8_t		m_nLanguage			= 0;		// 언어
	uint8_t		m_nModelType		= 0;		// 모델

	// * 예약된 Port 갯수
	//  - 포트 상태가 Empty -> 다른 상태로 변경 : 예약 Port 갯수 감소
	uint8_t		m_nReservedPortCnt	= 0;
	uint8_t		m_nReservedOvered	= 0; // 예약 최대치를 넘어서 예약하는 경우
	uint8_t		m_nAvablePortCnt	= MAX_RESERVE_COUNT_TESTER; // 실제 사용 가능한 Port 개수 (Disable, Alarm 포트 제외)

	// 가장 최신 알람
	//uint32_t	m_nAlarmCode		= 0;
	//CString		m_szAlarmInfo;

	std::vector<ST_PortStatus>		m_nPortStatus;
	std::vector<ST_ConveyorStatus>	m_nConveyorStatus;
	std::vector<ST_AlarmStatus>		m_nAlarmStatus;

	// 예약된 포트
	//std::queue< ST_ReservedSocket>	m_qReservePort;
	std::vector< ST_ReservedSocket>	m_vReservePort;
	// 예약된 소켓의 시간 체크 
	double							m_dReservedTimeoutSec = 3600.0f; // 1시간

	// 생산량 수율
	CYield_Equipment				m_Yield_Day;		// 하루치 수율 (Sifht 변경 시 자동 초기화)
	CYield_Equipment				m_Yield_Cumulative;	// 누적 수율 (수동 초기화)
	
	// 검사 결과 통보 받으면 Shift 체크하여 Log 생성 및 수율 초기화 
	bool							m_bAutoReset_Yield	= false;
	SYSTEMTIME						m_tm_CheckShift;	// 오전 8시 체크

	// 소켓 정보
	CSocketMonitoring*				m_pSocketInfo		= nullptr;

	// 마지막으로 Track In 요청한 소켓 정보 : RFID, 요청 시간 (2023.08.07)
	CString							m_szLastTrackIn_RFID;
	SYSTEMTIME						m_tmLastTrackIn;
	DWORD							m_nLastTrackIn_Time	= 0;

#ifdef USE_EQP_TACTTIME
	CTactTime		m_Tacttime;
#endif
protected:
	virtual void	Init_EquipmentType_UI();
	//virtual void	Init_EquipmentType	();

	HWND			m_hWndOwner			= nullptr;
	HWND			m_hWndGUI			= nullptr;
	UINT			m_nWM_Notify		= WM_EQUIPMENT_NOTIFY;
	inline void		WM_Notify_Equipment	(__in WM_Eqp_Nofity_Para IN_wParam, __in LPARAM IN_lParam);
	inline void		WM_Event_Equipment	(__in UINT IN_nWM_Event, __in LPCTSTR IN_szRFID = nullptr);
	inline void		WM_Event_Equipment	(__in UINT IN_nWM_Event, __in LPARAM IN_lParam);

	bool			Is_ChangeShift		(__in const SYSTEMTIME* IN_ptmCurrent);
	bool			Is_ChangeShift		();

	// Tester : 파라 L/R에 소켓이 투입된 시간
	virtual void	Set_Time_InputPara	(__in uint8_t IN_nPara){};
	
	//  Port 상태가 PtS_Exist_Socket으로 바뀌면 소켓 정보를 갱신한다.
	void			Update_SocketLocation	(__in uint8_t IN_nPortIndex, __in ST_PortStatus* IN_pstPort);

	void			Save_Equipment_Skip			();
	void			Save_Equipment_Reserve		();
	void			Save_Equipment_ReserveQueue	();
	void			Save_Equipment_EndProduction();
	void			Save_Equipment_Shift		();
	void			Save_Equipment_Port			(__in uint8_t IN_nPortIndex);
	void			Save_Equipment_Conveyor		(__in uint8_t IN_ConveyorIndex);
	void			Save_Equipment_Alarm		();
	void			Save_Equipment_Yield		();
	void			Save_Equipment_Tacttime		();

	// 사용 가능한 포트 체크
	virtual uint8_t	Check_AvablePortCnt			(__in uint8_t IN_OldStatus, __in uint8_t IN_NewStatus);

	// enPortIndex_Tester => enEqpPara로 변환
	uint8_t			PortIndex2TestPara			(__in uint8_t IN_nPortIndex);
	uint8_t			TestPara2PortIndex			(__in uint8_t IN_nTestPara);

	//void			Set_TrackInRequestSocket	(__in LPCTSTR IN_szRFID);

public:

	// 오너 윈도우 핸드
	void			Set_OwnerHWND		(__in HWND IN_hWnd);
	// Data를 GUI에 표시하기 위해 사용
	void			Set_GUI_HWND		(__in HWND IN_hWnd);
	// Notify Windows Message 설정
	void			Set_WinMsg_Notify	(__in UINT IN_nWM_Notify);

	// 소켓 정보 클래스 포인터 설정
	void			SetPtr_SocketInfo	(__in CSocketMonitoring* IN_pSocketInfo);

	// ---------------------------------------------------------------------------
	// 설비 종류 판단
	// ---------------------------------------------------------------------------
	virtual bool	Is_Tester			() const;	// 설비가 검사 설비인가?
	virtual bool	Is_Loader			() const;	// 설비가 로더인가?
	virtual bool	Is_Returner			() const;	// 설비가 리터너인가?
	virtual bool	Is_Handler			() const;	// 설비가 리터너인가?

public:
	// ---------------------------------------------------------------------------
	// 설비 데이터 처리
	// ---------------------------------------------------------------------------
	// 설비 순서
	uint8_t			Get_EqpOrder		();
	const uint8_t	Get_EqpOrder		() const;
	void			Set_EqpOrder		(__in uint8_t IN_nEqpOrder);

	// 포트 상태
	size_t			Get_PortCount		() const;
	virtual void	Set_PortClear		(__in uint8_t IN_nPortIndex);	
	std::vector<ST_PortStatus>&	Get_PortStatus	();
	const ST_PortStatus&		Get_PortStatus	(__in uint8_t IN_nPortIndex) const;
	const ST_PortStatus&		Get_PortStatus_byTestPara	(__in uint8_t IN_nTestPara);
	virtual void	Set_PortStatus		(__in uint8_t IN_nPortIndex, __in uint8_t IN_nStatus, __in LPCTSTR IN_szRFID, __in LPCTSTR IN_szBarcode, __in bool IN_bSave = true);

	
	// 컨베이어 상태
	size_t			Get_ConveyorCount	() const;
	std::vector<ST_ConveyorStatus>&	Get_ConveyorStatus	();
	const ST_ConveyorStatus&		Get_ConveyorStatus	(__in uint8_t IN_nConveyorIndex) const;
	virtual void	Set_ConveyorStatus	(__in uint8_t IN_nConveyorIndex, __in uint8_t IN_nStatus, __in uint8_t IN_nExistSocket, __in LPCTSTR IN_szRFID, __in LPCTSTR IN_szBarcode, __in bool IN_bSave = true);

	// 수율
	const CYield_Equipment& Get_Yield_Day		() const;
	const CYield_Equipment& Get_Yield_Cumulative() const;
	void	Set_Yield_Day				(__in CYield_Equipment* IN_pYield);
	void	Set_Yield_Cumulative		(__in CYield_Equipment* IN_pYield);
	void	Reset_Yield_Day				();
	void	Reset_Yield_Cumulative		();
	void	Increase_Yield_Pass			(__in uint8_t IN_nPara);
	void	Increase_Yield_Fail			(__in uint8_t IN_nPara);

	// 설비 활성화/비활성화
	bool	Get_EnableEquipment			() const;
	void	Set_EnableEquipment			(__in bool IN_bEnable);
	
	// 통신 상태
	uint8_t Get_ClientConnection		() const;
	void	Set_ClientConnection		(__in uint8_t IN_nConStatus);
	
	// 설비 인증
	bool	Get_VerifyEqpConnection		() const;
	void	Set_VerifyEqpConnection		(__in bool bVerified);

	// 검사 운용 모드 (Equipment Operate Mode)
	uint8_t Get_OperatingMode			() const;
	void	Set_OperatingMode			(__in uint8_t IN_nOperMode);

	// 설비 가동 상태 (Equipment Process Status)
	uint8_t Get_ProcessStatus			() const;
	void	Set_ProcessStatus			(__in uint8_t IN_nStatus);
	void	Set_ProcessStatus			(__in uint8_t IN_nStatus, __in uint32_t IN_nAlarmCode, __in LPCTSTR IN_szAlarmInfo);
	
	// 알람 갯수
	size_t	Get_AlarmCount				() const;
	const ST_AlarmStatus&			Get_AlarmStatus_Last() const;
	std::vector<ST_AlarmStatus>&	Get_AlarmStatus		();
	const ST_AlarmStatus&			Get_AlarmStatus		(__in uint32_t IN_nIndex) const;
	//void	Set_AlarmStatus_Last		(__in uint32_t	IN_nAlarmCode, __in LPCTSTR IN_szAlarmInfo);
	
	// 경광등 상태 (LED Status)
	uint8_t Get_Status_LED				() const;
	void	Set_Status_LED				(__in uint8_t IN_nLED_Status);

	// 설비 미사용 설정
	bool	Get_Skip					() const;
	void	Set_Skip					(__in bool IN_bSkip, __in bool IN_bSave = true);

	// 설비와 시간 동기화 설정 여부
	bool	Get_TimeSync				() const;
	void	Set_TimeSync				(__in bool IN_bTimeSync);

	// 포트 예약 갯수
	void	Reset_ReservedPortInfo		();
	uint8_t	Get_ReservedPortCnt			() const;
	void	Set_ReservedPortCnt			(__in uint8_t IN_nCount, __in bool IN_bSave = true);	

	// 소켓 투입 예약 (테스터만 사용)
	bool	Increase_ReservedPort		();
	bool	Increase_ReservedPort		(__in LPCTSTR IN_szRFID);
	void	Decrease_ReservedPort		();
	void	Decrease_ReservedPort		(__in LPCTSTR IN_szRFID);

	uint8_t	Get_ReservedOverCnt			() const;
	void	Set_ReservedOverCnt			(__in uint8_t IN_nCount);

	// 예약된 소켓들의 시간 체크 (오래된 소켓은 수동으로 제거 되어있다 판단하여 예약에서 제거한다.)
	// 예약된 시간, RFID 체크 
	uint8_t	Check_ReservedSocket		();

	// 예약된 소켓 정보 구하기
	std::vector<ST_ReservedSocket>&	Get_ReservedInfo();

	// Shift 변경 체크 시간
	const SYSTEMTIME&	Get_CheckShiftTime	() const;
	void Set_CheckShiftTime				(__in SYSTEMTIME* IN_ptmCheck, __in bool IN_bSave = true);

#ifdef USE_EQP_TACTTIME
	const CTactTime&	Get_Tacttime	() const;
#endif

public:

	// 알람 상태인가?
	bool	IsAlarm						();

	// 설비가 비어 있는가?
	bool	IsEmpty_Equipment			(__in bool bIgnore_EmptySocket = true);
	bool	IsEmpty_Equipment_AnySocket	();
	bool	IsLastSocket_onTestPort		();

	// 비어 있는 포트 갯수
	virtual uint8_t Get_EmptyPortCount	(__in bool bOnlyTestPort = false);

	virtual uint8_t Get_EmptyConveyorCount	();

	// 사용 중인 포트 갯수 (Buffer, Test L/R/C)
	virtual uint8_t Get_UsingPortCount	();

	// 현재 설비에서 테스트 중인 소켓 갯수 (테스트 + 배출대기)
	uint8_t	Get_TestingCount			();

	// 검사 완료 후 배출 대기 중인 소켓 갯수
	virtual uint8_t	Get_WaitOutCount	();

	// 설비내에 존재하는 소켓 갯수
	virtual uint8_t	Get_SocketCount		();
	
	// 설비내에 존재하는 제품이 실린 소켓 갯수
	virtual uint8_t Get_ProductCount	();

	// check end production
	bool	Check_EndProduction			();

	// 검사 설비 : 이 설비로 소켓 투입이 가능한가?
	virtual uint8_t	 Get_InputAvailabilityStatus	();

	// Tester : 파라 L/R에 소켓이 투입되어 경과된 시간
	virtual uint32_t Get_ElapsedTime_InputPara		(__in uint8_t IN_nPara);

	// ---------------------------------------------------------------------------
	// 이벤트 요청 처리
	// ---------------------------------------------------------------------------
	// 소켓 등록 요청 (로딩) (로더 / 언로더)
	bool	Recv_RegisterSocket			(__in LPCTSTR IN_szRFID, __in LPCTSTR IN_szBarcode);
	
	// 소켓 투입 승인 요청 -> 소켓 투입 승인 (테스터)
	bool	Recv_ReqAcceptSocket		(__in LPCTSTR IN_szRFID);

	// 검사 결과 통지 (테스터) -> 배출 승인
	bool	Recv_NotifyTestResult		(__in LPCTSTR IN_szRFID, __in int16_t IN_nNGCode, __in uint8_t IN_nPara);

	// 검사 결과 요청 (로더 / 언로더)
	bool	Recv_ReqTestResult			(__in LPCTSTR IN_szRFID, __out ST_TestResult& OUT_stResult);
	
	// 소켓 등록 해제 (언로딩) (로더 / 언로더)
	bool	Recv_UnregisterSocket		(__in LPCTSTR IN_szRFID);



	// ---------------------------------------------------------------------------
	// File Log
	// ---------------------------------------------------------------------------
protected:	
	CString m_szPath_Log;
	CString m_szPath_Report;
public:
	void	Set_Path					(__in LPCTSTR IN_szLog, __in LPCTSTR IN_szReport);
	bool	Report_Yield_Day			(); // 년/월/eqpid_yield.csv

	// ---------------------------------------------------------------------------
	// Fail Info
	// ---------------------------------------------------------------------------
protected:
	CFailInfo_Eqp*	m_pFailInfo_Eqp	= nullptr;

public:
	void	SetPtr_FailInfo_Eqp			(__in CFailInfo_Eqp* IN_pFailInfo_Eqp);
	void	IncreaseFailInfo			(__in int16_t IN_nNGCode, __in uint8_t IN_nPara);


	// 예약된 소켓이 오래되었을 경우 timeout 처리하면서 예약 정보에서 제거 할 때 비교 시간
	void	Set_ReservedTimeout_Second	(__in double IN_dSecond);


	void	Set_TrackInRequestSocket	(__in LPCTSTR IN_szRFID);
	// 마지막으로 Track In 요청 처리된 소켓 비교
	bool	IsTrackInRequest_Socket		(__in LPCTSTR IN_szRFID, __in uint32_t IN_nCheckTime = 900);

};

#endif // Equipment_h__

