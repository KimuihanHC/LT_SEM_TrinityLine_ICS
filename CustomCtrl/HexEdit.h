﻿// File    : HexEdit.cpp 
// Version : 1.0
// Date    : 21. Jan 2005
// Author  : Luo Pei'dn
// Email   : coollpe@hotmail.com
// Systems : VC6.0/7.0 and VC7.1 (Run under (Window 98/ME), Windows Nt 2000/XP/2003)
//
// For bugreport please email me
//
// You are free to use/modify this code but leave this header intact.
// You are free to use it in any of your applications.

#if !defined(AFX_HEXEDIT_H__52939FC1_17E0_467F_8AE8_6A0E367FB5F3__INCLUDED_)
#define AFX_HEXEDIT_H__52939FC1_17E0_467F_8AE8_6A0E367FB5F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HexEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHexEdit window
typedef struct _tag_TextColor {
		COLORREF clrTextColor;
		COLORREF clrBkColor;
}ST_TextColor;
class CHexEdit : public CEdit
{
// Construction
public:
	CHexEdit();

// Attributes
public:

protected:
	ST_TextColor m_icHexText;
	ST_TextColor m_icDecText;
private:
	bool m_bEnd;
	bool m_bHex;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHexEdit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHexEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CHexEdit)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
//	afx_msg void OnChange();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
public:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
private:
	CBrush m_bkDecBrush;//background dec color brush
	CBrush m_bkHexBrush;//background hex color brush
	bool IsHexConvertableText(LPTSTR _text);
	void FormatClipboard();
public:
	void SetHexColor(COLORREF clrBack, COLORREF clrText);
	void SetDecColor(COLORREF clrBack, COLORREF clrText);
	afx_msg void OnEnUpdate();
	afx_msg LRESULT OnConvert(WPARAM wParam, LPARAM lParam);
	unsigned int GetValue(void);
	void SetValue(unsigned int _value,bool _bHex=false);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HEXEDIT_H__52939FC1_17E0_467F_8AE8_6A0E367FB5F3__INCLUDED_)
