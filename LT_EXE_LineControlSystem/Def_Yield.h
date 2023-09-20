//*****************************************************************************
// Filename	: 	Def_Yield.h
// Created	:	2021/12/9 - 14:02
// Modified	:	2021/12/9 - 14:02
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#ifndef Def_Yield_h__
#define Def_Yield_h__


#pragma once

#include "Def_Yield_Cm.h"


// 설비 2파라 이상 사용 할 때
typedef enum 
{
	Para_Left,
	Para_Right,
	Para_Center,	// Hot Pixel 3Para 설비 대응
	Para_MaxCount,
}enEqpPara;

static LPCTSTR g_szEqpPara[] =
{
	_T("Left"),
	_T("Right"),
	_T("Center"),
	nullptr
};

static LPCTSTR g_szEqpPara_Mini[] =
{
	_T("L"),
	_T("R"),
	_T("C"),
	nullptr
};

//---------------------------------------------------------
// 설비별 수율
// T, P, F, LT, LP, LF, RT, RP, RF
//---------------------------------------------------------
class CYield_Equipment: public CYield
{
public:
	// 설비 전체 (각 파라 별)

	//std::vector<CYield> m_Para;
	CYield		m_Para[Para_MaxCount];

	CYield_Equipment()
	{

	};

	virtual ~CYield_Equipment()
	{

	};

	CYield_Equipment& operator= (const CYield_Equipment& ref)
	{
		__super::operator=(ref);

		for (auto nIdx = 0; nIdx < Para_MaxCount; ++nIdx)
		{
			m_Para[nIdx] = ref.m_Para[nIdx];
		}

		return *this;
	};

	inline virtual void	Reset()
	{
		__super::Reset();

		for (auto nIdx = 0; nIdx < Para_MaxCount; ++nIdx)
		{
			m_Para[nIdx].Reset();
		}
	};

	inline virtual void	ComputeYield()
	{
		__super::ComputeYield();

		for (auto nIdx = 0; nIdx < Para_MaxCount; ++nIdx)
		{
			m_Para[nIdx].ComputeYield();
		}
	};

	virtual void IncreasePass(__in uint8_t IN_nPara)
	{
		if (IN_nPara < Para_MaxCount)
		{
			++dwPass;

			m_Para[IN_nPara].IncreasePass();

			ComputeYield();
		}
	};

	virtual void IncreaseFail(__in uint8_t IN_nPara)
	{
		if (IN_nPara < Para_MaxCount)
		{
			++dwFail;

			m_Para[IN_nPara].IncreaseFail();

			ComputeYield();
		}
	};
};

//---------------------------------------------------------
// 소켓별 수율
// T, P, F 
// E_T, E_P, E_F, E_LT, E_LP, E_LF, E_RT, E_RP, E_RF, .....
//---------------------------------------------------------
class CYield_Socket: public CYield
{
public:
	// 소켓 전체

	// 설비별 (Equipment id) => 사용 할 지 판단 보류
#ifdef _UNICODE
	std::map<std::wstring, CYield_Equipment> m_Equipments;
#else
	std::map<std::string, CYield_Equipment> m_Equipments;
#endif

	CYield_Socket()
	{

	};

	virtual ~CYield_Socket()
	{

	};
	

	CYield_Socket& operator= (const CYield_Socket& ref)
	{
		__super::operator=(ref);

		// 추가사항
		m_Equipments.clear();

		m_Equipments = ref.m_Equipments;

		return *this;
	};

	inline virtual void	Reset()
	{
		__super::Reset();

		// 추가사항
		for (auto Iter = m_Equipments.begin(); Iter != m_Equipments.end(); Iter++)
		{
			Iter->second.Reset();
		}
	};

	virtual void IncreasePass_byEqp(__in LPCTSTR szEqpID, __in uint8_t IN_nPara)
	{
		auto result = m_Equipments.find(szEqpID);
		if (result != m_Equipments.end())
		{
			m_Equipments.at(szEqpID).IncreasePass(IN_nPara);
		}
		else
		{
			// 추가
			CYield_Equipment newEqp;

			auto Ret = m_Equipments.insert({ szEqpID, newEqp });
			if (Ret.second)
			{
				m_Equipments.at(szEqpID).IncreasePass(IN_nPara);
			}
		}
	};

	virtual void IncreaseFail_byEqp(__in LPCTSTR szEqpID, __in uint8_t IN_nPara)
	{
		auto result = m_Equipments.find(szEqpID);
		if (result != m_Equipments.end())
		{
			m_Equipments.at(szEqpID).IncreaseFail(IN_nPara);
		}
		else
		{
			// 추가
			CYield_Equipment newEqp;

			auto Ret = m_Equipments.insert({ szEqpID, newEqp });
			if (Ret.second)
			{
				m_Equipments.at(szEqpID).IncreaseFail(IN_nPara);
			}
		}
	};
};

//---------------------------------------------------------
// 라인 전체의 생산수율
//---------------------------------------------------------
class CYield_Line
{
public:
	// Total
	CYield		m_Unload; // 언로더 배출 기준 생산량

	 // 설비 기준 수율 (Equipment id)
#ifdef _UNICODE
	std::map<std::wstring, CYield_Equipment> m_Equipments; // 테스터 only????
#else
	std::map<std::string, CYield_Equipment> m_Equipments; // 테스터 only????
#endif

	// 소켓 기준 수율 (RFID)
#ifdef _UNICODE
	std::map<std::wstring, CYield_Socket> m_Sockets;
#else
	std::map<std::string, CYield_Socket> m_Sockets;
#endif

	CYield_Line()
	{

	};

	~CYield_Line()
	{

	};	

	CYield_Line& operator= (const CYield_Line& ref)
	{
		m_Unload = ref.m_Unload;

		m_Equipments.clear();
		m_Equipments = ref.m_Equipments;

		m_Sockets.clear();
		m_Sockets = ref.m_Sockets;

		return *this;
	};

	// 전체 데이터  초기화
	inline virtual void	Reset()
	{
		m_Unload.Reset();

		for (auto Iter = m_Equipments.begin(); Iter != m_Equipments.end(); Iter++)
		{
			Iter->second.Reset();
		}

		for (auto Iter = m_Sockets.begin(); Iter != m_Sockets.end(); Iter++)
		{
			Iter->second.Reset();
		}
	};

	// 특정 설비 데이터 초기화
	inline virtual void	Reset_Equipment(__in LPCTSTR szEqpID)
	{
		auto result = m_Equipments.find(szEqpID);
		if (result != m_Equipments.end())
		{
			m_Equipments.at(szEqpID).Reset();
		}
	};

	// 특정 소켓 데이터 초기화
	inline virtual void	Reset_Socket(__in LPCTSTR szRFID)
	{
		auto result = m_Sockets.find(szRFID);
		if (result != m_Sockets.end())
		{
			m_Sockets.at(szRFID).Reset();
		}
	};

	// 설비 목록 업데이트 (설비 데이터 추가)
#ifdef _UNICODE
	void Init_Equipments(__in const std::vector<std::wstring>* IN_EqpIDs)
#else
	void Init_Equipments(__in const std::vector<std::string>* IN_EqpIDs)
#endif
	{
		for (auto const &element : *IN_EqpIDs)
		{
			auto search = m_Equipments.find(element);
			if (search == m_Equipments.end())
			{
				CYield_Equipment newData;
				m_Equipments.insert({ element, newData });
			}
		}
	};

	// 소켓 목록 업데이트 (소켓 데이터 추가)
#ifdef _UNICODE
	void Init_Sockets(__in const std::vector<std::wstring>* IN_RFIDs)
#else
	void Init_Sockets(__in const std::vector<std::string>* IN_RFIDs)
#endif
	{
		for (auto const &element : *IN_RFIDs)
		{
			auto search = m_Sockets.find(element);
			if (search == m_Sockets.end())
			{
				CYield_Socket newData;
				m_Sockets.insert({ element, newData });
			}
		}
	};

	// Increase (equipment id, socket rfid, pass/fail)
	virtual void IncreasePass(__in LPCTSTR szEqpID, __in LPCTSTR szRFID, __in uint8_t IN_nPara)
	{
		auto result = m_Equipments.find(szEqpID);
		if (result != m_Equipments.end())
		{
			m_Equipments.at(szEqpID).IncreasePass(IN_nPara);
		}

		auto result_sock = m_Sockets.find(szRFID);
		if (result_sock != m_Sockets.end())
		{
			m_Sockets.at(szRFID).IncreasePass_byEqp(szEqpID, IN_nPara);
		}
	};

	virtual void IncreaseFail(__in LPCTSTR szEqpID, __in LPCTSTR szRFID, __in uint8_t IN_nPara)
	{
		auto result = m_Equipments.find(szEqpID);
		if (result != m_Equipments.end())
		{
			m_Equipments.at(szEqpID).IncreaseFail(IN_nPara);
		}

		auto result_sock = m_Sockets.find(szRFID);
		if (result_sock != m_Sockets.end())
		{
			m_Sockets.at(szRFID).IncreaseFail_byEqp(szEqpID, IN_nPara);
		}
	};
};





#endif // Def_Yield_h__
