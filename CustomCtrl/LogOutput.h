//*****************************************************************************
// Filename	: LogOutput.h
// Created	: 2010/10/19
// Modified	: 2010/10/19 - 10:49
//
// Author	: PiRing
//	
// Purpose	: 
//*****************************************************************************
#ifndef LogOutput_h__
#define LogOutput_h__

#pragma once

//=============================================================================
// CLogOutput
//=============================================================================
class CLogOutput : public CRichEditCtrl
{
	DECLARE_DYNAMIC(CLogOutput)

public:
	CLogOutput();
	virtual ~CLogOutput();

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg int		OnCreate		(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL	PreCreateWindow	(CREATESTRUCT& cs);
	afx_msg HBRUSH	OnCtlColor		(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void	OnChar			(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void	OnKeyDown		(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void	OnKeyUp			(UINT nChar, UINT nRepCnt, UINT nFlags);

	virtual BOOL	PreTranslateMessage(MSG* pMsg);

	COLORREF	m_crText;
	COLORREF	m_crBackGnd;
	CBrush		m_brBackGnd;	
	CFont		m_Font;

public:
	void	AddString		(LPCTSTR lpszLogMsg, ... );
	void	AddText			(LPCTSTR lpszLogMsg, COLORREF clrTextColor = RGB(0, 0, 0));
	void	SetTextColor	(COLORREF rgb);
	void	SetBackColor	(COLORREF rgb);
	
	void	ChangeFont		(CString strFont, int iFontSize);

	void	RemoveAll		();
	
};

#endif // LogOutput_h__

