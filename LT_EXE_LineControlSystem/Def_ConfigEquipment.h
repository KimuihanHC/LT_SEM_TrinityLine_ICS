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
// Í≤Ä?¨Í∏∞ ?§Ï†ï??Í∏∞Î≥∏ Íµ¨Ï°∞Ï≤?//-----------------------------------------------------------------------------
class CConfig_Eqp
{
protected:
//public://2022.12.29a
	uint8_t		m_nEqpOrder;			// ?ºÏù∏?êÏÑú???§ÎπÑ ?úÏÑú(Î≤àÌò∏)
	bool		m_bGroup;				// Í≤Ä??Í∑∏Î£π (?)
	uint8_t		m_nGroupIndex;			// Í≤Ä??Í∑∏Î£π???§ÎπÑ ?∏Îç±??
	CString		m_szAlias;				// Eqp Type + Í≤Ä?¨Í∏∞Î≥?Number (?§ÎπÑ ?úÏÑú???∞Î•∏ ?êÎèô ?ùÏÑ±)
	
	uint8_t		m_nEqpType_UI;			// Equipmnet Type (UI ?§Ï†ï Î∞??åÏùº ?Ä?•Ïö©)

private:
	uint8_t		m_nEquipmentType;		// Equipmnet Type	
protected:
	CString		m_szEquipmentId;		// Equipment id (Í≥†Ïú†id)
	uint32_t	m_nIP_Address;			// ip address

	// uint8_t		m_nPortCount;		// Port Count
	// uint8_t		m_nConveyorCount;	// Conveyor Count
	uint8_t		m_nTestPortCount;		// Í≤Ä?¨Í? ÏßÑÌñâ?òÎäî Port Í∞úÏàò
	uint8_t		m_nReservablePortCount;	// Í≤Ä?¨Í? ÏßÑÌñâ?òÎäî Port + Buffer Í∞úÏàò

public:
	CConfig_Eqp()
	{
		m_nEqpOrder				= 0;
		m_bGroup				= false;
		m_nGroupIndex			= 0;

		m_nEqpType_UI			= 0;
		m_nEquipmentType		= 0;
		m_nIP_Address			= 0;

		m_nTestPortCount		= 2;	// Tester Í∏∞Ï? 2 ?åÎùº (Í∏∞Î≥∏)
		m_nReservablePortCount	= 3;	// Tester Í∏∞Ï? 3 ?¨Ìä∏ (Í∏∞Î≥∏)
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

		m_nTestPortCount		= 2;
		m_nReservablePortCount	= 3;
	};

protected:

	virtual void Init_EquipmentType_UI()
	{
		switch (m_nEqpType_UI)
		{
		case EqpUI_Loader:				// 0
		case EqpUI_Returner:			// 1
			m_nTestPortCount		= 0;
			m_nReservablePortCount	= 0;
			break;

		case EqpUI_ColorCal:			// 2 
		case EqpUI_SFR_CL_46:			// 3 : SFR ?ëÍ∞Å
		case EqpUI_SFR_MultiCL_83:		// 4 : 83
		case EqpUI_SFR_MultiCL_180:		// 5 : 180
		case EqpUI_HotPixel:			// 6
		case EqpUI_StrayLight:			// 7 : Ghost & Flare
		case EqpUI_Distortion:			// 8
		case EqpUI_SFR_MultiCL:			// 9 : SFR Í¥ëÍ∞Å
			m_nTestPortCount		= 2;
			m_nReservablePortCount	= 3;
			break;
		case EqpUI_HotPixel3port:		// 10 : Hot Pixel 3 Para
			m_nTestPortCount		= 3;
			m_nReservablePortCount	= 4;
			break;
		}
	};

// 	virtual void Init_EquipmentType()
// 	{
// 		switch (m_nEquipmentType)
// 		{
// 		case Eqp_Loader:			// 0
// 		case Eqp_Returner:			// 1
// 			m_nTestPortCount		= 0;
// 			m_nReservablePortCount	= 0;
// 			break;
// 
// 		case Eqp_ColorCal:			// 2 
// 		case Eqp_SFR_CL_46:			// 3 : SFR ?ëÍ∞Å
// 		case Eqp_SFR_MultiCL_83:	// 4 : 83
// 		case Eqp_SFR_MultiCL_180:	// 5 : 180
// 		case Eqp_HotPixel:			// 6
// 		case Eqp_StrayLight:		// 7 : Ghost & Flare
// 		case Eqp_Distortion:		// 8
// 		case Eqp_SFR_MultiCL:		// 9 : SFR Í¥ëÍ∞Å
// 			m_nTestPortCount		= 2;
// 			m_nReservablePortCount	= 3;
// 			break;
// 		case Eqp_HotPixel3port:		// 10 : Hot Pixel 3 Para
// 			m_nTestPortCount		= 3;
// 			m_nReservablePortCount	= 4;
// 			break;
// 		}
// 	};
	

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
