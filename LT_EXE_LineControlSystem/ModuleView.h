#pragma once

#include "Def.h"
#include "GdCtrl.h"
#include "ModuleView.h"

// CModuleView 폼 보기

struct CGrdCtrl {
	CGdCtrl	*pGCC = nullptr;
	CMFCPropertyGridCtrl *pGC = nullptr;
	int ID = NULL;
};

class CModuleView : public CFormView
{
	DECLARE_DYNCREATE(CModuleView)

protected:
	
	virtual ~CModuleView();

public:	
	int nID = NULL;
	LONG_PTR pr = NULL;
	bool bUse = false;

	CModuleView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	CModuleView(int ID, int *Rslt); 

	CGrdCtrl	CGC[GC_MAX];
	int New_GC(CGrdCtrl	*m);

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MODULE_VIEW };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
//	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
protected:
	afx_msg LRESULT OnPropertyChanged(WPARAM wParam, LPARAM lParam);
public:
	virtual void OnInitialUpdate();
};

extern CModuleView* pMV;