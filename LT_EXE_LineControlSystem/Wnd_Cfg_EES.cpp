//*****************************************************************************
// Filename	: 	Wnd_Cfg_EES.cpp
// Created	:	
// Modified	:	2023.04.21 - 12:00
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
// Wnd_Cfg_EES.cpp : implementation file
//

#include "stdafx.h"
#include "Wnd_Cfg_EES.h"
#include "resource.h"
#include "Def_WindowMessage.h"
#include "XmlEESConfig.h"
#include "CommonFunction.h"
#include "Def_Language_Message.h"
#include "Dlg_ModifyEES.h"	

static LPCTSTR g_szTestCtrl_T[Lang_MaxCount][CWnd_Cfg_EES::ID_MaxEnum] =
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

static LPCTSTR g_szItemText_T[Lang_MaxCount][CWnd_Cfg_EES::EID_MaxEnum] =
{
	// 한국어
	{
		_T("설비상태"),		// EID_EquipmentState
		_T("상태명(K)"),	// TSH_EQUIPMENT_STATE_NAME_KOR
		_T("상태명(E)"),	// TSH_EQUIPMENT_STATE_NAME_ENG
		_T("LOSS CODE"),	// TSH_LOSSCODE
		_T("상태 색상"),	// TSH_STATE_COLOR
		_T("사용 여부"),	// TSH_USE
		_T("설명"),			// TSH_EXPLANATION
	},
	// 영어
	{
		_T("설비상태"),		// TSH_EQUIPMENT_STATE
		_T("상태명(K)"),	// TSH_EQUIPMENT_STATE_NAME_KOR
		_T("상태명(E)"),	// TSH_EQUIPMENT_STATE_NAME_ENG
		_T("LOSS CODE"),	// TSH_LOSSCODE
		_T("상태 색상"),	// TSH_STATE_COLOR
		_T("사용 여부"),	// TSH_USE
		_T("설명"),			// TSH_EXPLANATION
	},
	// 베트남어
	{
		_T("설비상태"),		// TSH_EQUIPMENT_STATE
		_T("상태명(K)"),	// TSH_EQUIPMENT_STATE_NAME_KOR
		_T("상태명(E)"),	// TSH_EQUIPMENT_STATE_NAME_ENG
		_T("LOSS CODE"),	// TSH_LOSSCODE
		_T("상태 색상"),	// TSH_STATE_COLOR
		_T("사용 여부"),	// TSH_USE
		_T("설명"),			// TSH_EXPLANATION
	},
	// 중국어
	{
		_T("설비상태"),		// TSH_EQUIPMENT_STATE
		_T("상태명(K)"),	// TSH_EQUIPMENT_STATE_NAME_KOR
		_T("상태명(E)"),	// TSH_EQUIPMENT_STATE_NAME_ENG
		_T("LOSS CODE"),	// TSH_LOSSCODE
		_T("상태 색상"),	// TSH_STATE_COLOR
		_T("사용 여부"),	// TSH_USE
		_T("설명"),			// TSH_EXPLANATION
	},
};


static LPCTSTR g_szListCtrl_T[Lang_MaxCount][CWnd_Cfg_EES::XML_MaxCount] =
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


#define IDC_ED_EQUIPMENTSTATE				3000
#define IDC_ED_EQUIPMENTSTATENAMEKOR		3001
#define IDC_ED_EQUIPMENTSTATENAMEENG		3002
#define IDC_ED_LOSSCODE						3003
#define IDC_ED_STATECOLOR					3004
#define IDC_CB_USE							3005
#define IDC_ED_EXPLANATION					3006

#define IDC_BN_STEPCTRL_S		1010//IDC_ED_EXPLANATION +1
#define IDC_BN_STEPCTRL_E		IDC_BN_STEPCTRL_S + ID_MaxEnum - 1

#define	IDC_BN_LISTCTRL_S		1100//IDC_BN_STEPCTRL_E +1
#define	IDC_BN_LISTCTRL_E		IDC_BN_LISTCTRL_S + XML_MaxCount - 1

IMPLEMENT_DYNAMIC(CWnd_Cfg_EES, CWnd_BaseView)

CWnd_Cfg_EES::CWnd_Cfg_EES()
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

CWnd_Cfg_EES::~CWnd_Cfg_EES()
{
	m_font_Default.DeleteObject();
	m_Font.DeleteObject();
}


BEGIN_MESSAGE_MAP(CWnd_Cfg_EES, CWnd_BaseView)
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
int CWnd_Cfg_EES::OnCreate(LPCREATESTRUCT lpCreateStruct)
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
	m_ed_EquipmentState.Create(dwStyle | ES_CENTER | WS_BORDER, rectDummy, this, IDC_ED_EQUIPMENTSTATE);
	m_ed_EquipmentState.SetFont(&m_Font);
	m_ed_EquipmentStateNameKor.Create(dwStyle | ES_CENTER | WS_BORDER, rectDummy, this, IDC_ED_EQUIPMENTSTATENAMEKOR);
	m_ed_EquipmentStateNameKor.SetFont(&m_Font);
	m_ed_EquipmentStateNameEng.Create(dwStyle | ES_CENTER | WS_BORDER, rectDummy, this, IDC_ED_EQUIPMENTSTATENAMEENG);
	m_ed_EquipmentStateNameEng.SetFont(&m_Font);
	m_ed_LossCode.Create(dwStyle | ES_CENTER | WS_BORDER, rectDummy, this, IDC_ED_LOSSCODE);
	m_ed_LossCode.SetFont(&m_Font);
	m_ed_StateColor.Create(dwStyle | ES_CENTER | WS_BORDER, rectDummy, this, IDC_ED_STATECOLOR);
	m_ed_StateColor.SetFont(&m_Font);
	m_cb_Use.Create(dwStyle | CBS_DROPDOWNLIST | WS_VSCROLL, rectDummy, this, IDC_CB_USE);
	m_cb_Use.SetFont(&m_Font);
	for (auto nIdx = 0; nIdx < enEES_USE::EES_USE_MAX; nIdx++)
	{
		m_cb_Use.AddString(g_szEES_USE[nIdx]);
	}
	m_ed_Explanation.Create(dwStyle | ES_CENTER | WS_BORDER, rectDummy, this, IDC_ED_EXPLANATION);
	m_ed_Explanation.SetFont(&m_Font);

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
void CWnd_Cfg_EES::OnSize(UINT nType, int cx, int cy)
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
	int iCtrlWidth		= 400;

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
	m_st_ItemData[EID_EquipmentState].MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
	m_ed_EquipmentState.MoveWindow(iLeft_Sub, iTop, iEdWidth, iCtrlHeight);
	// ID
	iLeft = iMargin;
	iTop += iCtrlHeight + iSpacing;
	m_st_ItemData[EID_EquipmentStateNameKor].MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
	m_ed_EquipmentStateNameKor.MoveWindow(iLeft_Sub, iTop, iEdWidth, iCtrlHeight);
	// ID Name
	iTop += iCtrlHeight + iSpacing;
	m_st_ItemData[EID_EquipmentStateNameEng].MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
	m_ed_EquipmentStateNameEng.MoveWindow(iLeft_Sub, iTop, iEdWidth, iCtrlHeight);
	// ID Text
	iTop += iCtrlHeight + iSpacing;
	m_st_ItemData[EID_LossCode].MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
	m_ed_LossCode.MoveWindow(iLeft_Sub, iTop, iEdWidth, iCtrlHeight);
	// SV Type
	iTop += iCtrlHeight + iSpacing;
	m_st_ItemData[EID_StateColor].MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
	m_ed_StateColor.MoveWindow(iLeft_Sub, iTop, iEdWidth, iCtrlHeight);
	//USE
	iTop += iCtrlHeight + iSpacing;
	m_st_ItemData[EID_USE].MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
	m_cb_Use.MoveWindow(iLeft_Sub, iTop, iEdWidth, iCtrlHeight);
	// SV
	iTop += iCtrlHeight + iSpacing;
	m_st_ItemData[EID_Explanation].MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
	m_ed_Explanation.MoveWindow(iLeft_Sub, iTop, iEdWidth, iCtrlHeight);


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

//=============================================================================
// Method		: OnBnClickedBnStepCtrl
// Access		: protected  
// Returns		: void
// Parameter	: UINT nID
// Qualifier	:
// Last Update	: 2017/9/25 - 9:51
// Desc.		:
//=============================================================================
void CWnd_Cfg_EES::OnBnClickedBnStepCtrl(UINT nID)
{
	enIDCtrl nIDIdx = (enIDCtrl)(nID - IDC_BN_STEPCTRL_S);
	switch (nIDIdx)
	{
	case CWnd_Cfg_EES::ID_Add:
		Item_Add();
		break;

	case CWnd_Cfg_EES::ID_Insert:
		Item_Insert();
		break;

	case CWnd_Cfg_EES::ID_Remove:
		Item_Remove();
		break;

	case CWnd_Cfg_EES::ID_Modify:
		Item_Modify();
		break;

	case CWnd_Cfg_EES::ID_Order_Up:
		Item_Up();
		break;

	case CWnd_Cfg_EES::ID_Order_Down:
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
void CWnd_Cfg_EES::OnBnClickedBnListCtrl(UINT nID)
{
	enXmlCtrl nIDIdx = (enXmlCtrl)(nID - IDC_BN_LISTCTRL_S);

	switch (nIDIdx)
	{
	case CWnd_Cfg_EES::XML_Load:
		LoadXML_EESInfo();
		break;

	case CWnd_Cfg_EES::XML_Save:
		SaveXML_EESInfo();
		break;

	case CWnd_Cfg_EES::XML_Clear:
	{
		if (IDYES == LT_MessageBox(g_szMessageBox_T[MB_Line_Delete_AllEqp][m_nLanguage], MB_YESNO))
		{
			m_lc_Info.Clear_EESInfo();
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
void CWnd_Cfg_EES::OnBnClickedBnClearList()
{
	if (IDYES == LT_MessageBox(g_szMessageBox_T[MB_Line_Delete_AllEqp][m_nLanguage], MB_YESNO))
	{
		m_lc_Info.Clear_EESInfo();
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
void CWnd_Cfg_EES::OnBnClickedBnVerifyLine()
{
	if (false == m_lc_Info.Check_EESCode())
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
void CWnd_Cfg_EES::OnBnClickedBnApply()
{
	Apply_EESInfo();
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
BOOL CWnd_Cfg_EES::Get_EESData(__out CConfig_EES& OUT_stData)
{
	CString szValue;
	m_ed_EquipmentState.GetWindowText(szValue);
	if (false == m_lc_Info.Check_EESCode(szValue))
	{
		LT_MessageBox(g_szMessageBox_T[MB_Line_IpAddr_Overlapped][m_nLanguage]);
		return FALSE;
	}
	//
	m_ed_EquipmentState.GetWindowText(szValue);
	OUT_stData.Set_EquipMentStateCode(szValue);
	//
	m_ed_EquipmentStateNameKor.GetWindowText(szValue);
	OUT_stData.Set_EquipMentStateKorText(szValue);
	// ---- SID	Name
	m_ed_EquipmentStateNameEng.GetWindowText(szValue);
	OUT_stData.Set_EquipMentStateEngText(szValue);
	//
	m_ed_LossCode.GetWindowText(szValue);
	OUT_stData.Set_LossCode(szValue);
	//
	m_ed_StateColor.GetWindowText(szValue);
	OUT_stData.Set_StateColor(szValue);
	//
	OUT_stData.Set_Use(m_cb_Use.GetCurSel());

	m_ed_Explanation.GetWindowText(szValue);
	OUT_stData.Set_Explanation(szValue);
	Verify_EES(OUT_stData);

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
void CWnd_Cfg_EES::Verify_EES(__inout CConfig_EES& OUT_stSV)
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
void CWnd_Cfg_EES::Item_Add()
{
	CConfig_EES stData;
	if (Get_EESData(stData))
	{
		m_lc_Info.Item_Add(stData);
		m_lc_Info.Get_SelectedEES(stData);
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
void CWnd_Cfg_EES::Item_Insert()
{
	CConfig_EES stData;
	if (Get_EESData(stData))
	{
		m_lc_Info.Item_Insert(stData);
		m_lc_Info.Get_SelectedEES(stData);
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
void CWnd_Cfg_EES::Item_Remove()
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
void CWnd_Cfg_EES::Item_Modify()
{
	if (m_lc_Info.GetSelectedCount() <= 0)
	{
		LT_MessageBox(g_szMessageBox_T[MB_Common_SelectItem][m_nLanguage]);
		return;
	}
	CConfig_EES Data;

	if (m_lc_Info.Get_SelectedEES(Data))
	{

		CDlg_ModifyEES	m_dlg_Modify;
		m_dlg_Modify.OnLanguage(m_nLanguage);
		m_dlg_Modify.Set_EESData(&Data);
		if (IDOK == m_dlg_Modify.DoModal())
		{
			CConfig_EES stData_New;
			stData_New = m_dlg_Modify.Get_EESData();
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
void CWnd_Cfg_EES::Item_Up()
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
void CWnd_Cfg_EES::Item_Down()
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
BOOL CWnd_Cfg_EES::LoadXML_EESInfo()
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
		CXmlEESConfig		StepSeq;
		CConfig_EES_LIST	stConfig_Info;

		if (StepSeq.LoadXML_EESInfo(strFullPath.GetBuffer(0), stConfig_Info))
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
BOOL CWnd_Cfg_EES::LoadXML_EESInfo(__out CConfig_EES_LIST& OUT_stInfo, __out CString& OUT_FilePath, __out CString& OUT_FileTitle)
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

		CXmlEESConfig	StepSeq;
		if (StepSeq.LoadXML_EESInfo(strFullPath.GetBuffer(0), OUT_stInfo))
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
BOOL CWnd_Cfg_EES::LoadXML_EESInfo(__in LPCTSTR IN_szFilePath, __out CConfig_EES_LIST& OUT_stInfo)
{
	CXmlEESConfig	StepSeq;
	if (StepSeq.LoadXML_EESInfo(IN_szFilePath, OUT_stInfo))
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
BOOL CWnd_Cfg_EES::SaveXML_EESInfo()
{
	// UI상의 데이터 얻기
	CConfig_EES_LIST stConfig_Info;
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
		CXmlEESConfig	StepSeq;
		if (FALSE == StepSeq.SaveXML_EESInfo(strFullPath, &stConfig_Info))
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
void CWnd_Cfg_EES::Apply_EESInfo()
{
	// UI상의 데이터 얻기
	CConfig_EES_LIST stInfo;
	Get_Configuration(stInfo);

	// 설비 구성이 정상인지 확인 ( loop begin -> loop end 확인)
	if (false == Verify_Sequence(&stInfo))
	{
		LT_MessageBox(g_szMessageBox_T[MB_Line_EqpCfg_Error_Cancel][m_nLanguage]);
		return;
	}
	// 저장	
	CXmlEESConfig	XmlConfig;
	if (FALSE == XmlConfig.SaveXML_EESInfo(m_szPath_Default, &stInfo))
	{
		LT_MessageBox(g_szMessageBox_T[MB_Common_SaveFile_Failed][m_nLanguage], MB_SYSTEMMODAL);
		return;
	}
	// Recipe 데이터 불러오기
#if (USE_XML)
	GetOwner()->SendNotifyMessage(WM_CHANGED_SETTING_INFO, (WPARAM)m_szPath_Default.GetBuffer(), WM_Setting_EES);
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
bool CWnd_Cfg_EES::Verify_Sequence(__in const CConfig_EES_LIST* IN_pInfo)
{
	if (nullptr == IN_pInfo)
		return false;
	// 설비 구성이 정상인지 확인 

	// SFR 협각 / 광각 및 0도 / 90도 검사를 동시에 사용하지 않음
	int iCheckPair = 0;
	bool bPass = true;

	for (auto nIdx = 0; nIdx < IN_pInfo->Get_EES_Count(); nIdx++)
	{
		CConfig_EES StepUnit = IN_pInfo->GetAt(nIdx);
		
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
void CWnd_Cfg_EES::OnLanguage(__in uint8_t IN_nLanguage)
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
void CWnd_Cfg_EES::Set_PermissionMode(__in enPermissionMode IN_PermissionMode)
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
		m_ed_EquipmentState.EnableWindow(FALSE);
		m_ed_EquipmentStateNameKor.EnableWindow(FALSE);
		m_ed_EquipmentStateNameEng.EnableWindow(FALSE);
		m_ed_LossCode.EnableWindow(FALSE);
		m_ed_StateColor.EnableWindow(FALSE);
		m_cb_Use.EnableWindow(FALSE);
		m_ed_Explanation.EnableWindow(FALSE);

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

		m_ed_EquipmentState.EnableWindow(TRUE);
		m_ed_EquipmentStateNameKor.EnableWindow(TRUE);
		m_ed_EquipmentStateNameEng.EnableWindow(TRUE);
		m_ed_LossCode.EnableWindow(TRUE);
		m_ed_StateColor.EnableWindow(TRUE);
		m_cb_Use.EnableWindow(TRUE);
		m_ed_Explanation.EnableWindow(TRUE);

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
void CWnd_Cfg_EES::Set_FileFullPath(__in LPCTSTR IN_szFullPath)
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
void CWnd_Cfg_EES::Set_XMLFilePath(__in LPCTSTR szPath)
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
void CWnd_Cfg_EES::Set_UseExtraData(__in bool bUse)
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
void CWnd_Cfg_EES::Set_Configuration(__in const CConfig_EES_LIST* IN_Info)
{
	//2023.02.13a uhkim 
	m_lc_Info.Set_EESInfo(IN_Info);
}

//=============================================================================
// Method		: Set_Configuration
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2022/1/5 - 18:17
// Desc.		:
//=============================================================================
void CWnd_Cfg_EES::Set_Configuration()
{
	if (FALSE == m_szPath_Default.IsEmpty())
	{
		CXmlEESConfig		XmlConfig;
		CConfig_EES_LIST	stConfig_Info;

		if (XmlConfig.LoadXML_EESInfo(m_szPath_Default.GetBuffer(0), stConfig_Info))
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
void CWnd_Cfg_EES::Get_Configuration(__out CConfig_EES_LIST& OUT_stInfo)
{
	m_lc_Info.Get_EESInfo(OUT_stInfo);
}

//=============================================================================
// Method		: Init_DefaultSet
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2017/9/25 - 20:56
// Desc.		:
//=============================================================================
void CWnd_Cfg_EES::Init_DefaultSet()
{
	// 리스트 초기화
	m_lc_Info.DeleteAllItems();
}
