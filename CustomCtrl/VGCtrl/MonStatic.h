//*****************************************************************************
// Filename	: MonStatic.h
// Created	: 2013/2/25
// Modified	: 2013/2/25 - 14:29
//
// Author	: PiRing
//	
// Purpose	: 
//*****************************************************************************
#ifndef MonStatic_h__
#define MonStatic_h__

#pragma once

#include "VGCtrlBase.h"

// CMonStatic
//==========================================================================
class CMonStatic : public CStatic,
				   public CVGCtrlBase
{
	DECLARE_DYNAMIC(CMonStatic)

public:
	CMonStatic();
	virtual ~CMonStatic();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	virtual void	ReDrawPanel			();
};


#endif // MonStatic_h__
