// RichEditCtrlEx.cpp : implementation file
//

#include "pch.h"
#include "RichEditCtrlEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define ID_MENU_UNDO		1001
#define ID_MENU_REDO		1002
#define ID_MENU_COPY		1003
#define ID_MENU_PASTE		1004
#define ID_MENU_CUT			1005
#define ID_MENU_ERASE		1006
#define ID_MENU_ERASE_ALL	1007
#define ID_MENU_SELECT_ALL	1008
#define ID_MENU_SAVE		1009

#define KS_DOWNED			0x8000

#ifndef VK_X
	#define VK_X	('X')
#endif

#ifndef VK_Z
	#define VK_Z	('Z')
#endif

#ifndef VK_Y
	#define VK_Y	('Y')
#endif

#ifdef UNICODE
	#define SZ_RICHEDIT	_T("RichEdit20W")
#else
	#define SZ_RICHEDIT	_T("RichEdit20A")
#endif

/////////////////////////////////////////////////////////////////////////////
// CRichEditCtrlEx

CRichEditCtrlEx::CRichEditCtrlEx()
{
}

CRichEditCtrlEx::~CRichEditCtrlEx()
{
}


BEGIN_MESSAGE_MAP(CRichEditCtrlEx, CRichEditCtrl)
	//{{AFX_MSG_MAP(CRichEditCtrlEx)
	ON_WM_CONTEXTMENU()
	ON_WM_INITMENUPOPUP()
	ON_COMMAND(ID_MENU_UNDO			, OnUndo)
	ON_COMMAND(ID_MENU_REDO			, OnRedo)
	ON_COMMAND(ID_MENU_COPY			, OnCopy)
	ON_COMMAND(ID_MENU_PASTE		, OnPaste)
	ON_COMMAND(ID_MENU_CUT			, OnCut)
	ON_COMMAND(ID_MENU_ERASE		, OnErase)
	ON_COMMAND(ID_MENU_ERASE_ALL	, OnEraseAll)
	ON_COMMAND(ID_MENU_SELECT_ALL	, OnSelectAll)
	ON_COMMAND(ID_MENU_SAVE			, OnSave)
//	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRichEditCtrlEx message handlers
BOOL CRichEditCtrlEx::Create(DWORD dwStyle, const RECT & rcRect, CWnd * pParentWnd, UINT nID)
{
	if(!::AfxInitRichEdit())
		return FALSE;

	if(!CWnd::Create(SZ_RICHEDIT, NULL, dwStyle, rcRect, pParentWnd, nID))
		return FALSE;

	::SendMessage(m_hWnd, EM_SETUNDOLIMIT, 100, 0);	// Maximum number of actions in the undo queue is 100
	::SendMessage(m_hWnd, EM_AUTOURLDETECT, 1, 0);

	return TRUE;
}

void CRichEditCtrlEx::AddString(LPCTSTR lpszText, CHARFORMAT * pCf /* = NULL */)
{
	long	lTotalLength = GetTextLength();	// Does not include the terminating null character.
	long	lLineCount = (long) GetLineCount() - 1L;

	SetSel(lTotalLength, lTotalLength);

	ReplaceSel(lpszText);
	SetSel(-1, -1);
}

void CRichEditCtrlEx::EraseAll()
{
	OnSelectAll();
	OnErase();
}

BOOL CRichEditCtrlEx::CanRedo() const
{
	if(!::IsWindow(m_hWnd))
		return FALSE;

	return (BOOL) ::SendMessage(m_hWnd, EM_CANREDO, 0, 0);
}

BOOL CRichEditCtrlEx::Redo()
{
	if(!::IsWindow(m_hWnd))
		return FALSE;

	return (BOOL) ::SendMessage(m_hWnd, EM_REDO, 0, 0);
}

void CRichEditCtrlEx::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	if(pWnd == this)
	{
		CMenu	Menu;
		
		Menu.CreatePopupMenu();
		
		// Menu.AppendMenu(MF_STRING	, ID_MENU_UNDO			, _T("Undo\tCtrl+Z"));
		// Menu.AppendMenu(MF_STRING	, ID_MENU_REDO			, _T("Redo\tCtrl+Y"));
		// Menu.AppendMenu(MF_SEPARATOR);
		Menu.AppendMenu(MF_STRING	, ID_MENU_COPY			, _T("&Copy\tCtrl+C"));
		// Menu.AppendMenu(MF_STRING	, ID_MENU_PASTE			, _T("Paste\tCtrl+V"));
		// Menu.AppendMenu(MF_STRING	, ID_MENU_CUT			, _T("Cut\tCtrl+X"));
		// Menu.AppendMenu(MF_STRING	, ID_MENU_ERASE			, _T("Erase"));
		Menu.AppendMenu(MF_STRING	, ID_MENU_ERASE_ALL		, _T("Erase All"));
		Menu.AppendMenu(MF_SEPARATOR);
		Menu.AppendMenu(MF_STRING	, ID_MENU_SELECT_ALL	, _T("Select All\tCtrl+A"));
		Menu.AppendMenu(MF_SEPARATOR);
		// Menu.AppendMenu(MF_STRING	, ID_MENU_SAVE			, _T("Save"));
		Menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
		Menu.DestroyMenu();
	}
}

void CRichEditCtrlEx::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu) 
{
	__super::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);
	
	// TODO: Add your message handler code here
	CHARRANGE	cr;

	GetSel(cr);

	BOOL	bReadOnly = ((GetStyle() & ECO_READONLY) == ECO_READONLY) ? TRUE : FALSE;
	UINT	unEnable = (cr.cpMax > cr.cpMin) ? MF_ENABLED : MF_GRAYED;

	// pPopupMenu->EnableMenuItem(ID_MENU_UNDO			, !bReadOnly && CanUndo() ? MF_ENABLED : MF_GRAYED);
	// pPopupMenu->EnableMenuItem(ID_MENU_REDO			, !bReadOnly && CanRedo() ? MF_ENABLED : MF_GRAYED);
	pPopupMenu->EnableMenuItem(ID_MENU_COPY			, unEnable);
	// pPopupMenu->EnableMenuItem(ID_MENU_PASTE		, !bReadOnly && CanPaste() ? MF_ENABLED : MF_GRAYED);
	// pPopupMenu->EnableMenuItem(ID_MENU_CUT			, !bReadOnly ? unEnable : MF_GRAYED);
	// pPopupMenu->EnableMenuItem(ID_MENU_ERASE		, unEnable);
	pPopupMenu->EnableMenuItem(ID_MENU_ERASE_ALL	, (GetTextLength() > 0) ? MF_ENABLED : MF_GRAYED);
	pPopupMenu->EnableMenuItem(ID_MENU_SELECT_ALL	, (GetTextLength() > 0) ? MF_ENABLED : MF_GRAYED);
	// pPopupMenu->EnableMenuItem(ID_MENU_SAVE			, (GetTextLength() > 0) ? MF_ENABLED : MF_GRAYED);
}

void CRichEditCtrlEx::OnUndo()
{
	Undo();
}

void CRichEditCtrlEx::OnRedo()
{
	Redo();
}

void CRichEditCtrlEx::OnCopy()
{
	Copy();
}

void CRichEditCtrlEx::OnPaste()
{
	Paste();
}

void CRichEditCtrlEx::OnCut()
{
	Cut();
}

void CRichEditCtrlEx::OnErase()
{
	Clear();
}

void CRichEditCtrlEx::OnEraseAll()
{
	EraseAll();
}

void CRichEditCtrlEx::OnSelectAll()
{
	SetSel(0, GetTextLength());
}

void CRichEditCtrlEx::OnSave()
{
	CFileDialog	Dlg(FALSE, _T("txt"), NULL, OFN_HIDEREADONLY | OFN_LONGNAMES | OFN_OVERWRITEPROMPT,
		_T("Text File (*.txt)|*.txt|All Files (*.*)|*.*||"), NULL);

	if(Dlg.DoModal() == IDOK)
		Save(Dlg.GetPathName(), TRUE);
}

BOOL CRichEditCtrlEx::Save(LPCTSTR lpszFileName, BOOL bOverWrite /* = TRUE */)
{
	BOOL bResult;

	try
	{
		CFile	file(lpszFileName,
			(!bOverWrite ? (CFile::modeCreate | CFile::modeNoTruncate) : CFile::modeCreate)
					 | CFile::modeWrite
					 #ifdef _UNICODE
					 | CFile::typeUnicode
					 #endif
		);

		file.SeekToEnd();

		CString	strText;

		GetWindowText(strText);

		file.Write((const void *)(LPCTSTR)strText, strText.GetLength() * sizeof(TCHAR));

		file.Close();

		bResult = TRUE;
	}
	catch(CFileException * e)
	{
		e->ReportError();
		e->Delete();

		bResult = FALSE;
	}

	return bResult;
}

LRESULT CRichEditCtrlEx::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(/*message == EM_UNDO ||	// Undo
	   message == EM_REDO ||	// Redo
	   message == WM_CUT ||		// Cut*/
	   message == WM_CLEAR ||	// Clear
	   message == WM_KEYDOWN && (::GetAsyncKeyState(VK_CONTROL) & KS_DOWNED) == KS_DOWNED && (wParam == VK_X || wParam == VK_Z || wParam == VK_Y))	// Ctrl+X(Cut) or Ctrl+Z(Undo) or Ctrl+Y(Redo)
		return WinProcEx(message, wParam, lParam);

	return __super::WindowProc(message, wParam, lParam);
}

LRESULT CRichEditCtrlEx::WinProcEx(UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT	lResult;

	if((GetStyle() & ECO_READONLY) == ECO_READONLY)
	{
		SetReadOnly(FALSE);

		lResult = __super::WindowProc(message, wParam, lParam);

		SetReadOnly(TRUE);
	}
	else
	{
		lResult = __super::WindowProc(message, wParam, lParam);
	}

	return lResult;
}
/*
BOOL CRichEditCtrlEx::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	if(nHitTest != HTCLIENT)
	{
		::SetCursor(::LoadCursor(nullptr, IDC_HAND));

		return TRUE;
	}

	return __super::OnSetCursor(pWnd, nHitTest, message);
}
*/