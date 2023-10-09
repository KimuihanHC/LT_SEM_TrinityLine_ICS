//*****************************************************************************
// Filename	: 	Def_Equipment_Type.h
// Created	:	2021/10/21 - 15:52
// Modified	:	2021/10/21 - 15:52
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#ifndef Def_Equipment_Type_h__
#define Def_Equipment_Type_h__

#include <stdint.h>
#include <afxwin.h>
#include "Def_CompileOption.h"

#pragma once

// ==================================================================
#if (SET_INSPECTOR == SYS_ICS_RIVIAN_LINE)
// ==================================================================

//-------------------------------------------------------------------
// 설비 타입 : version 2.0
//-------------------------------------------------------------------
typedef enum
{	
	Eqp_Handler,		// 0
	Eqp_Loader,			// 1
	Eqp_Returner,		// 2

	Eqp_ColorCal,		// 3 
	Eqp_SFR_CL,			// 4 : SFR 협각
	Eqp_SFR_MultiCL,	// 7 : SFR 광각
	Eqp_HotPixel,		// 8
	Eqp_StrayLight,		// 9 
	Eqp_Distortion,		// 10
	Eqp_EEPROM,			// 11 : 예약

	Max_EqpTypeCount,
	Eqp_Tester_First	= Eqp_ColorCal,
	Eqp_Tester_Last		= Eqp_EEPROM,

}enEquipmentType;

static LPCTSTR g_szEqpTypeName[] =
{
	_T("Handler"),
	_T("Loader Machine"),
	_T("Returner"),

	_T("Color Calibration"),
	_T("SFR CL"),			// 협각
	_T("SFR Multi CL"),		// 광각
	_T("Hot Pixel"),
	_T("Stray Light"),
	_T("Intrinsic Cal."),
	_T("EEPROM"),
	nullptr
};

static LPCTSTR g_szEqpTypeInitial[] =
{
	_T("Hlr"),
	_T("Loa"),
	_T("Ret"),
	_T("CCa"),
	_T("SCL"),
	_T("SMC"),
	_T("Hot"),
	_T("GnF"),
	_T("Cal"),
	_T("E2P"),
	nullptr
};

//-------------------------------------------------------------------
// UI 설정 및 상세 설정용 설비 타입
//-------------------------------------------------------------------
typedef enum
{	
	EqpUI_Handler,			//  0
	EqpUI_Loader,			//  1
	EqpUI_Returner,			//  2
							    
	EqpUI_ColorCal,			//  3 
	EqpUI_SFR_CL,			//  4 : SFR 협각
	EqpUI_SFR_MultiCL,		//  7 : SFR 광각
	EqpUI_HotPixel,			//  8
	EqpUI_StrayLight,		// 10 : Ghost & Flare
	EqpUI_Distortion,		// 11
	EqpUI_EEPROM,			// 12 : 예약

	Max_EqpTypeUICount,
}enEquipmentType_UI;

static LPCTSTR g_szEqpTypeName_UI[] =
{
	_T("Handler"),
	_T("Loader Machine"),
	_T("Returner"),

	_T("Color Calibration"),
	_T("SFR CL"),			// 협각
	_T("SFR Multi CL"),		// 광각 83 & 180
	_T("Hot Pixel"),
	_T("Stray Light"),
	_T("Intrinsic Cal."),
	_T("EEPROM"),
	nullptr
};

static LPCTSTR g_szEqpTypeUI_Initial[] =
{
	_T("Hlr"),
	_T("Loa"),
	_T("Ret"),
	_T("CCa"),
	_T("SCL"),
	_T("SMC"),
	_T("Hot"),
	_T("GnF"),
	_T("Cal"),
	_T("E2P"),
	nullptr
};

//-------------------------------------------------------------------
// 검사 설비 타입
//-------------------------------------------------------------------
typedef enum
{
	Tester_ColorCal,		// 0
	Tester_SFR_CL,			// 1 : 협각
	Tester_SFR_MultiCL,		// 4 : 광각
	Tester_HotPixel,		// 5
	Tester_StrayLight,		// 7 Ghost & Flare
	Tester_Distortion,		// 8
	Tester_EEPROM,
	Max_TesterCount,
}enTesterType;

static LPCTSTR g_szTesterTypeName[] =
{
	_T("Color Calibration"),
	_T("SFR CL"),			// 협각
	_T("SFR Multi CL"),		// 광각
	_T("Hot Pixel"),
	_T("Stray Light"),
	_T("Intrinsic Cal."),
	_T("EEPROM"),
	nullptr
};

static LPCTSTR g_szTesterTypeInitial[] =
{
	_T("CCa"),
	_T("SCL"),
	_T("SMC"),
	_T("Hot"),
	_T("GnF"),
	_T("Cal"),
	_T("E2P"),
	nullptr
};

// ==================================================================
#else // 기본 Trinity Line 
// ==================================================================

// 설비 타입 : version 1.0 (호환성 유지 위해 사용).
typedef enum
{
	Eqp_V1_Loader,			// 0
	Eqp_V1_Returner,		// 1
	Eqp_V1_ColorCal,		// 2 
	Eqp_V1_SFR_CL_46,		// 3 : SFR 협각
	Eqp_V1_SFR_MultiCL_83,	// 4 : 83
	Eqp_V1_SFR_MultiCL_180,	// 5 : 180
	Eqp_V1_HotPixel,		// 6
	Eqp_V1_StrayLight,		// 7 Ghost & Flare
	Eqp_V1_Distortion,		// 8
	Eqp_V1_SFR_MultiCL,		// 9 : SFR 광각
	Eqp_V1_HotPixel3port,	// 10 : Hot Pixel 3 Para
#if defined(EES_XML)//20231003
	Eqp_V1_ServerEes,		// 11 : Server_EES
#endif
	Max_EqpV1TypeCount,
}enEquipmentType_ver_1;



//-------------------------------------------------------------------
// 설비 타입 : version 2.0
//-------------------------------------------------------------------
typedef enum
{	
	Eqp_Handler,		// 0
	Eqp_Loader,			// 1
	Eqp_Returner,		// 2

	Eqp_ColorCal,		// 3 
	Eqp_SFR_CL_46,		// 4 : SFR 협각
	Eqp_SFR_MultiCL_83,	// 5 : 83
	Eqp_SFR_MultiCL_180,// 6 : 180
	Eqp_SFR_MultiCL,	// 7 : SFR 광각
	Eqp_HotPixel,		// 8
	Eqp_StrayLight,		// 9 Ghost & Flare
	Eqp_Distortion,		// 10
	Eqp_EEPROM,			// 11 : 예약

	Max_EqpTypeCount,
	Eqp_Tester_First	= Eqp_ColorCal,
	Eqp_Tester_Last		= Eqp_EEPROM,

}enEquipmentType;

static LPCTSTR g_szEqpTypeName[] =
{
	_T("Handler"),
	_T("Loader Machine"),
	_T("Returner"),

	_T("Color Calibration"),
	_T("SFR CL 46"),		// 협각
	_T("SFR Multi CL 83"),	// 광각 83
	_T("SFR Multi CL 180"),	// 광각 180
	_T("SFR Multi CL"),		// 광각 83 & 180
	_T("Hot Pixel"),
	_T("Stray Light"),
	_T("Distortion"),
	_T("EEPROM"),
	nullptr
};

static LPCTSTR g_szEqpTypeInitial[] =
{
	_T("Hlr"),
	_T("Loa"),
	_T("Ret"),
	_T("CCa"),
	_T("S46"),
	_T("S83"),
	_T("S180"),
	_T("SMC"),
	_T("Hot"),
	_T("GnF"),
	_T("Dis"),
	_T("E2P"),
	nullptr
};

//-------------------------------------------------------------------
// UI 설정 및 상세 설정용 설비 타입
//-------------------------------------------------------------------
typedef enum
{	
	EqpUI_Handler,			//  0
	EqpUI_Loader,			//  1
	EqpUI_Returner,			//  2
							    
	EqpUI_ColorCal,			//  3 
	EqpUI_SFR_CL_46,		//  4 : SFR 협각
	EqpUI_SFR_MultiCL_83,	//  5 : 83
	EqpUI_SFR_MultiCL_180,	//  6 : 180
	EqpUI_SFR_MultiCL,		//  7 : SFR 광각
	EqpUI_HotPixel,			//  8
	EqpUI_HotPixel3port,	//  9 : Hot Pixel 3 Para
	EqpUI_StrayLight,		// 10 : Ghost & Flare
	EqpUI_Distortion,		// 11
	EqpUI_EEPROM,			// 12 : 예약
#if defined (EES_XML)//20231001
	EqpUI_Ees,				// 13 : Server_EES	
#endif
	Max_EqpTypeUICount,
}enEquipmentType_UI;

static LPCTSTR g_szEqpTypeName_UI[] =
{
	_T("Handler"),
	_T("Loader Machine"),
	_T("Returner"),

	_T("Color Calibration"),
	_T("SFR CL 46"),		// 협각
	_T("SFR Multi CL 83"),	// 광각 83
	_T("SFR Multi CL 180"),	// 광각 180
	_T("SFR Multi CL"),		// 광각 83 & 180
	_T("Hot Pixel"),
	_T("Hop Pixel 3 Para"),
	_T("Stray Light"),
	_T("Distortion"),
	_T("EEPROM"),
	nullptr
};

static LPCTSTR g_szEqpTypeUI_Initial[] =
{
	_T("Hlr"),
	_T("Loa"),
	_T("Ret"),
	_T("CCa"),
	_T("S46"),
	_T("S83"),
	_T("S180"),
	_T("SMC"),
	_T("Hot"),
	_T("HP3"),
	_T("GnF"),
	_T("Dis"),
	_T("E2P"),
	nullptr
};

//-------------------------------------------------------------------
// 검사 설비 타입
//-------------------------------------------------------------------
typedef enum
{
	Tester_ColorCal,		// 0
	Tester_SFR_CL_46,		// 1 : 협각
	Tester_SFR_MultiCL_83,	// 2 : 광각
	Tester_SFR_MultiCL_180,	// 3 : 광각
	Tester_SFR_MultiCL,		// 4 : 광각
	Tester_HotPixel,		// 5
	Tester_StrayLight,		// 7 Ghost & Flare
	Tester_Distortion,		// 8
	Tester_EEPROM,
	Max_TesterCount,
}enTesterType;

static LPCTSTR g_szTesterTypeName[] =
{
	_T("Color Calibration"),
	_T("SFR CL 46"),		// 협각
	_T("SFR Multi CL 83"),// 광각 83
	_T("SFR Multi CL 180"),	// 광각 180
	_T("SFR Multi CL"),// 광각 83 & 180
	_T("Hot Pixel"),
	_T("Stray Light"),
	_T("Distortion"),
	_T("EEPROM"),
	nullptr
};

static LPCTSTR g_szTesterTypeInitial[] =
{
	_T("CCa"),
	_T("S46"),
	_T("S83"),
	_T("S180"),
	_T("SMC"),
	_T("Hot"),
	_T("GnF"),
	_T("Dis"),
	_T("E2P"),
	nullptr
};

#ifdef 	NEW_INSPECTION_TYPE_APPLY
//-------------------------------------------------------------------
// 검사 종류
//-------------------------------------------------------------------
typedef enum
{
	InspType_NotInspection = 0,// 0
	InspType_ColorCal,			// 1
	InspType_SFR,				// 2
	InspType_HotPixel,			// 3
	InspType_StrayLight,		// 4
	InspType_Distortion,		// 5
	InspType_EEPROM,			// 6
	Max_InspTypeCount,
}enInspectionType;

static LPCTSTR g_szInspectionTypeName[] =
{
	_T("Not Inspection"),
	_T("Color Calibration"),
	_T("SFR"),
	_T("Hot Pixel"),
	_T("Stray Light"),
	_T("Distortion"),
	_T("EEPROM"),
	nullptr
};
#endif	// NEW_INSPECTION_TYPE_APPLY

// Equipment id  구조
// 1. Line Name
// 2. Line 번호
// 3. Equipment Name
// 4. Equipment 번호
// ex) TLine_01_Loader_01


#ifdef 	NEW_INSPECTION_TYPE_APPLY
static int8_t ConvTo_InspectionType(__in enEquipmentType IN_nEqpType)
{

	switch (IN_nEqpType)
	{
	case Eqp_ColorCal:
		return static_cast<int8_t>(InspType_ColorCal);
		break;

	case Eqp_SFR_CL_46:
	case Eqp_SFR_MultiCL_83:
	case Eqp_SFR_MultiCL_180:
	case Eqp_SFR_MultiCL:
		return static_cast<int8_t>(InspType_SFR);
		break;

	case Eqp_HotPixel:
	//case Eqp_HotPixel3port:
		return static_cast<int8_t>(InspType_HotPixel);
		break;

	case Eqp_StrayLight:
		return static_cast<int8_t>(InspType_StrayLight);
		break;

	case Eqp_Distortion:
		return static_cast<int8_t>(InspType_Distortion);
		break;

	case Eqp_EEPROM:
		return static_cast<int8_t>(InspType_EEPROM);
		break;

	case Eqp_Handler:
	case Eqp_Loader:
	case Eqp_Returner:
	default:
		break;
	}


	return  static_cast<int8_t>(IN_nEqpType - Eqp_Tester_First);
};
#endif	// NEW_INSPECTION_TYPE_APPLY

//===================================================================
#endif // (SET_INSPECTOR == SYS_ICS_RIVIAN_LINE)
//===================================================================

static int8_t ConvTo_TesterType(__in enEquipmentType IN_nEqpType)
{
	return  static_cast<int8_t>(IN_nEqpType - Eqp_Tester_First);
};

static int8_t ConvTo_EquipmentType(__in enTesterType IN_nTesterType)
{
	return  static_cast<int8_t>(IN_nTesterType + Eqp_Tester_First);
};

static bool IsTester_byEqpType(__in uint8_t IN_nEqpType)
{
	return ((Eqp_Tester_First <= IN_nEqpType) && (IN_nEqpType <= Eqp_Tester_Last));
};







#endif // Def_Equipment_Type_h__
