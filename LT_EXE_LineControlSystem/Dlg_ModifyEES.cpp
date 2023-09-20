//*****************************************************************************
// Filename	: 	Dlg_ModifyEES.cpp
// Created	:	
// Modified	:	2023.04.20
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
// Dlg_ModifyEES.cpp : implementation file
//
#include "stdafx.h"
#include "resource.h"
#include "Dlg_ModifyEES.h"
#include "afxdialogex.h"
#include "Def_Language_Message.h"

// CDlg_ModifyEES 대화 상자
#define IDC_ED_EQUIPMENTSTATE				3000
#define IDC_ED_EQUIPMENTSTATENAMEKOR		3001
#define IDC_ED_EQUIPMENTSTATENAMEENG		3002
#define IDC_ED_LOSSCODE						3003
#define IDC_ED_STATECOLOR					3004
#define IDC_CB_USE							3005
#define IDC_ED_EXPLANATION					3006

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
		_T("설비상태"),		// EID_EquipmentState
		_T("상태명(K)"),	// TSH_EQUIPMENT_STATE_NAME_KOR
		_T("상태명(E)"),	// TSH_EQUIPMENT_STATE_NAME_ENG
		_T("LOSS CODE"),	// TSH_LOSSCODE
		_T("상태 색상"),	// TSH_STATE_COLOR
		_T("사용 여부"),	// TSH_USE
		_T("설명"),			// TSH_EXPLANATION
	},
	// 베트남어
	{
		_T("설비상태"),		// EID_EquipmentState
		_T("상태명(K)"),	// TSH_EQUIPMENT_STATE_NAME_KOR
		_T("상태명(E)"),	// TSH_EQUIPMENT_STATE_NAME_ENG
		_T("LOSS CODE"),	// TSH_LOSSCODE
		_T("상태 색상"),	// TSH_STATE_COLOR
		_T("사용 여부"),	// TSH_USE
		_T("설명"),			// TSH_EXPLANATION
	},
	// 중국어
	{
		_T("설비상태"),		// EID_EquipmentState
		_T("상태명(K)"),	// TSH_EQUIPMENT_STATE_NAME_KOR
		_T("상태명(E)"),	// TSH_EQUIPMENT_STATE_NAME_ENG
		_T("LOSS CODE"),	// TSH_LOSSCODE
		_T("상태 색상"),	// TSH_STATE_COLOR
		_T("사용 여부"),	// TSH_USE
		_T("설명"),			// TSH_EXPLANATION
	},
};

static LPCTSTR g_szCtrlText_T[Lang_MaxCount][CDlg_ModifyEES::Txt_MaxCount] =
{
	// 한국어
	{
		_T("설비 데이터 수정"),		// Txt_DialogCaption
		_T("적용"),					// Txt_OK
		_T("취소"),					// Txt_Cancel
	},
	// 영어
	{
		_T("Modify Equipment Data"),// Txt_DialogCaption
		_T("Apply"),				// Txt_OK
		_T("Cancel"),				// Txt_Cancel
	},
	// 베트남어
	{
		_T("Modify Equipment Data"),// Txt_DialogCaption
		_T("Apply"),				// Txt_OK
		_T("Cancel"),				// Txt_Cancel
	},
	// 중국어
	{
		_T("修改设备数据"),			// Txt_DialogCaption
		_T("适用"),					// Txt_OK
		_T("撤消"),					// Txt_Cancel
	},
};
//=============================================================================
// CDlg_ModifyEES 대화 상자입니다.
//=============================================================================
IMPLEMENT_DYNAMIC(CDlg_ModifyEES, CDialogEx)
//=============================================================================
// 생성자
//=============================================================================
CDlg_ModifyEES::CDlg_ModifyEES(CWnd* pParent /*=nullptr*/)
	: CDialogEx(CDlg_ModifyEES::IDD, pParent)
{
	VERIFY(m_font_Default.CreateFont(
		12,						// nHeight
		0,						// nWidth
		0,						// nEscapement
		0,						// nOrientation
		FW_BOLD,				// nWeight
		FALSE,					// bItalic
		FALSE,					// bUnderline
		0,						// cStrikeOut
		ANSI_CHARSET,			// nCharSet
		OUT_DEFAULT_PRECIS,		// nOutPrecision
		CLIP_DEFAULT_PRECIS,	// nClipPrecision
		ANTIALIASED_QUALITY,	// nQuality
		DEFAULT_PITCH,			// nPitchAndFamily
		_T("Arial")));			// lpszFacename

	VERIFY(m_Font.CreateFont(
		16,						// nHeight
		0,						// nWidth
		0,						// nEscapement
		0,						// nOrientation
		FW_BOLD,				// nWeight
		FALSE,					// bItalic
		FALSE,					// bUnderline
		0,						// cStrikeOut
		ANSI_CHARSET,			// nCharSet
		OUT_DEFAULT_PRECIS,		// nOutPrecision
		CLIP_DEFAULT_PRECIS,	// nClipPrecision
		ANTIALIASED_QUALITY,	// nQuality
		DEFAULT_PITCH,			// nPitchAndFamily
		_T("Arial")));			// lpszFacename
}
//=============================================================================
// 소멸자
//=============================================================================
CDlg_ModifyEES::~CDlg_ModifyEES()
{
	m_font_Default.DeleteObject();
	m_Font.DeleteObject();
}
void CDlg_ModifyEES::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

}
BEGIN_MESSAGE_MAP(CDlg_ModifyEES, CDialogEx)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDOK, &CDlg_ModifyEES::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlg_ModifySV 메시지 처리기


int CDlg_ModifyEES::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	DWORD dwStyle = WS_VISIBLE | WS_CHILD;
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	m_bn_OK.Create(g_szCtrlText_T[m_nLanguage][Txt_OK], dwStyle | WS_TABSTOP | BS_PUSHBUTTON, rectDummy, this, IDOK);
	m_bn_Cancel.Create(g_szCtrlText_T[m_nLanguage][Txt_Cancel], dwStyle | WS_TABSTOP | BS_PUSHBUTTON, rectDummy, this, IDCANCEL);

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
	m_ed_StateColor.SetWindowText(_T("255.0.0"));
	m_cb_Use.Create(dwStyle | CBS_DROPDOWNLIST | WS_VSCROLL, rectDummy, this, IDC_CB_USE);
	m_cb_Use.SetFont(&m_Font);
	for (auto nIdx = 0; nIdx < enEES_USE::EES_USE_MAX; nIdx++)
	{
		m_cb_Use.AddString(g_szEES_USE[nIdx]);
	}
	m_ed_Explanation.Create(dwStyle | ES_CENTER | WS_BORDER, rectDummy, this, IDC_ED_EXPLANATION);
	m_ed_Explanation.SetFont(&m_Font);

	// 설비 항목
	for (UINT nIdx = 0; nIdx < CWnd_Cfg_EES::EID_MaxEnum; nIdx++)
	{
		m_st_ItemData[nIdx].SetStaticStyle(CVGStatic::StaticStyle_Default);
		m_st_ItemData[nIdx].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_st_ItemData[nIdx].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_st_ItemData[nIdx].SetFont_Gdip(L"Tahoma", 10.0F, Gdiplus::FontStyleRegular);

		m_st_ItemData[nIdx].Create(g_szItemText_T[m_nLanguage][nIdx], dwStyle | SS_CENTER | SS_CENTERIMAGE, rectDummy, this, IDC_STATIC);
	}
	return 0;
}
void CDlg_ModifyEES::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if ((cx == 0) && (cy == 0))
		return;

	int iSpacing = 5;
	int iMagin = 20;
	int iLeft = iMagin;
	int iTop = iMagin;
	int nWidth = cx - iMagin - iMagin;
	int nHeight = cy - iMagin - iMagin;
	int	iStWidth = __min(200, ((nWidth - 20) / 3));
	int iHalfWidth = __min(415, (nWidth - iStWidth - iSpacing)); // 3
	int iQuadWidth = (iHalfWidth - (iSpacing * 2)) / 3;	// 1
	int iCtrlWidth = iHalfWidth - iQuadWidth - iSpacing;	// 2
	int iDescWidth = __max(100, __min(160, (iHalfWidth - iSpacing) / 2));
	int iEdWidth = iHalfWidth - iDescWidth - iSpacing;
	int iCtrlHeight = (295 < nHeight) ? 25 : static_cast<int>(round((nHeight - 45) / 10));
	int iLeft_Sub = 0;

	// 설비 데이터 항목 조건
	iLeft_Sub = iLeft + iStWidth + iSpacing;
	//---------------------------------------------------------------
	// 설비 리스트
	//---------------------------------------------------------------
	// ID Type
	m_st_ItemData[CWnd_Cfg_EES::EID_EquipmentState].MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
	m_ed_EquipmentState.MoveWindow(iLeft_Sub, iTop, iEdWidth, iCtrlHeight);
	// ID
	iTop += iCtrlHeight + iSpacing;
	m_st_ItemData[CWnd_Cfg_EES::EID_EquipmentStateNameKor].MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
	m_ed_EquipmentStateNameKor.MoveWindow(iLeft_Sub, iTop, iEdWidth, iCtrlHeight);
	// ID Name
	iTop += iCtrlHeight + iSpacing;
	m_st_ItemData[CWnd_Cfg_EES::EID_EquipmentStateNameEng].MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
	m_ed_EquipmentStateNameEng.MoveWindow(iLeft_Sub, iTop, iEdWidth, iCtrlHeight);
	// ID Text
	iTop += iCtrlHeight + iSpacing;
	m_st_ItemData[CWnd_Cfg_EES::EID_LossCode].MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
	m_ed_LossCode.MoveWindow(iLeft_Sub, iTop, iEdWidth, iCtrlHeight);
	// SV Type
	iTop += iCtrlHeight + iSpacing;
	m_st_ItemData[CWnd_Cfg_EES::EID_StateColor].MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
	m_ed_StateColor.MoveWindow(iLeft_Sub, iTop, iEdWidth, iCtrlHeight);
	//USE
	iTop += iCtrlHeight + iSpacing;
	m_st_ItemData[CWnd_Cfg_EES::EID_USE].MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
	m_cb_Use.MoveWindow(iLeft_Sub, iTop, iEdWidth, iCtrlHeight);
	// SV
	iTop += iCtrlHeight + iSpacing;
	m_st_ItemData[CWnd_Cfg_EES::EID_Explanation].MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
	m_ed_Explanation.MoveWindow(iLeft_Sub, iTop, iEdWidth, iCtrlHeight);

	// 추가, 제거, 저장
	iCtrlWidth = 160;
	iCtrlHeight = 40;
	iLeft = iMagin;
	iLeft_Sub = cx - iMagin - iCtrlWidth;
	iTop = cy - iMagin - iCtrlHeight;
	m_bn_OK.MoveWindow(iLeft, iTop, iCtrlWidth, iCtrlHeight);
	m_bn_Cancel.MoveWindow(iLeft_Sub, iTop, iCtrlWidth, iCtrlHeight);
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

BOOL CDlg_ModifyEES::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetBackgroundColor(RGB(0xFF, 0xFF, 0xFF));

	Set_EESData_UI(&m_stEES);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BOOL CDlg_ModifyEES::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialogEx::PreTranslateMessage(pMsg);
}
BOOL CDlg_ModifyEES::Get_EESData_UI(__out CConfig_EES& OUT_stData)
{
	CString szValue;
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
	return TRUE;
}
BOOL CDlg_ModifyEES::Set_EESData_UI(__in CConfig_EES* IN_pstData)
{
	CString szText;
	m_ed_EquipmentState.SetWindowText(IN_pstData->Get_EquipMentStateCode());
	m_ed_EquipmentStateNameKor.SetWindowText(IN_pstData->Get_EquipMentStateKorText());
	m_ed_EquipmentStateNameEng.SetWindowText(IN_pstData->Get_EquipMentStateEngText());
	m_ed_LossCode.SetWindowText(IN_pstData->Get_LossCode());	
	m_ed_StateColor.SetWindowText(IN_pstData->Get_StateColor());
	m_cb_Use.SetCurSel(IN_pstData->Get_Use());
	m_ed_Explanation.SetWindowText(IN_pstData->Get_Explanation());

	return TRUE;
}
void CDlg_ModifyEES::OnLanguage(__in uint8_t IN_nLanguage)
{
	if (m_nLanguage != IN_nLanguage)
	{
		m_nLanguage = IN_nLanguage;

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
			SetWindowText(g_szCtrlText_T[m_nLanguage][Txt_DialogCaption]);
			m_bn_OK.SetWindowText(g_szCtrlText_T[m_nLanguage][Txt_OK]);
			m_bn_Cancel.SetWindowText(g_szCtrlText_T[m_nLanguage][Txt_Cancel]);

			for (UINT nIdx = 0; nIdx < CWnd_Cfg_EES::EID_MaxEnum; nIdx++)
			{
				m_st_ItemData[nIdx].SetText(g_szItemText_T[m_nLanguage][nIdx]);
			}
		}
	}
}

void CDlg_ModifyEES::Set_EESData(__in const CConfig_EES* IN_pstData)
{
	m_stEES = *IN_pstData;
}

//=============================================================================
// Method		: Get_EquipmentData
// Access		: public  
// Returns		: CConfig_Eqp&
// Qualifier	:
// Last Update	: 2021/11/10 - 14:27
// Desc.		:
//=============================================================================
CConfig_EES& CDlg_ModifyEES::Get_EESData()
{
	return m_stEES;
}

void CDlg_ModifyEES::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Get_EESData_UI(m_stEES);
	CDialogEx::OnOK();
}
