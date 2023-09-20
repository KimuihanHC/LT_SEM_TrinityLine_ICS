#include "pch.h"
#include "Logger/LogData.h"
#include "Util/MemoryUtil.hpp"
#include <string>

using namespace lt;

CLogData::CLogData(LogLevel logLevel, ConstStringT string)
: CLogData(logLevel, StdStringT(string))
{
}

CLogData::CLogData(LogLevel logLevel, StdStringT && string)
: m_logLevel(logLevel)
{
	::GetLocalTime(&m_loggingTime);
	m_pString = new StdStringT(std::move(string));
}

CLogData::CLogData(const SYSTEMTIME & loggingTime, LogLevel logLevel, ConstStringT string)
: CLogData(loggingTime, logLevel, StdStringT(string))
{
}

CLogData::CLogData(const SYSTEMTIME & loggingTime, LogLevel logLevel, StdStringT && string)
: m_loggingTime(loggingTime)
, m_logLevel(logLevel)
{
	m_pString = new StdStringT(std::move(string));
}

CLogData::CLogData(const ILogData & rhs)
: CLogData(rhs.GetLoggingTime(), rhs.GetLogLevel(), rhs.GetLogString())
{
}

CLogData::CLogData(CLogData && rhs)
{
	Swap(m_loggingTime, rhs.m_loggingTime);
	Swap(m_logLevel, rhs.m_logLevel);
	Swap(m_pString, rhs.m_pString);
}

CLogData::~CLogData()
{
	SafeDelete(m_pString);
}

CLogData & CLogData::operator=(const ILogData & rhs)
{
	memcpy(&m_loggingTime, &rhs.GetLoggingTime(), sizeof(m_loggingTime));
	m_logLevel = rhs.GetLogLevel();
	*m_pString = rhs.GetLogString();

	return *this;
}

CLogData & CLogData::operator=(CLogData && rhs)
{
	m_loggingTime = {};
	m_logLevel = LogLevel::None;
	SafeDelete(m_pString);

	Swap(m_loggingTime, rhs.m_loggingTime);
	Swap(m_logLevel, rhs.m_logLevel);
	Swap(m_pString, rhs.m_pString);

	return *this;
}

const SYSTEMTIME & CLogData::GetLoggingTime() const
{
	return m_loggingTime;
}

LogLevel CLogData::GetLogLevel() const
{
	return m_logLevel;
}

ConstStringT CLogData::GetLogString() const
{
	if(m_pString == nullptr)
		return nullptr;

	return m_pString->c_str();
}
