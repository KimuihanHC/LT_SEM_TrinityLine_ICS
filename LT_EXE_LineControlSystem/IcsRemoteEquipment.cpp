#include "stdafx.h"
#include "IcsRemoteEquipment.h"
#include "Equipment.h"
#include "Logger/DebugLogDispatcher.h"
#include "Logger/Logger.h"
#include "Comm/IcsRemote.h"
#include "Event/EventMfnListener.hpp"
#include "Util/StringUtil.hpp"

CIcsRemoteEquipment::CIcsRemoteEquipment(CEquipment & equipment)
: m_equipment(equipment)
{
	// Logger
	m_pDebugLogDispatcher =
		new lt::CDebugLogDispatcher(m_equipment.Get_Alias(), lt::LogFilter::Detail);
	m_pLogger = new lt::CLogger();

	static_cast<lt::ILogDispatcher *>(*m_pDebugLogDispatcher)->Dispatch(
		lt::LogLevel::None,
		lt::Format(_T("============================= %s begins ============================="),
				   m_equipment.Get_Alias()).c_str()
	);
	m_pLogger->Attach(*m_pDebugLogDispatcher);

	// Remote
	m_pEquipmentIdEventListener =
		new EquipmentIdEventListener(this, &CIcsRemoteEquipment::OnEquipmentIdEvent);
	m_pEquipmentStatusEventListener =
		new EquipmentStatusEventListener(this, &CIcsRemoteEquipment::OnEquipmentStatusEvent);
	m_pOperationModeEventListener =
		new OperationModeEventListener(this, &CIcsRemoteEquipment::OnOperationModeEvent);
	m_pSocketRequestEventListener =
		new SocketRequestEventListener(this, &CIcsRemoteEquipment::OnSocketRequestEvent);
	m_pPortClearEventListener =
		new PortClearEventListener(this, &CIcsRemoteEquipment::OnPortClearEvent);
	m_pPortStatusEventListener =
		new PortStatusEventListener(this, &CIcsRemoteEquipment::OnPortStatusEvent);
	m_pPortStatusRequestEventListener =
		new PortStatusRequestEventListener(this, &CIcsRemoteEquipment::OnPortStatusRequestEvent);
	m_pConveyorStatusEventListener =
		new ConveyorStatusEventListener(this, &CIcsRemoteEquipment::OnConveyorStatusEvent);
	m_pConveyorStatusRequestEventListener =
		new ConveyorStatusRequestEventListener(this, &CIcsRemoteEquipment::OnConveyorStatusRequestEvent);
}

CIcsRemoteEquipment::~CIcsRemoteEquipment()
{
	// Remote
	delete m_pEquipmentIdEventListener;
	delete m_pEquipmentStatusEventListener;
	delete m_pOperationModeEventListener;
	delete m_pSocketRequestEventListener;
	delete m_pPortClearEventListener;
	delete m_pPortStatusEventListener;
	delete m_pPortStatusRequestEventListener;
	delete m_pConveyorStatusEventListener;
	delete m_pConveyorStatusRequestEventListener;

	// Logger
	m_pLogger->Detach();
	static_cast<lt::ILogDispatcher *>(*m_pDebugLogDispatcher)->Dispatch(
		lt::LogLevel::None,
		lt::Format(_T("============================= %s ends ============================="),
			m_equipment.Get_Alias()).c_str()
	);
	Sleep(100);

	delete m_pLogger;
	delete m_pDebugLogDispatcher;
}

bool CIcsRemoteEquipment::operator==(const lt::ICommunicator * commPtr) const
{
	return GetRemote() == commPtr;
}

bool CIcsRemoteEquipment::operator!=(const lt::ICommunicator * commPtr) const
{
	return GetRemote() != commPtr;
}

lt::ICommunicator * CIcsRemoteEquipment::Attach(lt::ICommunicator * commPtr)
{
	GetRemote().GetEquipmentIdEventNotifier() +=
		m_pEquipmentIdEventListener;
	GetRemote().GetEquipmentStatusEventNotifier() +=
		m_pEquipmentStatusEventListener;
	GetRemote().GetOperationModeEventNotifier() +=
		m_pOperationModeEventListener;
	GetRemote().GetSocketRequestEventNotifier() +=
		m_pSocketRequestEventListener;
	GetRemote().GetPortClearEventNotifier() +=
		m_pPortClearEventListener;
	GetRemote().GetPortStatusEventNotifier() +=
		m_pPortStatusEventListener;
	GetRemote().GetPortStatusRequestEventNotifier() +=
		m_pPortStatusRequestEventListener;
	GetRemote().GetConveyorStatusEventNotifier() +=
		m_pConveyorStatusEventListener;
	GetRemote().GetConveyorStatusRequestEventNotifier() +=
		m_pConveyorStatusRequestEventListener;
	GetRemote().SetLogDispatcher(*m_pDebugLogDispatcher);

	auto _commPtr = GetRemote().Attach(commPtr);

	GetLogger()
		.SetLogLevel(lt::LogLevel::Notice)
		.AddLog(_T("The communicator(%p) attached"),
				commPtr);

	GetEquipment().Set_ClientConnection(IsConnected());

	return _commPtr;
}

lt::ICommunicator * CIcsRemoteEquipment::Detach()
{
	auto commPtr = GetRemote().Detach();

	GetLogger()
		.SetLogLevel(lt::LogLevel::Notice)
		.AddLog(_T("The communicator(%p) detached"),
				commPtr);

	GetEquipment().Set_ClientConnection(IsConnected());

	GetRemote().GetEquipmentIdEventNotifier() -=
		m_pEquipmentIdEventListener;
	GetRemote().GetEquipmentStatusEventNotifier() -=
		m_pEquipmentStatusEventListener;
	GetRemote().GetOperationModeEventNotifier() -=
		m_pOperationModeEventListener;
	GetRemote().GetSocketRequestEventNotifier() -=
		m_pSocketRequestEventListener;
	GetRemote().GetPortClearEventNotifier() -=
		m_pPortClearEventListener;
	GetRemote().GetPortStatusEventNotifier() -=
		m_pPortStatusEventListener;
	GetRemote().GetPortStatusRequestEventNotifier() -=
		m_pPortStatusRequestEventListener;
	GetRemote().GetConveyorStatusEventNotifier() -=
		m_pConveyorStatusEventListener;
	GetRemote().GetConveyorStatusRequestEventNotifier() -=
		m_pConveyorStatusRequestEventListener;
	GetRemote().SetLogDispatcher(nullptr);

	return commPtr;
}

bool CIcsRemoteEquipment::IsConnected() const
{
	return GetRemote().IsConnected();
}

const CConfig_Eqp & CIcsRemoteEquipment::GetConfig() const
{
	return GetEquipment();
}

CEquipment & CIcsRemoteEquipment::GetEquipment()
{
	return m_equipment;
}

const CEquipment & CIcsRemoteEquipment::GetEquipment() const
{
	return m_equipment;
}

lt::CLogger & CIcsRemoteEquipment::GetLogger()
{
	return *m_pLogger;
}

lt::ILoggable & CIcsRemoteEquipment::GetLogger() const
{
	return *m_pLogger;
}

lt::uint32 CIcsRemoteEquipment::SendTimeSync()
{
	static constexpr lt::uint64 Timeout = 1000;

	SYSTEMTIME stime = {};

	::GetLocalTime(&stime);

	auto result = GetRemote().CommandTimeSync(
		{
			stime.wYear,
			stime.wMonth,
			stime.wDay,
			stime.wHour,
			stime.wMinute,
			stime.wSecond
		},
		Timeout
	);

	if(result != ICS_COMM_OK)
	{
		GetLogger()
			.SetLogLevel(lt::LogLevel::Error)
			.AddLog(_T("lt::CIcsRemote::CommandTimeSync("
					   "{ %d, %d, %d, %d, %d, %d }, %d) failed %d"),
					stime.wYear, stime.wMonth, stime.wDay,
					stime.wHour, stime.wMinute, stime.wSecond,
					Timeout,
					result);
	}

	return result;
}

lt::uint32 CIcsRemoteEquipment::SendUserLevel(enPermissionMode mode, lt::ConstStringT userId)
{
	static constexpr lt::uint64 Timeout = 1000;

	auto result = GetRemote().CommandUserLevel(
		{
			static_cast<lt::uint32>(mode),
			userId
		},
		Timeout
	);

	if(result != ICS_COMM_OK)
	{
		GetLogger()
			.SetLogLevel(lt::LogLevel::Error)
			.AddLog(_T("lt::CIcsRemote::CommandUserLevel({ %d, %s }, %d) failed %d"),
					mode,
					userId,
					Timeout,
					result);
	}

	return result;
}

lt::uint32 CIcsRemoteEquipment::SendLanguage(lt::uint32 language)
{
	static constexpr lt::uint64 Timeout = 1000;

	auto result = GetRemote().CommandLanguage(
		{
			language
		},
		Timeout
	);

	if(result != ICS_COMM_OK)
	{
		GetLogger()
			.SetLogLevel(lt::LogLevel::Error)
			.AddLog(_T("lt::CIcsRemote::CommandLanguage({ %d }, %d) failed %d"),
					language,
					Timeout,
					result);
	}

	return result;
}

lt::uint32 CIcsRemoteEquipment::SendModel(lt::uint32 model,
										  lt::uint32 socketType)
{
	return GetRemote().CommandModel(
		{
			model,
			socketType
		},
		1000
	);
}

lt::uint32 CIcsRemoteEquipment::SendInitialization()
{
	static constexpr lt::uint64 Timeout = 1000;

	auto result = GetRemote().CommandInitialization(
		Timeout
	);

	if(result != ICS_COMM_OK)
	{
		GetLogger()
			.SetLogLevel(lt::LogLevel::Error)
			.AddLog(_T("lt::CIcsRemote::CommandInitialization(%d) failed %d"),
					Timeout,
					result);
	}

	return result;
}

lt::uint32 CIcsRemoteEquipment::SendReset()
{
	static constexpr lt::uint64 Timeout = 1000;

	auto result = GetRemote().CommandReset(
		Timeout
	);

	if(result != ICS_COMM_OK)
	{
		GetLogger()
			.SetLogLevel(lt::LogLevel::Error)
			.AddLog(_T("lt::CIcsRemote::CommandReset(%d) failed %d"),
					Timeout,
					result);
	}

	return result;
}

lt::uint32 CIcsRemoteEquipment::SendRun()
{
	static constexpr lt::uint64 Timeout = 1000;

	auto result = GetRemote().CommandRun(
		Timeout
	);

	if(result != ICS_COMM_OK)
	{
		GetLogger()
			.SetLogLevel(lt::LogLevel::Error)
			.AddLog(_T("lt::CIcsRemote::CommandRun(%d) failed %d"),
					Timeout,
					result);
	}

	return result;
}

lt::uint32 CIcsRemoteEquipment::SendStop()
{
	static constexpr lt::uint64 Timeout = 1000;

	auto result = GetRemote().CommandStop(
		Timeout
	);

	if(result != ICS_COMM_OK)
	{
		GetLogger()
			.SetLogLevel(lt::LogLevel::Error)
			.AddLog(_T("lt::CIcsRemote::CommandStop(%d) failed %d"),
					Timeout,
					result);
	}

	return result;
}

lt::uint32 CIcsRemoteEquipment::SendBuzzerOff()
{
	static constexpr lt::uint64 Timeout = 1000;

	auto result = GetRemote().CommandBuzzerOff(
		Timeout
	);

	if(result != ICS_COMM_OK)
	{
		GetLogger()
			.SetLogLevel(lt::LogLevel::Error)
			.AddLog(_T("lt::CIcsRemote::CommandBuzzerOff(%d) failed %d"),
					Timeout,
					result);
	}

	return result;
}

lt::uint32 CIcsRemoteEquipment::SendUiVisible(int cmdShow)
{
	static constexpr lt::uint64 Timeout = 1000;

	auto result = GetRemote().CommandUiVisible(
		{
			cmdShow
		},
		Timeout
	);

	if(result != ICS_COMM_OK)
	{
		GetLogger()
			.SetLogLevel(lt::LogLevel::Error)
			.AddLog(_T("lt::CIcsRemote::CommandUiVisible({ %d }, %d) failed %d"),
					cmdShow,
					Timeout,
					result);
	}

	return result;
}

lt::uint32 CIcsRemoteEquipment::SendOperationActiveStatus(lt::uint32 status)
{
	static constexpr lt::uint64 Timeout = 1000;

	auto result = GetRemote().CommandOperationActiveStatus(
		{
			status
		},
		Timeout
	);

	if (result != ICS_COMM_OK)
	{
		GetLogger()
			.SetLogLevel(lt::LogLevel::Error)
			.AddLog(_T("lt::CIcsRemote::CommandOperationActiveStatus({ %d }, %d) failed %d"),
				status,
				Timeout,
				result);
	}

	return result;
}

lt::uint32 CIcsRemoteEquipment::SendSocketAccept(lt::ConstStringT rfid,
												 lt::uint32 flag,
												 lt::ConstStringT info)
{
	static constexpr lt::uint64 Timeout = 1000;

	auto result = GetRemote().CommandSocketAccepting(
		{
			rfid,
			flag,
			info
		},
		Timeout
	);

	if(result != ICS_COMM_OK)
	{
		GetLogger()
			.SetLogLevel(lt::LogLevel::Error)
			.AddLog(_T("lt::CIcsRemote::CommandSocketAccepting("
					   "{ %s, %d, %s }, %d) failed %d"),
					rfid, flag, info,
					Timeout,
					result);
	}

	return result;
}

lt::uint32 CIcsRemoteEquipment::SendEjectAccept(lt::ConstStringT rfid)
{
	static constexpr lt::uint64 Timeout = 1000;

	auto result = GetRemote().CommandEjectAccepting(
		{
			rfid
		},
		Timeout
	);

	if(result != ICS_COMM_OK)
	{
		GetLogger()
			.SetLogLevel(lt::LogLevel::Error)
			.AddLog(_T("lt::CIcsRemote::CommandEjectAccepting({ %s }, %d) failed %d"),
					rfid,
					Timeout,
					result);
	}

	return result;
}

lt::uint32 CIcsRemoteEquipment::SendProductionStartEnd(lt::uint32 flag)
{
	static constexpr lt::uint64 Timeout = 1000;

	auto result = GetRemote().CommandProductionStartEnd(
		{
			flag
		},
		Timeout
	);

	if(result != ICS_COMM_OK)
	{
		GetLogger()
			.SetLogLevel(lt::LogLevel::Error)
			.AddLog(_T("lt::CIcsRemote::CommandProductionStartEnd({ %d }, %d) failed %d"),
					flag,
					Timeout,
					result);
	}

	return result;
}

lt::uint32 CIcsRemoteEquipment::SendForceEject(bool onOff)
{
	static constexpr lt::uint64 Timeout = 1000;

	auto result = GetRemote().CommandForceEject(
		{
			static_cast<lt::uint32>(onOff)
		},
		Timeout
	);

	if(result != ICS_COMM_OK)
	{
		GetLogger()
			.SetLogLevel(lt::LogLevel::Error)
			.AddLog(_T("lt::CIcsRemote::CommandForceEject({ %d }, %d) failed %d"),
					onOff,
					Timeout,
					result);
	}

	return result;
}

void CIcsRemoteEquipment::OnEquipmentIdEvent(EquipmentIdEventArgs & eventArgs)
{
	auto & args = eventArgs.GetArgs();
	auto & reqArgs = args.GetTypedArgs();
	auto verified = GetEquipment().Get_EquipmentId().Compare(reqArgs.equipmentId) == 0;

	GetLogger()
		.SetLogLevel(lt::LogLevel::Notice)
		.AddLog(_T("Equipment Id verification %s (equipment id = %s, received = %s)"),
				verified ? _T("succeeded") : _T("failed"),
				GetEquipment().Get_EquipmentId(),
				reqArgs.equipmentId);

	GetEquipment().Set_VerifyEqpConnection(verified);

	args.SetResult(verified);
	eventArgs.Cancel();
}

void CIcsRemoteEquipment::OnEquipmentStatusEvent(EquipmentStatusEventArgs & eventArgs)
{
	auto & args = eventArgs.GetArgs();
	auto & reqArgs = args.GetTypedArgs();

	GetLogger()
		.SetLogLevel(lt::LogLevel::Notice)
		.AddLog(_T("Equipment Status (status = %d, alarm = %d, info = %s)"),
				reqArgs.status,
				reqArgs.alarmCode,
				reqArgs.info);

	GetEquipment().Set_ProcessStatus(reqArgs.status, reqArgs.alarmCode, reqArgs.info);

	args.SetResult(true);
	eventArgs.Cancel();
}

void CIcsRemoteEquipment::OnOperationModeEvent(OperationModeEventArgs & eventArgs)
{
	auto & args = eventArgs.GetArgs();
	auto & reqArgs = args.GetTypedArgs();

	GetLogger()
		.SetLogLevel(lt::LogLevel::Notice)
		.AddLog(_T("Operation Mode (mode = %d)"),
				reqArgs.mode);

	GetEquipment().Set_OperatingMode(reqArgs.mode);

	args.SetResult(true);
	eventArgs.Cancel();
}

void CIcsRemoteEquipment::OnSocketRequestEvent(SocketRequestEventArgs & eventArgs)
{
	auto & args = eventArgs.GetArgs();
	auto & reqArgs = args.GetTypedArgs();

	GetLogger()
		.SetLogLevel(lt::LogLevel::Notice)
		.AddLog(_T("Socket Request (rfid = %s)"),
				reqArgs.rfid);

	auto result = GetEquipment().Recv_ReqAcceptSocket(reqArgs.rfid);

	args.SetResult(result);
	eventArgs.Cancel();
}

void CIcsRemoteEquipment::OnPortClearEvent(PortClearEventArgs & eventArgs)
{
	auto & args = eventArgs.GetArgs();
	auto & reqArgs = args.GetTypedArgs();
	auto portAvailable = reqArgs.port >= 0;

	GetLogger()
		.SetLogLevel(lt::LogLevel::Notice)
		.AddLog(_T("Port Clear (port = %d)"),
				reqArgs.port);

	if(portAvailable)
		GetEquipment().Set_PortClear(reqArgs.port);

	args.SetResult(portAvailable);
	eventArgs.Cancel();
}

void CIcsRemoteEquipment::OnPortStatusEvent(PortStatusEventArgs & eventArgs)
{
	auto & args = eventArgs.GetArgs();
	auto & reqArgs = args.GetTypedArgs();
	auto portAvailable = reqArgs.port >= 0;

	GetLogger()
		.SetLogLevel(lt::LogLevel::Notice)
		.AddLog(_T("Port Status (port = %d, status = %d, rfid = %s, productSn = %s)"),
				reqArgs.port,
				reqArgs.status,
				reqArgs.socketInfo.rfid,
				reqArgs.socketInfo.productSn);

	if(portAvailable)
		GetEquipment().Set_PortStatus(reqArgs.port, reqArgs.status, reqArgs.socketInfo.rfid, reqArgs.socketInfo.productSn);

	args.SetResult(portAvailable);
	eventArgs.Cancel();
}

void CIcsRemoteEquipment::OnPortStatusRequestEvent(PortStatusRequestEventArgs & eventArgs)
{
	auto & args = eventArgs.GetArgs();
	auto & reqArgs = args.GetTypedArgs();
	auto portAvailable = reqArgs.port >= 0;

	if(portAvailable)
	{
		ST_PortStatus stPort = GetEquipment().Get_PortStatus(reqArgs.port);

		reqArgs.status = stPort.nStatus;
		_tcscpy_s(reqArgs.socketInfo.rfid,
			_countof(reqArgs.socketInfo.rfid),
			stPort.szRfid);
		_tcscpy_s(reqArgs.socketInfo.productSn,
			_countof(reqArgs.socketInfo.productSn),
			stPort.szBarcode);
	}
	else
	{
		reqArgs.status = 0;
		_tcscpy_s(reqArgs.socketInfo.rfid,
			_countof(reqArgs.socketInfo.rfid),
			_T(""));
		_tcscpy_s(reqArgs.socketInfo.productSn,
			_countof(reqArgs.socketInfo.productSn),
			_T(""));
	}

	GetLogger()
		.SetLogLevel(lt::LogLevel::Notice)
		.AddLog(_T("Port Status Request (port = %d, status = %d, rfid = %s, productSn = %s)"),
				reqArgs.port,
				reqArgs.status,
				reqArgs.socketInfo.rfid,
				reqArgs.socketInfo.productSn);

	args.SetResult(portAvailable);
	eventArgs.Cancel();
}

void CIcsRemoteEquipment::OnConveyorStatusEvent(ConveyorStatusEventArgs & eventArgs)
{
	auto & args = eventArgs.GetArgs();
	auto & reqArgs = args.GetTypedArgs();
	auto conveyorAvailable = reqArgs.conveyor >= 0;

	GetLogger()
		.SetLogLevel(lt::LogLevel::Notice)
		.AddLog(_T("Conveyor Status (conveyor = %d, status1 = %d, status2 = %d, rfid = %s, productSn = %s)"),
				reqArgs.conveyor,
				reqArgs.status1,
				reqArgs.status2,
				reqArgs.socketInfo.rfid,
				reqArgs.socketInfo.productSn);

	if(conveyorAvailable)
		GetEquipment().Set_ConveyorStatus(reqArgs.conveyor, reqArgs.status1, reqArgs.status2, reqArgs.socketInfo.rfid, reqArgs.socketInfo.productSn);

	args.SetResult(conveyorAvailable);
	eventArgs.Cancel();
}

void CIcsRemoteEquipment::OnConveyorStatusRequestEvent(ConveyorStatusRequestEventArgs & eventArgs)
{
	auto & args = eventArgs.GetArgs();
	auto & reqArgs = args.GetTypedArgs();
	auto conveyorAvailable = reqArgs.conveyor >= 0;

	if(conveyorAvailable)
	{
		ST_ConveyorStatus stConveyor = GetEquipment().Get_ConveyorStatus(reqArgs.conveyor);

		reqArgs.status1 = stConveyor.nStatus;
		reqArgs.status2 = stConveyor.nExistSocket;
		_tcscpy_s(reqArgs.socketInfo.rfid,
				  _countof(reqArgs.socketInfo.rfid),
				  stConveyor.szRfid);
		_tcscpy_s(reqArgs.socketInfo.productSn,
				  _countof(reqArgs.socketInfo.productSn),
				  stConveyor.szBarcode);
	}
	else
	{
		reqArgs.status1 = 0;
		reqArgs.status2 = 0;
		_tcscpy_s(reqArgs.socketInfo.rfid,
			_countof(reqArgs.socketInfo.rfid),
			_T(""));
		_tcscpy_s(reqArgs.socketInfo.productSn,
			_countof(reqArgs.socketInfo.productSn),
			_T(""));
	}

	GetLogger()
		.SetLogLevel(lt::LogLevel::Notice)
		.AddLog(_T("Conveyor Status Request (conveyor = %d, status1 = %d, status2 = %d, rfid = %s, productSn = %s)"),
				reqArgs.conveyor,
				reqArgs.status1,
				reqArgs.status2,
				reqArgs.socketInfo.rfid,
				reqArgs.socketInfo.productSn);

	args.SetResult(conveyorAvailable);
	eventArgs.Cancel();
}
