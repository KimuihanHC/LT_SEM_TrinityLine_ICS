#pragma once

namespace std
{
	template<class _Elem> struct char_traits;
	template<class _Ty> class allocator;
	template<class _Elem, class _Traits, class _Alloc> class basic_string;
	template<class _Ty, class _Alloc> class deque;
	template<class _Ty, class _Container> class queue;
	class mutex;
	template<class _Mutex> class lock_guard;
}

class CLogger
{
private:
	typedef std::basic_string<char, std::char_traits<char>, std::allocator<char>> StringA;
	typedef std::basic_string<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t>> StringW;
#if defined(UNICODE)
	typedef StringW StringT;
#else
	typedef StringA StringT;
#endif
	typedef std::queue<StringT, std::deque<StringT, std::allocator<StringT>>> LogQueue;
	typedef std::mutex Mutex;
	typedef std::lock_guard<Mutex> LockGuard;

public:
	typedef void (__stdcall *PFN_CALLBACK) (LPCTSTR);

	CLogger();
	CLogger(PFN_CALLBACK pfnCallback);
	~CLogger();

	CLogger(const CLogger &) = delete;
	CLogger & operator=(const CLogger &) = delete;

public:
	static const CLogger & GetDebugLogger();

	void SetCallback(PFN_CALLBACK pfnCallback);

	void OnAdd_Log(LPCTSTR lpszLog, ...) const;

	CLogger & operator=(PFN_CALLBACK pfnCallback);

private:
	void Initialize();
	void Release();

	void LogProc();

private:
	static CLogger debugLogger;

	PFN_CALLBACK m_pfnCallback;
	Mutex * m_pCallbackLocker;
	LogQueue * m_pLogQueue;
	Mutex * m_pLogQueueLocker;
	HANDLE m_hThreadLog;
	bool m_bShutdown;
};
