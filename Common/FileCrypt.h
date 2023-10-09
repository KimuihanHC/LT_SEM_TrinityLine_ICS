//*****************************************************************************
// Filename	: FileCrypt.h
// Created	: 2013/1/21
// Modified	: 2013/1/21 - 17:08
//
// Author	: PiRing
//	
// Purpose	: 
//*****************************************************************************
#ifndef FileCrypt_h__
#define FileCrypt_h__

#include <io.h>				// 파일관련.
#include <WinCrypt.h>		// 암호화.
#include "Define_HWInfoFile.h"

#define KEYLENGTH				0x00800000
#define ENCRYPT_ALGORITHM		CALG_RC4 
#define ENCRYPT_BLOCK_SIZE		8 	
#define TEXT_CRYPT_PASSWORD		TEXT("Luritech_TDD")


static bool EncryptData	(__inout BYTE *pbBuffer, __inout  DWORD* pdwCount, __in DWORD dwBufferLen);
static BOOL DecryptData	(__inout BYTE *pbBuffer, __in DWORD dwBufferLen, __out DWORD* pdwCount);
static BOOL WriteHWIFile(__in const ST_HW_INFO_FILE* stHWIFormat, __in LPCTSTR lpFileName);
static BOOL ReadHWIFile (__out ST_HW_INFO_FILE* stHWIFormat, __in LPCTSTR lpFileName);
static BOOL WriteHWIReg	(__in const ST_HW_INFO_FILE* stHWIFormat);
static BOOL ReadHWIReg	(__out ST_HW_INFO_FILE* stHWIFormat);

//=============================================================================
// Method		: EncryptData
// Access		: public static 
// Returns		: bool
// Parameter	: __inout BYTE * pbBuffer
// Parameter	: __inout DWORD * pdwCount
// Parameter	: __in DWORD dwBufferLen
// Qualifier	:
// Last Update	: 2013/1/23 - 10:45
// Desc.		:
//=============================================================================
static bool EncryptData(__inout BYTE* pbBuffer, __inout DWORD* pdwCount, __in DWORD dwBufferLen)
{ 
	bool		fReturn		= false;

	HCRYPTPROV	hCryptProv	= NULL;
	HCRYPTKEY	hKey		= NULL;
	HCRYPTKEY	hXchgKey	= NULL;
	HCRYPTHASH	hHash		= NULL;
	LPTSTR		pszPassword = TEXT_CRYPT_PASSWORD;

	//---------------------------------------------------------------
	// Get the handle to the default provider. 
	BOOL bReturn = FALSE;

	bReturn = CryptAcquireContext(&hCryptProv, NULL, MS_ENHANCED_PROV, PROV_RSA_FULL, 0);
	if (FALSE == bReturn)
	{
		bReturn = CryptAcquireContext(&hCryptProv, NULL, MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_NEWKEYSET);
	}

	if (bReturn)
	{
		OutputDebugString(TEXT("A cryptographic provider has been acquired. \n"));
		//---------------------------------------------------------------
		// Create the session key.	
		//-----------------------------------------------------------
		// Create a hash object. 
		if(CryptCreateHash(hCryptProv,CALG_MD5, 0, 0, &hHash))
		{
			OutputDebugString(TEXT("A hash object has been created. \n"));
			//-----------------------------------------------------------
			// Hash the password. 
			if(CryptHashData(hHash, (BYTE *)pszPassword, lstrlen(pszPassword), 0))
			{
				OutputDebugString(TEXT("The password has been added to the hash. \n"));
				//-----------------------------------------------------------
				// Derive a session key from the hash object. 
				if(CryptDeriveKey(hCryptProv, ENCRYPT_ALGORITHM, hHash, KEYLENGTH, &hKey))
				{
					OutputDebugString(TEXT("An encryption key is derived from the ")TEXT("password hash. \n")); 

					if(CryptEncrypt(hKey, NULL, TRUE, 0, pbBuffer, pdwCount, dwBufferLen))
					{
						fReturn = true;
					}
					else
					{ 
						OutputDebugString(TEXT("Error during CryptEncrypt.\n")); 							
					}	
				}
				else
				{
					OutputDebugString(TEXT("Error during CryptDeriveKey!\n")); 
				}
			}
			else
			{
				OutputDebugString(TEXT("Error during CryptHashData. \n")); 
			}
		}
		else
		{ 
			OutputDebugString(TEXT("Error during CryptCreateHash!\n"));
		}  
	}
	else
	{
		OutputDebugString(TEXT("Error during CryptAcquireContext!\n"));
	}

	//-----------------------------------------------------------
	// Release the hash object. 
	if(hHash) 
	{
		if(!(CryptDestroyHash(hHash)))
		{
			OutputDebugString(TEXT("Error during CryptDestroyHash.\n")); 
		}

		hHash = NULL;
	}

	//---------------------------------------------------------------
	// Release the session key. 
	if(hKey)
	{
		if(!(CryptDestroyKey(hKey)))
		{
			OutputDebugString(TEXT("Error during CryptDestroyKey!\n"));
		}
	}

	//---------------------------------------------------------------
	// Release the provider handle. 
	if(hCryptProv)
	{
		if(!(CryptReleaseContext(hCryptProv, 0)))
		{
			OutputDebugString(TEXT("Error during CryptReleaseContext!\n"));
		}
	}

	return fReturn; 
};

//=============================================================================
// Method		: DecryptData
// Access		: public static 
// Returns		: BOOL
// Parameter	: __inout BYTE * pbBuffer
// Parameter	: __in DWORD dwBufferLen
// Parameter	: __out DWORD * pdwCount
// Qualifier	:
// Last Update	: 2013/1/23 - 10:45
// Desc.		:
//=============================================================================
static BOOL DecryptData(__inout BYTE *pbBuffer, __in DWORD dwBufferLen, __out DWORD* pdwCount)
{ 
	bool		fReturn		= false;

	HCRYPTKEY	hKey		= NULL; 
	HCRYPTHASH	hHash		= NULL; 
	HCRYPTPROV	hCryptProv	= NULL; 		
	LPTSTR		pszPassword	= TEXT_CRYPT_PASSWORD;

	//---------------------------------------------------------------
	// Get the handle to the default provider. 
	if(CryptAcquireContext(&hCryptProv, NULL, MS_ENHANCED_PROV, PROV_RSA_FULL, 0))
	{
		OutputDebugString(TEXT("A cryptographic provider has been acquired. \n"));
		//---------------------------------------------------------------
		// Create the session key.
		//-----------------------------------------------------------
		// Create a hash object. 
		if(CryptCreateHash(hCryptProv, CALG_MD5, 0, 0, &hHash))
		{
			//-----------------------------------------------------------
			// Hash in the password data. 
			if(CryptHashData(hHash, (BYTE *)pszPassword, lstrlen(pszPassword), 0))
			{
				//-----------------------------------------------------------
				// Derive a session key from the hash object. 
				if(CryptDeriveKey(hCryptProv, ENCRYPT_ALGORITHM, hHash, KEYLENGTH, &hKey))
				{
					//-----------------------------------------------------------
					// Decrypt the block of data. 
					if(CryptDecrypt(hKey, 0, TRUE, 0, pbBuffer, pdwCount))
					{
						fReturn = true;
					}
					else
					{
						OutputDebugString(TEXT("Error during CryptDecrypt!\n")); 
					}
				}
				else
				{ 
					OutputDebugString(TEXT("Error during CryptDeriveKey!\n")) ; 
				}
			}
			else
			{
				OutputDebugString(TEXT("Error during CryptHashData!\n")); 
			}
		}
		else
		{
			OutputDebugString(TEXT("Error during CryptCreateHash!\n"));
		}
	}
	else
	{
		OutputDebugString(TEXT("Error during CryptAcquireContext!\n"));
	}

	//-----------------------------------------------------------
	// Release the hash object. 
	if(hHash) 
	{
		if(!(CryptDestroyHash(hHash)))
		{
			OutputDebugString(TEXT("Error during CryptDestroyHash.\n")); 
		}

		hHash = NULL;
	}

	//---------------------------------------------------------------
	// Release the session key. 
	if(hKey)
	{
		if(!(CryptDestroyKey(hKey)))
		{
			OutputDebugString(TEXT("Error during CryptDestroyKey!\n"));
		}
	} 

	//---------------------------------------------------------------
	// Release the provider handle. 
	if(hCryptProv)
	{
		if(!(CryptReleaseContext(hCryptProv, 0)))
		{
			OutputDebugString(TEXT("Error during CryptReleaseContext!\n"));
		}
	} 

	return fReturn;
}

//=============================================================================
// Method		: WriteHWIFile
// Access		: public static 
// Returns		: BOOL
// Parameter	: __in const ST_HW_INFO_FILE * stHWIFormat
// Parameter	: __in LPCTSTR lpFileName
// Qualifier	:
// Last Update	: 2013/1/22 - 15:23
// Desc.		:
//=============================================================================
static BOOL WriteHWIFile (__in const ST_HW_INFO_FILE* stHWIFormat, __in LPCTSTR lpFileName)
{
	BOOL	bReturn			= FALSE;
	HANDLE	hFile			= NULL; 	
	DWORD	dwBytesToWrite	= sizeof (ST_HW_INFO_FILE);
	DWORD	dwBytesWritten	= 0;
	DWORD	dwDataLen		= dwBytesToWrite;	
	BYTE*	pDataBuffer		= new BYTE[dwBytesToWrite];

	memcpy (pDataBuffer, (BYTE*)stHWIFormat, dwBytesToWrite);

	if (EncryptData (pDataBuffer, &dwDataLen, dwBytesToWrite))
	{
		hFile = CreateFile(	lpFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		if (hFile != INVALID_HANDLE_VALUE)
		{
			OutputDebugString (TEXT("Writing Datas to File\n"));

			while (dwBytesWritten < dwBytesToWrite)
			{
				if( FALSE == WriteFile(	hFile, pDataBuffer + dwBytesWritten, dwBytesToWrite - dwBytesWritten, &dwBytesWritten, NULL))
				{
					OutputDebugString (TEXT("Could not write to file\n"));
					CloseHandle(hFile);
					delete [] pDataBuffer;
					return FALSE;
				}
			}

			OutputDebugString (TEXT("Wrote Datas to File successfully.\n"));
			CloseHandle(hFile);
			bReturn	= TRUE;
		}
		else
		{ 
			OutputDebugString (TEXT("Could not open file (error %d)\n"));
		}
	}
	else
	{
		OutputDebugString (TEXT("Data Encode Error!"));		
	}
	
	delete [] pDataBuffer;
	return bReturn;
};

//=============================================================================
// Method		: ReadHWIFile
// Access		: public static 
// Returns		: BOOL
// Parameter	: __out ST_HW_INFO_FILE * stHWIFormat
// Parameter	: __in LPCTSTR lpFileName
// Qualifier	:
// Last Update	: 2013/1/22 - 15:23
// Desc.		:
//=============================================================================
static BOOL ReadHWIFile (__out ST_HW_INFO_FILE* stHWIFormat, __in LPCTSTR lpFileName)
{
	BOOL	bReturn = FALSE;
	HANDLE	hFile; 
	DWORD	dwBytesRead = 0;
	BYTE	ReadBuffer[256] = {0,};

	hFile = CreateFile(	lpFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile != INVALID_HANDLE_VALUE) 
	{
		// Read one character less than the buffer size to save room for the terminating NULL character.
		if( ReadFile(hFile, ReadBuffer, 255, &dwBytesRead, NULL) )
		{
			if (dwBytesRead > 0)
			{		
				OutputDebugString (TEXT("Data read from File\n"));

				if (dwBytesRead == sizeof (ST_HW_INFO_FILE))		
				{
					DWORD dwCount = dwBytesRead;
					if (DecryptData(ReadBuffer, dwBytesRead, &dwCount))			
					{
						memcpy ((BYTE*)stHWIFormat, &ReadBuffer, dwBytesRead);
						bReturn = TRUE;
					}
					else
					{
						OutputDebugString (TEXT("Data Decode Error!"));
					}
				}
				else // dwBytesRead == sizeof (ST_HW_INFO_FILE)
				{
					OutputDebugString (TEXT("Data is Not [ST_HW_INFO_FILE] File Format"));
				}
			}
			else // (dwBytesRead > 0)
			{
				OutputDebugString (TEXT("No data read from file \n"));
			}
		}
		else // ReadFile
		{
			OutputDebugString (TEXT("Could not read from file (error %d)\n"));
		}

		CloseHandle(hFile);
	}
	else
	{ 
		OutputDebugString (TEXT("Could not open file (error %d)\n"));
	}

	return bReturn;
};

//=============================================================================
// Method		: WriteHWIReg
// Access		: public static 
// Returns		: BOOL
// Parameter	: __in const ST_HW_INFO_FILE * stHWIFormat
// Qualifier	:
// Last Update	: 2013/1/24 - 15:28
// Desc.		:
//=============================================================================
static BOOL WriteHWIReg	(__in const ST_HW_INFO_FILE* stHWIFormat)
{		
	BOOL	bReturn			= FALSE;

	DWORD	dwBytesToWrite	= sizeof (ST_HW_INFO_FILE);
	DWORD	dwBytesWritten	= 0;
	DWORD	dwDataLen		= dwBytesToWrite;	
	BYTE*	pDataBuffer		= new BYTE[dwBytesToWrite];

	memcpy (pDataBuffer, (BYTE*)stHWIFormat, dwBytesToWrite);

	if (EncryptData (pDataBuffer, &dwDataLen, dwBytesToWrite))
	{
		HKEY	hKey	= NULL;
		DWORD	dwDisp	= 0;
		TCHAR	szSubKey[] = TEXT("Software\\HWInfo");

		if (RegCreateKeyEx(HKEY_CURRENT_USER, szSubKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, &dwDisp))
		{
			OutputDebugString (TEXT("Could not create the registry key!"));
			return FALSE;
		}

		if (ERROR_SUCCESS == RegSetValueEx(hKey, TEXT("HWInfoValue"), 0, REG_BINARY, pDataBuffer, dwDataLen))
		{
			bReturn = TRUE;
		}
		else
		{
			OutputDebugString (TEXT("Could not set the HWInfo Data!"));
		}

		RegCloseKey(hKey);
	}
	else
	{
		OutputDebugString (TEXT("Data Decode Error!"));
	}	

	return bReturn;
};

//=============================================================================
// Method		: ReadHWIReg
// Access		: public static 
// Returns		: BOOL
// Parameter	: __out ST_HW_INFO_FILE * stHWIFormat
// Qualifier	:
// Last Update	: 2013/1/24 - 15:28
// Desc.		:
//=============================================================================
static BOOL ReadHWIReg	(__out ST_HW_INFO_FILE* stHWIFormat)
{
	BOOL	bReturn		= FALSE;

	DWORD	dwType		= 0;
	HKEY	hKey		= NULL;
	TCHAR	szSubKey[]	= TEXT("Software\\HWInfo");
	DWORD	dwBytesRead = sizeof (ST_HW_INFO_FILE);
	BYTE	ReadBuffer[256] = {0,};
	
	if (RegOpenKeyEx(HKEY_CURRENT_USER, szSubKey, NULL, KEY_READ, &hKey))
	{
		OutputDebugString (TEXT("Could not open the registry key!"));
		return FALSE;
	}

	if (ERROR_SUCCESS == RegQueryValueEx( hKey, TEXT("HWInfoValue"), NULL, &dwType, ReadBuffer, &dwBytesRead ))
	{
		if(dwType == REG_BINARY)
		{
			if (dwBytesRead == sizeof (ST_HW_INFO_FILE))		
			{
				DWORD dwCount = dwBytesRead;
				if (DecryptData(ReadBuffer, dwBytesRead, &dwCount))			
				{
					memcpy ((BYTE*)stHWIFormat, &ReadBuffer, dwBytesRead);
					bReturn = TRUE;
				}
				else
				{
					OutputDebugString (TEXT("Data Decode Error!"));
				}
			}
			else // dwBytesRead == sizeof (ST_HW_INFO_FILE)
			{
				OutputDebugString (TEXT("Data is Not [ST_HW_INFO_FILE] File Format"));
			}
		}
		else
		{
			OutputDebugString (TEXT("Registry Value Types is not REG_BINARY"));
		}
	}
	else
	{
		OutputDebugString (TEXT("RegQueryValueEx failed !"));
	}

	RegCloseKey(hKey);
	
	return bReturn;
};

#endif // FileCrypt_h__
