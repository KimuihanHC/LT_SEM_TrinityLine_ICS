//*****************************************************************************
// Filename	: 	Delay.h
// Created	:	2020/12/23 - 11:41
// Modified	:	2020/12/23 - 11:41
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
#ifndef Delay_h__
#define Delay_h__

#pragma once
class CDelay
{
public:
	CDelay();
	~CDelay();
	static void Delay_Seconds		(float seconds);
	static void Delay_MiliSeconds	(float miliseconds);
	static void Delay_MicroSeconds	(float microseconds);
};

#endif // Delay_h__
