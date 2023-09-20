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

	// Î°úÍ∑∏ Î©îÏÑ∏ÏßÄ
	afx_msg	LRESULT	OnWM_LogMsg					(WPARAM wParam, LPARAM lParam);

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
	// CView_MainCtrl ?¨Ï†ï??	//=========================================================================
	//-----------------------------------------------------
	// Ï¥àÍ∏∞ ?§Ï†ï Í¥Ä??	//-----------------------------------------------------
	// ?ùÏÑ±?êÏóê??Ï¥àÍ∏∞?????∏ÌåÖ
	virtual void	OnInit_ConstructionSetting		();
	// Window ?ùÏÑ± ???∏ÌåÖ
	virtual void	OnInit_UISetting				();
	// Ï£ºÎ??•Ïπò??Í∏∞Î≥∏ ?§Ï†ï
	virtual void	OnInit_DeviceSetting			();

	//-------------------------------------------------------------------------
	// ?åÏùº Î∂àÎü¨?§Í∏∞ Î∞??∏ÌåÖ
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

	// ?§ÎπÑ ?∞Ïù¥?? ?µÏã†, GUI Îß§Ïπ≠
	virtual void	OnMatchingEquipment				();
	bool			Compare_LineConfig_ServRestart	(__in CConfig_Line* IN_Old, __in CConfig_Line* IN_New);
	//=========================================================================
	// CTestManager_EQP_ICS ?¨Ï†ï??	
	//=========================================================================
	//-----------------------------------------------------
	// Î°úÍ∑∏/?åÏùº Ï≤òÎ¶¨
	//-----------------------------------------------------
	virtual void	OnInit_LogFolder				();

	//-----------------------------------------------------
	// UI ?ÖÎç∞?¥Ìä∏
	//-----------------------------------------------------
	virtual void	OnSetStatus_Production			(__in bool IN_bEnd);

	//-----------------------------------------------------
	// Í≤∞Í≥º ?∞Ïù¥??Ï≤òÎ¶¨
	//-----------------------------------------------------
	virtual void	OnLog_EquipmentAlarm			(__in uint8_t IN_nEqpOder, __in const ST_AlarmStatus* IN_pAlarm);
	virtual void	OnLog_RegisterSocket			(__in LPCTSTR IN_szRFID);

	// ?úÌíà Í≤∞Í≥º ?îÎ©¥???úÏãú
	virtual void	OnReport_Socket					(__in LPCTSTR IN_szRFID);
	virtual void	OnReport_Socket_NG				(__in LPCTSTR IN_szRFID);

	// Í≤Ä??ÏßÑÌñâ ?úÍ∞Ñ Ï≤¥ÌÅ¨ (?§ÎπÑ, ?åÎùº L/R)
	virtual void	OnUpdate_ElapsedTime_Unit		(__in UINT nEquipmentIdx, __in uint8_t IN_nPara);
	virtual void	OnUpdate_ElapsedTime_All		();

public: 
	//=========================================================================
	// CView_MainCtrl ?¨Ï†ï??	//=========================================================================
	// Ï∞®Ïùº???àÎèÑ???ÑÌôò ???¨Ïö©
	virtual UINT	SwitchWindow				(__in UINT nIndex);
	// ?•Ïπò ?µÏã† ?ÅÌÉú ?úÏãú ?àÎèÑ???¨Ïù∏???§Ï†ï
	virtual void	SetCommPanePtr				(__in CWnd* pwndCommPane);

	// ?µÏÖò??Î≥ÄÍ≤??òÏóà??Í≤ΩÏö∞ ?§Ïãú UI???∞Ïù¥?∞Î? ?∏ÌåÖ?òÍ∏∞ ?ÑÌïú ?®Ïàò
	virtual void	ReloadOption				();

	// ?ÑÎ°úÍ∑∏Îû® Î°úÎî© ?ùÎÇú ???êÎèô Ï≤òÎ¶¨Î•??ÑÌïú ?®Ïàò
	virtual void	InitStartProgress			();	
	virtual BOOL	InitStartDeviceProgress		();	

	// ?ÑÎ°úÍ∑∏Îû® Ï¢ÖÎ£å??Ï≤òÎ¶¨?¥Ïïº ??Í∏∞Îä•?§ÏùÑ Ï≤òÎ¶¨?òÎäî ?®Ïàò
	virtual void	FinalExitProgress			();

	//=========================================================================
	// CTestManager_EQP_ICS ?¨Ï†ï??	//=========================================================================
	// Í≤Ä?¨Í∏∞ Ï¢ÖÎ•ò ?§Ï†ï
	virtual void	OnSet_SystemType			(__in enInsptrSysType nSysType);

	// Î°úÍ∑∏ Î©îÏÑ∏ÏßÄ Ï≤òÎ¶¨???®Ïàò
	virtual void	OnAdd_Log					(__in LPCTSTR lpszLog, __in BOOL bError = FALSE, __in UINT nLogType = LOGTYPE_NONE, __in BOOL bOnlyLogType = FALSE);
	virtual void	OnAdd_Log					(__in LPCTSTR lpszLog, __in UINT nLogType = LOGTYPE_NONE);

	// ?úÏñ¥ Í∂åÌïú ?§Ï†ï
	virtual void	OnInit_PermissionMode		(__in enPermissionMode nAcessMode);	
	virtual void	OnSet_PermissionMode		(__in enPermissionMode nAcessMode);	

	void			Proc_LineCtrlCmd			(__in uint8_t IN_nCmdIndex);
	void			Proc_EquipmentCtrlCmd		(__in uint8_t IN_nCmdIndex, __in uint8_t IN_nEqpOrder);

	//=========================================================================
	// TEST ÏΩîÎìú
	//=========================================================================
	virtual void	Test_Process				(__in UINT nTestNo);	

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
