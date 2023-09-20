// MainFrmV2.cpp: 구현 파일
//

#include "stdafx.h"
#include "MainFrmV2.h"
#include "resource.h"
//#include "NTVisualManager.h"
#include "ModuleView.h"
#include "Def.h"
#include "LT MainExecution.h"

// CMainFrmV2

IMPLEMENT_DYNCREATE(CMainFrmV2, CFrameWndEx)

CMainFrmV2::CMainFrmV2() {
}
CMainFrmV2::CMainFrmV2(int nInt)
{	
	switch(nInt){
	case MDL_MAX:
		for (int i = 0; i < MDL_MAX; i++) {
			FV[i].nID = IDD_MODULE_VIEW + i;
			FV[i].dwStyle = WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
			FV[i].pParentWnd = this;
			FV[i].rt.SetRectEmpty();
		}
		break;
	default:
		FV[nInt].nID = IDD_MODULE_VIEW + nInt;
		FV[nInt].dwStyle = WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
		FV[nInt].pParentWnd = this;
		FV[nInt].rt.SetRectEmpty();
		break;
	}

}
CMainFrmV2::~CMainFrmV2()
{
}

int CMainFrmV2::Check_Form(formViewData *m) {
	int nRslt = FORM_DUMMY;
	CWnd *pWnd = GetDlgItem(m->nID);
	if (m->pMV == nullptr) {
		nRslt = FORM_NULL;
	}
	else {
		if (pWnd != nullptr) {
			nRslt = FORM_CREATE;
		}
		else {
			nRslt = FORM_NEW;
		}
	}
	return  nRslt;
}
int CMainFrmV2::New_Form(formViewData *m) {
	int nRslt = RSLT_FAIL;
	if (m->pMV == nullptr) {
		m->pMV = new CModuleView(m->nID, &nRslt);
	}
	return  nRslt;
}
int CMainFrmV2::Create_Form(formViewData *m) {
	int nRslt = RSLT_FAIL;
	bool bRslt = false;
	if (m->pMV->GetSafeHwnd() == nullptr) {		
		bRslt = m->pMV->Create(m->lpszClassName, m->lpszWindowName, m->dwStyle, m->rt, m->pParentWnd, m->nID, NULL);
		if (bRslt) {
			nRslt = RSLT_OK;
		}
		else {
			nRslt = RSLT_FAIL;
		}
	}
	else {
		nRslt = RSLT_NG;
	}
	return  nRslt;
}
int CMainFrmV2::Delete_Form(formViewData *m) {
	int nRslt = RSLT_FAIL;
	if (m->pMV == nullptr) {
		delete *m->pMV;
		m->pMV = NULL;
	}
	return  nRslt;
}

BEGIN_MESSAGE_MAP(CMainFrmV2, CFrameWndEx)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_CLOSE()
	ON_WM_ACTIVATE()
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_WM_SETTINGCHANGE()
	ON_COMMAND(ID_CAPTION_DELETE, &CMainFrmV2::OnCaptionDelete)
	ON_COMMAND(ID_CAPTION_SHOW, &CMainFrmV2::OnCaptionShow)
END_MESSAGE_MAP()


// CMainFrmV2 메시지 처리기


int CMainFrmV2::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	//HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	//SetIcon(hIcon, FALSE);

	//CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
	//CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CNTVisualManager));
	//CMFCToolBarButton::m_bWrapText = FALSE;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	FV[MDL_T].rt.left = 0;
	FV[MDL_T].rt.top  = 0;
	FV[MDL_T].rt.right = 1280;
	FV[MDL_T].rt.bottom = 1024;

	int nRslt = RSLT_FAIL;
	int nState = FORM_DUMMY;
	nState = Check_Form(&FV[MDL_T]);
	switch (nState) {
	case FORM_NULL:
		nRslt = New_Form(&FV[MDL_T]);
		switch (nRslt) {
		case  RSLT_OK:
			nRslt = Create_Form(&FV[MDL_T]);
			break;
		}
		break;
	}
	return 0;
}


void CMainFrmV2::OnSetFocus(CWnd* pOldWnd)
{
	CFrameWndEx::OnSetFocus(pOldWnd);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CMainFrmV2::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CFrameWndEx::OnClose();
}


void CMainFrmV2::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CFrameWndEx::OnActivate(nState, pWndOther, bMinimized);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CMainFrmV2::OnSize(UINT nType, int cx, int cy)
{
	CFrameWndEx::OnSize(nType, cx, cy);	

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CMainFrmV2::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CFrameWndEx::OnGetMinMaxInfo(lpMMI);
}


void CMainFrmV2::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CFrameWndEx::OnSettingChange(uFlags, lpszSection);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CMainFrmV2::OnCaptionDelete()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	//ModifyStyle(WS_CAPTION | FWS_ADDTOTITLE, 0);
	//ModifyStyleEx(WS_EX_CLIENTEDGE, 0);
}


void CMainFrmV2::OnCaptionShow()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	//ModifyStyle(WS_OVERLAPPED | FWS_ADDTOTITLE, 0);
	//ModifyStyleEx(WS_EX_CLIENTEDGE, 0);
}
