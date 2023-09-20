#pragma once

#include "Exception/Exception.h"

namespace lt
{
	class LTICS_LIB_CLASS CWin32Exception final : public CException
	{
	public:
		CWin32Exception(ConstStringT strFile,
						ConstStringT strFunction,
						uint line,
						DWORD dwLastError);
		CWin32Exception(const CWin32Exception & rhs);
		CWin32Exception(CWin32Exception && rhs);
		~CWin32Exception() = default;

		CWin32Exception & operator=(const CWin32Exception & rhs);
		CWin32Exception & operator=(CWin32Exception && rhs);

		DWORD GetLastError() const;

	private:
		DWORD m_dwLastError = 0;
	};
}

#define ThrowWin32Exception(lastError) \
	throw lt::CWin32Exception(__FILET__, \
							  __FUNCTIONT__, \
							  __LINE__, \
							  (lastError))
