//*****************************************************************************
// Filename	: 	Wnd_Cfg_Alarm.h
// Created	:	2021/10/21 - 15:15
// Modified	:	2021/10/21 - 15:15
//
// Author	:	piring
//	
// Purpose	:	
//*****************************************************************************
#ifndef Wnd_Cfg_Alarm_h__
#define Wnd_Cfg_Alarm_h__

#pragma once
#include "Wnd_BaseView.h"
#include "VGStatic.h"
#include "List_SocketRFID.h"

//-----------------------------------------------------------------------------
// CWnd_Cfg_Alarm
//-----------------------------------------------------------------------------
class CWnd_Cfg_Alarm : public CWnd_BaseView
{
	DECLARE_DYNAMIC(CWnd_Cfg_Alarm)

public:
	CWnd_Cfg_Alarm();
	virtual ~CWnd_Cfg_Alarm();

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg int		OnCreate				(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnSize					(UINT nType, int cx, int cy);
	virtual BOOL	PreCreateWindow			(CREATESTRUCT& cs);
	virtual BOOL	PreTranslateMessage		(MSG* pMsg);

	afx_msg void	OnBnClicked_Add			();
	afx_msg void	OnBnClicked_Remove		();
	afx_msg void	OnBnClicked_Apply		();


	CFont			m_font_Default;
	CFont			m_font_Data;

		// NG Code 리스트
	CList_SocketRFID	m_lc_SocketList;
	
	

	// Socket 추가, 제거 (리스트 자동 정렬)
	CMFCButton			m_bn_Add;
	CMFCButton			m_bn_Remove;	// 삭제 확인 팝업 창 사용
	CMFCButton			m_bn_Apply;


	// 버튼 제어 함수
	void	Item_Insert					();
	void	Item_Remove					();
	void	Item_Modify					();

	// file 전체 경로
	CString		m_szSavePath;
	BOOL	SaveXML_SocketList			();
	BOOL	LoadXML_SocketList			();


	// UI에 세팅 된 데이터 -> 구조체
	void		Get_UIData				(__out CConfig_Socket& OUT_stSocket);
	// 구조체 -> UI에 세팅
	void		Set_UIData				(__in const CConfig_Socket* IN_pSocket);

public:

	// 권한 설정
	void		Set_PermissionMode		(__in enPermissionMode IN_PermissionMode);

	// UI에 표시된 데이터의 구조체 반환	
	void		Get_Configuration		(__out CSocketList& OUT_stSocketList);

	// 모델 데이터를 UI에 표시
	void		Set_Configuration		(__in const CSocketList* IN_pSocketList);
	
	// 파일 저장/불러오기 용도의 파일 전체 경로
	void		Set_FilePath			(__in LPCTSTR szFilePath);
};

#endif // Wnd_Cfg_Alarm_h__


