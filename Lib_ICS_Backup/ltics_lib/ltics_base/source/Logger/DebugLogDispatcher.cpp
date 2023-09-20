#include "pch.h"
#include "Logger/DebugLogDispatcher.h"
#include "Logger/LogDispatcher.h"
#include "Event/EventMfnListener.hpp"
#include "Event/IEventArgs.h"
#include "Logger/ILogData.h"
#include "Util/StringUtil.hpp"
#include "Util/FileUtil.hpp"
#include <fstream>

using namespace lt;

CDebugLogDispatcher::CDebugLogDispatcher(ConstStringT id, LogFilter logFilter)
{
	m_pStrId = new StdStringT(id);
	m_pStrPrefix = new StdStringT(Format(_T("[DEBUG:%s] ##"), id));
	m_pDispatcher = new CLogDispatcher();
	m_pListener = new Listener(this, &CDebugLogDispatcher::OnLogAdded);

	m_pDispatcher->SetLogFilter(logFilter);
	m_pDispatcher->GetLogAddedEventNotifier() += m_pListener;
}

CDebugLogDispatcher::~CDebugLogDispatcher()
{
	m_pDispatcher->GetLogAddedEventNotifier() -= m_pListener;

	delete m_pStrId;
	delete m_pStrPrefix;
	delete m_pDispatcher;
	delete m_pListener;
}

CDebugLogDispatcher::operator ILogDispatcher *()
{
	return m_pDispatcher;
}

StdStringT CDebugLogDispatcher::GetLogFilePath(const SYSTEMTIME & time) const
{
	return Format(_T("log\\%04d\\%02d\\%02d\\%04d%02d%02d%02d_%s_DebugLog.log"),
				  time.wYear, time.wMonth, time.wDay,
				  time.wYear, time.wMonth, time.wDay, time.wHour, m_pStrId->c_str());
}

void CDebugLogDispatcher::PrintLog(const StdStringT & string)
{
//	if(!IsDebuggerPresent())
//		return;

	OutputDebugString(string.c_str());
}

void CDebugLogDispatcher::SaveLog(const StdStringT & strFilePath, const StdStringT & string)
{
	StdOFStreamA ofs(strFilePath, StdOFStreamT::app);

	ofs <<
		#if defined(UNICODE)
		ToMultiByte(string)
		#else
		string
		#endif
		;
	ofs.close();
}

void CDebugLogDispatcher::OnLogAdded(EventArgs & eventArgs)
{
	static constexpr StringT strLogLevels[] =
	{
		_T("None"),
		_T("Info"),
		_T("Proc"),
		_T("Noti"),
		_T("Warn"),
		_T("Erro"),
		_T("Pass"),
		_T("Fail")
	};

	auto & args = eventArgs.GetArgs();
	auto & loggingTime = args.GetLoggingTime();
	auto logLevel = static_cast<int>(args.GetLogLevel());
	auto string = Format(_T("%04d.%02d.%02d %02d:%02d:%02d.%03d %s (%s) %s\n"),
						 loggingTime.wYear, loggingTime.wMonth, loggingTime.wDay,
						 loggingTime.wHour, loggingTime.wMinute, loggingTime.wSecond, loggingTime.wMilliseconds,
						 m_pStrPrefix->c_str(), strLogLevels[logLevel], args.GetLogString());

	PrintLog(string);

	auto strLogFilePath = GetLogFilePath(loggingTime);

	MakeDirectory(strLogFilePath);

	SaveLog(strLogFilePath, string);
}

//2022.12.06a uhkim 
void CDebugLogDispatcher::OnPublicLogAdded(EventArgs & eventArgs)
{
	static constexpr StringT strLogLevels[] =
	{
		_T("None"),
		_T("Info"),
		_T("Proc"),
		_T("Noti"),
		_T("Warn"),
		_T("Erro"),
		_T("Pass"),
		_T("Fail")
	};

	auto & args = eventArgs.GetArgs();
	auto & loggingTime = args.GetLoggingTime();
	auto logLevel = static_cast<int>(args.GetLogLevel());
	auto string = Format(_T("%04d.%02d.%02d %02d:%02d:%02d.%03d %s (%s) %s\n"),
		loggingTime.wYear, loggingTime.wMonth, loggingTime.wDay,
		loggingTime.wHour, loggingTime.wMinute, loggingTime.wSecond, loggingTime.wMilliseconds,
		m_pStrPrefix->c_str(), strLogLevels[logLevel], args.GetLogString());

	PrintLog(string);

	auto strLogFilePath = GetLogFilePath(loggingTime);

	MakeDirectory(strLogFilePath);

	SaveLog(strLogFilePath, string);
}
