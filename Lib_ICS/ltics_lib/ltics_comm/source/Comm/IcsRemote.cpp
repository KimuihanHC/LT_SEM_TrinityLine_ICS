#include "pch.h"
#include "Comm/IcsRemote.h"
#include "IcsRequestEvent.hpp"
#include "Util/StringUtil.hpp"

using namespace lt;

struct SendingCommand
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
};

struct ReceivingCommand
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

CIcsRemote::CIcsRemote()
: CIcsCommunicator(CommId::Server)
{
	//////////////////////////////////////////////////////////////////////////////////
	// System setting
	m_pEquipmentIdEventNotifier = new EquipmentIdEventNotifierImpl();

	AddRequestProcedure(ReceivingCommand::EquipmentId,
						reinterpret_cast<RequestProc>(
							&CIcsRemote::ProcRequestEquipmentId));
	//
	//////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////
	// Operation
	m_pEquipmentStatusEventNotifier = new EquipmentStatusEventNotifierImpl();

	m_pOperationModeEventNotifier = new OperationModeEventNotifierImpl();

	m_pSocketRequestEventNotifier = new SocketRequestEventNotifierImpl();

	m_pPortClearEventNotifier = new PortClearEventNotifierImpl();
	m_pPortStatusEventNotifier = new PortStatusEventNotifierImpl();
	m_pPortStatusRequestEventNotifier = new PortStatusRequestEventNotifierImpl();
	m_pConveyorStatusEventNotifier = new ConveyorStatusEventNotifierImpl();
	m_pConveyorStatusRequestEventNotifier = new ConveyorStatusRequestEventNotifierImpl();

	AddRequestProcedure(ReceivingCommand::EquipmentStatus,
						reinterpret_cast<RequestProc>(
							&CIcsRemote::ProcRequestEquipmentStatus));

	AddRequestProcedure(ReceivingCommand::OperationMode,
						reinterpret_cast<RequestProc>(
							&CIcsRemote::ProcRequestOperationMode));

	AddRequestProcedure(ReceivingCommand::SocketRequest,
						reinterpret_cast<RequestProc>(
							&CIcsRemote::ProcRequestSocketRequest));

	AddRequestProcedure(ReceivingCommand::PortClear,
						reinterpret_cast<RequestProc>(
							&CIcsRemote::ProcRequestPortClear));
	AddRequestProcedure(ReceivingCommand::PortStatus,
						reinterpret_cast<RequestProc>(
							&CIcsRemote::ProcRequestPortStatus));
	AddRequestProcedure(ReceivingCommand::PortStatusRequest,
						reinterpret_cast<RequestProc>(
							&CIcsRemote::ProcRequestPortStatusRequest));
	AddRequestProcedure(ReceivingCommand::ConveyorStatus,
						reinterpret_cast<RequestProc>(
							&CIcsRemote::ProcRequestConveyorStatus));
	AddRequestProcedure(ReceivingCommand::ConveyorStatusRequest,
						reinterpret_cast<RequestProc>(
							&CIcsRemote::ProcRequestConveyorStatusRequest));
}

CIcsRemote::~CIcsRemote()
{
	//////////////////////////////////////////////////////////////////////////////////
	// System setting
	delete m_pEquipmentIdEventNotifier;
	//
	//////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////
	// Operation
	delete m_pEquipmentStatusEventNotifier;

	delete m_pOperationModeEventNotifier;

	delete m_pSocketRequestEventNotifier;

	delete m_pPortClearEventNotifier;
	delete m_pPortStatusEventNotifier;
	delete m_pPortStatusRequestEventNotifier;
	delete m_pConveyorStatusEventNotifier;
	delete m_pConveyorStatusRequestEventNotifier;
}

uint32 CIcsRemote::CommandTimeSync(const STimeSync & timeSync,
								   uint64 timeout)
{
	return ProcCommand(SendingCommand::TimeSync,
					   {
						   Format("%04d%02d%02d%02d%02d%02d",
								  timeSync.year,
								  timeSync.month,
								  timeSync.day,
								  timeSync.hour,
								  timeSync.minute,
								  timeSync.second)
					   },
					   timeout);
}

uint32 CIcsRemote::CommandUserLevel(const SUserLevel & userLevel,
									uint64 timeout)
{
	return ProcCommand(SendingCommand::UserLevel,
					   {
						   Format("%d", userLevel.userLevel),
						   #if defined(UNICODE)
						   ToMultiByte(userLevel.userId),
						   #else
						   userLevel.userId,
						   #endif
					   },
					   timeout);
}

uint32 CIcsRemote::CommandLanguage(const SLanguage & language,
								   uint64 timeout)
{
	return ProcCommand(SendingCommand::Language,
					   {
						   Format("%d", language.language)
					   },
					   timeout);
}

uint32 CIcsRemote::CommandModel(const SModel & model,
								uint64 timeout)
{
	return ProcCommand(SendingCommand::Model,
					   {
						   Format("%d", model.model),
						   Format("%d", model.socketType)
					   },
					   timeout);
}

uint32 CIcsRemote::CommandInitialization(uint64 timeout)
{
	return ProcCommand(SendingCommand::Initialization,
					   timeout);
}

uint32 CIcsRemote::CommandReset(uint64 timeout)
{
	return ProcCommand(SendingCommand::Reset,
					   timeout);
}

uint32 CIcsRemote::CommandRun(uint64 timeout)
{
	return ProcCommand(SendingCommand::Run,
					   timeout);
}

uint32 CIcsRemote::CommandStop(uint64 timeout)
{
	return ProcCommand(SendingCommand::Stop,
					   timeout);
}

uint32 CIcsRemote::CommandBuzzerOff(uint64 timeout)
{
	return ProcCommand(SendingCommand::BuzzerOff,
					   timeout);
}

uint32 CIcsRemote::CommandUiVisible(const SUiVisible & uiVisible,
									uint64 timeout)
{
	return ProcCommand(SendingCommand::UiVisible,
					   {
						   Format("%d", uiVisible.cmdShow)
					   },
					   timeout);
}

uint32 CIcsRemote::CommandSocketAccepting(const SSocketAccepting & socketAccepting,
										  uint64 timeout)
{
	return ProcCommand(SendingCommand::SocketAccepting,
					   {
						   #if defined(UNICODE)
						   ToMultiByte(socketAccepting.rfid),
						   #else
						   socketAccepting.rfid,
						   #endif
						   Format("%d", socketAccepting.flag),
						   #if defined(UNICODE)
						   ToMultiByte(socketAccepting.info)
						   #else
						   socketAccepting.info
						   #endif
					   },
					   timeout);
}

uint32 CIcsRemote::CommandEjectAccepting(const SSocketRfid & socketRfid,
										 uint64 timeout)
{
	return ProcCommand(SendingCommand::EjectAccepting,
					   {
						   #if defined(UNICODE)
						   ToMultiByte(socketRfid.rfid)
						   #else
						   socketRfid.rfid
						   #endif
					   },
					   timeout);
}

uint32 CIcsRemote::CommandProductionStartEnd(const SProductionStartEnd & productionStartEnd,
											 uint64 timeout)
{
	return ProcCommand(SendingCommand::ProductionStartEnd,
					   {
						   Format("%d", productionStartEnd.flag)
					   },
					   timeout);
}

uint32 CIcsRemote::CommandForceEject(const SForceEject & forceEject,
									 uint64 timeout)
{
	return ProcCommand(SendingCommand::ForceEject,
					   {
						   Format("%d", forceEject.onOff)
					   },
					   timeout);
}

bool CIcsRemote::ProcRequestEquipmentId(const DataCntr & dataCntr,
										DataCntr & resultDataCntr)
{
	if(dataCntr.empty())
		return false;

	return RaiseEquipmentIdEvent(
		{
			ToTypeChar(dataCntr[0]).c_str()
		}
	);
}

bool CIcsRemote::ProcRequestEquipmentStatus(const DataCntr & dataCntr,
											DataCntr & /*resultDataCntr*/)
{
	if(dataCntr.size() < 3)
		return false;

	return RaiseEquipmentStatusEvent(
		{
			static_cast<uint32>(strtoul(dataCntr[0].c_str(), nullptr, 10)),
			static_cast<int32>(strtoul(dataCntr[1].c_str(), nullptr, 10)),
			ToTypeChar(dataCntr[2]).c_str()
		}
	);
}

bool CIcsRemote::ProcRequestOperationMode(const DataCntr & dataCntr,
										  DataCntr & /*resultDataCntr*/)
{
	if(dataCntr.empty())
		return false;

	return RaiseOperationModeEvent(
		{
			static_cast<uint32>(strtoul(dataCntr[0].c_str(), nullptr, 10))
		}
	);
}

bool CIcsRemote::ProcRequestSocketRequest(const DataCntr & dataCntr,
										  DataCntr & /*resultDataCntr*/)
{
	if(dataCntr.empty())
		return false;

	return RaiseSocketRequestEvent(
		{
			ToTypeChar(dataCntr[0]).c_str()
		}
	);
}

bool CIcsRemote::ProcRequestPortClear(const DataCntr & dataCntr,
									  DataCntr & /*resultDataCntr*/)
{
	if(dataCntr.empty())
		return false;

	return RaisePortClearEvent(
		{
			static_cast<uint32>(strtoul(dataCntr[0].c_str(), nullptr, 10))
		}
	);
}

bool CIcsRemote::ProcRequestPortStatus(const DataCntr & dataCntr,
									   DataCntr & /*resultDataCntr*/)
{
	if(dataCntr.size() < 4)
		return false;

	return RaisePortStatusEvent(
		{
			static_cast<uint32>(strtoul(dataCntr[0].c_str(), nullptr, 10)),
			static_cast<uint32>(strtoul(dataCntr[1].c_str(), nullptr, 10)),
			ToTypeChar(dataCntr[2]).c_str(),
			ToTypeChar(dataCntr[3]).c_str()
		}
	);
}

bool CIcsRemote::ProcRequestPortStatusRequest(const DataCntr & dataCntr,
											  DataCntr & resultDataCntr)
{
	if(dataCntr.empty())
		return false;

	auto port = static_cast<uint32>(strtoul(dataCntr[0].c_str(), nullptr, 10));
	PortStatusRequestArgs::Args args = { port, };
	auto result = RaisePortStatusRequestEvent(args);

	resultDataCntr = {
		Format("%d", args.status),
		#if defined(UNICODE)
		ToMultiByte(args.socketInfo.rfid),
		ToMultiByte(args.socketInfo.productSn)
		#else
		std::move(args.socketInfo.rfid),
		std::move(args.socketInfo.productSn)
		#endif
	};

	return result;
}

bool CIcsRemote::ProcRequestConveyorStatus(const DataCntr & dataCntr,
										   DataCntr & /*resultDataCntr*/)
{
	if(dataCntr.size() < 5)
		return false;

	return RaiseConveyorStatusEvent(
		{
			static_cast<uint32>(strtoul(dataCntr[0].c_str(), nullptr, 10)),
			static_cast<uint32>(strtoul(dataCntr[1].c_str(), nullptr, 10)),
			static_cast<uint32>(strtoul(dataCntr[2].c_str(), nullptr, 10)),
			ToTypeChar(dataCntr[3]).c_str(),
			ToTypeChar(dataCntr[4]).c_str()
		}
	);
}

bool CIcsRemote::ProcRequestConveyorStatusRequest(const DataCntr & dataCntr,
												  DataCntr & resultDataCntr)
{
	if(dataCntr.empty())
		return false;

	auto conveyor = static_cast<uint32>(strtoul(dataCntr[0].c_str(), nullptr, 10));
	ConveyorStatusRequestArgs::Args args = { conveyor, };
	auto result = RaiseConveyorStatusRequestEvent(args);

	resultDataCntr = {
		Format("%d", args.status1),
		Format("%d", args.status2),
		#if defined(UNICODE)
		ToMultiByte(args.socketInfo.rfid),
		ToMultiByte(args.socketInfo.productSn)
		#else
		std::move(args.socketInfo.rfid),
		std::move(args.socketInfo.productSn)
		#endif
	};

	return result;
}


//
bool CIcsRemote::RaiseEquipmentIdEvent(EquipmentIdArgs::Args & args)
{
	return RaiseEvent(*this,
					  *m_pEquipmentIdEventNotifier,
					  args);
}

bool CIcsRemote::RaiseEquipmentStatusEvent(EquipmentStatusArgs::Args & args)
{
	return RaiseEvent(*this,
					  *m_pEquipmentStatusEventNotifier,
					  args);
}

bool CIcsRemote::RaiseOperationModeEvent(OperationModeArgs::Args & args)
{
	return RaiseEvent(*this,
					  *m_pOperationModeEventNotifier,
					  args);
}

bool CIcsRemote::RaiseSocketRequestEvent(SocketRequestArgs::Args & args)
{
	return RaiseEvent(*this,
					  *m_pSocketRequestEventNotifier,
					  args);
}

bool CIcsRemote::RaisePortClearEvent(PortClearArgs::Args & args)
{
	return RaiseEvent(*this,
					  *m_pPortClearEventNotifier,
					  args);
}

bool CIcsRemote::RaisePortStatusEvent(PortStatusArgs::Args & args)
{
	return RaiseEvent(*this,
					  *m_pPortStatusEventNotifier,
					  args);
}

bool CIcsRemote::RaisePortStatusRequestEvent(PortStatusRequestArgs::Args & args)
{
	return RaiseEvent(*this,
					  *m_pPortStatusRequestEventNotifier,
					  args);
}

bool CIcsRemote::RaiseConveyorStatusEvent(ConveyorStatusArgs::Args & args)
{
	return RaiseEvent(*this,
					  *m_pConveyorStatusEventNotifier,
					  args);
}

bool CIcsRemote::RaiseConveyorStatusRequestEvent(ConveyorStatusRequestArgs::Args & args)
{
	return RaiseEvent(*this,
					  *m_pConveyorStatusRequestEventNotifier,
					  args);
}

EquipmentIdEventNotifier<CIcsRemote> &
CIcsRemote::GetEquipmentIdEventNotifier()
{
	return *m_pEquipmentIdEventNotifier;
}

EquipmentStatusEventNotifier<CIcsRemote> &
CIcsRemote::GetEquipmentStatusEventNotifier()
{
	return *m_pEquipmentStatusEventNotifier;
}

OperationModeEventNotifier<CIcsRemote> &
CIcsRemote::GetOperationModeEventNotifier()
{
	return *m_pOperationModeEventNotifier;
}

SocketRequestEventNotifier<CIcsRemote> &
CIcsRemote::GetSocketRequestEventNotifier()
{
	return *m_pSocketRequestEventNotifier;
}

PortClearEventNotifier<CIcsRemote> &
CIcsRemote::GetPortClearEventNotifier()
{
	return *m_pPortClearEventNotifier;
}

PortStatusEventNotifier<CIcsRemote> &
CIcsRemote::GetPortStatusEventNotifier()
{
	return *m_pPortStatusEventNotifier;
}

PortStatusRequestEventNotifier<CIcsRemote> &
CIcsRemote::GetPortStatusRequestEventNotifier()
{
	return *m_pPortStatusRequestEventNotifier;
}

ConveyorStatusEventNotifier<CIcsRemote> &
CIcsRemote::GetConveyorStatusEventNotifier()
{
	return *m_pConveyorStatusEventNotifier;
}

ConveyorStatusRequestEventNotifier<CIcsRemote> &
CIcsRemote::GetConveyorStatusRequestEventNotifier()
{
	return *m_pConveyorStatusRequestEventNotifier;
}
