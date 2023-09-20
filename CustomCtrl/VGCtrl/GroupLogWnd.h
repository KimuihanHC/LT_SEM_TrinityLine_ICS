//*****************************************************************************
// Filename	: GroupLogWnd.h
// Created	: 2010/12/2
// Modified	: 2010/12/2 - 19:11
//
// Author	: PiRing
//	
// Purpose	: 
//*****************************************************************************
#ifndef GroupLogWnd_h__
#define GroupLogWnd_h__

#pragma once

#include "VGGroupWnd.h"
#include "LogOutput.h"

class CGroupLogWnd : public CVGGroupWnd
{
public:
	CGroupLogWnd(void);
	virtual ~CGroupLogWnd(void);

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg int		OnCreate		(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnSize			(UINT nType, int cx, int cy);
	
	virtual void	ReDrawPanel		();

	CFont			m_Font;
	CLogOutput		m_ed_Log;

public:
	inline void		AddString		(LPCTSTR lpszLogMsg, ... )
	{m_ed_Log.AddString(lpszLogMsg);};

	inline void		AddText			(LPCTSTR lpszLogMsg, COLORREF clrTextColor = RGB(0, 0, 0))
	{m_ed_Log.AddText(lpszLogMsg, clrTextColor);};

	inline void		SetLogTextColor	(COLORREF rgb)
	{m_ed_Log.SetTextColor(rgb);};

	inline void		SetLogBackColor	(COLORREF rgb)
	{m_ed_Log.SetBackColor(rgb);};

	inline void		ChangeLogFont	(CString strFont, int iFontSize)
	{m_ed_Log.ChangeFont(strFont, iFontSize);};
};

#endif // GroupLogWnd_h__
