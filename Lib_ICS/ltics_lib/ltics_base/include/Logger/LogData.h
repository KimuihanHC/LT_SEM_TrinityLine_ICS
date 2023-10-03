#pragma once

#include "Logger/ILogData.h"

namespace lt
{
	class LTICS_LIB_CLASS CLogData final : public ILogData
	{
	public:
		CLogData() = default;
		CLogData(LogLevel logLevel, ConstStringT string);
		CLogData(LogLevel logLevel, StdStringT && string);
		CLogData(const SYSTEMTIME & loggingTime, LogLevel logLevel, ConstStringT string);
		CLogData(const SYSTEMTIME & loggingTime, LogLevel logLevel, StdStringT && string);
		CLogData(const ILogData & rhs);
		CLogData(CLogData && rhs);
		~CLogData();

		CLogData & operator=(const ILogData & rhs);
		CLogData & operator=(CLogData && rhs);

		const SYSTEMTIME & GetLoggingTime() const override;
		LogLevel GetLogLevel() const override;
		ConstStringT GetLogString() const override;

	private:
		SYSTEMTIME m_loggingTime = {};
		LogLevel m_logLevel = LogLevel::None;
		StdStringT * m_pString = nullptr;
	};
}
