//*****************************************************************************
// Filename	: 	Wnd_Cfg_NGCode.cpp
// Created	:	2016/3/14 - 10:57
// Modified	:	2016/3/14 - 10:57
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
// Wnd_Cfg_NGCode.cpp : implementation file
//

#include "stdafx.h"
#include <stdint.h>
#include <vector>
#include "Wnd_Cfg_NGCode.h"
#include "CommonFunction.h"
#include "Dlg_ModifyNGcode.h"
#include "Def_Constant.h"
#include "Def_Language_Message.h"


static LPCTSTR g_szNGCode_Static_T[Lang_MaxCount][CWnd_Cfg_NGCode::Item_MaxCount] =
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
		_T("MES 재검사 코드"),	// MES Rework NG Code
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
		_T("MES Rework Code"),	// MES Rework NG Code
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
		_T("MES Rework Code"),	// MES Rework NG Code
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
		_T("MES复查代码"),		// MES Rework NG Code
	},
};

static LPCTSTR g_szCtrlText_T[Lang_MaxCount][CWnd_Cfg_NGCode::Txt_MaxCount] =
{
	// 한국어
	{
		_T("추가"),				// Txt_Add
		_T("제거"),				// Txt_Remove
		_T("수정"),				// Txt_Modify
		_T("적용"),				// Txt_Apply
	},

	// 영어
	{
		_T("Insert"),			// Txt_Add
		_T("Delete"),			// Txt_Remove
		_T("Modify"),			// Txt_Modify
		_T("Apply"),			// Txt_Apply
	},

	// 베트남어
	{
		_T("Insert"),			// Txt_Add
		_T("Delete"),			// Txt_Remove
		_T("Modify"),			// Txt_Modify
		_T("Apply"),			// Txt_Apply
	},

	// 중국어
	{
		_T("代码"),			// Txt_Add
		_T("删除"),			// Txt_Remove
		_T("修整"),			// Txt_Modify
		_T("适用"),			// Txt_Apply
	},
};

#define		IDC_ED_ITEM_FST		1000
#define		IDC_ED_ITEM_LST		IDC_ED_ITEM_FST + Item_MaxCount - 1

#define		IDC_LC_NGCodeLIST	1030

#define		IDC_BN_ADD			1040
#define		IDC_BN_REMOVE		1041
#define		IDC_BN_MODIFY		1042
#define		IDC_BN_SAVE			1050

#define		IDC_ED_SHARE_PATH	1060
#define		IDC_BN_SHARE_PATH	1061


#define		TABSTYLE_COUNT			12
static UINT g_TabOrder[TABSTYLE_COUNT] =
{
	IDC_ED_ITEM_FST,			// Item_NG_Code
	IDC_ED_ITEM_FST + 1,		// Item_NG_Section
	IDC_ED_ITEM_FST + 2,		// Item_Retry_Mode
	IDC_ED_ITEM_FST + 3,		// Item_RetryCnt
	IDC_ED_ITEM_FST + 4,		// Item_Move_Retry
	IDC_ED_ITEM_FST + 5,		// Item_Reset
	IDC_ED_ITEM_FST + 6,		// Item_AlarmCnt
	IDC_ED_ITEM_FST + 7,		// Item_Description
	IDC_BN_ADD, 
	IDC_BN_REMOVE, 
	IDC_BN_MODIFY,
	IDC_BN_SAVE, 
};

// CWnd_Cfg_NGCode
IMPLEMENT_DYNAMIC(CWnd_Cfg_NGCode, CWnd_BaseView)

//=============================================================================
//
//=============================================================================
CWnd_Cfg_NGCode::CWnd_Cfg_NGCode()
{
	VERIFY(m_font_Default.CreateFont(
		16,						// nHeight
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
//
//=============================================================================
CWnd_Cfg_NGCode::~CWnd_Cfg_NGCode()
{
	m_font_Default.DeleteObject();
	m_font_Data.DeleteObject();
}


BEGIN_MESSAGE_MAP(CWnd_Cfg_NGCode, CWnd_BaseView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BN_ADD,			OnBnClicked_Add)
	ON_BN_CLICKED(IDC_BN_REMOVE,		OnBnClicked_Remove)
	ON_BN_CLICKED(IDC_BN_MODIFY,		OnBnClicked_Modify)
	ON_BN_CLICKED(IDC_BN_SAVE,			OnBnClicked_Apply)
	//ON_BN_CLICKED(IDC_BN_SHARE_PATH,	OnBnClicked_SharePath)
END_MESSAGE_MAP()


// CWnd_Cfg_NGCode message handlers
//=============================================================================
// Method		: OnCreate
// Access		: public  
// Returns		: int
// Parameter	: LPCREATESTRUCT lpCreateStruct
// Qualifier	:
// Last Update	: 2016/3/17 - 11:33
// Desc.		:
//=============================================================================
int CWnd_Cfg_NGCode::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;

	DWORD dwStyle = WS_VISIBLE | WS_CHILD /*| WS_CLIPCHILDREN*/ | WS_CLIPSIBLINGS;
	DWORD dwTempStyle = dwStyle;
	CRect rectDummy;
	rectDummy.SetRectEmpty();

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
		m_ed_Item[nIdx].SetWindowText(_T("0"));
	}

	m_lc_NGCodeList.Create(dwStyle | WS_VSCROLL, rectDummy, this, IDC_LC_NGCodeLIST);

	m_bn_Add.Create(g_szCtrlText_T[m_nLanguage][Txt_Add], dwStyle | WS_BORDER | BS_PUSHBUTTON, rectDummy, this, IDC_BN_ADD);
	m_bn_Remove.Create(g_szCtrlText_T[m_nLanguage][Txt_Remove], dwStyle | WS_BORDER | BS_PUSHBUTTON, rectDummy, this, IDC_BN_REMOVE);
	m_bn_Modify.Create(g_szCtrlText_T[m_nLanguage][Txt_Modify], dwStyle | WS_BORDER | BS_PUSHBUTTON, rectDummy, this, IDC_BN_MODIFY);
	m_bn_Apply.Create(g_szCtrlText_T[m_nLanguage][Txt_Apply], dwStyle | WS_BORDER | BS_PUSHBUTTON, rectDummy, this, IDC_BN_SAVE);

	// 저장 경로
	m_st_Path_Save.SetStaticStyle(CVGStatic::StaticStyle_Title_Alt);
	m_st_Path_Save.SetColorStyle(CVGStatic::ColorStyle_Black);
	m_st_Path_Save.SetFont_Gdip(L"Arial", 9.0F);
	m_st_Path_Save.Create(_T("Save Path"), dwStyle | SS_CENTER | SS_CENTERIMAGE, rectDummy, this, IDC_STATIC);
	m_ed_Path_Save.Create(dwStyle | ES_CENTER | ES_NUMBER | WS_BORDER, rectDummy, this, IDC_ED_SHARE_PATH);
	m_ed_Path_Save.SetFont(&m_font_Data);

	if (false == m_szSavePath.IsEmpty())
	{
		Load_IniFile();
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
void CWnd_Cfg_NGCode::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	int iMargin			= 10;
	int iSpacing		= 5;
	int iCateSpacing	= 10;
	int iLeft			= iMargin;
	int iTop			= iMargin;
	int iWidth			= cx - iMargin - iMargin;
	int iHeight			= cy - iMargin - iMargin;
	int iCtrlHeight		= 40;
	int iCtrlWidth		= 400;
	int iStWidth		= 140;
	int iEdWidth		= iCtrlWidth - iStWidth - iSpacing;
	int iQuadSt_W		= 100;
	int iQuadEd_W		= iEdWidth - iQuadSt_W;
	int iLeftSub		= iLeft + iStWidth + iSpacing;
	int iListWidth		= 800;
	int iListHeight		= iHeight - (iCtrlHeight + iSpacing);

	iLeft = iMargin + iCtrlWidth + iSpacing + iListWidth - 250;
	m_bn_Apply.MoveWindow(iLeft, iTop, 250, iCtrlHeight);


	m_st_Path_Save.MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
	m_ed_Path_Save.MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);


	// 아이템
	iLeft = iMargin;
	iTop += iCtrlHeight + iSpacing;
#ifdef NG_CODE_MINI_VERSION
	for (UINT nIdx = 0; nIdx < Item_MES_NGCode; nIdx++)
	{
		switch (nIdx)
		{
		case Item_NG_Section:	// Section (NG구분)	
		//case Item_Retry_Mode:	// Retry Mode	
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
	for (UINT nIdx = 0; nIdx < Item_MES_NGCode; nIdx++)
	{
		m_st_Item[nIdx].MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
		m_ed_Item[nIdx].MoveWindow(iLeftSub, iTop, iEdWidth, iCtrlHeight);

		iTop += iCtrlHeight + iSpacing;
	}
#endif

	// 추가, 제거, 저장
	iTop += iCtrlHeight + 30;
	iQuadSt_W = (iCtrlWidth - iSpacing) / 2;
	m_bn_Add.MoveWindow(iLeft, iTop, iQuadSt_W, iCtrlHeight);
	iLeft += iQuadSt_W + iSpacing;
	m_bn_Remove.MoveWindow(iLeft, iTop, iQuadSt_W, iCtrlHeight);

	iLeft = iMargin;
	iTop += iCtrlHeight + iSpacing;
	m_bn_Modify.MoveWindow(iLeft, iTop, iQuadSt_W, iCtrlHeight);

	// MES Rework Code
	iTop += iCtrlHeight + 30;
	iLeft = iMargin;
	m_st_Item[Item_MES_NGCode].MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
	m_ed_Item[Item_MES_NGCode].MoveWindow(iLeftSub, iTop, iEdWidth, iCtrlHeight);

	// 리스트 컨트롤
	iTop = iMargin + iCtrlHeight + iSpacing;
	iLeft = iMargin + iCtrlWidth + iCateSpacing;;
	iWidth = cx - iLeft - iMargin;
	m_lc_NGCodeList.MoveWindow(iLeft, iTop, iListWidth, iListHeight);

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
BOOL CWnd_Cfg_NGCode::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1), NULL);

	return __super::PreCreateWindow(cs);
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
BOOL CWnd_Cfg_NGCode::PreTranslateMessage(MSG* pMsg)
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

	return __super::PreTranslateMessage(pMsg);
}

//=============================================================================
// Method		: OnBnClicked_Add
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/11/15 - 11:21
// Desc.		:
//=============================================================================
void CWnd_Cfg_NGCode::OnBnClicked_Add()
{
	Item_Insert();
}

//=============================================================================
// Method		: OnBnClicked_Remove
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/11/15 - 11:21
// Desc.		:
//=============================================================================
void CWnd_Cfg_NGCode::OnBnClicked_Remove()
{
	if (0 < m_lc_NGCodeList.GetSelectedCount())
	{
		Item_Remove();
	}
}

//=============================================================================
// Method		: OnBnClicked_Modify
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/11/15 - 11:21
// Desc.		:
//=============================================================================
void CWnd_Cfg_NGCode::OnBnClicked_Modify()
{
	if (0 < m_lc_NGCodeList.GetSelectedCount())
	{
		Item_Modify();
	}
}

//=============================================================================
// Method		: OnBnClicked_Apply
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/11/15 - 11:21
// Desc.		:
//=============================================================================
void CWnd_Cfg_NGCode::OnBnClicked_Apply()
{
	// 파일에 저장
	if (Save_IniFile())
	{
		// Recipe 데이터 불러오기
		GetOwner()->SendNotifyMessage(WM_CHANGED_SETTING_INFO, (WPARAM)m_szSavePath.GetBuffer(), WM_Setting_NGCode);

		//LT_MessageBox(_T("The file has been saved successfully."));
		LT_MessageBox(g_szMessageBox_T[MB_Common_SaveFile_Succeed][m_nLanguage]);
	}
	else
	{
		//LT_MessageBox(_T("Failed to save file !!"));
		LT_MessageBox(g_szMessageBox_T[MB_Common_SaveFile_Failed][m_nLanguage]);
	}
}

void CWnd_Cfg_NGCode::OnBnClicked_SharePath()
{
}

//=============================================================================
// Method		: Item_Insert
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/11/15 - 11:21
// Desc.		:
//=============================================================================
void CWnd_Cfg_NGCode::Item_Insert()
{
	ST_NGCode stNew;

	Get_UIData(stNew);

	if (false == m_lc_NGCodeList.Item_Insert(stNew))
	{
		CString szMsg;
		//szMsg.Format(_T("NG code(%d) already added !"), stNew.m_nNG_Code);
		if (m_nLanguage == enLanguage::Lang_Chinese)
		{
			szMsg.Format(g_szMessageBox_T[MB_NGCode_Code_Exist][m_nLanguage], stNew.m_nNG_Code, stNew.m_nNG_Code);
		}
		else
		{
			szMsg.Format(g_szMessageBox_T[MB_NGCode_Code_Exist][m_nLanguage], stNew.m_nNG_Code);
		}

		LT_MessageBox(szMsg);
	}
}

//=============================================================================
// Method		: Item_Remove
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/11/15 - 11:22
// Desc.		:
//=============================================================================
void CWnd_Cfg_NGCode::Item_Remove()
{
	if (0 < m_lc_NGCodeList.GetSelectedCount())
	{
		ST_NGCode stItem;
		m_lc_NGCodeList.Get_SelectedItem(stItem);

		CString szMsg;
		//szMsg.Format(_T("Are you sure you want to delete \"NG Code: %d\" in the list?"), stItem.m_nNG_Code);
		if (m_nLanguage == enLanguage::Lang_Chinese)
		{
			szMsg.Format(g_szMessageBox_T[MB_NGCode_Delete_Select][m_nLanguage], stItem.m_nNG_Code, stItem.m_nNG_Code);
		}
		else
		{
			szMsg.Format(g_szMessageBox_T[MB_NGCode_Delete_Select][m_nLanguage], stItem.m_nNG_Code);
		}

		if (IDYES == LT_MessageBox(szMsg.GetBuffer(), MB_YESNO))
		{
			m_lc_NGCodeList.Item_Remove();
		}
	}
}

//=============================================================================
// Method		: Item_Modify
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/11/15 - 11:22
// Desc.		:
//=============================================================================
void CWnd_Cfg_NGCode::Item_Modify()
{
	// 권한, 암호 수정
	if (m_lc_NGCodeList.GetSelectedCount() <= 0)
	{
		//LT_MessageBox(_T("항목을 선택하세요."));
		//LT_MessageBox(_T("Please select an item."));
		LT_MessageBox(g_szMessageBox_T[MB_Common_SelectItem][m_nLanguage]);
		return;
	}

	ST_NGCode stNGCode_old;
	if (m_lc_NGCodeList.Get_SelectedItem(stNGCode_old))
	{
		CDlg_ModifyNGCode	dlg_ModifyNGCode;
		dlg_ModifyNGCode.OnLanguage(m_nLanguage);
		dlg_ModifyNGCode.Set_NGCodeData(&stNGCode_old);

		if (IDOK == dlg_ModifyNGCode.DoModal())
		{
			ST_NGCode stNGCode_new;
			stNGCode_new = dlg_ModifyNGCode.Get_NGCodeData();

			m_lc_NGCodeList.Item_Modify(stNGCode_new);
		}
	}
}

//=============================================================================
// Method		: Save_IniFile
// Access		: public  
// Returns		: bool
// Qualifier	:
// Last Update	: 2022/7/20 - 10:10
// Desc.		:
//=============================================================================
bool CWnd_Cfg_NGCode::Save_IniFile()
{
	CNGCode_Info NGInfo;
	Get_Configuration(NGInfo);

	if (NGInfo.Save_IniFile(m_szSavePath))
	{
		// 공유 폴더로 파일 카피
		if (false == m_szSharedPath.IsEmpty())
		{
			Backup_File(m_szSharedPath);
			::DeleteFile(m_szSharedPath);

			::CopyFile(m_szSavePath.GetBuffer(), m_szSharedPath.GetBuffer(), FALSE);
		}
	}

	return true;
}

//=============================================================================
// Method		: Load_IniFile
// Access		: public  
// Returns		: bool
// Qualifier	:
// Last Update	: 2022/7/20 - 15:10
// Desc.		:
//=============================================================================
bool CWnd_Cfg_NGCode::Load_IniFile()
{
	CNGCode_Info NGInfo;
	if (NGInfo.Load_IniFile(m_szSavePath))
	{
		// UI에 표시
		Set_Configuration(&NGInfo);
	}

	return true;
}

//=============================================================================
// Method		: Get_UIData
// Access		: public  
// Returns		: void
// Parameter	: __out ST_NGCode & OUT_stNGCode
// Qualifier	:
// Last Update	: 2021/6/14 - 16:33
// Desc.		:
//=============================================================================
void CWnd_Cfg_NGCode::Get_UIData(__out ST_NGCode& OUT_stNGCode)
{
	CString szValue;	

	m_ed_Item[Item_NG_Code].GetWindowText(szValue);
	OUT_stNGCode.m_nNG_Code = _ttoi(szValue.GetBuffer());

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
}

//=============================================================================
// Method		: Set_UIData
// Access		: public  
// Returns		: void
// Parameter	: __in const ST_NGCode * IN_pNGCode
// Qualifier	:
// Last Update	: 2021/6/14 - 16:33
// Desc.		:
//=============================================================================
void CWnd_Cfg_NGCode::Set_UIData(__in const ST_NGCode* IN_pNGCode)
{
	
	
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
void CWnd_Cfg_NGCode::OnLanguage(__in uint8_t IN_nLanguage)
{
	__super::OnLanguage(IN_nLanguage);

	m_lc_NGCodeList.OnLanguage(IN_nLanguage);

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
		m_ed_Item[nIdx].SetFont(&m_font_Data);
	}
	m_ed_Path_Save.SetFont(&m_font_Data);

	if (GetSafeHwnd())
	{
		m_bn_Add.SetWindowText(g_szCtrlText_T[m_nLanguage][Txt_Add]);
		m_bn_Remove.SetWindowText(g_szCtrlText_T[m_nLanguage][Txt_Remove]);
		m_bn_Modify.SetWindowText(g_szCtrlText_T[m_nLanguage][Txt_Modify]);
		m_bn_Apply.SetWindowText(g_szCtrlText_T[m_nLanguage][Txt_Apply]);

		for (UINT nIdx = 0; nIdx < Item_MaxCount; nIdx++)
		{
			m_st_Item[nIdx].SetText(g_szNGCode_Static_T[m_nLanguage][nIdx]);
		}
	}
}

//=============================================================================
// Method		: Set_Path
// Access		: public  
// Returns		: void
// Parameter	: LPCTSTR IN_szSavePath
// Qualifier	:
// Last Update	: 2022/7/19 - 17:02
// Desc.		:
//=============================================================================
void CWnd_Cfg_NGCode::Set_Path(LPCTSTR IN_szSavePath)
{
	m_szSavePath.Format(_T("%s\\%s"), IN_szSavePath, NGCODE_INFO_FILENAME);

	if (GetSafeHwnd())
	{
		Load_IniFile();
	}
}

void CWnd_Cfg_NGCode::Set_PathShared(LPCTSTR IN_szSharedPath)
{
	m_szSharedPath.Format(_T("%s\\%s"), IN_szSharedPath, NGCODE_INFO_FILENAME);
}

void CWnd_Cfg_NGCode::Set_Path(LPCTSTR IN_szSavePath, LPCTSTR IN_szSharePath)
{
	m_szSavePath.Format(_T("%s\\%s"), IN_szSavePath, NGCODE_INFO_FILENAME);
	m_szSharedPath.Format(_T("%s\\%s"), IN_szSharePath, NGCODE_INFO_FILENAME);
}

//=============================================================================
// Method		: Set_PermissionMode
// Access		: public  
// Returns		: void
// Parameter	: __in enPermissionMode IN_PermissionMode
// Qualifier	:
// Last Update	: 2022/1/7 - 11:11
// Desc.		:
//=============================================================================
void CWnd_Cfg_NGCode::Set_PermissionMode(__in enPermissionMode IN_PermissionMode)
{
	switch (IN_PermissionMode)
	{
	case enPermissionMode::Permission_Operator:
		for (UINT nIdx = 0; nIdx < Item_MaxCount; nIdx++)
		{
			m_ed_Item[nIdx].EnableWindow(FALSE);
		}
		m_bn_Add.EnableWindow(FALSE);
		m_bn_Remove.EnableWindow(FALSE);
		m_bn_Modify.EnableWindow(FALSE);
		m_bn_Apply.EnableWindow(FALSE);
		break;

	case enPermissionMode::Permission_Engineer:
		for (UINT nIdx = 0; nIdx < Item_MaxCount; nIdx++)
		{
			m_ed_Item[nIdx].EnableWindow(FALSE);
		}
		m_bn_Add.EnableWindow(FALSE);
		m_bn_Remove.EnableWindow(FALSE);
		m_bn_Modify.EnableWindow(FALSE);
		m_bn_Apply.EnableWindow(FALSE);
		break;

	case enPermissionMode::Permission_Administrator:
		for (UINT nIdx = 0; nIdx < Item_MaxCount; nIdx++)
		{
			m_ed_Item[nIdx].EnableWindow(TRUE);
		}
		m_bn_Add.EnableWindow(TRUE);
		m_bn_Remove.EnableWindow(TRUE);
		m_bn_Modify.EnableWindow(TRUE);
		m_bn_Apply.EnableWindow(TRUE);
		break;

	default:
		break;
	}
}

//=============================================================================
// Method		: Get_Configuration
// Access		: public  
// Returns		: void
// Parameter	: __out CNGCode_Info & OUT_stNGCodeList
// Qualifier	:
// Last Update	: 2021/6/14 - 16:34
// Desc.		:
//=============================================================================
void CWnd_Cfg_NGCode::Get_Configuration(__out CNGCode_Info& OUT_stNGCodeList)
{
	m_lc_NGCodeList.Get_ItemList(OUT_stNGCodeList);

	// MES
	CString szValue;
	m_ed_Item[Item_MES_NGCode].GetWindowText(szValue);
	OUT_stNGCodeList.m_nMES_ReworkNGCode = _ttoi(szValue.GetBuffer());
}

//=============================================================================
// Method		: Set_Configuration
// Access		: public  
// Returns		: void
// Parameter	: __in const CNGCode_Info * IN_pNGCodeList
// Qualifier	:
// Last Update	: 2021/6/14 - 16:34
// Desc.		:
//=============================================================================
void CWnd_Cfg_NGCode::Set_Configuration(__in const CNGCode_Info* IN_pNGCodeList)
{
	m_lc_NGCodeList.Set_ItemList(IN_pNGCodeList);

	// MES
	CString szValue;
	szValue.Format(_T("%d"), IN_pNGCodeList->m_nMES_ReworkNGCode);
	m_ed_Item[Item_MES_NGCode].SetWindowText(szValue.GetBuffer());

}

