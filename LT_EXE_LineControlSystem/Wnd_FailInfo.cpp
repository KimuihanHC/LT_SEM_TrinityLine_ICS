//*****************************************************************************
// Filename	: 	Wnd_FailInfo.cpp
// Created	:	2021/12/22 - 10:39
// Modified	:	2021/12/22 - 10:39
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
// Wnd_FailInfo.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Wnd_FailInfo.h"
#include "Def_Language_Message.h"

enum enCtrlText
{
	Txt_RefreshList,
	Txt_ResetCount_All,
	Txt_Save_CSV,

	Txt_MaxCount,
};

static LPCTSTR g_szCtrlText_T[Lang_MaxCount][Txt_MaxCount] =
{
	// 한국어
	{
		_T("목록 다시 읽기"),					// Txt_RefreshList
		_T("전체 NG 카운트 초기화"),			// Txt_ResetCount_All
		_T("CSV로 저장"),					// Txt_Save_CSV
	},

	// 영어
	{
		_T("Refresh List"),					// Txt_RefreshList
		_T("Reset All NG Count"),			// Txt_ResetCount_All
		_T("Save CSV"),						// Txt_Save_CSV
	},

	// 베트남어
	{
		_T("Refresh List"),					// Txt_RefreshList
		_T("Reset All NG Count"),			// Txt_ResetCount_All
		_T("Save CSV"),						// Txt_Save_CSV
	},

	// 중국어
	{
		_T("刷新列表"),						// Txt_RefreshList
		_T("重置所有NG计数"),					// Txt_ResetCount_All
		_T("保存CSV文件"),					// Txt_Save_CSV
	},
};

#define		IDC_LC_NGCOUNT_LIST_FST	1000
#define		IDC_LC_NGCOUNT_LIST_LST	IDC_LC_NGCOUNT_LIST_FST + Max_EquipmentCount - 1

#define		IDC_BN_RESET_COUNT		1003
#define		IDC_BN_RESET_COUNT_A	1004
#define		IDC_BN_SAVE_CSV			1005

#define		IDC_CB_EQUIPMENT		1008

//=============================================================================
// CWnd_FailInfo
//=============================================================================
IMPLEMENT_DYNAMIC(CWnd_FailInfo, CWnd_BaseView)

CWnd_FailInfo::CWnd_FailInfo()
{
	VERIFY(m_font_Default.CreateFont(
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
		_T("Arial")));			// lpszFacename
	
	VERIFY(m_Font.CreateFont(
		28,						// nHeight
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

CWnd_FailInfo::~CWnd_FailInfo()
{
	TRACE(_T("<<< Start ~CWnd_FailInfo >>> \n"));
	
	m_font_Default.DeleteObject();
	m_Font.DeleteObject();
}

BEGIN_MESSAGE_MAP(CWnd_FailInfo, CWnd_BaseView)
	ON_WM_CREATE()
	ON_WM_SIZE	()
	ON_BN_CLICKED(IDC_BN_RESET_COUNT,	OnBnClicked_RefreshList)
	ON_BN_CLICKED(IDC_BN_RESET_COUNT_A, OnBnClicked_ResetCount_A)
	ON_BN_CLICKED(IDC_BN_SAVE_CSV,		OnBnClicked_Save_CSV)
END_MESSAGE_MAP()


//=============================================================================
// CWnd_FailInfo 메시지 처리기입니다.
//=============================================================================
//=============================================================================
// Method		: OnCreate
// Access		: protected  
// Returns		: int
// Parameter	: LPCREATESTRUCT lpCreateStruct
// Qualifier	:
// Last Update	: 2021/12/22 - 10:39
// Desc.		:
//=============================================================================
int CWnd_FailInfo::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	//SetBackgroundColor(RGB(0, 0, 0));

	DWORD dwStyle = WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	CRect rectDummy;
	rectDummy.SetRectEmpty();
	
	/*m_Font.CreateStockObject(DEFAULT_GUI_FONT);*/

	UINT nWindowsID = 1;

	CString szText;
	for (auto nIdx = 0; nIdx < MAX_EQUIPMENT_COUNT; ++nIdx)
	{
		szText.Format(_T("No %d"), nIdx);
		m_st_Equipment[nIdx].SetStaticStyle(CVGStatic::StaticStyle_Title_Alt);
		m_st_Equipment[nIdx].SetColorStyle(CVGStatic::ColorStyle_Black);
		m_st_Equipment[nIdx].SetFont_Gdip(L"Arial", 12.0F);
		m_st_Equipment[nIdx].Create(szText, dwStyle | SS_CENTER | SS_CENTERIMAGE, rectDummy, this, IDC_STATIC);
	}

	for (auto nIdx = 0; nIdx < MAX_EQUIPMENT_COUNT; ++nIdx)
	{
		m_lc_FailInfoList[nIdx].Create(dwStyle | WS_VSCROLL, rectDummy, this, IDC_LC_NGCOUNT_LIST_FST + nIdx);
	}

	m_bn_RefreshList.Create(g_szCtrlText_T[m_nLanguage][Txt_RefreshList], dwStyle | BS_PUSHBUTTON, rectDummy, this, IDC_BN_RESET_COUNT);
	m_bn_ResetCount_All.Create(g_szCtrlText_T[m_nLanguage][Txt_ResetCount_All], dwStyle | BS_PUSHBUTTON, rectDummy, this, IDC_BN_RESET_COUNT_A);

	m_bn_Save_CSV.Create(g_szCtrlText_T[m_nLanguage][Txt_Save_CSV], dwStyle | BS_PUSHBUTTON, rectDummy, this, IDC_BN_SAVE_CSV);

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
// Last Update	: 2021/12/22 - 10:39
// Desc.		:
//=============================================================================
void CWnd_FailInfo::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	if ((cx == 0) && (cy == 0))
		return;

	int iMargin			= 5;
 	int iSpacing		= 5;
 	int iCateSpacing	= 10;

	int iLeft			= iMargin;
	int iTop			= iMargin;
	int iWidth			= cx - iMargin - iMargin;
	int iHeight			= cy - iMargin - iMargin;	
	int iCtrlWidth		= 200;
	int iCtrlHeight		= 35;
	int iStHeight		= 30;
	int iListWidth		= 0;
	int iListHeight		= iHeight - iCtrlHeight - iSpacing - iStHeight;

	iLeft = cx - iMargin - iCtrlWidth;
	m_bn_ResetCount_All.MoveWindow(iLeft, iTop, iCtrlWidth, iCtrlHeight);
	iLeft -= (iCtrlWidth + iSpacing);
	m_bn_RefreshList.MoveWindow(iLeft, iTop, iCtrlWidth, iCtrlHeight);
	iLeft -= (iCtrlWidth + iSpacing);
	m_bn_Save_CSV.MoveWindow(iLeft, iTop, iCtrlWidth, iCtrlHeight);

	iLeft = iMargin;
	iTop += iCtrlHeight + iSpacing;
	int iTop_Sub = iTop + iStHeight;
	iHeight = iHeight - iCtrlHeight - iSpacing;
	//iWidth -= (40 + iSpacing);

	int nTesterCount = 0;
	if (nullptr != m_pFailInfo)
	{
		nTesterCount = static_cast<int>(m_pFailInfo->Get_EquipmentCount()); // 로더, 리터너 제외
		nTesterCount = (nTesterCount <= MAX_EQUIPMENT_COUNT) ? nTesterCount : MAX_EQUIPMENT_COUNT;

		if (0 < nTesterCount)
		{
			iListWidth = (iWidth - (iSpacing * (nTesterCount - 1))) / nTesterCount;

			int iRemain = iWidth - (iListWidth * nTesterCount) - (iSpacing * (nTesterCount - 1));
			int iAdjust = 0;
			if (0 < nTesterCount)
			{
				for (auto nIdx = 0; nIdx < nTesterCount; ++nIdx)
				{
					iAdjust = (0 < iRemain) ? 1 : 0;

					m_st_Equipment[nIdx].MoveWindow(iLeft, iTop, iListWidth + iAdjust, iStHeight);
					m_lc_FailInfoList[nIdx].MoveWindow(iLeft, iTop_Sub, iListWidth + iAdjust, iListHeight);
					iLeft += iListWidth + iAdjust + iSpacing;

					--iRemain;
				}
			}
		}
	}

	for (auto nIdx = nTesterCount; nIdx < MAX_EQUIPMENT_COUNT; ++nIdx)
	{
		m_st_Equipment[nIdx].MoveWindow(0, 0, 0, 0);
		m_lc_FailInfoList[nIdx].MoveWindow(0, 0, 0, 0);
	}
}

//=============================================================================
// Method		: OnBnClicked_ResetCount
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2022/2/19 - 18:03
// Desc.		:
//=============================================================================
void CWnd_FailInfo::OnBnClicked_RefreshList()
{
	Refresh_List();
}

//=============================================================================
// Method		: OnBnClicked_ResetCount_A
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2022/4/20 - 17:09
// Desc.		:
//=============================================================================
void CWnd_FailInfo::OnBnClicked_ResetCount_A()
{
	Reset_Count_All();
}

//=============================================================================
// Method		: OnBnClicked_Save_CSV
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2022/4/20 - 17:09
// Desc.		:
//=============================================================================
void CWnd_FailInfo::OnBnClicked_Save_CSV()
{
	if (m_pFailInfo)
	{
		// 저장 할 경로를 선택하세요.
		CString szInitPath = _T("C:\\");
		CFolderPickerDialog Picker(szInitPath, OFN_FILEMUSTEXIST, NULL, 0);
		if (Picker.DoModal() == IDOK)
		{
			CString szFolderPath = Picker.GetPathName();

			if (m_pFailInfo->Write_CSV_File(szFolderPath))
			{
				// 저장에 성공
			}
			else
			{
				// 저장에 실패
				//LT_MessageBox(_T("Failed to save .csv file."));
				LT_MessageBox(g_szMessageBox_T[MB_NGCount_Save_Failed_CSV][m_nLanguage]);
			}

			// 테스트 코드
// 			CString szImageFile;
// 			szImageFile.Format(_T("%s\\NGCount.bmp"), Picker.GetPathName());
// 			WindowCapture(GetSafeHwnd(), szImageFile);
		}
	}
	else
	{
		//LT_MessageBox(_T("FailInfo Pointer is null !!"));
		LT_MessageBox(g_szMessageBox_T[MB_NGCount_PointerNull][m_nLanguage]);
	}
}

//=============================================================================
// Method		: Init_FailInfoUI_List
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/12/22 - 10:39
// Desc.		:
//=============================================================================
void CWnd_FailInfo::Init_FailInfoUI_List()
{
	if (m_pFailInfo)
	{
		uint8_t nEqpIdx = 0;
		uint8_t nCount = 0;
		CString szText;

		for (auto Iter = m_pFailInfo->m_EqpList.begin(); Iter != m_pFailInfo->m_EqpList.end(); Iter++)
		{
			nEqpIdx = Iter->first;

			// 윈도우 핸들 설정
			m_pFailInfo->Get_Equipment(nEqpIdx).Set_GUI_HWND(Get_FailInfoList_HWND(nCount));

			// 리스트 컨트롤에 불량 정보 클래스 연결
			m_lc_FailInfoList[nCount].Set_NGCountList(&m_pFailInfo->Get_Equipment(nEqpIdx));

			// 리스트 컨트롤의 검사 para 개수 설정
			m_lc_FailInfoList[nCount].Set_ParaCount(m_pFailInfo->Get_Equipment(nEqpIdx).Get_TestPortCount());


			// 설비 순서, 설비 id를 UI에 표시
			szText.Format(_T("%d. %s"), m_pFailInfo->Get_Equipment(nEqpIdx).Get_EqpOrder(), m_pFailInfo->Get_Equipment(nEqpIdx).Get_EquipmentID());
			m_st_Equipment[nCount].SetText(szText);

			++nCount;
		}

		// UI 다시 그리기
		if (GetSafeHwnd())
		{
			CRect rc;
			GetClientRect(rc);
			SendMessage(WM_SIZE, (WPARAM)SIZE_RESTORED, MAKELPARAM(rc.Width(), rc.Height()));
		}
	}
}

//=============================================================================
// Method		: Reset_Count_SelectedSocket
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2022/2/19 - 21:13
// Desc.		:
//=============================================================================
void CWnd_FailInfo::Reset_Count_SelectedEqp()
{
	/*if (IDYES == LT_MessageBox(_T("Reset count of selected Equipment?"), MB_YESNO))
	{
		if (m_pFailInfo)
		{
			int iSelType = m_cb_Equipment.GetCurSel();
			if (0 <= iSelType)
			{
				m_pFailInfo->Reset_Count(iSelType);
			}
		}
	}*/
}

//=============================================================================
// Method		: Reset_Count_All
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2022/4/20 - 17:09
// Desc.		:
//=============================================================================
void CWnd_FailInfo::Reset_Count_All()
{
	//if (IDYES == LT_MessageBox(_T("Reset the Count of all Equipment?"), MB_YESNO))
	if (IDYES == LT_MessageBox(g_szMessageBox_T[MB_NGCount_ResetCount_All][m_nLanguage], MB_YESNO))
	{
		// 리셋 전에 자동으로 CSV 저장
		if (m_pFailInfo)
		{
			if (m_pFailInfo->Write_CSV_File_Default())
			{
				// 저장에 성공
			}
			else
			{
				// 저장에 실패
				//LT_MessageBox(_T("Failed to save .csv file."));
				LT_MessageBox(g_szMessageBox_T[MB_NGCount_Save_Failed_CSV][m_nLanguage]);
			}

			m_pFailInfo->Reset_Count_All();
		}
	}
}

//=============================================================================
// Method		: Refresh_List
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2022/4/20 - 17:08
// Desc.		:
//=============================================================================
void CWnd_FailInfo::Refresh_List()
{
	//if (IDYES == LT_MessageBox(_T("Refresh the Count of all Equipment?"), MB_YESNO))
	if (IDYES == LT_MessageBox(g_szMessageBox_T[MB_NGCount_RefreshCount][m_nLanguage], MB_YESNO))
	{
		if (m_pFailInfo)
		{
			for (auto nIdx = 0; nIdx < m_pFailInfo->Get_EquipmentCount(); ++nIdx)
			{
				m_lc_FailInfoList[nIdx].Refresh_NGCount();
			}
		}
	}
}

//=============================================================================
// Method		: Save
// Access		: protected  
// Returns		: BOOL
// Parameter	: HBITMAP & hBitmap
// Parameter	: CString strFilePath
// Qualifier	:
// Last Update	: 2022/8/10 - 15:13
// Desc.		:
//=============================================================================
BOOL CWnd_FailInfo::Save(HBITMAP& hBitmap, CString strFilePath)
{
	if (!hBitmap)
		return FALSE;

	Gdiplus::Bitmap gBmp(hBitmap, NULL);
	CLSID clsid;
	GetEncoderClsid(_T("image/bmp"), &clsid);//HBITMAP을 bmp파일로 저장
	if (gBmp.Save(strFilePath, &clsid) == Gdiplus::Ok)
		return TRUE;//성공
	else
		return FALSE;//실패

	return TRUE;
}

int CWnd_FailInfo::GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT  num = 0;          // 이미지 인코더의 개수
	UINT  size = 0;         // 이미지 인코더 배열의 바이트 크기

	ImageCodecInfo* pImageCodecInfo = NULL;
	GetImageEncodersSize(&num, &size);

	if (size == 0)
		return -1;  //실패

	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));

	if (pImageCodecInfo == NULL)
		return -1;  //실패

	GetImageEncoders(num, size, pImageCodecInfo);//이미지 인코더 정보를 구함

	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)//사용자가 요구한 그래픽 형태와 일치하면
		{
			*pClsid = pImageCodecInfo[j].Clsid;//코덱아이디값 설정
			free(pImageCodecInfo);
			return j;  //성공
		}
	}

	free(pImageCodecInfo);//해제
	return -1;  //실패
}

//=============================================================================
// Method		: ScreenCapture
// Access		: protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2022/8/10 - 15:04
// Desc.		:
//=============================================================================
void CWnd_FailInfo::ScreenCapture()
{
	/*
	int nx = 0, ny = 0;
	CImage capImage;
	CWnd *pDesktopWnd = GetDesktopWindow();
	HDC hDC = NULL;
	if (!pDesktopWnd)
		return;

	CWindowDC DeskTopDC(pDesktopWnd);
	nx = GetSystemMetrics(SM_CXSCREEN);
	ny = GetSystemMetrics(SM_CYSCREEN);
	if (!capImage.Create(nx, ny, 32))
		return;

	hDC = capImage.GetDC();
	BitBlt(hDC, 0, 0, nx, ny, DeskTopDC.m_hDC, 0, 0, SRCCOPY);
	capImage.Save(_T("test.jpg"), Gdiplus::ImageFormatJPEG);
	capImage.ReleaseDC();
	*/
}

//=============================================================================
// Method		: WindowCapture
// Access		: protected  
// Returns		: bool
// Parameter	: __in HWND hTargetWnd
// Qualifier	:
// Last Update	: 2022/8/10 - 15:06
// Desc.		:
//=============================================================================
bool CWnd_FailInfo::WindowCapture(__in HWND hTargetWnd)
{
	CRect rct;
	if (hTargetWnd)
		::GetClientRect(hTargetWnd, &rct);
	else
		return false;

	CRect rcClient;
	GetClientRect(&rcClient);

	HBITMAP hBitmap = NULL;
	HBITMAP hOldBitmap = NULL;
	bool bSuccess = false;

	HDC hDC = ::GetDC(hTargetWnd);
	HDC hMemDC = ::CreateCompatibleDC(hDC);
	hBitmap = ::CreateCompatibleBitmap(hDC, rct.Width(), rct.Height());

	if (!hBitmap)
		return false;

	hOldBitmap = (HBITMAP)::SelectObject(hMemDC, hBitmap);

	// 캡쳐한 윈도우 현재 클라이언트 화면 크기에 맞춰 그리기
	CClientDC dc(this);
	dc.SetStretchBltMode(COLORONCOLOR);
	dc.StretchBlt(0, 0, rcClient.Width(), rcClient.Height(), CDC::FromHandle(hDC),
		0, 0, rct.Width(), rct.Height(), SRCCOPY);

	::SelectObject(hMemDC, hOldBitmap);
	DeleteObject(hBitmap);
	::DeleteDC(hMemDC);
	::ReleaseDC(hTargetWnd, hDC);

	return bSuccess;
}

bool CWnd_FailInfo::WindowCapture(__in HWND hTargetWnd, __in LPCTSTR lpszFilePath)
{
	CRect rct;
	if (hTargetWnd)
		::GetWindowRect(hTargetWnd, &rct);
	else
		return FALSE;

	HBITMAP hBitmap = NULL;
	HBITMAP hOldBitmap = NULL;
	BOOL bSuccess = FALSE;
	HDC hDC = ::GetDC(hTargetWnd);
	HDC hMemDC = ::CreateCompatibleDC(hDC);
	hBitmap = ::CreateCompatibleBitmap(hDC, rct.Width(), rct.Height());
	if (!hBitmap)
		return FALSE;

	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
	if (!::PrintWindow(hTargetWnd, hMemDC, 0))
		bSuccess = FALSE;
	else
		bSuccess = TRUE;
	

	if(bSuccess && !Save(hBitmap, lpszFilePath))
		bSuccess = FALSE;
	else
		bSuccess = TRUE;

	SelectObject(hMemDC, hOldBitmap);
	DeleteObject(hBitmap);
	::DeleteDC(hMemDC);
	::ReleaseDC(m_hWnd, hDC);

	return bSuccess;

}

//=============================================================================
// Method		: OnLanguage
// Access		: virtual public  
// Returns		: void
// Parameter	: __in uint8_t IN_nLanguage
// Qualifier	:
// Last Update	: 2022/7/20 - 14:35
// Desc.		:
//=============================================================================
void CWnd_FailInfo::OnLanguage(__in uint8_t IN_nLanguage)
{
	__super::OnLanguage(IN_nLanguage);

	// CharSet 변경
// 	LOGFONT lf_def;
// 	m_font_Default.GetLogFont(&lf_def);
// 	lf_def.lfCharSet = Get_CharSet(m_nLanguage);
//	m_font_Default.DeleteObject();
// 	m_font_Default.CreateFontIndirect(&lf_def);
// 
// 	// CharSet 변경
// 	LOGFONT lf;
// 	m_Font.GetLogFont(&lf);
// 	lf.lfCharSet = Get_CharSet(m_nLanguage);
//	m_Font.DeleteObject();
// 	m_Font.CreateFontIndirect(&lf);

	for (auto nIdx = 0; nIdx < MAX_EQUIPMENT_COUNT; ++nIdx)
	{
		m_lc_FailInfoList[nIdx].OnLanguage(IN_nLanguage);
	}

	if (GetSafeHwnd())
	{		
		m_bn_RefreshList.SetWindowText(g_szCtrlText_T[m_nLanguage][Txt_RefreshList]);
		m_bn_ResetCount_All.SetWindowText(g_szCtrlText_T[m_nLanguage][Txt_ResetCount_All]);
		m_bn_Save_CSV.SetWindowText(g_szCtrlText_T[m_nLanguage][Txt_Save_CSV]);
	}
}

//=============================================================================
// Method		: Set_PermissionMode
// Access		: public  
// Returns		: void
// Parameter	: enPermissionMode InspMode
// Qualifier	:
// Last Update	: 2021/12/22 - 10:39
// Desc.		:
//=============================================================================
void CWnd_FailInfo::Set_PermissionMode(enPermissionMode InspMode)
{
	switch (InspMode)
	{
	case Permission_Operator:
		m_InspMode = Permission_Operator;

		m_bn_RefreshList.EnableWindow(FALSE);
		m_bn_ResetCount_All.EnableWindow(FALSE);
		m_bn_Save_CSV.EnableWindow(FALSE);
		break;

	case Permission_Engineer:
		m_bn_RefreshList.EnableWindow(TRUE);
		m_bn_ResetCount_All.EnableWindow(TRUE);
		m_bn_Save_CSV.EnableWindow(TRUE);
		break;

	case Permission_Administrator:
		m_InspMode = Permission_Administrator;
		m_bn_RefreshList.EnableWindow(TRUE);
		m_bn_ResetCount_All.EnableWindow(TRUE);
		m_bn_Save_CSV.EnableWindow(TRUE);
		break;

	default:
		break;
	}
}

//=============================================================================
// Method		: Get_FailInfoList_HWND
// Access		: public  
// Returns		: HWND
// Parameter	: uint8_t IN_nIndex
// Qualifier	:
// Last Update	: 2022/4/20 - 17:09
// Desc.		:
//=============================================================================
HWND CWnd_FailInfo::Get_FailInfoList_HWND(uint8_t IN_nIndex)
{
	if (IN_nIndex < MAX_EQUIPMENT_COUNT)
	{
		return m_lc_FailInfoList[IN_nIndex].GetSafeHwnd();
	}
	else
	{
		return NULL;
	}
}

//=============================================================================
// Method		: Update_FailInfo
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/12/22 - 10:39
// Desc.		:
//=============================================================================
void CWnd_FailInfo::Update_FailInfo()
{
 	Init_FailInfoUI_List();
}
