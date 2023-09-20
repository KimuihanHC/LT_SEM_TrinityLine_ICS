//*****************************************************************************
// Filename	: 	NTCaptionBarButton.h
//
// Created	:	2010/03/10
// Modified	:	2010/03/10 - 10:44
//
// Author	:	PiRing
//	
// Purpose	:	캡션바에 사용할 버튼
//*****************************************************************************
#ifndef __NTCaptionBarButton_H__
#define __NTCaptionBarButton_H__


#pragma once

//=============================================================================
// CNTCaptionBarButton
//=============================================================================
class CNTCaptionBarButton : public CMFCButton
{
	DECLARE_DYNAMIC(CNTCaptionBarButton)

public:
	CNTCaptionBarButton();
	virtual ~CNTCaptionBarButton();

	UINT			GetSysCommand		() const;
	void			SetSysCommand		(UINT uiSysCommand);

protected:
	virtual void	OnDraw				(CDC* pDC, const CRect& rect, UINT uiState);

protected:
	DECLARE_MESSAGE_MAP()
};
#endif // __NTCaptionBarButton_H__


