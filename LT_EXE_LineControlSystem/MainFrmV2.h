#pragma once

#include "ModuleView.h"

// CMainFrmV2 프레임
struct formViewData{
	CModuleView	*pMV	= nullptr;	
	UINT		nID		= 0;
	CRect	rt;
	LPCTSTR	lpszClassName = _T("");
	LPCTSTR	lpszWindowName = _T("");
	DWORD	dwStyle = WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	CWnd*	pParentWnd = nullptr;
	CCreateContext* pContext = nullptr;
};

class CMainFrmV2 : public CFrameWndEx
{
	DECLARE_DYNCREATE(CMainFrmV2)
protected:
	virtual ~CMainFrmV2();
public:
	CMainFrmV2();
	CMainFrmV2(int nInt);           // 동적 만들기에 사용되는 protected 생성자입니다.
	formViewData FV[MDL_MAX];
	int Check_Form(formViewData *m);
	int New_Form(formViewData *m);
	int Create_Form(formViewData *m);
	int Delete_Form(formViewData *m);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnClose();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg void OnCaptionDelete();
	afx_msg void OnCaptionShow();
};


