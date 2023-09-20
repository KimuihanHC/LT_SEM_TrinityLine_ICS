//*****************************************************************************
// Filename	: 	Def_ProductInfo.h
// Created	:	2022/1/10 - 15:58
// Modified	:	2022/1/10 - 15:58
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#ifndef Def_ProductInfo_h__
#define Def_ProductInfo_h__

#pragma once

#pragma pack(push, 1)

#include <stdint.h>

typedef struct _tag_ReworkProduct
{
	//CString		szBarcode;		// 제품 바코드 S/N
	uint8_t		nNG_EqpType;	// Fail 처리된 검사 유형
	int16_t		nNG_Code;		// Fail 사유 ?
	SYSTEMTIME	InputTime;		// 제품 투입된 시간

	//uint8_t		m_nJudgement[Max_TesterCount];		// 제품 검사별 판정

	_tag_ReworkProduct()
	{
		Reset();
	};

	void Reset()
	{
		//szBarcode.Empty();
		nNG_EqpType = 0;
		nNG_Code	= 0;

		ZeroMemory(&InputTime, sizeof(SYSTEMTIME));
	};

	_tag_ReworkProduct& operator= (const _tag_ReworkProduct& ref)
	{
		//szBarcode		= ref.szBarcode;
		nNG_EqpType		= ref.nNG_EqpType;
		nNG_Code		= ref.nNG_Code;
		memcpy(&InputTime, &ref.InputTime, sizeof(SYSTEMTIME));

		return *this;
	};

}ST_ReworkProduct, *PST_ReworkProduct;

#pragma pack (pop)

#endif // Def_ProductInfo_h__
