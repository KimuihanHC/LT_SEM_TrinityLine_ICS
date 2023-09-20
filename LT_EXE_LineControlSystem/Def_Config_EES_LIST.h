//*****************************************************************************
// Filename	: 	Def_Config_EES_LIST.h
// Created	:	
// Modified	:	2023.04.20 17:00
//
// Author	:	piring
//	
// Purpose	:	
//*****************************************************************************
#ifndef Def_Config_EES_LIST_h__
#define Def_Config_EES_LIST_h__


#pragma once

#include <afxwin.h>
#include <stdint.h>
#include <vector>

#include "Def_Config_EES.h"

class CConfig_EES_LIST
{
public:
	std::vector<CConfig_EES> EES_LIST;
	CConfig_EES_LIST()	{

	};
	CConfig_EES_LIST& operator= (const CConfig_EES_LIST& ref)	{
		EES_LIST.clear();
		EES_LIST = ref.EES_LIST;
		return *this;
	};
	size_t Get_EES_Count() const	{
		return EES_LIST.size();
	};
	virtual void RemoveAll()	{
		EES_LIST.clear();
	};
	virtual void EES_Add(
		__in CConfig_EES IN_stTestStep)	{
		EES_LIST.push_back(IN_stTestStep);
	};
	virtual void EES_Insert(
		__in uint16_t IN_nIdx, 
		__in CConfig_EES IN_stTestStep)	{
		if (0 < EES_LIST.size())	{
			EES_LIST.insert(EES_LIST.begin() + IN_nIdx, IN_stTestStep);
		}
	};
	virtual void EES_Remove(
		__in uint16_t IN_nIdx)	{
		if (0 < EES_LIST.size())		{
			EES_LIST.erase(EES_LIST.begin() + IN_nIdx);
		}
	};
	virtual void EES_Modify(
		__in uint16_t IN_nIdx, 
		__in CConfig_EES IN_stTestStep)	{
		if (0 < EES_LIST.size())		{
			EES_LIST.at(IN_nIdx) = IN_stTestStep;
		}
	};
	virtual void EES_Up(
		__in uint16_t IN_nIdx)	{
		if (0 < EES_LIST.size())		{
			if ((0 < IN_nIdx) && (1 < EES_LIST.size()))			{
				CConfig_EES stStep = EES_LIST.at(IN_nIdx);
				EES_LIST.erase(EES_LIST.begin() + IN_nIdx);
				EES_LIST.insert(EES_LIST.begin() + IN_nIdx, stStep);
			}
		}
	};
	virtual void EES_Down(
		__in uint16_t IN_nIdx)	{
		if (0 < EES_LIST.size())		{
			if ((IN_nIdx < (EES_LIST.size() - 1)) && (1 < EES_LIST.size()))			{
				CConfig_EES stStep = EES_LIST.at(IN_nIdx);
				EES_LIST.erase(EES_LIST.begin() + IN_nIdx);
				if ((IN_nIdx + 1) < (EES_LIST.size()))				{
					EES_LIST.insert(EES_LIST.begin() + IN_nIdx, stStep);
				}	else	{
					EES_LIST.push_back(stStep);
				}
			}
		}
	};
	virtual CConfig_EES& GetAt(
		__in uint16_t IN_nIdx)	
	{	return EES_LIST.at(IN_nIdx);	}
	virtual const CConfig_EES& GetAt(
		__in uint16_t IN_nIdx) const
	{		return EES_LIST.at(IN_nIdx);	}
}; 
#endif // Def_Config_EES_LIST_h__
