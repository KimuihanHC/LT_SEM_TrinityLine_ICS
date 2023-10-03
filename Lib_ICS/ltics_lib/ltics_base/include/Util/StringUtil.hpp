#pragma once

#include "TypeDecl.h"
#include <string>
#include <sstream>
#include <varargs.h>
#include <assert.h>
#include <tchar.h>

namespace lt
{
	inline auto FormatV(ConstStringA format, va_list args)
	{
		auto length = _vscprintf(format, args);
		auto size = static_cast<size_t>(length) + 1;
		auto ptr = std::make_unique<char[]>(size);
		auto written = _vsnprintf_s(ptr.get(),
									size,
									_TRUNCATE,
									format,
									args);

		assert(length == written);

		return StdStringA(ptr.get());
	}

	inline auto FormatV(ConstStringW format, va_list args)
	{
		auto length = _vscwprintf(format, args);
		auto size = static_cast<size_t>(length) + 1;
		auto ptr = std::make_unique<wchar_t[]>(size);
		auto written = _vsnwprintf_s(ptr.get(),
									 size,
									 _TRUNCATE,
									 format,
									 args);

		assert(length == written);

		return StdStringW(ptr.get());
	}

#if 1
	template<typename TypeChar>
	inline auto Format(const TypeChar * format, ...)
	{
		va_list args;

		va_start(args, format);

		auto string = FormatV(format, args);

		va_end(args);

		return string;
	}

	template<typename TypeChar>
	inline void AppendFormat(typename StdString<TypeChar>::String & string,
							 const TypeChar * format, ...)
	{
		va_list args;

		va_start(args, format);

		string.append(FormatV(format, args));

		va_end(args);
	}

#else
	template<typename... TypeArgs>
	inline auto Format(ConstStringA format, TypeArgs &&... args)
	{
		#pragma warning(push)
		#pragma warning(disable:4996)
		auto length = _snprintf(nullptr,
								0,
								format,
								std::forward<TypeArgs>(args)...);
		#pragma warning(pop)

		auto size = static_cast<size_t>(length) + 1;
		auto ptr = std::make_unique<char[]>(size);
		auto written = _snprintf_s(ptr.get(),
								   size,
								   _TRUNCATE,
								   format,
								   std::forward<TypeArgs>(args)...);

		assert(length == written);

		return StdStringA(ptr.get());
	}

	template<typename... TypeArgs>
	inline auto Format(ConstStringW format, TypeArgs &&... args)
	{
		#pragma warning(push)
		#pragma warning(disable:4996)
		auto length = _snwprintf(nullptr,
								 0,
								 format,
								 std::forward<TypeArgs>(args)...);
		#pragma warning(pop)

		auto size = static_cast<size_t>(length) + 1;
		auto ptr = std::make_unique<wchar_t[]>(size);
		auto written = _snwprintf_s(ptr.get(),
									size,
									_TRUNCATE,
									format,
									std::forward<TypeArgs>(args)...);

		assert(length == written);

		return StdStringW(ptr.get());
	}

	template<typename TypeChar, typename... TypeArgs>
	inline void AppendFormat(typename StdString<TypeChar>::String & string,
							 const TypeChar * format, TypeArgs &&... args)
	{
		string.append(Format(format, std::forward<TypeArgs>(args)...));
	}
#endif

	template<typename TypeChar, typename TypeValue>
	inline auto ToString(const TypeValue & value)
	{
		StdString<TypeChar>::OSStream oss;

		oss << value;

		return std::move(oss.str());
	}

	template<typename TypeValue, typename TypeString>
	inline auto ToValue(const TypeString & string)
	{
		StdString<typename TypeString::value_type>::ISStream iss(string);
		TypeValue value;

		iss >> value;

		return value;
	}

	template<typename TypeValue, typename TypeChar>
	inline auto ToValue(const TypeChar * string)
	{
		return ToValue(StdString<TypeChar>::String(string));
	}

	inline auto ToWideChar(ConstStringA string)
	{
		auto requiredChars = ::MultiByteToWideChar(
			CP_ACP, 0, string, -1, nullptr, 0);

		assert(requiredChars > 0);

		auto ptr = std::make_unique<wchar_t[]>(requiredChars + 1);
		auto writtenChars = ::MultiByteToWideChar(
			CP_ACP, 0, string, -1, ptr.get(), requiredChars);

		assert(requiredChars == writtenChars);

		return StdStringW(ptr.get());
	}

	inline auto ToWideChar(const StdStringA & string)
	{
		return ToWideChar(string.c_str());
	}

	inline auto ToMultiByte(ConstStringW string)
	{
		auto requiredChars = ::WideCharToMultiByte(
			CP_ACP, 0, string, -1, nullptr, 0, nullptr, nullptr);

		assert(requiredChars > 0);

		auto ptr = std::make_unique<char[]>(requiredChars + 1);
		auto writtenChars = ::WideCharToMultiByte(
			CP_ACP, 0, string, -1, ptr.get(), requiredChars, nullptr, nullptr);

		assert(requiredChars == writtenChars);

		return StdStringA(ptr.get());
	}

	inline auto ToMultiByte(const StdStringW & string)
	{
		return ToMultiByte(string.c_str());
	}

	inline auto ToTypeChar(ConstStringA string)
	{
		#if defined(UNICODE)
		return ToWideChar(string);
		#else
		return StdStringT(string);
		#endif
	}

	inline auto ToTypeChar(ConstStringW string)
	{
		#if defined(UNICODE)
		return StdStringT(string);
		#else
		return ToMultiByte(string);
		#endif
	}

	inline auto ToTypeChar(const StdStringA & string)
	{
		return ToTypeChar(string.c_str());
	}

	inline auto ToTypeChar(const StdStringW & string)
	{
		return ToTypeChar(string.c_str());
	}

	//convert 2023.01.09a 
	/* Test 20231001
	inline auto ConvertCtoCW(ConstStringA csa)
	{
		ConstStringW cst;
		wchar_t		 *wch;
		auto strSize = MultiByteToWideChar(CP_ACP, 0, csa, -1, NULL, NULL);
		wch = new wchar_t[strSize];
		MultiByteToWideChar(CP_ACP, 0, csa, strlen(csa) + 1, wch, strSize);
		//cst = (ConstStringW)(LPWSTR)wch;	
		cst = std::move(wch);
		return cst;
	}
	inline auto ConvertCtoCT(ConstStringA csa)
	{
#if defined(UNICODE)
		return ConvertCtoCW(csa);
#else
		return ConstStringT(csa);
#endif
	}

	inline auto ConvertCWtoC(ConstStringW cst) {
		char		 *wch;
		ConstStringA	csa;
		int strsize = WideCharToMultiByte(CP_ACP, 0, cst, -1, NULL, 0, NULL, NULL);
		wch = new char[strsize];
		WideCharToMultiByte(CP_ACP, 0, cst, -1, wch, strsize, 0, 0);
		//csa = (ConstStringA)(LPWSTR)wch;		
		//return ConstStringA(csa);
		csa = std::move(wch);
		return csa;
	}
	inline auto ConvertCTtoC(ConstStringT cst)
	{
#if defined(UNICODE)
		return ConvertCWtoC((ConstStringW)cst);
#else
		return ConstStringA(csa);
#endif
	}*/



	template<typename TypeChar>
	struct TrimmingCharSet
	{
	};

	template<>
	struct TrimmingCharSet<char>
	{
		static constexpr char * CharSet = " \t\r\n\v";
	};

	template<>
	struct TrimmingCharSet<wchar_t>
	{
		static constexpr wchar_t * CharSet = L" \t\r\n\v";
	};

	template<typename TypeString>
	inline void TrimLeft(TypeString & string,
						 const typename TypeString::value_type * charSet =
						 TrimmingCharSet<typename TypeString::value_type>::CharSet)
	{
		string.erase(0, string.find_first_not_of(charSet));
	}

	template<typename TypeString>
	inline auto TrimLeft(TypeString && string,
						 const typename TypeString::value_type * charSet =
						 TrimmingCharSet<typename TypeString::value_type>::CharSet)
	{
		TrimLeft(string);

		return std::move(string);
	}

	template<typename TypeString>
	inline void TrimRight(TypeString & string,
						  const typename TypeString::value_type * charSet =
						  TrimmingCharSet<typename TypeString::value_type>::CharSet)
	{
		string.erase(string.find_last_not_of(charSet) + 1);
	}

	template<typename TypeString>
	inline auto TrimRight(TypeString && string,
						  const typename TypeString::value_type * charSet =
						  TrimmingCharSet<typename TypeString::value_type>::CharSet)
	{
		TrimRight(string);

		return std::move(string);
	}

	template<typename TypeString>
	inline void Trim(TypeString & string,
					 const typename TypeString::value_type * charSet =
					 TrimmingCharSet<typename TypeString::value_type>::CharSet)
	{
		TrimLeft(string, charSet);
		TrimRight(string, charSet);
	}

	template<typename TypeString>
	inline auto Trim(TypeString && string,
					 const typename TypeString::value_type * charSet =
					 TrimmingCharSet<typename TypeString::value_type>::CharSet)
	{
		Trim(string);

		return std::move(string);
	}
}
