//*****************************************************************************
// Filename	: SheetOption.h
// Created	: 2010/8/27
// Modified	: 2013/2/20 - 15:53
//
// Author	: PiRing
//	
// Purpose	: 
//*****************************************************************************
#ifndef SheetOption_h__
#define SheetOption_h__


#pragma once

#include "LT_Option.h"
#include "Def_Enum_Cm.h"

typedef struct _stPropertyPage_ID
{
	UINT nIDTemplate;
	UINT nIDCaption;
}stPropertyPage_ID;

//=============================================================================
// CSheetOption
//=============================================================================
class CSheetOption : public CMFCPropertySheet
{
	DECLARE_DYNAMIC(CSheetOption)

public:	
	CSheetOption						();
	CSheetOption						(UINT nIDCaption, CWnd* pParentWnd = NULL);
	virtual ~CSheetOption				();

protected:

	afx_msg void	OnSize				(UINT nType, int cx, int cy);
	afx_msg void	OnBnClickedApplyNow ();
	afx_msg void	OnOK				();
	afx_msg void	OnCancel			();

	virtual BOOL	OnInitDialog		();
	virtual void	OnDrawPageHeader	(CDC* pDC, int nPage, CRect rectHeader);
	DECLARE_MESSAGE_MAP()

	afx_msg LRESULT OnPropertyChanged	(WPARAM wParam, LPARAM lParam);

	CStringArray			m_strCategoryName;
	CArray<void*, void*>	m_ptrPage;

	CLT_Option				m_LT_Option;

	BOOL					m_bItemChanged;
	BOOL					m_bSavedOption;	// 변경된 내용이 저장되었는지 판단하는 플래그
	UINT					m_WMItemChanged;// 옵션이 변경되었음을 알리는 윈도우 메세지

	enInsptrSysType			m_InsptrType;

public:

	void			SetRegistryPath		(__in LPCTSTR lpszRegPath)
	{
		m_LT_Option.SetRegistryPath(lpszRegPath);
	};

	// 내용이 변경되면 메인윈도우로 변경되었음을 알리는 메세지를 통보한다. (2013.02.20)
	void			SetWM_ItemChanged	(__in UINT nWindowMessage)
	{
		m_WMItemChanged = nWindowMessage;
	};
		
	virtual void	InitPage			();
	virtual void	InitPage			(__in UINT nIDTemplate, __in CList<UINT, UINT> &listPageID);
		
	// 저장, 불러오기
	void			SaveOption			();
	void			LoadOption			();
	
	// 검사기 구분
	void			SetInspectorType	(__in enInsptrSysType nInsptrType);

};


#endif // SheetOption_h__
