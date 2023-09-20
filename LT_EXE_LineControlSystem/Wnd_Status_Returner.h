//*****************************************************************************
// Filename	: 	Wnd_Status_Returner.h
// Created	:	2021/12/16 - 17:58
// Modified	:	2021/12/16 - 17:58
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#ifndef Wnd_Status_Returner_h__
#define Wnd_Status_Returner_h__

#pragma once
#include <stdint.h>
#include "Def_Constant.h"
#include "Wnd_Status_Equipment.h"

#define CUI_Equipment	CWnd_Status_Equipment

//-----------------------------------------------------------------------------
// CWnd_Status_Returner
//-----------------------------------------------------------------------------
class CWnd_Status_Returner : public CUI_Equipment
{
	DECLARE_DYNAMIC(CWnd_Status_Returner)

public:
	CWnd_Status_Returner();
	virtual ~CWnd_Status_Returner();

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg int		OnCreate				(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnSize					(UINT nType, int cx, int cy);
	
	void			MoveWindow_Returner		(int x, int y, int nWidth, int nHeight);

	
	CVGStatic		m_st_Port[PtI_R_MaxCount];
	CVGStatic		m_st_Conveyor[CvI_R_MaxCount];


	virtual void Init_Returner();

	virtual void Popup_SubMenu();

public:


	// 설비내 소켓 정보
	// buffer, test 1, tese 2, 컨베이어, 리턴 컨베이어


// 		PtI_R_Buffer_1,
// 		PtI_R_Transfer,
// 		PtI_R_Buffer_2,

// 		CvI_R_Buffer_1,
// 		CvI_R_Transfer,
// 		CvI_R_Buffer_2,

	virtual void Set_PortClear		(__in uint8_t IN_nPortIndex);
	virtual void Set_PortStatus		(__in uint8_t IN_nPortIndex, __in uint8_t IN_nStatus);
	virtual void Set_PortStatus		(__in uint8_t IN_nPortIndex, __in const ST_PortStatus* IN_nStatus);
	virtual void Set_ConveyorStatus	(__in uint8_t IN_nConveyorIndex, __in uint8_t IN_nStatus, __in uint8_t IN_nExistSocket);
	virtual void Set_ConveyorStatus	(__in uint8_t IN_nConveyorIndex, __in const ST_ConveyorStatus* IN_nStatus);
};

#endif // Wnd_Status_Returner_h__


