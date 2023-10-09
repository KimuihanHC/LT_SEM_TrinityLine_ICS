//*****************************************************************************
// Filename	: 	InOutCount.h
// Created	:	2023/3/16 - 13:51
// Modified	:	2023/3/16 - 13:51
//
// Author	:	piring
//	
// Purpose	: �ζ��ο� ���� ����/����/���� ī��Ʈ
//*****************************************************************************
#ifndef InOutCount_h__
#define InOutCount_h__


#pragma once

class CInOutCount
{
protected:
	int32_t		m_nIn		= 0;	// ����
	uint32_t	m_nOut		= 0;	// ����
	uint32_t	m_nOngoing	= 0;	// ���� ��
	uint32_t	m_nRemove	= 0;	// �߰��� ���� ��

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

	// ������Ʈ�� ����
	bool Save_Registry();

	// ������Ʈ�� �ҷ�����
	bool Load_Registry();

public:
	virtual void OnReset();

	// �� ������ ���� �ؾ� �Ѵ�.
	virtual void OnIncrease_In();

	// �� ������ ���� �ؾ� �Ѵ�.
	virtual void OnIncrease_Out();

	// �� ������ ���� �ؾ� �Ѵ�.
	virtual void OnIncrease_Remove();

	// csv ����
	bool Save_CSVFile		(__in LPCTSTR IN_szFullPath);

	
	// ���񿡼��� �׽�Ʈ �Ϸ� ���� ī��Ʈ
	// ���� ��ε� ���� ī��Ʈ

};

#endif // InOutCount_h__
