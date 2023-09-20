//*****************************************************************************
// Filename	: LogOutput.cpp
// Created	: 2010/10/19
// Modified	: 2010/10/19 - 10:49
//
// Author	: PiRing
//	
// Purpose	: 
//*****************************************************************************
// LogOutput.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "LogOutput.h"
#include <strsafe.h>

#define	COLOR_BK			RGB(0xFF, 0x00, 0x00)
#define MAX_LOG_BUFFER		2048
//=============================================================================
// CLogOutput
//=============================================================================
IMPLEMENT_DYNAMIC(CLogOutput, CRichEditCtrl)

//=============================================================================
//
//=============================================================================
CLogOutput::CLogOutput()
{
	m_crText = COLOR_BK;

	LOGFONT logFont;
	::ZeroMemory(&logFont, sizeof(LOGFONT));
	logFont.lfHeight = 15;
	logFont.lfWeight = FW_BOLD;
	::lstrcpy(logFont.lfFaceName, (LPCTSTR)_T("Tahoma"));
	m_Font.CreateFontIndirect(&logFont);
}

//=============================================================================
//
//=============================================================================
CLogOutput::~CLogOutput()
{
	if (m_brBackGnd.GetSafeHandle())
		m_brBackGnd.DeleteObject();

	m_Font.DeleteObject();
}

BEGIN_MESSAGE_MAP(CLogOutput, CRichEditCtrl)
	ON_WM_CTLCOLOR()
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_CREATE()
END_MESSAGE_MAP()

//=============================================================================
// CLogOutput 메시지 처리기입니다.
//=============================================================================
//=============================================================================
// Method		: CLogOutput::OnCreate
// Access		: public 
// Returns		: int
// Parameter	: LPCREATESTRUCT lpCreateStruct
// Qualifier	:
// Last Update	: 2010/11/1 - 16:43
// Desc.		:
//=============================================================================
int CLogOutput::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;
	 
	SetReadOnly();

	SetFont(&m_Font);

	ModifyStyleEx(WS_EX_CLIENTEDGE, 0, SWP_DRAWFRAME);

	return 0;
}

//=============================================================================
// Method		: CLogOutput::OnCtlColor
// Access		: public 
// Returns		: HBRUSH
// Parameter	: CDC * pDC
// Parameter	: CWnd * pWnd
// Parameter	: UINT nCtlColor
// Qualifier	:
// Last Update	: 2010/10/19 - 12:59
// Desc.		:
//=============================================================================
HBRUSH CLogOutput::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	pDC->SetTextColor(m_crText);
	pDC->SetBkColor(m_crBackGnd);

	return m_brBackGnd;
}

//=============================================================================
// Method		: CLogOutput::OnChar
// Access		: protected 
// Returns		: void
// Parameter	: UINT nChar
// Parameter	: UINT nRepCnt
// Parameter	: UINT nFlags
// Qualifier	:
// Last Update	: 2010/10/19 - 14:29
// Desc.		:
//=============================================================================
void CLogOutput::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
}

//=============================================================================
// Method		: CLogOutput::OnKeyDown
// Access		: protected 
// Returns		: void
// Parameter	: UINT nChar
// Parameter	: UINT nRepCnt
// Parameter	: UINT nFlags
// Qualifier	:
// Last Update	: 2010/10/19 - 14:29
// Desc.		:
//=============================================================================
void CLogOutput::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
}

//=============================================================================
// Method		: CLogOutput::OnKeyUp
// Access		: protected 
// Returns		: void
// Parameter	: UINT nChar
// Parameter	: UINT nRepCnt
// Parameter	: UINT nFlags
// Qualifier	:
// Last Update	: 2010/10/19 - 14:29
// Desc.		:
//=============================================================================
void CLogOutput::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
}

//=============================================================================
// Method		: CLogOutput::PreCreateWindow
// Access		: virtual protected 
// Returns		: BOOL
// Parameter	: CREATESTRUCT & cs
// Qualifier	:
// Last Update	: 2010/10/19 - 11:07
// Desc.		:
//=============================================================================
BOOL CLogOutput::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= ES_MULTILINE | ES_AUTOVSCROLL | ES_NOHIDESEL | ES_READONLY | WS_VSCROLL | WS_BORDER;

	return CRichEditCtrl::PreCreateWindow(cs);
}

//=============================================================================
// Method		: CLogOutput::SetBackColor
// Access		: public 
// Returns		: void
// Parameter	: COLORREF rgb
// Qualifier	:
// Last Update	: 2010/10/19 - 14:24
// Desc.		:
//=============================================================================
void CLogOutput::SetBackColor(COLORREF rgb)
{
	m_crBackGnd = rgb;

	if (m_brBackGnd.GetSafeHandle())
		m_brBackGnd.DeleteObject();
	
	m_brBackGnd.CreateSolidBrush(rgb);

	Invalidate(TRUE);
}

//=============================================================================
// Method		: CLogOutput::SetTextColor
// Access		: public 
// Returns		: void
// Parameter	: COLORREF rgb
// Qualifier	:
// Last Update	: 2010/10/19 - 14:24
// Desc.		:
//=============================================================================
void CLogOutput::SetTextColor(COLORREF rgb)
{
	m_crText = rgb;

	Invalidate(TRUE);
}

//=============================================================================
// Method		: CLogOutput::ChangeFont
// Access		: public 
// Returns		: void
// Parameter	: CString strFont
// Parameter	: int iFontSize
// Qualifier	:
// Last Update	: 2010/11/1 - 16:46
// Desc.		:
//=============================================================================
void CLogOutput::ChangeFont(CString strFont, int iFontSize)
{
	CHARFORMAT2    cf;

	cf.cbSize          = sizeof(CHARFORMAT2);
	cf.dwMask          = CFM_FACE | CFM_SIZE | CFM_OFFSET | CFM_COLOR | CFM_PROTECTED;
	cf.dwEffects       = CFE_PROTECTED;
	// 데스크탑 모니터와 노트북 모니터의 픽셀 크기를 맞추기 위함
	cf.yHeight         = MulDiv( iFontSize,GetDeviceCaps(::GetDC(HWND_DESKTOP),LOGPIXELSY),72);
	cf.yHeight        *= (1440 / GetDeviceCaps(::GetDC(HWND_DESKTOP),LOGPIXELSY));
	cf.wWeight         = FW_NORMAL;
	cf.yOffset         = 0;
	cf.crTextColor     = RGB(255, 0, 0);	
	cf.bPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	
	StringCbCopy ( cf.szFaceName, sizeof(cf.szFaceName), strFont );

	// 기본 폰트 변경
	SetDefaultCharFormat  ( cf );
}

//=============================================================================
// Method		: CLogOutput::AddString
// Access		: public 
// Returns		: void
// Parameter	: LPCTSTR lpszLogMsg
// Parameter	: ...
// Qualifier	:
// Last Update	: 2010/10/19 - 10:51
// Desc.		:
//=============================================================================
void CLogOutput::AddString( LPCTSTR lpszLogMsg, ... )
{
	if ( !::IsWindow(GetSafeHwnd()) ) 
		return;

	__try
	{
		if( GetLineCount() > 600 )
		{
			SetReadOnly(FALSE);				
			int nLineIndex = LineIndex(300);
			long nEndChar = nLineIndex + LineLength(nLineIndex);
			SetSel(0, nEndChar);
			Clear();
			SetReadOnly(TRUE);
		}
		
		TCHAR *lpszBuffer = new TCHAR[MAX_LOG_BUFFER];
		HWND hWnd = GetSafeHwnd();

		DWORD_PTR dwResult = 0;
		if (SendMessageTimeout(hWnd, WM_GETTEXTLENGTH, 0, 0, SMTO_NORMAL, 500L, &dwResult) != 0)
		{
			int nLen = (int) dwResult;
			if (SendMessageTimeout(hWnd, EM_SETSEL, nLen, nLen, SMTO_NORMAL, 500L, &dwResult) != 0)
			{
				size_t cb = 0;
				va_list args;
				va_start(args, lpszLogMsg);
				::StringCchVPrintfEx(lpszBuffer, MAX_LOG_BUFFER, NULL, &cb, 0, lpszLogMsg, args);
				va_end(args);			
				SendMessageTimeout(hWnd, EM_REPLACESEL, FALSE, reinterpret_cast<LPARAM>(lpszBuffer), SMTO_NORMAL, 500L, &dwResult);
			}
		}
		delete [] lpszBuffer;

		LineScroll(1);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		TRACE (_T("*** Exception Error : CLogOutput::AddString()\n"));
		SetSel(0, -1);
		Clear();
	}
}

//=============================================================================
// Method		: CLogOutput::AddText
// Access		: public 
// Returns		: void
// Parameter	: LPCTSTR lpszLogMsg
// Parameter	: COLORREF clrTextColor
// Qualifier	:
// Last Update	: 2010/11/1 - 17:24
// Desc.		:
//=============================================================================
void CLogOutput::AddText( LPCTSTR lpszLogMsg, COLORREF clrTextColor /*= RGB(0, 0, 0)*/ )
{	
	if (!GetSafeHwnd())
		return;

	try
	{
		CHARFORMAT cf;
		cf.cbSize = sizeof(CHARFORMAT);
		cf.dwMask = CFM_COLOR | CFM_UNDERLINE | CFM_BOLD;
		cf.dwEffects = (unsigned long)~(CFE_AUTOCOLOR | CFE_UNDERLINE | CFE_BOLD);
		cf.crTextColor = clrTextColor;

		SetSelectionCharFormat(cf);

		if( GetLineCount() > 600 )
		{
			SetReadOnly(FALSE);				
			int nLineIndex = LineIndex(300);		
			long nEndChar = nLineIndex + LineLength(nLineIndex);
			SetSel(0, nEndChar);
			Clear();
			SetReadOnly(TRUE);
		}

		SetSel(-1, -1);
		ReplaceSel(lpszLogMsg);

		LineScroll(1);
	}
	catch (...)
	{
		SetSel(0, -1);
		Clear();
	}
}

//=============================================================================
// Method		: CLogOutput::RemoveAll
// Access		: public 
// Returns		: void
// Qualifier	:
// Last Update	: 2013/7/23 - 11:56
// Desc.		:
//=============================================================================
void CLogOutput::RemoveAll()
{
	SetReadOnly(FALSE);
	SetSel(0, -1);
	Clear();
	SetReadOnly(TRUE);
}

//=============================================================================
// Method		: PreTranslateMessage
// Access		: virtual public  
// Returns		: BOOL
// Parameter	: MSG * pMsg
// Qualifier	:
// Last Update	: 2014/7/30 - 15:23
// Desc.		:
//=============================================================================
BOOL CLogOutput::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && ::GetKeyState(VK_CONTROL) < 0)
	{
		switch (pMsg->wParam)
		{
// 		case 'Z':
// 			Undo();
// 			return TRUE;
// 
// 		case 'X':
// 			Cut();
// 			return TRUE;

		case 'C':
			Copy();
			return TRUE;

// 		case 'V':
// 			Paste();
// 			return TRUE;

		case 'A':
			SetSel(0, -1);

			return TRUE;
		}
	}

	return CRichEditCtrl::PreTranslateMessage(pMsg);
}
