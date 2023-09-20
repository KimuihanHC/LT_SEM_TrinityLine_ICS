//*****************************************************************************
// Filename	: 	File_Recipe.cpp
// Created	:	2021/11/11 - 17:01
// Modified	:	2021/11/11 - 17:01
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#include "stdafx.h"
#include "File_Recipe.h"
#include "CommonFunction.h"
#include "XmlLineConfig.h"

//#include "Def_Enum.h"


#define		Model_AppName			_T("Model")
#define		Common_AppName			_T("Common")

#define		ModelCode_KeyName		_T("ModelCode")


CFile_Recipe::CFile_Recipe()
{
}


CFile_Recipe::~CFile_Recipe()
{
}


//=============================================================================
// Method		: Backup_File
// Access		: protected  
// Returns		: void
// Parameter	: __in LPCTSTR szPath
// Qualifier	:
// Last Update	: 2021/11/17 - 19:12
// Desc.		:
//=============================================================================
void CFile_Recipe::Backup_File(__in LPCTSTR szPath)
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

//=============================================================================
// Method		: Load_RecipeFile
// Access		: public  
// Returns		: BOOL
// Parameter	: __in LPCTSTR szPath
// Parameter	: __out CRecipeInfo & OUT_stRecipeInfo
// Qualifier	:
// Last Update	: 2021/11/11 - 17:00
// Desc.		:
//=============================================================================
BOOL CFile_Recipe::Load_RecipeFile(__in LPCTSTR szPath, __out CRecipeInfo& OUT_stRecipeInfo)
{
	if (NULL == szPath)
		return FALSE;

	// 파일이 존재하는가?
	if (!PathFileExists(szPath))
	{
		return FALSE;
	}

	CXmlLineConfig xml_Config;

	xml_Config.LoadXML_LineInfo(szPath, OUT_stRecipeInfo.LineInfo);
	//xml_Config.LoadXML_LineModelInfo(szPath, OUT_stRecipeInfo.LineInfo, OUT_stRecipeInfo.Model);


	return TRUE;
}

//=============================================================================
// Method		: Save_RecipeFile
// Access		: public  
// Returns		: BOOL
// Parameter	: __in LPCTSTR szPath
// Parameter	: __in const CRecipeInfo * IN_pRecipeInfo
// Qualifier	:
// Last Update	: 2021/11/11 - 17:01
// Desc.		:
//=============================================================================
BOOL CFile_Recipe::Save_RecipeFile(__in LPCTSTR szPath, __in const CRecipeInfo* IN_pRecipeInfo)
{
	if (NULL == szPath)
		return FALSE;

	if (NULL == IN_pRecipeInfo)
		return FALSE;

	// 기존 파일을 백업하고 저장한다.
	Backup_File(szPath);

	::DeleteFile(szPath);

	CXmlLineConfig xml_Config;

	xml_Config.SaveXML_LineInfo(szPath, &IN_pRecipeInfo->LineInfo);
	//xml_Config.SaveXML_LineModelInfo(szPath, &IN_pRecipeInfo->LineInfo, &IN_pRecipeInfo->Model);


	return TRUE;
}

//=============================================================================
// Method		: Load_Common
// Access		: public  
// Returns		: BOOL
// Parameter	: __in LPCTSTR szPath
// Parameter	: __out CRecipeInfo & OUT_stRecipeInfo
// Qualifier	:
// Last Update	: 2021/11/11 - 17:01
// Desc.		:
//=============================================================================
BOOL CFile_Recipe::Load_Common(__in LPCTSTR szPath, __out CRecipeInfo& OUT_stRecipeInfo)
{
	if (NULL == szPath)
		return FALSE;

	TCHAR   inBuff[255] = { 0, };
	DWORD	iRet = 0;

	CString strValue;
	CString strKeyName;

	

	return TRUE;
}

//=============================================================================
// Method		: Save_Common
// Access		: public  
// Returns		: BOOL
// Parameter	: __in LPCTSTR szPath
// Parameter	: __in const CRecipeInfo * IN_pRecipeInfo
// Qualifier	:
// Last Update	: 2021/11/11 - 17:01
// Desc.		:
//=============================================================================
BOOL CFile_Recipe::Save_Common(__in LPCTSTR szPath, __in const CRecipeInfo* IN_pRecipeInfo)
{
	if (NULL == szPath)
		return FALSE;

	CString strValue;
	CString strKeyName;

	// Version, Format
	strValue.Format(_T("%s %s"), SYS_CUSTOMER, g_szInsptrSysType[SET_INSPECTOR]);
	WritePrivateProfileString(_T("VersionInfo"), _T("Equipment"), strValue, szPath);

	strValue.Format(_T("%s"), GetVersionInfo(_T("ProductVersion")));
	WritePrivateProfileString(_T("VersionInfo"), _T("SWVersion"), strValue, szPath);

	strValue.Format(_T("%s"), GetVersionInfo(_T("FileVersion")));
	WritePrivateProfileString(_T("VersionInfo"), _T("Update_Date"), strValue, szPath);


	

	return TRUE;
}

BOOL CFile_Recipe::Load_RecipeXml(__in LPCTSTR szPath, __out CRecipeInfo& OUT_stRecipeInfo)
{
	return TRUE;
}

BOOL CFile_Recipe::Save_RecipeXml(__in LPCTSTR szPath, __in const CRecipeInfo* IN_pRecipelInfo)
{
	return TRUE;
}
