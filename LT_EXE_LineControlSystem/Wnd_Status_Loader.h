//*****************************************************************************
// Filename	: 	Wnd_Status_Loader.h
// Created	:	2016/3/14 - 10:57
// Modified	:	2016/3/14 - 10:57
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
#ifndef Wnd_Status_Loader_h__
#define Wnd_Status_Loader_h__

#pragma once
#include <stdint.h>
#include "Def_Constant.h"
#include "Wnd_Status_Equipment.h"

#define		CUI_Equipment			CWnd_Status_Equipment
#define		LOADER_BUFFER_CNT		3

//-----------------------------------------------------------------------------
// CWnd_Status_Loader
//-----------------------------------------------------------------------------
class CWnd_Status_Loader : public CUI_Equipment
{
	DECLARE_DYNAMIC(CWnd_Status_Loader)

public:
	CWnd_Status_Loader();
	virtual ~CWnd_Status_Loader();

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg int		OnCreate				(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnSize					(UINT nType, int cx, int cy);
	
	void			MoveWindow_Loader		(int x, int y, int nWidth, int nHeight);

	// RFID Buffer > NG > Pass > Clean >
	// CM투입 > Buffer > Transfer > Buffer > Buffer > Buffer
	CVGStatic		m_st_Port[PtI_L_MaxCount];
	CVGStatic		m_st_Conveyor[CvI_L_MaxCount];


	virtual void Init_Loader();

	virtual void Popup_SubMenu();

public:

// 	PtI_L_Load,
// 	PtI_L_RFID,
// 	PtI_L_Transfer,
// 	PtI_L_Buffer_1,
// 	PtI_L_Buffer_2,
// 	PtI_L_Buffer_3,
// 	PtI_L_Un_RFID,
// 	PtI_L_Un_NG,
// 	PtI_L_Un_OK_Wait,
// 	PtI_L_Un_OK,
// 	PtI_L_Un_Clean,

// 	CvI_L_Load,
// 	CvI_L_RFID,
// 	CvI_L_Transfer,
// 	CvI_L_Buffer_1,
// 	CvI_L_Buffer_2,
// 	CvI_L_Buffer_3,
// 	CvI_L_Un_RFID,
// 	CvI_L_Un_NG,
// 	CvI_L_Un_OK,
// 	CvI_L_Un_Clean,

	virtual void Set_PortClear		(__in uint8_t IN_nPortIndex);
	virtual void Set_PortStatus		(__in uint8_t IN_nPortIndex, __in uint8_t IN_nStatus);
	virtual void Set_PortStatus		(__in uint8_t IN_nPortIndex, __in const ST_PortStatus* IN_nStatus);
	virtual void Set_ConveyorStatus	(__in uint8_t IN_nConveyorIndex, __in uint8_t IN_nStatus, __in uint8_t IN_nExistSocket);
	virtual void Set_ConveyorStatus	(__in uint8_t IN_nConveyorIndex, __in const ST_ConveyorStatus* IN_nStatus);
};

#endif // Wnd_Status_Loader_h__


