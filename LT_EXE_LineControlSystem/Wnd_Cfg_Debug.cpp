//*****************************************************************************
// Filename	: 	Wnd_Cfg_Debug.cpp
// Created	:	2016/3/14 - 10:57
// Modified	:	2016/3/14 - 10:57
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
// Wnd_Cfg_Debug.cpp : implementation file
//

#include "stdafx.h"
#include "Wnd_Cfg_Debug.h"
#include "Def_WindowMessage.h"
#include "RegDebugInfo.h"
#include "Def_Language_Message.h"


static LPCTSTR g_szDebugMode_T[Lang_MaxCount][CWnd_Cfg_Debug::Item_MaxCount] =
{
	// 한국어
	{
		_T("디버그 모드 사용"),					// Item_UseDebugMode,
		_T("바이패스 안함: NG 소켓"),				// Item_NotBypass_NG,
		_T("바이패스 안함: 검사 스킵"),			// Item_NotBypass_TestSkip,
		_T("바이패스 안함: 빈 소켓"),				// Item_NotBypass_Empty
		_T("시프트 변경에 의한 자동 수율 초기화"),	// Item_AutoReset_Yield_byShift,
		_T("소켓 배출 조건"),						// Item_SocketTrackOut_Condition,
		_T("소켓 대기 조건"),						// Item_WaitingSocket_Condition,
		_T("검사 그룹의 설비 선택 방법"),			// Item_MultiTester_SelectMethod,
	},

	// 영어
	{
		_T("Use Debug Mode"),				// Item_UseDebugMode,
		_T("Not Bypass: NG"),				// Item_NotBypass_NG,
		_T("Not Bypass: TestSkip"),			// Item_NotBypass_TestSkip,
		_T("Not Bypass: Empty"),			// Item_NotBypass_Empty
		_T("Auto Reset Yield byShift"),		// Item_AutoReset_Yield_byShift,
		_T("Socket Track Out Condition"),	// Item_SocketTrackOut_Condition,
		_T("Waiting Socket Condition"),		// Item_WaitingSocket_Condition,
		_T("Multi Tester Select Method"),	// Item_MultiTester_SelectMethod,
	},

	// 베트남어
	{
		_T("Use Debug Mode"),				// Item_UseDebugMode,
		_T("Not Bypass: NG"),				// Item_NotBypass_NG,
		_T("Not Bypass: TestSkip"),			// Item_NotBypass_TestSkip,
		_T("Not Bypass: Empty"),			// Item_NotBypass_Empty
		_T("Auto Reset Yield byShift"),		// Item_AutoReset_Yield_byShift,
		_T("Socket Track Out Condition"),	// Item_SocketTrackOut_Condition,
		_T("Waiting Socket Condition"),		// Item_WaitingSocket_Condition,
		_T("Multi Tester Select Method"),	// Item_MultiTester_SelectMethod,
	},

	// 중국어
	{
		_T("使用调试模式"),					// Item_UseDebugMode,
		_T("不旁路: 残次品"),					// Item_NotBypass_NG,
		_T("不旁路: 跳过检查"),				// Item_NotBypass_TestSkip,
		_T("不旁路: 空插口夹具"),				// Item_NotBypass_Empty
		_T("Auto Reset Yield byShift"),		// Item_AutoReset_Yield_byShift,
		_T("Socket Track Out Condition"),	// Item_SocketTrackOut_Condition,
		_T("Waiting Socket Condition"),		// Item_WaitingSocket_Condition,
		_T("Multi Tester Select Method"),	// Item_MultiTester_SelectMethod,
	},
};

static LPCTSTR g_szCtrlText_T[Lang_MaxCount][CWnd_Cfg_Debug::Txt_MaxCount] =
{
	// 한국어
	{
		_T("적용"),				// Txt_Apply
	},

	// 영어
	{
		_T("Apply"),			// Txt_Apply
	},

	// 베트남어
	{
		_T("Apply"),			// Txt_Apply
	},

	// 중국어
	{
		_T("适用"),				// Txt_Apply
	},
};

#define		IDC_BN_APPLY				1000

#define		IDC_CB_TESTER_FST			2000
#define		IDC_CB_TESTER_LST			IDC_CB_TESTER_FST + Item_MaxCount - 1


#define		TABSTYLE_COUNT			10
static UINT g_TabOrder[TABSTYLE_COUNT] =
{
	IDC_BN_APPLY,
	IDC_CB_TESTER_FST, 
	IDC_CB_TESTER_FST + 1, 
	IDC_CB_TESTER_FST + 2,
	IDC_CB_TESTER_FST + 3,
	IDC_CB_TESTER_FST + 4,
	IDC_CB_TESTER_FST + 5,
	IDC_CB_TESTER_FST + 6,
	IDC_CB_TESTER_FST + 7,
	IDC_CB_TESTER_FST + 8,
};

// CWnd_Cfg_Debug
IMPLEMENT_DYNAMIC(CWnd_Cfg_Debug, CWnd_BaseView)

CWnd_Cfg_Debug::CWnd_Cfg_Debug()
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

	VERIFY(m_font_Data.CreateFont(
		30,						// nHeight
		0,						// nWidth
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

CWnd_Cfg_Debug::~CWnd_Cfg_Debug()
{
	m_font_Default.DeleteObject();
	m_font_Data.DeleteObject();
}


BEGIN_MESSAGE_MAP(CWnd_Cfg_Debug, CWnd_BaseView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BN_APPLY,			OnBnClicked_Apply)
	ON_CBN_SELENDOK(IDC_CB_TESTER_FST,	OnCbnSelEndOk_DebugMode)
END_MESSAGE_MAP()


// CWnd_Cfg_Debug message handlers
//=============================================================================
// Method		: OnCreate
// Access		: public  
// Returns		: int
// Parameter	: LPCREATESTRUCT lpCreateStruct
// Qualifier	:
// Last Update	: 2016/3/17 - 11:33
// Desc.		:
//=============================================================================
int CWnd_Cfg_Debug::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd_BaseView::OnCreate(lpCreateStruct) == -1)
		return -1;

	DWORD dwStyle = WS_VISIBLE | WS_CHILD /*| WS_CLIPCHILDREN*/ | WS_CLIPSIBLINGS;
	DWORD dwTempStyle = dwStyle;
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	m_bn_Apply.Create(g_szCtrlText_T[m_nLanguage][Txt_Apply], dwStyle | WS_BORDER | BS_PUSHBUTTON, rectDummy, this, IDC_BN_APPLY);


	for (UINT nIdx = 0; nIdx < Item_MaxCount; nIdx++)
	{
		m_st_Item[nIdx].SetStaticStyle(CVGStatic::StaticStyle_Title_Alt);
		m_st_Item[nIdx].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_st_Item[nIdx].SetFont_Gdip(L"Arial", 9.0F);
		m_st_Item[nIdx].Create(g_szDebugMode_T[m_nLanguage][nIdx], dwStyle | SS_CENTER | SS_CENTERIMAGE, rectDummy, this, IDC_STATIC);
	
		m_cb_Item[nIdx].Create(dwStyle | CBS_DROPDOWNLIST | WS_VSCROLL, rectDummy, this, IDC_CB_TESTER_FST + nIdx);
		m_cb_Item[nIdx].SetFont(&m_font_Data);
	}

	LPCTSTR szUsable[] =
	{
		_T("Not Use (Default)"),	// Deb_NotUse
		_T("Use"),					// Deb_Use,
	};
	for (UINT nIdx = Item_UseDebugMode; nIdx <= Item_AutoReset_Yield_byShift; nIdx++)
	{
		m_cb_Item[nIdx].AddString(szUsable[0]);
		m_cb_Item[nIdx].AddString(szUsable[1]);
		m_cb_Item[nIdx].SetCurSel(0);
	}

	LPCTSTR szSocketTrackOut_Cond[] =
	{
		_T("Default"),				// TO_Default
		_T("Forced Accept"),		// TO_ForcedAccept
	};
	for (auto nIdx = 0; nIdx < TO_MaxCount; ++nIdx)
	{
		m_cb_Item[Item_SocketTrackOut_Condition].AddString(szSocketTrackOut_Cond[nIdx]);
	}
	m_cb_Item[Item_SocketTrackOut_Condition].SetCurSel(0);

	LPCTSTR szWaitingSocket_Cond[] =
	{
		_T("Default"),				// WTO_Default
		_T("Forced Accept"),		// WTO_Use
	};
	for (auto nIdx = 0; nIdx < WTO_MaxCount; ++nIdx)
	{
		m_cb_Item[Item_WaitingSocket_Condition].AddString(szWaitingSocket_Cond[nIdx]);
	}
	m_cb_Item[Item_WaitingSocket_Condition].SetCurSel(0);

	LPCTSTR szMultiTester_SelMethod[] =
	{
		_T("Default"),		// MTSel_Default
		_T("Far"),			// MTSel_Far
		_T("Near"),			// MTSel_Near
		_T("Time"),			// MTSel_Duration
	};
	for (auto nIdx = 0; nIdx < MTSel_MaxCount; ++nIdx)
	{
		m_cb_Item[Item_MultiTester_SelectMethod].AddString(szMultiTester_SelMethod[nIdx]);
	}
	m_cb_Item[Item_MultiTester_SelectMethod].SetCurSel(0);

	// 기존 데이터 불러오기
	CRegDebugInfo reg;
	ST_DebugInfo stDebugNew;
	if (reg.Get_DebugInfo(stDebugNew))
	{
		Set_UIData(&stDebugNew);
	}

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
void CWnd_Cfg_Debug::OnSize(UINT nType, int cx, int cy)
{
	CWnd_BaseView::OnSize(nType, cx, cy);

	if ((cx == 0) && (cy == 0))
		return;

	int iMagrin		= 10;
	int iSpacing	= 10;
	int iLeft		= iMagrin;
	int iTop		= iMagrin;
	int iCtrlHeight	= 40;
	int iStWidth	= 200;
	int iCbWidth	= 280;
	int iLeftSub	= iLeft + iStWidth + iSpacing;

	m_bn_Apply.MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);

	iTop += iCtrlHeight + iSpacing;

	for (auto nIdx = 0; nIdx < Item_AutoReset_Yield_byShift; ++nIdx)
	{
		m_st_Item[nIdx].MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
		m_cb_Item[nIdx].MoveWindow(iLeftSub, iTop, iCbWidth, iCtrlHeight);

		iTop += iCtrlHeight + iSpacing;
	}
}

//=============================================================================
// Method		: PreCreateWindow
// Access		: virtual public  
// Returns		: BOOL
// Parameter	: CREATESTRUCT & cs
// Qualifier	:
// Last Update	: 2016/3/17 - 11:33
// Desc.		:
//=============================================================================
BOOL CWnd_Cfg_Debug::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1), NULL);

	return CWnd_BaseView::PreCreateWindow(cs);
}

//=============================================================================
// Method		: PreTranslateMessage
// Access		: virtual public  
// Returns		: BOOL
// Parameter	: MSG * pMsg
// Qualifier	:
// Last Update	: 2016/3/17 - 11:33
// Desc.		:
//=============================================================================
BOOL CWnd_Cfg_Debug::PreTranslateMessage(MSG* pMsg)
{
	switch (pMsg->message)
	{
	case WM_CHAR:
		if (VK_TAB == pMsg->wParam)
		{
			UINT nID = GetFocus()->GetDlgCtrlID();
			UINT nID_Old = nID;
			for (int iCnt = 0; iCnt < TABSTYLE_COUNT; iCnt++)
			{
				if (nID == g_TabOrder[iCnt])
				{
					if ((TABSTYLE_COUNT - 1) == iCnt)
					{
						nID = g_TabOrder[0];
					}
					else
					{
						// Disable 된 컨트롤은 tab 처리가 안됨
						for (int iDst = iCnt + 1; iDst < TABSTYLE_COUNT; iDst++)
						{
							if (GetDlgItem(g_TabOrder[iDst])->IsWindowEnabled())
							{
								nID = g_TabOrder[iDst];
								break;
							}
						}
						// 다른 컨트롤이 모두 Diable이면 초기 컨트롤 설정
						if (nID == g_TabOrder[iCnt])
						{
							nID = g_TabOrder[0];
						}
					}

					break;
				}
			}

			if (nID != nID_Old)
			{
				GetDlgItem(nID)->SetFocus();
			}
		}
		break;

	default:
		break;
	}

	return CWnd_BaseView::PreTranslateMessage(pMsg);
}

//=============================================================================
// Method		: OnBnClicked_Apply
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2022/7/20 - 15:09
// Desc.		:
//=============================================================================
void CWnd_Cfg_Debug::OnBnClicked_Apply()
{
	ST_DebugInfo stDebugNew;
	Get_UIData(stDebugNew);

	CRegDebugInfo reg;
	if (reg.Set_DebugInfo(&stDebugNew))
	{
		GetOwner()->SendNotifyMessage(WM_CHANGED_SETTING_INFO, (WPARAM)0, WM_Setting_Debug);

		// 성공 메세지
		//LT_MessageBox(_T("Debug Infomation has been applied to the System"));
		LT_MessageBox(g_szMessageBox_T[MB_Debug_Apply_System][m_nLanguage]);
	}
	else
	{
		//LT_MessageBox(_T("failed apply to system."));
		LT_MessageBox(g_szMessageBox_T[MB_Debug_Apply_System_Failed][m_nLanguage]);
	}
}

//=============================================================================
// Method		: OnCbnSelEndOk_DebugMode
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2022/7/20 - 15:09
// Desc.		:
//=============================================================================
void CWnd_Cfg_Debug::OnCbnSelEndOk_DebugMode()
{
	int iSel = m_cb_Item[Item_UseDebugMode].GetCurSel();
	if (0 <= iSel)
	{
		EnableControl(iSel ? TRUE : FALSE);
	}
}

//=============================================================================
// Method		: Get_UIData
// Access		: public  
// Returns		: void
// Parameter	: __out ST_DebugInfo & OUT_stDebugInfo
// Qualifier	:
// Last Update	: 2021/6/14 - 16:33
// Desc.		:
//=============================================================================
void CWnd_Cfg_Debug::Get_UIData(__out ST_DebugInfo& OUT_stDebugInfo)
{
	int iSel = 0;

	iSel = m_cb_Item[Item_UseDebugMode].GetCurSel();
	if (0 <= iSel)
	{
		OUT_stDebugInfo.m_bUse_DebugMode = iSel ? true : false;
	}

	iSel = m_cb_Item[Item_NotBypass_NG].GetCurSel();
	if (0 <= iSel)
	{
		OUT_stDebugInfo.m_bUse_NotBypass_NG = iSel ? true : false;
	}

	iSel = m_cb_Item[Item_NotBypass_TestSkip].GetCurSel();
	if (0 <= iSel)
	{
		OUT_stDebugInfo.m_bUse_NotBypass_TestSkip = iSel ? true : false;
	}

	iSel = m_cb_Item[Item_NotBypass_Empty].GetCurSel();
	if (0 <= iSel)
	{
		OUT_stDebugInfo.m_bUse_NotBypass_Empty = iSel ? true : false;
	}

	iSel = m_cb_Item[Item_AutoReset_Yield_byShift].GetCurSel();
	if (0 <= iSel)
	{
		OUT_stDebugInfo.m_AutoReset_Yield_byShift = iSel ? true : false;
	}

	iSel = m_cb_Item[Item_SocketTrackOut_Condition].GetCurSel();
	if (0 <= iSel)
	{
		OUT_stDebugInfo.m_nSocketTrackOut_Condition = iSel;
	}

	iSel = m_cb_Item[Item_WaitingSocket_Condition].GetCurSel();
	if (0 <= iSel)
	{
		OUT_stDebugInfo.m_nWaitingSocket_Condition = iSel;
	}

	iSel = m_cb_Item[Item_MultiTester_SelectMethod].GetCurSel();
	if (0 <= iSel)
	{
		OUT_stDebugInfo.m_nMultiTester_SelectMethod = iSel;
	}

}

//=============================================================================
// Method		: Set_UIData
// Access		: public  
// Returns		: void
// Parameter	: __in const ST_DebugInfo * IN_pDebugInfo
// Qualifier	:
// Last Update	: 2021/6/14 - 16:33
// Desc.		:
//=============================================================================
void CWnd_Cfg_Debug::Set_UIData(__in const ST_DebugInfo* IN_pDebugInfo)
{
	int iSel = 0;

	iSel = IN_pDebugInfo->m_bUse_DebugMode ? 1 : 0;
	m_cb_Item[Item_UseDebugMode].SetCurSel(iSel);

	iSel = IN_pDebugInfo->m_bUse_NotBypass_NG ? 1 : 0;
	m_cb_Item[Item_NotBypass_NG].SetCurSel(iSel);

	iSel = IN_pDebugInfo->m_bUse_NotBypass_TestSkip ? 1 : 0;
	m_cb_Item[Item_NotBypass_TestSkip].SetCurSel(iSel);

	iSel = IN_pDebugInfo->m_bUse_NotBypass_Empty ? 1 : 0;
	m_cb_Item[Item_NotBypass_Empty].SetCurSel(iSel);

	iSel = IN_pDebugInfo->m_AutoReset_Yield_byShift ? 1 : 0;
	m_cb_Item[Item_AutoReset_Yield_byShift].SetCurSel(iSel);

	iSel = (TO_MaxCount <= IN_pDebugInfo->m_nSocketTrackOut_Condition) ? 0 : IN_pDebugInfo->m_nSocketTrackOut_Condition;
	m_cb_Item[Item_SocketTrackOut_Condition].SetCurSel(iSel);

	iSel = (WTO_MaxCount <= IN_pDebugInfo->m_nWaitingSocket_Condition) ? 0 : IN_pDebugInfo->m_nWaitingSocket_Condition;
	m_cb_Item[Item_WaitingSocket_Condition].SetCurSel(iSel);

	iSel = (MTSel_MaxCount <= IN_pDebugInfo->m_nMultiTester_SelectMethod) ? 0 : IN_pDebugInfo->m_nMultiTester_SelectMethod;
	m_cb_Item[Item_MultiTester_SelectMethod].SetCurSel(iSel);


	EnableControl(IN_pDebugInfo->m_bUse_DebugMode);
}

//=============================================================================
// Method		: EnableControl
// Access		: public  
// Returns		: void
// Parameter	: BOOL bEnable
// Qualifier	:
// Last Update	: 2022/7/19 - 17:03
// Desc.		:
//=============================================================================
void CWnd_Cfg_Debug::EnableControl(BOOL bEnable)
{
	for (UINT nIdx = Item_NotBypass_NG; nIdx < Item_MaxCount; nIdx++)
	{
		m_cb_Item[nIdx].EnableWindow(bEnable);
	}
}

//=============================================================================
// Method		: OnLanguage
// Access		: virtual public  
// Returns		: void
// Parameter	: __in uint8_t IN_nLanguage
// Qualifier	:
// Last Update	: 2022/7/19 - 17:03
// Desc.		:
//=============================================================================
void CWnd_Cfg_Debug::OnLanguage(__in uint8_t IN_nLanguage)
{
	__super::OnLanguage(IN_nLanguage);

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

	for (UINT nIdx = 0; nIdx < Item_MaxCount; nIdx++)
	{
		m_cb_Item[nIdx].SetFont(&m_font_Data);
	}

	if (GetSafeHwnd())
	{
		m_bn_Apply.SetWindowText(g_szCtrlText_T[m_nLanguage][Txt_Apply]);

		for (UINT nIdx = 0; nIdx < Item_MaxCount; nIdx++)
		{
			m_st_Item[nIdx].SetText(g_szDebugMode_T[m_nLanguage][nIdx]);
		}
	}
}

//=============================================================================
// Method		: Set_PermissionMode
// Access		: public  
// Returns		: void
// Parameter	: enPermissionMode IN_PermissionMode
// Qualifier	:
// Last Update	: 2022/7/19 - 17:03
// Desc.		:
//=============================================================================
void CWnd_Cfg_Debug::Set_PermissionMode(enPermissionMode IN_PermissionMode)
{
	switch (IN_PermissionMode)
	{
	case enPermissionMode::Permission_Operator:
	{
		m_cb_Item[Item_UseDebugMode].EnableWindow(FALSE);
		EnableControl(FALSE);

		m_bn_Apply.EnableWindow(FALSE);
	}
		break;

	case enPermissionMode::Permission_Engineer:
	{
		m_cb_Item[Item_UseDebugMode].EnableWindow(FALSE);
		EnableControl(FALSE);

		m_bn_Apply.EnableWindow(FALSE);
	}
		break;

	case enPermissionMode::Permission_Administrator:
	{
		m_cb_Item[Item_UseDebugMode].EnableWindow(TRUE);
		OnCbnSelEndOk_DebugMode();

		m_bn_Apply.EnableWindow(TRUE);
	}
		break;

	default:
		break;
	}
}

//=============================================================================
// Method		: Set_Configuration
// Access		: public  
// Returns		: void
// Parameter	: __in const ST_DebugInfo * IN_pDebugInfo
// Qualifier	:
// Last Update	: 2021/11/11 - 16:35
// Desc.		:
//=============================================================================
void CWnd_Cfg_Debug::Set_Configuration(__in const ST_DebugInfo* IN_pDebugInfo)
{
	Set_UIData(IN_pDebugInfo);
}

//=============================================================================
// Method		: Get_Configuration
// Access		: public  
// Returns		: void
// Parameter	: __out CConfig_Model & OUT_stDebugInfo
// Qualifier	:
// Last Update	: 2021/11/11 - 16:35
// Desc.		:
//=============================================================================
void CWnd_Cfg_Debug::Get_Configuration(__out ST_DebugInfo& OUT_stDebugInfo)
{
	Get_UIData(OUT_stDebugInfo);
}

