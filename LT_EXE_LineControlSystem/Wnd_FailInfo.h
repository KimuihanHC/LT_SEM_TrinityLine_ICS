//*****************************************************************************
// Filename	: Wnd_FailInfo.h
// Created	: 2016/03/11
// Modified	: 2016/03/11
//
// Author	: PiRing
//	
// Purpose	: 기본 화면용 윈도우
//*****************************************************************************
#ifndef Wnd_FailInfo_h__
#define Wnd_FailInfo_h__

#pragma once

#include <afxwin.h>
#include "Wnd_BaseView.h"
#include "VGStatic.h"
#include "Def_Constant.h"
#include "List_FailInfo.h"
#include "FailInfo.h"

//=============================================================================
// CWnd_FailInfo
//=============================================================================
class CWnd_FailInfo : public CWnd_BaseView
{
	DECLARE_DYNAMIC(CWnd_FailInfo)

public:
	CWnd_FailInfo();
	virtual ~CWnd_FailInfo();

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg int		OnCreate				(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnSize					(UINT nType, int cx, int cy);

	afx_msg void	OnBnClicked_RefreshList	();
	afx_msg void	OnBnClicked_ResetCount_A();
	afx_msg void	OnBnClicked_Save_CSV	();

	CFont			m_font_Default;	
	CFont			m_Font;

	// UI
	CVGStatic			m_st_Equipment[MAX_EQUIPMENT_COUNT];
	CList_FailInfo		m_lc_FailInfoList[MAX_EQUIPMENT_COUNT];

	// 불량 정보 카운트 초기화
	CMFCButton			m_bn_RefreshList;
	CMFCButton			m_bn_ResetCount_All;

	// CSV 저장
	CMFCButton			m_bn_Save_CSV;

	// 데이터
	enPermissionMode	m_InspMode			= enPermissionMode::Permission_Operator;
	CFailInfo*			m_pFailInfo			= nullptr;

	// 불량 리스트 UI 초기화
	void	Init_FailInfoUI_List			();

	void	Reset_Count_SelectedEqp			();
	void	Reset_Count_All					();
	void	Refresh_List					();


	BOOL	Save							(HBITMAP& hBitmap, CString strFilePath);
	int		GetEncoderClsid					(const WCHAR* format, CLSID* pClsid);

	void	ScreenCapture					();
	bool	WindowCapture					(__in HWND hTargetWnd);
	bool	WindowCapture					(__in HWND hTargetWnd, __in LPCTSTR lpszFilePath);
		

public:
	
	virtual void	OnLanguage				(__in uint8_t IN_nLanguage);

	void	SetPtr_FailInfo					(__in CFailInfo* IN_pstFailInfo)
	{
		m_pFailInfo = IN_pstFailInfo;
	};

	// 검사 모드 설정
	void	Set_PermissionMode				(__in enPermissionMode InspMode);

	// 불량 정보 윈도우의 핸들 구하기
	HWND	Get_FailInfoList_HWND			(__in uint8_t IN_nIndex);
	
	// 전체 불량 정보 갱신
	void	Update_FailInfo					();

};

#endif // Wnd_FailInfo_h__
