
//*****************************************************************************
// Filename	: 	Def_Language.h
// Created	:	2022/7/18 - 14:27
// Modified	:	2022/7/18 - 14:27
//
// Author	:	piring 
//	
// Purpose	:	
//*****************************************************************************
#ifndef Def_Language_h__
#define Def_Language_h__


#pragma once

#include <tchar.h>
#include <windows.h>

typedef enum
{
	Lang_Korean,
	Lang_English,
	Lang_Vietnamese,
	Lang_Chinese,

	Lang_MaxCount,
}enLanguage;

static LPCTSTR g_szLanguage[] =
{
	_T("한국어 (Korean)"),
	_T("English"),
	_T("Tiếng Việt (Vietnamese)"),
	_T("中國語 (Chinese)"),
	nullptr
};

static BYTE Get_CharSet(uint8_t IN_nLanguage)
{
	switch (IN_nLanguage)
	{
	case Lang_Korean:
		return HANGUL_CHARSET;
		break;

	case Lang_English:
		return ANSI_CHARSET;
		break;

	case Lang_Vietnamese:
		return VIETNAMESE_CHARSET;
		break;

	case Lang_Chinese:
		return CHINESEBIG5_CHARSET;
		break;

	default:
		break;
	}

	return ANSI_CHARSET;
};

#endif // Def_Language_h__