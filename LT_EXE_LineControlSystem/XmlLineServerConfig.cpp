//*****************************************************************************
// Filename	: 	XmlLineConfig.cpp
// Created	:	2021/11/2 - 15:50
// Modified	:	2021/11/2 - 15:50
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#include "stdafx.h"
#include "XmlLineServerConfig.h"
#include "CommonFunction.h"
CXmlLineServerConfig::CXmlLineServerConfig()
{

}
CXmlLineServerConfig::~CXmlLineServerConfig()
{

}

void CXmlLineServerConfig::Backup_File(__in LPCTSTR szPath)
{
	//if (m_bUseBackup)
	{
		// C:\\Recipe\\SEM_T01_SocketInfo.xml
		// => C:\\Recipe\\BAK\\SEM_T01_SocketInfo_2021_1117_181900.xml
		if (!PathFileExists(szPath))		{
			return;		
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
		::CopyFile(szPath, szBackFilePath.GetBuffer(), FALSE);
	}
}
uint8_t CXmlLineServerConfig::Convert_SvrType_UI(__in int IN_iVer, __in uint8_t IN_nEqpTypeUI_Old)
{
	switch (IN_nEqpTypeUI_Old)
	{
	case SERVER_UI_EES:
		return enServerUI::SERVER_UI_EES;
		break;
	default:
		return IN_nEqpTypeUI_Old;
		break;
	}
	return IN_nEqpTypeUI_Old;
}
uint8_t CXmlLineServerConfig::Convert_EquipmentType(__in int IN_iVer, __in uint8_t IN_nEqpType_Old)
{
	return IN_nEqpType_Old;
}

BOOL CXmlLineServerConfig::Save_XML_String(__in LPCTSTR szFullPath, __in LPCTSTR szXML)
{
	CFile File;
	CFileException e;
	if (!PathFileExists(szFullPath))	{
		if (!File.Open(szFullPath, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite, &e))	{
			return FALSE;
		}
	}else{
		if (!File.Open(szFullPath, CFile::modeWrite | CFile::shareDenyWrite, &e))	{
			return FALSE;
		}
	}
	CStringA szANSI;
	USES_CONVERSION;
	szANSI = CT2A(szXML);
	File.SeekToEnd();
	File.Write(szANSI.GetBuffer(0), szANSI.GetLength());
	File.Flush();
	File.Close();
	return TRUE;
}
int CXmlLineServerConfig::ReadXML_Header(__in tinyxml2::XMLElement* IN_pRoot)
{
	if (IN_pRoot)	{
		IN_pRoot->Attribute("Format");
		CString szTemp;
		szTemp = IN_pRoot->Attribute("Version", 0);
		int iVer = _ttoi(szTemp.GetBuffer());
		return iVer;
	}	else	{
		return 0;
	}
}

//=============================================================================
// Method		: WriteXML_Header
// Access		: protected  
// Returns		: BOOL
// Parameter	: __in tinyxml2::XMLDocument * IN_pDoc
// Parameter	: __in tinyxml2::XMLElement * IN_pRoot
// Qualifier	:
// Last Update	: 2021/11/12 - 10:31
// Desc.		:
//=============================================================================
BOOL CXmlLineServerConfig::WriteXML_Header(__in tinyxml2::XMLDocument* IN_pDoc, __in tinyxml2::XMLElement* IN_pRoot)
{
	tinyxml2::XMLDeclaration* decl = IN_pDoc->NewDeclaration();
	IN_pDoc->LinkEndChild(decl);	//Test 2023.02.19a uhkim
	
	CStringA szVersion;
	IN_pRoot->SetAttribute("Format", "Line Configuration File");
	IN_pDoc->LinkEndChild(IN_pRoot);

	return TRUE;
}
BOOL CXmlLineServerConfig::WriteXML_Comment(__in tinyxml2::XMLDocument* IN_pDoc, __out tinyxml2::XMLElement* OUT_pRoot)
{
	tinyxml2::XMLComment* pComment = IN_pDoc->NewComment("Settings for Equipment");
	OUT_pRoot->LinkEndChild(pComment);
	return TRUE;
}
BOOL CXmlLineServerConfig::WriteXML_File(__in LPCTSTR szPath, __in tinyxml2::XMLDocument* IN_pDoc)
{
	CStringA szFileName;
	szFileName = CT2A(szPath);
	if (tinyxml2::XML_SUCCESS == IN_pDoc->SaveFile(szFileName.GetBuffer(0)))	{
		return TRUE;
	}else{
		return FALSE;
	}
}
BOOL CXmlLineServerConfig::Read_ServerInfo(__in tinyxml2::XMLElement* IN_pRoot, __out CConfig_Server& OUT_stLineInfo)
{
 	CStringA szTemp;
	if (IN_pRoot)	{
		tinyxml2::XMLElement* pLineInfo = IN_pRoot->FirstChildElement("LineInfo");
		if (pLineInfo)		{
			tinyxml2::XMLElement* pSvrList = pLineInfo->FirstChildElement("SvrList");
			if (pSvrList)			{
				OUT_stLineInfo.RemoveAll();
				INT_PTR iItemCount = pSvrList->Int64Attribute("Count", 0);
				CConfig_Svr* OUT_pSvr = NULL;
				for (INT_PTR nIdx = 0; nIdx < iItemCount; nIdx++)
				{
					CConfig_Svr stServer;
					OUT_stLineInfo.Svr_Add(stServer);

					OUT_pSvr = &OUT_stLineInfo.SvrList.at(nIdx);

					szTemp.Format("Svr_%03d", nIdx + 1);
					tinyxml2::XMLElement* pEquipment = pSvrList->FirstChildElement(szTemp.GetBuffer(0));
					if (pEquipment)
					{
						OUT_pSvr->Set_SvrOrder(pEquipment->UnsignedAttribute("SvrOrder", 0));
						OUT_pSvr->Set_Group(pEquipment->BoolAttribute("Group", false));
						// uint8_t		m_nGroupIndex;		// Í≤Ä??Í∑∏Î£π???§ÎπÑ ?∏Îç±??						
						//OUT_pEqp->Set_GroupIndex(pEquipment->UnsignedAttribute("GroupIndex", 0));
						szTemp = pEquipment->Attribute("Alias", 0);
						OUT_pSvr->Set_Alias(CA2T(szTemp.GetBuffer()));
						//OUT_pEqp->Set_EquipmentType(Convert_EquipmentType(IN_iVer, pEquipment->UnsignedAttribute("Equipment_Type", 0)));
						OUT_pSvr->Set_SvrType_UI(pEquipment->UnsignedAttribute("Server_Type", 0));
						szTemp = pEquipment->Attribute("Server_ID", 0);
						OUT_pSvr->Set_ServerId(CA2T(szTemp.GetBuffer()));
						szTemp = pEquipment->Attribute("IP_Address", 0);
						OUT_pSvr->Set_IP_Address(ntohl(inet_addr(szTemp.GetBuffer())));
					} // if (pEquipment)
				} // for()
			}
			if (pSvrList == nullptr)
			{
				return FALSE;
			}
		} // if (pLineInfo)
		else
		{
			return FALSE;
		}
	} // if (IN_pRoot)
	else
	{
		return FALSE;
	}

	return TRUE;
}
BOOL CXmlLineServerConfig::Write_ServerInfo(__in tinyxml2::XMLDocument* IN_pDoc, __in tinyxml2::XMLElement* IN_pRoot, __in const CConfig_Server* IN_pLineInfo)
{
	CStringA szTemp;
	tinyxml2::XMLElement* pLineInfo = IN_pDoc->NewElement("LineInfo");	
	{
		tinyxml2::XMLElement* pSvrList = IN_pDoc->NewElement("SvrList");
		size_t iItemCount = IN_pLineInfo->SvrList.size();
		pSvrList->SetAttribute("Count", (int64_t)iItemCount);
		const CConfig_Svr* IN_pSvr = NULL;
		for (size_t nIdx = 0; nIdx < iItemCount; nIdx++)		{
			IN_pSvr = &IN_pLineInfo->SvrList.at(nIdx);
			szTemp.Format("Svr_%03d", nIdx + 1);
			tinyxml2::XMLElement* pEquipment = IN_pDoc->NewElement(szTemp.GetBuffer(0));
			{
				pEquipment->SetAttribute("SvrOrder", (unsigned)IN_pSvr->Get_SvrOrder());
				pEquipment->SetAttribute("Group", IN_pSvr->Get_Group());
				//pEquipment->SetAttribute("GroupIndex", (unsigned)IN_pEqp->Get_GroupIndex());
				// CString		szAlias;
				szTemp = CT2A(IN_pSvr->Get_Alias());
				pEquipment->SetAttribute("Alias", szTemp.GetBuffer());
				//pEquipment->SetAttribute("Equipment_Type", (unsigned)IN_pEqp->Get_EquipmentType());
				pEquipment->SetAttribute("Server_Type", (unsigned)IN_pSvr->Get_SvrType_UI());
				szTemp = CT2A(IN_pSvr->Get_ServerId());
				pEquipment->SetAttribute("Server_ID", szTemp.GetBuffer());
				DWORD dwAddress = ntohl(IN_pSvr->Get_IP_Address());
				//szTemp = inet_ntoa(*(IN_ADDR*)&dwAddress);
				szTemp.Format("%d.%d.%d.%d", FOURTH_IPADDRESS(dwAddress), THIRD_IPADDRESS(dwAddress), SECOND_IPADDRESS(dwAddress), FIRST_IPADDRESS(dwAddress));
				pEquipment->SetAttribute("IP_Address", szTemp.GetBuffer());
			}

			pSvrList->LinkEndChild(pEquipment);
		}
		pLineInfo->LinkEndChild(pSvrList);
	}
	IN_pRoot->LinkEndChild(pLineInfo);

	return TRUE;
}
BOOL CXmlLineServerConfig::LoadXML_ServerInfo(__in LPCTSTR szPath, __out CConfig_Server& OUT_stLineInfo)
{
	if (NULL == szPath)
		return FALSE;
	if (!PathFileExists(szPath))	{
		return FALSE;
	}
 	tinyxml2::XMLDocument	ReadDoc; 
 	USES_CONVERSION;
	CStringA szTemp;
 	szTemp = CT2A(szPath);
 	if (tinyxml2::XML_SUCCESS != ReadDoc.LoadFile(szTemp.GetBuffer())) 	{
 		TRACE("LoadXML_LineInfo : Failed to open [%s]\n", szTemp);
 	} 
 	tinyxml2::XMLDeclaration*	pDecl = ReadDoc.ToDeclaration();
 	tinyxml2::XMLElement*		pRoot = ReadDoc.FirstChildElement("Luritech"); 
	int iVer = ReadXML_Header(pRoot);
	//if (iVer <= 0)
 	//{
 	//	return FALSE;
 	//}
	if (FALSE == Read_ServerInfo(pRoot, OUT_stLineInfo))	{
		return FALSE;
	}
	return TRUE;
}
BOOL CXmlLineServerConfig::SaveXML_ServerInfo(__in LPCTSTR szPath, __in const CConfig_Server* IN_pLineInfo)
{
	tinyxml2::XMLDocument doc;
	USES_CONVERSION;
	tinyxml2::XMLElement* pRoot = doc.NewElement("Luritech");
	WriteXML_Header(&doc, pRoot);	
	WriteXML_Comment(&doc, pRoot);
	Write_ServerInfo(&doc, pRoot, IN_pLineInfo);
	return WriteXML_File(szPath, &doc);
}

//=============================================================================
// Method		: Read_ModelInfo
// Access		: public  
// Returns		: BOOL
// Parameter	: __in tinyxml2::XMLElement * IN_pRoot
// Parameter	: __out CConfig_Model & OUT_stModelInfo
// Qualifier	:
// Last Update	: 2021/11/12 - 10:31
// Desc.		:
//=============================================================================
/*
BOOL CXmlLineServerConfig::Read_ModelInfo(__in tinyxml2::XMLElement* IN_pRoot, __out CConfig_Model& OUT_stModelInfo)
{
	tinyxml2::XMLElement* pModelInfo = IN_pRoot->FirstChildElement("ModelInfo");
	if (pModelInfo)
	{
		CStringA szTemp;
		
		// Alias
		szTemp = pModelInfo->Attribute("Alias", 0);
		OUT_stModelInfo.m_szAlias = CA2T(szTemp.GetBuffer());

		// Socket Type
		OUT_stModelInfo.m_nSocketType = pModelInfo->UnsignedAttribute("Socket_Type", 0);

		// Tester ?¨Ïö©?¨Î?
		tinyxml2::XMLElement* pTesterList = pModelInfo->FirstChildElement("Tester_List");
		if (pTesterList)
		{
			// Tester Ï¢ÖÎ•ò Í∞úÏàò
			INT_PTR iItemCount = __min(pTesterList->Int64Attribute("Count", 0), Max_TesterCount);

			CConfig_Eqp* OUT_pEqp = NULL;
			for (INT_PTR nIdx = 0; nIdx < iItemCount; nIdx++)
			{
				szTemp = CT2A(g_szTesterTypeName[nIdx]);
				szTemp.Replace(' ', '_');
				OUT_stModelInfo.m_bEnable_TestType[nIdx] = pTesterList->BoolAttribute(szTemp.GetBuffer(), false);
			} // for()

		} // if (pTesterList)
		else
		{
			return FALSE;
		}
	} // if (pModelInfo)
	else
	{
		return FALSE;
	}

	return TRUE;
}
*/
//=============================================================================
// Method		: Write_ModelInfo
// Access		: public  
// Returns		: BOOL
// Parameter	: __in tinyxml2::XMLDocument * IN_pDoc
// Parameter	: __in tinyxml2::XMLElement * IN_pRoot
// Parameter	: __in const CConfig_Model * IN_pModelInfo
// Qualifier	:
// Last Update	: 2021/11/12 - 10:31
// Desc.		:
//=============================================================================
/*
BOOL CXmlLineConfig::Write_ModelInfo(__in tinyxml2::XMLDocument* IN_pDoc, __in tinyxml2::XMLElement* IN_pRoot, __in const CConfig_Model* IN_pModelInfo)
{
	tinyxml2::XMLElement* pModelInfo = IN_pDoc->NewElement("ModelInfo");
	{
		CStringA szTemp;

		// Alias
		szTemp = CT2A(IN_pModelInfo->m_szAlias);
		pModelInfo->SetAttribute("Alias", szTemp.GetBuffer());

		// Socket Type
		pModelInfo->SetAttribute("Socket_Type", (unsigned)IN_pModelInfo->m_nSocketType);

		// Tester ?¨Ïö©?¨Î?
		tinyxml2::XMLElement* pTesterList = IN_pDoc->NewElement("Tester_List");
		size_t iItemCount = Max_TesterCount;
		pTesterList->SetAttribute("Count", (int64_t)iItemCount);

		for (size_t nIdx = 0; nIdx < iItemCount; nIdx++)
		{
			szTemp = CT2A(g_szTesterTypeName[nIdx]);
			szTemp.Replace(' ', '_');

			pTesterList->SetAttribute(szTemp.GetBuffer(), IN_pModelInfo->m_bEnable_TestType[nIdx]);
		}

		pModelInfo->LinkEndChild(pTesterList);
	}
	IN_pRoot->LinkEndChild(pModelInfo);

	return TRUE;
}*/

//=============================================================================
// Method		: LoadXML_ModelInfo
// Access		: public  
// Returns		: BOOL
// Parameter	: __in LPCTSTR szPath
// Parameter	: __out CConfig_Model & OUT_stModelInfo
// Qualifier	:
// Last Update	: 2021/11/11 - 15:30
// Desc.		:
//=============================================================================
/*
BOOL CXmlLineConfig::LoadXML_ModelInfo(__in LPCTSTR szPath, __out CConfig_Model& OUT_stModelInfo)
{
	if (NULL == szPath)
		return FALSE;

	// ?åÏùº??Ï°¥Ïû¨?òÎäîÍ∞Ä?
	if (!PathFileExists(szPath))
	{
		return FALSE;
	}

	tinyxml2::XMLDocument	ReadDoc;

	USES_CONVERSION;
	CStringA szTemp;
	szTemp = CT2A(szPath);
	if (tinyxml2::XML_SUCCESS != ReadDoc.LoadFile(szTemp.GetBuffer()))
	{
		TRACE("LoadXML_LineInfo : Failed to open [%s]\n", szTemp);
	}

	tinyxml2::XMLDeclaration*	pDecl = ReadDoc.ToDeclaration();
	tinyxml2::XMLElement*		pRoot = ReadDoc.FirstChildElement("Luritech");

	if (ReadXML_Header(pRoot) <= 0)
	{
		return FALSE;
	}

	if (FALSE == Read_ModelInfo(pRoot, OUT_stModelInfo))
	{
		return FALSE;
	}

	return TRUE;
}
*/
//=============================================================================
// Method		: SaveXML_ModelInfo
// Access		: public  
// Returns		: BOOL
// Parameter	: __in LPCTSTR szPath
// Parameter	: __in const CConfig_Model * IN_pModelInfo
// Qualifier	:
// Last Update	: 2021/11/11 - 15:30
// Desc.		:
//=============================================================================
/*
BOOL CXmlLineConfig::SaveXML_ModelInfo(__in LPCTSTR szPath, __in const CConfig_Model* IN_pModelInfo)
{
	tinyxml2::XMLDocument doc;

	USES_CONVERSION;
	tinyxml2::XMLElement* pRoot = doc.NewElement("Luritech");

	WriteXML_Header(&doc, pRoot);

	// Ï£ºÏÑù
	WriteXML_Comment(&doc, pRoot);

	// Model Configuration 
	Write_ModelInfo(&doc, pRoot, IN_pModelInfo);

	// ?åÏùºÎ°??Ä??	Backup_File(szPath);
	return WriteXML_File(szPath, &doc);
}

//=============================================================================
// Method		: LoadXML_LineModelInfo
// Access		: public  
// Returns		: BOOL
// Parameter	: __in LPCTSTR szPath
// Parameter	: __out CConfig_Line & OUT_stLineInfo
// Parameter	: __out CConfig_Model & OUT_stModelInfo
// Qualifier	:
// Last Update	: 2021/11/12 - 11:18
// Desc.		:
//=============================================================================
BOOL CXmlLineConfig::LoadXML_LineModelInfo(__in LPCTSTR szPath, __out CConfig_Line& OUT_stLineInfo, __out CConfig_Model& OUT_stModelInfo)
{
	if (NULL == szPath)
		return FALSE;

	// ?åÏùº??Ï°¥Ïû¨?òÎäîÍ∞Ä?
	if (!PathFileExists(szPath))
	{
		return FALSE;
	}

	tinyxml2::XMLDocument	ReadDoc;

	USES_CONVERSION;
	CStringA szTemp;
	szTemp = CT2A(szPath);
	if (tinyxml2::XML_SUCCESS != ReadDoc.LoadFile(szTemp.GetBuffer()))
	{
		TRACE("LoadXML_LineInfo : Failed to open [%s]\n", szTemp);
	}

	tinyxml2::XMLDeclaration*	pDecl = ReadDoc.ToDeclaration();
	tinyxml2::XMLElement*		pRoot = ReadDoc.FirstChildElement("Luritech");

	int iVer = ReadXML_Header(pRoot);
	if (iVer <= 0)
	{
		return FALSE;
	}

	// version ?ïÏù∏ ?¥Ïïº ??

	if (FALSE == Read_LineInfo(pRoot, OUT_stLineInfo, iVer))
	{
		return FALSE;
	}

	if (FALSE == Read_ModelInfo(pRoot, OUT_stModelInfo))
	{
		return FALSE;
	}

	return TRUE;
}

//=============================================================================
// Method		: SaveXML_LineModelInfo
// Access		: public  
// Returns		: BOOL
// Parameter	: __in LPCTSTR szPath
// Parameter	: __in const CConfig_Line * IN_pLineInfo
// Parameter	: __in const CConfig_Model * IN_pModelInfo
// Qualifier	:
// Last Update	: 2021/11/12 - 11:18
// Desc.		:
//=============================================================================
BOOL CXmlLineConfig::SaveXML_LineModelInfo(__in LPCTSTR szPath, __in const CConfig_Line* IN_pLineInfo, __in const CConfig_Model* IN_pModelInfo)
{
	tinyxml2::XMLDocument doc;

	USES_CONVERSION;
	tinyxml2::XMLElement* pRoot = doc.NewElement("Luritech");

	WriteXML_Header(&doc, pRoot);

	// Ï£ºÏÑù
	WriteXML_Comment(&doc, pRoot);

	// Line Configuration 
	Write_LineInfo(&doc, pRoot, IN_pLineInfo);

	// Model Configuration 
	Write_ModelInfo(&doc, pRoot, IN_pModelInfo);

	// ?åÏùºÎ°??Ä??	Backup_File(szPath);
	return WriteXML_File(szPath, &doc);
}
*/