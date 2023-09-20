//*****************************************************************************
// Filename	: 	Def_ConfigEquipment.h
// Created	:	2021/11/19 - 10:25
// Modified	:	2021/11/19 - 10:25
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#ifndef Def_ConfigServer_h__
#define Def_ConfigServer_h__

#pragma once

#include <stdint.h>
#include <vector>
#include "Def_Server_Type.h"

//-----------------------------------------------------------------------------
class CConfig_SUB
{
protected:
	CString	EQUIPMENTID;
	CString	PORTID;
public:
	CConfig_SUB() {
		EQUIPMENTID = "";
		PORTID = "";
	};
	~CConfig_SUB() {
	};
	void Reset() {
		EQUIPMENTID = "";
		PORTID = "";
	};
	CConfig_SUB& operator= (const CConfig_SUB& ref) {
		EQUIPMENTID = ref.EQUIPMENTID;
		PORTID = ref.PORTID;
		return *this;	};
	CString		Get_EQUIPMENTID() const {
		return EQUIPMENTID;	};
	CString		Get_PORTID() const {
		return PORTID;	};
	bool	Compare_EQUIPMENTID(__in LPCTSTR	IN_nIndex) const {
		return EQUIPMENTID.Compare(IN_nIndex);	};
	bool	Compare_PORTID(__in LPCTSTR	IN_nIndex) const {
		return PORTID.Compare(IN_nIndex);	};
	void		Set_EQUIPMENTID(__in CString	IN_PARA) {
		EQUIPMENTID = IN_PARA;	};
	void		Set_PORTID(__in CString	IN_PARA) {
		PORTID = IN_PARA;	};
};
class CConfig_Svr
{
protected:
	uint8_t		m_nSvrOrder;			
	bool		m_bGroup;				
	uint8_t		m_nGroupIndex;			
	CString		m_szAlias;					
	uint8_t		m_nSvrType_UI;			
private:
	uint8_t		m_nServerType;		
protected:
	CString		m_szServerId;		
	uint32_t	m_nIP_Address;			
	uint8_t		m_nTestPortCount;
	uint8_t		m_nReservablePortCount;
public:
	std::vector<CConfig_SUB> Sub;
	CConfig_Svr()	{
		m_nSvrOrder = 0;
		m_bGroup				= false;
		m_nGroupIndex			= 0;

		m_nSvrType_UI			= 0;
		m_nServerType			= 0;
		m_nIP_Address			= 0;

		m_nTestPortCount		= 6;
		m_nReservablePortCount	= 6;
	};
	~CConfig_Svr()	{
	};
	CConfig_Svr& operator= (const CConfig_Svr& ref)	{
		Sub.clear();
		Sub						= ref.Sub;
		m_nSvrOrder				= ref.m_nSvrOrder;
		m_bGroup				= ref.m_bGroup;
		m_nGroupIndex			= ref.m_nGroupIndex;
		m_szAlias				= ref.m_szAlias;
		m_nSvrType_UI			= ref.m_nSvrType_UI;
		m_nServerType			= ref.m_nServerType;
		m_szServerId			= ref.m_szServerId;
		m_nIP_Address			= ref.m_nIP_Address;
		m_nTestPortCount		= ref.m_nTestPortCount;
		m_nReservablePortCount	= ref.m_nReservablePortCount;
		return *this;
	};
	CConfig_Svr& operator= (const std::vector<CConfig_SUB> & ref) {
		Sub.clear();
		Sub = ref;
		return *this;
	};
	void Reset()
	{
		Sub.clear();
		m_nSvrOrder = 0;
		m_bGroup				= false;
		m_nGroupIndex			= 0;
		m_szAlias.Empty();
		m_nSvrType_UI			= 0;
		m_nServerType			= 0;
		m_szServerId.Empty();
		m_nIP_Address			= 0;
		m_nTestPortCount		= 6;
		m_nReservablePortCount	= 6;
	};

protected:
	virtual void Init_ServerType_UI()	{
		switch (m_nSvrType_UI)		{
		case SERVER_EES:				// 0
			break;
		}
	};
public:
	uint8_t		Get_SvrOrder		() const	{
		return m_nSvrOrder;	};
	bool		Get_Group			() const	{
		return m_bGroup;	};
	uint8_t		Get_GroupIndex		() const	{
		return m_nGroupIndex;	};
	CString		Get_Alias			() const	{
		return m_szAlias;	};
	uint8_t		Get_SvrType_UI		() const	{
		return m_nSvrType_UI;	};
	uint8_t		Get_ServerType	() const	{
		return m_nServerType;	};
	CString		Get_ServerId		() const	{
		return m_szServerId;	};
	uint32_t	Get_IP_Address		() const	{
		return m_nIP_Address;	};
	uint8_t		Get_TestPortCount() const	{
		return m_nTestPortCount;	};
	uint8_t		Get_ReservablePortCount() const	{
		return m_nReservablePortCount;	};
	
	bool	Compare_Alias(__in LPCTSTR	IN_nIndex) const {
		return m_szAlias.Compare(IN_nIndex);	};
	bool	Compare_ServerId(__in LPCTSTR	IN_nIndex) const {
		return m_szServerId.Compare(IN_nIndex);	};

	CConfig_SUB		Get_Sub(__in uint8_t	IN_nIndex) const {
		return Sub.at(IN_nIndex);	};
	std::vector<CConfig_SUB>	Get_Sub() const {
		return Sub;	};

	void		Set_SvrOrder		(__in uint8_t	IN_nSvrOrder)	{
		m_nSvrOrder = IN_nSvrOrder;	};
	void		Set_Group			(__in bool		IN_bGroup)	{
		m_bGroup = IN_bGroup;	};
	void		Set_GroupIndex		(__in uint8_t	IN_nGroupIndex)	{
		m_nGroupIndex = IN_nGroupIndex;	};
	void		Set_Alias			(__in LPCTSTR	IN_szAlias)	{
		m_szAlias = IN_szAlias;	};
	void		Set_SvrType_UI		(__in uint8_t	IN_nSvrType_UI)	{
		m_nSvrType_UI = IN_nSvrType_UI;
		switch (IN_nSvrType_UI)		{
		case SERVER_UI_EES:
			Set_ServerType(enServerType::SERVER_EES);
			break;
		default:
			break;
		}		
		Init_ServerType_UI();
	};
private:
	void	Set_ServerType(__in uint8_t	IN_nEquipmentType)	{
		if (IN_nEquipmentType < enServerType::SERVER_MAX)		{
			m_nServerType = IN_nEquipmentType;		}
		else		{
			m_nServerType = enServerType::SERVER_EES;
		}
	};
public:
	void		Set_ServerId		(__in LPCTSTR	IN_szEquipmentId)	{
		m_szServerId = IN_szEquipmentId;	};
	void		Set_IP_Address		(__in uint32_t	IN_nIP_Address)	{
		m_nIP_Address = IN_nIP_Address;	};
	void		Set_TestPortCount(__in uint8_t IN_nTestPortCount)	{
		m_nTestPortCount = IN_nTestPortCount;	};
	void		Set_ReservablePortCount(__in uint8_t IN_nReservablePortCount)	{
		m_nReservablePortCount = IN_nReservablePortCount;	};
}; // CConfig_Eqp

#endif // Def_ConfigEquipment_h__
