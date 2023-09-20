//*****************************************************************************
// Filename	: 	Def_RecipeInfo_Cm.h
// Created	:	2021/1/25 - 13:49
// Modified	:	2021/1/25 - 13:49
//
// Author	:	piring
//	
// Purpose	:	
//*****************************************************************************
#ifndef Def_RecipeInfo_Cm_h__
#define Def_RecipeInfo_Cm_h__


#pragma once

#pragma pack(push, 1)


//---------------------------------------------------------
// 레시피 설정값 구조체
//---------------------------------------------------------
class CRecipeInfo_Base
{
public:
	CString			szRecipe_FileName;	// 레시피 설정 파일 타이틀
	CString			szRecipe_FullPath;	// 레시피 파일 Full Path

	CRecipeInfo_Base()
	{
	};

	CRecipeInfo_Base& operator= (CRecipeInfo_Base& ref)
	{
		szRecipe_FileName	= ref.szRecipe_FileName;
		szRecipe_FullPath	= ref.szRecipe_FullPath;

		return *this;
	};

	virtual void Reset()
	{
		szRecipe_FileName.Empty();
		szRecipe_FullPath.Empty();

	};

};



#pragma pack (pop)

#endif // Def_RecipeInfo_Cm_h__
