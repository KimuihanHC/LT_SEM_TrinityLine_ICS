
// IcsClientDlg.h: 헤더 파일
//

#pragma once

#include "TypeDecl.h"
#include "Event/IcsCommEvent.h"

namespace lt
{
	class CDebugLogDispatcher;
	class ILogDispatcher;
	class ILogData;
	class CLogDispatcher;
	class CLogger;
	class CIcsClient;
	class CIcsClientLoUnloader;
	class CIcsClientTester;
	class CIcsClientReturner;
	class CTcpCommunicator;
	class ICommunicator;
	struct SCommOption;
	template<typename TypeSender, typename TypeArgs>
	class IEventArgs;
	template<typename TypeOwner, typename TypeEventArgs>
	class CEventMfnListener;
}

class CRichEditCtrlEx;

// CIcsClientDlg 대화 상자
class CIcsClientDlg : public CDialogEx
{
// 생성입니다.
public:
	CIcsClientDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	~CIcsClientDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ICSCLIENT_DIALOG };
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

	afx_msg void OnBnClickedButtonConnect();
	afx_msg void OnBnClickedButtonDisconnect();
	afx_msg void OnBnClickedButtonEquipmentId();
	afx_msg void OnBnClickedButtonOperationMode();
	afx_msg void OnBnClickedButtonLoading();
	afx_msg void OnBnClickedButtonRfidTestResultRequest();
	afx_msg void OnBnClickedButtonTestResult();
	afx_msg void OnBnClickedButtonUnloading();
	afx_msg void OnBnClickedButtonSocketRequest();
	afx_msg void OnBnClickedButtonEquipmentStatus();
	afx_msg void OnBnClickedButtonPortClear();
	afx_msg void OnBnClickedButtonPortStatus();
	afx_msg void OnBnClickedButtonPortStatusRequest();
	afx_msg void OnBnClickedButtonConveyorStatus();
	afx_msg void OnBnClickedButtonConveyorStatusRequest();
	DECLARE_MESSAGE_MAP()

private:
	bool CreateClient(int equipmentType);
	void RemoveClient();

	template<typename TypeRes, typename... TypeArgs>
	TypeRes InvokeClient(TypeRes (lt::CIcsClient::*fn)(TypeArgs...),
						 TypeArgs&&... args);

	void EnableConnectionCtrls(bool enable);
	void EnableCommunicationCtrls(bool enable);

	void ClearData();

	using LogAddedEventArgs =
		lt::IEventArgs<lt::ILogDispatcher, lt::ILogData>;

	void OnLogAddedEvent(LogAddedEventArgs & eventArgs);

	// Comm
	using CommConnectionEventArgs =
		lt::IEventArgs<lt::ICommunicator, const lt::SCommOption>;

	void OnCommConnectedEvent(CommConnectionEventArgs & eventArgs);
	void OnCommDisconnectingEvent(CommConnectionEventArgs & eventArgs);

	// Common
	void SendEquipmentId();
	void SendOperationMode();
	void SendSocketRequest();
	void SendEquipmentStatus();
	void SendPortClear();
	void SendPortStatus();
	void SendPortStatusRequest();
	void SendConveyorStatus();
	void SendConveyorStatusRequest();

	using TimeSyncEventArgs =
		lt::TimeSyncEventArgs<lt::CIcsClient>;
	using UserLevelEventArgs =
		lt::UserLevelEventArgs<lt::CIcsClient>;
	using LanguageEventArgs =
		lt::LanguageEventArgs<lt::CIcsClient>;
	using ModelEventArgs =
		lt::ModelEventArgs<lt::CIcsClient>;
	using InitializationEventArgs =
		lt::InitializationEventArgs<lt::CIcsClient>;
	using ResetEventArgs =
		lt::ResetEventArgs<lt::CIcsClient>;
	using RunEventArgs =
		lt::RunEventArgs<lt::CIcsClient>;
	using StopEventArgs =
		lt::StopEventArgs<lt::CIcsClient>;
	using BuzzerOffEventArgs =
		lt::BuzzerOffEventArgs<lt::CIcsClient>;
	using UiVisibleEventArgs =
		lt::UiVisibleEventArgs<lt::CIcsClient>;
	using SocketAcceptingEventArgs =
		lt::SocketAcceptingEventArgs<lt::CIcsClient>;
	using EjectAcceptingEventArgs =
		lt::EjectAcceptingEventArgs<lt::CIcsClient>;
	using ProductionStartEndEventArgs =
		lt::ProductionStartEndEventArgs<lt::CIcsClient>;
	using ForceEjectEventArgs =
		lt::ForceEjectEventArgs<lt::CIcsClient>;
	//
	void OnTimeSyncEvent(TimeSyncEventArgs & eventArgs);
	void OnUserLevelEvent(UserLevelEventArgs & eventArgs);
	void OnLanguageEvent(LanguageEventArgs & eventArgs);
	void OnModelEvent(ModelEventArgs & eventArgs);
	void OnInitializationEvent(InitializationEventArgs & eventArgs);
	void OnResetEvent(ResetEventArgs & eventArgs);
	void OnRunEvent(RunEventArgs & eventArgs);
	void OnStopEvent(StopEventArgs & eventArgs);
	void OnBuzzerOffEvent(BuzzerOffEventArgs & eventArgs);
	void OnUiVisibleEvent(UiVisibleEventArgs & eventArgs);
	void OnSocketAcceptingEvent(SocketAcceptingEventArgs & eventArgs);
	void OnEjectAcceptingEvent(EjectAcceptingEventArgs & eventArgs);
	void OnProductionStartEndEvent(ProductionStartEndEventArgs & eventArgs);
	void OnForceEjectEvent(ForceEjectEventArgs & eventArgs);

	// LoUnloader
	void SendLoading();
	void SendTestResultRequest();
	void SendUnloading();

	// Tester
	void SendTestResult();

private:
	CRichEditCtrlEx * m_pRichEditCtrl = nullptr;

	lt::CDebugLogDispatcher * m_pDebugLogDispatcher = nullptr;

	using LogAddedEventListener =
		lt::CEventMfnListener<CIcsClientDlg, LogAddedEventArgs>;

	LogAddedEventListener * m_pLogAddedEventListener = nullptr;

	lt::CLogDispatcher * m_pLogDispatcher = nullptr;
	lt::CLogger * m_pLogger = nullptr;

	lt::CTcpCommunicator * m_pComm = nullptr;
	lt::CIcsClient * m_pClient = nullptr;
	lt::StdMutex * m_pClientMutex = nullptr;
	int m_equipmentType = 0;

	// Comm
	using CommConnectionEventListener =
		lt::CEventMfnListener<CIcsClientDlg, CommConnectionEventArgs>;

	CommConnectionEventListener * m_pCommConnectedEventListener = nullptr;
	CommConnectionEventListener * m_pCommDisconnectingEventListener = nullptr;

	// Common
	using TimeSyncEventListener =
		lt::CEventMfnListener<CIcsClientDlg, TimeSyncEventArgs>;
	using UserLevelEventListener =
		lt::CEventMfnListener<CIcsClientDlg, UserLevelEventArgs>;
	using LanguageEventListener =
		lt::CEventMfnListener<CIcsClientDlg, LanguageEventArgs>;
	using ModelEventListener =
		lt::CEventMfnListener<CIcsClientDlg, ModelEventArgs>;
	using InitializationEventListener =
		lt::CEventMfnListener<CIcsClientDlg, InitializationEventArgs>;
	using ResetEventListener =
		lt::CEventMfnListener<CIcsClientDlg, ResetEventArgs>;
	using RunEventListener =
		lt::CEventMfnListener<CIcsClientDlg, RunEventArgs>;
	using StopEventListener =
		lt::CEventMfnListener<CIcsClientDlg, StopEventArgs>;
	using BuzzerOffEventListener =
		lt::CEventMfnListener<CIcsClientDlg, BuzzerOffEventArgs>;
	using UiVisibleEventListener =
		lt::CEventMfnListener<CIcsClientDlg, UiVisibleEventArgs>;
	using SocketAcceptingEventListener =
		lt::CEventMfnListener<CIcsClientDlg, SocketAcceptingEventArgs>;
	using EjectAcceptingEventListener =
		lt::CEventMfnListener<CIcsClientDlg, EjectAcceptingEventArgs>;
	using ProductionStartEndEventListener =
		lt::CEventMfnListener<CIcsClientDlg, ProductionStartEndEventArgs>;
	using ForceEjectEventListener =
		lt::CEventMfnListener<CIcsClientDlg, ForceEjectEventArgs>;

	TimeSyncEventListener * m_pTimeSyncEventListener = nullptr;
	UserLevelEventListener * m_pUserLevelEventListener = nullptr;
	LanguageEventListener * m_pLanguageEventListener = nullptr;
	ModelEventListener * m_pModelEventListener = nullptr;
	InitializationEventListener * m_pInitializationEventListener = nullptr;
	ResetEventListener * m_pResetEventListener = nullptr;
	RunEventListener * m_pRunEventListener = nullptr;
	StopEventListener * m_pStopEventListener = nullptr;
	BuzzerOffEventListener * m_pBuzzerOffEventListener = nullptr;
	UiVisibleEventListener * m_pUiVisibleEventListener = nullptr;
	SocketAcceptingEventListener * m_pSocketAcceptingEventListener = nullptr;
	EjectAcceptingEventListener * m_pEjectAcceptingEventListener = nullptr;
	ProductionStartEndEventListener * m_pProductionStartEndEventListener = nullptr;
	ForceEjectEventListener * m_pForceEjectEventListener = nullptr;
	
};
