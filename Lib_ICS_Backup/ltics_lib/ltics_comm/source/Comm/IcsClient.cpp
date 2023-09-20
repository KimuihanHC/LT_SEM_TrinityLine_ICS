#include "pch.h"
#include "Comm/IcsClient.h"
#include "IcsRequestEvent.hpp"
#include "Util/StringUtil.hpp"

using namespace lt;

struct SendingCommand
{
	//////////////////////////////////////////////////////////////////////////////////
	// System setting
	static constexpr uint16 EquipmentId = 0x0103;
	//
	//////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////
	// Operation
	static constexpr uint16 EquipmentStatus = 0x0300;

	static constexpr uint16 OperationMode = 0x0302;

	static constexpr uint16 SocketRequest = 0x0310;

	static constexpr uint16 PortClear = 0x0330;
	static constexpr uint16 PortStatus = 0x0331;
	static constexpr uint16 PortStatusRequest = 0x0332;
	static constexpr uint16 ConveyorStatus = 0x0333;
	static constexpr uint16 ConveyorStatusRequest = 0x0334;
	//
	//////////////////////////////////////////////////////////////////////////////////
	//TEST Terminal  2022.11.27 uhkim
	static constexpr uint16 SVID = 0x0601;

};

struct ReceivingCommand
{
	//////////////////////////////////////////////////////////////////////////////////
	// System setting
	static constexpr uint16 TimeSync = 0x0100;
	static constexpr uint16 UserLevel = 0x0101;
	static constexpr uint16 Language = 0x0102;
	static constexpr uint16 Model = 0x0104;
	//
	//////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////
	// Control
	static constexpr uint16 Initialization = 0x0200;
	static constexpr uint16 Reset = 0x0201;
	static constexpr uint16 Run = 0x0202;
	static constexpr uint16 Stop = 0x0203;
	static constexpr uint16 BuzzerOff = 0x0204;
	static constexpr uint16 UiVisible = 0x0205;
	//
	//////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////
	// Operation
	static constexpr uint16 SocketAccepting = 0x0311;
	static constexpr uint16 EjectAccepting = 0x0312;
	static constexpr uint16 ProductionStartEnd = 0x0313;
	static constexpr uint16 ForceEject = 0x0314;
	//
	//////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////
	//TEST Terminal  2022.11.27 uhkim
	static constexpr uint16 TerminalMassage = 0x1003;
	//
	//////////////////////////////////////////////////////////////////////////////////
};

CIcsClient::CIcsClient()
: CIcsCommunicator(CommId::Client)
{
	//////////////////////////////////////////////////////////////////////////////////
	// System setting
	m_pTimeSyncEventNotifier = new TimeSyncEventNotifierImpl();
	m_pUserLevelEventNotifier = new UserLevelEventNotifierImpl();
	m_pLanguageEventNotifier = new LanguageEventNotifierImpl();
	m_pModelEventNotifier = new ModelEventNotifierImpl();

	AddRequestProcedure(ReceivingCommand::TimeSync,
						reinterpret_cast<RequestProc>(
							&CIcsClient::ProcRequestTimeSync));
	AddRequestProcedure(ReceivingCommand::UserLevel,
						reinterpret_cast<RequestProc>(
							&CIcsClient::ProcRequestUserLevel));
	AddRequestProcedure(ReceivingCommand::Language,
						reinterpret_cast<RequestProc>(
							&CIcsClient::ProcRequestLanguage));
	AddRequestProcedure(ReceivingCommand::Model,
						reinterpret_cast<RequestProc>(
							&CIcsClient::ProcRequestModel));
	//
	//////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////
	// Control
	m_pInitializationEventNotifier = new InitializationEventNotifierImpl();
	m_pResetEventNotifier = new ResetEventNotifierImpl();
	m_pRunEventNotifier = new RunEventNotifierImpl();
	m_pStopEventNotifier = new StopEventNotifierImpl();
	m_pBuzzerOffEventNotifier = new BuzzerOffEventNotifierImpl();
	m_pUiVisibleEventNotifier = new UiVisibleEventNotifierImpl();

	AddRequestProcedure(ReceivingCommand::Initialization,
						reinterpret_cast<RequestProc>(
							&CIcsClient::ProcRequestInitialization));
	AddRequestProcedure(ReceivingCommand::Reset,
						reinterpret_cast<RequestProc>(
							&CIcsClient::ProcRequestReset));
	AddRequestProcedure(ReceivingCommand::Run,
						reinterpret_cast<RequestProc>(
							&CIcsClient::ProcRequestRun));
	AddRequestProcedure(ReceivingCommand::Stop,
						reinterpret_cast<RequestProc>(
							&CIcsClient::ProcRequestStop));
	AddRequestProcedure(ReceivingCommand::BuzzerOff,
						reinterpret_cast<RequestProc>(
							&CIcsClient::ProcRequestBuzzerOff));
	AddRequestProcedure(ReceivingCommand::UiVisible,
						reinterpret_cast<RequestProc>(
							&CIcsClient::ProcRequestUiVisible));
	//
	//////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////
	// Operation
	m_pSocketAcceptingEventNotifier = new SocketAcceptingEventNotifierImpl();
	m_pEjectAcceptingEventNotifier = new EjectAcceptingEventNotifierImpl();
	m_pProductionStartEndEventNotifier = new ProductionStartEndEventNotifierImpl();
	m_pForceEjectEventNotifier = new ForceEjectEventNotifierImpl();

	AddRequestProcedure(ReceivingCommand::SocketAccepting,
						reinterpret_cast<RequestProc>(
							&CIcsClient::ProcRequestSocketAccepting));
	AddRequestProcedure(ReceivingCommand::EjectAccepting,
						reinterpret_cast<RequestProc>(
							&CIcsClient::ProcRequestEjectAccepting));
	AddRequestProcedure(ReceivingCommand::ProductionStartEnd,
						reinterpret_cast<RequestProc>(
							&CIcsClient::ProcProductionStartEnd));
	AddRequestProcedure(ReceivingCommand::ForceEject,
						reinterpret_cast<RequestProc>(
							&CIcsClient::ProcForceEject));
}

CIcsClient::~CIcsClient()
{
	//
	//////////////////////////////////////////////////////////////////////////////////
	// System setting
	delete m_pTimeSyncEventNotifier;
	delete m_pUserLevelEventNotifier;
	delete m_pLanguageEventNotifier;
	//
	//////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////
	// Control
	delete m_pInitializationEventNotifier;
	delete m_pResetEventNotifier;
	delete m_pRunEventNotifier;
	delete m_pStopEventNotifier;
	delete m_pBuzzerOffEventNotifier;
	delete m_pUiVisibleEventNotifier;
	//
	//////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////
	// Operation
	delete m_pSocketAcceptingEventNotifier;
	delete m_pEjectAcceptingEventNotifier;
	delete m_pProductionStartEndEventNotifier;
	delete m_pForceEjectEventNotifier;
}

uint32 CIcsClient::CommandEquipmentId(const SEquipmentId & equipmentId,
									  uint64 timeout)
{
	return ProcCommand(SendingCommand::EquipmentId,
					   {
						   #if defined(UNICODE)
						   ToMultiByte(equipmentId.equipmentId)
						   #else
						   equipmentId.equipmentId
						   #endif
					   },
					   timeout);
}

uint32 CIcsClient::CommandEquipmentStatus(const SEquipmentStatus & equipmentStatus,
										  uint64 timeout)
{
	return ProcCommand(SendingCommand::EquipmentStatus,
					   {
						   Format("%d", equipmentStatus.status),
						   Format("%d", equipmentStatus.alarmCode),
						   equipmentStatus.info == nullptr ? "" :
						   #if defined(UNICODE)
						   ToMultiByte(equipmentStatus.info)
						   #else
						   equipmentStatus.info
						   #endif
					   },
					   timeout);
}

uint32 CIcsClient::CommandOperationMode(const SOperationMode & operationMode,
										uint64 timeout)
{
	return ProcCommand(SendingCommand::OperationMode,
					   {
						   Format("%d", operationMode.mode)
					   },
					   timeout);
}

uint32 CIcsClient::CommandSocketRequest(const SSocketRfid & socketRfid,
										uint64 timeout)
{
	return ProcCommand(SendingCommand::SocketRequest,
					   {
						   #if defined(UNICODE)
						   ToMultiByte(socketRfid.rfid)
						   #else
						   socketRfid.rfid
						   #endif
					   },
					   timeout);
}

uint32 CIcsClient::CommandPortClear(const SPort & port,
									uint64 timeout)
{
	return ProcCommand(SendingCommand::PortClear,
					   {
						   Format("%d", port.port)
					   },
					   timeout);
}

uint32 CIcsClient::CommandPortStatus(const SPortStatus & portStatus,
									 uint64 timeout)
{
	return ProcCommand(SendingCommand::PortStatus,
					   {
						   Format("%d", portStatus.port),
						   Format("%d", portStatus.status),
						   #if defined(UNICODE)
						   ToMultiByte(portStatus.socketInfo.rfid),
						   ToMultiByte(portStatus.socketInfo.productSn)
						   #else
						   portStatus.socketInfo.rfid,
						   portStatus.socketInfo.productSn
						   #endif
					   },
					   timeout);
}

uint32 CIcsClient::CommandPortStatusRequest(SPortStatus2 & portStatus,
											uint64 timeout)
{
	DataCntr responseDataCntr;
	auto result = ProcCommand(SendingCommand::PortStatusRequest,
							  {
								  Format("%d", portStatus.port)
							  },
							  timeout,
							  responseDataCntr);

	if(responseDataCntr.size() < 3)
		return ICS_COMM_WRONG_RESPONSE;

	if(result == ICS_COMM_OK)
	{
		portStatus.status =
			static_cast<decltype(portStatus.status)>(
				strtoul(responseDataCntr[0].c_str(), nullptr, 10));
		_tcscpy_s(portStatus.socketInfo.rfid,
				  _countof(portStatus.socketInfo.rfid),
				  ToTypeChar(responseDataCntr[1]).c_str());
		_tcscpy_s(portStatus.socketInfo.productSn,
				  _countof(portStatus.socketInfo.productSn),
				  ToTypeChar(responseDataCntr[2]).c_str());
	}

	return result;
}

uint32 CIcsClient::CommandConveyorStatus(const SConveyorStatus & conveyorStatus,
										 uint64 timeout)
{
	return ProcCommand(SendingCommand::ConveyorStatus,
					   {
						   Format("%d", conveyorStatus.conveyor),
						   Format("%d", conveyorStatus.status1),
						   Format("%d", conveyorStatus.status2),
						   #if defined(UNICODE)
						   ToMultiByte(conveyorStatus.socketInfo.rfid),
						   ToMultiByte(conveyorStatus.socketInfo.productSn)
						   #else
						   conveyorStatus.socketInfo.rfid,
						   conveyorStatus.socketInfo.productSn
						   #endif
					   },
					   timeout);
}

uint32 CIcsClient::CommandConveyorStatusRequest(SConveyorStatus2 & conveyorStatus,
												uint64 timeout)
{
	DataCntr responseDataCntr;
	auto result = ProcCommand(SendingCommand::ConveyorStatusRequest,
							  {
								  Format("%d", conveyorStatus.conveyor)
							  },
							  timeout,
							  responseDataCntr);

	if(responseDataCntr.size() < 4)
		return ICS_COMM_WRONG_RESPONSE;

	if(result == ICS_COMM_OK)
	{
		conveyorStatus.status1 =
			static_cast<decltype(conveyorStatus.status1)>(
				strtoul(responseDataCntr[0].c_str(), nullptr, 10));
		conveyorStatus.status2 =
			static_cast<decltype(conveyorStatus.status2)>(
				strtoul(responseDataCntr[1].c_str(), nullptr, 10));
		_tcscpy_s(conveyorStatus.socketInfo.rfid,
				  _countof(conveyorStatus.socketInfo.rfid),
				  ToTypeChar(responseDataCntr[2]).c_str());
		_tcscpy_s(conveyorStatus.socketInfo.productSn,
				  _countof(conveyorStatus.socketInfo.productSn),
				  ToTypeChar(responseDataCntr[3]).c_str());
	}

	return result;
}

bool CIcsClient::ProcRequestTimeSync(const DataCntr & dataCntr,
									 DataCntr & /*resultDataCntr*/)
{
	if(dataCntr.empty())
		return false;

	auto & time = dataCntr[0];
	auto year = time.substr(0, 4);
	auto month = time.substr(4, 2);
	auto day = time.substr(6, 2);
	auto hour = time.substr(8, 2);
	auto minute = time.substr(10, 2);
	auto second = time.substr(12, 2);

	return RaiseTimeSyncEvent(
		{
			static_cast<uint16>(strtoul(year.c_str(), nullptr, 10)),
			static_cast<uint16>(strtoul(month.c_str(), nullptr, 10)),
			static_cast<uint16>(strtoul(day.c_str(), nullptr, 10)),
			static_cast<uint16>(strtoul(hour.c_str(), nullptr, 10)),
			static_cast<uint16>(strtoul(minute.c_str(), nullptr, 10)),
			static_cast<uint16>(strtoul(second.c_str(), nullptr, 10))
		}
	);
}

bool CIcsClient::ProcRequestUserLevel(const DataCntr & dataCntr,
									  DataCntr & /*resultDataCntr*/)
{
	if (dataCntr.size() < 2)
		return false;

	return RaiseUserLevelEvent(
		{
			static_cast<uint32>(strtoul(dataCntr[0].c_str(), nullptr, 10)),
			ToTypeChar(dataCntr[1]).c_str()
		}
	);
}

bool CIcsClient::ProcRequestLanguage(const DataCntr & dataCntr,
									 DataCntr & /*resultDataCntr*/)
{
	if(dataCntr.empty())
		return false;

	return RaiseLanguageEvent(
		{
			static_cast<uint32>(strtoul(dataCntr[0].c_str(), nullptr, 10))
		}
	);
}

bool CIcsClient::ProcRequestModel(const DataCntr & dataCntr,
								  DataCntr & /*resultDataCntr*/)
{
	if(dataCntr.size() < 2)
		return false;

	return RaiseModelEvent(
		{
			static_cast<uint16>(strtoul(dataCntr[0].c_str(), nullptr, 10)),
			static_cast<uint16>(strtoul(dataCntr[1].c_str(), nullptr, 10))
		}
	);
}

bool CIcsClient::ProcRequestInitialization(const DataCntr & /*dataCntr*/,
										   DataCntr & /*resultDataCntr*/)
{
	return RaiseInitializationEvent();
}

bool CIcsClient::ProcRequestReset(const DataCntr & /*dataCntr*/,
								  DataCntr & /*resultDataCntr*/)
{
	return RaiseResetEvent();
}

bool CIcsClient::ProcRequestRun(const DataCntr & /*dataCntr*/,
								DataCntr & /*resultDataCntr*/)
{
	return RaiseRunEvent();
}

bool CIcsClient::ProcRequestStop(const DataCntr & /*dataCntr*/,
								 DataCntr & /*resultDataCntr*/)
{
	return RaiseStopEvent();
}

bool CIcsClient::ProcRequestBuzzerOff(const DataCntr & /*dataCntr*/,
									  DataCntr & /*resultDataCntr*/)
{
	return RaiseBuzzerOffEvent();
}

bool CIcsClient::ProcRequestUiVisible(const DataCntr & dataCntr,
									  DataCntr & /*resultDataCntr*/)
{
	if(dataCntr.empty())
		return false;

	return RaiseUiVisibleEvent(
		{
			static_cast<int>(strtol(dataCntr[0].c_str(), nullptr, 10)),
		}
	);
}

bool CIcsClient::ProcRequestSocketAccepting(const DataCntr & dataCntr,
											DataCntr & /*resultDataCntr*/)
{
	if(dataCntr.size() < 3)
		return false;

	return RaiseSocketAcceptingEvent(
		{
			ToTypeChar(dataCntr[0]).c_str(),
			static_cast<uint32>(strtoul(dataCntr[1].c_str(), nullptr, 10)),
			ToTypeChar(dataCntr[2]).c_str()
		}
	);
}

bool CIcsClient::ProcRequestEjectAccepting(const DataCntr & dataCntr,
										   DataCntr & /*resultDataCntr*/)
{
	if(dataCntr.empty())
		return false;

	return RaiseEjectAcceptingEvent(
		{
			ToTypeChar(dataCntr[0]).c_str()
		}
	);
}

bool CIcsClient::ProcProductionStartEnd(const DataCntr & dataCntr,
										DataCntr & /*resultDataCntr*/)
{
	if(dataCntr.empty())
		return false;

	return RaiseProductionStartEndEvent(
		{
			static_cast<uint32>(strtoul(dataCntr[0].c_str(), nullptr, 10)),
		}
	);
}

bool CIcsClient::ProcForceEject(const DataCntr & dataCntr,
								DataCntr & /*resultDataCntr*/)
{
	if(dataCntr.empty())
		return false;

	return RaiseForceEjectEvent(
		{
			static_cast<uint32>(strtoul(dataCntr[0].c_str(), nullptr, 10)),
		}
	);
}

bool CIcsClient::RaiseTimeSyncEvent(TimeSyncArgs::Args & args)
{
	return RaiseEvent(*this,
					  *m_pTimeSyncEventNotifier,
					  args);
}

bool CIcsClient::RaiseUserLevelEvent(UserLevelArgs::Args & args)
{
	return RaiseEvent(*this,
					  *m_pUserLevelEventNotifier,
					  args);
}

bool CIcsClient::RaiseLanguageEvent(LanguageArgs::Args & args)
{
	return RaiseEvent(*this,
					  *m_pLanguageEventNotifier,
					  args);
}

bool CIcsClient::RaiseModelEvent(ModelArgs::Args & args)
{
	return RaiseEvent(*this,
					  *m_pModelEventNotifier,
					  args);
}

bool CIcsClient::RaiseInitializationEvent()
{
	return RaiseEvent(*this,
					  *m_pInitializationEventNotifier);
}

bool CIcsClient::RaiseResetEvent()
{
	return RaiseEvent(*this,
					  *m_pResetEventNotifier);
}

bool CIcsClient::RaiseRunEvent()
{
	return RaiseEvent(*this,
					  *m_pRunEventNotifier);
}

bool CIcsClient::RaiseStopEvent()
{
	return RaiseEvent(*this,
					  *m_pStopEventNotifier);
}

bool CIcsClient::RaiseBuzzerOffEvent()
{
	return RaiseEvent(*this,
					  *m_pBuzzerOffEventNotifier);
}

bool CIcsClient::RaiseUiVisibleEvent(UiVisibleArgs::Args & args)
{
	return RaiseEvent(*this,
					  *m_pUiVisibleEventNotifier,
					  args);
}

bool CIcsClient::RaiseSocketAcceptingEvent(SocketAcceptingArgs::Args & args)
{
	return RaiseEvent(*this,
					  *m_pSocketAcceptingEventNotifier,
					  args);
}

bool CIcsClient::RaiseEjectAcceptingEvent(EjectAcceptingArgs::Args & args)
{
	return RaiseEvent(*this,
					  *m_pEjectAcceptingEventNotifier,
					  args);
}

bool CIcsClient::RaiseProductionStartEndEvent(ProductionStartEndArgs::Args & args)
{
	return RaiseEvent(*this,
					  *m_pProductionStartEndEventNotifier,
					  args);
}

bool CIcsClient::RaiseForceEjectEvent(ForceEjectArgs::Args & args)
{
	return RaiseEvent(*this,
					  *m_pForceEjectEventNotifier,
					  args);
}

TimeSyncEventNotifier<CIcsClient> &
CIcsClient::GetTimeSyncEventNotifier()
{
	return *m_pTimeSyncEventNotifier;
}

UserLevelEventNotifier<CIcsClient> &
CIcsClient::GetUserLevelEventNotifier()
{
	return *m_pUserLevelEventNotifier;
}

LanguageEventNotifier<CIcsClient> &
CIcsClient::GetLanguageEventNotifier()
{
	return *m_pLanguageEventNotifier;
}

ModelEventNotifier<CIcsClient> &
CIcsClient::GetModelEventNotifier()
{
	return *m_pModelEventNotifier;
}

InitializationEventNotifier<CIcsClient> &
CIcsClient::GetInitializationEventNotifier()
{
	return *m_pInitializationEventNotifier;
}

ResetEventNotifier<CIcsClient> &
CIcsClient::GetResetEventNotifier()
{
	return *m_pResetEventNotifier;
}

RunEventNotifier<CIcsClient> &
CIcsClient::GetRunEventNotifier()
{
	return *m_pRunEventNotifier;
}

StopEventNotifier<CIcsClient> &
CIcsClient::GetStopEventNotifier()
{
	return *m_pStopEventNotifier;
}

BuzzerOffEventNotifier<CIcsClient> &
CIcsClient::GetBuzzerOffEventNotifier()
{
	return *m_pBuzzerOffEventNotifier;
}

UiVisibleEventNotifier<CIcsClient> &
CIcsClient::GetUiVisibleEventNotifier()
{
	return *m_pUiVisibleEventNotifier;
}

SocketAcceptingEventNotifier<CIcsClient> &
CIcsClient::GetSocketAcceptingEventNotifier()
{
	return *m_pSocketAcceptingEventNotifier;
}

EjectAcceptingEventNotifier<CIcsClient> &
CIcsClient::GetEjectAcceptingEventNotifier()
{
	return *m_pEjectAcceptingEventNotifier;
}

ProductionStartEndEventNotifier<CIcsClient> &
CIcsClient::GetProductionStartEndEventNotifier()
{
	return *m_pProductionStartEndEventNotifier;
}

ForceEjectEventNotifier<CIcsClient> &
CIcsClient::GetForceEjectEventNotifier()
{
	return *m_pForceEjectEventNotifier;
}

