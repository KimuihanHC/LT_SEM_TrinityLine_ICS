//*****************************************************************************
// Filename	: 	File_Config.h
// Created	:	2016/12/14 - 17:51
// Modified	:	2016/12/14 - 17:51
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
#ifndef File_Config_h__
#define File_Config_h__

#pragma once

class CFile_Config
{
public:
	CFile_Config();
	~CFile_Config();

	// Model
	BOOL	Load_Model		(__in LPCTSTR szPath, __out CString& szModel);
	BOOL	Save_Model		(__in LPCTSTR szPath, __in LPCTSTR szModel);

	BOOL	Load_ModelFile	(__in LPCTSTR szPath, __out CString& szModelFile);
	BOOL	Save_ModelFile	(__in LPCTSTR szPath, __in LPCTSTR szModelFile);
	
	// EQP Model
	// Model
	// Recipe File



};

#endif // File_Config_h__

