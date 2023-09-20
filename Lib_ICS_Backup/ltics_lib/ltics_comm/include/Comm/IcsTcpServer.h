#pragma once

#include "Enumerator/IEnumerableContainer.h"
#include "Comm/ICommunicator.h"
#include "Logger/ILogDispatcher.h"

namespace lt
{
	class ILoggable;
	class CLogger;
	class CTcpCommunicator;
	template<typename TypeSender, typename TypeArgs>
	class CBasicEventNotifier;
	template<typename TypeEventArgs>
	class IEventListener;
	template<typename TypeOwner, typename TypeEventArgs>
	class CEventMfnListener;

	class LTICS_LIB_CLASS CIcsTcpServer final : public IEnumerableContainer<ICommunicator>
	{
	public:
		CIcsTcpServer();
		CIcsTcpServer(const CIcsTcpServer &) = delete;
		~CIcsTcpServer();

		CIcsTcpServer & operator=(const CIcsTcpServer &) = delete;

		ILogDispatcher * SetLogDispatcher(ILogDispatcher * logDispatcherPtr);

		bool Open(ConstStringT address, ushort port, size_t sizeOfRxBuffer);
		void Close();
		bool IsOpened() const;

		using Value = IEnumerableContainer<ICommunicator>::Value;
		using ConstValue = IEnumerableContainer<ICommunicator>::ConstValue;

		Value & operator[](size_t index) override;
		ConstValue & operator[](size_t index) const override;

		size_t GetCount() const override;

		using RemoteConnectionEventArgs = IEventArgs<CIcsTcpServer, ICommunicator>;
		using RemoteConnectionEventNotifier = IEventNotifier<RemoteConnectionEventArgs>;

		RemoteConnectionEventNotifier & GetRemoteConnectedEventNotifier();
		RemoteConnectionEventNotifier & GetRemoteDisconnectedEventNotifier();

	private:
		CLogger & GetLogger();
		ILoggable & GetLogger() const;

		CTcpCommunicator & GetCommServer();
		const CTcpCommunicator & GetCommServer() const;

		size_t GetSizeOfRxBuffer() const;

		using CommConnectionEventArgs = IEventArgs<ICommunicator, const SCommOption>;
		using CommConnectionEventListener = IEventListener<CommConnectionEventArgs>;

		CommConnectionEventListener * GetCommConnectedEventListener() const;
		CommConnectionEventListener * GetCommDisconnectingEventListener() const;

		void BeginListening();
		void EndListening();

		void SetListeningShutdown(bool shutdown);
		bool IsListeningShutdown() const;

		void SetListeningTerminated(bool terminated);
		bool IsListeningTerminated() const;
		bool WaitForListeningTerminated(DWORD timeout) const;

		void AddRemoteComm(CTcpCommunicator * commPtr);
		CTcpCommunicator * GetRemoteComm(size_t index) const;
		void RemoveRemoteComm(const ICommunicator * commPtr);
		void DisconnectRemoteComms();
		size_t GetNumberOfRemoteComms() const;

		void RaiseRemoteConnectedEvent(ICommunicator & comm);
		void RaiseRemoteDisconnectedEvent(ICommunicator & comm);

		void OnCommConnectedEvent(CommConnectionEventArgs & eventArgs);
		void OnCommDisconnectingEvent(CommConnectionEventArgs & eventArgs);

	private:
		bool m_opened = false;

		CLogger * m_pLogger = nullptr;

		CTcpCommunicator * m_pCommServer = nullptr;
		size_t m_sizeOfRxBuffer = 0;

		bool m_bListeningShutdown = false;
		HANDLE m_hEventListeningTerminated = nullptr;

		using RemoteCommCntr = StdSequenceContainer<CTcpCommunicator *>::Vector;

		RemoteCommCntr * m_pRemoteCommCntr = nullptr;
		StdMutex * m_pRemoteCommCntrMutex = nullptr;

		using RemoteConnectionEventNotifierImpl =
			CBasicEventNotifier<RemoteConnectionEventArgs::Sender,
								RemoteConnectionEventArgs::Args>;

		RemoteConnectionEventNotifierImpl * m_pRemoteConnectedEventNotifier = nullptr;
		RemoteConnectionEventNotifierImpl * m_pRemoteDisconnectedEventNotifier = nullptr;

		using CommConnectionEventListenerImpl =
			CEventMfnListener<CIcsTcpServer, CommConnectionEventListener::EventArgs>;

		CommConnectionEventListenerImpl * m_pCommConnectedEventListener = nullptr;
		CommConnectionEventListenerImpl * m_pCommDisconnectingEventListener = nullptr;
	};
}
