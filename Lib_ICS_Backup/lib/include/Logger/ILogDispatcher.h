#pragma once

#include "ltics_type.h"
#include "Logger/ILogData.h"

namespace lt
{
	class CLogData;

	class LTICS_LIB_CLASS ILogDispatcher abstract
	{
	public:
		virtual ~ILogDispatcher() = default;

		virtual ILogDispatcher & Dispatch(LogLevel logLevel, ConstStringT string) = 0;
		virtual ILogDispatcher & Dispatch(const SYSTEMTIME & loggingTime,
										  LogLevel logLevel, ConstStringT string) = 0;
		virtual ILogDispatcher & Dispatch(const ILogData & logData) = 0;
		virtual ILogDispatcher & Dispatch(CLogData && logData) = 0;
	};
}
