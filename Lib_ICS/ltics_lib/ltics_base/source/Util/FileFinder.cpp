#include "pch.h"
#include "Util/FileFinder.h"
#include "Enumerator/Enumerator.hpp"
#include "Util/SyncUtil.hpp"
#include <tchar.h>
#include <vector>
#include <mutex>

using namespace lt;

CFileFinder::CFileFinder()
{
	m_pCntr = new Cntr();
	m_pCntrMutex = new StdMutex();
	m_pEnumerator = new FoundFilesImpl(this);
}

CFileFinder::~CFileFinder()
{
	delete m_pCntr;
	delete m_pCntrMutex;
	delete m_pEnumerator;
}

CFileFinder::FoundFiles & CFileFinder::operator()(ConstStringT strPath)
{
	SimpleLockPtr(m_pCntrMutex);

	m_pCntr->clear();

	WIN32_FIND_DATA fd = { 0, };
	auto handle = ::FindFirstFile(strPath, &fd);

	if(handle != INVALID_HANDLE_VALUE)
	{
		tchar strDrv[MAX_PATH] = { 0, };
		tchar strDir[MAX_PATH] = { 0, };
		tchar strDirectory[MAX_PATH] = { 0, };
		tchar strFileName[MAX_PATH] = { 0, };

		_tsplitpath_s(strPath, strDrv, MAX_PATH, strDir, MAX_PATH, nullptr, 0, nullptr, 0);
		_stprintf_s(strDirectory, _T("%s%s"), strDrv, strDir);

		do
		{
			if(fd.cFileName[0] == '.')
				continue;

			_tcscpy_s(strFileName, MAX_PATH, fd.cFileName);
			_stprintf_s(fd.cFileName, _T("%s%s"), strDirectory, strFileName);

			m_pCntr->emplace_back(fd);
		}
		while(::FindNextFile(handle, &fd));

		::FindClose(handle);
	}

	return GetFoundFiles();
}

CFileFinder::FoundFiles & CFileFinder::GetFoundFiles() const
{
	return *m_pEnumerator;
}

CFileFinder::Value & CFileFinder::operator[](size_t index)
{
	SimpleLockPtr(m_pCntrMutex);

	auto iter = m_pCntr->begin();

	std::advance(iter, index);

	return *iter;
}

CFileFinder::ConstValue & CFileFinder::operator[](size_t index) const
{
	SimpleLockPtr(m_pCntrMutex);

	auto iter = m_pCntr->begin();

	std::advance(iter, index);

	return *iter;
}

size_t CFileFinder::GetCount() const
{
	SimpleLockPtr(m_pCntrMutex);

	return m_pCntr->size();
}
