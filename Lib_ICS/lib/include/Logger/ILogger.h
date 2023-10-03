#pragma once

#include "Logger/ILoggable.h"
#include "Logger/ILogDispatcher.h"

namespace lt
{
	class LTICS_LIB_CLASS ILogger abstract : public ILoggable
	{
	public:
		virtual ~ILogger() = default;

		virtual ILogDispatcher * operator=(ILogDispatcher * pDispatcher) = 0;

		virtual bool operator==(const ILogDispatcher * pDispatcher) const = 0;
		virtual bool operator!=(const ILogDispatcher * pDispatcher) const = 0;

		virtual ILogDispatcher * Attach(ILogDispatcher * pDispatcher) = 0;
		virtual ILogDispatcher * Detach() = 0;

		virtual ILogDispatcher * GetLogDispatcher() = 0;
		virtual const ILogDispatcher * GetLogDispatcher() const = 0;
	};
}
