//*****************************************************************************
// Filename	: Dialgo_Base.cpp
// Created	: 2010/12/1
// Modified	: 2010/12/1 - 13:11
//
// Author	: PiRing
//	
// Purpose	: 
//*****************************************************************************
// Dialgo_Base.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Dialgo_Base.h"

//=============================================================================
// CDialgo_Base
//=============================================================================
IMPLEMENT_DYNAMIC(CDialgo_Base, CDialogEx)

//=============================================================================
// Method		: CDialgo_Base::CDialgo_Base
// Access		: public 
// Returns		: 
// Qualifier	:
// Last Update	: 2010/12/1 - 18:56
// Desc.		:
//=============================================================================
CDialgo_Base::CDialgo_Base( LPCTSTR lpszCaption /*= NULL*/ )
{		
	// Caption
	//Font, Font Size

	// Dialog Size

	if (NULL != lpszCaption)
		m_strCaption	= lpszCaption;

	m_strFontName	= _T("ARIAL");
	m_nFontSize		= 11;

	hLocal = NULL;
	SetDlgTemplate();
	InitTemplate();

	SetBackgroundColor(RGB(0xFF, 0x00, 0x00), FALSE);
}

//=============================================================================
// Method		: CDialgo_Base::~CDialgo_Base
// Access		: public 
// Returns		: 
// Qualifier	:
// Last Update	: 2010/12/1 - 18:56
// Desc.		:
//=============================================================================
CDialgo_Base::~CDialgo_Base()
{
	LocalUnlock(hLocal);
	LocalFree(hLocal);
}

//=============================================================================
// Method		: CDialgo_Base::SetDlgTemplate
// Access		: public 
// Returns		: void
// Qualifier	:
// Last Update	: 2010/12/1 - 18:56
// Desc.		:
//=============================================================================
void CDialgo_Base::SetDlgTemplate()
{
	// now initialize the DLGTEMPLATE structure	
	m_dlgTemplate.cx = 280;  // 4 horizontal units are the width of one character
	m_dlgTemplate.cy = 160;  // 8 vertical units are the height of one character
	m_dlgTemplate.style = WS_CAPTION | WS_VISIBLE | WS_DLGFRAME | WS_POPUPWINDOW | DS_MODALFRAME | DS_SETFONT;
	m_dlgTemplate.dwExtendedStyle = 0;
	m_dlgTemplate.x = 0;
	m_dlgTemplate.y = 0;
	m_dlgTemplate.cdit = 0;
}

//=============================================================================
// Method		: CDialgo_Base::InitTemplate
// Access		: public 
// Returns		: void
// Qualifier	:
// Last Update	: 2010/12/1 - 18:56
// Desc.		:
//=============================================================================
void CDialgo_Base::InitTemplate()
{
	// will first convert the control captions to UNICODE	
	TRY  // catch memory exceptions and don't worry about allocation failures
	{		
		int iSizeCaption	= (m_strCaption.GetLength() + 1) * sizeof(WCHAR);
		int iSizeFontName	= (m_strFontName.GetLength() + 1) * sizeof(WCHAR);
		int nBufferSize =  sizeof(DLGTEMPLATE) + (2 * sizeof(WORD))/*menu and class*/ + iSizeCaption;

		/* font information*/
		nBufferSize += sizeof(WORD) + iSizeFontName;

		// adjust size to make first control DWORD aligned
		nBufferSize = (nBufferSize + 3) & ~3;

		HLOCAL hLocal = LocalAlloc(LHND, nBufferSize);
		if (hLocal == NULL)
			AfxThrowMemoryException();

		BYTE*   pBuffer = (BYTE*)LocalLock(hLocal);
		if (pBuffer == NULL)
		{
			LocalFree(hLocal);
			AfxThrowMemoryException();
		}

		BYTE*   pdest	= pBuffer;
		BYTE* pdestEnd	= pdest + nBufferSize;

		// transfer DLGTEMPLATE structure to the buffer
		memcpy_s(pdest, pdestEnd - pdest, &m_dlgTemplate, sizeof(DLGTEMPLATE));
		pdest += sizeof(DLGTEMPLATE);

		*(WORD*)pdest = 0; // no menu
		*(WORD*)(pdest + sizeof(WORD)) = 0;  // use default window class

		pdest += 2 * sizeof(WORD);
		memcpy_s(pdest, pdestEnd - pdest, m_strCaption.GetBuffer(), iSizeCaption);
		pdest += iSizeCaption;

		*(WORD*)pdest = m_nFontSize;
		pdest += sizeof(WORD);
		memcpy_s(pdest, pdestEnd - pdest, m_strFontName.GetBuffer(), iSizeFontName);
		pdest += iSizeFontName;
		
		InitModalIndirect((DLGTEMPLATE*)pBuffer);
		
		//LocalUnlock(hLocal);
		//LocalFree(hLocal);
	}
	CATCH(CMemoryException, e)
	{
		TRACE(_T("Memory allocation for dialog template failed!"));
	}
	END_CATCH
}
