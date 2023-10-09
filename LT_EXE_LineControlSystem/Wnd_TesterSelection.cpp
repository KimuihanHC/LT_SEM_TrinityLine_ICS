//*****************************************************************************
// Filename	: 	Wnd_TesterSelection.cpp
// Created	:	2021/1/26 - 14:21
// Modified	:	2021/1/26 - 14:21
//
// Author	:	piring
//	
// Purpose	:	
//*****************************************************************************
#include "stdafx.h"
#include "Wnd_TesterSelection.h"
#include "resource.h"
#include "Def_Socket_Info.h"
#include "Def_WindowMessage.h"
#include "Def_Language_Message.h"

enum enCtrlText
{
	Txt_Title,
	Txt_ModelType,
	Txt_SocketType,
	Txt_Apply,

	Txt_MaxCount,
};

static LPCTSTR g_szCtrlText_T[Lang_MaxCount][Txt_MaxCount] =
{
	// 한국어
	{
		_T("[ 진행 스킵 정보 ]"),		// Txt_Title
		_T("모델 종류   "),			// Txt_ModelType
		_T("소켓 지그 종류   "),		// Txt_SocketType
		_T("적용"),					// Txt_Apply
	},

	// 영어
	{
		_T("[ PROCESS SKIP INFO ]"),	// Txt_Title
		_T("Model Type   "),			// Txt_ModelType
		_T("Socket JIG Type   "),		// Txt_SocketType
		_T("Apply"),					// Txt_Apply
	},

	// 베트남어
	{
		_T("[ PROCESS SKIP INFO ]"),	// Txt_Title
		_T("Model Type   "),			// Txt_ModelType
		_T("Socket JIG Type   "),		// Txt_SocketType
		_T("Apply"),					// Txt_Apply
	},

	// 중국어
	{
		_T("[ 工序跳过信息 ]"),			// Txt_Title
		_T("模型类型   "),				// Txt_ModelType
		_T("插口夹具类型   "),			// Txt_SocketType
		_T("适用"),						// Txt_Apply
	},
};


#define		IDC_CB_SOKCET_TYPE			1002
#define		IDC_CB_MODEL_TYPE			1003
#define		IDC_CHK_TESTER_FST			2000
#define		IDC_CHK_TESTER_LST			IDC_CHK_TESTER_FST + Max_TesterCount - 1
#define		IDC_BN_APPLY				1001

// CWnd_TesterSelection
IMPLEMENT_DYNAMIC(CWnd_TesterSelection, CWnd_BaseView)

//=============================================================================
//
//=============================================================================
CWnd_TesterSelection::CWnd_TesterSelection()
{
	VERIFY(m_font_Data.CreateFont(
		30,						// nHeight
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

	LOGFONT logFont;
	::ZeroMemory(&logFont, sizeof(LOGFONT));
	logFont.lfHeight = 20;
	logFont.lfWeight = FW_MEDIUM;
	::lstrcpy(logFont.lfFaceName, _T("Tahoma"));
	m_font.CreateFontIndirect(&logFont);
}

//=============================================================================
//
//=============================================================================
CWnd_TesterSelection::~CWnd_TesterSelection()
{
	m_font.DeleteObject();
}

BEGIN_MESSAGE_MAP(CWnd_TesterSelection, CWnd_BaseView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_CBN_SELENDOK	(IDC_CB_SOKCET_TYPE,	OnCbnSelendokSocketType)
	ON_CBN_SELENDOK(IDC_CB_MODEL_TYPE,		OnCbnSelendokModelType)
	ON_BN_CLICKED	(IDC_BN_APPLY,			OnBnClickedBnApply)
END_MESSAGE_MAP()


//=============================================================================
// Method		: OnCreate
// Access		: protected  
// Returns		: int
// Parameter	: LPCREATESTRUCT lpCreateStruct
// Qualifier	:
// Last Update	: 2021/1/26 - 14:25
// Desc.		:
//=============================================================================
int CWnd_TesterSelection::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetBackgroundColor(RGB(0, 0, 0));

	DWORD dwStyle = WS_VISIBLE | WS_CHILD /*| WS_CLIPCHILDREN*/ | WS_CLIPSIBLINGS;
	DWORD dwTempStyle = dwStyle;
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// 제목
	m_st_Title.SetStaticStyle(CVGStatic::StaticStyle_Title_Alt);
	m_st_Title.SetColorStyle(CVGStatic::ColorStyle_Black);
	m_st_Title.SetFont_Gdip(L"Arial", 24.0F);
	m_st_Title.SetUseBorder(FALSE);
	m_st_Title.SetTextAlignment(StringAlignmentNear);
	m_st_Title.Create(g_szCtrlText_T[m_nLanguage][Txt_Title], dwStyle | SS_CENTER | SS_CENTERIMAGE, rectDummy, this, IDC_STATIC);

	// 모델 종류
	m_st_ModelType.SetStaticStyle(CVGStatic::StaticStyle_Title_Alt);
	m_st_ModelType.SetColorStyle(CVGStatic::ColorStyle_Black);
	m_st_ModelType.SetFont_Gdip(L"Arial", 16.0F);
	m_st_ModelType.SetBorderTickness(0);
	m_st_ModelType.SetBorderColor_COLORREF(RGB(0, 0, 0));
	m_st_ModelType.SetTextAlignment(StringAlignmentFar);
	m_st_ModelType.Create(g_szCtrlText_T[m_nLanguage][Txt_ModelType], dwStyle | SS_CENTER | SS_CENTERIMAGE, rectDummy, this, IDC_STATIC);

	m_cb_ModelType.Create(dwStyle | CBS_DROPDOWNLIST, rectDummy, this, IDC_CB_MODEL_TYPE);
	m_cb_ModelType.SetFont(&m_font_Data);
	for (auto nIdx = 0; nIdx < enModelType::Max_ModelCount; ++nIdx)
	{
		m_cb_ModelType.AddString(g_szModelType[nIdx]);
	}
	m_cb_ModelType.SetCurSel(0);

	// 소켓 종류
	m_st_SocketType.SetStaticStyle(CVGStatic::StaticStyle_Title_Alt);
	m_st_SocketType.SetColorStyle(CVGStatic::ColorStyle_Black);
	m_st_SocketType.SetFont_Gdip(L"Arial", 16.0F);
	m_st_SocketType.SetBorderTickness(0);
	m_st_SocketType.SetBorderColor_COLORREF(RGB(0, 0, 0));
	m_st_SocketType.SetTextAlignment(StringAlignmentFar);
	m_st_SocketType.Create(g_szCtrlText_T[m_nLanguage][Txt_SocketType], dwStyle | SS_CENTER | SS_CENTERIMAGE, rectDummy, this, IDC_STATIC);

	m_cb_SocketType.Create(dwStyle | CBS_DROPDOWNLIST, rectDummy, this, IDC_CB_SOKCET_TYPE);
	m_cb_SocketType.SetFont(&m_font_Data);
	for (auto nIdx = 0; nIdx < Max_SocketTypeCount; ++nIdx)
	{
		m_cb_SocketType.AddString(g_szSocketTypeName[nIdx]);
	}
	m_cb_SocketType.SetCurSel(0);

	// 검사 설비 종류
	for (auto nIdx = 0; nIdx < Max_TesterCount; ++nIdx)
	{
		m_chk_TesterType[nIdx].Create(g_szTesterTypeName[nIdx], dwStyle | BS_AUTOCHECKBOX | BS_PUSHLIKE, rectDummy, this, IDC_CHK_TESTER_FST + nIdx);
		m_chk_TesterType[nIdx].SetFont(&m_font);

		m_chk_TesterType[nIdx].SetMouseCursorHand();
		m_chk_TesterType[nIdx].SetImage(IDB_UNCHECKED_16);
		m_chk_TesterType[nIdx].SetCheckedImage(IDB_CHECKED_16);
		m_chk_TesterType[nIdx].SizeToContent();
	}

	// 설정 적용
	m_bn_Apply.Create(g_szCtrlText_T[m_nLanguage][Txt_Apply], dwStyle | WS_BORDER | BS_PUSHBUTTON, rectDummy, this, IDC_BN_APPLY);
	m_bn_Apply.SetFont(&m_font);


	Update_SocketType(true);
	Update_ModelType(0);

	Set_PermissionMode(enPermissionMode::Permission_Operator);

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
// Last Update	: 2021/1/26 - 14:26
// Desc.		:
//=============================================================================
void CWnd_TesterSelection::OnSize(UINT nType, int cx, int cy)
{
	CWnd_BaseView::OnSize(nType, cx, cy);

	if ((cx == 0) && (cy == 0))
		return;

	int iMagrin		= 10;
	int iSpacing	= 10;
	int iLeft		= iMagrin;
	int iTop		= iMagrin;
	int iCtrlHeight	= 50; 
	//int iTestCount	= (0 < m_TestOrder.size()) ? (int)m_TestOrder.size() : Max_TesterCount;
	//int iCtrlWidth	= (cx - (iMagrin * 2) - (iSpacing * (iTestCount - 1))) / iTestCount;
	int iCtrlWidth	= (cx - (iMagrin * 2) - (iSpacing * (Max_TesterCount - 1))) / Max_TesterCount;
	int iCbStWidth	= 200;
	int iComboWidth = 260;
	//int iLongWidth  = iCtrlWidth + iSpacing + iCtrlWidth;
	int iLongWidth = 480;

	m_st_Title.MoveWindow(iLeft, iTop, iLongWidth, iCtrlHeight);
	iLeft += iLongWidth + iSpacing;

	// 소켓
	m_st_SocketType.MoveWindow(iLeft, iTop, iCbStWidth, iCtrlHeight);
	iLeft += iCbStWidth + iSpacing;
	m_cb_SocketType.MoveWindow(iLeft, iTop, iComboWidth, iCtrlHeight);

	// 적용
	iLeft = cx - iMagrin - iLongWidth;
	m_bn_Apply.MoveWindow(iLeft, iTop, iLongWidth, iCtrlHeight);

	// 모델
	iLeft = iMagrin + iLongWidth + iSpacing;
	iTop += iCtrlHeight + iSpacing;
	m_st_ModelType.MoveWindow(iLeft, iTop, iCbStWidth, iCtrlHeight);
	iLeft += iCbStWidth + iSpacing;
	m_cb_ModelType.MoveWindow(iLeft, iTop, iComboWidth, iCtrlHeight);

	iLeft		= iMagrin;
	iTop += iCtrlHeight + iSpacing;
	m_nTop_TestButton	= iTop;
	m_iTest_Width		= iCtrlWidth;
	m_iTest_Height		= iCtrlHeight;
	if (0 < m_TestOrder.size())
	{
		Rebatch_TestButton();
	}
	else
	{
		for (auto nIdx = 0; nIdx < Max_TesterCount; ++nIdx)
		{
			m_chk_TesterType[nIdx].MoveWindow(iLeft, iTop, iCtrlWidth, iCtrlHeight);

			iLeft += iCtrlWidth + iSpacing;
		}
	}

}

//=============================================================================
// Method		: PreCreateWindow
// Access		: virtual protected  
// Returns		: BOOL
// Parameter	: CREATESTRUCT & cs
// Qualifier	:
// Last Update	: 2021/1/26 - 14:26
// Desc.		:
//=============================================================================
BOOL CWnd_TesterSelection::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1), NULL);

	return __super::PreCreateWindow(cs);
}

//=============================================================================
// Method		: PreTranslateMessage
// Access		: virtual protected  
// Returns		: BOOL
// Parameter	: MSG * pMsg
// Qualifier	:
// Last Update	: 2021/1/26 - 14:26
// Desc.		:
//=============================================================================
BOOL CWnd_TesterSelection::PreTranslateMessage(MSG* pMsg)
{
	return __super::PreTranslateMessage(pMsg);
}

//=============================================================================
// Method		: OnCbnSelendokSocketType
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2022/1/3 - 17:01
// Desc.		:
//=============================================================================
void CWnd_TesterSelection::OnCbnSelendokSocketType()
{
	Update_SocketType(true);	
}

//=============================================================================
// Method		: OnCbnSelendokModelType
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2022/1/13 - 15:57
// Desc.		:
//=============================================================================
void CWnd_TesterSelection::OnCbnSelendokModelType()
{
	int iSel = m_cb_SocketType.GetCurSel();

	if (0 <= iSel)
	{
		Update_ModelType(iSel);
	}
}

//=============================================================================
// Method		: OnBnClickedBnApply
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/12/3 - 17:54
// Desc.		:
//=============================================================================
void CWnd_TesterSelection::OnBnClickedBnApply()
{
	//if (IDYES == LT_MessageBox(_T("Apply the changed settings?"), MB_YESNO))
	if (IDYES == LT_MessageBox(g_szMessageBox_T[MB_Common_Apply][m_nLanguage], MB_YESNO))
	{
		int nSocketType = m_cb_SocketType.GetCurSel();

		int nModelType = -1;

		CString szText;
		m_cb_ModelType.GetWindowText(szText);
		for (auto nIdx = 0; nIdx < Max_ModelCount; nIdx++)
		{
			if (0 == szText.Compare(g_szModelType[nIdx]))
			{
				nModelType = nIdx;
			}
		}

		m_pConfigModel->m_nSocketType = nSocketType;
		m_pConfigModel->m_nModelType = nModelType;

		for (auto nIdx = 0; nIdx < Max_TesterCount; ++nIdx)
		{
			m_pConfigModel->m_bEnable_TestType[nIdx] = (m_chk_TesterType[nIdx].GetCheck() == BST_CHECKED) ? true : false;
		}

		GetOwner()->SendMessage(WM_TEST_SELECT, (WPARAM)m_pConfigModel->m_bEnable_TestType, MAKELPARAM(Max_TesterCount, nSocketType));
	}
}

//=============================================================================
// Method		: Rebatch_TestButton
// Access		: private  
// Returns		: void
// Qualifier	:
// Last Update	: 2022/7/20 - 16:54
// Desc.		:
//=============================================================================
void CWnd_TesterSelection::Rebatch_TestButton()
{
	int		iLeft = 10;
	uint8_t nTestType = 0;
	CRect	rc;

	GetClientRect(rc);
	m_iTest_Width = (rc.Width() - 20 - (10 * ((int)m_TestOrder.size() - 1))) / (int)m_TestOrder.size();

	// 검사 타입 체크 버튼 숨기기
	for (auto nIdx = 0; nIdx < Max_TesterCount; ++nIdx)
	{
		m_chk_TesterType[nIdx].MoveWindow(0, 0, 0, 0);
	}

	// 라인 설정에 사용되는 검사 타입의 버튼 표시
	for (auto nOrderIdx = 0; nOrderIdx < m_TestOrder.size(); ++nOrderIdx)
	{
		nTestType = ConvTo_TesterType((enEquipmentType)m_TestOrder.at(nOrderIdx));
		m_chk_TesterType[nTestType].MoveWindow(iLeft, m_nTop_TestButton, m_iTest_Width, m_iTest_Height);

		iLeft += m_iTest_Width + 10;
	}
}

//=============================================================================
// Method		: SetUI_Configuration
// Access		: public  
// Returns		: void
// Parameter	: __in const CConfig_Model * IN_pModelConfig
// Qualifier	:
// Last Update	: 2021/12/2 - 18:21
// Desc.		:
//=============================================================================
void CWnd_TesterSelection::SetUI_Configuration(__in const CConfig_Model* IN_pModelConfig)
{
	if (NULL != IN_pModelConfig)
	{
		CString strValue;

		// 소켓
		m_cb_SocketType.SetCurSel(IN_pModelConfig->m_nSocketType);
		Update_SocketType(true);

		// 모델
		int iSel = m_cb_ModelType.FindString(0, g_szModelType[IN_pModelConfig->m_nModelType]);
		m_cb_ModelType.SetCurSel(iSel);
		Update_ModelType(iSel);

		for (auto nIdx = 0; nIdx < Max_TesterCount; ++nIdx)
		{
			m_chk_TesterType[nIdx].SetCheck(IN_pModelConfig->m_bEnable_TestType[nIdx] ? BST_CHECKED : BST_UNCHECKED);
		}
		
	}
}

//=============================================================================
// Method		: Update_SocketType
// Access		: private  
// Returns		: void
// Parameter	: __in bool IN_bUpdateModel
// Qualifier	:
// Last Update	: 2022/1/13 - 18:26
// Desc.		:
//=============================================================================
void CWnd_TesterSelection::Update_SocketType(__in bool IN_bUpdateModel)
{
	int iSel = m_cb_SocketType.GetCurSel();

#if (SET_INSPECTOR == SYS_ICS_RIVIAN_LINE)
	if (0 <= iSel)
	{
		switch ((enSocketType)iSel)
		{
		case enSocketType::Socket_30_FOV:	// SFR 협각
			m_chk_TesterType[Tester_Distortion].SetCheck(BST_UNCHECKED); // 8M 30도 모델은 Distortion 단독 설비 사용
			if (Permission_Operator != m_PermissionMode)
			{
				m_chk_TesterType[Tester_SFR_CL].EnableWindow(TRUE);				
			}
			m_chk_TesterType[Tester_SFR_MultiCL].EnableWindow(FALSE);
			m_chk_TesterType[Tester_Distortion].EnableWindow(FALSE);
			break;

		case enSocketType::Socket_180_FOV:	// SFR 광각
			m_chk_TesterType[Tester_SFR_CL].SetCheck(BST_UNCHECKED);			
			if (Permission_Operator != m_PermissionMode)
			{
				m_chk_TesterType[Tester_SFR_MultiCL].EnableWindow(TRUE);
				m_chk_TesterType[Tester_Distortion].EnableWindow(TRUE);
			}
			m_chk_TesterType[Tester_SFR_CL].EnableWindow(FALSE);
			break;

		default:
			break;
		}
	}

	if (IN_bUpdateModel)
	{
		if (0 <= iSel)
		{
			m_cb_ModelType.ResetContent();

			switch ((enSocketType)iSel)
			{
			case enSocketType::Socket_30_FOV:	// SFR 협각
				m_cb_ModelType.AddString(g_szModelType[Model_8M_30FOV]);
				//m_cb_ModelType.AddString(g_szModelType[Model_3M_180FOV]);

				break;

			case enSocketType::Socket_180_FOV:	// SFR 광각
				//m_cb_ModelType.AddString(g_szModelType[Model_8M_30FOV]);
				m_cb_ModelType.AddString(g_szModelType[Model_3M_180FOV_SIDE]);
				m_cb_ModelType.AddString(g_szModelType[Model_3M_180FOV_FRONT]);
				m_cb_ModelType.AddString(g_szModelType[Model_3M_180FOV_REAR]);
				break;

			default:
				break;
			}

			m_cb_ModelType.SetCurSel(0);

			Update_ModelType(iSel);
		}
	}

#else
	if (0 <= iSel)
	{
		switch ((enSocketType)iSel)
		{
		case enSocketType::Socket_H:	// SFR 광각 H, SFR 협각 H
			if (Permission_Operator != m_PermissionMode)
			{
				m_chk_TesterType[Tester_SFR_CL_46].EnableWindow(TRUE);
				m_chk_TesterType[Tester_SFR_MultiCL_83].EnableWindow(TRUE);
				m_chk_TesterType[Tester_SFR_MultiCL_180].EnableWindow(TRUE);
				m_chk_TesterType[Tester_SFR_MultiCL].EnableWindow(TRUE);
			}
			break;

		case enSocketType::Socket_V:	// SFR 광각 V
			m_chk_TesterType[Tester_SFR_CL_46].SetCheck(BST_UNCHECKED);
			//m_chk_TesterType[Tester_SFR_MultiCL_180].SetCheck(BST_UNCHECKED);

			if (Permission_Operator != m_PermissionMode)
			{
				m_chk_TesterType[Tester_SFR_MultiCL_83].EnableWindow(TRUE);
				m_chk_TesterType[Tester_SFR_MultiCL].EnableWindow(TRUE);
				m_chk_TesterType[Tester_SFR_MultiCL_180].EnableWindow(TRUE);
			}
			m_chk_TesterType[Tester_SFR_CL_46].EnableWindow(FALSE);
			//m_chk_TesterType[Tester_SFR_MultiCL_180].EnableWindow(FALSE);
			break;

		case enSocketType::Socket_Highland:
			m_chk_TesterType[Tester_SFR_CL_46].SetCheck(BST_UNCHECKED);
			m_chk_TesterType[Tester_SFR_MultiCL_83].SetCheck(BST_UNCHECKED);

			if (Permission_Operator != m_PermissionMode)
			{
				m_chk_TesterType[Tester_SFR_MultiCL].EnableWindow(TRUE);
				m_chk_TesterType[Tester_SFR_MultiCL_180].EnableWindow(TRUE);
			}
			m_chk_TesterType[Tester_SFR_CL_46].EnableWindow(FALSE);
			m_chk_TesterType[Tester_SFR_MultiCL_83].EnableWindow(FALSE);
			break;

		default:
			break;
		}
	}

	if (IN_bUpdateModel)
	{
		if (0 <= iSel)
		{
			m_cb_ModelType.ResetContent();

			switch ((enSocketType)iSel)
			{
			case enSocketType::Socket_H:	// SFR 광각 H, SFR 협각 H
				m_cb_ModelType.AddString(g_szModelType[Model_46]);
				m_cb_ModelType.AddString(g_szModelType[Model_83]);
				m_cb_ModelType.AddString(g_szModelType[Model_180]);

				break;

			case enSocketType::Socket_V:	// SFR 광각 V
				m_cb_ModelType.AddString(g_szModelType[Model_83]);
				m_cb_ModelType.AddString(g_szModelType[Model_180]);
				break;

			case enSocketType::Socket_Highland:
				m_cb_ModelType.AddString(g_szModelType[Model_180]);
				break;

			default:
				break;
			}

			m_cb_ModelType.SetCurSel(0);

			Update_ModelType(iSel);
		}
	}

#endif // (SET_INSPECTOR == SYS_ICS_RIVIAN_LINE)
}

//=============================================================================
// Method		: Update_ModelType
// Access		: private  
// Returns		: void
// Parameter	: __in uint8_t IN_nSocketType
// Qualifier	:
// Last Update	: 2022/1/13 - 18:26
// Desc.		:
//=============================================================================
void CWnd_TesterSelection::Update_ModelType(__in uint8_t IN_nSocketType)
{
	int nType = -1;
	CString szText;
	m_cb_ModelType.GetWindowText(szText);
	for (auto nIdx = 0; nIdx < Max_ModelCount; nIdx++)
	{
		if (0 == szText.Compare(g_szModelType[nIdx]))
		{
			nType = nIdx;
		}
	}

#if (SET_INSPECTOR == SYS_ICS_RIVIAN_LINE)
	switch ((enModelType)nType)
	{
		//-----------------------------------------------------
	case Model_8M_30FOV:	// SFR 협각 (Socket_H 인 경우)
		switch ((enSocketType)IN_nSocketType)
		{
		case enSocketType::Socket_30_FOV:	// 협각
		{
			if (Permission_Operator != m_PermissionMode)
			{
				m_chk_TesterType[Tester_SFR_CL].SetCheck(BST_CHECKED);
				m_chk_TesterType[Tester_SFR_CL].EnableWindow(TRUE);
			}

			m_chk_TesterType[Tester_SFR_MultiCL].SetCheck(BST_UNCHECKED);
			m_chk_TesterType[Tester_SFR_MultiCL].EnableWindow(FALSE);

			m_chk_TesterType[Tester_Distortion].SetCheck(BST_UNCHECKED);
			m_chk_TesterType[Tester_Distortion].EnableWindow(FALSE);

		}
			break;

		case enSocketType::Socket_180_FOV:	// 광각
			break;
		}

		break;

		//-----------------------------------------------------
	case Model_3M_180FOV_SIDE:	// SFR 광각 H, V
	case Model_3M_180FOV_FRONT:	// SFR 광각 H, V
	case Model_3M_180FOV_REAR:	// SFR 광각 H, V
		switch ((enSocketType)IN_nSocketType)
		{
		case enSocketType::Socket_30_FOV:	// SFR 협각
			break;

		case enSocketType::Socket_180_FOV:	// SFR 광각
		{
			m_chk_TesterType[Tester_SFR_CL].SetCheck(BST_UNCHECKED);
			m_chk_TesterType[Tester_SFR_CL].EnableWindow(FALSE);
			if (Permission_Operator != m_PermissionMode)
			{
				m_chk_TesterType[Tester_SFR_MultiCL].SetCheck(BST_CHECKED);
				m_chk_TesterType[Tester_SFR_MultiCL].EnableWindow(TRUE);

				m_chk_TesterType[Tester_Distortion].SetCheck(BST_CHECKED);
				m_chk_TesterType[Tester_Distortion].EnableWindow(TRUE);
			}
		}
			break;
		}

		break;
	}

#else

	switch ((enModelType)nType)
	{
		//-----------------------------------------------------
	case Model_46:	// SFR 협각 (Socket_H 인 경우)
		switch ((enSocketType)IN_nSocketType)
		{
		case enSocketType::Socket_H:	// SFR 광각 H, SFR 협각 H
		{
			if (Permission_Operator != m_PermissionMode)
			{
				m_chk_TesterType[Tester_SFR_CL_46].SetCheck(BST_CHECKED);
				m_chk_TesterType[Tester_SFR_CL_46].EnableWindow(TRUE);
			}

			m_chk_TesterType[Tester_SFR_MultiCL_83].SetCheck(BST_UNCHECKED);
			m_chk_TesterType[Tester_SFR_MultiCL_83].EnableWindow(FALSE);

			m_chk_TesterType[Tester_SFR_MultiCL_180].SetCheck(BST_UNCHECKED);
			m_chk_TesterType[Tester_SFR_MultiCL_180].EnableWindow(FALSE);

			m_chk_TesterType[Tester_SFR_MultiCL].SetCheck(BST_UNCHECKED);
			m_chk_TesterType[Tester_SFR_MultiCL].EnableWindow(FALSE);

		}
		break;

		case enSocketType::Socket_V:	// SFR 광각 V
			break;

		case enSocketType::Socket_Highland:
			break;
		}

		break;

		//-----------------------------------------------------
	case Model_83:	// SFR 광각 H, V
		switch ((enSocketType)IN_nSocketType)
		{
		case enSocketType::Socket_H:	// SFR 광각 H, SFR 협각 H
		{
			m_chk_TesterType[Tester_SFR_CL_46].SetCheck(BST_UNCHECKED);
			m_chk_TesterType[Tester_SFR_CL_46].EnableWindow(FALSE);
			m_chk_TesterType[Tester_SFR_MultiCL_180].SetCheck(BST_UNCHECKED);
			m_chk_TesterType[Tester_SFR_MultiCL_180].EnableWindow(FALSE);
			if (Permission_Operator != m_PermissionMode)
			{
				m_chk_TesterType[Tester_SFR_MultiCL_83].SetCheck(BST_CHECKED);
				m_chk_TesterType[Tester_SFR_MultiCL_83].EnableWindow(TRUE);

				m_chk_TesterType[Tester_SFR_MultiCL].SetCheck(BST_CHECKED);
				m_chk_TesterType[Tester_SFR_MultiCL].EnableWindow(TRUE);
			}
		}
			break;

		case enSocketType::Socket_V:	// SFR 광각 V
		{
			m_chk_TesterType[Tester_SFR_MultiCL_180].SetCheck(BST_UNCHECKED);
			m_chk_TesterType[Tester_SFR_MultiCL_180].EnableWindow(FALSE);
			if (Permission_Operator != m_PermissionMode)
			{
				m_chk_TesterType[Tester_SFR_MultiCL_83].SetCheck(BST_CHECKED);
				m_chk_TesterType[Tester_SFR_MultiCL_83].EnableWindow(TRUE);

				m_chk_TesterType[Tester_SFR_MultiCL].SetCheck(BST_CHECKED);
				m_chk_TesterType[Tester_SFR_MultiCL].EnableWindow(TRUE);
			}
		}
			break;

		case enSocketType::Socket_Highland:
		{
			m_chk_TesterType[Tester_SFR_CL_46].SetCheck(BST_UNCHECKED);
			m_chk_TesterType[Tester_SFR_CL_46].EnableWindow(FALSE);
			m_chk_TesterType[Tester_SFR_MultiCL_83].SetCheck(BST_UNCHECKED);
			m_chk_TesterType[Tester_SFR_MultiCL_83].EnableWindow(FALSE);
			m_chk_TesterType[Tester_SFR_MultiCL_180].SetCheck(BST_CHECKED);
			m_chk_TesterType[Tester_SFR_MultiCL_180].EnableWindow(FALSE);
			m_chk_TesterType[Tester_SFR_MultiCL].SetCheck(BST_UNCHECKED);
			m_chk_TesterType[Tester_SFR_MultiCL].EnableWindow(FALSE);
		}
			break;
		}

		break;

		//-----------------------------------------------------
	case Model_180:	// SFR 광각 H, V
		switch ((enSocketType)IN_nSocketType)
		{
		case enSocketType::Socket_H:	// SFR 광각 H, SFR 협각 H
		{
			m_chk_TesterType[Tester_SFR_CL_46].SetCheck(BST_UNCHECKED);
			m_chk_TesterType[Tester_SFR_CL_46].EnableWindow(FALSE);
			m_chk_TesterType[Tester_SFR_MultiCL_83].SetCheck(BST_UNCHECKED);
			m_chk_TesterType[Tester_SFR_MultiCL_83].EnableWindow(FALSE);
			if (Permission_Operator != m_PermissionMode)
			{
				m_chk_TesterType[Tester_SFR_MultiCL_180].SetCheck(BST_CHECKED);
				m_chk_TesterType[Tester_SFR_MultiCL_180].EnableWindow(TRUE);

				m_chk_TesterType[Tester_SFR_MultiCL].SetCheck(BST_CHECKED);
				m_chk_TesterType[Tester_SFR_MultiCL].EnableWindow(TRUE);
			}
		}
			break;

		case enSocketType::Socket_V:	// SFR 광각 V
		{
			m_chk_TesterType[Tester_SFR_CL_46].SetCheck(BST_UNCHECKED);
			m_chk_TesterType[Tester_SFR_CL_46].EnableWindow(FALSE);
			m_chk_TesterType[Tester_SFR_MultiCL_83].SetCheck(BST_UNCHECKED);
			m_chk_TesterType[Tester_SFR_MultiCL_83].EnableWindow(FALSE);
			if (Permission_Operator != m_PermissionMode)
			{
				m_chk_TesterType[Tester_SFR_MultiCL_180].SetCheck(BST_CHECKED);
				m_chk_TesterType[Tester_SFR_MultiCL_180].EnableWindow(TRUE);

				m_chk_TesterType[Tester_SFR_MultiCL].SetCheck(BST_CHECKED);
				m_chk_TesterType[Tester_SFR_MultiCL].EnableWindow(TRUE);
			}
		}
			break;

		case enSocketType::Socket_Highland:
		{
			m_chk_TesterType[Tester_SFR_CL_46].SetCheck(BST_UNCHECKED);
			m_chk_TesterType[Tester_SFR_CL_46].EnableWindow(FALSE);
			m_chk_TesterType[Tester_SFR_MultiCL_83].SetCheck(BST_UNCHECKED);
			m_chk_TesterType[Tester_SFR_MultiCL_83].EnableWindow(FALSE);
			if (Permission_Operator != m_PermissionMode)
			{
				m_chk_TesterType[Tester_SFR_MultiCL_180].SetCheck(BST_CHECKED);
				m_chk_TesterType[Tester_SFR_MultiCL_180].EnableWindow(TRUE);

				m_chk_TesterType[Tester_SFR_MultiCL].SetCheck(BST_CHECKED);
				m_chk_TesterType[Tester_SFR_MultiCL].EnableWindow(TRUE);
			}
		}
			break;
		}


		break;
	}

#endif // (SET_INSPECTOR == SYS_ICS_RIVIAN_LINE)

}

//=============================================================================
// Method		: OnLanguage
// Access		: virtual public  
// Returns		: void
// Parameter	: __in uint8_t IN_nLanguage
// Qualifier	:
// Last Update	: 2022/7/20 - 16:52
// Desc.		:
//=============================================================================
void CWnd_TesterSelection::OnLanguage(__in uint8_t IN_nLanguage)
{
	__super::OnLanguage(IN_nLanguage);

	// CharSet 변경
	LOGFONT lf_data;
	m_font_Data.GetLogFont(&lf_data);
	lf_data.lfCharSet = Get_CharSet(m_nLanguage);
	m_font_Data.DeleteObject();
	m_font_Data.CreateFontIndirect(&lf_data);

	// CharSet 변경
	LOGFONT lf;
	m_font.GetLogFont(&lf);
	lf.lfCharSet = Get_CharSet(m_nLanguage);
	m_font.DeleteObject();
	m_font.CreateFontIndirect(&lf);

	m_cb_ModelType.SetFont(&m_font_Data);
	m_cb_SocketType.SetFont(&m_font_Data);
	for (auto nIdx = 0; nIdx < Max_TesterCount; ++nIdx)
	{
		m_chk_TesterType[nIdx].SetFont(&m_font);
	}
	m_bn_Apply.SetFont(&m_font);

	if (GetSafeHwnd())
	{
		m_st_Title.SetText(g_szCtrlText_T[m_nLanguage][Txt_Title]);
		m_st_ModelType.SetText(g_szCtrlText_T[m_nLanguage][Txt_ModelType]);
		m_st_SocketType.SetText(g_szCtrlText_T[m_nLanguage][Txt_SocketType]);
		m_bn_Apply.SetWindowText(g_szCtrlText_T[m_nLanguage][Txt_Apply]);
	}
}

//=============================================================================
// Method		: Set_Config_Line
// Access		: public  
// Returns		: void
// Parameter	: const CConfig_Line * IN_pConfig_Line
// Qualifier	:
// Last Update	: 2022/7/20 - 16:52
// Desc.		:
//=============================================================================
void CWnd_TesterSelection::Set_Config_Line(const CConfig_Line * IN_pConfig_Line)
{
	// 각각의 설비 유형에 따른 첫번째 설비
	m_TestOrder.clear();

	// 검사 순서 설정
	for (auto nEqpIdx = 0; nEqpIdx < IN_pConfig_Line->GetCount(); ++nEqpIdx)
	{
		// 등록되지 않음 검사 판별
		bool bFind = false;
		for (auto nOrderIdx = 0; nOrderIdx < m_TestOrder.size(); ++nOrderIdx)
		{
			if (m_TestOrder.at(nOrderIdx) == IN_pConfig_Line->GetAt(nEqpIdx).Get_EquipmentType())
			{
				bFind = true;
				break;
			}
		}

		if (false == bFind)
		{
			// Test 순서 설정
			/*if ((Eqp_Tester_First <= IN_pConfig_Line->GetAt(nEqpIdx).Get_EquipmentType()) &&
				(IN_pConfig_Line->GetAt(nEqpIdx).Get_EquipmentType() <= Eqp_Tester_Last))*/
			if (IsTester_byEqpType(IN_pConfig_Line->GetAt(nEqpIdx).Get_EquipmentType()))
			{
				m_TestOrder.push_back(IN_pConfig_Line->GetAt(nEqpIdx).Get_EquipmentType());
			}
		}
	}

	Rebatch_TestButton();
}

//=============================================================================
// Method		: Set_Configuration
// Access		: public  
// Returns		: void
// Parameter	: __in CConfig_Model & IN_ModelConfig
// Qualifier	:
// Last Update	: 2022/1/13 - 16:37
// Desc.		:
//=============================================================================
void CWnd_TesterSelection::Set_Configuration(__in CConfig_Model& IN_ModelConfig)
{
	m_pConfigModel = &IN_ModelConfig;

	SetUI_Configuration(m_pConfigModel);	
}

//=============================================================================
// Method		: Get_Condition
// Access		: public  
// Returns		: void
// Parameter	: __out bool * OUT_pbCondition
// Parameter	: __out uint8_t & OUT_nDataCount
// Qualifier	:
// Last Update	: 2022/1/13 - 17:06
// Desc.		:
//=============================================================================
void CWnd_TesterSelection::Get_Condition(__out bool* OUT_pbCondition, __out uint8_t& OUT_nDataCount)
{
	for (auto nIdx = 0; nIdx < Max_TesterCount; ++nIdx)
	{
		OUT_pbCondition[nIdx] = (m_chk_TesterType[nIdx].GetCheck() == BST_CHECKED) ? true : false;
	}

	OUT_nDataCount = Max_TesterCount;
}

//=============================================================================
// Method		: Set_PermissionMode
// Access		: public  
// Returns		: void
// Parameter	: __in enPermissionMode IN_PermissionMode
// Qualifier	:
// Last Update	: 2022/1/6 - 20:39
// Desc.		:
//=============================================================================
void CWnd_TesterSelection::Set_PermissionMode(__in enPermissionMode IN_PermissionMode)
{
	m_PermissionMode = IN_PermissionMode;

	switch (IN_PermissionMode)
	{
	case Permission_Operator:
		m_bn_Apply.EnableWindow(FALSE);
		m_cb_SocketType.EnableWindow(FALSE);
		m_cb_ModelType.EnableWindow(FALSE);

		// 체크 버튼 전체 비활성화
		for (auto nIdx = 0; nIdx < Max_TesterCount; ++nIdx)
		{
			m_chk_TesterType[nIdx].EnableWindow(FALSE);
		}
		break;

	case Permission_Engineer:
	case Permission_Administrator:
		m_bn_Apply.EnableWindow(TRUE);
		m_cb_SocketType.EnableWindow(TRUE);
		m_cb_ModelType.EnableWindow(TRUE);

		// 체크 버튼 소켓 타입에 따른 활성화
		m_chk_TesterType[Tester_ColorCal].EnableWindow(TRUE);
		m_chk_TesterType[Tester_HotPixel].EnableWindow(TRUE);
		m_chk_TesterType[Tester_StrayLight].EnableWindow(TRUE);
		m_chk_TesterType[Tester_Distortion].EnableWindow(TRUE);
		//m_chk_TesterType[Tester_HotPixel3port].EnableWindow(TRUE);
		m_chk_TesterType[Tester_EEPROM].EnableWindow(TRUE);
		
		Update_SocketType(false);

		Update_ModelType(m_cb_SocketType.GetCurSel());
		break;

	default:
		break;
	}
}

#if defined(EES_XML)//20231003
void CWnd_TesterSelection::Set_Config_Server(const CConfig_Server * IN_pConfig_Server)
{
	// 각각의 설비 유형에 따른 첫번째 설비
	m_TestOrder.clear();
	// 검사 순서 설정
	for (auto nEqpIdx = 0; nEqpIdx < IN_pConfig_Server->GetCount(); ++nEqpIdx)
	{
		// 등록되지 않음 검사 판별
		bool bFind = false;
		for (auto nOrderIdx = 0; nOrderIdx < m_TestOrder.size(); ++nOrderIdx)
		{
			if (m_TestOrder.at(nOrderIdx) == IN_pConfig_Server->GetAt(nEqpIdx).Get_ServerType())
			{
				bFind = true;
				break;
			}
		}
		if (false == bFind)
		{
			// Test 순서 설정
			/*if ((Eqp_Tester_First <= IN_pConfig_Line->GetAt(nEqpIdx).Get_EquipmentType()) &&
				(IN_pConfig_Line->GetAt(nEqpIdx).Get_EquipmentType() <= Eqp_Tester_Last))*/
			if (IsTester_byEqpType(IN_pConfig_Server->GetAt(nEqpIdx).Get_ServerType()))//20231003
			{
				m_TestOrder.push_back(IN_pConfig_Server->GetAt(nEqpIdx).Get_ServerType());
			}
		}
	}
	//Rebatch_TestButton();
}
#endif