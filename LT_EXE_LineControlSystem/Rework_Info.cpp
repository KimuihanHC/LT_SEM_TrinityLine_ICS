//*****************************************************************************
// Filename	: 	Rework_Info.cpp
// Created	:	2022/2/9 - 19:35
// Modified	:	2022/2/9 - 19:35
//
// Author	:	Author
//	
// Purpose	:	
//*****************************************************************************
#include "stdafx.h"
#include "Rework_Info.h"
#include "CommonFunction.h"
#include "RegRework.h"

#define DayToSecond		 86400.0f
#define WeekToSecond	604800.0f

CRework_Info::CRework_Info()
{
	m_pReg = new CRegRework();

	Load_ReworkInfo();
	Remove_ReworkInfo_Old();
}

CRework_Info::~CRework_Info()
{
	delete m_pReg;
}

//=============================================================================
// Method		: Save_ReworkInfo
// Access		: protected  
// Returns		: bool
// Parameter	: __in LPCTSTR IN_szBarcode
// Parameter	: __in ST_ReworkProduct * IN_pFailProduct
// Qualifier	:
// Last Update	: 2022/2/10 - 14:14
// Desc.		:
//=============================================================================
bool CRework_Info::Save_ReworkProduct(__in LPCTSTR IN_szBarcode, __in ST_ReworkProduct* IN_pFailProduct)
{
	return m_pReg->Set_ReworkProduct(IN_szBarcode, IN_pFailProduct);
}

bool CRework_Info::Load_ReworkProduct(__in LPCTSTR IN_szBarcode, __out ST_ReworkProduct& OUT_FailProduct)
{
	return m_pReg->Get_ReworkProduct(IN_szBarcode, OUT_FailProduct);
}

bool CRework_Info::Remove_ReworkProduct(__in LPCTSTR IN_szBarcode)
{
	return m_pReg->Remove_ReworkProduct(IN_szBarcode, this);
}

bool CRework_Info::Save_ReworkInfo()
{
	return m_pReg->Set_ReworkInfo(this);
}

bool CRework_Info::Load_ReworkInfo()
{
	return m_pReg->Get_ReworkInfo(m_ReworkInfo);
}

bool CRework_Info::Remove_ReworkInfo()
{
	return m_pReg->Remove_ReworkInfo();
}

//=============================================================================
// Method		: Remove_OldReworkInfo
// Access		: protected  
// Returns		: bool
// Qualifier	:
// Last Update	: 2022/2/10 - 14:58
// Desc.		:
//=============================================================================
bool CRework_Info::Remove_ReworkInfo_Old()
{
	// 오래된 제품 정보 삭제
	SYSTEMTIME tmCurrent;
	GetLocalTime(&tmCurrent);

	double dDiffTime = 0.0f;
	for (auto Iter = m_ReworkInfo.begin(); Iter != m_ReworkInfo.end(); Iter++)
	{
		dDiffTime = CompareSystemTime(&tmCurrent, &Iter->second.InputTime);

		// 시간 차이가 1주 이상 차이나면 제품 정보 삭제
		// 1일 : 86400 초
		if (WeekToSecond <= dDiffTime)
		{
			// 데이터에서 삭제
			// 레지스트리에서 삭제
			Remove_ReworkProduct(Iter->first.c_str());
		}
	}

	return true;
}

//=============================================================================
// Method		: Is_Rework
// Access		: public  
// Returns		: bool
// Parameter	: __in LPCTSTR IN_szBarcode
// Qualifier	:
// Last Update	: 2022/2/10 - 14:14
// Desc.		:
//=============================================================================
bool CRework_Info::Is_Rework(__in LPCTSTR IN_szBarcode)
{
	auto result_fail = m_ReworkInfo.find(IN_szBarcode);
	if (result_fail != m_ReworkInfo.end())
	{
		return true;
	}

	return false;
}

ST_ReworkProduct& CRework_Info::Get_ReworkProduct(__in LPCTSTR IN_szBarcode)
{
	return m_ReworkInfo.at(IN_szBarcode);
}

const ST_ReworkProduct& CRework_Info::Get_ReworkProduct(__in LPCTSTR IN_szBarcode) const
{
	return m_ReworkInfo.at(IN_szBarcode);
}

bool CRework_Info::Add_ReworkProduct(__in LPCTSTR IN_szBarcode, __in ST_ReworkProduct* IN_pFailProduct)
{
	auto result_fail = m_ReworkInfo.find(IN_szBarcode);
	if (result_fail != m_ReworkInfo.end())
	{
		// Modify (재불량 발생으로 데이터 업데이트)
		m_ReworkInfo.at(IN_szBarcode) = *IN_pFailProduct;

		// 레지스트리에 저장
		Save_ReworkProduct(IN_szBarcode, IN_pFailProduct);
	}
	else
	{
		// Insert
		auto Ret = m_ReworkInfo.insert({ IN_szBarcode, *IN_pFailProduct });

		if (Ret.second)
		{
			Save_ReworkProduct(IN_szBarcode, IN_pFailProduct);
		}

		return Ret.second;
	}	

	return true;
}

bool CRework_Info::Add_ReworkProduct(__in LPCTSTR IN_szBarcode, __in uint8_t IN_nEqpType, __in uint16_t IN_nNG_Code, __in SYSTEMTIME* IN_pInputTime)
{
	ST_ReworkProduct stNew;
	//stNew.szBarcode		= IN_szBarcode;
	stNew.nNG_EqpType	= IN_nEqpType;
	stNew.nNG_Code		= IN_nNG_Code;
	memcpy(&stNew.InputTime, IN_pInputTime, sizeof(SYSTEMTIME));

	return Add_ReworkProduct(IN_szBarcode, &stNew);
}

bool CRework_Info::Delete_ReworkProduct(__in LPCTSTR IN_szBarcode)
{
	auto result_fail = m_ReworkInfo.find(IN_szBarcode);
	if (result_fail != m_ReworkInfo.end())
	{
		m_ReworkInfo.erase(IN_szBarcode);
		Remove_ReworkProduct(IN_szBarcode);
	}

	return true;
}

bool CRework_Info::Delete_ReworkInfo()
{
	m_ReworkInfo.clear();

	return Remove_ReworkInfo();
}
