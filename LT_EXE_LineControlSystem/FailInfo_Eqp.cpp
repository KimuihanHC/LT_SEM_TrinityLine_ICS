#include "stdafx.h"
#include "FailInfo_Eqp.h"

#define CSV_HEADER_FAILINFO _T("NG Code,Count All,Count Left,Count Right,Count Center\r\n")

//=============================================================================
// Method		: WM_Notify_NGCode
// Access		: protected  
// Returns		: void
// Parameter	: int16_t IN_nNGCode
// Qualifier	:
// Last Update	: 2022/4/20 - 16:35
// Desc.		:
//=============================================================================
inline void CFailInfo_Eqp::WM_Notify_NGCode(int16_t IN_nNGCode)
{
	if (m_hWndGUI)
	{
		TRACE(_T("WM_Notify_NGCode() => WM: eqp (%d), ng code (%d) \n"), m_nEqpOrder, IN_nNGCode);
		::SendNotifyMessage(m_hWndGUI, m_nWM_Notify, (WPARAM)m_nEqpOrder, (LPARAM)IN_nNGCode);
	}
}

//=============================================================================
// Method		: WM_Notify_NGCode_All
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2022/4/20 - 16:35
// Desc.		:
//=============================================================================
inline void CFailInfo_Eqp::WM_Notify_NGCode_All()
{
	if (m_hWndGUI)
	{
		TRACE(_T("WM_Notify_NGCode() => WM: eqp (%d), ng code All \n"), m_nEqpOrder);
		::SendNotifyMessage(m_hWndGUI, m_nWM_Notify, (WPARAM)m_nEqpOrder, (LPARAM)0);
	}
}

//=============================================================================
// Method		: Save_IniFile_NGCode
// Access		: protected  
// Returns		: bool
// Parameter	: LPCTSTR IN_szPathFile
// Parameter	: int16_t IN_nNGCode
// Parameter	: __in bool IN_bNewData
// Qualifier	:
// Last Update	: 2022/4/20 - 16:35
// Desc.		:
//=============================================================================
bool CFailInfo_Eqp::Save_IniFile_NGCode(LPCTSTR IN_szPathFile, int16_t IN_nNGCode, __in bool IN_bNewData /*= false*/)
{
	CString szValue;
	CString szAppName;
	CString szKeyName;

	if (IN_bNewData)
	{// NG Code 목록
		szAppName = _T("NG_Code_List");
		szValue.Format(_T("%d"), m_NG_Count.size());
		WritePrivateProfileString(szAppName, _T("Count"), szValue, IN_szPathFile);

		auto nIndex = 0;
		for (auto Iter = m_NG_Count.begin(); Iter != m_NG_Count.end(); Iter++)
		{
			szKeyName.Format(_T("%04d"), nIndex++);
			szValue.Format(_T("%d"), Iter->first);
			WritePrivateProfileString(szAppName, szKeyName, szValue, IN_szPathFile);
		}
	}

	// NG Code 데이터
	auto result_sock = m_NG_Count.find(IN_nNGCode);
	if (result_sock != m_NG_Count.end())
	{
		szAppName.Format(_T("%d"), IN_nNGCode);

		// int16_t		m_nNGCode		= 0;
		szValue.Format(_T("%d"), m_NG_Count.at(IN_nNGCode).m_nNGCode);
		WritePrivateProfileString(szAppName, _T("NG_Code"), szValue, IN_szPathFile);

		// uint32_t	m_nCount_All = 0;
		szValue.Format(_T("%d"), m_NG_Count.at(IN_nNGCode).m_nCount_All);
		WritePrivateProfileString(szAppName, _T("Count_All"), szValue, IN_szPathFile);

		// uint32_t	m_nCount_Para[Para_MaxCount] = { 0, };
		for (auto nIdx = 0; nIdx < Para_MaxCount; ++nIdx)
		{
			szKeyName.Format(_T("Para_%d"), nIdx);

			szValue.Format(_T("%d"), m_NG_Count.at(IN_nNGCode).m_nCount_Para[nIdx]);
			WritePrivateProfileString(szAppName, szKeyName, szValue, IN_szPathFile);
		}
	}
	   	  
	return true;
}

//=============================================================================
// Method		: Write_File
// Access		: protected  
// Returns		: bool
// Parameter	: __in LPCTSTR IN_szPathFile
// Parameter	: __in LPCTSTR IN_szData
// Parameter	: __in LPCTSTR IN_szHeader
// Qualifier	:
// Last Update	: 2022/4/20 - 16:35
// Desc.		:
//=============================================================================
bool CFailInfo_Eqp::Write_File(__in LPCTSTR IN_szPathFile, __in LPCTSTR IN_szData, __in LPCTSTR IN_szHeader)
{
	CString szFullPath = IN_szPathFile;
	CString szUnicode = IN_szData;
	CStringA szANSI;

	CFile File;
	CFileException e;

	if (!PathFileExists(szFullPath.GetBuffer()))
	{
		if (!File.Open(szFullPath.GetBuffer(), CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite, &e))
			return false;

		// 헤더 추가
		// equpment id, ng code, all, left, right)
		CString szHeader = IN_szHeader;
		szUnicode = szHeader + szUnicode;
	}
	else
	{
		if (!File.Open(szFullPath.GetBuffer(), CFile::modeWrite | CFile::shareDenyWrite, &e))
			return false;
	}

	USES_CONVERSION;
	szANSI = CT2A(szUnicode.GetBuffer());

	File.SeekToEnd();
	File.Write(szANSI.GetBuffer(0), szANSI.GetLength());
	File.Flush();
	File.Close();

	return true;
}

//=============================================================================
// Method		: Set_GUI_HWND
// Access		: public  
// Returns		: void
// Parameter	: HWND IN_hWnd
// Qualifier	:
// Last Update	: 2022/4/20 - 16:35
// Desc.		:
//=============================================================================
void CFailInfo_Eqp::Set_GUI_HWND(HWND IN_hWnd)
{
	m_hWndGUI = IN_hWnd;
}

//=============================================================================
// Method		: Set_WinMsg_Notify
// Access		: public  
// Returns		: void
// Parameter	: UINT IN_nWM_Notify
// Qualifier	:
// Last Update	: 2022/4/20 - 16:35
// Desc.		:
//=============================================================================
void CFailInfo_Eqp::Set_WinMsg_Notify(UINT IN_nWM_Notify)
{
	m_nWM_Notify = IN_nWM_Notify;
}

//=============================================================================
// Method		: Set_EquipmentID
// Access		: public  
// Returns		: void
// Parameter	: __in uint8_t IN_nEqpOrder
// Parameter	: __in LPCTSTR IN_szEquipmentID
// Qualifier	:
// Last Update	: 2022/4/20 - 16:35
// Desc.		:
//=============================================================================
void CFailInfo_Eqp::Set_EquipmentID(__in uint8_t IN_nEqpOrder, __in LPCTSTR IN_szEquipmentID)
{
	m_nEqpOrder		= IN_nEqpOrder;
	m_szEquipmentID = IN_szEquipmentID;

	if (false == m_szPath.IsEmpty())
	{
		m_szPath.Format(_T("%s\\%s.ini"), m_szPath, m_szEquipmentID);
	}
}

//=============================================================================
// Method		: Get_EquipmentID
// Access		: public  
// Returns		: CString &
// Qualifier	:
// Last Update	: 2022/4/20 - 16:35
// Desc.		:
//=============================================================================
CString & CFailInfo_Eqp::Get_EquipmentID()
{
	return m_szEquipmentID;
}

//=============================================================================
// Method		: Get_EquipmentID
// Access		: public  
// Returns		: const CString &
// Qualifier	: const
// Last Update	: 2022/4/20 - 16:36
// Desc.		:
//=============================================================================
const CString & CFailInfo_Eqp::Get_EquipmentID() const
{
	return m_szEquipmentID;
}

void CFailInfo_Eqp::Set_TestPortCount(uint8_t IN_nTestPortCount)
{
	m_nTestPortCount = IN_nTestPortCount;
}

uint8_t CFailInfo_Eqp::Get_TestPortCount() const
{
	return m_nTestPortCount;
}

//=============================================================================
// Method		: Get_EqpOrder
// Access		: public  
// Returns		: uint8_t
// Qualifier	: const
// Last Update	: 2022/4/20 - 16:36
// Desc.		:
//=============================================================================
uint8_t CFailInfo_Eqp::Get_EqpOrder() const
{
	return m_nEqpOrder;
}

//=============================================================================
// Method		: Get_NGCount
// Access		: public  
// Returns		: ST_NGCode_Count &
// Parameter	: __in int16_t IN_nNGCode
// Qualifier	:
// Last Update	: 2022/4/20 - 16:36
// Desc.		:
//=============================================================================
ST_NGCode_Count & CFailInfo_Eqp::Get_NGCount(__in int16_t IN_nNGCode)
{
	return m_NG_Count.at(IN_nNGCode);
}

//=============================================================================
// Method		: Get_NGCount
// Access		: public  
// Returns		: const ST_NGCode_Count &
// Parameter	: __in int16_t IN_nNGCode
// Qualifier	: const
// Last Update	: 2022/4/20 - 16:36
// Desc.		:
//=============================================================================
const ST_NGCode_Count & CFailInfo_Eqp::Get_NGCount(__in int16_t IN_nNGCode) const
{
	return m_NG_Count.at(IN_nNGCode);
}

//=============================================================================
// Method		: IsExist
// Access		: public  
// Returns		: bool
// Parameter	: int16_t IN_nNGCode
// Qualifier	: const
// Last Update	: 2022/4/20 - 16:36
// Desc.		:
//=============================================================================
bool CFailInfo_Eqp::IsExist(int16_t IN_nNGCode) const
{
	auto result = m_NG_Count.find(IN_nNGCode);
	if (result != m_NG_Count.end())
	{
		return true;
	}
	else
	{
		return false;
	}
}

//=============================================================================
// Method		: Set_Path
// Access		: public  
// Returns		: void
// Parameter	: LPCTSTR IN_szPath
// Qualifier	:
// Last Update	: 2022/4/20 - 16:36
// Desc.		:
//=============================================================================
void CFailInfo_Eqp::Set_Path(LPCTSTR IN_szPath)
{
	// ex) C:\\Log\\FailInfo\\eqp_id.ini
	m_szPath = IN_szPath;

	if (false == m_szEquipmentID.IsEmpty())
	{
		m_szPath.Format(_T("%s\\%s.ini"), m_szPath, m_szEquipmentID);
	}
}

//=============================================================================
// Method		: Clear_NGCode
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2022/4/20 - 16:36
// Desc.		:
//=============================================================================
void CFailInfo_Eqp::Clear_NGCode()
{
	m_NG_Count.clear();
}

//=============================================================================
// Method		: Reset_Count_All
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2022/4/20 - 16:36
// Desc.		:
//=============================================================================
void CFailInfo_Eqp::Reset_Count_All()
{
	for (auto Iter = m_NG_Count.begin(); Iter != m_NG_Count.end(); Iter++)
	{
		Iter->second.Reset();
	}

	Remove_ZeroCount_Item();

	Save_IniFile(m_szPath);

	WM_Notify_NGCode_All();
}

//=============================================================================
// Method		: Reset_Count
// Access		: public  
// Returns		: void
// Parameter	: int16_t IN_nNGCode
// Qualifier	:
// Last Update	: 2022/4/20 - 16:36
// Desc.		:
//=============================================================================
void CFailInfo_Eqp::Reset_Count(int16_t IN_nNGCode)
{
	auto result_sock = m_NG_Count.find(IN_nNGCode);
	if (result_sock != m_NG_Count.end())
	{
		m_NG_Count.at(IN_nNGCode).Reset();
	}

	Save_IniFile_NGCode(m_szPath, IN_nNGCode);

	WM_Notify_NGCode(IN_nNGCode);
}

//=============================================================================
// Method		: Insert
// Access		: public  
// Returns		: bool
// Parameter	: int16_t IN_nNGCode
// Qualifier	:
// Last Update	: 2022/4/20 - 16:36
// Desc.		:
//=============================================================================
bool CFailInfo_Eqp::Insert(int16_t IN_nNGCode)
{
	ST_NGCode_Count newCount;
	newCount.m_nNGCode = IN_nNGCode;

	auto Ret = m_NG_Count.insert({ IN_nNGCode, newCount });

	return Ret.second;
}

//=============================================================================
// Method		: IncreaseCount
// Access		: public  
// Returns		: void
// Parameter	: int16_t IN_nNGCode
// Parameter	: uint8_t IN_nNG_Para
// Qualifier	:
// Last Update	: 2022/4/20 - 16:36
// Desc.		:
//=============================================================================
void CFailInfo_Eqp::IncreaseCount(int16_t IN_nNGCode, uint8_t IN_nNG_Para)
{
	auto result_sock = m_NG_Count.find(IN_nNGCode);
	if (result_sock != m_NG_Count.end())
	{
		m_NG_Count.at(IN_nNGCode).IncreaseCount(IN_nNG_Para);

		Save_IniFile_NGCode(m_szPath, IN_nNGCode);
	}
	else
	{
		if (Insert(IN_nNGCode))
		{
			m_NG_Count.at(IN_nNGCode).IncreaseCount(IN_nNG_Para);
		}

		Save_IniFile_NGCode(m_szPath, IN_nNGCode, true);
	}

	WM_Notify_NGCode(IN_nNGCode);
}

//=============================================================================
// Method		: Save_IniFile
// Access		: public  
// Returns		: bool
// Parameter	: LPCTSTR IN_szPathFile
// Qualifier	:
// Last Update	: 2022/4/20 - 16:36
// Desc.		:
//=============================================================================
bool CFailInfo_Eqp::Save_IniFile(LPCTSTR IN_szPathFile)
{
	CString szValue;
	CString szAppName;
	CString szKeyName;

	// NG Code 목록
	szAppName = _T("NG_Code_List");
	szValue.Format(_T("%d"), m_NG_Count.size());
	WritePrivateProfileString(szAppName, _T("Count"), szValue, IN_szPathFile);

	auto nIndex = 0;
	for (auto Iter = m_NG_Count.begin(); Iter != m_NG_Count.end(); Iter++)
	{
		szKeyName.Format(_T("%04d"), nIndex++);
		szValue.Format(_T("%d"), Iter->first);
		WritePrivateProfileString(szAppName, szKeyName, szValue, IN_szPathFile);
	}

	// NG Code 데이터
	for (auto Iter = m_NG_Count.begin(); Iter != m_NG_Count.end(); Iter++)
	{
		szAppName.Format(_T("%d"), Iter->first);

		// int16_t		m_nNGCode		= 0;
		szValue.Format(_T("%d"), Iter->second.m_nNGCode);
		WritePrivateProfileString(szAppName, _T("NG_Code"), szValue, IN_szPathFile);

		// uint32_t	m_nCount_All = 0;
		szValue.Format(_T("%d"), Iter->second.m_nCount_All);
		WritePrivateProfileString(szAppName, _T("Count_All"), szValue, IN_szPathFile);

		// uint32_t	m_nCount_Para[Para_MaxCount] = { 0, };
		for (auto nIdx = 0; nIdx < Para_MaxCount; ++nIdx)
		{
			szKeyName.Format(_T("Para_%d"), nIdx);

			szValue.Format(_T("%d"), Iter->second.m_nCount_Para[nIdx]);
			WritePrivateProfileString(szAppName, szKeyName, szValue, IN_szPathFile);
		}
	}

	return true;
}

//=============================================================================
// Method		: Save_IniFile
// Access		: public  
// Returns		: bool
// Qualifier	:
// Last Update	: 2022/4/20 - 16:36
// Desc.		:
//=============================================================================
bool CFailInfo_Eqp::Save_IniFile()
{
	return Save_IniFile(m_szPath);
}

//=============================================================================
// Method		: Load_IniFile
// Access		: public  
// Returns		: bool
// Parameter	: LPCTSTR IN_szPathFile
// Qualifier	:
// Last Update	: 2022/4/20 - 16:36
// Desc.		:
//=============================================================================
bool CFailInfo_Eqp::Load_IniFile(LPCTSTR IN_szPathFile)
{
	// 파일이 존재하는가?
	if (!PathFileExists(IN_szPathFile))
	{
		return false;
	}

	TCHAR   inBuff[1025] = { 0, };
	CString szValue;
	CString szAppName;
	CString szKeyName;

	m_NG_Count.clear();

	// NG Code 목록
	szAppName = _T("NG_Code_List");
	GetPrivateProfileString(szAppName, _T("Count"), _T("0"), inBuff, 80, IN_szPathFile);
	long lCount = _ttoi(inBuff);

	std::vector<int16_t> nNGCodeList;
	for (auto nIndex = 0; nIndex < lCount; ++nIndex)
	{
		szKeyName.Format(_T("%04d"), nIndex);
		if (0 < GetPrivateProfileString(szAppName, szKeyName, _T("0"), inBuff, 80, IN_szPathFile))
		{
			int16_t nNGCode = _ttoi(inBuff);
			nNGCodeList.push_back(nNGCode);
		}
	}

	for (auto nIdx = 0; nIdx < nNGCodeList.size(); ++nIdx)
	{
		szAppName.Format(_T("%d"), nNGCodeList.at(nIdx));

		ST_NGCode_Count NGCode_New;

		// int16_t		m_nNGCode;			// Code (NGCode	)
		if (0 < GetPrivateProfileString(szAppName, _T("NG_Code"), _T("0"), inBuff, 80, IN_szPathFile))
		{
			NGCode_New.m_nNGCode = _ttoi(inBuff);

			if (nNGCodeList.at(nIdx) != NGCode_New.m_nNGCode)
			{
				continue;
			}
		}
		else
		{
			continue;
		}

		// uint32_t	m_nCount_All = 0;
		GetPrivateProfileString(szAppName, _T("Count_All"), _T("0"), inBuff, 80, IN_szPathFile);
		NGCode_New.m_nCount_All = _ttol(inBuff);

		// uint32_t	m_nCount_Para[Para_MaxCount] = { 0, };
		for (auto nIdx = 0; nIdx < Para_MaxCount; ++nIdx)
		{
			szKeyName.Format(_T("Para_%d"), nIdx);

			GetPrivateProfileString(szAppName, szKeyName, _T("0"), inBuff, 80, IN_szPathFile);
			NGCode_New.m_nCount_Para[nIdx] = _ttol(inBuff);
		}

		// 목록에 추가
		m_NG_Count.insert({ nNGCodeList.at(nIdx), NGCode_New });
	}

	return true;
}

//=============================================================================
// Method		: Load_IniFile
// Access		: public  
// Returns		: bool
// Qualifier	:
// Last Update	: 2022/4/20 - 16:36
// Desc.		:
//=============================================================================
bool CFailInfo_Eqp::Load_IniFile()
{
	return Load_IniFile(m_szPath);
}

//=============================================================================
// Method		: Write_CSV_File
// Access		: public  
// Returns		: bool
// Parameter	: LPCTSTR IN_szPathFile
// Qualifier	:
// Last Update	: 2022/4/20 - 16:36
// Desc.		:
//=============================================================================
bool CFailInfo_Eqp::Write_CSV_File(LPCTSTR IN_szPathFile)
{
	if (m_NG_Count.size() == 0)
	{
		TRACE(_T("[%s] NG Count is Empty : %s\n"), _T(__FUNCTION__), m_szEquipmentID);
		return true;
	}

	CString szData;
	CString szLine;
	// 데이터 스트링 만들기
	// ng code, all count, left count, right count \r\n
	for (auto Iter = m_NG_Count.begin(); Iter != m_NG_Count.end(); Iter++)
	{
		//Iter->first;
		szLine.Format(_T("%d,%d,%d,%d,%d\r\n"),	Iter->second.m_nNGCode,
												Iter->second.m_nCount_All,
												Iter->second.m_nCount_Para[Para_Left],
												Iter->second.m_nCount_Para[Para_Right],
												Iter->second.m_nCount_Para[Para_Center]);

		/*szLine.Format(_T("%d,%d,%d,%d\r\n"),	Iter->second.m_nNGCode,
												Iter->second.m_nCount_All,
												Iter->second.m_nCount_Para[Para_Left],
												Iter->second.m_nCount_Para[Para_Right]);*/

		szData += szLine;
	}

	// 파일로 저장 (ex). report\\failinfo\\eqpid_2022_0325.csv)
	return Write_File(IN_szPathFile, szData, CSV_HEADER_FAILINFO);
}

//=============================================================================
// Method		: Remove_ZeroCount_Item
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2022/4/20 - 16:36
// Desc.		:
//=============================================================================
void CFailInfo_Eqp::Remove_ZeroCount_Item()
{
	auto nIndex = 0;

	std::vector <uint16_t> RemoveList;
	
	for (auto Iter = m_NG_Count.begin(); Iter != m_NG_Count.end(); Iter++)
	{
		if (0 == Iter->second.m_nCount_All)
		{
			RemoveList.push_back(Iter->second.m_nNGCode);
		}
	}

	for (auto nIdx = 0; nIdx < RemoveList.size(); ++nIdx)
	{
		m_NG_Count.erase(RemoveList.at(nIdx));
	}
}
