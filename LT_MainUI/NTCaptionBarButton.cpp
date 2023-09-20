// NTCaptionBarButton.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NTCaptionBarButton.h"
#include "NTVisualManager.h"
#include "NTStyle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CNTCaptionBarButton

IMPLEMENT_DYNAMIC(CNTCaptionBarButton, CMFCButton)

//=============================================================================
// 생성자
//=============================================================================
CNTCaptionBarButton::CNTCaptionBarButton()
{
	m_bDrawFocus = FALSE;
}

//=============================================================================
// 소멸자
//=============================================================================
CNTCaptionBarButton::~CNTCaptionBarButton()
{
}


BEGIN_MESSAGE_MAP(CNTCaptionBarButton, CMFCButton)
END_MESSAGE_MAP()

// CNTCaptionBarButton 메시지 처리기입니다.
//=============================================================================
// Method		: CNTCaptionBarButton::OnDraw
// Access		: virtual protected 
// Returns		: void
// Parameter	: CDC * pDC
// Parameter	: const CRect & rect
// Parameter	: UINT uiState
// Qualifier	:
// Last Update	: 2010/03/10 - 10:48
// Desc.		: 
//=============================================================================
void CNTCaptionBarButton::OnDraw( CDC* pDC, const CRect& rect, UINT uiState )
{
	CNTVisualManager* pManager = DYNAMIC_DOWNCAST (CNTVisualManager, CMFCVisualManager::GetInstance ());

	long nID = GetSysCommand ();

	if (pManager == NULL || nID == 0)
	{
		ASSERT (FALSE);
	}
	else
	{
		CRect rt;
		GetClientRect(rt);

		CMFCVisualManager::AFX_BUTTON_STATE state = CMFCVisualManager::ButtonsIsRegular;

		if (m_bPushed)
		{
			state = CMFCVisualManager::ButtonsIsPressed;
		}
		else if (m_bHighlighted)
		{
			state = CMFCVisualManager::ButtonsIsHighlighted;
		}

		pManager->NTDrawCaptionButton (pDC, rt, state, nID);
	}
}

//=============================================================================
// Method		: CNTCaptionBarButton::GetSysCommand
// Access		: public 
// Returns		: UINT
// Qualifier	: const
// Last Update	: 2010/03/10 - 10:48
// Desc.		: 
//=============================================================================
UINT CNTCaptionBarButton::GetSysCommand() const
{
	HWND hWnd = GetSafeHwnd();

	ASSERT(hWnd != NULL);

	if (hWnd != NULL)
	{
		return ::GetWindowLong (hWnd, GWL_ID);
	}

	return 0;
}

//=============================================================================
// Method		: CNTCaptionBarButton::SetSysCommand
// Access		: public 
// Returns		: void
// Parameter	: UINT uiSysCommand
// Qualifier	:
// Last Update	: 2010/03/10 - 10:48
// Desc.		: 
//=============================================================================
void CNTCaptionBarButton::SetSysCommand( UINT uiSysCommand )
{
	HWND hWnd = GetSafeHwnd();

	ASSERT(hWnd != NULL);

	if (GetSysCommand () != uiSysCommand && hWnd != NULL)
	{
		::SetWindowLong (hWnd, GWL_ID, uiSysCommand);
	}
}





