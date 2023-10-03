#if !defined(AFX_RICHEDITCTRLEX_H__22638C75_E85F_4178_AB85_C63687F5CB3E__INCLUDED_)
#define AFX_RICHEDITCTRLEX_H__22638C75_E85F_4178_AB85_C63687F5CB3E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RichEditCtrlEx.h : header file
//
#include <Richedit.h>

/////////////////////////////////////////////////////////////////////////////
// CRichEditCtrlEx window

class CRichEditCtrlEx : public CRichEditCtrl
{
// Construction
public:
	CRichEditCtrlEx();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRichEditCtrlEx)
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL Create(DWORD dwStyle, const RECT & rcRect, CWnd * pParentWnd, UINT nID);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRichEditCtrlEx();

	// Generated message map functions
protected:
	//{{AFX_MSG(CRichEditCtrlEx)
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	afx_msg void OnUndo();
	afx_msg void OnRedo();
	afx_msg void OnCopy();
	afx_msg void OnPaste();
	afx_msg void OnCut();
	afx_msg void OnErase();
	afx_msg void OnEraseAll();
	afx_msg void OnSelectAll();
	afx_msg void OnSave();
//	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

protected:
	LRESULT WinProcEx(UINT message, WPARAM wParam, LPARAM lParam);

public:
	void	AddString(LPCTSTR lpszText, CHARFORMAT * pCf = NULL);
	void	EraseAll();
	BOOL	Save(LPCTSTR lpszFileName, BOOL bOverWrite = TRUE);

	BOOL	CanRedo() const;
	BOOL	Redo();

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RICHEDITCTRLEX_H__22638C75_E85F_4178_AB85_C63687F5CB3E__INCLUDED_)
