//*****************************************************************************
// Filename	: 	File_Config.cpp
// Created	:	2016/12/14 - 17:51
// Modified	:	2016/12/14 - 17:51
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
#include "stdafx.h"
#include "File_Config.h"

#define		CONFIG_FILE		_T("config.ini")

CFile_Config::CFile_Config()
{
}


CFile_Config::~CFile_Config()
{
}

//=============================================================================
// Method		: Load_Model
// Access		: public  
// Returns		: BOOL
// Parameter	: __in LPCTSTR szPath
// Parameter	: __out CString & szModel
// Qualifier	:
// Last Update	: 2016/12/14 - 18:18
// Desc.		:
//=============================================================================
BOOL CFile_Config::Load_Model(__in LPCTSTR szPath, __out CString& szModel)
{
	BOOL bReturn(TRUE);
	if (NULL == szPath)
		return FALSE;

	// 파일이 존재하는가?
	if (!PathFileExists(szPath))
	{
		return FALSE;
	}

	CString szFullPath;
	TCHAR   inBuff[1024] = { 0, };
	
	szFullPath.Format(_T("%s\\%s"), szPath, CONFIG_FILE);
	
	// 모델명
	GetPrivateProfileString(_T("System"), _T("ModelName"), _T("Default"), inBuff, 80, szFullPath);
	szModel = inBuff;

	return TRUE;
}

//=============================================================================
// Method		: Save_Model
// Access		: public  
// Returns		: BOOL
// Parameter	: __in LPCTSTR szPath
// Parameter	: __in LPCTSTR szModel
// Qualifier	:
// Last Update	: 2016/12/14 - 18:18
// Desc.		:
//=============================================================================
BOOL CFile_Config::Save_Model(__in LPCTSTR szPath, __in LPCTSTR szModel)
{
	if (NULL == szPath)
		return FALSE;

	if (NULL == szModel)
		return FALSE;

	::DeleteFile(szPath);

	BOOL bReturn = TRUE;

	CString szFullPath;
	CString strValue;

	szFullPath.Format(_T("%s\\%s"), szPath, CONFIG_FILE);

	// 모델명
	strValue = szModel;
	return WritePrivateProfileString(_T("System"), _T("ModelName"), strValue, szFullPath);
}

//=============================================================================
// Method		: Load_ModelFile
// Access		: public  
// Returns		: BOOL
// Parameter	: __in LPCTSTR szPath
// Parameter	: __out CString & szModelFile
// Qualifier	:
// Last Update	: 2016/12/14 - 19:36
// Desc.		:
//=============================================================================
BOOL CFile_Config::Load_ModelFile(__in LPCTSTR szPath, __out CString& szModelFile)
{
	BOOL bReturn(TRUE);
	if (NULL == szPath)
		return FALSE;

	// 파일이 존재하는가?
	if (!PathFileExists(szPath))
	{
		return FALSE;
	}

	CString szFullPath;
	TCHAR   inBuff[1024] = { 0, };

	szFullPath.Format(_T("%s\\%s"), szPath, CONFIG_FILE);

	// 모델명
	GetPrivateProfileString(_T("System"), _T("ModelFile"), _T("Default"), inBuff, 80, szFullPath);
	szModelFile = inBuff;

	return TRUE;
}

//=============================================================================
// Method		: Save_ModelFile
// Access		: public  
// Returns		: BOOL
// Parameter	: __in LPCTSTR szPath
// Parameter	: __in LPCTSTR szModelFile
// Qualifier	:
// Last Update	: 2016/12/14 - 19:36
// Desc.		:
//=============================================================================
BOOL CFile_Config::Save_ModelFile(__in LPCTSTR szPath, __in LPCTSTR szModelFile)
{
	if (NULL == szPath)
		return FALSE;

	if (NULL == szModelFile)
		return FALSE;

	::DeleteFile(szPath);

	BOOL bReturn = TRUE;

	CString szFullPath;
	CString strValue;

	szFullPath.Format(_T("%s\\%s"), szPath, CONFIG_FILE);

	// 모델명
	strValue = szModelFile;
	return WritePrivateProfileString(_T("System"), _T("ModelFile"), strValue, szFullPath);
}

