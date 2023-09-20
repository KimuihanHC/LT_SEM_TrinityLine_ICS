
#include "stdafx.h"
#include "TactTime.h"
#include "Registry.h"

CTactTime::CTactTime()
{
	ZeroMemory(&m_tmUnload_Prev, sizeof(SYSTEMTIME));
	ZeroMemory(&m_tmUnload_First, sizeof(SYSTEMTIME));
}

CTactTime::~CTactTime()
{
}

CTactTime & CTactTime::operator=(const CTactTime & ref)
{
	m_bFlag_timeCheck	= ref.m_bFlag_timeCheck;
	m_dTacttime			= ref.m_dTacttime;
	m_dTacttime_Ave		= ref.m_dTacttime_Ave;
	m_dCount			= ref.m_dCount;
	memcpy(&m_tmUnload_First, &ref.m_tmUnload_First, sizeof(SYSTEMTIME));
	memcpy(&m_tmUnload_Prev, &ref.m_tmUnload_First, sizeof(SYSTEMTIME));

	return *this;
}

void CTactTime::Reset()
{
	m_bFlag_timeCheck	= false;
	m_dTacttime			= 0.0f;
	m_dTacttime_Ave		= 0.0f;
	m_dCount			= 0.0f;
	ZeroMemory(&m_tmUnload_First, sizeof(SYSTEMTIME));
	ZeroMemory(&m_tmUnload_Prev, sizeof(SYSTEMTIME));
}

bool CTactTime::Get_Flag_timeCheck() const
{
	return m_bFlag_timeCheck;
}

double CTactTime::Get_Count() const
{
	return m_dCount;
}

inline double CTactTime::CompareSystemTime(SYSTEMTIME * pEndTime, SYSTEMTIME * pStartTime)
{
	/*double dStartTime;
	double dEndTime;
	SystemTimeToVariantTime(pStartTime, &dStartTime);
	SystemTimeToVariantTime(pEndTime, &dEndTime);

	return (dEndTime - dStartTime) * 100000.0f;*/

	tm tmTime1, tmTime2;
	time_t timeTime1, timeTime2;

	tmTime1.tm_sec		= pEndTime->wSecond;
	tmTime1.tm_min		= pEndTime->wMinute;
	tmTime1.tm_hour		= pEndTime->wHour;
	tmTime1.tm_mday		= pEndTime->wDay;
	tmTime1.tm_mon		= pEndTime->wMonth - 1;
	tmTime1.tm_year		= pEndTime->wYear - 1900;
	tmTime1.tm_isdst	= 0;
	timeTime1			= ::mktime(&tmTime1);

	tmTime2.tm_sec		= pStartTime->wSecond;
	tmTime2.tm_min		= pStartTime->wMinute;
	tmTime2.tm_hour		= pStartTime->wHour;
	tmTime2.tm_mday		= pStartTime->wDay;
	tmTime2.tm_mon		= pStartTime->wMonth - 1;
	tmTime2.tm_year		= pStartTime->wYear - 1900;
	tmTime2.tm_isdst	= 0;
	timeTime2			= ::mktime(&tmTime2);

	// time2와 time1의 second 차이를 double 형으로 리턴
	return ::difftime(timeTime1, timeTime2);
}

inline CString CTactTime::SystemTimeToFormatString(SYSTEMTIME * IN_pTime)
{
	CString strTemp;
	strTemp.Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), IN_pTime->wYear, IN_pTime->wMonth, IN_pTime->wDay,
		IN_pTime->wHour, IN_pTime->wMinute, IN_pTime->wSecond);

	return strTemp;
}

inline BOOL CTactTime::FormatStringToSystemTime(LPCTSTR IN_szTime, SYSTEMTIME & OUT_Time)
{
	if (NULL == IN_szTime)
		return FALSE;

	if (_tcslen(IN_szTime) < 19)
		return FALSE;

	CString strSource = IN_szTime;
	CString strValue;

	strValue = strSource.Left(4);
	strSource.Delete(0, 5);
	OUT_Time.wYear = (WORD)_ttoi(strValue);

	strValue = strSource.Left(2);
	strSource.Delete(0, 3);
	OUT_Time.wMonth = (WORD)_ttoi(strValue);

	strValue = strSource.Left(2);
	strSource.Delete(0, 3);
	OUT_Time.wDay = (WORD)_ttoi(strValue);

	strValue = strSource.Left(2);
	strSource.Delete(0, 3);
	OUT_Time.wHour = (WORD)_ttoi(strValue);

	strValue = strSource.Left(2);
	strSource.Delete(0, 3);
	OUT_Time.wMinute = (WORD)_ttoi(strValue);

	strValue = strSource.Left(2);
	strSource.Delete(0, 2);
	OUT_Time.wSecond = (WORD)_ttoi(strValue);

	return TRUE;
}

double CTactTime::Set_UnloadTime(SYSTEMTIME * IN_pUnloadTime)
{
	if (m_bFlag_timeCheck)
	{
		m_dCount += 1.0f;

		m_dTacttime		= CompareSystemTime(IN_pUnloadTime, &m_tmUnload_Prev);
		m_dTacttime_Ave = CompareSystemTime(IN_pUnloadTime, &m_tmUnload_First) / m_dCount;
	
		memcpy(&m_tmUnload_Prev, IN_pUnloadTime, sizeof(SYSTEMTIME));
	}
	else
	{
		memcpy(&m_tmUnload_First, IN_pUnloadTime, sizeof(SYSTEMTIME));
		memcpy(&m_tmUnload_Prev, IN_pUnloadTime, sizeof(SYSTEMTIME));

		m_bFlag_timeCheck = true;

		m_dTacttime		= 0.0f;
		m_dTacttime_Ave = 0.0f;
		m_dCount		= 0.0f;
	}

	return m_dTacttime;
}

double CTactTime::Get_TactTime() const
{
	return m_dTacttime;
}

double CTactTime::Get_TactTime_Ave() const
{
	return m_dTacttime_Ave;
}

const SYSTEMTIME & CTactTime::Get_Time_First() const
{
	return m_tmUnload_First;
}

const SYSTEMTIME & CTactTime::Get_Time_Previous() const
{
	return m_tmUnload_Prev;
}

bool CTactTime::Save_Tacttime(LPCTSTR IN_szRegPath)
{
	CRegistry	m_Reg;
	CString		szRegPath;
	CString		szValue;
	szRegPath.Format(_T("%s\\Tacttime"), IN_szRegPath);

	if (!m_Reg.VerifyKey(HKEY_CURRENT_USER, szRegPath))
	{
		m_Reg.CreateKey(HKEY_CURRENT_USER, szRegPath);
	}

	if (m_Reg.Open(HKEY_CURRENT_USER, szRegPath))
	{
		//-------------------------------------------------------
		// Tacttime
		//-------------------------------------------------------
		//bool			m_bFlag_timeCheck
		m_Reg.WriteDWORD(_T("Flag_timeCheck"), m_bFlag_timeCheck ? 1 : 0);

		//double		m_dTacttime
		szValue.Format(_T("%.4f"), m_dTacttime);
		m_Reg.WriteString(_T("Tacttime"), szValue.GetBuffer());

		//double		m_dTacttime_Ave
		szValue.Format(_T("%.4f"), m_dTacttime_Ave);
		m_Reg.WriteString(_T("Tacttime_Ave"), szValue.GetBuffer());

		//double		m_dCount
		szValue.Format(_T("%.4f"), m_dCount);
		m_Reg.WriteString(_T("Count"), szValue.GetBuffer());

		//SYSTEMTIME	m_tmUnload_First;
		szValue = SystemTimeToFormatString(&m_tmUnload_First);
		m_Reg.WriteString(_T("Time_First"), szValue.GetBuffer());

		//SYSTEMTIME	m_tmUnload_Prev;
		szValue = SystemTimeToFormatString(&m_tmUnload_Prev);
		m_Reg.WriteString(_T("Time_Prev"), szValue.GetBuffer());
	}
	else
	{
		return false;
	}

	m_Reg.Close();
#
	return true;
}

bool CTactTime::Load_Tacttime(LPCTSTR IN_szRegPath)
{
	CRegistry	m_Reg;
	CString		szRegPath;
	CString		szKey;
	CString		szValue;
	DWORD		dwValue = 0;
	szRegPath.Format(_T("%s\\Tacttime"), IN_szRegPath);

	if (m_Reg.Open(HKEY_CURRENT_USER, szRegPath))
	{
		//-------------------------------------------------------
		// Tacttime
		//-------------------------------------------------------
		Reset();

		//bool			m_bFlag_timeCheck;
		if (m_Reg.ReadDWORD(_T("Flag_timeCheck"), dwValue))
			m_bFlag_timeCheck = dwValue ? true : false;

		//double		m_dTacttime;
		if (m_Reg.ReadString(_T("Tacttime"), szValue))
			m_dTacttime = _ttof(szValue.GetBuffer());

		//double		m_dTacttime_Ave;
		if (m_Reg.ReadString(_T("Tacttime_Ave"), szValue))
			m_dTacttime_Ave = _ttof(szValue.GetBuffer());

		//double		m_dCount;
		if (m_Reg.ReadString(_T("Count"), szValue))
			m_dCount = _ttof(szValue.GetBuffer());

		//SYSTEMTIME	m_tmUnload_First;
		if (m_Reg.ReadString(_T("Time_First"), szValue))
		{
			FormatStringToSystemTime(szValue.GetBuffer(), m_tmUnload_First);
		}

		//SYSTEMTIME	m_tmUnload_Prev;
		if (m_Reg.ReadString(_T("Time_Prev"), szValue))
		{
			FormatStringToSystemTime(szValue.GetBuffer(), m_tmUnload_Prev);
		}
	}
	else
	{
		return false;
	}

	m_Reg.Close();

	return true;
}


