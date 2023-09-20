//*****************************************************************************
// Filename	: 	NTTabViewBarButton.h
//
// Created	:	
// Modified	:	2010/04/02 - 13:18
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
#ifndef __NTTabViewBarButton_H__
#define __NTTabViewBarButton_H__

#pragma once

//=============================================================================
// CNTTabViewBarButton
//=============================================================================
class CNTTabViewBarButton : public CMFCToolBarButton
{
	DECLARE_SERIAL(CNTTabViewBarButton)

public:
	CNTTabViewBarButton();
	//CNTTabViewBarButton (LPCTSTR lpszLabel, UINT nID);
	CNTTabViewBarButton (LPCTSTR lpszLabel, UINT nID, int nWidth);

	virtual ~CNTTabViewBarButton();

	virtual void	OnDraw (	CDC* pDC, 
								const CRect& rect, 
								CMFCToolBarImages* pImages,
								BOOL bHorz = TRUE, 
								BOOL bCustomizeMode = FALSE,
								BOOL bHighlight = FALSE,
								BOOL bDrawBorder = TRUE,
								BOOL bGrayDisabledButtons = TRUE
							);

protected:	

	virtual BOOL	OnClick		(CWnd* pWnd, BOOL bDelay = TRUE);

	void			Initialize ();
	virtual BOOL	IsEditable () const
	{
		return FALSE;
	}

	virtual SIZE	OnCalculateSize (CDC* pDC, const CSize& sizeDefault, BOOL bHorz);

	int				m_nStdWidth = 280;	// 버튼 너비 기준 수치

	BOOL			m_bEnable = TRUE;

public:
	void	Set_StandardWidth	(__in int IN_nWidth);

	void	Set_Enable(__in BOOL IN_bEnable)
	{
		if (m_bEnable != IN_bEnable)
		{
			m_bEnable = IN_bEnable;
		}
	}

	BOOL	Get_Enable()
	{
		return m_bEnable;
	};

};

#endif // __NTTabViewBarButton_H__


