#pragma once

#include "Logger/ILogger.h"

namespace lt
{
	class CLogData;

	class LTICS_LIB_CLASS CLogger final : public ILogger
	{
	private:
		using Cntr = StdSequenceContainer<CLogData>::Queue;

	public:
		CLogger();
		CLogger(const CLogger &) = delete;
		~CLogger();

		CLogger & operator=(const CLogger &) = delete;

		ILoggable & operator+=(ConstStringT string) override;

		ILogDispatcher * operator=(ILogDispatcher * pDispatcher) override;

		bool operator==(const ILogDispatcher * pDispatcher) const override;
		bool operator!=(const ILogDispatcher * pDispatcher) const override;

		ILogDispatcher * Attach(ILogDispatcher * pDispatcher) override;
		ILogDispatcher * Detach() override;

		ILogDispatcher * GetLogDispatcher() override;
		const ILogDispatcher * GetLogDispatcher() const override;

		CLogger & SetPrefix(ConstStringT string);
		CLogger & SetPostfix(ConstStringT string);

		ILoggable & SetLogLevel(LogLevel logLevel) override;
		LogLevel GetLogLevel() const override;

		ILoggable & AddLog(ConstStringT format, ...) override;
		ILoggable & AddLog(const ILogData & logData) override;

	private:
		void AddLog(LogLevel logLevel, ConstStringT string);
		void AddLog(CLogData && logData);

		StdStringT MakeSentence(ConstStringT string);

		bool DispatchLog(CLogData && logData);
		void PushLog(CLogData && logData);

	private:
		Cntr * m_pCntr = nullptr;
		StdMutex * m_pCntrMutex = nullptr;
		StdMutex * m_pLogMutex = nullptr;
		StdMutex * m_pLogLevelMutex = nullptr;
		ILogDispatcher * m_pDispatcher = nullptr;
		StdMutex * m_pDispatcherMutex = nullptr;
		LogLevel m_logLevel = LogLevel::None;
		StdStringT * m_pStrPrefix = nullptr;
		StdStringT * m_pStrPostfix = nullptr;
	};
}
