//*****************************************************************************
// Filename	: Define_Option.h
// Created	: 2015/12/16
// Modified	: 2016/05/18
//
// Author	: PiRing
//	
// Purpose	: 옵션 관련 설정값 선언 파일
//*****************************************************************************
#ifndef Define_Option_h__
#define Define_Option_h__

#pragma pack (push,1)

#include "Define_OptionItem.h"
//#include "Def_Enum_Cm.h"
#include <afxwin.h>
#include <stdint.h>

namespace Luritech_Option
{
	//---------------------------------
	// Option Base
	//---------------------------------
	typedef struct _stOption_Base
	{
		_stOption_Base()
		{
			Reset();
		};

		virtual void Reset()
		{
		};

		_stOption_Base& operator= (_stOption_Base& ref)
		{
			return *this;
		};
	}stOption_Base;

	//---------------------------------
	// Serial 통신
	//---------------------------------
	typedef struct _stOption_Serial : public stOption_Base
	{
		BYTE	Port;
		DWORD	BaudRate;	/* Baudrate at which running       */
		BYTE	ByteSize;	/* Number of bits/byte, 4-8        */
		BYTE	Parity;		/* 0-4=None,Odd,Even,Mark,Space    */
		BYTE	StopBits;	/* 0,1,2 = 1, 1.5, 2               */

		_stOption_Serial ()
		{
			Reset ();
		};

		void Reset ()
		{
			Port		= 1;
			BaudRate	= CBR_115200;
			ByteSize	= 8;
			Parity		= NOPARITY;
			StopBits	= ONESTOPBIT;
		};

		_stOption_Serial& operator= (_stOption_Serial& ref)
		{
			Port		= ref.Port;
			BaudRate	= ref.BaudRate;
			ByteSize	= ref.ByteSize;
			Parity		= ref.Parity;
			StopBits	= ref.StopBits;

			return *this;
		};
	}stOpt_Serial;

	//---------------------------------
	// 이더넷 통신
	//---------------------------------
	typedef struct _stOpt_Ethernet : public stOption_Base
	{
		DWORD	dwAddress;
		DWORD	dwPort;

		_stOpt_Ethernet ()
		{
			Reset ();
		};

		void Reset ()
		{
			dwAddress	= 0;
			dwPort		= 0;
		};

		_stOpt_Ethernet& operator= (_stOpt_Ethernet& ref)
		{
			dwAddress	= ref.dwAddress;
			dwPort		= ref.dwPort;

			return *this;
		};
	}stOpt_Ethernet;
	
	//---------------------------------
	// 기타
	//---------------------------------
	typedef struct _stOpt_Misc : public stOption_Base
	{
		_stOpt_Misc ()
		{
			Reset ();
		};

		void Reset ()
		{
			
		};

		_stOpt_Misc& operator= (_stOpt_Misc& ref)
		{
			return *this;
		};
	}stOpt_Misc;

	//---------------------------------
	// MES
	//---------------------------------	
	typedef struct _stOpt_Server : public stOption_Base
	{
		CString			szEquipmentID;
		stOpt_Ethernet	Address;
		CString			szPath_Log;

		_stOpt_Server()
		{
			Address.Reset();
			szPath_Log = _T("C:\\Server_Log\\");
		};

		void Reset()
		{
			szEquipmentID.Empty();
			Address.Reset();
			szPath_Log.Empty();
		};

		_stOpt_Server& operator= (_stOpt_Server& ref)
		{
			szEquipmentID	= ref.szEquipmentID;
			Address			= ref.Address;
			szPath_Log		= ref.szPath_Log;

			return *this;
		};
	}stOpt_Server;
	
	//---------------------------------
	// Barcode Reader & Motor
	//---------------------------------	
	typedef struct _stOpt_BCR : public stOption_Base
	{	
		stOpt_Serial	ComPort;

		_stOpt_BCR()
		{
			Reset ();
		};

		void Reset ()
		{
			ComPort.Reset();
			
		};

		_stOpt_BCR& operator= (_stOpt_BCR& ref)
		{
			ComPort			 = ref.ComPort;
			
			return *this;
		};
	}stOpt_BCR;

	//---------------------------------
	// 검사기 설정
	//---------------------------------
	typedef struct _stOption_Inspector : public stOption_Base
	{
		// 운영
		uint8_t		nLanguage			= 1;
		uint16_t	nAutoModeDuration	= 10; // 분 단위

		// 저장 경로
		CString		szPath_Log;
		CString		szPath_Report;
		CString		szPath_Recipe;
		CString		szPath_Socket;
		CString		szPath_Shared;
		CString		szPath_FailInfo;

		// Auto 모드에서 설비의 통신 끊김 알람
		BOOL		bAlarm_EqpDiscon_AutoMode;

		// 자동 재 실행 사용 여부
		BOOL		UseAutoRestart;
		// 모델 설정 화면 잠금 기능
		BOOL		UseDeviceInfoPane;

		//*** 기타 ***//
		//CString		Password_Admin;

		_stOption_Inspector ()
		{
			nLanguage			= 1;
			nAutoModeDuration	= 10;

			szPath_Log			= _T("C:\\Log");
			szPath_Report		= _T("C:\\Report");
			szPath_Recipe		= _T("C:\\Recipe");
			szPath_Socket		= _T("C:\\Socket");
			szPath_Shared		= _T("C:\\Shared");
			szPath_FailInfo		= _T("C:\\FailInfo");

			UseAutoRestart		= FALSE;

			bAlarm_EqpDiscon_AutoMode	= FALSE;
		};

		void Reset ()
		{
			nLanguage			= 1;
			nAutoModeDuration	= 10;

			szPath_Log.Empty();
			szPath_Report.Empty();
			szPath_Recipe.Empty();
			szPath_Socket.Empty();
			szPath_Shared.Empty();
			szPath_FailInfo.Empty();

			UseAutoRestart		= FALSE;

			//Password_Admin.Empty();
		};

		_stOption_Inspector& operator= (_stOption_Inspector& ref)
		{
			nLanguage			= ref.nLanguage;
			nAutoModeDuration	= ref.nAutoModeDuration;

			szPath_Log			= ref.szPath_Log;
			szPath_Report		= ref.szPath_Report;
			szPath_Recipe		= ref.szPath_Recipe;
			szPath_Socket		= ref.szPath_Socket;
			szPath_Shared		= ref.szPath_Shared;
			szPath_FailInfo		= ref.szPath_FailInfo;

			UseAutoRestart		= ref.UseAutoRestart;

			//Password_Admin	= ref.Password_Admin;

			bAlarm_EqpDiscon_AutoMode = ref.bAlarm_EqpDiscon_AutoMode;

			return *this;
		};
	}stOpt_Insp;

	//-----------------------------------------------------
	// 통합 검사기용
	//-----------------------------------------------------
	typedef struct _stLT_Option
	{
		stOpt_Insp			Inspector;
		stOpt_Server		Server;
		stOpt_Misc			Misc;

		_stLT_Option ()
		{
			Reset ();
		};

		void Reset ()
		{
			Inspector.Reset();
			Misc.Reset();
		};

		_stLT_Option& operator= (_stLT_Option& ref)
		{
			Inspector		= ref.Inspector;
			Misc			= ref.Misc;

			return *this;
		}
	}stLT_Option;

	
	typedef enum
	{
		OPT_INSPECTOR		= 0,
		OPT_SERVER,
		OPT_MISC,
		OPT_TYPE_MAX,
	}enumOptionCategory;

	static LPCTSTR lpszOptionCategory [] = 
	{
		_T("System"),
		_T("Server"),
		_T("Misc"),
		NULL
	};
}

#pragma pack (pop)

#endif // Define_Option_h__

