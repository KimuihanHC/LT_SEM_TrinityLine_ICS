//*****************************************************************************
// Filename	: 	EditEx.cpp
// Created	:	2014/7/30 - 11:20
// Modified	:	2014/7/30 - 11:20
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
#include "stdafx.h"
#include "EditEx.h"


// CEditEx

IMPLEMENT_DYNAMIC(CEditEx, CEdit)

CEditEx::CEditEx()
{

}

CEditEx::~CEditEx()
{
}


BEGIN_MESSAGE_MAP(CEditEx, CEdit)
END_MESSAGE_MAP()


//=============================================================================
// Method		: PreTranslateMessage
// Access		: virtual public  
// Returns		: BOOL
// Parameter	: MSG * pMsg
// Qualifier	:
// Last Update	: 2014/7/30 - 11:20
// Desc.		:
//=============================================================================
BOOL CEditEx::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && ::GetKeyState(VK_CONTROL) < 0)
	{
		switch (pMsg->wParam)
		{
		case 'Z':
			Undo();

			return TRUE;

		case 'X':
			Cut();

			return TRUE;

		case 'C':
			Copy();

			return TRUE;

		case 'V':
			Paste();

			return TRUE;

		case 'A':
			SetSel(0, -1);

			return TRUE;
		}
	}

	return CEdit::PreTranslateMessage(pMsg);
}
