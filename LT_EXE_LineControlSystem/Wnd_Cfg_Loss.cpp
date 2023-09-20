//*****************************************************************************
// Filename	: 	Wnd_Cfg_Loss.cpp
// Created	:	
// Modified	:	2023.04.21 - 12:00
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
// Wnd_Cfg_Loss.cpp : implementation file
//

#include "stdafx.h"
#include "Wnd_Cfg_Loss.h"
#include "resource.h"
#include "Def_WindowMessage.h"
#include "XmlLossConfig.h"
#include "CommonFunction.h"
#include "Def_Language_Message.h"
#include "Dlg_ModifyLoss.h"	

static LPCTSTR g_szTestCtrl_T[Lang_MaxCount][CWnd_Cfg_Loss::ID_MaxEnum] =
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

static LPCTSTR g_szItemText_T[Lang_MaxCount][CWnd_Cfg_Loss::EID_MaxEnum] =
{
	// 한국어
	{
		_T("로스이름"),		// 
		_T("아이템"),		// T
		_T("코드로스"),		// T
		_T("로스이름(E)"),	// T
		_T("설명"),			// TSH_EXPLANATION
	},
	// 영어
	{
		_T("로스이름"),		// 
		_T("아이템"),		// T
		_T("코드로스"),		// T
		_T("로스이름(E)"),	// T
		_T("설명"),			// TSH_EXPLANATION
	},
	// 베트남어
	{
		_T("로스이름"),		// 
		_T("아이템"),		// T
		_T("코드로스"),		// T
		_T("로스이름(E)"),	// T
		_T("설명"),			// TSH_EXPLANATION
	},
	// 중국어
	{
		_T("로스이름"),		// 
		_T("아이템"),		// T
		_T("코드로스"),		// T
		_T("로스이름(E)"),	// T
		_T("설명"),			// TSH_EXPLANATION
	},
};


static LPCTSTR g_szListCtrl_T[Lang_MaxCount][CWnd_Cfg_Loss::XML_MaxCount] =
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



#define IDC_LC_STEPLIST			1200
#define IDC_CB_EQP_TYPE			1201
#define IDC_BN_VERIFY_LINE		1202
#define IDC_BN_MODIFY_STEP		1203
#define IDC_CB_SVR_TYPE			1204

#define IDC_ED_DATA_S			1210
#define IDC_ED_DATA_E			IDC_ED_DATA_S + MAX_STEP_DATA - 1


#define IDC_ED_NAME_LOSS_TEM				3000
#define IDC_ED_TEM							3001
#define IDC_ED_CODELOSS						3002
#define IDC_ED_NAME_LOSS_TEM_ENG			3003
#define IDC_ED_EXPLAIN						3004

#define IDC_BN_STEPCTRL_S		1010//IDC_ED_EXPLANATION +1
#define IDC_BN_STEPCTRL_E		IDC_BN_STEPCTRL_S + ID_MaxEnum - 1

#define	IDC_BN_LISTCTRL_S		1100//IDC_BN_STEPCTRL_E +1
#define	IDC_BN_LISTCTRL_E		IDC_BN_LISTCTRL_S + XML_MaxCount - 1

IMPLEMENT_DYNAMIC(CWnd_Cfg_Loss, CWnd_BaseView)

CWnd_Cfg_Loss::CWnd_Cfg_Loss()
{
	VERIFY(m_font_Default.CreateFont(
		11,						// nHeight
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

CWnd_Cfg_Loss::~CWnd_Cfg_Loss()
{
	m_font_Default.DeleteObject();
	m_Font.DeleteObject();
}


BEGIN_MESSAGE_MAP(CWnd_Cfg_Loss, CWnd_BaseView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND_RANGE(IDC_BN_STEPCTRL_S,		IDC_BN_STEPCTRL_E,		OnBnClickedBnStepCtrl)
	ON_COMMAND_RANGE(IDC_BN_LISTCTRL_S,		IDC_BN_LISTCTRL_E,		OnBnClickedBnListCtrl)
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
// Last Update	: 2023.04.21 - 12:00
// Desc.		:
//=============================================================================
int CWnd_Cfg_Loss::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd_BaseView::OnCreate(lpCreateStruct) == -1)
		return -1;

	DWORD dwStyle = WS_VISIBLE | WS_CHILD;
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	m_bn_Apply.Create(g_szCtrlText_T[m_nLanguage][Txt_Apply], dwStyle | BS_PUSHBUTTON, rectDummy, this, IDC_BN_APPLY);

	//---------------------------------------------------------------
	// 설비 리스트
	//---------------------------------------------------------------
	m_lc_Info.Create(WS_CHILD | WS_VISIBLE, rectDummy, this, IDC_LC_STEPLIST);

	m_bn_VerifyConfig.Create(g_szCtrlText_T[m_nLanguage][Txt_VerifyLineConfig], dwStyle | BS_PUSHLIKE, rectDummy, this, IDC_BN_VERIFY_LINE);

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
	m_ed_NameLossTem.Create(dwStyle | ES_CENTER | WS_BORDER, rectDummy, this, IDC_ED_NAME_LOSS_TEM);
	m_ed_NameLossTem.SetFont(&m_Font);
	m_ed_Tem.Create(dwStyle | ES_CENTER | WS_BORDER, rectDummy, this, IDC_ED_TEM);
	m_ed_Tem.SetFont(&m_Font);
	m_ed_CodeLoss.Create(dwStyle | ES_CENTER | WS_BORDER, rectDummy, this, IDC_ED_CODELOSS);
	m_ed_CodeLoss.SetFont(&m_Font);
	m_ed_NameLossTemEng.Create(dwStyle | ES_CENTER | WS_BORDER, rectDummy, this, IDC_ED_NAME_LOSS_TEM_ENG);
	m_ed_NameLossTemEng.SetFont(&m_Font);
	m_ed_Explan.Create(dwStyle | ES_CENTER | WS_BORDER, rectDummy, this, IDC_ED_EXPLAIN);
	m_ed_Explan.SetFont(&m_font_Default);


	// 설비 항목
	for (UINT nIdx = 0; nIdx < EID_MaxEnum; nIdx++)
	{
		m_st_ItemData[nIdx].SetStaticStyle(CVGStatic::StaticStyle_Default);
		m_st_ItemData[nIdx].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_st_ItemData[nIdx].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_st_ItemData[nIdx].SetFont_Gdip(L"Tahoma", 10.0F, Gdiplus::FontStyleRegular);

		m_st_ItemData[nIdx].Create(g_szItemText_T[m_nLanguage][nIdx], dwStyle | SS_CENTER | SS_CENTERIMAGE, rectDummy, this, IDC_STATIC);
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
// Last Update	: 2023.04.21 - 12:00
// Desc.		:
//=============================================================================
void CWnd_Cfg_Loss::OnSize(UINT nType, int cx, int cy)
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
	int iListWidth		= 900; 
	int iListHeight		= iHeight - ((iCtrlHeight + iSpacing) * 2);
	//---------------------------------------------------------------
	// 설비 리스트
	//---------------------------------------------------------------
	iLeft = iMargin + iCtrlWidth + iSpacing + iListWidth - 250;
	m_bn_Apply.MoveWindow(iLeft, iTop, 250, iCtrlHeight);
	iLeft = iMargin;
	iTop += iCtrlHeight + iSpacing;

	// ID Type
	m_st_ItemData[EID_NameLoss].MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
	m_ed_NameLossTem.MoveWindow(iLeft_Sub, iTop, iEdWidth, iCtrlHeight);
	// ID
	iLeft = iMargin;
	iTop += iCtrlHeight + iSpacing;
	m_st_ItemData[EID_Tem].MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
	m_ed_Tem.MoveWindow(iLeft_Sub, iTop, iEdWidth, iCtrlHeight);
	// ID Name
	iTop += iCtrlHeight + iSpacing;
	m_st_ItemData[EID_CodeLoss].MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
	m_ed_CodeLoss.MoveWindow(iLeft_Sub, iTop, iEdWidth, iCtrlHeight);
	// ID Text
	iTop += iCtrlHeight + iSpacing;
	m_st_ItemData[EID_NameLossEng].MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
	m_ed_NameLossTemEng.MoveWindow(iLeft_Sub, iTop, iEdWidth, iCtrlHeight);
	// SV Type
	iTop += iCtrlHeight + iSpacing;
	m_st_ItemData[EID_ExplainLoss].MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
	m_ed_Explan.MoveWindow(iLeft_Sub, iTop, iEdWidth, iCtrlHeight);


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
	m_bn_VerifyConfig.MoveWindow(iLeft, iTop, iCtrlWidth, iCtrlHeight);


	// 리스트 컨트롤
	iTop = iMargin + iCtrlHeight + iSpacing;
	iLeft = iMargin + iCtrlWidth + iCateSpacing;;//2023.02.13a List추가 (크기)
	iWidth = cx - iLeft - iMargin;
	m_lc_Info.MoveWindow(iLeft, iTop, iListWidth, iListHeight);

	
	iTop += iListHeight + iSpacing;
	iCtrlWidth	= __min(160, ((iListWidth - (iSpacing * 4)) / 5));
	for (auto nIdx = 0; nIdx < XML_MaxCount; nIdx++)
	{
		m_bn_XmlCtrl[nIdx].MoveWindow(iLeft, iTop, iCtrlWidth, iCtrlHeight);
		iLeft += iCtrlWidth + iSpacing;
	}
}
/*
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
void CWnd_Cfg_EES::MoveWindow_EqpCtrl(int x, int y, int nWidth, int nHeight)
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
void CWnd_Cfg_EES::MoveWindow_Item(int x, int y, int nWidth, int nHeight)
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

	iTop += iCtrlHeight + iSpacing;
	m_st_SVItemData[EID_SID_Type].MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
	m_cb_nSID_Type.MoveWindow(iLeftSub, iTop, iHalfWidth, iCtrlHeight);

	iTop += iCtrlHeight + iSpacing;
	m_st_SVItemData[EID_SID].MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
	m_ed_nSID.MoveWindow(iLeftSub, iTop, iHalfWidth, iCtrlHeight);

	iTop += iCtrlHeight + iSpacing;
	m_st_SVItemData[EID_SID_Name].MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
	m_ed_SID_Name.MoveWindow(iLeftSub, iTop, iHalfWidth, iCtrlHeight);

	iTop += iCtrlHeight + iSpacing;
	m_st_SVItemData[EID_SID_TX].MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
	m_ed_SID_TX.MoveWindow(iLeftSub, iTop, iHalfWidth, iCtrlHeight);

	iTop += iCtrlHeight + iSpacing;
	m_st_SVItemData[EID_SV_Type].MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
	m_cb_SV_Type.MoveWindow(iLeftSub, iTop, iHalfWidth, iCtrlHeight);

	iTop += iCtrlHeight + iSpacing;
	m_st_SVItemData[EID_USE].MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
	m_cb_SID_Use.MoveWindow(iLeftSub, iTop, iHalfWidth, iCtrlHeight);

	iTop += iCtrlHeight + iSpacing;
	m_st_SVItemData[EID_SV].MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
	m_ed_SV.MoveWindow(iLeftSub, iTop, iHalfWidth, iCtrlHeight);
}*/

//=============================================================================
// Method		: OnBnClickedBnStepCtrl
// Access		: protected  
// Returns		: void
// Parameter	: UINT nID
// Qualifier	:
// Last Update	: 2017/9/25 - 9:51
// Desc.		:
//=============================================================================
void CWnd_Cfg_Loss::OnBnClickedBnStepCtrl(UINT nID)
{
	enIDCtrl nIDIdx = (enIDCtrl)(nID - IDC_BN_STEPCTRL_S);
	switch (nIDIdx)
	{
	case CWnd_Cfg_Loss::ID_Add:
		Item_Add();
		break;

	case CWnd_Cfg_Loss::ID_Insert:
		Item_Insert();
		break;

	case CWnd_Cfg_Loss::ID_Remove:
		Item_Remove();
		break;

	case CWnd_Cfg_Loss::ID_Modify:
		Item_Modify();
		break;

	case CWnd_Cfg_Loss::ID_Order_Up:
		Item_Up();
		break;

	case CWnd_Cfg_Loss::ID_Order_Down:
		Item_Down();
		break;

	default:
		break;
	}
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
void CWnd_Cfg_Loss::OnBnClickedBnListCtrl(UINT nID)
{
	enXmlCtrl nIDIdx = (enXmlCtrl)(nID - IDC_BN_LISTCTRL_S);

	switch (nIDIdx)
	{
	case CWnd_Cfg_Loss::XML_Load:
		LoadXML_LossInfo();
		break;

	case CWnd_Cfg_Loss::XML_Save:
		SaveXML_LossInfo();
		break;

	case CWnd_Cfg_Loss::XML_Clear:
	{
		if (IDYES == LT_MessageBox(g_szMessageBox_T[MB_Line_Delete_AllEqp][m_nLanguage], MB_YESNO))
		{
			m_lc_Info.Clear_LossInfo();
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
void CWnd_Cfg_Loss::OnBnClickedBnClearList()
{
	if (IDYES == LT_MessageBox(g_szMessageBox_T[MB_Line_Delete_AllEqp][m_nLanguage], MB_YESNO))
	{
		m_lc_Info.Clear_LossInfo();
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
void CWnd_Cfg_Loss::OnBnClickedBnVerifyLine()
{
	if (false == m_lc_Info.Check_LossCode())
	{
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
void CWnd_Cfg_Loss::OnBnClickedBnApply()
{
	Apply_LossInfo();
}

//=============================================================================
// Method		: OnBnClickedBnModifyEqpData
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/11/9 - 15:45
// Desc.		:
//=============================================================================
// void CWnd_Cfg_EES::OnBnClickedBnModifyEqpData()
// {
// 	Item_Modify();
// }

//=============================================================================
// Method		: Get_EESData
// Access		: public  
// Returns		: BOOL
// Parameter	: __out CConfig_SV& OUT_stSV
// Qualifier	:
// Last Update	: 2021/11/9 - 14:21
// Desc.		:
//=============================================================================
BOOL CWnd_Cfg_Loss::Get_LossData(__out CConfig_Loss& OUT_stData)
{
	CString szValue;
	m_ed_CodeLoss.GetWindowText(szValue);
	if (false == m_lc_Info.Check_LossCode(szValue))
	{
		LT_MessageBox(g_szMessageBox_T[MB_Line_IpAddr_Overlapped][m_nLanguage]);
		return FALSE;
	}
	//
	m_ed_NameLossTem.GetWindowText(szValue);
	OUT_stData.Set_NameLoss(szValue);
	//
	m_ed_Tem.GetWindowText(szValue);
	OUT_stData.Set_Tem(szValue);
	// ---- SID	Name
	m_ed_CodeLoss.GetWindowText(szValue);
	OUT_stData.Set_CodeLoss(szValue);
	//
	m_ed_NameLossTemEng.GetWindowText(szValue);
	OUT_stData.Set_NameLossEng(szValue);
	//
	m_ed_Explan.GetWindowText(szValue);
	OUT_stData.Set_Explain(szValue);

	Verify_Loss(OUT_stData);

	return TRUE;
}

//=============================================================================
// Method		: Verify_EES
// Access		: protected  
// Returns		: void
// Parameter	: __inout CConfig_Eqp & OUT_stEquipment
// Qualifier	:
// Last Update	: 2021/1/8 - 13:55
// Desc.		:
//=============================================================================
void CWnd_Cfg_Loss::Verify_Loss(__inout CConfig_Loss& OUT_stData)
{

}

//=============================================================================
// Method		: Item_Add
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2017/9/25 - 10:25
// Desc.		:
//=============================================================================
void CWnd_Cfg_Loss::Item_Add()
{
	CConfig_Loss stData;
	if (Get_LossData(stData))
	{
		m_lc_Info.Item_Add(stData);
		m_lc_Info.Get_SelectedLoss(stData);
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
void CWnd_Cfg_Loss::Item_Insert()
{
	CConfig_Loss stData;
	if (Get_LossData(stData))
	{
		m_lc_Info.Item_Insert(stData);
		m_lc_Info.Get_SelectedLoss(stData);
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
void CWnd_Cfg_Loss::Item_Remove()
{
	m_lc_Info.Item_Remove();
}

//=============================================================================
// Method		: Item_Modify
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/11/10 - 13:55
// Desc.		:
//=============================================================================
void CWnd_Cfg_Loss::Item_Modify()
{
	if (m_lc_Info.GetSelectedCount() <= 0)
	{
		LT_MessageBox(g_szMessageBox_T[MB_Common_SelectItem][m_nLanguage]);
		return;
	}
	CConfig_Loss Data;

	if (m_lc_Info.Get_SelectedLoss(Data))
	{

		CDlg_ModifyLoss	m_dlg_Modify;
		m_dlg_Modify.OnLanguage(m_nLanguage);
		m_dlg_Modify.Set_LossData(&Data);
		if (IDOK == m_dlg_Modify.DoModal())
		{
			CConfig_Loss stData_New;
			stData_New = m_dlg_Modify.Get_LossData();
			m_lc_Info.Item_Modify(stData_New);
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
void CWnd_Cfg_Loss::Item_Up()
{
	m_lc_Info.Item_Up();
}

//=============================================================================
// Method		: Item_Down
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2017/9/25 - 19:17
// Desc.		:
//=============================================================================
void CWnd_Cfg_Loss::Item_Down()
{
	m_lc_Info.Item_Down();
}

//=============================================================================
// Method		: LoadXML
// Access		: protected  
// Returns		: BOOL
// Qualifier	:
// Last Update	: 2018/3/29 - 15:28
// Desc.		:
//=============================================================================
BOOL CWnd_Cfg_Loss::LoadXML_LossInfo()
{
	// 설정된 데이터가 삭제 됩니다.
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
		CXmlLossConfig		StepSeq;
		CConfig_Loss_LIST	stConfig_Info;

		if (StepSeq.LoadXML_LossInfo(strFullPath.GetBuffer(0), stConfig_Info))
		{
			// UI에 세팅
			Set_Configuration(&stConfig_Info);

			// 설비 구성이 정상인지 확인
			if (false == Verify_Sequence(&stConfig_Info))
			{
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
BOOL CWnd_Cfg_Loss::LoadXML_LossInfo(__out CConfig_Loss_LIST& OUT_stInfo, __out CString& OUT_FilePath, __out CString& OUT_FileTitle)
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

		CXmlLossConfig	StepSeq;
		if (StepSeq.LoadXML_LossInfo(strFullPath.GetBuffer(0), OUT_stInfo))
		{
			OUT_FilePath = strFullPath;
			OUT_FileTitle = fileDlg.GetFileTitle();

			// 설비 구성이 정상인지 확인 ( loop begin -> loop end 확인)
			if (false == Verify_Sequence(&OUT_stInfo))
			{
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
BOOL CWnd_Cfg_Loss::LoadXML_LossInfo(__in LPCTSTR IN_szFilePath, __out CConfig_Loss_LIST& OUT_stInfo)
{
	CXmlLossConfig	StepSeq;
	if (StepSeq.LoadXML_LossInfo(IN_szFilePath, OUT_stInfo))
	{
		// 설비 구성이 정상인지 확인 ( loop begin -> loop end 확인)
		if (false == Verify_Sequence(&OUT_stInfo))
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
BOOL CWnd_Cfg_Loss::SaveXML_LossInfo()
{
	// UI상의 데이터 얻기
	CConfig_Loss_LIST stConfig_Info;
	Get_Configuration(stConfig_Info);

	// 설비 구성이 정상인지 확인 ( loop begin -> loop end 확인)
	if (false == Verify_Sequence(&stConfig_Info))
	{
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
		CXmlLossConfig	StepSeq;
		if (FALSE == StepSeq.SaveXML_LossInfo(strFullPath, &stConfig_Info))
		{	
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
void CWnd_Cfg_Loss::Apply_LossInfo()
{
	// UI상의 데이터 얻기
	CConfig_Loss_LIST stInfo;
	Get_Configuration(stInfo);

	// 설비 구성이 정상인지 확인 ( loop begin -> loop end 확인)
	if (false == Verify_Sequence(&stInfo))
	{
		LT_MessageBox(g_szMessageBox_T[MB_Line_EqpCfg_Error_Cancel][m_nLanguage]);
		return;
	}
	// 저장	
	CXmlLossConfig	XmlConfig;
	if (FALSE == XmlConfig.SaveXML_LossInfo(m_szPath_Default, &stInfo))
	{
		LT_MessageBox(g_szMessageBox_T[MB_Common_SaveFile_Failed][m_nLanguage], MB_SYSTEMMODAL);
		return;
	}
	// Recipe 데이터 불러오기
#if (USE_XML)
	GetOwner()->SendNotifyMessage(WM_CHANGED_SETTING_INFO, (WPARAM)m_szPath_Default.GetBuffer(), WM_Setting_Loss);
#endif
	LT_MessageBox(g_szMessageBox_T[MB_Common_SaveFile_Succeed][m_nLanguage]);
}

//=============================================================================
// Method		: Verify_Sequence
// Access		: protected  
// Returns		: bool
// Parameter	: __in const CConfig_Line * IN_pLineInfo
// Qualifier	:
// Last Update	: 2021/1/19 - 18:06
// Desc.		:
//=============================================================================
bool CWnd_Cfg_Loss::Verify_Sequence(__in const CConfig_Loss_LIST* IN_pInfo)
{
	if (nullptr == IN_pInfo)
		return false;
	// 설비 구성이 정상인지 확인 

	// SFR 협각 / 광각 및 0도 / 90도 검사를 동시에 사용하지 않음
	int iCheckPair = 0;
	bool bPass = true;

	for (auto nIdx = 0; nIdx < IN_pInfo->Get_Loss_Count(); nIdx++)
	{
		CConfig_Loss StepUnit = IN_pInfo->GetAt(nIdx);
		
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
void CWnd_Cfg_Loss::OnLanguage(__in uint8_t IN_nLanguage)
{
	__super::OnLanguage(IN_nLanguage);

	m_lc_Info.OnLanguage(IN_nLanguage);

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

	if (GetSafeHwnd())
	{
		m_bn_Apply.SetWindowText(g_szCtrlText_T[m_nLanguage][Txt_Apply]);
		m_bn_VerifyConfig.SetWindowText(g_szCtrlText_T[m_nLanguage][Txt_VerifyLineConfig]);
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
			m_st_ItemData[nIdx].SetText(g_szItemText_T[m_nLanguage][nIdx]);
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
void CWnd_Cfg_Loss::Set_PermissionMode(__in enPermissionMode IN_PermissionMode)
{
	switch (IN_PermissionMode)
	{
	case enPermissionMode::Permission_Operator:
	case enPermissionMode::Permission_Engineer:
	{
		m_bn_Apply.EnableWindow(FALSE);
		m_lc_Info.EnableWindow(FALSE);
		m_bn_VerifyConfig.EnableWindow(FALSE);

		for (auto nIdx = 0; nIdx < EID_MaxEnum; nIdx++)
		{
			m_bn_ItemCtrl[nIdx].EnableWindow(FALSE);
		}
		m_ed_NameLossTem.EnableWindow(FALSE);
		m_ed_Tem.EnableWindow(FALSE);
		m_ed_CodeLoss.EnableWindow(FALSE);
		m_ed_NameLossTemEng.EnableWindow(FALSE);
		m_ed_Explan.EnableWindow(FALSE);

		for (auto nIdx = 0; nIdx < XML_MaxCount; nIdx++)
		{
			m_bn_XmlCtrl[nIdx].EnableWindow(FALSE);
		}
	}
		break;

	case enPermissionMode::Permission_Administrator:
	{
		m_bn_Apply.EnableWindow(TRUE);
		m_lc_Info.EnableWindow(TRUE);
		m_bn_VerifyConfig.EnableWindow(TRUE);

		for (auto nIdx = 0; nIdx < EID_MaxEnum; nIdx++)
		{
			m_bn_ItemCtrl[nIdx].EnableWindow(TRUE);
		}
		m_ed_NameLossTem.EnableWindow(TRUE);
		m_ed_Tem.EnableWindow(TRUE);
		m_ed_CodeLoss.EnableWindow(TRUE);
		m_ed_NameLossTemEng.EnableWindow(TRUE);
		m_ed_Explan.EnableWindow(TRUE);

		for (auto nIdx = 0; nIdx < XML_MaxCount; nIdx++)
		{
			m_bn_XmlCtrl[nIdx].EnableWindow(TRUE);
		}
	}
		break;

	default:
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
void CWnd_Cfg_Loss::Set_FileFullPath(__in LPCTSTR IN_szFullPath)
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
void CWnd_Cfg_Loss::Set_XMLFilePath(__in LPCTSTR szPath)
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
void CWnd_Cfg_Loss::Set_UseExtraData(__in bool bUse)
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
void CWnd_Cfg_Loss::Set_Configuration(__in const CConfig_Loss_LIST* IN_Info)
{
	//2023.02.13a uhkim 
	m_lc_Info.Set_LossInfo(IN_Info);
}

//=============================================================================
// Method		: Set_Configuration
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2022/1/5 - 18:17
// Desc.		:
//=============================================================================
void CWnd_Cfg_Loss::Set_Configuration()
{
	if (FALSE == m_szPath_Default.IsEmpty())
	{
		CXmlLossConfig		XmlConfig;
		CConfig_Loss_LIST	stConfig_Info;

		if (XmlConfig.LoadXML_LossInfo(m_szPath_Default.GetBuffer(0), stConfig_Info))
		{
			// UI에 세팅
			Set_Configuration(&stConfig_Info);
			// 설비 구성이 정상인지 확인
			//if (false == Verify_Sequence(&stConfig_Info))
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
void CWnd_Cfg_Loss::Get_Configuration(__out CConfig_Loss_LIST& OUT_stInfo)
{
	m_lc_Info.Get_LossInfo(OUT_stInfo);
}

//=============================================================================
// Method		: Init_DefaultSet
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2017/9/25 - 20:56
// Desc.		:
//=============================================================================
void CWnd_Cfg_Loss::Init_DefaultSet()
{
	// 리스트 초기화
	m_lc_Info.DeleteAllItems();
}
