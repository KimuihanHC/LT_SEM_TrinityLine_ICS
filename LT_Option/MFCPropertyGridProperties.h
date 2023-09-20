//*****************************************************************************
// Filename	: MFCPropertyGridProperties.h
// Created	: 2010/10/5
// Modified	: 2010/10/5 - 11:14
//
// Author	: PiRing
//	
// Purpose	: 
//*****************************************************************************
#ifndef MFCPropertyGridProperties_h__
#define MFCPropertyGridProperties_h__


#pragma once
#include <Winsock2.h>

//=============================================================================
// CNotifyEdit
//=============================================================================
class CNotifyEdit : public CEdit
{
public:
	CNotifyEdit();
	virtual ~CNotifyEdit();
public:
	DECLARE_MESSAGE_MAP()
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};

class CMFCPropertyGridDateTimeProperty;

//=============================================================================
// CPropDateTimeCtrl
//=============================================================================
class CPropDateTimeCtrl: public CDateTimeCtrl
{
public:
	CPropDateTimeCtrl(CMFCPropertyGridDateTimeProperty *pProp, COLORREF clrBack);

protected:
	CBrush m_brBackground;
	COLORREF m_clrBack;
	CMFCPropertyGridDateTimeProperty *m_pProp;

public:
	DECLARE_MESSAGE_MAP()

	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void HScroll(UINT nSBCode, UINT nPos);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnDestroy();
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};

//=============================================================================
// CMFCPropertyGridDateTimeProperty
//=============================================================================
class CMFCPropertyGridDateTimeProperty : public CMFCPropertyGridProperty
{
	DECLARE_DYNAMIC(CMFCPropertyGridDateTimeProperty)

	BOOL m_style;
	BOOL m_updown;
	CString m_setformat;
	CString m_format;
public:
	CMFCPropertyGridDateTimeProperty(const CString& strName, COleDateTime &nValue, LPCTSTR lpszDescr = NULL, DWORD dwData = 0, BOOL style = TRUE, BOOL updown = FALSE, LPCTSTR setFormat = NULL, LPCTSTR format = NULL);

	virtual BOOL OnUpdateValue();
	virtual CString FormatProperty();
	virtual CString FormatOriginalProperty();

protected:
	virtual CWnd* CreateInPlaceEdit(CRect rectEdit, BOOL& bDefaultFormat);
	virtual BOOL OnSetCursor() const { return FALSE; /* Use default */ }
	BOOL IsValueChanged() const;
};

class CMFCPropertyGridIPAdressProperty;

//=============================================================================
// CPropIPAddressCtrl
//=============================================================================
class CPropIPAddressCtrl: public CIPAddressCtrl
{
public:
	CPropIPAddressCtrl(CMFCPropertyGridIPAdressProperty *pProp, COLORREF clrBack);

protected:
	CNotifyEdit m_IPEdit[4];
	CBrush m_brBackground;
	COLORREF m_clrBack;
	CMFCPropertyGridIPAdressProperty *m_pProp;
	BOOL m_bSetFont;
	WORD m_wFiled;

public:
	void EnableSetFont(BOOL bSetFont=TRUE) {m_bSetFont=bSetFont;}
	DECLARE_MESSAGE_MAP()

	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void HScroll(UINT nSBCode, UINT nPos);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnDestroy();
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

struct ULONG_VARIANT: public VARIANT
{
	ULONG_VARIANT(ULONG val)
	{
		vt=VT_UI4;
		ulVal=val;
	}
};

//=============================================================================
// CMFCPropertyGridIPAdressProperty
//=============================================================================
class CMFCPropertyGridIPAdressProperty : public CMFCPropertyGridProperty
{
	DECLARE_DYNAMIC(CMFCPropertyGridIPAdressProperty)

public:
	CMFCPropertyGridIPAdressProperty(const CString& strName, in_addr &nValue, LPCTSTR lpszDescr = NULL, DWORD dwData = 0);

	virtual BOOL OnUpdateValue();
	virtual CString FormatProperty();
	virtual CString FormatOriginalProperty();

protected:
	virtual CWnd* CreateInPlaceEdit(CRect rectEdit, BOOL& bDefaultFormat);
	virtual BOOL OnSetCursor() const { return FALSE; /* Use default */ }


};
#endif // MFCPropertyGridProperties_h__