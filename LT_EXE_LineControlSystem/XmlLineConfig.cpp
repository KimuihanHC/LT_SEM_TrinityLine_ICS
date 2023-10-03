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
#include "XmlLineConfig.h"
#include "CommonFunction.h"

CXmlLineConfig::CXmlLineConfig()
{

}

CXmlLineConfig::~CXmlLineConfig()
{

}
//=============================================================================
// Method		: Backup_File
// Access		: protected  
// Returns		: void
// Parameter	: __in LPCTSTR szPath
// Qualifier	:
// Last Update	: 2022/1/5 - 18:20
// Desc.		:
//=============================================================================
void CXmlLineConfig::Backup_File(__in LPCTSTR szPath)
{
	//if (m_bUseBackup)
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

//=============================================================================
// Method		: Convert_EqpType_UI
// Access		: protected  
// Returns		: uint8_t
// Parameter	: __in int IN_iVer
// Parameter	: __in uint8_t IN_nEqpTypeUI_Old
// Qualifier	:
// Last Update	: 2022/10/21 - 12:23
// Desc.		:
//=============================================================================
uint8_t CXmlLineConfig::Convert_EqpType_UI(__in int IN_iVer, __in uint8_t IN_nEqpTypeUI_Old)
{
	if (1 < m_xml_version)
	{
#if (SET_INSPECTOR == SYS_ICS_RIVIAN_LINE)
		return IN_nEqpTypeUI_Old;
#else
		switch (IN_iVer)
		{
		case 1: // version 1.0 => 2.0
			//return (IN_nEqpType_Old + 1); // Eqp_Handler가 0번 인덱스에 추가됨
			switch (IN_nEqpTypeUI_Old)
			{
			case Eqp_V1_Loader:			// 0
				return enEquipmentType_UI::EqpUI_Loader;
				break;

			case Eqp_V1_Returner:		// 1
				return enEquipmentType_UI::EqpUI_Returner;
				break;

			case Eqp_V1_ColorCal:		// 2 
				return enEquipmentType_UI::EqpUI_ColorCal;
				break;

			case Eqp_V1_SFR_CL_46:		// 3 : SFR 협각
				return enEquipmentType_UI::EqpUI_SFR_CL_46;
				break;

			case Eqp_V1_SFR_MultiCL_83:	// 4 : 83
				return enEquipmentType_UI::EqpUI_SFR_MultiCL_83;
				break;

			case Eqp_V1_SFR_MultiCL_180:	// 5 : 180
				return enEquipmentType_UI::EqpUI_SFR_MultiCL_180;
				break;

			case Eqp_V1_HotPixel:		// 6
				return enEquipmentType_UI::EqpUI_HotPixel;
				break;

			case Eqp_V1_StrayLight:		// 7 Ghost & Flare
				return enEquipmentType_UI::EqpUI_StrayLight;
				break;

			case Eqp_V1_Distortion:		// 8
				return enEquipmentType_UI::EqpUI_Distortion;
				break;

			case Eqp_V1_SFR_MultiCL:		// 9 : SFR 愿묎컖
				return enEquipmentType_UI::EqpUI_SFR_MultiCL;
				break;

			case Eqp_V1_HotPixel3port:	// 10 : Hot Pixel 3 Para
				return enEquipmentType_UI::EqpUI_HotPixel3port;
				break;
			//2023.01.29a uhkim [Test]
			case Eqp_V1_ServerEes:
				return enEquipmentType_UI::EqpUI_Ees;
				break;
			default:
				return IN_nEqpTypeUI_Old;
				break;
			}

			break;

		default:
			return IN_nEqpTypeUI_Old;
			break;
		}
#endif // (SET_INSPECTOR == SYS_ICS_RIVIAN_LINE)
	}

	return IN_nEqpTypeUI_Old;
}

//=============================================================================
// Method		: Convert_EquipmentType
// Access		: protected  
// Returns		: uint8_t
// Parameter	: __in int IN_iVer
// Parameter	: __in uint8_t IN_nEqpType_Old
// Qualifier	:
// Last Update	: 2022/8/12 - 15:39
// Desc.		:
//=============================================================================
uint8_t CXmlLineConfig::Convert_EquipmentType(__in int IN_iVer, __in uint8_t IN_nEqpType_Old)
{
	if (1 < m_xml_version)
	{
#if (SET_INSPECTOR == SYS_ICS_RIVIAN_LINE)
		return IN_nEqpType_Old;
#else
		switch (IN_iVer)
		{
		case 1: // version 1.0 => 2.0
			//return (IN_nEqpType_Old + 1); // Eqp_Handler가 0번 인덱스에 추가됨
			switch (IN_nEqpType_Old)
			{
			case Eqp_V1_Loader:			// 0
				return enEquipmentType::Eqp_Loader;
				break;

			case Eqp_V1_Returner:		// 1
				return enEquipmentType::Eqp_Returner;
				break;

			case Eqp_V1_ColorCal:		// 2 
				return enEquipmentType::Eqp_ColorCal;
				break;

			case Eqp_V1_SFR_CL_46:		// 3 : SFR 협각
				return enEquipmentType::Eqp_SFR_CL_46;
				break;

			case Eqp_V1_SFR_MultiCL_83:	// 4 : 83
				return enEquipmentType::Eqp_SFR_MultiCL_83;
				break;

			case Eqp_V1_SFR_MultiCL_180:	// 5 : 180
				return enEquipmentType::Eqp_SFR_MultiCL_180;
				break;

			case Eqp_V1_HotPixel:		// 6
				return enEquipmentType::Eqp_HotPixel;
				break;

			case Eqp_V1_StrayLight:		// 7 Ghost & Flare
				return enEquipmentType::Eqp_StrayLight;
				break;

			case Eqp_V1_Distortion:		// 8
				return enEquipmentType::Eqp_Distortion;
				break;

			case Eqp_V1_SFR_MultiCL:		// 9 : SFR 愿묎컖
				return enEquipmentType::Eqp_SFR_MultiCL;
				break;

			case Eqp_V1_HotPixel3port:	// 10 : Hot Pixel 3 Para
				//return enEquipmentType::Eqp_HotPixel3port;
				return enEquipmentType::Eqp_HotPixel;
				break;

			default:
				return IN_nEqpType_Old;
				break;
			}
			break;
		default:
			return IN_nEqpType_Old;
			break;
		}
#endif // (SET_INSPECTOR == SYS_ICS_RIVIAN_LINE)
	}
	
	return IN_nEqpType_Old;
}

//=============================================================================
// Method		: Save_XML_String
// Access		: protected  
// Returns		: BOOL
// Parameter	: __in LPCTSTR szFullPath
// Parameter	: __in LPCTSTR szXML
// Qualifier	:
// Last Update	: 2022/1/5 - 18:20
// Desc.		:
//=============================================================================
BOOL CXmlLineConfig::Save_XML_String(__in LPCTSTR szFullPath, __in LPCTSTR szXML)
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
// Returns		: int		=> xml version
// Parameter	: __in tinyxml2::XMLElement * IN_pRoot
// Qualifier	:
// Last Update	: 2021/11/11 - 19:59
// Desc.		:
//=============================================================================
int CXmlLineConfig::ReadXML_Header(__in tinyxml2::XMLElement* IN_pRoot)
{
	if (IN_pRoot)
	{
		IN_pRoot->Attribute("Version"); // ver2.0 에서 eqp type이 바뀜 호환 체크해아 함
		IN_pRoot->Attribute("Maker");
		IN_pRoot->Attribute("Format");

		CString szTemp;
		szTemp = IN_pRoot->Attribute("Version", 0);
		int iVer = _ttoi(szTemp.GetBuffer());

		return iVer;
	} // if (pRoot)
	else
	{
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
BOOL CXmlLineConfig::WriteXML_Header(__in tinyxml2::XMLDocument* IN_pDoc, __in tinyxml2::XMLElement* IN_pRoot)
{
	tinyxml2::XMLDeclaration* decl = IN_pDoc->NewDeclaration();
	IN_pDoc->LinkEndChild(decl);	
	
	CStringA szVersion;
	szVersion.Format("%d.0", m_xml_version);	
	//IN_pRoot->SetAttribute("Version",	"1.0"); // ver2.0 에서 eqp type이 바뀜 호환 체크해아 함
	IN_pRoot->SetAttribute("Version", szVersion.GetBuffer()); // ver2.0 에서 eqp type이 바뀜 호환 체크해아 함
	IN_pRoot->SetAttribute("Maker", "Luritech");
	IN_pRoot->SetAttribute("Format", "Line Configuration File");	
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
BOOL CXmlLineConfig::WriteXML_Comment(__in tinyxml2::XMLDocument* IN_pDoc, __out tinyxml2::XMLElement* OUT_pRoot)
{
	// 二쇱꽍
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
BOOL CXmlLineConfig::WriteXML_File(__in LPCTSTR szPath, __in tinyxml2::XMLDocument* IN_pDoc)
{
	// 파일 저장
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
// Last Update	: 2023.05.08
// Desc.		:
//=============================================================================
BOOL CXmlLineConfig::Read_LineInfo(__in tinyxml2::XMLElement* IN_pRoot, __out CConfig_Line& OUT_stLineInfo, __in int IN_iVer)
{
	CStringA szTemp;

	if (IN_pRoot)
	{
		tinyxml2::XMLElement* pLineInfo = IN_pRoot->FirstChildElement("LineInfo");
		if (pLineInfo)
		{
			tinyxml2::XMLElement* pEqpList = pLineInfo->FirstChildElement("EqpList");
			if (pEqpList)
			{
				// 라인 설비 정보 초기화			
				OUT_stLineInfo.RemoveAll();

				// 설비 개수
				INT_PTR iItemCount = pEqpList->Int64Attribute("Count", 0);

				CConfig_Eqp* OUT_pEqp = NULL;
				for (INT_PTR nIdx = 0; nIdx < iItemCount; nIdx++)
				{
					// 설비 1개 추가
					CConfig_Eqp stEquipment;
					OUT_stLineInfo.Eqp_Add(stEquipment);
			
					OUT_pEqp = &OUT_stLineInfo.EqpList.at(nIdx);

					szTemp.Format("Eqp_%03d", nIdx + 1);
					tinyxml2::XMLElement* pEquipment = pEqpList->FirstChildElement(szTemp.GetBuffer(0));
					if (pEquipment)
					{
						// uint8_t		m_nEqpOrder;		// 라인에서의 설비 순서(번호)
						OUT_pEqp->Set_EqpOrder(pEquipment->UnsignedAttribute("EqpOrder", 0));

						// bool			m_bGroup;			// 검사 그룹 (?)
						OUT_pEqp->Set_Group(pEquipment->BoolAttribute("Group", false));

						// uint8_t		m_nGroupIndex;		// 검사 그룹내 설비 인덱스					
						OUT_pEqp->Set_GroupIndex(pEquipment->UnsignedAttribute("GroupIndex", 0));

						// CString		szAlias;			// Eqp Type + 검사기별 Number (설비 순서에 따른 자동 생성)
						szTemp = pEquipment->Attribute("Alias", 0);
						OUT_pEqp->Set_Alias(CA2T(szTemp.GetBuffer()));

						// uint8_t		nEquipmenttype;		// Equipmnet Type
						//OUT_pEqp->Set_EquipmentType(Convert_EquipmentType(IN_iVer, pEquipment->UnsignedAttribute("Equipment_Type", 0)));
						OUT_pEqp->Set_EqpType_UI(Convert_EqpType_UI(IN_iVer, pEquipment->UnsignedAttribute("Equipment_Type", 0)));

						// CString		szEquipmentId;		// Equipment id (怨좎쑀id)
						szTemp = pEquipment->Attribute("Equipment_ID", 0);
						OUT_pEqp->Set_EquipmentId(CA2T(szTemp.GetBuffer()));

						// uint32_t		nIP_Address;		// ip address
						szTemp = pEquipment->Attribute("IP_Address", 0);
						OUT_pEqp->Set_IP_Address(ntohl(inet_addr(szTemp.GetBuffer())));

						// uint8_t		m_nPortCount;;		// Port Count
						//OUT_pEqp->m_nPortCount = pEquipment->UnsignedAttribute("Port_Count", 1);
						// uint8_t		nConveyorCount;		// Conveyor Count
						//OUT_pEqp->m_nConveyorCount = pEquipment->UnsignedAttribute("Conveyor_Count", 1);
						// uint8_t		m_nRFID_Count;			// RFID 리더 갯수

					} // if (pEquipment)
				} // for()
			} // if (pEqpList)
			else
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

//=============================================================================
// Method		: Write_LineInfo
// Access		: public  
// Returns		: BOOL
// Parameter	: __in tinyxml2::XMLDocument * IN_pDoc
// Parameter	: __in tinyxml2::XMLElement * IN_pRoot
// Parameter	: __in const CConfig_Line * IN_pLineInfo
// Qualifier	:
// Last Update	: 2021/11/11 - 19:56
// Desc.		:
//=============================================================================
BOOL CXmlLineConfig::Write_LineInfo(__in tinyxml2::XMLDocument* IN_pDoc, __in tinyxml2::XMLElement* IN_pRoot, __in const CConfig_Line* IN_pLineInfo)
{
	CStringA szTemp;

	tinyxml2::XMLElement* pLineInfo = IN_pDoc->NewElement("LineInfo");
	{
		tinyxml2::XMLElement* pEqpList = IN_pDoc->NewElement("EqpList");
		size_t iItemCount = IN_pLineInfo->EqpList.size();
		pEqpList->SetAttribute("Count", (int64_t)iItemCount);

		const CConfig_Eqp* IN_pEqp = NULL;

		for (size_t nIdx = 0; nIdx < iItemCount; nIdx++)
		{
			IN_pEqp = &IN_pLineInfo->EqpList.at(nIdx);

			szTemp.Format("Eqp_%03d", nIdx + 1);
			tinyxml2::XMLElement* pEquipment = IN_pDoc->NewElement(szTemp.GetBuffer(0));
			{
				// uint8_t		m_nEqpOrder;		// 라인에서의 설비 순서(번호)
				pEquipment->SetAttribute("EqpOrder", (unsigned)IN_pEqp->Get_EqpOrder());
				// bool			m_bGroup;			// 검사 그룹 (?)
				pEquipment->SetAttribute("Group", IN_pEqp->Get_Group());
				// uint8_t		m_nGroupIndex;		// 검사 그룹내 설비 인덱스			
				pEquipment->SetAttribute("GroupIndex", (unsigned)IN_pEqp->Get_GroupIndex());

				// CString		szAlias;			// Eqp Type + 검사기별 Number (설비 순서에 따른 자동 생성)
				szTemp = CT2A(IN_pEqp->Get_Alias());
				pEquipment->SetAttribute("Alias", szTemp.GetBuffer());

				// uint8_t		nEquipmenttype;		// Equipmnet Type
				//pEquipment->SetAttribute("Equipment_Type", (unsigned)IN_pEqp->Get_EquipmentType());
				pEquipment->SetAttribute("Equipment_Type", (unsigned)IN_pEqp->Get_EqpType_UI());

				// CString		szEquipmentId;		// Equipment id (怨좎쑀id)
				szTemp = CT2A(IN_pEqp->Get_EquipmentId());
				pEquipment->SetAttribute("Equipment_ID", szTemp.GetBuffer());

				// uint32_t		nIP_Address;		// ip address
				DWORD dwAddress = ntohl(IN_pEqp->Get_IP_Address());
				//szTemp = inet_ntoa(*(IN_ADDR*)&dwAddress);
				szTemp.Format("%d.%d.%d.%d", FOURTH_IPADDRESS(dwAddress), THIRD_IPADDRESS(dwAddress), SECOND_IPADDRESS(dwAddress), FIRST_IPADDRESS(dwAddress));
				pEquipment->SetAttribute("IP_Address", szTemp.GetBuffer());


				// uint8_t		m_nPortCount;		// Buffer Count
				//pEquipment->SetAttribute("Port_Count", (unsigned)IN_pEqp->m_nPortCount);
				// uint8_t		nConveyorCount;		// Conveyor Count
				//pEquipment->SetAttribute("Conveyor_Count", (unsigned)IN_pEqp->m_nConveyorCount);
				// uint8_t		m_nRFID_Count;			// RFID 리더 갯수
			}

			pEqpList->LinkEndChild(pEquipment);
		}

		pLineInfo->LinkEndChild(pEqpList);
	}
	IN_pRoot->LinkEndChild(pLineInfo);

	return TRUE;
}

//=============================================================================
// Method		: LoadXML_StepInfo
// Access		: public  
// Returns		: BOOL
// Parameter	: __in LPCTSTR szPath
// Parameter	: __out CConfig_Line & OUT_stLineInfo
// Qualifier	:
// Last Update	: 2020/12/28 - 18:12
// Desc.		:
//=============================================================================
BOOL CXmlLineConfig::LoadXML_LineInfo(__in LPCTSTR szPath, __out CConfig_Line& OUT_stLineInfo)
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
 
	int iVer = ReadXML_Header(pRoot);
	if (iVer <= 0)
 	{
 		return FALSE;
 	}

	if (FALSE == Read_LineInfo(pRoot, OUT_stLineInfo, iVer))
	{
		return FALSE;
	}

	return TRUE;
}

//=============================================================================
// Method		: SaveXML_StepInfo
// Access		: public  
// Returns		: BOOL
// Parameter	: __in LPCTSTR szPath
// Parameter	: __in const CConfig_Line * IN_pLineInfo
// Qualifier	:
// Last Update	: 2020/12/28 - 18:12
// Desc.		:
//=============================================================================
BOOL CXmlLineConfig::SaveXML_LineInfo(__in LPCTSTR szPath, __in const CConfig_Line* IN_pLineInfo)
{
	tinyxml2::XMLDocument doc;

	USES_CONVERSION;
	tinyxml2::XMLElement* pRoot = doc.NewElement("Luritech");

	WriteXML_Header(&doc, pRoot);	

	// 二쇱꽍
	WriteXML_Comment(&doc, pRoot);

	// Line Configuration 
	Write_LineInfo(&doc, pRoot, IN_pLineInfo);

	// 파일로 저장	
	Backup_File(szPath);
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
BOOL CXmlLineConfig::Read_ModelInfo(__in tinyxml2::XMLElement* IN_pRoot, __out CConfig_Model& OUT_stModelInfo)
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

		// Tester 사용여부
		tinyxml2::XMLElement* pTesterList = pModelInfo->FirstChildElement("Tester_List");
		if (pTesterList)
		{
			// Tester 醫낅쪟 媛쒖닔
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

		// Tester 사용여부
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
}

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
BOOL CXmlLineConfig::LoadXML_ModelInfo(__in LPCTSTR szPath, __out CConfig_Model& OUT_stModelInfo)
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
BOOL CXmlLineConfig::SaveXML_ModelInfo(__in LPCTSTR szPath, __in const CConfig_Model* IN_pModelInfo)
{
	tinyxml2::XMLDocument doc;

	USES_CONVERSION;
	tinyxml2::XMLElement* pRoot = doc.NewElement("Luritech");

	WriteXML_Header(&doc, pRoot);

	// 二쇱꽍
	WriteXML_Comment(&doc, pRoot);

	// Model Configuration 
	Write_ModelInfo(&doc, pRoot, IN_pModelInfo);

	// 파일로 저장
	Backup_File(szPath);
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

	int iVer = ReadXML_Header(pRoot);
	if (iVer <= 0)
	{
		return FALSE;
	}

	// version 확인 해야 함

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

	// 二쇱꽍
	WriteXML_Comment(&doc, pRoot);

	// Line Configuration 
	Write_LineInfo(&doc, pRoot, IN_pLineInfo);

	// Model Configuration 
	Write_ModelInfo(&doc, pRoot, IN_pModelInfo);

	// 파일로 저장
	Backup_File(szPath);
	return WriteXML_File(szPath, &doc);
}
