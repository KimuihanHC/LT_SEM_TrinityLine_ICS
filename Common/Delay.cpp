//*****************************************************************************
// Filename	: 	Delay.cpp
// Created	:	2020/12/23 - 11:42
// Modified	:	2020/12/23 - 11:42
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************

#include <AfxWin.h>
#include "Delay.h"
#include <Mmsystem.h>
#pragma comment (lib,"winmm.lib")

CDelay::CDelay()
{

}
CDelay::~CDelay()
{

}

//=============================================================================
// Method		: Delay_Seconds
// Access		: public static  
// Returns		: void
// Parameter	: float seconds
// Qualifier	:
// Last Update	: 2020/12/23 - 11:44
// Desc.		:
//=============================================================================
void CDelay::Delay_Seconds(float seconds)
{
	__int64 timeEllapsed;
	__int64 timeStart;
	__int64 timeDelta;

	QueryPerformanceFrequency((LARGE_INTEGER*)(&timeDelta));
	QueryPerformanceCounter((LARGE_INTEGER*)(&timeStart));

	__int64 timeToWait = (__int64)((double)timeDelta * (double)seconds);

	timeEllapsed = timeStart;

	while ((timeEllapsed - timeStart) < timeToWait)
	{
		QueryPerformanceCounter((LARGE_INTEGER*)(&timeEllapsed));

	};
}

//=============================================================================
// Method		: Delay_MiliSeconds
// Access		: public static  
// Returns		: void
// Parameter	: float miliseconds
// Qualifier	:
// Last Update	: 2020/12/23 - 11:45
// Desc.		:
//=============================================================================
void CDelay::Delay_MiliSeconds(float miliseconds)
{
	__int64 timeEllapsed;
	__int64 timeStart;
	__int64 timeDelta;

	QueryPerformanceFrequency((LARGE_INTEGER*)(&timeDelta));

	__int64 timeToWait = (__int64)((double)timeDelta * (double)miliseconds / 1000.0f);

	QueryPerformanceCounter((LARGE_INTEGER*)(&timeStart));

	timeEllapsed = timeStart;

	while ((timeEllapsed - timeStart) < timeToWait)
	{
		QueryPerformanceCounter((LARGE_INTEGER*)(&timeEllapsed));

	};
}

//=============================================================================
// Method		: Delay_MicroSeconds
// Access		: public static  
// Returns		: void
// Parameter	: float microseconds
// Qualifier	:
// Last Update	: 2020/12/23 - 11:45
// Desc.		:
//=============================================================================
void CDelay::Delay_MicroSeconds(float microseconds)
{
	__int64 timeEllapsed;
	__int64 timeStart;
	__int64 timeDelta;

	QueryPerformanceFrequency((LARGE_INTEGER*)(&timeDelta));

	__int64 timeToWait = __int64((double)timeDelta * (double)microseconds / 1000000.0f);

	QueryPerformanceCounter((LARGE_INTEGER*)(&timeStart));

	timeEllapsed = timeStart;

	while ((timeEllapsed - timeStart) < timeToWait)
	{
		QueryPerformanceCounter((LARGE_INTEGER*)(&timeEllapsed));

	};
}