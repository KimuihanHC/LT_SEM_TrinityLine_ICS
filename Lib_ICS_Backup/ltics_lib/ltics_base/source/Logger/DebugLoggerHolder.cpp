#include "pch.h"
#include "Logger/DebugLoggerHolder.h"
#include "Logger/Logger.h"
#include "Logger/IDebugLogDispatcher.h"

using namespace lt;

CDebugLoggerHolder::CDebugLoggerHolder(ConstStringT strPrefix, IDebugLogDispatcher & debugLogDispatcher)
{
	m_pLogger = new CLogger();
	m_pLogger->SetPrefix(strPrefix);
	m_pLogger->Attach(debugLogDispatcher);
}

CDebugLoggerHolder::~CDebugLoggerHolder()
{
	delete m_pLogger;
}

ILoggable & CDebugLoggerHolder::GetLogger() const
{
	return *m_pLogger;
}
