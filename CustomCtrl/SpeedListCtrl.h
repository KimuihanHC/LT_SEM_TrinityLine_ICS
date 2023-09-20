//*****************************************************************************
// Filename	: 	SpeedListCtrl.h
// Created	:	2015/8/17 - 16:15
// Modified	:	2015/8/17 - 16:15
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
#ifndef SpeedListCtrl_h__
#define SpeedListCtrl_h__

#pragma once
#include <afxtempl.h>

/*-----------------------------------------------------------------------------
* Template Class: OCArray
* Author: Russell Robinson
* Purpose:
*    To provide a generic array class like CArray without the problems.
*    OCArray takes one parameter - TYPE.  Unlike CArray, OCArray always
*    returns references and expects references as parameters.
-----------------------------------------------------------------------------*/
template <class TYPE> class OCArray : public CArray<TYPE, TYPE&>
{
public:
	/*
	* Method:		OCArray::operator[] const
	* Parameters:	i_index			the array index to access
	* Returns:		const TYPE&		reference to the element at the index
	* Author:		Russell Robinson
	* Purpose:
	*		To return an element of the array for const access.
	*/
	inline const TYPE&	operator[](int i_index) const
	{
		ASSERT(0 <= i_index && i_index < GetSize());
		return (GetData()[i_index]);
	};

	/*
	* Method:		OCArray::GetAt
	* Parameters:	i_index			the array index to access
	* Returns:		const TYPE&		reference to the element at the index
	* Author:		Russell Robinson
	* Purpose:
	*		To return an element of the array for const access.
	*/
	inline const TYPE&	GetAt(int i_index) const
	{
		ASSERT(0 <= i_index && i_index < GetSize());
		return (GetData()[i_index]);
	};

	/*
	* Method:		OCArray::operator[]
	* Parameters:	i_index		the array index to access
	* Returns:		TYPE&		reference to the element at the index
	* Author:		Russell Robinson
	* Purpose:
	*		To return an element of the array for possible modification.
	*		This method is needed because the compiler loses the base class's method.
	*/
	inline TYPE&		operator[](int i_index)
	{
		ASSERT(0 <= i_index && i_index < GetSize());
		return (GetData()[i_index]);
	};
};

//=============================================================================
//
//=============================================================================
class CLabelItem : public CObject
{
public:
	CArray<CString, CString> sItem;

public:
	CLabelItem()
	{
		sItem.SetSize(99);
	};

	CLabelItem(const CLabelItem& ci)
	{
		sItem.Copy(ci.sItem);
	};

	const CLabelItem& operator=(const CLabelItem& ci)
	{
		sItem.Copy(ci.sItem);
		return *this;
	}

	virtual ~CLabelItem()
	{	
		//for (int i = sItem.GetCount() - 1; i >= 0; i--)
			//sItem.RemoveAt(i);
		sItem.RemoveAll();		
	};
};

//=============================================================================
//
//=============================================================================
//class CSpeedListCtrl : public CListCtrl
class CSpeedListCtrl : public CMFCListCtrl
{
public:
	CSpeedListCtrl();
	virtual ~CSpeedListCtrl();

protected:
	BOOL hColumn[255];
	CArray <CLabelItem, CLabelItem> m_arLabels;	
	unsigned int	m_LabelCount;
	virtual int		InsertItem		(UINT nMask, int nItem, LPCTSTR lpszItem, UINT nState, UINT nStateMask, int nImage, LPARAM lParam);
	void			SortByCol		(const int ColIndex);

	afx_msg void	OnGetdispinfo	(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void	OnColumnclick	(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()	

public:
	virtual int		AddItem			(LPCTSTR lpszItem);
	virtual BOOL	SetItemText		(int nItem, int nSubItem, LPCTSTR lpszText);
	void			Complete		();

	BOOL			DeleteAllItems	();
	BOOL			DeleteItem		(int nItem);

	void			SetSize(UINT nNewSize)
	{
		m_arLabels.SetSize(nNewSize);
		m_arLabels.FreeExtra();
	};

};


#endif // SpeedListCtrl_h__
