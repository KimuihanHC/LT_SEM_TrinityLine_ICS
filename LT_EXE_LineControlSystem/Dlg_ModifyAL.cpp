// Dlg_ModifySV.cpp: 구현 파일
//

#include "stdafx.h"
#include "resource.h"
#include "Dlg_ModifyAL.h"
#include "afxdialogex.h"
#include "Def_Language_Message.h"



// CDlg_ModifySV 대화 상자

#define IDC_ED_ALID				3000
#define IDC_CB_ALCD				3001
#define IDC_ED_ALTX				3002


IMPLEMENT_DYNAMIC(CDlg_ModifyAL, CDialogEx)

static LPCTSTR g_szItemText_T[Lang_MaxCount][CWnd_Cfg_ALID_List::EID_MaxEnum] =
{
	// 한국어
	{
		_T("ID"),	// EID_Item_Para_1
		_T("CD"),	// EID_Item_Para_2
		_T("TX"),	// EID_Item_Para_3
	},

	// 영어
	{
		_T("ID"),	// EID_Item_Para_1
		_T("CD"),	// EID_Item_Para_2
		_T("TX"),	// EID_Item_Para_3
	},

	// 베트남어
	{
		_T("ID"),	// EID_Item_Para_1
		_T("CD"),	// EID_Item_Para_2
		_T("TX"),	// EID_Item_Para_3
	},

	// 중국어
	{
		_T("ID"),	// EID_Item_Para_1
		_T("CD"),	// EID_Item_Para_2
		_T("TX"),	// EID_Item_Para_3
	},
};

static LPCTSTR g_szCtrlText_T[Lang_MaxCount][CDlg_ModifyAL::Txt_MaxCount] =
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
CDlg_ModifyAL::CDlg_ModifyAL(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_MODIFY_AL, pParent)
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

CDlg_ModifyAL::~CDlg_ModifyAL()
{
	m_font_Default.DeleteObject();
	m_Font.DeleteObject();
}

void CDlg_ModifyAL::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_bn_OK);
	DDX_Control(pDX, IDCANCEL, m_bn_Cancel);
	DDX_Control(pDX, IDC_ED_ALID, m_ed_nALID);
	DDX_Control(pDX, IDC_CB_ALCD, m_cb_nALCD);
	DDX_Control(pDX, IDC_ED_ALTX, m_ed_cstALTX);
}


BEGIN_MESSAGE_MAP(CDlg_ModifyAL, CDialogEx)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDOK, &CDlg_ModifyAL::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlg_ModifySV 메시지 처리기


int CDlg_ModifyAL::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	DWORD dwStyle = WS_VISIBLE | WS_CHILD;
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	m_ed_nALID.Create(dwStyle | ES_CENTER | WS_BORDER, rectDummy, this, IDC_ED_ALID);
	m_ed_nALID.EnableWindow(FALSE);

	m_cb_nALCD.Create(dwStyle | CBS_DROPDOWNLIST | WS_VSCROLL, rectDummy, this, IDC_CB_ALCD);
	m_cb_nALCD.SetFont(&m_Font);
	for (auto nIdx = 0; nIdx < enALCD::ALCD_Max; nIdx++)
	{
		m_cb_nALCD.AddString(g_szALCD[nIdx]);
	}
	m_cb_nALCD.SetCurSel(0);

	m_ed_cstALTX.Create(dwStyle | ES_CENTER | WS_BORDER, rectDummy, this, IDC_ED_ALTX);
	m_ed_cstALTX.SetFont(&m_font_Default);

	// 설비 항목
	for (UINT nIdx = 0; nIdx < CWnd_Cfg_ALID_List::EID_MaxEnum; nIdx++)
	{
		m_st_SVItemData[nIdx].SetStaticStyle(CVGStatic::StaticStyle_Default);
		m_st_SVItemData[nIdx].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_st_SVItemData[nIdx].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_st_SVItemData[nIdx].SetFont_Gdip(L"Tahoma", 10.0F, Gdiplus::FontStyleRegular);

		m_st_SVItemData[nIdx].Create(g_szItemText_T[m_nLanguage][nIdx], dwStyle | SS_CENTER | SS_CENTERIMAGE, rectDummy, this, IDC_STATIC);
	}

	return 0;
}


void CDlg_ModifyAL::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if ((cx == 0) && (cy == 0))
		return;

	int iMargin = 10;
	int iSpacing = 5;
	int iCateSpacing = 10;
	int iLeft = iMargin;
	int iTop = iMargin;
	int iWidth = cx - iMargin - iMargin;
	int iHeight = cy - iMargin - iMargin;
	int iCtrlHeight = 25;
	int iCtrlWidth = iWidth;

	int iStWidth = 100;
	int iEdWidth = iCtrlWidth - iStWidth - iSpacing;
	int iLeft_Sub = iLeft + iStWidth + iSpacing;
	int iListWidth = 800;
	int iListHeight = iHeight - ((iCtrlHeight + iSpacing) * 2);

	//---------------------------------------------------------------
	// 설비 리스트
	//---------------------------------------------------------------

	//iLeft = iMargin + iCtrlWidth + iSpacing + iListWidth - 250;
	//m_bn_Apply.MoveWindow(iLeft, iTop, 250, iCtrlHeight);

	iLeft = iMargin;
	iTop += iSpacing;
	// ID Type
	m_st_SVItemData[CWnd_Cfg_ALID_List::EID_ALID].MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
	m_ed_nALID.MoveWindow(iLeft_Sub, iTop, iEdWidth, iCtrlHeight);

	// ID
	iLeft = iMargin;
	iTop += iCtrlHeight + iSpacing;
	m_st_SVItemData[CWnd_Cfg_ALID_List::EID_ALCD].MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
	m_cb_nALCD.MoveWindow(iLeft_Sub, iTop, iEdWidth, iCtrlHeight);

	// ID Name
	iTop += iCtrlHeight + iSpacing;
	m_st_SVItemData[CWnd_Cfg_ALID_List::EID_ALTX].MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
	m_ed_cstALTX.MoveWindow(iLeft_Sub, iTop, iEdWidth, iCtrlHeight);


	/*
	// 추가, 제거, 저장
	iTop += iCtrlHeight + 30;
	int iQuadSt_W = (iCtrlWidth - iSpacing) / 2;
	m_bn_ItemCtrl[CWnd_Cfg_SVID_List::ID_Add].MoveWindow(iLeft, iTop, iQuadSt_W, iCtrlHeight);
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
	m_lc_SVInfo.MoveWindow(iLeft, iTop, iListWidth, iListHeight);


	iTop += iListHeight + iSpacing;
	iCtrlWidth = __min(160, ((iListWidth - (iSpacing * 4)) / 5));
	for (auto nIdx = 0; nIdx < XML_MaxCount; nIdx++)
	{
		m_bn_XmlCtrl[nIdx].MoveWindow(iLeft, iTop, iCtrlWidth, iCtrlHeight);
		iLeft += iCtrlWidth + iSpacing;
	}
	*/
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

BOOL CDlg_ModifyAL::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	//Set_SVData(&m_stSV);	//2023.02.28a uhkim
	Set_ALData_UI(&m_stSV);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BOOL CDlg_ModifyAL::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialogEx::PreTranslateMessage(pMsg);
}
BOOL CDlg_ModifyAL::Get_ALData_UI(__out CConfig_AL& OUT_stSV)
{
	CString szValue;	
	auto iSel_CD = m_cb_nALCD.GetCurSel();
	m_ed_nALID.GetWindowText(szValue);
	auto iID = _ttoi(szValue);
	if (0 > iID)
	{
		return FALSE;
	}
	if (0 > iSel_CD)
	{
		return FALSE;
	}


	// ---- SID
	OUT_stSV.Set_ALID(iID);
	OUT_stSV.Set_ALCD(iSel_CD);
	// ---- SID	Name
	m_ed_cstALTX.GetWindowText(szValue);
	OUT_stSV.Set_ALTX(szValue);
	// Text.


	return TRUE;
}
BOOL CDlg_ModifyAL::Set_ALData_UI(__in CConfig_AL* IN_pstSV)
{
	CString szValue;
	m_cb_nALCD.SetCurSel(IN_pstSV->Get_ALCD());
	szValue.Format(_T("%d"), IN_pstSV->Get_ALID());
	m_ed_nALID.SetWindowText(szValue);
	m_ed_cstALTX.SetWindowText(IN_pstSV->Get_ALTX().GetBuffer());
	return TRUE;
}


void CDlg_ModifyAL::OnLanguage(__in uint8_t IN_nLanguage)
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

		m_cb_nALCD.SetFont(&m_Font);

		/*
		if (GetSafeHwnd())
		{
			SetWindowText(g_szCtrlText_T[m_nLanguage][Txt_DialogCaption]);
			m_bn_OK.SetWindowText(g_szCtrlText_T[m_nLanguage][Txt_OK]);
			m_bn_Cancel.SetWindowText(g_szCtrlText_T[m_nLanguage][Txt_Cancel]);

			for (UINT nIdx = 0; nIdx < EID_MaxEnum; nIdx++)
			{
				m_st_EqpItemData[nIdx].SetText(g_szItemText_T[m_nLanguage][nIdx]);
			}
		}*/
	}
}

void CDlg_ModifyAL::Set_ALData(__in const CConfig_AL* IN_pstSV)
{
	m_stSV = *IN_pstSV;
}

//=============================================================================
// Method		: Get_EquipmentData
// Access		: public  
// Returns		: CConfig_Eqp&
// Qualifier	:
// Last Update	: 2021/11/10 - 14:27
// Desc.		:
//=============================================================================
CConfig_AL& CDlg_ModifyAL::Get_SVData()
{
	return m_stSV;
}

void CDlg_ModifyAL::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Get_ALData_UI(m_stSV);
	CDialogEx::OnOK();
}
