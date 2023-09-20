//*****************************************************************************
// Filename	: 	Def_ConfigLine.h
// Created	:	2021/11/1 - 15:23
// Modified	:	2021/11/1 - 15:23
//
// Author	:	piring
//	
// Purpose	:	
//*****************************************************************************
#ifndef Def_ConfigLineServer_h__
#define Def_ConfigLineServer_h__


#pragma once

#include <afxwin.h>
#include <stdint.h>
#include <vector>

#include "Def_ConfigServer.h"

//-----------------------------------------------------------------------------
// ?�인??검?�기 구성 ?�정??구조�?//-----------------------------------------------------------------------------
class CConfig_Server
{
public:
	std::vector<CConfig_Svr> SvrList;
	CConfig_Server()	{
	};
	CConfig_Server& operator= (const CConfig_Server& ref)	{
		SvrList.clear();
		SvrList = ref.SvrList;
		return *this;
	};
	size_t GetCount() const	{
		return SvrList.size();	};
	virtual void RemoveAll()	{
		SvrList.clear();	};
	virtual void Svr_Add(__in CConfig_Svr IN_stTestStep)	{
		SvrList.push_back(IN_stTestStep);	};
	virtual void Svr_Insert(__in uint16_t IN_nIdx, __in CConfig_Svr IN_stTestStep)	{
		if (0 < SvrList.size())		{
			SvrList.insert(SvrList.begin() + IN_nIdx, IN_stTestStep);		}	};
	virtual void Svr_Remove(__in uint16_t IN_nIdx)	{
		if (0 < SvrList.size())		{
			SvrList.erase(SvrList.begin() + IN_nIdx);		}	};
	virtual void Svr_Modify(__in uint16_t IN_nIdx, __in CConfig_Svr IN_stTestStep)	{		
		if (0 < SvrList.size())		{
			SvrList.at(IN_nIdx) = IN_stTestStep;		}	};
	virtual void Svr_Up(__in uint16_t IN_nIdx)	{
		if (0 < SvrList.size())		{
			if ((0 < IN_nIdx) && (1 < SvrList.size()))			{
				CConfig_Svr stStep = SvrList.at(IN_nIdx);
				SvrList.erase(SvrList.begin() + IN_nIdx);
				SvrList.insert(SvrList.begin() + IN_nIdx, stStep);
			}
		}
	};
	virtual void Svr_Down(__in uint16_t IN_nIdx)	{
		if (0 < SvrList.size())		{
			if ((IN_nIdx < (SvrList.size() - 1)) && (1 < SvrList.size()))	{
				CConfig_Svr stStep = SvrList.at(IN_nIdx);
				SvrList.erase(SvrList.begin() + IN_nIdx);
				if ((IN_nIdx + 1) < (SvrList.size()))	{
					SvrList.insert(SvrList.begin() + IN_nIdx, stStep);
				}else	{
					SvrList.push_back(stStep);
				}
			}
		}
	};
	virtual CConfig_Svr& GetAt(__in uint16_t IN_nIdx)	{
		return SvrList.at(IN_nIdx);	}
	virtual const CConfig_Svr& GetAt(__in uint16_t IN_nIdx) const	{
		return SvrList.at(IN_nIdx);	}
	bool Get_ServerType(__in LPCTSTR IN_szEqpID, __out uint8_t& OUT_nEqpType)	{
		for (auto nIdx = 0; nIdx < SvrList.size(); ++nIdx)		{
			if (0 == SvrList.at(nIdx).Get_ServerId().Compare(IN_szEqpID))			{
				OUT_nEqpType = SvrList.at(nIdx).Get_ServerType();
				return true;
			}
		}
		return false;
	};
	bool Get_ServerID(__in uint8_t IN_LineOrder, __out CString& OUT_szEqpID)	{
		if (IN_LineOrder < SvrList.size())		{
			OUT_szEqpID = SvrList.at(IN_LineOrder).Get_ServerId();
			return true;
		}
		return false;
	};
	uint8_t Get_ServerCount(__in uint8_t nEquipmentType)	{
		uint8_t nCount = 0;
		for (auto nIdx = 0; nIdx < SvrList.size(); ++nIdx)		{
			if (nEquipmentType == SvrList.at(nIdx).Get_ServerType())			{
				++nCount;
			}
		}
		return nCount;
	};
	uint8_t Get_ServerCount(__in LPCTSTR IN_szEqpID)	{
		uint8_t nEquipmentType = 0;
		if (Get_ServerType(IN_szEqpID, nEquipmentType))		{
			return Get_ServerCount(nEquipmentType);
		}	else	{
			return 0;
		}
	};
};
#endif // Def_LineConfig_h__
