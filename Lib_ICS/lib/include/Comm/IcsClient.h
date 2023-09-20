#pragma once

#include "Comm/IcsCommunicator.h"
#include "Event/IcsCommEvent.h"

namespace lt
{
	template<typename TypeSender, typename TypeArgs>
	class CCancellableEventNotifier;

	class LTICS_LIB_CLASS CIcsClient : public CIcsCommunicator
	{
	public:
		CIcsClient();
		CIcsClient(const CIcsClient &) = delete;
		virtual ~CIcsClient();

		CIcsClient & operator=(const CIcsClient &) = delete;

		//////////////////////////////////////////////////////////////////////////////////
		// System setting
		uint32 CommandEquipmentId(const SEquipmentId & equipmentId,
								  uint64 timeout);

		TimeSyncEventNotifier<CIcsClient> &
			GetTimeSyncEventNotifier();
		UserLevelEventNotifier<CIcsClient> &
			GetUserLevelEventNotifier();
		LanguageEventNotifier<CIcsClient> &
			GetLanguageEventNotifier();
		ModelEventNotifier<CIcsClient> &
			GetModelEventNotifier();
		//
		//////////////////////////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////////////////////////
		// Control
		InitializationEventNotifier<CIcsClient> &
			GetInitializationEventNotifier();
		ResetEventNotifier<CIcsClient> &
			GetResetEventNotifier();
		RunEventNotifier<CIcsClient> &
			GetRunEventNotifier();
		StopEventNotifier<CIcsClient> &
			GetStopEventNotifier();
		BuzzerOffEventNotifier<CIcsClient> &
			GetBuzzerOffEventNotifier();
		UiVisibleEventNotifier<CIcsClient> &
			GetUiVisibleEventNotifier();
		//
		//////////////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////////////
		// Operation
		uint32 CommandEquipmentStatus(const SEquipmentStatus & equipmentStatus,
									  uint64 timeout);

		uint32 CommandOperationMode(const SOperationMode & operationMode,
									uint64 timeout);

		uint32 CommandSocketRequest(const SSocketRfid & socketRfid,
									uint64 timeout);

		uint32 CommandPortClear(const SPort & port,
								uint64 timeout);
		uint32 CommandPortStatus(const SPortStatus & portStatus,
								 uint64 timeout);
		uint32 CommandPortStatusRequest(SPortStatus2 & portStatus,
										uint64 timeout);
		uint32 CommandConveyorStatus(const SConveyorStatus & conveyorStatus,
									 uint64 timeout);
		uint32 CommandConveyorStatusRequest(SConveyorStatus2 & conveyorStatus,
											uint64 timeout);

		SocketAcceptingEventNotifier<CIcsClient> &
			GetSocketAcceptingEventNotifier();
		EjectAcceptingEventNotifier<CIcsClient> &
			GetEjectAcceptingEventNotifier();
		ProductionStartEndEventNotifier<CIcsClient> &
			GetProductionStartEndEventNotifier();
		ForceEjectEventNotifier<CIcsClient> &
			GetForceEjectEventNotifier();
		//
	private:
		//////////////////////////////////////////////////////////////////////////////////
		// System setting
		bool ProcRequestTimeSync(const DataCntr & dataCntr,
								 DataCntr & resultDataCntr);
		bool ProcRequestUserLevel(const DataCntr & dataCntr,
								  DataCntr & resultDataCntr);
		bool ProcRequestLanguage(const DataCntr & dataCntr,
								 DataCntr & resultDataCntr);
		bool ProcRequestModel(const DataCntr & dataCntr,
							  DataCntr & resultDataCntr);

		bool RaiseTimeSyncEvent(TimeSyncArgs::Args & args);
		bool RaiseUserLevelEvent(UserLevelArgs::Args & args);
		bool RaiseLanguageEvent(LanguageArgs::Args & args);
		bool RaiseModelEvent(ModelArgs::Args & args);
		//
		//////////////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////////////
		// Control
		bool ProcRequestInitialization(const DataCntr & dataCntr,
									   DataCntr & resultDataCntr);
		bool ProcRequestReset(const DataCntr & dataCntr,
							  DataCntr & resultDataCntr);
		bool ProcRequestRun(const DataCntr & dataCntr,
							DataCntr & resultDataCntr);
		bool ProcRequestStop(const DataCntr & dataCntr,
							 DataCntr & resultDataCntr);
		bool ProcRequestBuzzerOff(const DataCntr & dataCntr,
								  DataCntr & resultDataCntr);
		bool ProcRequestUiVisible(const DataCntr & dataCntr,
								  DataCntr & resultDataCntr);

		bool RaiseInitializationEvent();
		bool RaiseResetEvent();
		bool RaiseRunEvent();
		bool RaiseStopEvent();
		bool RaiseBuzzerOffEvent();
		bool RaiseUiVisibleEvent(UiVisibleArgs::Args & args);
		//
		//////////////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////////////
		// Operation
		bool ProcRequestSocketAccepting(const DataCntr & dataCntr,
										DataCntr & resultDataCntr);
		bool ProcRequestEjectAccepting(const DataCntr & dataCntr,
									   DataCntr & resultDataCntr);
		bool ProcProductionStartEnd(const DataCntr & dataCntr,
									DataCntr & resultDataCntr);
		bool ProcForceEject(const DataCntr & dataCntr,
							DataCntr & resultDataCntr);

		bool RaiseSocketAcceptingEvent(SocketAcceptingArgs::Args & args);
		bool RaiseEjectAcceptingEvent(EjectAcceptingArgs::Args & args);
		bool RaiseProductionStartEndEvent(ProductionStartEndArgs::Args & args);
		bool RaiseForceEjectEvent(ForceEjectArgs::Args & args);
		//
		//////////////////////////////////////////////////////////////////////////////////
	private:
		//////////////////////////////////////////////////////////////////////////////////
		// System setting
		using TimeSyncEventNotifierImpl =
			CCancellableEventNotifier<CIcsClient, TimeSyncArgs>;
		using UserLevelEventNotifierImpl =
			CCancellableEventNotifier<CIcsClient, UserLevelArgs>;
		using LanguageEventNotifierImpl =
			CCancellableEventNotifier<CIcsClient, LanguageArgs>;
		using ModelEventNotifierImpl =
			CCancellableEventNotifier<CIcsClient, ModelArgs>;

		TimeSyncEventNotifierImpl * m_pTimeSyncEventNotifier = nullptr;
		UserLevelEventNotifierImpl * m_pUserLevelEventNotifier = nullptr;
		LanguageEventNotifierImpl * m_pLanguageEventNotifier = nullptr;
		ModelEventNotifierImpl * m_pModelEventNotifier = nullptr;
		//
		//////////////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////////////
		// Control
		using InitializationEventNotifierImpl =
			CCancellableEventNotifier<CIcsClient, InitializationArgs>;
		using ResetEventNotifierImpl =
			CCancellableEventNotifier<CIcsClient, ResetArgs>;
		using RunEventNotifierImpl =
			CCancellableEventNotifier<CIcsClient, RunArgs>;
		using StopEventNotifierImpl =
			CCancellableEventNotifier<CIcsClient, StopArgs>;
		using BuzzerOffEventNotifierImpl =
			CCancellableEventNotifier<CIcsClient, BuzzerOffArgs>;
		using UiVisibleEventNotifierImpl =
			CCancellableEventNotifier<CIcsClient, UiVisibleArgs>;

		InitializationEventNotifierImpl * m_pInitializationEventNotifier = nullptr;
		ResetEventNotifierImpl * m_pResetEventNotifier = nullptr;
		RunEventNotifierImpl * m_pRunEventNotifier = nullptr;
		StopEventNotifierImpl * m_pStopEventNotifier = nullptr;
		BuzzerOffEventNotifierImpl * m_pBuzzerOffEventNotifier = nullptr;
		UiVisibleEventNotifierImpl * m_pUiVisibleEventNotifier = nullptr;
		//
		//////////////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////////////
		// Operation
		using SocketAcceptingEventNotifierImpl =
			CCancellableEventNotifier<CIcsClient, SocketAcceptingArgs>;
		using EjectAcceptingEventNotifierImpl =
			CCancellableEventNotifier<CIcsClient, EjectAcceptingArgs>;
		using ProductionStartEndEventNotifierImpl =
			CCancellableEventNotifier<CIcsClient, ProductionStartEndArgs>;
		using ForceEjectEventNotifierImpl =
			CCancellableEventNotifier<CIcsClient, ForceEjectArgs>;
		//

		SocketAcceptingEventNotifierImpl * m_pSocketAcceptingEventNotifier = nullptr;
		EjectAcceptingEventNotifierImpl * m_pEjectAcceptingEventNotifier = nullptr;
		ProductionStartEndEventNotifierImpl * m_pProductionStartEndEventNotifier = nullptr;
		ForceEjectEventNotifierImpl * m_pForceEjectEventNotifier = nullptr;
	};								  
}
