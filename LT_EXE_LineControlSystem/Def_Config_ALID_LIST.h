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

	// ?§ÌÖù Í∞?àò
	size_t Get_AL_Count() const
	{
		return AL_LIST.size();
	};
	// Î™®Îì† ?§ÌÖù ??†ú
	virtual void RemoveAll()
	{
		AL_LIST.clear();
	};
	// ?§ÌÖù Ï∂îÍ?
	virtual void AL_Add(
		__in CConfig_AL IN_stTestStep)
	{
		AL_LIST.push_back(IN_stTestStep);
	};
	// ?§ÌÖù ?ΩÏûÖ
	virtual void AL_Insert(
		__in uint16_t IN_nIdx,
		__in CConfig_AL IN_stTestStep)
	{
		if (0 < AL_LIST.size())
		{
			AL_LIST.insert(AL_LIST.begin() + IN_nIdx, IN_stTestStep);
		}
	};
	// ?§ÌÖù ??†ú
	virtual void AL_Remove(
		__in uint16_t IN_nIdx)
	{
		if (0 < AL_LIST.size())
		{
			AL_LIST.erase(AL_LIST.begin() + IN_nIdx);
		}
	};
	// ?§ÌÖù ?òÏ†ï
	virtual void AL_Modify(
		__in uint16_t IN_nIdx,
		__in CConfig_AL IN_stTestStep)
	{
		if (0 < AL_LIST.size())
		{
			AL_LIST.at(IN_nIdx) = IN_stTestStep;
		}
	};
	// ?§ÌÖù ?ÑÎ°ú ?¥Îèô
	virtual void AL_Up(
		__in uint16_t IN_nIdx)
	{
		if (0 < AL_LIST.size())
		{
			// 0Î≤??∏Îç±?§Îäî ?ÑÎ°ú ?¥Îèô Î∂àÍ?
			if ((0 < IN_nIdx) && (1 < AL_LIST.size()))
			{
				CConfig_AL stStep = AL_LIST.at(IN_nIdx);

				AL_LIST.erase(AL_LIST.begin() + IN_nIdx);
				AL_LIST.insert(AL_LIST.begin() + IN_nIdx, stStep);
			}
		}
	};
	// ?§ÌÖù ?ÑÎûòÎ°??¥Îèô
	virtual void AL_Down(
		__in uint16_t IN_nIdx)
	{
		if (0 < AL_LIST.size())
		{
			// ÎßàÏ?Îß??∏Îç±?§Îäî ?ÑÎûòÎ°??¥Îèô Î∂àÍ?
			if ((IN_nIdx < (AL_LIST.size() - 1)) && (1 < AL_LIST.size()))
			{
				CConfig_AL stStep = AL_LIST.at(IN_nIdx);

				AL_LIST.erase(AL_LIST.begin() + IN_nIdx);

				// Î≥ÄÍ≤ΩÎêò???ÑÏπòÍ∞Ä ÏµúÌïò?®Ïù¥Î©? Insert ?Ä??Add ?¨Ïö©
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
