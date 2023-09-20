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
#include "SocketMonitoring.h"
#include "FailInfo_Eqp.h"


#ifdef USE_EQP_TACTTIME
#include "TactTime.h"
#endif

#if (USE_XML)
#include "CommonModule.h"
#endif
#pragma pack(push, 1)

// ?¨Ìä∏ ?ÅÌÉú Íµ¨Ï°∞Ï≤?
// Ïª®Î≤†?¥Ïñ¥ ?ÅÌÉú Íµ¨Ï°∞Ï≤?/
typedef struct _PortStatus
{
	uint8_t nStatus = 0;
	CString szRfid;
	CString szBarcode;

#if ADD_SOCKET_EES_XML
	int8_t nEquipmentState = -1;
#endif
	_PortStatus& operator= (const _PortStatus& ref)
	{
		nStatus		= ref.nStatus;
		szRfid		= ref.szRfid;
		szBarcode	= ref.szBarcode;

#if ADD_SOCKET_EES_XML
		nEquipmentState = ref.nEquipmentState;
#endif
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
		m_nAlarmCode = ref.m_nAlarmCode;
		m_szAlarmInfo = ref.m_szAlarmInfo;
		return *this;
	};
}ST_AlarmStatus;

#if ADD_SOCKET_EES_XML

//-----------------------------------------------------------------------------
// Equipment State Display 
//-----------------------------------------------------------------------------
#if TEST
typedef struct _EquipmentStateDisplay
{
	CConfig_EES * EES;
	ST_xml_REQUEST_EQUIPMENT_STATE_DISPLAY * Dsp;

	_EquipmentStateDisplay& operator= (const _EquipmentStateDisplay& ref)
	{
		EES = ref.EES;
		Dsp = ref.Dsp;

		return *this;
	};
}ST_EquipmentStateDisplay;
#endif
//-----------------------------------------------------------------------------
// Equipment ID 
//-----------------------------------------------------------------------------

#endif	//ADD_SOCKET_EES_XML
#pragma pack (pop)

typedef enum 
{
	CP_Init,
	CP_Checking,
	CP_End,

	CP_MaxCount,
}enChkProduction;

//-----------------------------------------------------------------------------
// ?§ÎπÑ Î™®Îãà?∞ÎßÅ ?ïÎ≥¥
//-----------------------------------------------------------------------------
#if (USE_XML)
class CEquipment : public CConfig_Eqp , public CCommonModule
#else
class CEquipment : public CConfig_Eqp
#endif
{
public:
	CEquipment();
	virtual ~CEquipment();

	CEquipment& operator= (const CConfig_Eqp& ref);
	CEquipment& operator= (const CEquipment& ref);
#if (USE_XML)
	CEquipment& operator= (const CCommonModule& ref);
#endif	
protected:

	CRegEquipment*	m_pRegEqp		= nullptr;
	
	bool		m_bEnable			= false;	// ?§ÎπÑ ?¨Ïö© ??Î¨?(Í≤Ä?¨Ìï≠Î™?ÏßÑÌñâ ?¨Î?)
	bool		m_bSkip				= false;	// ?§ÎπÑ Skip ?¨Î?
	uint8_t		m_nConnection		= 0;		// tcp/ip connection
	bool		m_bTimeSync			= false;	// time sync
	bool		m_bVerifyID			= false;	// eqp id ?∏Ï¶ù (ip address?Ä eqp id Îß§Ïπ≠ Í≤Ä??

	// Show / Hide

	uint8_t		m_nOperMode			= 0;		// ?¥ÏòÅÎ™®Îìú
	uint8_t		m_nProcessStatus	= 0;		// ?§ÎπÑ Í∞Ä???ÅÌÉú
	uint8_t		m_bLED_Status		= 0;		// Í≤ΩÍ¥ë?? Î≤ÑÏ?
	uint8_t		m_nLanguage			= 0;		// ?∏Ïñ¥
	uint8_t		m_nModelType		= 0;		// Î™®Îç∏

	// * ?àÏïΩ??Port Í∞?àò
	//  - ?¨Ìä∏ ?ÅÌÉúÍ∞Ä Empty -> ?§Î•∏ ?ÅÌÉúÎ°?Î≥ÄÍ≤?: ?àÏïΩ Port Í∞?àò Í∞êÏÜå
	uint8_t		m_nReservedPortCnt	= 0;
	uint8_t		m_nReservedOvered	= 0; // ?àÏïΩ ÏµúÎ?ÏπòÎ? ?òÏñ¥???àÏïΩ?òÎäî Í≤ΩÏö∞
	uint8_t		m_nAvablePortCnt	= MAX_RESERVE_COUNT_TESTER; // ?§Ï†ú ?¨Ïö© Í∞Ä?•Ìïú Port Í∞úÏàò (Disable, Alarm ?¨Ìä∏ ?úÏô∏)

	// Í∞Ä??ÏµúÏã† ?åÎûå
	//uint32_t	m_nAlarmCode		= 0;
	//CString		m_szAlarmInfo;

	std::vector<ST_PortStatus>		m_nPortStatus;
	std::vector<ST_ConveyorStatus>	m_nConveyorStatus;
	std::vector<ST_AlarmStatus>		m_nAlarmStatus;

	// ?ùÏÇ∞???òÏú®
	CYield_Equipment				m_Yield_Day;		// ?òÎ£®Ïπ??òÏú® (Sifht Î≥ÄÍ≤????êÎèô Ï¥àÍ∏∞??
	CYield_Equipment				m_Yield_Cumulative;	// ?ÑÏ†Å ?òÏú® (?òÎèô Ï¥àÍ∏∞??
	
	// Í≤Ä??Í≤∞Í≥º ?µÎ≥¥ Î∞õÏúºÎ©?Shift Ï≤¥ÌÅ¨?òÏó¨ Log ?ùÏÑ± Î∞??òÏú® Ï¥àÍ∏∞??
	bool							m_bAutoReset_Yield	= false;
	SYSTEMTIME						m_tm_CheckShift;	// ?§Ï†Ñ 8??Ï≤¥ÌÅ¨

	// ?åÏºì ?ïÎ≥¥
	CSocketMonitoring*				m_pSocketInfo		= nullptr;

#if TEST
	ST_EquipmentStateDisplay*		m_pEES_EqpDsp;
#endif
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

	// Tester : ?åÎùº L/R???åÏºì???¨ÏûÖ???úÍ∞Ñ
	virtual void	Set_Time_InputPara	(__in uint8_t IN_nPara){};
	
	//  Port ?ÅÌÉúÍ∞Ä PtS_Exist_Socket?ºÎ°ú Î∞îÎÄåÎ©¥ ?åÏºì ?ïÎ≥¥Î•?Í∞±Ïã†?úÎã§.
	void			Update_SocketLocation	(__in uint8_t IN_nPortIndex, __in ST_PortStatus* IN_pstPort);

	void			Save_Equipment_Skip			();
	void			Save_Equipment_Reserve		();
	void			Save_Equipment_EndProduction();
	void			Save_Equipment_Shift		();
	void			Save_Equipment_Port			(__in uint8_t IN_nPortIndex);
	void			Save_Equipment_Conveyor		(__in uint8_t IN_ConveyorIndex);
	void			Save_Equipment_Alarm		();
	void			Save_Equipment_Yield		();
	void			Save_Equipment_Tacttime		();

	// ?¨Ïö© Í∞Ä?•Ìïú ?¨Ìä∏ Ï≤¥ÌÅ¨
	virtual uint8_t	Check_AvablePortCnt			(__in uint8_t IN_OldStatus, __in uint8_t IN_NewStatus);

	// enPortIndex_Tester => enEqpParaÎ°?Î≥Ä??/
	uint8_t			PortIndex2TestPara			(__in uint8_t IN_nPortIndex);

public:

	// ?§ÎÑà ?àÎèÑ???∏Îìú
	void			Set_OwnerHWND		(__in HWND IN_hWnd);
	// DataÎ•?GUI???úÏãú?òÍ∏∞ ?ÑÌï¥ ?¨Ïö©
	void			Set_GUI_HWND		(__in HWND IN_hWnd);
	// Notify Windows Message ?§Ï†ï
	void			Set_WinMsg_Notify	(__in UINT IN_nWM_Notify);

	// ?åÏºì ?ïÎ≥¥ ?¥Îûò???¨Ïù∏???§Ï†ï
	void			SetPtr_SocketInfo	(__in CSocketMonitoring* IN_pSocketInfo);

	// ---------------------------------------------------------------------------
	// ?§ÎπÑ Ï¢ÖÎ•ò ?êÎã®
	// ---------------------------------------------------------------------------
	virtual bool	Is_Tester			() const;	// ?§ÎπÑÍ∞Ä Í≤Ä???§ÎπÑ?∏Í??
	virtual bool	Is_Loader			() const;	// ?§ÎπÑÍ∞Ä Î°úÎçî?∏Í??
	virtual bool	Is_Returner			() const;	// ?§ÎπÑÍ∞Ä Î¶¨ÌÑ∞?àÏù∏Í∞Ä?
	virtual bool	Is_Handler			() const;	// ?§ÎπÑÍ∞Ä Î¶¨ÌÑ∞?àÏù∏Í∞Ä?

public:
	// ---------------------------------------------------------------------------
	// ?§ÎπÑ ?∞Ïù¥??Ï≤òÎ¶¨
	// ---------------------------------------------------------------------------
	// ?§ÎπÑ ?úÏÑú
	uint8_t			Get_EqpOrder		();
	const uint8_t	Get_EqpOrder		() const;
	void			Set_EqpOrder		(__in uint8_t IN_nEqpOrder);

	// ?¨Ìä∏ ?ÅÌÉú
	size_t			Get_PortCount		() const;
	virtual void	Set_PortClear		(__in uint8_t IN_nPortIndex);	
	std::vector<ST_PortStatus>&	Get_PortStatus	();
	const ST_PortStatus&		Get_PortStatus	(__in uint8_t IN_nPortIndex) const;	
	virtual void	Set_PortStatus		(__in uint8_t IN_nPortIndex, __in uint8_t IN_nStatus, __in LPCTSTR IN_szRFID, __in LPCTSTR IN_szBarcode, __in bool IN_bSave = true);

	// Ïª®Î≤†?¥Ïñ¥ ?ÅÌÉú
	size_t			Get_ConveyorCount	() const;
	std::vector<ST_ConveyorStatus>&	Get_ConveyorStatus	();
	const ST_ConveyorStatus&		Get_ConveyorStatus	(__in uint8_t IN_nConveyorIndex) const;
	virtual void	Set_ConveyorStatus	(__in uint8_t IN_nConveyorIndex, __in uint8_t IN_nStatus, __in uint8_t IN_nExistSocket, __in LPCTSTR IN_szRFID, __in LPCTSTR IN_szBarcode, __in bool IN_bSave = true);

	// ?òÏú®
	const CYield_Equipment& Get_Yield_Day		() const;
	const CYield_Equipment& Get_Yield_Cumulative() const;
	void	Set_Yield_Day				(__in CYield_Equipment* IN_pYield);
	void	Set_Yield_Cumulative		(__in CYield_Equipment* IN_pYield);
	void	Reset_Yield_Day				();
	void	Reset_Yield_Cumulative		();
	void	Increase_Yield_Pass			(__in uint8_t IN_nPara);
	void	Increase_Yield_Fail			(__in uint8_t IN_nPara);

	// ?§ÎπÑ ?úÏÑ±??ÎπÑÌôú?±Ìôî
	bool	Get_EnableEquipment			() const;
	void	Set_EnableEquipment			(__in bool IN_bEnable);
	
	// ?µÏã† ?ÅÌÉú
	uint8_t Get_ClientConnection		() const;
	void	Set_ClientConnection		(__in uint8_t IN_nConStatus);
	
	// ?§ÎπÑ ?∏Ï¶ù
	bool	Get_VerifyEqpConnection		() const;
	void	Set_VerifyEqpConnection		(__in bool bVerified);

	// Í≤Ä???¥Ïö© Î™®Îìú (Equipment Operate Mode)
	uint8_t Get_OperatingMode			() const;
	void	Set_OperatingMode			(__in uint8_t IN_nOperMode);

	// ?§ÎπÑ Í∞Ä???ÅÌÉú (Equipment Process Status)
	uint8_t Get_ProcessStatus			() const;
	void	Set_ProcessStatus			(__in uint8_t IN_nStatus);
	void	Set_ProcessStatus			(__in uint8_t IN_nStatus, __in uint32_t IN_nAlarmCode, __in LPCTSTR IN_szAlarmInfo);

	// ?åÎûå Í∞?àò
	size_t	Get_AlarmCount				() const;
	const ST_AlarmStatus&			Get_AlarmStatus_Last() const;
	std::vector<ST_AlarmStatus>&	Get_AlarmStatus		();
	const ST_AlarmStatus&			Get_AlarmStatus		(__in uint32_t IN_nIndex) const;
	//void	Set_AlarmStatus_Last		(__in uint32_t	IN_nAlarmCode, __in LPCTSTR IN_szAlarmInfo);

	// Í≤ΩÍ¥ë???ÅÌÉú (LED Status)
	uint8_t Get_Status_LED				() const;
	void	Set_Status_LED				(__in uint8_t IN_nLED_Status);

	// ?§ÎπÑ ÎØ∏ÏÇ¨???§Ï†ï
	bool	Get_Skip					() const;
	void	Set_Skip					(__in bool IN_bSkip, __in bool IN_bSave = true);

	// ?§ÎπÑ?Ä ?úÍ∞Ñ ?ôÍ∏∞???§Ï†ï ?¨Î?
	bool	Get_TimeSync				() const;
	void	Set_TimeSync				(__in bool IN_bTimeSync);

	// ?¨Ìä∏ ?àÏïΩ Í∞?àò
	uint8_t	Get_ReservedPortCnt			() const;
	void	Set_ReservedPortCnt			(__in uint8_t IN_nCount, __in bool IN_bSave = true);
	// ?åÏºì ?¨ÏûÖ ?àÏïΩ (?åÏä§?∞Îßå ?¨Ïö©)
	bool	Increase_ReservedPort		();
	void	Decrease_ReservedPort		();

	uint8_t	Get_ReservedOverCnt			() const;
	void	Set_ReservedOverCnt			(__in uint8_t IN_nCount);


	// Shift Î≥ÄÍ≤?Ï≤¥ÌÅ¨ ?úÍ∞Ñ
	const SYSTEMTIME&	Get_CheckShiftTime	() const;
	void Set_CheckShiftTime				(__in SYSTEMTIME* IN_ptmCheck, __in bool IN_bSave = true);

#ifdef USE_EQP_TACTTIME
	const CTactTime&	Get_Tacttime	() const;
#endif

public:

	// ?åÎûå ?ÅÌÉú?∏Í??
	bool	IsAlarm						();

	// ?§ÎπÑÍ∞Ä ÎπÑÏñ¥ ?àÎäîÍ∞Ä?
	bool	IsEmpty_Equipment			(__in bool bIgnore_EmptySocket = true);
	bool	IsLastSocket_onTestPort		();

	// ÎπÑÏñ¥ ?àÎäî ?¨Ìä∏ Í∞?àò
	virtual uint8_t Get_EmptyPortCount	(__in bool bCount_EmptySocket = true);

	virtual uint8_t Get_EmptyConveyorCount	();

	// ?¨Ïö© Ï§ëÏù∏ ?¨Ìä∏ Í∞?àò (Buffer, Test L/R/C)
	virtual uint8_t Get_UsingPortCount	();

	// ?ÑÏû¨ ?§ÎπÑ?êÏÑú ?åÏä§??Ï§ëÏù∏ ?åÏºì Í∞?àò (?åÏä§??+ Î∞∞Ï∂ú?ÄÍ∏?
	uint8_t	Get_TestingCount			();

	// Í≤Ä???ÑÎ£å ??Î∞∞Ï∂ú ?ÄÍ∏?Ï§ëÏù∏ ?åÏºì Í∞?àò
	virtual uint8_t	Get_WaitOutCount	();

	// ?§ÎπÑ?¥Ïóê Ï°¥Ïû¨?òÎäî ?åÏºì Í∞?àò
	virtual uint8_t	Get_SocketCount		();
	
	// ?§ÎπÑ?¥Ïóê Ï°¥Ïû¨?òÎäî ?úÌíà???§Î¶∞ ?åÏºì Í∞?àò
	virtual uint8_t Get_ProductCount	();

	// check end production
	bool	Check_EndProduction			();

	// Í≤Ä???§ÎπÑ : ???§ÎπÑÎ°??åÏºì ?¨ÏûÖ??Í∞Ä?•ÌïúÍ∞Ä?
	virtual uint8_t	 Get_InputAvailabilityStatus	();

	// Tester : ?åÎùº L/R???åÏºì???¨ÏûÖ?òÏñ¥ Í≤ΩÍ≥º???úÍ∞Ñ
	virtual uint32_t Get_ElapsedTime_InputPara		(__in uint8_t IN_nPara);

	// ---------------------------------------------------------------------------
	// ?¥Î≤§???îÏ≤≠ Ï≤òÎ¶¨
	// ---------------------------------------------------------------------------
	// ?åÏºì ?±Î°ù ?îÏ≤≠ (Î°úÎî©) (Î°úÎçî / ?∏Î°ú??
	bool	Recv_RegisterSocket			(__in LPCTSTR IN_szRFID, __in LPCTSTR IN_szBarcode);
	
	// ?åÏºì ?¨ÏûÖ ?πÏù∏ ?îÏ≤≠ -> ?åÏºì ?¨ÏûÖ ?πÏù∏ (?åÏä§??
	bool	Recv_ReqAcceptSocket		(__in LPCTSTR IN_szRFID);

	// Í≤Ä??Í≤∞Í≥º ?µÏ? (?åÏä§?? -> Î∞∞Ï∂ú ?πÏù∏
	bool	Recv_NotifyTestResult		(__in LPCTSTR IN_szRFID, __in int16_t IN_nNGCode, __in uint8_t IN_nPara);

	// Í≤Ä??Í≤∞Í≥º ?îÏ≤≠ (Î°úÎçî / ?∏Î°ú??
	bool	Recv_ReqTestResult			(__in LPCTSTR IN_szRFID, __out ST_TestResult& OUT_stResult);
	
	// ?åÏºì ?±Î°ù ?¥Ï†ú (?∏Î°ú?? (Î°úÎçî / ?∏Î°ú??
	bool	Recv_UnregisterSocket		(__in LPCTSTR IN_szRFID);

	// ---------------------------------------------------------------------------
	// File Log
	// ---------------------------------------------------------------------------
protected:	
	CString m_szPath_Log;
	CString m_szPath_Report;
public:
	void	Set_Path					(__in LPCTSTR IN_szLog, __in LPCTSTR IN_szReport);
	bool	Report_Yield_Day			(); // ????eqpid_yield.csv

	// ---------------------------------------------------------------------------
	// Fail Info
	// ---------------------------------------------------------------------------
protected:
	CFailInfo_Eqp*	m_pFailInfo_Eqp	= nullptr;

public:
	void	SetPtr_FailInfo_Eqp			(__in CFailInfo_Eqp* IN_pFailInfo_Eqp);
	void	IncreaseFailInfo			(__in int16_t IN_nNGCode, __in uint8_t IN_nPara);

#if (USE_XML)

public:
	void	Set_Notify_EquipmentState(__in lt::Report_Equipment_State_Args::Args * IN_DATA);	
	/*20230906
	void	Set_Notify_EquipmentStateDisplay(__in CCommonModule* IN_DATA);
	void	Set_Notify_RGBDisplay(__in lt::Request_Equipment_State_Display_Args::Args & N_DATA);
	void	Set_Notify_LOTID(LPCTSTR IN_DATA);
*/
public:
	//bool	Recv_RegisterSocketLOT(__in LPCTSTR IN_szRFID, __in uint32_t IN_TYPE, __in LPCTSTR IN_szData);
protected:
	uint8_t		m_nEquipmentStatus = 0;
//public:
	//uint8_t Get_EquipmentStatus() const;
	//void	Set_EquipmentStatus(__in uint8_t IN_nStatus);
public:
	void Set_DEFINEDATA(CEquipment & Data);
#endif
#if ADD_SOCKET_EES_XML
// ---------------------------------------------------------------------------
// add
// ---------------------------------------------------------------------------
		protected:
	uint8_t		m_nOldEquipmentStatus	= -1;
	uint8_t		m_nOldOperMode = -1;
	uint8_t		m_nOldProcessStatus = -1;
	uint8_t		m_nOldConnection = -1;
	

#if TESTTERMINAL
	std::vector<ST_TerminalLog>		m_nTerminalLog;
#endif	//TESTTERMINAL
	CString							m_nSubEqpID;
public:

	//2023.04.27a uhkim
	virtual void	Set_OldPortStatus(__in uint8_t IN_nPortIndex, __in uint8_t IN_nStatus, __in LPCTSTR IN_szRFID, __in LPCTSTR IN_szBarcode);
	virtual void	Set_PortStatusEquipmentStateEvent(__in uint8_t IN_nPortIndex, __in uint8_t IN_nData);

	uint8_t Get_OldProcessStatus() const;
	void	Set_OldProcessStatus(__in uint8_t IN_nStatus);

	uint8_t Get_OldOperatingMode() const;
	void	Set_OldOperatingMode(__in uint8_t IN_nStatus);

	// ?§ÎπÑ ?ÅÌÉú (Equipment Status)

	uint8_t Get_OldEquipmentStatus() const;//2023.04.11;
	void	Set_OldEquipmentStatus(__in uint8_t IN_nStatus);//2023.04.11;

	//2023.04.11
	std::vector<ST_PortStatus>&	 Get_OldPortStatus() ;
	const ST_PortStatus&	Get_OldPortStatus(__in uint8_t IN_nStatus) const;
	virtual void	Set_OldPortStatusEquipmentStateEvent(__in uint8_t IN_nPortIndex, __in uint8_t IN_nData);

	//2023.04.28a uhkim
	uint8_t Get_OldClientConnection() const;//2023.04.11;
	void	Set_OldClientConnection(__in uint8_t IN_nStatus);//2023.04.11;

	void	Set_Notify_EquipmentState(__in lt::Report_Equipment_State_Args::Args & IN_DATA);
#if TEST
	void	Set_Notify_EquipmentStateDisplay(ST_EquipmentStateDisplay * IN_DATA);
#endif	//TEST
	void	Set_Notify_RGBDisplay(__in lt::Request_Equipment_State_Display_Args::Args & N_DATA);
	void	Set_Notify_LOTID(LPCTSTR IN_DATA);
public:

	CString GetDateTime();
#if TEST
	//UINTID_READ
	void							Set_UINTID_READ(__in ST_xml_UNITID_READ * IN_Data);
	//REPORT_START_LOT
	void							Set_REPORT_START_LOT(__in ST_xml_REPORT_START_LOT * IN_Data);
	//REPORT_END_EVENT
	void							Set_REPORT_END_EVENT(__in ST_xml_REPORT_END_EVENT * IN_Data);
	//REPORT_START_PROCESS
	void							Set_REPORT_START_PROCESS(__in ST_xml_REPORT_START_PROCESS * IN_Data);
	//REPORT_END_PROCESS
	void							Set_REPORT_END_PROCESS(__in ST_xml_REPORT_END_PROCESS * IN_Data);
#endif	//TEST
public:
#if TESTTERMINAL
	size_t Get_TerminalCount() const;
	const ST_TerminalLog& Get_TerminalStatus_Last() const;
	std::vector<ST_TerminalLog>& Get_TerminalStatus();
	const ST_TerminalLog & Get_TerminalStatus(uint32_t IN_nIndex) const;
#endif	//TESTTERMINAL
	//2023.05.24a uhkim
	CString Get_FromPortToLOTID(__in uint8_t IN_nPortIndex);

	//2023.05.25a uhkim
	size_t Get_EquipmentIDCount() const;
	std::vector<ST_EquipmentID>& Get_EquipmentIDStatus();
	const ST_EquipmentID & Get_EquipmentIDStatus(uint8_t IN_nPortIndex) const;

	CString Get_SubEqpID() const;
	void	Set_SubEqpID(__in LPCTSTR IN_DATA);

	virtual void	Set_PortStatusEquipmentIDEvent(__in uint8_t IN_nPortIndex, __in ST_EquipmentID * IN_nData);
#endif // ADD_SOCKET_EES_XML

};	
#endif // Equipment_h__

