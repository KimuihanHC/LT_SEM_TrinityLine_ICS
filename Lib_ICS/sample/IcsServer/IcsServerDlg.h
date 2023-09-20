
// IcsServerDlg.h: 헤더 파일
//

#pragma once

#include "TypeDecl.h"
#include "Event/IcsCommEvent.h"

#if USE_XML
#include "Event/IcsCommEventEes.h"
#endif

namespace lt
{
	class CDebugLogDispatcher;
	class ILogDispatcher;
	class ILogData;
	class CLogDispatcher;
	class CLogger;
	class CIcsTcpServer;
	class CIcsRemote;
	class CIcsRemoteLoUnloader;
	class CIcsRemoteTester;
	class CIcsRemoteReturner;
	class ICommunicator;
	template<typename TypeSender, typename TypeArgs>
	class IEventArgs;
	template<typename TypeOwner, typename TypeEventArgs>
	class CEventMfnListener;

#if USE_XML
	class CIcsRemoteEes;	//추가
#endif
}

class CRichEditCtrlEx;

// CIcsServerDlg 대화 상자
class CIcsServerDlg : public CDialogEx
{
// 생성입니다.
public:
	CIcsServerDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	~CIcsServerDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ICSSERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	afx_msg void OnBnClickedButtonOpen();
	afx_msg void OnBnClickedButtonClose();
	afx_msg void OnBnClickedButtonTimeSync();
	afx_msg void OnBnClickedButtonUserLevel();
	afx_msg void OnBnClickedButtonLanguage();
	afx_msg void OnBnClickedButtonModel();
	afx_msg void OnBnClickedButtonInitialize();
	afx_msg void OnBnClickedButtonReset();
	afx_msg void OnBnClickedButtonRun();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedButtonBuzzerOff();
	afx_msg void OnBnClickedButtonMinimize();
	afx_msg void OnBnClickedButtonRestore();
	afx_msg void OnBnClickedButtonSocketAccept();
	afx_msg void OnBnClickedButtonEjectAccept();
	afx_msg void OnBnClickedButtonProductionStartEnd();
	afx_msg void OnBnClickedButtonForceEject();
	DECLARE_MESSAGE_MAP()

private:
	template<typename TypeRes, typename... TypeArgs>
	TypeRes InvokeRemote(TypeRes (lt::CIcsRemote::*fn)(TypeArgs...),
						 TypeArgs&&... args);
#if USE_XML
	template<typename TypeRes, typename... TypeArgs>
	TypeRes InvokeRemoteEes(TypeRes(lt::CIcsRemoteEes::*fn)(TypeArgs...),
		TypeArgs&&... args);
#endif
	bool CreateRemote(int equipmentType);
	void RemoveRemote();
	//
	void EnableConnectionCtrls(bool enable);
	void EnableCommunicationCtrls(bool enable);

	void ClearData();
	void ClearData(LPCTSTR rfid);

	using LogAddedEventArgs =
		lt::IEventArgs<lt::ILogDispatcher, lt::ILogData>;

	void OnLogAddedEvent(LogAddedEventArgs & eventArgs);

	// Server
	using RemoteConnectionEventArgs =
		lt::IEventArgs<lt::CIcsTcpServer, lt::ICommunicator>;

	//2023.06.23a
	void OnRemoteConnectedEvent(RemoteConnectionEventArgs & eventArgs);
	void OnRemoteDisconnectedEvent(RemoteConnectionEventArgs & eventArgs);

	// Common
	void SendTimeSync();
	void SendUserLevel();
	void SendLanguage();
	void SendModel();
	void SendInitialize();
	void SendReset();
	void SendRun();
	void SendStop();
	void SendBuzzerOff();
	void SendUiVisible(int cmdShow);
	void SendSocketAccept();
	void SendEjectAccept();
	void SendProductionStartEnd();
	void SendForceEject();

	using EquipmentIdEventArgs =
		lt::EquipmentIdEventArgs<lt::CIcsRemote>;
	using EquipmentStatusEventArgs =
		lt::EquipmentStatusEventArgs<lt::CIcsRemote>;
	using OperationModeEventArgs =
		lt::OperationModeEventArgs<lt::CIcsRemote>;
	using SocketRequestEventArgs =
		lt::SocketRequestEventArgs<lt::CIcsRemote>;
	using PortClearEventArgs =
		lt::PortClearEventArgs<lt::CIcsRemote>;
	using PortStatusEventArgs =
		lt::PortStatusEventArgs<lt::CIcsRemote>;
	using PortStatusRequestEventArgs =
		lt::PortStatusRequestEventArgs<lt::CIcsRemote>;
	using ConveyorStatusEventArgs =
		lt::ConveyorStatusEventArgs<lt::CIcsRemote>;
	using ConveyorStatusRequestEventArgs =
		lt::ConveyorStatusRequestEventArgs<lt::CIcsRemote>;

	void OnEquipmentIdEvent(EquipmentIdEventArgs & eventArgs);
	void OnEquipmentStatusEvent(EquipmentStatusEventArgs & eventArgs);
	void OnOperationModeEvent(OperationModeEventArgs & eventArgs);
	void OnSocketRequestEvent(SocketRequestEventArgs & eventArgs);
	void OnPortClearEvent(PortClearEventArgs & eventArgs);
	void OnPortStatusEvent(PortStatusEventArgs & eventArgs);
	void OnPortStatusRequestEvent(PortStatusRequestEventArgs & eventArgs);
	void OnConveyorStatusEvent(ConveyorStatusEventArgs & eventArgs);
	void OnConveyorStatusRequestEvent(ConveyorStatusRequestEventArgs & eventArgs);

	// LoUnloader
	using LoadingEventArgs =
		lt::LoadingEventArgs<lt::CIcsRemoteLoUnloader>;
	using TestResultRequestEventArgs =
		lt::TestResultRequestEventArgs<lt::CIcsRemoteLoUnloader>;
	using UnloadingEventArgs =
		lt::UnloadingEventArgs<lt::CIcsRemoteLoUnloader>;

	void OnLoadingEvent(LoadingEventArgs & eventArgs);
	void OnTestResultRequestEvent(TestResultRequestEventArgs & eventArgs);
	void OnUnloadingEvent(UnloadingEventArgs & eventArgs);

	// Tester
	using TestResultEventArgs =
		lt::TestResultEventArgs<lt::CIcsRemoteTester>;

	void OnTestResultEvent(TestResultEventArgs & eventArgs);

	//using ConstStringT =
//		lt::ConstStringT;
//	using ConstStringA =
		//lt::ConstStringA;
	//using ConstStringW =
//		lt::ConstStringW;

private:
	CRichEditCtrlEx * m_pRichEditCtrl = nullptr;

	lt::CDebugLogDispatcher * m_pDebugLogDispatcher = nullptr;

	using LogAddedEventListener =
		lt::CEventMfnListener<CIcsServerDlg, LogAddedEventArgs>;

	LogAddedEventListener * m_pLogAddedEventListener = nullptr;

	lt::CLogDispatcher * m_pLogDispatcher = nullptr;
	lt::CLogger * m_pLogger = nullptr;

	lt::CIcsTcpServer * m_pServer = nullptr;
#if USE_XML
	lt::CIcsRemoteEes * m_pRemoteEes = nullptr;
#endif
	lt::CIcsRemote * m_pRemote = nullptr;
	lt::StdMutex * m_pRemoteMutex = nullptr;
	int m_equipmentType = 0;

	// Server
	using RemoteConnectionEventListener =
		lt::CEventMfnListener<CIcsServerDlg, RemoteConnectionEventArgs>;

	RemoteConnectionEventListener * m_pRemoteConnectedEventListener = nullptr;
	RemoteConnectionEventListener * m_pRemoteDisconnectedEventListener = nullptr;

	// Common
	using EquipmentIdEventListener =
		lt::CEventMfnListener<CIcsServerDlg, EquipmentIdEventArgs>;
	using EquipmentStatusEventListener =
		lt::CEventMfnListener<CIcsServerDlg, EquipmentStatusEventArgs>;
	using OperationModeEventListener =
		lt::CEventMfnListener<CIcsServerDlg, OperationModeEventArgs>;
	using SocketRequestEventListener =
		lt::CEventMfnListener<CIcsServerDlg, SocketRequestEventArgs>;
	using PortClearEventListener =
		lt::CEventMfnListener<CIcsServerDlg, PortClearEventArgs>;
	using PortStatusEventListener =
		lt::CEventMfnListener<CIcsServerDlg, PortStatusEventArgs>;
	using PortStatusRequestEventListener =
		lt::CEventMfnListener<CIcsServerDlg, PortStatusRequestEventArgs>;
	using ConveyorStatusEventListener =
		lt::CEventMfnListener<CIcsServerDlg, ConveyorStatusEventArgs>;
	using ConveyorStatusRequestEventListener =
		lt::CEventMfnListener<CIcsServerDlg, ConveyorStatusRequestEventArgs>;

	EquipmentIdEventListener * m_pEquipmentIdEventListener = nullptr;
	EquipmentStatusEventListener * m_pEquipmentStatusEventListener = nullptr;
	OperationModeEventListener * m_pOperationModeEventListener = nullptr;
	SocketRequestEventListener * m_pSocketRequestEventListener = nullptr;
	PortClearEventListener * m_pPortClearEventListener = nullptr;
	PortStatusEventListener * m_pPortStatusEventListener = nullptr;
	PortStatusRequestEventListener * m_pPortStatusRequestEventListener = nullptr;
	ConveyorStatusEventListener * m_pConveyorStatusEventListener = nullptr;
	ConveyorStatusRequestEventListener * m_pConveyorStatusRequestEventListener = nullptr;

	// LoUnloader
	using LoadingEventListener =
		lt::CEventMfnListener<CIcsServerDlg, LoadingEventArgs>;
	using TestResultRequestEventListener =
		lt::CEventMfnListener<CIcsServerDlg, TestResultRequestEventArgs>;
	using UnloadingEventListener =
		lt::CEventMfnListener<CIcsServerDlg, UnloadingEventArgs>;

	LoadingEventListener * m_pLoadingEventListener = nullptr;
	TestResultRequestEventListener * m_pTestResultRequestEventListener = nullptr;
	UnloadingEventListener * m_pUnloadingEventListener = nullptr;

	// Tester
	using TestResultEventListener =
		lt::CEventMfnListener<CIcsServerDlg, TestResultEventArgs>;

	TestResultEventListener * m_pTestResultEventListener = nullptr;

#if USE_XML	//2023.08.01a
private:
	bool CreateRemoteEES(int equipmentType);
	void RemoveRemoteEES();

	void SendxmlReplyLinkTestMassage();
	void SendxmlReportUserChangeMassage();
	void SendxmlReplyUserCommandMassage();
	void SendxmlReportOnlineStateMassage();
	void SendxmlReportEqpStateMassage();
	void SendxmlReplyLossWindow();
	void SendxmlReplyAlarmList();
	//
private:
	using Request_Link_Test_EventArgs =
		lt::Request_Link_Test_EventArgs<lt::CIcsRemoteEes>;
	void OnRequestLinkTestEvent(Request_Link_Test_EventArgs & eventArgs);
	using Request_User_Command_EventArgs =
		lt::Request_User_Command_EventArgs<lt::CIcsRemoteEes>;
	void OnRequestUserCommandEvent(Request_User_Command_EventArgs & eventArgs);
	using Request_Equipment_State_Display_EventArgs =
		lt::Request_Equipment_State_Display_EventArgs<lt::CIcsRemoteEes>;
	void OnRequestEquipmentStateEvent(Request_Equipment_State_Display_EventArgs & eventArgs);
	using Request_Loss_Window_EventArgs =
		lt::Request_Loss_Window_EventArgs<lt::CIcsRemoteEes>;
	void OnRequestLossWindowEvent(Request_Loss_Window_EventArgs & eventArgs);
	using Request_Alarm_List_EventArgs =
		lt::Request_Alarm_List_EventArgs<lt::CIcsRemoteEes>;
	void OnRequestAlarmListEventEvent(Request_Alarm_List_EventArgs & eventArgs);
	using Request_Set_DateTime_EventArgs =
		lt::Request_Set_DateTime_EventArgs<lt::CIcsRemoteEes>;
	void OnRequestSetDateTimeEventEvent(Request_Set_DateTime_EventArgs & eventArgs);
	using Request_Terminal_Message_EventArgs =
		lt::Request_Terminal_Message_EventArgs<lt::CIcsRemoteEes>;
	void OnRequestTerminalMessageEventEvent(Request_Terminal_Message_EventArgs & eventArgs);
	using Request_Opcall_EventArgs =
		lt::Request_Opcall_EventArgs<lt::CIcsRemoteEes>;
	void OnRequestOpCallEventEvent(Request_Opcall_EventArgs & eventArgs);

	using Request_Link_Test_EventListener =
		lt::CEventMfnListener<CIcsServerDlg, Request_Link_Test_EventArgs>;
	Request_Link_Test_EventListener * m_pRequestLinkTestEventListener = nullptr;
	using Request_User_Command_EventListener =
		lt::CEventMfnListener<CIcsServerDlg, Request_User_Command_EventArgs>;
	Request_User_Command_EventListener * m_pRequestUserCommandEventListener = nullptr;
	using Request_Equipment_State_Display_EventListener =
		lt::CEventMfnListener<CIcsServerDlg, Request_Equipment_State_Display_EventArgs>;
	Request_Equipment_State_Display_EventListener * m_pRequestEquipmentStateEventListener = nullptr;
	using Request_Loss_Window_EventListener =
		lt::CEventMfnListener<CIcsServerDlg, Request_Loss_Window_EventArgs>;
	Request_Loss_Window_EventListener * m_pRequestLossWindowListener = nullptr;
	using Request_Alarm_List_EventListener =
		lt::CEventMfnListener<CIcsServerDlg, Request_Alarm_List_EventArgs>;
	Request_Alarm_List_EventListener *  m_pRequestAlarmListEventListener = nullptr;
	using Request_Set_DateTime_EventListener =
		lt::CEventMfnListener<CIcsServerDlg, Request_Set_DateTime_EventArgs>;
	Request_Set_DateTime_EventListener *  m_pRequestSetDateTimeEventListener = nullptr;
	using Request_Terminal_Message_EventListener =
		lt::CEventMfnListener<CIcsServerDlg, Request_Terminal_Message_EventArgs>;
	Request_Terminal_Message_EventListener *  m_pRequestTerminalMessageEventListener = nullptr;
	using Request_Opcall_EventListener =
		lt::CEventMfnListener<CIcsServerDlg, Request_Opcall_EventArgs>;
	Request_Opcall_EventListener *  m_pRequestOpcallEventListener = nullptr;
protected:
	HANDLE	m_hTimerCheck = NULL;
	HANDLE	m_hTimerQueue = NULL;
	void	CreateTimeOutCheck();
	void	DeleteTimerOutCheck();

	static VOID CALLBACK TimerRoutineTimeOutCheck(__in PVOID lpParam, __in BOOLEAN TimerOrWaitFired);
public:
	afx_msg void OnBnClickedButtonReportOnlineState();
	afx_msg void OnBnClickedButtonReportUserChange();	
#endif
};
