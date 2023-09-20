//*****************************************************************************
// Filename	: 	Wnd_Judgment.cpp
// Created	:	2021/11/19 - 11:14
// Modified	:	2021/11/19 - 11:14
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************

#include "stdafx.h"
#include "Wnd_Judgment.h"
#include "Def_Language.h"

static LPCTSTR g_szHeader_T[Lang_MaxCount][CWnd_Judgment::HI_MaxCount] =
{
	// 한국어
	{
		_T("최종 판정"),		// HI_Judgment,		// 1. Judgment
		_T("NG 코드"),		// HI_NG_Code,		// 2. NG Code
		_T("NG 판정 검사"),	// HI_NG_Equipment,	// 3. Equipment
		_T("NG 판정 파라"),	// HI_NG_Para,		// 4. Para
		_T("RFID"),			// HI_RFID,			// 5. RFID
		_T("S/N"),			// HI_Barcode,		// 6. S/N
		_T("진행 시간"),		// HI_OutTime,		// 7. Time
	},

	// 영어
	{
		_T("Judgment"),		// HI_Judgment,		// 1. Judgment
		_T("NG Code"),		// HI_NG_Code,		// 2. NG Code
		_T("NG Test"),		// HI_NG_Equipment,	// 3. Equipment
		_T("NG Para"),		// HI_NG_Para,		// 4. Para
		_T("RFID"),			// HI_RFID,			// 5. RFID
		_T("S/N"),			// HI_Barcode,		// 6. S/N
		_T("Time"),			// HI_OutTime,		// 7. Time
	},

	// 베트남어
	{
		_T("Judgment"),		// HI_Judgment,		// 1. Judgment
		_T("NG Code"),		// HI_NG_Code,		// 2. NG Code
		_T("NG Test"),		// HI_NG_Equipment,	// 3. Equipment
		_T("NG Para"),		// HI_NG_Para,		// 4. Para
		_T("RFID"),			// HI_RFID,			// 5. RFID
		_T("S/N"),			// HI_Barcode,		// 6. S/N
		_T("Time"),			// HI_OutTime,		// 7. Time
	},

	// 중국어
	{
		_T("判断"),			// HI_Judgment,		// 1. Judgment
		_T("NG码"),			// HI_NG_Code,		// 2. NG Code
		_T("NG测试"),		// HI_NG_Equipment,	// 3. Equipment
		_T("NG位置"),		// HI_NG_Para,		// 4. Para
		_T("RFID"),			// HI_RFID,			// 5. RFID (射频识别)
		_T("序列号"),		// HI_Barcode,		// 6. S/N
		_T("进行时间"),		// HI_OutTime,		// 7. Time
	},
};

static LPCTSTR g_szParaText_T[Lang_MaxCount][Para_MaxCount] =
{
	// 한국어
	{
		_T("왼쪽 포트"),		// Para_Left,
		_T("오른쪽 포트"),	// Para_Right,
		_T("중앙 포트"),		// Para_Center,
	},

	// 영어
	{
		_T("Left Port"),	// Para_Left,
		_T("Right Port"),	// Para_Right,
		_T("Center Port"),	// Para_Center,
	},

	// 베트남어
	{
		_T("Left Port"),	// Para_Left,
		_T("Right Port"),	// Para_Right,
		_T("Center Port"),	// Para_Center,
	},

	// 중국어
	{
		_T("左港"),			// Para_Left,
		_T("右港"),			// Para_Right,
		_T("中心港"),		// Para_Center,
	},
};

enum enJudge_Status
{
	Judge_Ready,
	Judge_Pass,
	Judge_NG,

	Judge_MaxCount,
};

static LPCTSTR g_szJudge_T[Lang_MaxCount][Para_MaxCount] =
{
	// 한국어
	{
		_T("READY"),		// Judge_Ready
		_T("PASS"),			// Judge_Pass
		_T("NG"),			// Judge_NG
	},

	// 영어
	{
		_T("READY"),		// Judge_Ready
		_T("PASS"),			// Judge_Pass
		_T("NG"),			// Judge_NG
	},

	// 베트남어
	{
		_T("READY"),		// Judge_Ready
		_T("PASS"),			// Judge_Pass
		_T("NG"),			// Judge_NG
	},

	// 중국어
	{
		_T("结果"),			// Judge_Ready
		_T("过关"),			// Judge_Pass
		_T("失败"),			// Judge_NG
	},
};


#define RGB_PASS		RGB(200, 255, 200)
#define RGB_FAIL		RGB(255, 200, 200)

#define RGB_ENABLE		RGB(255, 255, 255)
#define RGB_DISABLE		RGB(100, 100, 100)

//-----------------------------------------------------------------------------
// CWnd_Judgment
//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CWnd_Judgment, CWnd)

CWnd_Judgment::CWnd_Judgment()
{
	m_Margin.left	= 0;
	m_Margin.top	= 0;
	m_Margin.right	= 0;
	m_Margin.bottom	= 0;
	m_nSpacing		= 5;
	m_nCateSpacing	= 10;

	m_nFontSize_Caption.SetSize(0, 13);
	m_nFontSize_Data.SetSize(0, 13);
}

CWnd_Judgment::~CWnd_Judgment()
{
	Release_Font();

}

BEGIN_MESSAGE_MAP(CWnd_Judgment, CWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CWnd_Judgment message handlers
//=============================================================================
// Method		: OnCreate
// Access		: protected  
// Returns		: int
// Parameter	: LPCREATESTRUCT lpCreateStruct
// Qualifier	:
// Last Update	: 2021/11/19 - 11:14
// Desc.		:
//=============================================================================
int CWnd_Judgment::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	Set_Background (RGB(0xFF, 0xFF, 0xFF));
	Create_Font();

 	DWORD dwStyle = WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
 	CRect rectDummy;
 	rectDummy.SetRectEmpty();


// 	m_st_Title.SetFont_Gdip(L"Arial", 8.0F);
// 	m_st_Title.SetBorderTickness(1);
// 	m_st_Title.SetTextAlignment(StringAlignmentNear);
// 	m_st_Title.Create(_T(""), dwStyle, rectDummy, this, IDC_STATIC);

	m_st_Header[HI_Judgment].SetFont_Gdip(L"Arial", 16.0F);
	m_st_Header[HI_Judgment].Create(g_szHeader_T[m_nLanguage][HI_Judgment], dwStyle | SS_CENTER | SS_CENTERIMAGE, rectDummy, this, IDC_STATIC);
	m_st_Data[HI_Judgment].SetFont_Gdip(L"Arial", 84.0F);
	m_st_Data[HI_Judgment].Create(g_szHeader_T[m_nLanguage][HI_Judgment], dwStyle | SS_CENTER | SS_CENTERIMAGE, rectDummy, this, IDC_STATIC);
	m_st_Data[HI_Judgment].SetText(_T("READY"));

	for (uint8_t nIdx = HI_NG_Code; nIdx < HI_MaxCount; nIdx++)
	{
		m_st_Header[nIdx].SetFont_Gdip(L"Arial", 24.0F);
		m_st_Header[nIdx].SetBackColor_COLORREF(RGB(40, 40, 40));
		m_st_Header[nIdx].SetTextColor_COLORREF(RGB(255, 255, 255), RGB(255, 255, 255), RGB(255, 255, 255));
		m_st_Header[nIdx].Create(g_szHeader_T[m_nLanguage][nIdx], dwStyle | SS_CENTER | SS_CENTERIMAGE, rectDummy, this, IDC_STATIC);
	}

	float fFontList[] =
	{
		24.0f, // 1. HI_Judgment
		24.0f, // 2. HI_NG_Code
		24.0f, // 3. HI_NG_Equipment
		24.0f, // 4. HI_NG_Para
		24.0f, // 5. HI_RFID
		18.0f, // 6. HI_Barcode
		22.0f, // 7. HI_OutTime
	};

	for (uint8_t nIdx = HI_NG_Code; nIdx < HI_MaxCount; nIdx++)
	{
		m_st_Data[nIdx].SetFont_Gdip(L"Arial", fFontList[nIdx]);
		m_st_Data[nIdx].Create(_T(""), dwStyle | SS_CENTER | SS_CENTERIMAGE, rectDummy, this, IDC_STATIC);
	}

#ifdef _DEBUG
	m_st_Data[HI_NG_Code]		.SetText(_T("999"));
	m_st_Data[HI_NG_Equipment]	.SetText(_T("Ghost Flare"));
	m_st_Data[HI_NG_Para]		.SetText(_T("Eqp 00: Right Port"));
	m_st_Data[HI_RFID]			.SetText(_T("H-220215-001"));
	m_st_Data[HI_Barcode]		.SetText(_T("P1637041-00-A:SSMS220080000003"));
	m_st_Data[HI_OutTime]		.SetText(_T("11:11:11 -> 22:22:22 \n(2000.000 sec)"));
#endif

	// 초기값 (테스트 용도)
	//m_st_Data[nIdx].SetText(_T("0"));

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
// Last Update	: 2021/11/19 - 11:14
// Desc.		:
//=============================================================================
void CWnd_Judgment::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	if ((cx == 0) && (cy == 0))
		return;

	int iSpacing		= 10;
	int iLeft			= 0;
	int iTop			= 0;
	int iWidth			= cx;
	int iHeight			= cy;

	int iJudgeWidth		= iWidth / 4;
	
	int iHeadWidth		= (iWidth - iJudgeWidth) / 2 / 3;
	int iDataWidth		= (iWidth - iJudgeWidth) / 2 - iHeadWidth;
	int iUnitHeight		= iHeight / 3;
	
	int iJudgeHeight	= iUnitHeight * 3 - 2;
	iJudgeWidth			= iWidth - (iHeadWidth * 2 + iDataWidth * 2) + 4;

	m_st_Header[HI_Judgment].MoveWindow(iLeft, iTop, iJudgeWidth, 0);
	m_st_Data[HI_Judgment].MoveWindow(iLeft, iTop, iJudgeWidth, iJudgeHeight);

	iLeft += (iJudgeWidth - 1);
	int iLeftSub = iLeft + iHeadWidth - 1;
	m_st_Header[HI_NG_Code].MoveWindow(iLeft, iTop, iHeadWidth, iUnitHeight);
	m_st_Data[HI_NG_Code].MoveWindow(iLeftSub, iTop, iDataWidth, iUnitHeight);

	iTop += iUnitHeight - 1;
	m_st_Header[HI_NG_Equipment].MoveWindow(iLeft, iTop, iHeadWidth, iUnitHeight);
	m_st_Data[HI_NG_Equipment].MoveWindow(iLeftSub, iTop, iDataWidth, iUnitHeight);

	iTop += iUnitHeight - 1;
	m_st_Header[HI_NG_Para].MoveWindow(iLeft, iTop, iHeadWidth, iUnitHeight);
	m_st_Data[HI_NG_Para].MoveWindow(iLeftSub, iTop, iDataWidth, iUnitHeight);
	

	iTop = 0;
	iLeft = iLeftSub + iDataWidth - 1;
	iLeftSub = iLeft + iHeadWidth - 1;
	m_st_Header[HI_RFID].MoveWindow(iLeft, iTop, iHeadWidth, iUnitHeight);
	m_st_Data[HI_RFID].MoveWindow(iLeftSub, iTop, iDataWidth, iUnitHeight);

	iTop += iUnitHeight - 1;
	m_st_Header[HI_Barcode].MoveWindow(iLeft, iTop, iHeadWidth, iUnitHeight);
	m_st_Data[HI_Barcode].MoveWindow(iLeftSub, iTop, iDataWidth, iUnitHeight);

	iTop += iUnitHeight - 1;
	m_st_Header[HI_OutTime].MoveWindow(iLeft, iTop, iHeadWidth, iUnitHeight);
	m_st_Data[HI_OutTime].MoveWindow(iLeftSub, iTop, iDataWidth, iUnitHeight);

}

//=============================================================================
// Method		: OnEraseBkgnd
// Access		: protected  
// Returns		: BOOL
// Parameter	: CDC * pDC
// Qualifier	:
// Last Update	: 2021/11/19 - 11:15
// Desc.		:
//=============================================================================
BOOL CWnd_Judgment::OnEraseBkgnd(CDC* pDC)
{
	if (m_brBkgr.GetSafeHandle() == NULL)
	{
		return CWnd::OnEraseBkgnd(pDC);
	}

	ASSERT_VALID(pDC);

	CRect rectClient;
	GetClientRect(rectClient);

	if (m_brBkgr.GetSafeHandle() != NULL)
	{
		pDC->FillRect(rectClient, &m_brBkgr);
	}
	else
	{
		CWnd::OnEraseBkgnd(pDC);
	}

	return TRUE;
}

//=============================================================================
// Method		: OnCtlColor
// Access		: protected  
// Returns		: HBRUSH
// Parameter	: CDC * pDC
// Parameter	: CWnd * pWnd
// Parameter	: UINT nCtlColor
// Qualifier	:
// Last Update	: 2021/11/19 - 11:15
// Desc.		:
//=============================================================================
HBRUSH CWnd_Judgment::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (m_brBkgr.GetSafeHandle() != NULL)
	{
#define AFX_MAX_CLASS_NAME 255
#define AFX_STATIC_CLASS _T("Static")
#define AFX_BUTTON_CLASS _T("Button")

		if (nCtlColor == CTLCOLOR_STATIC)
		{
			TCHAR lpszClassName[AFX_MAX_CLASS_NAME + 1];

			::GetClassName(pWnd->GetSafeHwnd(), lpszClassName, AFX_MAX_CLASS_NAME);
			CString strClass = lpszClassName;

			if (strClass == AFX_STATIC_CLASS)
			{
				pDC->SetBkMode(TRANSPARENT);
				return(HBRUSH) ::GetStockObject(HOLLOW_BRUSH);
			}

			if (strClass == AFX_BUTTON_CLASS)
			{
				if ((pWnd->GetStyle() & BS_GROUPBOX) == 0)
				{
					pDC->SetBkMode(TRANSPARENT);
				}

				return(HBRUSH) ::GetStockObject(HOLLOW_BRUSH);
			}
		}
	}

	return CWnd::OnCtlColor(pDC, pWnd, nCtlColor);
}

//=============================================================================
// Method		: PreTranslateMessage
// Access		: virtual protected  
// Returns		: BOOL
// Parameter	: MSG * pMsg
// Qualifier	:
// Last Update	: 2021/11/19 - 11:15
// Desc.		:
//=============================================================================
BOOL CWnd_Judgment::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	return __super::PreTranslateMessage(pMsg);
}

//=============================================================================
// Method		: Create_Font
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/11/19 - 11:15
// Desc.		:
//=============================================================================
void CWnd_Judgment::Create_Font()
{
	VERIFY(m_font_Default.CreateFont(
		m_nFontSize_Caption.cy,	// nHeight
		m_nFontSize_Caption.cx,	// nWidth
		0,						// nEscapement
		0,						// nOrientation
		FW_NORMAL,				// nWeight
		FALSE,					// bItalic
		FALSE,					// bUnderline
		0,						// cStrikeOut
		DEFAULT_CHARSET,		// nCharSet
		OUT_DEFAULT_PRECIS,		// nOutPrecision
		CLIP_DEFAULT_PRECIS,	// nClipPrecision
		DEFAULT_QUALITY,		// nQuality
		DEFAULT_PITCH,			// nPitchAndFamily
		_T("Tahoma")));			// lpszFacename

	VERIFY(m_font_Data.CreateFont(
		m_nFontSize_Data.cy,	// nHeight
		m_nFontSize_Data.cx,	// nWidth
		0,						// nEscapement
		0,						// nOrientation
		FW_NORMAL,				// nWeight
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
// Method		: Release_Font
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/11/19 - 11:15
// Desc.		:
//=============================================================================
void CWnd_Judgment::Release_Font()
{
	m_font_Default.DeleteObject();
	m_font_Data.DeleteObject();
}

//=============================================================================
// Method		: Init_TestResult
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2022/7/20 - 14:47
// Desc.		:
//=============================================================================
void CWnd_Judgment::Init_TestResult()
{
	// 초기 화면 표시
	m_st_Data[HI_NG_Code].SetText(_T(""));
	m_st_Data[HI_Judgment].SetBackColor_COLORREF(RGB(255, 255, 255));
	m_st_Data[HI_Judgment].SetText(g_szJudge_T[m_nLanguage][Judge_Ready]);
	m_st_Data[HI_NG_Equipment].SetText(_T(""));
	m_st_Data[HI_NG_Para].SetText(_T(""));
	m_st_Data[HI_RFID].SetText(_T(""));
	m_st_Data[HI_Barcode].SetText(_T(""));
	m_st_Data[HI_OutTime].SetText(_T(""));
}

//=============================================================================
// Method		: OnLanguage
// Access		: public  
// Returns		: void
// Parameter	: __in uint8_t IN_nLanguage
// Qualifier	:
// Last Update	: 2022/7/20 - 14:47
// Desc.		:
//=============================================================================
void CWnd_Judgment::OnLanguage(__in uint8_t IN_nLanguage)
{
	if (m_nLanguage != IN_nLanguage)
	{
		m_nLanguage = IN_nLanguage;

		if (GetSafeHwnd())
		{
			m_st_Header[HI_Judgment].SetText(g_szHeader_T[m_nLanguage][HI_Judgment]);
			//m_st_Data[HI_Judgment].SetText(g_szHeader_T[m_nLanguage][HI_Judgment]);

			for (uint8_t nIdx = HI_NG_Code; nIdx < HI_MaxCount; nIdx++)
			{
				m_st_Header[nIdx].SetText(g_szHeader_T[m_nLanguage][nIdx]);
			}
		}
	}
}

//=============================================================================
// Method		: Set_Background
// Access		: virtual public  
// Returns		: void
// Parameter	: __in COLORREF color
// Parameter	: __in BOOL bRepaint
// Qualifier	:
// Last Update	: 2021/11/19 - 15:28
// Desc.		:
//=============================================================================
void CWnd_Judgment::Set_Background(__in COLORREF color, __in BOOL bRepaint /*= TRUE*/)
{
	if (m_brBkgr.GetSafeHandle() != NULL)
	{
		m_brBkgr.DeleteObject();
	}

	if (color != (COLORREF)-1)
	{
		m_brBkgr.CreateSolidBrush(color);
	}

	if (bRepaint && GetSafeHwnd() != NULL)
	{
		Invalidate();
		UpdateWindow();
	}
}

//=============================================================================
// Method		: Set_Margin
// Access		: public  
// Returns		: void
// Parameter	: __in UINT nLeft
// Parameter	: __in UINT nTop
// Parameter	: __in UINT nRight
// Parameter	: __in UINT nBottom
// Qualifier	:
// Last Update	: 2021/3/12 - 14:10
// Desc.		:
//=============================================================================
void CWnd_Judgment::Set_Margin(__in UINT nLeft, __in UINT nTop, __in UINT nRight, __in UINT nBottom)
{
	m_Margin.left	= nLeft;
	m_Margin.top	= nTop;
	m_Margin.right	= nRight;
	m_Margin.bottom = nBottom;
}

//=============================================================================
// Method		: Set_Spacing
// Access		: public  
// Returns		: void
// Parameter	: __in UINT nSpacing
// Parameter	: __in UINT nCateSpacing
// Qualifier	:
// Last Update	: 2021/3/12 - 14:11
// Desc.		:
//=============================================================================
void CWnd_Judgment::Set_Spacing(__in UINT nSpacing, __in UINT nCateSpacing)
{
	m_nSpacing = nSpacing;
	m_nCateSpacing = nCateSpacing;
}

//=============================================================================
// Method		: Set_FontSize_Caption
// Access		: public  
// Returns		: void
// Parameter	: __in UINT nWidth
// Parameter	: __in UINT nHeghit
// Qualifier	:
// Last Update	: 2021/3/12 - 14:11
// Desc.		:
//=============================================================================
void CWnd_Judgment::Set_FontSize_Caption(__in UINT nWidth, __in UINT nHeghit)
{
	m_nFontSize_Caption.SetSize(nWidth, nHeghit);
}

//=============================================================================
// Method		: Set_FontSize_Data
// Access		: public  
// Returns		: void
// Parameter	: __in UINT nWidth
// Parameter	: __in UINT nHeghit
// Qualifier	:
// Last Update	: 2021/3/12 - 14:11
// Desc.		:
//=============================================================================
void CWnd_Judgment::Set_FontSize_Data(__in UINT nWidth, __in UINT nHeghit)
{
	m_nFontSize_Data.SetSize(nWidth, nHeghit);
}

//=============================================================================
// Method		: Set_TestResult
// Access		: public  
// Returns		: void
// Parameter	: __in LPCTSTR IN_szRFID
// Parameter	: __in LPCTSTR IN_szBarcode
// Parameter	: __in ST_TestResult * IN_pstResult
// Qualifier	:
// Last Update	: 2022/2/15 - 18:27
// Desc.		:
//=============================================================================
void CWnd_Judgment::Set_TestResult(__in LPCTSTR IN_szRFID, __in LPCTSTR IN_szBarcode, __in ST_TestResult* IN_pstResult)
{
	if ((nullptr != IN_pstResult) && GetSafeHwnd())
	{
		CString szText;

		// NG Code
		szText.Format(_T("%d"), IN_pstResult->m_nNG_Code);
		m_st_Data[HI_NG_Code].SetText(szText.GetBuffer());

		if (0 == IN_pstResult->m_nNG_Code)
		{
			m_st_Data[HI_Judgment].SetBackColor_COLORREF(RGB(0, 192, 0));
			m_st_Data[HI_Judgment].SetText(g_szJudge_T[m_nLanguage][Judge_Pass]);
			//m_st_Data[HI_Judgment].SetText(_T("EMPTY"));

			m_st_Data[HI_NG_Equipment].SetText(_T(""));

			m_st_Data[HI_NG_Para].SetText(_T(""));
		}
		else
		{
			m_st_Data[HI_Judgment].SetBackColor_COLORREF(RGB(192, 0, 0));
			m_st_Data[HI_Judgment].SetText(g_szJudge_T[m_nLanguage][Judge_NG]);

			// NG 검사 유형
			szText.Format(_T("%s"), g_szEqpTypeName[IN_pstResult->m_nNG_EqpType]);
			m_st_Data[HI_NG_Equipment].SetText(szText.GetBuffer());

			// NG 설비 파라
			if ((0 <= IN_pstResult->m_nNG_Para) && (IN_pstResult->m_nNG_Para < Para_MaxCount))
			{
				szText.Format(_T("Eqp.%02d : %s"), IN_pstResult->m_nNG_EqpOrder, g_szParaText_T[m_nLanguage][IN_pstResult->m_nNG_Para]);
			}
			else
			{
				szText.Format(_T("Eqp.%02d : Unknown"), IN_pstResult->m_nNG_EqpOrder);
			}
			m_st_Data[HI_NG_Para].SetText(szText.GetBuffer());
		}
		
		if (IN_szRFID)
		{
			m_st_Data[HI_RFID].SetText(IN_szRFID);
		}
		else
		{
			m_st_Data[HI_RFID].SetText(_T(""));
		}

		if (IN_szBarcode)
		{
			m_st_Data[HI_Barcode].SetText(IN_szBarcode);
		}
		else
		{
			m_st_Data[HI_Barcode].SetText(_T(""));
		}

		SYSTEMTIME tmOut;
		GetLocalTime(&tmOut);
		szText.Format(_T("%02d:%02d:%02d"), tmOut.wHour, tmOut.wMinute, tmOut.wSecond);
		m_st_Data[HI_OutTime].SetText(szText.GetBuffer());

	}
}

void CWnd_Judgment::Set_TestResult(const CSocketInfo_Unit * IN_Socket)
{
	if (GetSafeHwnd())
	{
		if (nullptr != IN_Socket)
		{
			CString szText;

			// NG Code
			szText.Format(_T("%d"), IN_Socket->m_stTestResult.m_nNG_Code);
			m_st_Data[HI_NG_Code].SetText(szText.GetBuffer());

			if (0 == IN_Socket->m_stTestResult.m_nNG_Code) // PASS
			{
				// 초기화된 소켓
				if ((0 == IN_Socket->m_stTestResult.m_nNG_EqpOrder) && 
					(IN_Socket->m_stTestResult.m_szNG_EqpID.IsEmpty()) &&
					(0 == IN_Socket->m_stTestResult.m_nNG_EqpType))
				{
					Init_TestResult();
				}
				else // PASS
				{
					m_st_Data[HI_Judgment].SetBackColor_COLORREF(RGB(0, 192, 0));
					m_st_Data[HI_Judgment].SetText(g_szJudge_T[m_nLanguage][Judge_Pass]);
					//m_st_Data[HI_Judgment].SetText(_T("EMPTY"));

					m_st_Data[HI_NG_Equipment].SetText(_T(""));

					m_st_Data[HI_NG_Para].SetText(_T(""));
				}
			}
			else // NG
			{
				m_st_Data[HI_Judgment].SetBackColor_COLORREF(RGB(192, 0, 0));
				m_st_Data[HI_Judgment].SetText(g_szJudge_T[m_nLanguage][Judge_NG]);

				// NG 검사 유형
				szText.Format(_T("%s"), g_szEqpTypeName[IN_Socket->m_stTestResult.m_nNG_EqpType]);
				m_st_Data[HI_NG_Equipment].SetText(szText.GetBuffer());

				// NG 설비 파라
				if ((0 <= IN_Socket->m_stTestResult.m_nNG_Para) && (IN_Socket->m_stTestResult.m_nNG_Para < Para_MaxCount))
				{
					szText.Format(_T("Eqp.%02d : %s"), IN_Socket->m_stTestResult.m_nNG_EqpOrder, g_szParaText_T[m_nLanguage][IN_Socket->m_stTestResult.m_nNG_Para]);
				}
				else
				{
					szText.Format(_T("Eqp.%02d : Unknown"), IN_Socket->m_stTestResult.m_nNG_EqpOrder);
				}
				m_st_Data[HI_NG_Para].SetText(szText.GetBuffer());
			}

			szText = IN_Socket->szRFID;
			m_st_Data[HI_RFID].SetText(szText.GetBuffer());

			szText = IN_Socket->m_szBarcode;
			m_st_Data[HI_Barcode].SetText(szText.GetBuffer());

			szText.Format(_T("%02d:%02d:%02d -> %02d:%02d:%02d\n(%.1f sec)"),
				IN_Socket->m_tmLoad.wHour, IN_Socket->m_tmLoad.wMinute, IN_Socket->m_tmLoad.wSecond,
				IN_Socket->m_tmUnload.wHour, IN_Socket->m_tmUnload.wMinute, IN_Socket->m_tmUnload.wSecond,
				IN_Socket->m_dCycleTime);
			m_st_Data[HI_OutTime].SetText(szText.GetBuffer());
		}
		else
		{
			// 초기 화면 표시
			Init_TestResult();
		}
	}
}

