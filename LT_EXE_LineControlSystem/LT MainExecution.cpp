
// LT MainExecution.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "LT MainExecution.h"
#include "MainFrm.h"
#include "LimitSingleInstance.h"
#include "Rockey.h"


#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//#define DEBUG_NORMALBLOCK new (_NORMAL_BLOCK, __FILE__, __LINE__)
//#ifdef new
//#undef new
//#endif
//#define new DEBUG_NORMALBLOCK

//=============================================================================
// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.
//=============================================================================
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
//	afx_msg LRESULT OnPropertyChanged(WPARAM wParam, LPARAM lParam);
public:
//	virtual BOOL Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
//	virtual BOOL OnInitDialog();
//	afx_msg void OnClose();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
//	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, &CAboutDlg::OnPropertyChanged)
//ON_WM_CLOSE()
END_MESSAGE_MAP()

//=============================================================================
// CNTInspectionSystemApp
//=============================================================================
BEGIN_MESSAGE_MAP(CNTInspectionSystemApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CNTInspectionSystemApp::OnAppAbout)
END_MESSAGE_MAP()


// CNTInspectionSystemApp 생성
//=============================================================================
//
//=============================================================================
CNTInspectionSystemApp::CNTInspectionSystemApp()
{
	// Registry의 workspace에 데이터 저장 안함
	m_bSaveState = FALSE;

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_DEBUG);
	//_CrtDumpMemoryLeaks();
		
	//_CrtSetBreakAlloc(295);
	//_CrtMemDumpAllObjectsSince(0);

	m_bHiColorIcons = TRUE;

	Gdiplus::GdiplusStartup(&m_gdiplusToken, &m_gdiplusStartupInput, NULL);
}

//=============================================================================
//
//=============================================================================
CNTInspectionSystemApp::~CNTInspectionSystemApp()
{
	TRACE(_T("<<< Start ~CNTInspectionSystemApp >>> \n"));

	Gdiplus::GdiplusShutdown(m_gdiplusToken);

	//_CrtSetBreakAlloc(295);
	_CrtDumpMemoryLeaks();

	TRACE(_T("<<< End ~CNTInspectionSystemApp >>> \n"));
}

// 유일한 CNTInspectionSystemApp 개체입니다.

CNTInspectionSystemApp theApp;
CLimitSingleInstance g_SingleInstanceObj(TEXT("Global\\{080F0A26-E85C-4D12-AF6D-4C0879802237}"));

// CNTInspectionSystemApp 초기화

//=============================================================================
// Method		: InitInstance
// Access		: virtual public  
// Returns		: BOOL
// Qualifier	:
// Last Update	: 2016/9/22 - 14:32
// Desc.		:
//=============================================================================
BOOL CNTInspectionSystemApp::InitInstance()
{
 #ifndef USE_ANOTHER_INSTANCE_APP
 	if (g_SingleInstanceObj.IsAnotherInstanceRunning())
 		return FALSE; 
 #endif

#ifdef USE_HW_LOCK_KEY
	// 락키 체크용 루틴
	CRockey Rockey;
	if (!Rockey.CheckRockey(ROCKEY_MODE0))
	{
	 	return FALSE;
	}
#endif

	SetLFH(); // Low fragmentation Heap 적용


	CWinAppEx::InitInstance();

 	if (!AfxSocketInit())
 	{
 		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
 		return FALSE;
 	}

	// OLE 라이브러리를 초기화합니다.
// 	if (!AfxOleInit())
// 	{
// 		LT_MessageBox(IDP_OLE_INIT_FAILED);
// 		return FALSE;
// 	}
	AfxEnableControlContainer();

	SetRegistryKey(_T("Luritech"));

	m_pszAppName = _tcsdup(_T("In-Line Control System"));

	// 주 창을 만들기 위해 이 코드에서는 새 프레임 창 개체를
	// 만든 다음 이를 응용 프로그램의 주 창 개체로 설정합니다.
		CMainFrame* pFrame = new CMainFrame;

		if (!pFrame)
			return FALSE;
		m_pMainWnd = pFrame;
#ifdef _DEBUG
		pFrame->LoadFrame(IDR_MAINFRAME, WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL, NULL);
#else
		//pFrame->LoadFrame(IDR_MAINFRAME, WS_OVERLAPPED | WS_SYSMENU | WS_THICKFRAME | WS_BORDER | WS_MINIMIZEBOX | WS_MAXIMIZEBOX, NULL, NULL);
		pFrame->LoadFrame(IDR_MAINFRAME, WS_OVERLAPPED | WS_SYSMENU | WS_THICKFRAME | WS_BORDER | WS_MINIMIZEBOX | WS_MAXIMIZEBOX & ~FWS_ADDTOTITLE & ~WS_CAPTION, NULL, NULL);
#endif

		// 창 하나만 초기화되었으므로 이를 표시하고 업데이트합니다.
#ifdef _DEBUG
		pFrame->ShowWindow(SW_SHOWDEFAULT);
#else
		pFrame->ShowWindow(SW_SHOWMAXIMIZED);
#endif

		pFrame->UpdateWindow();

		// 프로그램 로딩이 끝났을을 메세지로 알림??
		pFrame->PostMessage(WM_LOAD_COMPLETE, 0, 0);



	return TRUE;
	// 프레임을 만들어 리소스와 함께 로드합니다.

}

// CNTInspectionSystemApp 메시지 처리기

// 대화 상자를 실행하기 위한 응용 프로그램 명령입니다.
void CNTInspectionSystemApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CNTInspectionSystemApp 사용자 지정 로드/저장 메서드

void CNTInspectionSystemApp::PreLoadState()
{
	/*BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);*/
}

void CNTInspectionSystemApp::LoadCustomState()
{
}

void CNTInspectionSystemApp::SaveCustomState()
{
}

void CNTInspectionSystemApp::SetLFH()
{
	HANDLE Heaps[1024];
	DWORD Count = GetProcessHeaps( 1024, Heaps );
	for( DWORD i = 0; i < Count; ++i )
	{
		ULONG  HeapFragValue = 2;

		if( HeapSetInformation( Heaps[i], HeapCompatibilityInformation, &HeapFragValue, sizeof(HeapFragValue) ) )
		{
			//.............
		}
		else 
		{
			//...................
		}
	}
}
// CNTInspectionSystemApp 메시지 처리기





//afx_msg LRESULT CAboutDlg::OnPropertyChanged(WPARAM wParam, LPARAM lParam)
//{
//	return 0;
//}


//BOOL CAboutDlg::Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd)
//{
//	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
//
//	return CDialog::Create(lpszTemplateName, pParentWnd);
//}


//BOOL CAboutDlg::OnInitDialog()
//{
//	CDialog::OnInitDialog();
//
//	// TODO:  여기에 추가 초기화 작업을 추가합니다.
//
//	return TRUE;  // return TRUE unless you set the focus to a control
//				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
//}


//void CAboutDlg::OnClose()
//{
//	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
//
//	CDialog::OnClose();
//}
