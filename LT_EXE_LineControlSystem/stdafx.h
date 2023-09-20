
// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이 
// 들어 있는 포함 파일입니다.

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
#endif

//#define CRTDBG_MAP_ALLOC

#include "targetver.h"
//#include <crtdbg.h>

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 일부 CString 생성자는 명시적으로 선언됩니다.

// MFC의 공통 부분과 무시 가능한 경고 메시지에 대한 숨기기를 해제합니다.
#define _AFX_ALL_WARNINGS

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


#include <afxsock.h>            // MFC 소켓 확장



//#include "vld.h"

// 메모리 누수 체크 매크로 (boost::pool 쪽 라이브러리하고 충돌 남)
// #ifdef _DEBUG
// #include <crtdbg.h>
// #ifdef malloc
// #undef malloc
// #endif
// #define malloc(s) (_malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE__ ))
// #ifdef calloc
// #undef calloc
// #endif
// #define calloc(c, s) (_calloc_dbg(c, s, _NORMAL_BLOCK, __FILE__, __LINE__ ))
// #ifdef realloc
// #undef realloc
// #endif
// #define realloc(p, s) (_realloc_dbg(p, s, _NORMAL_BLOCK, __FILE__, __LINE__ ))
// #ifdef _expand
// #undef _expand
// #endif
// #define _expand(p, s) (_expand_dbg(p, s, _NORMAL_BLOCK, __FILE__, __LINE__  ))
// #ifdef free
// #undef free
// #endif
// #define free(p) (_free_dbg(p, _NORMAL_BLOCK))
// #ifdef _msize
// #undef _msize
// #endif
// #define _msize(p) (_msize_dbg(p, _NORMAL_BLOCK))
// #endif

#include <Specstrings.h>

#include <Gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;


// 전역 변수 선언
#include "Def_WindowMessage.h"
#include "Def_CompileOption.h"

#include "Dlg_MessageBox.h"
#include <afxext.h>
#include <afxcontrolbars.h>
#include <afxext.h>
#include <afxext.h>
#include <afxcontrolbars.h>
#include <afxext.h>
#include <afxext.h>
#include <afxcontrolbars.h>
#include <afxext.h>
#include <afxcontrolbars.h>
#include <afxcontrolbars.h>
static INT_PTR LT_MessageBox(LPCTSTR IN_szText, UINT IN_nMB_Type = MB_OK, CWnd* pParent  = nullptr)
{
	CDlg_MessageBox dlgMsg(IN_szText, IN_nMB_Type, pParent);
	return dlgMsg.DoModal();
};

static INT_PTR LT_MessageBox_Solution(LPCTSTR IN_szText, LPCTSTR IN_szSolution, UINT IN_nMB_Type = MB_OK, CWnd* pParent = nullptr)
{
	CDlg_MessageBox dlgMsg(IN_szText, IN_nMB_Type, pParent);
	dlgMsg.Set_Solution(IN_szSolution);
	return dlgMsg.DoModal();
};

//#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
//#endif


