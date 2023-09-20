// ModuleView.cpp: 구현 파일
//

#include "stdafx.h"
#include "resource.h"
#include "ModuleView.h"
#include "Comm/IcsTcpServer.h"

// CModuleView

IMPLEMENT_DYNCREATE(CModuleView, CFormView)

CModuleView* pMV;

CModuleView::CModuleView()
	: CFormView(IDD_MODULE_VIEW)
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif

}
CModuleView::CModuleView(int ID, int *Rslt)
	: CFormView(IDD_MODULE_VIEW) {
	nID = ID;
	*Rslt = RSLT_OK;
}
CModuleView::~CModuleView()
{

}

int CModuleView::New_GC(CGrdCtrl	*m) {
	int nRslt = RSLT_FAIL;
	if (m->pGCC == nullptr) {
		m->pGCC = new CGdCtrl(m->ID, &nRslt);
	}
	else {
		nRslt = RSLT_NG;
	}
	return nRslt;
}
void CModuleView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CModuleView, CFormView)
//	ON_WM_CREATE()
ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, &CModuleView::OnPropertyChanged)
END_MESSAGE_MAP()


// CModuleView 진단

#ifdef _DEBUG
void CModuleView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CModuleView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CModuleView 메시지 처리기
BOOL CModuleView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	return CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}
void CModuleView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}

afx_msg LRESULT CModuleView::OnPropertyChanged(WPARAM wParam, LPARAM lParam)
{
	CStringA strText = "";
	CStringA strName;
	CMFCPropertyGridProperty  *pProp = (CMFCPropertyGridProperty*)lParam;
	strName = CStringA(pProp->GetName());
	return 0;
}



