//*****************************************************************************
// Filename	: 	TestManager_EQP.h
// Created	:	2016/5/9 - 13:31
// Modified	:	2016/07/22
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
#ifndef TestManager_EQP_h__
#define TestManager_EQP_h__

#pragma once

#include "TestManager_Device.h"
#include "Reg_InspInfo.h"
#include <vector>
#include "Def_DataStruct.h"

//-----------------------------------------------------------------------------
// CTestManager_EQP
//-----------------------------------------------------------------------------
class CTestManager_EQP : public CTestManager_Device
{
public:
	CTestManager_EQP();
	virtual ~CTestManager_EQP();

protected:

	//-----------------------------------------------------
	// LOG
	//-----------------------------------------------------
	CLogFile_V2		m_log_EqpAlarm;		// 설비 알람 로그
	CLogFile_V2		m_log_Socket;		// 소켓 이벤트 로그
	CLogFile_V2		m_log_NG_History;	// 검사 결과 NG 이력 로그

	//-----------------------------------------------------
	// 옵션
	//-----------------------------------------------------
	// 환경설정 데이터 불러오기
	virtual BOOL	OnLoad_Option					();	
	//virtual void	OnInitUISetting					(__in HWND hWndOwner = NULL);

	//-----------------------------------------------------	
	// 검사전 체크 
	//-----------------------------------------------------
	// 모델 설정 데이터가 정상인가 판단
	virtual LRESULT	OnVerify_RecipeInfo				();	

	//-----------------------------------------------------
	// log 처리 
	//-----------------------------------------------------
	virtual void	OnLog_EquipmentAlarm			(__in uint8_t IN_nEqpOder, __in const ST_AlarmStatus* IN_pAlarm);
	virtual void	OnLog_SocketEvent				(__in LPCTSTR IN_szRFID, __in LPCTSTR IN_szLog);
	virtual void	OnLog_SocketEvent				(__in LPCTSTR IN_szRFID, __in LPCTSTR IN_szLog, ...);
	virtual void	OnLog_SocketTestResult			(__in uint8_t IN_FromEqp, __in LPCTSTR IN_szRFID);// Unloader로 배출시 소켓 최종 결과 출력	
	virtual void	OnLog_RegisterSocket			(__in LPCTSTR IN_szRFID) = 0;
	virtual void	OnLog_NG_History				(__in LPCTSTR IN_szLog, ...);

	virtual void	OnReport_Socket					(__in LPCTSTR IN_szRFID) = 0;
	virtual void	OnReport_Socket_NG				(__in LPCTSTR IN_szRFID) = 0;
	bool			OnLogFile_SocketResult			(__in LPCTSTR IN_szRFID, __in LPCTSTR IN_szPath);

	//-----------------------------------------------------
	// 타이머 
	//-----------------------------------------------------
	//virtual void	OnCreateTimer_UpdateUI			(__in DWORD DueTime = 5000, __in DWORD Period = 250);
	//virtual void	OnDeleteTimer_UpdateUI			();
	virtual void	OnMonitor_TimeCheck				();
	virtual void	OnMonitor_UpdateUI				();
	DWORD			m_dwTimeCheck					= 0;	// 시간 체크용 임시 변수
	std::mutex		m_mTimeMutex;
	std::mutex		m_mUiMutex;

	//-----------------------------------------------------
	// UI 업데이트
	//-----------------------------------------------------
	virtual void	OnSetStatus_Production			(__in bool IN_bEnd);

	// 검사 진행 시간 체크 (설비, 파라 L/R)
	virtual void	OnUpdate_ElapsedTime_Unit		(__in UINT nEquipmentIdx, __in uint8_t IN_nPara);
	virtual void	OnUpdate_ElapsedTime_All		(){};

	//-----------------------------------------------------
	// 검사에 관련된 모든 정보가 들어있는 구조체 
	//-----------------------------------------------------
	ST_SystemInfo	m_stInspInfo;

	inline CRecipeInfo&	Get_SettingInfo()
	{
		return m_stInspInfo.SettingInfo;	
	};

	inline CLineInfo& Get_LineInfo()
	{
		return m_stInspInfo.LineInfo;
	};

	// Equipment Info
	inline size_t Get_EquipmentCount()	
	{
		return m_stInspInfo.LineInfo.GetCount();	
	}

	inline CEquipment& Get_Equipment(__in uint16_t IN_nEqpIndex)	
	{
		return m_stInspInfo.LineInfo.GetAt(IN_nEqpIndex);
	}

	inline CEquipment& Get_Loader()	
	{
		return m_stInspInfo.LineInfo.Get_Loader();
	}

	inline CEquipment& Get_Returner()
	{
		return m_stInspInfo.LineInfo.Get_Returner();
	}

	inline CString Get_EquipmentID(__in uint8_t IN_nEqpIndex)	
	{
		return m_stInspInfo.LineInfo.GetAt(IN_nEqpIndex).Get_EquipmentId();
	}

	inline uint8_t Get_EquipmentType(__in uint8_t IN_nEqpIndex)
	{
		return m_stInspInfo.LineInfo.GetAt(IN_nEqpIndex).Get_EquipmentType();
	}

	inline const ST_PortStatus& Get_EqpPort(__in uint16_t IN_nEqpIndex, __in uint8_t IN_nPortIndex) const	
	{
		return m_stInspInfo.LineInfo.GetAt(IN_nEqpIndex).Get_PortStatus(IN_nPortIndex);	
	}

	inline const ST_ConveyorStatus& Get_EqpConveyor(__in uint16_t IN_nEqpIndex, __in uint8_t IN_nConveyorIndex) const
	{
		return m_stInspInfo.LineInfo.GetAt(IN_nEqpIndex).Get_ConveyorStatus(IN_nConveyorIndex);
	}

	inline const ST_AlarmStatus& Get_EqpAlarm(__in uint16_t IN_nEqpIndex) const	
	{
		return m_stInspInfo.LineInfo.GetAt(IN_nEqpIndex).Get_AlarmStatus_Last();
	}

	//-----------------------------------------------------
	inline CString Get_EqpAlias(__in uint8_t IN_nEqpIndex) {
		return m_stInspInfo.LineInfo.GetAt(IN_nEqpIndex).Get_Alias();}
	//-----------------------------------------------------

	// Socket Info
	inline CSocketMonitoring& Get_SocketInfo()	
	{
		return m_stInspInfo.SocketInfo;	
	}

	inline CSocketInfo_Unit& Get_Socket(__in LPCTSTR IN_szRFID)	
	{
		return m_stInspInfo.SocketInfo.GetAt(IN_szRFID);
	}

	inline CString Get_SocketBarcode(__in LPCTSTR IN_szRFID)	
	{
		return m_stInspInfo.SocketInfo.GetAt(IN_szRFID).m_szBarcode;
	}

	inline uint8_t Get_SocketType(__in LPCTSTR IN_szRFID)	
	{
		return m_stInspInfo.SocketInfo.GetAt(IN_szRFID).nSocketType;
	}

	inline const ST_TestResult& Get_SocketTestResult(__in LPCTSTR IN_szRFID) const	
	{
		return m_stInspInfo.SocketInfo.GetAt(IN_szRFID).m_stTestResult;
	}

	inline uint16_t Get_SocketTarget(__in LPCTSTR IN_szRFID){
		return m_stInspInfo.SocketInfo.GetAt(IN_szRFID).m_nTargetEqpOrder;
	}

	inline  CString Get_SocketTargetID(__in LPCTSTR IN_szRFID)
	{
		return m_stInspInfo.SocketInfo.GetAt(IN_szRFID).m_szTargetEqpID;
	}

	inline ST_DebugInfo& Get_DebugInfo()
	{
		return m_stInspInfo.DebugMode;
	}

	inline CFailInfo& Get_FailInfo()
	{
		return m_stInspInfo.FailInfo;
	}

 	inline int16_t Get_MES_ReworkCode()
	{
 		return m_stInspInfo.SocketInfo.Get_MES_ReworkCode();
	}

	// 레지스트리 데이터 처리용 클래스
	CReg_InspInfo		m_regInspInfo;

	// 검사 시작 조건이 될때까지 검사 시작 방지
	BOOL				m_bFlag_ReadyTest			= FALSE;

	// Auto 상태에서 설비와의 통신 끊어짐 체크 기능 사용 여부 (2023.03.20)
	bool				m_bFlag_CheckEqpConnection = false;

public:
	
	// 생성자 처리용 코드
	virtual void		OnInitialize				();
	// 소멸자 처리용 코드
	virtual	void		OnFinalize					();

	// 관리자 모드 확인 (구버전)
	BOOL				IsManagerMode				();
	// 제어 권한 상태 구하기
	enPermissionMode	GetPermissionMode			();
	// 제어 권한 상태 설정
	virtual void		OnSet_PermissionMode		(__in enPermissionMode nAcessMode);

//-----------------------------------------------------
// Add 
//-----------------------------------------------------
#if TESTTERMINAL
protected:
	bool			OnLogFile_TerminalMessageResult(__in lt::Request_Terminal_Message_Args::Args& IN_szData, __in LPCTSTR IN_szPath);
#endif	//TESTTERMINAL
#if TESTOPCALL
protected:
	bool			OnLogFile_OpCallResult(__in lt::Request_Opcall_Args::Args&  IN_Data, __in LPCTSTR IN_szPath);
#endif	//TESTOPCALL

#if (USE_XML)
protected:
	inline CString Get_SocketLOTID(__in LPCTSTR IN_szRFID) {
		return m_stInspInfo.SocketInfo.GetAt(IN_szRFID).m_LotID;
	}
protected:
	inline CServerInfo& Get_ServerInfo() {
		return m_stInspInfo.ServerInfo;	}
	//-----------------------------------------------------
	inline size_t Get_ServerCount() {
		return m_stInspInfo.ServerInfo.GetCount();	}
	inline CServer& Get_Server(__in uint16_t IN_nEqpIndex) {
		return m_stInspInfo.ServerInfo.GetAt(IN_nEqpIndex);	}
	inline CString Get_ServerID(__in uint8_t IN_nEqpIndex) {
		return m_stInspInfo.ServerInfo.GetAt(IN_nEqpIndex).Get_ServerId();	}
	inline uint8_t Get_ServerType(__in uint8_t IN_nEqpIndex) {
		return m_stInspInfo.ServerInfo.GetAt(IN_nEqpIndex).Get_ServerType();	}
	inline CString Get_ServerAlias(__in uint8_t IN_nEqpIndex) {
		return m_stInspInfo.ServerInfo.GetAt(IN_nEqpIndex).Get_Alias();	}
	inline CServer& Get_ServerEES() {
		return m_stInspInfo.ServerInfo.Get_EES();	}

protected:
	inline CConfig_EES_LIST&	Get_EES_Info() {
		return m_stInspInfo.SettingInfo.EES_Info;	};
	inline CConfig_EES&			Get_EES(__in uint32_t IN_szData) {
		return m_stInspInfo.SettingInfo.EES_Info.GetAt(IN_szData);	}
	inline size_t				Get_EESCount() {
		return m_stInspInfo.SettingInfo.EES_Info.Get_EES_Count();	}
	inline CString				Get_EESEqpCode(__in uint32_t IN_szData) {
		return m_stInspInfo.SettingInfo.EES_Info.GetAt(IN_szData).Get_EquipMentStateCode();	}
	inline CString				Get_EESEqpNameKor(__in uint32_t IN_szData) {
		return m_stInspInfo.SettingInfo.EES_Info.GetAt(IN_szData).Get_EquipMentStateKorText();	}
	inline CString				Get_EESEqpNameEng(__in uint32_t IN_szData) {
		return m_stInspInfo.SettingInfo.EES_Info.GetAt(IN_szData).Get_EquipMentStateEngText();	}
	inline CString				Get_EESLossCode(__in uint32_t IN_szData) {
		return m_stInspInfo.SettingInfo.EES_Info.GetAt(IN_szData).Get_LossCode();	}
	inline CString				Get_EESColor(__in uint32_t IN_szData) {
		return m_stInspInfo.SettingInfo.EES_Info.GetAt(IN_szData).Get_StateColor();	}
	inline bool					Get_EESUse(__in uint32_t IN_szData) {
		return m_stInspInfo.SettingInfo.EES_Info.GetAt(IN_szData).Get_Use();	}
	inline CString				Get_EESExplanation(__in uint32_t IN_szData) {
		return m_stInspInfo.SettingInfo.EES_Info.GetAt(IN_szData).Get_Explanation();	}

	inline CConfig_Loss_LIST&	Get_Loss_Info() {
		return m_stInspInfo.SettingInfo.Loss_Info;	};
	inline CConfig_Loss&			Get_Loss(__in uint32_t IN_szData) {
		return m_stInspInfo.SettingInfo.Loss_Info.GetAt(IN_szData);	}
	inline size_t				Get_LossCount() {
		return m_stInspInfo.SettingInfo.Loss_Info.Get_Loss_Count();	}
	inline CString				Get_Loss_NameTem(__in uint32_t IN_szData) {
		return m_stInspInfo.SettingInfo.Loss_Info.GetAt(IN_szData).Get_NameLoss();	}
	inline CString				Get_Loss_Tem(__in uint32_t IN_szData) {
		return m_stInspInfo.SettingInfo.Loss_Info.GetAt(IN_szData).Get_Tem();	}
	inline CString				Get_Loss_Code(__in uint32_t IN_szData) {
		return m_stInspInfo.SettingInfo.Loss_Info.GetAt(IN_szData).Get_CodeLoss();	}
	inline CString				Get_Loss_NameEng(__in uint32_t IN_szData) {
		return m_stInspInfo.SettingInfo.Loss_Info.GetAt(IN_szData).Get_NameLossEng();	}
	inline CString				Get_Loss_Explan(__in uint32_t IN_szData) {
		return m_stInspInfo.SettingInfo.Loss_Info.GetAt(IN_szData).Get_Explain();	}

	inline CConfig_AL_LIST&	Get_AL_Info() {
		return m_stInspInfo.SettingInfo.AL_Info;	}
	inline CConfig_AL& Get_AL(__in uint32_t IN_szRFID) {
		return m_stInspInfo.SettingInfo.AL_Info.GetAt(IN_szRFID);	}

	inline const ST_AlarmStatus& Get_EqpAlarmState(__in uint16_t IN_nEqpIndex, __in uint32_t IN_nState) const {
		return m_stInspInfo.LineInfo.GetAt(IN_nEqpIndex).Get_AlarmStatus(IN_nState);	}
	inline const size_t Get_EqpAlarmCount(__in uint16_t IN_nEqpIndex) const {
		return m_stInspInfo.LineInfo.GetAt(IN_nEqpIndex).Get_AlarmCount();	}

protected:
	inline CAlarmStatus Get_ptAlarmStatus(__in uint16_t IN_nEqpIndex, __in uint32_t IN_nState) {
		return m_stInspInfo.LineInfo.GetAt(IN_nEqpIndex).Get_mAlarmStatus(IN_nState);	}
	inline const size_t Get_AlarmStatusCount(__in uint16_t IN_nEqpIndex) {
		return m_stInspInfo.LineInfo.GetAt(IN_nEqpIndex).Get_mAlarmStatusCount();	}
public:
	void				OnSet_EESMode				(__in enEES_Mode nAcessMode);
#endif

};

#endif // TestManager_EQP_h__

