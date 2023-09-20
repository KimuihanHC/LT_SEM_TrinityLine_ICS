//*****************************************************************************
// Filename	: 	Wnd_Status_Tester.h
// Created	:	2021/12/16 - 17:58
// Modified	:	2021/12/16 - 17:58
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#ifndef Wnd_Status_EES_h__
#define Wnd_Status_EES_h__

#pragma once
#include <stdint.h>
#include "Def_Constant.h"
#include "Wnd_Status_Server.h"

#define CUI_Server	CWnd_Status_Server

//-----------------------------------------------------------------------------
// CWnd_Status_Tester
//-----------------------------------------------------------------------------
class CWnd_Status_EES : public CUI_Server
{
	DECLARE_DYNAMIC(CWnd_Status_EES)

public:
	CWnd_Status_EES();
	virtual ~CWnd_Status_EES();

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg int		OnCreate				(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnSize					(UINT nType, int cx, int cy);

	void			MoveWindow_EES			(int x, int y, int nWidth, int nHeight);

	enum enEES
	{
		EES_EquipMentStateDisplay,
		//EES_EquipMentNameKor,
		//EES_EquipMentNameEng,
		//EES_LossCode,
		EES_StateColor,
		//EES_Use,
		//EES_Explanation,

		EES_MaxCount,
	};

	virtual void Init_Tester();

	virtual void Popup_SubMenu_EESMode();
	virtual void Popup_SubMenu_LossWindow();

public:
#if ADD_SOCKET_EES_XML
	virtual void Set_VerifyRequestLinkTest(__in lt::Request_Link_Test_Args::Args & lParam);
	virtual void Set_VerifyRequestUserCommand(__in lt::Request_User_Cpmmand_Args::Args & lParam);
	virtual void Set_VerifyRequestEquipmentStateDisplay(__in lt::Request_Equipment_State_Display_Args::Args & lParam);
	virtual void Set_VerifyRequestLossWindow(__in lt::Request_Loss_Window_Args::Args & lParam);
	virtual void Set_VerifyRequestAlarmList(__in lt::Request_Alarm_List_Args::Args & lParam);
	virtual void Set_VerifyRequestSetDateTime(__in lt::Request_Set_DateTime_Args::Args & lParam);
	virtual void Set_VerifyRequestTerminalMessage(__in lt::Request_Terminal_Message_Args::Args & lParam);
	virtual void Set_VerifyRequestOpCall(__in lt::Request_Opcall_Args::Args & lParam);

	virtual void SetUI_EquipMentStateDisplay(__in CVGStatic* IN_pwndPort, __in uint8_t IN_nStatus);
#endif	//ADD_SOCKET_EES_XML
	//=============================================================================
	// Add
	//=============================================================================

public:
	virtual void	Set_EquipmentID(__in CServer* IN_Data);

protected:
	CVGStatic		m_st_SubEqpID;
	CVGStatic		m_st_EquipmentID[PtI_EES_MaxCount];
	CVGStatic		m_st_EquipMentState[PtI_EES_MaxCount];// IDLE,	
	CVGStatic		m_st_EquipMentStateDisplay[PtI_EES_MaxCount];// IDLE,
#if ADD_SOCKET_EES_XML
public:
	virtual void	Set_SvrNotify_EQUIPMENTSTATE(__in lt::Report_Equipment_State_Args::Args& IN_DATA);
	virtual void	Set_SvrNotify_EQUIPMENTSTATEDISPLAY(__in lt::Reply_Equipment_State_Display_Args::Args&  IN_DATA);
#endif	//ADD_SOCKET_EES_XML
};

#endif // Wnd_Status_Tester_h__


