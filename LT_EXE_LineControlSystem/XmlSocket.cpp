//*****************************************************************************
// Filename	: 	XmlSocket.cpp
// Created	:	2021/11/2 - 15:50
// Modified	:	2021/11/2 - 15:50
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#include "stdafx.h"
#include "XmlSocket.h"
#include "CommonFunction.h"

CXmlSocket::CXmlSocket()
{

}

CXmlSocket::~CXmlSocket()
{

}

//=============================================================================
// Method		: Backup_File
// Access		: protected  
// Returns		: void
// Parameter	: __in LPCTSTR szPath
// Qualifier	:
// Last Update	: 2021/11/17 - 19:15
// Desc.		:
//=============================================================================
void CXmlSocket::Backup_File(__in LPCTSTR szPath)
{
	if (m_bUseBackup)
	{
		// 기존 파일명 + 현재 날짜/시간
		// C:\\Recipe\\SEM_T01_SocketInfo.xml
		// => C:\\Recipe\\BAK\\SEM_T01_SocketInfo_2021_1117_181900.xml

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

BOOL CXmlSocket::Save_XML_String(__in LPCTSTR szFullPath, __in LPCTSTR szXML)
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

//=============================================================================
// Method		: ReadXML_Header
// Access		: protected  
// Returns		: BOOL
// Parameter	: __in tinyxml2::XMLElement * IN_pRoot
// Qualifier	:
// Last Update	: 2021/11/11 - 19:59
// Desc.		:
//=============================================================================
BOOL CXmlSocket::ReadXML_Header(__in tinyxml2::XMLElement* IN_pRoot)
{
	if (IN_pRoot)
	{
		IN_pRoot->Attribute("Version");
		IN_pRoot->Attribute("Maker");
		IN_pRoot->Attribute("Format");

		return TRUE;
	} // if (pRoot)
	else
	{
		return FALSE;
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
BOOL CXmlSocket::WriteXML_Header(__in tinyxml2::XMLDocument* IN_pDoc, __in tinyxml2::XMLElement* IN_pRoot)
{
	tinyxml2::XMLDeclaration* decl = IN_pDoc->NewDeclaration();
	IN_pDoc->LinkEndChild(decl);
	
	IN_pRoot->SetAttribute("Version",	"1.0");
	IN_pRoot->SetAttribute("Maker",		"Luritech");
	IN_pRoot->SetAttribute("Format",	"Socket Configuration File"); 
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
// Last Update	: 2021/11/12 - 10:31
// Desc.		:
//=============================================================================
BOOL CXmlSocket::WriteXML_Comment(__in tinyxml2::XMLDocument* IN_pDoc, __out tinyxml2::XMLElement* OUT_pRoot)
{
	// 주석
	tinyxml2::XMLComment* pComment = IN_pDoc->NewComment("Settings for Equipment");
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
// Last Update	: 2021/11/12 - 10:31
// Desc.		:
//=============================================================================
BOOL CXmlSocket::WriteXML_File(__in LPCTSTR szPath, __in tinyxml2::XMLDocument* IN_pDoc)
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
// Method		: Read_SocketInfo
// Access		: public  
// Returns		: BOOL
// Parameter	: __in tinyxml2::XMLElement * IN_pRoot
// Parameter	: __out CSocketList & OUT_stSocketList
// Qualifier	:
// Last Update	: 2021/11/11 - 19:56
// Desc.		:
//=============================================================================
BOOL CXmlSocket::Read_Socket_CfgList(__in tinyxml2::XMLElement* IN_pRoot, __out CSocketList& OUT_stSocketList)
{
 	CStringA szTemp;

	if (IN_pRoot)
	{
		tinyxml2::XMLElement* pSocketInfo = IN_pRoot->FirstChildElement("SocketInfo");
		if (pSocketInfo)
		{
			tinyxml2::XMLElement* pSocketList = pSocketInfo->FirstChildElement("SocketList");
			if (pSocketList)
			{
				// 스텝 정보 초기화
				OUT_stSocketList.RemoveAll();

				// Socket 개수
				INT_PTR iItemCount = pSocketList->Int64Attribute("Count", 0);

				uint8_t nSocketCount = 0;
				for (INT_PTR nIdx = 0; (nSocketCount < iItemCount) && (nIdx < MAX_SOCKET_COUNT); nIdx++)
				{
					// Socket 1개 추가
					CConfig_Socket stSocket;

					szTemp.Format("Socket_%04d", nIdx + 1);
					tinyxml2::XMLElement* pSocketUnit = pSocketList->FirstChildElement(szTemp.GetBuffer(0));
					if (pSocketUnit)
					{
						// CString		szRFID;			// RFID 
						szTemp = pSocketUnit->Attribute("RFID", 0);
						stSocket.szRFID = CA2T(szTemp.GetBuffer());

						// uint8_t		nSocketType;	// Socket Type
						stSocket.nSocketType = pSocketUnit->UnsignedAttribute("Socket_Type", 0);

						// CString		szComment;		// Comment
						szTemp = pSocketUnit->Attribute("Comment", 0);
						stSocket.szComment = CA2T(szTemp.GetBuffer());

						// 해당 RFID가 있으면 추가
						OUT_stSocketList.Socket_Insert(stSocket);
						++nSocketCount;
					} // if (pSocketUnit)

				} // for()
			} // if (pSocketList)
			else
			{
				return FALSE;
			}
		} // if (pSocketInfo)
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
// Method		: Write_SocketInfo
// Access		: public  
// Returns		: BOOL
// Parameter	: __in tinyxml2::XMLDocument * IN_pDoc
// Parameter	: __in tinyxml2::XMLElement * IN_pRoot
// Parameter	: __in const CSocketList * IN_pSocketList
// Qualifier	:
// Last Update	: 2021/11/11 - 19:56
// Desc.		:
//=============================================================================
BOOL CXmlSocket::Write_Socket_CfgList(__in tinyxml2::XMLDocument* IN_pDoc, __in tinyxml2::XMLElement* IN_pRoot, __in const CSocketList* IN_pSocketList)
{
	CStringA szTemp;

	tinyxml2::XMLElement* pSocketInfo = IN_pDoc->NewElement("SocketInfo");
	{
		tinyxml2::XMLElement* pSocketList = IN_pDoc->NewElement("SocketList");
		size_t iItemCount = IN_pSocketList->SocketList.size();
		pSocketList->SetAttribute("Count", (int64_t)iItemCount);

		int nCount = 1;

		auto eIter = IN_pSocketList->SocketList.end();
		auto Iter = IN_pSocketList->SocketList.begin();
		while (Iter != eIter)
		{
			szTemp.Format("Socket_%04d", nCount);
			tinyxml2::XMLElement* pSocketUnit = IN_pDoc->NewElement(szTemp.GetBuffer(0));
			{
				//Iter->second.szRFID;
				szTemp = CT2A(Iter->second.szRFID);
				pSocketUnit->SetAttribute("RFID", szTemp.GetBuffer());
							   
				//Iter->second.nSocketType;
				pSocketUnit->SetAttribute("Socket_Type", (unsigned)Iter->second.nSocketType);

				//Iter->second.szComment;
				szTemp = CT2A(Iter->second.szComment);
				pSocketUnit->SetAttribute("Comment", szTemp.GetBuffer());
			}

			pSocketList->LinkEndChild(pSocketUnit);

			++nCount;

			Iter++;
		}

		pSocketInfo->LinkEndChild(pSocketList);
	}
	IN_pRoot->LinkEndChild(pSocketInfo);

	return TRUE;
}

//=============================================================================
// Method		: LoadXML_SocketInfo
// Access		: public  
// Returns		: BOOL
// Parameter	: __in LPCTSTR szPath
// Parameter	: __out CSocketList & OUT_stSocketList
// Qualifier	:
// Last Update	: 2020/12/28 - 18:12
// Desc.		:
//=============================================================================
BOOL CXmlSocket::LoadXML_Socket_CfgList(__in LPCTSTR szPath, __out CSocketList& OUT_stSocketList)
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
 		TRACE("LoadXML_LineInfo : Failed to open [%s]\n", szTemp);
 	}
 
 	tinyxml2::XMLDeclaration*	pDecl = ReadDoc.ToDeclaration();
 	tinyxml2::XMLElement*		pRoot = ReadDoc.FirstChildElement("Luritech");
 
 	if (FALSE == ReadXML_Header(pRoot))
 	{
 		return FALSE;
 	}

	if (FALSE == Read_Socket_CfgList(pRoot, OUT_stSocketList))
	{
		return FALSE;
	}

	return TRUE;
}

//=============================================================================
// Method		: SaveXML_SocketInfo
// Access		: public  
// Returns		: BOOL
// Parameter	: __in LPCTSTR szPath
// Parameter	: __in const CSocketList * IN_pSocketList
// Qualifier	:
// Last Update	: 2020/12/28 - 18:12
// Desc.		:
//=============================================================================
BOOL CXmlSocket::SaveXML_Socket_CfgList(__in LPCTSTR szPath, __in const CSocketList* IN_pSocketList)
{
	// 기존 파일을 백업하고 저장한다.
	Backup_File(szPath);


	tinyxml2::XMLDocument doc;

	USES_CONVERSION;
	tinyxml2::XMLElement* pRoot = doc.NewElement("Luritech");

	WriteXML_Header(&doc, pRoot);	

	// 주석
	WriteXML_Comment(&doc, pRoot);

	// Line Configuration 
	Write_Socket_CfgList(&doc, pRoot, IN_pSocketList);

	// 파일로 저장.
	return WriteXML_File(szPath, &doc);
}

