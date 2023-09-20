//*****************************************************************************
// Filename	: 	ProgressWnd.h
// Created	:	2015/8/18 - 12:10
// Modified	:	2015/8/18 - 12:10
//
// Author	:	Chris Maunder (Code Project)
//	
// Purpose	:	http://www.codeproject.com/Articles/73/A-Popup-Progress-Window
//*****************************************************************************
#ifndef ProgressWnd_h__
#define ProgressWnd_h__

#pragma once
#include "afxwin.h"

//=============================================================================
//
//=============================================================================
class CProgressWnd : public CWnd
{
public:
	CProgressWnd();
	CProgressWnd(CWnd* pParent, LPCTSTR pszTitle, BOOL bSmooth = FALSE);
	virtual ~CProgressWnd();

	virtual BOOL DestroyWindow();

	BOOL		Create(CWnd* pParent, LPCTSTR pszTitle, BOOL bSmooth = FALSE);
	BOOL		GoModal(LPCTSTR pszTitle = _T("Progress"), BOOL bSmooth = FALSE);


protected:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnCancel();

	DECLARE_MESSAGE_MAP()

	void		CommonConstruct();
	void		GetPreviousSettings();
	void		SaveCurrentSettings();

	CWnd *			m_wRenenableWnd;

	BOOL			m_bCancelled;
	BOOL			m_bModal;
	BOOL			m_bPersistantPosition;
	int				m_nPrevPos;
	int				m_nPrevPercent;
	int				m_nStep;
	int				m_nMaxValue;
	int				m_nMinValue;
	int				m_nNumTextLines;

	CStatic			m_Text;
	CProgressCtrl	m_wndProgress;
	CButton			m_CancelButton;
	CString			m_strTitle;
	CString			m_strCancelLabel;
	CFont			m_font;

public:
	void		SetRange(int nLower, int nUpper, int nStep = 1);
	
	// Set range and step size
	int			OffsetPos(int nPos);                        // Same as CProgressCtrl
	int			StepIt();                                   //    "
	int			SetStep(int nStep);                         //    "
	int			SetPos(int nPos);                           //    "

	void		SetText(LPCTSTR fmt, ...);                 // Set text in text area

	void		Clear();                                   // Clear text, reset bar
	void		Hide();                                    // Hide window
	void		Show();                                    // Show window

	BOOL		Cancelled() { return m_bCancelled; }       // Was "Cancel" hit?

	void		SetWindowSize(int nNumTextLines, int nWindowWidth = 390);

	void		PeekAndPump(BOOL bCancelOnESCkey = TRUE);  // Message pumping for modal operation   

#if defined(UNICODE)
	void		SetMarquee(BOOL fMarqueeMode, int nInterval)
	{
		m_wndProgress.SetMarquee(fMarqueeMode, nInterval);
	};
#endif

};

#endif // ProgressWnd_h__



// 사용법
/*
CProgressWnd wndProgress(this, "Progress", TRUE);

wndProgress.SetRange(0, TEST_RANGE);
wndProgress.SetText("This is a progress window...\n\n""Try dragging it around or hitting Cancel.");

for (int i = 0; i < TEST_RANGE; i++) 
{
	wndProgress.StepIt();
	wndProgress.PeekAndPump();

	if (wndProgress.Cancelled()) 
	{
		MessageBox("Progress Cancelled");
		break;
	}
}
*/