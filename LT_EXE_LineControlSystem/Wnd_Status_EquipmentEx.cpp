//*****************************************************************************
// Filename	: 	Wnd_Status_EquipmentEx.cpp
// Created	:	2016/3/14 - 10:57
// Modified	:	2016/3/14 - 10:57
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
// Wnd_Status_EquipmentEx.cpp : implementation file
//

#include "stdafx.h"
#include "Wnd_Status_EquipmentEx.h"
#include "Def_Equipment_Type.h"
#include "Dlg_PopupMenu.h"


//-----------------------------------------------------------------------------
// CWnd_Status_EquipmentEx
//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CWnd_Status_EquipmentEx, CWnd_Status_Equipment)

CWnd_Status_EquipmentEx::CWnd_Status_EquipmentEx()
{
// 	m_sizeCaption.SetSize(120, 20);
// 	m_sizeData.SetSize(160, 20);
// 	m_Margin.left	= 0;
// 	m_Margin.top	= 0;
// 	m_Margin.right	= 0;
// 	m_Margin.bottom	= 0;
// 	m_nSpacing		= 5;
// 	m_nCateSpacing	= 10;
// 
 	m_nFontSize_Caption.SetSize(0, 11);
 	m_nFontSize_Data.SetSize(0, 11);
}

CWnd_Status_EquipmentEx::~CWnd_Status_EquipmentEx()
{
	Release_Font();

}

BEGIN_MESSAGE_MAP(CWnd_Status_EquipmentEx, CWnd_Status_Equipment)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CWnd_Status_EquipmentEx message handlers
//=============================================================================
// Method		: OnCreate
// Access		: public  
// Returns		: int
// Parameter	: LPCREATESTRUCT lpCreateStruct
// Qualifier	:
// Last Update	: 2016/3/17 - 10:04
// Desc.		:
//=============================================================================
int CWnd_Status_EquipmentEx::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	// 타이틀바, 외곽선
	if (CWnd_Status_Equipment::OnCreate(lpCreateStruct) == -1)
		return -1;

 	DWORD dwStyle = WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
 	CRect rectDummy;
 	rectDummy.SetRectEmpty();


	

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
void CWnd_Status_EquipmentEx::OnSize(UINT nType, int cx, int cy)
{
 	CWnd_Status_Base::OnSize(nType, cx, cy);	

	int iMargin			= 5;
	int iSpacing		= 5;
	int iLeft			= 5;
	int iTop			= m_nTitle_Height + iMargin;
	int iWidth			= cx - iMargin - iMargin;
	int iHeight			= m_nDefInfo_Height - iMargin - iMargin;
	int iBnHeight		= m_nTitle_Height - 6;
	int iBnWidth		= iBnHeight;

	// 설비 제어 버튼 ----------------------------
	iTop = 3;
	iLeft = cx - iMargin - iBnWidth;
	m_bn_Control.MoveWindow(iLeft, iTop, iBnWidth, iBnHeight);

	// View 전환 Radio 버튼 ----------------------
	//iLeft = iMargin;
	//m_rb_Socket.MoveWindow(iLeft, iTop, iBnWidth, iBnHeight);
	//iLeft += iSpacing + iBnWidth;
	//m_rb_Status.MoveWindow(iLeft, iTop, iBnWidth, iBnHeight);

	iTop = m_nTitle_Height + iSpacing;
	MoveWindow_Status(iLeft, iTop, iWidth, iHeight);

	// 320 높이까지 기존 정보 표시?





// 	__super::OnSize(nType, cx, cy);
// 
// 
// 	int iMargin			= 5;
// 	int iSpacing		= 5;
// 	int iLeft			= 5;
// 	int iTop			= m_nTitle_Height + iMargin;
// 	int iWidth			= cx - iMargin - iMargin;
// 	int iHeight			= cy - iMargin - iMargin;
// 	int iBnHeight		= m_nTitle_Height - 6;
// 	int iBnWidth		= iBnHeight;

	
}


//=============================================================================
// Method		: MoveWindow_Status
// Access		: protected  
// Returns		: void
// Parameter	: int x
// Parameter	: int y
// Parameter	: int nWidth
// Parameter	: int nHeight
// Qualifier	:
// Last Update	: 2021/12/16 - 17:54
// Desc.		:
//=============================================================================
void CWnd_Status_EquipmentEx::MoveWindow_Status(int x, int y, int nWidth, int nHeight)
{
	__super::MoveWindow_Status(x, y, nWidth, nHeight);


	
	
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
void CWnd_Status_EquipmentEx::MoveWindow_Tester(int x, int y, int nWidth, int nHeight)
{
	

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
void CWnd_Status_EquipmentEx::MoveWindow_Loader(int x, int y, int nWidth, int nHeight)
{
	
}

//=============================================================================
// Method		: MoveWindow_Unloader
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
void CWnd_Status_EquipmentEx::MoveWindow_Unloader(int x, int y, int nWidth, int nHeight)
{
	
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
void CWnd_Status_EquipmentEx::MoveWindow_Returner(int x, int y, int nWidth, int nHeight)
{

}

//=============================================================================
// Method		: Popup_SubMenu
// Access		: virtual protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2022/1/17 - 18:03
// Desc.		:
//=============================================================================
// void CWnd_Status_EquipmentEx::Popup_SubMenu()
// {
// 	CDlg_PopupMenu dlgMenu(this);
// 
// 	CRect rtClient;
// 	GetClientRect(&rtClient);
// 	ClientToScreen(rtClient);
// 
// 	dlgMenu.Set_InitPos(rtClient.left + 50, rtClient.top + 50);
// 	if (m_pEquipmentStatus)
// 	{
// 		dlgMenu.Set_Title(m_pEquipmentStatus->szAlias);
// 		dlgMenu.Set_EquipmentID(m_pEquipmentStatus->szEquipmentId, m_nEqpOrder);
// 	}
// 	dlgMenu.DoModal();
// }

//=============================================================================
// Method		: Set_PortClear
// Access		: virtual public  
// Returns		: void
// Parameter	: __in uint8_t IN_nPortIndex
// Qualifier	:
// Last Update	: 2022/1/13 - 11:07
// Desc.		:
//=============================================================================
void CWnd_Status_EquipmentEx::Set_PortClear(__in uint8_t IN_nPortIndex)
{

}

//=============================================================================
// Method		: Set_PortStatus
// Access		: virtual public  
// Returns		: void
// Parameter	: __in uint8_t IN_nPortIndex
// Parameter	: __in uint8_t IN_nStatus
// Qualifier	:
// Last Update	: 2021/12/24 - 14:27
// Desc.		:
//=============================================================================
void CWnd_Status_EquipmentEx::Set_PortStatus(__in uint8_t IN_nPortIndex, __in uint8_t IN_nStatus)
{
	if (IN_nPortIndex < m_nPortCount)
	{

	}
	else if (PtI_T_All == IN_nPortIndex) // Port All
	{

	}
	else // 오류
	{

	}
}

void CWnd_Status_EquipmentEx::Set_PortStatus(__in uint8_t IN_nPortIndex, __in const ST_PortStatus* IN_nStatus)
{

}

//=============================================================================
// Method		: Set_ConveyorStatus
// Access		: virtual public  
// Returns		: void
// Parameter	: __in uint8_t IN_nConveyorIndex
// Parameter	: __in uint8_t IN_nStatus
// Parameter	: __in uint8_t IN_nExistSocket
// Qualifier	:
// Last Update	: 2022/1/13 - 14:46
// Desc.		:
//=============================================================================
void CWnd_Status_EquipmentEx::Set_ConveyorStatus(__in uint8_t IN_nConveyorIndex, __in uint8_t IN_nStatus, __in uint8_t IN_nExistSocket)
{
	if (IN_nConveyorIndex < m_nConveyorCount)
	{

	}
	else // 오류
	{

	}
}

void CWnd_Status_EquipmentEx::Set_ConveyorStatus(__in uint8_t IN_nConveyorIndex, __in const ST_ConveyorStatus* IN_nStatus)
{

}

