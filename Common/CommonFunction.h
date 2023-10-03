//*****************************************************************************
// Filename	: 	CommonFunction.h
//
// Created	:	
// Modified	:	2010/01/15 - 17:05
//
// Author	:	PiRing
//	
// Purpose	:	�������� ��ɵ��� �Լ��� ����
//*****************************************************************************
#ifndef __COMMONFUNCTION_H__
#define __COMMONFUNCTION_H__

#define MAX_LOG_LEN		1024*50

// Macro ----------------------------------------------------------------------
// ��Ʋ/�� ����� �Ǻ� ��ũ��
//const int iIs_bigendian = 1;
//#define is_bigendian() ( (*(char*)&iIs_bigendian) == 0 )

//#define CHECK(x,y,n) ( (x > 0 && x < n && y > 0 && y < n) ? 1 : 0)Common/
#define CHECK_RANGE(x, und, ovr) ( (x >= und && x <= ovr ) ? 1 : 0)
#define RANGE_CHECK(value, min, max, fail_return_value) { VALIDATE(min<=value && value<=max, return fail_return_value); }

#define AbsoluteValue(Num)	(Num>=0 ? Num : -Num)
//#define GetPrecentage(Numerator, Denominator)	(Numerator * 100.0 / Denominator)
// static inline double GetPrecentage(double Numerator, double Denominator) 
// {
// 	return (Numerator * 100.0) / Denominator;
// };

template <class T> inline T GetPrecentage(const T& Numerator, const T&Denominator)
{
	return (Numerator * T(100)) / Denominator;
}


#define ClearBits(Data, Area, Loc)		((Data) &= ~((area) << (loc)))		// ���ӵ� bit Clear
//#define SetBit(Data, Loc)				((Data) |= (0x01 << (loc)))			// �� bit Set
#define SetBits(Data, Loc)              ((Data) |= ((area) << (loc)))		// ���ӵ� bit
#define InvertBit(Data, Loc)            ((Data) ^= (0x1 << (loc)))			// �� bit ����
#define InvertBits(Data, Area, Loc)		((Data) ^= ((area) << (loc)))		// ���ӵ� bit ����
#define CheckBit(Data, Loc)             ((Data) & (0x01 << (loc)))			// ��Ʈ �˻�
#define ExtractBit(Data, Area, Loc)		(((Data) >> (Loc)) & (Area))		// ��Ʈ ����


#define SetBit(value,bit)			(value) |= 1 << (bit)
//#define SetBit(value, place)		(value | (1 << place))
#define ClearBit(value, place)		(value & (~(1 << place)))
#define ToggleBit(value, place)		(value ^ (1 << place))
#define GetBit(value, place)		((value >> place) & 1)
#define GetMSB(value, datatype)		GetBit(value, (sizeof(datatype) * 4 - 1))
#define GetLSB(value)				GetBit(value, 0)
// Macro ----------------------------------------------------------------------


//=============================================================================
// ���ڿ� ó��
//=============================================================================
//-------------------------------------------------------------------
// �� ���ڿ��� 10������ �ٲ۴�.
//-------------------------------------------------------------------
int			HexStringToDecimal		(const char *s , int count);
int			HexStringToDecimal		(LPCTSTR szHex);
//-------------------------------------------------------------------
// �� ����Ÿ�� �ƽ�Ű ������ ��Ʈ������ �ٲ۴�.
//-------------------------------------------------------------------
CString		HexToCString			(LPVOID lpBuff, UINT nBuffSize);
BOOL		HexToString				(LPCSTR lpInBuff, UINT nInSize, LPSTR lpOutBuff, UINT& nOutSize);

CString		BufferToHexCString		(BYTE* lpBuff, DWORD nDataCount);
DWORD		HexCStringToBuffer		(CStringA szHex, BYTE* pOutBuffer, DWORD nBufferSize);

//=============================================================================
// Method		: HexToString
// Access		: public  
// Returns		: BOOL
// Parameter	: __in const CHAR * lpInBuff
// Parameter	: __in UINT nInSize
// Parameter	: __out CHAR * lpOutBuff
// Parameter	: __out UINT & nOutSize
// Last Update	: 2016/3/28 - 19:14
// Desc.		:
//=============================================================================
BOOL HexToString (__in const CHAR* lpInBuff, __in UINT nInSize, __out CHAR* lpOutBuff, __out UINT& nOutSize);

//-------------------------------------------------------------------
// Null ����('\0')�� ���Ե� ����Ÿ�� �����̽� ����(0x20)�� �����Ѵ�.
//-------------------------------------------------------------------
CString		NullCharToSpaceChar		(LPVOID lpBuff, DWORD dwBuffSize);

char		HexCharzToByte			(char cFirstChar, char cSecondChar);
BYTE		HexCharToByte			(char cHexChar);

//-------------------------------------------------------------------
// �� ������ ��ġ�� �ð�, ��, �� ������ ��Ʈ������ ��ȯ�Ѵ�.
//-------------------------------------------------------------------
CString		SecondToTimeString		(DWORD dwSecond);
CString		MilisecondToTimeString	(DWORD dwMilisecond);

BOOL		StringTimeToSystemTime	(LPCSTR lpszTime, SYSTEMTIME& TimeTarget);
CString		SystemTimeToString		(SYSTEMTIME TimeSource);

BOOL		FormatStringToSystemTimeA(LPCSTR lpszTime, SYSTEMTIME& TimeTarget);
BOOL		FormatStringToSystemTime(LPCTSTR lpszTime, SYSTEMTIME& TimeTarget);
CString		SystemTimeToFormatString(SYSTEMTIME TimeSource);
CString		SystemTimeToFormatString_Month(SYSTEMTIME TimeSource);
double		CompareSystemTime		(SYSTEMTIME* pEndTime, SYSTEMTIME* pStartTime);
SYSTEMTIME	DifferSystemTime		(const SYSTEMTIME& pSr, const SYSTEMTIME& pSl);
ULONGLONG	DifferSystemTime_ms		(const SYSTEMTIME& pSr, const SYSTEMTIME& pSl);
double		DifferSystemTime_sec	(const SYSTEMTIME& pSr, const SYSTEMTIME& pSl);

//-------------------------------------------------------------------
// �Ϲ� ���� ��Ʈ���� �޸��� ���Ե� ��Ʈ������ ��ȯ
//-------------------------------------------------------------------
CString		DigitToCommaDigitStr	(long long llDigit);

CString	ConvNumberToFormatString(DOUBLE dData, BYTE byLength, BYTE byPoint);

//-------------------------------------------------------------------
// ������ ������ ��Ʈ������ ��ȯ
//-------------------------------------------------------------------
//CString	FixedSizeString			(__in CString strSrc, __in INT nSize, __in CHAR chFill = 0x20, __in BOOL bLeftFill = TRUE);
CStringA	FixedSizeString			(__in LPCSTR szSrc, __in INT nSize, __in CHAR chFill = 0x20, __in BOOL bLeftFill = TRUE);

BOOL isStringDouble(char *szString);


//=============================================================================
// ���� / ���丮 ó��
//=============================================================================
//-------------------------------------------------------------------
// ������ ������ ���� �ð��� ���´�.
//-------------------------------------------------------------------
BOOL		GetLastWriteTime		(HANDLE hFile, LPTSTR lpszString, DWORD dwSize);
BOOL		GetLastWriteTime		(__in LPCTSTR lpszFile, __out SYSTEMTIME& tmFile);
BOOL		GetLastWriteTime		(__in LPCTSTR lpszFile, __out FILETIME& tmFile);

BOOL		UtilIsDirectoryExists	(LPCTSTR directory);
int			UtilCreateDirectory		(LPCTSTR directory);
int			DeleteAllFiles			(LPCTSTR szDir, int recur);
void		MakeDirectory			(LPCTSTR szPath);
// ���丮 �˻� �� �������� �ڵ�����
void		MakeSubDirectory		(LPCTSTR lpszDirName);

BOOL		Backup_File				(__in LPCTSTR szPath);

//=============================================================================
// ����ȭ ó��
//=============================================================================
//-------------------------------------------------------------------
// ������ �޼��� ó���ϸ鼭 ����ϴ� ���
//-------------------------------------------------------------------
void		DoEvents				();
void		DoEvents				(DWORD dwMiliSeconds);
BOOL		WaitWithMessageLoop		(HANDLE hEvent, int nTimeout);
void		Delay					(DWORD nTimeMs);
void		GetAppDir				(CString& strAppDir);
void		WaitTerminateHandle		(HANDLE hParam, DWORD dwTimeOut = INFINITE);
void		WaitForThreadToTerminate(HANDLE hThread, DWORD dwTimeOut = INFINITE);
BOOL		WaitForTimerIsNull		(HANDLE hTimer, DWORD dwTimeOut = INFINITE);
//=============================================================================
// �ý��� ó��
//=============================================================================
//-------------------------------------------------------------------
// �ѱ�/���� ��ȯ ���
//-------------------------------------------------------------------
//BOOL __fastcall GetHanMode		(void);
//void __fastcall ToggleHanMode		(void);

//=============================================================================
// ���α׷� ���� ���ϱ�
//=============================================================================
// ���α׷� ���� ���ϴ� �Լ�
LPCTSTR		GetVersionInfo			(HMODULE hLib, LPCTSTR lpszEntry);
CString		GetVersionInfo			(LPCTSTR lpszEntry);
LPCTSTR		GetProductVersionInfo	();
CString		FormatVersion			(CString cs);

//=============================================================================
// IP �ּ� ��ȯ
//=============================================================================
BOOL		FormatIP				(LPTSTR pszIPAddr, UINT nSize, DWORD dwAddr);
CString		ConvIPAddrToString		(DWORD dwAddr);
DWORD		ConvIPStringToAddr		(LPCSTR lpszIPString);
CString		GetMyIPAddress			();
bool		isValidateIP4Address	(const char *s);

//=============================================================================
// Ʈ���� ������ �ܻ� ����
//=============================================================================
void		EraseTrayIcon			();
// ���μ��� ���� ����
BOOL		FocredCloseProcess		(LPCTSTR lpszProcessName, LPCTSTR lpszFuncDesc);
BOOL		NTCreateProcess			(PROCESS_INFORMATION &ProcessInformation, LPCTSTR lpszTargetName, LPSTR lpszCurrentFullPath);
void		NTExitProcess			(PROCESS_INFORMATION &ProcessInformation);
void		ForcedExitProcess		(CString strTargetProcName, DWORD dwExceptProcId);

//=============================================================================
// ��Ʈ��ũ ����
//=============================================================================
CString		GetPingResultCodeString	(ULONG lStatus);
DWORD		_tinet_addr				(const TCHAR *cp);

//=============================================================================
// ��¥ ó�� ����
//=============================================================================
UINT getDaysInMonth (UINT nMonth, UINT nYear);

//=============================================================================
// �ѱ� ó�� ����
//=============================================================================
char* UTF8toANSI(char *pszCode);

//=============================================================================
// 32bit/64bit ����
//=============================================================================
//32��Ʈ 64��Ʈ ���μ��� Ȯ��
BOOL IsWow64();
//�ü��(OS)�� 32��Ʈ 64��Ʈ Ȯ��
BOOL Is64BitWindows();


CString GetDefaultPrinterName();

//=============================================================================
//
//=============================================================================
BOOL	KeyBoardEvent( const char* lpcBuffer, DWORD dwBufferSize );
BOOL	RunTouchKeyboard();

#endif __COMMONFUNCTION_H__