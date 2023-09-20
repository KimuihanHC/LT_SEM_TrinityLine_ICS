//*****************************************************************************
// Filename	: VGBase_Content.h
// Created	: 2013/3/13
// Modified	: 2013/3/13 - 16:13
//
// Author	: PiRing
//	
// Purpose	: 
//*****************************************************************************
#ifndef VGBase_Content_h__
#define VGBase_Content_h__

#pragma once
#include "Define_VGBase.h"
#include "VGBase.h"

//=============================================================================
// CVGBase_Content
//=============================================================================
class CVGBase_Content : public CVGBase
{
public:
	CVGBase_Content(void);
	virtual ~CVGBase_Content(void);

protected:	

	INT				m_nMaxContents;// 컨텐츠 최대 사용 개수
	BOOL			m_bContentsVertical;
	CArray <Content, Content>	m_arrContent;

	int				m_iContentSpacing;// 컨텐츠 사이 간격 Spacing
	int				m_iContentPadding;// 보더와 컨텐츠 사이 간격 Padding

	void		DrawContent					(Graphics& graphics, Gdiplus::Rect rectContent, INT iBorderTickness);

	// Content 관련
	BOOL		AddContent					(Content newContent);
	BOOL		ModifyContent				(int Index, Content newContent);
	void		ResetContent				();

public:

	virtual void	SetContentBatchType			(BOOL bBatchVertical);
	virtual void	SetContentText				(LPCTSTR szText, UINT nContentIndex = 0);

	virtual	void	SetContentBackColor			(Color clrBack, UINT nContentIndex = 0);	
	virtual void	SetContentRect				(RectF rectContent, UINT nContentIndex = 0);
};

#endif // VGBase_Content_h__
