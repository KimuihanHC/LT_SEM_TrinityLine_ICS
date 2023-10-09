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
#include "Wnd_Cfg_ALID_List.h"
#include "resource.h"
#include "Def_WindowMessage.h"
#include "XmlALConfig.h"
#include "CommonFunction.h"
#include "Def_Language_Message.h"
#include "Dlg_ModifyAL.h"	//Modify popup window

static LPCTSTR g_szTestCtrl_T[Lang_MaxCount][CWnd_Cfg_ALID_List::ID_MaxEnum] =
{
	// 한국어
	{
		_T("추가"),				// EC_Add
		_T("삽입"),				// EC_Insert
		_T("제거"),				// EC_Remove
		_T("설비 수정"),		// EC_Modify
		_T("위로 이동"),		// EC_Order_Up
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

static LPCTSTR g_szItemText_T[Lang_MaxCount][CWnd_Cfg_ALID_List::EID_MaxEnum] =
{
	// 한국어
	{
		_T("ID"),	// EID_Item_Para_2
		_T("CD"),	// EID_Item_Para_3
		_T("TX"),	// EID_Item_Para_4
	},

	// 영어
	{
		_T("ID"),	// EID_Item_Para_2
		_T("CD"),	// EID_Item_Para_3
		_T("TX"),	// EID_Item_Para_4
	},

	// 베트남어
	{
		_T("ID"),	// EID_Item_Para_2
		_T("CD"),	// EID_Item_Para_3
		_T("TX"),	// EID_Item_Para_4
	},

	// 중국어
	{
		_T("ID"),	// EID_Item_Para_2
		_T("CD"),	// EID_Item_Para_3
		_T("TX"),	// EID_Item_Para_4
	},
};


static LPCTSTR g_szListCtrl_T[Lang_MaxCount][CWnd_Cfg_ALID_List::XML_MaxCount] =
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
#define IDC_BN_STEPCTRL_E		IDC_BN_STEPCTRL_S + ID_MaxEnum - 1

#define IDC_LC_AL_STEPLIST		1200
#define IDC_CB_EQP_TYPE			1201
#define IDC_BN_VERIFY_LINE		1202
#define IDC_BN_MODIFY_STEP		1203
#define IDC_CB_SVR_TYPE			1204
#define IDC_ED_DATA_S			1210
#define IDC_ED_DATA_E			IDC_ED_DATA_S + MAX_STEP_DATA - 1


#define	IDC_BN_LISTCTRL_S		2000
#define	IDC_BN_LISTCTRL_E		IDC_BN_LISTCTRL_S + XML_MaxCount - 1

#define IDC_ED_ALID				3000
#define IDC_CB_ALCD				3001
#define IDC_ED_ALTX				3002

IMPLEMENT_DYNAMIC(CWnd_Cfg_ALID_List, CWnd_BaseView)

CWnd_Cfg_ALID_List::CWnd_Cfg_ALID_List()
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

CWnd_Cfg_ALID_List::~CWnd_Cfg_ALID_List()
{
	m_font_Default.DeleteObject();
	m_Font.DeleteObject();
}


BEGIN_MESSAGE_MAP(CWnd_Cfg_ALID_List, CWnd_BaseView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND_RANGE(IDC_BN_STEPCTRL_S,		IDC_BN_STEPCTRL_E,		OnBnClickedBnALCtrl)
	ON_COMMAND_RANGE(IDC_BN_LISTCTRL_S,		IDC_BN_LISTCTRL_E,		OnBnClickedBnALListCtrl)
	ON_CBN_SELENDOK	(IDC_CB_ALCD,			OnCbnSelendokALCD)
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
int CWnd_Cfg_ALID_List::OnCreate(LPCREATESTRUCT lpCreateStruct)
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
	m_lc_ALInfo.Create(WS_CHILD | WS_VISIBLE, rectDummy, this, IDC_LC_AL_STEPLIST);

	m_bn_VerifyLineConfig.Create(g_szCtrlText_T[m_nLanguage][Txt_VerifyLineConfig], dwStyle | BS_PUSHLIKE, rectDummy, this, IDC_BN_VERIFY_LINE);

	// 설비 제어
	m_bn_ItemCtrl[ID_Order_Up].SetImage(IDB_ARROW_UP, IDB_ARROW_UP);
	m_bn_ItemCtrl[ID_Order_Down].SetImage(IDB_ARROW_DOWN, IDB_ARROW_DOWN);

	for (UINT nIdx = 0; nIdx < ID_MaxEnum; nIdx++)
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
	//
	//---------------------------------------------------------------
	// 설비 데이터 항목
	//---------------------------------------------------------------
	//Type설정.
	m_ed_nALID.Create(dwStyle | ES_CENTER | WS_BORDER, rectDummy, this, IDC_ED_ALID);

	m_cb_nALCD.Create(dwStyle | CBS_DROPDOWNLIST | WS_VSCROLL, rectDummy, this, IDC_CB_ALCD);
	m_cb_nALCD.SetFont(&m_Font);

	for (auto nIdx = 0; nIdx < enALCD::ALCD_Max; nIdx++)
	{
		m_cb_nALCD.AddString(g_szALCD[nIdx]);
	}
	m_cb_nALCD.SetCurSel(0);

	m_ed_cstALTX.Create(dwStyle | ES_CENTER | WS_BORDER, rectDummy, this, IDC_ED_ALTX);

	// 설비 항목
	for (UINT nIdx = 0; nIdx < EID_MaxEnum; nIdx++)
	{
		m_st_SVItemData[nIdx].SetStaticStyle(CVGStatic::StaticStyle_Default);
		m_st_SVItemData[nIdx].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_st_SVItemData[nIdx].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_st_SVItemData[nIdx].SetFont_Gdip(L"Tahoma", 10.0F, Gdiplus::FontStyleRegular);

		m_st_SVItemData[nIdx].Create(g_szItemText_T[m_nLanguage][nIdx], dwStyle | SS_CENTER | SS_CENTERIMAGE, rectDummy, this, IDC_STATIC);
	}


	// 포커스 설정
	m_bn_ItemCtrl[ID_Add].SetFocus();

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
void CWnd_Cfg_ALID_List::OnSize(UINT nType, int cx, int cy)
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
	int iCtrlHeight		= 30;
	int iCtrlWidth		= 500;

	int iStWidth		= 100;
	int iEdWidth		= iCtrlWidth - iStWidth - iSpacing;
	int iLeft_Sub		= iLeft + iStWidth + iSpacing;
	int iListWidth		= 800;
	int iListHeight		= iHeight - ((iCtrlHeight + iSpacing) * 2);

	//---------------------------------------------------------------
	// 설비 리스트
	//---------------------------------------------------------------

	iLeft = iMargin + iCtrlWidth + iSpacing + iListWidth - 250;
	m_bn_Apply.MoveWindow(iLeft, iTop, 250, iCtrlHeight);

	iLeft = iMargin;
	iTop += iCtrlHeight + iSpacing;
	// ID Type
	m_st_SVItemData[EID_ALID].MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
	m_ed_nALID.MoveWindow(iLeft_Sub, iTop, iEdWidth, iCtrlHeight);

	// ID
	iLeft = iMargin;
	iTop += iCtrlHeight + iSpacing;
	m_st_SVItemData[EID_ALCD].MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
	m_cb_nALCD.MoveWindow(iLeft_Sub, iTop, iEdWidth, iCtrlHeight);

	// ID Name
	iTop += iCtrlHeight + iSpacing;
	m_st_SVItemData[EID_ALTX].MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
	m_ed_cstALTX.MoveWindow(iLeft_Sub, iTop, iEdWidth, iCtrlHeight);





	// 추가, 제거, 저장
	iTop += iCtrlHeight + 30;
	int iQuadSt_W = (iCtrlWidth - iSpacing) / 2;
	m_bn_ItemCtrl[ID_Add].MoveWindow(iLeft, iTop, iQuadSt_W, iCtrlHeight);
	iLeft += iQuadSt_W + iSpacing;
	m_bn_ItemCtrl[ID_Insert].MoveWindow(iLeft, iTop, iQuadSt_W, iCtrlHeight);

	iTop += iCtrlHeight + iSpacing;
	iLeft = iMargin;
	m_bn_ItemCtrl[ID_Remove].MoveWindow(iLeft, iTop, iQuadSt_W, iCtrlHeight);
	iLeft += iQuadSt_W + iSpacing;
	m_bn_ItemCtrl[ID_Modify].MoveWindow(iLeft, iTop, iQuadSt_W, iCtrlHeight);

	iTop += iCtrlHeight + iCtrlHeight + iSpacing;
	m_bn_ItemCtrl[ID_Order_Up].MoveWindow(iLeft, iTop, iQuadSt_W, 50);

	iTop += 50 + iSpacing;
	m_bn_ItemCtrl[ID_Order_Down].MoveWindow(iLeft, iTop, iQuadSt_W, 50);


	// Verify Line Setting
	iLeft = iMargin;
	iTop = iMargin + iListHeight;
	m_bn_VerifyLineConfig.MoveWindow(iLeft, iTop, iCtrlWidth, iCtrlHeight);


	// 리스트 컨트롤
	iTop = iMargin + iCtrlHeight + iSpacing;
	iLeft = iMargin + iCtrlWidth + iCateSpacing;;//2023.02.13a List추가 (크기)
	iWidth = cx - iLeft - iMargin;
	m_lc_ALInfo.MoveWindow(iLeft, iTop, iListWidth, iListHeight);

	
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
void CWnd_Cfg_ALID_List::MoveWindow_EqpCtrl(int x, int y, int nWidth, int nHeight)
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
void CWnd_Cfg_ALID_List::MoveWindow_ALItem(int x, int y, int nWidth, int nHeight)
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
	
	m_bn_ItemCtrl[ID_Order_Up].MoveWindow(iLeft, iTop, iCtrlWidth, iCtrlHeight);

	iTop += iCtrlHeight + iSpacing;
	m_bn_ItemCtrl[ID_Order_Down].MoveWindow(iLeft, iTop, iCtrlWidth, iCtrlHeight);

	iTop += iCtrlHeight + iSpacing;

	iLeft = x;
	iTop += iCtrlHeight + iSpacing;
	m_bn_ItemCtrl[ID_Modify].MoveWindow(iLeft, iTop, iCtrlWidth, iCtrlHeight);

	iTop += iCtrlHeight + iSpacing;
	m_bn_ItemCtrl[ID_Add].MoveWindow(iLeft, iTop, iCtrlWidth, iCtrlHeight);

	iLeft += iCtrlWidth + iSpacing;
	m_bn_ItemCtrl[ID_Insert].MoveWindow(iLeft, iTop, iCtrlWidth, iCtrlHeight);

	iLeft += iCtrlWidth + iSpacing;
	m_bn_ItemCtrl[ID_Remove].MoveWindow(iLeft, iTop, iCtrlWidth, iCtrlHeight);

	// 설비 데이터 항목 조건
	iTop += iCtrlHeight + iSpacing;

	iCtrlWidth	= iHalfWidth - iQuadWidth - iSpacing;	// 2

	iLeft = x;
	iLeftSub = iLeft + iStWidth + iSpacing;

	// Server Type 2023.02.13a uhkim [MoveWindow_EqpItem]
	iTop += iCtrlHeight + iSpacing;
	m_st_SVItemData[EID_ALID].MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
	m_ed_nALID.MoveWindow(iLeftSub, iTop, iHalfWidth, iCtrlHeight);

	iTop += iCtrlHeight + iSpacing;
	m_st_SVItemData[EID_ALCD].MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
	m_cb_nALCD.MoveWindow(iLeftSub, iTop, iHalfWidth, iCtrlHeight);

	iTop += iCtrlHeight + iSpacing;
	m_st_SVItemData[EID_ALTX].MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
	m_ed_cstALTX.MoveWindow(iLeftSub, iTop, iHalfWidth, iCtrlHeight);
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
void CWnd_Cfg_ALID_List::OnBnClickedBnALCtrl(UINT nID)
{
	enIDCtrl nIDIdx = (enIDCtrl)(nID - IDC_BN_STEPCTRL_S);
	switch (nIDIdx)
	{
	case CWnd_Cfg_ALID_List::ID_Add:
		Item_Add();
		break;

	case CWnd_Cfg_ALID_List::ID_Insert:
		Item_Insert();
		break;

	case CWnd_Cfg_ALID_List::ID_Remove:
		Item_Remove();
		break;

	case CWnd_Cfg_ALID_List::ID_Modify:
		Item_Modify();
		break;

	case CWnd_Cfg_ALID_List::ID_Order_Up:
		Item_Up();
		break;

	case CWnd_Cfg_ALID_List::ID_Order_Down:
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
void CWnd_Cfg_ALID_List::OnCbnSelendokALCD()
{
	/*
	int iSel = m_cb_EquipmentType.GetCurSel();

	if (iSel < 0)
		return;

	UpdateUI_EquipmentData(iSel);
	*/
}
//=============================================================================
// Method		: OnBnClickedBnSVListCtrl
// Access		: protected  
// Returns		: void
// Parameter	: UINT nID
// Qualifier	:
// Last Update	: 2021/1/22 - 11:55
// Desc.		:
//=============================================================================
void CWnd_Cfg_ALID_List::OnBnClickedBnALListCtrl(UINT nID)
{
	enXmlCtrl nIDIdx = (enXmlCtrl)(nID - IDC_BN_LISTCTRL_S);

	switch (nIDIdx)
	{
	case CWnd_Cfg_ALID_List::XML_Load:
		LoadXML_SVInfo();
		break;

	case CWnd_Cfg_ALID_List::XML_Save:
		SaveXML_SVInfo();
		break;

	case CWnd_Cfg_ALID_List::XML_Clear:
	{
		//if (IDYES == LT_MessageBox(_T("리스트의 모든 설비를 삭제 하시겠습니까?"), MB_YESNO))
		//if (IDYES == LT_MessageBox(_T("Are you sure you want to delete all equipments in the list?"), MB_YESNO))
		if (IDYES == LT_MessageBox(g_szMessageBox_T[MB_Line_Delete_AllEqp][m_nLanguage], MB_YESNO))
		{
			m_lc_ALInfo.Clear_ALInfo();	//2023.02.13a uhkim [선택 옵션 추가]
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
void CWnd_Cfg_ALID_List::OnBnClickedBnClearList()
{
	//if (IDYES == LT_MessageBox(_T("리스트의 모든 설비를 삭제 하시겠습니까?"), MB_YESNO))
	//if (IDYES == LT_MessageBox(_T("Are you sure you want to delete all equipments in the list?"), MB_YESNO))
	if (IDYES == LT_MessageBox(g_szMessageBox_T[MB_Line_Delete_AllEqp][m_nLanguage], MB_YESNO))
	{
		m_lc_ALInfo.Clear_ALInfo();
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
void CWnd_Cfg_ALID_List::OnBnClickedBnVerifyLine()
{
	if (false == m_lc_ALInfo.Check_ALID())
	{
		// There are Overlapped Equipment ID. please check.
		//LT_MessageBox(_T("중복된 설비 ID가 있습니다. 확인하세요."));
		//LT_MessageBox(_T("There are Overlapped ""Equipment ID"". please check."));
		LT_MessageBox(g_szMessageBox_T[MB_Line_EqpID_Overlapped][m_nLanguage]);
		return;
	}
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
void CWnd_Cfg_ALID_List::OnBnClickedBnApply()
{
	Apply_SVInfo();
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
// Parameter	: __out CConfig_SV& OUT_stSV
// Qualifier	:
// Last Update	: 2021/11/9 - 14:21
// Desc.		:
//=============================================================================
BOOL CWnd_Cfg_ALID_List::Get_ALData(__out CConfig_AL& OUT_stSV)
{
	CString szValue;
	m_ed_nALID.GetWindowText(szValue);
	auto iID = _ttoi(szValue);
	m_cb_nALCD.GetWindowText(szValue);
	auto iCD = _ttoi(szValue);
	if (0 > iID)
	{
		return FALSE;
	}
	if (0 > iCD)
	{
		return FALSE;
	}

	if (false == m_lc_ALInfo.Check_ALID(iID))
	{
		LT_MessageBox(_T("중복된 ALID가 있습니다. 확인하세요."));
		return FALSE;
	}
	// ---- SID Type
	OUT_stSV.Set_ALID(iID);
	// ---- SID
	OUT_stSV.Set_ALCD(iCD);
	// ---- SID	Name
	m_ed_cstALTX.GetWindowText(szValue);
	OUT_stSV.Set_ALTX(szValue);

	Verify_AL(OUT_stSV);

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
void CWnd_Cfg_ALID_List::Verify_AL(__inout CConfig_AL& OUT_stSV)
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
void CWnd_Cfg_ALID_List::Item_Add()
{
	CConfig_AL stAL;
	if (Get_ALData(stAL))
	{
		m_lc_ALInfo.Item_Add(stAL);
		m_lc_ALInfo.Get_SelectedAL(stAL);
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
void CWnd_Cfg_ALID_List::Item_Insert()
{
	CConfig_AL stAL;
	if (Get_ALData(stAL))
	{
		m_lc_ALInfo.Item_Insert(stAL);
		m_lc_ALInfo.Get_SelectedAL(stAL);
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
void CWnd_Cfg_ALID_List::Item_Remove()
{
	m_lc_ALInfo.Item_Remove();
}

//=============================================================================
// Method		: Item_Modify
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/11/10 - 13:55
// Desc.		:
//=============================================================================
void CWnd_Cfg_ALID_List::Item_Modify()
{
	if (m_lc_ALInfo.GetSelectedCount() <= 0)
	{
		//LT_MessageBox(_T("항목을 선택하세요."));
		//LT_MessageBox(_T("Please select an item."));
		LT_MessageBox(g_szMessageBox_T[MB_Common_SelectItem][m_nLanguage]);
		return;
	}
	CConfig_AL AL;

	if (m_lc_ALInfo.Get_SelectedAL(AL))
	{
		CDlg_ModifyAL	m_dlg_ModifySV;
		m_dlg_ModifySV.OnLanguage(m_nLanguage);
		m_dlg_ModifySV.Set_ALData(&AL);
		if (IDOK == m_dlg_ModifySV.DoModal())
		{
			CConfig_AL stSV_New;
			stSV_New = m_dlg_ModifySV.Get_SVData();
			m_lc_ALInfo.Item_Modify(stSV_New);
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
void CWnd_Cfg_ALID_List::Item_Up()
{
	m_lc_ALInfo.Item_Up();
}

//=============================================================================
// Method		: Item_Down
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2017/9/25 - 19:17
// Desc.		:
//=============================================================================
void CWnd_Cfg_ALID_List::Item_Down()
{
	m_lc_ALInfo.Item_Down();
}

//=============================================================================
// Method		: LoadXML
// Access		: protected  
// Returns		: BOOL
// Qualifier	:
// Last Update	: 2018/3/29 - 15:28
// Desc.		:
//=============================================================================
BOOL CWnd_Cfg_ALID_List::LoadXML_SVInfo()
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
		//파일 불러오기.
		CXmlALConfig	StepSeq;
		CConfig_AL_LIST	stConfigSV_Info;

		if (StepSeq.LoadXML_ALInfo(strFullPath.GetBuffer(0), stConfigSV_Info))
		{
			// UI에 세팅
			Set_Configuration(&stConfigSV_Info);

			// 설비 구성이 정상인지 확인
			if (false == Verify_SVSequence(&stConfigSV_Info))
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
BOOL CWnd_Cfg_ALID_List::LoadXML_SVInfo(__out CConfig_AL_LIST& OUT_stLineInfo, __out CString& OUT_FilePath, __out CString& OUT_FileTitle)
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

		CXmlALConfig	StepSeq;
		if (StepSeq.LoadXML_ALInfo(strFullPath.GetBuffer(0), OUT_stLineInfo))
		{
			OUT_FilePath = strFullPath;
			OUT_FileTitle = fileDlg.GetFileTitle();

			// 설비 구성이 정상인지 확인 ( loop begin -> loop end 확인)
			if (false == Verify_SVSequence(&OUT_stLineInfo))
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
BOOL CWnd_Cfg_ALID_List::LoadXML_SVInfo(__in LPCTSTR IN_szFilePath, __out CConfig_AL_LIST& OUT_stLineInfo)
{
	CXmlALConfig	StepSeq;
	if (StepSeq.LoadXML_ALInfo(IN_szFilePath, OUT_stLineInfo))
	{
		// 설비 구성이 정상인지 확인 ( loop begin -> loop end 확인)
		if (false == Verify_SVSequence(&OUT_stLineInfo))
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
BOOL CWnd_Cfg_ALID_List::SaveXML_SVInfo()
{
	// UI상의 데이터 얻기
	CConfig_AL_LIST stConfigLine_Info;
	Get_Configuration(stConfigLine_Info);

	// 설비 구성이 정상인지 확인 ( loop begin -> loop end 확인)
	if (false == Verify_SVSequence(&stConfigLine_Info))
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
		CXmlALConfig	StepSeq;
		if (FALSE == StepSeq.SaveXML_ALInfo(strFullPath, &stConfigLine_Info))
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
void CWnd_Cfg_ALID_List::Apply_SVInfo()
{
	// UI상의 데이터 얻기
	CConfig_AL_LIST stLineInfo;
	Get_Configuration(stLineInfo);

	// 설비 구성이 정상인지 확인 ( loop begin -> loop end 확인)
	if (false == Verify_SVSequence(&stLineInfo))
	{
		//LT_MessageBox(_T("Loop Begin / End 구성을 확인하세요. 저장을 취소합니다."));
		//LT_MessageBox(_T("There is a problem with the equipment configuration. Cancel save."));
		LT_MessageBox(g_szMessageBox_T[MB_Line_EqpCfg_Error_Cancel][m_nLanguage]);
		return;
	}

	// 저장	
	CXmlALConfig	XmlConfig;
	if (FALSE == XmlConfig.SaveXML_ALInfo(m_szPath_Default, &stLineInfo))
	{
		//LT_MessageBox(_T("Save Failed!!"), MB_SYSTEMMODAL);
		LT_MessageBox(g_szMessageBox_T[MB_Common_SaveFile_Failed][m_nLanguage], MB_SYSTEMMODAL);
		return;
	}

	// Recipe 데이터 불러오기
#if defined(EES_XML)//20231003
	GetOwner()->SendNotifyMessage(WM_CHANGED_SETTING_INFO, (WPARAM)m_szPath_Default.GetBuffer(), WM_Setting_ALARMID);
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
void CWnd_Cfg_ALID_List::UpdateUI_EquipmentData(__in int IN_nIndex)
{
	//m_ed_EqpAlias.SetWindowText(_T(""));


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

bool CWnd_Cfg_ALID_List::Verify_SVSequence(__in const CConfig_AL_LIST* IN_pSVInfo)
{
	if (nullptr == IN_pSVInfo)
		return false;

	int iCheckPair = 0;
	bool bPass = true;

	for (auto nIdx = 0; nIdx < IN_pSVInfo->Get_AL_Count(); nIdx++)
	{
		CConfig_AL StepUnit = IN_pSVInfo->GetAt(nIdx);		
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
void CWnd_Cfg_ALID_List::OnLanguage(__in uint8_t IN_nLanguage)
{
	__super::OnLanguage(IN_nLanguage);

	m_lc_ALInfo.OnLanguage(IN_nLanguage);

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

	m_cb_nALCD.SetFont(&m_Font);

	if (GetSafeHwnd())
	{
		m_bn_Apply.SetWindowText(g_szCtrlText_T[m_nLanguage][Txt_Apply]);
		m_bn_VerifyLineConfig.SetWindowText(g_szCtrlText_T[m_nLanguage][Txt_VerifyLineConfig]);

		for (UINT nIdx = 0; nIdx < ID_MaxEnum; nIdx++)
		{
			m_bn_ItemCtrl[nIdx].SetWindowText(g_szTestCtrl_T[m_nLanguage][nIdx]);
		}

		for (auto nIdx = 0; nIdx < XML_MaxCount; nIdx++)
		{
			m_bn_XmlCtrl[nIdx].SetWindowText(g_szListCtrl_T[m_nLanguage][nIdx]);
		}

		for (UINT nIdx = 0; nIdx < EID_MaxEnum; nIdx++)
		{
			m_st_SVItemData[nIdx].SetText(g_szItemText_T[m_nLanguage][nIdx]);
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
/*
void CWnd_Cfg_SVID_List::Set_PermissionMode(__in enPermissionMode IN_PermissionMode)
{
	switch (IN_PermissionMode)
	{
	case enPermissionMode::Permission_Operator:
	case enPermissionMode::Permission_Engineer:
	{
		m_bn_Apply.EnableWindow(FALSE);
		m_lc_LineInfo[m_cb_ServerType.GetCurSel()].EnableWindow(FALSE);
		m_bn_VerifyLineConfig.EnableWindow(FALSE);

		for (auto nIdx = 0; nIdx < EC_MaxEnum; nIdx++)
		{
			m_bn_ItemCtrl[nIdx].EnableWindow(FALSE);
		}

		m_ed_EqpAlias.EnableWindow(FALSE);
		m_cb_EquipmentType.EnableWindow(FALSE);
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
		m_lc_LineInfo[m_cb_ServerType.GetCurSel()].EnableWindow(TRUE);
		m_bn_VerifyLineConfig.EnableWindow(TRUE);

		for (auto nIdx = 0; nIdx < EC_MaxEnum; nIdx++)
		{
			m_bn_ItemCtrl[nIdx].EnableWindow(TRUE);
		}

		m_ed_EqpAlias.EnableWindow(TRUE);
		m_cb_EquipmentType.EnableWindow(TRUE);
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

	default:
		break;
	}
}*/

//=============================================================================
// Method		: Set_FileFullPath
// Access		: public  
// Returns		: void
// Parameter	: __in LPCTSTR IN_szFullPath
// Qualifier	:
// Last Update	: 2022/1/5 - 17:32
// Desc.		:
//=============================================================================
void CWnd_Cfg_ALID_List::Set_FileFullPath(__in LPCTSTR IN_szFullPath)
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
void CWnd_Cfg_ALID_List::Set_XMLFilePath(__in LPCTSTR szPath)
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
void CWnd_Cfg_ALID_List::Set_UseExtraData(__in bool bUse)
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
void CWnd_Cfg_ALID_List::Set_Configuration(__in const CConfig_AL_LIST* IN_SVInfo)
{
	//2023.02.13a uhkim 
	m_lc_ALInfo.Set_ALInfo(IN_SVInfo);
}

//=============================================================================
// Method		: Set_Configuration
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2022/1/5 - 18:17
// Desc.		:
//=============================================================================
void CWnd_Cfg_ALID_List::Set_Configuration()
{
	if (FALSE == m_szPath_Default.IsEmpty())
	{
		CXmlALConfig	XmlConfig;
		CConfig_AL_LIST	stConfigLine_Info;

		if (XmlConfig.LoadXML_ALInfo(m_szPath_Default.GetBuffer(0), stConfigLine_Info))
		{
			// UI에 세팅
			Set_Configuration(&stConfigLine_Info);

			// 설비 구성이 정상인지 확인
			//if (false == Verify_SVSequence(&stConfigLine_Info))
			//{
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
void CWnd_Cfg_ALID_List::Get_Configuration(__out CConfig_AL_LIST& OUT_stLineInfo)
{
	m_lc_ALInfo.Get_ALInfo(OUT_stLineInfo);
}

//=============================================================================
// Method		: Init_DefaultSet
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2017/9/25 - 20:56
// Desc.		:
//=============================================================================
void CWnd_Cfg_ALID_List::Init_DefaultSet()
{
	// 리스트 초기화
	m_lc_ALInfo.DeleteAllItems();

	//m_cb_nSID_Type.SetCurSel(0);
	//m_cb_nSID_Type.EnableWindow(FALSE);

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
