#pragma once

#include "Comm/ICommunicator.h"
#include "Logger/ILoggable.h"

namespace lt
{
	template<typename TypeSender, typename TypeArgs>
	class CBasicEventNotifier;
	class CLogger;
	class ILogDispatcher;

	class LTICS_LIB_CLASS CCommunicator abstract : public ICommunicator
	{
	public:
		using CommConnectionEventArgs = ICommunicator::CommConnectionEventArgs;
		using CommConnectionEventNotifier = ICommunicator::CommConnectionEventNotifier;
		using CommTxEventArgs = ICommunicator::CommTxEventArgs;
		using CommTxEventNotifier = ICommunicator::CommTxEventNotifier;
		using CommRxEventArgs = ICommunicator::CommRxEventArgs;
		using CommRxEventNotifier = ICommunicator::CommRxEventNotifier;

	private:
		using CommConnectionEventNotifierImpl =
			CBasicEventNotifier<CommConnectionEventArgs::Sender, CommConnectionEventArgs::Args>;
		using CommTxEventNotifierImpl =
			CBasicEventNotifier<CommTxEventArgs::Sender, CommTxEventArgs::Args>;
		using CommRxEventNotifierImpl =
			CBasicEventNotifier<CommRxEventArgs::Sender, CommRxEventArgs::Args>;

	public:
		CCommunicator(ConstStringT strPrefix);
		CCommunicator(const CCommunicator &) = delete;
		virtual ~CCommunicator();

		CCommunicator & operator=(const CCommunicator &) = delete;

		bool Connect(const SCommOption & option);
		void Disconnect();
		bool IsConnected() const override;

		size_t Send(const SCommData & data) override;

		CommConnectionEventNotifier & GetCommConnectedEventNotifier() override;
		CommConnectionEventNotifier & GetCommDisconnectingEventNotifier() override;

		CommTxEventNotifier & GetCommTxEventNotifier() override;
		CommRxEventNotifier & GetCommRxEventNotifier() override;

		ILogDispatcher * SetLogDispatcher(ILogDispatcher * pLogDispatcher);

	protected:
		ILoggable & GetLogger() const;

		virtual bool SetOption(const SCommOption & option) = 0;

		virtual bool ConnectComm() = 0;
		virtual void DisconnectComm() = 0;

		virtual size_t SendComm(const int8 * bufferPtr, size_t sizeOfBuffer) = 0;
		virtual size_t ReceiveComm(int8 * bufferPtr, size_t sizeOfBuffer) = 0;

		virtual bool ClearRx() = 0;

	private:
		void ProcComm(size_t sizeOfRxBuffer);
		void ProcReceiving(size_t sizeOfRxBuffer);

		bool IsCommShutdown() const;

		void OnConnectedEvent();
		void OnDisconnectingEvent();
		void OnTxEvent(const SCommTxData & data);
		void OnRxEvent(const SCommRxData & data);

	private:
		StdMutex * m_pConnectionMutex = nullptr;
		CLogger * m_pLogger = nullptr;
		bool m_bConnected = false;
		bool m_bCommShutdown = false;
		HANDLE m_hEventCommProcTerminated = nullptr;
		HANDLE m_hEventReceivingProcTerminated = nullptr;

		CommConnectionEventNotifierImpl * m_pCommConnectedEventNotifier = nullptr;
		CommConnectionEventNotifierImpl * m_pCommDisconnectingEventNotifier = nullptr;
		CommTxEventNotifierImpl * m_pCommTxEventNotifier = nullptr;
		CommRxEventNotifierImpl * m_pCommRxEventNotifier = nullptr;
	};
}
