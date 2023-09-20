//*****************************************************************************
// Filename	: 	Def_FailInfo.h
// Created	:	2021/12/6 - 13:34
// Modified	:	2021/12/6 - 13:34
//
// Author	:	piring
//	
// Purpose	:	
//*****************************************************************************
#ifndef Def_FailInfo_h__
#define Def_FailInfo_h__

#pragma once

#include <afxwin.h>
#include <stdint.h>

#include <Def_Yield.h>

// 설비
//	- 불량 정보
//		- 불량 코드 카운트 (전체)
//		- 불량 코드 카운트 (L)
//		- 불량 코드 카운트 (R)

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
typedef struct _tag_NGCode_Count
{
	int16_t		m_nNGCode		= 0;

	uint32_t	m_nCount_All	= 0;
	uint32_t	m_nCount_Para[Para_MaxCount] = { 0, };

	_tag_NGCode_Count& operator= (const _tag_NGCode_Count& ref)
	{
		m_nNGCode	 = ref.m_nNGCode;
		m_nCount_All = ref.m_nCount_All;

		memcpy(&m_nCount_Para, ref.m_nCount_Para, sizeof(uint32_t) * Para_MaxCount);

		return *this;
	};

	void Reset()
	{
		m_nCount_All = 0;

		memset(&m_nCount_Para, 0, sizeof(uint32_t) * Para_MaxCount);
	};

	void IncreaseCount(__in uint8_t IN_nPara)
	{
		if (IN_nPara < Para_MaxCount)
		{
			++m_nCount_All;

			++m_nCount_Para[IN_nPara];
		}
	};

}ST_NGCode_Count;


#endif // Def_FailInfo_h__
