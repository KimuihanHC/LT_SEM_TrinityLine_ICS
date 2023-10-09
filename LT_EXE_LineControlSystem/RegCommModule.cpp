//*****************************************************************************
// Filename	: 	RegEquipment.cpp
// Created	:	2021/12/21 - 13:46
// Modified	:	2021/12/21 - 13:46
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#include "stdafx.h"
#include "RegCommModule.h"
#include "Registry.h"

//#define USE_VERIFY_KEY_ALWAYS	// ?ˆì??¤íŠ¸ë¦¬ì— ?°ì´??ê¸°ë¡ ???Œë§ˆ???ˆì??¤íŠ¸ë¦?ê²½ë¡œ ì²´í¬ ë°??ì„±

CRegCommModule::CRegCommModule()
{

}
CRegCommModule::CRegCommModule(__in LPCTSTR lpszRegPath)
{
}
CRegCommModule::~CRegCommModule()
{
}

bool CRegCommModule::Save_Equipment_ID(__in const CEquipment* IN_pEquipment)
{
#if defined(EES_XML)//20231003
	CRegistry*	m_pReg = new CRegistry(HKEY_CURRENT_USER);
	CString		szRegPath;
	CString		szKey;
	CString		szValue;
	DWORD		dwValue = 0;
	szRegPath.Format(_T("%s\\%s"), m_szRegModulePath.GetBuffer(), IN_pEquipment->Get_EquipmentId());
#ifdef USE_VERIFY_KEY_ALWAYS
	if (!m_pReg->VerifyKey(szRegPath))
	{
		m_pReg->CreateKey(HKEY_CURRENT_USER, szRegPath);
	}
	m_pReg->Close();
#endif
	if (m_pReg->Open(HKEY_CURRENT_USER, szRegPath))	{
		m_pReg->WriteString(_T("SUBEQPID"), IN_pEquipment->Get_SubEqpID());
		for (auto nIdx = 0; nIdx < IN_pEquipment->Get_EquipmentIDCount(); ++nIdx)		{
			szKey.Format(_T("Port_%02d_EquipID"), nIdx);
			m_pReg->WriteString(szKey.GetBuffer(), IN_pEquipment->Get_EquipmentIDStatus(nIdx).Get_EQUIPMENTID());
			szKey.Format(_T("Port_%02d_PortID"), nIdx);
			m_pReg->WriteString(szKey.GetBuffer(), IN_pEquipment->Get_EquipmentIDStatus(nIdx).Get_PORTID());
		}
	}else{
		delete m_pReg;
		return false;
	}

	m_pReg->Close();

	delete m_pReg;
#endif
	return true;
}

bool CRegCommModule::Load_Equipment_ID(__in LPCTSTR IN_szEqpID, __out CEquipment& OUT_Equipment)
{
#if defined(EES_XML)//20231003
	CRegistry*	m_pReg = new CRegistry(HKEY_CURRENT_USER);
	CString		szRegPath;
	CString		szKey;
	CString		szValue;
	DWORD		dwValue = 0;
	szRegPath.Format(_T("%s\\%s"), m_szRegModulePath.GetBuffer(), IN_szEqpID);

	if (m_pReg->Open(HKEY_CURRENT_USER, szRegPath))
	{
		szKey.Format(_T("SUBEQPID"));
		if (m_pReg->ReadString(szKey.GetBuffer(), szValue)) {
			szValue.GetBuffer();
		}
		OUT_Equipment.Set_SubEqpID(szValue);

		std::vector<ST_EquipmentID> mEquipmentList;
		for (auto nIdx = 0; nIdx < OUT_Equipment.Get_EquipmentIDCount(); ++nIdx)
		{
			ST_EquipmentID newEquipment;

			szKey.Format(_T("Port_%02d_EquipID"), nIdx);
			if (m_pReg->ReadString(szKey.GetBuffer(), szValue))
				newEquipment.Set_EQUIPMENTID(szValue.GetBuffer());
			else
				newEquipment.Set_EQUIPMENTID(_T(""));

			szKey.Format(_T("Port_%02d_PortID"), nIdx);
			if (m_pReg->ReadString(szKey.GetBuffer(), szValue))
				newEquipment.Set_PORTID(szValue.GetBuffer()) ;
			else
				newEquipment.Set_PORTID(_T(""));
			OUT_Equipment.Set_PortStatusEquipmentIDEvent(nIdx, &newEquipment);
		}
		
	}else{
		delete m_pReg;
		return false;
	}
	m_pReg->Close();
	delete m_pReg;
#endif
	return true;
}

bool CRegCommModule::Save_Equipment_ID(__in const CServer* IN_pServer)
{
#if defined(EES_XML)//20231003
	CRegistry*	m_pReg = new CRegistry(HKEY_CURRENT_USER);
	CString		szRegPath;
	CString		szKey;
	CString		szValue;
	DWORD		dwValue = 0;
	szRegPath.Format(_T("%s\\%s"), m_szRegModulePath.GetBuffer(), IN_pServer->Get_ServerId());
#ifdef USE_VERIFY_KEY_ALWAYS
	if (!m_pReg->VerifyKey(szRegPath))
	{
		m_pReg->CreateKey(HKEY_CURRENT_USER, szRegPath);
	}
	m_pReg->Close();
#endif
	if (m_pReg->Open(HKEY_CURRENT_USER, szRegPath))	{
		m_pReg->WriteString(_T("SUBEQPID"), IN_pServer->Get_SubEqpID());
		for (auto nIdx = 0; nIdx < IN_pServer->Get_EquipmentIDCount(); ++nIdx)		{
			szKey.Format(_T("Port_%02d_EquipID"), nIdx);
			m_pReg->WriteString(szKey.GetBuffer(), IN_pServer->Get_EquipmentIDStatus(nIdx).Get_EQUIPMENTID());
			szKey.Format(_T("Port_%02d_PortID"), nIdx);
			m_pReg->WriteString(szKey.GetBuffer(), IN_pServer->Get_EquipmentIDStatus(nIdx).Get_PORTID());
		}
	}else{
		delete m_pReg;
		return false;
	}
	m_pReg->Close();
	delete m_pReg;
#endif
	return true;
}
bool CRegCommModule::Load_Equipment_ID(__in LPCTSTR IN_szSvrID, __out CServer& OUT_Server)
{
#if defined(EES_XML)//20231003
	CRegistry*	m_pReg = new CRegistry(HKEY_CURRENT_USER);
	CString		szRegPath;
	CString		szKey;
	CString		szValue;
	DWORD		dwValue = 0;
	szRegPath.Format(_T("%s\\%s"), m_szRegModulePath.GetBuffer(), IN_szSvrID);
	if (m_pReg->Open(HKEY_CURRENT_USER, szRegPath))
	{
		szKey.Format(_T("SUBEQPID"));
		if (m_pReg->ReadString(szKey.GetBuffer(), szValue)) {
			szValue.GetBuffer();
		}
		OUT_Server.Set_SubEqpID(szValue);		
		std::vector<ST_EquipmentID> mServerList;
		for (auto nIdx = 0; nIdx < OUT_Server.Get_EquipmentIDCount(); ++nIdx)		{
			ST_EquipmentID newServer;
			szKey.Format(_T("Port_%02d_EquipID"), nIdx);
			if (m_pReg->ReadString(szKey.GetBuffer(), szValue))
				newServer.Set_EQUIPMENTID(szValue.GetBuffer());
			else
				newServer.Set_EQUIPMENTID(_T(""));

			szKey.Format(_T("Port_%02d_PortID"), nIdx);
			if (m_pReg->ReadString(szKey.GetBuffer(), szValue))
				newServer.Set_PORTID(szValue.GetBuffer());
			else
				newServer.Set_PORTID(_T(""));
			OUT_Server.Set_PortStatusEquipmentIDEvent(nIdx, &newServer);
		}
	}else{
		delete m_pReg;
		return false;
	}
	m_pReg->Close();
	delete m_pReg;
#endif
	return true;
}



bool CRegCommModule::Save_Equipment_ID(__in LPCTSTR IN_szEqpID, __in uint8_t IN_nPortIndex, __in uint8_t Type, LPCTSTR IN_Data)
{
#if defined(EES_XML)//20231003
	CRegistry*	m_pReg = new CRegistry(HKEY_CURRENT_USER);
	CString		szRegPath;
	CString		szKey;
	CString		szValue;
	DWORD		dwValue = 0;
	szRegPath.Format(_T("%s\\%s"), m_szRegModulePath.GetBuffer(), IN_szEqpID);
#ifdef USE_VERIFY_KEY_ALWAYS
	if (!m_pReg->VerifyKey(szRegPath))
	{
		m_pReg->CreateKey(HKEY_CURRENT_USER, szRegPath);
	}
	m_pReg->Close();
#endif
	if (m_pReg->Open(HKEY_CURRENT_USER, szRegPath))	{
		switch (Type) {
		case 0:
			m_pReg->WriteString(_T("SUBEQPID"), IN_Data);
			break;
		case 1:
			szKey.Format(_T("Port_%02d_EquipID"), IN_nPortIndex);
			m_pReg->WriteString(szKey.GetBuffer(), IN_Data);
			break;
		case 2:
			szKey.Format(_T("Port_%02d_PortID"), IN_nPortIndex);
			m_pReg->WriteString(szKey.GetBuffer(), IN_Data);
			break;
		}
	}else{
		delete m_pReg;
		return false;
	}
	m_pReg->Close();
	delete m_pReg;
#endif
	return true;
}

CString CRegCommModule::Load_Equipment_ID(__in LPCTSTR IN_szEqpID, __in uint8_t IN_nPortIndex, __in uint8_t Type)
{
	CString		szValue;
#if defined(EES_XML)//20231003
	CRegistry*	m_pReg = new CRegistry(HKEY_CURRENT_USER);
	CString		szRegPath;
	CString		szKey;

	DWORD		dwValue = 0;
	szRegPath.Format(_T("%s\\%s"), m_szRegModulePath.GetBuffer(), IN_szEqpID);
	if (m_pReg->Open(HKEY_CURRENT_USER, szRegPath))	{
		switch (Type) {
		case  0:
			szKey.Format(_T("SUBEQPID"));
			break;
		case 1:
			szKey.Format(_T("Port_%02d_EquipID"), IN_nPortIndex);
			break;
		case 2:
			szKey.Format(_T("Port_%02d_PortID"), IN_nPortIndex);
			break;
		}
		m_pReg->ReadString(szKey.GetBuffer(), szValue);
	}	else	{
		delete m_pReg;
		return false;
	}
	m_pReg->Close();
	delete m_pReg;
#endif
	return szValue.GetBuffer();
}
