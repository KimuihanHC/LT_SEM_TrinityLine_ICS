//*****************************************************************************
// Filename	: 	CommonLineInfoData.cpp
// Created	:	2021/11/19 - 10:38
// Modified	:	2021/11/19 - 10:38
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#include "stdafx.h"
#include "CommonModule.h"
//#include "Util/StringUtil.hpp"

CCommonModule::CCommonModule()
{

}

CCommonModule::~CCommonModule()
{

}

CCommonModule& CCommonModule::operator=(const CCommonModule& ref)
{
	//m_ptPortSubStatus = ref.m_ptPortSubStatus;
	m_ptEES_PortSubStatus = ref.m_ptEES_PortSubStatus;
	m_ptLossWindow = ref.m_ptLossWindow;
	m_ptTerminalLog = ref.m_ptTerminalLog;
	m_ptAlarmStatus = ref.m_ptAlarmStatus;

	//m_EqPortSubStatus = ref.m_EqPortSubStatus;
	m_EqEES_PortSubStatus = ref.m_EqEES_PortSubStatus;
	m_EqLossWindow = ref.m_EqLossWindow;
	m_EqTerminalLog = ref.m_EqTerminalLog;
	m_EqAlarmStatus = ref.m_EqAlarmStatus;

	EES = ref.EES;
	return *this;
}
CCommonModule& CCommonModule::operator=(const CConfig_EES& ref){
	EES = ref;
	return *this;
}
CCommonModule& CCommonModule::operator=(const CDEFINEDATA& ref) {
	DEFINEDATA = ref;
	return *this;
}
CCommonModule& CCommonModule::operator=(std::vector<ST_EquipmentID>& ref) {
	m_nEquipmentID = ref;
	return *this;
}
CCommonModule& CCommonModule::operator=(std::vector<CEES_PortSubStatus>& ref){
	m_ptEES_PortSubStatus = ref;
	return *this;
}
CCommonModule& CCommonModule::operator=(std::vector<CLossWindow>	& ref){
	m_ptLossWindow = ref;
	return *this;
}
CCommonModule& CCommonModule::operator=(std::vector<CTerminalLog>	& ref){
	m_ptTerminalLog = ref;
	return *this;
}
CCommonModule& CCommonModule::operator=(std::vector<CAlarmStatus>	& ref){
	m_ptAlarmStatus = ref;
	return *this;
}
/*
CCommonModule& CCommonModule::operator=(ST_EquipmentID& ref) {
	m_EqEES_Cfg = ref;
	return *this;
}*/
CCommonModule& CCommonModule::operator=(CEES_PortSubStatus& ref){
	m_EqEES_PortSubStatus = ref;
	return *this;
}
CCommonModule& CCommonModule::operator=(CLossWindow	& ref){
	m_EqLossWindow = ref;
	return *this;
}
CCommonModule& CCommonModule::operator=(CTerminalLog	& ref){
	m_EqTerminalLog = ref;
	return *this;
}
CCommonModule& CCommonModule::operator=(CAlarmStatus	& ref){
	m_EqAlarmStatus = ref;
	return *this;
}
void CCommonModule::Reset(){
	EES.Reset();
	//m_ptPortSubStatus.clear();
	m_ptEES_PortSubStatus.clear();
	m_ptLossWindow.clear();
	m_ptTerminalLog.clear();
	m_ptAlarmStatus.clear();

	//m_EqPortSubStatus.Reset();
	m_EqEES_PortSubStatus.Reset();
	m_EqLossWindow.Reset();
	m_EqTerminalLog.Reset();
	m_EqAlarmStatus.Reset();
};
CConfig_EES & CCommonModule::Get_EES()
{	return EES;}
void CCommonModule::Set_EES(CConfig_EES & Data)
{	EES = Data;}

CDEFINEDATA & CCommonModule::Get_DEFINEDATA(){
	return DEFINEDATA;}
void CCommonModule::Set_DEFINEDATA(CDEFINEDATA & Data){
	DEFINEDATA = Data;}

size_t CCommonModule::Get_EquipmentIDCount() const {
	return m_nEquipmentID.size();}
std::vector<ST_EquipmentID>& CCommonModule::Get_EquipmentIDStatus() {
	return m_nEquipmentID;}
const ST_EquipmentID& CCommonModule::Get_EquipmentIDStatus(__in uint8_t IN_nPortIndex) const {
	return m_nEquipmentID.at(IN_nPortIndex);}
void CCommonModule::Set_PortStatusEquipmentIDEvent(__in uint8_t IN_nPortIndex, __in ST_EquipmentID * IN_nData)
{
	if (IN_nPortIndex < m_nEquipmentID.size())
	{
		m_nEquipmentID.at(IN_nPortIndex).Set_EQUIPMENTID(IN_nData->Get_EQUIPMENTID());
		m_nEquipmentID.at(IN_nPortIndex).Set_PORTID(IN_nData->Get_PORTID());		
	}
}
void CCommonModule::assign_EquipmentIDStatus(__in uint8_t IN_nPortIndex, __in ST_EquipmentID In_DATA) {
	m_nEquipmentID.assign(IN_nPortIndex, In_DATA);
}

CEES_PortSubStatus CCommonModule::Get_mEES_PortSubStatus(__in uint8_t IN_nStatus)const
{	return m_ptEES_PortSubStatus.at(IN_nStatus);}
void CCommonModule::Set_mEES_PortSubStatus(__in uint8_t IN_nStatus, __in  CEES_PortSubStatus In_DATA)
{	m_ptEES_PortSubStatus.at(IN_nStatus) = In_DATA;}
size_t CCommonModule::Get_mEES_PortSubStatusCount()const
{	return	m_ptEES_PortSubStatus.size(); }
std::vector<CEES_PortSubStatus> CCommonModule::Get_mEES_PortSubStatus() const {
	return m_ptEES_PortSubStatus;}
void CCommonModule::assign_mEES_PortSubStatus(__in uint8_t IN_nStatus, __in  CEES_PortSubStatus In_DATA){
	m_ptEES_PortSubStatus.assign(IN_nStatus, In_DATA);
}

CLossWindow CCommonModule::Get_mLossWindow(__in uint8_t IN_nStatus)const
{	return m_ptLossWindow.at(IN_nStatus);}
void CCommonModule::Set_mLossWindow(__in uint8_t IN_nStatus, __in  CLossWindow In_DATA)
{	m_ptLossWindow.at(IN_nStatus) = In_DATA;}
size_t CCommonModule::Get_mLossWindowCount()const
{	return	m_ptLossWindow.size();}

CTerminalLog CCommonModule::Get_mTerminalLog(__in uint8_t IN_nStatus)const
{	return m_ptTerminalLog.at(IN_nStatus);}
void CCommonModule::Set_mTerminalLog(__in uint8_t IN_nStatus, __in  CTerminalLog In_DATA)
{	m_ptTerminalLog.at(IN_nStatus) = In_DATA;}
size_t CCommonModule::Get_mTerminalLogCount()const
{	return	m_ptTerminalLog.size();}

CAlarmStatus CCommonModule::Get_mAlarmStatus(__in uint8_t IN_nStatus)const
{	return m_ptAlarmStatus.at(IN_nStatus);}
void CCommonModule::Set_mAlarmStatus(__in uint8_t IN_nStatus, __in  CAlarmStatus In_DATA)
{	m_ptAlarmStatus.at(IN_nStatus) = In_DATA;}
void CCommonModule::Set_mAlarmStatus( __in  CAlarmStatus In_DATA)
{	m_ptAlarmStatus.push_back(In_DATA);}
size_t CCommonModule::Get_mAlarmStatusCount()const
{	return	m_ptAlarmStatus.size();}

CEES_PortSubStatus CCommonModule::Get_sEES_PortSubStatus()const
{	return m_EqEES_PortSubStatus;}
void CCommonModule::Set_sEES_PortSubStatus(__in CEES_PortSubStatus In_DATA)
{	m_EqEES_PortSubStatus = In_DATA;}

CLossWindow CCommonModule::Get_sLossWindow()const
{	return m_EqLossWindow;}
void CCommonModule::Set_sLossWindow(__in CLossWindow In_DATA)
{	m_EqLossWindow = In_DATA;}

CTerminalLog CCommonModule::Get_sTerminalLog()const
{	return m_EqTerminalLog;}
void CCommonModule::Set_sTerminalLog(__in CTerminalLog In_DATA)
{	m_EqTerminalLog= In_DATA;}

CAlarmStatus CCommonModule::Get_sAlarmStatus()const
{	return m_EqAlarmStatus;}
void CCommonModule::Set_sAlarmStatus(__in  CAlarmStatus In_DATA)
{	m_EqAlarmStatus = In_DATA;}


CString CCommonModule::Get_SubEqpID() const {
	return m_nSubEqpID;}
void CCommonModule::Set_SubEqpID(__in LPCTSTR IN_DATA) {
	m_nSubEqpID = IN_DATA;}


lt::Report_Online_State_Args::Args * CCommonModule::Create_Report_Online_StateArgs(lt::Report_Online_State_Args::Args * Data) {
	if (Data == nullptr) {
		Data = new lt::Report_Online_State_Args::Args();
	}	
	if (Data->Hd.Get_transactionId().compare("") == 0) {
		Data->Hd.CreateUUID();
	}
	Data->Hd.Get_timeStamp();
	Data->Body.Set_EESMODE(Get_DEFINEDATA().Get_EESMODE());
	Data->Body.Set_ONLINESTATE(Get_DEFINEDATA().Get_ONLINESTATE());

	Data->Body.Set_EQUIPMENTID(Get_DEFINEDATA().Get_EQUIPMENTID());
	Data->Body.Set_IPADDRESS(Get_DEFINEDATA().Get_IPADDRESS());
	Data->Body.Set_USERID(Get_DEFINEDATA().Get_USERID());
	Data->Body.Set_SUBEQPID(Get_DEFINEDATA().Get_SUBEQPID());
	return Data;
}
lt::Report_User_Change_Args::Args * CCommonModule::Create_Report_User_ChangeArgs(lt::Report_User_Change_Args::Args * Data) {
	if (Data == nullptr) {
		Data = new lt::Report_User_Change_Args::Args();
	}
	if (Data->Hd.Get_transactionId().compare("") == 0) {
		Data->Hd.CreateUUID();
	}
	Data->Hd.Get_timeStamp();
	Data->Body.Set_EESMODE(Get_DEFINEDATA().Get_EESMODE());
	Data->Body.Set_EQUIPMENTID(Get_DEFINEDATA().Get_EQUIPMENTID());
	Data->Body.Set_IPADDRESS(Get_DEFINEDATA().Get_IPADDRESS());
	Data->Body.Set_USERID(Get_DEFINEDATA().Get_USERID());
	return Data;
}

lt::Reply_User_Change_Args::Args * CCommonModule::Create_Reply_User_ChangeArgs(lt::Reply_User_Change_Args::Args * Data) {
	if (Data == nullptr) {
		Data = new lt::Reply_User_Change_Args::Args();
	}
	//Data->Hd.CreateUUID();
	Data->Hd.Get_timeStamp();
	Data->Body.Set_EQUIPMENTID(Get_DEFINEDATA().Get_EQUIPMENTID());
	Data->Body.Set_IPADDRESS(Get_DEFINEDATA().Get_IPADDRESS());
	Data->Body.Set_USERID(Get_DEFINEDATA().Get_USERID());
	return Data;
}

lt::Report_Equipment_State_Args::Args * CCommonModule::Create_Report_Equipment_StateArgs(lt::Report_Equipment_State_Args::Args * Data) {
	if (Data == nullptr) {
		Data = new lt::Report_Equipment_State_Args::Args();
	}
	if (Data->Hd.Get_transactionId().compare("") == 0) {
		Data->Hd.CreateUUID();
	}
	Data->Hd.Get_timeStamp();
	Data->Body.Set_EQUIPMENTID(Get_DEFINEDATA().Get_EQUIPMENTID());
	Data->Body.Set_IPADDRESS(Get_DEFINEDATA().Get_IPADDRESS());
	Data->Body.Set_USERID(Get_DEFINEDATA().Get_USERID());
	Data->Body.Set_EESMODE(Get_DEFINEDATA().Get_EESMODE());
	return Data;
}

lt::Reply_Equipment_State_Display_Args::Args * CCommonModule::Create_Reply_Equipment_State_DisplayArgs(lt::Reply_Equipment_State_Display_Args::Args * Data) {
	if (Data == nullptr) {
		Data = new lt::Reply_Equipment_State_Display_Args::Args();
	}
	//Data->Hd.CreateUUID();
	Data->Hd.Get_timeStamp();
	Data->Body.Set_EQUIPMENTID(Get_DEFINEDATA().Get_EQUIPMENTID());
	Data->Body.Set_SUBEQPID(Get_DEFINEDATA().Get_SUBEQPID());
	return Data;
}

lt::Report_Loss_State_Args::Args * CCommonModule::Create_Report_Loss_StateArgs(lt::Report_Loss_State_Args::Args * Data) {
	if (Data == nullptr) {
		Data = new lt::Report_Loss_State_Args::Args();
	}
	if (Data->Hd.Get_transactionId().compare("") == 0) {
		Data->Hd.CreateUUID();
	}
	Data->Hd.Get_timeStamp();
	Data->Body.Set_EQUIPMENTID(Get_DEFINEDATA().Get_EQUIPMENTID());
	Data->Body.Set_SUBEQPID(Get_DEFINEDATA().Get_SUBEQPID());
	Data->Body.Set_EESMODE(Get_DEFINEDATA().Get_EESMODE());
	Data->Body.Set_IPADDRESS(Get_DEFINEDATA().Get_IPADDRESS());
	Data->Body.Set_USERID(Get_DEFINEDATA().Get_USERID());
	return Data;
}

lt::Reply_Loss_Window_Args::Args * CCommonModule::Create_Reply_Loss_WindowArgs(lt::Reply_Loss_Window_Args::Args * Data) {
	if (Data == nullptr) {
		Data = new lt::Reply_Loss_Window_Args::Args();
	}
	//Data->Hd.CreateUUID();
	Data->Hd.Get_timeStamp();
	Data->Body.Set_EQUIPMENTID(Get_DEFINEDATA().Get_EQUIPMENTID());
	Data->Body.Set_SUBEQPID(Get_DEFINEDATA().Get_SUBEQPID());
	Data->Body.Set_IPADDRESS(Get_DEFINEDATA().Get_IPADDRESS());
	return Data;
}

lt::Report_Alarm_State_Args::Args * CCommonModule::Create_Report_Alarm_StateArgs(lt::Report_Alarm_State_Args::Args * Data) {
	if (Data == nullptr) {
		Data = new lt::Report_Alarm_State_Args::Args();
	}
	if (Data->Hd.Get_transactionId().compare("") == 0) {
		Data->Hd.CreateUUID();
	}
	Data->Hd.Get_timeStamp();
	Data->Body.Set_EQUIPMENTID(Get_DEFINEDATA().Get_EQUIPMENTID());
	Data->Body.Set_SUBEQPID(Get_DEFINEDATA().Get_SUBEQPID());
	Data->Body.Set_IPADDRESS(Get_DEFINEDATA().Get_IPADDRESS());
	Data->Body.Set_EESMODE(Get_DEFINEDATA().Get_EESMODE());
	Data->Body.Set_USERID(Get_DEFINEDATA().Get_USERID());
	return Data;
}

lt::Reply_Alarm_List_Args::Args * CCommonModule::Create_Reply_Alarm_ListArgs(lt::Reply_Alarm_List_Args::Args * Data) {
	if (Data == nullptr) {
		Data = new lt::Reply_Alarm_List_Args::Args();
	}
	//Data->Hd.CreateUUID();
	Data->Hd.Get_timeStamp();
	Data->Body.Set_EQUIPMENTID(Get_DEFINEDATA().Get_EQUIPMENTID());
	Data->Body.Set_SUBEQPID(Get_DEFINEDATA().Get_SUBEQPID());
	Data->Body.Set_IPADDRESS(Get_DEFINEDATA().Get_IPADDRESS());
	return Data;
}

lt::Report_Rms_Mode_Args::Args * CCommonModule::Create_Report_Rms_ModeArgs(lt::Report_Rms_Mode_Args::Args * Data) {
	if (Data == nullptr) {
		Data = new lt::Report_Rms_Mode_Args::Args();
	}
	if (Data->Hd.Get_transactionId().compare("") == 0) {
		Data->Hd.CreateUUID();
	}
	Data->Hd.Get_timeStamp();
	Data->Body.Set_EQUIPMENTID(Get_DEFINEDATA().Get_EQUIPMENTID());
	Data->Body.Set_SUBEQPID(Get_DEFINEDATA().Get_SUBEQPID());
	Data->Body.Set_IPADDRESS(Get_DEFINEDATA().Get_IPADDRESS());
	Data->Body.Set_EESMODE(Get_DEFINEDATA().Get_EESMODE());
	Data->Body.Set_USERID(Get_DEFINEDATA().Get_USERID());
	return Data;
}

lt::Reply_Set_DateTime_Args::Args * CCommonModule::Create_Reply_Set_DateTimeArgs(lt::Reply_Set_DateTime_Args::Args * Data) {
	if (Data == nullptr) {
		Data = new lt::Reply_Set_DateTime_Args::Args();
	}
	Data->Hd.Get_timeStamp();
	Data->Body.Set_EQUIPMENTID(Get_DEFINEDATA().Get_EQUIPMENTID());
	Data->Body.Set_IPADDRESS(Get_DEFINEDATA().Get_IPADDRESS());
	return Data;
}

lt::Reply_Terminal_Message_Args::Args * CCommonModule::Create_Reply_Terminal_MessageArgs(lt::Reply_Terminal_Message_Args::Args * Data) {
	if (Data == nullptr) {
		Data = new lt::Reply_Terminal_Message_Args::Args();
	}
	if (Data->Hd.Get_transactionId().compare("") == 0) {
		Data->Hd.CreateUUID();
	}
	Data->Hd.Get_timeStamp();
	Data->Body.Set_EQUIPMENTID(Get_DEFINEDATA().Get_EQUIPMENTID());
	Data->Body.Set_IPADDRESS(Get_DEFINEDATA().Get_IPADDRESS());
	Data->Body.Set_SUBEQPID(Get_DEFINEDATA().Get_SUBEQPID());
	return Data;
}

lt::Reply_Opcall_Args::Args * CCommonModule::Create_Reply_OpcallArgs(lt::Reply_Opcall_Args::Args * Data) {
	if (Data == nullptr) {
		Data = new lt::Reply_Opcall_Args::Args();
	}
	//Data->Hd.CreateUUID();
	Data->Hd.Get_timeStamp();
	Data->Body.Set_EQUIPMENTID(Get_DEFINEDATA().Get_EQUIPMENTID());
	Data->Body.Set_IPADDRESS(Get_DEFINEDATA().Get_IPADDRESS());
	Data->Body.Set_SUBEQPID(Get_DEFINEDATA().Get_SUBEQPID());
	return Data;
}
