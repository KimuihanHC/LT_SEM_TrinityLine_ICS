//*****************************************************************************
// Filename	: 	Def_InOutCount.h
// Created	:	2023/3/15 - 16:17
// Modified	:	2023/3/15 - 16:17
//
// Author	:	piring
//	
// Purpose	:	���� / ���� / ���� / ����   ���� ����
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
	uint32_t nIn;		// ����
	uint32_t nOut;		// ����
	uint32_t nOngoing;	// ���� ��
	uint32_t nRemove;	// �߰��� ���� ��

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

	// �� ������ ���� �ؾ� �Ѵ�.
	void Increase_In()
	{
		++nIn;

		Compute_Ongoing();
	};

	// �� ������ ���� �ؾ� �Ѵ�.
	void Increase_Out()
	{
		++nOut;

		Compute_Ongoing();
	};

	// �� ������ ���� �ؾ� �Ѵ�.
	void Increase_Remove()
	{
		++nRemove;
		
		Compute_Ongoing();
	}

}ST_InOutCount, *PST_InOutCount;


// csv ����
// ������Ʈ�� ����
// ������Ʈ�� �ҷ�����



#endif // Def_InOutCount_h__
