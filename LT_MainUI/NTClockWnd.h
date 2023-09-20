//*****************************************************************************
// Filename	: NTClockWnd.h
// Created	: 2010/4/15
// Modified	: 2010/4/15 - 17:20
//
// Author	: PiRing
//	
// Purpose	: 시간/날짜 표시용 윈도우
//*****************************************************************************
#ifndef NTClockWnd_h__
#define NTClockWnd_h__

#pragma once

//=============================================================================
// CNTClockWnd
//=============================================================================
class CNTClockWnd : public CStatic//CWnd
{
	DECLARE_DYNAMIC(CNTClockWnd)

public:
	CNTClockWnd();
	virtual ~CNTClockWnd();

	void			Start ();

	void			SetFontSize (UINT nDateFontSize, UINT nTimeFontSize, UINT nSecondFontSize)
	{
		m_nDateFontSize		= nDateFontSize;
		m_nTimeFontSize		= nTimeFontSize;
		m_nSecondFontSize	= nSecondFontSize;
	};

	void			Set_Adjust_Date		(int nLeft, int nTop);
	void			Set_Adjust_Time		(int nLeft, int nTop);
	void			Set_Adjust_Second	(int nLeft, int nTop);

	enum
	{
		CT_DATE	= 0,
		CT_TIME,
		CT_SECOND,
	};

protected:

	afx_msg void	OnDestroy();
	afx_msg void	OnSize(UINT nType, int cx, int cy);
	afx_msg void	OnTimer(UINT_PTR nIDEvent);
	afx_msg void	OnPaint();
	afx_msg BOOL	OnEraseBkgnd(CDC* pDC);

	DECLARE_MESSAGE_MAP()

	void			Clean ();

	void			DrawClock		(HDC hDC);
	void			DrawClock		();
	void			DrawClockText	(Graphics* pGrp, CString strText, UINT nOffsetX, UINT nOffsetY, UINT nType);


	CString			m_strDate;
	CString			m_strTime;
	CString			m_strSecond;

	UINT			m_idTimer;

	UINT			m_nDateFontSize;
	UINT			m_nTimeFontSize;
	UINT			m_nSecondFontSize;

	CPoint			m_ptDate;
	CPoint			m_ptTime;
	CPoint			m_ptSecond;

	SYSTEMTIME		m_timePrev;

	CachedBitmap *	m_pCBit;	

};
#endif // NTClockWnd_h__


