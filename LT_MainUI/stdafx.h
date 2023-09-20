// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

//Windows XP	NTDDI_WINXP		(0x05010000)
//Windows 7		NTDDI_WIN7		(0x06010000)
//Windows 8		NTDDI_WIN8		(0x06020000)
//Windows 8.1	NTDDI_WINBLUE	(0x06030000)

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#include <afxwin.h>         // MFC 핵심 및 표준 구성 요소입니다.
#include <afxext.h>         // MFC 확장입니다.


#include <afxdisp.h>        // MFC 자동화 클래스입니다.



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // Internet Explorer 4 공용 컨트롤에 대한 MFC 지원입니다.
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // Windows 공용 컨트롤에 대한 MFC 지원입니다.
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC의 리본 및 컨트롤 막대 지원

//#include "vld.h"

//#ifndef ULONG_PTR
//#define ULONG_PTR unsigned long*
//#endif

#define _CRT_SECURE_NO_DEPRECATE

// this line is needed if you get the error -> C2061: syntax error : identifier 'Count'
// if the environment can't find this file, try commenting it out
#include <Specstrings.h>

#include <Gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;



// TODO: reference additional headers your program requires here
