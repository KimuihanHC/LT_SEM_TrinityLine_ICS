//*****************************************************************************
// Filename	: 	List_Error.h
// Created	:	2016/10/29 - 16:56
// Modified	:	2016/10/29 - 16:56
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
#ifndef List_Error_h__
#define List_Error_h__

#pragma once

#include "Def_DataStruct_Cm.h"

//-----------------------------------------------------------------------------
// CList_Error
//-----------------------------------------------------------------------------
class CList_Error : public CListCtrl
{
	DECLARE_DYNAMIC(CList_Error)

public:
	CList_Error();
	virtual ~CList_Error();

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg int		OnCreate		(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnSize			(UINT nType, int cx, int cy);
	afx_msg void	OnNMClick		(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL	PreCreateWindow	(CREATESTRUCT& cs);

	CFont		m_Font;

	virtual void	InitHeader		();
	virtual void	InsertRowItemz	(__in const ST_ErrorInfo* pstErrorlist);
	void			AdjustColWidth	();

	void		SetRowItemz			(__in UINT nRow, __in const ST_ErrorInfo* pstErrorlist);

	CArray<ST_ErrorInfo, ST_ErrorInfo&> m_Errorlist;

public:

	void		InsertErrorInfo		(__in const ST_ErrorInfo* pstErrorlist);

	void		ClearErrorlist		();

	BOOL		GetErrorInfo		(__in UINT nRowIndex, __out ST_ErrorInfo& stErrorlist);

	void		DeleteError			(__in UINT nRowIndex);

	void		GetErrorInfo		();
};

#endif // List_Error_h__
