//*****************************************************************************
// Filename	: 	Wnd_Dummy.h
// Created	:	2021/1/26 - 14:20
// Modified	:	2021/1/26 - 14:20
//
// Author	:	piring
//	
// Purpose	:	
//*****************************************************************************
#ifndef Wnd_Dummy_h__
#define Wnd_Dummy_h__

#pragma once
#include <afxwin.h>
#include "Wnd_BaseView.h"

//-----------------------------------------------------------------------------
// CWnd_Dummy
//-----------------------------------------------------------------------------
class CWnd_Dummy : public CWnd_BaseView
{
	DECLARE_DYNAMIC(CWnd_Dummy)

public:
	CWnd_Dummy();
	virtual ~CWnd_Dummy();

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg int		OnCreate			(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnSize				(UINT nType, int cx, int cy);
	virtual BOOL	PreCreateWindow		(CREATESTRUCT& cs);
	virtual BOOL	PreTranslateMessage	(MSG* pMsg);

private:
	CFont			m_font;



public:



};

#endif // Wnd_Dummy_h__
