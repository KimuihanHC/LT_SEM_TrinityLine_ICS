//*****************************************************************************
// Filename	: 	Grid_Yield.h
// Created	:	2016/3/30 - 18:35
// Modified	:	2016/3/30 - 18:35
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
#ifndef Grid_Yield_h__
#define Grid_Yield_h__

#pragma once

#include "Grid_Base.h"
#include "Def_DataStruct.h"

//-----------------------------------------------------------------------------
// CGrid_Yield
//-----------------------------------------------------------------------------
class CGrid_Yield : public CGrid_Base
{
public:
	CGrid_Yield();
	virtual ~CGrid_Yield();

protected:
	virtual void	OnSetup();
	virtual int		OnHint(int col, long row, int section, CString *string);
	virtual void	OnGetCell(int col, long row, CUGCell *cell);
	virtual void	OnDrawFocusRect(CDC *dc, RECT *rect);	

	// 그리드 외형 및 내부 문자열을 채운다.
	virtual void	DrawGridOutline();

	// 셀 갯수 가변에 따른 다시 그리기 위한 함수
	virtual void	CalGridOutline();

	// 헤더를 초기화
	void			InitHeader();

	CFont		m_font_Header;
	CFont		m_font_Data;

public:
	
	void		SetYield			(__in const ST_Yield* pstYield);

	void		ShowResetButton		(__in BOOL bShow);

};
#endif // Grid_Yield_h__

