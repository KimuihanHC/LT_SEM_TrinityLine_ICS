//*****************************************************************************
// Filename	: 	Wnd_Cfg_Socket.cpp
// Created	:	2016/3/14 - 10:57
// Modified	:	2016/3/14 - 10:57
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
// Wnd_Cfg_Socket.cpp : implementation file
//

#include "stdafx.h"
#include "Wnd_Cfg_Socket.h"
#include "resource.h"
#include "XmlSocket.h"
#include "CommonFunction.h"
#include "Def_Language_Message.h"


static LPCTSTR g_szModel_Static_T[Lang_MaxCount][CWnd_Cfg_Socket::Item_MaxCount] =
{
	// 한국어
	{
		_T("RFID"),				// Item_RFID
		_T("RFID 접두어"),		// Item_Prefix
		_T("시작 번호"),			// Item_BeginNo
		_T("종료 번호"),			// Item_EndNo
		_T("소켓 지그 종류"),		// Item_SocketType
		_T("설명"),				// Item_Comment
	},

	// 영어
	{
		_T("RFID"),				// Item_RFID
		_T("RFID Prefix"),		// Item_Prefix
		_T("Begin No."),		// Item_BeginNo
		_T("End No."),			// Item_EndNo
		_T("Socket JIG Type"),	// Item_SocketType
		_T("Comment"),			// Item_Comment
	},

	// 베트남어
	{
		_T("RFID"),				// Item_RFID
		_T("RFID Prefix"),		// Item_Prefix
		_T("Begin No."),		// Item_BeginNo
		_T("End No."),			// Item_EndNo
		_T("Socket JIG Type"),	// Item_SocketType
		_T("Comment"),			// Item_Comment
	},

	// 중국어
	{
		_T("RFID"),				// Item_RFID
		_T("RFID 前缀"),			// Item_Prefix
		_T("开始编号"),			// Item_BeginNo
		_T("结束编号"),			// Item_EndNo
		_T("插口夹具类型"),		// Item_SocketType
		_T("评语"),				// Item_Comment
	},
};

static LPCTSTR g_szCtrlText_T[Lang_MaxCount][CWnd_Cfg_Socket::Txt_MaxCount] =
{
	// 한국어
	{
		_T("적용"),					// Txt_Apply
		_T("다시 읽기"),				// Txt_Reload
		_T("단품처리"),				// Txt_Single
		_T("일괄처리"),				// Txt_Multi		
		_T("소켓 지그 추가"),			// Txt_Add
		_T("소켓 지그 제거"),			// Txt_Remove
		_T("목록 지우기"),			// Txt_Clear		
		_T("목록 정렬"),				// Txt_Sort
		_T("번호"),					// Txt_RFID_Begin_Single
	},

	// 영어
	{
		_T("Apply"),					// Txt_Apply
		_T("Reload"),					// Txt_Reload
		_T("Single"),					// Txt_Single
		_T("Batch"),					// Txt_Multi		
		_T("Add Socket JIG"),			// Txt_Add
		_T("Remove Socket JIG"),		// Txt_Remove
		_T("Clear List"),				// Txt_Clear		
		_T("Sort List"),				// Txt_Sort
		_T("Number"),					// Txt_RFID_Begin_Single
	},

	// 베트남어
	{
		_T("Apply"),					// Txt_Apply
		_T("Reload"),					// Txt_Reload
		_T("Single"),					// Txt_Single
		_T("Batch"),					// Txt_Multi		
		_T("Add Socket JIG"),			// Txt_Add
		_T("Remove Socket JIG"),		// Txt_Remove
		_T("Clear List"),				// Txt_Clear		
		_T("Sort List"),				// Txt_Sort
		_T("Number"),					// Txt_RFID_Begin_Single
	},

	// 중국어
	{
		_T("适用"),						// Txt_Apply
		_T("重读"),						// Txt_Reload
		_T("单一类型"),					// Txt_Single
		_T("分批类型"),					// Txt_Multi		
		_T("添加插口夹具"),				// Txt_Add
		_T("删除插口夹具"),				// Txt_Remove
		_T("清空列表"),					// Txt_Clear		
		_T("排序"),						// Txt_Sort
		_T("号码"),						// Txt_RFID_Begin_Single
	},
};


#define		IDC_RB_SINGLE		1001
#define		IDC_RB_MULTI		1002

#define		IDC_ED_PREFIX		1010
#define		IDC_ED_BEGIN_NO		1011
#define		IDC_ED_END_NO		1012
#define		IDC_ED_COMMENT		1013

#define		IDC_CB_SOCKETTYPE	1020

#define		IDC_LC_SOCKETLIST	1030

#define		IDC_BN_ADD			1040
#define		IDC_BN_REMOVE		1041
#define		IDC_BN_SORT			1042
#define		IDC_BN_CLEAR		1043

#define		IDC_BN_SAVE			1050
#define		IDC_BN_RELOAD		1051


#define		TABSTYLE_COUNT			13
static UINT g_TabOrder[TABSTYLE_COUNT] =
{
	IDC_ED_PREFIX, 
	IDC_RB_SINGLE, 
	IDC_RB_MULTI, 
	IDC_ED_BEGIN_NO, 
	IDC_ED_END_NO, 
	IDC_CB_SOCKETTYPE, 
	IDC_ED_COMMENT, 
	IDC_BN_ADD, 
	IDC_BN_REMOVE, 
	IDC_BN_SORT,
	IDC_BN_CLEAR,
	IDC_BN_RELOAD,
	IDC_BN_SAVE, 
};

// CWnd_Cfg_Socket
IMPLEMENT_DYNAMIC(CWnd_Cfg_Socket, CWnd_BaseView)

//=============================================================================
//
//=============================================================================
CWnd_Cfg_Socket::CWnd_Cfg_Socket()
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
CWnd_Cfg_Socket::~CWnd_Cfg_Socket()
{
	m_font_Default.DeleteObject();
	m_font_Data.DeleteObject();
}


BEGIN_MESSAGE_MAP(CWnd_Cfg_Socket, CWnd_BaseView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND_RANGE(IDC_RB_SINGLE, IDC_RB_MULTI,	OnBnClicked_RFID)
	ON_CBN_SELENDOK(IDC_CB_SOCKETTYPE,				OnCbnSelendokSocketType)
	ON_BN_CLICKED(IDC_BN_ADD,						OnBnClicked_Add)
	ON_BN_CLICKED(IDC_BN_REMOVE,					OnBnClicked_Remove)
	ON_BN_CLICKED(IDC_BN_CLEAR,						OnBnClicked_Clear)
	ON_BN_CLICKED(IDC_BN_SORT,						OnBnClicked_Sort)
	ON_BN_CLICKED(IDC_BN_SAVE,						OnBnClicked_Apply)
	ON_BN_CLICKED(IDC_BN_RELOAD,					OnBnClicked_Reload)
END_MESSAGE_MAP()


// CWnd_Cfg_Socket message handlers
//=============================================================================
// Method		: OnCreate
// Access		: public  
// Returns		: int
// Parameter	: LPCREATESTRUCT lpCreateStruct
// Qualifier	:
// Last Update	: 2016/3/17 - 11:33
// Desc.		:
//=============================================================================
int CWnd_Cfg_Socket::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd_BaseView::OnCreate(lpCreateStruct) == -1)
		return -1;

	DWORD dwStyle = WS_VISIBLE | WS_CHILD /*| WS_CLIPCHILDREN*/ | WS_CLIPSIBLINGS;
	DWORD dwTempStyle = dwStyle;
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	for (UINT nIdx = 0; nIdx < Item_MaxCount; nIdx++)
	{
		m_st_Item[nIdx].SetStaticStyle(CVGStatic::StaticStyle_Title_Alt);
		m_st_Item[nIdx].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_st_Item[nIdx].SetFont_Gdip(L"Arial", 10.0F);
		m_st_Item[nIdx].Create(g_szModel_Static_T[m_nLanguage][nIdx], dwStyle | SS_CENTER | SS_CENTERIMAGE, rectDummy, this, IDC_STATIC);
	}

	m_rb_Single.Create(g_szCtrlText_T[m_nLanguage][Txt_Single], dwStyle | BS_AUTORADIOBUTTON | BS_PUSHLIKE, rectDummy, this, IDC_RB_SINGLE);
	m_rb_Multi.Create(g_szCtrlText_T[m_nLanguage][Txt_Multi], dwStyle | BS_AUTORADIOBUTTON | BS_PUSHLIKE | WS_TABSTOP, rectDummy, this, IDC_RB_MULTI);
	m_rb_Single.SetFont(&m_font_Default);
	m_rb_Multi.SetFont(&m_font_Default);
	m_rb_Single.SetImage(IDB_SELECTNO_16);
	m_rb_Single.SetCheckedImage(IDB_SELECT_16);
	m_rb_Single.SizeToContent();
	m_rb_Multi.SetImage(IDB_SELECTNO_16);
	m_rb_Multi.SetCheckedImage(IDB_SELECT_16);
	m_rb_Multi.SizeToContent();

	m_ed_RFID_Prefix.Create(dwStyle | WS_BORDER | ES_CENTER, rectDummy, this, IDC_ED_PREFIX);
	m_ed_RFID_Prefix.SetFont(&m_font_Data);
	m_ed_RFID_Begin.Create(dwStyle | WS_BORDER | ES_CENTER, rectDummy, this, IDC_ED_BEGIN_NO);
	m_ed_RFID_Begin.SetFont(&m_font_Data);
	m_ed_RFID_End.Create(dwStyle | WS_BORDER | ES_CENTER, rectDummy, this, IDC_ED_END_NO);
	m_ed_RFID_End.SetFont(&m_font_Data);
	m_ed_Comment.Create(dwStyle | WS_BORDER | ES_CENTER, rectDummy, this, IDC_ED_COMMENT);
	m_ed_Comment.SetFont(&m_font_Data);

	m_cb_SocketType.Create(dwStyle | WS_BORDER | CBS_DROPDOWNLIST, rectDummy, this, IDC_CB_SOCKETTYPE);
	m_cb_SocketType.SetFont(&m_font_Data);
	for (auto nIdx = 0; nIdx < Max_SocketTypeCount; ++nIdx)
	{
		m_cb_SocketType.AddString(g_szSocketTypeName[nIdx]);
	}
	m_cb_SocketType.SetCurSel(0);
		
	m_lc_SocketList.Create(dwStyle | WS_VSCROLL, rectDummy, this, IDC_LC_SOCKETLIST);

	m_bn_Apply.Create(g_szCtrlText_T[m_nLanguage][Txt_Apply], dwStyle | WS_BORDER | BS_PUSHBUTTON, rectDummy, this, IDC_BN_SAVE);
	m_bn_Reload.Create(g_szCtrlText_T[m_nLanguage][Txt_Reload], dwStyle | WS_BORDER | BS_PUSHBUTTON, rectDummy, this, IDC_BN_RELOAD);

	m_bn_Add.Create(g_szCtrlText_T[m_nLanguage][Txt_Add], dwStyle | WS_BORDER | BS_PUSHBUTTON, rectDummy, this, IDC_BN_ADD);
	m_bn_Remove.Create(g_szCtrlText_T[m_nLanguage][Txt_Remove], dwStyle | WS_BORDER | BS_PUSHBUTTON, rectDummy, this, IDC_BN_REMOVE);
	m_bn_Clear.Create(g_szCtrlText_T[m_nLanguage][Txt_Clear], dwStyle | WS_BORDER | BS_PUSHBUTTON, rectDummy, this, IDC_BN_CLEAR);
	m_bn_Sort.Create(g_szCtrlText_T[m_nLanguage][Txt_Sort], dwStyle | WS_BORDER | BS_PUSHBUTTON, rectDummy, this, IDC_BN_SORT);

	// H-220215-001
	m_ed_RFID_Prefix.EnableMask(_T("A-AAAAAA-"), _T("_-______-"), _T('_'));
	m_ed_RFID_Prefix.SetValidChars(NULL);
	m_ed_RFID_Prefix.EnableGetMaskedCharsOnly(FALSE);

	m_ed_RFID_Begin.EnableMask(_T("ddd"), _T("___"), _T('_'));
	m_ed_RFID_Begin.SetValidChars(_T("0123456789"));
	m_ed_RFID_End.EnableMask(_T("ddd"), _T("___"), _T('_'));
	m_ed_RFID_End.SetValidChars(_T("0123456789"));

#if (SET_INSPECTOR == SYS_ICS_RIVIAN_LINE)
	m_ed_RFID_Prefix.SetWindowText(_T("A-230114-"));
#else
	m_ed_RFID_Prefix.SetWindowText(_T("H-220215-"));
#endif
	m_ed_RFID_Begin.SetWindowText(_T("000"));
	m_ed_RFID_End.SetWindowText(_T("099"));

	// 초기 설정
	m_rb_Single.SetCheck(BST_CHECKED);
	OnBnClicked_RFID(IDC_RB_SINGLE);

	m_ed_RFID_Prefix.SetFocus();


	// 저장된 데이터 불러오기
	LoadXML_SocketList();

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
void CWnd_Cfg_Socket::OnSize(UINT nType, int cx, int cy)
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
	int iStWidth		= 100;
	int iEdWidth		= iCtrlWidth - iStWidth - iSpacing;
	int iQuadSt_W		= 100;
	int iQuadEd_W		= iEdWidth - iQuadSt_W;
	int iLeftSub		= iLeft + iStWidth + iSpacing;
	int iListWidth		= 800;
	int iListHeight		= iHeight - (iCtrlHeight + iSpacing);

	iLeft = iMargin + iCtrlWidth + iSpacing + iListWidth - 250;
	m_bn_Apply.MoveWindow(iLeft, iTop, 250, iCtrlHeight);

	// RFID 입력
	iLeft			= iMargin;
	iTop += iCtrlHeight + iSpacing;
	m_st_Item[Item_RFID].MoveWindow(iLeft, iTop, iCtrlWidth, iCtrlHeight);

	iTop += iCtrlHeight + iSpacing;
	m_st_Item[Item_RFID_Prefix].MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
	iLeft += iStWidth + iSpacing;
	m_ed_RFID_Prefix.MoveWindow(iLeft, iTop, iEdWidth, iCtrlHeight);

	// RFID Single
	iLeft = iMargin;
	iTop += iCtrlHeight + iSpacing;
	m_rb_Single.MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);

	iLeft += iStWidth + iSpacing;
	m_st_Item[Item_RFID_Begin].MoveWindow(iLeft, iTop, iQuadSt_W, iCtrlHeight);
	iLeft += iQuadSt_W;
	m_ed_RFID_Begin.MoveWindow(iLeft, iTop, iQuadEd_W, iCtrlHeight);
	
	// RFID Multiple
	iLeft = iMargin;
	iTop += iCtrlHeight + iSpacing;
	m_rb_Multi.MoveWindow(iLeft, iTop, iStWidth, iCtrlHeight);
	
	iLeft += iStWidth + iSpacing;
	m_st_Item[Item_RFID_End].MoveWindow(iLeft, iTop, iQuadSt_W, iCtrlHeight);
	iLeft += iQuadSt_W;
	m_ed_RFID_End.MoveWindow(iLeft, iTop, iQuadEd_W, iCtrlHeight);
	
	// Socket Type
	iLeft = iMargin;
	iTop += iCtrlHeight + iCateSpacing;
	m_st_Item[Item_SocketType].MoveWindow(iLeft, iTop, iCtrlWidth, iCtrlHeight);
	iTop += iCtrlHeight + iSpacing;
	m_cb_SocketType.MoveWindow(iLeft, iTop, iCtrlWidth, iCtrlHeight);

	// Comment
	iTop += iCtrlHeight + iCateSpacing;
	m_st_Item[Item_Comment].MoveWindow(iLeft, iTop, iCtrlWidth, iCtrlHeight);
	iTop += iCtrlHeight + iSpacing;
	m_ed_Comment.MoveWindow(iLeft, iTop, iCtrlWidth, iCtrlHeight);

	// 추가, 제거, 저장
	iTop += iCtrlHeight + 30;
	iQuadSt_W = (iCtrlWidth - iSpacing) / 2;
	m_bn_Add.MoveWindow(iLeft, iTop, iQuadSt_W, iCtrlHeight);
	iLeft += iQuadSt_W + iSpacing;
	m_bn_Remove.MoveWindow(iLeft, iTop, iQuadSt_W, iCtrlHeight);

	iTop += iCtrlHeight + iCateSpacing;
	iLeft = iMargin;
	m_bn_Sort.MoveWindow(iLeft, iTop, iQuadSt_W, iCtrlHeight);
	iLeft += iQuadSt_W + iSpacing;
	m_bn_Clear.MoveWindow(iLeft, iTop, iQuadSt_W, iCtrlHeight);


	iLeft = iMargin;
	iTop = cy - iCtrlHeight - iMargin;
	m_bn_Reload.MoveWindow(iLeft, iTop, iCtrlWidth, iCtrlHeight);

	iTop = iTop - iCtrlHeight - iMargin;
	//m_bn_Apply.MoveWindow(iLeft, iTop, iCtrlWidth, iCtrlHeight);

	// 리스트 컨트롤
	iTop = iMargin + iCtrlHeight + iSpacing;
	iLeft = iMargin + iCtrlWidth + iCateSpacing;;
	iWidth = cx - iLeft - iMargin;
	m_lc_SocketList.MoveWindow(iLeft, iTop, iListWidth, iListHeight);

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
BOOL CWnd_Cfg_Socket::PreCreateWindow(CREATESTRUCT& cs)
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
BOOL CWnd_Cfg_Socket::PreTranslateMessage(MSG* pMsg)
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
// Method		: OnBnClicked_RFID
// Access		: public  
// Returns		: void
// Parameter	: UINT nID
// Qualifier	:
// Last Update	: 2021/11/12 - 19:33
// Desc.		:
//=============================================================================
void CWnd_Cfg_Socket::OnBnClicked_RFID(UINT nID)
{
	switch (nID)
	{
	case IDC_RB_SINGLE:
		m_st_Item[Item_RFID_Begin].SetWindowText(g_szCtrlText_T[m_nLanguage][Txt_RFID_Begin_Single]);
		m_st_Item[Item_RFID_End].ShowWindow(SW_HIDE);
		m_ed_RFID_End.ShowWindow(SW_HIDE);
		break;

	case IDC_RB_MULTI:
		m_st_Item[Item_RFID_Begin].SetWindowText(g_szModel_Static_T[m_nLanguage][Item_RFID_Begin]);
		m_st_Item[Item_RFID_End].ShowWindow(SW_SHOW);
		m_ed_RFID_End.ShowWindow(SW_SHOW);
		break;

	default:
		break;
	}
}

//=============================================================================
// Method		: OnCbnSelendokSocketType
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/11/15 - 10:53
// Desc.		:
//=============================================================================
void CWnd_Cfg_Socket::OnCbnSelendokSocketType()
{
	int iSelType = m_cb_SocketType.GetCurSel();

	if (0 <= iSelType)
	{
		// #if (SET_INSPECTOR == SYS_ICS_RIVIAN_LINE)
		// 		switch (iSelType)
		// 		{
		// 		case enSocketType::Socket_30_FOV:
		// 			m_ed_RFID_Prefix.SetWindowText(_T("A-230114-"));
		// 			break;
		// 
		// 		case enSocketType::Socket_180_FOV:
		// 			m_ed_RFID_Prefix.SetWindowText(_T("B-230114-"));
		// 			break;
		// 		}
		// #else
		// 		switch (iSelType)
		// 		{
		// 		case enSocketType::Socket_H:
		// 			m_ed_RFID_Prefix.SetWindowText(_T("H-220215-"));
		// 			break;
		// 
		// 		case enSocketType::Socket_V:
		// 			m_ed_RFID_Prefix.SetWindowText(_T("V-220215-"));
		// 			break;
		// 
		// 		case enSocketType::Socket_Highland:
		// 			m_ed_RFID_Prefix.SetWindowText(_T("L-220215-"));
		// 			break;
		// 		}
		// #endif // (SET_INSPECTOR == SYS_ICS_RIVIAN_LINE)
	}

}

//=============================================================================
// Method		: OnBnClicked_Add
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/11/15 - 11:21
// Desc.		:
//=============================================================================
void CWnd_Cfg_Socket::OnBnClicked_Add()
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
void CWnd_Cfg_Socket::OnBnClicked_Remove()
{
	if (0 < m_lc_SocketList.GetSelectedCount())
	{
		Item_Remove();
	}
}

//=============================================================================
// Method		: OnBnClicked_Clear
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/11/17 - 15:48
// Desc.		:
//=============================================================================
void CWnd_Cfg_Socket::OnBnClicked_Clear()
{
	if (0 < m_lc_SocketList.GetItemCount())
	{
		Item_Clear();
	}
}

//=============================================================================
// Method		: OnBnClicked_Sort
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/11/17 - 15:48
// Desc.		:
//=============================================================================
void CWnd_Cfg_Socket::OnBnClicked_Sort()
{
	if (0 < m_lc_SocketList.GetItemCount())
	{
		Item_Sort();
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
void CWnd_Cfg_Socket::OnBnClicked_Apply()
{
	// 파일에 저장
	if (SaveXML_SocketList())
	{
		//LT_MessageBox(_T("The file has been saved successfully."));
		LT_MessageBox(g_szMessageBox_T[MB_Common_SaveFile_Succeed][m_nLanguage]);
	}
	else
	{
		//LT_MessageBox(_T("Failed to save file !!"));
		LT_MessageBox(g_szMessageBox_T[MB_Common_SaveFile_Failed][m_nLanguage]);
	}
}

//=============================================================================
// Method		: OnBnClicked_Reload
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/11/17 - 19:20
// Desc.		:
//=============================================================================
void CWnd_Cfg_Socket::OnBnClicked_Reload()
{
	//if (IDYES == LT_MessageBox(_T("Reload socket information from file?"), MB_YESNO))
	if (IDYES == LT_MessageBox(g_szMessageBox_T[MB_Socket_Reload][m_nLanguage], MB_YESNO))
	{
		if (FALSE == LoadXML_SocketList())
		{
			//LT_MessageBox(_T("Failed to load file !!"));
			LT_MessageBox(g_szMessageBox_T[MB_Common_LoadFile_Failed][m_nLanguage]);
		}
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
void CWnd_Cfg_Socket::Item_Insert()
{
	CConfig_Socket stSocket;

	Get_UIData(stSocket);

	if (BST_CHECKED == m_rb_Single.GetCheck())
	{
		if (false == m_lc_SocketList.Item_Insert(stSocket))
		{
			//LT_MessageBox(_T("Insert failed, RFID already exists."));
			LT_MessageBox(g_szMessageBox_T[MB_Socket_InsertFailed][m_nLanguage]);
		}
	}
	else // if (BST_CHECKED == m_rb_Multi.GetCheck())
	{
		CString szPrefix;
		CString szBegin;
		CString szEnd;

		m_ed_RFID_Prefix.GetWindowText(szPrefix); // Masked 문자 처리가 안됨
		m_ed_RFID_Begin.GetWindowText(szBegin);
		m_ed_RFID_End.GetWindowText(szEnd);

		int nBegin	= _ttoi(szBegin);
		int nEnd	= _ttoi(szEnd);
		
		if (nBegin < nEnd)
		{
			int nCount = nEnd - nBegin + 1;
			CString szMsg;
			//szMsg.Format(_T("총 %d 개의 소켓을 추가하시겠습니까?"), nCount);
			//szMsg.Format(_T("Do you want to add a total of %d sockets?"), nCount);
			if (m_nLanguage == enLanguage::Lang_Chinese)
			{
				szMsg.Format(g_szMessageBox_T[MB_Socket_Add_Selected][m_nLanguage], nCount, nCount);
			}
			else
			{
				szMsg.Format(g_szMessageBox_T[MB_Socket_Add_Selected][m_nLanguage], nCount);
			}
			
			if (IDYES == LT_MessageBox(szMsg.GetBuffer(), MB_YESNO))
			{
				bool bSuccess = true;
				for (auto nIdx = nBegin; nIdx <= nEnd; ++nIdx)
				{
					CConfig_Socket stUnit;

					//stUnit.szRFID.Format(_T("%s%04d"), szPrefix, nIdx);
					stUnit.szRFID.Format(_T("%s%03d"), szPrefix, nIdx);

					stUnit.nSocketType = stSocket.nSocketType;
					stUnit.szComment = stSocket.szComment;

					if (false == m_lc_SocketList.Item_Insert(stUnit))
					{
						bSuccess = false;
					}
				}

				if (false == bSuccess)
				{
					//LT_MessageBox(_T("Some Insert failed, Some RFID already exists."));
					LT_MessageBox(g_szMessageBox_T[MB_Socket_Add_Failed_Batch][m_nLanguage]);
				}
			}
		}
		else
		{
			//LT_MessageBox(_T("End 수치가 Begin 수치보다 커야 합니다."));
			//LT_MessageBox(_T("The End value must be greater than the Begin value."));
			LT_MessageBox(g_szMessageBox_T[MB_Socket_Add_Failed_Batch_No][m_nLanguage]);
		}
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
void CWnd_Cfg_Socket::Item_Remove()
{
	if (0 < m_lc_SocketList.GetSelectedCount())
	{
		CConfig_Socket stSocket;
		m_lc_SocketList.Get_SelectedSocket(stSocket);

		CString szMsg;
		//szMsg.Format(_T("Are you sure you want to delete \"%s\" socket in the list?"), stSocket.szRFID.GetBuffer());
		if (m_nLanguage == enLanguage::Lang_Chinese)
		{
			szMsg.Format(g_szMessageBox_T[MB_Socket_Delete_Selected][m_nLanguage], stSocket.szRFID.GetBuffer(), stSocket.szRFID.GetBuffer());
		}
		else
		{
			szMsg.Format(g_szMessageBox_T[MB_Socket_Delete_Selected][m_nLanguage], stSocket.szRFID.GetBuffer());
		}

		if (IDYES == LT_MessageBox(szMsg.GetBuffer(), MB_YESNO))
		{
			m_lc_SocketList.Item_Remove();
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
void CWnd_Cfg_Socket::Item_Modify()
{

	//m_lc_SocketList.Item_Modify();
}

//=============================================================================
// Method		: Item_Clear
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/11/15 - 16:31
// Desc.		:
//=============================================================================
void CWnd_Cfg_Socket::Item_Clear()
{
	//if (IDYES == LT_MessageBox(_T("리스트의 모든 소켓을 삭제 하시겠습니까?"), MB_YESNO))
	//if (IDYES == LT_MessageBox(_T("Are you sure you want to delete all sockets in the list?"), MB_YESNO))
	if (IDYES == LT_MessageBox(g_szMessageBox_T[MB_Socket_Delete_All][m_nLanguage], MB_YESNO))
	{
		m_lc_SocketList.Clear_SocketList();
	}
}

//=============================================================================
// Method		: Item_Sort
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/11/15 - 16:31
// Desc.		:
//=============================================================================
void CWnd_Cfg_Socket::Item_Sort()
{
	m_lc_SocketList.Refresh_Socket();
}

//=============================================================================
// Method		: SaveXML_SocketList
// Access		: public  
// Returns		: BOOL
// Qualifier	:
// Last Update	: 2021/11/15 - 11:22
// Desc.		:
//=============================================================================
BOOL CWnd_Cfg_Socket::SaveXML_SocketList()
{
	CXmlSocket xml;

	CSocketList stSocketList;
	m_lc_SocketList.Get_SocketList(stSocketList);

	BOOL bReturn = xml.SaveXML_Socket_CfgList(m_szPath_Default.GetBuffer(), &stSocketList);

	// Socket 데이터 불러오기
	GetOwner()->SendNotifyMessage(WM_CHANGED_SETTING_INFO, (WPARAM)m_szPath_Default.GetBuffer(), WM_Setting_Socket);

	return bReturn;
}

//=============================================================================
// Method		: LoadXML_SocketList
// Access		: public  
// Returns		: BOOL
// Qualifier	:
// Last Update	: 2021/11/17 - 16:59
// Desc.		:
//=============================================================================
BOOL CWnd_Cfg_Socket::LoadXML_SocketList()
{
	CXmlSocket xml;
	CSocketList stSocketList;

	if (xml.LoadXML_Socket_CfgList(m_szPath_Default.GetBuffer(), stSocketList))
	{
		m_lc_SocketList.Set_SocketList(&stSocketList);
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

//=============================================================================
// Method		: Get_UIData
// Access		: public  
// Returns		: void
// Parameter	: __out CConfig_Model & OUT_stSocket
// Qualifier	:
// Last Update	: 2021/6/14 - 16:33
// Desc.		:
//=============================================================================
void CWnd_Cfg_Socket::Get_UIData(__out CConfig_Socket& OUT_stSocket)
{
	CString szValue;	
	CString szNumber;

	// RFID
	m_ed_RFID_Prefix.GetWindowText(szValue);
	m_ed_RFID_Begin.GetWindowText(szNumber);

	OUT_stSocket.szRFID = szValue + szNumber;;

	// Socket Type
	int iSel = m_cb_SocketType.GetCurSel();
	if (0 <= iSel)
	{
		OUT_stSocket.nSocketType = iSel;
	}

	// Comment
	m_ed_Comment.GetWindowText(szValue);
	OUT_stSocket.szComment = szValue;

}

//=============================================================================
// Method		: Set_UIData
// Access		: public  
// Returns		: void
// Parameter	: __in const CConfig_Model * IN_pSocket
// Qualifier	:
// Last Update	: 2021/6/14 - 16:33
// Desc.		:
//=============================================================================
void CWnd_Cfg_Socket::Set_UIData(__in const CConfig_Socket* IN_pSocket)
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
void CWnd_Cfg_Socket::OnLanguage(__in uint8_t IN_nLanguage)
{
	__super::OnLanguage(IN_nLanguage);

	m_lc_SocketList.OnLanguage(IN_nLanguage);

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

	m_rb_Single.SetFont(&m_font_Default);
	m_rb_Multi.SetFont(&m_font_Default);
	m_ed_RFID_Prefix.SetFont(&m_font_Data);
	m_ed_RFID_Begin.SetFont(&m_font_Data);
	m_ed_RFID_End.SetFont(&m_font_Data);
	m_ed_Comment.SetFont(&m_font_Data);
	m_cb_SocketType.SetFont(&m_font_Data);

	if (GetSafeHwnd())
	{
		for (UINT nIdx = 0; nIdx < Item_MaxCount; nIdx++)
		{
			m_st_Item[nIdx].SetText(g_szModel_Static_T[m_nLanguage][nIdx]);
		}

		m_rb_Single.SetWindowText(g_szCtrlText_T[m_nLanguage][Txt_Single]);
		m_rb_Multi.SetWindowText(g_szCtrlText_T[m_nLanguage][Txt_Multi]);
		m_bn_Apply.SetWindowText(g_szCtrlText_T[m_nLanguage][Txt_Apply]);
		m_bn_Reload.SetWindowText(g_szCtrlText_T[m_nLanguage][Txt_Reload]);

		m_bn_Add.SetWindowText(g_szCtrlText_T[m_nLanguage][Txt_Add]);
		m_bn_Remove.SetWindowText(g_szCtrlText_T[m_nLanguage][Txt_Remove]);
		m_bn_Clear.SetWindowText(g_szCtrlText_T[m_nLanguage][Txt_Clear]);
		m_bn_Sort.SetWindowText(g_szCtrlText_T[m_nLanguage][Txt_Sort]);
	}
}

//=============================================================================
// Method		: Set_PermissionMode
// Access		: public  
// Returns		: void
// Parameter	: __in enPermissionMode IN_PermissionMode
// Qualifier	:
// Last Update	: 2022/1/7 - 11:12
// Desc.		:
//=============================================================================
void CWnd_Cfg_Socket::Set_PermissionMode(__in enPermissionMode IN_PermissionMode)
{
	switch (IN_PermissionMode)
	{
	case enPermissionMode::Permission_Operator:
	case enPermissionMode::Permission_Engineer:
	{
		m_ed_RFID_Prefix.EnableWindow(FALSE);
		m_rb_Single		.EnableWindow(FALSE);
		m_rb_Multi		.EnableWindow(FALSE);
		m_ed_RFID_Begin	.EnableWindow(FALSE);
		m_ed_RFID_End	.EnableWindow(FALSE);
		m_cb_SocketType	.EnableWindow(FALSE);
		m_ed_Comment	.EnableWindow(FALSE);
		m_lc_SocketList	.EnableWindow(FALSE);
		m_bn_Apply		.EnableWindow(FALSE);
		m_bn_Reload		.EnableWindow(FALSE);
		m_bn_Add		.EnableWindow(FALSE);
		m_bn_Remove		.EnableWindow(FALSE);
		m_bn_Clear		.EnableWindow(FALSE);
		m_bn_Sort		.EnableWindow(FALSE);
	}
	break;

	case enPermissionMode::Permission_Administrator:
	{
		m_ed_RFID_Prefix.EnableWindow(TRUE);
		m_rb_Single		.EnableWindow(TRUE);
		m_rb_Multi		.EnableWindow(TRUE);
		m_ed_RFID_Begin	.EnableWindow(TRUE);
		m_ed_RFID_End	.EnableWindow(TRUE);
		m_cb_SocketType	.EnableWindow(TRUE);
		m_ed_Comment	.EnableWindow(TRUE);
		m_lc_SocketList	.EnableWindow(TRUE);
		m_bn_Apply		.EnableWindow(TRUE);
		m_bn_Reload		.EnableWindow(TRUE);
		m_bn_Add		.EnableWindow(TRUE);
		m_bn_Remove		.EnableWindow(TRUE);
		m_bn_Clear		.EnableWindow(TRUE);
		m_bn_Sort		.EnableWindow(TRUE);
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
// Last Update	: 2022/1/5 - 19:56
// Desc.		:
//=============================================================================
void CWnd_Cfg_Socket::Set_FileFullPath(__in LPCTSTR IN_szFullPath)
{
	m_szPath_Default = IN_szFullPath;
}

//=============================================================================
// Method		: Get_Option
// Access		: public  
// Returns		: void
// Parameter	: __out CConfig_Model & OUT_stSocketList
// Qualifier	:
// Last Update	: 2021/6/14 - 16:34
// Desc.		:
//=============================================================================
void CWnd_Cfg_Socket::Get_Configuration(__out CSocketList& OUT_stSocketList)
{
	m_lc_SocketList.Get_SocketList(OUT_stSocketList);
}

//=============================================================================
// Method		: Set_Option
// Access		: public  
// Returns		: void
// Parameter	: __in const CConfig_Model * IN_pSocketList
// Qualifier	:
// Last Update	: 2021/6/14 - 16:34
// Desc.		:
//=============================================================================
void CWnd_Cfg_Socket::Set_Configuration(__in const CSocketList* IN_pSocketList)
{
	m_lc_SocketList.Set_SocketList(IN_pSocketList);
}

//=============================================================================
// Method		: Set_Configuration
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2022/1/5 - 19:59
// Desc.		:
//=============================================================================
void CWnd_Cfg_Socket::Set_Configuration()
{
	if (FALSE == m_szPath_Default.IsEmpty())
	{
		CXmlSocket		XmlConfig;
		CSocketList		stConfig_SocketList;

		if (XmlConfig.LoadXML_Socket_CfgList(m_szPath_Default.GetBuffer(0), stConfig_SocketList))
		{
			// UI에 세팅
			Set_Configuration(&stConfig_SocketList);
		}
	}
}
