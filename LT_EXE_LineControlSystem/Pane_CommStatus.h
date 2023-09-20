//*****************************************************************************
// Filename	: 	Pane_CommStatus.h
// Created	:	2014/7/5 - 10:23
// Modified	:	2016/09/21
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
#ifndef Pane_CommStatus_h__
#define Pane_CommStatus_h__

#pragma once

#include "VGStatic.h"
#include <stdint.h>

#define		TEST_BUTTON_CNT		4

//=============================================================================
// CPane_CommStatus
//=============================================================================
class CPane_CommStatus : public CMFCTasksPane
{
	DECLARE_DYNAMIC(CPane_CommStatus)

public:
	CPane_CommStatus();
	virtual ~CPane_CommStatus();

	enum enDeviceStaticIndex
	{
		DevSI_BaseIndex = 0,
		DevSI_PermissionMode = 0,
		DevSI_User,

		DevSI_Production,
		DevSI_Dummy_2,
		DevSI_Dummy_3,
		DevSI_Dummy_4,
		DevSI_Dummy_5,
		DevSI_Dummy_6,
		DevSI_Dummy_7,
		DevSI_Dummy_8,
		DevSI_MaxCount,
	};

	enum enDeviceButtonIndex
	{
		DEV_BN_LogIn,

		DEV_BN_Init,				// CC_Init,	
		DEV_BN_Reset,				// CC_Reset,	
		DEV_BN_Run,					// CC_Run,		
		DEV_BN_Stop,				// CC_Stop,	
		DEV_BN_Show,				// CC_Show,	
		DEV_BN_Hide,				// CC_Hide,	
		DEV_BN_Keyboard,
		DEV_BN_MaxCount,
	};

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg int		OnCreate			(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnSize				(UINT nType, int cx, int cy);
	afx_msg void	OnUpdateCmdUI_Test	(CCmdUI* pCmdUI);
	afx_msg void	OnBnClickedTest		(UINT nID);

	afx_msg void	OnUpdateCmdUI_Dev	(CCmdUI* pCmdUI);
	afx_msg void	OnBnClicked_Dev		(UINT nID);

	virtual void	ScrollChild			(HWND /*hwndTask*/, int /*nScrollValue*/);

	int				m_nWidth		= 100;
	CFont			m_Font;
	CFont			m_Font_S;

	bool			m_bShowButton	= false;

	uint8_t			m_nLanguage		= 1;

	CMFCButton		m_bn_Test[TEST_BUTTON_CNT];
	
	CVGStatic		m_st_Device[DevSI_MaxCount];
	CMFCButton		m_bn_Device[DEV_BN_MaxCount];

	void			Add_SystemInfo			();
	void			Add_PermissionMode		();	

	void			Add_Production			();
	void			Add_Command				();
	void			Add_TCPIP				();
	void			Add_Dummy				();

	void			Add_Utilities			();
	
	void			Add_TestCtrl			();

public:

	void			OnLanguage					(__in uint8_t IN_nLanguage);

	void			Set_Width					(__in int IN_nWidth);
	virtual CSize	CalcFixedLayout				(BOOL, BOOL);
	
	void			OnSet_SystemType			(__in enInsptrSysType nSysType);

	void			SetStatus_PermissionMode	(__in enPermissionMode InspMode);
	void			SetStatus_Login				(__in enPermissionMode IN_nAuthority, __in LPCTSTR IN_szUserID);

	void			SetStatus_Production		(__in bool bEnd);
};

#endif // Pane_CommStatus_h__
