//*****************************************************************************
// Filename	: 	Dlg_ModifyNGCode.cpp
// Created	:	2021/10/20 - 12:09
// Modified	:	2021/10/20 - 12:09
//
// Author	:	piring
//	
// Purpose	:	
//*****************************************************************************
// Dlg_ModifyNGCode.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Dlg_ModifyNGCode.h"
#include "Registry.h"
#include "resource.h"
#include "CommonFunction.h"
#include "Def_Equipment_Type.h"
#include "Def_Language.h"

static LPCTSTR g_szNGCode_Static_T[Lang_MaxCount][CDlg_ModifyNGCode::Item_MaxCount] =
{
	// 한국어
	{
		_T("코드"),				// Code (NGCode	)
		_T("섹션"),				// Section (NG구분)	
		_T("재시도 방법"),		// Retry Mode	
		_T("재검사 횟수"),		// Retry Count	
		_T("이동 후 재시도"),		// Move Retry	
		_T("초기화 종류"),		// Reset	
		_T("연속 NG 알람 횟수"),	// Alarm Count(연속NG알람수)
		_T("설명"),				// Descript
	},

	// 영어
	{
		_T("Code"),				// Code (NGCode	)
		_T("Section"),			// Section (NG구분)	
		_T("Retry Mode"),		// Retry Mode	
		_T("Retry Count"),		// Retry Count	
		_T("Move Retry"),		// Move Retry	
		_T("Reset"),			// Reset	
		_T("Alarm Count"),		// Alarm Count(연속NG알람수)
		_T("Descript"),			// Descript
	},

	// 베트남어
	{
		_T("Code"),				// Code (NGCode	)
		_T("Section"),			// Section (NG구분)	
		_T("Retry Mode"),		// Retry Mode	
		_T("Retry Count"),		// Retry Count	
		_T("Move Retry"),		// Move Retry	
		_T("Reset"),			// Reset	
		_T("Alarm Count"),		// Alarm Count(연속NG알람수)
		_T("Descript"),			// Descript
	},

	// 중국어
	{
		_T("代码"),				// Code (NGCode	)
		_T("科室"),				// Section (NG구분)	
		_T("重试方法"),			// Retry Mode	
		_T("重试数量"),			// Retry Count	
		_T("移动后重试方法"),		// Move Retry	
		_T("初始化类型"),		// Reset	
		_T("连续报警数量"),		// Alarm Count(연속NG알람수)
		_T("说明"),				// Descript
	},
};

static LPCTSTR g_szCtrlText_T[Lang_MaxCount][CDlg_ModifyNGCode::Txt_MaxCount] =
{
	// 한국어
	{
		_T("NG Code 수정"),		//Txt_DialogCaption
		_T("적용"),				//Txt_OK
		_T("취소"),				//Txt_Cancel
	},

	// 영어
	{
		_T("Modify NG Code"),	//Txt_DialogCaption
		_T("Apply"),			//Txt_OK
		_T("Cancel"),			//Txt_Cancel
	},

	// 베트남어
	{
		_T("Modify NG Code"),	//Txt_DialogCaption
		_T("Apply"),			//Txt_OK
		_T("Cancel"),			//Txt_Cancel
	},

	// 중국어
	{
		_T("修改 NG 代码"),		//Txt_DialogCaption
		_T("适用"),				// Txt_OK
		_T("撤消"),				// Txt_Cancel
	},
};


#define		IDC_BN_ADD				1040
#define		IDC_BN_REMOVE			1041

#define		IDC_ED_ITEM_FST			1000
#define		IDC_ED_ITEM_LST			IDC_ED_ITEM_FST + Item_MaxCount - 1


//=============================================================================
// CDlg_ModifyNGCode 대화 상자입니다.
//=============================================================================
IMPLEMENT_DYNAMIC(CDlg_ModifyNGCode, CDialogEx)

//=============================================================================
// 생성자
//=============================================================================
CDlg_ModifyNGCode::CDlg_ModifyNGCode(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlg_ModifyNGCode::IDD, pParent)
{
	VERIFY(m_font_Default.CreateFont(
		32,						// nHeight
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
// 소멸자
//=============================================================================
CDlg_ModifyNGCode::~CDlg_ModifyNGCode()
{
	m_font_Default.DeleteObject();
	m_font_Data.DeleteObject();
}

void CDlg_ModifyNGCode::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlg_ModifyNGCode, CDialogEx)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

//=============================================================================
// CDlg_ModifyNGCode 메시지 처리기입니다.
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
int CDlg_ModifyNGCode::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	DWORD dwStyle = WS_VISIBLE | WS_CHILD /*| WS_CLIPCHILDREN*/ | WS_CLIPSIBLINGS;
	DWORD dwTempStyle = dwStyle;
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	SetWindowText(g_szCtrlText_T[m_nLanguage][Txt_DialogCaption]);

	m_bn_OK.Create(g_szCtrlText_T[m_nLanguage][Txt_OK], dwStyle | WS_TABSTOP | BS_PUSHBUTTON, rectDummy, this, IDOK);
	m_bn_OK.SetFont(&m_font_Default);
	m_bn_Cancel.Create(g_szCtrlText_T[m_nLanguage][Txt_Cancel], dwStyle | WS_TABSTOP | BS_PUSHBUTTON, rectDummy, this, IDCANCEL);
	m_bn_Cancel.SetFont(&m_font_Default);

	for (UINT nIdx = 0; nIdx < Item_MaxCount; nIdx++)
	{
		m_st_Item[nIdx].SetStaticStyle(CVGStatic::StaticStyle_Title_Alt);
		m_st_Item[nIdx].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_st_Item[nIdx].SetFont_Gdip(L"Arial", 9.0F);
		m_st_Item[nIdx].Create(g_szNGCode_Static_T[m_nLanguage][nIdx], dwStyle | SS_CENTER | SS_CENTERIMAGE, rectDummy, this, IDC_STATIC);

		m_ed_Item[nIdx].Create(dwStyle | ES_CENTER | ES_AUTOHSCROLL | WS_BORDER, rectDummy, this, IDC_ED_ITEM_FST + nIdx);
		m_ed_Item[nIdx].SetFont(&m_font_Data);
	}

	for (UINT nIdx = 0; nIdx < Item_Description; nIdx++)
	{
		m_ed_Item[nIdx].SetValidChars(_T("0123456789-"));
	}

	m_ed_Item[Item_NG_Code].EnableWindow(FALSE);

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
void CDlg_ModifyNGCode::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if ((0 == cx) || (0 == cy))
		return;

	int iMargin			= 10;
	int iSpacing		= 5;
	int iCateSpacing	= 10;
	int iLeft			= iMargin;
	int iTop			= iMargin;
	int iWidth			= cx - iMargin - iMargin;
	int iHeight			= cy - iMargin - iMargin;
	int iCtrlHeight		= 40;
	int iCtrlWidth		= iWidth; // 400;
	int iStWidth		= 140;
	int iEdWidth		= iCtrlWidth - iStWidth - iSpacing;
	int iLeftSub		= iLeft + iStWidth + iSpacing;

	// 아이템
	iLeft = iMargin;
#ifdef NG_CODE_MINI_VERSION
	for (UINT nIdx = 0; nIdx < Item_MaxCount; nIdx++)
	{
		switch (nIdx)
		{
		case Item_NG_Section:	// Section (NG구분)	
		case Item_Retry_Mode:	// Retry Mode	
		case Item_Move_Retry:	// Move Retry	
		case Item_Reset:		// Reset	
			m_st_Item[nIdx].MoveWindow(iLeft, iTop, iStWidth, 0);
			m_ed_Item[nIdx].MoveWindow(iLeftSub, iTop, iEdWidth, 0);
			break;

		default:
			m_st_Item[nIdx].MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
			m_ed_Item[nIdx].MoveWindow(iLeftSub, iTop, iEdWidth, iCtrlHeight);

			iTop += iCtrlHeight + iSpacing;
			break;
		}
	}
#else
	for (UINT nIdx = 0; nIdx < Item_MaxCount; nIdx++)
	{
		m_st_Item[nIdx].MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
		m_ed_Item[nIdx].MoveWindow(iLeftSub, iTop, iEdWidth, iCtrlHeight);

		iTop += iCtrlHeight + iSpacing;
	}
#endif
	
	// 버튼
	iCtrlWidth	= iWidth / 3;
	iCtrlHeight = 80;
	iLeft		= iMargin;
	iLeftSub	= cx - iMargin - iCtrlWidth;
	iTop		= cy - iMargin - iCtrlHeight;
	m_bn_OK.MoveWindow(iLeft, iTop, iCtrlWidth, iCtrlHeight);
	m_bn_Cancel.MoveWindow(iLeftSub, iTop, iCtrlWidth, iCtrlHeight);
}

//=============================================================================
// Method		: OnInitDialog
// Access		: virtual protected  
// Returns		: BOOL
// Qualifier	:
// Last Update	: 2021/11/10 - 14:26
// Desc.		:
//=============================================================================
BOOL CDlg_ModifyNGCode::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetBackgroundColor(RGB(0xFF, 0xFF, 0xFF));

	Set_UIData(&m_stNGCode);

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
BOOL CDlg_ModifyNGCode::PreTranslateMessage(MSG* pMsg)
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
void CDlg_ModifyNGCode::OnCancel()
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
void CDlg_ModifyNGCode::OnOK()
{
	// UI 데이터 저장
	if (Get_UIData(m_stNGCode))
	{
		CDialogEx::OnOK();
	}
}

//=============================================================================
// Method		: Get_UIData
// Access		: protected  
// Returns		: bool
// Parameter	: __out ST_NGCode & OUT_stNGCode
// Qualifier	:
// Last Update	: 2022/1/7 - 13:08
// Desc.		:
//=============================================================================
bool CDlg_ModifyNGCode::Get_UIData(__out ST_NGCode& OUT_stNGCode)
{
	CString szValue;

	//m_ed_Item[Item_NG_Code].GetWindowText(szValue);
	//OUT_stNGCode.m_nNG_Code = _ttoi(szValue.GetBuffer());
	OUT_stNGCode.m_nNG_Code = m_stNGCode.m_nNG_Code;

	m_ed_Item[Item_NG_Section].GetWindowText(szValue);
	OUT_stNGCode.m_nNG_Section = static_cast<uint16_t>(_ttoi(szValue.GetBuffer()));

	m_ed_Item[Item_Retry_Mode].GetWindowText(szValue);
	OUT_stNGCode.m_nRetry_Mode = static_cast<uint16_t>(_ttoi(szValue.GetBuffer()));

	m_ed_Item[Item_RetryCnt].GetWindowText(szValue);
	OUT_stNGCode.m_nRetryCnt = static_cast<uint16_t>(_ttoi(szValue.GetBuffer()));

	m_ed_Item[Item_Move_Retry].GetWindowText(szValue);
	OUT_stNGCode.m_nMove_Retry = static_cast<uint16_t>(_ttoi(szValue.GetBuffer()));

	m_ed_Item[Item_Reset].GetWindowText(szValue);
	OUT_stNGCode.m_nReset = static_cast<uint16_t>(_ttoi(szValue.GetBuffer()));

	m_ed_Item[Item_AlarmCnt].GetWindowText(szValue);
	OUT_stNGCode.m_nAlarmCnt = static_cast<uint16_t>(_ttoi(szValue.GetBuffer()));

	m_ed_Item[Item_Description].GetWindowText(szValue);
	OUT_stNGCode.m_szDescription = szValue;

	return true;
}

//=============================================================================
// Method		: Set_UIData
// Access		: protected  
// Returns		: void
// Parameter	: __in const ST_NGCode * IN_pNGCode
// Qualifier	:
// Last Update	: 2022/1/7 - 13:39
// Desc.		:
//=============================================================================
void CDlg_ModifyNGCode::Set_UIData(__in const ST_NGCode* IN_pNGCode)
{
	CString szValue;

	szValue.Format(_T("%d"), IN_pNGCode->m_nNG_Code);
	m_ed_Item[Item_NG_Code].SetWindowText(szValue);

	szValue.Format(_T("%d"), IN_pNGCode->m_nNG_Section);
	m_ed_Item[Item_NG_Section].SetWindowText(szValue);

	szValue.Format(_T("%d"), IN_pNGCode->m_nRetry_Mode);
	m_ed_Item[Item_Retry_Mode].SetWindowText(szValue);

	szValue.Format(_T("%d"), IN_pNGCode->m_nRetryCnt);
	m_ed_Item[Item_RetryCnt].SetWindowText(szValue);

	szValue.Format(_T("%d"), IN_pNGCode->m_nMove_Retry);
	m_ed_Item[Item_Move_Retry].SetWindowText(szValue);

	szValue.Format(_T("%d"), IN_pNGCode->m_nReset);
	m_ed_Item[Item_Reset].SetWindowText(szValue);

	szValue.Format(_T("%d"), IN_pNGCode->m_nAlarmCnt);
	m_ed_Item[Item_AlarmCnt].SetWindowText(szValue);

	szValue = IN_pNGCode->m_szDescription;
	m_ed_Item[Item_Description].SetWindowText(szValue);

}

//=============================================================================
// Method		: OnLanguage
// Access		: public  
// Returns		: void
// Parameter	: __in uint8_t IN_nLanguage
// Qualifier	:
// Last Update	: 2022/7/20 - 9:46
// Desc.		:
//=============================================================================
void CDlg_ModifyNGCode::OnLanguage(__in uint8_t IN_nLanguage)
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
		m_font_Data.GetLogFont(&lf);
		lf.lfCharSet = Get_CharSet(m_nLanguage);
		m_font_Data.DeleteObject();
		m_font_Data.CreateFontIndirect(&lf);

		m_bn_OK.SetFont(&m_font_Default);
		m_bn_Cancel.SetFont(&m_font_Default);
		for (UINT nIdx = 0; nIdx < Item_MaxCount; nIdx++)
		{
			m_ed_Item[nIdx].SetFont(&m_font_Data);
		}

		if (GetSafeHwnd())
		{
			SetWindowText(g_szCtrlText_T[m_nLanguage][Txt_DialogCaption]);
			m_bn_OK.SetWindowText(g_szCtrlText_T[m_nLanguage][Txt_OK]);
			m_bn_Cancel.SetWindowText(g_szCtrlText_T[m_nLanguage][Txt_Cancel]);

			for (UINT nIdx = 0; nIdx < Item_MaxCount; nIdx++)
			{
				m_st_Item[nIdx].SetText(g_szNGCode_Static_T[m_nLanguage][nIdx]);
			}
		}
	}
}

//=============================================================================
// Method		: Set_NGCodeData
// Access		: public  
// Returns		: void
// Parameter	: __in const ST_NGCode * IN_pNGCode
// Qualifier	:
// Last Update	: 2021/11/10 - 14:27
// Desc.		:
//=============================================================================
void CDlg_ModifyNGCode::Set_NGCodeData(__in const ST_NGCode* IN_pNGCode)
{
	m_stNGCode = *IN_pNGCode;
}

//=============================================================================
// Method		: Get_NGCodeData
// Access		: public  
// Returns		: ST_NGCode&
// Qualifier	:
// Last Update	: 2021/11/10 - 14:27
// Desc.		:
//=============================================================================
ST_NGCode& CDlg_ModifyNGCode::Get_NGCodeData()
{
	return m_stNGCode;
}

