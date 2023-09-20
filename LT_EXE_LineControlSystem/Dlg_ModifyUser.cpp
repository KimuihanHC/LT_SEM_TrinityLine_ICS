//*****************************************************************************
// Filename	: 	Dlg_ModifyUser.cpp
// Created	:	2021/10/20 - 12:09
// Modified	:	2021/10/20 - 12:09
//
// Author	:	piring
//	
// Purpose	:	
//*****************************************************************************
// Dlg_ModifyUser.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Dlg_ModifyUser.h"
#include "Registry.h"
#include "resource.h"
#include "CommonFunction.h"
#include "Def_Equipment_Type.h"
#include "Def_Language_Message.h"

static LPCTSTR g_szCaption_T[Lang_MaxCount][CDlg_ModifyUser::US_MaxCount] =
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

static LPCTSTR g_szCtrlText_T[Lang_MaxCount][CDlg_ModifyUser::Txt_MaxCount] =
{
	// 한국어
	{
		_T("사용자 수정"),		// Txt_DialogCaption
		_T("적용"),				// Txt_OK
		_T("취소"),				// Txt_Cancel
	},

	// 영어
	{
		_T("Modify User"),		// Txt_DialogCaption
		_T("Apply"),			// Txt_OK
		_T("Cancel"),			// Txt_Cancel
	},

	// 베트남어
	{
		_T("Modify User"),		// Txt_DialogCaption
		_T("Apply"),			// Txt_OK
		_T("Cancel"),			// Txt_Cancel
	},

	// 중국어
	{
		_T("使用者修改"),		// Txt_DialogCaption
		_T("适用"),				// Txt_OK
		_T("撤消"),				// Txt_Cancel
	},
};


#define		IDC_BN_ADD				1040
#define		IDC_BN_REMOVE			1041

#define		IDC_CB_AUTHORITY		1000
#define		IDC_ED_NAME				1001
#define		IDC_ED_ID				1002
#define		IDC_ED_USER_PASSWORD	1003
#define		IDC_ED_PASSWORD_2ND		1004


//=============================================================================
// CDlg_ModifyUser 대화 상자입니다.
//=============================================================================
IMPLEMENT_DYNAMIC(CDlg_ModifyUser, CDialogEx)

//=============================================================================
// 생성자
//=============================================================================
CDlg_ModifyUser::CDlg_ModifyUser(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlg_ModifyUser::IDD, pParent)
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
CDlg_ModifyUser::~CDlg_ModifyUser()
{
	m_font_Default.DeleteObject();
	m_Font.DeleteObject();
}

void CDlg_ModifyUser::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlg_ModifyUser, CDialogEx)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

//=============================================================================
// CDlg_ModifyUser 메시지 처리기입니다.
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
int CDlg_ModifyUser::OnCreate(LPCREATESTRUCT lpCreateStruct)
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

// 	LPCTSTR szCaption[] =
// 	{
// 		_T("Authority"),		// US_Authority
// 		_T("Name"),				// US_Name
// 		_T("ID"),				// US_ID
// 		_T("Password"),			// US_Password
// 		_T("Confirm Password"),	// US_Password_Confirm
// 		NULL,
// 	};

	for (auto nIdx = 0; nIdx < US_MaxCount; ++nIdx)
	{
		m_st_Caption[nIdx].SetStaticStyle(CVGStatic::StaticStyle_Title_Alt);
		m_st_Caption[nIdx].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_st_Caption[nIdx].SetFont_Gdip(L"Arial", 12.0F);
		m_st_Caption[nIdx].Create(g_szCaption_T[m_nLanguage][nIdx], dwStyle | SS_CENTER | SS_CENTERIMAGE, rectDummy, this, IDC_STATIC);
	}

	m_cb_Authority.Create(dwStyle | CBS_DROPDOWNLIST | WS_TABSTOP, rectDummy, this, IDC_CB_AUTHORITY);
	
	m_ed_Name.Create(dwStyle | ES_CENTER | WS_BORDER, rectDummy, this, IDC_ED_NAME);
	m_ed_Name.SetFont(&m_font_Default);
	m_ed_ID.Create(dwStyle | ES_CENTER | WS_BORDER, rectDummy, this, IDC_ED_ID);
	m_ed_ID.SetFont(&m_font_Default);
	m_ed_Password.Create(dwStyle | ES_CENTER | WS_BORDER | ES_PASSWORD, rectDummy, this, IDC_ED_USER_PASSWORD);
	m_ed_Password.SetFont(&m_font_Default);
	m_ed_Password_Confirm.Create(dwStyle | ES_CENTER | WS_BORDER | ES_PASSWORD, rectDummy, this, IDC_ED_PASSWORD_2ND);
	m_ed_Password_Confirm.SetFont(&m_font_Default);

	m_cb_Authority.SetFont(&m_font_Default);
	for (auto nIdx = 0; nIdx < Permission_MaxCount; ++nIdx)
	{
		m_cb_Authority.AddString(g_szPermissionMode[nIdx]);
	}
	m_cb_Authority.SetCurSel(0);

	m_ed_Name.EnableWindow(FALSE);
	m_ed_ID.EnableWindow(FALSE);

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
void CDlg_ModifyUser::OnSize(UINT nType, int cx, int cy)
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
	int iCtrlWidth		= 400;
	int iStWidth		= 180;
	int iEdWidth		= iWidth - iStWidth - iSpacing;
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

	// 버튼
	iCtrlWidth = iWidth / 3;
	iCtrlHeight = 80;
	iLeft = iMargin;
	iLeft_Sub = cx - iMargin - iCtrlWidth;
	iTop = cy - iMargin - iCtrlHeight;
	m_bn_OK.MoveWindow(iLeft, iTop, iCtrlWidth, iCtrlHeight);
	m_bn_Cancel.MoveWindow(iLeft_Sub, iTop, iCtrlWidth, iCtrlHeight);
}

//=============================================================================
// Method		: OnInitDialog
// Access		: virtual protected  
// Returns		: BOOL
// Qualifier	:
// Last Update	: 2021/11/10 - 14:26
// Desc.		:
//=============================================================================
BOOL CDlg_ModifyUser::OnInitDialog()
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
BOOL CDlg_ModifyUser::PreTranslateMessage(MSG* pMsg)
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
void CDlg_ModifyUser::OnCancel()
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
void CDlg_ModifyUser::OnOK()
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
// Parameter	: __out ST_User_Unit & OUT_stUser
// Qualifier	:
// Last Update	: 2022/1/7 - 13:08
// Desc.		:
//=============================================================================
bool CDlg_ModifyUser::Get_UIData(__out ST_User_Unit& OUT_stUser)
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
// Method		: Set_UIData
// Access		: protected  
// Returns		: void
// Parameter	: __in const ST_User_Unit * IN_pstUser
// Qualifier	:
// Last Update	: 2022/1/7 - 13:39
// Desc.		:
//=============================================================================
void CDlg_ModifyUser::Set_UIData(__in const ST_User_Unit* IN_pstUser)
{
	m_cb_Authority.SetCurSel(IN_pstUser->nAuthority);
	if (0 == IN_pstUser->szID.Compare(_T("Admin")))
	{
		m_cb_Authority.EnableWindow(FALSE);
	}

	m_ed_ID.SetWindowText (IN_pstUser->szID);

	m_ed_Name.SetWindowText (IN_pstUser->szName);

	m_ed_Password.SetWindowText (IN_pstUser->szPassword);
	m_ed_Password_Confirm.SetWindowText (IN_pstUser->szPassword);
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
void CDlg_ModifyUser::OnLanguage(__in uint8_t IN_nLanguage)
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
		//lf.lfHeight = (enLanguage::Lang_Chinese == m_nLanguage) ? 24 : 18;
		m_Font.DeleteObject();
		m_Font.CreateFontIndirect(&lf);

		m_bn_OK.SetFont(&m_font_Default);
		m_bn_Cancel.SetFont(&m_font_Default);
		m_ed_Name.SetFont(&m_font_Default);
		m_ed_ID.SetFont(&m_font_Default);
		m_ed_Password.SetFont(&m_font_Default);
		m_ed_Password_Confirm.SetFont(&m_font_Default);

		m_cb_Authority.SetFont(&m_font_Default);

		if (GetSafeHwnd())
		{
			SetWindowText(g_szCtrlText_T[m_nLanguage][Txt_DialogCaption]);
			m_bn_OK.SetWindowText(g_szCtrlText_T[m_nLanguage][Txt_OK]);
			m_bn_Cancel.SetWindowText(g_szCtrlText_T[m_nLanguage][Txt_Cancel]);

			for (auto nIdx = 0; nIdx < US_MaxCount; ++nIdx)
			{
				m_st_Caption[nIdx].SetText(g_szCaption_T[m_nLanguage][nIdx]);
			}
		}
	}
}

//=============================================================================
// Method		: Set_NGCodeData
// Access		: public  
// Returns		: void
// Parameter	: __in const ST_User_Unit * IN_pstUser
// Qualifier	:
// Last Update	: 2021/11/10 - 14:27
// Desc.		:
//=============================================================================
void CDlg_ModifyUser::Set_NGCodeData(__in const ST_User_Unit* IN_pstUser)
{
	m_stNGCode = *IN_pstUser;
}

//=============================================================================
// Method		: Get_NGCodeData
// Access		: public  
// Returns		: ST_User_Unit&
// Qualifier	:
// Last Update	: 2021/11/10 - 14:27
// Desc.		:
//=============================================================================
ST_User_Unit& CDlg_ModifyUser::Get_NGCodeData()
{
	return m_stNGCode;
}

