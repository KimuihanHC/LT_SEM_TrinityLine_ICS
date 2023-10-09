//*****************************************************************************
// Filename	: 	Wnd_Status_Loader.cpp
// Created	:	2016/3/14 - 10:57
// Modified	:	2016/3/14 - 10:57
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
// Wnd_Status_Loader.cpp : implementation file
//

#include "stdafx.h"
#include "Wnd_Status_Loader.h"
#include "Def_Equipment_Type.h"

//-----------------------------------------------------------------------------
// CWnd_Status_Loader
//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CWnd_Status_Loader, CUI_Equipment)

CWnd_Status_Loader::CWnd_Status_Loader()
{
	m_nPortCount		= PtI_L_MaxCount;
	m_nConveyorCount	= CvI_L_MaxCount;
}

CWnd_Status_Loader::~CWnd_Status_Loader()
{

}

BEGIN_MESSAGE_MAP(CWnd_Status_Loader, CUI_Equipment)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CWnd_Status_Loader message handlers
//=============================================================================
// Method		: OnCreate
// Access		: public  
// Returns		: int
// Parameter	: LPCREATESTRUCT lpCreateStruct
// Qualifier	:
// Last Update	: 2016/3/17 - 10:04
// Desc.		:
//=============================================================================
int CWnd_Status_Loader::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
 	DWORD dwStyle = WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
 	CRect rectDummy;
 	rectDummy.SetRectEmpty();

	// 설비 상태 정보 ----------------------------

	for (auto nIdx = 0; nIdx < PtI_L_MaxCount; ++nIdx)
	{
		m_st_Port[nIdx].Create(_T(""), dwStyle /*| WS_BORDER*/ | SS_CENTER, rectDummy, this, IDC_STATIC);
	}

	LPCTSTR g_szLd_Conveyor[CvI_L_MaxCount] =
	{
		_T("Load"),		// 	CvI_L_Load,
		_T("RFID"),		// 	CvI_L_RFID,
		_T("Transfer"),	// 	CvI_L_Transfer,
		_T("Buf 1"),	// 	CvI_L_Buffer_1,
		_T("Buf 2"),	// 	CvI_L_Buffer_2,
		_T("Buf 3"),	// 	CvI_L_Buffer_3,
		_T("RFID"),		// 	CvI_L_Un_RFID,
		_T("NG"),		// 	CvI_L_Un_NG,
		_T("OK"),		// 	CvI_L_Un_OK,
		_T("Clean"),	// 	CvI_L_Un_Clean,
	};

	for (auto nIdx = 0; nIdx < CvI_L_MaxCount; ++nIdx)
	{
		m_st_Conveyor[nIdx].SetFont_Gdip(L"Arial", 8.0F);
		m_st_Conveyor[nIdx].Create(g_szLd_Conveyor[nIdx], dwStyle /*| WS_BORDER*/ | SS_CENTER, rectDummy, this, IDC_STATIC);
	}

#if defined(EES_XML)//20231003
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
void CWnd_Status_Loader::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	int iSpacing	= 5;
	int iLeft		= 15;
	int iTop		= m_nTop_Status; // 타이틀 바, LED, 설비 Config 제외 위치

	//MoveWindow_Loader(5, m_nTop_Status, cx - iMargin - iMargin, cy - iMargin - m_nTop_Status);
	MoveWindow_Loader(5, m_nTop_Status, cx - iSpacing - iSpacing, m_nDefInfo_Height - iSpacing - m_nTop_Status);

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
// Method		: MoveWindow_Loader
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
void CWnd_Status_Loader::MoveWindow_Loader(int x, int y, int nWidth, int nHeight)
{
	int iMargin		= 5;
	int iSpacing	= 5;
	int iLeft		= x;
	int iTop		= y;
	int iCvWidth	= (nWidth - (iSpacing * 3) - (m_nPort_Width * 5)) / 4;

	// Unloader
	iTop = y + nHeight - m_nCtrl_Height - iSpacing;
	m_st_Port[PtI_L_Un_Clean].MoveWindow(iLeft, iTop, m_nPort_Width + 1, m_nCtrl_Height);
	iLeft += m_nPort_Width;
	m_st_Conveyor[CvI_L_Un_Clean].MoveWindow(iLeft, iTop, iCvWidth, m_nCtrl_Height);

	iLeft += iCvWidth + iSpacing;
	m_st_Port[PtI_L_Un_OK].MoveWindow(iLeft, iTop, m_nPort_Width + 1, m_nCtrl_Height);
	iLeft += m_nPort_Width;
	m_st_Conveyor[CvI_L_Un_OK].MoveWindow(iLeft, iTop, iCvWidth + 1, m_nCtrl_Height);
	iLeft += iCvWidth;
	m_st_Port[PtI_L_Un_OK_Wait].MoveWindow(iLeft, iTop, m_nPort_Width, m_nCtrl_Height);

	iLeft += m_nPort_Width + iSpacing;
	m_st_Port[PtI_L_Un_NG].MoveWindow(iLeft, iTop, m_nPort_Width + 1, m_nCtrl_Height);
	iLeft += m_nPort_Width;
	m_st_Conveyor[CvI_L_Un_NG].MoveWindow(iLeft, iTop, iCvWidth, m_nCtrl_Height);

	iLeft += iCvWidth + iSpacing;
	m_st_Port[PtI_L_Un_RFID].MoveWindow(iLeft, iTop, m_nPort_Width + 1, m_nCtrl_Height);
	iLeft += m_nPort_Width;
	m_st_Conveyor[CvI_L_Un_RFID].MoveWindow(iLeft, iTop, iCvWidth, m_nCtrl_Height);

	// Loader
	iCvWidth	= (nWidth - (iSpacing * 2) - (m_nPort_Width * 2) - m_nCtrl_Height) / 2;
	iLeft = x + m_nCtrl_Height + iSpacing;
	iTop -= (m_nCtrl_Height + iSpacing);
	m_st_Port[PtI_L_RFID].MoveWindow(iLeft, iTop, m_nPort_Width + 1, m_nCtrl_Height);
	iLeft += m_nPort_Width;
	m_st_Conveyor[CvI_L_RFID].MoveWindow(iLeft, iTop, iCvWidth, m_nCtrl_Height);

	iLeft += iCvWidth + iSpacing;
	m_st_Port[PtI_L_Load].MoveWindow(iLeft, iTop, m_nPort_Width + 1, m_nCtrl_Height);
	iLeft += m_nPort_Width;
	m_st_Conveyor[CvI_L_Load].MoveWindow(iLeft, iTop, iCvWidth, m_nCtrl_Height);

	// 배출 버퍼
	iCvWidth = (nWidth - (iSpacing * 3) - (m_nPort_Width * 3) - m_nCtrl_Height) / 3;
	int iLine_Depth = m_nLine_Depth - (m_nCtrl_Height + iSpacing);
	iLeft = x;
	iTop -= (iLine_Depth - m_nCtrl_Height);
	m_st_Port[PtI_L_Transfer].MoveWindow(iLeft, iTop, m_nCtrl_Height, m_nCtrl_Height + 1);
	m_st_Conveyor[CvI_L_Transfer].MoveWindow(iLeft, iTop + m_nCtrl_Height, m_nCtrl_Height, iLine_Depth - m_nCtrl_Height);

	iLeft += m_nCtrl_Height + iSpacing;
	m_st_Conveyor[CvI_L_Buffer_1].MoveWindow(iLeft, iTop, iCvWidth + 1, m_nCtrl_Height);
	iLeft += iCvWidth;
	m_st_Port[PtI_L_Buffer_1].MoveWindow(iLeft, iTop, m_nPort_Width, m_nCtrl_Height);
	
	iLeft += m_nPort_Width + iSpacing;
	m_st_Conveyor[CvI_L_Buffer_2].MoveWindow(iLeft, iTop, iCvWidth + 1, m_nCtrl_Height);
	iLeft += iCvWidth;
	m_st_Port[PtI_L_Buffer_2].MoveWindow(iLeft, iTop, m_nPort_Width, m_nCtrl_Height);
	
	iLeft += m_nPort_Width + iSpacing;
	m_st_Conveyor[CvI_L_Buffer_3].MoveWindow(iLeft, iTop, iCvWidth + 1, m_nCtrl_Height);
	iLeft += iCvWidth;
	m_st_Port[PtI_L_Buffer_3].MoveWindow(iLeft, iTop, m_nPort_Width, m_nCtrl_Height);
	

}

void CWnd_Status_Loader::Init_Loader()
{

}

void CWnd_Status_Loader::Popup_SubMenu()
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
void CWnd_Status_Loader::Set_PortClear(__in uint8_t IN_nPortIndex)
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
void CWnd_Status_Loader::Set_PortStatus(__in uint8_t IN_nPortIndex, __in uint8_t IN_nStatus)
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

void CWnd_Status_Loader::Set_PortStatus(__in uint8_t IN_nPortIndex, __in const ST_PortStatus* IN_nStatus)
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

void CWnd_Status_Loader::Set_ConveyorStatus(__in uint8_t IN_nConveyorIndex, __in uint8_t IN_nStatus, __in uint8_t IN_nExistSocket)
{
	if (IN_nConveyorIndex < m_nConveyorCount)
	{
		SetUI_ConveyorStatus(&m_st_Conveyor[IN_nConveyorIndex], IN_nStatus, IN_nExistSocket);
	}
	else // 오류
	{

	}
}

void CWnd_Status_Loader::Set_ConveyorStatus(__in uint8_t IN_nConveyorIndex, __in const ST_ConveyorStatus* IN_nStatus)
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
void CWnd_Status_Loader::Set_EquipmentID(__in CEquipment* IN_Data)
{
	__super::Set_EquipmentID(IN_Data);
	m_st_SubEqpID.SetText(IN_Data->Get_SubEqpID());
	m_st_EquipmentID[PtI_L_Load].SetText(IN_Data->Get_EquipmentIDStatus(PtI_L_Load).Get_EQUIPMENTID());
}
#endif // SOCKET