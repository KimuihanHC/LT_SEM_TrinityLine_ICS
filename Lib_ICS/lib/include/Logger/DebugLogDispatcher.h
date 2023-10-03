#pragma once

#include "Logger/IDebugLogDispatcher.h"
#include "Logger/LogFilter.h"

namespace lt
{
	template<typename TypeOwner, typename TypeEventArgs>
	class CEventMfnListener;
	template<typename TypeSender, typename TypeArgs>
	class IEventArgs;
	class ILogData;
	class CLogDispatcher;

	class LTICS_LIB_CLASS CDebugLogDispatcher final : public IDebugLogDispatcher
	{
	private:
		using EventArgs = IEventArgs<ILogDispatcher, ILogData>;
		using Listener = CEventMfnListener<CDebugLogDispatcher, EventArgs>;

	public:
		CDebugLogDispatcher(ConstStringT id, LogFilter logFilter);
		CDebugLogDispatcher(const CDebugLogDispatcher &) = delete;
		~CDebugLogDispatcher();

		CDebugLogDispatcher & operator=(const CDebugLogDispatcher &) = delete;
		//2022.12.06a uhkim
		void OnPublicLogAdded(EventArgs & eventArgs);
		//
		operator ILogDispatcher *() override;

	private:
		StdStringT GetLogFilePath(const SYSTEMTIME & time) const;

		void PrintLog(const StdStringT & string);
		void SaveLog(const StdStringT & strFilePath, const StdStringT & string);

		void OnLogAdded(EventArgs & eventArgs);

	private:
		StdStringT * m_pStrId = nullptr;
		StdStringT * m_pStrPrefix = nullptr;
		CLogDispatcher * m_pDispatcher = nullptr;
		Listener * m_pListener = nullptr;
	};
}
