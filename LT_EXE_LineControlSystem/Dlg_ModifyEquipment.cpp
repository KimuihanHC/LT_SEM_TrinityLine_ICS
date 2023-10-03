//*****************************************************************************
// Filename	: 	Dlg_ModifyEquipment.cpp
// Created	:	2021/10/20 - 12:09
// Modified	:	2021/10/20 - 12:09
//
// Author	:	piring
//	
// Purpose	:	
//*****************************************************************************
// Dlg_ModifyEquipment.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Dlg_ModifyEquipment.h"
#include "Registry.h"
#include "resource.h"
#include "CommonFunction.h"
#include "Def_Equipment_Type.h"
#include "Def_Language_Message.h"


#define IDC_CB_EQP_TYPE			1201
#define IDC_BN_VERIFY_LINE		1202
#define IDC_BN_MODIFY_STEP		1203
#define IDC_ED_DATA_S			1210
#define IDC_ED_DATA_E			IDC_ED_DATA_S + MAX_STEP_DATA - 1

#define IDC_ED_EQUIPID_S		4000
#define IDC_ED_EQUIPID_E		IDC_ED_EQUIPID_S + 99
#define IDC_ED_PORTID_S			IDC_ED_EQUIPID_E + 1
#define IDC_ED_PORTID_E			IDC_ED_PORTID_S + 99

#define IDC_ED_EQP_ALIAS		3000
#define IDC_ED_EQP_ID			3001
#define IDC_IP_ADDRESS			3002
#define IDC_ED_SUBEQPID			3003
// #define IDC_CB_TESTZONE_CNT		3003
// #define IDC_CB_BUFF_CNT			3004
// #define IDC_CB_CONVOYER_CNT		3005
// #define IDC_CB_RETURN_CNT		3006


static LPCTSTR g_szItemText_T[Lang_MaxCount][CDlg_ModifyEquipment::EID_MaxEnum] =
{
	// 한국어
	{
		_T("설비 별칭"),				// EID_Item
		_T("설비 종류"),				// EID_Item_Para_1
		_T("설비 id"),				// EID_Item_Para_2
		_T("IP 주소"),				// EID_Item_Para_3
		_T("검사 영역 개수"),			// EID_Item_Para_4
		_T("버퍼 개수"),				// EID_Item_Para_5
		_T("컨베이어 개수"),			// EID_Item_Para_6
		_T("리턴 컨베이어 개수"),		// EID_Item_Para_7
	},

	// 영어
	{
		_T("Equipment Alias"),			// EID_Item
		_T("Equipment Type"),			// EID_Item_Para_1
		_T("Equipment id"),				// EID_Item_Para_2
		_T("IP Address"),				// EID_Item_Para_3
		_T("Test Zone Count"),			// EID_Item_Para_4
		_T("Buffer Count"),				// EID_Item_Para_5
		_T("Conveyor Count"),			// EID_Item_Para_6
		_T("Return Conveyor Count"),	// EID_Item_Para_7
	},

	// 베트남어
	{
		_T("Equipment Alias"),			// EID_Item
		_T("Equipment Type"),			// EID_Item_Para_1
		_T("Equipment id"),				// EID_Item_Para_2
		_T("IP Address"),				// EID_Item_Para_3
		_T("Test Zone Count"),			// EID_Item_Para_4
		_T("Buffer Count"),				// EID_Item_Para_5
		_T("Conveyor Count"),			// EID_Item_Para_6
		_T("Return Conveyor Count"),	// EID_Item_Para_7
	},

	// 중국어
	{
		_T("设备别称"),					// EID_Item
		_T("设备类型"),					// EID_Item_Para_1
		_T("设备ID"),					// EID_Item_Para_2
		_T("IP地址"),					// EID_Item_Para_3
		_T("检查区数目"),				// EID_Item_Para_4
		_T("缓冲区数量"),				// EID_Item_Para_5
		_T("输送机数量"),				// EID_Item_Para_6
		_T("回程输送机数量"),				// EID_Item_Para_7
	},
};

static LPCTSTR g_szCtrlText_T[Lang_MaxCount][CDlg_ModifyEquipment::Txt_MaxCount] =
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
// CDlg_ModifyEquipment 대화 상자입니다.
//=============================================================================
IMPLEMENT_DYNAMIC(CDlg_ModifyEquipment, CDialogEx)

//=============================================================================
// 생성자
//=============================================================================
CDlg_ModifyEquipment::CDlg_ModifyEquipment(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlg_ModifyEquipment::IDD, pParent)
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
CDlg_ModifyEquipment::~CDlg_ModifyEquipment()
{
	m_font_Default.DeleteObject();
	m_Font.DeleteObject();
}

void CDlg_ModifyEquipment::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlg_ModifyEquipment, CDialogEx)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_CBN_SELENDOK	(IDC_CB_EQP_TYPE,		OnCbnSelendokEqpType)
END_MESSAGE_MAP()

//=============================================================================
// CDlg_ModifyEquipment 메시지 처리기입니다.
//=============================================================================
//=============================================================================
// Method		: OnCreate
// Access		: protected  
// Returns		: int
// Parameter	: LPCREATESTRUCT lpCreateStruct
// Qualifier	:
// Last Update	: 2021/11/10 - 14:25
// Desc.		:
//=============================================================================
int CDlg_ModifyEquipment::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	DWORD dwStyle = WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	SetWindowText(g_szCtrlText_T[m_nLanguage][Txt_DialogCaption]);


	m_bn_OK.Create(g_szCtrlText_T[m_nLanguage][Txt_OK], dwStyle | WS_TABSTOP | BS_PUSHBUTTON, rectDummy, this, IDOK);
	m_bn_Cancel.Create(g_szCtrlText_T[m_nLanguage][Txt_Cancel], dwStyle | WS_TABSTOP | BS_PUSHBUTTON, rectDummy, this, IDCANCEL);


	//---------------------------------------------------------------
	// 설비 데이터 항목
	//---------------------------------------------------------------
	m_ed_EqpAlias.Create(dwStyle | ES_CENTER | WS_BORDER, rectDummy, this, IDC_ED_EQP_ALIAS);
	m_ed_EqpAlias.EnableWindow(FALSE);

	m_cb_EquipmentType.Create(dwStyle | CBS_DROPDOWNLIST | WS_VSCROLL, rectDummy, this, IDC_CB_EQP_TYPE);
	m_cb_EquipmentType.SetFont(&m_Font);
// 	for (auto nIdx = 0; nIdx < enEquipmentType::Max_EqpTypeCount; nIdx++)
// 	{
// 		m_cb_EquipmentType.AddString(g_szEqpTypeName[nIdx]);
// 	}
	for (auto nIdx = 0; nIdx < enEquipmentType_UI::Max_EqpTypeUICount; nIdx++)
	{
		m_cb_EquipmentType.AddString(g_szEqpTypeName_UI[nIdx]);
	}
	m_cb_EquipmentType.SetCurSel(0);

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

	// Step 항목 조건
	for (UINT nIdx = 0; nIdx < EID_MaxEnum; nIdx++)
	{
		m_st_EqpItemData[nIdx].SetStaticStyle(CVGStatic::StaticStyle_Default);
		m_st_EqpItemData[nIdx].SetColorStyle(CVGStatic::ColorStyle_DarkGray);
		m_st_EqpItemData[nIdx].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_st_EqpItemData[nIdx].SetFont_Gdip(L"Tahoma", 9.0F, Gdiplus::FontStyleRegular);

		m_st_EqpItemData[nIdx].Create(g_szItemText_T[m_nLanguage][nIdx], dwStyle | SS_CENTER | SS_CENTERIMAGE, rectDummy, this, IDC_STATIC);

		//m_st_EqpItemData[nIdx].ModifyStyleEx(WS_EX_CLIENTEDGE | WS_EX_CLIENTEDGE, NULL, SWP_FRAMECHANGED);
		//m_st_EqpItemData[nIdx].ModifyStyle(NULL, WS_BORDER, SWP_FRAMECHANGED);
	}

	//2023.05.26
	m_st_SubEqpID.SetStaticStyle(CVGStatic::StaticStyle_Default);
	m_st_SubEqpID.SetColorStyle(CVGStatic::ColorStyle_DarkGray);
	m_st_SubEqpID.SetColorStyle(CVGStatic::ColorStyle_Black);
	m_st_SubEqpID.SetFont_Gdip(L"Tahoma", 9.0F, Gdiplus::FontStyleRegular);
	m_st_SubEqpID.Create(_T("SUBEQPID"), dwStyle | SS_CENTER | SS_CENTERIMAGE, rectDummy, this, IDC_STATIC);
	m_ed_SubEqpID.Create(dwStyle | ES_CENTER | WS_BORDER, rectDummy, this, IDC_ED_SUBEQPID);
	CString str;
	for (auto nIdx = 0; nIdx < 100; nIdx++)
	{
		str.Format(_T("EquipID %02d"), nIdx);
		m_ed_EquipID[nIdx].Create(dwStyle | ES_CENTER | WS_BORDER, rectDummy, this, IDC_ED_EQUIPID_S + nIdx);
		m_st_EquipID[nIdx].Create(str, dwStyle | SS_CENTER | SS_CENTERIMAGE, rectDummy, this, IDC_STATIC);
		m_st_EquipID[nIdx].MoveWindow(0, 0, 0, 0);
		
	}

	for (auto nIdx = 0; nIdx < 100; nIdx++)
	{
		str.Format(_T("PortID %02d"), nIdx);
		m_ed_PortID[nIdx].Create(dwStyle | ES_CENTER | WS_BORDER, rectDummy, this, IDC_ED_PORTID_S + nIdx);
		m_st_PortID[nIdx].Create(str, dwStyle | SS_CENTER | SS_CENTERIMAGE, rectDummy, this, IDC_STATIC);
		m_st_PortID[nIdx].MoveWindow(0, 0, 0, 0);
	}
	//
	return 0;
}

//=============================================================================
// Method		: OnSize
// Access		: protected  
// Returns		: void
// Parameter	: UINT nType
// Parameter	: int cx
// Parameter	: int cy
// Qualifier	:
// Last Update	: 2021/11/10 - 14:25
// Desc.		:
//=============================================================================
void CDlg_ModifyEquipment::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if ((0 == cx) || (0 == cy))
		return;

	int iSpacing	= 5;
	int iMagin		= 20;
	int iLeft		= iMagin;
	int iTop		= iMagin;
	int nWidth		= cx - iMagin - iMagin;
	int nHeight		= cy - iMagin - iMagin;
	int	iStWidth	= __min(200, ((nWidth - 20) / 3));
	int iHalfWidth	= __min(415, (nWidth - iStWidth - iSpacing)); // 3
	int iQuadWidth	= (iHalfWidth - (iSpacing * 2)) / 3;	// 1
	int iCtrlWidth	= iHalfWidth - iQuadWidth - iSpacing;	// 2
	int iDescWidth	= __max(100, __min(160, (iHalfWidth - iSpacing) / 2 ));
	int iEdWidth	= iHalfWidth - iDescWidth - iSpacing;
	int iCtrlHeight	= (295 < nHeight) ? 25 : static_cast<int>(round((nHeight - 45) / 10));
	int iLeftSub	= 0;

	// 설비 데이터 항목 조건
	iLeftSub = iLeft + iStWidth + iSpacing;

// 	iTop += iCtrlHeight + iSpacing;
// 	m_st_EqpItemData[EID_Eqp_Alias].MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
// 	m_ed_EqpAlias.MoveWindow(iLeftSub, iTop, iHalfWidth, iCtrlHeight);

	iTop += iCtrlHeight + iSpacing;
	m_st_EqpItemData[EID_Eqp_Type].MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
	m_cb_EquipmentType.MoveWindow(iLeftSub, iTop, iHalfWidth, iCtrlHeight);

	iTop += iCtrlHeight + iSpacing;
	m_st_EqpItemData[EID_Eqp_Id].MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
	m_ed_EquipmentId.MoveWindow(iLeftSub, iTop, iHalfWidth, iCtrlHeight);

	iTop += iCtrlHeight + iSpacing;
	m_st_EqpItemData[EID_IP_Address].MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
	m_ed_IPAddress.MoveWindow(iLeftSub, iTop, iHalfWidth, iCtrlHeight);

	// 버튼
	iCtrlWidth = 160;
	iCtrlHeight = 40;
	iLeft = iMagin;
	iLeftSub = cx - iMagin - iCtrlWidth;
	iTop = cy - iMagin - iCtrlHeight;
	m_bn_OK.MoveWindow(iLeft, iTop, iCtrlWidth, iCtrlHeight);
	m_bn_Cancel.MoveWindow(iLeftSub, iTop, iCtrlWidth, iCtrlHeight);

	nEquip_cx = cx;
	nEquip_cy = cy;
}

//=============================================================================
// Method		: OnInitDialog
// Access		: virtual protected  
// Returns		: BOOL
// Qualifier	:
// Last Update	: 2021/11/10 - 14:26
// Desc.		:
//=============================================================================
BOOL CDlg_ModifyEquipment::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetBackgroundColor(RGB(0xFF, 0xFF, 0xFF));

	//UpdateUI_EquipmentType();

	Set_EquipmentData_UI(&m_stEquipment);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

//=============================================================================
// Method		: PreTranslateMessage
// Access		: virtual protected  
// Returns		: BOOL
// Parameter	: MSG * pMsg
// Qualifier	:
// Last Update	: 2021/11/10 - 14:26
// Desc.		:
//=============================================================================
BOOL CDlg_ModifyEquipment::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		OnOK();
		return TRUE;
	}
	else if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		// 여기에 ESC키 기능 작성       
		return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

//=============================================================================
// Method		: OnCancel
// Access		: virtual protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/11/10 - 14:26
// Desc.		:
//=============================================================================
void CDlg_ModifyEquipment::OnCancel()
{
	CDialogEx::OnCancel();
}

//=============================================================================
// Method		: OnOK
// Access		: virtual protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/11/10 - 14:26
// Desc.		:
//=============================================================================
void CDlg_ModifyEquipment::OnOK()
{
	// UI 데이터 저장
	Get_EquipmentData_UI(m_stEquipment);



	CDialogEx::OnOK();
}

//=============================================================================
// Method		: OnCbnSelendokEqpType
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/11/10 - 14:26
// Desc.		:
//=============================================================================
void CDlg_ModifyEquipment::OnCbnSelendokEqpType()
{
	int iSel = m_cb_EquipmentType.GetCurSel();

	if (iSel < 0)
		return;

	UpdateUI_EquipmentData(iSel);
}

//=============================================================================
// Method		: Get_EquipmentData_UI
// Access		: public  
// Returns		: BOOL
// Parameter	: __out CConfig_Eqp & OUT_stEquipment
// Qualifier	:
// Last Update	: 2021/11/10 - 14:26
// Desc.		:
//=============================================================================
BOOL CDlg_ModifyEquipment::Get_EquipmentData_UI(__out CConfig_Eqp& OUT_stEquipment)
{
	CString szValue;
	DWORD dwAddress = 0;

	// ---- Equipment Type
	int iSel = m_cb_EquipmentType.GetCurSel();
	if (0 <= iSel)
	{
		//OUT_stEquipment.Set_EquipmentType(iSel);
		OUT_stEquipment.Set_EqpType_UI(iSel);
	}
	else
	{
		return FALSE;
	}

	// ---- Equipment ID
	m_ed_EquipmentId.GetWindowText(szValue);
	OUT_stEquipment.Set_EquipmentId(szValue);

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
#if SOCKET
	m_ed_SubEqpID.GetWindowText(szValue);
	m_reg.Save_Equipment_ID(OUT_stEquipment.Get_EquipmentId(), 0, 0, szValue);	
	switch (OUT_stEquipment.Get_EqpType_UI())
	{
	case EqpUI_Handler:
		m_ed_EquipID[PtI_L_Load].GetWindowText(szValue);
		m_reg.Save_Equipment_ID(OUT_stEquipment.Get_EquipmentId(), PtI_L_Load, 1, szValue);
		m_ed_PortID[PtI_L_Load].GetWindowText(szValue);
		m_reg.Save_Equipment_ID(OUT_stEquipment.Get_EquipmentId(), PtI_L_Load, 2, szValue);		
		break;
	case EqpUI_Loader:
		m_ed_EquipID[PtI_L_Load].GetWindowText(szValue);
		m_reg.Save_Equipment_ID(OUT_stEquipment.Get_EquipmentId(), PtI_L_Load, 1, szValue);
		m_ed_PortID[PtI_L_Load].GetWindowText(szValue);
		m_reg.Save_Equipment_ID(OUT_stEquipment.Get_EquipmentId(), PtI_L_Load, 2, szValue);
		break;
	case EqpUI_Returner:
		m_ed_EquipID[PtI_R_Buffer_1].GetWindowText(szValue);
		m_reg.Save_Equipment_ID(OUT_stEquipment.Get_EquipmentId(), PtI_R_Buffer_1, 1, szValue);
		m_ed_PortID[PtI_R_Buffer_1].GetWindowText(szValue);
		m_reg.Save_Equipment_ID(OUT_stEquipment.Get_EquipmentId(), PtI_R_Buffer_1, 2, szValue);
		break;
	default:
		auto nCount = OUT_stEquipment.Get_TestPortCount();
		switch (nCount) {
		case 2:
			m_ed_EquipID[PtI_T_Test_L].GetWindowText(szValue);
			m_reg.Save_Equipment_ID(OUT_stEquipment.Get_EquipmentId(), PtI_T_Test_L, 1, szValue);
			m_ed_PortID[PtI_T_Test_L].GetWindowText(szValue);
			m_reg.Save_Equipment_ID(OUT_stEquipment.Get_EquipmentId(), PtI_T_Test_L, 2, szValue);

			m_ed_EquipID[PtI_T_Test_R].GetWindowText(szValue);
			m_reg.Save_Equipment_ID(OUT_stEquipment.Get_EquipmentId(), PtI_T_Test_R, 1, szValue);
			m_ed_PortID[PtI_T_Test_R].GetWindowText(szValue);
			m_reg.Save_Equipment_ID(OUT_stEquipment.Get_EquipmentId(), PtI_T_Test_R, 2, szValue);
			break;
		case 3:
			m_ed_EquipID[PtI_T_Test_L].GetWindowText(szValue);
			m_reg.Save_Equipment_ID(OUT_stEquipment.Get_EquipmentId(), PtI_T_Test_L, 1, szValue);
			m_ed_PortID[PtI_T_Test_L].GetWindowText(szValue);
			m_reg.Save_Equipment_ID(OUT_stEquipment.Get_EquipmentId(), PtI_T_Test_L, 2, szValue);

			m_ed_EquipID[PtI_T_Test_C].GetWindowText(szValue);
			m_reg.Save_Equipment_ID(OUT_stEquipment.Get_EquipmentId(), PtI_T_Test_C, 1, szValue);
			m_ed_PortID[PtI_T_Test_C].GetWindowText(szValue);
			m_reg.Save_Equipment_ID(OUT_stEquipment.Get_EquipmentId(), PtI_T_Test_C, 2, szValue);

			m_ed_EquipID[PtI_T_Test_R].GetWindowText(szValue);
			m_reg.Save_Equipment_ID(OUT_stEquipment.Get_EquipmentId(), PtI_T_Test_R, 1, szValue);
			m_ed_PortID[PtI_T_Test_R].GetWindowText(szValue);
			m_reg.Save_Equipment_ID(OUT_stEquipment.Get_EquipmentId(), PtI_T_Test_R, 2, szValue);
			break;
		}
		break;
	}
#endif //SOCKET
	// 검사 항목에 따라서 데이터 범위 체크 해야 함.
	Verify_Equipment(OUT_stEquipment);

	return TRUE;
}

//=============================================================================
// Method		: Set_EquipmentData_UI
// Access		: public  
// Returns		: BOOL
// Parameter	: __in CConfig_Eqp * IN_pstEquipment
// Qualifier	:
// Last Update	: 2021/11/10 - 14:26
// Desc.		:
//=============================================================================
BOOL CDlg_ModifyEquipment::Set_EquipmentData_UI(__in CConfig_Eqp* IN_pstEquipment)
{
	m_ed_EqpAlias.SetWindowText (IN_pstEquipment->Get_Alias().GetBuffer());
	//m_cb_EquipmentType.SetCurSel (IN_pstEquipment->Get_EquipmentType());
	m_cb_EquipmentType.SetCurSel(IN_pstEquipment->Get_EqpType_UI());
	m_ed_EquipmentId.SetWindowText (IN_pstEquipment->Get_EquipmentId().GetBuffer());

	CString szText;
	szText = ConvIPAddrToString(IN_pstEquipment->Get_IP_Address());
	m_ed_IPAddress.SetWindowText (szText.GetBuffer());

	/*if (m_bUseExtraData)
	{
		m_cb_BufferCount.SetCurSel(IN_pstEquipment->m_nPortCount);
		m_cb_ConveyorCount.SetCurSel(IN_pstEquipment->m_nConveyorCount);
	}*/

	//UpdateUI_EquipmentData(IN_pstEquipment->Get_EquipmentType());
	UpdateUI_EquipmentData(IN_pstEquipment->Get_EqpType_UI());

#if (USE_XML)
	OnEquipSize(IN_pstEquipment, nEquip_cx, nEquip_cy);
	szText = m_reg.Load_Equipment_ID(IN_pstEquipment->Get_EquipmentId(), 0, 0);
	m_ed_SubEqpID.SetWindowText(szText);
	switch (IN_pstEquipment->Get_EqpType_UI())
	{
	case EqpUI_Handler:
		szText = m_reg.Load_Equipment_ID(IN_pstEquipment->Get_EquipmentId(), PtI_L_Load, 1);
		m_ed_EquipID[PtI_L_Load].SetWindowText(szText);
		szText = m_reg.Load_Equipment_ID(IN_pstEquipment->Get_EquipmentId(), PtI_L_Load, 2);
		m_ed_PortID[PtI_L_Load].SetWindowText(szText);
		break;
	case EqpUI_Loader:
		szText = m_reg.Load_Equipment_ID(IN_pstEquipment->Get_EquipmentId(), PtI_L_Load, 1);
		m_ed_EquipID[PtI_L_Load].SetWindowText(szText);
		szText = m_reg.Load_Equipment_ID(IN_pstEquipment->Get_EquipmentId(), PtI_L_Load, 2);
		m_ed_PortID[PtI_L_Load].SetWindowText(szText);
		break;
	case EqpUI_Returner:
		szText = m_reg.Load_Equipment_ID(IN_pstEquipment->Get_EquipmentId(), PtI_R_Buffer_1, 1);
		m_ed_EquipID[PtI_R_Buffer_1].SetWindowText(szText);
		szText = m_reg.Load_Equipment_ID(IN_pstEquipment->Get_EquipmentId(), PtI_R_Buffer_1, 2);
		m_ed_PortID[PtI_R_Buffer_1].SetWindowText(szText);
		break;
	default:
	{
		auto nCount = IN_pstEquipment->Get_TestPortCount();
		switch (nCount) {
		case 2:
			szText = m_reg.Load_Equipment_ID(IN_pstEquipment->Get_EquipmentId(), PtI_T_Test_L, 1);
			m_ed_EquipID[PtI_T_Test_L].SetWindowText(szText);
			szText = m_reg.Load_Equipment_ID(IN_pstEquipment->Get_EquipmentId(), PtI_T_Test_L, 2);
			m_ed_PortID[PtI_T_Test_L].SetWindowText(szText);

			szText = m_reg.Load_Equipment_ID(IN_pstEquipment->Get_EquipmentId(), PtI_T_Test_R, 1);
			m_ed_EquipID[PtI_T_Test_R].SetWindowText(szText);
			szText = m_reg.Load_Equipment_ID(IN_pstEquipment->Get_EquipmentId(), PtI_T_Test_R, 2);
			m_ed_PortID[PtI_T_Test_R].SetWindowText(szText);
			break;
		case 3:
			szText = m_reg.Load_Equipment_ID(IN_pstEquipment->Get_EquipmentId(), PtI_T_Test_L, 1);
			m_ed_EquipID[PtI_T_Test_L].SetWindowText(szText);
			szText = m_reg.Load_Equipment_ID(IN_pstEquipment->Get_EquipmentId(), PtI_T_Test_L, 2);
			m_ed_PortID[PtI_T_Test_L].SetWindowText(szText);

			szText = m_reg.Load_Equipment_ID(IN_pstEquipment->Get_EquipmentId(), PtI_T_Test_C, 1);
			m_ed_EquipID[PtI_T_Test_C].SetWindowText(szText);
			szText = m_reg.Load_Equipment_ID(IN_pstEquipment->Get_EquipmentId(), PtI_T_Test_C, 2);
			m_ed_PortID[PtI_T_Test_C].SetWindowText(szText);

			szText = m_reg.Load_Equipment_ID(IN_pstEquipment->Get_EquipmentId(), PtI_T_Test_R, 1);
			m_ed_EquipID[PtI_T_Test_R].SetWindowText(szText);
			szText = m_reg.Load_Equipment_ID(IN_pstEquipment->Get_EquipmentId(), PtI_T_Test_R, 2);
			m_ed_PortID[PtI_T_Test_R].SetWindowText(szText);
			break;

		}
	}
	break;
	}
#endif //SOCKET
	return TRUE;
}

//=============================================================================
// Method		: Verify_Equipment
// Access		: protected  
// Returns		: void
// Parameter	: __inout CConfig_Eqp & OUT_stEquipment
// Qualifier	:
// Last Update	: 2021/11/10 - 14:26
// Desc.		:
//=============================================================================
void CDlg_ModifyEquipment::Verify_Equipment(__inout CConfig_Eqp& OUT_stEquipment)
{
	// 검사 항목에 따라서 데이터 범위 체크 해야 함.

	// 동일한 IP 체크

	// Loader / Unloader 체크

	// Returner 체크



	// 검사 항목에 따라서 데이터 범위 체크 해야 함.
	/*OUT_stEquipment.m_szAlias;
	OUT_stEquipment.m_nEquipmentType;
	OUT_stEquipment.m_szEquipmentId;
	OUT_stEquipment.m_nIP_Address;	*/
	//OUT_stEquipment.m_nPortCount;
	//OUT_stEquipment.m_nConveyorCount;


}

//=============================================================================
// Method		: UpdateUI_EquipmentData
// Access		: protected  
// Returns		: void
// Parameter	: __in int IN_nIndex
// Qualifier	:
// Last Update	: 2021/11/10 - 14:27
// Desc.		:
//=============================================================================
void CDlg_ModifyEquipment::UpdateUI_EquipmentData(__in int IN_nIndex)
{
	m_ed_EqpAlias.SetWindowText(_T(""));

// 	if (m_EquipmentTypeList.size() <= IN_nIndex)
// 		return;
// 
// 	CString EqpItem = m_EquipmentTypeList.at(IN_nIndex);



}

//=============================================================================
// Method		: UpdateUI_EquipmentType
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/11/10 - 14:27
// Desc.		:
//=============================================================================
void CDlg_ModifyEquipment::UpdateUI_EquipmentType()
{
	if (NULL == GetSafeHwnd())
		return;

	m_cb_EquipmentType.ResetContent();

	for (auto itr = m_EquipmentTypeList.begin(); itr != m_EquipmentTypeList.end(); ++itr)
	{
		m_cb_EquipmentType.AddString(*itr);
	}

	m_cb_EquipmentType.SetCurSel(0);
}

//=============================================================================
// Method		: OnLanguage
// Access		: public  
// Returns		: void
// Parameter	: __in uint8_t IN_nLanguage
// Qualifier	:
// Last Update	: 2022/7/19 - 17:44
// Desc.		:
//=============================================================================
void CDlg_ModifyEquipment::OnLanguage(__in uint8_t IN_nLanguage)
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

		m_cb_EquipmentType.SetFont(&m_Font);

		if (GetSafeHwnd())
		{
			SetWindowText(g_szCtrlText_T[m_nLanguage][Txt_DialogCaption]);
			m_bn_OK.SetWindowText(g_szCtrlText_T[m_nLanguage][Txt_OK]);
			m_bn_Cancel.SetWindowText(g_szCtrlText_T[m_nLanguage][Txt_Cancel]);

			for (UINT nIdx = 0; nIdx < EID_MaxEnum; nIdx++)
			{
				m_st_EqpItemData[nIdx].SetText(g_szItemText_T[m_nLanguage][nIdx]);
			}
		}
	}
}

//=============================================================================
// Method		: Init_DefaultSet
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/11/10 - 14:27
// Desc.		:
//=============================================================================
void CDlg_ModifyEquipment::Init_DefaultSet()
{
	m_cb_EquipmentType.SetCurSel(0);
	m_cb_EquipmentType.EnableWindow(FALSE);
}

//=============================================================================
// Method		: Reset_EquipmentTypeList
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/11/10 - 14:27
// Desc.		:
//=============================================================================
void CDlg_ModifyEquipment::Reset_EquipmentTypeList()
{
	m_EquipmentTypeList.clear();

	if (NULL != GetSafeHwnd())
	{
		m_cb_EquipmentType.ResetContent();
	}
}

//=============================================================================
// Method		: Set_EquipmenTypetList
// Access		: public  
// Returns		: void
// Parameter	: __in std::vector<CString> * IN_pEquipmentTypeList
// Qualifier	:
// Last Update	: 2021/11/10 - 14:27
// Desc.		:
//=============================================================================
void CDlg_ModifyEquipment::Set_EquipmenTypetList(__in std::vector<CString>* IN_pEquipmentTypeList)
{
	m_EquipmentTypeList.clear();
	m_EquipmentTypeList = *IN_pEquipmentTypeList;

	//m_EquipmentTypeList.resize(IN_pEquipmentTypeList->size());
	//std::copy(IN_pEquipmentTypeList->begin(), IN_pEquipmentTypeList->end, m_EquipmentTypeList.begin());

	UpdateUI_EquipmentType();
}

//=============================================================================
// Method		: Set_EquipmentData
// Access		: public  
// Returns		: void
// Parameter	: __in const CConfig_Eqp * IN_pstEuquipment
// Qualifier	:
// Last Update	: 2021/11/10 - 14:27
// Desc.		:
//=============================================================================
void CDlg_ModifyEquipment::Set_EquipmentData(__in const CConfig_Eqp* IN_pstEuquipment)
{
	m_stEquipment = *IN_pstEuquipment;
}

//=============================================================================
// Method		: Get_EquipmentData
// Access		: public  
// Returns		: CConfig_Eqp&
// Qualifier	:
// Last Update	: 2021/11/10 - 14:27
// Desc.		:
//=============================================================================
CConfig_Eqp& CDlg_ModifyEquipment::Get_EquipmentData()
{
	return m_stEquipment;
}
//=============================================================================
// Method		: OnEquipSize
// Access		: public  
// Returns		: CConfig_Eqp&
// Qualifier	:
// Last Update	: 2023.05.26
// Desc.		:
//=============================================================================
void CDlg_ModifyEquipment::OnEquipSize(__in const CConfig_Eqp* IN_pstEuquipment, int cx, int cy)
{

	if ((0 == cx) || (0 == cy))
		return;

	int iSpacing = 5;
	int iMagin = 20;
	int iLeft = iMagin;
	int iTop = iMagin;
	int nWidth = cx - iMagin - iMagin;
	int nHeight = cy - iMagin - iMagin;
	int	iStWidth = __min(200, ((nWidth - 20) / 4));
	int iHalfWidth = __min(415, (nWidth - iStWidth - iSpacing)); // 3
	int iQuadWidth = (iHalfWidth - (iSpacing * 2)) / 3;	// 1
	int iCtrlWidth = iHalfWidth - iQuadWidth - iSpacing;	// 2
	int iDescWidth = __max(100, __min(160, (iHalfWidth - iSpacing) / 2));
	int iEdWidth = iHalfWidth - iDescWidth - iSpacing;
	int iCtrlHeight = (295 < nHeight) ? 25 : static_cast<int>(round((nHeight - 45) / 10));
	int iLeftSub = 0;
	int iLeftSub2 = 0;

	// 설비 데이터 항목 조건
	iLeftSub = iLeft + iStWidth + iSpacing;
	iLeftSub2 = iLeftSub + iStWidth + iSpacing;

	iTop += iCtrlHeight + iSpacing;
	iTop += iCtrlHeight + iSpacing;
	iTop += iCtrlHeight + iSpacing;
	iTop += iCtrlHeight + iSpacing;
	//2023.05.26a uhkim

	m_st_SubEqpID.MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
	m_ed_SubEqpID.MoveWindow(iLeftSub, iTop, iStWidth, iCtrlHeight);
	switch (IN_pstEuquipment->Get_EqpType_UI())
	{
	case EqpUI_Handler:
		iTop += iCtrlHeight + iSpacing;
		m_st_EquipID[PtI_L_Load].SetText(g_szPort_Loader[PtI_L_Load]);
		m_st_EquipID[PtI_L_Load].MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
		m_ed_EquipID[PtI_L_Load].MoveWindow(iLeftSub, iTop, iStWidth, iCtrlHeight);
		m_ed_PortID[PtI_L_Load].MoveWindow(iLeftSub2, iTop, iStWidth + iStWidth, iCtrlHeight);
		break;
	case EqpUI_Loader:
		iTop += iCtrlHeight + iSpacing;
		m_st_EquipID[PtI_L_Load].SetText(g_szPort_Loader[PtI_L_Load]);
		m_st_EquipID[PtI_L_Load].MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
		m_ed_EquipID[PtI_L_Load].MoveWindow(iLeftSub, iTop, iStWidth, iCtrlHeight);
		m_ed_PortID[PtI_L_Load].MoveWindow(iLeftSub2, iTop, iStWidth + iStWidth, iCtrlHeight);
		break;
	case EqpUI_Returner:
		iTop += iCtrlHeight + iSpacing;
		m_st_EquipID[PtI_R_Buffer_1].SetText(g_szPort_Loader[PtI_R_Buffer_1]);
		m_st_EquipID[PtI_R_Buffer_1].MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
		m_ed_EquipID[PtI_R_Buffer_1].MoveWindow(iLeftSub, iTop, iStWidth, iCtrlHeight);
		m_ed_PortID[PtI_R_Buffer_1].MoveWindow(iLeftSub2, iTop, iStWidth + iStWidth, iCtrlHeight);
		break;
	default:
		{
		auto nCount = IN_pstEuquipment->Get_TestPortCount();
		switch (nCount) {
		case 2:
			iTop += iCtrlHeight + iSpacing;
			m_st_EquipID[PtI_T_Test_L].SetText(g_szPort_Tester[PtI_T_Test_L]);
			m_st_EquipID[PtI_T_Test_L].MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
			m_ed_EquipID[PtI_T_Test_L].MoveWindow(iLeftSub, iTop, iStWidth, iCtrlHeight);
			m_ed_PortID[PtI_T_Test_L].MoveWindow(iLeftSub2, iTop, iStWidth + iStWidth, iCtrlHeight);
			//iTop += iCtrlHeight + iSpacing;
			m_st_EquipID[PtI_T_Test_C].MoveWindow(0,0,0,0);
			m_ed_EquipID[PtI_T_Test_C].MoveWindow(0, 0, 0, 0);
			m_ed_PortID[PtI_T_Test_C].MoveWindow(0, 0, 0, 0);
			iTop += iCtrlHeight + iSpacing;
			m_st_EquipID[PtI_T_Test_R].SetText(g_szPort_Tester[PtI_T_Test_R]);
			m_st_EquipID[PtI_T_Test_R].MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
			m_ed_EquipID[PtI_T_Test_R].MoveWindow(iLeftSub, iTop, iStWidth, iCtrlHeight);
			m_ed_PortID[PtI_T_Test_R].MoveWindow(iLeftSub2, iTop, iStWidth + iStWidth, iCtrlHeight);
			break;
		case 3:
			iTop += iCtrlHeight + iSpacing;
			m_st_EquipID[PtI_T_Test_L].SetText(g_szPort_Tester[PtI_T_Test_L]);
			m_st_EquipID[PtI_T_Test_L].MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
			m_ed_EquipID[PtI_T_Test_L].MoveWindow(iLeftSub, iTop, iStWidth, iCtrlHeight);
			m_ed_PortID[PtI_T_Test_L].MoveWindow(iLeftSub2, iTop, iStWidth + iStWidth, iCtrlHeight);

			iTop += iCtrlHeight + iSpacing;
			m_st_EquipID[PtI_T_Test_C].SetText(g_szPort_Tester[PtI_T_Test_C]);
			m_st_EquipID[PtI_T_Test_C].MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
			m_ed_EquipID[PtI_T_Test_C].MoveWindow(iLeftSub, iTop, iStWidth, iCtrlHeight);
			m_ed_PortID[PtI_T_Test_C].MoveWindow(iLeftSub2, iTop, iStWidth + iStWidth, iCtrlHeight);

			iTop += iCtrlHeight + iSpacing;
			m_st_EquipID[PtI_T_Test_R].SetText(g_szPort_Tester[PtI_T_Test_R]);
			m_st_EquipID[PtI_T_Test_R].MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
			m_ed_EquipID[PtI_T_Test_R].MoveWindow(iLeftSub, iTop, iStWidth, iCtrlHeight);
			m_ed_PortID[PtI_T_Test_R].MoveWindow(iLeftSub2, iTop, iStWidth + iStWidth, iCtrlHeight);
			break;

		}
		}		
		break;
	}

}