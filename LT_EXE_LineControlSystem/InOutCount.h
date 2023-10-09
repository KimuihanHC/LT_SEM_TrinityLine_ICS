//*****************************************************************************
// Filename	: 	InOutCount.h
// Created	:	2023/3/16 - 13:51
// Modified	:	2023/3/16 - 13:51
//
// Author	:	piring
//	
// Purpose	: 인라인에 소켓 투입/배출/제거 카운트
//*****************************************************************************
#ifndef InOutCount_h__
#define InOutCount_h__


#pragma once

class CInOutCount
{
protected:
	int32_t		m_nIn		= 0;	// 투입
	uint32_t	m_nOut		= 0;	// 배출
	uint32_t	m_nOngoing	= 0;	// 진행 중
	uint32_t	m_nRemove	= 0;	// 중간에 제거 됨

	CString		m_szRegPath;
public:
	CInOutCount();
	~CInOutCount();
	CInOutCount& operator= (const CInOutCount& ref)
	{
		m_nIn		= ref.m_nIn;
		m_nOut		= ref.m_nOut;
		m_nOngoing	= ref.m_nOngoing;
		m_nRemove	= ref.m_nRemove;

		return *this;
	};

protected:
	void Compute_Ongoing();

	// 레지스트리 저장
	bool Save_Registry();

	// 레지스트리 불러오기
	bool Load_Registry();

public:
	virtual void OnReset();

	// 빈 소켓은 제외 해야 한다.
	virtual void OnIncrease_In();

	// 빈 소켓은 제외 해야 한다.
	virtual void OnIncrease_Out();

	// 빈 소켓은 제외 해야 한다.
	virtual void OnIncrease_Remove();

	// csv 저장
	bool Save_CSVFile		(__in LPCTSTR IN_szFullPath);

	
	// 설비에서의 테스트 완료 기준 카운트
	// 소켓 언로드 기준 카운트

};

#endif // InOutCount_h__
