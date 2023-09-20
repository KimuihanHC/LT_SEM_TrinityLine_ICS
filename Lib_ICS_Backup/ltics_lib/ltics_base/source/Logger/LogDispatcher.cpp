#include "pch.h"
#include "Logger/LogDispatcher.h"
#include "Logger/LogData.h"
#include "Event/BasicEventNotifier.hpp"
#include "Util/SyncUtil.hpp"
#include <thread>
#include <queue>

using namespace lt;

CLogDispatcher::CLogDispatcher()
{
	m_pCntr = new Cntr();
	m_pCntrMutex = new StdMutex();
	m_pEventNotifier = new EventNotifierImpl();
	m_pThreadDispatching = new StdThread(&CLogDispatcher::ProcDispatch, this);
}

CLogDispatcher::~CLogDispatcher()
{
	m_bShutdown = true;

	m_pThreadDispatching->join();

	delete m_pThreadDispatching;

	delete m_pCntr;
	delete m_pCntrMutex;
	delete m_pEventNotifier;
}

CLogDispatcher::EventNotifier & CLogDispatcher::GetLogAddedEventNotifier()
{
	return *m_pEventNotifier;
}

CLogDispatcher & CLogDispatcher::SetLogFilter(LogFilter logFilter)
{
	SimpleLockPtr(m_pCntrMutex);

	m_logFilter = logFilter;

	return *this;
}

LogFilter CLogDispatcher::GetLogFilter() const
{
	return m_logFilter;
}

ILogDispatcher & CLogDispatcher::Dispatch(LogLevel logLevel, ConstStringT string)
{
	return Dispatch(CLogData(logLevel, string));
}

ILogDispatcher & CLogDispatcher::Dispatch(const SYSTEMTIME & loggingTime,
										  LogLevel logLevel, ConstStringT string)
{
	return Dispatch(CLogData(loggingTime, logLevel, string));
}

ILogDispatcher & CLogDispatcher::Dispatch(const ILogData & logData)
{
	return Dispatch(CLogData(logData));
}

ILogDispatcher & CLogDispatcher::Dispatch(CLogData && logData)
{
	PushLog(std::move(logData));

	return *this;
}

void CLogDispatcher::PushLog(CLogData && logData)
{
	SimpleLockPtr(m_pCntrMutex);

	auto approved = false;

	switch(GetLogFilter())
	{
	case LogFilter::Simple:
		approved = logData.GetLogLevel() >= LogLevel::Warning;
		break;
	case LogFilter::Normal:
		approved = logData.GetLogLevel() >= LogLevel::Proc;
		break;
	case LogFilter::Detail:
		approved = true;
		break;
	}

	if(approved)
		m_pCntr->push(std::move(logData));
}

bool CLogDispatcher::PopLog(CLogData & logData)
{
	SimpleLockPtr(m_pCntrMutex);

	if(m_pCntr->empty())
		return false;

	logData = std::move(m_pCntr->front());
	m_pCntr->pop();

	return true;
}

void CLogDispatcher::OnLogAdded(ILogData & logData)
{
	m_pEventNotifier->OnEvent(*this, logData);
}

void CLogDispatcher::ProcDispatch()
{
	CLogData logData;

	for(; !m_bShutdown;)
	{
		for (; PopLog(logData);) 
			OnLogAdded(logData);		
		
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}
