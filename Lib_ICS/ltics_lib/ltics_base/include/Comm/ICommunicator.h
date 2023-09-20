#pragma once

#include "ltics_type.h"
#include "Event/IEventNotifier.h"
#include "Event/IEventArgs.h"
#include "Comm/CommOption.h"
#include "Comm/CommData.h"

namespace lt
{
	class LTICS_LIB_CLASS ICommunicator abstract
	{
	public:
		using CommConnectionEventArgs = IEventArgs<ICommunicator, const SCommOption>;
		using CommConnectionEventNotifier = IEventNotifier<CommConnectionEventArgs>;
		using CommTxEventArgs = IEventArgs<ICommunicator, const SCommTxData>;
		using CommTxEventNotifier = IEventNotifier<CommTxEventArgs>;
		using CommRxEventArgs = IEventArgs<ICommunicator, const SCommRxData>;
		using CommRxEventNotifier = IEventNotifier<CommRxEventArgs>;

		virtual ~ICommunicator() = default;

		virtual const SCommOption & GetOption() const = 0;

		virtual bool IsConnected() const = 0;

		virtual size_t Send(const SCommData & data) = 0;

		virtual CommConnectionEventNotifier & GetCommConnectedEventNotifier() = 0;
		virtual CommConnectionEventNotifier & GetCommDisconnectingEventNotifier() = 0;

		virtual CommTxEventNotifier & GetCommTxEventNotifier() = 0;
		virtual CommRxEventNotifier & GetCommRxEventNotifier() = 0;
	};
}
