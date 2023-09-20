//*****************************************************************************
// Filename	: MFCPropertyGridIPAdressProperty.cpp
// Created	: 2010/10/5
// Modified	: 2010/10/5 - 11:18
//
// Author	: PiRing
//	
// Purpose	: 
//*****************************************************************************
// MFCPropertyGridIPAdressProperty.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include <locale.h>
#include "MFCPropertyGridProperties.h"

//=============================================================================
// CNotifyEdit
//=============================================================================
CNotifyEdit::CNotifyEdit()
{
}

CNotifyEdit::~CNotifyEdit()
{
}

BEGIN_MESSAGE_MAP(CNotifyEdit, CEdit)
END_MESSAGE_MAP()

//=============================================================================
// Method		: CNotifyEdit::WindowProc
// Access		: virtual protected 
// Returns		: LRESULT
// Parameter	: UINT message
// Parameter	: WPARAM wParam
// Parameter	: LPARAM lParam
// Qualifier	:
// Last Update	: 2010/10/5 - 11:52
// Desc.		:
//=============================================================================
LRESULT CNotifyEdit::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if(message==WM_KILLFOCUS)
		GetParent()->PostMessage(message,wParam,lParam);

	return CEdit::WindowProc(message, wParam, lParam);
}

//=============================================================================
// CPropDateTimeCtrl
//=============================================================================
CPropDateTimeCtrl::CPropDateTimeCtrl(CMFCPropertyGridDateTimeProperty *pProp, COLORREF clrBack)
{
	m_clrBack = clrBack;
	m_brBackground.CreateSolidBrush(m_clrBack);
	m_pProp = pProp;
}

BEGIN_MESSAGE_MAP(CPropDateTimeCtrl,CDateTimeCtrl)
	ON_WM_KILLFOCUS()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_HSCROLL_REFLECT()
END_MESSAGE_MAP()

//=============================================================================
// Method		: CPropDateTimeCtrl::OnKillFocus
// Access		: protected 
// Returns		: void
// Parameter	: CWnd * pNewWnd
// Qualifier	:
// Last Update	: 2010/10/5 - 11:52
// Desc.		:
//=============================================================================
void CPropDateTimeCtrl::OnKillFocus(CWnd* pNewWnd)
{
	if(pNewWnd!=NULL && IsChild(pNewWnd))
		return;
	CDateTimeCtrl::OnKillFocus(pNewWnd);
}

//=============================================================================
// Method		: CPropDateTimeCtrl::CtlColor
// Access		: protected 
// Returns		: HBRUSH
// Parameter	: CDC * pDC
// Parameter	: UINT
// Qualifier	:
// Last Update	: 2010/10/5 - 11:52
// Desc.		:
//=============================================================================
HBRUSH CPropDateTimeCtrl::CtlColor(CDC* pDC, UINT /*nCtlColor*/)
{
	pDC->SetBkColor(m_clrBack);
	return m_brBackground;
}

//=============================================================================
// Method		: CPropDateTimeCtrl::HScroll
// Access		: protected 
// Returns		: void
// Parameter	: UINT
// Parameter	: UINT
// Qualifier	:
// Last Update	: 2010/10/5 - 11:52
// Desc.		:
//=============================================================================
void CPropDateTimeCtrl::HScroll(UINT /*nSBCode*/, UINT /*nPos*/)
{
	ASSERT_VALID(m_pProp);

	m_pProp->OnUpdateValue();
	m_pProp->Redraw();
}

//=============================================================================
// Method		: CPropDateTimeCtrl::WindowProc
// Access		: virtual protected 
// Returns		: LRESULT
// Parameter	: UINT message
// Parameter	: WPARAM wParam
// Parameter	: LPARAM lParam
// Qualifier	:
// Last Update	: 2010/10/5 - 11:52
// Desc.		:
//=============================================================================
LRESULT CPropDateTimeCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if(message>=WM_MOUSEFIRST && message<=WM_MOUSELAST)
	{
		POINT pt;
		GetCursorPos(&pt);
		CWnd *wnd=NULL;
		CRect rect;
		wnd=GetWindow(GW_CHILD);
		while(wnd)
		{
			wnd->GetWindowRect(rect);
			if(rect.PtInRect(pt))
			{
				wnd->SendMessage(message,wParam,lParam);
				return TRUE;
			}
			wnd=wnd->GetWindow(GW_HWNDNEXT);
		}
	}
	if(message==WM_DESTROY)
		SetFont(NULL,FALSE);

	return CDateTimeCtrl::WindowProc(message, wParam, lParam);
}

//=============================================================================
// CMFCPropertyGridDateTimeProperty
//=============================================================================
IMPLEMENT_DYNAMIC(CMFCPropertyGridDateTimeProperty,CMFCPropertyGridProperty)

CMFCPropertyGridDateTimeProperty::CMFCPropertyGridDateTimeProperty(const CString& strName, COleDateTime &nValue, LPCTSTR lpszDescr, DWORD dwData, BOOL style, BOOL updown, LPCTSTR setFormat, LPCTSTR format):
CMFCPropertyGridProperty(strName, nValue, lpszDescr, dwData)
{
	m_style=style;
	m_updown=updown;
	m_setformat=setFormat;
	m_format=format;
}

//=============================================================================
// Method		: CMFCPropertyGridDateTimeProperty::CreateInPlaceEdit
// Access		: virtual protected 
// Returns		: CWnd*
// Parameter	: CRect rectEdit
// Parameter	: BOOL & bDefaultFormat
// Qualifier	:
// Last Update	: 2010/10/5 - 11:52
// Desc.		:
//=============================================================================
CWnd* CMFCPropertyGridDateTimeProperty::CreateInPlaceEdit(CRect rectEdit, BOOL& bDefaultFormat)
{
	CPropDateTimeCtrl* pWndDateTime = new CPropDateTimeCtrl(this,m_pWndList->GetBkColor());

	rectEdit.InflateRect(4,2,0,3);

	DWORD style=WS_VISIBLE | WS_CHILD;
	if(m_style==TRUE)
		style|=DTS_SHORTDATEFORMAT|m_updown?DTS_UPDOWN:0;
	else
		style|=DTS_TIMEFORMAT|DTS_UPDOWN;

	pWndDateTime->Create(style, rectEdit, m_pWndList, AFX_PROPLIST_ID_INPLACE);

	if(!m_setformat.IsEmpty())
		pWndDateTime->SetFormat(m_setformat);

	pWndDateTime->SetTime(m_varValue);

	bDefaultFormat = TRUE;
	return pWndDateTime;
}

//=============================================================================
// Method		: CMFCPropertyGridDateTimeProperty::OnUpdateValue
// Access		: public 
// Returns		: BOOL
// Qualifier	:
// Last Update	: 2010/10/5 - 11:52
// Desc.		:
//=============================================================================
BOOL CMFCPropertyGridDateTimeProperty::OnUpdateValue()
{
	ASSERT_VALID(this);
	ASSERT_VALID(m_pWndInPlace);
	ASSERT_VALID(m_pWndList);
	ASSERT(::IsWindow(m_pWndInPlace->GetSafeHwnd()));

	COleDateTime lCurrValue = COleDateTime(m_varValue.date);

	CDateTimeCtrl* pProp = (CDateTimeCtrl*) m_pWndInPlace;

	COleDateTime datetime;
	pProp->GetTime(datetime);
	m_varValue.date = datetime;

	if (lCurrValue != COleDateTime(m_varValue.date))
		m_pWndList->OnPropertyChanged(this);

	return TRUE;
}

//=============================================================================
// Method		: CMFCPropertyGridDateTimeProperty::IsValueChanged
// Access		: protected 
// Returns		: BOOL
// Qualifier	: const
// Last Update	: 2010/10/5 - 11:52
// Desc.		:
//=============================================================================
BOOL CMFCPropertyGridDateTimeProperty::IsValueChanged() const
{
	ASSERT_VALID(this);

	if (m_varValueOrig.vt != m_varValue.vt)
		return FALSE;

	const COleDateTime var(m_varValue);
	const COleDateTime var1(m_varValueOrig);

	if(m_varValue.vt==VT_DATE)
		return var != var1;

	return FALSE;
}

//=============================================================================
// Method		: CMFCPropertyGridDateTimeProperty::FormatProperty
// Access		: public 
// Returns		: CString
// Qualifier	:
// Last Update	: 2010/10/5 - 11:52
// Desc.		:
//=============================================================================
CString CMFCPropertyGridDateTimeProperty::FormatProperty()
{
	CString strVal;
	if(m_format.IsEmpty())
	{
		setlocale(LC_ALL, "");
		strVal = COleDateTime(m_varValue).Format(m_style?_T("%x"):_T("%X"));
	}
	else
		strVal = COleDateTime(m_varValue).Format(m_format);

	return strVal;
}

//=============================================================================
// Method		: CMFCPropertyGridDateTimeProperty::FormatOriginalProperty
// Access		: public 
// Returns		: CString
// Qualifier	:
// Last Update	: 2010/10/5 - 11:52
// Desc.		:
//=============================================================================
CString CMFCPropertyGridDateTimeProperty::FormatOriginalProperty()
{
	CString strVal;
	if(m_format.IsEmpty())
	{
		setlocale(LC_ALL, "");
		strVal = COleDateTime(m_varValueOrig).Format(m_style?_T("%x"):_T("%X"));
	}
	else
		strVal = COleDateTime(m_varValueOrig).Format(m_format);

	return strVal;
}

//=============================================================================
// CPropIPAddressCtrl
//=============================================================================
CPropIPAddressCtrl::CPropIPAddressCtrl(CMFCPropertyGridIPAdressProperty *pProp, COLORREF clrBack)
{
	m_clrBack = clrBack;
	m_brBackground.CreateSolidBrush(m_clrBack);
	m_pProp = pProp;
	m_bSetFont=TRUE;
	m_wFiled	= 0;
}

BEGIN_MESSAGE_MAP(CPropIPAddressCtrl,CIPAddressCtrl)
	ON_WM_KILLFOCUS()
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_HSCROLL_REFLECT()
	ON_WM_CREATE()
END_MESSAGE_MAP()

//=============================================================================
// Method		: CPropIPAddressCtrl::OnKillFocus
// Access		: protected 
// Returns		: void
// Parameter	: CWnd * pNewWnd
// Qualifier	:
// Last Update	: 2010/10/5 - 11:52
// Desc.		:
//=============================================================================
void CPropIPAddressCtrl::OnKillFocus(CWnd* pNewWnd)
{
	if(pNewWnd!=NULL && IsChild(pNewWnd))
		return;
	if(pNewWnd==this)
		return;
	CIPAddressCtrl::OnKillFocus(pNewWnd);
}

//=============================================================================
// Method		: CPropIPAddressCtrl::OnDestroy
// Access		: protected 
// Returns		: void
// Qualifier	:
// Last Update	: 2010/10/5 - 11:52
// Desc.		:
//=============================================================================
void CPropIPAddressCtrl::OnDestroy()
{
	m_bSetFont;
	SetFont(NULL,FALSE);
	CWnd *editCtrl=NULL,*editTmp=NULL;
	editCtrl=GetWindow(GW_CHILD);
	while(editCtrl)
	{
		editTmp=editCtrl;
		editCtrl=editCtrl->GetWindow(GW_HWNDNEXT);
		//		editTmp->UnsubclassWindow();
		editTmp->DestroyWindow();
	}

	CIPAddressCtrl::OnDestroy();
}

//=============================================================================
// Method		: CPropIPAddressCtrl::CtlColor
// Access		: protected 
// Returns		: HBRUSH
// Parameter	: CDC * pDC
// Parameter	: UINT
// Qualifier	:
// Last Update	: 2010/10/5 - 11:52
// Desc.		:
//=============================================================================
HBRUSH CPropIPAddressCtrl::CtlColor(CDC* pDC, UINT /*nCtlColor*/)
{
	pDC->SetBkColor(m_clrBack);
	return m_brBackground;
}

//=============================================================================
// Method		: CPropIPAddressCtrl::HScroll
// Access		: protected 
// Returns		: void
// Parameter	: UINT
// Parameter	: UINT
// Qualifier	:
// Last Update	: 2010/10/5 - 11:52
// Desc.		:
//=============================================================================
void CPropIPAddressCtrl::HScroll(UINT /*nSBCode*/, UINT /*nPos*/)
{
	ASSERT_VALID(m_pProp);

	m_pProp->OnUpdateValue();
	m_pProp->Redraw();
}

//=============================================================================
// Method		: CPropIPAddressCtrl::WindowProc
// Access		: virtual protected 
// Returns		: LRESULT
// Parameter	: UINT message
// Parameter	: WPARAM wParam
// Parameter	: LPARAM lParam
// Qualifier	:
// Last Update	: 2010/10/5 - 11:52
// Desc.		:
//=============================================================================
LRESULT CPropIPAddressCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if(message>=WM_MOUSEFIRST && message<=WM_MOUSELAST)
	{
		m_wFiled = 0;
		POINT pt;
		GetCursorPos(&pt);
		CWnd *edit=NULL;
		CRect rect;
		edit=GetWindow(GW_CHILD);
		while(edit)
		{
			edit->GetWindowRect(rect);
			if(rect.PtInRect(pt))
			{
				edit->SendMessage(message,wParam,lParam);
				return TRUE;
			}
			edit=edit->GetWindow(GW_HWNDNEXT);
		}
	}
	if(message==WM_SETFONT && !m_bSetFont)
		return TRUE;

	return CIPAddressCtrl::WindowProc(message, wParam, lParam);
}

//=============================================================================
// Method		: CPropIPAddressCtrl::OnCreate
// Access		: public 
// Returns		: int
// Parameter	: LPCREATESTRUCT lpCreateStruct
// Qualifier	:
// Last Update	: 2010/10/5 - 11:52
// Desc.		:
//=============================================================================
int CPropIPAddressCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CIPAddressCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	int i=0;
	CWnd *edit=NULL;
	edit=GetWindow(GW_CHILD);
	while(edit && i<4)
	{
		m_IPEdit[i++].SubclassWindow(edit->GetSafeHwnd());
		edit=edit->GetWindow(GW_HWNDNEXT);
	}

	return 0;
}

BOOL CPropIPAddressCtrl::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_TAB)
	{
		if (3 == m_wFiled)
			m_wFiled = 0;
		else
			++m_wFiled;

		SetFieldFocus(m_wFiled);

		return TRUE;
	}

	return CIPAddressCtrl::PreTranslateMessage(pMsg);
}

//=============================================================================
// CMFCPropertyGridDateTimeProperty
//=============================================================================
IMPLEMENT_DYNAMIC(CMFCPropertyGridIPAdressProperty,CMFCPropertyGridProperty)

CMFCPropertyGridIPAdressProperty::CMFCPropertyGridIPAdressProperty(const CString& strName, in_addr &nValue, LPCTSTR lpszDescr, DWORD dwData):
CMFCPropertyGridProperty(strName, ULONG_VARIANT(nValue.s_addr), lpszDescr, dwData)
{
}

//=============================================================================
// Method		: CMFCPropertyGridIPAdressProperty::CreateInPlaceEdit
// Access		: virtual protected 
// Returns		: CWnd*
// Parameter	: CRect rectEdit
// Parameter	: BOOL & bDefaultFormat
// Qualifier	:
// Last Update	: 2010/10/5 - 11:52
// Desc.		:
//=============================================================================
CWnd* CMFCPropertyGridIPAdressProperty::CreateInPlaceEdit(CRect rectEdit, BOOL& bDefaultFormat)
{
	CPropIPAddressCtrl* pWndIPAddress = new CPropIPAddressCtrl(this,m_pWndList->GetBkColor());

	rectEdit.InflateRect(4,2,0,3);

	DWORD style=WS_VISIBLE | WS_CHILD;

	pWndIPAddress->Create(style, rectEdit, m_pWndList, AFX_PROPLIST_ID_INPLACE);
	//pWndIPAddress->SetFont(m_pWndList->GetFont(),FALSE);
	pWndIPAddress->EnableSetFont(FALSE);
	pWndIPAddress->SetAddress(m_varValue.ulVal);

	bDefaultFormat = TRUE;
	return pWndIPAddress;
}

//=============================================================================
// Method		: CMFCPropertyGridIPAdressProperty::OnUpdateValue
// Access		: public 
// Returns		: BOOL
// Qualifier	:
// Last Update	: 2010/10/5 - 11:52
// Desc.		:
//=============================================================================
BOOL CMFCPropertyGridIPAdressProperty::OnUpdateValue()
{
	ASSERT_VALID(this);
	ASSERT_VALID(m_pWndInPlace);
	ASSERT_VALID(m_pWndList);
	ASSERT(::IsWindow(m_pWndInPlace->GetSafeHwnd()));

	in_addr lCurrValue;
	lCurrValue.s_addr = m_varValue.ulVal;

	CPropIPAddressCtrl* pProp = (CPropIPAddressCtrl*) m_pWndInPlace;

	pProp->GetAddress(m_varValue.ulVal);
	m_varValue.ulVal=ntohl(m_varValue.ulVal);

	if (lCurrValue.s_addr != m_varValue.ulVal)
		m_pWndList->OnPropertyChanged(this);

	return TRUE;
}

//=============================================================================
// Method		: CMFCPropertyGridIPAdressProperty::FormatProperty
// Access		: public 
// Returns		: CString
// Qualifier	:
// Last Update	: 2010/10/5 - 11:52
// Desc.		:
//=============================================================================
CString CMFCPropertyGridIPAdressProperty::FormatProperty()
{
	CString strVal;

	in_addr address;
	address.s_addr=m_varValue.ulVal;
	strVal=inet_ntoa(address);

	return strVal;
}

//=============================================================================
// Method		: CMFCPropertyGridIPAdressProperty::FormatOriginalProperty
// Access		: public 
// Returns		: CString
// Qualifier	:
// Last Update	: 2010/10/5 - 11:51
// Desc.		:
//=============================================================================
CString CMFCPropertyGridIPAdressProperty::FormatOriginalProperty()
{
	CString strVal;

	in_addr address;
	address.s_addr=m_varValueOrig.ulVal;
	strVal=inet_ntoa(address);

	return strVal;
}

