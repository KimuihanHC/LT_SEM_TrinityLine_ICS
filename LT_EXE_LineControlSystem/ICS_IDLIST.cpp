// ICS_IDLIST.cpp: 구현 파일
//

#include "stdafx.h"
#include "resource.h"
#include "ICS_IDLIST.h"


// CICS_IDLIST

IMPLEMENT_DYNCREATE(CICS_IDLIST, CFormView)

CICS_IDLIST::CICS_IDLIST()
	: CFormView(IDD_FORMVIEW_ID_LIST)
{

}

CICS_IDLIST::~CICS_IDLIST()
{
}

void CICS_IDLIST::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CICS_IDLIST, CFormView)
END_MESSAGE_MAP()


// CICS_IDLIST 진단

#ifdef _DEBUG
void CICS_IDLIST::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CICS_IDLIST::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CICS_IDLIST 메시지 처리기
