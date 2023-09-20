//*****************************************************************************
// Filename	: 	VGProgressBar_Variable.h
// Created	:	2016/3/10 - 18:23
// Modified	:	2016/3/10 - 18:23
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
#ifndef VGProgressBar_Variable_h__
#define VGProgressBar_Variable_h__

#pragma once
#include "afxwin.h"
#include "VGProgressBar_Variable_Base.h"

class CVGProgressBar_Variable : public CWnd, public CVGProgressBar_Variable_Base
{
public:
	CVGProgressBar_Variable();
	virtual ~CVGProgressBar_Variable();

protected:
	afx_msg void OnPaint();

	virtual afx_msg LRESULT		OnStep		(WPARAM wParam, LPARAM lParam);	// response to PBM_STEPIT
	virtual afx_msg LRESULT		OnSetRange	(WPARAM wParam, LPARAM lParam);	// response to PBM_SETRANGE32
	virtual afx_msg LRESULT		OnGetPos	(WPARAM wParam, LPARAM lParam);	// response to PBM_GETPOS
	virtual afx_msg LRESULT		OnSetPosF	(WPARAM wParam, LPARAM lParam);	// response to PBM_FLOAT_SETPOS - see message description for warning
	virtual afx_msg LRESULT		OnSetStepF	(WPARAM wParam, LPARAM lParam);	// response to PBM_FLOAT_SETSTEP - see message description for warning

	DECLARE_MESSAGE_MAP()
	

	BOOL	m_bShowPercentage;
	
	float	m_fStepAmount;

	virtual void	ReDrawPanel			();

public:

	float	GetStepAmount		() const;

	// increment the position by the step amount
	void	Step				();
	void	SetStepAmount		(__in float fStepAmount);
	void	SetPosition			(__in float fPos);
	float	GetPosition			() const;

	// set range (maximum and minimum)
	void	SetRange			(__in UINT nStartPos, __in UINT nEndPos);

	void	SetOffset			(__in FLOAT fOffsetPos);

	void	SetPercent			(__in FLOAT fPercent);
	void	SetPercent			(__in BYTE byPercent);
	void	SetText				(__in LPCTSTR szText);

	void	ResetProgressBar	();
};

#endif // VGProgressBar_Variable_h__

