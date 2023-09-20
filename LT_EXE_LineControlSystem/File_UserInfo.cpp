//*****************************************************************************
// Filename	: 	File_UserInfo.cpp
// Created	:	2022/5/18 - 16:35
// Modified	:	2022/5/18 - 16:35
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#include "stdafx.h"
#include "File_UserInfo.h"

#define USE_CRYPT

#ifdef USE_CRYPT
#include "FileCrypt.h"
#endif


File_UserInfo::File_UserInfo()
{

}

File_UserInfo::~File_UserInfo()
{

}

BYTE File_UserInfo::HexCharToByte(char cHexChar)
{
	char cValue = (char)toupper(cHexChar);
	BYTE cAsciiValue;

	if (('0' <= cValue) && (cValue <= '9'))
	{
		cAsciiValue = cValue - '0';
	}
	else if (('A' <= cValue) && (cValue <= 'F'))
	{
		cAsciiValue = (cValue - 'A') + 10;
	}
	else
	{
		cAsciiValue = 0xFF;
	}

	return cAsciiValue;
}

char File_UserInfo::HexCharzToByte(char cFirstChar, char cSecondChar)
{
	char cResult = 0;
	char cAsciiValue[2] = { 0, 0 };

	cAsciiValue[0] = HexCharToByte(cFirstChar);
	cAsciiValue[1] = HexCharToByte(cSecondChar);

	cResult = cResult | (cAsciiValue[0] << 4);
	cResult = cResult | (cAsciiValue[1] & 0x0F);

	return cResult;
}

CString File_UserInfo::BufferToHexCString(BYTE* lpBuff, DWORD nDataCount)
{
	ASSERT(NULL != lpBuff);

	CString strTemp;
	CString strResult;
	unsigned char* szHexString;
	szHexString = new unsigned char[nDataCount];
	memset(szHexString, 0, nDataCount);
	memcpy_s(szHexString, nDataCount, (unsigned char*)lpBuff, nDataCount);

	for (UINT nCnt = 0; nCnt < nDataCount; ++nCnt)
	{
		if (szHexString[nCnt] < 0x0F)
			strTemp.Format(_T("%02X"), szHexString[nCnt]);
		else
			strTemp.Format(_T("%02X"), szHexString[nCnt]);
		strResult = strResult + strTemp;
	}

	delete[] szHexString;

	return strResult;
}

DWORD File_UserInfo::HexCStringToBuffer(CStringA szHex, BYTE* pOutBuffer, DWORD nBufferSize)
{
	// BufferToHexCString로 변환된 스트링을 원래대로 다시 변환
	int nDataCount = szHex.GetLength();

	BYTE cResult = 0;
	BYTE cAsciiValue[2] = { 0, 0 };
	DWORD dwCount = 0;
	for (int nCnt = 0; (nCnt < nDataCount) && (dwCount <= nBufferSize); ++dwCount)
	{
		cAsciiValue[0] = HexCharToByte(szHex.GetAt(nCnt++));
		cAsciiValue[1] = HexCharToByte(szHex.GetAt(nCnt++));

		cResult = cAsciiValue[0] << 4;
		cResult = cResult | (cAsciiValue[1] & 0x0F);

		pOutBuffer[dwCount] = cResult;
	}


	return dwCount;
}

//=============================================================================
// Method		: Save_UserInfo
// Access		: private  
// Returns		: bool
// Parameter	: __in LPCTSTR szPath
// Parameter	: __in const ST_UserInfo * IN_pUserInfo
// Qualifier	:
// Last Update	: 2022/5/18 - 14:26
// Desc.		:
//=============================================================================
bool File_UserInfo::Save_UserInfo(__in LPCTSTR szPath, __in const ST_UserInfo* IN_pUserInfo)
{
	if (NULL == szPath)
		return false;

	::DeleteFile(szPath);

	CString szValue;
	CString szAppName;
	CString szKeyName;
	DWORD	dwValue = 0;

	// User Count
	szAppName = _T("User_Count");
	szValue.Format(_T("%d"), static_cast<DWORD>(IN_pUserInfo->m_Users.size()));
	WritePrivateProfileString(szAppName, _T("User_Count"), szValue, szPath);

	BYTE	pDataBuffer[4095] = {0, };
	DWORD	dwBufferLen = 4095;
	DWORD	dwDataCount = 0;

	for (auto nIdx = 0; nIdx < IN_pUserInfo->m_Users.size(); ++nIdx)
	{
		szAppName.Format(_T("UserInfo_%03d"), nIdx);

		// ID
		szValue = IN_pUserInfo->m_Users.at(nIdx).szID;
		WritePrivateProfileString(szAppName, _T("ID"), szValue, szPath);

		// Name
		szValue = IN_pUserInfo->m_Users.at(nIdx).szName;
		WritePrivateProfileString(szAppName, _T("Name"), szValue, szPath);

		// Authority
		szValue.Format(_T("%d"), IN_pUserInfo->m_Users.at(nIdx).nAuthority);
		WritePrivateProfileString(szAppName, _T("Authority"), szValue, szPath);

		// Password 
		CStringA szValue_a;
		USES_CONVERSION;
		szValue = IN_pUserInfo->m_Users.at(nIdx).szPassword;
		szValue_a = CT2A(szValue.GetBuffer());
		dwDataCount = szValue_a.GetLength() * sizeof(CHAR);
#ifdef USE_CRYPT // (암호화 처리)
		dwBufferLen = dwDataCount;
		memcpy(pDataBuffer, szValue_a.GetBuffer(), dwDataCount);
		if (EncryptData(pDataBuffer, &dwDataCount, dwBufferLen))
		{
			szValue = BufferToHexCString(pDataBuffer, dwDataCount);
		}
		else
		{
			szValue.Empty();
		}
#else

		szValue = BufferToHexCString((BYTE*)szValue_a.GetBuffer(), dwDataCount);

#endif
		WritePrivateProfileString(szAppName, _T("Password"), szValue, szPath);


		// Password Length
// 		szValue.Format(_T("%d"), IN_pUserInfo->m_Users.at(nIdx).szPassword.GetLength());
// 		WritePrivateProfileString(szAppName, _T("Password_Leng"), szValue, szPath);
	}

	return true;
}

//=============================================================================
// Method		: Load_UserInfo
// Access		: private  
// Returns		: bool
// Parameter	: __in LPCTSTR szPath
// Parameter	: __out ST_UserInfo & OUT_UserInto
// Qualifier	:
// Last Update	: 2022/5/18 - 14:26
// Desc.		:
//=============================================================================
bool File_UserInfo::Load_UserInfo(__in LPCTSTR szPath, __out ST_UserInfo& OUT_UserInto)
{
	// 파일이 존재하는가?
	if (!PathFileExists(szPath))
	{
		return false;
	}

	// 유저 목록 초기화
	OUT_UserInto.Reset();

	TCHAR   inBuff[1025] = { 0, };
	CString szValue;
	CString szAppName;
	CString szKeyName;
	DWORD	dwValue = 0;
	
	// User Count
	szAppName = _T("User_Count");
	GetPrivateProfileString(szAppName, _T("User_Count"), _T("0"), inBuff, 80, szPath);
	DWORD dwUserCount = static_cast<DWORD>(_ttoi(inBuff));


	BYTE	pDataBuffer[4095] = { 0, };
	DWORD	dwBufferLen = 4095;
	DWORD	dwDataCount = 0;
	CStringA szANSI;

	for (DWORD nIdx = 0; nIdx < dwUserCount; ++nIdx)
	{
		szAppName.Format(_T("UserInfo_%03d"), nIdx);

		ST_User_Unit stUser;

		// ID
		GetPrivateProfileString(szAppName, _T("ID"), _T(""), inBuff, 1024, szPath);
		stUser.szID = inBuff;

		// Name
		GetPrivateProfileString(szAppName, _T("Name"), _T(""), inBuff, 1024, szPath);
		stUser.szName = inBuff;

		// Authority
		GetPrivateProfileString(szAppName, _T("NG_Section"), _T("0"), inBuff, 80, szPath);
		stUser.nAuthority = static_cast<uint8_t>(_ttoi(inBuff));

		// Password Length
// 		GetPrivateProfileString(szAppName, _T("Password_Leng"), _T("0"), inBuff, 80, szPath);
// 		DWORD dwLen = static_cast<DWORD>(_ttoi(inBuff));

		// Password 
		bool bSuceed = false;
		if (0 < GetPrivateProfileString(szAppName, _T("Password"), _T(""), inBuff, 1024, szPath))
		{
			USES_CONVERSION;
			szANSI = CT2A(inBuff);

			// Hex String ==> Hex Data
			dwDataCount = HexCStringToBuffer(szANSI, pDataBuffer, dwBufferLen);
			if (0 < dwDataCount)
			{
#ifdef USE_CRYPT // (복호화 처리)
				if (DecryptData(pDataBuffer, dwBufferLen, &dwDataCount))
#endif
				{
					pDataBuffer[dwDataCount] = 0x00;
					//stUser.szPassword = (TCHAR*)pDataBuffer;
					stUser.szPassword = CA2T((CHAR*)pDataBuffer);

					bSuceed = true;
				}
			}
		}

		if (false == bSuceed)
		{
			stUser.szPassword.Empty();
		}

		// 데이터 추가
		OUT_UserInto.m_Users.push_back(stUser);
	}

	// *** 기본 유저 정보를 추가한다.
	OUT_UserInto.Set_Default_Users();

	return true;
}
