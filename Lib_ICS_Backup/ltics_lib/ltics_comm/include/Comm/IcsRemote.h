#pragma once

#include "Comm/IcsCommunicator.h"
#include "Event/IcsCommEvent.h"

namespace lt
{
	template<typename TypeSender, typename TypeArgs>
	class CCancellableEventNotifier;

	class LTICS_LIB_CLASS CIcsRemote : public CIcsCommunicator
	{
	public:
		CIcsRemote();
		CIcsRemote(const CIcsRemote &) = delete;
		virtual ~CIcsRemote();

		CIcsRemote & operator=(const CIcsRemote &) = delete;


		//////////////////////////////////////////////////////////////////////////////////
		// System setting
		uint32 CommandTimeSync(const STimeSync & timeSync,
							   uint64 timeout);
		uint32 CommandUserLevel(const SUserLevel & userLevel,
								uint64 timeout);
		uint32 CommandLanguage(const SLanguage & language,
							   uint64 timeout);
		uint32 CommandModel(const SModel & model,
							uint64 timeout);

		EquipmentIdEventNotifier<CIcsRemote> &
			GetEquipmentIdEventNotifier();
		//
		//////////////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////////////
		// Control
		uint32 CommandInitialization(uint64 timeout);
		uint32 CommandReset(uint64 timeout);
		uint32 CommandRun(uint64 timeout);
		uint32 CommandStop(uint64 timeout);
		uint32 CommandBuzzerOff(uint64 timeout);
		uint32 CommandUiVisible(const SUiVisible & uiVisible,
								uint64 timeout);
		//
		//////////////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////////////
		// Operation
		uint32 CommandSocketAccepting(const SSocketAccepting & socketAccepting,
									  uint64 timeout);
		uint32 CommandEjectAccepting(const SSocketRfid & socketRfid,
									 uint64 timeout);
		uint32 CommandProductionStartEnd(const SProductionStartEnd & productionStartEnd,
										 uint64 timeout);
		uint32 CommandForceEject(const SForceEject & forceEject,
								 uint64 timeout);

		EquipmentStatusEventNotifier<CIcsRemote> &
			GetEquipmentStatusEventNotifier();
		OperationModeEventNotifier<CIcsRemote> &
			GetOperationModeEventNotifier();
		SocketRequestEventNotifier<CIcsRemote> &
			GetSocketRequestEventNotifier();
		PortClearEventNotifier<CIcsRemote> &
			GetPortClearEventNotifier();
		PortStatusEventNotifier<CIcsRemote> &
			GetPortStatusEventNotifier();
		PortStatusRequestEventNotifier<CIcsRemote> &
			GetPortStatusRequestEventNotifier();
		ConveyorStatusEventNotifier<CIcsRemote> &
			GetConveyorStatusEventNotifier();
		ConveyorStatusRequestEventNotifier<CIcsRemote> &
			GetConveyorStatusRequestEventNotifier();

	private:
		//////////////////////////////////////////////////////////////////////////////////
		// System setting
		bool ProcRequestEquipmentId(const DataCntr & dataCntr,
									DataCntr & resultDataCntr);

		bool RaiseEquipmentIdEvent(EquipmentIdArgs::Args & args);
		//
		//////////////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////////////
		// Operation
		bool ProcRequestEquipmentStatus(const DataCntr & dataCntr,
										DataCntr & resultDataCntr);

		bool ProcRequestOperationMode(const DataCntr & dataCntr,
									  DataCntr & resultDataCntr);

		bool ProcRequestSocketRequest(const DataCntr & dataCntr,
									  DataCntr & resultDataCntr);

		bool ProcRequestPortClear(const DataCntr & dataCntr,
								  DataCntr & resultDataCntr);
		bool ProcRequestPortStatus(const DataCntr & dataCntr,
								   DataCntr & resultDataCntr);
		bool ProcRequestPortStatusRequest(const DataCntr & dataCntr,
										  DataCntr & resultDataCntr);
		bool ProcRequestConveyorStatus(const DataCntr & dataCntr,
									   DataCntr & resultDataCntr);
		bool ProcRequestConveyorStatusRequest(const DataCntr & dataCntr,
											  DataCntr & resultDataCntr);		

		bool RaiseEquipmentStatusEvent(EquipmentStatusArgs::Args & args);

		bool RaiseOperationModeEvent(OperationModeArgs::Args & args);

		bool RaiseSocketRequestEvent(SocketRequestArgs::Args & args);

		bool RaisePortClearEvent(PortClearArgs::Args & args);
		bool RaisePortStatusEvent(PortStatusArgs::Args & args);
		bool RaisePortStatusRequestEvent(PortStatusRequestArgs::Args & args);
		bool RaiseConveyorStatusEvent(ConveyorStatusArgs::Args & args);
		bool RaiseConveyorStatusRequestEvent(ConveyorStatusRequestArgs::Args & args);
	
	private:
		//////////////////////////////////////////////////////////////////////////////////
		// System setting
		using EquipmentIdEventNotifierImpl =
			CCancellableEventNotifier<CIcsRemote, EquipmentIdArgs>;

		EquipmentIdEventNotifierImpl * m_pEquipmentIdEventNotifier = nullptr;
		//
		//////////////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////////////
		// Operation
		using EquipmentStatusEventNotifierImpl =
			CCancellableEventNotifier<CIcsRemote, EquipmentStatusArgs>;

		EquipmentStatusEventNotifierImpl * m_pEquipmentStatusEventNotifier = nullptr;

		using OperationModeEventNotifierImpl =
			CCancellableEventNotifier<CIcsRemote, OperationModeArgs>;

		OperationModeEventNotifierImpl * m_pOperationModeEventNotifier = nullptr;

		using SocketRequestEventNotifierImpl =
			CCancellableEventNotifier<CIcsRemote, SocketRequestArgs>;

		SocketRequestEventNotifierImpl * m_pSocketRequestEventNotifier = nullptr;

		using PortClearEventNotifierImpl =
			CCancellableEventNotifier<CIcsRemote, PortClearArgs>;
		using PortStatusEventNotifierImpl =
			CCancellableEventNotifier<CIcsRemote, PortStatusArgs>;
		using PortStatusRequestEventNotifierImpl =
			CCancellableEventNotifier<CIcsRemote, PortStatusRequestArgs>;
		using ConveyorStatusEventNotifierImpl =
			CCancellableEventNotifier<CIcsRemote, ConveyorStatusArgs>;
		using ConveyorStatusRequestEventNotifierImpl =
			CCancellableEventNotifier<CIcsRemote, ConveyorStatusRequestArgs>;

		PortClearEventNotifierImpl * m_pPortClearEventNotifier = nullptr;
		PortStatusEventNotifierImpl * m_pPortStatusEventNotifier = nullptr;
		PortStatusRequestEventNotifierImpl * m_pPortStatusRequestEventNotifier = nullptr;
		ConveyorStatusEventNotifierImpl * m_pConveyorStatusEventNotifier = nullptr;
		ConveyorStatusRequestEventNotifierImpl * m_pConveyorStatusRequestEventNotifier = nullptr;
		
	};
}
