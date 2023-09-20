//*****************************************************************************
// Filename	: 	Def_InOutCount.h
// Created	:	2023/3/15 - 16:17
// Modified	:	2023/3/15 - 16:17
//
// Author	:	piring
//	
// Purpose	:	투입 / 배출 / 진행 / 제거   갯수 집계
//*****************************************************************************
#ifndef Def_InOutCount_h__
#define Def_InOutCount_h__


#include <stdint.h>

#pragma once

typedef enum
{
	SCnt_In,
	SCnt_Out,
	SCnt_Ongoing,
	SCnt_Remove,

	SCnt_MaxCount
}enSocketInOutCount;

typedef struct _tag_InOutCount
{
	uint32_t nIn;		// 투입
	uint32_t nOut;		// 배출
	uint32_t nOngoing;	// 진행 중
	uint32_t nRemove;	// 중간에 제거 됨

	_tag_InOutCount()
	{

	};

	void Reset()
	{
		nIn		= 0;
		nOut	= 0;
		nOngoing	= 0;
		nRemove	= 0;
	};

	_tag_InOutCount& operator= (const _tag_InOutCount& ref)
	{
		nIn			= ref.nIn;
		nOut		= ref.nOut;
		nOngoing	= ref.nOngoing;
		nRemove		= ref.nRemove;

		return *this;
	};

	void Compute_Ongoing()
	{
		nOngoing = nIn - nOut - nRemove;
	};

	// 빈 소켓은 제외 해야 한다.
	void Increase_In()
	{
		++nIn;

		Compute_Ongoing();
	};

	// 빈 소켓은 제외 해야 한다.
	void Increase_Out()
	{
		++nOut;

		Compute_Ongoing();
	};

	// 빈 소켓은 제외 해야 한다.
	void Increase_Remove()
	{
		++nRemove;
		
		Compute_Ongoing();
	}

}ST_InOutCount, *PST_InOutCount;


// csv 저장
// 레지스트리 저장
// 레지스트리 불러오기



#endif // Def_InOutCount_h__
