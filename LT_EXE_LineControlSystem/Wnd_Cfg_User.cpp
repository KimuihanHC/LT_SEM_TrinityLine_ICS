//*****************************************************************************
// Filename	: 	Wnd_Cfg_User.cpp
// Created	:	2016/3/14 - 10:57
// Modified	:	2016/3/14 - 10:57
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
// Wnd_Cfg_User.cpp : implementation file
//

#include "stdafx.h"
#include "Wnd_Cfg_User.h"
#include "CommonFunction.h"
#include "Reg_InspInfo.h"
#include "Dlg_ModifyUser.h"
#include "Def_Constant.h"
#include "File_UserInfo.h"
#include "Def_Language_Message.h"


static LPCTSTR g_szCaption_T[Lang_MaxCount][CWnd_Cfg_User::US_MaxCount] =
{
	// 한국어
	{
		_T("권한"),				// US_Authority
		_T("이름"),				// US_Name
		_T("ID"),				// US_ID
		_T("비밀번호"),			// US_Password
		_T("비밀번호 확인"),		// US_Password_Confirm
	},

	// 영어
	{
		_T("Authority"),		// US_Authority
		_T("Name"),				// US_Name
		_T("ID"),				// US_ID
		_T("Password"),			// US_Password
		_T("Confirm Password"),	// US_Password_Confirm
	},

	// 베트남어
	{
		_T("Authority"),		// US_Authority
		_T("Name"),				// US_Name
		_T("ID"),				// US_ID
		_T("Password"),			// US_Password
		_T("Confirm Password"),	// US_Password_Confirm
	},

	// 중국어
	{
		_T("权限"),				// US_Authority
		_T("名字"),				// US_Name
		_T("ID"),				// US_ID
		_T("密码"),				// US_Password
		_T("确认密码"),			// US_Password_Confirm
	},
};

static LPCTSTR g_szCtrlText_T[Lang_MaxCount][CWnd_Cfg_User::Txt_MaxCount] =
{
	// 한국어
	{
		_T("등록"),				// Txt_Add
		_T("삭제"),				// Txt_Remove
		_T("사용자 수정"),		// Txt_Modify
	},

	// 영어
	{
		_T("Register"),			// Txt_Add
		_T("Delete"),			// Txt_Remove
		_T("Edit User"),		// Txt_Modify
	},

	// 베트남어
	{
		_T("Register"),			// Txt_Add
		_T("Delete"),			// Txt_Remove
		_T("Edit User"),		// Txt_Modify
	},

	// 중국어
	{
		_T("登记"),				// Txt_Add
		_T("删除"),				// Txt_Remove
		_T("使用者修改"),		// Txt_Modify
	},
};

#define		IDC_LC_SOCKETLIST		1030

#define		IDC_BN_ADD				1040
#define		IDC_BN_REMOVE			1041
#define		IDC_BN_MODIFY			1042

#define		IDC_CB_AUTHORITY		1000
#define		IDC_ED_NAME				1001
#define		IDC_ED_ID				1002
#define		IDC_ED_USER_PASSWORD			1003
#define		IDC_ED_PASSWORD_2ND		1004

#define		TABSTYLE_COUNT			8
static UINT g_TabOrder[TABSTYLE_COUNT] =
{
	IDC_CB_AUTHORITY,
	IDC_ED_NAME,
	IDC_ED_ID,
	IDC_ED_USER_PASSWORD,
	IDC_ED_PASSWORD_2ND,
	IDC_BN_ADD, 
	IDC_BN_REMOVE, 
	IDC_BN_MODIFY,
};

// CWnd_Cfg_User
IMPLEMENT_DYNAMIC(CWnd_Cfg_User, CWnd_BaseView)

//=============================================================================
//
//=============================================================================
CWnd_Cfg_User::CWnd_Cfg_User()
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
CWnd_Cfg_User::~CWnd_Cfg_User()
{
	m_font_Default.DeleteObject();
	m_font_Data.DeleteObject();
}


BEGIN_MESSAGE_MAP(CWnd_Cfg_User, CWnd_BaseView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BN_ADD,		OnBnClicked_Add)
	ON_BN_CLICKED(IDC_BN_REMOVE,	OnBnClicked_Remove)
	ON_BN_CLICKED(IDC_BN_MODIFY,	OnBnClicked_Modify)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CWnd_Cfg_User message handlers
//=============================================================================
// Method		: OnCreate
// Access		: public  
// Returns		: int
// Parameter	: LPCREATESTRUCT lpCreateStruct
// Qualifier	:
// Last Update	: 2016/3/17 - 11:33
// Desc.		:
//=============================================================================
int CWnd_Cfg_User::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd_BaseView::OnCreate(lpCreateStruct) == -1)
		return -1;

	DWORD dwStyle = WS_VISIBLE | WS_CHILD /*| WS_CLIPCHILDREN*/ | WS_CLIPSIBLINGS;
	DWORD dwTempStyle = dwStyle;
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	
	m_lc_UserList.Create(dwStyle | WS_VSCROLL, rectDummy, this, IDC_LC_SOCKETLIST);

	m_bn_Add.Create(g_szCtrlText_T[m_nLanguage][Txt_Add], dwStyle | WS_BORDER | BS_PUSHBUTTON, rectDummy, this, IDC_BN_ADD);
	m_bn_Remove.Create(g_szCtrlText_T[m_nLanguage][Txt_Remove], dwStyle | WS_BORDER | BS_PUSHBUTTON, rectDummy, this, IDC_BN_REMOVE);
	m_bn_Modify.Create(g_szCtrlText_T[m_nLanguage][Txt_Modify], dwStyle | WS_BORDER | BS_PUSHBUTTON, rectDummy, this, IDC_BN_MODIFY);

	for (auto nIdx = 0; nIdx < US_MaxCount; ++nIdx)
	{
		m_st_Caption[nIdx].SetStaticStyle(CVGStatic::StaticStyle_Title_Alt);
		m_st_Caption[nIdx].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_st_Caption[nIdx].SetFont_Gdip(L"Arial", 10.0F);
		m_st_Caption[nIdx].Create(g_szCaption_T[m_nLanguage][nIdx], dwStyle | SS_CENTER | SS_CENTERIMAGE, rectDummy, this, IDC_STATIC);
	}

	m_cb_Authority.Create(dwStyle | CBS_DROPDOWNLIST | WS_TABSTOP, rectDummy, this, IDC_CB_AUTHORITY);
	m_ed_Name.Create(dwStyle | ES_CENTER | WS_BORDER, rectDummy, this, IDC_ED_NAME);
	m_ed_Name.SetFont(&m_font_Data);
	m_ed_ID.Create(dwStyle | ES_CENTER | WS_BORDER, rectDummy, this, IDC_ED_ID);
	m_ed_ID.SetFont(&m_font_Data);
	m_ed_Password.Create(dwStyle | ES_CENTER | WS_BORDER | ES_PASSWORD, rectDummy, this, IDC_ED_USER_PASSWORD);
	m_ed_Password.SetFont(&m_font_Data);
	m_ed_Password_Confirm.Create(dwStyle | ES_CENTER | WS_BORDER | ES_PASSWORD, rectDummy, this, IDC_ED_PASSWORD_2ND);
	m_ed_Password_Confirm.SetFont(&m_font_Data);

	m_cb_Authority.SetFont(&m_font_Data);
	for (auto nIdx = 0; nIdx < Permission_MaxCount; ++nIdx)
	{
		m_cb_Authority.AddString(g_szPermissionMode[nIdx]);
	}
	m_cb_Authority.SetCurSel(0);


	Load_UserList();

	m_cb_Authority.SetFocus();

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
void CWnd_Cfg_User::OnSize(UINT nType, int cx, int cy)
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
	int iCtrlHeight		= 40;
	int iCtrlWidth		= 400;
	int iStWidth		= 140;
	int iEdWidth		= iCtrlWidth - iStWidth - iSpacing;
	int iLeft_Sub		= iLeft + iStWidth + iSpacing;

	// 	Authority
	m_st_Caption[US_Authority].MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
	m_cb_Authority.MoveWindow(iLeft_Sub, iTop, iEdWidth, iCtrlHeight);

	// 	Name
	iTop += iCtrlHeight + iSpacing;
	m_st_Caption[US_Name].MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
	m_ed_Name.MoveWindow(iLeft_Sub, iTop, iEdWidth, iCtrlHeight);

	// 	ID
	iTop += iCtrlHeight + iSpacing;
	m_st_Caption[US_ID].MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
	m_ed_ID.MoveWindow(iLeft_Sub, iTop, iEdWidth, iCtrlHeight);

	// 	Password
	iTop += iCtrlHeight + iSpacing;
	m_st_Caption[US_Password].MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
	m_ed_Password.MoveWindow(iLeft_Sub, iTop, iEdWidth, iCtrlHeight);

	// 	Password_2nd
	iTop += iCtrlHeight + iSpacing;
	m_st_Caption[US_Password_Confirm].MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
	m_ed_Password_Confirm.MoveWindow(iLeft_Sub, iTop, iEdWidth, iCtrlHeight);

	// 추가, 제거, 저장
	iTop += iCtrlHeight + 30;
	int iQuadSt_W = (iCtrlWidth - iSpacing) / 2;
	m_bn_Add.MoveWindow(iLeft, iTop, iQuadSt_W, iCtrlHeight);
	iLeft += iQuadSt_W + iSpacing;
	m_bn_Remove.MoveWindow(iLeft, iTop, iQuadSt_W, iCtrlHeight);
	iTop += iCtrlHeight + iCateSpacing;
	m_bn_Modify.MoveWindow(iLeft, iTop, iQuadSt_W, iCtrlHeight);

	// 리스트 컨트롤
	iTop = iMargin;
	iLeft = iMargin + iCtrlWidth + iCateSpacing;;
	iWidth = cx - iLeft - iMargin;
	m_lc_UserList.MoveWindow(iLeft, iTop, 720, iHeight);
	//m_lc_UserList.MoveWindow(iMargin, iMargin, 600, iHeight);

}

//=============================================================================
// Method		: OnShowWindow
// Access		: public  
// Returns		: void
// Parameter	: BOOL bShow
// Parameter	: UINT nStatus
// Qualifier	:
// Last Update	: 2022/1/5 - 15:22
// Desc.		:
//=============================================================================
void CWnd_Cfg_User::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CWnd_BaseView::OnShowWindow(bShow, nStatus);

	if (bShow)
	{
		m_cb_Authority.SetFocus();
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
BOOL CWnd_Cfg_User::PreCreateWindow(CREATESTRUCT& cs)
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
BOOL CWnd_Cfg_User::PreTranslateMessage(MSG* pMsg)
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
			else
			{
				m_cb_Authority.SetFocus();
			}
		}
		break;

	default:
		break;
	}

	return CWnd_BaseView::PreTranslateMessage(pMsg);
}

//=============================================================================
// Method		: OnBnClicked_Add
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/11/15 - 11:21
// Desc.		:
//=============================================================================
void CWnd_Cfg_User::OnBnClicked_Add()
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
void CWnd_Cfg_User::OnBnClicked_Remove()
{
	if (0 < m_lc_UserList.GetSelectedCount())
	{
		Item_Remove();
	}
}

//=============================================================================
// Method		: OnBnClicked_Modify
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2022/1/7 - 13:57
// Desc.		:
//=============================================================================
void CWnd_Cfg_User::OnBnClicked_Modify()
{
	if (0 < m_lc_UserList.GetSelectedCount())
	{
		Item_Modify();
	}	
}

//=============================================================================
// Method		: Item_Insert
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/11/15 - 11:21
// Desc.		:
//=============================================================================
void CWnd_Cfg_User::Item_Insert()
{
	ST_User_Unit stNewItem;
	if (Get_UIData(stNewItem))
	{
		m_lc_UserList.Item_Insert(stNewItem);

		// 파일에 저장
		Save_UserList();

		Item_New();
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
void CWnd_Cfg_User::Item_Remove()
{
	if (0 < m_lc_UserList.GetSelectedCount())
	{
		// 현재 로그인된 사용자는 삭제 불가, Admin 삭제 불가
		ST_User_Unit stUser_select;
		ST_User_Unit stUser_login;
		if (m_lc_UserList.Get_SelectedItem(stUser_select))
		{
			if (0 == stUser_select.szID.Compare(_T("Admin")))
			{
				//LT_MessageBox(_T("\"Admin\" user cannot be deleted."));
				LT_MessageBox(g_szMessageBox_T[MB_User_Admin_CantDelete][m_nLanguage]);
				return;
			}

			CReg_InspInfo reg;
			if (reg.Get_LogIn_User(stUser_login))
			{
				if (0 == stUser_select.szID.Compare(stUser_login.szID))
				{
					//LT_MessageBox(_T("The currently logged in user cannot be deleted. \r\n Please log out and delete."));
					LT_MessageBox(g_szMessageBox_T[MB_User_LoginUser_CantDelete][m_nLanguage]);
					return;
				}
			}
		}


		//if (IDYES == LT_MessageBox(_T("Are you sure you want to remove a user from the list?"), MB_YESNO))
		if (IDYES == LT_MessageBox(g_szMessageBox_T[MB_User_Remove_User][m_nLanguage], MB_YESNO))
		{
			m_lc_UserList.Item_Remove();

			// 파일에 저장
			Save_UserList();
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
void CWnd_Cfg_User::Item_Modify()
{
	// 권한, 암호 수정
	if (m_lc_UserList.GetSelectedCount() <= 0)
	{
		//LT_MessageBox(_T("항목을 선택하세요."));
		//LT_MessageBox(_T("Please select an item."));
		LT_MessageBox(g_szMessageBox_T[MB_Common_SelectItem][m_nLanguage]);
		return;
	}

	ST_User_Unit stUser_old;
	if (m_lc_UserList.Get_SelectedItem(stUser_old))
	{
		CDlg_ModifyUser	m_dlg_ModifyEqp;
		m_dlg_ModifyEqp.OnLanguage(m_nLanguage);
		m_dlg_ModifyEqp.Set_NGCodeData(&stUser_old);

		if (IDOK == m_dlg_ModifyEqp.DoModal())
		{
			ST_User_Unit stUser_new;
			stUser_new = m_dlg_ModifyEqp.Get_NGCodeData();

			// Admin 유저는 권한 변경 불가
			if (0 == stUser_new.szID.Compare(_T("Admin")))
			{
				stUser_new.nAuthority = enPermissionMode::Permission_Administrator;
			}

			m_lc_UserList.Item_Modify(&stUser_new);

			// 파일에 저장
			Save_UserList();
		}
	}

	//m_lc_UserList.Item_Modify();
}

//=============================================================================
// Method		: Item_New
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2022/1/7 - 14:30
// Desc.		:
//=============================================================================
void CWnd_Cfg_User::Item_New()
{
	m_ed_Name.SetWindowText(_T(""));
	m_ed_ID.SetWindowText(_T(""));
	m_ed_Password.SetWindowText(_T(""));
	m_ed_Password_Confirm.SetWindowText(_T(""));
}

//=============================================================================
// Method		: Save_UserList
// Access		: public  
// Returns		: bool
// Qualifier	:
// Last Update	: 2022/1/6 - 15:42
// Desc.		:
//=============================================================================
bool CWnd_Cfg_User::Save_UserList()
{
	CReg_InspInfo	Reg;
 	ST_UserInfo		stUserList;

 	m_lc_UserList.Get_ItemList(stUserList);
 
	if (Reg.Set_UserInfo(&stUserList))
	{
		File_UserInfo iniFile;
		if (false == iniFile.Save_UserInfo(m_szSharedPath.GetBuffer(), &stUserList))
		{
			return false;
		}

		return true;
	}

	return false;
}

//=============================================================================
// Method		: Load_UserList
// Access		: public  
// Returns		: bool
// Qualifier	:
// Last Update	: 2022/1/6 - 15:42
// Desc.		:
//=============================================================================
bool CWnd_Cfg_User::Load_UserList()
{
	CReg_InspInfo	Reg;
 	ST_UserInfo		stUserList;

	if (Reg.Get_UserInfo(stUserList))
	{
		m_lc_UserList.Set_ItemList(&stUserList);
		return true;
	}

	return false;
}

//=============================================================================
// Method		: Get_UIData
// Access		: public  
// Returns		: bool
// Parameter	: __out CConfig_Model & OUT_stUser
// Qualifier	:
// Last Update	: 2021/6/14 - 16:33
// Desc.		:
//=============================================================================
bool CWnd_Cfg_User::Get_UIData(__out ST_User_Unit& OUT_stUser)
{
	CString szValue;	
	CString szPass;

	// Authority --------------------------
	int iSel = m_cb_Authority.GetCurSel();
	if (iSel < 0)
	{
		//LT_MessageBox(_T("Select Authority."));
		LT_MessageBox(g_szMessageBox_T[MB_User_Authority_Select][m_nLanguage]);
		return false;
	}
	OUT_stUser.nAuthority = static_cast<uint8_t>(iSel);

	// Name --------------------------
	m_ed_Name.GetWindowText(szValue);
	if (szValue.IsEmpty())
	{
		//LT_MessageBox(_T("Please enter a Name."));
		LT_MessageBox(g_szMessageBox_T[MB_User_Name_Enter][m_nLanguage]);
		return false;
	}
	// 중복인가?
	if (m_lc_UserList.IsExist_Name(szValue.GetBuffer()))
	{
		//LT_MessageBox(_T("The name you entered already exists."));
		LT_MessageBox(g_szMessageBox_T[MB_User_Name_Exist][m_nLanguage]);
		return false;
	}

	OUT_stUser.szName = szValue;

	// ID --------------------------
	m_ed_ID.GetWindowText(szValue);
	if (szValue.IsEmpty())
	{
		//LT_MessageBox(_T("Please enter a ID."));
		LT_MessageBox(g_szMessageBox_T[MB_User_ID_Enter][m_nLanguage]);
		return false;
	}
	// 중복인가?
	if (m_lc_UserList.IsExist_ID(szValue.GetBuffer()))
	{
		//LT_MessageBox(_T("The ID you entered already exists."));
		LT_MessageBox(g_szMessageBox_T[MB_User_ID_Exist][m_nLanguage]);
		return false;
	}
	OUT_stUser.szID = szValue;

	// Password --------------------------
	m_ed_Password.GetWindowText(szValue);
	if (szValue.IsEmpty())
	{
		//LT_MessageBox(_T("Please enter a Password."));
		LT_MessageBox(g_szMessageBox_T[MB_User_Password_Enter][m_nLanguage]);
		return false;
	}
	OUT_stUser.szPassword = szValue;

	m_ed_Password_Confirm.GetWindowText(szValue);
	if (szValue.IsEmpty())
	{
		//LT_MessageBox(_T("Please enter a Confirm Password."));
		LT_MessageBox(g_szMessageBox_T[MB_User_PasswordConfirm_Enter][m_nLanguage]);
		return false;
	}

	if (0 != szValue.Compare(OUT_stUser.szPassword.GetBuffer()))
	{
		//LT_MessageBox(_T("Password and Confirm Password does not match."));
		LT_MessageBox(g_szMessageBox_T[MB_User_Password_NotMatch][m_nLanguage]);
		return false;
	}

	return true;
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
void CWnd_Cfg_User::OnLanguage(__in uint8_t IN_nLanguage)
{
	__super::OnLanguage(IN_nLanguage);

	m_lc_UserList.OnLanguage(IN_nLanguage);

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
	//lf.lfHeight = (enLanguage::Lang_Chinese == m_nLanguage) ? 24 : 18;
	m_font_Data.DeleteObject();
	m_font_Data.CreateFontIndirect(&lf);

	m_ed_Name.SetFont(&m_font_Data);
	m_ed_ID.SetFont(&m_font_Data);
	m_ed_Password.SetFont(&m_font_Data);
	m_ed_Password_Confirm.SetFont(&m_font_Data);

	if (GetSafeHwnd())
	{
		m_bn_Add.SetWindowText(g_szCtrlText_T[m_nLanguage][Txt_Add]);
		m_bn_Remove.SetWindowText(g_szCtrlText_T[m_nLanguage][Txt_Remove]);
		m_bn_Modify.SetWindowText(g_szCtrlText_T[m_nLanguage][Txt_Modify]);

		for (auto nIdx = 0; nIdx < US_MaxCount; ++nIdx)
		{
			m_st_Caption[nIdx].SetText(g_szCaption_T[m_nLanguage][nIdx]);
		}
	}
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
void CWnd_Cfg_User::Set_PermissionMode(__in enPermissionMode IN_PermissionMode)
{
	switch (IN_PermissionMode)
	{
	case enPermissionMode::Permission_Operator:
	case enPermissionMode::Permission_Engineer:
	{
		m_lc_UserList			.EnableWindow(FALSE);
		m_bn_Add				.EnableWindow(FALSE);
		m_bn_Remove				.EnableWindow(FALSE);
		m_bn_Modify				.EnableWindow(FALSE);
		m_cb_Authority			.EnableWindow(FALSE);
		m_ed_Name				.EnableWindow(FALSE);
		m_ed_ID					.EnableWindow(FALSE);
		m_ed_Password			.EnableWindow(FALSE);
		m_ed_Password_Confirm	.EnableWindow(FALSE);
	}
	break;

	case enPermissionMode::Permission_Administrator:
	{
		m_lc_UserList			.EnableWindow(TRUE);
		m_bn_Add				.EnableWindow(TRUE);
		m_bn_Remove				.EnableWindow(TRUE);
		m_bn_Modify				.EnableWindow(TRUE);
		m_cb_Authority			.EnableWindow(TRUE);
		m_ed_Name				.EnableWindow(TRUE);
		m_ed_ID					.EnableWindow(TRUE);
		m_ed_Password			.EnableWindow(TRUE);
		m_ed_Password_Confirm	.EnableWindow(TRUE);
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
// Last Update	: 2022/1/6 - 11:38
// Desc.		:
//=============================================================================
void CWnd_Cfg_User::Set_FileFullPath(__in LPCTSTR IN_szFullPath)
{
	m_szPath_Default = IN_szFullPath;
}

void CWnd_Cfg_User::Set_PathShared(__in LPCTSTR IN_szSharedPath)
{
	m_szSharedPath.Format(_T("%s\\%s"), IN_szSharedPath, USER_INFO_FILENAME);
}

//=============================================================================
// Method		: Get_Option
// Access		: public  
// Returns		: void
// Parameter	: __out CConfig_Model & OUT_stUserList
// Qualifier	:
// Last Update	: 2021/6/14 - 16:34
// Desc.		:
//=============================================================================
void CWnd_Cfg_User::Get_Configuration(__out ST_UserInfo& OUT_stUserList)
{
	m_lc_UserList.Get_ItemList(OUT_stUserList);
}

//=============================================================================
// Method		: Set_Option
// Access		: public  
// Returns		: void
// Parameter	: __in const CConfig_Model * IN_pUserList
// Qualifier	:
// Last Update	: 2021/6/14 - 16:34
// Desc.		:
//=============================================================================
void CWnd_Cfg_User::Set_Configuration(__in const ST_UserInfo* IN_pUserList)
{
	m_lc_UserList.Set_ItemList(IN_pUserList);
}
