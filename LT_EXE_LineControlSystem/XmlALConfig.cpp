//*****************************************************************************
// Filename	: 	XmlALConfig.cpp
// Created	:	
// Modified	:	2023.04.20 - 17:00
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#include "stdafx.h"
#include "XmlALConfig.h"
#include "CommonFunction.h"

CXmlALConfig::CXmlALConfig()
{

}
CXmlALConfig::~CXmlALConfig()
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
void CXmlALConfig::Backup_File(__in LPCTSTR szPath)
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
BOOL CXmlALConfig::Save_XML_String(__in LPCTSTR szFullPath, __in LPCTSTR szXML)
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
	File.Write(szANSI.GetBuffer(0), szANSI.GetLength());

	File.Flush();
	File.Close();

	return TRUE;
}

//=============================================================================
// Method		: WriteXML_Header
// Access		: protected  
// Returns		: BOOL
// Parameter	: __in tinyxml2::XMLDocument * IN_pDoc
// Parameter	: __in tinyxml2::XMLElement * IN_pRoot
// Qualifier	:
// Last Update	: 2023.04.20 - 17:00
// Desc.		:
//=============================================================================
BOOL CXmlALConfig::WriteXML_Header(__in tinyxml2::XMLDocument* IN_pDoc, __in tinyxml2::XMLElement* IN_pRoot)
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
BOOL CXmlALConfig::WriteXML_Comment(__in tinyxml2::XMLDocument* IN_pDoc, __out tinyxml2::XMLElement* OUT_pRoot)
{
	// Ï£ºÏÑù
	tinyxml2::XMLComment* pComment = IN_pDoc->NewComment("Settings for ALID");
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
BOOL CXmlALConfig::WriteXML_File(__in LPCTSTR szPath, __in tinyxml2::XMLDocument* IN_pDoc)
{
	// ?åÏùº ?Ä??
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
// Method		: Read_ALInfo
// Access		: public  
// Returns		: BOOL
// Parameter	: __in tinyxml2::XMLElement * IN_pRoot
// Parameter	: __out CConfig_Line & OUT_stLineInfo
// Parameter	: __in int IN_iVer
// Qualifier	:
// Last Update	: 2023.04.20 - 17:00
// Desc.		:
//=============================================================================
BOOL CXmlALConfig::Read_ALInfo(__in tinyxml2::XMLElement* IN_pRoot, __out CConfig_AL_LIST& OUT_stInfo)
{
 	CStringA szTemp;	
	if (IN_pRoot)
	{
		tinyxml2::XMLElement* pALInfo = IN_pRoot->FirstChildElement("ALInfo");
		if (pALInfo) {
			tinyxml2::XMLElement* pALList = pALInfo->FirstChildElement("ALList");
			if (pALList)
			{
				OUT_stInfo.RemoveAll();

				INT_PTR iItemCount = pALList->Int64Attribute("Count", 0);
				std::vector<CConfig_AL> *cALList;
				
				CConfig_AL* OUT_pAL = NULL;
				for (INT_PTR nIdx = 0; nIdx < iItemCount; nIdx++)
				{
					CConfig_AL stAL;
					OUT_stInfo.AL_Add(stAL);
				}
				cALList = &OUT_stInfo.AL_LIST;
				for (INT_PTR nIdx = 0; nIdx < iItemCount; nIdx++) {
					OUT_pAL = &cALList->at(nIdx);
					if (OUT_pAL == nullptr) {
						continue;
					}
					szTemp.Format("AL_%04d", nIdx+1);
					tinyxml2::XMLElement* pSV = pALList->FirstChildElement(szTemp.GetBuffer(0));
					if (pSV) {
						OUT_pAL->Set_ALID(pSV->UnsignedAttribute("ALID", 0));
						OUT_pAL->Set_ALCD(pSV->UnsignedAttribute("ALCD", 0));
						szTemp = pSV->Attribute("ALTX");
						OUT_pAL->Set_ALTX(CA2T(szTemp.GetBuffer()));
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
	} // if (IN_pRoot)
	else
	{
		return FALSE;
	}

	return TRUE;
}

//=============================================================================
// Method		: Write_ALInfo
// Access		: public  
// Returns		: BOOL
// Parameter	: __in tinyxml2::XMLDocument * IN_pDoc
// Parameter	: __in tinyxml2::XMLElement * IN_pRoot
// Parameter	: __in const CConfig_Line * IN_pLineInfo
// Qualifier	:
// Last Update	: 2023.04.20 - 17:00
// Desc.		:
//=============================================================================
BOOL CXmlALConfig::Write_ALInfo(__in tinyxml2::XMLDocument* IN_pDoc, __in tinyxml2::XMLElement* IN_pRoot, __in const CConfig_AL_LIST* IN_pSVInfo)
{
	CStringA szTemp;
	tinyxml2::XMLElement* pALInfo = IN_pDoc->NewElement("ALInfo");
	{
		tinyxml2::XMLElement* pALList = IN_pDoc->NewElement("ALList");
		size_t iItemCount = IN_pSVInfo->AL_LIST.size();
		pALList->SetAttribute("Count", (int64_t)iItemCount);

		const CConfig_AL* IN_pAL = NULL;

		for (size_t nIdx = 0; nIdx < iItemCount; nIdx++)
		{
			IN_pAL = &IN_pSVInfo->AL_LIST.at(nIdx);

			szTemp.Format("AL_%04d", nIdx + 1);
			tinyxml2::XMLElement* pAL = IN_pDoc->NewElement(szTemp.GetBuffer(0));
			{
				pAL->SetAttribute("ALID", (unsigned)IN_pAL->Get_ALID());

				pAL->SetAttribute("ALCD", (unsigned)IN_pAL->Get_ALCD());

				szTemp = CT2A(IN_pAL->Get_ALTX());

				pAL->SetAttribute("ALTX", szTemp);
			}
			pALList->LinkEndChild(pAL);
		}
		pALInfo->LinkEndChild(pALList);

	}
	IN_pRoot->LinkEndChild(pALInfo);

	return TRUE;
}

//=============================================================================
// Method		: LoadXML_ALInfo
// Access		: public  
// Returns		: BOOL
// Parameter	: __in LPCTSTR szPath
// Parameter	: __out CConfig_Line & OUT_stLineInfo
// Qualifier	:
// Last Update	: 2023.04.20 - 17:00
// Desc.		:
//=============================================================================
BOOL CXmlALConfig::LoadXML_ALInfo(__in LPCTSTR szPath, __out CConfig_AL_LIST& OUT_stSVInfo)
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
 		TRACE("LoadXML_ALInfo : Failed to open [%s]\n", szTemp);
 	}
 
 	tinyxml2::XMLDeclaration*	pDecl = ReadDoc.ToDeclaration();
 	tinyxml2::XMLElement*		pRoot = ReadDoc.FirstChildElement("Luritech");
 
	if (FALSE == Read_ALInfo(pRoot, OUT_stSVInfo))
	{
		return FALSE;
	}

	return TRUE;
}

//=============================================================================
// Method		: SaveXML_ALInfo
// Access		: public  
// Returns		: BOOL
// Parameter	: __in LPCTSTR szPath
// Parameter	: __in const CConfig_Line * IN_pLineInfo
// Qualifier	:
// Last Update	: 2023.04.20 - 17:00
// Desc.		:
//=============================================================================
BOOL CXmlALConfig::SaveXML_ALInfo(__in LPCTSTR szPath, __in const CConfig_AL_LIST* IN_pLineInfo)
{
	tinyxml2::XMLDocument doc;

	USES_CONVERSION;
	tinyxml2::XMLElement* pRoot = doc.NewElement("Luritech");

	WriteXML_Header(&doc, pRoot);	
	Write_ALInfo(&doc, pRoot, IN_pLineInfo);

	// ?åÏùºÎ°??Ä??	Backup_File(szPath);

	return WriteXML_File(szPath, &doc);
}