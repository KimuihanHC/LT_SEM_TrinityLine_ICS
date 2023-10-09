//*****************************************************************************
// Filename	: 	Wnd_Cfg_Line.cpp
// Created	:	2016/3/14 - 10:57
// Modified	:	2016/3/14 - 10:57
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
// Wnd_Cfg_Line.cpp : implementation file
//

#include "stdafx.h"
#include "Wnd_Cfg_Server.h"
#include "resource.h"
#include "Def_WindowMessage.h"
#include "Dlg_ModifyServer.h"
#include "XmlLineServerConfig.h"
#include "CommonFunction.h"
#include "Def_Language_Message.h"

static LPCTSTR g_szTestCtrl_T[Lang_MaxCount][CWnd_Cfg_Server::EC_MaxEnum] =
{
	// 한국어
	{
		_T("추가"),				// EC_Add
		_T("삽입"),				// EC_Insert
		_T("제거"),				// EC_Remove
		_T("설비 수정"),			// EC_Modify
		_T("위로 이동"),			// EC_Order_Up
		_T("아래로 이동"),		// EC_Order_Down
	},

	// 영어
	{
		_T("Add"),				// EC_Add
		_T("Insert"),			// EC_Insert
		_T("Remove"),			// EC_Remove
		_T("Edit Equipment"),	// EC_Modify
		_T("Up"),				// EC_Order_Up
		_T("Down"),				// EC_Order_Down
	},

	// 베트남어
	{
		_T("Add"),				// EC_Add
		_T("Insert"),			// EC_Insert
		_T("Remove"),			// EC_Remove
		_T("Edit Equipment"),	// EC_Modify
		_T("Up"),				// EC_Order_Up
		_T("Down"),				// EC_Order_Down
	},

	// 중국어
	{
		_T("追加"),				// EC_Add
		_T("插入"),				// EC_Insert
		_T("删除"),				// EC_Remove
		_T("编辑设备"),			// EC_Modify
		_T("向上移动"),			// EC_Order_Up
		_T("向下移动"),			// EC_Order_Down
	},
};

static LPCTSTR g_szItemText_T[Lang_MaxCount][CWnd_Cfg_Server::EID_MaxEnum] =
{
	// 한국어
	{
		_T("설비 별칭"),				// EID_Item
		_T("설비 종류"),				// EID_Item_Para_1
		_T("설비 ID"),				// EID_Item_Para_2
		_T("IP 주소"),				// EID_Item_Para_3
	},

	// 영어
	{
		_T("Equipment Alias"),			// EID_Item
		_T("Equipment Type"),			// EID_Item_Para_1
		_T("Equipment id"),				// EID_Item_Para_2
		_T("IP Address"),				// EID_Item_Para_3
	},

	// 베트남어
	{
		_T("Equipment Alias"),			// EID_Item
		_T("Equipment Type"),			// EID_Item_Para_1
		_T("Equipment id"),				// EID_Item_Para_2
		_T("IP Address"),				// EID_Item_Para_3
	},

	// 중국어
	{
		_T("设备别称"),					// EID_Item
		_T("设备类型"),					// EID_Item_Para_1
		_T("设备ID"),					// EID_Item_Para_2
		_T("IP地址"),					// EID_Item_Para_3
	},
};

static LPCTSTR g_szListCtrl_T[Lang_MaxCount][CWnd_Cfg_Server::XML_MaxCount] =
{
	// 한국어
	{
		_T("목록 불러오기"),		// XML_Load
		_T("목록 저장"),			// XML_Save
		_T("목록 지우기"),		// XML_Clear
	},

	// 영어
	{
		_T("Load List"),		// XML_Load
		_T("Save List"),		// XML_Save
		_T("Clear List"),		// XML_Clear
	},

	// 베트남어
	{
		_T("Load List"),		// XML_Load
		_T("Save List"),		// XML_Save
		_T("Clear List"),		// XML_Clear
	},

	// 중국어
	{
		_T("从文件载入"),		// XML_Load
		_T("保存到文件"),		// XML_Save
		_T("清空列表"),			// XML_Clear
	},
};

enum enCtrlText
{
	Txt_Apply,
	Txt_VerifyLineConfig,

	Txt_MaxCount,
};

static LPCTSTR g_szCtrlText_T[Lang_MaxCount][Txt_MaxCount] =
{
	// 한국어
	{
		_T("적용"),				// Txt_Apply
		_T("라인 설정 검증"),		// Txt_VerifyLineConfig
	},

	// 영어
	{
		_T("Apply"),				// Txt_Apply
		_T("Verify Line Config."),	// Txt_VerifyLineConfig
	},

	// 베트남어
	{
		_T("Apply"),				// Txt_Apply
		_T("Verify Line Config."),	// Txt_VerifyLineConfig
	},

	// 중국어
	{
		_T("适用"),					// Txt_Apply
		_T("验证设置"),				// Txt_VerifyLineConfig
	},
};

// CWnd_Cfg_Line

#define IDC_BN_APPLY			1000

#define IDC_BN_STEPCTRL_S		1100
#define IDC_BN_STEPCTRL_E		IDC_BN_STEPCTRL_S + EC_MaxEnum - 1

#define IDC_LC_STEPLIST			1200
#define IDC_CB_EQP_TYPE			1201
#define IDC_BN_VERIFY_LINE		1202
#define IDC_BN_MODIFY_STEP		1203
#define IDC_CB_SVR_TYPE			1204
#define IDC_ED_DATA_S			1210
#define IDC_ED_DATA_E			IDC_ED_DATA_S + MAX_STEP_DATA - 1

#define IDC_LC_STEPLIST_EES		1300//2023.02.13a 


#define	IDC_BN_LISTCTRL_S		2000
#define	IDC_BN_LISTCTRL_E		IDC_BN_LISTCTRL_S + XML_MaxCount - 1

#define IDC_ED_EQP_ALIAS		3000
#define IDC_ED_EQP_ID			3001
#define IDC_IP_ADDRESS			3002
// #define IDC_CB_TESTZONE_CNT		3003
// #define IDC_CB_BUFF_CNT			3004
// #define IDC_CB_CONVOYER_CNT		3005
// #define IDC_CB_RETURN_CNT		3006

IMPLEMENT_DYNAMIC(CWnd_Cfg_Server, CWnd_BaseView)

CWnd_Cfg_Server::CWnd_Cfg_Server()
{
	VERIFY(m_font_Default.CreateFont(
		13,						// nHeight
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

	VERIFY(m_Font.CreateFont(
		20,						// nHeight  (25 -> 13, 30 -> 20)
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

CWnd_Cfg_Server::~CWnd_Cfg_Server()
{
	m_font_Default.DeleteObject();
	m_Font.DeleteObject();
}


BEGIN_MESSAGE_MAP(CWnd_Cfg_Server, CWnd_BaseView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND_RANGE(IDC_BN_STEPCTRL_S,		IDC_BN_STEPCTRL_E,		OnBnClickedBnEqpCtrl)
	ON_COMMAND_RANGE(IDC_BN_LISTCTRL_S,		IDC_BN_LISTCTRL_E,		OnBnClickedBnListCtrl)
	ON_CBN_SELENDOK	(IDC_CB_EQP_TYPE,		OnCbnSelendokEqpType)
	ON_CBN_SELENDOK (IDC_CB_SVR_TYPE, OnCbnSelendokSvrType)
	ON_BN_CLICKED	(IDC_BN_VERIFY_LINE,	OnBnClickedBnVerifyLine)
	ON_BN_CLICKED	(IDC_BN_APPLY,			OnBnClickedBnApply)
END_MESSAGE_MAP()


// CWnd_Cfg_Line message handlers
//=============================================================================
// Method		: OnCreate
// Access		: public  
// Returns		: int
// Parameter	: LPCREATESTRUCT lpCreateStruct
// Qualifier	:
// Last Update	: 2016/3/17 - 11:33
// Desc.		:
//=============================================================================
int CWnd_Cfg_Server::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd_BaseView::OnCreate(lpCreateStruct) == -1)
		return -1;

	//SetScrollRange(SB_VERT, 0, 10, 0);
	//EnableScrollBarCtrl(SB_VERT, TRUE);

	DWORD dwStyle = WS_VISIBLE | WS_CHILD;
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	m_bn_Apply.Create(g_szCtrlText_T[m_nLanguage][Txt_Apply], dwStyle | BS_PUSHBUTTON, rectDummy, this, IDC_BN_APPLY);

	//---------------------------------------------------------------
	// 설비 리스트
	//---------------------------------------------------------------
	//2023.02.13a uhkim [리스트 추가]
	m_lc_ServerInfo.Create(WS_CHILD | WS_VISIBLE, rectDummy, this, IDC_LC_STEPLIST);
	//m_lc_LineInfo[ICS_SERVER_EES].Create(WS_CHILD | WS_VISIBLE, rectDummy, this, IDC_LC_STEPLIST_EES);

	m_bn_VerifyLineConfig.Create(g_szCtrlText_T[m_nLanguage][Txt_VerifyLineConfig], dwStyle | BS_PUSHLIKE, rectDummy, this, IDC_BN_VERIFY_LINE);
	//m_bn_ModifyEquipment.Create(_T("Edit Equipment"), dwStyle | BS_PUSHLIKE, rectDummy, this, IDC_BN_MODIFY_STEP);

	// 설비 제어
	m_bn_ItemCtrl[EC_Order_Up].SetImage(IDB_ARROW_UP, IDB_ARROW_UP);
	m_bn_ItemCtrl[EC_Order_Down].SetImage(IDB_ARROW_DOWN, IDB_ARROW_DOWN);
	//m_bn_StepCtrl[EC_Order_Down].m_bRightImage = FALSE;
	//m_bn_StepCtrl[EC_Order_Down].m_bTopImage = FALSE;
	for (UINT nIdx = 0; nIdx < EC_MaxEnum; nIdx++)
	{
		m_bn_ItemCtrl[nIdx].m_bTransparent = TRUE;
		m_bn_ItemCtrl[nIdx].Create(g_szTestCtrl_T[m_nLanguage][nIdx], dwStyle | BS_PUSHLIKE, rectDummy, this, IDC_BN_STEPCTRL_S + nIdx);
		m_bn_ItemCtrl[nIdx].SetMouseCursorHand();
	}

	//---------------------------------------------------------------
	// XML 불러오기/저장하기
	//---------------------------------------------------------------
	for (auto nIdx = 0; nIdx < XML_MaxCount; nIdx++)
	{
		m_bn_XmlCtrl[nIdx].m_bTransparent = TRUE;
		m_bn_XmlCtrl[nIdx].Create(g_szListCtrl_T[m_nLanguage][nIdx], dwStyle | BS_PUSHLIKE, rectDummy, this, IDC_BN_LISTCTRL_S + nIdx);
		m_bn_XmlCtrl[nIdx].SetMouseCursorHand();
	}
	//2023.02.13a uhkim [선택 옵션]
	m_cb_ServerType.Create(dwStyle | CBS_DROPDOWNLIST | WS_VSCROLL, rectDummy, this, IDC_CB_SVR_TYPE);
	m_cb_ServerType.SetFont(&m_Font);
#if defined(EES_XML)//20231003
	for (auto nIdx = 0; nIdx < _ICS_SERVER_Type::ICS_SERVER_MAX; nIdx++)
	{
		m_cb_ServerType.AddString(strLogName[nIdx]);
	}
#endif
	m_cb_ServerType.SetCurSel(0);
	//
	//---------------------------------------------------------------
	// 설비 데이터 항목
	//---------------------------------------------------------------
	m_ed_EqpAlias.Create(dwStyle | ES_CENTER | WS_BORDER, rectDummy, this, IDC_ED_EQP_ALIAS);
	m_ed_EqpAlias.EnableWindow(FALSE);

	m_cb_ServerType.Create(dwStyle | CBS_DROPDOWNLIST | WS_VSCROLL, rectDummy, this, IDC_CB_EQP_TYPE);
	m_cb_ServerType.SetFont(&m_Font);
// 	for (auto nIdx = 0; nIdx < enEquipmentType::Max_EqpTypeCount; nIdx++)
// 	{
// 		m_cb_EquipmentType.AddString(g_szEqpTypeName[nIdx]);
// 	}
	for (auto nIdx = 0; nIdx < enServerUI::SERVER_UI_MAX; nIdx++)
	{
		m_cb_ServerType.AddString(g_szServerUIName[nIdx]);
	}
	m_cb_ServerType.SetCurSel(0);

	m_ed_EquipmentId.Create(dwStyle | ES_CENTER | WS_BORDER, rectDummy, this, IDC_ED_EQP_ID);

	m_ed_IPAddress.Create(dwStyle | ES_CENTER | WS_BORDER, rectDummy, this, IDC_IP_ADDRESS);
	m_ed_IPAddress.SetValidChars(_T("0123456789."));
	m_ed_IPAddress.SetWindowText(_T("192.168.0.1"));

// 	static LPCTSTR szZoneCount[] =
// 	{
// 		_T("0"),
// 		_T("1"),
// 		_T("2"),
// 		NULL
// 	};
// 	m_cb_TestZoneCount.Create(dwStyle | CBS_DROPDOWNLIST | WS_VSCROLL, rectDummy, this, IDC_CB_TESTZONE_CNT);
// 	for (auto nIdx = 0; NULL != szZoneCount[nIdx]; nIdx++)
// 	{
// 		m_cb_TestZoneCount.AddString(szZoneCount[nIdx]);
// 	}
// 	m_cb_TestZoneCount.SetCurSel(2);
// 
// 	static LPCTSTR szBufferCount[] =
// 	{
// 		_T("0"),
// 		_T("1"),
// 		NULL
// 	};
// 	m_cb_BufferCount.Create(dwStyle | CBS_DROPDOWNLIST | WS_VSCROLL, rectDummy, this, IDC_CB_BUFF_CNT);
// 	for (auto nIdx = 0; NULL != szBufferCount[nIdx]; nIdx++)
// 	{
// 		m_cb_BufferCount.AddString(szBufferCount[nIdx]);
// 	}
// 	m_cb_BufferCount.SetCurSel(1);
// 
// 	static LPCTSTR szConveyorCount[] =
// 	{
// 		_T("0"),
// 		_T("1"),
// 		NULL
// 	};
// 	m_cb_ConveyorCount.Create(dwStyle | CBS_DROPDOWNLIST | WS_VSCROLL, rectDummy, this, IDC_CB_CONVOYER_CNT);
// 	for (auto nIdx = 0; NULL != szConveyorCount[nIdx]; nIdx++)
// 	{
// 		m_cb_ConveyorCount.AddString(szConveyorCount[nIdx]);
// 	}
// 	m_cb_ConveyorCount.SetCurSel(1);
// 
// 	static LPCTSTR szReturnCount[] =
// 	{
// 		_T("0"),
// 		_T("1"),
// 		NULL
// 	};
// 	m_cb_ReturnConveyorCount.Create(dwStyle | CBS_DROPDOWNLIST | WS_VSCROLL, rectDummy, this, IDC_CB_RETURN_CNT);
// 	for (auto nIdx = 0; NULL != szReturnCount[nIdx]; nIdx++)
// 	{
// 		m_cb_ReturnConveyorCount.AddString(szReturnCount[nIdx]);
// 	}
// 	m_cb_ReturnConveyorCount.SetCurSel(1);

	// 설비 항목
	for (UINT nIdx = 0; nIdx < EID_MaxEnum; nIdx++)
	{
		m_st_EqpItemData[nIdx].SetStaticStyle(CVGStatic::StaticStyle_Default);
		m_st_EqpItemData[nIdx].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_st_EqpItemData[nIdx].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_st_EqpItemData[nIdx].SetFont_Gdip(L"Tahoma", 10.0F, Gdiplus::FontStyleRegular);

		m_st_EqpItemData[nIdx].Create(g_szItemText_T[m_nLanguage][nIdx], dwStyle | SS_CENTER | SS_CENTERIMAGE, rectDummy, this, IDC_STATIC);

		//m_st_EqpItemData[nIdx].ModifyStyleEx(WS_EX_CLIENTEDGE | WS_EX_CLIENTEDGE, NULL, SWP_FRAMECHANGED);
		//m_st_EqpItemData[nIdx].ModifyStyle(NULL, WS_BORDER, SWP_FRAMECHANGED);
	}


	// 포커스 설정
	m_bn_ItemCtrl[EC_Add].SetFocus();

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
void CWnd_Cfg_Server::OnSize(UINT nType, int cx, int cy)
{
	CWnd_BaseView::OnSize(nType, cx, cy);

	if ((cx == 0) && (cy == 0))
		return;

	int iMargin			= 10;
	int iSpacing		= 5;
	int iCateSpacing	= 10;
	int iLeft			= iMargin;
	int iTop			= iMargin;
	int iWidth			= cx - iMargin - iMargin;
	int iHeight			= cy - iMargin - iMargin;
	int iCtrlHeight		= 40;
	//int iCtrlWidth		= 400;//2023.02.13a List추가 (크기)
	int iCtrlWidth		= 300;
	//int iStWidth		= 140;//2023.02.13a List추가 (크기)
	int iStWidth		= 120;
	int iEdWidth		= iCtrlWidth - iStWidth - iSpacing;
	int iLeft_Sub		= iLeft + iStWidth + iSpacing;
	//int iListWidth		= 800;
	int iListWidth = 700;//2023.02.13a
	int iListHeight		= iHeight - ((iCtrlHeight + iSpacing) * 2);

	iLeft = iMargin + iCtrlWidth + iSpacing + iListWidth - 250;
	m_bn_Apply.MoveWindow(iLeft, iTop, 250, iCtrlHeight);

	// Server Type 2023.02.13 uhkim [OnSize]
	//iLeft = iMargin;
	//iTop += iCtrlHeight + iSpacing;
	//m_st_EqpItemData[EID_Svr_Type].MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
	//m_cb_ServerType.MoveWindow(iLeft_Sub, iTop, iEdWidth, iCtrlHeight);

	// Alias
	iLeft = iMargin;
	iTop += iCtrlHeight + iSpacing;
	m_st_EqpItemData[EID_Eqp_Alias].MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
	m_ed_EqpAlias.MoveWindow(iLeft_Sub, iTop, iEdWidth, iCtrlHeight);

	// Equipmnet Type 
	iTop += iCtrlHeight + iSpacing;
	m_st_EqpItemData[EID_Eqp_Type].MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
	m_cb_ServerType.MoveWindow(iLeft_Sub, iTop, iEdWidth, iCtrlHeight);

	// Equipment id
	iTop += iCtrlHeight + iSpacing;
	m_st_EqpItemData[EID_Eqp_Id].MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
	m_ed_EquipmentId.MoveWindow(iLeft_Sub, iTop, iEdWidth, iCtrlHeight);

	// ip address
	iTop += iCtrlHeight + iSpacing;
	m_st_EqpItemData[EID_IP_Address].MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
	m_ed_IPAddress.MoveWindow(iLeft_Sub, iTop, iEdWidth, iCtrlHeight);

	// 추가, 제거, 저장
	iTop += iCtrlHeight + 30;
	int iQuadSt_W = (iCtrlWidth - iSpacing) / 2;
	m_bn_ItemCtrl[EC_Add].MoveWindow(iLeft, iTop, iQuadSt_W, iCtrlHeight);
	iLeft += iQuadSt_W + iSpacing;
	m_bn_ItemCtrl[EC_Insert].MoveWindow(iLeft, iTop, iQuadSt_W, iCtrlHeight);

	iTop += iCtrlHeight + iSpacing;
	iLeft = iMargin;
	m_bn_ItemCtrl[EC_Remove].MoveWindow(iLeft, iTop, iQuadSt_W, iCtrlHeight);
	iLeft += iQuadSt_W + iSpacing;
	m_bn_ItemCtrl[EC_Modify].MoveWindow(iLeft, iTop, iQuadSt_W, iCtrlHeight);

	iTop += iCtrlHeight + iCtrlHeight + iSpacing;
	m_bn_ItemCtrl[EC_Order_Up].MoveWindow(iLeft, iTop, iQuadSt_W, 50);

	iTop += 50 + iSpacing;
	m_bn_ItemCtrl[EC_Order_Down].MoveWindow(iLeft, iTop, iQuadSt_W, 50);


	// Verify Line Setting
	iLeft = iMargin;
	iTop = iMargin + iListHeight;
	m_bn_VerifyLineConfig.MoveWindow(iLeft, iTop, iCtrlWidth, iCtrlHeight);


	// 리스트 컨트롤
	iTop = iMargin + iCtrlHeight + iSpacing;
	iLeft = iMargin + iCtrlWidth + iCateSpacing;;//2023.02.13a List추가 (크기)
	iWidth = cx - iLeft - iMargin;
	//2023.02.13a List추가 (크기)
	m_lc_ServerInfo.MoveWindow(iLeft, iTop, iListWidth, iListHeight);
	//iTop = iMargin + iCtrlHeight + iSpacing;
	//iLeft = iMargin + iListWidth + iCateSpacing + iLeft;
	//iWidth = cx - iLeft - iMargin;
	//m_lc_LineInfo[ICS_SERVER_EES].MoveWindow(iLeft, iTop, iListWidth, iListHeight);
	//m_lc_LineInfo.MoveWindow(iMargin, iMargin, 600, iHeight);


	iTop += iListHeight + iSpacing;
	iCtrlWidth	= __min(160, ((iListWidth - (iSpacing * 4)) / 5));
	for (auto nIdx = 0; nIdx < XML_MaxCount; nIdx++)
	{
		m_bn_XmlCtrl[nIdx].MoveWindow(iLeft, iTop, iCtrlWidth, iCtrlHeight);
		iLeft += iCtrlWidth + iSpacing;
	}
}

//=============================================================================
// Method		: MoveWindow_EqpCtrl
// Access		: protected  
// Returns		: void
// Parameter	: int x
// Parameter	: int y
// Parameter	: int nWidth
// Parameter	: int nHeight
// Qualifier	:
// Last Update	: 2021/1/4 - 12:02
// Desc.		:
//=============================================================================
void CWnd_Cfg_Server::MoveWindow_EqpCtrl(int x, int y, int nWidth, int nHeight)
{
	int iSpacing	= 10;
	int iLeft		= x;
	int iTop		= y;
	int iCtrlWidth	= __min(160, ((nWidth - (iSpacing * 4)) / 5));
	int iCtrlHeight	= 30;

	// XML 불러오기/저장하기
	for (auto nIdx = 0; nIdx < XML_MaxCount; nIdx++)
	{
		m_bn_XmlCtrl[nIdx].MoveWindow(iLeft, iTop, iCtrlWidth, iCtrlHeight);
		iLeft += iCtrlWidth + iSpacing;
	}
}

//=============================================================================
// Method		: MoveWindow_EqpItem
// Access		: protected  
// Returns		: void
// Parameter	: int x
// Parameter	: int y
// Parameter	: int nWidth
// Parameter	: int nHeight
// Qualifier	:
// Last Update	: 2021/1/4 - 12:02
// Desc.		:
//=============================================================================
void CWnd_Cfg_Server::MoveWindow_EqpItem(int x, int y, int nWidth, int nHeight)
{
	int iSpacing	= 5;
	int iMagin		= 10;
	int iLeft		= x;
	int iTop		= y;
	int	iStWidth	= __min(200, ((nWidth - 10) / 4));
	int iHalfWidth	= __min(415, (nWidth - iStWidth - iSpacing));
	int iQuadWidth	= (iHalfWidth - (iSpacing * 2)) / 3;
	int iCtrlWidth	= iHalfWidth - iQuadWidth - iSpacing;
	int iDescWidth	= __max(100, __min(160, (iHalfWidth - iSpacing) / 2 ));
	int iEdWidth	= iHalfWidth - iDescWidth - iSpacing;
	int iCtrlHeight	= (295 < nHeight) ? 30 : static_cast<int>(round((nHeight - 45) / 10));
	int iLeftSub	= 0;


	// 설비 추가/삭제/이동 -----------------------------------------------------
	iCtrlWidth	= __min(160, ((nWidth - (iSpacing * 3)) / 4));

	iLeft = x;
	iTop += iCtrlHeight + iSpacing;
	
	m_bn_ItemCtrl[EC_Order_Up].MoveWindow(iLeft, iTop, iCtrlWidth, iCtrlHeight);

	iTop += iCtrlHeight + iSpacing;
	m_bn_ItemCtrl[EC_Order_Down].MoveWindow(iLeft, iTop, iCtrlWidth, iCtrlHeight);

	iTop += iCtrlHeight + iSpacing;

	iLeft = x;
	iTop += iCtrlHeight + iSpacing;
	m_bn_ItemCtrl[EC_Modify].MoveWindow(iLeft, iTop, iCtrlWidth, iCtrlHeight);

	iTop += iCtrlHeight + iSpacing;
	m_bn_ItemCtrl[EC_Add].MoveWindow(iLeft, iTop, iCtrlWidth, iCtrlHeight);

	iLeft += iCtrlWidth + iSpacing;
	m_bn_ItemCtrl[EC_Insert].MoveWindow(iLeft, iTop, iCtrlWidth, iCtrlHeight);

	iLeft += iCtrlWidth + iSpacing;
	m_bn_ItemCtrl[EC_Remove].MoveWindow(iLeft, iTop, iCtrlWidth, iCtrlHeight);

	// 설비 데이터 항목 조건
	iTop += iCtrlHeight + iSpacing;

	iCtrlWidth	= iHalfWidth - iQuadWidth - iSpacing;	// 2

	iLeft = x;
	iLeftSub = iLeft + iStWidth + iSpacing;

	// Server Type 2023.02.13a uhkim [MoveWindow_EqpItem]
	//iTop += iCtrlHeight + iSpacing;
	//m_st_EqpItemData[EID_Svr_Type].MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
	//m_cb_ServerType.MoveWindow(iLeftSub, iTop, iHalfWidth, iCtrlHeight);

	iTop += iCtrlHeight + iSpacing;
	m_st_EqpItemData[EID_Eqp_Alias].MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
	m_ed_EqpAlias.MoveWindow(iLeftSub, iTop, iHalfWidth, iCtrlHeight);	

	iTop += iCtrlHeight + iSpacing;
	m_st_EqpItemData[EID_Eqp_Id].MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
	m_ed_EquipmentId.MoveWindow(iLeftSub, iTop, iHalfWidth, iCtrlHeight);	

	iTop += iCtrlHeight + iSpacing;
	m_st_EqpItemData[EID_IP_Address].MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
	m_ed_IPAddress.MoveWindow(iLeftSub, iTop, iHalfWidth, iCtrlHeight);	
	
}

//=============================================================================
// Method		: OnBnClickedBnStepCtrl
// Access		: protected  
// Returns		: void
// Parameter	: UINT nID
// Qualifier	:
// Last Update	: 2017/9/25 - 9:51
// Desc.		:
//=============================================================================
void CWnd_Cfg_Server::OnBnClickedBnEqpCtrl(UINT nID)
{
	enEqpCtrl nIDIdx = (enEqpCtrl)(nID - IDC_BN_STEPCTRL_S);
	switch (nIDIdx)
	{
	case CWnd_Cfg_Server::EC_Add:
		Item_Add();
		break;

	case CWnd_Cfg_Server::EC_Insert:
		Item_Insert();
		break;

	case CWnd_Cfg_Server::EC_Remove:
		Item_Remove();
		break;

	case CWnd_Cfg_Server::EC_Modify:
		Item_Modify();
		break;

	case CWnd_Cfg_Server::EC_Order_Up:
		Item_Up();
		break;

	case CWnd_Cfg_Server::EC_Order_Down:
		Item_Down();
		break;

	default:
		break;
	}
}

//=============================================================================
// Method		: OnCbnSelendokTestItem
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2017/12/4 - 11:57
// Desc.		:
//=============================================================================
void CWnd_Cfg_Server::OnCbnSelendokEqpType()
{
	int iSel = m_cb_ServerType.GetCurSel();

	if (iSel < 0)
		return;

	UpdateUI_EquipmentData(iSel);
}
//2023.02.13a 서버 선택
void CWnd_Cfg_Server::OnCbnSelendokSvrType()
{
	m_cb_ServerType.GetCurSel();
}
//=============================================================================
// Method		: OnBnClickedBnListCtrl
// Access		: protected  
// Returns		: void
// Parameter	: UINT nID
// Qualifier	:
// Last Update	: 2021/1/22 - 11:55
// Desc.		:
//=============================================================================
void CWnd_Cfg_Server::OnBnClickedBnListCtrl(UINT nID)
{
	enXmlCtrl nIDIdx = (enXmlCtrl)(nID - IDC_BN_LISTCTRL_S);

	switch (nIDIdx)
	{
	case CWnd_Cfg_Server::XML_Load:
		LoadXML_ServerInfo();
		break;

	case CWnd_Cfg_Server::XML_Save:
		SaveXML_ServerInfo();
		break;

	case CWnd_Cfg_Server::XML_Clear:
	{
		//if (IDYES == LT_MessageBox(_T("리스트의 모든 설비를 삭제 하시겠습니까?"), MB_YESNO))
		//if (IDYES == LT_MessageBox(_T("Are you sure you want to delete all equipments in the list?"), MB_YESNO))
		if (IDYES == LT_MessageBox(g_szMessageBox_T[MB_Line_Delete_AllEqp][m_nLanguage], MB_YESNO))
		{
			m_lc_ServerInfo.Clear_LineInfo();	//2023.02.13a uhkim [선택 옵션 추가]
		}
	}
	break;

	default:
		break;
	}
}

//=============================================================================
// Method		: OnBnClickedBnClearList
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/1/22 - 11:36
// Desc.		:
//=============================================================================
void CWnd_Cfg_Server::OnBnClickedBnClearList()
{
	//if (IDYES == LT_MessageBox(_T("리스트의 모든 설비를 삭제 하시겠습니까?"), MB_YESNO))
	//if (IDYES == LT_MessageBox(_T("Are you sure you want to delete all equipments in the list?"), MB_YESNO))
	if (IDYES == LT_MessageBox(g_szMessageBox_T[MB_Line_Delete_AllEqp][m_nLanguage], MB_YESNO))
	{
		m_lc_ServerInfo.Clear_LineInfo();
	}
}

//=============================================================================
// Method		: OnBnClickedBnManualTest
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/1/7 - 17:36
// Desc.		:
//=============================================================================
void CWnd_Cfg_Server::OnBnClickedBnVerifyLine()
{
	if (false == m_lc_ServerInfo.Check_ServerID())
	{
		// There are Overlapped Equipment ID. please check.
		//LT_MessageBox(_T("중복된 설비 ID가 있습니다. 확인하세요."));
		//LT_MessageBox(_T("There are Overlapped ""Equipment ID"". please check."));
		LT_MessageBox(g_szMessageBox_T[MB_Line_EqpID_Overlapped][m_nLanguage]);
		return;
	}

	if (false == m_lc_ServerInfo.Check_IPAddress())
	{
		// There are Overlapped IP Address. please check.
		//LT_MessageBox(_T("중복된 IP Address가 있습니다. 확인하세요."));
		//LT_MessageBox(_T("There are Overlapped ""IP Address"". please check"));
		LT_MessageBox(g_szMessageBox_T[MB_Line_IpAddr_Overlapped][m_nLanguage]);
		return;
	}

	//LT_MessageBox(_T("Settings are OK."));
	LT_MessageBox(g_szMessageBox_T[MB_Line_SettingOK][m_nLanguage]);
}

//=============================================================================
// Method		: OnBnClickedBnApply
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2022/1/5 - 16:35
// Desc.		:
//=============================================================================
void CWnd_Cfg_Server::OnBnClickedBnApply()
{
	Apply_ServerInfo();
}

//=============================================================================
// Method		: OnBnClickedBnModifyEqpData
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/11/9 - 15:45
// Desc.		:
//=============================================================================
// void CWnd_Cfg_Line::OnBnClickedBnModifyEqpData()
// {
// 	Item_Modify();
// }

//=============================================================================
// Method		: Get_EquipmentData
// Access		: public  
// Returns		: BOOL
// Parameter	: __out CConfig_Eqp & OUT_stEquipment
// Qualifier	:
// Last Update	: 2021/11/9 - 14:21
// Desc.		:
//=============================================================================
BOOL CWnd_Cfg_Server::Get_EquipmentData(__out CConfig_Svr& OUT_stEquipment)
{
	CString szValue;
	DWORD dwAddress = 0;

	// ---- Equipment Type
	int iSel = m_cb_ServerType.GetCurSel();
	if (0 <= iSel)
	{
		//OUT_stEquipment.Set_EquipmentType(iSel);
		OUT_stEquipment.Set_SvrType_UI(iSel);
	}
	else
	{
		return FALSE;
	}
	
	// ---- Equipment ID
	m_ed_EquipmentId.GetWindowText(szValue);
	OUT_stEquipment.Set_ServerId(szValue);

	if (false == m_lc_ServerInfo.Check_ServerID(szValue.GetBuffer()))
	{
		//LT_MessageBox(_T("중복된 IP Address가 있습니다. 확인하세요."));
		//LT_MessageBox(_T("There are Overlapped ""IP Address"". please check"));
		LT_MessageBox(g_szMessageBox_T[MB_Line_IpAddr_Overlapped][m_nLanguage]);
	}

	// ---- IP Address
	if (0 < m_ed_IPAddress.GetWindowTextLength())
	{
		m_ed_IPAddress.GetWindowText(szValue);

		CStringA szAddr;
		USES_CONVERSION;
		szAddr = CT2A(szValue.GetBuffer());

		if (isValidateIP4Address(szAddr.GetBuffer()))
		{
			dwAddress = ConvIPStringToAddr(szAddr.GetBuffer());

			OUT_stEquipment.Set_IP_Address(dwAddress);

			if (false == m_lc_ServerInfo.Check_IPAddress(dwAddress))
			{
				//LT_MessageBox(_T("중복된 IP Address가 있습니다. 확인하세요."));
				//LT_MessageBox(_T("There are Overlapped ""IP Address"". please check"));
				LT_MessageBox(g_szMessageBox_T[MB_Line_IpAddr_Overlapped][m_nLanguage]);
				return FALSE;
			}
		}
		else
		{
			//LT_MessageBox(_T("Not a valid IP Address!"));
			LT_MessageBox(g_szMessageBox_T[MB_Line_IpAddr_Invalid][m_nLanguage]);
			return FALSE;
		}
	}
	else
	{
		OUT_stEquipment.Set_IP_Address(0);

 		//LT_MessageBox(_T("IP Address is Blank!"));
		LT_MessageBox(g_szMessageBox_T[MB_Line_IpAddr_Blank][m_nLanguage]);
 		return FALSE;
	}

	// 검사 항목에 따라서 데이터 범위 체크 해야 함.
	Verify_Equipment(OUT_stEquipment);

	return TRUE;
}

//=============================================================================
// Method		: Verify_TestStepData
// Access		: protected  
// Returns		: void
// Parameter	: __inout CConfig_Eqp & OUT_stEquipment
// Qualifier	:
// Last Update	: 2021/1/8 - 13:55
// Desc.		:
//=============================================================================
void CWnd_Cfg_Server::Verify_Equipment(__inout CConfig_Svr& OUT_stEquipment)
{
	// 검사 항목에 따라서 데이터 범위 체크 해야 함.

	// 동일한 IP 체크

	// 동일한 Equipment ID 체크

	// Loader / Unloader 체크

	// Returner 체크


	/*OUT_stEquipment.m_szAlias;
	OUT_stEquipment.m_nEquipmentType;
	OUT_stEquipment.m_szEquipmentId;
	OUT_stEquipment.m_nIP_Address;*/

	/*if (m_bUseExtraData)
	{
		OUT_stEquipment.m_nPortCount;
		OUT_stEquipment.m_nConveyorCount;
	}*/

}

//=============================================================================
// Method		: Item_Add
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2017/9/25 - 10:25
// Desc.		:
//=============================================================================
void CWnd_Cfg_Server::Item_Add()
{
	CConfig_Svr stEquipment;
	if (Get_EquipmentData(stEquipment))
	{
		m_lc_ServerInfo.Item_Add(stEquipment);
		m_lc_ServerInfo.Get_Selected(stEquipment);
		m_ed_EqpAlias.SetWindowText(stEquipment.Get_Alias());
	}
}

//=============================================================================
// Method		: Item_Insert
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2017/9/25 - 19:17
// Desc.		:
//=============================================================================
void CWnd_Cfg_Server::Item_Insert()
{
	CConfig_Svr stEquipment;
	if (Get_EquipmentData(stEquipment))
	{
		m_lc_ServerInfo.Item_Insert(stEquipment);
		m_lc_ServerInfo.Get_Selected(stEquipment);
		m_ed_EqpAlias.SetWindowText(stEquipment.Get_Alias());
	}
}

//=============================================================================
// Method		: Item_Remove
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2017/9/25 - 19:17
// Desc.		:
//=============================================================================
void CWnd_Cfg_Server::Item_Remove()
{
	auto iSel = m_cb_ServerType.GetCurSel();
	m_lc_ServerInfo.Item_Remove();

}

//=============================================================================
// Method		: Item_Modify
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/11/10 - 13:55
// Desc.		:
//=============================================================================
void CWnd_Cfg_Server::Item_Modify()
{
	auto iSel = m_cb_ServerType.GetCurSel();
	if (m_lc_ServerInfo.GetSelectedCount() <= 0)
	{
		//LT_MessageBox(_T("항목을 선택하세요."));
		//LT_MessageBox(_T("Please select an item."));
		LT_MessageBox(g_szMessageBox_T[MB_Common_SelectItem][m_nLanguage]);
		return;
	}

	CConfig_Svr Equipment;	
	if (m_lc_ServerInfo.Get_Selected(Equipment))
	{
		CDlg_ModifyServer	m_dlg_ModifyEqp;
		m_dlg_ModifyEqp.OnLanguage(m_nLanguage);
		//m_dlg_ModifyEqp.Set_EquipmenTypetList(&m_EquipmentTypeList);
		m_dlg_ModifyEqp.Set_EquipmentData(&Equipment);
		if (IDOK == m_dlg_ModifyEqp.DoModal())
		{
			CConfig_Svr stEuipment_New;
			stEuipment_New = m_dlg_ModifyEqp.Get_EquipmentData();
			m_lc_ServerInfo.Item_Modify(stEuipment_New);
		}
	}
}

//=============================================================================
// Method		: Item_Up
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2017/9/25 - 19:17
// Desc.		:
//=============================================================================
void CWnd_Cfg_Server::Item_Up()
{
	m_lc_ServerInfo.Item_Up();
}

//=============================================================================
// Method		: Item_Down
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2017/9/25 - 19:17
// Desc.		:
//=============================================================================
void CWnd_Cfg_Server::Item_Down()
{
	m_lc_ServerInfo.Item_Down();
}

//=============================================================================
// Method		: LoadXML
// Access		: protected  
// Returns		: BOOL
// Qualifier	:
// Last Update	: 2018/3/29 - 15:28
// Desc.		:
//=============================================================================
BOOL CWnd_Cfg_Server::LoadXML_ServerInfo()
{
	// 설정된 데이터가 삭제 됩니다.
	//if (IDNO == LT_MessageBox(_T("The data being edited is deleted.\r\n Do you want to continue?"), MB_YESNO))
	if (IDNO == LT_MessageBox(g_szMessageBox_T[MB_Line_Delete_EditData][m_nLanguage], MB_YESNO))
	{
		return TRUE;
	}

	CString strFullPath;
	CString strFileExt;
	strFileExt = _T("XML File (*.xml)| *.xml||");
	CString strFileSel;
	strFileSel = _T("*.xml");

	// 파일 불러오기
	CFileDialog fileDlg(TRUE, _T("xml"), strFileSel, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, strFileExt);
	if (FALSE == m_szPath_XML.IsEmpty())
	{
		fileDlg.m_ofn.lpstrInitialDir = m_szPath_XML;
	}

	if (fileDlg.DoModal() == IDOK)
	{
		strFullPath = fileDlg.GetPathName();

		CXmlLineServerConfig	StepSeq;
		CConfig_Server	stConfigLine_Info;

		if (StepSeq.LoadXML_ServerInfo(strFullPath.GetBuffer(0), stConfigLine_Info))
		{
			// UI에 세팅
			Set_Configuration(&stConfigLine_Info);

			// 설비 구성이 정상인지 확인
			if (false == Verify_EquipmentSequence(&stConfigLine_Info))
			{
				//LT_MessageBox(_T("설비 구성에 문제가 있습니다. 수정하세요."));
				//LT_MessageBox(_T("There is a problem with the equipment configuration. Please fix."));
				LT_MessageBox(g_szMessageBox_T[MB_Line_EqpCfg_Error_Fix][m_nLanguage]);
			}
		}
		else
		{
			return FALSE;
		}
	}

	return TRUE;
}

//=============================================================================
// Method		: LoadXML_StepList
// Access		: protected  
// Returns		: BOOL
// Parameter	: __out CConfig_Line & OUT_stLineInfo
// Qualifier	:
// Last Update	: 2021/1/6 - 16:12
// Desc.		:
//=============================================================================
BOOL CWnd_Cfg_Server::LoadXML_ServerInfo(__out CConfig_Server& OUT_stLineInfo, __out CString& OUT_FilePath, __out CString& OUT_FileTitle)
{
	CString strFullPath;
	CString strFileExt;
	strFileExt = _T("XML File (*.xml)| *.xml||");
	CString strFileSel;
	strFileSel = _T("*.xml");

	OUT_FilePath.Empty();
	OUT_FileTitle.Empty();

	// 파일 불러오기
	CFileDialog fileDlg(TRUE, _T("xml"), strFileSel, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, strFileExt);
	if (FALSE == m_szPath_XML.IsEmpty())
	{
		fileDlg.m_ofn.lpstrInitialDir = m_szPath_XML;
	}

	if (fileDlg.DoModal() == IDOK)
	{
		strFullPath = fileDlg.GetPathName();

		CXmlLineServerConfig	StepSeq;
		if (StepSeq.LoadXML_ServerInfo(strFullPath.GetBuffer(0), OUT_stLineInfo))
		{
			OUT_FilePath = strFullPath;
			OUT_FileTitle = fileDlg.GetFileTitle();

			// 설비 구성이 정상인지 확인 ( loop begin -> loop end 확인)
			if (false == Verify_EquipmentSequence(&OUT_stLineInfo))
			{
				//LT_MessageBox(_T("There is a problem with the equipment configuration. Please fix."));
				LT_MessageBox(g_szMessageBox_T[MB_Line_EqpCfg_Error_Fix][m_nLanguage]);
			}

			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}

	return TRUE;
}

//=============================================================================
// Method		: LoadXML_StepList
// Access		: protected  
// Returns		: BOOL
// Parameter	: __in LPCTSTR IN_szFilePath
// Parameter	: __out CConfig_Line & OUT_stLineInfo
// Qualifier	:
// Last Update	: 2021/1/6 - 19:09
// Desc.		:
//=============================================================================
BOOL CWnd_Cfg_Server::LoadXML_ServerInfo(__in LPCTSTR IN_szFilePath, __out CConfig_Server& OUT_stLineInfo)
{
	CXmlLineServerConfig	StepSeq;
	if (StepSeq.LoadXML_ServerInfo(IN_szFilePath, OUT_stLineInfo))
	{
		// 설비 구성이 정상인지 확인 ( loop begin -> loop end 확인)
		if (false == Verify_EquipmentSequence(&OUT_stLineInfo))
		{
			//LT_MessageBox(_T("There is a problem with the equipment configuration. Please fix."));
			LT_MessageBox(g_szMessageBox_T[MB_Line_EqpCfg_Error_Fix][m_nLanguage]);
		}

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//=============================================================================
// Method		: SaveXML
// Access		: protected  
// Returns		: BOOL
// Qualifier	:
// Last Update	: 2018/3/29 - 15:28
// Desc.		:
//=============================================================================
BOOL CWnd_Cfg_Server::SaveXML_ServerInfo()
{
	// UI상의 데이터 얻기
	CConfig_Server stConfigLine_Info;
	Get_Configuration(stConfigLine_Info);

	// 설비 구성이 정상인지 확인 ( loop begin -> loop end 확인)
	if (false == Verify_EquipmentSequence(&stConfigLine_Info))
	{
		//LT_MessageBox(_T("Loop Begin / End 구성을 확인하세요. 저장을 취소합니다."));
		//LT_MessageBox(_T("There is a problem with the equipment configuration. Cancel save."));
		LT_MessageBox(g_szMessageBox_T[MB_Line_EqpCfg_Error_Cancel][m_nLanguage]);
		return FALSE;
	}

	CString strFullPath;
	//CString strFileTitle;
	CString strFileExt;
	strFileExt = _T("XML File (*.xml)| *.xml||");

	CFileDialog fileDlg(FALSE, _T("xml"), NULL, OFN_OVERWRITEPROMPT, strFileExt);
	if (FALSE == m_szPath_XML.IsEmpty())
	{
		fileDlg.m_ofn.lpstrInitialDir = m_szPath_XML;
	}

	if (fileDlg.DoModal() == IDOK)
	{
		strFullPath = fileDlg.GetPathName();

		// 저장	
		CXmlLineServerConfig	StepSeq;
		if (FALSE == StepSeq.SaveXML_ServerInfo(strFullPath, &stConfigLine_Info))
		{
			//LT_MessageBox(_T("Save Failed!!"), MB_SYSTEMMODAL);			
			LT_MessageBox(g_szMessageBox_T[MB_Common_SaveFile_Failed][m_nLanguage], MB_SYSTEMMODAL);
			return FALSE;
		}
	}

	return TRUE;
}

//=============================================================================
// Method		: Apply_LineInfo
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2022/1/5 - 16:49
// Desc.		:
//=============================================================================
void CWnd_Cfg_Server::Apply_ServerInfo()
{
	// UI상의 데이터 얻기
	CConfig_Server stLineInfo;
	Get_Configuration(stLineInfo);

	// 설비 구성이 정상인지 확인 ( loop begin -> loop end 확인)
	if (false == Verify_EquipmentSequence(&stLineInfo))
	{
		//LT_MessageBox(_T("Loop Begin / End 구성을 확인하세요. 저장을 취소합니다."));
		//LT_MessageBox(_T("There is a problem with the equipment configuration. Cancel save."));
		LT_MessageBox(g_szMessageBox_T[MB_Line_EqpCfg_Error_Cancel][m_nLanguage]);
		return;
	}

	// 저장	
	CXmlLineServerConfig	XmlConfig;
	if (FALSE == XmlConfig.SaveXML_ServerInfo(m_szPath_Default, &stLineInfo))
	{
		//LT_MessageBox(_T("Save Failed!!"), MB_SYSTEMMODAL);
		LT_MessageBox(g_szMessageBox_T[MB_Common_SaveFile_Failed][m_nLanguage], MB_SYSTEMMODAL);
		return;
	}

	// Recipe 데이터 불러오기
#if defined(EES_XML)//20231003
	GetOwner()->SendNotifyMessage(WM_CHANGED_SETTING_INFO, (WPARAM)m_szPath_Default.GetBuffer(), WM_Setting_Sever);
#endif
	//LT_MessageBox(_T("The file has been saved successfully."));
	LT_MessageBox(g_szMessageBox_T[MB_Common_SaveFile_Succeed][m_nLanguage]);
}

//=============================================================================
// Method		: Update_StepData
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/1/4 - 16:40
// Desc.		:
//=============================================================================
void CWnd_Cfg_Server::UpdateUI_EquipmentData(__in int IN_nIndex)
{
	m_ed_EqpAlias.SetWindowText(_T(""));


// 	if (m_EquipmentTypeList.size() <= IN_nIndex)
// 		return;
// 
// 	CString EqpItem = m_EquipmentTypeList.at(IN_nIndex);

}

//=============================================================================
// Method		: UpdateUI_EquipmentType
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/11/5 - 13:43
// Desc.		:
//=============================================================================
// void CWnd_Cfg_Line::UpdateUI_EquipmentType()
// {
// 	if (NULL == GetSafeHwnd())
// 		return;
// 
// 	m_cb_EquipmentType.ResetContent();
// 
// 	for (auto itr = m_EquipmentTypeList.begin(); itr != m_EquipmentTypeList.end(); ++itr)
// 	{
// 		m_cb_EquipmentType.AddString(*itr);
// 	}
// 
// 	m_cb_EquipmentType.SetCurSel(0);
// }

//=============================================================================
// Method		: Verify_StepSequence
// Access		: protected  
// Returns		: bool
// Parameter	: __in const CConfig_Line * IN_pLineInfo
// Qualifier	:
// Last Update	: 2021/1/19 - 18:06
// Desc.		:
//=============================================================================
bool CWnd_Cfg_Server::Verify_EquipmentSequence(__in const CConfig_Server* IN_pLineInfo)
{
	if (nullptr == IN_pLineInfo)
		return false;

	// 설비 구성이 정상인지 확인 
	// SFR 협각 / 광각 및 0도 / 90도 검사를 동시에 사용하지 않음
	int iCheckPair = 0;
	bool bPass = true;

	for (auto nIdx = 0; nIdx < IN_pLineInfo->GetCount(); nIdx++)
	{
		CConfig_Svr StepUnit = IN_pLineInfo->GetAt(nIdx);	
	}
	if ((0 == iCheckPair) && (true == bPass))
	{
		return true;
	}
	else
	{
		return false;
	}
}
//=============================================================================
// Method		: OnLanguage
// Access		: virtual public  
// Returns		: void
// Parameter	: __in uint8_t IN_nLanguage
// Qualifier	:
// Last Update	: 2022/7/19 - 17:02
// Desc.		:
//=============================================================================
void CWnd_Cfg_Server::OnLanguage(__in uint8_t IN_nLanguage)
{
	__super::OnLanguage(IN_nLanguage);

	m_lc_ServerInfo.OnLanguage(IN_nLanguage);

	// CharSet 변경
	LOGFONT lf_def;
	m_font_Default.GetLogFont(&lf_def);
	lf_def.lfCharSet = Get_CharSet(m_nLanguage);
	m_font_Default.DeleteObject();
	m_font_Default.CreateFontIndirect(&lf_def);

	// CharSet 변경
	LOGFONT lf;
	m_Font.GetLogFont(&lf);
	lf.lfCharSet = Get_CharSet(m_nLanguage);
	m_Font.DeleteObject();
	m_Font.CreateFontIndirect(&lf);

	m_cb_ServerType.SetFont(&m_Font);

	if (GetSafeHwnd())
	{
		m_bn_Apply.SetWindowText(g_szCtrlText_T[m_nLanguage][Txt_Apply]);
		m_bn_VerifyLineConfig.SetWindowText(g_szCtrlText_T[m_nLanguage][Txt_VerifyLineConfig]);

		for (UINT nIdx = 0; nIdx < EC_MaxEnum; nIdx++)
		{
			m_bn_ItemCtrl[nIdx].SetWindowText(g_szTestCtrl_T[m_nLanguage][nIdx]);
		}

		for (auto nIdx = 0; nIdx < XML_MaxCount; nIdx++)
		{
			m_bn_XmlCtrl[nIdx].SetWindowText(g_szListCtrl_T[m_nLanguage][nIdx]);
		}

		for (UINT nIdx = 0; nIdx < EID_MaxEnum; nIdx++)
		{
			m_st_EqpItemData[nIdx].SetText(g_szItemText_T[m_nLanguage][nIdx]);
		}
	}
}

//=============================================================================
// Method		: Set_PermissionMode
// Access		: public  
// Returns		: void
// Parameter	: __in enPermissionMode IN_PermissionMode
// Qualifier	:
// Last Update	: 2022/1/7 - 11:12
// Desc.		:
//=============================================================================
void CWnd_Cfg_Server::Set_PermissionMode(__in enPermissionMode IN_PermissionMode)
{
	//수정 필요
	return;
	switch (IN_PermissionMode)
	{
	case enPermissionMode::Permission_Operator:
	case enPermissionMode::Permission_Engineer:
	{
		m_bn_Apply.EnableWindow(FALSE);
		m_lc_ServerInfo.EnableWindow(FALSE);
		m_bn_VerifyLineConfig.EnableWindow(FALSE);

		for (auto nIdx = 0; nIdx < EC_MaxEnum; nIdx++)
		{
			m_bn_ItemCtrl[nIdx].EnableWindow(FALSE);
		}

		m_ed_EqpAlias.EnableWindow(FALSE);
		m_cb_ServerType.EnableWindow(FALSE);
		m_ed_EquipmentId.EnableWindow(FALSE);
		m_ed_IPAddress.EnableWindow(FALSE);

// 		m_cb_TestZoneCount.EnableWindow(FALSE);
// 		m_cb_BufferCount.EnableWindow(FALSE);
// 		m_cb_ConveyorCount.EnableWindow(FALSE);
// 		m_cb_ReturnConveyorCount.EnableWindow(FALSE);

		for (auto nIdx = 0; nIdx < XML_MaxCount; nIdx++)
		{
			m_bn_XmlCtrl[nIdx].EnableWindow(FALSE);
		}
	}
		break;

	case enPermissionMode::Permission_Administrator:
	{
		m_bn_Apply.EnableWindow(TRUE);
		m_lc_ServerInfo.EnableWindow(TRUE);
		m_bn_VerifyLineConfig.EnableWindow(TRUE);

		for (auto nIdx = 0; nIdx < EC_MaxEnum; nIdx++)
		{
			m_bn_ItemCtrl[nIdx].EnableWindow(TRUE);
		}

		m_ed_EqpAlias.EnableWindow(TRUE);
		m_cb_ServerType.EnableWindow(TRUE);
		m_ed_EquipmentId.EnableWindow(TRUE);
		m_ed_IPAddress.EnableWindow(TRUE);

// 		m_cb_TestZoneCount.EnableWindow(TRUE);
// 		m_cb_BufferCount.EnableWindow(TRUE);
// 		m_cb_ConveyorCount.EnableWindow(TRUE);
// 		m_cb_ReturnConveyorCount.EnableWindow(TRUE);

		for (auto nIdx = 0; nIdx < XML_MaxCount; nIdx++)
		{
			m_bn_XmlCtrl[nIdx].EnableWindow(TRUE);
		}
	}
		break;
	}
}

//=============================================================================
// Method		: Set_FileFullPath
// Access		: public  
// Returns		: void
// Parameter	: __in LPCTSTR IN_szFullPath
// Qualifier	:
// Last Update	: 2022/1/5 - 17:32
// Desc.		:
//=============================================================================
void CWnd_Cfg_Server::Set_FileFullPath(__in LPCTSTR IN_szFullPath)
{
	m_szPath_Default = IN_szFullPath;
}

//=============================================================================
// Method		: Set_XMLFilePath
// Access		: public  
// Returns		: void
// Parameter	: __in LPCTSTR szPath
// Qualifier	:
// Last Update	: 2018/3/29 - 15:57
// Desc.		:
//=============================================================================
void CWnd_Cfg_Server::Set_XMLFilePath(__in LPCTSTR szPath)
{
	m_szPath_XML = szPath;
}

//=============================================================================
// Method		: Set_UseExtraData
// Access		: public  
// Returns		: void
// Parameter	: __in bool bUse
// Qualifier	:
// Last Update	: 2021/11/10 - 12:01
// Desc.		:
//=============================================================================
void CWnd_Cfg_Server::Set_UseExtraData(__in bool bUse)
{
	m_bUseExtraData = bUse;
}

//=============================================================================
// Method		: Set_Configuration
// Access		: public  
// Returns		: void
// Parameter	: __in const CConfig_Line * IN_pLineInfo
// Qualifier	:
// Last Update	: 2021/11/11 - 16:36
// Desc.		:
//=============================================================================
void CWnd_Cfg_Server::Set_Configuration(__in const CConfig_Server* IN_pLineInfo)
{
	m_lc_ServerInfo.Set_LineInfo(IN_pLineInfo);
}

//=============================================================================
// Method		: Set_Configuration
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2022/1/5 - 18:17
// Desc.		:
//=============================================================================
void CWnd_Cfg_Server::Set_Configuration()
{
	if (FALSE == m_szPath_Default.IsEmpty())
	{
		CXmlLineServerConfig	XmlConfig;
		CConfig_Server	stConfigLine_Info;

		if (XmlConfig.LoadXML_ServerInfo(m_szPath_Default.GetBuffer(0), stConfigLine_Info))
		{
			// UI에 세팅
			Set_Configuration(&stConfigLine_Info);

			// 설비 구성이 정상인지 확인
			//if (false == Verify_EquipmentSequence(&stConfigLine_Info))
			//{
				//LT_MessageBox(_T("설비 구성에 문제가 있습니다. 수정하세요."));
				//LT_MessageBox(_T("There is a problem with the equipment configuration. Please fix."));
			//	LT_MessageBox(g_szMessageBox_T[MB_Line_EqpCfg_Error_Fix][m_nLanguage]);
			//}
		}
	}
}

//=============================================================================
// Method		: Get_Configuration
// Access		: public  
// Returns		: void
// Parameter	: __out CConfig_Line & OUT_stLineInfo
// Qualifier	:
// Last Update	: 2021/11/11 - 16:36
// Desc.		:
//=============================================================================
void CWnd_Cfg_Server::Get_Configuration(__out CConfig_Server& OUT_stLineInfo)
{
	m_lc_ServerInfo.Get_LineInfo(OUT_stLineInfo);
}

//=============================================================================
// Method		: Init_DefaultSet
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2017/9/25 - 20:56
// Desc.		:
//=============================================================================
void CWnd_Cfg_Server::Init_DefaultSet()
{
	// 리스트 초기화
	m_lc_ServerInfo.DeleteAllItems();

	m_cb_ServerType.SetCurSel(0);
	m_cb_ServerType.EnableWindow(FALSE);

}
// 
// //=============================================================================
// // Method		: Reset_EquipmentTypeList
// // Access		: public  
// // Returns		: void
// // Qualifier	:
// // Last Update	: 2021/11/9 - 15:48
// // Desc.		:
// //=============================================================================
// void CWnd_Cfg_Line::Reset_EquipmentTypeList()
// {
// 	m_EquipmentTypeList.clear();
// 
// 	if (NULL != GetSafeHwnd())
// 	{
// 		m_cb_EquipmentType.ResetContent();
// 	}
// }
// 
// //=============================================================================
// // Method		: Set_EquipmentTypeList
// // Access		: public  
// // Returns		: void
// // Parameter	: __in CString * IN_pEqpTypeList
// // Parameter	: __in UINT IN_nCount
// // Qualifier	:
// // Last Update	: 2021/11/9 - 15:49
// // Desc.		:
// //=============================================================================
// void CWnd_Cfg_Line::Set_EquipmentTypeList(__in CString* IN_pEqpTypeList, __in UINT IN_nCount)
// {
// 	m_EquipmentTypeList.clear();
// 
// 	for (UINT nIdx = 0; nIdx < IN_nCount; nIdx++)
// 	{
// 		m_EquipmentTypeList.push_back(IN_pEqpTypeList[nIdx]);
// 	}
// 
// 	UpdateUI_EquipmentType();
// }
// 
// //=============================================================================
// // Method		: Set_EquipmentTypeList
// // Access		: public  
// // Returns		: void
// // Parameter	: __in std::vector<CString> * IN_pEqpTypeVector
// // Qualifier	:
// // Last Update	: 2021/11/9 - 15:49
// // Desc.		:
// //=============================================================================
// void CWnd_Cfg_Line::Set_EquipmentTypeList(__in std::vector<CString>* IN_pEqpTypeVector)
// {
// 	m_EquipmentTypeList.clear();
// 	m_EquipmentTypeList = *IN_pEqpTypeVector;
// 
// 	//m_EquipmentTypeList.resize(IN_pEqpTypeVector->size());
// 	//std::copy(IN_pEqpTypeVector->begin(), IN_pEqpTypeVector->end, m_EquipmentTypeList.begin());
// 
// 	UpdateUI_EquipmentType();
// }
// 
// //=============================================================================
// // Method		: Add_EquipmentTypeList
// // Access		: public  
// // Returns		: void
// // Parameter	: __in CString * IN_pEqpTypeList
// // Parameter	: __in UINT IN_nCount
// // Qualifier	:
// // Last Update	: 2021/11/9 - 15:51
// // Desc.		:
// //=============================================================================
// void CWnd_Cfg_Line::Add_EquipmentTypeList(__in CString* IN_pEqpTypeList, __in UINT IN_nCount)
// {
// 	for (UINT nIdx = 0; nIdx < IN_nCount; nIdx++)
// 	{
// 		m_EquipmentTypeList.push_back(IN_pEqpTypeList[nIdx]);
// 	}
// 
// 	UpdateUI_EquipmentType();
// }
// 
// //=============================================================================
// // Method		: Add_EquipmentTypeList
// // Access		: public  
// // Returns		: void
// // Parameter	: __in std::vector<CString> * IN_pEqpTypeVector
// // Qualifier	:
// // Last Update	: 2021/11/9 - 15:51
// // Desc.		:
// //=============================================================================
// void CWnd_Cfg_Line::Add_EquipmentTypeList(__in std::vector<CString>* IN_pEqpTypeVector)
// {
// 	//m_EquipmentTypeList.emplace_back(*IN_pEqpTypeVector);
// 	for (auto itr = IN_pEqpTypeVector->begin(); itr != IN_pEqpTypeVector->end(); ++itr)
// 	{
// 		m_EquipmentTypeList.push_back(*itr);
// 	}
// 
// 	UpdateUI_EquipmentType();
// }
