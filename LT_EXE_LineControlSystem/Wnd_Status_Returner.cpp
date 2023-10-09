//*****************************************************************************
// Filename	: 	Wnd_Status_Returner.cpp
// Created	:	2021/12/16 - 17:58
// Modified	:	2021/12/16 - 17:58
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
// Wnd_Status_Returner.cpp : implementation file
//

#include "stdafx.h"
#include "Wnd_Status_Returner.h"
#include "Def_Equipment_Type.h"

//-----------------------------------------------------------------------------
// CWnd_Status_Returner
//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CWnd_Status_Returner, CUI_Equipment)

CWnd_Status_Returner::CWnd_Status_Returner()
{
	m_nPortCount		= PtI_R_MaxCount;
	m_nConveyorCount	= CvI_R_MaxCount;
}

CWnd_Status_Returner::~CWnd_Status_Returner()
{

}

BEGIN_MESSAGE_MAP(CWnd_Status_Returner, CUI_Equipment)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CWnd_Status_Returner message handlers
//=============================================================================
// Method		: OnCreate
// Access		: public  
// Returns		: int
// Parameter	: LPCREATESTRUCT lpCreateStruct
// Qualifier	:
// Last Update	: 2016/3/17 - 10:04
// Desc.		:
//=============================================================================
int CWnd_Status_Returner::OnCreate(LPCREATESTRUCT lpCreateStruct)
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

#if defined(EES_XML)//20231003
	m_st_SubEqpID.SetFont_Gdip(L"Arial", 7.0F);
	m_st_SubEqpID.Create(_T(" "), dwStyle /*| WS_BORDER*/ | SS_CENTER, rectDummy, this, IDC_STATIC);
	for (auto nIdx = 0; nIdx < PtI_R_MaxCount; ++nIdx)
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
void CWnd_Status_Returner::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	int iMargin = 5;

	//MoveWindow_Returner(5, m_nTop_Status, cx - iMargin - iMargin, cy - iMargin - m_nTop_Status);
	MoveWindow_Returner(5, m_nTop_Status, cx - iMargin - iMargin, m_nDefInfo_Height - iMargin - m_nTop_Status);
}

//=============================================================================
// Method		: MoveWindow_Returner
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
void CWnd_Status_Returner::MoveWindow_Returner(int x, int y, int nWidth, int nHeight)
{
	int iSpacing	= 5;	
	int iCvWidth	= nWidth - iSpacing - m_nPort_Width - m_nCtrl_Height;
	int iLeft		= iSpacing;
	int iTop		= nHeight - m_nCtrl_Height;

	// 설비 상태 정보 ----------------------------

	// ---ㅁ	|
	//		|
	// ㅁ---ㅁ

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


#if defined(EES_XML)//20231003
	iLeft = iSpacing;
	iTop = (y + iSpacing + iSpacing);
	m_st_SubEqpID.MoveWindow(iLeft, iTop, nWidth, m_nCtrl_Height);	
	iTop += (m_nCtrl_Height - 1);
	m_st_EquipmentID[CvI_R_Buffer_1].MoveWindow(iLeft, iTop, nWidth, m_nCtrl_Height);
#endif //ADD_SOCKET_EES_XML
}

void CWnd_Status_Returner::Init_Returner()
{

}

void CWnd_Status_Returner::Popup_SubMenu()
{
	__super::Popup_SubMenu();
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
void CWnd_Status_Returner::Set_PortClear(__in uint8_t IN_nPortIndex)
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
void CWnd_Status_Returner::Set_PortStatus(__in uint8_t IN_nPortIndex, __in uint8_t IN_nStatus)
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

void CWnd_Status_Returner::Set_PortStatus(__in uint8_t IN_nPortIndex, __in const ST_PortStatus* IN_nStatus)
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

void CWnd_Status_Returner::Set_ConveyorStatus(__in uint8_t IN_nConveyorIndex, __in uint8_t IN_nStatus, __in uint8_t IN_nExistSocket)
{
	if (IN_nConveyorIndex < m_nConveyorCount)
	{
		SetUI_ConveyorStatus(&m_st_Conveyor[IN_nConveyorIndex], IN_nStatus, IN_nExistSocket);
	}
	else // 오류
	{

	}
}

void CWnd_Status_Returner::Set_ConveyorStatus(__in uint8_t IN_nConveyorIndex, __in const ST_ConveyorStatus* IN_nStatus)
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
void CWnd_Status_Returner::Set_EquipmentID(__in CEquipment* IN_Data)
{
	__super::Set_EquipmentID(IN_Data);
	m_st_SubEqpID.SetText(IN_Data->Get_SubEqpID());
	m_st_EquipmentID[PtI_R_Buffer_1].SetText(IN_Data->Get_EquipmentIDStatus(PtI_R_Buffer_1).Get_EQUIPMENTID());
}
#endif // SOCKET