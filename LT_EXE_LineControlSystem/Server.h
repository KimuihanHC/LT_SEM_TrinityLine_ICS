//*****************************************************************************
// Filename	: 	Server.h
// Created	:	2021/11/19 - 10:38
// Modified	:	2021/11/19 - 10:38
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#ifndef Server_h__
#define Server_h__

#pragma once

//#include "Def_Constant.h"
#include "Def_ConstantServer.h"
#include "SocketMonitoring.h"
#include "FailInfo_Eqp.h"
#include "Def_ConfigServer.h"

#include <vector>


#include "xmlArgs.h"
#include "IcsRemoteServerEes.h"
#include "CommonModule.h"

#ifdef USE_EQP_TACTTIME
#include "TactTime.h"
#endif

#pragma pack(push, 1)

#pragma pack (pop)

class CRegServer;

class CServer : public CConfig_Svr, public CCommonModule
{
public:
	CServer();
	virtual ~CServer();
	CServer& operator= (const CConfig_Svr& ref);
	CServer& operator= (const CServer& ref);
	CServer& operator= (const CCommonModule& ref);
protected:
//Reg 
	CRegServer*	m_pRegSvr = nullptr;
//socket State
	CSocketMonitoring*				m_pSocketInfo = nullptr;
//Svr State
	uint8_t		m_nConnection = 0;
	bool		m_bEnable = false;	// ?§ÎπÑ ?¨Ïö© ??Î¨?(Í≤Ä?¨Ìï≠Î™?ÏßÑÌñâ ?¨Î?)
	bool		m_bSkip = false;	// ?§ÎπÑ Skip ?¨Î?
	
	bool		m_bTimeSync = false;	// time sync
	bool		m_bVerifyID = false;	// eqp id ?∏Ï¶ù (ip address?Ä eqp id Îß§Ïπ≠ Í≤Ä??

	// Show / Hide
//Old State
	uint8_t		m_nOperMode = 0;		 
	uint8_t		m_nProcessStatus = 0;	 
	uint8_t		m_bLED_Status = 0;		
	uint8_t		m_nLanguage = 0;		
	uint8_t		m_nModelType = 0;		
	uint8_t		m_nReservedPortCnt = 0;
	uint8_t		m_nReservedOvered = 0; 
	uint8_t		m_nAvablePortCnt = MAX_RESERVE_COUNT_TESTER; 
	bool							m_bAutoReset_Yield = false;
	SYSTEMTIME						m_tm_CheckShift;	

#ifdef USE_EQP_TACTTIME
	CTactTime		m_Tacttime;
#endif
protected:
	HWND			m_hWndOwner = nullptr;
	HWND			m_hWndGUI = nullptr;
	UINT			m_nWM_Notify = WM_SERVER_NOTIFY;

	virtual void	Init_ServerType_UI();
	inline void		WM_Notify_Server(__in WM_Svr_Nofity_Para IN_wParam, __in LPARAM IN_lParam);
	inline void		WM_Event_Server(__in UINT IN_nWM_Event, __in LPCTSTR IN_szRFID = nullptr);
	inline void		WM_Event_Server(__in UINT IN_nWM_Event, __in LPARAM IN_lParam);

	bool			Is_ChangeShift(__in const SYSTEMTIME* IN_ptmCurrent);
	bool			Is_ChangeShift();

	virtual void	Set_Time_InputPara(__in uint8_t IN_nPara) {};
#if SOCKET
//Reg
	void			Save_Equipment_Skip();
#endif //SOCKET
	void			Save_Equipment_Reserve();
#if SOCKET
	void			Save_Equipment_EndProduction();
	void			Save_Equipment_Shift();
	void			Save_Equipment_Port(__in uint8_t IN_nPortIndex);
	void			Save_Equipment_Conveyor(__in uint8_t IN_ConveyorIndex);
	void			Save_Equipment_Alarm();
	void			Save_Equipment_Yield();
	void			Save_Equipment_Tacttime();
#endif //SOCKET
public:
//
	void			Set_OwnerHWND(__in HWND IN_hWnd);
	void			Set_GUI_HWND(__in HWND IN_hWnd);
	void			Set_WinMsg_Notify(__in UINT IN_nWM_Notify);
	void			SetPtr_SocketInfo(__in CSocketMonitoring* IN_pSocketInfo);

//Config
	uint8_t			Get_SvrOrder();
	const uint8_t	Get_SvrOrder() const;
	void			Set_SvrOrder(__in uint8_t IN_nSvrOrder);
//Connect
	uint8_t Get_ClientConnection() const;
	void	Set_ClientConnection(__in uint8_t IN_nConStatus);
//Old State
	bool	Get_EnableServer() const;
	void	Set_EnableServer(__in bool IN_bEnable);

	bool	Get_VerifySvrConnection() const;
	void	Set_VerifySvrConnection(__in bool bVerified);

	uint8_t Get_OperatingMode() const;
	void	Set_OperatingMode(__in uint8_t IN_nOperMode);

	uint8_t Get_ProcessStatus() const;
	void	Set_ProcessStatus(__in uint8_t IN_nStatus);
	void	Set_ProcessStatus(__in uint8_t IN_nStatus, __in uint32_t IN_nAlarmCode, __in LPCTSTR IN_szAlarmInfo);

	uint8_t Get_Status_LED() const;
	void	Set_Status_LED(__in uint8_t IN_nLED_Status);

	bool	Get_Skip() const;
	void	Set_Skip(__in bool IN_bSkip, __in bool IN_bSave = true);

	bool	Get_TimeSync() const;
	void	Set_TimeSync(__in bool IN_bTimeSync);

	uint8_t	Get_ReservedPortCnt() const;
	void	Set_ReservedPortCnt(__in uint8_t IN_nCount, __in bool IN_bSave = true);

	bool	Increase_ReservedPort();
	void	Decrease_ReservedPort();

	uint8_t	Get_ReservedOverCnt() const;
	void	Set_ReservedOverCnt(__in uint8_t IN_nCount);
#if SOCKET
	const SYSTEMTIME&	Get_CheckShiftTime() const;
	void Set_CheckShiftTime(__in SYSTEMTIME* IN_ptmCheck, __in bool IN_bSave = true);
#endif //if SOCKET
	CString GetDateTime();
#ifdef USE_EQP_TACTTIME
	const CTactTime&	Get_Tacttime() const;
#endif

public:
	bool	IsAlarm();

//socket
	bool	IsEmpty_Equipment(__in bool bIgnore_EmptySocket = true);
#if SOCKET
	bool	IsLastSocket_onTestPort();
#endif
	// ÎπÑÏñ¥ ?àÎäî ?¨Ìä∏ Í∞?àò
	virtual uint8_t Get_EmptyPortCount(__in bool bCount_EmptySocket = true);
#if SOCKET
	virtual uint8_t Get_EmptyConveyorCount();

	virtual uint8_t Get_UsingPortCount();

	uint8_t	Get_TestingCount();
	virtual uint8_t	Get_WaitOutCount();
	virtual uint8_t	Get_SocketCount();
	virtual uint8_t Get_ProductCount();
	bool	Check_EndProduction();
	virtual uint8_t	 Get_InputAvailabilityStatus();
#endif
	virtual uint32_t Get_ElapsedTime_InputPara(__in uint8_t IN_nPara);

#if SOCKET
	// ---------------------------------------------------------------------------
	// CSocketMonitoring
	// ---------------------------------------------------------------------------
	bool	Recv_RegisterSocket(__in LPCTSTR IN_szRFID, __in LPCTSTR IN_szBarcode);
	bool	Recv_ReqAcceptSocket(__in LPCTSTR IN_szRFID);
	bool	Recv_NotifyTestResult(__in LPCTSTR IN_szRFID, __in int16_t IN_nNGCode, __in uint8_t IN_nPara);
	bool	Recv_ReqTestResult(__in LPCTSTR IN_szRFID, __out ST_TestResult& OUT_stResult);
	bool	Recv_UnregisterSocket(__in LPCTSTR IN_szRFID);
#endif	//SOCKET
	// ---------------------------------------------------------------------------
	// File Log
	// ---------------------------------------------------------------------------
protected:
	CString m_szPath_Log;
	CString m_szPath_Report;
public:
	void	Set_Path(__in LPCTSTR IN_szLog, __in LPCTSTR IN_szReport);
#if SOCKET
	bool	Report_Yield_Day(); // ????eqpid_yield.csv
#endif	//SOCKET
	// ---------------------------------------------------------------------------
	// Fail Info
	// ---------------------------------------------------------------------------
protected:
	CFailInfo_Eqp*	m_pFailInfo_Eqp = nullptr;

public:
	void	SetPtr_FailInfo_Eqp(__in CFailInfo_Eqp* IN_pFailInfo_Eqp);
	void	IncreaseFailInfo(__in int16_t IN_nNGCode, __in uint8_t IN_nPara);
	
// ---------------------------------------------------------------------------
// Add
// ---------------------------------------------------------------------------

public:
	virtual bool					Is_EES() const;	// ?úÎ≤Ñ Ï¢ÖÎ•ò.
#if (USE_XML)
//Event
public:
	//LinkTest.
	void							Set_SERVER_CONNECTION(__in LPARAM IN_Data);
	//Online State.
	void							Set_REQUEST_LINK_TEST(__in lt::Request_Link_Test_Args::Args * IN_Data);
	void							Set_REPLY_LINK_TEST(__in lt::Reply_Link_Test_Args::Args * IN_Data);

	void							Set_REPORT_ONLINE_STATE(__in lt::Report_Online_State_Args::Args * IN_Data);

	//User Change.
	void							Set_REPORT_USER_CHANGE(	__in lt::Report_User_Change_Args::Args * IN_Data);
	void							Set_REQUEST_USER_COMMAND(__in lt::Request_User_Cpmmand_Args::Args *   IN_Data);
	void							Set_REPLY_USER_COMMAND(	__in lt::Reply_User_Change_Args::Args * IN_Data);
	//EquipMent.
	void							Set_REPORT_EQUIPMENT_STATE(			__in lt::Report_Equipment_State_Args::Args *IN_Data);
	void							Set_REQUEST_EQUIPMENT_STATE_DISPLAY(__in lt::Request_Equipment_State_Display_Args::Args *IN_Data);
	void							Set_REPLY_EQUIPMENT_STATE_DISPLAY(	__in lt::Reply_Equipment_State_Display_Args::Args * IN_Data);
	//Loss.
	void							Set_REPORT_LOSS_STATE(__in lt::Report_Loss_State_Args::Args * IN_Data);
	void							Set_REQUEST_LOSS_WINDOW(__in lt::Request_Loss_Window_Args::Args * IN_Data);
	void							Set_REPLY_LOSS_WINDOW(__in lt::Reply_Loss_Window_Args::Args *IN_Data);
	//Alarm State
	void							Set_REPORT_ALARM_STATE(__in lt::Report_Alarm_State_Args::Args *IN_Data);
	//Alarm List
	void							Set_REQUEST_ALARM_LIST(__in lt::Request_Alarm_List_Args::Args *IN_Data);
	void							Set_REPLY_ALARM_LIST(__in lt::Reply_Alarm_List_Args::Args *IN_Data);
	//RMS
	void							Set_REPORT_RMS_MODE(__in lt::Report_Rms_Mode_Args::Args * IN_Data);
	//DATETIME
	void							Set_REQUEST_SET_DATETIME(__in lt::Request_Set_DateTime_Args::Args *IN_Data);
	void							Set_REPLY_SET_DATETIME(__in lt::Reply_Set_DateTime_Args::Args *IN_Data);
	//TERMINAL MESSAGE
	void							Set_REQUEST_TERMINAL_MESSAGE(__in  lt::Request_Terminal_Message_Args::Args *IN_Data);
	void							Set_REPLY_TERMINAL_MESSAGE(__in lt::Reply_Terminal_Message_Args::Args * IN_Data);
	//OPCALL_MESSAGE
	void							Set_REQUEST_OPCALL_MESSAGE(__in lt::Request_Opcall_Args::Args * IN_Data);
	void							Set_REPLY_OPCALL_MESSAGE(__in lt::Reply_Opcall_Args::Args * IN_Data);	

public:
	void Set_DEFINEDATA(CServer & Data);
#endif
#if TEST
	void							Set_UINTID_READ(__in ST_xml_UNITID_READ * IN_Data);
	void							Set_REQUEST_UNITID_READ(__in ST_xml_REQUEST_UNITID_READ * IN_Data);
	void							Set_REPLY_UNITID_READ(__in ST_xml_REPLY_UNITID_READ * IN_Data);
	void							Set_REPORT_START_LOT(__in ST_xml_REPORT_START_LOT * IN_Data);
	void							Set_REPORT_END_EVENT(__in ST_xml_REPORT_END_EVENT * IN_Data);
	void							Set_REPORT_START_PROCESS(__in ST_xml_REPORT_START_PROCESS * IN_Data);
	void							Set_REPORT_END_PROCESS(__in ST_xml_REPORT_END_PROCESS * IN_Data);
#endif	//TEST
#if TESTUI
	void							Set_Notify_EquipmentState			(ST_xml_REPORT_EQUIPMENT_STATE * IN_DATA);
	void							Set_Notify_EquipmentStateDisplay	(ST_EquipmentStateDisplay * IN_DATA);
	void							Set_Notify_RGBDisplay				(int nPORTID, LPCTSTR IN_DATA);
	void							Set_Notify_LOSSWINDOW				(LPCTSTR IN_DATA);
	void							Set_Notify_LOSSCODE					(LPCTSTR IN_DATA);
	void							Set_Notify_TERMINAL_MESSAGE			(ST_xml_REQUEST_TERMINAL_MESSAGE * IN_DATA);
	void							Set_Notify_OPCALL					(ST_xml_REQUEST_OPCALL * IN_DATA);
	void							Set_Notify_RMS						(LPCTSTR IN_DATA);
#endif	//TESTUI

#if TEST
protected:
	uint8_t				m_nEquipmentStatus = 0;		// ?§ÎπÑ Í∞Ä???ÅÌÉú
	uint8_t				m_nOldEquipmentStatus = -1;		// ?§ÎπÑ Í∞Ä???ÅÌÉú
#if TESTTERMINAL
	std::vector<ST_TerminalLog>			m_nTerminalLog;
#endif	//TESTTERMINAL
	std::vector<ST_PortStatus>			m_nPortStatus;
	std::vector<ST_PortStatus>			m_nOldPortStatus;
	virtual uint8_t	Check_AvablePortCnt(__in uint8_t IN_OldStatus, __in uint8_t IN_NewStatus);
	void	Update_SocketLocation(__in uint8_t IN_nPortIndex, __in ST_PortStatus* IN_pstPort);
	uint8_t		PortIndex2TestPara(__in uint8_t IN_nPortIndex);

	std::vector<ST_ConveyorStatus>		m_nConveyorStatus;

	CString								m_nSubEqpID;
	

public:
//Terminal
#if TESTTERMINAL
	size_t Get_TerminalCount() const;
	const ST_TerminalLog& Get_TerminalStatus_Last() const;
	std::vector<ST_TerminalLog>& Get_TerminalStatus();
	const ST_TerminalLog & Get_TerminalStatus(uint32_t IN_nIndex) const;
#endif	//TESTTERMINAL
//EquipmentID ->Port
	size_t Get_EquipmentIDCount() const;
	std::vector<ST_EquipmentID>& Get_EquipmentIDStatus();
	const ST_EquipmentID & Get_EquipmentIDStatus(uint8_t IN_nPortIndex) const;
	virtual void	Set_PortStatusEquipmentIDEvent(__in uint8_t IN_nPortIndex, __in ST_EquipmentID * IN_nData);
//Port
	size_t			Get_PortCount() const;
	virtual void	Set_PortClear(__in uint8_t IN_nPortIndex);
	std::vector<ST_PortStatus>&	Get_PortStatus();
	const ST_PortStatus&		Get_PortStatus(__in uint8_t IN_nPortIndex) const;
	void						Set_PortStatusEquipmentStateEvent(__in uint8_t IN_nPortIndex, __in uint8_t IN_nData);
	virtual void	Set_PortStatus(__in uint8_t IN_nPortIndex, __in uint8_t IN_nStatus, __in LPCTSTR IN_szRFID, __in LPCTSTR IN_szBarcode, __in bool IN_bSave = true);

	std::vector<ST_PortStatus>&	 Get_OldPortStatus();
	const ST_PortStatus&	Get_OldPortStatus(__in uint8_t IN_nStatus) const;
	virtual void	Set_OldPortStatusEquipmentStateEvent(__in uint8_t IN_nPortIndex, __in uint8_t IN_nData);

	size_t			Get_ConveyorCount() const;
	std::vector<ST_ConveyorStatus>&	Get_ConveyorStatus();
	const ST_ConveyorStatus&		Get_ConveyorStatus(__in uint8_t IN_nConveyorIndex) const;
	virtual void	Set_ConveyorStatus(__in uint8_t IN_nConveyorIndex, __in uint8_t IN_nStatus, __in uint8_t IN_nExistSocket, __in LPCTSTR IN_szRFID, __in LPCTSTR IN_szBarcode, __in bool IN_bSave = true);
//SubeqpID
	CString Get_SubEqpID() const;
	void	Set_SubEqpID(__in LPCTSTR IN_DATA);

//Equipment State
	uint8_t Get_EquipmentStatus() const;
	void	Set_EquipmentStatus(__in uint8_t IN_nStatus);
	uint8_t Get_OldEquipmentStatus() const;//2023.04.11;
	void	Set_OldEquipmentStatus(__in uint8_t IN_nStatus);//2023.04.11;
//ALARM STATE
	size_t	Get_AlarmCount() const;
	const ST_AlarmStatus&			Get_AlarmStatus_Last() const;
	std::vector<ST_AlarmStatus>&	Get_AlarmStatus();
	const ST_AlarmStatus&			Get_AlarmStatus(__in uint32_t IN_nIndex) const;

//LossWindow
	size_t	Get_LossWindowCount() const;
	std::vector<ST_LossWindow>&	Get_LossWindow();
	const ST_LossWindow&			Get_LossWindow(__in uint32_t IN_nIndex) const;

#endif	//TEST
};
#endif //Server_h__

