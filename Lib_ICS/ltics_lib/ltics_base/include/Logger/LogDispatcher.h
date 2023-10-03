#pragma once

#include "Logger/ILogDispatcher.h"
#include "Logger/LogFilter.h"
#include "Event/IEventNotifier.h"
#include "Event/IEventArgs.h"

namespace lt
{
	template<typename TypeSender, typename TypeArgs>
	class CBasicEventNotifier;

	class LTICS_LIB_CLASS CLogDispatcher final : public ILogDispatcher
	{
	public:
		using EventNotifier = IEventNotifier<IEventArgs<ILogDispatcher, ILogData>>;

	private:
		using Cntr = StdSequenceContainer<CLogData>::Queue;
		using EventNotifierImpl = CBasicEventNotifier<EventNotifier::EventArgs::Sender,
													  EventNotifier::EventArgs::Args>;

	public:
		CLogDispatcher();
		CLogDispatcher(const CLogDispatcher &) = delete;
		~CLogDispatcher();

		CLogDispatcher & operator=(const CLogDispatcher &) = delete;

		EventNotifier & GetLogAddedEventNotifier();

		CLogDispatcher & SetLogFilter(LogFilter logFilter);
		LogFilter GetLogFilter() const;

		ILogDispatcher & Dispatch(LogLevel logLevel, ConstStringT string) override;
		ILogDispatcher & Dispatch(const SYSTEMTIME & loggingTime,
								  LogLevel logLevel, ConstStringT string) override;
		ILogDispatcher & Dispatch(const ILogData & logData) override;
		ILogDispatcher & Dispatch(CLogData && logData) override;

	private:
		void PushLog(CLogData && logData);
		bool PopLog(CLogData & logData);

		void OnLogAdded(ILogData & logData);

		void ProcDispatch();

	private:
		Cntr * m_pCntr = nullptr;
		StdMutex * m_pCntrMutex = nullptr;
		EventNotifierImpl * m_pEventNotifier = nullptr;
		StdThread * m_pThreadDispatching = nullptr;
		bool m_bShutdown = false;
		LogFilter m_logFilter = LogFilter::Normal;
	};
}
