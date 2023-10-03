//*****************************************************************************
// Filename	: 	Def_ConfigEquipment.h
// Created	:	2021/11/19 - 10:25
// Modified	:	2021/11/19 - 10:25
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#ifndef Def_ConfigEquipment_h__
#define Def_ConfigEquipment_h__

#pragma once

#include <stdint.h>
#include "Def_Equipment_Type.h"

//-----------------------------------------------------------------------------
// 검사기 설정용 기본 구조체
//-----------------------------------------------------------------------------
class CConfig_Eqp
{
//public:
protected:

	uint8_t		m_nEqpOrder;			// 라인에서의 설비 순서(번호)
	bool		m_bGroup;				// 검사 그룹 (?)
	uint8_t		m_nGroupIndex;			// 검사 그룹내 설비 인덱스

	CString		m_szAlias;				// Eqp Type + 검사기별 Number (설비 순서에 따른 자동 생성)

	uint8_t		m_nEqpType_UI;			// Equipmnet Type (UI 설정 및 파일 저장용)

private:
	uint8_t		m_nEquipmentType;		// Equipmnet Type	
protected:
	CString		m_szEquipmentId;		// Equipment id (怨좎쑀id)
	uint32_t	m_nIP_Address;			// ip address

	// uint8_t		m_nPortCount;		// Port Count
	// uint8_t		m_nConveyorCount;	// Conveyor Count
	uint8_t		m_nBuffercount; // Buffer Count
	uint8_t		m_nTestPortCount;		// 검사가 진행되는 Port 개수
	uint8_t		m_nReservablePortCount;	// 검사가 진행되는 Port + Buffer 개수

public:
	CConfig_Eqp()
	{
		m_nEqpOrder				= 0;
		m_bGroup				= false;
		m_nGroupIndex			= 0;

		m_nEqpType_UI			= 0;
		m_nEquipmentType		= 0;
		m_nIP_Address			= 0;

		m_nBuffercount = 1;
		m_nTestPortCount = 2;	// Tester 기준 2 파라 (기본)
		m_nReservablePortCount = 3;	// Tester 기준 3 포트 (기본)
	};

	~CConfig_Eqp()
	{

	};

	CConfig_Eqp& operator= (const CConfig_Eqp& ref)
	{
		m_nEqpOrder				= ref.m_nEqpOrder;
		m_bGroup				= ref.m_bGroup;
		m_nGroupIndex			= ref.m_nGroupIndex;

		m_szAlias				= ref.m_szAlias;
		m_nEqpType_UI			= ref.m_nEqpType_UI;
		m_nEquipmentType		= ref.m_nEquipmentType;
		m_szEquipmentId			= ref.m_szEquipmentId;
		m_nIP_Address			= ref.m_nIP_Address;

		m_nBuffercount			= ref.m_nBuffercount;
		m_nTestPortCount		= ref.m_nTestPortCount;
		m_nReservablePortCount	= ref.m_nReservablePortCount;
		return *this;
	};

	void Reset()
	{
		m_nEqpOrder				= 0;
		m_bGroup				= false;
		m_nGroupIndex			= 0;

		m_szAlias.Empty();
		m_nEqpType_UI			= 0;
		m_nEquipmentType		= 0;
		m_szEquipmentId.Empty();
		m_nIP_Address			= 0;

		m_nBuffercount			= 1;
		m_nTestPortCount		= 2;
		m_nReservablePortCount	= 3;
	};

protected:

	virtual void Init_EquipmentType_UI()
	{
#if (SET_INSPECTOR == SYS_ICS_RIVIAN_LINE)
		switch (m_nEqpType_UI)
		{
		case EqpUI_Handler:				// 0
		case EqpUI_Loader:				// 1
		case EqpUI_Returner:			// 2
			m_nBuffercount = 0;
			m_nTestPortCount = 0;
			m_nReservablePortCount = 0;
			break;

		case EqpUI_ColorCal:			// 3 
		case EqpUI_SFR_CL:				// 4 : SFR 협각
		case EqpUI_SFR_MultiCL:			// 7 : SFR 광각
		case EqpUI_HotPixel:			// 8
		case EqpUI_StrayLight:			// 10 : Ghost & Flare
			m_nBuffercount = 0;
			m_nTestPortCount = 1;
			m_nReservablePortCount = 2; // 1;
			break;

		case EqpUI_Distortion:			// 11
			m_nBuffercount = 0;
			m_nTestPortCount = 2;
			m_nReservablePortCount = 3; // 2;
			break;
		}
#else
		switch (m_nEqpType_UI)
		{
		case EqpUI_Handler:				// 0
		case EqpUI_Loader:				// 1
		case EqpUI_Returner:			// 2
			m_nBuffercount = 0;
			m_nTestPortCount = 0;
			m_nReservablePortCount = 0;
			break;

		case EqpUI_ColorCal:			// 3 
		case EqpUI_SFR_CL_46:			// 4 : SFR 협각
		case EqpUI_SFR_MultiCL_83:		// 5 : 83
		case EqpUI_SFR_MultiCL_180:		// 6 : 180
		case EqpUI_SFR_MultiCL:			// 7 : SFR 광각
		case EqpUI_HotPixel:			// 8
		case EqpUI_StrayLight:			// 10 : Ghost & Flare
		case EqpUI_Distortion:			// 11
			m_nBuffercount = 1;
			m_nTestPortCount = 2;
			m_nReservablePortCount = 3;
			break;

		case EqpUI_HotPixel3port:		// 9 : Hot Pixel 3 Para
			m_nBuffercount = 1;
			m_nTestPortCount = 3;
			m_nReservablePortCount = 4;
			break;
		}
#endif // (SET_INSPECTOR == SYS_ICS_RIVIAN_LINE)
	};

public:
	uint8_t		Get_EqpOrder		() const
	{
		return m_nEqpOrder;
	};

	bool		Get_Group			() const
	{
		return m_bGroup;
	};

	uint8_t		Get_GroupIndex		() const
	{
		return m_nGroupIndex;
	};

	CString		Get_Alias			() const
	{
		return m_szAlias;
	};

	uint8_t		Get_EqpType_UI		() const
	{
		return m_nEqpType_UI;
	};

	uint8_t		Get_EquipmentType	() const
	{
		return m_nEquipmentType;
	};

	uint8_t		Get_TesterType		() const
	{
		return ConvTo_TesterType((enEquipmentType)m_nEquipmentType);
	};

#ifdef 	NEW_INSPECTION_TYPE_APPLY
	uint8_t		Get_InspectionType	() const
	{
		return ConvTo_InspectionType((enEquipmentType)m_nEquipmentType);
	};
#endif	// NEW_INSPECTION_TYPE_APPLY

	CString		Get_EquipmentId		() const
	{
		return m_szEquipmentId;
	};

	uint32_t	Get_IP_Address		() const
	{
		return m_nIP_Address;
	};
	
	uint8_t		Get_BufferCount() const
	{
		return m_nBuffercount;
	};

	uint8_t		Get_TestPortCount() const
	{
		return m_nTestPortCount;
	};

	uint8_t		Get_ReservablePortCount() const
	{
		return m_nReservablePortCount;
	};

	void		Set_EqpOrder		(__in uint8_t	IN_nEqpOrder)
	{
		m_nEqpOrder = IN_nEqpOrder;
	};

	void		Set_Group			(__in bool		IN_bGroup)
	{
		m_bGroup = IN_bGroup;
	};

	void		Set_GroupIndex		(__in uint8_t	IN_nGroupIndex)
	{
		m_nGroupIndex = IN_nGroupIndex;
	};

	void		Set_Alias			(__in LPCTSTR	IN_szAlias)
	{
		m_szAlias = IN_szAlias;
	};

	void		Set_EqpType_UI		(__in uint8_t	IN_nEqpType_UI)
	{
		m_nEqpType_UI = IN_nEqpType_UI;

#if (SET_INSPECTOR == SYS_ICS_RIVIAN_LINE)
		switch (IN_nEqpType_UI)
		{
		case EqpUI_Handler:
			Set_EquipmentType(enEquipmentType::Eqp_Handler);
			break;

		case EqpUI_Loader:
			Set_EquipmentType(enEquipmentType::Eqp_Loader);
			break;

		case EqpUI_Returner:
			Set_EquipmentType(enEquipmentType::Eqp_Returner);
			break;

		case EqpUI_ColorCal:
			Set_EquipmentType(enEquipmentType::Eqp_ColorCal);
			break;

		case EqpUI_SFR_CL:
			Set_EquipmentType(enEquipmentType::Eqp_SFR_CL);
			break;

		case EqpUI_SFR_MultiCL:
			Set_EquipmentType(enEquipmentType::Eqp_SFR_MultiCL);
			break;

		case EqpUI_HotPixel:
			Set_EquipmentType(enEquipmentType::Eqp_HotPixel);
			break;

		case EqpUI_StrayLight:
			Set_EquipmentType(enEquipmentType::Eqp_StrayLight);
			break;

		case EqpUI_Distortion:
			Set_EquipmentType(enEquipmentType::Eqp_Distortion);
			break;

		case EqpUI_EEPROM:
			Set_EquipmentType(enEquipmentType::Eqp_EEPROM);
			break;

		default:
			break;
		}
#else
		switch (IN_nEqpType_UI)
		{
		case EqpUI_Handler:
			Set_EquipmentType(enEquipmentType::Eqp_Handler);
			break;

		case EqpUI_Loader:
			Set_EquipmentType(enEquipmentType::Eqp_Loader);
			break;

		case EqpUI_Returner:
			Set_EquipmentType(enEquipmentType::Eqp_Returner);
			break;

		case EqpUI_ColorCal:
			Set_EquipmentType(enEquipmentType::Eqp_ColorCal);
			break;

		case EqpUI_SFR_CL_46:
			Set_EquipmentType(enEquipmentType::Eqp_SFR_CL_46);
			break;

		case EqpUI_SFR_MultiCL_83:
			Set_EquipmentType(enEquipmentType::Eqp_SFR_MultiCL_83);
			break;

		case EqpUI_SFR_MultiCL_180:
			Set_EquipmentType(enEquipmentType::Eqp_SFR_MultiCL_180);
			break;

		case EqpUI_SFR_MultiCL:
			Set_EquipmentType(enEquipmentType::Eqp_SFR_MultiCL);
			break;

		case EqpUI_HotPixel:
		case EqpUI_HotPixel3port:
			Set_EquipmentType(enEquipmentType::Eqp_HotPixel);
			break;

		case EqpUI_StrayLight:
			Set_EquipmentType(enEquipmentType::Eqp_StrayLight);
			break;

		case EqpUI_Distortion:
			Set_EquipmentType(enEquipmentType::Eqp_Distortion);
			break;

		case EqpUI_EEPROM:
			Set_EquipmentType(enEquipmentType::Eqp_EEPROM);
			break;

		default:
			break;
		}		
#endif // (SET_INSPECTOR == SYS_ICS_RIVIAN_LINE)

		Init_EquipmentType_UI();
	};

private:
	void		Set_EquipmentType	(__in uint8_t	IN_nEquipmentType)
	{
		if (IN_nEquipmentType < enEquipmentType::Max_EqpTypeCount)
		{
			m_nEquipmentType = IN_nEquipmentType;
		}
		else
		{
			m_nEquipmentType = enEquipmentType::Eqp_Tester_First;
		}

		//Init_EquipmentType();
	};
public:

	void		Set_EquipmentId		(__in LPCTSTR	IN_szEquipmentId)
	{
		m_szEquipmentId = IN_szEquipmentId;
	};

	void		Set_IP_Address		(__in uint32_t	IN_nIP_Address)
	{
		m_nIP_Address = IN_nIP_Address;
	};

	void		Set_TestPortCount(__in uint8_t IN_nTestPortCount)
	{
		m_nTestPortCount = IN_nTestPortCount;
	};

	void		Set_ReservablePortCount(__in uint8_t IN_nReservablePortCount)
	{
		m_nReservablePortCount = IN_nReservablePortCount;
	};

}; // CConfig_Eqp

#endif // Def_ConfigEquipment_h__
