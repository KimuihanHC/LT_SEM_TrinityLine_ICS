#pragma once

#include "VGStatic.h"
#include "resource.h"
#include "Def_ErrorCode.h"

#ifndef ErrView_h__
#define ErrView_h__

class CDlg_ErrView : public CDialogEx
{
	DECLARE_DYNAMIC(CDlg_ErrView)

public:
	CDlg_ErrView(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlg_ErrView();

	//	타이머
	HANDLE			m_hTimer_InputCheck;
	HANDLE			m_hTimerQueue;

	static VOID CALLBACK TimerRoutine_InputCheck(__in PVOID lpParam, __in BOOLEAN TimerOrWaitFired);

	void				CreateTimerQueue_Mon();
	void				DeleteTimerQueue_Mon();
	void				CreateTimer_InputCheck();
	void				DeleteTimer_InputCheck();
	void				OnMonitor_InputCheck();

	CVGStatic			m_st_frame;
	CButton				m_bn_Cancel;
	CFont				m_font_Default;
	UINT				m_nErrNum;

	enum { IDD = IDD_DLG_ERR };

	void			ErrMessage(UINT nNum);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int		OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnSize(UINT nType, int cx, int cy);
	afx_msg void	OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void	OnBnClickedButtenOK();

	virtual void OnCancel();
};

#endif// ErrView_h__
