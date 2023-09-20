//*****************************************************************************
// Filename	: 	Wnd_Status_EquipmentEx.h
// Created	:	2016/3/14 - 10:57
// Modified	:	2016/3/14 - 10:57
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
#ifndef Wnd_Status_EquipmentEx_h__
#define Wnd_Status_EquipmentEx_h__

#pragma once
#include <stdint.h>
#include "Wnd_Status_Equipment.h"


//-----------------------------------------------------------------------------
// CWnd_Status_EquipmentEx
//-----------------------------------------------------------------------------
class CWnd_Status_EquipmentEx : public CWnd_Status_Equipment
{
	DECLARE_DYNAMIC(CWnd_Status_EquipmentEx)

public:
	CWnd_Status_EquipmentEx();
	virtual ~CWnd_Status_EquipmentEx();

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg int		OnCreate				(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnSize					(UINT nType, int cx, int cy);
	
	virtual void	MoveWindow_Status		(int x, int y, int nWidth, int nHeight);
	virtual void	MoveWindow_Tester		(int x, int y, int nWidth, int nHeight);
	virtual void	MoveWindow_Loader		(int x, int y, int nWidth, int nHeight);
	virtual void	MoveWindow_Unloader		(int x, int y, int nWidth, int nHeight);
	virtual void	MoveWindow_Returner		(int x, int y, int nWidth, int nHeight);


	// 소켓지그 UI

	// 수율 UI



//	virtual void Popup_SubMenu();

// 	void SetUI_PortStatus		(__in CVGStatic* IN_pwndPort, __in uint8_t IN_nStatus);
// 	void SetUI_PortStatus		(__in CVGStatic* IN_pwndPort, __in const ST_PortStatus* IN_nStatus);
// 	void SetUI_ConveyorStatus	(__in CVGStatic* IN_pwndConveyor, __in uint8_t IN_nStatus, __in uint8_t IN_nExistSocket);
// 	void SetUI_ConveyorStatus	(__in CVGStatic* IN_pwndConveyor, __in const ST_ConveyorStatus* IN_nStatus);

public:

	
	


	// 설비내 소켓 정보
	// buffer, test 1, tese 2, 컨베이어, 리턴 컨베이어

	virtual void Set_PortClear		(__in uint8_t IN_nPortIndex);
	virtual void Set_PortStatus		(__in uint8_t IN_nPortIndex, __in uint8_t IN_nStatus);
	virtual void Set_PortStatus		(__in uint8_t IN_nPortIndex, __in const ST_PortStatus* IN_nStatus);
	virtual void Set_ConveyorStatus	(__in uint8_t IN_nConveyorIndex, __in uint8_t IN_nStatus, __in uint8_t IN_nExistSocket);
	virtual void Set_ConveyorStatus	(__in uint8_t IN_nConveyorIndex, __in const ST_ConveyorStatus* IN_nStatus);
	

};

#endif // Wnd_Status_EquipmentEx_h__


