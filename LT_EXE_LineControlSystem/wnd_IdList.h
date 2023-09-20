#ifndef wnd_IdList_h__
#define wnd_IdList_h__

#pragma once

// Cwnd_IdList 폼 보기

class Cwnd_IdList : public CFormView
{
	DECLARE_DYNCREATE(Cwnd_IdList)

//protected:
public:
	Cwnd_IdList();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~Cwnd_IdList();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FORMVIEW_ID_LIST };
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
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	virtual void OnInitialUpdate();
};

extern Cwnd_IdList* pwndIdList;

#endif wnd_IdList_h__