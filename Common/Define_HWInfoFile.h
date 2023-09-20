//*****************************************************************************
// Filename	: Define_HWInfoFile.h
// Created	: 2013/1/17
// Modified	: 2013/1/17 - 15:15
//
// Author	: PiRing
//	
// Purpose	: 
//*****************************************************************************
#ifndef Define_HWInfoFile_h__
#define Define_HWInfoFile_h__

#pragma pack(push, 1)

#define		HWI_FILENAME	TEXT("HWInfo.HWI")
// 파일 포맷
typedef struct _tagHWInfoFile_Ver_1_0
{
	// Header -------------------------
	BYTE	byFileCode; // .HWI
	DWORD	dwFileSize;

	// File Format Version	
	DWORD	dwMajorVersion;
	DWORD	dwMinorVersion;
	DWORD	dwBuildNumber;
	
	// Data ---------------------------
	// Processor ID
	CHAR	Processor_ID[32];
	// HDD S/N
	CHAR	DiskDriveSN[32];

	// 프로그램 설치 날짜
	SYSTEMTIME InstallTime;	
	// 사용 가능 기간 (단위 : 일)
	DWORD	AvailablePeriod;

	// 체크 사용여부
	BOOL	bUseProcessorID;
	BOOL	bUseDiskDriveSN;
	BOOL	bUseTimeLock;

	// 암호 (1st, 2nd, 3rd)	
}ST_HW_INFO_FILE_VER_1_0;

#define  ST_HW_INFO_FILE	ST_HW_INFO_FILE_VER_1_0

#pragma pack (pop)

#endif // Define_HWInfoFile_h__
