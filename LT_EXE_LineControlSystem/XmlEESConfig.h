//*****************************************************************************
// Filename	: 	XmlEESConfig.h
// Created	:	
// Modified	:	2023.04.20 - 17:00
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#ifndef XmlEESConfig_h__
#define XmlEESConfig_h__

#pragma once

#include "Def_Config_EES_LIST.h"
#include "tinyxml2.h"

class CXmlEESConfig
{
public:
	CXmlEESConfig();
	~CXmlEESConfig();

protected:

	int		m_xml_version			= 2;

	void	Backup_File				(__in LPCTSTR szPath);

	BOOL	Save_XML_String			(__in LPCTSTR szFullPath, __in LPCTSTR szXML);

	BOOL	WriteXML_Header			(__in tinyxml2::XMLDocument* IN_pDoc, __in tinyxml2::XMLElement* IN_pRoot);
	BOOL	WriteXML_Comment		(__in tinyxml2::XMLDocument* IN_pDoc, __out tinyxml2::XMLElement* OUT_Root);
	BOOL	WriteXML_File			(__in LPCTSTR szPath, __in tinyxml2::XMLDocument* IN_pDoc);
public:
	// Line Configuration
	BOOL	Read_EESInfo			(__in tinyxml2::XMLElement* IN_pRoot, __out CConfig_EES_LIST& OUT_stSVInfo);
	BOOL	Write_EESInfo			(__in tinyxml2::XMLDocument* IN_pDoc, __in tinyxml2::XMLElement* IN_pRoot, __in const CConfig_EES_LIST* IN_pSVInfo);

	BOOL	LoadXML_EESInfo			(__in LPCTSTR szPath, __out CConfig_EES_LIST& OUT_stSVInfo);
	BOOL	SaveXML_EESInfo			(__in LPCTSTR szPath, __in const CConfig_EES_LIST* IN_pSVInfo);
};
#endif // XmlEESConfig_h__
