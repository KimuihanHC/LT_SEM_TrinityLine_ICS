//*****************************************************************************
// Filename	: Dialgo_Base.h
// Created	: 2010/12/1
// Modified	: 2010/12/1 - 13:09
//
// Author	: PiRing
//	
// Purpose	: 
//*****************************************************************************
#ifndef Dialgo_Base_h__
#define Dialgo_Base_h__

#pragma once


//=============================================================================
// CDialgo_Base
//=============================================================================
class CDialgo_Base : public CDialogEx
{
	DECLARE_DYNAMIC(CDialgo_Base)

public:
	CDialgo_Base(LPCTSTR lpszCaption = NULL);
	//CDialgo_Base(CaptionText, Font, FontSize);
	virtual ~CDialgo_Base();
	
protected:
	DLGTEMPLATE		m_dlgTemplate;
	HLOCAL			hLocal;

	CStringW		m_strCaption;
	CStringW		m_strFontName;
	UINT			m_nFontSize;

public:
	void			SetDlgTemplate			();
	void			InitTemplate			();
	
};

#endif // Dialgo_Base_h__


