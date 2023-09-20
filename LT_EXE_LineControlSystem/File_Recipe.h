//*****************************************************************************
// Filename	: 	File_Recipe.h
// Created	:	2021/11/11 - 17:01
// Modified	:	2021/11/11 - 17:01
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#ifndef File_Recipe_h__
#define File_Recipe_h__

#pragma once

#include "Def_DataStruct.h"


class CFile_Recipe
{
public:
	CFile_Recipe();
	~CFile_Recipe();

protected:

	bool	m_bUseBackup		= true;
	void	Backup_File			(__in LPCTSTR szPath);

public:
	BOOL	Load_RecipeFile		(__in LPCTSTR szPath, __out CRecipeInfo& OUT_stRecipeInfo);
	BOOL	Save_RecipeFile		(__in LPCTSTR szPath, __in const CRecipeInfo* IN_pRecipeInfo);

	BOOL	Load_Common			(__in LPCTSTR szPath, __out CRecipeInfo& OUT_stRecipeInfo);
	BOOL	Save_Common			(__in LPCTSTR szPath, __in const CRecipeInfo* IN_pRecipeInfo);

	
	BOOL	Load_RecipeXml		(__in LPCTSTR szPath, __out CRecipeInfo& OUT_stRecipeInfo);
	BOOL	Save_RecipeXml		(__in LPCTSTR szPath, __in const CRecipeInfo* IN_pRecipelInfo);


};

#endif // File_Recipe_h__
