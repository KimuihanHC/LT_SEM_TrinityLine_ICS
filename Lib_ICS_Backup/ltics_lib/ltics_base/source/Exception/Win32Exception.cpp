#include "pch.h"
#include "Exception/Win32Exception.h"
#include "Util/MemoryUtil.hpp"

using namespace lt;

CWin32Exception::CWin32Exception(ConstStringT strFile,
								 ConstStringT strFunction,
								 uint line,
								 DWORD dwLastError)
: CException(strFile, strFunction, line)
, m_dwLastError(dwLastError)
{
	StringT strError = nullptr;

	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
				  nullptr,
				  m_dwLastError,
				  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				  reinterpret_cast<LPTSTR>(&strError),
				  0,
				  nullptr);

	SetReason(strError);

	LocalFree(strError);
}

CWin32Exception::CWin32Exception(const CWin32Exception & rhs)
: CException(rhs)
, m_dwLastError(rhs.m_dwLastError)
{
}

CWin32Exception::CWin32Exception(CWin32Exception && rhs)
: CException(std::move(rhs))
{
	Swap(m_dwLastError, rhs.m_dwLastError);
}

CWin32Exception & CWin32Exception::operator=(const CWin32Exception & rhs)
{
	__super::operator=(rhs);

	m_dwLastError = rhs.m_dwLastError;

	return *this;
}

CWin32Exception & CWin32Exception::operator=(CWin32Exception && rhs)
{
	__super::operator=(std::move(rhs));

	Swap(m_dwLastError, rhs.m_dwLastError);

	return *this;
}

DWORD CWin32Exception::GetLastError() const
{
	return m_dwLastError;
}
