#pragma once

#include "ltics_type.h"
#include "TypeDecl.h"
#include "Logger/LogLevel.h"

namespace lt
{
	class LTICS_LIB_CLASS ILogData abstract
	{
	public:
		virtual ~ILogData() = default;

		virtual const SYSTEMTIME & GetLoggingTime() const = 0;
		virtual LogLevel GetLogLevel() const = 0;
		virtual ConstStringT GetLogString() const = 0;
	};
}
