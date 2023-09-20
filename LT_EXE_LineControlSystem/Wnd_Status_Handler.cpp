//*****************************************************************************
// Filename	: 	Wnd_Status_Handler.cpp
// Created	:	2021/12/16 - 17:58
// Modified	:	2021/12/16 - 17:58
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
// Wnd_Status_Handler.cpp : implementation file
//

#include "stdafx.h"
#include "Wnd_Status_Handler.h"
#include "Def_Equipment_Type.h"

//-----------------------------------------------------------------------------
// CWnd_Status_Handler
//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CWnd_Status_Handler, CWnd_Status_Equipment)

CWnd_Status_Handler::CWnd_Status_Handler()
{
	m_nPortCount		= PtI_R_MaxCount;
	m_nConveyorCount	= CvI_R_MaxCount;
}

CWnd_Status_Handler::~CWnd_Status_Handler()
{

}

BEGIN_MESSAGE_MAP(CWnd_Status_Handler, CWnd_Status_Equipment)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CWnd_Status_Handler message handlers
//=============================================================================
// Method		: OnCreate
// Access		: public  
// Returns		: int
// Parameter	: LPCREATESTRUCT lpCreateStruct
// Qualifier	:
// Last Update	: 2016/3/17 - 10:04
// Desc.		:
//=============================================================================
int CWnd_Status_Handler::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
 	DWORD dwStyle = WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
 	CRect rectDummy;
 	rectDummy.SetRectEmpty();

	// 설비 상태 정보 ----------------------------

	for (auto nIdx = 0; nIdx < PtI_R_MaxCount; ++nIdx)
	{
		m_st_Port[nIdx].Create(_T(""), dwStyle /*| WS_BORDER*/ | SS_CENTER, rectDummy, this, IDC_STATIC);
	}

	LPCTSTR g_szRt_Conveyor[CvI_R_MaxCount] =
	{
		_T("Buffer 1"),	   // CvI_R_Buffer_1,
		_T("Transfer"),	   // CvI_R_Transfer,
		_T("Buffer 2"),	   // CvI_R_Buffer_2,
	};
	for (auto nIdx = 0; nIdx < CvI_R_MaxCount; ++nIdx)
	{
		m_st_Conveyor[nIdx].SetFont_Gdip(L"Arial", 8.0F);
		m_st_Conveyor[nIdx].Create(g_szRt_Conveyor[nIdx], dwStyle /*| WS_BORDER*/ | SS_CENTER, rectDummy, this, IDC_STATIC);
	}

#if (USE_XML)
	m_st_SubEqpID.SetFont_Gdip(L"Arial", 7.0F);
	m_st_SubEqpID.Create(_T(" "), dwStyle /*| WS_BORDER*/ | SS_CENTER, rectDummy, this, IDC_STATIC);
	for (auto nIdx = 0; nIdx < PtI_L_MaxCount; ++nIdx)
	{
		m_st_EquipmentID[nIdx].SetFont_Gdip(L"Arial", 7.0F);
		m_st_EquipmentID[nIdx].Create(_T(" "), dwStyle /*| WS_BORDER*/ | SS_CENTER, rectDummy, this, IDC_STATIC);
	}
#endif

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
void CWnd_Status_Handler::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

#if (USE_XML)
	int iSpacing = 5;
	int iLeft = 15;
	int iTop = m_nTop_Status; // 타이틀 바, LED, 설비 Config 제외 위치
	MoveWindow_Handler(5, m_nTop_Status, cx - iSpacing - iSpacing, m_nDefInfo_Height - iSpacing - m_nTop_Status);
#else
	int iMargin = 5;
	MoveWindow_Handler(5, m_nTop_Status, cx - iMargin - iMargin, m_nDefInfo_Height - iMargin - m_nTop_Status);
#endif	//ADD_SOCKET_EES_XML
}

//=============================================================================
// Method		: MoveWindow_Handler
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
void CWnd_Status_Handler::MoveWindow_Handler(int x, int y, int nWidth, int nHeight)
{
	int iSpacing	= 5;	
	int iCvWidth	= nWidth - iSpacing - m_nPort_Width - m_nCtrl_Height;
	int iLeft		= iSpacing;
	int iTop		= nHeight - m_nCtrl_Height;

	// 설비 상태 정보 ----------------------------
#if (ADD_SOCKET_EES_XML)
	iLeft = iSpacing;
	iTop = (y + iSpacing + iSpacing);
	m_st_SubEqpID.MoveWindow(iLeft, iTop, nWidth, m_nCtrl_Height);
	iTop += (m_nCtrl_Height - 1);
	m_st_EquipmentID[CvI_R_Buffer_1].MoveWindow(iLeft, iTop, nWidth, m_nCtrl_Height);
#else
	iTop = y + nHeight - m_nCtrl_Height - iSpacing;
	m_st_Port[PtI_R_Buffer_2].MoveWindow(iLeft, iTop, m_nPort_Width + 1, m_nCtrl_Height);
	iLeft += m_nPort_Width;
	m_st_Conveyor[CvI_R_Buffer_2].MoveWindow(iLeft, iTop, iCvWidth, m_nCtrl_Height);

	iLeft += iCvWidth + iSpacing;
	m_st_Port[PtI_R_Transfer].MoveWindow(iLeft, iTop, m_nCtrl_Height, m_nCtrl_Height);
	iTop -= (m_nLine_Depth - m_nCtrl_Height);
	m_st_Conveyor[CvI_R_Transfer].MoveWindow(iLeft, iTop, m_nCtrl_Height, m_nLine_Depth + 1 - m_nCtrl_Height);

	iLeft = iSpacing;
	m_st_Conveyor[CvI_R_Buffer_1].MoveWindow(iLeft, iTop, iCvWidth + 1, m_nCtrl_Height);
	iLeft += iCvWidth;
	m_st_Port[PtI_R_Buffer_1].MoveWindow(iLeft, iTop, m_nPort_Width, m_nCtrl_Height);
#endif	//ADD_SOCKET_EES_XML
}

void CWnd_Status_Handler::Init_Returner()
{

}

void CWnd_Status_Handler::Popup_SubMenu()
{
	__super::Popup_SubMenu();
}
#if (USE_XML)
//=============================================================================
// Method		: MoveWindow_EES
// Access		: protected  
// Returns		: void
// Parameter	: int x
// Parameter	: int y
// Parameter	: int nWidth
// Parameter	: int nHeight
// Qualifier	:
// Last Update	: 2023.05.25
// Desc.		:
//=============================================================================
void CWnd_Status_Handler::Set_EquipmentID(__in CEquipment* IN_Data)
{
	__super::Set_EquipmentID(IN_Data);
	m_st_SubEqpID.SetText(IN_Data->Get_SubEqpID());
	m_st_EquipmentID[PtI_L_Load].SetText(IN_Data->Get_EquipmentIDStatus(PtI_L_Load).Get_EQUIPMENTID());
}
#endif // SOCKET