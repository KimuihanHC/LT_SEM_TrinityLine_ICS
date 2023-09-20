//*****************************************************************************
// Filename	: 	XmlLossConfig.cpp
// Created	:	
// Modified	:	2023.04.20 - 17:00
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#include "stdafx.h"
#include "XmlLossConfig.h"
#include "CommonFunction.h"

CXmlLossConfig::CXmlLossConfig()
{

}
CXmlLossConfig::~CXmlLossConfig()
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
void CXmlLossConfig::Backup_File(__in LPCTSTR szPath)
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
BOOL CXmlLossConfig::Save_XML_String(__in LPCTSTR szFullPath, __in LPCTSTR szXML)
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
BOOL CXmlLossConfig::WriteXML_Header(__in tinyxml2::XMLDocument* IN_pDoc, __in tinyxml2::XMLElement* IN_pRoot)
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
BOOL CXmlLossConfig::WriteXML_Comment(__in tinyxml2::XMLDocument* IN_pDoc, __out tinyxml2::XMLElement* OUT_pRoot)
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
BOOL CXmlLossConfig::WriteXML_File(__in LPCTSTR szPath, __in tinyxml2::XMLDocument* IN_pDoc)
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
// Parameter	: __out CConfig_Line & OUT_stInfo
// Parameter	: __in int IN_iVer
// Qualifier	:
// Last Update	: 2023.04.20 - 17:00
// Desc.		:
//=============================================================================
BOOL CXmlLossConfig::Read_LossInfo(
	__in tinyxml2::XMLElement* IN_pRoot, 
	__out CConfig_Loss_LIST& OUT_stInfo)
{
 	CStringA szTemp;
	
	if (IN_pRoot)
	{
		tinyxml2::XMLElement* pInfo = IN_pRoot->FirstChildElement("LossInfo");
		if (pInfo) {
			tinyxml2::XMLElement* pList = pInfo->FirstChildElement("LossList");
			if (pList)
			{
				OUT_stInfo.RemoveAll();

				INT_PTR iItemCount = pList->Int64Attribute("Count", 0);
				std::vector<CConfig_Loss> *cList;
				
				CConfig_Loss* OUT_p = NULL;
				for (INT_PTR nIdx = 0; nIdx < iItemCount; nIdx++)
				{
					CConfig_Loss st;
					OUT_stInfo.Loss_Add(st);
				}
				cList = &OUT_stInfo.Loss_LIST;
				for (INT_PTR nIdx = 0; nIdx < iItemCount; nIdx++) {
					OUT_p = &cList->at(nIdx);
					if (OUT_p == nullptr) {
						continue;
					}
					//BoolAttribute
					szTemp.Format("Loss_%04d", nIdx+1);
					tinyxml2::XMLElement* p = pList->FirstChildElement(szTemp.GetBuffer(0));
					if (p) {
						szTemp = p->Attribute("NameLoss");
						OUT_p->Set_NameLoss(CA2T(szTemp.GetBuffer()));
						szTemp = p->Attribute("Tem");
						OUT_p->Set_Tem(CA2T(szTemp.GetBuffer()));
						szTemp = p->Attribute("CodeLoss");
						OUT_p->Set_CodeLoss(CA2T(szTemp.GetBuffer()));
						szTemp = p->Attribute("NameLossEng");
						OUT_p->Set_NameLossEng(CA2T(szTemp.GetBuffer()));
						szTemp = p->Attribute("Explanation");
						OUT_p->Set_Explain(CA2T(szTemp.GetBuffer()));
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
// Method		: Write_LossInfo
// Access		: public  
// Returns		: BOOL
// Parameter	: __in tinyxml2::XMLDocument * IN_pDoc
// Parameter	: __in tinyxml2::XMLElement * IN_pRoot
// Parameter	: __in const CConfig_Line * IN_pInfo
// Qualifier	:
// Last Update	: 2023.04.20 - 17:00
// Desc.		:
//=============================================================================
BOOL CXmlLossConfig::Write_LossInfo(
	__in tinyxml2::XMLDocument* IN_pDoc, 
	__in tinyxml2::XMLElement* IN_pRoot, 
	__in const CConfig_Loss_LIST* IN_pInfo)
{
	CStringA szTemp;

	tinyxml2::XMLElement* pInfo = IN_pDoc->NewElement("LossInfo");
	{
		tinyxml2::XMLElement* pList = IN_pDoc->NewElement("LossList");
		size_t iItemCount = IN_pInfo->Loss_LIST.size();
		pList->SetAttribute("Count", (int64_t)iItemCount);

		const CConfig_Loss* IN_p = NULL;

		for (size_t nIdx = 0; nIdx < iItemCount; nIdx++)
		{
			IN_p = &IN_pInfo->Loss_LIST.at(nIdx);

			szTemp.Format("Loss_%04d", nIdx + 1);
			tinyxml2::XMLElement* p = IN_pDoc->NewElement(szTemp.GetBuffer(0));
			{
				szTemp = CT2A(IN_p->Get_NameLoss());
				p->SetAttribute("NameLoss", szTemp);
				szTemp = CT2A(IN_p->Get_Tem());
				p->SetAttribute("Tem", szTemp);
				szTemp = CT2A(IN_p->Get_CodeLoss());
				p->SetAttribute("CodeLoss", szTemp);
				szTemp = CT2A(IN_p->Get_NameLossEng());
				p->SetAttribute("NameLossEng", szTemp);
				szTemp = CT2A(IN_p->Get_Explain());
				p->SetAttribute("Explanation", szTemp);
			}
			pList->LinkEndChild(p);
		}
		pInfo->LinkEndChild(pList);
	}
	IN_pRoot->LinkEndChild(pInfo);
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
BOOL CXmlLossConfig::LoadXML_LossInfo(__in LPCTSTR szPath, __out CConfig_Loss_LIST& OUT_stInfo)
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
 
	if (FALSE == Read_LossInfo(pRoot, OUT_stInfo))
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
BOOL CXmlLossConfig::SaveXML_LossInfo(__in LPCTSTR szPath, __in const CConfig_Loss_LIST* IN_pInfo)
{
	tinyxml2::XMLDocument doc;

	USES_CONVERSION;
	tinyxml2::XMLElement* pRoot = doc.NewElement("Luritech");

	WriteXML_Header(&doc, pRoot);	

	// Line Configuration 
	Write_LossInfo(&doc, pRoot, IN_pInfo);

	// 파일로 저장
	Backup_File(szPath);

	return WriteXML_File(szPath, &doc);
}