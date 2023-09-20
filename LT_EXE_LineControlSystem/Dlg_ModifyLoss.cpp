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
#include "Dlg_ModifyLoss.h"
#include "afxdialogex.h"
#include "Def_Language_Message.h"

// CDlg_ModifyEES 대화 상자
#define IDC_ED_NAME_LOSS_TEM				3000
#define IDC_ED_TEM							3001
#define IDC_ED_CODELOSS						3002
#define IDC_ED_NAME_LOSS_TEM_ENG			3003
#define IDC_ED_EXPLAIN						3004

static LPCTSTR g_szItemText_T[Lang_MaxCount][CWnd_Cfg_Loss::EID_MaxEnum] =
{
	// 한국어
	{
		_T("로스명칭"),		// TSH_NO
		_T("템"),		// TSH_EQUIPMENT_STATE
		_T("코드"),		// TSH_EQUIPMENT_STATE_NAME_KOR
		_T("로스명칭(E)"),	// TSH_EQUIPMENT_STATE_NAME_ENG
		_T("설명"),			// TSH_EXPLANATION
	},
	// 영어
	{
		_T("로스명칭"),		// TSH_NO
		_T("템"),		// TSH_EQUIPMENT_STATE
		_T("코드"),		// TSH_EQUIPMENT_STATE_NAME_KOR
		_T("로스명칭(E)"),	// TSH_EQUIPMENT_STATE_NAME_ENG
		_T("설명"),			// TSH_EXPLANATION
	},
	// 베트남어
	{
		_T("로스명칭"),		// TSH_NO
		_T("템"),		// TSH_EQUIPMENT_STATE
		_T("코드"),		// TSH_EQUIPMENT_STATE_NAME_KOR
		_T("로스명칭(E)"),	// TSH_EQUIPMENT_STATE_NAME_ENG
		_T("설명"),			// TSH_EXPLANATION
	},
	// 중국어
	{
		_T("로스명칭"),		// TSH_NO
		_T("템"),		// TSH_EQUIPMENT_STATE
		_T("코드"),		// TSH_EQUIPMENT_STATE_NAME_KOR
		_T("로스명칭(E)"),	// TSH_EQUIPMENT_STATE_NAME_ENG
		_T("설명"),			// TSH_EXPLANATION
	},
};

static LPCTSTR g_szCtrlText_T[Lang_MaxCount][CWnd_Cfg_Loss::XML_MaxCount] =
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
IMPLEMENT_DYNAMIC(CDlg_ModifyLoss, CDialogEx)
//=============================================================================
// 생성자
//=============================================================================
CDlg_ModifyLoss::CDlg_ModifyLoss(CWnd* pParent /*=nullptr*/)
	: CDialogEx(CDlg_ModifyLoss::IDD, pParent)
{
	VERIFY(m_font_Default.CreateFont(
		11,						// nHeight
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
CDlg_ModifyLoss::~CDlg_ModifyLoss()
{
	m_font_Default.DeleteObject();
	m_Font.DeleteObject();
}
void CDlg_ModifyLoss::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

}
BEGIN_MESSAGE_MAP(CDlg_ModifyLoss, CDialogEx)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDOK, &CDlg_ModifyLoss::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlg_ModifySV 메시지 처리기


int CDlg_ModifyLoss::OnCreate(LPCREATESTRUCT lpCreateStruct)
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
	for (UINT nIdx = 0; nIdx < CWnd_Cfg_Loss::EID_MaxEnum; nIdx++)
	{
		m_st_ItemData[nIdx].SetStaticStyle(CVGStatic::StaticStyle_Default);
		m_st_ItemData[nIdx].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_st_ItemData[nIdx].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_st_ItemData[nIdx].SetFont_Gdip(L"Tahoma", 10.0F, Gdiplus::FontStyleRegular);

		m_st_ItemData[nIdx].Create(g_szItemText_T[m_nLanguage][nIdx], dwStyle | SS_CENTER | SS_CENTERIMAGE, rectDummy, this, IDC_STATIC);
	}
	return 0;
}


void CDlg_ModifyLoss::OnSize(UINT nType, int cx, int cy)
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
	m_st_ItemData[CWnd_Cfg_Loss::EID_NameLoss].MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
	m_ed_NameLossTem.MoveWindow(iLeft_Sub, iTop, iEdWidth, iCtrlHeight);
	// ID
	iTop += iCtrlHeight + iSpacing;
	m_st_ItemData[CWnd_Cfg_Loss::EID_Tem].MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
	m_ed_Tem.MoveWindow(iLeft_Sub, iTop, iEdWidth, iCtrlHeight);
	// ID Name
	iTop += iCtrlHeight + iSpacing;
	m_st_ItemData[CWnd_Cfg_Loss::EID_CodeLoss].MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
	m_ed_CodeLoss.MoveWindow(iLeft_Sub, iTop, iEdWidth, iCtrlHeight);
	// ID Text
	iTop += iCtrlHeight + iSpacing;
	m_st_ItemData[CWnd_Cfg_Loss::EID_NameLossEng].MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
	m_ed_NameLossTemEng.MoveWindow(iLeft_Sub, iTop, iEdWidth, iCtrlHeight);
	// SV Type
	iTop += iCtrlHeight + iSpacing;
	m_st_ItemData[CWnd_Cfg_Loss::EID_ExplainLoss].MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
	m_ed_Explan.MoveWindow(iLeft_Sub, iTop, iHalfWidth, iCtrlHeight);
	//USE

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

BOOL CDlg_ModifyLoss::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetBackgroundColor(RGB(0xFF, 0xFF, 0xFF));

	Set_LossData_UI(&m_stLoss);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BOOL CDlg_ModifyLoss::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialogEx::PreTranslateMessage(pMsg);
}
BOOL CDlg_ModifyLoss::Get_LossData_UI(__out CConfig_Loss& OUT_stData)
{
	CString szValue;
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

	return TRUE;
}

BOOL CDlg_ModifyLoss::Set_LossData_UI(__in CConfig_Loss* IN_pstData)
{
	CString szText;
	m_ed_NameLossTem.SetWindowText(IN_pstData->Get_NameLoss());
	m_ed_Tem.SetWindowText(IN_pstData->Get_Tem());
	m_ed_CodeLoss.SetWindowText(IN_pstData->Get_CodeLoss());
	m_ed_NameLossTemEng.SetWindowText(IN_pstData->Get_NameLossEng());
	m_ed_Explan.SetWindowText(IN_pstData->Get_Explain());

	return TRUE;
}
void CDlg_ModifyLoss::OnLanguage(__in uint8_t IN_nLanguage)
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

			for (UINT nIdx = 0; nIdx < CWnd_Cfg_Loss::EID_MaxEnum; nIdx++)
			{
				m_st_ItemData[nIdx].SetText(g_szItemText_T[m_nLanguage][nIdx]);
			}
		}
	}
}

void CDlg_ModifyLoss::Set_LossData(__in const CConfig_Loss* IN_pstData)
{
	m_stLoss = *IN_pstData;
}

//=============================================================================
// Method		: Get_EquipmentData
// Access		: public  
// Returns		: CConfig_Eqp&
// Qualifier	:
// Last Update	: 2021/11/10 - 14:27
// Desc.		:
//=============================================================================
CConfig_Loss& CDlg_ModifyLoss::Get_LossData()
{
	return m_stLoss;
}

void CDlg_ModifyLoss::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Get_LossData_UI(m_stLoss);
	CDialogEx::OnOK();
}
