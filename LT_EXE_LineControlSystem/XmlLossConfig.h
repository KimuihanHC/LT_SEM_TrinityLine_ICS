//*****************************************************************************
// Filename	: 	XmlLossConfig.h
// Created	:	
// Modified	:	2023.04.20 - 17:00
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#ifndef XmlLossConfig_h__
#define XmlLossConfig_h__

#pragma once

#include "Def_Config_Loss_LIST.h"
#include "tinyxml2.h"

class CXmlLossConfig
{
public:
	CXmlLossConfig();
	~CXmlLossConfig();

protected:

	int		m_xml_version			= 2;

	void	Backup_File				(__in LPCTSTR szPath);

	BOOL	Save_XML_String			(__in LPCTSTR szFullPath, __in LPCTSTR szXML);

	BOOL	WriteXML_Header			(__in tinyxml2::XMLDocument* IN_pDoc, __in tinyxml2::XMLElement* IN_pRoot);
	BOOL	WriteXML_Comment		(__in tinyxml2::XMLDocument* IN_pDoc, __out tinyxml2::XMLElement* OUT_Root);
	BOOL	WriteXML_File			(__in LPCTSTR szPath, __in tinyxml2::XMLDocument* IN_pDoc);
public:
	// Line Configuration
	BOOL	Read_LossInfo			(__in tinyxml2::XMLElement* IN_pRoot, __out CConfig_Loss_LIST& OUT_stInfo);
	BOOL	Write_LossInfo			(__in tinyxml2::XMLDocument* IN_pDoc, __in tinyxml2::XMLElement* IN_pRoot, __in const CConfig_Loss_LIST* IN_pInfo);

	BOOL	LoadXML_LossInfo			(__in LPCTSTR szPath, __out CConfig_Loss_LIST& OUT_stInfo);
	BOOL	SaveXML_LossInfo			(__in LPCTSTR szPath, __in const CConfig_Loss_LIST* IN_pInfo);
};
#endif // XmlLossConfig_h__
