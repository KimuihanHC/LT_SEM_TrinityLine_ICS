//*****************************************************************************
// Filename	: 	Def_Config_ALID_LIST.h
// Created	:	
// Modified	:	2023.04.20 17:00
//
// Author	:	piring
//	
// Purpose	:	
//*****************************************************************************
#ifndef Def_Config_ALID_LIST_h__
#define Def_Config_ALID_LIST_h__


#pragma once

#include <afxwin.h>
#include <stdint.h>
#include <vector>

#include "Def_Config_ALID.h"

class CConfig_AL_LIST
{
public:
	std::vector<CConfig_AL> AL_LIST;
	CConfig_AL_LIST()
	{

	};

	CConfig_AL_LIST& operator= (const CConfig_AL_LIST& ref)
	{
		AL_LIST.clear();
		AL_LIST = ref.AL_LIST;
		return *this;
	};

	// ?€ν κ°?
	size_t Get_AL_Count() const
	{
		return AL_LIST.size();
	};
	// λͺ¨λ  ?€ν ?? 
	virtual void RemoveAll()
	{
		AL_LIST.clear();
	};
	// ?€ν μΆκ?
	virtual void AL_Add(
		__in CConfig_AL IN_stTestStep)
	{
		AL_LIST.push_back(IN_stTestStep);
	};
	// ?€ν ?½μ
	virtual void AL_Insert(
		__in uint16_t IN_nIdx,
		__in CConfig_AL IN_stTestStep)
	{
		if (0 < AL_LIST.size())
		{
			AL_LIST.insert(AL_LIST.begin() + IN_nIdx, IN_stTestStep);
		}
	};
	// ?€ν ?? 
	virtual void AL_Remove(
		__in uint16_t IN_nIdx)
	{
		if (0 < AL_LIST.size())
		{
			AL_LIST.erase(AL_LIST.begin() + IN_nIdx);
		}
	};
	// ?€ν ?μ 
	virtual void AL_Modify(
		__in uint16_t IN_nIdx,
		__in CConfig_AL IN_stTestStep)
	{
		if (0 < AL_LIST.size())
		{
			AL_LIST.at(IN_nIdx) = IN_stTestStep;
		}
	};
	// ?€ν ?λ‘ ?΄λ
	virtual void AL_Up(
		__in uint16_t IN_nIdx)
	{
		if (0 < AL_LIST.size())
		{
			// 0λ²??Έλ±?€λ ?λ‘ ?΄λ λΆκ?
			if ((0 < IN_nIdx) && (1 < AL_LIST.size()))
			{
				CConfig_AL stStep = AL_LIST.at(IN_nIdx);

				AL_LIST.erase(AL_LIST.begin() + IN_nIdx);
				AL_LIST.insert(AL_LIST.begin() + IN_nIdx, stStep);
			}
		}
	};
	// ?€ν ?λλ‘??΄λ
	virtual void AL_Down(
		__in uint16_t IN_nIdx)
	{
		if (0 < AL_LIST.size())
		{
			// λ§μ?λ§??Έλ±?€λ ?λλ‘??΄λ λΆκ?
			if ((IN_nIdx < (AL_LIST.size() - 1)) && (1 < AL_LIST.size()))
			{
				CConfig_AL stStep = AL_LIST.at(IN_nIdx);

				AL_LIST.erase(AL_LIST.begin() + IN_nIdx);

				// λ³κ²½λ???μΉκ° μ΅ν?¨μ΄λ©? Insert ???Add ?¬μ©
				if ((IN_nIdx + 1) < (AL_LIST.size()))
				{
					AL_LIST.insert(AL_LIST.begin() + IN_nIdx, stStep);
				}
				else
				{
					AL_LIST.push_back(stStep);
				}
			}
		}
	};
	virtual CConfig_AL& GetAt(
		__in uint16_t IN_nIdx)
	{
		return AL_LIST.at(IN_nIdx);
	}
	virtual const CConfig_AL& GetAt(
		__in uint16_t IN_nIdx) const
	{
		return AL_LIST.at(IN_nIdx);
	}

}; // CConfig_AL


#endif // Def_Config_ALID_LIST_h__
