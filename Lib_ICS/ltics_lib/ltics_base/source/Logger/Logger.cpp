#include "pch.h"
#include "Logger/Logger.h"
#include "Event/BasicEventNotifier.hpp"
#include "Logger/LogData.h"
#include "Logger/ILogDispatcher.h"
#include "Util/StringUtil.hpp"
#include "Util/SyncUtil.hpp"
#include <process.h>
#include <queue>

using namespace lt;

CLogger::CLogger()
{
	m_pCntr = new Cntr();
	m_pCntrMutex = new StdMutex();
	m_pLogMutex = new StdMutex();
	m_pLogLevelMutex = new StdMutex();
	m_pDispatcherMutex = new StdMutex();
	m_pStrPrefix = new StdStringT();
	m_pStrPostfix = new StdStringT();
}

CLogger::~CLogger()
{
	Detach();

	delete m_pCntr;
	delete m_pCntrMutex;
	delete m_pLogMutex;
	delete m_pLogLevelMutex;
	delete m_pDispatcherMutex;
	delete m_pStrPrefix;
	delete m_pStrPostfix;
}

ILoggable & CLogger::operator+=(ConstStringT string)
{
	SimpleLockPtr(m_pLogLevelMutex);

	//2023.06.28 TEST 임시 삭제
	AddLog(GetLogLevel(), string);

	return *this;
}

ILogDispatcher * CLogger::operator=(ILogDispatcher * pDispatcher)
{
	return Attach(pDispatcher);
}

bool CLogger::operator==(const ILogDispatcher * pDispatcher) const
{
	return GetLogDispatcher() == pDispatcher;
}

bool CLogger::operator!=(const ILogDispatcher * pDispatcher) const
{
	return GetLogDispatcher() != pDispatcher;
}

ILogDispatcher * CLogger::Attach(ILogDispatcher * pDispatcher)
{
	SimpleLockPtr(m_pDispatcherMutex);

	auto currentDispatcherPtr = m_pDispatcher;

	m_pDispatcher = pDispatcher;

	if(m_pDispatcher != nullptr)
	{
		SimpleLockPtr(m_pCntrMutex);

		for(; !m_pCntr->empty();)
		{
			m_pDispatcher->Dispatch(std::move(m_pCntr->front()));
			m_pCntr->pop();
		}
	}

	return currentDispatcherPtr;
}

ILogDispatcher * CLogger::Detach()
{
	return Attach(nullptr);
}

ILogDispatcher * CLogger::GetLogDispatcher()
{
	SimpleLockPtr(m_pDispatcherMutex);

	return m_pDispatcher;
}

const ILogDispatcher * CLogger::GetLogDispatcher() const
{
	SimpleLockPtr(m_pDispatcherMutex);

	return m_pDispatcher;
}

CLogger & CLogger::SetPrefix(ConstStringT string)
{
	SimpleLockPtr(m_pLogMutex);

	*m_pStrPrefix = string;

	return *this;
}

CLogger & CLogger::SetPostfix(ConstStringT string)
{
	SimpleLockPtr(m_pLogMutex);

	*m_pStrPostfix = string;

	return *this;
}

ILoggable & CLogger::SetLogLevel(LogLevel logLevel)
{
	SimpleLockPtr(m_pLogLevelMutex);

	m_logLevel = logLevel;

	return *this;
}

LogLevel CLogger::GetLogLevel() const
{
	return m_logLevel;
}

ILoggable & CLogger::AddLog(ConstStringT format, ...)
{
	SimpleLockPtr(m_pLogLevelMutex);

	va_list args;

	va_start(args, format);

	auto string = FormatV(format, args);

	va_end(args);

	AddLog(GetLogLevel(), string.c_str());


	return *this;
}

ILoggable & CLogger::AddLog(const ILogData & logData)
{
	AddLog(CLogData(logData.GetLoggingTime(),
					logData.GetLogLevel(),
					MakeSentence(logData.GetLogString())));
	return *this;
}

void CLogger::AddLog(LogLevel logLevel, ConstStringT string)
{
	auto logdata = CLogData(logLevel,
		MakeSentence(string));
	AddLog(logdata);
	AddLog(CLogData(logLevel,
		MakeSentence(string)));	
}

void CLogger::AddLog(CLogData && logData)
{
	if(!DispatchLog(std::move(logData)))
		PushLog(std::move(logData));

	logData.~CLogData();
}

StdStringT CLogger::MakeSentence(ConstStringT string)
{
	static long lcount = 0;//2022.12.10a uhkim

	SimpleLockPtr(m_pLogMutex);

	//2022.12.26a uhkim
	auto logString = Format(_T("[%d]%s%s%s"),
		lcount++,
		m_pStrPrefix->c_str(),
		string,
		m_pStrPostfix->c_str());

	//auto logString = Format(_T("%s%s%s"),
	//						m_pStrPrefix->c_str(),
	//						string,
	//						m_pStrPostfix->c_str());

	return logString;
}

bool CLogger::DispatchLog(CLogData && logData)
{
	SimpleLockPtr(m_pDispatcherMutex);

	auto result = m_pDispatcher != nullptr;

	if(result)
		m_pDispatcher->Dispatch(std::move(logData));

	return result;
}

void CLogger::PushLog(CLogData && logData)
{
	SimpleLockPtr(m_pCntrMutex);

	m_pCntr->push(std::move(logData));
}
