//*****************************************************************************
// Filename	: 	InOutCount.cpp
// Created	:	2023/3/16 - 13:51
// Modified	:	2023/3/16 - 13:51
//
// Author	:	piring
//	
// Purpose	:	
//*****************************************************************************
#include "stdafx.h"
#include "InOutCount.h"
#include "Registry.h"

CInOutCount::CInOutCount()
{
	m_szRegPath.Format(_T("%s\\Socket_InOutCount"), REG_PATH_APP_BASE);
}

CInOutCount::~CInOutCount()
{

}

void CInOutCount::Compute_Ongoing()
{
	m_nOngoing = m_nIn - m_nOut - m_nRemove;
};

bool CInOutCount::Save_Registry()
{
	CRegistry*	m_pReg = new CRegistry(HKEY_CURRENT_USER);
	CString		szKey;
	CString		szValue;

#ifdef USE_VERIFY_KEY_ALWAYS
	if (!m_pReg->VerifyKey(m_szRegPath))
	{
		m_pReg->CreateKey(HKEY_CURRENT_USER, m_szRegPath);
	}
	m_pReg->Close();
#endif

	if (m_pReg->Open(HKEY_CURRENT_USER, m_szRegPath))
	{
		m_pReg->WriteDWORD(_T("In_Count"), m_nIn);
		m_pReg->WriteDWORD(_T("Out_Count"), m_nIn);
		m_pReg->WriteDWORD(_T("Remove_Count"), m_nIn);
	}
	else
	{
		delete m_pReg;
		return false;
	}

	m_pReg->Close();

	delete m_pReg;
	return true;
}

bool CInOutCount::Load_Registry()
{
	CRegistry*	m_pReg = new CRegistry(HKEY_CURRENT_USER);
	CString		szKey;
	CString		szValue;
	DWORD		dwValue = 0;

	if (m_pReg->Open(HKEY_CURRENT_USER, m_szRegPath))
	{
		if (m_pReg->ReadDWORD(_T("In_Count"), dwValue))
			m_nIn = static_cast<uint8_t>(dwValue);
		else
			m_nIn = 0;

		if (m_pReg->ReadDWORD(_T("Out_Count"), dwValue))
			m_nOut = static_cast<uint8_t>(dwValue);
		else
			m_nOut = 0;

		if (m_pReg->ReadDWORD(_T("Remove_Count"), dwValue))
			m_nRemove = static_cast<uint8_t>(dwValue);
		else
			m_nRemove = 0;

		Compute_Ongoing();
	}
	else
	{
		delete m_pReg;
		return false;
	}

	m_pReg->Close();

	delete m_pReg;
	return true;
}

void CInOutCount::OnReset()
{
	m_nIn		= 0;
	m_nOut		= 0;
	m_nOngoing	= 0;
	m_nRemove	= 0;
};

void CInOutCount::OnIncrease_In()
{
	++m_nIn;

	Compute_Ongoing();
};

// 后 家南篮 力寇 秦具 茄促.
void CInOutCount::OnIncrease_Out()
{
	++m_nOut;

	Compute_Ongoing();
};

// 后 家南篮 力寇 秦具 茄促.
void CInOutCount::OnIncrease_Remove()
{
	++m_nRemove;

	Compute_Ongoing();
}

bool CInOutCount::Save_CSVFile(__in LPCTSTR IN_szFullPath)
{
	CString szText;
	szText.Format(_T("IN: %d, OUT: %d, REMOVE: %d"), m_nIn, m_nOut, m_nRemove);

	SYSTEMTIME tmLocal;
	GetLocalTime(&tmLocal);

	tmLocal.wYear;
	tmLocal.wMonth;
	tmLocal.wDay;
	tmLocal.wHour;
	tmLocal.wMinute;
	tmLocal.wSecond;

	return true;
}
