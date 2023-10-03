//*****************************************************************************
// Filename	: 	View_MainCtrl_ICS.h
// Created	:	2021/1/27 - 13:55
// Modified	:	2021/1/27 - 13:55
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#ifndef View_MainCtrl_ICS_h__
#define View_MainCtrl_ICS_h__


#pragma once

#include "View_MainCtrl.h"
#include "TestManager_EQP_ICS.h"


//=============================================================================
// CView_MainCtrl_ICS
//=============================================================================
class CView_MainCtrl_ICS : public CView_MainCtrl, public CTestManager_EQP_ICS
{
public:
	CView_MainCtrl_ICS();
	virtual ~CView_MainCtrl_ICS();

protected:
	afx_msg int		OnCreate		(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnSize			(UINT nType, int cx, int cy);

	// ë¡œê·¸ ë©”ì„¸ì§€
	afx_msg	LRESULT	OnWM_LogMsg					(WPARAM wParam, LPARAM lParam);

	// °Ë»ç Á¦¾î
	afx_msg	LRESULT	OnWM_SwitchPermissionMode	(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnWM_ChangeRecipe			(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnWM_ChangedSetting			(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnWM_EqpCtrlCmd				(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnWM_TestSelect				(WPARAM wParam, LPARAM lParam);
	
	afx_msg LRESULT	OnWM_Eqp_Connection			(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnWM_Eqp_AutoMode			(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnWM_Eqp_Alarm				(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnWM_Eqp_PortStatus			(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnWM_Eqp_ConveyorStatus		(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnWM_Eqp_EndOfProduction	(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnWM_Eqp_CheckEmptyEqp		(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT	OnWM_Loader_RegisterSocket	(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnWM_Loader_Check_TrackOut	(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnWM_Tester_TrackIn			(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnWM_Tester_EndInspection	(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnWM_Unload_ReqTestResult	(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnWM_Unload_NG_Info			(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnWM_Unload_UnregisterSocket(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT	OnWM_Update_SocketData		(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnWM_Update_SocketData_All	(WPARAM wParam, LPARAM lParam);

#if (USE_XML)
	afx_msg LRESULT	OnWM_Svr_Connection						(WPARAM wParam, LPARAM lParam);	
	afx_msg LRESULT OnWM_Svr_REQUEST_LINK_TEST				(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnWM_Svr_REPLY_LINK_TEST				(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnWM_Svr_REPORT_ONLINE_STATE			(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnWM_Svr_REPORT_USER_CHANGE				(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnWM_Svr_REQUEST_USER_CHANGE			(WPARAM wParam, LPARAM lParam);	
	afx_msg LRESULT	OnWM_Svr_REPLY_USER_COMMAND				(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnWM_Svr_REPORT_EQUIPMENT_STATE			(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnWM_Svr_REQUEST_EQUIPMENT_STATE_DISPLAY(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnWM_Svr_REPLY_EQUIPMENT_STATE_DISPLAY	(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnWM_Svr_REPORT_LOSS_STATE				(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnWM_Svr_REQUEST_LOSS_WINDOW			(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnWM_Svr_REPLY_LOSS_WINDOW				(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWM_Svr_REPORT_ALARM_STATE				(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWM_Svr_REQUEST_ALARM_LIST				(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWM_Svr_REPLY_ALARM_LIST				(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWM_Svr_REPORT_RMS_MODE				(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWM_Svr_REQUEST_SET_DATETIME			(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWM_Svr_REPLY_SET_DATETIME				(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWM_Svr_REQUEST_TERMINAL_MESSAGE		(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWM_Svr_REPLY_TERMINAL_MESSAGE			(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWM_Svr_REQUEST_OPCALL_MESSAGE			(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWM_Svr_REPLY_OPCALL					(WPARAM wParam, LPARAM lParam);
	//=============================================================================
	// WM_EVENT_EQUIPMENT 2023.04.11a uhkim
	//=============================================================================
	afx_msg LRESULT	OnWM_Eqp_REPORT_ONLINE_STATE(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnWM_Eqp_REPORT_USER_CHANGE(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnWM_Eqp_REPLY_USER_COMMAND(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnWM_Eqp_REPORT_EQUIPMENT_STATE(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnWM_Eqp_REPLY_EQUIPMENT_STATE_DISPLAY(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnWM_Eqp_REPORT_LOSS_STATE(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnWM_Eqp_REPLY_LOSS_WINDOW(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWM_Eqp_REPORT_ALARM_STATE(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWM_Eqp_REPORT_RMS_MODE(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWM_Eqp_REPLY_OPCALL(WPARAM wParam, LPARAM lParam);
#endif	//TEST
#if TEST
	afx_msg LRESULT OnWM_Svr_UNITID_READ					(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWM_Svr_REQUEST_UNITID_READ			(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnWM_Svr_REPLY_UNITID_READ				(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnWM_Svr_REPORT_START_PROCESS			(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnWM_Svr_REPORT_START_LOT				(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWM_Svr_REPORT_END_EVENT				(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnWM_Svr_REPORT_END_PROCESS				(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnWM_Eqp_UNITID_READ					(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnWM_Eqp_REPLY_UNITID_READ				(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWM_Eqp_REPORT_START_PROCESS			(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnWM_Eqp_REPORT_START_LOT				(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWM_Eqp_REPORT_END_EVENT				(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnWM_Eqp_REPORT_END_PROCESS				(WPARAM wParam, LPARAM lParam);
#endif	//TEST
	DECLARE_MESSAGE_MAP()

	//=========================================================================
	// CView_MainCtrl ÀçÁ¤ÀÇ
	//=========================================================================
	//-----------------------------------------------------
	// ÃÊ±â ¼³Á¤ °ü·Ã
	//-----------------------------------------------------
	// »ý¼ºÀÚ¿¡¼­ ÃÊ±âÈ­ ÇÒ ¼¼ÆÃ
	virtual void	OnInit_ConstructionSetting();
	// Window »ý¼º ÈÄ ¼¼ÆÃ
	virtual void	OnInit_UISetting();
	// ÁÖº¯ÀåÄ¡µé ±âº» ¼³Á¤
	virtual void	OnInit_DeviceSetting();

	//-------------------------------------------------------------------------
	// ÆÄÀÏ ºÒ·¯¿À±â ¹× ¼¼ÆÃ
	bool			OnLoad_LineInfo					(__in bool IN_bNotifySettingWnd = true);
	bool			OnLoad_SocketInfo				(__in bool IN_bNotifySettingWnd = true);
	bool			OnLoad_UserInfo					(__in bool IN_bNotifySettingWnd = true);
	//bool			OnLoad_AlarmInfo				(__in bool IN_bNotifySettingWnd = true);
	bool			OnLoad_NGCodeInfo				(__in bool IN_bNotifySettingWnd = true);
	bool			OnLoad_DebugInfo				(__in bool IN_bNotifySettingWnd = true);
	bool			OnLoad_ModelInfo				();
	bool			OnLoad_FailInfo					();

	bool			OnLoad_Prev_LineInfo			();
	bool			OnLoad_Prev_LineInfo_OnlyYield	();

	// ¼³ºñ µ¥ÀÌÅÍ, Åë½Å, GUI ¸ÅÄª
	virtual void	OnMatchingEquipment				();
	bool			Compare_LineConfig_ServRestart	(__in CConfig_Line* IN_Old, __in CConfig_Line* IN_New);

	//=========================================================================
	// CTestManager_EQP_ICS ÀçÁ¤ÀÇ
	//=========================================================================
	//-----------------------------------------------------
	// ·Î±×/ÆÄÀÏ Ã³¸®
	//-----------------------------------------------------
	virtual void	OnInit_LogFolder				();

	//-----------------------------------------------------
	// UI ¾÷µ¥ÀÌÆ®
	//-----------------------------------------------------
	virtual void	OnSetStatus_Production			(__in bool IN_bEnd);

	//-----------------------------------------------------
	// °á°ú µ¥ÀÌÅÍ Ã³¸®
	//-----------------------------------------------------
	virtual void	OnLog_EquipmentAlarm			(__in uint8_t IN_nEqpOder, __in const ST_AlarmStatus* IN_pAlarm);
	virtual void	OnLog_RegisterSocket			(__in LPCTSTR IN_szRFID);

	// Á¦Ç° °á°ú È­¸é¿¡ Ç¥½Ã
	virtual void	OnReport_Socket					(__in LPCTSTR IN_szRFID);
	virtual void	OnReport_Socket_NG				(__in LPCTSTR IN_szRFID);

	// °Ë»ç ÁøÇà ½Ã°£ Ã¼Å© (¼³ºñ, ÆÄ¶ó L/R)
	virtual void	OnUpdate_ElapsedTime_Unit		(__in UINT nEquipmentIdx, __in uint8_t IN_nPara);
	virtual void	OnUpdate_ElapsedTime_All		();

public: 
	//=========================================================================
	// CView_MainCtrl ÀçÁ¤ÀÇ
	//=========================================================================
	// Â÷ÀÏµå À©µµ¿ì ÀüÈ¯ ½Ã »ç¿ë
	virtual UINT	SwitchWindow				(__in UINT nIndex);
	// ÀåÄ¡ Åë½Å »óÅÂ Ç¥½Ã À©µµ¿ì Æ÷ÀÎÅÍ ¼³Á¤
	virtual void	SetCommPanePtr				(__in CWnd* pwndCommPane);

	// ¿É¼ÇÀÌ º¯°æ µÇ¾úÀ» °æ¿ì ´Ù½Ã UI³ª µ¥ÀÌÅÍ¸¦ ¼¼ÆÃÇÏ±â À§ÇÑ ÇÔ¼ö
	virtual void	ReloadOption				();

	// ÇÁ·Î±×·¥ ·Îµù ³¡³­ ÈÄ ÀÚµ¿ Ã³¸®¸¦ À§ÇÑ ÇÔ¼ö
	virtual void	InitStartProgress			();	
	virtual BOOL	InitStartDeviceProgress		();	

	// ÇÁ·Î±×·¥ Á¾·á½Ã Ã³¸®ÇØ¾ß ÇÒ ±â´ÉµéÀ» Ã³¸®ÇÏ´Â ÇÔ¼ö
	virtual void	FinalExitProgress			();

	//=========================================================================
	// CTestManager_EQP_ICS ÀçÁ¤ÀÇ
	//=========================================================================
	// °Ë»ç±â Á¾·ù ¼³Á¤
	virtual void	OnSet_SystemType			(__in enInsptrSysType nSysType);

	// ·Î±× ¸Þ¼¼Áö Ã³¸®¿ë ÇÔ¼ö
	virtual void	OnAdd_Log					(__in LPCTSTR lpszLog, __in BOOL bError = FALSE, __in UINT nLogType = LOGTYPE_NONE, __in BOOL bOnlyLogType = FALSE);
	virtual void	OnAdd_Log					(__in LPCTSTR lpszLog, __in UINT nLogType = LOGTYPE_NONE);

	// Á¦¾î ±ÇÇÑ ¼³Á¤
	virtual void	OnInit_PermissionMode		(__in enPermissionMode nAcessMode);	
	virtual void	OnSet_PermissionMode		(__in enPermissionMode nAcessMode);	

	void			Proc_LineCtrlCmd			(__in uint8_t IN_nCmdIndex);
	void			Proc_EquipmentCtrlCmd		(__in uint8_t IN_nCmdIndex, __in uint8_t IN_nEqpOrder);

	//=========================================================================
	// TEST ì½”ë“œ
	//=========================================================================
	virtual void	Test_Process				(__in UINT nTestNo);	
	//--------------------- TEST --------------------------

#if (USE_XML)
protected:
	bool			OnLoad_ServerInfo(__in bool IN_bNotifySettingWnd = true);
	bool			OnLoad_EESInfo(__in bool IN_bNotifySettingWnd = true);
	bool			OnLoad_ALInfo(__in bool IN_bNotifySettingWnd = true);
	bool			OnLoad_LossInfo(__in bool IN_bNotifySettingWnd = true);
protected:
	virtual void	OnMatchingServer();
	bool            Compare_ServerConfig_ServRestart(__in CConfig_Server* IN_Old, __in CConfig_Server* IN_New);
public:
	void			OnSet_Connection						(LPARAM IN_LPARAM);
	void			Proc_ServerRequestLinkTest				(__in WPARAM IN_WPARAM, __in LPARAM IN_LPARAM);
	void			Proc_ServerReplyLinkTest				(__in WPARAM IN_WPARAM, __in LPARAM IN_LPARAM);
	void			Proc_ServerReportOnlineState			(__in WPARAM IN_WPARAM, __in LPARAM IN_LPARAM);

	void			Proc_ServerReportUserChange				(__in WPARAM IN_WPARAM, __in LPARAM IN_LPARAM);
	void			Proc_ServerRequestUserChange			(__in WPARAM IN_WPARAM, __in LPARAM IN_LPARAM);
	void			Proc_ServerReplyUserCommand				(__in WPARAM IN_WPARAM, __in LPARAM IN_LPARAM);

	void			Proc_ServerReportEquipmentState			(__in WPARAM IN_WPARAM, __in LPARAM IN_LPARAM);
	void			Proc_ServerRequestEquipmentStateDisplay	(__in WPARAM IN_WPARAM, __in LPARAM IN_LPARAM);
	void			Proc_ServerReplyEquipmentStateDisplay	(__in WPARAM IN_WPARAM, __in LPARAM IN_LPARAM);

	void			Proc_ServerReplyLossState				(__in WPARAM IN_WPARAM, __in LPARAM IN_LPARAM);
	void			Proc_ServerRequestLossState				(__in WPARAM IN_WPARAM, __in LPARAM IN_LPARAM);
	void			Proc_ServerReplyLossWindow				(__in WPARAM IN_WPARAM, __in LPARAM IN_LPARAM);

	void			Proc_ServerReportAlarmState				(__in WPARAM IN_WPARAM, __in LPARAM IN_LPARAM);
	void			Proc_ServerRequestAlarmList				(__in WPARAM IN_WPARAM, __in LPARAM IN_LPARAM);
	void			Proc_ServerReplyAlarmList				(__in WPARAM IN_WPARAM, __in LPARAM IN_LPARAM);

	void			Proc_ServerEESMode						(__in WPARAM IN_WPARAM, __in LPARAM IN_LPARAM);

	void			Proc_ServerRequestSetDateTime			(__in WPARAM IN_WPARAM, __in LPARAM IN_LPARAM);
	void			Proc_ServerReplySetDateTime				(__in WPARAM IN_WPARAM, __in LPARAM IN_LPARAM);

	void			Proc_ServerRequestTerminalMessage		(__in WPARAM IN_WPARAM, __in LPARAM IN_LPARAM);
	void			Proc_ServerReplyTerminalMessage			(__in WPARAM IN_WPARAM, __in LPARAM IN_LPARAM);

	void			Proc_ServerRequestOpCallMessage			(__in WPARAM IN_WPARAM, __in LPARAM IN_LPARAM);
	void			Proc_ServerReplyOpCall					(__in WPARAM IN_WPARAM, __in LPARAM IN_LPARAM);
public:
	virtual void	OnSet_EESMode(__in enEES_Mode nAcessMode);
#endif
#if TEST
protected:
	virtual void	OnReport_TerminalMessage(__in lt::Request_Terminal_Message_Args::Args& IN_Data);
	virtual	void	OnReport_OpCall(__in lt::Request_Opcall_Args::Args&  IN_Data);
	virtual BOOL	GetNtPrivilege();
public:
	void			Proc_ServerUnitIdRead(__in WPARAM IN_WPARAM, __in LPARAM IN_LPARAM);
	void			Proc_ServerRequestUnitIdRead(__in WPARAM IN_WPARAM, __in LPARAM IN_LPARAM);
	void			Proc_ServerReplytUnitIdRead(__in WPARAM IN_WPARAM, __in LPARAM IN_LPARAM);

	void			Proc_ServerReportStartProcess(__in WPARAM IN_WPARAM, __in LPARAM IN_LPARAM);
	void			Proc_ServerReportStartLot(__in WPARAM IN_WPARAM, __in LPARAM IN_LPARAM);
	void			Proc_ServerReportEndEvent(__in WPARAM IN_WPARAM, __in LPARAM IN_LPARAM);
	void			Proc_ServerReportEndProcess(__in WPARAM IN_WPARAM, __in LPARAM IN_LPARAM);
#endif
};
#if (USE_XML)
extern CView_MainCtrl_ICS* mView_MainCtrl_ICS;
#endif
#endif // View_MainCtrl_ICS_h__
