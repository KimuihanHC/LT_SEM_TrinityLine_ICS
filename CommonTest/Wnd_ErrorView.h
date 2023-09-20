//*****************************************************************************
// Filename	: 	Wnd_ErrorView.h
// Created	:	2016/10/29 - 16:47
// Modified	:	2016/10/29 - 16:47
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
#ifndef Wnd_ErrorView_h__
#define Wnd_ErrorView_h__

#pragma once

#include "List_Error.h"
#include "VGStatic.h"

//-----------------------------------------------------------------------------
// CWnd_ErrorView
//-----------------------------------------------------------------------------
class CWnd_ErrorView : public CWnd
{
	DECLARE_DYNAMIC(CWnd_ErrorView)

public:
	CWnd_ErrorView();
	virtual ~CWnd_ErrorView();

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg int		OnCreate			(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnSize				(UINT nType, int cx, int cy);
	virtual BOOL	PreCreateWindow		(CREATESTRUCT& cs);
	afx_msg void	OnNMClickList		(NMHDR * pNMHDR, LRESULT * result );
	afx_msg void	OnBnClickedBnDelete	();


	CFont	m_font_Default;
	CFont	m_font_Data;

	CVGStatic		m_st_Title;
	CVGStatic		m_st_ErrorCap;
	CList_Error		m_lc_Error;

	CVGStatic		m_st_DescCap;
	CEdit			m_ed_Desc;
	CButton			m_bn_Delete;


public:

	void		InsertErrorInfo		(__in const ST_ErrorInfo* pstErrorlist);
	//void		InsertErrorInfoList	(__in const ST_ErrorInfo* pstErrorlist);


};

#endif // Wnd_ErrorView_h__

