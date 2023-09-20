//*****************************************************************************
// Filename	: 	List_Barcode.h
// Created	:	2016/10/22
// Modified	:	2016/10/22
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
#ifndef List_Barcode_h__
#define List_Barcode_h__

#pragma once

//-----------------------------------------------------------------------------
// CList_Barcode
//-----------------------------------------------------------------------------
class CList_Barcode : public CListCtrl
{
	DECLARE_DYNAMIC(CList_Barcode)

public:
	CList_Barcode();
	virtual ~CList_Barcode();

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg int		OnCreate			(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnSize				(UINT nType, int cx, int cy);
	afx_msg void	OnNMClick			(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL	PreCreateWindow		(CREATESTRUCT& cs);

	CFont		m_Font;

	virtual void	InitHeader		();
	void			AdjustColWidth	();

public:
	
	void		ClearWorklist		();

	void		InsertBarcode		(__in LPCTSTR szBarcode);
	void		InsertBarcode		(__in LPCTSTR szBarcode, __in UINT nTableIdx);
};

#endif // List_Barcode_h__


