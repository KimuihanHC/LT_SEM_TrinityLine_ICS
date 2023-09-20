//*****************************************************************************
// Filename	: Define_OptionItem.h
// Created	: 2010/9/3
// Modified	: 2010/9/3 - 11:00
//
// Author	: PiRing
//	
// Purpose	: 옵션의 컨트롤 항목의 문자 데이터들 모음
//*****************************************************************************
#ifndef Define_OptionItem_h__
#define Define_OptionItem_h__

//-----------------------------------------------
// 시리얼 통신
//-----------------------------------------------
static LPCTSTR lpszComPort[] = 
{
	_T("COM:1"),
	_T("COM:2"),
	_T("COM:3"),
	_T("COM:4"),
	_T("COM:5"),
	_T("COM:6"),
	_T("COM:7"),
	_T("COM:8"),
	_T("COM:9"),
	_T("COM:10"),
	_T("COM:11"),
	_T("COM:12"),
	_T("COM:13"),
	_T("COM:14"),
	_T("COM:15"),
	_T("COM:16"),
	_T("COM:17"),
	_T("COM:18"),
	_T("COM:19"),
	_T("COM:20"),
	_T("COM:21"),
	_T("COM:22"),
	_T("COM:23"),
	_T("COM:24"),
	_T("COM:25"),
	_T("COM:26"),
	_T("COM:27"),
	_T("COM:28"),
	_T("COM:29"),
	_T("COM:30"),
	_T("COM:31"),
	_T("COM:32"),
	_T("COM:33"),
	_T("COM:34"),
	_T("COM:35"),
	_T("COM:36"),
	_T("COM:37"),
	_T("COM:38"),
	_T("COM:39"),
	_T("COM:40"),
	NULL
};

static LPCTSTR lpszBaudRate[] = 
{
	_T("9600"),
	_T("14400"), 
	_T("19200"),
	_T("38400"), 
	_T("56000"), 
	_T("57600"), 
	_T("115200"), 
	_T("128000"), 
	_T("256000"),
	_T("460800"),
	_T("921600"),
	NULL
};

#define CBR_460800          460800
#define CBR_921600          921600

static const DWORD BaudRateTable[] = 
{	
	CBR_9600, 
	CBR_14400, 
	CBR_19200, 
	CBR_38400,
	CBR_56000, 
	CBR_57600, 
	CBR_115200, 
	CBR_128000, 
	CBR_256000,
	CBR_460800,
	CBR_921600
};

static LPCTSTR lpszParity[] = 
{ 
	_T("None"), 
	_T("Even"), 
	_T("Odd"), 
	_T("Mark"), 
	_T("Space"),
	NULL
};
static const BYTE ParityTable[] = 
{
	NOPARITY, 
	EVENPARITY, 
	ODDPARITY, 
	MARKPARITY, 
	SPACEPARITY 
};

static LPCTSTR lpszStopBits[] =  
{ 
	_T("1"), 
	_T("1.5"), 
	_T("2"),
	NULL
};

static const BYTE StopBitsTable[] = 
{ 
	ONESTOPBIT, 
	ONE5STOPBITS, 
	TWOSTOPBITS 
};


//-----------------------------------------------
// 핸디 BCR 형태
//-----------------------------------------------
// Serial, PS2 & USB
static LPCTSTR lpszHandyBCRType [] =
{
	_T("Serial Type"),
	_T("PS2, USB Type"),
	NULL
};

typedef enum enBCRType
{
	BCR_Fixed,
	BCR_Handy,
}enBCRType;

static LPCTSTR lpszBCRType[] =
{
	_T("Fix Type"),
	_T("Handy Type"),
	NULL
};

//-----------------------------------------------
// 공용
//-----------------------------------------------
static LPCTSTR lpszUsableTable [] =
{
	_T("Not use"),
	_T("Use"),
	NULL
};

static LPCTSTR lpszYesNo [] =
{
	_T("No"),
	_T("Yes"),
	NULL
};

typedef enum enSaveType
{	
	SaveType_OnlyFail,
	SaveType_All,
	SaveType_NoSave,
}enSaveType;

static LPCTSTR lpszSaveType[] =
{
	_T("불량만 저장"),
	_T("모두 저장"),
	_T("저장 안함"),
	NULL
};

typedef enum enSaveFileType
{
	Save_Name_Barcode,
	Save_Name_Overwrite,
}enSaveFileType;

static LPCTSTR lpszSaveFilenameType[] =
{
	_T("바코드로 파일 이름 사용"),
	_T("파일 덮어쓰기"),
	NULL
};


static LPCTSTR lpszChartIndex[] =
{
	_T("Monitor 1"),
	_T("Monitor 2"),
	_T("Monitor 3"),
	NULL
};

typedef enum enChartIndex
{
	ChartIndex_Monitor1,
	ChartIndex_Monitor2,
	ChartIndex_Monitor3,
}enChartIndex;

typedef enum enRecordType
{
	Rec_FPS_Based,
}enRecordType;

static LPCTSTR lpszRecordType[] =
{
	_T("Time - Based"),
	NULL
};

typedef enum
{
	OptLang_Korean,
	OptLang_English,
	OptLang_Vietnamese,
	OptLang_Chinese,

	OptLang_MaxCount,
}enOptLanguage;

static LPCTSTR g_szOptLanguage[] =
{
	_T("한국어 (Korean)"),
	_T("English"),
	_T("Vietnamese"),//_T("Tiếng Việt (Vietnamese)"),
	_T("中國語 (Chinese)"),
	NULL
};

#endif // Define_OptionItem_h__
