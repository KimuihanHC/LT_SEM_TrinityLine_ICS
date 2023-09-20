#pragma once

#include "ltics_type.h"
#include "TypeDecl.h"
#include "Logger/LogLevel.h"

namespace lt
{
	class ILogData;

	class LTICS_LIB_CLASS ILoggable abstract
	{
	public:
		virtual ~ILoggable() = default;

		virtual ILoggable & operator+=(ConstStringT string) = 0;

		virtual ILoggable & SetLogLevel(LogLevel logLevel) = 0;
		virtual LogLevel GetLogLevel() const = 0;

		virtual ILoggable & AddLog(ConstStringT format, ...) = 0;
		virtual ILoggable & AddLog(const ILogData & logData) = 0;
	};
}
