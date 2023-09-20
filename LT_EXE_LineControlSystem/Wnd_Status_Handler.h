//*****************************************************************************
// Filename	: 	Wnd_Status_Handler.h
// Created	:	2021/12/16 - 17:58
// Modified	:	2021/12/16 - 17:58
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#ifndef Wnd_Status_Handler_h__
#define Wnd_Status_Handler_h__

#pragma once
#include <stdint.h>
#include "Def_Constant.h"
#include "Wnd_Status_Equipment.h"

//-----------------------------------------------------------------------------
// CWnd_Status_Handler
//-----------------------------------------------------------------------------
class CWnd_Status_Handler : public CWnd_Status_Equipment
{
	DECLARE_DYNAMIC(CWnd_Status_Handler)

public:
	CWnd_Status_Handler();
	virtual ~CWnd_Status_Handler();

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg int		OnCreate				(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnSize					(UINT nType, int cx, int cy);
	
	void			MoveWindow_Handler		(int x, int y, int nWidth, int nHeight);

	
	CVGStatic		m_st_Port[PtI_R_MaxCount];
	CVGStatic		m_st_Conveyor[CvI_R_MaxCount];


	virtual void Init_Returner();

	virtual void Popup_SubMenu();

public:
#if (USE_XML)
//=============================================================================
// Add
//=============================================================================
protected:
	CVGStatic		m_st_SubEqpID;
	CVGStatic		m_st_EquipmentID[PtI_L_MaxCount];
public:
	virtual void	Set_EquipmentID(__in CEquipment* IN_Data);
#endif // SOCKET
};

#endif // Wnd_Status_Handler_h__


