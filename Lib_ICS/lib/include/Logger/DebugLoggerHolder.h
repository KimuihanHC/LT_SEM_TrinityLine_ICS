#pragma once

#include "Logger/ILoggable.h"

namespace lt
{
	class IDebugLogDispatcher;
	class CLogger;

	class LTICS_LIB_CLASS CDebugLoggerHolder
	{
	public:
		CDebugLoggerHolder(ConstStringT strPrefix, IDebugLogDispatcher & debugLogDispatcher);
		CDebugLoggerHolder(const CDebugLoggerHolder &) = delete;
		~CDebugLoggerHolder();

		CDebugLoggerHolder & operator=(const CDebugLoggerHolder &) = delete;

	protected:
		ILoggable & GetLogger() const;

	private:
		CLogger * m_pLogger = nullptr;
	};
}
