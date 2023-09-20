//*****************************************************************************
// Filename	: 	Def_Yield_Cm.h
// Created	:	2021/12/3 - 18:06
// Modified	:	2021/12/3 - 18:06
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#ifndef Def_Yield_Cm_h__
#define Def_Yield_Cm_h__


#pragma once

#include <stdint.h>
#include <map>
#include <vector>
#include <windows.h>

//---------------------------------------------------------
// 수율 구조체
//---------------------------------------------------------
class CYield
{
public:
	uint32_t	dwTotal;	// 총계
	uint32_t	dwPass;		// 양품 수
	uint32_t	dwFail;		// 불량 수
	float		fYield;		// 수율

	uint32_t	dwNoTest;	// 미검사 수
	uint32_t	dwRework;	// 재검 수

	CYield()
	{
		Reset();
	};

	

	CYield& operator= (const CYield& ref)
	{
		dwTotal		= ref.dwTotal;
		dwPass		= ref.dwPass;
		dwFail		= ref.dwFail;
		fYield		= ref.fYield;
		dwNoTest	= ref.dwNoTest;
		dwRework	= ref.dwRework;

		return *this;
	};

	inline virtual void	Reset()
	{
		dwTotal		= 0;
		dwPass		= 0;
		dwFail		= 0;
		fYield		= 0.0f;
		dwNoTest	= 0;
		dwRework	= 0;
	};

	inline virtual void	ComputeYield()
	{
		dwTotal = dwPass + dwFail;

		if (0 == dwTotal)
			fYield = 0.0;
		else
			fYield = (float)dwPass / (float)dwTotal * 100.0f;
	};

	virtual void IncreasePass()
	{
		++dwPass;
		ComputeYield();
	};

	virtual void IncreaseFail()
	{
		++dwFail;
		ComputeYield();
	};

	virtual void IncreaseNoTest()
	{
		++dwNoTest;
	};

	virtual void IncreaseRework()
	{
		++dwRework;
	};

};



#endif // Def_Yield_Cm_h__
