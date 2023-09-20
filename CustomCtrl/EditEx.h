//*****************************************************************************
// Filename	: 	EditEx.h
// Created	:	2014/7/30 - 11:21
// Modified	:	2014/7/30 - 11:21
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
#ifndef EditEx_h__
#define EditEx_h__

#pragma once

class CEditEx : public CEdit
{
	DECLARE_DYNAMIC(CEditEx)

public:
	CEditEx();
	virtual ~CEditEx();

protected:
	DECLARE_MESSAGE_MAP()

	virtual BOOL PreTranslateMessage(MSG* pMsg);


public:
	
};
#endif // EditEx_h__


