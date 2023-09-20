//*****************************************************************************
// Filename	: 	XmlEESConfig.cpp
// Created	:	
// Modified	:	2023.04.20 - 17:00
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#include "stdafx.h"
#include "XmlEESConfig.h"
#include "CommonFunction.h"

CXmlEESConfig::CXmlEESConfig()
{

}
CXmlEESConfig::~CXmlEESConfig()
{

}

//=============================================================================
// Method		: Backup_File
// Access		: protected  
// Returns		: void
// Parameter	: __in LPCTSTR szPath
// Qualifier	:
// Last Update	: 2023.04.20 - 17:00
// Desc.		:
//=============================================================================
void CXmlEESConfig::Backup_File(__in LPCTSTR szPath)
{
	{
		if (!PathFileExists(szPath))
		{
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
//=============================================================================
// Method		: Save_XML_String
// Access		: protected  
// Returns		: BOOL
// Parameter	: __in LPCTSTR szFullPath
// Parameter	: __in LPCTSTR szXML
// Qualifier	:
// Last Update	: 2023.04.20 - 17:00
// Desc.		:
//=============================================================================
BOOL CXmlEESConfig::Save_XML_String(__in LPCTSTR szFullPath, __in LPCTSTR szXML)
{
	CFile File;
	CFileException e;
	if (!PathFileExists(szFullPath))
	{
		if (!File.Open(szFullPath, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite, &e))
		{
			return FALSE;
		}
	}
	else
	{
		if (!File.Open(szFullPath, CFile::modeWrite | CFile::shareDenyWrite, &e))
		{
			return FALSE;
		}
	}

	CStringA szANSI;
	USES_CONVERSION;
	szANSI = CT2A(szXML);

	File.SeekToEnd();
	//File.Write(szBuff.GetBuffer(0), szBuff.GetLength() * sizeof(TCHAR));
	File.Write(szANSI.GetBuffer(0), szANSI.GetLength());

	File.Flush();
	File.Close();

	return TRUE;
}
BOOL CXmlEESConfig::WriteXML_Header(__in tinyxml2::XMLDocument* IN_pDoc, __in tinyxml2::XMLElement* IN_pRoot)
{
	tinyxml2::XMLDeclaration* decl = IN_pDoc->NewDeclaration();
	IN_pDoc->LinkEndChild(decl);	//Test 2023.02.19a uhkim
	
	
	CStringA szVersion;
	IN_pDoc->LinkEndChild(IN_pRoot);
	
	return TRUE;
}
//=============================================================================
// Method		: WriteXML_Comment
// Access		: protected  
// Returns		: BOOL
// Parameter	: __in tinyxml2::XMLDocument * IN_pDoc
// Parameter	: __out tinyxml2::XMLElement * OUT_pRoot
// Qualifier	:
// Last Update	: 2023.04.20 - 17:00
// Desc.		:
//=============================================================================
BOOL CXmlEESConfig::WriteXML_Comment(__in tinyxml2::XMLDocument* IN_pDoc, __out tinyxml2::XMLElement* OUT_pRoot)
{
	// 주석
	tinyxml2::XMLComment* pComment = IN_pDoc->NewComment("Settings for EES");
	OUT_pRoot->LinkEndChild(pComment);

	return TRUE;
}

//=============================================================================
// Method		: WriteXML_File
// Access		: protected  
// Returns		: BOOL
// Parameter	: __in LPCTSTR szPath
// Parameter	: __in tinyxml2::XMLDocument * IN_pDoc
// Qualifier	:
// Last Update	: 2023.04.20 - 17:00
// Desc.		:
//=============================================================================
BOOL CXmlEESConfig::WriteXML_File(__in LPCTSTR szPath, __in tinyxml2::XMLDocument* IN_pDoc)
{
	// 파일 저장.
	CStringA szFileName;
	szFileName = CT2A(szPath);
	if (tinyxml2::XML_SUCCESS == IN_pDoc->SaveFile(szFileName.GetBuffer(0)))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//=============================================================================
// Method		: Read_LineInfo
// Access		: public  
// Returns		: BOOL
// Parameter	: __in tinyxml2::XMLElement * IN_pRoot
// Parameter	: __out CConfig_Line & OUT_stLineInfo
// Parameter	: __in int IN_iVer
// Qualifier	:
// Last Update	: 2023.04.20 - 17:00
// Desc.		:
//=============================================================================
BOOL CXmlEESConfig::Read_EESInfo(
	__in tinyxml2::XMLElement* IN_pRoot, 
	__out CConfig_EES_LIST& OUT_stInfo)
{
 	CStringA szTemp;
	
	if (IN_pRoot)
	{
		tinyxml2::XMLElement* pSVInfo = IN_pRoot->FirstChildElement("EESInfo");
		if (pSVInfo) {
			tinyxml2::XMLElement* pSVList = pSVInfo->FirstChildElement("EESList");
			if (pSVList)
			{
				OUT_stInfo.RemoveAll();

				INT_PTR iItemCount = pSVList->Int64Attribute("Count", 0);
				std::vector<CConfig_EES> *cEESList;
				
				CConfig_EES* OUT_pEES = NULL;
				for (INT_PTR nIdx = 0; nIdx < iItemCount; nIdx++)
				{
					CConfig_EES stEES;
					OUT_stInfo.EES_Add(stEES);
				}
				cEESList = &OUT_stInfo.EES_LIST;
				for (INT_PTR nIdx = 0; nIdx < iItemCount; nIdx++) {
					OUT_pEES = &cEESList->at(nIdx);
					if (OUT_pEES == nullptr) {
						continue;
					}
					//BoolAttribute
					szTemp.Format("EES_%04d", nIdx+1);
					tinyxml2::XMLElement* pEES = pSVList->FirstChildElement(szTemp.GetBuffer(0));
					if (pEES) {
						szTemp = pEES->Attribute("EquipmentState");
						OUT_pEES->Set_EquipMentStateCode		(CA2T(szTemp.GetBuffer()));
						szTemp = pEES->Attribute("EquipmentStateKor");
						OUT_pEES->Set_EquipMentStateKorText	(CA2T(szTemp.GetBuffer()));
						szTemp = pEES->Attribute("EquipmentStateEng");
						OUT_pEES->Set_EquipMentStateEngText	(CA2T(szTemp.GetBuffer()));
						szTemp = pEES->Attribute("LossCode");
						OUT_pEES->Set_LossCode(CA2T(szTemp.GetBuffer()));
						szTemp = pEES->Attribute("StateColor");
						OUT_pEES->Set_StateColor(CA2T(szTemp.GetBuffer()));
						OUT_pEES->Set_Use(pEES->UnsignedAttribute("Use"));
						szTemp = pEES->Attribute("Explanation");
						OUT_pEES->Set_Explanation(CA2T(szTemp.GetBuffer())); 
					}
				}
			}
			else
			{
				return FALSE;
			}
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

//=============================================================================
// Method		: Write_EESInfo
// Access		: public  
// Returns		: BOOL
// Parameter	: __in tinyxml2::XMLDocument * IN_pDoc
// Parameter	: __in tinyxml2::XMLElement * IN_pRoot
// Parameter	: __in const CConfig_Line * IN_pLineInfo
// Qualifier	:
// Last Update	: 2023.04.20 - 17:00
// Desc.		:
//=============================================================================
BOOL CXmlEESConfig::Write_EESInfo(
	__in tinyxml2::XMLDocument* IN_pDoc, 
	__in tinyxml2::XMLElement* IN_pRoot, 
	__in const CConfig_EES_LIST* IN_pInfo)
{
	CStringA szTemp;

	tinyxml2::XMLElement* pEESInfo = IN_pDoc->NewElement("EESInfo");
	{
		tinyxml2::XMLElement* pEESList = IN_pDoc->NewElement("EESList");
		size_t iItemCount = IN_pInfo->EES_LIST.size();
		pEESList->SetAttribute("Count", (int64_t)iItemCount);

		const CConfig_EES* IN_pEES = NULL;

		for (size_t nIdx = 0; nIdx < iItemCount; nIdx++)
		{
			IN_pEES = &IN_pInfo->EES_LIST.at(nIdx);

			szTemp.Format("EES_%04d", nIdx + 1);
			tinyxml2::XMLElement* pEES = IN_pDoc->NewElement(szTemp.GetBuffer(0));
			{
				szTemp = CT2A(IN_pEES->Get_EquipMentStateCode());
				pEES->SetAttribute("EquipmentState", szTemp);
				szTemp = CT2A(IN_pEES->Get_EquipMentStateKorText());
				pEES->SetAttribute("EquipmentStateKor", szTemp);
				szTemp = CT2A(IN_pEES->Get_EquipMentStateEngText());
				pEES->SetAttribute("EquipmentStateEng", szTemp);
				szTemp = CT2A(IN_pEES->Get_LossCode());
				pEES->SetAttribute("LossCode", szTemp);
				szTemp = CT2A(IN_pEES->Get_StateColor());
				pEES->SetAttribute("StateColor", szTemp);
				pEES->SetAttribute("Use", (unsigned)IN_pEES->Get_Use());
				szTemp = CT2A(IN_pEES->Get_Explanation());
				pEES->SetAttribute("Explanation", szTemp);
			}
			pEESList->LinkEndChild(pEES);
		}
		pEESInfo->LinkEndChild(pEESList);
	}
	IN_pRoot->LinkEndChild(pEESInfo);
	return TRUE;
}

//=============================================================================
// Method		: LoadXML_EESInfo
// Access		: public  
// Returns		: BOOL
// Parameter	: __in LPCTSTR szPath
// Parameter	: __out CConfig_Line & OUT_stLineInfo
// Qualifier	:
// Last Update	: 2023.04.20 - 17:00
// Desc.		:
//=============================================================================
BOOL CXmlEESConfig::LoadXML_EESInfo(__in LPCTSTR szPath, __out CConfig_EES_LIST& OUT_stSVInfo)
{
	if (NULL == szPath)
		return FALSE;

	// 파일이 존재하는가?
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
 		TRACE("LoadXML_EESInfo : Failed to open [%s]\n", szTemp);
 	}
 
 	tinyxml2::XMLDeclaration*	pDecl = ReadDoc.ToDeclaration();
 	tinyxml2::XMLElement*		pRoot = ReadDoc.FirstChildElement("Luritech");
 
	if (FALSE == Read_EESInfo(pRoot, OUT_stSVInfo))
	{
		return FALSE;
	}

	return TRUE;
}

//=============================================================================
// Method		: SaveXML_EESInfo
// Access		: public  
// Returns		: BOOL
// Parameter	: __in LPCTSTR szPath
// Parameter	: __in const CConfig_Line * IN_pLineInfo
// Qualifier	:
// Last Update	: 2023.04.20 - 17:00
// Desc.		:
//=============================================================================
BOOL CXmlEESConfig::SaveXML_EESInfo(__in LPCTSTR szPath, __in const CConfig_EES_LIST* IN_pLineInfo)
{
	tinyxml2::XMLDocument doc;

	USES_CONVERSION;
	tinyxml2::XMLElement* pRoot = doc.NewElement("Luritech");

	WriteXML_Header(&doc, pRoot);	

	// Line Configuration 
	Write_EESInfo(&doc, pRoot, IN_pLineInfo);

	// 파일로 저장
	Backup_File(szPath);

	return WriteXML_File(szPath, &doc);
}