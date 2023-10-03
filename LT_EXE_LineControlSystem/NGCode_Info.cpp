//*****************************************************************************
// Filename	: 	NGCode_Info.cpp
// Created	:	2016/3/14 - 10:57
// Modified	:	2016/3/14 - 10:57
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
// NGCode_Info.cpp : implementation file
//

#include "stdafx.h"
#include <io.h>
#include <stdint.h>
#include <vector>
#include "NGCode_Info.h"


//=============================================================================
//
//=============================================================================
CNGCode_Info::CNGCode_Info()
{

}

//=============================================================================
//
//=============================================================================
CNGCode_Info::~CNGCode_Info()
{
	
}

//=============================================================================
// Method		: MakeDirectory
// Access		: private  
// Returns		: void
// Parameter	: __in LPCTSTR szPath
// Qualifier	:
// Last Update	: 2022/6/21 - 17:28
// Desc.		:
//=============================================================================
void CNGCode_Info::MakeDirectory(__in LPCTSTR szPath)
{
	CFileFind	filefind;
	// 해당 폴더가 없으면 생성한다.
	if (!filefind.FindFile(szPath))
	{
		if (_taccess(szPath, 0) != 0)
		{
			BOOL	l_bExtractSucc = TRUE;
			int		iCnt = 0;
			CString l_strDirPath = _T("");
			CString l_strNowPath = _T("");

			while (l_bExtractSucc)
			{
				l_bExtractSucc = AfxExtractSubString(l_strNowPath, szPath, iCnt, '\\');
				l_strDirPath += l_strNowPath + _T("\\");

				if (l_bExtractSucc && _taccess(l_strDirPath, 0) != 0)
				{
					CreateDirectory(l_strDirPath, NULL);
				}
				iCnt++;
			}
		}
	}
}

//=============================================================================
// Method		: Backup_File
// Access		: private  
// Returns		: BOOL
// Parameter	: __in LPCTSTR szPath
// Qualifier	:
// Last Update	: 2022/6/21 - 17:27
// Desc.		:
//=============================================================================
BOOL CNGCode_Info::Backup_File(__in LPCTSTR szPath)
{
	// 기존 파일명 + 현재 날짜/시간
	// C:\\Recipe\\SEM_T01_SocketInfo.xml
	// => C:\\Recipe\\BAK\\SEM_T01_SocketInfo_2021_1117_181900.xml

	if (!PathFileExists(szPath))
	{
		return FALSE;
	}

	TCHAR drive[_MAX_DRIVE];
	TCHAR dir[_MAX_DIR];
	TCHAR fname[_MAX_FNAME];
	TCHAR ext[_MAX_EXT];

	_tsplitpath_s(szPath, drive, _MAX_DRIVE, dir, _MAX_DIR, fname, _MAX_FNAME, ext, _MAX_EXT);

	SYSTEMTIME lcTm;
	GetLocalTime(&lcTm);

	CString szTime;
	szTime.Format(_T("%04d%02d%02d_%02d%02d%02d"), lcTm.wYear, lcTm.wMonth, lcTm.wDay,
		lcTm.wHour, lcTm.wMinute, lcTm.wSecond);

	CString szBackFilePath;
	szBackFilePath.Format(_T("%s%sBAK\\"), drive, dir);

	MakeDirectory(szBackFilePath.GetBuffer());

	szBackFilePath.Format(_T("%s%sBAK\\%s_%s%s"), drive, dir, fname, szTime.GetBuffer(), ext);

	return ::CopyFile(szPath, szBackFilePath.GetBuffer(), FALSE);
}

//=============================================================================
// Method		: Save_IniFile_MES
// Access		: public  
// Returns		: bool
// Parameter	: __in LPCTSTR szPath
// Parameter	: __in int16_t IN_nMES_ReworkCode
// Qualifier	:
// Last Update	: 2022/6/21 - 17:27
// Desc.		:
//=============================================================================
bool CNGCode_Info::Save_IniFile_MES(__in LPCTSTR szPath, __in int16_t IN_nMES_ReworkCode)
{
	CString szValue;
	CString szKeyName;

	// MES rework NG Code
	szValue.Format(_T("%d"), IN_nMES_ReworkCode);
	WritePrivateProfileString(_T("MES"), _T("NG_Code"), szValue, szPath);

	return true;
}

bool CNGCode_Info::Load_IniFile_MES(__in LPCTSTR szPath, __out int16_t& OUT_nMES_ReworkCode)
{
	TCHAR   inBuff[1025] = { 0, };
	CString szValue;
	CString szKeyName;

	// MES rework NG Code
	GetPrivateProfileString(_T("MES"), _T("NG_Code"), _T("0"), inBuff, 80, szPath);
	OUT_nMES_ReworkCode = _ttoi(inBuff);

	return true;
}

bool CNGCode_Info::Save_IniFile(__in LPCTSTR szPath, __in const std::map<int16_t, ST_NGCode>* IN_pNGCode_Info)
{
	// 기존 파일을 백업하고 저장한다.
	Backup_File(szPath);
	::DeleteFile(szPath);

	CString szValue;
	CString szAppName;
	CString szKeyName;

	// NG Code 목록
	szAppName = _T("NG_Code_List");
	szValue.Format(_T("%d"), m_NG_Info.size());
	WritePrivateProfileString(szAppName, _T("Count"), szValue, szPath);

	auto nIndex = 0;
	for (auto Iter = m_NG_Info.begin(); Iter != m_NG_Info.end(); Iter++)
	{
		szKeyName.Format(_T("%04d"), nIndex++);
		szValue.Format(_T("%d"), Iter->first);
		WritePrivateProfileString(szAppName, szKeyName, szValue, szPath);
	}

	// NG Code 데이터
	for (auto Iter = m_NG_Info.begin(); Iter != m_NG_Info.end(); Iter++)
	{
		szAppName.Format(_T("%d"), Iter->first);

		// int16_t		m_nNG_Code;			// Code (NGCode	)
		szValue.Format(_T("%d"), Iter->second.m_nNG_Code);
		WritePrivateProfileString(szAppName, _T("NG_Code"), szValue, szPath);

		// uint16_t		m_nNG_Section;		// Section (NG구분)	
		szValue.Format(_T("%d"), Iter->second.m_nNG_Section);
		WritePrivateProfileString(szAppName, _T("NG_Section"), szValue, szPath);

		// uint16_t		m_nRetry_Mode;		// Retry Mode	
		szValue.Format(_T("%d"), Iter->second.m_nRetry_Mode);
		WritePrivateProfileString(szAppName, _T("Retry_Mode"), szValue, szPath);

		// uint16_t		m_nRetryCnt;		// Retry Count	
		szValue.Format(_T("%d"), Iter->second.m_nRetryCnt);
		WritePrivateProfileString(szAppName, _T("RetryCnt"), szValue, szPath);

		// uint16_t		m_nMove_Retry;		// Move Retry	=> (0: 미사용, 1: 사용)
		szValue.Format(_T("%d"), Iter->second.m_nMove_Retry);
		WritePrivateProfileString(szAppName, _T("Move_Retry"), szValue, szPath);

		// uint16_t		m_nReset;			// Reset		=> (0: 미사용, 1: 사용)
		szValue.Format(_T("%d"), Iter->second.m_nReset);
		WritePrivateProfileString(szAppName, _T("Reset"), szValue, szPath);

		// uint16_t		m_nAlarmCnt;		// Alarm Count(연속NG알람수)
		szValue.Format(_T("%d"), Iter->second.m_nAlarmCnt);
		WritePrivateProfileString(szAppName, _T("AlarmCnt"), szValue, szPath);

		// CString		m_szDescription;	// Descript
		szValue = Iter->second.m_szDescription;
		WritePrivateProfileString(szAppName, _T("Description"), szValue, szPath);
	}

	return true;
}

bool CNGCode_Info::Save_IniFile(LPCTSTR szPath)
{
	bool bReturn = Save_IniFile(szPath, &m_NG_Info);
	bReturn &= Save_IniFile_MES(szPath, m_nMES_ReworkNGCode);

	return bReturn;
}

bool CNGCode_Info::Load_IniFile(__in LPCTSTR szPath, __out std::map<int16_t, ST_NGCode>& OUT_NGCode_Info)
{
	// 파일이 존재하는가?
	if (!PathFileExists(szPath))
	{
		return false;
	}

	TCHAR   inBuff[1025] = { 0, };
	CString szValue;
	CString szAppName;
	CString szKeyName;

	OUT_NGCode_Info.clear();

	// NG Code 목록
	szAppName = _T("NG_Code_List");
	GetPrivateProfileString(szAppName, _T("Count"), _T("0"), inBuff, 80, szPath);
	long lCount = _ttoi(inBuff);

	std::vector<int16_t> nNGCodeList;
	for (auto nIndex = 0; nIndex < lCount; ++nIndex)
	{
		szKeyName.Format(_T("%04d"), nIndex);
		if (0 < GetPrivateProfileString(szAppName, szKeyName, _T("0"), inBuff, 80, szPath))
		{
			int16_t nNGCode = _ttoi(inBuff);
			nNGCodeList.push_back(nNGCode);
		}
	}

	for (auto nIdx = 0; nIdx < nNGCodeList.size(); ++nIdx)
	{
		szAppName.Format(_T("%d"), nNGCodeList.at(nIdx));

		ST_NGCode NGCode_New;

		// int16_t		m_nNG_Code;			// Code (NGCode	)
		if (0 < GetPrivateProfileString(szAppName, _T("NG_Code"), _T("0"), inBuff, 80, szPath))
		{
			NGCode_New.m_nNG_Code = _ttoi(inBuff);

			if (nNGCodeList.at(nIdx) != NGCode_New.m_nNG_Code)
			{
				continue;
			}
		}
		else
		{
			continue;
		}

		// uint16_t		m_nNG_Section;		// Section (NG구분)	
		GetPrivateProfileString(szAppName, _T("NG_Section"), _T("0"), inBuff, 80, szPath);
		NGCode_New.m_nNG_Section = _ttoi(inBuff);

		// uint16_t		m_nRetry_Mode;		// Retry Mode	
		GetPrivateProfileString(szAppName, _T("Retry_Mode"), _T("0"), inBuff, 80, szPath);
		NGCode_New.m_nRetry_Mode = _ttoi(inBuff);

		// uint16_t		m_nRetryCnt;		// Retry Count	
		GetPrivateProfileString(szAppName, _T("RetryCnt"), _T("0"), inBuff, 80, szPath);
		NGCode_New.m_nRetryCnt = _ttoi(inBuff);

		// uint16_t		m_nMove_Retry;		// Move Retry	=> (0: 미사용, 1: 사용)
		GetPrivateProfileString(szAppName, _T("Move_Retry"), _T("0"), inBuff, 80, szPath);
		NGCode_New.m_nMove_Retry = _ttoi(inBuff);

		// uint16_t		m_nReset;			// Reset		=> (0: 미사용, 1: 사용)
		GetPrivateProfileString(szAppName, _T("Reset"), _T("1"), inBuff, 80, szPath);
		NGCode_New.m_nReset = _ttoi(inBuff);

		// uint16_t		m_nAlarmCnt;		// Alarm Count(연속NG알람수)
		GetPrivateProfileString(szAppName, _T("AlarmCnt"), _T("0"), inBuff, 80, szPath);
		NGCode_New.m_nAlarmCnt = _ttoi(inBuff);

		// CString		m_szDescription;	// Descript
		GetPrivateProfileString(szAppName, _T("Description"), _T(""), inBuff, 1024, szPath);
		NGCode_New.m_szDescription = inBuff;

		// 목록에 추가
		OUT_NGCode_Info.insert({ nNGCodeList.at(nIdx), NGCode_New });
	}

	return true;
}

bool CNGCode_Info::Load_IniFile(LPCTSTR szPath)
{
	bool bReturn = Load_IniFile(szPath, m_NG_Info);
	bReturn &= Load_IniFile_MES(szPath, m_nMES_ReworkNGCode);
	
	return bReturn;
}

bool CNGCode_Info::Insert(ST_NGCode * IN_pNGCode)
{
	if (nullptr == IN_pNGCode)
		return false;

	auto Re = m_NG_Info.insert({ IN_pNGCode->m_nNG_Code, *IN_pNGCode });
	if (Re.second)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool CNGCode_Info::Remove(int16_t IN_nNG_Code)
{
	auto result = m_NG_Info.find(IN_nNG_Code);
	if (result != m_NG_Info.end())
	{
		m_NG_Info.erase(IN_nNG_Code);

		return true;
	}

	return false;
}

bool CNGCode_Info::Modify(ST_NGCode * IN_pNGCode)
{
	if (nullptr == IN_pNGCode)
		return false;

	auto result  = m_NG_Info.find(IN_pNGCode->m_nNG_Code);
	if (result != m_NG_Info.end())
	{
		m_NG_Info.at(IN_pNGCode->m_nNG_Code) = *IN_pNGCode;

		return true;
	}

	return false;
}

void CNGCode_Info::Clear()
{
	m_NG_Info.clear();
}

bool CNGCode_Info::IsExist(int16_t IN_nNG_Code)
{
	auto result = m_NG_Info.find(IN_nNG_Code);
	if (result != m_NG_Info.end())
	{
		return true;
	}
	else
	{
		return false;
	}
}

ST_NGCode & CNGCode_Info::Get_NGCode(int16_t IN_nNG_Code)
{
	return m_NG_Info.at(IN_nNG_Code);
}

const ST_NGCode & CNGCode_Info::Get_NGCode(int16_t IN_nNG_Code) const
{
	return m_NG_Info.at(IN_nNG_Code);
}

