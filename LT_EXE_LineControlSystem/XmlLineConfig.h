//*****************************************************************************
// Filename	: 	XmlLineConfig.h
// Created	:	2021/11/2 - 15:50
// Modified	:	2021/11/2 - 15:50
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#ifndef LineConfig_h__
#define LineConfig_h__

#pragma once

#include "Def_ConfigLine.h"
#include "Def_ModelConfig.h"
#include "tinyxml2.h"

class CXmlLineConfig
{
public:
	CXmlLineConfig();
	~CXmlLineConfig();

	
protected:

	int		m_xml_version			= 2;

	void	Backup_File				(__in LPCTSTR szPath);

	// version Check : 구버전 파일을 읽어 올 때 처리
	uint8_t Convert_EquipmentType	(__in int IN_iVer, __in uint8_t IN_nEqpType_Old);
	uint8_t Convert_EqpType_UI		(__in int IN_iVer, __in uint8_t IN_nEqpTypeUI_Old);

	BOOL	Save_XML_String			(__in LPCTSTR szFullPath, __in LPCTSTR szXML);

	int		ReadXML_Header			(__in tinyxml2::XMLElement* IN_pRoot);

	BOOL	WriteXML_Header			(__in tinyxml2::XMLDocument* IN_pDoc, __in tinyxml2::XMLElement* IN_pRoot);
	BOOL	WriteXML_Comment		(__in tinyxml2::XMLDocument* IN_pDoc, __out tinyxml2::XMLElement* OUT_Root);
	BOOL	WriteXML_File			(__in LPCTSTR szPath, __in tinyxml2::XMLDocument* IN_pDoc);

public:
	// Line Configuration
	BOOL	Read_LineInfo			(__in tinyxml2::XMLElement* IN_pRoot, __out CConfig_Line& OUT_stLineInfo, __in int IN_iVer);
	BOOL	Write_LineInfo			(__in tinyxml2::XMLDocument* IN_pDoc, __in tinyxml2::XMLElement* IN_pRoot, __in const CConfig_Line* IN_pLineInfo);

	BOOL	LoadXML_LineInfo		(__in LPCTSTR szPath, __out CConfig_Line& OUT_stLineInfo);
	BOOL	SaveXML_LineInfo		(__in LPCTSTR szPath, __in const CConfig_Line* IN_pLineInfo);

	// Model Configration
	BOOL	Read_ModelInfo			(__in tinyxml2::XMLElement* IN_pRoot, __out CConfig_Model& OUT_stModelInfo);
	BOOL	Write_ModelInfo			(__in tinyxml2::XMLDocument* IN_pDoc, __in tinyxml2::XMLElement* IN_pRoot, __in const CConfig_Model* IN_pModelInfo);

	BOOL	LoadXML_ModelInfo		(__in LPCTSTR szPath, __out CConfig_Model& OUT_stModelInfo);
	BOOL	SaveXML_ModelInfo		(__in LPCTSTR szPath, __in const CConfig_Model* IN_pModelInfo);

	
	// 레시피 처리용도
	BOOL	LoadXML_LineModelInfo	(__in LPCTSTR szPath, __out CConfig_Line& OUT_stLineInfo, __out CConfig_Model& OUT_stModelInfo);
	BOOL	SaveXML_LineModelInfo	(__in LPCTSTR szPath, __in const CConfig_Line* IN_pLineInfo, __in const CConfig_Model* IN_pModelInfo);


	

};


#endif // LineConfig_h__
