//*****************************************************************************
// Filename	: 	Def_Config_Loss_LIST.h
// Created	:	
// Modified	:	2023.04.21 16:00
//
// Author	:	piring
//	
// Purpose	:	
//*****************************************************************************
#ifndef Def_Config_Loss_LIST_h__
#define Def_Config_Loss_LIST_h__


#pragma once

#include <afxwin.h>
#include <stdint.h>
#include <vector>

#include "Def_Config_Loss.h"

//-----------------------------------------------------------------------------
// EES κ΅¬μ± ?€μ ??κ΅¬μ‘°μ²?//-----------------------------------------------------------------------------
class CConfig_Loss_LIST
{
public:
	std::vector<CConfig_Loss> Loss_LIST;
	CConfig_Loss_LIST()	{

	};

	CConfig_Loss_LIST& operator= (const CConfig_Loss_LIST& ref)
	{
		Loss_LIST.clear();
		Loss_LIST = ref.Loss_LIST;
		return *this;
	};

	// ?€ν κ°?
	size_t Get_Loss_Count() const
	{
		return Loss_LIST.size();
	};
	// λͺ¨λ  ?€ν ?? 
	virtual void RemoveAll()
	{
		Loss_LIST.clear();
	};
	// ?€ν μΆκ?
	virtual void Loss_Add(
		__in CConfig_Loss IN_stTestStep)
	{
		Loss_LIST.push_back(IN_stTestStep);
	};
	// ?€ν ?½μ
	virtual void Loss_Insert(
		__in uint16_t IN_nIdx, 
		__in CConfig_Loss IN_stTestStep)
	{
		if (0 < Loss_LIST.size())
		{
			Loss_LIST.insert(Loss_LIST.begin() + IN_nIdx, IN_stTestStep);
		}
	};
	// ?€ν ?? 
	virtual void Loss_Remove(
		__in uint16_t IN_nIdx)
	{
		if (0 < Loss_LIST.size())
		{
			Loss_LIST.erase(Loss_LIST.begin() + IN_nIdx);
		}
	};
	// ?€ν ?μ 
	virtual void Loss_Modify(
		__in uint16_t IN_nIdx, 
		__in CConfig_Loss IN_stTestStep)
	{
		if (0 < Loss_LIST.size())
		{
			Loss_LIST.at(IN_nIdx) = IN_stTestStep;
		}
	};
	// ?€ν ?λ‘ ?΄λ
	virtual void Loss_Up(
		__in uint16_t IN_nIdx)
	{
		if (0 < Loss_LIST.size())
		{
			// 0λ²??Έλ±?€λ ?λ‘ ?΄λ λΆκ?
			if ((0 < IN_nIdx) && (1 < Loss_LIST.size()))
			{
				CConfig_Loss stStep = Loss_LIST.at(IN_nIdx);

				Loss_LIST.erase(Loss_LIST.begin() + IN_nIdx);
				Loss_LIST.insert(Loss_LIST.begin() + IN_nIdx, stStep);
			}
		}
	};
	// ?€ν ?λλ‘??΄λ
	virtual void Loss_Down(
		__in uint16_t IN_nIdx)
	{
		if (0 < Loss_LIST.size())
		{
			// λ§μ?λ§??Έλ±?€λ ?λλ‘??΄λ λΆκ?
			if ((IN_nIdx < (Loss_LIST.size() - 1)) && (1 < Loss_LIST.size()))
			{
				CConfig_Loss stStep = Loss_LIST.at(IN_nIdx);

				Loss_LIST.erase(Loss_LIST.begin() + IN_nIdx);

				// λ³κ²½λ???μΉκ° μ΅ν?¨μ΄λ©? Insert ???Add ?¬μ©
				if ((IN_nIdx + 1) < (Loss_LIST.size()))
				{
					Loss_LIST.insert(Loss_LIST.begin() + IN_nIdx, stStep);
				}
				else
				{
					Loss_LIST.push_back(stStep);
				}
			}
		}
	};
	virtual CConfig_Loss& GetAt(
		__in uint16_t IN_nIdx)
	{
		return Loss_LIST.at(IN_nIdx);
	}
	virtual const CConfig_Loss& GetAt(
		__in uint16_t IN_nIdx) const
	{
		return Loss_LIST.at(IN_nIdx);
	}
}; 
#endif // Def_Config_Loss_LIST_h__
