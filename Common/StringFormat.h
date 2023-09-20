#pragma once

#include <string>
#include <tchar.h>
#include <stdarg.h>

inline std::basic_string<TCHAR> Format(const TCHAR * lpszFormat, ...)
{
	va_list args;

	va_start(args, lpszFormat);

	auto str = Format(lpszFormat, args);

	va_end(args);

	return str;
}

inline std::basic_string<TCHAR> Format(const TCHAR * lpszFormat, va_list args)
{
	auto length = _vsctprintf(lpszFormat, args) + 1;
	auto stringPtr = new TCHAR [length];

	_vstprintf_s(stringPtr, length, lpszFormat, args);

	auto str = std::basic_string<TCHAR>(stringPtr);

	delete [] stringPtr;

	return str;
}

inline std::basic_string<TCHAR> GetSystemErrorMessage(int nError)
{
	LPTSTR lpszError = nullptr;

	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
				  nullptr,
				  nError,
				  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				  reinterpret_cast <LPTSTR> (&lpszError),
				  0,
				  nullptr);

	auto str = std::basic_string<TCHAR>(lpszError);

	LocalFree(lpszError);

	return str;
}
