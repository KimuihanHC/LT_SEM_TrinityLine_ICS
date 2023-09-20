//*****************************************************************************
// Filename	: 	File_UserInfo.h
// Created	:	2022/5/18 - 14:24
// Modified	:	2022/5/18 - 14:24
//
// Author	:	piring
//	
// Purpose	:	
//*****************************************************************************
#ifndef File_UserInfo_h__
#define File_UserInfo_h__


#pragma once

#include "Def_UserInfo.h"

class File_UserInfo
{
public:
	File_UserInfo();
	~File_UserInfo();

private:
	BYTE	HexCharToByte		(char cHexChar);
	char	HexCharzToByte		(char cFirstChar, char cSecondChar);
	CString	BufferToHexCString	(BYTE* lpBuff, DWORD nDataCount);
	DWORD	HexCStringToBuffer	(CStringA szHex, BYTE* pOutBuffer, DWORD nBufferSize);

public:
	// 사용자 정보
	bool	Save_UserInfo	(__in LPCTSTR szPath, __in const ST_UserInfo* IN_pUserInfo);
	bool	Load_UserInfo	(__in LPCTSTR szPath, __out ST_UserInfo& OUT_UserInto);

};

#endif // File_UserInfo_h__
