//*****************************************************************************
// Filename	: 	Wnd_Status_Tester.cpp
// Created	:	2021/12/16 - 17:58
// Modified	:	2021/12/16 - 17:58
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
// Wnd_Status_Tester.cpp : implementation file
//

#include "stdafx.h"
#include "Wnd_Status_Tester.h"
#include "Def_Equipment_Type.h"
#if defined(EES_XML)//20231003
#include "Util/StringUtil.hpp"
#endif
//-----------------------------------------------------------------------------
// CWnd_Status_Tester
//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CWnd_Status_Tester, CUI_Equipment)

CWnd_Status_Tester::CWnd_Status_Tester()
{
	m_nPortCount		= PtI_T_MaxCount;
	m_nConveyorCount	= CvI_T_MaxCount;
}

CWnd_Status_Tester::~CWnd_Status_Tester()
{

}

BEGIN_MESSAGE_MAP(CWnd_Status_Tester, CUI_Equipment)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CWnd_Status_Tester message handlers
//=============================================================================
// Method		: OnCreate
// Access		: public  
// Returns		: int
// Parameter	: LPCREATESTRUCT lpCreateStruct
// Qualifier	:
// Last Update	: 2016/3/17 - 10:04
// Desc.		:
//=============================================================================
int CWnd_Status_Tester::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
 	DWORD dwStyle = WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
 	CRect rectDummy;
 	rectDummy.SetRectEmpty();

	// 설비 상태 정보 ----------------------------
	LPCTSTR szZoneName[] =
	{
		_T("L"),
		_T("R"),
		_T("C"),
	};
	for (auto nIdx = 0; nIdx < Zone_MaxCount; ++nIdx)
	{
		m_st_Zone[nIdx].SetFont_Gdip(L"Arial", 8.0F);
		m_st_Zone[nIdx].Create(szZoneName[nIdx], dwStyle /*| WS_BORDER*/ | SS_CENTER, rectDummy, this, IDC_STATIC);
		m_st_Zone[nIdx].SetFont(&m_font_Default);
	}

	//m_st_Zone_L			.SetFont_Gdip(L"Arial", 8.0F);
	//m_st_Zone_R			.SetFont_Gdip(L"Arial", 8.0F);
	//m_st_Zone_L.Create(_T("Left"), dwStyle /*| WS_BORDER*/ | SS_CENTER, rectDummy, this, IDC_STATIC);
	//m_st_Zone_L.SetFont(&m_font_Default);
	//m_st_Zone_R.Create(_T("Right"), dwStyle /*| WS_BORDER*/ | SS_CENTER, rectDummy, this, IDC_STATIC);
	//m_st_Zone_R.SetFont(&m_font_Default);

	LPCTSTR g_szTst_Port[PtI_T_MaxCount] =
	{
		_T(""),			 // PtI_T_RFID,
		_T("Buffer"),	 // PtI_T_Buffer,
		_T("0"),		 // PtI_T_Test_L,
		_T("0"),		 // PtI_T_Test_R,
		_T("0"),		 // PtI_T_Test_C,
	};
	for (auto nIdx = 0; nIdx < PtI_T_MaxCount; ++nIdx)
	{
		m_st_Port[nIdx].SetFont_Gdip(L"Arial", 8.0F);
		m_st_Port[nIdx].Create(g_szTst_Port[nIdx], dwStyle /*| WS_BORDER*/ | SS_CENTER, rectDummy, this, IDC_STATIC);
	}

	LPCTSTR g_szTst_Conveyor[CvI_T_MaxCount] =
	{
		_T("RFID"),		// 	CvI_T_RFID,
		_T("Test"),		// 	CvI_T_Test,
		_T("Return"),	// 	CvI_T_Return
	};
	for (auto nIdx = 0; nIdx < CvI_T_MaxCount; ++nIdx)
	{
		m_st_Conveyor[nIdx].SetFont_Gdip(L"Arial", 8.0F);
		m_st_Conveyor[nIdx].Create(g_szTst_Conveyor[nIdx], dwStyle /*| WS_BORDER*/ | SS_CENTER, rectDummy, this, IDC_STATIC);
	}

#if defined(EES_XML)//20231003
	for (auto nIdx = 0; nIdx < PtI_T_MaxCount; ++nIdx)
	{
		m_st_EquipMentState[nIdx].SetFont_Gdip(L"Arial", 8.0F);
		m_st_EquipMentState[nIdx].Create(_T(" "), dwStyle /*| WS_BORDER*/ | SS_CENTER, rectDummy, this, IDC_STATIC);
	}
	for (auto nIdx = 0; nIdx < PtI_T_MaxCount; ++nIdx)
	{
		m_st_EquipMentStateDisplay[nIdx].SetFont_Gdip(L"Arial", 8.0F);
		m_st_EquipMentStateDisplay[nIdx].Create(_T(" "), dwStyle /*| WS_BORDER*/ | SS_CENTER, rectDummy, this, IDC_STATIC);
	}
	m_st_SubEqpID.SetFont_Gdip(L"Arial", 7.0F);
	m_st_SubEqpID.Create(_T(" "), dwStyle /*| WS_BORDER*/ | SS_CENTER, rectDummy, this, IDC_STATIC);
	for (auto nIdx = 0; nIdx < PtI_T_MaxCount; ++nIdx)
	{
		m_st_EquipmentID[nIdx].SetFont_Gdip(L"Arial", 7.0F);
		m_st_EquipmentID[nIdx].Create(_T(" "), dwStyle /*| WS_BORDER*/ | SS_CENTER, rectDummy, this, IDC_STATIC);
	}
	//for (auto nIdx = 0; nIdx < PtI_T_MaxCount; ++nIdx)
	//{
	//	m_st_LossCode[nIdx].SetFont_Gdip(L"Arial", 8.0F);
	//	m_st_LossCode[nIdx].Create(_T(" "), dwStyle /*| WS_BORDER*/ | SS_CENTER, rectDummy, this, IDC_STATIC);
	//}
#endif	//ADD_SOCKET_EES_XML

	// 타이틀바, 외곽선
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

//=============================================================================
// Method		: OnSize
// Access		: public  
// Returns		: void
// Parameter	: UINT nType
// Parameter	: int cx
// Parameter	: int cy
// Qualifier	:
// Last Update	: 2016/3/17 - 10:04
// Desc.		:
//=============================================================================
void CWnd_Status_Tester::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	int iSpacing = 5;
	int iLeft = 15;
	int iTop = m_nTop_Status; // 타이틀 바, LED, 설비 Config 제외 위치

	// 검사 존 상태
// 	int iStHeight = m_nTitle_Height - 6;
// 	int iStWidth = (cx - 20 - iSpacing) / 2;
// 	
// 	m_st_Zone_L.MoveWindow(iLeft, iTop, iStWidth, iStHeight);
// 	m_st_Zone_R.MoveWindow(iLeft + iStWidth + iSpacing, iTop, iStWidth, iStHeight);
// 	iTop += m_nLED_Height - 1;
// 	m_st_Zone_L_Data.MoveWindow(iLeft, iTop, iStWidth, iStHeight);
// 	m_st_Zone_R_Data.MoveWindow(iLeft + iStWidth + iSpacing, iTop, iStWidth, iStHeight);
// 
// 	iTop += m_nLED_Height + iSpacing;
	
	
	MoveWindow_Tester(5, iTop, cx - iSpacing - iSpacing, m_nDefInfo_Height - iSpacing - iTop);

	// 수율 (로더, 검사 설비만 사용?)
	iTop = cy - iSpacing - m_nCtrl_Height;

#ifdef USE_EQP_TACTTIME
	// TactTime
	int iStWidth = 40;
	int iDataWidth = cx - 10 - iStWidth + 1;
	iLeft = 5;
	int iLeftSub = iLeft + iStWidth - 1;
	m_st_Tacttime[TT_Curr].MoveWindow(iLeft, iTop, iStWidth, m_nCtrl_Height);
	m_st_Tacttime[TT_Curr_Data].MoveWindow(iLeftSub, iTop, iDataWidth, m_nCtrl_Height);

	iTop -= (m_nCtrl_Height - 1);
	m_st_Tacttime[TT_Ave].MoveWindow(iLeft, iTop, iStWidth, m_nCtrl_Height);
	m_st_Tacttime[TT_Ave_Data].MoveWindow(iLeftSub, iTop, iDataWidth, m_nCtrl_Height);
#endif

	iTop -= (m_nYield_Height + iSpacing);
	m_wnd_Yield.MoveWindow(5, iTop, cx - 10, m_nYield_Height);

}

//=============================================================================
// Method		: MoveWindow_Tester
// Access		: protected  
// Returns		: void
// Parameter	: int x
// Parameter	: int y
// Parameter	: int nWidth
// Parameter	: int nHeight
// Qualifier	:
// Last Update	: 2021/12/2 - 16:48
// Desc.		:
//=============================================================================
void CWnd_Status_Tester::MoveWindow_Tester(int x, int y, int nWidth, int nHeight)
{
	int iMargin		= 5;
	int iSpacing	= 5;
	int iLeft		= x;
	int iTop		= y;
	int iCvWidth	= (nWidth - iSpacing) / 2;

	iTop = y + nHeight - m_nCtrl_Height - iSpacing;
	m_st_Conveyor[CvI_T_Return].MoveWindow(iLeft, iTop, nWidth, m_nCtrl_Height);

	iTop -= (m_nLine_Depth - m_nCtrl_Height);
	m_st_Conveyor[CvI_T_RFID].MoveWindow(iLeft, iTop, iCvWidth + 1 - m_nPort_Width, m_nCtrl_Height);
	iLeft += (iCvWidth - m_nPort_Width);
	m_st_Port[PtI_T_RFID].MoveWindow(iLeft, iTop, m_nPort_Width, m_nCtrl_Height);
	iLeft += m_nPort_Width + iSpacing;
	m_st_Conveyor[CvI_T_Test].MoveWindow(iLeft, iTop, iCvWidth, m_nCtrl_Height);

	iLeft = x;
	iTop -= (m_nCtrl_Height + iSpacing);

	int nBufferCount = (m_pEquipmentStatus) ? m_pEquipmentStatus->Get_BufferCount() : 1;
	if (0 < nBufferCount)
		m_st_Port[PtI_T_Buffer].MoveWindow(iLeft, iTop, iCvWidth, m_nCtrl_Height);
	else
		m_st_Port[PtI_T_Buffer].MoveWindow(0, 0, 0, 0);

	// 검사 존
	int nTestCount = (m_pEquipmentStatus) ? m_pEquipmentStatus->Get_TestPortCount() : 2;


#if defined(EES_XML)//20231003
	switch (nTestCount)
	{
	case 1:
	{
		int iStHeight = m_nCtrl_Height;
		int iStWidth = (nWidth - 15);
		iLeft = x + 15;

		iTop -= (m_nCtrl_Height + iSpacing + iSpacing);
		m_st_Port[PtI_T_Test_L].MoveWindow(iLeft, iTop, iStWidth, iStHeight);
		m_st_Port[PtI_T_Test_C].MoveWindow(0, 0, 0, 0);
		m_st_Port[PtI_T_Test_R].MoveWindow(0, 0, 0, 0);

		iTop -= (iStHeight - 1);
		m_st_Zone[Zone_L].MoveWindow(iLeft, iTop, iStWidth, iStHeight);
		m_st_Zone[Zone_C].MoveWindow(0, 0, 0, 0);
		m_st_Zone[Zone_R].MoveWindow(0, 0, 0, 0);
	}
		break;		
	case 2:
	{
		iMargin = 5;
		iSpacing = 5;
		iLeft = x;
		iCvWidth =nWidth / 2;
		iTop -= (m_nCtrl_Height + iSpacing);
		m_st_EquipMentState[PtI_T_Test_L].MoveWindow(iLeft, iTop, iCvWidth, m_nCtrl_Height);
		m_st_EquipMentState[PtI_T_Test_C].MoveWindow(0, 0, 0, 0);
		m_st_EquipMentState[PtI_T_Test_R].MoveWindow(iLeft + iCvWidth, iTop, iCvWidth, m_nCtrl_Height);
		iTop -= (m_nCtrl_Height - 1);
		m_st_EquipMentStateDisplay[PtI_T_Test_L].MoveWindow(iLeft, iTop, iCvWidth, m_nCtrl_Height);
		m_st_EquipMentStateDisplay[PtI_T_Test_C].MoveWindow(0, 0, 0, 0);
		m_st_EquipMentStateDisplay[PtI_T_Test_R].MoveWindow(iLeft + iCvWidth, iTop, iCvWidth, m_nCtrl_Height);
		iTop -= (m_nCtrl_Height - 1);
		m_st_EquipmentID[PtI_T_Test_L].MoveWindow(iLeft, iTop, iCvWidth, m_nCtrl_Height);
		m_st_EquipmentID[PtI_T_Test_C].MoveWindow(0, 0, 0, 0);
		m_st_EquipmentID[PtI_T_Test_R].MoveWindow(iLeft + iCvWidth, iTop, iCvWidth, m_nCtrl_Height);
	}
		break;

	case 3:
	{
		iMargin = 5;
		iSpacing = 5;
		iLeft = x;
		iCvWidth = nWidth / 3;
		for (auto nIdx = 0; nIdx < PtI_T_MaxCount; ++nIdx)
		{
			//m_st_LossCode[nIdx].SetFont_Gdip(L"Arial", 7.0F);
			m_st_EquipMentStateDisplay[nIdx].SetFont_Gdip(L"Arial", 7.0F);
			m_st_EquipMentState[nIdx].SetFont_Gdip(L"Arial", 7.0F);
		}
		iTop -= (m_nCtrl_Height + iSpacing);
		//m_st_LossCode[PtI_T_Test_L].MoveWindow(iLeft, iTop, iCvWidth, m_nCtrl_Height);
		//m_st_LossCode[PtI_T_Test_C].MoveWindow(iLeft + iCvWidth, iTop, iCvWidth, m_nCtrl_Height);
		//m_st_LossCode[PtI_T_Test_R].MoveWindow(iLeft + (iCvWidth * 2), iTop, iCvWidth, m_nCtrl_Height);
		m_st_EquipMentState[PtI_T_Test_L].MoveWindow(iLeft, iTop, iCvWidth, m_nCtrl_Height);
		m_st_EquipMentState[PtI_T_Test_C].MoveWindow(iLeft + iCvWidth, iTop, iCvWidth, m_nCtrl_Height);
		m_st_EquipMentState[PtI_T_Test_R].MoveWindow(iLeft + (iCvWidth * 2), iTop, iCvWidth, m_nCtrl_Height);
		iTop -= (m_nCtrl_Height - 1);
		m_st_EquipMentStateDisplay[PtI_T_Test_L].MoveWindow(iLeft, iTop, iCvWidth, m_nCtrl_Height);
		m_st_EquipMentStateDisplay[PtI_T_Test_C].MoveWindow(iLeft + iCvWidth, iTop, iCvWidth, m_nCtrl_Height);
		m_st_EquipMentStateDisplay[PtI_T_Test_R].MoveWindow(iLeft + (iCvWidth * 2), iTop, iCvWidth, m_nCtrl_Height);
		iTop -= (m_nCtrl_Height - 1);
		m_st_EquipmentID[PtI_T_Test_L].MoveWindow(iLeft, iTop, iCvWidth, m_nCtrl_Height);
		m_st_EquipmentID[PtI_T_Test_C].MoveWindow(iLeft + iCvWidth, iTop, iCvWidth, m_nCtrl_Height);
		m_st_EquipmentID[PtI_T_Test_R].MoveWindow(iLeft + (iCvWidth * 2), iTop, iCvWidth, m_nCtrl_Height);
	}
		break;
	default:
		break;
	}
	iTop -= (m_nCtrl_Height - 1);
	m_st_SubEqpID.MoveWindow(iLeft, iTop, nWidth, m_nCtrl_Height);
#else
	switch (nTestCount)
	{
	case 1:
	{
		int iStHeight = m_nCtrl_Height;
		int iStWidth = (nWidth - 15);
		iLeft = x + 15;

		iTop -= (m_nCtrl_Height + iSpacing + iSpacing);
		m_st_Port[PtI_T_Test_L].MoveWindow(iLeft, iTop, iStWidth, iStHeight);
		m_st_Port[PtI_T_Test_C].MoveWindow(0, 0, 0, 0);
		m_st_Port[PtI_T_Test_R].MoveWindow(0, 0, 0, 0);

		iTop -= (iStHeight - 1);
		m_st_Zone[Zone_L].MoveWindow(iLeft, iTop, iStWidth, iStHeight);
		m_st_Zone[Zone_C].MoveWindow(0, 0, 0, 0);
		m_st_Zone[Zone_R].MoveWindow(0, 0, 0, 0);
	}
		break;

	case 2:
	{
		int iStHeight = m_nCtrl_Height;
		int iStWidth = (nWidth - 15 - iSpacing) / 2;
		iLeft = x + 15;

		iTop -= (m_nCtrl_Height + iSpacing + iSpacing);
		m_st_Port[PtI_T_Test_L].MoveWindow(iLeft, iTop, iStWidth, iStHeight);
		m_st_Port[PtI_T_Test_C].MoveWindow(0, 0, 0, 0);
		m_st_Port[PtI_T_Test_R].MoveWindow(iLeft + iStWidth + iSpacing, iTop, iStWidth, iStHeight);

		iTop -= (iStHeight - 1);
		m_st_Zone[Zone_L].MoveWindow(iLeft, iTop, iStWidth, iStHeight);
		m_st_Zone[Zone_C].MoveWindow(0, 0, 0, 0);
		m_st_Zone[Zone_R].MoveWindow(iLeft + iStWidth + iSpacing, iTop, iStWidth, iStHeight);
	}
		break;

	case 3:
	{
		int iStHeight = m_nCtrl_Height;
		int iStWidth = (nWidth - 15 - iSpacing - iSpacing) / 3;
		iLeft = x + 15;

		iTop -= (m_nCtrl_Height + iSpacing + iSpacing);
		m_st_Port[PtI_T_Test_L].MoveWindow(iLeft, iTop, iStWidth, iStHeight);
		m_st_Port[PtI_T_Test_C].MoveWindow(iLeft + iStWidth + iSpacing, iTop, iStWidth, iStHeight);
		m_st_Port[PtI_T_Test_R].MoveWindow(iLeft + (iStWidth + iSpacing) * 2, iTop, iStWidth, iStHeight);

		iTop -= (iStHeight - 1);
		m_st_Zone[Zone_L].MoveWindow(iLeft, iTop, iStWidth, iStHeight);
		m_st_Zone[Zone_C].MoveWindow(iLeft + iStWidth + iSpacing, iTop, iStWidth, iStHeight);
		m_st_Zone[Zone_R].MoveWindow(iLeft + (iStWidth + iSpacing) * 2, iTop, iStWidth, iStHeight);
	}
		break;

	default:
		break;
	}
#endif

// 	if (2 == nTestCount)
// 	{
// 		int iStHeight = m_nCtrl_Height;
// 		int iStWidth = (nWidth - 15 - iSpacing) / 2;
// 		iLeft = x + 15;
// 
// 		iTop -= (m_nCtrl_Height + iSpacing + iSpacing);
// 		m_st_Port[PtI_T_Test_L].MoveWindow(iLeft, iTop, iStWidth, iStHeight);
// 		m_st_Port[PtI_T_Test_C].MoveWindow(0, 0, 0, 0);
// 		m_st_Port[PtI_T_Test_R].MoveWindow(iLeft + iStWidth + iSpacing, iTop, iStWidth, iStHeight);
// 
// 		iTop -= (iStHeight - 1);
// 		m_st_Zone[Zone_L].MoveWindow(iLeft, iTop, iStWidth, iStHeight);
// 		m_st_Zone[Zone_C].MoveWindow(0, 0, 0, 0);
// 		m_st_Zone[Zone_R].MoveWindow(iLeft + iStWidth + iSpacing, iTop, iStWidth, iStHeight);
// 	}
// 	else // 3 para
// 	{
// 		int iStHeight = m_nCtrl_Height;
// 		int iStWidth = (nWidth - 15 - iSpacing - iSpacing) / 3;
// 		iLeft = x + 15;
// 
// 		iTop -= (m_nCtrl_Height + iSpacing + iSpacing);
// 		m_st_Port[PtI_T_Test_L].MoveWindow(iLeft, iTop, iStWidth, iStHeight);
// 		m_st_Port[PtI_T_Test_C].MoveWindow(iLeft + iStWidth + iSpacing, iTop, iStWidth, iStHeight);
// 		m_st_Port[PtI_T_Test_R].MoveWindow(iLeft + (iStWidth + iSpacing) * 2, iTop, iStWidth, iStHeight);
// 
// 		iTop -= (iStHeight - 1);
// 		m_st_Zone[Zone_L].MoveWindow(iLeft, iTop, iStWidth, iStHeight);
// 		m_st_Zone[Zone_C].MoveWindow(iLeft + iStWidth + iSpacing, iTop, iStWidth, iStHeight);
// 		m_st_Zone[Zone_R].MoveWindow(iLeft + (iStWidth + iSpacing) * 2, iTop, iStWidth, iStHeight);
// 	}
}

//=============================================================================
// Method		: Init_Tester
// Access		: virtual protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2022/7/27 - 17:34
// Desc.		:
//=============================================================================
void CWnd_Status_Tester::Init_Tester()
{

}

void CWnd_Status_Tester::Popup_SubMenu()
{
	__super::Popup_SubMenu();
}

//=============================================================================
// Method		: Set_EnableEquipment
// Access		: virtual public  
// Returns		: void
// Parameter	: __in bool IN_bEnable
// Qualifier	:
// Last Update	: 2022/1/3 - 11:47
// Desc.		:
//=============================================================================
void CWnd_Status_Tester::Set_EnableEquipment(__in bool IN_bEnable)
{
	__super::Set_EnableEquipment(IN_bEnable);

	if (IN_bEnable)
	{
		if (m_pEquipmentStatus->Get_Skip())
		{

		}
		else
		{
			for (auto nIdx = 0; nIdx < Zone_MaxCount; ++nIdx)
			{
				m_st_Zone[nIdx].SetBackColor_COLORREF(RGB(255, 255, 255));
			}

			for (auto nIdx = 0; nIdx < PtI_T_MaxCount; ++nIdx)
			{
				m_st_Port[nIdx].SetBackColor_COLORREF(RGB(255, 255, 255));
			}

#ifdef USE_EQP_TACTTIME
			// TactTime
			for (auto nIdx = 0; nIdx < TT_MaxCount; ++nIdx)
			{
				m_st_Tacttime[nIdx].SetBackColor_COLORREF(RGB(255, 255, 255));
			}
#endif
		}
	}
	else
	{
		m_st_Border.SetBackColor_COLORREF(RGB(100, 100, 100));
		for (auto nIdx = 0; nIdx < Zone_MaxCount; ++nIdx)
		{
			m_st_Zone[nIdx].SetBackColor_COLORREF(RGB(100, 100, 100));
		}

		for (auto nIdx = 0; nIdx < PtI_T_MaxCount; ++nIdx)
		{
			m_st_Port[nIdx].SetBackColor_COLORREF(RGB(100, 100, 100));
		}

#ifdef USE_EQP_TACTTIME
		// TactTime
		for (auto nIdx = 0; nIdx < TT_MaxCount; ++nIdx)
		{
			m_st_Tacttime[nIdx].SetBackColor_COLORREF(RGB(100, 100, 100));
		}
#endif
	}
}

//=============================================================================
// Method		: Set_Skip
// Access		: virtual public  
// Returns		: void
// Parameter	: __in bool IN_bSkip
// Qualifier	:
// Last Update	: 2022/7/27 - 17:34
// Desc.		:
//=============================================================================
void CWnd_Status_Tester::Set_Skip(__in bool IN_bSkip)
{
	__super::Set_Skip(IN_bSkip);

	if (m_pEquipmentStatus->Get_EnableEquipment())
	{
		if (IN_bSkip)
		{
			m_st_Border.SetBackColor_COLORREF(RGB(130, 100, 100));
			for (auto nIdx = 0; nIdx < Zone_MaxCount; ++nIdx)
			{
				m_st_Zone[nIdx].SetBackColor_COLORREF(RGB(130, 100, 100));
			}

			for (auto nIdx = 0; nIdx < PtI_T_MaxCount; ++nIdx)
			{
				m_st_Port[nIdx].SetBackColor_COLORREF(RGB(130, 100, 100));
			}

#ifdef USE_EQP_TACTTIME
			// TactTime
			for (auto nIdx = 0; nIdx < TT_MaxCount; ++nIdx)
			{
				m_st_Tacttime[nIdx].SetBackColor_COLORREF(RGB(130, 100, 100));
			}
#endif
		}
		else
		{
			for (auto nIdx = 0; nIdx < Zone_MaxCount; ++nIdx)
			{
				m_st_Zone[nIdx].SetBackColor_COLORREF(RGB(255, 255, 255));
			}

			for (auto nIdx = 0; nIdx < PtI_T_MaxCount; ++nIdx)
			{
				m_st_Port[nIdx].SetBackColor_COLORREF(RGB(255, 255, 255));
			}

#ifdef USE_EQP_TACTTIME
			// TactTime
			for (auto nIdx = 0; nIdx < TT_MaxCount; ++nIdx)
			{
				m_st_Tacttime[nIdx].SetBackColor_COLORREF(RGB(255, 255, 255));
			}
#endif
		}
	}
	else
	{
		// 아무것도 안한다.
	}
}

//=============================================================================
// Method		: Set_PortClear
// Access		: virtual public  
// Returns		: void
// Parameter	: __in uint8_t IN_nPortIndex
// Qualifier	:
// Last Update	: 2022/1/13 - 11:30
// Desc.		:
//=============================================================================
void CWnd_Status_Tester::Set_PortClear(__in uint8_t IN_nPortIndex)
{
	Set_PortStatus(IN_nPortIndex, (uint8_t)0);
}

//=============================================================================
// Method		: Set_PortStatus
// Access		: virtual public  
// Returns		: void
// Parameter	: __in uint8_t IN_nPortIndex
// Parameter	: __in uint8_t IN_nStatus
// Qualifier	:
// Last Update	: 2021/12/24 - 14:43
// Desc.		:
//=============================================================================
void CWnd_Status_Tester::Set_PortStatus(__in uint8_t IN_nPortIndex, __in uint8_t IN_nStatus)
{
	if (IN_nPortIndex < m_nPortCount)
	{
		SetUI_PortStatus(&m_st_Port[IN_nPortIndex], IN_nStatus);
	}
	else if (PtI_T_All == IN_nPortIndex) // Port All
	{
		for (auto nIdx = 0; nIdx < m_nPortCount; nIdx++)
		{
			SetUI_PortStatus(&m_st_Port[nIdx], IN_nStatus);
		}
	}
	else // 오류
	{

	}
}

void CWnd_Status_Tester::Set_PortStatus(__in uint8_t IN_nPortIndex, __in const ST_PortStatus* IN_nStatus)
{
	if (IN_nPortIndex < m_nPortCount)
	{
		SetUI_PortStatus(&m_st_Port[IN_nPortIndex], IN_nStatus);
	}
	else if (PtI_T_All == IN_nPortIndex) // Port All
	{
		for (auto nIdx = 0; nIdx < m_nPortCount; nIdx++)
		{
			SetUI_PortStatus(&m_st_Port[nIdx], IN_nStatus);
		}
	}
	else // 오류
	{

	}
}

//=============================================================================
// Method		: Set_ConveyorStatus
// Access		: virtual public  
// Returns		: void
// Parameter	: __in uint8_t IN_nConveyorIndex
// Parameter	: __in uint8_t IN_nStatus
// Parameter	: __in uint8_t IN_nExistSocket
// Qualifier	:
// Last Update	: 2022/7/27 - 17:35
// Desc.		:
//=============================================================================
void CWnd_Status_Tester::Set_ConveyorStatus(__in uint8_t IN_nConveyorIndex, __in uint8_t IN_nStatus, __in uint8_t IN_nExistSocket)
{
	if (IN_nConveyorIndex < m_nConveyorCount)
	{
		SetUI_ConveyorStatus(&m_st_Conveyor[IN_nConveyorIndex], IN_nStatus, IN_nExistSocket);
	}
	else // 오류
	{

	}
}

void CWnd_Status_Tester::Set_ConveyorStatus(__in uint8_t IN_nConveyorIndex, __in const ST_ConveyorStatus* IN_nStatus)
{
	if (IN_nConveyorIndex < m_nConveyorCount)
	{
		SetUI_ConveyorStatus(&m_st_Conveyor[IN_nConveyorIndex], IN_nStatus);
	}
	else // 오류
	{

	}
}

#if defined(EES_XML)//20231003
//=============================================================================
// Method		: Get_Color
// Access		: virtual public  
// Returns		: void
// Parameter	: __in uint8_t IN_nConveyorIndex
// Parameter	: __in uint8_t IN_nStatus
// Parameter	: __in uint8_t IN_nExistSocket
// Qualifier	:
// Last Update	: 2023.04.19
// Desc.		:
//=============================================================================
void CWnd_Status_Tester::Set_EqpNotify_EQUIPMENTSTATE(__in lt::Report_Equipment_State_Args::Args * IN_DATA)
{
	CString EQUIPMENTID;
	CString EQUIPMENTSTATE;
	CString SUBEQPID;
	//CString PORTID(IN_DATA->Body.PORTID);	//2023.06.22
	
	for (int nPORTID = 0; nPORTID < m_pEquipmentStatus->Get_EquipmentIDCount(); nPORTID++) {
		if (nPORTID < m_pEquipmentStatus->Get_mEES_PortSubStatusCount()) {
			auto EQUIPMENTID = m_pEquipmentStatus->Get_EquipmentIDStatus(nPORTID).Get_EQUIPMENTID();
			auto sEQUIPMENTID = lt::ToTypeChar(IN_DATA->Body.Get_EQUIPMENTID()).c_str();
			if (sEQUIPMENTID == EQUIPMENTID) {
				CVGStatic * staic = &m_st_EquipMentState[nPORTID];
				auto EquipmentStatus = m_pEquipmentStatus->Get_mEES_PortSubStatus(nPORTID).Get_nEquipmentStatus();
				SetUI_EQUIPMENTSTATE(staic, g_szPortStatus[EquipmentStatus]);
			}
		}
	}
}

void CWnd_Status_Tester::Set_EquipmentID(__in CEquipment* IN_Data)
{
	__super::Set_EquipmentID(IN_Data);
	m_st_SubEqpID.SetText(IN_Data->Get_SubEqpID());
	m_st_EquipmentID[PtI_T_Test_L].SetText(IN_Data->Get_EquipmentIDStatus(PtI_T_Test_L).Get_EQUIPMENTID());
	m_st_EquipmentID[PtI_T_Test_C].SetText(IN_Data->Get_EquipmentIDStatus(PtI_T_Test_C).Get_EQUIPMENTID());
	m_st_EquipmentID[PtI_T_Test_R].SetText(IN_Data->Get_EquipmentIDStatus(PtI_T_Test_R).Get_EQUIPMENTID());
}
#endif