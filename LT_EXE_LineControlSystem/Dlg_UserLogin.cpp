//*****************************************************************************
// Filename	: 	Dlg_UserLogin.cpp
// Created	:	2021/11/6 - 19:09
// Modified	:	2021/11/6 - 19:09
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
// Dlg_UserLogin.cpp : implementation file
//

#include "stdafx.h"
#include <afxdialogex.h>
#include "Dlg_UserLogin.h"
#include "Def_WindowMessage_Cm.h"
#include "Reg_InspInfo.h"
#include "CommonFunction.h"
#include "Def_Language_Message.h"

enum enCtrlText
{
	Txt_Title,
	Txt_ID,
	Txt_Password,
	Txt_Login,
	Txt_Logout,
	Txt_Keyboard,

	Txt_MaxCount,
};

static LPCTSTR g_szCtrlText_T[Lang_MaxCount][Txt_MaxCount] =
{
	// 한국어
	{
		_T("사용자 로그인"),		// Txt_Title
		_T("ID 선택"),			// Txt_ID
		_T("비밀번호"),			// Txt_Password
		_T("로그 인"),			// Txt_Login
		_T("로그 아웃"),			// Txt_Logout
		_T("키보드"),			// Txt_Keyboard
	},

	// 영어
	{
		_T("User Log-In"),		// Txt_Title
		_T("Select ID"),		// Txt_ID
		_T("Password"),			// Txt_Password
		_T("Log In"),			// Txt_Login
		_T("Log Out"),			// Txt_Logout
		_T("Keyboard"),			// Txt_Keyboard
	},

	// 베트남어
	{
		_T("User Log-In"),		// Txt_Title
		_T("Select ID"),		// Txt_ID
		_T("Password"),			// Txt_Password
		_T("Log In"),			// Txt_Login
		_T("Log Out"),			// Txt_Logout
		_T("Keyboard"),			// Txt_Keyboard
	},

	// 중국어
	{
		_T("用户登录"),			// Txt_Title
		_T("选择ID"),			// Txt_ID
		_T("密码"),				// Txt_Password
		_T("登录"),				// Txt_Login
		_T("注销"),				// Txt_Logout
		_T("键盘"),				// Txt_Keyboard
	},
};


#define		IDC_LC_USER_LIST		1010
#define		IDC_LC_USER_LIST_OPERA	1011
#define		IDC_LC_USER_LIST_ENGIN	1012
#define		IDC_LC_USER_LIST_ADMIN	1013
#define		IDC_RB_AUTHORITY_FST	1021
#define		IDC_RB_AUTHORITY_LST	1023

#define		IDC_CB_ID				1100
#define		IDC_ST_ACESS_MODE_T		1101
#define		IDC_ED_ACESS_PASSWORD	1102

#define		IDC_BN_LOGIN			1201
#define		IDC_BN_LOGOUT			1202
#define		IDC_BN_KEYBOARD			1203


//-----------------------------------------------------------------------------
// CDlg_UserLogin dialog
//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CDlg_UserLogin, CDialogEx)

CDlg_UserLogin::CDlg_UserLogin(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlg_UserLogin::IDD, pParent)
{
	VERIFY(m_font_Large.CreateFont(
		24,							// nHeight
		0,							// nWidth
		0,							// nEscapement
		0,							// nOrientation
		FW_BOLD,					// nWeight
		FALSE,						// bItalic
		FALSE,						// bUnderline
		0,							// cStrikeOut
		ANSI_CHARSET,				// nCharSet
		OUT_DEFAULT_PRECIS,			// nOutPrecision
		CLIP_DEFAULT_PRECIS,		// nClipPrecision
		DEFAULT_QUALITY,			// nQuality
		VARIABLE_PITCH,				// nPitchAndFamily
		_T("Arial")));		// lpszFacename

	VERIFY(m_font_Default.CreateFont(
		30,							// nHeight
		0,							// nWidth
		0,							// nEscapement
		0,							// nOrientation
		FW_BOLD,					// nWeight
		FALSE,						// bItalic
		FALSE,						// bUnderline
		0,							// cStrikeOut
		ANSI_CHARSET,				// nCharSet
		OUT_DEFAULT_PRECIS,			// nOutPrecision
		CLIP_DEFAULT_PRECIS,		// nClipPrecision
		DEFAULT_QUALITY,			// nQuality
		VARIABLE_PITCH,				// nPitchAndFamily
		_T("Arial")));		// lpszFacename

}

CDlg_UserLogin::~CDlg_UserLogin()
{
	m_font_Large.DeleteObject();
	m_font_Default.DeleteObject();
}

void CDlg_UserLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlg_UserLogin, CDialogEx)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_EN_CHANGE(IDC_ED_ACESS_PASSWORD, OnEnChange_Password)
	ON_BN_CLICKED(IDC_BN_LOGIN,			OnBnClicked_LogIn)
	ON_BN_CLICKED(IDC_BN_LOGOUT,		OnBnClicked_LogOut)
	ON_BN_CLICKED(IDC_BN_KEYBOARD,		OnBnClicked_Keyboard)
	ON_COMMAND_RANGE(IDC_RB_AUTHORITY_FST, IDC_RB_AUTHORITY_LST, OnAuthoritySelect)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LC_USER_LIST_ADMIN, OnLbnSelchangeList_User_Admin)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LC_USER_LIST_ENGIN, OnLbnSelchangeList_User_Engineer)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LC_USER_LIST_OPERA, OnLbnSelchangeList_User_Oper)
END_MESSAGE_MAP()


// CDlg_UserLogin message handlers
//=============================================================================
// Method		: OnCreate
// Access		: protected  
// Returns		: int
// Parameter	: LPCREATESTRUCT lpCreateStruct
// Qualifier	:
// Last Update	: 2016/11/4 - 11:45
// Desc.		:
//=============================================================================
int CDlg_UserLogin::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	DWORD dwStyle = WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	m_st_Title.SetBackColor_COLORREF(RGB(0, 0, 0));
	m_st_Title.SetTextColor(Color::White, Color::White);
	m_st_Title.SetFont_Gdip(L"Arial", 24.0F);
	m_st_Title.Create(g_szCtrlText_T[m_nLanguage][Txt_Title], dwStyle | SS_CENTER | SS_CENTERIMAGE, rectDummy, this, IDC_STATIC);

	for (auto nIdx = 0; nIdx < Permission_MaxCount; ++nIdx)
	{
		m_lc_UserList[nIdx].Create(dwStyle | WS_VSCROLL, rectDummy, this, IDC_LC_USER_LIST_OPERA + nIdx);

		m_rb_Authority[nIdx].m_nFlatStyle = CMFCButton::BUTTONSTYLE_SEMIFLAT;
		m_rb_Authority[nIdx].SetImage(IDB_SELECTNO_16);
		m_rb_Authority[nIdx].SetCheckedImage(IDB_SELECT_16);
		m_rb_Authority[nIdx].Create(g_szPermissionMode[nIdx], dwStyle | BS_AUTORADIOBUTTON, rectDummy, this, IDC_RB_AUTHORITY_FST + nIdx);
	}

	m_st_ID.SetBackColor_COLORREF(RGB(0, 0, 0));
	m_st_ID.SetTextColor(Color::White, Color::White);
	m_st_ID.SetFont_Gdip(L"Arial", 20.0F);
	m_st_ID.Create(g_szCtrlText_T[m_nLanguage][Txt_ID], dwStyle | SS_CENTER | SS_CENTERIMAGE, rectDummy, this, IDC_STATIC);

	m_cb_ID.Create(dwStyle | CBS_DROPDOWNLIST | WS_TABSTOP, rectDummy, this, IDC_CB_ID);
	m_cb_ID.SetFont(&m_font_Default);

	m_st_Password.SetBackColor_COLORREF(RGB(0, 0, 0));
	m_st_Password.SetTextColor(Color::White, Color::White);
	m_st_Password.SetFont_Gdip(L"Arial", 20.0F);
	m_st_Password.Create(g_szCtrlText_T[m_nLanguage][Txt_Password], dwStyle | SS_CENTER | SS_CENTERIMAGE, rectDummy, this, IDC_STATIC);

	m_ed_Password.Create(dwStyle | WS_BORDER | WS_TABSTOP | ES_CENTER | ES_PASSWORD, rectDummy, this, IDC_ED_ACESS_PASSWORD);
	m_ed_Password.SetFont(&m_font_Default);
	m_ed_Password.SetFocus();
	//m_ed_Password.EnableWindow(FALSE);

	m_bn_LogIn.Create(g_szCtrlText_T[m_nLanguage][Txt_Login], WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, rectDummy, this, IDC_BN_LOGIN);
	m_bn_LogOut.Create(g_szCtrlText_T[m_nLanguage][Txt_Logout], WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, rectDummy, this, IDC_BN_LOGOUT);
	m_bn_LogIn.SetFont(&m_font_Large);
	m_bn_LogOut.SetFont(&m_font_Large);

	m_bn_Keyboard.Create(g_szCtrlText_T[m_nLanguage][Txt_Keyboard], WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, rectDummy, this, IDC_BN_KEYBOARD);

	// 기본 Radio Buttion 설정
	m_rb_Authority[Permission_Operator].SetCheck(BST_CHECKED);
	OnAuthoritySelect(IDC_RB_AUTHORITY_FST);

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
// Last Update	: 2016/11/4 - 11:45
// Desc.		:
//=============================================================================
void CDlg_UserLogin::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if ((0 == cx) || (0 == cy))
		return;

	int iMagrin			= 10;
	int iSpacing		= 5;
	int iCateSpacing	= 10;

	int iLeft			= iMagrin;
	int iTop			= iMagrin;
	int iWidth			= cx - iMagrin - iMagrin;
	int iHeight			= cy - iMagrin - iMagrin;
	int iCtrlWidth		= (iWidth - iSpacing) / 2;
	int iCtrlHeight		= 40;
	int iStWidth		= 180;
	int iEdWidth		= iWidth - iStWidth - iSpacing;
	int iSubLeft		= iLeft + iStWidth + iSpacing;
	int iListHeight		= 300 - 25;

	m_st_Title.MoveWindow(iLeft, iTop, iWidth, iCtrlHeight);
	iTop += iCtrlHeight + iSpacing;
	
	m_bn_Keyboard.MoveWindow(cx - iMagrin - iStWidth, iTop, iStWidth, 30);

	iTop += 30 + iSpacing;

	iListHeight -= (iCtrlHeight + iSpacing);
	iCtrlWidth = (iWidth - (iSpacing * (Permission_MaxCount - 1))) / Permission_MaxCount;
	for (auto nIdx = 0; nIdx < Permission_MaxCount; ++nIdx)
	{
		m_rb_Authority[nIdx].MoveWindow(iLeft, iTop, iCtrlWidth, iCtrlHeight);

		iLeft += iCtrlWidth + iSpacing;
	}

	iTop += iCtrlHeight + iSpacing;
	iLeft = iMagrin;
	for (auto nIdx = 0; nIdx < Permission_MaxCount; ++nIdx)
	{
		m_lc_UserList[nIdx].MoveWindow(iLeft, iTop, iWidth, iListHeight);
	}

	iTop += iListHeight + iCateSpacing;
	m_st_ID.MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
	m_cb_ID.MoveWindow(iSubLeft, iTop, iEdWidth, iCtrlHeight);

	iTop += iCtrlHeight + iSpacing;
	m_st_Password.MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
	m_ed_Password.MoveWindow(iSubLeft, iTop, iEdWidth, iCtrlHeight);

	iTop += iCtrlHeight + iCateSpacing;
	iCtrlWidth = (iWidth - (iSpacing * 2)) * 2 / 5;
	m_bn_LogIn.MoveWindow(iLeft, iTop, iCtrlWidth, 70);
	iLeft = cx - iMagrin - iCtrlWidth;
	m_bn_LogOut.MoveWindow(iLeft, iTop, iCtrlWidth, 70);

}

//=============================================================================
// Method		: PreTranslateMessage
// Access		: virtual protected  
// Returns		: BOOL
// Parameter	: MSG * pMsg
// Qualifier	:
// Last Update	: 2016/11/5 - 20:03
// Desc.		:
//=============================================================================
BOOL CDlg_UserLogin::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		// 여기에 Enter키 기능 작성
		if (pMsg->hwnd == m_ed_Password.GetSafeHwnd())
		{
			OnBnClicked_LogIn();
		}

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
// Method		: OnInitDialog
// Access		: virtual protected  
// Returns		: BOOL
// Qualifier	:
// Last Update	: 2016/11/5 - 20:03
// Desc.		:
//=============================================================================
BOOL CDlg_UserLogin::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	Load_UserList();

	m_ed_Password.SetFocus();
	m_ed_Password.SetSel(0, -1);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

//=============================================================================
// Method		: OnEnChangeEdPassword
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2017/1/2 - 11:33
// Desc.		:
//=============================================================================
void CDlg_UserLogin::OnEnChange_Password()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

//=============================================================================
// Method		: OnBnClickedRbOperMode
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2017/1/2 - 11:33
// Desc.		:
//=============================================================================
void CDlg_UserLogin::OnBnClicked_LogIn()
{
	if (CheckPassword())
	{
		AfxGetApp()->GetMainWnd()->SendMessage(WM_PERMISSION_MODE, (LPARAM)m_AcessMode, 0);

		OnOK();
	}
	else
	{
		m_ed_Password.SetFocus();
		m_ed_Password.SetSel(0, -1);
	}
}

//=============================================================================
// Method		: OnBnClickedRbManagerMode
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2017/1/2 - 11:33
// Desc.		:
//=============================================================================
void CDlg_UserLogin::OnBnClicked_LogOut()
{
	CReg_InspInfo	Reg;
	CString szText;
	ST_User_Unit	stUser_Prev;

	Reg.Get_LogIn_User(stUser_Prev);

	if (Reg.Set_LogOut_User())
	{
		szText.Format(_T("Set Registry Log-out [Succeed] => id: %s, name: %s, level: %d"), stUser_Prev.szID, stUser_Prev.szName, stUser_Prev.nAuthority);
		AfxGetApp()->GetMainWnd()->SendMessage(WM_LOGMSG, (WPARAM)szText.GetBuffer(), MAKELPARAM(LOGTYPE_NONE, 0));
	}
	else
	{
		szText.Format(_T("Set Registry Log-out [Faild] => id: %s, name: %s, level: %d"), stUser_Prev.szID, stUser_Prev.szName, stUser_Prev.nAuthority);
		AfxGetApp()->GetMainWnd()->SendMessage(WM_LOGMSG, (WPARAM)szText.GetBuffer(), MAKELPARAM(LOGTYPE_ERROR, 1));
	}


	m_stLogin.Reset();
	m_bLogIn = false;

	m_bn_LogOut.EnableWindow(FALSE);

	OnOK();

	m_AcessMode = Permission_Operator;
	AfxGetApp()->GetMainWnd()->SendMessage(WM_PERMISSION_MODE, (LPARAM)m_AcessMode, 0);
}

//=============================================================================
// Method		: OnBnClicked_Keyboard
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2022/1/6 - 19:35
// Desc.		:
//=============================================================================
void CDlg_UserLogin::OnBnClicked_Keyboard()
{
	RunTouchKeyboard();

	m_ed_Password.SetFocus();
	m_ed_Password.SetSel(0, -1);
}

//=============================================================================
// Method		: OnAuthoritySelect
// Access		: protected  
// Returns		: void
// Parameter	: UINT nID
// Qualifier	:
// Last Update	: 2022/7/20 - 15:08
// Desc.		:
//=============================================================================
void CDlg_UserLogin::OnAuthoritySelect(UINT nID)
{
	UINT nTabIndex = nID - IDC_RB_AUTHORITY_FST;

	switch (nTabIndex)
	{
	case Permission_Operator:
		m_lc_UserList[Permission_Engineer].ShowWindow(SW_HIDE);
		m_lc_UserList[Permission_Administrator].ShowWindow(SW_HIDE);
		break;

	case Permission_Engineer:
		m_lc_UserList[Permission_Operator].ShowWindow(SW_HIDE);
		m_lc_UserList[Permission_Administrator].ShowWindow(SW_HIDE);
		break;

	case Permission_Administrator:
		m_lc_UserList[Permission_Engineer].ShowWindow(SW_HIDE);
		m_lc_UserList[Permission_Operator].ShowWindow(SW_HIDE);
		break;

	default:
		break;
	}

	m_lc_UserList[nTabIndex].ShowWindow(SW_SHOW);
}

//=============================================================================
// Method		: OnLbnSelchangeList_User_Admin
// Access		: protected  
// Returns		: void
// Parameter	: NMHDR * pNMHDR
// Parameter	: LRESULT * pResult
// Qualifier	:
// Last Update	: 2022/7/20 - 15:08
// Desc.		:
//=============================================================================
void CDlg_UserLogin::OnLbnSelchangeList_User_Admin(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if (((pNMLV->uChanged & LVIF_STATE) &&
		(pNMLV->uNewState & (LVIS_SELECTED | LVIS_FOCUSED))))
	{
		INT nSelectItem = pNMLV->iItem;
		if (-1 == nSelectItem)
			return;


		ST_User_Unit stUser;
		if (m_lc_UserList[Permission_Administrator].Get_SelectedItem(stUser))
		{
 			int iFind = m_cb_ID.FindStringExact(0, stUser.szID);
 
 			if (0 <= iFind)
 			{


 				m_cb_ID.SetCurSel(iFind);
 			}
		}
	}
}

//=============================================================================
// Method		: OnLbnSelchangeList_User_Engineer
// Access		: protected  
// Returns		: void
// Parameter	: NMHDR * pNMHDR
// Parameter	: LRESULT * pResult
// Qualifier	:
// Last Update	: 2022/7/20 - 15:08
// Desc.		:
//=============================================================================
void CDlg_UserLogin::OnLbnSelchangeList_User_Engineer(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if (((pNMLV->uChanged & LVIF_STATE) &&
		(pNMLV->uNewState & (LVIS_SELECTED | LVIS_FOCUSED))))
	{
		INT nSelectItem = pNMLV->iItem;
		if (-1 == nSelectItem)
			return;


		ST_User_Unit stUser;
		if (m_lc_UserList[Permission_Engineer].Get_SelectedItem(stUser))
		{
			int iFind = m_cb_ID.FindString(0, stUser.szID);

			if (0 <= iFind)
			{
				m_cb_ID.SetCurSel(iFind);
			}
		}
	}
}

//=============================================================================
// Method		: OnLbnSelchangeList_User_Oper
// Access		: protected  
// Returns		: void
// Parameter	: NMHDR * pNMHDR
// Parameter	: LRESULT * pResult
// Qualifier	:
// Last Update	: 2022/7/20 - 15:08
// Desc.		:
//=============================================================================
void CDlg_UserLogin::OnLbnSelchangeList_User_Oper(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if (((pNMLV->uChanged & LVIF_STATE) &&
		(pNMLV->uNewState & (LVIS_SELECTED | LVIS_FOCUSED))))
	{
		INT nSelectItem = pNMLV->iItem;
		if (-1 == nSelectItem)
			return;


		ST_User_Unit stUser;
		if (m_lc_UserList[Permission_Operator].Get_SelectedItem(stUser))
		{
			int iFind = m_cb_ID.FindString(0, stUser.szID);

			if (0 <= iFind)
			{
				m_cb_ID.SetCurSel(iFind);
			}
		}
	}
}

//=============================================================================
// Method		: OnOK
// Access		: virtual protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2016/11/7 - 14:19
// Desc.		:
//=============================================================================
void CDlg_UserLogin::OnOK()
{
	CDialogEx::OnOK();
}

//=============================================================================
// Method		: OnCancel
// Access		: virtual protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2016/11/7 - 19:10
// Desc.		:
//=============================================================================
void CDlg_UserLogin::OnCancel()
{
	CDialogEx::OnCancel();
}

//=============================================================================
// Method		: Load_UserList
// Access		: protected  
// Returns		: bool
// Qualifier	:
// Last Update	: 2022/1/6 - 16:44
// Desc.		:
//=============================================================================
bool CDlg_UserLogin::Load_UserList()
{
	m_cb_ID.Clear();
	m_stUserList.Reset();

	CReg_InspInfo	Reg;

	if (Reg.Get_UserInfo(m_stUserList))
	{
		for (auto nIdx = 0; nIdx < m_stUserList.m_Users.size(); nIdx++)
		{
			m_lc_UserList[m_stUserList.m_Users.at(nIdx).nAuthority].Item_Insert(m_stUserList.m_Users.at(nIdx));

			m_cb_ID.AddString(m_stUserList.m_Users.at(nIdx).szID);
		}

		return true;
	}

	if (Reg.Get_LogIn_User(m_stLogin))
	{
		if (m_stUserList.IsExist_ID(m_stLogin.szID))
		{
			// 로그인된 유저 정보
			m_stLogin.szID;
			m_stLogin.szName;
			m_AcessMode = static_cast<enPermissionMode>(m_stLogin.nAuthority);

			m_bLogIn = true;
		}
		else
		{
			Reg.Set_LogOut_User();

			m_bLogIn = false;
		}
	}
	else
	{
		m_bLogIn = false;
	}

	return false;
}

//=============================================================================
// Method		: CheckPassword
// Access		: protected  
// Returns		: bool
// Qualifier	:
// Last Update	: 2016/11/7 - 18:00
// Desc.		:
//=============================================================================
bool CDlg_UserLogin::CheckPassword()
{
// 	Permission_Operator,		// 옵션 보기/수정 불가
// 	Permission_Engineer,		// 옵션 보기 가능, 수정 불가
// 	Permission_Administrator,	// 옵션 보기/수정 가능

	CString szPass;
	CString szLoadPass;
	CString szText;


	if (0 <= m_cb_ID.GetCurSel())
	{
		m_cb_ID.GetWindowText (szText);

		for (auto nIdx = 0; nIdx < m_stUserList.m_Users.size(); nIdx++)
		{
			if (0 == szText.Compare(m_stUserList.m_Users.at(nIdx).szID))
			{
				m_ed_Password.GetWindowText(szPass);

				if (0 == szPass.Compare(m_stUserList.m_Users.at(nIdx).szPassword))
				{
					// 로그인 처리
					m_stLogin = m_stUserList.m_Users.at(nIdx);
					CReg_InspInfo	Reg;				
					if (Reg.Set_LogIn_User(m_stLogin))
					{
						szText.Format(_T("Set Registry Log-in [Succeed] => id: %s, name: %s, level: %d"), m_stLogin.szID, m_stLogin.szName, m_stLogin.nAuthority);
						AfxGetApp()->GetMainWnd()->SendMessage(WM_LOGMSG, (WPARAM)szText.GetBuffer(), MAKELPARAM(LOGTYPE_NONE, 0));
					}
					else
					{
						szText.Format(_T("Set Registry Log-in [Faild] => id: %s, name: %s, level: %d"), m_stLogin.szID, m_stLogin.szName, m_stLogin.nAuthority);
						AfxGetApp()->GetMainWnd()->SendMessage(WM_LOGMSG, (WPARAM)szText.GetBuffer(), MAKELPARAM(LOGTYPE_ERROR, 1));
					}

					m_AcessMode = static_cast<enPermissionMode>(m_stLogin.nAuthority);

					return true;

				}
				else
				{
					// Password가 틀림
					//LT_MessageBox(_T("The password is incorrect."));
					LT_MessageBox(g_szMessageBox_T[MB_User_Password_Incorrect][m_nLanguage]);
					return false;
				}

			}
		}
	}
	else
	{
		// ID 선택
		//LT_MessageBox(_T("Please select ID."));
		LT_MessageBox(g_szMessageBox_T[MB_User_ID_Select][m_nLanguage]);
		return false;
	}

	return false;
}

//=============================================================================
// Method		: OnLanguage
// Access		: public  
// Returns		: void
// Parameter	: __in uint8_t IN_nLanguage
// Qualifier	:
// Last Update	: 2022/7/19 - 19:42
// Desc.		:
//=============================================================================
void CDlg_UserLogin::OnLanguage(__in uint8_t IN_nLanguage)
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
		m_font_Large.GetLogFont(&lf);
		lf.lfCharSet = Get_CharSet(m_nLanguage);
		//lf.lfHeight = (enLanguage::Lang_Chinese == m_nLanguage) ? 24 : 18;
		m_font_Large.DeleteObject();
		m_font_Large.CreateFontIndirect(&lf);

		m_cb_ID.SetFont(&m_font_Default);
		m_ed_Password.SetFont(&m_font_Default);
		m_bn_LogIn.SetFont(&m_font_Large);
		m_bn_LogOut.SetFont(&m_font_Large);

		for (auto nIdx = 0; nIdx < Permission_MaxCount; ++nIdx)
		{
			m_lc_UserList[nIdx].OnLanguage(IN_nLanguage);
		}

		if (GetSafeHwnd())
		{
			m_st_Title.SetText			(g_szCtrlText_T[m_nLanguage][Txt_Title]);
			m_st_ID.SetText				(g_szCtrlText_T[m_nLanguage][Txt_ID]);
			m_st_Password.SetText		(g_szCtrlText_T[m_nLanguage][Txt_Password]);
			m_bn_LogIn.SetWindowText	(g_szCtrlText_T[m_nLanguage][Txt_Login]);
			m_bn_LogOut.SetWindowText	(g_szCtrlText_T[m_nLanguage][Txt_Logout]);
			m_bn_Keyboard.SetWindowText	(g_szCtrlText_T[m_nLanguage][Txt_Keyboard]);
		}
	}
}

//=============================================================================
// Method		: Get_Login_User
// Access		: public  
// Returns		: ST_User_Unit&
// Qualifier	:
// Last Update	: 2022/1/6 - 18:01
// Desc.		:
//=============================================================================
ST_User_Unit& CDlg_UserLogin::Get_Login_User()
{
	return m_stLogin;
}

//=============================================================================
// Method		: SetAcessMode
// Access		: public  
// Returns		: void
// Parameter	: __in enPermissionMode nAcessMode
// Qualifier	:
// Last Update	: 2016/11/7 - 17:53
// Desc.		:
//=============================================================================
void CDlg_UserLogin::SetAcessMode(__in enPermissionMode nAcessMode)
{
	m_AcessMode = nAcessMode;
}
