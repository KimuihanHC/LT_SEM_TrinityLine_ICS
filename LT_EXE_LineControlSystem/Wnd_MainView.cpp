//*****************************************************************************
// Filename	: Wnd_MainView.cpp
// Created	: 2021/11/22 - 12:29
// Modified	: 2021/11/22 - 12:29
//
// Author	: PiRing
//	
// Purpose	: 
//*****************************************************************************
// Wnd_MainView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Wnd_MainView.h"
#include "resource.h"
#include "Dlg_ChkPassword.h"

#if defined(EES_XML)
#include "Pane_CommStatus.h"
#endif

#define IDC_BN_START_INIT	2001
#define IDC_BN_START_FINAL	2002
#define IDC_BN_LOAD_INITIAL	2003


//=============================================================================
// CWnd_MainView
//=============================================================================
IMPLEMENT_DYNAMIC(CWnd_MainView, CWnd_BaseView)

CWnd_MainView::CWnd_MainView()
{
	VERIFY(m_font_Default.CreateFont(
		13,						// nHeight
		0,						// nWidth
		0,						// nEscapement
		0,						// nOrientation
		FW_NORMAL,				// nWeight
		FALSE,					// bItalic
		FALSE,					// bUnderline
		0,						// cStrikeOut
		DEFAULT_CHARSET,		// nCharSet
		OUT_DEFAULT_PRECIS,		// nOutPrecision
		CLIP_DEFAULT_PRECIS,	// nClipPrecision
		DEFAULT_QUALITY,		// nQuality
		DEFAULT_PITCH,			// nPitchAndFamily
		_T("Tahoma")));			// lpszFacename
	
	VERIFY(m_Font.CreateFont(
		13,						// nHeight
		0,						// nWidth
		0,						// nEscapement
		0,						// nOrientation
		FW_NORMAL,				// nWeight
		FALSE,					// bItalic
		FALSE,					// bUnderline
		0,						// cStrikeOut
		DEFAULT_CHARSET,		// nCharSet
		OUT_DEFAULT_PRECIS,		// nOutPrecision
		CLIP_DEFAULT_PRECIS,	// nClipPrecision
		DEFAULT_QUALITY,		// nQuality
		DEFAULT_PITCH,			// nPitchAndFamily
		_T("Tahoma")));			// lpszFacename

	m_PermissionMode		= Permission_Operator;
	m_pstSysInfo	= NULL;
}

CWnd_MainView::~CWnd_MainView()
{
	TRACE(_T("<<< Start ~CWnd_MainView >>> \n"));
	
	m_font_Default.DeleteObject();
	m_Font.DeleteObject();
}

BEGIN_MESSAGE_MAP(CWnd_MainView, CWnd_BaseView)
	ON_WM_CREATE	()
	ON_WM_SIZE		()
END_MESSAGE_MAP()


//=============================================================================
// CWnd_MainView 메시지 처리기입니다.
//=============================================================================
//=============================================================================
// Method		: OnCreate
// Access		: protected  
// Returns		: int
// Parameter	: LPCREATESTRUCT lpCreateStruct
// Qualifier	:
// Last Update	: 2021/11/22 - 12:29
// Desc.		:
//=============================================================================
int CWnd_MainView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd_BaseView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	SetBackgroundColor(RGB(0, 0, 0));

	DWORD dwStyle = WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	CRect rectDummy;
	rectDummy.SetRectEmpty();
	
	/*m_Font.CreateStockObject(DEFAULT_GUI_FONT);*/

	UINT nWindowsID = 100;

#if defined(EES_XML)//20231003
	m_wndSvrEES.Create(_T("STATIC"), _T(""), WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, rectDummy, this, nWindowsID++);
#endif

#ifndef USE_DYNAMIC_WND_CREATE

	m_wndEqpHandler.Create(_T("STATIC"), _T(""), WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, rectDummy, this, nWindowsID++);
	m_wndEqpLoader.Create(_T("STATIC"), _T(""), WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, rectDummy, this, nWindowsID++);
	m_wndEqpReturner.Create(_T("STATIC"), _T(""), WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, rectDummy, this, nWindowsID++);

	for (auto nIdx = 0; nIdx < MAX_EQUIPMENT_COUNT; ++nIdx)
	{
		if (!m_wndEqpTesterList[nIdx].Create(_T("STATIC"), _T(""), WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, rectDummy, this, nWindowsID++))
		{
			TRACE0("Failed to create CWnd_Status_Equipment\n");
		}
	}

#endif

	m_wndTestSelect.SetOwner(GetParent());
	if (!m_wndTestSelect.Create(_T("STATIC"), _T(""), WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, rectDummy, this, nWindowsID++))
	{
		TRACE0("Failed to create m_wndTestSelect\n");
	}

	return 0;
}

//=============================================================================
// Method		: OnSize
// Access		: protected  
// Returns		: void
// Parameter	: UINT nType
// Parameter	: int cx
// Parameter	: int cy
// Qualifier	:
// Last Update	: 2021/11/22 - 12:29
// Desc.		:
//=============================================================================
void CWnd_MainView::OnSize(UINT nType, int cx, int cy)
{
	CWnd_BaseView::OnSize(nType, cx, cy);

	if ((cx == 0) && (cy == 0))
		return;

 	int iMagrin			= 5;

	int iLeft			= iMagrin;
	int iTop			= iMagrin;
	int iWidth			= cx - iMagrin - iMagrin;
	int iHeight			= cy - iMagrin - iMagrin;	

	MoveWindow_Equipment(0, iLeft, iTop, iWidth, 400);

	int nUnitHeight = 190;
	iTop = cy - nUnitHeight - iMagrin;
	m_wndTestSelect.MoveWindow(iLeft, iTop, iWidth, nUnitHeight);

}

//=============================================================================
// Method		: MoveWindow_TestSelect
// Access		: protected  
// Returns		: void
// Parameter	: int x
// Parameter	: int y
// Parameter	: int nWidth
// Parameter	: int nHeight
// Parameter	: BOOL bRepaint
// Qualifier	:
// Last Update	: 2021/11/22 - 12:23
// Desc.		:
//=============================================================================
void CWnd_MainView::MoveWindow_TestSelect(int x, int y, int nWidth, int nHeight, BOOL bRepaint /*= TRUE*/)
{
	
}

//=============================================================================
// Method		: MoveWindow_Equipment
// Access		: protected  
// Returns		: void
// Parameter	: UINT nEquipmentIdx
// Parameter	: int x
// Parameter	: int y
// Parameter	: int nWidth
// Parameter	: int nHeight
// Parameter	: BOOL bRepaint
// Qualifier	:
// Last Update	: 2021/11/22 - 12:23
// Desc.		:
//=============================================================================
void CWnd_MainView::MoveWindow_Equipment(UINT nEquipmentIdx, int x, int y, int nWidth, int nHeight, BOOL bRepaint /*= TRUE*/)
{
	int nMagrin = 5;
	int nSpacing = 5;
	int nLeft = x;
	int nTop = y;	
	int nUnitWidth = (nWidth - (nSpacing * (m_nMaxCol - 1))) / m_nMaxCol;

#ifdef USE_EQP_TACTTIME
	int nUnitHeight = 570;// 320;
#else
	int nUnitHeight = 520;// 320;
#endif

	// Handler
	if (m_bUseHandler)
	{
		m_wndEqpHandler.MoveWindow(nLeft, nTop, nUnitWidth, nUnitHeight);
		nLeft += nUnitWidth + nSpacing ;
	}
	else
	{
		m_wndEqpHandler.MoveWindow(0, 0, 0, 0);
	}

	// Loader
	m_wndEqpLoader.MoveWindow(nLeft, nTop, nUnitWidth + nSpacing + nUnitWidth, nUnitHeight);
	nLeft += nUnitWidth + nSpacing + nUnitWidth + nSpacing;

	// Tester
#ifdef USE_DYNAMIC_WND_CREATE
	size_t nTesterCount = m_wndEqpTesterList.size();
	if (0 < nTesterCount)
	{
		for (auto nIdx = 0; nIdx < m_wndEqpTesterList.size(); ++nIdx)
		{
			if (NULL != m_wndEqpTesterList.at(nIdx)->GetSafeHwnd())
			{
				if (::IsWindow(m_wndEqpTesterList.at(nIdx)->GetSafeHwnd()))
				{
					m_wndEqpTesterList.at(nIdx)->MoveWindow(nLeft, nTop, nUnitWidth, nUnitHeight);
					nLeft += nUnitWidth + nSpacing;
				}
			}
		}
	}
#else

	size_t nTesterCount = 0;
	if (nullptr != m_pstSysInfo)
	{
		//nTesterCount = m_pstSysInfo->SettingInfo.LineInfo.GetCount() - 2; // 로더, 리터너 제외
		nTesterCount = m_pstSysInfo->LineInfo.Get_TesterCount();
		nTesterCount = (nTesterCount <= MAX_EQUIPMENT_COUNT) ? nTesterCount : MAX_EQUIPMENT_COUNT;
		if (0 < nTesterCount)
		{
			for (auto nIdx = 0; nIdx < nTesterCount; ++nIdx)
			{
				m_wndEqpTesterList[nIdx].MoveWindow(nLeft, nTop, nUnitWidth, nUnitHeight);
				nLeft += nUnitWidth + nSpacing;
			}
		}
	}

	for (auto nIdx = nTesterCount; nIdx < MAX_EQUIPMENT_COUNT; ++nIdx)
	{
		m_wndEqpTesterList[nIdx].MoveWindow(0, 0, 0, 0);
	}

#endif

	// Returner
	m_wndEqpReturner.MoveWindow(nLeft, nTop, nUnitWidth, nUnitHeight);
}

//=============================================================================
// Method		: MoveWindow_EquipmentEx
// Access		: protected  
// Returns		: void
// Parameter	: UINT nEquipmentIdx
// Parameter	: int x
// Parameter	: int y
// Parameter	: int nWidth
// Parameter	: int nHeight
// Parameter	: BOOL bRepaint
// Qualifier	:
// Last Update	: 2022/1/20 - 15:39
// Desc.		:
//=============================================================================
void CWnd_MainView::MoveWindow_EquipmentEx(UINT nEquipmentIdx, int x, int y, int nWidth, int nHeight, BOOL bRepaint /*= TRUE*/)
{
	// 좌측 1920 모니터와 우측 1920 모니터 경계에 설비UI가 잘리지 않게 처리해야 한다.
	// 좌측 (로더 ~ 01 ~ 05) : 1920
	// 우측 (06 ~ 12) : 1920 - Pane 너비 (100)

	int nMagrin			= 5;
	int nSpacing		= 5;
	int nLeft			= x;
	int nTop			= y;
	int nR_Area_Width	= (nWidth ) - 1915;
	int nR_Area_Left	= x + 1920;
	int nUnitWidth		= (nR_Area_Width - (nSpacing * 6)) / 7;
	int nUnitHeight		= 700;//320;
	int nLoaderWidth	= 1910 - ((nUnitWidth + nSpacing) * 5);

	// Handler
	if (m_bUseHandler)
	{
		m_wndEqpHandler.MoveWindow(nLeft, nTop, nUnitWidth, nUnitHeight);
		nLeft += nUnitWidth + nSpacing;
	}
	else
	{
		m_wndEqpHandler.MoveWindow(0, 0, 0, 0);
	}

	// Loader
	m_wndEqpLoader.MoveWindow(nLeft, nTop, nLoaderWidth, nUnitHeight);
	nLeft += nLoaderWidth + nSpacing;

	// Tester
	size_t nTesterCount = 0;
	if (nullptr != m_pstSysInfo)
	{
		//nTesterCount = m_pstSysInfo->SettingInfo.LineInfo.GetCount() - 2; // 로더, 리터너 제외
		nTesterCount = m_pstSysInfo->LineInfo.Get_TesterCount();
		nTesterCount = (nTesterCount <= MAX_EQUIPMENT_COUNT) ? nTesterCount : MAX_EQUIPMENT_COUNT;
		if (0 < nTesterCount)
		{
			// 왼쪽
			for (auto nIdx = 0; nIdx < 5; ++nIdx)
			{
				m_wndEqpTesterList[nIdx].MoveWindow(nLeft, nTop, nUnitWidth, nUnitHeight);
				nLeft += nUnitWidth + nSpacing;
			}

			// 오른쪽
			//nLeft += 10;
			nLeft = nR_Area_Left;
			for (auto nIdx = 5; nIdx < nTesterCount; ++nIdx)
			{
				m_wndEqpTesterList[nIdx].MoveWindow(nLeft, nTop, nUnitWidth, nUnitHeight);
				nLeft += nUnitWidth + nSpacing;
			}
		}
	}

	for (auto nIdx = nTesterCount; nIdx < MAX_EQUIPMENT_COUNT; ++nIdx)
	{
		m_wndEqpTesterList[nIdx].MoveWindow(0, 0, 0, 0);
	}

	// Returner
	m_wndEqpReturner.MoveWindow(nLeft, nTop, nUnitWidth, nUnitHeight);
}

//=============================================================================
// Method		: RemoveAll_EquipmentUI
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/11/24 - 12:24
// Desc.		:
//=============================================================================
void CWnd_MainView::RemoveAll_EquipmentUI()
{
#ifdef USE_DYNAMIC_WND_CREATE
	if (0 < m_wndEqpTesterList.size())
	{
		for (auto nIdx = 0; nIdx < m_wndEqpTesterList.size(); ++nIdx)
		{
			// ui 제거
			if (::IsWindow(m_wndEqpTesterList.at(nIdx)->GetSafeHwnd()))
			{
				m_wndEqpTesterList.at(nIdx)->DestroyWindow();

				delete m_wndEqpTesterList.at(nIdx);
				m_wndEqpTesterList.at(nIdx) = NULL;
			}
		}

		// class 제거
		m_wndEqpTesterList.clear();
	}
#endif
}

//=============================================================================
// Method		: Create_EquipmentUI
// Access		: protected  
// Returns		: void
// Parameter	: __in CConfig_Eqp * IN_pEqpuipment
// Parameter	: __in const RECT & IN_Rect
// Qualifier	:
// Last Update	: 2021/11/24 - 19:26
// Desc.		:
//=============================================================================
void CWnd_MainView::Create_EquipmentUI(__in CConfig_Eqp* IN_pEqpuipment, __in const RECT& IN_Rect)
{
#ifdef USE_DYNAMIC_WND_CREATE
	static UINT nWndID = 100;

	//CRuntimeClass * pWnd = RUNTIME_CLASS(CWnd_Status_Equipment);
	//CWnd_Status_Equipment * pWnd = RUNTIME_CLASS(CWnd_Status_Equipment);
 	CWnd_Status_Equipment* pWnd = new CWnd_Status_Equipment();
 	pWnd->Set_EquipmentBasicInfo(IN_pEqpuipment);
	if (pWnd->Create(_T("STATIC"), IN_pEqpuipment->m_szAlias.GetBuffer(), WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, IN_Rect, this, nWndID++))
	{
		m_wndEqpTesterList.push_back(pWnd);
	}
	else
	{
		TRACE0("Failed to create CWnd_Status_Equipment\n");
	}
#endif

}

//=============================================================================
// Method		: Init_EquipmentUI_List
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/11/24 - 12:24
// Desc.		:
//=============================================================================
void CWnd_MainView::Init_EquipmentUI_List()
{
	if (nullptr != m_pstSysInfo)
	{
		m_pwndEquipmentList.clear();
		m_bUseHandler = false;

		size_t nCount = m_pstSysInfo->LineInfo.GetCount();
		nCount = (nCount <= MAX_EQUIPMENT_COUNT) ? nCount : MAX_EQUIPMENT_COUNT;

		m_nMaxCol = static_cast<uint8_t>(__max(m_nMinCol, nCount + 1));

		if (0 < nCount)
		{
			uint8_t nTesterIdx = 0;
			for (auto nIdx = 0; nIdx < nCount; ++nIdx)
			{
				switch (m_pstSysInfo->LineInfo.GetAt(nIdx).Get_EquipmentType())
				{
				case enEquipmentType::Eqp_Handler:
				{
					m_bUseHandler = true;
					m_pwndEquipmentList.push_back(dynamic_cast<CWnd_Status_Equipment*>(&m_wndEqpHandler));
				}
				break;

				case enEquipmentType::Eqp_Loader:
				{
					m_pwndEquipmentList.push_back(dynamic_cast<CWnd_Status_Equipment*>(&m_wndEqpLoader));
				}
				break;

				case enEquipmentType::Eqp_Returner:
				{
					m_pwndEquipmentList.push_back(dynamic_cast<CWnd_Status_Equipment*>(&m_wndEqpReturner));
				}
				break;

				default: // Tester
				{
					m_pwndEquipmentList.push_back(dynamic_cast<CWnd_Status_Equipment*>(&m_wndEqpTesterList[nTesterIdx]));
					++nTesterIdx;
				}
				break;
				}
			}
		}

#ifdef USE_DYNAMIC_WND_CREATE
		// 기존 UI 제거
		RemoveAll_EquipmentUI();

		auto nCount = 10;//= m_pstSysInfo->RecipeInfo.LineInfo.GetCount();
		for (auto nIdx = 0; nIdx < nCount; ++nIdx)
		{
			// 설비 UI 추가
			Create_EquipmentUI(&m_pstSysInfo->SettingInfo.LineInfo.GetAt(nIdx), CRect(0, 0, 0, 0));
		}
#else

		if (0 < nCount)
		{
			for (auto nIdx = 0; nIdx < nCount; ++nIdx)
			{
#if defined(EES_XML)//20231003
				m_pstSysInfo->LineInfo.GetAt(nIdx).Set_DEFINEDATA(m_pstSysInfo->LineInfo.GetAt(nIdx));
#endif				
				m_pwndEquipmentList.at(nIdx)->SetPtr_EquipmentInfo(nIdx, &m_pstSysInfo->LineInfo.GetAt(nIdx));

				if (m_pstSysInfo->LineInfo.GetAt(nIdx).Is_Tester())
				{
					//int8_t nTesterType = ConvTo_TesterType(static_cast<enEquipmentType>(m_pstSysInfo->LineInfo.GetAt(nIdx).m_nEquipmentType));
					int8_t nTesterType = m_pstSysInfo->LineInfo.GetAt(nIdx).Get_TesterType();
					if (0 <= nTesterType)
					{
						m_pwndEquipmentList.at(nIdx)->Set_EnableEquipment(m_pstSysInfo->LineInfo.m_bEnable_TestType[nTesterType]);
					}
				}
#if defined(EES_XML)//20231003
				m_pwndEquipmentList.at(nIdx)->Set_EquipmentID(&m_pstSysInfo->LineInfo.GetAt(nIdx));
#endif				
			}
		}

#endif

		// UI 다시 그리기
		if (GetSafeHwnd())
		{
			CRect rc;
			GetClientRect(rc);
			SendMessage(WM_SIZE, (WPARAM)SIZE_RESTORED, MAKELPARAM(rc.Width(), rc.Height()));
		}
	}
}

//=============================================================================
// Method		: OnLanguage
// Access		: virtual public  
// Returns		: void
// Parameter	: __in uint8_t IN_nLanguage
// Qualifier	:
// Last Update	: 2022/7/18 - 13:46
// Desc.		:
//=============================================================================
void CWnd_MainView::OnLanguage(__in uint8_t IN_nLanguage)
{
	__super::OnLanguage(IN_nLanguage);

	for (auto nIdx = 0; nIdx < MAX_EQUIPMENT_COUNT; ++nIdx)
	{
		m_wndEqpTesterList[nIdx].OnLanguage(m_nLanguage);
	}

	m_wndEqpLoader.OnLanguage(m_nLanguage);
	m_wndEqpReturner.OnLanguage(m_nLanguage);
	m_wndTestSelect.OnLanguage(m_nLanguage);
	// Handler
	m_wndEqpHandler.OnLanguage(m_nLanguage);
}

//=============================================================================
// Method		: Set_PermissionMode
// Access		: public  
// Returns		: void
// Parameter	: enPermissionMode IN_PermissionMode
// Qualifier	:
// Last Update	: 2021/11/22 - 12:19
// Desc.		:
//=============================================================================
void CWnd_MainView::Set_PermissionMode(enPermissionMode IN_PermissionMode)
{
	switch (IN_PermissionMode)
	{
	case Permission_Operator:
		m_PermissionMode = Permission_Operator;		
		break;

	case Permission_Engineer:
	case Permission_Administrator:
		m_PermissionMode = Permission_Administrator;
		break;

	default:
		break;
	}

	m_wndTestSelect.Set_PermissionMode(IN_PermissionMode);

	for (auto nIdx = 0; nIdx < m_pwndEquipmentList.size(); ++nIdx)
	{
		m_pwndEquipmentList.at(nIdx)->Set_PermissionMode(IN_PermissionMode);
	}
#if defined(EES_XML)//20231003
	for (auto nIdx = 0; nIdx < m_pwndServerList.size(); ++nIdx)
	{
		m_pwndServerList.at(nIdx)->Set_PermissionMode(IN_PermissionMode);
		mPane_CommStatus->m_pwndServerList.at(nIdx)->Set_PermissionMode(IN_PermissionMode);
	}
#endif	
}

//=============================================================================
// Method		: Get_EquipmentHWND
// Access		: public  
// Returns		: HWND
// Parameter	: __in uint8_t IN_nEqpOrder
// Qualifier	:
// Last Update	: 2022/1/12 - 19:54
// Desc.		:
//=============================================================================
HWND CWnd_MainView::Get_EquipmentHWND(__in uint8_t IN_nEqpOrder)
{
	return m_pwndEquipmentList.at(IN_nEqpOrder)->GetSafeHwnd();
}


//=============================================================================
// Method		: Update_LineInfo
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/11/22 - 12:23
// Desc.		:
//=============================================================================
void CWnd_MainView::Update_LineInfo()
{
	if (m_pstSysInfo)
	{
		m_wndTestSelect.Set_Configuration(m_pstSysInfo->LineInfo);
		m_wndTestSelect.Set_Config_Line(&m_pstSysInfo->SettingInfo.LineInfo);
	}

	Init_EquipmentUI_List();
}

//=============================================================================
// Method		: UpdateUI_EnableEquipment
// Access		: public  
// Returns		: void
// Parameter	: __in uint8_t IN_nEqpNo
// Parameter	: __in bool IN_bEnable
// Qualifier	:
// Last Update	: 2022/1/3 - 18:20
// Desc.		:
//=============================================================================
void CWnd_MainView::UpdateUI_EnableEquipment(__in uint8_t IN_nEqpNo, __in bool IN_bEnable)
{
	if (!m_pstSysInfo)
		return;

	size_t nCount = __min (m_pstSysInfo->LineInfo.GetCount(), MAX_EQUIPMENT_COUNT);
	if (IN_nEqpNo < nCount)
	{
		m_pwndEquipmentList.at(IN_nEqpNo)->Set_EnableEquipment(IN_bEnable);
	}
}

#if defined(EES_XML)//20231003
HWND CWnd_MainView::Get_ServerHWND(__in uint8_t IN_nSvrOrder)
{
	return m_pwndServerList.at(IN_nSvrOrder)->GetSafeHwnd();
}
void CWnd_MainView::Init_ServerUI_List()
{
	if (nullptr != m_pstSysInfo)
	{
		//2023.09.03
		mPane_CommStatus->m_pwndServerList.clear();
		m_pwndServerList.clear();
		//
		m_bUseEES = false;
		size_t nCount = m_pstSysInfo->ServerInfo.GetCount();
		nCount = (nCount <= MAX_EQUIPMENT_COUNT) ? nCount : MAX_EQUIPMENT_COUNT;
		m_nMaxCol = __max(m_nMinCol, nCount + 1);
		if (0 < nCount)
		{
			uint8_t nTesterIdx = 0;
			for (auto nIdx = 0; nIdx < nCount; ++nIdx)
			{
				switch (m_pstSysInfo->ServerInfo.GetAt(nIdx).Get_ServerType())
				{
				case enServerType::SERVER_EES:
				{
					//2023.09.03
					//m_bUseEES = true;
					mPane_CommStatus->m_pwndServerList.push_back(dynamic_cast<CWnd_Status_Server*>(&m_wndSvrEES));
					m_pwndServerList.push_back(dynamic_cast<CWnd_Status_Server*>(&m_wndSvrEES));
					
				}
				break;
				}
			}
		}
#ifdef USE_DYNAMIC_WND_CREATE
		// 기존 UI 제거
		RemoveAll_EquipmentUI();
		auto nCount = 10;//= m_pstSysInfo->RecipeInfo.LineInfo.GetCount();
		for (auto nIdx = 0; nIdx < nCount; ++nIdx)
		{
			// 설비 UI 추가
			Create_EquipmentUI(&m_pstSysInfo->SettingInfo.LineInfo.GetAt(nIdx), CRect(0, 0, 0, 0));
		}
#else
		if (0 < nCount)
		{
			for (auto nIdx = 0; nIdx < nCount; ++nIdx)
			{
				m_pstSysInfo->ServerInfo.GetAt(nIdx).Set_DEFINEDATA(m_pstSysInfo->ServerInfo.GetAt(nIdx));				
				m_pwndServerList.at(nIdx)->SetPtr_EquipmentInfo(nIdx, &m_pstSysInfo->ServerInfo.GetAt(nIdx));
				//2023.09.03
				mPane_CommStatus->m_pwndServerList.at(nIdx)->SetPtr_EquipmentInfo(nIdx, &m_pstSysInfo->ServerInfo.GetAt(nIdx));
				if (m_pstSysInfo->ServerInfo.GetAt(nIdx).Is_EES())
				{
					//int8_t nTesterType = ConvTo_TesterType(static_cast<enEquipmentType>(m_pstSysInfo->LineInfo.GetAt(nIdx).m_nEquipmentType));
					int8_t nEesType = m_pstSysInfo->ServerInfo.GetAt(nIdx).Get_ServerType();
					if (0 <= nEesType)
					{
						m_pwndServerList.at(nIdx)->Set_EnableEquipment(m_pstSysInfo->ServerInfo.m_bEnable_TestType[nEesType]);
						//2023.09.03
						mPane_CommStatus->m_pwndServerList.at(nIdx)->Set_EnableEquipment(m_pstSysInfo->ServerInfo.m_bEnable_TestType[nEesType]);
					}
				}			
				m_pwndServerList.at(nIdx)->Set_EquipmentID(&m_pstSysInfo->ServerInfo.GetAt(nIdx));
				//2023.09.03
				mPane_CommStatus->m_pwndServerList.at(nIdx)->Set_EquipmentID(&m_pstSysInfo->ServerInfo.GetAt(nIdx));
			}
		}
#endif
		// UI 다시 그리기
		if (GetSafeHwnd())
		{
			CRect rc;
			GetClientRect(rc);
			SendMessage(WM_SIZE, (WPARAM)SIZE_RESTORED, MAKELPARAM(rc.Width(), rc.Height()));
		}
	}
}
void CWnd_MainView::Update_ServerInfo()
{
	if (m_pstSysInfo)
	{
		m_wndTestSelect.Set_Configuration(m_pstSysInfo->ServerInfo);
		m_wndTestSelect.Set_Config_Server(&m_pstSysInfo->SettingInfo.ServerInfo);
	}
	Init_ServerUI_List();
}
#endif