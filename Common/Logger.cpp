#include "stdafx.h"
#include "Logger.h"
#include "StringFormat.h"
#include <debugapi.h>
#include <mutex>
#include <queue>
#include <thread>

CLogger CLogger::debugLogger = &OutputDebugString;

CLogger::CLogger()
: m_pfnCallback(nullptr)
, m_pCallbackLocker(nullptr)
, m_pLogQueue(nullptr)
, m_pLogQueueLocker(nullptr)
, m_hThreadLog(nullptr)
, m_bShutdown(false)
{
	Initialize();
}

CLogger::CLogger(PFN_CALLBACK pfnCallback)
: m_pfnCallback(pfnCallback)
, m_pCallbackLocker(nullptr)
, m_pLogQueue(nullptr)
, m_pLogQueueLocker(nullptr)
, m_hThreadLog(nullptr)
, m_bShutdown(false)
{
	Initialize();
}

CLogger::~CLogger()
{
	Release();
}

const CLogger & CLogger::GetDebugLogger()
{
	return CLogger::debugLogger;
}

void CLogger::SetCallback(PFN_CALLBACK pfnCallback)
{
	LockGuard locker(*m_pCallbackLocker);

	m_pfnCallback = pfnCallback;
}

void CLogger::OnAdd_Log(LPCTSTR lpszLog, ...) const
{
	va_list args;

	va_start(args, lpszLog);

	auto strLog = Format(lpszLog, args);

	va_end(args);

	LockGuard locker(*m_pLogQueueLocker);

	m_pLogQueue->push(std::move(strLog));
}

CLogger & CLogger::operator=(CLogger::PFN_CALLBACK pfnCallback)
{
	SetCallback(pfnCallback);

	return *this;
}

void CLogger::Initialize()
{
	m_bShutdown = false;

	m_pCallbackLocker = new Mutex();
	m_pLogQueue = new LogQueue();
	m_pLogQueueLocker = new Mutex();
	m_hThreadLog = reinterpret_cast <HANDLE> (_beginthreadex(nullptr, 0, [](void * pvParam)->unsigned {
		auto loggerPtr = static_cast <CLogger *> (pvParam);

		loggerPtr->LogProc();

		_endthreadex(0);

		return 0;
	}, this, 0, nullptr));
}

void CLogger::Release()
{
	m_bShutdown = true;

	::WaitForSingleObject(m_hThreadLog, INFINITE);

	delete m_pCallbackLocker;
	delete m_pLogQueue;
	delete m_pLogQueueLocker;
	::CloseHandle(m_hThreadLog);
}

void CLogger::LogProc()
{
	for(; !m_bShutdown;)
	{
		StringT strLog;

		{
			LockGuard locker(*m_pLogQueueLocker);

			if(m_pLogQueue->empty())
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
				continue;
			}

			strLog = std::move(m_pLogQueue->front());
			m_pLogQueue->pop();
		}

		LockGuard locker(*m_pCallbackLocker);

		if(m_pfnCallback != nullptr)
			m_pfnCallback(strLog.c_str());
	}
}
