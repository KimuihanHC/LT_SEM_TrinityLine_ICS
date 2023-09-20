#pragma once

#include "Logger/ILogDispatcher.h"

namespace lt
{
	class LTICS_LIB_CLASS IDebugLogDispatcher abstract
	{
	public:
		virtual ~IDebugLogDispatcher() = default;

		virtual operator ILogDispatcher *() = 0;
	};
}
