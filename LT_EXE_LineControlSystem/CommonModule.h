//*****************************************************************************
// Filename	: CommonLineInfoData.h
// Created	: 2012/11/1
// Modified	: 2016/12/29
//
// Author	: PiRing
//	
// Purpose	: 
//*****************************************************************************
#ifndef CommonModule_h__
#define CommonModule_h__

//#include <afxwin.h>
#include <vector>
#include "Def_Config_EES.h"
#include "Event/IcsCommEventEes.h"
#include "Def_ConstantServer.h"

//#include "Equipment.h"
//#include "Server.h"
#include "Xml/xmlArgs.h"


#pragma once
using CDEFINEDATA = lt::CDEFINEDATA;
class ST_EquipmentID
{
protected:
	//Add EES Port Data
	CString		EQUIPMENTID;
	CString		IPADDRESS;
	CString		USERID;
	CString		SUBEQPID;
	CString		EESMODE;
	CString		PORTID;
public:
	ST_EquipmentID& operator= (const ST_EquipmentID& ref)
	{
		//Add EES Port Data
		EQUIPMENTID = ref.EQUIPMENTID;
		IPADDRESS = ref.IPADDRESS;		
		USERID = ref.USERID;
		SUBEQPID = ref.SUBEQPID;
		EESMODE = ref.EESMODE;
		PORTID = ref.PORTID;
		return *this;
	};
	void Reset() {
		EQUIPMENTID = "";
		IPADDRESS = "";
		USERID = "";
		SUBEQPID = "";
		EESMODE = "";
		PORTID = "";

	};
	CString		Get_EQUIPMENTID() const {
		return EQUIPMENTID;	};
	CString		Get_IPADDRESS() const {
		return IPADDRESS;	};
	CString		Get_USERID() const {
		return USERID;	};
	CString		Get_SUBEQPID() const {
		return SUBEQPID;	};
	CString		Get_EESMODE() const {
		return EESMODE;	};
	CString		Get_PORTID() const {
		return PORTID;	};


	void		Set_EQUIPMENTID(CString	IN_Data) {
		EQUIPMENTID = IN_Data;	};
	void		Set_IPADDRESS(CString	IN_Data) {
		IPADDRESS = IN_Data;	};
	void		Set_USERID(CString	IN_Data) {
		USERID = IN_Data;	};
	void		Set_SUBEQPID(CString	IN_Data) {
		SUBEQPID = IN_Data;};
	void		Set_EESMODE(CString	IN_Data) {
		EESMODE = IN_Data;};
	void		Set_PORTID(CString	IN_Data) {
		PORTID = IN_Data;};


};
class CEES_PortSubStatus
{
protected:
	//Base Port State
	int8_t		nStatus = 0;
	CString		szRfid;
	CString		szBarcode;
	CString		szLotID;
	//Add EES Port State
	int8_t		nOperMode = 0;		// ?댁쁺紐⑤뱶
	int8_t		nOldOperMode = 0;

	int8_t		nPortStatus = 0;		// ?ㅻ퉬 媛???곹깭
	int8_t		nOldPortStatus = 0;

	int8_t		nProcessStatus = 0;		// ?ㅻ퉬 媛???곹깭
	int8_t		nOldProcessStatus = 0;

	int8_t		nEquipmentStatus = 0;
	int8_t		nOldEquipmentStatus = 0;

	int8_t		nConnection = 0;
	int8_t		nOldConnection = 0;
public:

	CEES_PortSubStatus& operator= (const CEES_PortSubStatus& ref)
	{		
		//Base Port State
		nStatus = ref.nStatus;
		szRfid = ref.szRfid;
		szBarcode = ref.szBarcode;
		szLotID = ref.szLotID;
		//Add EES Port State
		nOperMode = ref.nOperMode;
		nOldOperMode = ref.nOldOperMode;

		nPortStatus = ref.nPortStatus;
		nOldPortStatus = ref.nOldPortStatus;

		nProcessStatus = ref.nProcessStatus;
		nOldProcessStatus = ref.nOldProcessStatus;

		nEquipmentStatus = ref.nEquipmentStatus;
		nOldEquipmentStatus = ref.nOldEquipmentStatus;

		nConnection = ref.nConnection;
		nOldConnection = ref.nOldConnection;
		return *this;
	};
	void Reset() {
		nStatus = 0;
		szRfid = "";
		szBarcode = "";
		szLotID = "";

		nOperMode = -1;
		nOldOperMode = -1;

		nPortStatus = -1;		// ?ㅻ퉬 媛???곹깭
		nOldPortStatus = -1;

		nProcessStatus = -1;		// ?ㅻ퉬 媛???곹깭
		nOldProcessStatus = -1;
		nEquipmentStatus = -1;
		nOldEquipmentStatus = -1;
		nConnection = -1;
		nOldConnection = -1;
	};
	int8_t		Get_nStatus() const {
		return nStatus;	};
	CString		Get_szRfid() const {
		return szRfid;	};
	CString		Get_szBarcode() const {
		return szBarcode;	};
	CString		Get_szLotID() const {
		return szLotID;	};

	int8_t		Get_nOperMode() const {
		return nOperMode;	};
	int8_t		Get_nOldOperMode() const {
		return nOldOperMode;};
	int8_t		Get_nPortStatus() const {
		return nPortStatus;	};
	int8_t		Get_nOldPortStatus() const {
		return nOldPortStatus;	};
	int8_t		Get_nProcessStatus() const {
		return nProcessStatus;};
	int8_t		Get_nOldProcessStatus() const {
		return nOldProcessStatus;};
	int8_t		Get_nEquipmentStatus() const {
		return nEquipmentStatus;	};
	int8_t		Get_nOldEquipmentStatus() const {
		return nOldEquipmentStatus;	};
	int8_t		Get_nConnection() const {
		return nConnection;};
	int8_t		Get_nOldConnection() const {
		return nOldConnection;	};

	void		Set_nStatus(int8_t	IN_Data) {
		nStatus = IN_Data;	};
	void		Set_szRfid(CString	IN_Data) {
		szRfid = IN_Data;	};
	void		Set_szBarcode(CString	IN_Data) {
		szBarcode = IN_Data;	};
	void		Set_szLotID(CString	IN_Data) {
		szLotID = IN_Data;	};

	void		Set_nOperMode(int8_t	IN_Data) {
		if (IN_Data < 0) {
			IN_Data = 0;
		}
		nOperMode = IN_Data;	};
	void		Set_nOldOperMode(int8_t	IN_Data) {
		nOldOperMode = IN_Data;	};
	void		Set_nPortStatus(int8_t	IN_Data) {
		if (IN_Data < 0) {
			IN_Data = 0;
		}
		nPortStatus = IN_Data;	};
	void		Set_nOldPortStatus(int8_t	IN_Data) {
		nOldPortStatus = IN_Data;	};
	void		Set_nProcessStatus(int8_t	IN_Data) {
		if (IN_Data < 0) {
			IN_Data = 0;
		}
		nProcessStatus = IN_Data;	};
	void		Set_nOldProcessStatus(int8_t	IN_Data) {
		nOldProcessStatus = IN_Data;	};
	void		Set_nEquipmentStatus(int8_t	IN_Data) {
		if (IN_Data < 0) {
			IN_Data = 0;
		}
		nEquipmentStatus = IN_Data;	};
	void		Set_nOldEquipmentStatus(int8_t	IN_Data) {
		nOldEquipmentStatus = IN_Data;	};
	void		Set_nConnection(int8_t	IN_Data) {
		nConnection = IN_Data;};
	void		Set_nOldConnection(int8_t	IN_Data) {
		nOldConnection = IN_Data;};
};

class CLossWindow
{
protected:
	uint32_t	LossState;
	CString		LossCount;
public:
	CLossWindow& operator= (const CLossWindow& ref)
	{
		LossState = ref.LossState;
		LossCount = ref.LossCount;
		return *this;
	};
	void Reset()
	{
		LossState = 0;
		LossCount = "";
	};
	uint8_t		Get_LossState() const {
		return LossState;	};
	CString		Get_LossCount() const {
		return LossCount;	};

	void		Set_LossState(uint8_t	IN_Data) {
		LossState = IN_Data;	};
	void		Set_LossCount(CString	IN_Data) {
		LossCount = IN_Data;	};
};
class CTerminalLog
{
protected:
	uint32_t	m_nAlarmCode = 0;
	uint32_t	m_nAlarmSet = ALARMSET_NULL;
	CString		m_szAlarmInfo;
public:
	CTerminalLog& operator= (const CTerminalLog& ref)
	{
		m_nAlarmCode = ref.m_nAlarmCode;
		m_szAlarmInfo = ref.m_szAlarmInfo;
		m_nAlarmSet = ref.m_nAlarmSet;
		return *this;
	};
	void Reset() {
		m_nAlarmCode = 0;
		m_nAlarmSet = ALARMSET_NULL;
		m_szAlarmInfo = "";
	};
};
class CAlarmStatus
{
protected:
	uint32_t	nAlarmCode = 0;
	uint32_t	nAlarmSet = ALARMSET_NULL;
	CString		szAlarmInfo ;
public:
	CAlarmStatus::CAlarmStatus(){

	};
	CAlarmStatus::~CAlarmStatus(){

	};
	CAlarmStatus& operator= (const CAlarmStatus& ref)	{
		nAlarmCode = ref.nAlarmCode;		
		nAlarmSet = ref.nAlarmSet;
		szAlarmInfo = ref.szAlarmInfo;
		return *this;
	};
	void Reset()	{
		nAlarmCode = 0;
		nAlarmSet = ALARMSET_NULL;
		szAlarmInfo = "";
	};
	uint8_t		Get_nAlarmCode() const {
		return nAlarmCode;	};
	uint8_t		Get_nAlarmSet() const {
		return nAlarmSet;	};
	CString		Get_szAlarmInfo() const {
		return szAlarmInfo;	};

	void		Set_nAlarmCode(uint8_t	IN_Data) {
		nAlarmCode = IN_Data;	};
	void		Set_nAlarmSet(uint8_t	IN_Data) {
		nAlarmSet = IN_Data;	};
	void		Set_szAlarmInfo(CString	IN_Data) {
		szAlarmInfo = IN_Data;	};
};

class CCommonModule 
{
protected:
	CConfig_EES			EES;
	CDEFINEDATA			DEFINEDATA;

	std::vector<ST_EquipmentID>		m_nEquipmentID;
	std::vector<CEES_PortSubStatus>	m_ptEES_PortSubStatus;
	std::vector<CLossWindow>		m_ptLossWindow;
	std::vector<CTerminalLog>		m_ptTerminalLog;
	std::vector<CAlarmStatus>		m_ptAlarmStatus;

	
	ST_EquipmentID		m_EqEES_Cfg;
	CEES_PortSubStatus	m_EqEES_PortSubStatus;
	CLossWindow			m_EqLossWindow;
	CTerminalLog		m_EqTerminalLog;
	CAlarmStatus		m_EqAlarmStatus;

public:	
	CCommonModule();
	~CCommonModule();
	CCommonModule& operator= (const CCommonModule	&		ref);
	CCommonModule& operator= (const CConfig_EES	&		ref);
	CCommonModule& operator= (const CDEFINEDATA	&		ref);

	CCommonModule& operator= (std::vector<ST_EquipmentID>& ref);
	CCommonModule& operator= (std::vector<CEES_PortSubStatus>& ref);
	CCommonModule& operator= (std::vector<CLossWindow>	&		ref);
	CCommonModule& operator= (std::vector<CTerminalLog>	&	ref);
	CCommonModule& operator= (std::vector<CAlarmStatus>	& ref);

	//CCommonModule& operator= (ST_EquipmentID& ref);
	CCommonModule& operator= (CEES_PortSubStatus& ref);
	CCommonModule& operator= (CLossWindow	&		ref);
	CCommonModule& operator= (CTerminalLog	&	ref);
	CCommonModule& operator= (CAlarmStatus	& ref);

	void Reset();
public:
	
	CConfig_EES & Get_EES();
	void Set_EES(CConfig_EES & Data);

	CDEFINEDATA & Get_DEFINEDATA();
	void Set_DEFINEDATA(CDEFINEDATA & Data);

	const ST_EquipmentID & Get_EquipmentIDStatus(uint8_t IN_nPortIndex) const;
	std::vector<ST_EquipmentID>& Get_EquipmentIDStatus();
	size_t Get_EquipmentIDCount() const;
	virtual void	Set_PortStatusEquipmentIDEvent(__in uint8_t IN_nPortIndex, __in ST_EquipmentID * IN_nData);
	void assign_EquipmentIDStatus(__in uint8_t IN_nPortIndex, __in ST_EquipmentID In_DATA);

	CEES_PortSubStatus Get_mEES_PortSubStatus(__in uint8_t IN_nStatus) const;
	void	Set_mEES_PortSubStatus(__in uint8_t IN_nStatus, __in CEES_PortSubStatus In_DATA);
	size_t Get_mEES_PortSubStatusCount()const;
	std::vector<CEES_PortSubStatus> Get_mEES_PortSubStatus() const;
	void assign_mEES_PortSubStatus(__in uint8_t IN_nStatus, __in  CEES_PortSubStatus In_DATA);

	CLossWindow Get_mLossWindow(__in uint8_t IN_nStatus) const;
	void	Set_mLossWindow(__in uint8_t IN_nStatus, __in CLossWindow In_DATA);
	size_t Get_mLossWindowCount()const;

	CTerminalLog Get_mTerminalLog(__in uint8_t IN_nStatus) const;
	void	Set_mTerminalLog(__in uint8_t IN_nStatus, __in CTerminalLog In_DATA);
	size_t Get_mTerminalLogCount()const;

	CAlarmStatus Get_mAlarmStatus(__in uint8_t IN_nStatus) const;
	void	Set_mAlarmStatus(__in uint8_t IN_nStatus, __in CAlarmStatus In_DATA);
	void	Set_mAlarmStatus(__in CAlarmStatus  In_DATA);
	size_t Get_mAlarmStatusCount()const;


	CEES_PortSubStatus Get_sEES_PortSubStatus() const;
	void	Set_sEES_PortSubStatus(__in CEES_PortSubStatus In_DATA);

	CLossWindow Get_sLossWindow() const;
	void	Set_sLossWindow(__in CLossWindow In_DATA);

	CTerminalLog Get_sTerminalLog() const;
	void	Set_sTerminalLog(__in CTerminalLog In_DATA);

	CAlarmStatus Get_sAlarmStatus() const;
	void	Set_sAlarmStatus(__in CAlarmStatus In_DATA);
	//XML Server CFG
protected:
	CString m_nSubEqpID;
public:
	CString Get_SubEqpID() const;
	void	Set_SubEqpID(__in LPCTSTR IN_DATA);


	lt::Report_Online_State_Args::Args *	Create_Report_Online_StateArgs(lt::Report_Online_State_Args::Args * Data);
	lt::Report_User_Change_Args::Args *		Create_Report_User_ChangeArgs(lt::Report_User_Change_Args::Args * Data);
	lt::Reply_User_Change_Args::Args *	Create_Reply_User_ChangeArgs(lt::Reply_User_Change_Args::Args * Data);
	lt::Report_Equipment_State_Args::Args * Create_Report_Equipment_StateArgs(lt::Report_Equipment_State_Args::Args * Data);
	lt::Reply_Equipment_State_Display_Args::Args * Create_Reply_Equipment_State_DisplayArgs(lt::Reply_Equipment_State_Display_Args::Args * Data);
	lt::Report_Loss_State_Args::Args * Create_Report_Loss_StateArgs(lt::Report_Loss_State_Args::Args * Data);
	lt::Reply_Loss_Window_Args::Args * Create_Reply_Loss_WindowArgs(lt::Reply_Loss_Window_Args::Args * Data);
	lt::Report_Alarm_State_Args::Args * Create_Report_Alarm_StateArgs(lt::Report_Alarm_State_Args::Args * Data);
	lt::Reply_Alarm_List_Args::Args * Create_Reply_Alarm_ListArgs(lt::Reply_Alarm_List_Args::Args * Data);
	lt::Report_Rms_Mode_Args::Args * Create_Report_Rms_ModeArgs(lt::Report_Rms_Mode_Args::Args * Data);
	lt::Reply_Set_DateTime_Args::Args * Create_Reply_Set_DateTimeArgs(lt::Reply_Set_DateTime_Args::Args * Data);
	lt::Reply_Terminal_Message_Args::Args * Create_Reply_Terminal_MessageArgs(lt::Reply_Terminal_Message_Args::Args * Data);
	lt::Reply_Opcall_Args::Args * Create_Reply_OpcallArgs(lt::Reply_Opcall_Args::Args * Data);

	void	Set_Notify_EquipmentState(__in lt::Report_Equipment_State_Args::Args & IN_DATA);
};
//---------------------------------------------------------
#endif // CommonModule_h__