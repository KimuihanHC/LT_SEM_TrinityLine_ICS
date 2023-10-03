//*****************************************************************************
// Filename	: 	Def_ConfigLine.h
// Created	:	2021/11/1 - 15:23
// Modified	:	2021/11/1 - 15:23
//
// Author	:	piring
//	
// Purpose	:	
//*****************************************************************************
#ifndef Def_LineConfig_h__
#define Def_LineConfig_h__


#pragma once

#include <afxwin.h>
#include <stdint.h>
#include <vector>

#include "Def_ConfigEquipment.h"

//-----------------------------------------------------------------------------
// 라인의 검사기 구성 설정용 구조체
//-----------------------------------------------------------------------------
class CConfig_Line
{
public:
	std::vector<CConfig_Eqp> EqpList;

	CConfig_Line()
	{

	};

	CConfig_Line& operator= (const CConfig_Line& ref)
	{
		EqpList.clear();
		EqpList = ref.EqpList;
		return *this;
	};

	// 스텝 갯수
	size_t GetCount() const
	{
		return EqpList.size();
	};

	// 모든 스텝 삭제
	virtual void RemoveAll()
	{
		EqpList.clear();
	};

	// 스텝 추가
	virtual void Eqp_Add(__in CConfig_Eqp IN_stTestStep)
	{
		EqpList.push_back(IN_stTestStep);
	};

	// 스텝 삽입
	virtual void Eqp_Insert(__in uint16_t IN_nIdx, __in CConfig_Eqp IN_stTestStep)
	{
		if (0 < EqpList.size())
		{
			EqpList.insert(EqpList.begin() + IN_nIdx, IN_stTestStep);
		}
	};
	// 스텝 삭제
	virtual void Eqp_Remove(__in uint16_t IN_nIdx)
	{
		if (0 < EqpList.size())
		{
			EqpList.erase(EqpList.begin() + IN_nIdx);
		}
	};

	// 스텝 수정
	virtual void Eqp_Modify(__in uint16_t IN_nIdx, __in CConfig_Eqp IN_stTestStep)
	{
		if (0 < EqpList.size())
		{
			EqpList.at(IN_nIdx) = IN_stTestStep;
		}
	};

	// 스텝 위로 이동
	virtual void Eqp_Up(__in uint16_t IN_nIdx)
	{
		if (0 < EqpList.size())
		{
			// 0번 인덱스는 위로 이동 불가
			if ((0 < IN_nIdx) && (1 < EqpList.size()))
			{
				CConfig_Eqp stStep = EqpList.at(IN_nIdx);

				EqpList.erase(EqpList.begin() + IN_nIdx);
				EqpList.insert(EqpList.begin() + IN_nIdx, stStep);
			}
		}
	};
	// 스텝 아래로 이동
	virtual void Eqp_Down(__in uint16_t IN_nIdx)
	{
		if (0 < EqpList.size())
		{
			// 마지막 인덱스는 아래로 이동 불가
			if ((IN_nIdx < (EqpList.size() - 1)) && (1 < EqpList.size()))
			{
				CConfig_Eqp stStep = EqpList.at(IN_nIdx);

				EqpList.erase(EqpList.begin() + IN_nIdx);

				// 변경되는 위치가 최하단이면, Insert 대신 Add 사용
				if ((IN_nIdx + 1) < (EqpList.size()))
				{
					EqpList.insert(EqpList.begin() + IN_nIdx, stStep);
				}
				else
				{
					EqpList.push_back(stStep);
				}
			}
		}
	};
	virtual CConfig_Eqp& GetAt(__in uint16_t IN_nIdx)
	{
		return EqpList.at(IN_nIdx);
	}
	virtual const CConfig_Eqp& GetAt(__in uint16_t IN_nIdx) const
	{
		return EqpList.at(IN_nIdx);
	}
	// Eqp id로 Eqp type 구하기
	bool Get_EquipmentType(__in LPCTSTR IN_szEqpID, __out uint8_t& OUT_nEqpType)
	{
		for (auto nIdx = 0; nIdx < EqpList.size(); ++nIdx)
		{
			if (0 == EqpList.at(nIdx).Get_EquipmentId().Compare(IN_szEqpID))
			{
				OUT_nEqpType = EqpList.at(nIdx).Get_EquipmentType();
				return true;
			}
		}

		return false;
	};
	// 설비 순서로 Eqp id 구하기
	bool Get_EquipmentID(__in uint8_t IN_LineOrder, __out CString& OUT_szEqpID)
	{
		if (IN_LineOrder < EqpList.size())
		{
			OUT_szEqpID = EqpList.at(IN_LineOrder).Get_EquipmentId();
			return true;
		}

		return false;
	};
	// 설비 타입으로 라인의 동일 설비 개수 구하기
	uint8_t Get_EquipmentCount(__in uint8_t nEquipmentType)
	{
		uint8_t nCount = 0;

		for (auto nIdx = 0; nIdx < EqpList.size(); ++nIdx)
		{
			if (nEquipmentType == EqpList.at(nIdx).Get_EquipmentType())
			{
				++nCount;
			}
		}

		return nCount;
	};
	// Eqp ID로 라인의 동일 설비 개수 구하기
	uint8_t Get_EquipmentCount(__in LPCTSTR IN_szEqpID)
	{
		uint8_t nEquipmentType = 0;

		if (Get_EquipmentType(IN_szEqpID, nEquipmentType))
		{
			return Get_EquipmentCount(nEquipmentType);
		}
		else
		{
			return 0;
		}
	};

// 	WORD Get_ItemID(__in UINT IN_nIdx)
// 	{
// 		return EqpList.at(IN_nIdx).nItem_ID;
// 	}
	
}; // CConfig_Line
#endif // Def_LineConfig_h__
