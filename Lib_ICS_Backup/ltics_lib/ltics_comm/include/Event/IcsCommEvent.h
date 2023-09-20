#pragma once

#include "Event/IEventNotifier.h"
#include "Event/ICancellableEventArgs.h"
#include "Comm/IcsRequestTypedArgs.h"
#include "Comm/IcsCommArgs.h"

namespace lt
{
	//////////////////////////////////////////////////////////////////////////////////
	// System setting
	using TimeSyncArgs =
		CIcsRequestTypedArgs<const STimeSync>;
	template<typename TypeSender>
	using TimeSyncEventArgs =
		ICancellableEventArgs<TypeSender, TimeSyncArgs>;
	template<typename TypeSender>
	using TimeSyncEventNotifier =
		IEventNotifier<TimeSyncEventArgs<TypeSender>>;

	using UserLevelArgs =
		CIcsRequestTypedArgs<const SUserLevel>;
	template<typename TypeSender>
	using UserLevelEventArgs =
		ICancellableEventArgs<TypeSender, UserLevelArgs>;
	template<typename TypeSender>
	using UserLevelEventNotifier =
		IEventNotifier<UserLevelEventArgs<TypeSender>>;

	using LanguageArgs =
		CIcsRequestTypedArgs<const SLanguage>;
	template<typename TypeSender>
	using LanguageEventArgs =
		ICancellableEventArgs<TypeSender, LanguageArgs>;
	template<typename TypeSender>
	using LanguageEventNotifier =
		IEventNotifier<LanguageEventArgs<TypeSender>>;

	using EquipmentIdArgs =
		CIcsRequestTypedArgs<const SEquipmentId>;
	template<typename TypeSender>
	using EquipmentIdEventArgs =
		ICancellableEventArgs<TypeSender, EquipmentIdArgs>;
	template<typename TypeSender>
	using EquipmentIdEventNotifier =
		IEventNotifier<EquipmentIdEventArgs<TypeSender>>;

	using ModelArgs =
		CIcsRequestTypedArgs<const SModel>;
	template<typename TypeSender>
	using ModelEventArgs =
		ICancellableEventArgs<TypeSender, ModelArgs>;
	template<typename TypeSender>
		using ModelEventNotifier =
		IEventNotifier<ModelEventArgs<TypeSender>>;
	//
	//////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////
	// Control
	using InitializationArgs =
		CIcsRequestArgs;
	template<typename TypeSender>
	using InitializationEventArgs =
		ICancellableEventArgs<TypeSender, InitializationArgs>;
	template<typename TypeSender>
	using InitializationEventNotifier =
		IEventNotifier<InitializationEventArgs<TypeSender>>;

	using ResetArgs =
		CIcsRequestArgs;
	template<typename TypeSender>
	using ResetEventArgs =
		ICancellableEventArgs<TypeSender, ResetArgs>;
	template<typename TypeSender>
	using ResetEventNotifier =
		IEventNotifier<ResetEventArgs<TypeSender>>;

	using RunArgs =
		CIcsRequestArgs;
	template<typename TypeSender>
	using RunEventArgs =
		ICancellableEventArgs<TypeSender, RunArgs>;
	template<typename TypeSender>
	using RunEventNotifier =
		IEventNotifier<RunEventArgs<TypeSender>>;

	using StopArgs =
		CIcsRequestArgs;
	template<typename TypeSender>
	using StopEventArgs =
		ICancellableEventArgs<TypeSender, StopArgs>;
	template<typename TypeSender>
	using StopEventNotifier =
		IEventNotifier<StopEventArgs<TypeSender>>;

	using BuzzerOffArgs =
		CIcsRequestArgs;
	template<typename TypeSender>
	using BuzzerOffEventArgs =
		ICancellableEventArgs<TypeSender, BuzzerOffArgs>;
	template<typename TypeSender>
	using BuzzerOffEventNotifier =
		IEventNotifier<BuzzerOffEventArgs<TypeSender>>;

	using UiVisibleArgs =
		CIcsRequestTypedArgs<const SUiVisible>;
	template<typename TypeSender>
	using UiVisibleEventArgs =
		ICancellableEventArgs<TypeSender, UiVisibleArgs>;
	template<typename TypeSender>
	using UiVisibleEventNotifier =
		IEventNotifier<UiVisibleEventArgs<TypeSender>>;
	//
	//////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////
	// Operation
	using EquipmentStatusArgs =
		CIcsRequestTypedArgs<const SEquipmentStatus>;
	template<typename TypeSender>
	using EquipmentStatusEventArgs =
		ICancellableEventArgs<TypeSender, EquipmentStatusArgs>;
	template<typename TypeSender>
	using EquipmentStatusEventNotifier =
		IEventNotifier<EquipmentStatusEventArgs<TypeSender>>;

	using OperationModeArgs =
		CIcsRequestTypedArgs<const SOperationMode>;
	template<typename TypeSender>
	using OperationModeEventArgs =
		ICancellableEventArgs<TypeSender, OperationModeArgs>;
	template<typename TypeSender>
	using OperationModeEventNotifier =
		IEventNotifier<OperationModeEventArgs<TypeSender>>;

	using SocketRequestArgs =
		CIcsRequestTypedArgs<const SSocketRfid>;
	template<typename TypeSender>
	using SocketRequestEventArgs =
		ICancellableEventArgs<TypeSender, SocketRequestArgs>;
	template<typename TypeSender>
	using SocketRequestEventNotifier =
		IEventNotifier<SocketRequestEventArgs<TypeSender>>;

	using SocketAcceptingArgs =
		CIcsRequestTypedArgs<const SSocketAccepting>;
	template<typename TypeSender>
	using SocketAcceptingEventArgs =
		ICancellableEventArgs<TypeSender, SocketAcceptingArgs>;
	template<typename TypeSender>
	using SocketAcceptingEventNotifier =
		IEventNotifier<SocketAcceptingEventArgs<TypeSender>>;

	using EjectAcceptingArgs =
		CIcsRequestTypedArgs<const SSocketRfid>;
	template<typename TypeSender>
	using EjectAcceptingEventArgs =
		ICancellableEventArgs<TypeSender, EjectAcceptingArgs>;
	template<typename TypeSender>
	using EjectAcceptingEventNotifier =
		IEventNotifier<EjectAcceptingEventArgs<TypeSender>>;

	using ProductionStartEndArgs =
		CIcsRequestTypedArgs<const SProductionStartEnd>;
	template<typename TypeSender>
	using ProductionStartEndEventArgs =
		ICancellableEventArgs<TypeSender, ProductionStartEndArgs>;
	template<typename TypeSender>
	using ProductionStartEndEventNotifier =
		IEventNotifier<ProductionStartEndEventArgs<TypeSender>>;

	using ForceEjectArgs =
		CIcsRequestTypedArgs<const SForceEject>;
	template<typename TypeSender>
	using ForceEjectEventArgs =
		ICancellableEventArgs<TypeSender, ForceEjectArgs>;
	template<typename TypeSender>
	using ForceEjectEventNotifier =
		IEventNotifier<ForceEjectEventArgs<TypeSender>>;

	using LoadingArgs =
		CIcsRequestTypedArgs<const SSocketInfo>;
	template<typename TypeSender>
	using LoadingEventArgs =
		ICancellableEventArgs<TypeSender, LoadingArgs>;
	template<typename TypeSender>
	using LoadingEventNotifier =
		IEventNotifier<LoadingEventArgs<TypeSender>>;

	using TestResultArgs =
		CIcsRequestTypedArgs<const STestResult>;
	template<typename TypeSender>
	using TestResultEventArgs =
		ICancellableEventArgs<TypeSender, TestResultArgs>;
	template<typename TypeSender>
	using TestResultEventNotifier =
		IEventNotifier<TestResultEventArgs<TypeSender>>;

	using UnloadingArgs =
		CIcsRequestTypedArgs<const SSocketRfid>;
	template<typename TypeSender>
	using UnloadingEventArgs =
		ICancellableEventArgs<TypeSender, UnloadingArgs>;
	template<typename TypeSender>
	using UnloadingEventNotifier =
		IEventNotifier<UnloadingEventArgs<TypeSender>>;

	using TestResultRequestArgs =
		CIcsRequestTypedArgs<STestResult2>;
	template<typename TypeSender>
	using TestResultRequestEventArgs =
		ICancellableEventArgs<TypeSender, TestResultRequestArgs>;
	template<typename TypeSender>
	using TestResultRequestEventNotifier =
		IEventNotifier<TestResultRequestEventArgs<TypeSender>>;

	using PortClearArgs =
		CIcsRequestTypedArgs<const SPort>;
	template<typename TypeSender>
	using PortClearEventArgs =
		ICancellableEventArgs<TypeSender, PortClearArgs>;
	template<typename TypeSender>
	using PortClearEventNotifier =
		IEventNotifier<PortClearEventArgs<TypeSender>>;

	using PortStatusArgs =
		CIcsRequestTypedArgs<const SPortStatus>;
	template<typename TypeSender>
	using PortStatusEventArgs =
		ICancellableEventArgs<TypeSender, PortStatusArgs>;
	template<typename TypeSender>
	using PortStatusEventNotifier =
		IEventNotifier<PortStatusEventArgs<TypeSender>>;

	using PortStatusRequestArgs =
		CIcsRequestTypedArgs<SPortStatus2>;
	template<typename TypeSender>
	using PortStatusRequestEventArgs =
		ICancellableEventArgs<TypeSender, PortStatusRequestArgs>;
	template<typename TypeSender>
	using PortStatusRequestEventNotifier =
		IEventNotifier<PortStatusRequestEventArgs<TypeSender>>;

	using ConveyorStatusArgs =
		CIcsRequestTypedArgs<const SConveyorStatus>;
	template<typename TypeSender>
	using ConveyorStatusEventArgs =
		ICancellableEventArgs<TypeSender, ConveyorStatusArgs>;
	template<typename TypeSender>
	using ConveyorStatusEventNotifier =
		IEventNotifier<ConveyorStatusEventArgs<TypeSender>>;

	using ConveyorStatusRequestArgs =
		CIcsRequestTypedArgs<SConveyorStatus2>;
	template<typename TypeSender>
	using ConveyorStatusRequestEventArgs =
		ICancellableEventArgs<TypeSender, ConveyorStatusRequestArgs>;
	template<typename TypeSender>
	using ConveyorStatusRequestEventNotifier =
		IEventNotifier<ConveyorStatusRequestEventArgs<TypeSender>>;
}
