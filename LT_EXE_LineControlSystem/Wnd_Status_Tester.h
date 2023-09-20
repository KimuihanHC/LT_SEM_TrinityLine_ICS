//*****************************************************************************
// Filename	: 	Wnd_Status_Tester.h
// Created	:	2021/12/16 - 17:58
// Modified	:	2021/12/16 - 17:58
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#ifndef Wnd_Status_Tester_h__
#define Wnd_Status_Tester_h__

#pragma once
#include <stdint.h>
#include "Def_Constant.h"
#include "Wnd_Status_Equipment.h"

#define CUI_Equipment	CWnd_Status_Equipment

//-----------------------------------------------------------------------------
// CWnd_Status_Tester
//-----------------------------------------------------------------------------
class CWnd_Status_Tester : public CUI_Equipment
{
	DECLARE_DYNAMIC(CWnd_Status_Tester)

public:
	CWnd_Status_Tester();
	virtual ~CWnd_Status_Tester();

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg int		OnCreate				(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnSize					(UINT nType, int cx, int cy);
	
	void			MoveWindow_Tester		(int x, int y, int nWidth, int nHeight);

	enum enTestZone
	{
		Zone_L,
		Zone_R,
		Zone_C,

		Zone_MaxCount,
	};

	// 검사 Zone 상태 L / R / C
	//CVGStatic		m_st_Zone_L;
	//CVGStatic		m_st_Zone_R;
	CVGStatic		m_st_Zone[Zone_MaxCount];
	
	CVGStatic		m_st_Port[PtI_T_MaxCount];
	CVGStatic		m_st_Conveyor[CvI_T_MaxCount];



	virtual void Init_Tester();

	virtual void Popup_SubMenu();

public:

	// 설비를 사용 유/무 선택
	virtual void Set_EnableEquipment	(__in bool IN_bEnable);

	virtual void Set_Skip				(__in bool IN_bSkip);

	// 설비내 소켓 정보
	// buffer, test 1, tese 2, 컨베이어, 리턴 컨베이어


// 	PtI_T_RFID,
// 	PtI_T_Buffer,
// 	PtI_T_Test_L,
// 	PtI_T_Test_R,

// 	CvI_T_RFID,
// 	CvI_T_Test,
// 	CvI_T_Return,

	virtual void Set_PortClear		(__in uint8_t IN_nPortIndex);
	virtual void Set_PortStatus		(__in uint8_t IN_nPortIndex, __in uint8_t IN_nStatus);
	virtual void Set_PortStatus		(__in uint8_t IN_nPortIndex, __in const ST_PortStatus* IN_nStatus);
	virtual void Set_ConveyorStatus	(__in uint8_t IN_nConveyorIndex, __in uint8_t IN_nStatus, __in uint8_t IN_nExistSocket);
	virtual void Set_ConveyorStatus	(__in uint8_t IN_nConveyorIndex, __in const ST_ConveyorStatus* IN_nStatus);

	//=============================================================================
	// Add
	//=============================================================================
#if (USE_XML)
public:
	virtual void	Set_EqpNotify_EQUIPMENTSTATE(__in lt::Report_Equipment_State_Args::Args * IN_DATA);
#endif
#if ADD_SOCKET_EES_XML
public:
	//COLORREF			Get_Color(__in CString lParam);

	virtual void	Set_EqpNotify_EQUIPMENTSTATEDISPLAY	(__in lt::Reply_Equipment_State_Display_Args::Args& IN_DATA);
	virtual void	Set_EqpNotify_RGBDISPLAY			(__in  lt::Request_Equipment_State_Display_Args::Args&  IN_DATA);
	//2023.05.25a uhkim
	//virtual void	SetPtr_EquipmentInfo(__in uint8_t IN_nEqpNo, const CEquipment* IN_pEquipment);
#endif	//ADD_SOCKET_EES_XML
#if (USE_XML) 
protected:
	CVGStatic		m_st_SubEqpID;
	CVGStatic		m_st_EquipmentID[PtI_T_MaxCount];
	CVGStatic		m_st_EquipMentState[PtI_T_MaxCount];// IDLE,	
	CVGStatic		m_st_EquipMentStateDisplay[PtI_T_MaxCount];// IDLE,
public:
	virtual void	Set_EquipmentID(__in CEquipment* IN_Data);
#endif
};

#endif // Wnd_Status_Tester_h__


