//*****************************************************************************
// Filename	: 	Wnd_Cfg_Alarm.cpp
// Created	:	2016/3/14 - 10:57
// Modified	:	2016/3/14 - 10:57
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
// Wnd_Cfg_Alarm.cpp : implementation file
//

#include "stdafx.h"
#include "Wnd_Cfg_Alarm.h"
#include "resource.h"
#include "XmlSocket.h"
#include "CommonFunction.h"



#define		IDC_LC_SOCKETLIST	1030

#define		IDC_BN_ADD			1040
#define		IDC_BN_REMOVE		1041
#define		IDC_BN_SAVE			1050


#define		TABSTYLE_COUNT			3
static UINT g_TabOrder[TABSTYLE_COUNT] =
{
	IDC_BN_ADD, 
	IDC_BN_REMOVE, 
	IDC_BN_SAVE, 
};

// CWnd_Cfg_Alarm
IMPLEMENT_DYNAMIC(CWnd_Cfg_Alarm, CWnd_BaseView)

//=============================================================================
//
//=============================================================================
CWnd_Cfg_Alarm::CWnd_Cfg_Alarm()
{
	VERIFY(m_font_Default.CreateFont(
		16,						// nHeight
		0,						// nWidth
		0,						// nEscapement
		0,						// nOrientation
		FW_MEDIUM,				// nWeight
		FALSE,					// bItalic
		FALSE,					// bUnderline
		0,						// cStrikeOut
		ANSI_CHARSET,			// nCharSet
		OUT_DEFAULT_PRECIS,		// nOutPrecision
		CLIP_DEFAULT_PRECIS,	// nClipPrecision
		ANTIALIASED_QUALITY,	// nQuality
		DEFAULT_PITCH,			// nPitchAndFamily
		_T("Tahoma")));			// lpszFacename

	VERIFY(m_font_Data.CreateFont(
		20,						// nHeight
		0,						// nWidth
		0,						// nEscapement
		0,						// nOrientation
		FW_SEMIBOLD,			// nWeight
		FALSE,					// bItalic
		FALSE,					// bUnderline
		0,						// cStrikeOut
		ANSI_CHARSET,			// nCharSet
		OUT_DEFAULT_PRECIS,		// nOutPrecision
		CLIP_DEFAULT_PRECIS,	// nClipPrecision
		ANTIALIASED_QUALITY,	// nQuality
		DEFAULT_PITCH,			// nPitchAndFamily
		_T("Tahoma")));			// lpszFacename

}

//=============================================================================
//
//=============================================================================
CWnd_Cfg_Alarm::~CWnd_Cfg_Alarm()
{
	m_font_Default.DeleteObject();
	m_font_Data.DeleteObject();
}


BEGIN_MESSAGE_MAP(CWnd_Cfg_Alarm, CWnd_BaseView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BN_ADD,						OnBnClicked_Add)
	ON_BN_CLICKED(IDC_BN_REMOVE,					OnBnClicked_Remove)
	ON_BN_CLICKED(IDC_BN_SAVE,						OnBnClicked_Apply)
END_MESSAGE_MAP()


// CWnd_Cfg_Alarm message handlers
//=============================================================================
// Method		: OnCreate
// Access		: public  
// Returns		: int
// Parameter	: LPCREATESTRUCT lpCreateStruct
// Qualifier	:
// Last Update	: 2016/3/17 - 11:33
// Desc.		:
//=============================================================================
int CWnd_Cfg_Alarm::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd_BaseView::OnCreate(lpCreateStruct) == -1)
		return -1;

	DWORD dwStyle = WS_VISIBLE | WS_CHILD /*| WS_CLIPCHILDREN*/ | WS_CLIPSIBLINGS;
	DWORD dwTempStyle = dwStyle;
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	

	m_lc_SocketList.Create(dwStyle | WS_VSCROLL, rectDummy, this, IDC_LC_SOCKETLIST);

	m_bn_Add.Create(_T("Insert"), dwStyle | WS_BORDER | BS_PUSHBUTTON, rectDummy, this, IDC_BN_ADD);
	m_bn_Remove.Create(_T("Delete"), dwStyle | WS_BORDER | BS_PUSHBUTTON, rectDummy, this, IDC_BN_REMOVE);
	m_bn_Apply.Create(_T("Apply"), dwStyle | WS_BORDER | BS_PUSHBUTTON, rectDummy, this, IDC_BN_SAVE);

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
// Last Update	: 2016/3/17 - 11:33
// Desc.		:
//=============================================================================
void CWnd_Cfg_Alarm::OnSize(UINT nType, int cx, int cy)
{
	CWnd_BaseView::OnSize(nType, cx, cy);

// 	if ((cx == 0) && (cy == 0))
// 		return;
// 
// 	int iMargin			= 10;
// 	int iSpacing		= 5;
// 	int iCateSpacing	= 10;
// 	int iLeft			= iMargin;
// 	int iTop			= iMargin;
// 	int iWidth			= cx - iMargin - iMargin;
// 	int iHeight			= cy - iMargin - iMargin;
// 	int iCtrlHeight		= 30;
// 	int iCtrlWidth		= 400;
// 
// 	// 리스트 컨트롤
// 	iTop = iMargin;
// 	iLeft = iMargin + iCtrlWidth + iCateSpacing;;
// 	iWidth = cx - iLeft - iMargin;
// 	m_lc_SocketList.MoveWindow(iLeft, iTop, iWidth, iHeight);

}

//=============================================================================
// Method		: PreCreateWindow
// Access		: virtual public  
// Returns		: BOOL
// Parameter	: CREATESTRUCT & cs
// Qualifier	:
// Last Update	: 2016/3/17 - 11:33
// Desc.		:
//=============================================================================
BOOL CWnd_Cfg_Alarm::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1), NULL);

	return CWnd_BaseView::PreCreateWindow(cs);
}

//=============================================================================
// Method		: PreTranslateMessage
// Access		: virtual public  
// Returns		: BOOL
// Parameter	: MSG * pMsg
// Qualifier	:
// Last Update	: 2016/3/17 - 11:33
// Desc.		:
//=============================================================================
BOOL CWnd_Cfg_Alarm::PreTranslateMessage(MSG* pMsg)
{
	switch (pMsg->message)
	{
	case WM_CHAR:
		if (VK_TAB == pMsg->wParam)
		{
			UINT nID = GetFocus()->GetDlgCtrlID();
			UINT nID_Old = nID;
			for (int iCnt = 0; iCnt < TABSTYLE_COUNT; iCnt++)
			{
				if (nID == g_TabOrder[iCnt])
				{
					if ((TABSTYLE_COUNT - 1) == iCnt)
					{
						nID = g_TabOrder[0];
					}
					else
					{
						// Disable 된 컨트롤은 tab 처리가 안됨
						for (int iDst = iCnt + 1; iDst < TABSTYLE_COUNT; iDst++)
						{
							if (GetDlgItem(g_TabOrder[iDst])->IsWindowEnabled())
							{
								nID = g_TabOrder[iDst];
								break;
							}
						}
						// 다른 컨트롤이 모두 Diable이면 초기 컨트롤 설정
						if (nID == g_TabOrder[iCnt])
						{
							nID = g_TabOrder[0];
						}
					}

					break;
				}
			}

			if (nID != nID_Old)
			{
				GetDlgItem(nID)->SetFocus();
			}
		}
		break;

	default:
		break;
	}

	return CWnd_BaseView::PreTranslateMessage(pMsg);
}

//=============================================================================
// Method		: OnBnClicked_Add
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/11/15 - 11:21
// Desc.		:
//=============================================================================
void CWnd_Cfg_Alarm::OnBnClicked_Add()
{
	Item_Insert();
}

//=============================================================================
// Method		: OnBnClicked_Remove
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/11/15 - 11:21
// Desc.		:
//=============================================================================
void CWnd_Cfg_Alarm::OnBnClicked_Remove()
{
	if (0 < m_lc_SocketList.GetSelectedCount())
	{
		Item_Remove();
	}
}

//=============================================================================
// Method		: OnBnClicked_Apply
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/11/15 - 11:21
// Desc.		:
//=============================================================================
void CWnd_Cfg_Alarm::OnBnClicked_Apply()
{
	// 파일에 저장
	if (SaveXML_SocketList())
	{
		LT_MessageBox(_T("The file has been saved successfully."));
	}
	else
	{
		LT_MessageBox(_T("Failed to save file !!"));
	}
}

//=============================================================================
// Method		: Item_Insert
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/11/15 - 11:21
// Desc.		:
//=============================================================================
void CWnd_Cfg_Alarm::Item_Insert()
{
	
}

//=============================================================================
// Method		: Item_Remove
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/11/15 - 11:22
// Desc.		:
//=============================================================================
void CWnd_Cfg_Alarm::Item_Remove()
{
	
}

//=============================================================================
// Method		: Item_Modify
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/11/15 - 11:22
// Desc.		:
//=============================================================================
void CWnd_Cfg_Alarm::Item_Modify()
{

	//m_lc_SocketList.Item_Modify();
}

//=============================================================================
// Method		: SaveXML_SocketList
// Access		: public  
// Returns		: BOOL
// Qualifier	:
// Last Update	: 2021/11/15 - 11:22
// Desc.		:
//=============================================================================
BOOL CWnd_Cfg_Alarm::SaveXML_SocketList()
{
	CXmlSocket xml;

	CSocketList stSocketList;
	m_lc_SocketList.Get_SocketList(stSocketList);

	return xml.SaveXML_Socket_CfgList(m_szSavePath.GetBuffer(), &stSocketList);
}

//=============================================================================
// Method		: LoadXML_SocketList
// Access		: public  
// Returns		: BOOL
// Qualifier	:
// Last Update	: 2021/11/17 - 16:59
// Desc.		:
//=============================================================================
BOOL CWnd_Cfg_Alarm::LoadXML_SocketList()
{
	CXmlSocket xml;
	CSocketList stSocketList;

	if (xml.LoadXML_Socket_CfgList(m_szSavePath.GetBuffer(), stSocketList))
	{
		m_lc_SocketList.Set_SocketList(&stSocketList);
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

//=============================================================================
// Method		: Get_UIData
// Access		: public  
// Returns		: void
// Parameter	: __out CConfig_Model & OUT_stSocket
// Qualifier	:
// Last Update	: 2021/6/14 - 16:33
// Desc.		:
//=============================================================================
void CWnd_Cfg_Alarm::Get_UIData(__out CConfig_Socket& OUT_stSocket)
{
	CString szValue;	
	CString szNumber;

	

}

//=============================================================================
// Method		: Set_UIData
// Access		: public  
// Returns		: void
// Parameter	: __in const CConfig_Model * IN_pSocket
// Qualifier	:
// Last Update	: 2021/6/14 - 16:33
// Desc.		:
//=============================================================================
void CWnd_Cfg_Alarm::Set_UIData(__in const CConfig_Socket* IN_pSocket)
{
	
	
}

//=============================================================================
// Method		: Set_PermissionMode
// Access		: public  
// Returns		: void
// Parameter	: __in enPermissionMode IN_PermissionMode
// Qualifier	:
// Last Update	: 2022/1/7 - 11:11
// Desc.		:
//=============================================================================
void CWnd_Cfg_Alarm::Set_PermissionMode(__in enPermissionMode IN_PermissionMode)
{

}

//=============================================================================
// Method		: Get_Option
// Access		: public  
// Returns		: void
// Parameter	: __out CConfig_Model & OUT_stSocketList
// Qualifier	:
// Last Update	: 2021/6/14 - 16:34
// Desc.		:
//=============================================================================
void CWnd_Cfg_Alarm::Get_Configuration(__out CSocketList& OUT_stSocketList)
{
	m_lc_SocketList.Get_SocketList(OUT_stSocketList);
}

//=============================================================================
// Method		: Set_Option
// Access		: public  
// Returns		: void
// Parameter	: __in const CConfig_Model * IN_pSocketList
// Qualifier	:
// Last Update	: 2021/6/14 - 16:34
// Desc.		:
//=============================================================================
void CWnd_Cfg_Alarm::Set_Configuration(__in const CSocketList* IN_pSocketList)
{
	m_lc_SocketList.Set_SocketList(IN_pSocketList);
}

//=============================================================================
// Method		: Set_FilePath
// Access		: public  
// Returns		: void
// Parameter	: __in LPCTSTR szFilePath
// Qualifier	:
// Last Update	: 2021/11/17 - 17:01
// Desc.		:
//=============================================================================
void CWnd_Cfg_Alarm::Set_FilePath(__in LPCTSTR szFilePath)
{
	m_szSavePath = szFilePath;
}
