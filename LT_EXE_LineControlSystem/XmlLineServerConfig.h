//*****************************************************************************
// Filename	: 	XmlLineConfig.h
// Created	:	2021/11/2 - 15:50
// Modified	:	2021/11/2 - 15:50
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#ifndef LineServerConfig_h__
#define LineServerConfig_h__

#pragma once

#include "Def_ConfigLineServer.h"
#include "Def_ModelConfig.h"
#include "tinyxml2.h"

class CXmlLineServerConfig
{
public:
	CXmlLineServerConfig();
	~CXmlLineServerConfig();
protected:
	void	Backup_File				(__in LPCTSTR szPath);

	uint8_t Convert_EquipmentType	(__in int IN_iVer, __in uint8_t IN_nEqpType_Old);
	uint8_t Convert_SvrType_UI		(__in int IN_iVer, __in uint8_t IN_nEqpTypeUI_Old);

	BOOL	Save_XML_String			(__in LPCTSTR szFullPath, __in LPCTSTR szXML);

	int		ReadXML_Header			(__in tinyxml2::XMLElement* IN_pRoot);

	BOOL	WriteXML_Header			(__in tinyxml2::XMLDocument* IN_pDoc, __in tinyxml2::XMLElement* IN_pRoot);
	BOOL	WriteXML_Comment		(__in tinyxml2::XMLDocument* IN_pDoc, __out tinyxml2::XMLElement* OUT_Root);
	BOOL	WriteXML_File			(__in LPCTSTR szPath, __in tinyxml2::XMLDocument* IN_pDoc);

public:
	// Line Configuration
	BOOL	Read_ServerInfo			(__in tinyxml2::XMLElement* IN_pRoot, __out CConfig_Server& OUT_stLineInfo);
	BOOL	Write_ServerInfo		(__in tinyxml2::XMLDocument* IN_pDoc, __in tinyxml2::XMLElement* IN_pRoot, __in const CConfig_Server* IN_pLineInfo);

	BOOL	LoadXML_ServerInfo		(__in LPCTSTR szPath, __out CConfig_Server& OUT_stLineInfo);
	BOOL	SaveXML_ServerInfo		(__in LPCTSTR szPath, __in const CConfig_Server* IN_pLineInfo);

	// Model Configration
	//BOOL	Read_ModelInfo			(__in tinyxml2::XMLElement* IN_pRoot, __out CConfig_Model& OUT_stModelInfo);
	//BOOL	Write_ModelInfo			(__in tinyxml2::XMLDocument* IN_pDoc, __in tinyxml2::XMLElement* IN_pRoot, __in const CConfig_Model* IN_pModelInfo);

	//BOOL	LoadXML_ModelInfo		(__in LPCTSTR szPath, __out CConfig_Model& OUT_stModelInfo);
	//BOOL	SaveXML_ModelInfo		(__in LPCTSTR szPath, __in const CConfig_Model* IN_pModelInfo);

	
	// ?àÏãú??Ï≤òÎ¶¨?©ÎèÑ
	//BOOL	LoadXML_LineModelInfo	(__in LPCTSTR szPath, __out CConfig_Server& OUT_stLineInfo, __out CConfig_Model& OUT_stModelInfo);
	//BOOL	SaveXML_LineModelInfo	(__in LPCTSTR szPath, __in const CConfig_Server* IN_pLineInfo, __in const CConfig_Model* IN_pModelInfo);

	//?úÎ≤Ñ ?Ä?•Ïö©.
	//BOOL	LoadXML_LineServer(__in LPCTSTR szPath, __out CConfig_Server& OUT_stLineInfo);
	//BOOL	SaveXML_LineServer(__in LPCTSTR szPath, __in const CConfig_Server* IN_pLineInfo);
	

};


#endif // LineConfig_h__
