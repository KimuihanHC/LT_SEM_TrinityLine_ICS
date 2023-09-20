#include "stdafx.h"
#include "FailInfo.h"
#include "CommonFunction.h"

CFailInfo::CFailInfo()
{
}

CFailInfo::~CFailInfo()
{
}

void CFailInfo::Set_Path(LPCTSTR IN_szPath)
{
	m_szPath = IN_szPath;
}

void CFailInfo::Set_Path_Report(__in LPCTSTR IN_szPath)
{
	m_szPath_Report = IN_szPath;
}

void CFailInfo::Clear_EqpList()
{
	m_EqpList.clear();
}

void CFailInfo::Add_Equipment(__in uint8_t IN_nEqpOrder, LPCTSTR IN_szEquipmentID)
{
	CFailInfo_Eqp newEqp;
	newEqp.Set_EquipmentID(IN_nEqpOrder, IN_szEquipmentID);
	newEqp.Set_Path(m_szPath);

	m_EqpList.insert({ IN_nEqpOrder, newEqp });
}

void CFailInfo::Add_Equipment(uint8_t IN_nEqpOrder, LPCTSTR IN_szEquipmentID, uint8_t IN_nTestPortCount)
{
	CFailInfo_Eqp newEqp;
	newEqp.Set_EquipmentID(IN_nEqpOrder, IN_szEquipmentID);
	newEqp.Set_TestPortCount(IN_nTestPortCount);
	newEqp.Set_Path(m_szPath);

	m_EqpList.insert({ IN_nEqpOrder, newEqp });
}

size_t CFailInfo::Get_EquipmentCount()
{
	return m_EqpList.size();
}

bool CFailInfo::IsExist(uint8_t IN_nEqpOrder)
{
	auto result = m_EqpList.find(IN_nEqpOrder);
	if (result != m_EqpList.end())
	{
		return true;
	}
	else
	{
		return false;
	}
}

CFailInfo_Eqp & CFailInfo::Get_Equipment(uint8_t IN_nEqpOrder)
{
	return m_EqpList.at(IN_nEqpOrder);
}

const CFailInfo_Eqp & CFailInfo::Get_Equipment(uint8_t IN_nEqpOrder) const
{
	return m_EqpList.at(IN_nEqpOrder);
}

void CFailInfo::Reset_Count_All()
{
	for (auto Iter = m_EqpList.begin(); Iter != m_EqpList.end(); Iter++)
	{
		Iter->second.Reset_Count_All();
	}
}

void CFailInfo::Reset_Count(uint8_t IN_nEqpOrder)
{
	if (IsExist(IN_nEqpOrder))
	{
		m_EqpList.at(IN_nEqpOrder).Reset_Count_All();
	}
}

void CFailInfo::Reset_Count_NGCode(uint8_t IN_nEqpOrder, int16_t IN_nNGCode)
{
	if (IsExist(IN_nEqpOrder))
	{
		m_EqpList.at(IN_nEqpOrder).Reset_Count(IN_nNGCode);
	}
}

void CFailInfo::IncreaseCount(uint8_t IN_nEqpOrder, int16_t IN_nNGCode, uint8_t IN_nNG_Para)
{
	if (IsExist(IN_nEqpOrder))
	{
		m_EqpList.at(IN_nEqpOrder).IncreaseCount(IN_nNGCode, IN_nNG_Para);
	}
}

bool CFailInfo::Save_IniFile()
{
	bool bReturn = true;
	for (auto Iter = m_EqpList.begin(); Iter != m_EqpList.end(); Iter++)
	{
		bReturn &= Iter->second.Save_IniFile();
	}

	return bReturn;
}

bool CFailInfo::Load_IniFile()
{
	bool bReturn = true;
	for (auto Iter = m_EqpList.begin(); Iter != m_EqpList.end(); Iter++)
	{
		bReturn &= Iter->second.Load_IniFile();
	}

	return bReturn;
}

bool CFailInfo::Write_CSV_File(uint8_t IN_nEqpOrder, __in LPCTSTR IN_szPathFile)
{
	if (IsExist(IN_nEqpOrder))
	{
		return m_EqpList.at(IN_nEqpOrder).Write_CSV_File(IN_szPathFile);
	}

	return false;
}

bool CFailInfo::Write_CSV_File(__in LPCTSTR IN_szPath)
{
	CString szFile;

	SYSTEMTIME tmLc;
	GetLocalTime(&tmLc);

	bool bReturn = true;
	for (auto Iter = m_EqpList.begin(); Iter != m_EqpList.end(); Iter++)
	{
		szFile.Format(_T("%s\\FailInfo_%s_%04d%02d%02d_%02d%02d%02d.csv"), 
			IN_szPath, 
			Iter->second.Get_EquipmentID(),
			tmLc.wYear, tmLc.wMonth, tmLc.wDay,
			tmLc.wHour, tmLc.wMinute, tmLc.wSecond
			);

		bReturn &= Iter->second.Write_CSV_File(szFile);
	}

	return bReturn;
}

bool CFailInfo::Write_CSV_File_Default()
{
	if (false == m_szPath_Report.IsEmpty())
	{
		CString szPath;

		SYSTEMTIME tmLc;
		GetLocalTime(&tmLc);

		szPath.Format(_T("%s\\NGCount\\%04d\\%02d"), m_szPath_Report.GetBuffer(), tmLc.wYear, tmLc.wMonth);
		MakeSubDirectory(szPath.GetBuffer());

		return Write_CSV_File(szPath.GetBuffer());
	}

	return false;
}
