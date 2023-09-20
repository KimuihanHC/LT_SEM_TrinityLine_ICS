//*****************************************************************************
// Filename	: 	Wnd_Cfg_Socket.h
// Created	:	2021/10/21 - 15:15
// Modified	:	2021/10/21 - 15:15
//
// Author	:	piring
//	
// Purpose	:	
//*****************************************************************************
#ifndef Wnd_Cfg_Socket_h__
#define Wnd_Cfg_Socket_h__

#pragma once
#include "Wnd_BaseView.h"
#include "Def_Enum.h"
#include "VGStatic.h"
#include "Def_Socket_Config.h"
#include "List_SocketRFID.h"

//-----------------------------------------------------------------------------
// CWnd_Cfg_Socket
//-----------------------------------------------------------------------------
class CWnd_Cfg_Socket : public CWnd_BaseView
{
	DECLARE_DYNAMIC(CWnd_Cfg_Socket)

public:
	CWnd_Cfg_Socket();
	virtual ~CWnd_Cfg_Socket();

	enum enItem
	{
		Item_RFID,
		Item_RFID_Prefix,
		Item_RFID_Begin,
		Item_RFID_End,
		Item_SocketType,
		Item_Comment,

		Item_MaxCount,
	};

	enum enCtrlText
	{
		Txt_Apply,
		Txt_Reload,
		Txt_Single,
		Txt_Multi,
		Txt_Add,
		Txt_Remove,
		Txt_Clear,
		Txt_Sort,
		Txt_RFID_Begin_Single,

		Txt_MaxCount,
	};

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg int		OnCreate				(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnSize					(UINT nType, int cx, int cy);
	virtual BOOL	PreCreateWindow			(CREATESTRUCT& cs);
	virtual BOOL	PreTranslateMessage		(MSG* pMsg);

	afx_msg void	OnBnClicked_RFID		(UINT nID);
	afx_msg void	OnCbnSelendokSocketType	();
	afx_msg void	OnBnClicked_Add			();
	afx_msg void	OnBnClicked_Remove		();
	afx_msg void	OnBnClicked_Clear		();
	afx_msg void	OnBnClicked_Sort		();
	afx_msg void	OnBnClicked_Apply		();
	afx_msg void	OnBnClicked_Reload		();


	CFont			m_font_Default;
	CFont			m_font_Data;

	CVGStatic			m_st_Item[Item_MaxCount];

	// RFID Prefix (RFID 숫자 제외한 접두어)
	CMFCMaskedEdit		m_ed_RFID_Prefix;
	
	// 1개 추가, 다중 RFID 번호 추가 선택
	CMFCButton			m_rb_Single;	// 1개 소켓 추가
	CMFCButton			m_rb_Multi;		// 여러개 소켓 추가
		
	// 시작 번호 (0000 ~ )
	CMFCMaskedEdit		m_ed_RFID_Begin;
	// 끝 번호   (~ 9999)
	CMFCMaskedEdit		m_ed_RFID_End;

	// 소켓 타입
	CComboBox			m_cb_SocketType;
	// 설명
	CEdit				m_ed_Comment;

	// RFID 등록된 소켓 리스트
	CList_SocketRFID	m_lc_SocketList;
	
	// 현재 리스트의 소켓 정보 저장
	CMFCButton			m_bn_Apply;
	CMFCButton			m_bn_Reload;

	// Socket 추가, 제거 (리스트 자동 정렬)
	CMFCButton			m_bn_Add;
	CMFCButton			m_bn_Remove;	// 삭제 확인 팝업 창 사용
	CMFCButton			m_bn_Clear;		// 리스트 목록 모두 제거
	CMFCButton			m_bn_Sort;

	

	// 버튼 제어 함수
	void	Item_Insert					();
	void	Item_Remove					();
	void	Item_Modify					();

	void	Item_Clear					();
	void	Item_Sort					();

	// file 전체 경로
	CString		m_szPath_Default;
	BOOL	SaveXML_SocketList			();
	BOOL	LoadXML_SocketList			();


	// UI에 세팅 된 데이터 -> 구조체
	void		Get_UIData				(__out CConfig_Socket& OUT_stSocket);
	// 구조체 -> UI에 세팅
	void		Set_UIData				(__in const CConfig_Socket* IN_pSocket);

public:

	virtual void	OnLanguage			(__in uint8_t IN_nLanguage);

	// 권한 설정
	void		Set_PermissionMode		(__in enPermissionMode IN_PermissionMode);

	// 기본 저장 파일 전체 경로
	void		Set_FileFullPath		(__in LPCTSTR IN_szFullPath);

	// UI에 표시된 데이터의 구조체 반환	
	void		Get_Configuration		(__out CSocketList& OUT_stSocketList);

	// 모델 데이터를 UI에 표시
	void		Set_Configuration		(__in const CSocketList* IN_pSocketList);
	void		Set_Configuration		();
	
	
};

#endif // Wnd_Cfg_Socket_h__


