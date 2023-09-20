
// IcsClientDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "IcsClient.h"
#include "IcsClientDlg.h"
#include "afxdialogex.h"
#include "RichEditCtrlEx.h"
#include "Logger/DebugLogDispatcher.h"
#include "Logger/Logger.h"
#include "Logger/LogDispatcher.h"
#include "Comm/TcpCommunicator.h"
#include "Comm/IcsClientLoUnloader.h"
#include "Comm/IcsClientTester.h"
#include "Event/EventMfnListener.hpp"
#include "Util/SyncUtil.hpp"
#include "Util/BasicTimer.h"

#include "Util/FileUtil.hpp"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CIcsClientDlg 대화 상자



CIcsClientDlg::CIcsClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ICSCLIENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pRichEditCtrl = new CRichEditCtrlEx();

	m_pDebugLogDispatcher =
		new lt::CDebugLogDispatcher(_T("ICS_CLIENT"), lt::LogFilter::Detail);
	static_cast<lt::ILogDispatcher *>(*m_pDebugLogDispatcher)->Dispatch(
		lt::LogLevel::None,
		_T("============================= ICS Client begins =============================")
	);

	m_pLogAddedEventListener =
		new LogAddedEventListener(this, &CIcsClientDlg::OnLogAddedEvent);

	m_pLogDispatcher = new lt::CLogDispatcher();
	m_pLogDispatcher->SetLogFilter(lt::LogFilter::Detail);
	m_pLogDispatcher->GetLogAddedEventNotifier() +=
		m_pLogAddedEventListener;

	m_pLogger = new lt::CLogger();
	m_pLogger->Attach(m_pLogDispatcher);

	m_pComm = new lt::CTcpCommunicator(_T(""));
	m_pComm->SetLogDispatcher(*m_pDebugLogDispatcher);

	m_pClientMutex = new lt::StdMutex();

	// Comm
	m_pCommConnectedEventListener =
		new CommConnectionEventListener(this, &CIcsClientDlg::OnCommConnectedEvent);
	m_pCommDisconnectingEventListener =
		new CommConnectionEventListener(this, &CIcsClientDlg::OnCommDisconnectingEvent);

	m_pComm->GetCommConnectedEventNotifier() +=
		m_pCommConnectedEventListener;
	m_pComm->GetCommDisconnectingEventNotifier() +=
		m_pCommDisconnectingEventListener;

	// Common
	m_pTimeSyncEventListener =
		new TimeSyncEventListener(this, &CIcsClientDlg::OnTimeSyncEvent);
	m_pUserLevelEventListener =
		new UserLevelEventListener(this, &CIcsClientDlg::OnUserLevelEvent);
	m_pLanguageEventListener =
		new LanguageEventListener(this, &CIcsClientDlg::OnLanguageEvent);
	m_pModelEventListener =
		new ModelEventListener(this, &CIcsClientDlg::OnModelEvent);
	m_pInitializationEventListener =
		new InitializationEventListener(this, &CIcsClientDlg::OnInitializationEvent);
	m_pResetEventListener =
		new ResetEventListener(this, &CIcsClientDlg::OnResetEvent);
	m_pRunEventListener =
		new RunEventListener(this, &CIcsClientDlg::OnRunEvent);
	m_pStopEventListener =
		new StopEventListener(this, &CIcsClientDlg::OnStopEvent);
	m_pBuzzerOffEventListener =
		new BuzzerOffEventListener(this, &CIcsClientDlg::OnBuzzerOffEvent);
	m_pUiVisibleEventListener =
		new UiVisibleEventListener(this, &CIcsClientDlg::OnUiVisibleEvent);
	m_pSocketAcceptingEventListener =
		new SocketAcceptingEventListener(this, &CIcsClientDlg::OnSocketAcceptingEvent);
	m_pEjectAcceptingEventListener =
		new EjectAcceptingEventListener(this, &CIcsClientDlg::OnEjectAcceptingEvent);
	m_pProductionStartEndEventListener =
		new ProductionStartEndEventListener(this, &CIcsClientDlg::OnProductionStartEndEvent);
	m_pForceEjectEventListener =
		new ForceEjectEventListener(this, &CIcsClientDlg::OnForceEjectEvent);
}

CIcsClientDlg::~CIcsClientDlg()
{
	// Common
	delete m_pTimeSyncEventListener;
	delete m_pUserLevelEventListener;
	delete m_pLanguageEventListener;
	delete m_pModelEventListener;
	delete m_pInitializationEventListener;
	delete m_pResetEventListener;
	delete m_pRunEventListener;
	delete m_pStopEventListener;
	delete m_pBuzzerOffEventListener;
	delete m_pUiVisibleEventListener;
	delete m_pSocketAcceptingEventListener;
	delete m_pEjectAcceptingEventListener;
	delete m_pProductionStartEndEventListener;
	delete m_pForceEjectEventListener;

	// Comm
	m_pComm->GetCommConnectedEventNotifier() -=
		m_pCommConnectedEventListener;
	m_pComm->GetCommDisconnectingEventNotifier() -=
		m_pCommDisconnectingEventListener;

	delete m_pCommConnectedEventListener;
	delete m_pCommDisconnectingEventListener;

	delete m_pClientMutex;

	m_pComm->SetLogDispatcher(nullptr);
	delete m_pComm;

	m_pLogger->Detach();
	delete m_pLogger;

	m_pLogDispatcher->GetLogAddedEventNotifier() -=
		m_pLogAddedEventListener;
	delete m_pLogDispatcher;

	delete m_pLogAddedEventListener;

	static_cast<lt::ILogDispatcher *>(*m_pDebugLogDispatcher)->Dispatch(
		lt::LogLevel::None,
		_T("============================= ICS Client ends =============================")
	);
	Sleep(100);
	delete m_pDebugLogDispatcher;

	delete m_pRichEditCtrl;
}

void CIcsClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_RICHEDIT_LOG, *m_pRichEditCtrl);
}

BEGIN_MESSAGE_MAP(CIcsClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CIcsClientDlg::OnBnClickedButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON_DISCONNECT, &CIcsClientDlg::OnBnClickedButtonDisconnect)
	ON_BN_CLICKED(IDC_BUTTON_EQUIPMENT_ID, &CIcsClientDlg::OnBnClickedButtonEquipmentId)
	ON_BN_CLICKED(IDC_BUTTON_OPERATION_MODE, &CIcsClientDlg::OnBnClickedButtonOperationMode)
	ON_BN_CLICKED(IDC_BUTTON_LOADING, &CIcsClientDlg::OnBnClickedButtonLoading)
	ON_BN_CLICKED(IDC_BUTTON_RFID_TEST_RESULT_REQUEST, &CIcsClientDlg::OnBnClickedButtonRfidTestResultRequest)
	ON_BN_CLICKED(IDC_BUTTON_TEST_RESULT, &CIcsClientDlg::OnBnClickedButtonTestResult)
	ON_BN_CLICKED(IDC_BUTTON_UNLOADING, &CIcsClientDlg::OnBnClickedButtonUnloading)
	ON_BN_CLICKED(IDC_BUTTON_SOCKET_REQUEST, &CIcsClientDlg::OnBnClickedButtonSocketRequest)
	ON_BN_CLICKED(IDC_BUTTON_EQUIPMENT_STATUS, &CIcsClientDlg::OnBnClickedButtonEquipmentStatus)
	ON_BN_CLICKED(IDC_BUTTON_PORT_CLEAR, &CIcsClientDlg::OnBnClickedButtonPortClear)
	ON_BN_CLICKED(IDC_BUTTON_PORT_STATUS, &CIcsClientDlg::OnBnClickedButtonPortStatus)
	ON_BN_CLICKED(IDC_BUTTON_PORT_STATUS_REQUEST, &CIcsClientDlg::OnBnClickedButtonPortStatusRequest)
	ON_BN_CLICKED(IDC_BUTTON_CONVEYOR_STATUS, &CIcsClientDlg::OnBnClickedButtonConveyorStatus)
	ON_BN_CLICKED(IDC_BUTTON_CONVEYOR_STATUS_REQUEST, &CIcsClientDlg::OnBnClickedButtonConveyorStatusRequest)
END_MESSAGE_MAP()


// CIcsClientDlg 메시지 처리기

BOOL CIcsClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	EnableConnectionCtrls(true);
	EnableCommunicationCtrls(false);

	static_cast<CIPAddressCtrl *>(GetDlgItem(IDC_IPADDRESS))->SetAddress(127, 0, 0, 1);
	SetDlgItemInt(IDC_EDIT_PORT, 5000);
	static_cast<CComboBox *>(GetDlgItem(IDC_COMBO_EQUIPMENT))->SetCurSel(0);
	m_pRichEditCtrl->SetReadOnly();

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

BOOL CIcsClientDlg::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(m_pComm->IsConnected())
		OnBnClickedButtonDisconnect();

	RemoveClient();

	for(lt::CBasicTimer timer;;)
	{
		MSG msg = {};

		if(!::PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if(timer.GetElapsedTime(lt::CBasicTimer::Unit::Milliseconds) > 100)
				break;

			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			continue;
		}

		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}

	return CDialogEx::DestroyWindow();
}

void CIcsClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CIcsClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CIcsClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CIcsClientDlg::OnBnClickedButtonConnect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	auto equipmentType = static_cast<CComboBox *>(GetDlgItem(IDC_COMBO_EQUIPMENT))->GetCurSel();

	CreateClient(equipmentType);

	DWORD address = 0;

	static_cast<CIPAddressCtrl *>(GetDlgItem(IDC_IPADDRESS))->GetAddress(address);

	lt::SSocketCommOption commOption = {};

	_stprintf_s(commOption.remoteAddress,
				_countof(commOption.remoteAddress),
				_T("%d.%d.%d.%d"),
				(address >> 24) & 0xFF,
				(address >> 16) & 0xFF,
				(address >> 8) & 0xFF,
				(address >> 0) & 0xFF);
	commOption.sizeOfStructure = sizeof(commOption);
	commOption.remotePort = static_cast<lt::ushort>(GetDlgItemInt(IDC_EDIT_PORT));
	commOption.sizeOfRxBuffer = 1024;

	if(!m_pComm->Connect(commOption))
	{
		RemoveClient();
		return;
	}
}

void CIcsClientDlg::OnBnClickedButtonDisconnect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pComm->Disconnect();

	RemoveClient();
}

void CIcsClientDlg::OnBnClickedButtonEquipmentId()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	std::thread(&CIcsClientDlg::SendEquipmentId, this)
		.detach();
}

void CIcsClientDlg::OnBnClickedButtonOperationMode()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	std::thread(&CIcsClientDlg::SendOperationMode, this)
		.detach();
}

void CIcsClientDlg::OnBnClickedButtonLoading()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	std::thread(&CIcsClientDlg::SendLoading, this)
		.detach();
}

void CIcsClientDlg::OnBnClickedButtonRfidTestResultRequest()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	std::thread(&CIcsClientDlg::SendTestResultRequest, this)
		.detach();
}

void CIcsClientDlg::OnBnClickedButtonTestResult()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	std::thread(&CIcsClientDlg::SendTestResult, this)
		.detach();
}

void CIcsClientDlg::OnBnClickedButtonUnloading()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	std::thread(&CIcsClientDlg::SendUnloading, this)
		.detach();
}

void CIcsClientDlg::OnBnClickedButtonSocketRequest()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	std::thread(&CIcsClientDlg::SendSocketRequest, this)
		.detach();
}

void CIcsClientDlg::OnBnClickedButtonEquipmentStatus()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	std::thread(&CIcsClientDlg::SendEquipmentStatus, this)
		.detach();
}

void CIcsClientDlg::OnBnClickedButtonPortClear()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	std::thread(&CIcsClientDlg::SendPortClear, this)
		.detach();
}

void CIcsClientDlg::OnBnClickedButtonPortStatus()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	std::thread(&CIcsClientDlg::SendPortStatus, this)
		.detach();
}

void CIcsClientDlg::OnBnClickedButtonPortStatusRequest()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	std::thread(&CIcsClientDlg::SendPortStatusRequest, this)
		.detach();
}

void CIcsClientDlg::OnBnClickedButtonConveyorStatus()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	std::thread(&CIcsClientDlg::SendConveyorStatus, this)
		.detach();
}

void CIcsClientDlg::OnBnClickedButtonConveyorStatusRequest()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	std::thread(&CIcsClientDlg::SendConveyorStatusRequest, this)
		.detach();
}

template<typename TypeRes, typename... TypeArgs>
TypeRes CIcsClientDlg::InvokeClient(TypeRes (lt::CIcsClient::*fn)(TypeArgs...),
									TypeArgs&&... args)
{
	SimpleLockPtr(m_pClientMutex);

	return (m_pClient->*fn)(std::forward<TypeArgs>(args)...);
}

bool CIcsClientDlg::CreateClient(int equipmentType)
{
	SimpleLockPtr(m_pClientMutex);

	if(m_pClient != nullptr)
		return false;

	if(equipmentType == 0)
		m_pClient = new lt::CIcsClientLoUnloader();
	else if(equipmentType == 1)
		m_pClient = new lt::CIcsClientTester();
	else
		m_pClient = new lt::CIcsClient();

	if(m_pClient == nullptr)
		return false;

	m_pClient->GetTimeSyncEventNotifier() +=
		m_pTimeSyncEventListener;
	m_pClient->GetUserLevelEventNotifier() +=
		m_pUserLevelEventListener;
	m_pClient->GetLanguageEventNotifier() +=
		m_pLanguageEventListener;
	m_pClient->GetModelEventNotifier() +=
		m_pModelEventListener;
	m_pClient->GetInitializationEventNotifier() +=
		m_pInitializationEventListener;
	m_pClient->GetResetEventNotifier() +=
		m_pResetEventListener;
	m_pClient->GetRunEventNotifier() +=
		m_pRunEventListener;
	m_pClient->GetStopEventNotifier() +=
		m_pStopEventListener;
	m_pClient->GetBuzzerOffEventNotifier() +=
		m_pBuzzerOffEventListener;
	m_pClient->GetUiVisibleEventNotifier() +=
		m_pUiVisibleEventListener;
	m_pClient->GetSocketAcceptingEventNotifier() +=
		m_pSocketAcceptingEventListener;
	m_pClient->GetEjectAcceptingEventNotifier() +=
		m_pEjectAcceptingEventListener;
	m_pClient->GetProductionStartEndEventNotifier() +=
		m_pProductionStartEndEventListener;
	m_pClient->GetForceEjectEventNotifier() +=
		m_pForceEjectEventListener;

	m_pClient->SetLogDispatcher(m_pLogDispatcher);

	m_equipmentType = equipmentType;

	return true;
}

void CIcsClientDlg::RemoveClient()
{
	SimpleLockPtr(m_pClientMutex);
	if (m_pClient == nullptr)
		return;
	m_pClient->GetTimeSyncEventNotifier() -=
		m_pTimeSyncEventListener;
	m_pClient->GetUserLevelEventNotifier() -=
		m_pUserLevelEventListener;
	m_pClient->GetLanguageEventNotifier() -=
		m_pLanguageEventListener;
	m_pClient->GetModelEventNotifier() -=
		m_pModelEventListener;
	m_pClient->GetInitializationEventNotifier() -=
		m_pInitializationEventListener;
	m_pClient->GetResetEventNotifier() -=
		m_pResetEventListener;
	m_pClient->GetRunEventNotifier() -=
		m_pRunEventListener;
	m_pClient->GetStopEventNotifier() -=
		m_pStopEventListener;
	m_pClient->GetBuzzerOffEventNotifier() -=
		m_pBuzzerOffEventListener;
	m_pClient->GetUiVisibleEventNotifier() -=
		m_pUiVisibleEventListener;
	m_pClient->GetSocketAcceptingEventNotifier() -=
		m_pSocketAcceptingEventListener;
	m_pClient->GetEjectAcceptingEventNotifier() -=
		m_pEjectAcceptingEventListener;
	m_pClient->GetProductionStartEndEventNotifier() -=
		m_pProductionStartEndEventListener;
	m_pClient->GetForceEjectEventNotifier() -=
		m_pForceEjectEventListener;

	m_pClient->SetLogDispatcher(nullptr);
	delete m_pClient;
	m_pClient = nullptr;
}

void CIcsClientDlg::EnableConnectionCtrls(bool enable)
{
	GetDlgItem(IDC_IPADDRESS)->EnableWindow(enable);
	GetDlgItem(IDC_EDIT_PORT)->EnableWindow(enable);
	GetDlgItem(IDC_COMBO_EQUIPMENT)->EnableWindow(enable);
	GetDlgItem(IDC_BUTTON_CONNECT)->EnableWindow(enable);
	GetDlgItem(IDC_BUTTON_DISCONNECT)->EnableWindow(!enable);
}

void CIcsClientDlg::EnableCommunicationCtrls(bool enable)
{
	static constexpr struct
	{
		int id = 0;
		bool enable[3] = {};
	} Ctrls[] =
	{
		{ IDC_EDIT_TIME_SYNC, true, true, true },
		{ IDC_EDIT_USER_LEVEL, true, true, true },
		{ IDC_EDIT_USER_ID, true, true, true },
		{ IDC_EDIT_LANGUAGE, true, true, true },
		{ IDC_EDIT_EQUIPMENT_ID, true, true, true },
		{ IDC_BUTTON_EQUIPMENT_ID, true, true, true },
		{ IDC_EDIT_MODEL, true, true, true },
		{ IDC_EDIT_SOCKET_TYPE, true, true, true },
		{ IDC_EDIT_OPERATION_MODE, true, true, true },
		{ IDC_BUTTON_OPERATION_MODE, true, true, true },
		{ IDC_EDIT_LOADING_RFID, true, false, false },
		{ IDC_EDIT_LOADING_BARCODE, true, false, false },
		{ IDC_BUTTON_LOADING, true, false, false },
		{ IDC_EDIT_RFID_TEST_RESULT_RFID, true, false, false },
		{ IDC_EDIT_RFID_TEST_RESULT_ERROR, true, false, false },
		{ IDC_EDIT_RFID_TEST_RESULT_PARA, true, false, false },
		{ IDC_EDIT_RFID_TEST_RESULT_EQUIPMENT, true, false, false },
		{ IDC_BUTTON_RFID_TEST_RESULT_REQUEST, true, false, false },
		{ IDC_EDIT_TEST_RESULT_RFID, false, true, false },
		{ IDC_EDIT_TEST_RESULT_ERROR, false, true, false },
		{ IDC_EDIT_TEST_RESULT_PARA, false, true, false },
		{ IDC_BUTTON_TEST_RESULT, false, true, false },
		{ IDC_EDIT_UNLOADING_RFID, true, false, false },
		{ IDC_BUTTON_UNLOADING, true, false, false },
		{ IDC_EDIT_SOCKET_REQUEST_RFID, true, true, true },
		{ IDC_BUTTON_SOCKET_REQUEST, true, true, true },
		{ IDC_EDIT_SOCKET_ACCEPT_RFID, true, true, true },
		{ IDC_EDIT_SOCKET_ACCEPT_FLAG, true, true, true },
		{ IDC_EDIT_SOCKET_ACCEPT_BARCODE, true, true, true },
		{ IDC_EDIT_EJECT_ACCEPT_RFID, true, true, true },
		{ IDC_EDIT_EQUIPMENT_STATUS, true, true, true },
		{ IDC_EDIT_EQUIPMENT_STATUS_ERROR, true, true, true },
		{ IDC_EDIT_EQUIPMENT_STATUS_REASON, true, true, true },
		{ IDC_BUTTON_EQUIPMENT_STATUS, true, true, true },
		{ IDC_EDIT_PORT_CLEAR_PORT, true, true, true },
		{ IDC_BUTTON_PORT_CLEAR, true, true, true },
		{ IDC_EDIT_PORT_STATUS_PORT, true, true, true },
		{ IDC_EDIT_PORT_STATUS, true, true, true },
		{ IDC_EDIT_PORT_STATUS_RFID, true, true, true },
		{ IDC_EDIT_PORT_STATUS_BARCODE, true, true, true },
		{ IDC_BUTTON_PORT_STATUS, true, true, true },
		{ IDC_EDIT_PORT_STATUS_REQUEST_PORT, true, true, true },
		{ IDC_EDIT_PORT_STATUS_REQUEST_STATUS, true, true, true },
		{ IDC_EDIT_PORT_STATUS_REQUEST_RFID, true, true, true },
		{ IDC_EDIT_PORT_STATUS_REQUEST_BARCODE, true, true, true },
		{ IDC_BUTTON_PORT_STATUS_REQUEST, true, true, true },
		{ IDC_EDIT_CONVEYOR_STATUS_CONVEYOR, true, true, true },
		{ IDC_EDIT_CONVEYOR_STATUS1, true, true, true },
		{ IDC_EDIT_CONVEYOR_STATUS2, true, true, true },
		{ IDC_EDIT_CONVEYOR_STATUS_RFID, true, true, true },
		{ IDC_EDIT_CONVEYOR_STATUS_BARCODE, true, true, true },
		{ IDC_BUTTON_CONVEYOR_STATUS, true, true, true },
		{ IDC_EDIT_CONVEYOR_STATUS_REQUEST_CONVEYOR, true, true, true },
		{ IDC_EDIT_CONVEYOR_STATUS_REQUEST_STATUS1, true, true, true },
		{ IDC_EDIT_CONVEYOR_STATUS_REQUEST_STATUS2, true, true, true },
		{ IDC_EDIT_CONVEYOR_STATUS_REQUEST_RFID, true, true, true },
		{ IDC_EDIT_CONVEYOR_STATUS_REQUEST_BARCODE, true, true, true },
		{ IDC_BUTTON_CONVEYOR_STATUS_REQUEST, true, true, true },
		{ IDC_EDIT_FORCE_EJECT, true, true, true },
		{ IDC_EDIT_PRODUCTION_START_END, true, true, true },
		//2022.11.30a uhkim 
		{IDC_TERMINAL_MASSAGE, true, true, true },
	    {IDC_BUTTON_SVID, true, true, true }
	};

	for(auto index = 0; index < _countof(Ctrls); ++index)
	{
		auto ctrlPtr = GetDlgItem(Ctrls[index].id);

		if(ctrlPtr == nullptr)
			continue;

		ctrlPtr->EnableWindow(enable && Ctrls[index].enable[m_equipmentType]);
	}

	if(!enable)
		ClearData();
}

void CIcsClientDlg::ClearData()
{
	static constexpr int Ids[] =
	{
		IDC_EDIT_TIME_SYNC,
		IDC_EDIT_USER_LEVEL,
		IDC_EDIT_USER_ID,
		IDC_EDIT_LANGUAGE,
		IDC_EDIT_EQUIPMENT_ID,
		IDC_EDIT_MODEL,
		IDC_EDIT_SOCKET_TYPE,
		IDC_EDIT_OPERATION_MODE,
		IDC_EDIT_LOADING_RFID,
		IDC_EDIT_LOADING_BARCODE,
		IDC_EDIT_RFID_TEST_RESULT_RFID,
		IDC_EDIT_RFID_TEST_RESULT_ERROR,
		IDC_EDIT_RFID_TEST_RESULT_PARA,
		IDC_EDIT_RFID_TEST_RESULT_EQUIPMENT,
		IDC_EDIT_TEST_RESULT_RFID,
		IDC_EDIT_TEST_RESULT_ERROR,
		IDC_EDIT_TEST_RESULT_PARA,
		IDC_EDIT_UNLOADING_RFID,
		IDC_EDIT_SOCKET_REQUEST_RFID,
		IDC_EDIT_SOCKET_ACCEPT_RFID,
		IDC_EDIT_SOCKET_ACCEPT_FLAG,
		IDC_EDIT_SOCKET_ACCEPT_BARCODE,
		IDC_EDIT_EJECT_ACCEPT_RFID,
		IDC_EDIT_EQUIPMENT_STATUS,
		IDC_EDIT_EQUIPMENT_STATUS_ERROR,
		IDC_EDIT_EQUIPMENT_STATUS_REASON,
		IDC_EDIT_PORT_CLEAR_PORT,
		IDC_EDIT_PORT_STATUS_PORT,
		IDC_EDIT_PORT_STATUS,
		IDC_EDIT_PORT_STATUS_RFID,
		IDC_EDIT_PORT_STATUS_BARCODE,
		IDC_EDIT_PORT_STATUS_REQUEST_PORT,
		IDC_EDIT_PORT_STATUS_REQUEST_STATUS,
		IDC_EDIT_PORT_STATUS_REQUEST_RFID,
		IDC_EDIT_PORT_STATUS_REQUEST_BARCODE,
		IDC_EDIT_CONVEYOR_STATUS_CONVEYOR,
		IDC_EDIT_CONVEYOR_STATUS1,
		IDC_EDIT_CONVEYOR_STATUS2,
		IDC_EDIT_CONVEYOR_STATUS_RFID,
		IDC_EDIT_CONVEYOR_STATUS_BARCODE,
		IDC_EDIT_CONVEYOR_STATUS_REQUEST_CONVEYOR,
		IDC_EDIT_CONVEYOR_STATUS_REQUEST_STATUS1,
		IDC_EDIT_CONVEYOR_STATUS_REQUEST_STATUS2,
		IDC_EDIT_CONVEYOR_STATUS_REQUEST_RFID,
		IDC_EDIT_CONVEYOR_STATUS_REQUEST_BARCODE,
		IDC_EDIT_FORCE_EJECT,
		IDC_EDIT_PRODUCTION_START_END,
		//2022.11.30a uhkim
		IDC_TERMINAL_MASSAGE
	};

	for(auto index = 0; index < _countof(Ids); ++index)
	{
		auto ctrlPtr = GetDlgItem(Ids[index]);

		if(ctrlPtr == nullptr)
			continue;

		ctrlPtr->SetWindowText(nullptr);
	}
}

void CIcsClientDlg::OnLogAddedEvent(LogAddedEventArgs & eventArgs)
{
	static constexpr DWORD CharFormatMask = CFM_BOLD | CFM_FACE | CFM_COLOR | CFM_SIZE;
	static constexpr LONG CharHeight = 210;
	static constexpr struct
	{
		LPCTSTR strLogLevel = nullptr;
		CHARFORMAT charFormat = {};
	} LogLevelFormats[] =
	{
		{
			_T("None"),
			{
				sizeof(CHARFORMAT), // cbSize
				CharFormatMask, // dwMask
				0, // dwEffects
				CharHeight, // yHeight
				0, // yOffset
				RGB(100, 100, 100), // crTextColor
				0, // bCharSet
				0, // bPitchAndFamily
				_T("Tahoma") // szFaceName
			}
		},
		{
			_T("Info"),
			{
				sizeof(CHARFORMAT), // cbSize
				CharFormatMask, // dwMask
				0, // dwEffects
				CharHeight, // yHeight
				0, // yOffset
				RGB(100, 100, 100), // crTextColor
				0, // bCharSet
				0, // bPitchAndFamily
				_T("Tahoma") // szFaceName
			}
		},
		{
			_T("Proc"),
			{
				sizeof(CHARFORMAT), // cbSize
				CharFormatMask, // dwMask
				0, // dwEffects
				CharHeight, // yHeight
				0, // yOffset
				0, // crTextColor
				0, // bCharSet
				0, // bPitchAndFamily
				_T("Tahoma") // szFaceName
			}
		},
		{
			_T("Noti"),
			{
				sizeof(CHARFORMAT), // cbSize
				CharFormatMask, // dwMask
				0, // dwEffects
				CharHeight, // yHeight
				0, // yOffset
				RGB(100, 60, 20), // crTextColor
				0, // bCharSet
				0, // bPitchAndFamily
				_T("Tahoma") // szFaceName
			}
		},
		{
			_T("Warn"),
			{
				sizeof(CHARFORMAT), // cbSize
				CharFormatMask, // dwMask
				CFE_BOLD, // dwEffects
				CharHeight, // yHeight
				0, // yOffset
				0, // crTextColor
				0, // bCharSet
				0, // bPitchAndFamily
				_T("Tahoma") // szFaceName
			}
		},
		{
			_T("Erro"),
			{
				sizeof(CHARFORMAT), // cbSize
				CharFormatMask, // dwMask
				CFE_BOLD, // dwEffects
				CharHeight, // yHeight
				0, // yOffset
				RGB(255, 0, 0), // crTextColor
				0, // bCharSet
				0, // bPitchAndFamily
				_T("Tahoma") // szFaceName
			}
		},
		{
			_T("Pass"),
			{
				sizeof(CHARFORMAT), // cbSize
				CharFormatMask, // dwMask
				0, // dwEffects
				CharHeight, // yHeight
				0, // yOffset
				RGB(0, 0, 255), // crTextColor
				0, // bCharSet
				0, // bPitchAndFamily
				_T("Tahoma") // szFaceName
			}
		},
		{
			_T("Fail"),
			{
				sizeof(CHARFORMAT), // cbSize
				CharFormatMask, // dwMask
				0, // dwEffects
				CharHeight, // yHeight
				0, // yOffset
				RGB(255, 0, 0), // crTextColor
				0, // bCharSet
				0, // bPitchAndFamily
				_T("Tahoma") // szFaceName
			}
		}
	};

	auto & logData = eventArgs.GetArgs();
	auto & loggingTime = logData.GetLoggingTime();
	auto & logLevelFormat = LogLevelFormats[static_cast<int>(logData.GetLogLevel())];
	CString strLog;

	strLog.Format(_T("%02d:%02d:%02d.%03d (%s) %s\r\n"),
		loggingTime.wHour, loggingTime.wMinute,
		loggingTime.wSecond, loggingTime.wMilliseconds,
		logLevelFormat.strLogLevel,
		logData.GetLogString());

	m_pRichEditCtrl->SetSel(-1, -1);
	m_pRichEditCtrl->SetSelectionCharFormat(CHARFORMAT(logLevelFormat.charFormat));
	m_pRichEditCtrl->ReplaceSel(strLog);
}

void CIcsClientDlg::OnCommConnectedEvent(CommConnectionEventArgs & eventArgs)
{
	SimpleLockPtr(m_pClientMutex);

	if(m_pClient == nullptr)
		return;

	auto & tcpCommOption = reinterpret_cast<const lt::SSocketCommOption &>(eventArgs.GetArgs());

	m_pLogger->SetLogLevel(lt::LogLevel::Notice)
		.AddLog(_T("%s:%d connected"),
				tcpCommOption.remoteAddress,
				tcpCommOption.remotePort);

	m_pClient->Attach(std::addressof(eventArgs.GetSender()));

	EnableConnectionCtrls(false);
	EnableCommunicationCtrls(true);
}


void CIcsClientDlg::SendEquipmentId()
{
	CString strEquipmentId;

	GetDlgItemText(IDC_EDIT_EQUIPMENT_ID, strEquipmentId);

	lt::SEquipmentId equipmentId =
	{
		strEquipmentId
	};

	InvokeClient(&lt::CIcsClient::CommandEquipmentId,
				 std::forward<const lt::SEquipmentId &>(equipmentId),
				 std::forward<lt::uint64>(1000));
}

void CIcsClientDlg::SendOperationMode()
{
	lt::SOperationMode operationMode =
	{
		static_cast<lt::uint32>(GetDlgItemInt(IDC_EDIT_OPERATION_MODE))
	};

	InvokeClient(&lt::CIcsClient::CommandOperationMode,
				 std::forward<const lt::SOperationMode &>(operationMode),
				 std::forward<lt::uint64>(5000));
}

void CIcsClientDlg::SendSocketRequest()
{
	CString strRfid;

	GetDlgItemText(IDC_EDIT_SOCKET_REQUEST_RFID, strRfid);

	lt::SSocketRfid socketRfid =
	{
		strRfid
	};

	InvokeClient(&lt::CIcsClient::CommandSocketRequest,
				 std::forward<const lt::SSocketRfid &>(socketRfid),
				 std::forward<lt::uint64>(1000));
}

void CIcsClientDlg::SendEquipmentStatus()
{
	CString strReason;

	GetDlgItemText(IDC_EDIT_EQUIPMENT_STATUS_REASON, strReason);

	lt::SEquipmentStatus equipmentStatus =
	{
		static_cast<lt::uint32>(GetDlgItemInt(IDC_EDIT_EQUIPMENT_STATUS)),
		static_cast<lt::int32>(GetDlgItemInt(IDC_EDIT_EQUIPMENT_STATUS_ERROR)),
		strReason
	};

	InvokeClient(&lt::CIcsClient::CommandEquipmentStatus,
				 std::forward<const lt::SEquipmentStatus &>(equipmentStatus),
				 std::forward<lt::uint64>(1000));
}

void CIcsClientDlg::SendPortClear()
{
	lt::SPort port =
	{
		static_cast<lt::uint32>(GetDlgItemInt(IDC_EDIT_PORT_CLEAR_PORT))
	};

	InvokeClient(&lt::CIcsClient::CommandPortClear,
				 std::forward<const lt::SPort &>(port),
				 std::forward<lt::uint64>(1000));
}

void CIcsClientDlg::SendPortStatus()
{
	CString strRfid;
	CString strProductSn;

	GetDlgItemText(IDC_EDIT_PORT_STATUS_RFID, strRfid);
	GetDlgItemText(IDC_EDIT_PORT_STATUS_BARCODE, strProductSn);

	lt::SPortStatus portStatus =
	{
		static_cast<lt::uint32>(GetDlgItemInt(IDC_EDIT_PORT_STATUS_PORT)),
		static_cast<lt::uint32>(GetDlgItemInt(IDC_EDIT_PORT_STATUS)),
		strRfid,
		strProductSn
	};

	InvokeClient(&lt::CIcsClient::CommandPortStatus,
				 std::forward<const lt::SPortStatus &>(portStatus),
				 std::forward<lt::uint64>(1000));
}

void CIcsClientDlg::SendPortStatusRequest()
{
	lt::SPortStatus2 portStatus =
	{
		static_cast<lt::uint32>(GetDlgItemInt(IDC_EDIT_PORT_STATUS_REQUEST_PORT))
	};

	InvokeClient(&lt::CIcsClient::CommandPortStatusRequest,
				 std::forward<lt::SPortStatus2 &>(portStatus),
				 std::forward<lt::uint64>(1000));

	SetDlgItemInt(IDC_EDIT_PORT_STATUS_REQUEST_STATUS, portStatus.status);
	SetDlgItemText(IDC_EDIT_PORT_STATUS_REQUEST_RFID, portStatus.socketInfo.rfid);
	SetDlgItemText(IDC_EDIT_PORT_STATUS_REQUEST_BARCODE, portStatus.socketInfo.productSn);
}

void CIcsClientDlg::SendConveyorStatus()
{
	CString strRfid;
	CString strProductSn;

	GetDlgItemText(IDC_EDIT_CONVEYOR_STATUS_RFID, strRfid);
	GetDlgItemText(IDC_EDIT_CONVEYOR_STATUS_BARCODE, strProductSn);

	lt::SConveyorStatus conveyorStatus =
	{
		static_cast<lt::uint32>(GetDlgItemInt(IDC_EDIT_CONVEYOR_STATUS_CONVEYOR)),
		static_cast<lt::uint32>(GetDlgItemInt(IDC_EDIT_CONVEYOR_STATUS1)),
		static_cast<lt::uint32>(GetDlgItemInt(IDC_EDIT_CONVEYOR_STATUS2)),
		strRfid,
		strProductSn
	};

	InvokeClient(&lt::CIcsClient::CommandConveyorStatus,
				 std::forward<const lt::SConveyorStatus &>(conveyorStatus),
				 std::forward<lt::uint64>(1000));
}

void CIcsClientDlg::SendConveyorStatusRequest()
{
	lt::SConveyorStatus2 conveyorStatus =
	{
		static_cast<lt::uint32>(GetDlgItemInt(IDC_EDIT_CONVEYOR_STATUS_REQUEST_CONVEYOR)),
	};

	InvokeClient(&lt::CIcsClient::CommandConveyorStatusRequest,
				 std::forward<lt::SConveyorStatus2 &>(conveyorStatus),
				 std::forward<lt::uint64>(1000));

	SetDlgItemInt(IDC_EDIT_CONVEYOR_STATUS_REQUEST_STATUS1, conveyorStatus.status1);
	SetDlgItemInt(IDC_EDIT_CONVEYOR_STATUS_REQUEST_STATUS2, conveyorStatus.status2);
	SetDlgItemText(IDC_EDIT_CONVEYOR_STATUS_REQUEST_RFID, conveyorStatus.socketInfo.rfid);
	SetDlgItemText(IDC_EDIT_CONVEYOR_STATUS_REQUEST_BARCODE, conveyorStatus.socketInfo.productSn);
}

void CIcsClientDlg::OnCommDisconnectingEvent(CommConnectionEventArgs & eventArgs)
{
	SimpleLockPtr(m_pClientMutex);

	if(m_pClient == nullptr)
		return;

	EnableConnectionCtrls(true);
	EnableCommunicationCtrls(false);

	auto & tcpCommOption = reinterpret_cast<const lt::SSocketCommOption &>(eventArgs.GetArgs());

	m_pLogger->SetLogLevel(lt::LogLevel::Notice)
		.AddLog(_T("%s:%d disconnected"),
				tcpCommOption.remoteAddress,
				tcpCommOption.remotePort);

	m_pClient->Detach();
}

void CIcsClientDlg::OnTimeSyncEvent(TimeSyncEventArgs & eventArgs)
{
	auto & args = eventArgs.GetArgs();
	auto & reqArgs = args.GetTypedArgs();

	m_pLogger->SetLogLevel(lt::LogLevel::Notice)
		.AddLog(_T("Time Sync (year = %d, month = %02d, day = %02d, hour = %02d, minute = %02d, second = %02d)"),
				reqArgs.year,
				reqArgs.month,
				reqArgs.day,
				reqArgs.hour,
				reqArgs.minute,
				reqArgs.second);

	CString strTime;

	strTime.Format(_T("%d/%02d/%02d %02d:%02d:%02d"),
				   reqArgs.year,
				   reqArgs.month,
				   reqArgs.day,
				   reqArgs.hour,
				   reqArgs.minute,
				   reqArgs.second);
	SetDlgItemText(IDC_EDIT_TIME_SYNC, strTime);

	args.SetResult(true);
	eventArgs.Cancel();
}

void CIcsClientDlg::OnUserLevelEvent(UserLevelEventArgs & eventArgs)
{
	auto & args = eventArgs.GetArgs();
	auto & reqArgs = args.GetTypedArgs();

	m_pLogger->SetLogLevel(lt::LogLevel::Notice)
		.AddLog(_T("User Level (user level = %d), (user id = %s)"),
				reqArgs.userLevel,
				reqArgs.userId);

	SetDlgItemInt(IDC_EDIT_USER_LEVEL, reqArgs.userLevel);
	SetDlgItemText(IDC_EDIT_USER_ID, reqArgs.userId);

	args.SetResult(true);
	eventArgs.Cancel();
}

void CIcsClientDlg::OnLanguageEvent(LanguageEventArgs & eventArgs)
{
	auto & args = eventArgs.GetArgs();
	auto & reqArgs = args.GetTypedArgs();

	m_pLogger->SetLogLevel(lt::LogLevel::Notice)
		.AddLog(_T("Language (language = %d)"),
				reqArgs.language);

	SetDlgItemInt(IDC_EDIT_LANGUAGE, reqArgs.language);

	args.SetResult(true);
	eventArgs.Cancel();
}

void CIcsClientDlg::OnModelEvent(ModelEventArgs & eventArgs)
{
	auto & args = eventArgs.GetArgs();
	auto & reqArgs = args.GetTypedArgs();

	m_pLogger->SetLogLevel(lt::LogLevel::Notice)
		.AddLog(_T("Model (model = %d, socket type = %d)"),
				reqArgs.model,
				reqArgs.socketType);

	SetDlgItemInt(IDC_EDIT_MODEL, reqArgs.model);
	SetDlgItemInt(IDC_EDIT_SOCKET_TYPE, reqArgs.socketType);

	args.SetResult(true);
	eventArgs.Cancel();
}

void CIcsClientDlg::OnInitializationEvent(InitializationEventArgs & eventArgs)
{
	auto & args = eventArgs.GetArgs();

	m_pLogger->SetLogLevel(lt::LogLevel::Notice)
		.AddLog(_T("Initialization"));

	args.SetResult(true);
	eventArgs.Cancel();
}

void CIcsClientDlg::OnResetEvent(ResetEventArgs & eventArgs)
{
	auto & args = eventArgs.GetArgs();

	m_pLogger->SetLogLevel(lt::LogLevel::Notice)
		.AddLog(_T("Reset"));

	args.SetResult(true);
	eventArgs.Cancel();
}

void CIcsClientDlg::OnRunEvent(RunEventArgs & eventArgs)
{
	auto & args = eventArgs.GetArgs();

	m_pLogger->SetLogLevel(lt::LogLevel::Notice)
		.AddLog(_T("Run"));

	SetDlgItemInt(IDC_EDIT_OPERATION_MODE, 0);

	OnBnClickedButtonOperationMode();

	args.SetResult(true);
	eventArgs.Cancel();
}

void CIcsClientDlg::OnStopEvent(StopEventArgs & eventArgs)
{
	auto & args = eventArgs.GetArgs();

	m_pLogger->SetLogLevel(lt::LogLevel::Notice)
		.AddLog(_T("Stop"));

	SetDlgItemInt(IDC_EDIT_OPERATION_MODE, 1);

	OnBnClickedButtonOperationMode();

	args.SetResult(true);
	eventArgs.Cancel();
}

void CIcsClientDlg::OnBuzzerOffEvent(BuzzerOffEventArgs & eventArgs)
{
	auto & args = eventArgs.GetArgs();

	m_pLogger->SetLogLevel(lt::LogLevel::Notice)
		.AddLog(_T("Buzzer off"));

	args.SetResult(true);
	eventArgs.Cancel();
}

void CIcsClientDlg::OnUiVisibleEvent(UiVisibleEventArgs & eventArgs)
{
	auto & args = eventArgs.GetArgs();
	auto & reqArgs = args.GetTypedArgs();

	m_pLogger->SetLogLevel(lt::LogLevel::Notice)
		.AddLog(_T("Ui visible (cmdShow = %d)"),
				reqArgs.cmdShow);

	ShowWindow(reqArgs.cmdShow);

	args.SetResult(true);
	eventArgs.Cancel();
}

void CIcsClientDlg::OnSocketAcceptingEvent(SocketAcceptingEventArgs & eventArgs)
{
	auto & args = eventArgs.GetArgs();
	auto & reqArgs = args.GetTypedArgs();

	m_pLogger->SetLogLevel(lt::LogLevel::Notice)
		.AddLog(_T("Socket accepting (rfid = %s, flag = %d, info = %s)"),
				reqArgs.rfid,
				reqArgs.flag,
				reqArgs.info);

	SetDlgItemText(IDC_EDIT_SOCKET_ACCEPT_RFID, reqArgs.rfid);
	SetDlgItemInt(IDC_EDIT_SOCKET_ACCEPT_FLAG, reqArgs.flag);
	SetDlgItemText(IDC_EDIT_SOCKET_ACCEPT_BARCODE, reqArgs.info);

	args.SetResult(true);
	eventArgs.Cancel();
}

void CIcsClientDlg::OnEjectAcceptingEvent(EjectAcceptingEventArgs & eventArgs)
{
	auto & args = eventArgs.GetArgs();
	auto & reqArgs = args.GetTypedArgs();

	m_pLogger->SetLogLevel(lt::LogLevel::Notice)
		.AddLog(_T("Eject accepting (rfid = %s)"),
				reqArgs.rfid);

	SetDlgItemText(IDC_EDIT_EJECT_ACCEPT_RFID, reqArgs.rfid);

	args.SetResult(true);
	eventArgs.Cancel();
}

void CIcsClientDlg::OnProductionStartEndEvent(ProductionStartEndEventArgs & eventArgs)
{
	auto & args = eventArgs.GetArgs();
	auto & reqArgs = args.GetTypedArgs();

	m_pLogger->SetLogLevel(lt::LogLevel::Notice)
		.AddLog(_T("Production start/end (flag = %d)"),
				reqArgs.flag);

	SetDlgItemInt(IDC_EDIT_PRODUCTION_START_END, reqArgs.flag);

	args.SetResult(true);
	eventArgs.Cancel();
}

void CIcsClientDlg::OnForceEjectEvent(ForceEjectEventArgs & eventArgs)
{
	auto & args = eventArgs.GetArgs();
	auto & reqArgs = args.GetTypedArgs();

	m_pLogger->SetLogLevel(lt::LogLevel::Notice)
		.AddLog(_T("Force eject (onOff = %d)"),
				reqArgs.onOff);

	SetDlgItemInt(IDC_EDIT_FORCE_EJECT, reqArgs.onOff);

	args.SetResult(true);
	eventArgs.Cancel();
}


void CIcsClientDlg::SendLoading()
{
	CString strRfid;
	CString strProductSn;

	GetDlgItemText(IDC_EDIT_LOADING_RFID, strRfid);
	GetDlgItemText(IDC_EDIT_LOADING_BARCODE, strProductSn);

	lt::SSocketInfo socketInfo =
	{
		strRfid,
		strProductSn
	};

	InvokeClient(reinterpret_cast<
				 lt::uint32 (lt::CIcsClient::*)(const lt::SSocketInfo &, lt::uint64)>(
					 &lt::CIcsClientLoUnloader::CommandLoading),
				 std::forward<const lt::SSocketInfo &>(socketInfo),
				 std::forward<lt::uint64>(1000));
}

void CIcsClientDlg::SendTestResultRequest()
{
	CString strRfid;

	GetDlgItemText(IDC_EDIT_RFID_TEST_RESULT_RFID, strRfid);

	lt::STestResult2 testResult =
	{
		strRfid
	};

	InvokeClient(reinterpret_cast<
				 lt::uint32 (lt::CIcsClient::*)(lt::STestResult2 &, lt::uint64)>(
					 &lt::CIcsClientLoUnloader::CommandTestResultRequest),
				 std::forward<lt::STestResult2 &>(testResult),
				 std::forward<lt::uint64>(1000));

	SetDlgItemInt(IDC_EDIT_RFID_TEST_RESULT_ERROR, testResult.result);
	SetDlgItemInt(IDC_EDIT_RFID_TEST_RESULT_PARA, testResult.para);
	SetDlgItemText(IDC_EDIT_RFID_TEST_RESULT_EQUIPMENT, testResult.equipmentName);
}

void CIcsClientDlg::SendUnloading()
{
	CString strRfid;

	GetDlgItemText(IDC_EDIT_UNLOADING_RFID, strRfid);

	lt::SSocketRfid socketRfid =
	{
		strRfid
	};

	InvokeClient(reinterpret_cast<
				 lt::uint32 (lt::CIcsClient::*)(const lt::SSocketRfid &, lt::uint64)>(
					 &lt::CIcsClientLoUnloader::CommandUnloading),
				 std::forward<const lt::SSocketRfid &>(socketRfid),
				 std::forward<lt::uint64>(1000));
}

void CIcsClientDlg::SendTestResult()
{
	CString strRfid;

	GetDlgItemText(IDC_EDIT_TEST_RESULT_RFID, strRfid);

	lt::STestResult testResult =
	{
		strRfid,
		static_cast<lt::int32>(GetDlgItemInt(IDC_EDIT_TEST_RESULT_ERROR)),
		static_cast<lt::uint32>(GetDlgItemInt(IDC_EDIT_TEST_RESULT_PARA))
	};

	InvokeClient(reinterpret_cast<
				 lt::uint32 (lt::CIcsClient::*)(const lt::STestResult &, lt::uint64)>(
					 &lt::CIcsClientTester::CommandTestResult),
				 std::forward<const lt::STestResult &>(testResult),
				 std::forward<lt::uint64>(1000));
}

