#pragma once

#include "Util/StringUtil.hpp"
#include <winver.h>

#pragma comment(lib, "version.lib")

namespace lt
{
	inline auto GetFixedFileInfo(ConstStringT strFilePath)
	{
		VS_FIXEDFILEINFO fixedFileInfo = {};
		auto infoSize = ::GetFileVersionInfoSize(strFilePath, 0);

		if(infoSize > 0)
		{
			auto infoPtr = new char [infoSize];

			if(::GetFileVersionInfo(strFilePath, 0, infoSize, infoPtr))
			{
				VS_FIXEDFILEINFO * pFixedFileInfo = nullptr;
				UINT nLength = 0;

				if(::VerQueryValue(infoPtr,
								   _T("\\"),
								   reinterpret_cast<LPVOID *>(&pFixedFileInfo), &nLength))
					memcpy_s(&fixedFileInfo, sizeof(fixedFileInfo), pFixedFileInfo, nLength);
			}

			delete [] infoPtr;
		}

		return fixedFileInfo;
	}

	inline auto MakeDirectory(const StdStringT & strPath)
	{
		auto length = strPath.length();

		for(decltype(length) index = 0; index < length;)
		{
			auto index_found = strPath.find_first_of(_T("\\/"), index);

			if(index_found == StdStringT::npos)
			{
				if(strPath.find('.', index) != StdStringT::npos)
					break;

				index_found = length;
			}

			if(index_found > 0)
			{
				auto strSubPath = strPath.substr(0, index_found);

				if(strSubPath.back() != ':')
				{
					if(_tmkdir(strSubPath.c_str()) != 0)
					{
						if(errno != EEXIST)
							return false;
					}
				}
			}

			index = index_found + 1;
		}

		return true;
	}

	inline auto MakeDirectory(ConstStringT strPath)
	{
		return MakeDirectory(StdStringT(strPath));
	}
}
