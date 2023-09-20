//*****************************************************************************
// Filename	: VS2008Panel.h
// Created	: 2013/2/19
// Modified	: 2013/2/19 - 14:59
//
// Author	: PiRing
//	
// Purpose	: 
//*****************************************************************************
#ifndef VS2008Panel_h__
#define VS2008Panel_h__

#pragma once
#include "VS2008PanelBase.h"
//=============================================================================
// CVS2008Panel
//=============================================================================
class CVS2008Panel : public CWnd,
					 public CVS2008PanelBase
{
	DECLARE_DYNAMIC(CVS2008Panel)

public:
	CVS2008Panel();
	virtual ~CVS2008Panel();

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnSize	(UINT nType, int cx, int cy);
	afx_msg void OnPaint();

	virtual void ReDrawPanel		();
	
public:
	
	void	SetCheck	(BOOL bHighlight);
	BOOL	GetCheck	();
};


#endif // VS2008Panel_h__
