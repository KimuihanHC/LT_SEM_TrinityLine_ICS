//*****************************************************************************
// Filename	: 	XmlSocket.h
// Created	:	2021/11/2 - 15:50
// Modified	:	2021/11/2 - 15:50
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#ifndef XmlSocket_h__
#define XmlSocket_h__

#pragma once

#include "Def_Socket_Config.h"
#include "tinyxml2.h"

class CXmlSocket
{
public:
	CXmlSocket();
	~CXmlSocket();

protected:

	bool	m_bUseBackup			= true;
	void	Backup_File				(__in LPCTSTR szPath);

	BOOL	Save_XML_String			(__in LPCTSTR szFullPath, __in LPCTSTR szXML);

	BOOL	ReadXML_Header			(__in tinyxml2::XMLElement* IN_pRoot);

	BOOL	WriteXML_Header			(__in tinyxml2::XMLDocument* IN_pDoc, __in tinyxml2::XMLElement* IN_pRoot);
	BOOL	WriteXML_Comment		(__in tinyxml2::XMLDocument* IN_pDoc, __out tinyxml2::XMLElement* OUT_Root);
	BOOL	WriteXML_File			(__in LPCTSTR szPath, __in tinyxml2::XMLDocument* IN_pDoc);

	// Socket Setting
	BOOL	Read_Socket_CfgList		(__in tinyxml2::XMLElement* IN_pRoot, __out CSocketList& OUT_stSocketList);
	BOOL	Write_Socket_CfgList	(__in tinyxml2::XMLDocument* IN_pDoc, __in tinyxml2::XMLElement* IN_pRoot, __in const CSocketList* IN_pSocketList);

public:
	
	BOOL	LoadXML_Socket_CfgList	(__in LPCTSTR szPath, __out CSocketList& OUT_stSocketList);
	BOOL	SaveXML_Socket_CfgList	(__in LPCTSTR szPath, __in const CSocketList* IN_pSocketList);

};


#endif // XmlSocket_h__
