//*****************************************************************************
// Filename	: 	LineInfo.cpp
// Created	:	2021/11/22 - 18:00
// Modified	:	2021/11/22 - 18:00
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#include "stdafx.h"
#include "LineInfo.h"
#include "EqpLoader.h"
#include "EqpReturner.h"
#include "EqpTester.h"
#include "EqpHandler.h"

CLineInfo::CLineInfo()
{

}

CLineInfo::~CLineInfo()
{
	RemoveAll();
}

//=============================================================================
// Method		: Compare_PreConfigLine
// Access		: protected  
// Returns		: bool
// Parameter	: __in const CConfig_Line * IN_pCConfig_Line
// Qualifier	:
// Last Update	: 2022/1/14 - 17:20
// Desc.		:
//=============================================================================
bool CLineInfo::Compare_PreConfigLine(__in const CConfig_Line* IN_pCConfig_Line)
{
	// ������ ������ ���� ������ ���ο� ������ ������ �� �Ѵ�.
	size_t nEqpCnt_Prev = m_EqpList.size();
	size_t nEqpCnt_New = IN_pCConfig_Line->GetCount();

	if (nEqpCnt_Prev != nEqpCnt_New)
	{
		return false;
	}

	for (auto nIdx = 0; nIdx < nEqpCnt_Prev; ++nIdx)
	{
		if (m_EqpList.at(nIdx)->Get_EquipmentType() != IN_pCConfig_Line->GetAt(nIdx).Get_EquipmentType())
		{
			return false;
		}
	}

	return true;
}

//=============================================================================
// Method		: IsSkip_EquipmentType
// Access		: protected  
// Returns		: bool
// Parameter	: __in enEquipmentType IN_nEqpType
// Qualifier	:
// Last Update	: 2022/2/3 - 18:12
// Desc.		:
//=============================================================================
bool CLineInfo::IsSkip_EquipmentType(__in enEquipmentType IN_nEqpType)
{
	// �ش� �˻��� ������� Skip �������� Ȯ��
	if (Get_FirstEquipment((enEquipmentType)IN_nEqpType).Get_Group())
	{
		for (auto nEqpIdx = Get_FirstEquipment((enEquipmentType)IN_nEqpType).Get_EqpOrder(); nEqpIdx < m_EqpList.size(); ++nEqpIdx)
		{
			if (m_EqpList.at(nEqpIdx)->Get_EquipmentType() == IN_nEqpType)
			{
				if (false == m_EqpList.at(nEqpIdx)->Get_Skip())
				{
					return false;
				}
			}
		}// end of for()
	}
	else
	{
		return Get_FirstEquipment((enEquipmentType)IN_nEqpType).Get_Skip();
	}

	return true;
}

#ifdef 	NEW_INSPECTION_TYPE_APPLY
bool CLineInfo::IsSkip_InspectionType(__in enInspectionType IN_nInspType)
{
	for (auto nIdx = 0; nIdx < m_EqpList.size(); ++nIdx)
	{
		if (m_EqpList.at(nIdx)->Get_InspectionType() == IN_nInspType)
		{
			if (m_EqpList.at(nIdx)->Get_Skip())
			{
				return true;
			}
		}
	}

	return false;
}
#endif	// NEW_INSPECTION_TYPE_APPLY

//=============================================================================
// Method		: Write_File
// Access		: protected  
// Returns		: bool
// Parameter	: LPCTSTR IN_szPathFile
// Parameter	: LPCTSTR IN_szData
// Parameter	: LPCTSTR IN_szHeader
// Qualifier	:
// Last Update	: 2022/4/20 - 17:16
// Desc.		:
//=============================================================================
bool CLineInfo::Write_File(LPCTSTR IN_szPathFile, LPCTSTR IN_szData, LPCTSTR IN_szHeader)
{
	CString szFullPath = IN_szPathFile;
	CString szUnicode = IN_szData;
	CStringA szANSI;

	CFile File;
	CFileException e;

	if (!PathFileExists(szFullPath.GetBuffer()))
	{
		if (!File.Open(szFullPath.GetBuffer(), CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite, &e))
			return false;

		// ��� �߰�
		CString szHeader = IN_szHeader;
		szUnicode = szHeader + szUnicode;
	}
	else
	{
		if (!File.Open(szFullPath.GetBuffer(), CFile::modeWrite | CFile::shareDenyWrite, &e))
			return false;
	}

	USES_CONVERSION;
	szANSI = CT2A(szUnicode.GetBuffer());

	File.SeekToEnd();
	File.Write(szANSI.GetBuffer(0), szANSI.GetLength());
	File.Flush();
	File.Close();

	return true;
}

//=============================================================================
// Method		: GetCount
// Access		: public  
// Returns		: size_t
// Qualifier	: const
// Last Update	: 2022/1/14 - 17:20
// Desc.		:
//=============================================================================
size_t CLineInfo::GetCount() const
{
	return m_EqpList.size();
}

void CLineInfo::RemoveAll()
{
	for (auto nIdx = 0; nIdx < m_EqpList.size(); ++nIdx)
	{
		delete m_EqpList.at(nIdx);
	}

	m_EqpList.clear();
}

CEquipment& CLineInfo::GetAt(__in uint16_t IN_nIdx)
{
	return *m_EqpList.at(IN_nIdx);
}

const CEquipment& CLineInfo::GetAt(__in uint16_t IN_nIdx) const
{
	return *m_EqpList.at(IN_nIdx);
}

//=============================================================================
// Method		: Get_EquipmentIndex
// Access		: public  
// Returns		: int
// Parameter	: __in LPCTSTR IN_szEqpId
// Qualifier	:
// Last Update	: 2022/1/14 - 17:21
// Desc.		:
//=============================================================================
int CLineInfo::Get_EquipmentIndex(__in LPCTSTR IN_szEqpId)
{
	int iReturn = -1;
	size_t nEqpCnt = m_EqpList.size();

	for (auto nEqpIdx = 0; nEqpIdx < nEqpCnt; ++nEqpIdx)
	{
		if (0 == m_EqpList.at(nEqpIdx)->Get_EquipmentId().Compare(IN_szEqpId))
		{
			return nEqpIdx;
		}
	}

	return iReturn;
}

//=============================================================================
// Method		: Get_FirstEquipment
// Access		: public  
// Returns		: CEquipment&
// Parameter	: __in enEquipmentType IN_nEqpType
// Qualifier	:
// Last Update	: 2022/8/12 - 18:14
// Desc.		:
//=============================================================================
CEquipment& CLineInfo::Get_FirstEquipment(__in enEquipmentType IN_nEqpType)
{
	return **m_pEquipment_Group[IN_nEqpType].begin();
}

const CEquipment& CLineInfo::Get_FirstEquipment(__in enEquipmentType IN_nEqpType) const
{
	return **m_pEquipment_Group[IN_nEqpType].begin();
}

CEquipment& CLineInfo::Get_LastEquipment(__in enEquipmentType IN_nEqpType)
{
	return **m_pEquipment_Group[IN_nEqpType].end();
}

const CEquipment& CLineInfo::Get_LastEquipment(__in enEquipmentType IN_nEqpType) const
{
	return **m_pEquipment_Group[IN_nEqpType].end();
}

//=============================================================================
// Method		: Get_Loader
// Access		: public  
// Returns		: CEquipment&
// Qualifier	:
// Last Update	: 2022/8/12 - 18:14
// Desc.		:
//=============================================================================
CEquipment& CLineInfo::Get_Loader()
{
	return **m_pEquipment_Group[Eqp_Loader].begin();
}

const CEquipment& CLineInfo::Get_Loader() const
{
	return **m_pEquipment_Group[Eqp_Loader].begin();
}

CEquipment& CLineInfo::Get_Returner()
{
	return **m_pEquipment_Group[Eqp_Returner].begin();
}

const CEquipment& CLineInfo::Get_Returner() const
{
	return **m_pEquipment_Group[Eqp_Returner].begin();
}

CEquipment& CLineInfo::Get_Handler()
{
	return **m_pEquipment_Group[Eqp_Handler].begin();
}

const CEquipment& CLineInfo::Get_Handler() const
{
	return **m_pEquipment_Group[Eqp_Handler].begin();
}

//=============================================================================
// Method		: Get_FirstTester
// Access		: public  
// Returns		: CEquipment&
// Parameter	: __in enTesterType IN_nTesterType
// Qualifier	:
// Last Update	: 2022/8/12 - 18:14
// Desc.		:
//=============================================================================
CEquipment& CLineInfo::Get_FirstTester(__in enTesterType IN_nTesterType)
{
	return **m_pEquipment_Group[ConvTo_EquipmentType(IN_nTesterType)].begin();
}

const CEquipment& CLineInfo::Get_FirstTester(__in enTesterType IN_nTesterType) const
{
	return **m_pEquipment_Group[ConvTo_EquipmentType(IN_nTesterType)].begin();
}

#ifdef 	NEW_INSPECTION_TYPE_APPLY
CEquipment& CLineInfo::Get_FirstTester(__in enInspectionType IN_nInspType)
{
	return **m_pInspection_Group[IN_nInspType].begin();
}

const CEquipment& CLineInfo::Get_FirstTester(__in enInspectionType IN_nInspType) const
{
	return **m_pEquipment_Group[IN_nInspType].begin();
}
#endif	// NEW_INSPECTION_TYPE_APPLY

CEquipment& CLineInfo::Get_LastTester(__in enTesterType IN_nTesterType)
{
	return **m_pEquipment_Group[ConvTo_EquipmentType(IN_nTesterType)].end();
}

const CEquipment& CLineInfo::Get_LastTester(__in enTesterType IN_nTesterType) const
{
	return **m_pEquipment_Group[ConvTo_EquipmentType(IN_nTesterType)].end();
}

#ifdef 	NEW_INSPECTION_TYPE_APPLY
CEquipment& CLineInfo::Get_LastTester(__in enInspectionType IN_nInspType)
{
	return **m_pEquipment_Group[IN_nInspType].end();
}

const CEquipment& CLineInfo::Get_LastTester(__in enInspectionType IN_nInspType) const
{
	return **m_pEquipment_Group[IN_nInspType].end();
}
#endif	// NEW_INSPECTION_TYPE_APPLY

//=============================================================================
// Method		: Get_NextEquipment
// Access		: public  
// Returns		: uint8_t
// Parameter	: __in uint8_t IN_FromEqp
// Qualifier	:
// Last Update	: 2022/2/4 - 17:54
// Desc.		:
//=============================================================================
uint8_t CLineInfo::Get_NextEquipment(__in uint8_t IN_FromEqp)
{
	if (IN_FromEqp < m_EqpList.size())
	{
		for (auto nIdx = IN_FromEqp; nIdx < m_EqpList.size(); ++nIdx)
		{
			if (m_EqpList.at(IN_FromEqp)->Get_EquipmentType() != m_EqpList.at(nIdx)->Get_EquipmentType())
			{
				if (m_bEnable_TestType[m_EqpList.at(nIdx)->Get_TesterType()])
				{
					if (false == m_EqpList.at(nIdx)->Get_Skip())
					{
						return nIdx;
					}
				}
			}
		}
	}

	return Get_Returner().Get_EqpOrder();
}

//=============================================================================
// Method		: Get_PrevEquipment
// Access		: public  
// Returns		: uint8_t
// Parameter	: __in uint8_t IN_FromEqp
// Qualifier	:
// Last Update	: 2022/4/20 - 17:16
// Desc.		:
//=============================================================================
uint8_t CLineInfo::Get_PrevEquipment(__in uint8_t IN_FromEqp)
{
	if (IN_FromEqp < m_EqpList.size())
	{
		for (auto nIdx = IN_FromEqp; 0 < nIdx; --nIdx)
		{
			if (m_EqpList.at(IN_FromEqp)->Get_EquipmentType() != m_EqpList.at(nIdx)->Get_EquipmentType())
			{
				if (m_bEnable_TestType[m_EqpList.at(nIdx)->Get_TesterType()])
				{
					if (false == m_EqpList.at(nIdx)->Get_Skip())
					{
						return nIdx;
					}
				}
			}
		}
	}

	return Get_Loader().Get_EqpOrder();
}

//=============================================================================
// Method		: Get_TesterCount
// Access		: public  
// Returns		: uint8_t
// Qualifier	:
// Last Update	: 2022/8/12 - 17:57
// Desc.		:
//=============================================================================
uint8_t CLineInfo::Get_TesterCount()
{
	uint8_t nCount = 0;

	if (0 < m_EqpList.size())
	{		
		for (auto nIdx = 0; nIdx < m_EqpList.size(); ++nIdx)
		{
			if (m_EqpList.at(nIdx)->Is_Tester())
			{
				++nCount;
			}
		}
	}

	return nCount;
}

//=============================================================================
// Method		: Set_Config_Line
// Access		: public  
// Returns		: void
// Parameter	: __in const CConfig_Line * IN_pConfig_Line
// Qualifier	:
// Last Update	: 2022/1/14 - 16:23
// Desc.		:
//=============================================================================
void CLineInfo::Set_Config_Line(__in const CConfig_Line* IN_pConfig_Line)
{
	// ���� �����͸� ����ϰ� �����ؾ� �ϴ���.....
	if (Compare_PreConfigLine(IN_pConfig_Line))
	{
		// ���� ������ ���� (�ٲ� �����͸� ����)
		size_t nEqpCnt = m_EqpList.size();

		for (auto nIdx = 0; nIdx < nEqpCnt; ++nIdx)
		{
			*m_EqpList.at(nIdx) = IN_pConfig_Line->GetAt(nIdx);
		}

	}
	else
	{
		// ���� ������ �ٸ� (���� ������ �ʱ�ȭ�ϰ� �ٽ� ����)
		RemoveAll();
		
		size_t nEqpCnt = IN_pConfig_Line->GetCount();

		for (auto nIdx = 0; nIdx < nEqpCnt; ++nIdx)
		{
			switch (IN_pConfig_Line->GetAt(nIdx).Get_EquipmentType())
			{
			case enEquipmentType::Eqp_Handler:
			{
				CEqpHandler* newEqp = new CEqpHandler();
				*newEqp = IN_pConfig_Line->GetAt(nIdx);
				newEqp->Set_EqpOrder(nIdx);
				m_EqpList.push_back(newEqp);
			}
			break;

			case enEquipmentType::Eqp_Loader:
			{
				CEqpLoader* newEqp = new CEqpLoader();
				*newEqp = IN_pConfig_Line->GetAt(nIdx);
				newEqp->Set_EqpOrder(nIdx);
				m_EqpList.push_back(newEqp);
			}
			break;

			case enEquipmentType::Eqp_Returner:
			{
				CEqpReturner* newEqp = new CEqpReturner();
				*newEqp = IN_pConfig_Line->GetAt(nIdx);
				newEqp->Set_EqpOrder(nIdx);
				m_EqpList.push_back(newEqp);
			}
			break;
			
			default: // Tester
			{
				CEqpTester* newEqp = new CEqpTester();				
				*newEqp = IN_pConfig_Line->GetAt(nIdx);
				newEqp->Set_EqpOrder(nIdx);
				newEqp->Update_DataSize(); // Port ���� ���� (3 Para �˻� ���� ����)
				m_EqpList.push_back(newEqp);
			}
			break;
			}
		}// End of for()

		// ������ ���� ������ ���� ù��° ����
		m_TestOrder.clear();
		for (auto nEqpTypeIdx = 0; nEqpTypeIdx < Max_EqpTypeCount; nEqpTypeIdx++)
		{
			// �ʱ�ȭ		
			m_pFstEqp_EqpType[nEqpTypeIdx] = nullptr;
		}// End of for()

		//�˻� �׷��� ù��° ���� ����
		for (auto nEqpIdx = 0; nEqpIdx < nEqpCnt; ++nEqpIdx)
		{
			if (nullptr != m_pFstEqp_EqpType[m_EqpList.at(nEqpIdx)->Get_EquipmentType()])
			{
				m_pFstEqp_EqpType[m_EqpList.at(nEqpIdx)->Get_EquipmentType()] = m_EqpList.at(nEqpIdx);
			}
		}

		// �˻� ���� ����
		for (auto nEqpIdx = 0; nEqpIdx < nEqpCnt; ++nEqpIdx)
		{
			// ��ϵ��� ���� �˻� �Ǻ�
			bool bFind = false;
			for (auto nOrderIdx = 0; nOrderIdx < m_TestOrder.size(); ++nOrderIdx)
			{
				if (m_TestOrder.at(nOrderIdx) == m_EqpList.at(nEqpIdx)->Get_EquipmentType())
				{
					bFind = true;
					break;
				}
			}

			if (false == bFind)
			{
				// Test ���� ����
				if (m_EqpList.at(nEqpIdx)->Is_Tester())
				{
					m_TestOrder.push_back(m_EqpList.at(nEqpIdx)->Get_EquipmentType());
				}
			}
		}
#ifdef 	NEW_INSPECTION_TYPE_APPLY
		for (auto nEqpIdx = 0; nEqpIdx < nEqpCnt; ++nEqpIdx)
		{
			// ��ϵ��� ���� �˻� �Ǻ�
			bool bFind = false;
			for (auto nOrderIdx = 0; nOrderIdx < m_InspectionOrder.size(); ++nOrderIdx)
			{
				if (m_InspectionOrder.at(nOrderIdx) == m_EqpList.at(nEqpIdx)->Get_InspectionType())
				{
					bFind = true;
					break;
				}
			}

			if (false == bFind)
			{
				// Test ���� ����
				if (m_EqpList.at(nEqpIdx)->Is_Tester())
				{
					m_InspectionOrder.push_back(m_EqpList.at(nEqpIdx)->Get_InspectionType());
				}
			}
		}
#endif	// NEW_INSPECTION_TYPE_APPLY

		// ���� ������ ���� ���� �׷� ó��
		for (auto nEqpTypeIdx = 0; nEqpTypeIdx < Max_EqpTypeCount; nEqpTypeIdx++)
		{
			// �ʱ�ȭ			
			m_pEquipment_Group[nEqpTypeIdx].clear();

			// ���� ������ ù��° ���� ã��
			for (auto nEqpIdx = 0; nEqpIdx < nEqpCnt; ++nEqpIdx)
			{
				if (m_EqpList.at(nEqpIdx)->Get_EquipmentType() == nEqpTypeIdx)
				{
					m_pEquipment_Group[nEqpTypeIdx].push_back(m_EqpList.at(nEqpIdx));
				}
			}
		}// End of for()

#ifdef 	NEW_INSPECTION_TYPE_APPLY
		for (auto nInspTypeIdx = 0; nInspTypeIdx < Max_EqpTypeCount; nInspTypeIdx++)
		{
			// 초기??			m_pInspection_Group[nInspTypeIdx].clear();

			// ?�비 ?�형??첫번�??�비 찾기
			for (auto nEqpIdx = 0; nEqpIdx < nEqpCnt; ++nEqpIdx)
			{
				if (m_EqpList.at(nEqpIdx)->Get_InspectionType() == nInspTypeIdx)
				{
					m_pInspection_Group[nInspTypeIdx].push_back(m_EqpList.at(nEqpIdx));
				}
			}
		}// End of for()
#endif	// NEW_INSPECTION_TYPE_APPLY


	}// End of if()
}

//=============================================================================
// Method		: Set_Config_Model
// Access		: public  
// Returns		: void
// Parameter	: __in const CConfig_Model * IN_pConfig_Model
// Qualifier	:
// Last Update	: 2022/1/14 - 16:40
// Desc.		:
//=============================================================================
void CLineInfo::Set_Config_Model(__in const CConfig_Model* IN_pConfig_Model)
{
	CConfig_Model::operator=(*IN_pConfig_Model);
	
	size_t nEqpCnt = m_EqpList.size();

	for (auto nIdx = 0; nIdx < nEqpCnt; ++nIdx)
	{
		if (m_EqpList.at(nIdx)->Is_Tester())
		{
			m_EqpList.at(nIdx)->Set_EnableEquipment(IN_pConfig_Model->m_bEnable_TestType[m_EqpList.at(nIdx)->Get_TesterType()]);
		}
	}
}

//=============================================================================
// Method		: Set_Path
// Access		: public  
// Returns		: void
// Parameter	: LPCTSTR IN_szLog
// Parameter	: LPCTSTR IN_szReport
// Qualifier	:
// Last Update	: 2022/4/20 - 17:16
// Desc.		:
//=============================================================================
void CLineInfo::Set_Path(LPCTSTR IN_szLog, LPCTSTR IN_szReport)
{
	for (auto nIdx = 0; nIdx < m_EqpList.size(); ++nIdx)
	{
		m_EqpList.at(nIdx)->Set_Path(IN_szLog, IN_szReport);
	}
}

//=============================================================================
// Method		: SetPtr_DebugMode
// Access		: public  
// Returns		: void
// Parameter	: const ST_DebugInfo * IN_pDebugInfo
// Qualifier	:
// Last Update	: 2022/4/20 - 17:16
// Desc.		:
//=============================================================================
void CLineInfo::SetPtr_DebugMode(const ST_DebugInfo * IN_pDebugInfo)
{
	m_pDebugInfo = IN_pDebugInfo;
}

//=============================================================================
// Method		: Get_OperateMode
// Access		: public  
// Returns		: uint8_t
// Qualifier	: const
// Last Update	: 2022/1/14 - 17:20
// Desc.		:
//=============================================================================
uint8_t CLineInfo::Get_OperateMode() const
{
	return m_OperateMode;
}

void CLineInfo::Set_OperateMode(__in uint8_t IN_nOperateMode)
{
	m_OperateMode = IN_nOperateMode;
}

//=============================================================================
// Method		: IsEndOfProduction
// Access		: public  
// Returns		: bool
// Parameter	: __in uint8_t IN_nEqpOrder
// Qualifier	:
// Last Update	: 2022/1/14 - 17:20
// Desc.		:
//=============================================================================
bool CLineInfo::IsEndOfProduction(__in uint8_t IN_nEqpOrder)
{
	// ���� ������ ���� ������� ���񳻿� ������ ��� �ֳ� �Ǵ��Ѵ�. (Loader ����)
	size_t nEqpCnt = m_EqpList.size();

	if (IN_nEqpOrder < nEqpCnt)
	{
		for (auto nIdx = 0; nIdx < IN_nEqpOrder; ++nIdx)
		{
			// �δ� ����, �˻� ���� üũ
			//if (m_EqpList.at(nIdx)->Is_Tester())
			{
				if (false == m_EqpList.at(nIdx)->IsEmpty_Equipment())
				{
					return false;
				}
			}
		}
	}
	else
	{
		return false;
	}

	return true;
}

//=============================================================================
// Method		: Get_EndOfProduction
// Access		: public  
// Returns		: bool
// Qualifier	: const
// Last Update	: 2022/1/14 - 17:20
// Desc.		:
//=============================================================================
bool CLineInfo::Get_EndOfProduction() const
{
	return m_bEndOfProduction;
}

void CLineInfo::Set_EndOfProduction(__in bool IN_bEnd)
{
	m_bEndOfProduction = IN_bEnd;
}

//=============================================================================
// Method		: Get_Language
// Access		: public  
// Returns		: uint8_t
// Qualifier	: const
// Last Update	: 2022/1/14 - 17:20
// Desc.		:
//=============================================================================
//uint8_t CLineInfo::Get_Language() const
//{
//	return m_nLanguage;
//}
//
//void CLineInfo::Set_Language(__in uint8_t IN_nLanguage)
//{
//	m_nLanguage = IN_nLanguage;
//}

//=============================================================================
// Method		: Get_TargetEquipmentType
// Access		: public  
// Returns		: uint8_t							: ���� ����
// Parameter	: __in enEquipmentType IN_nEqpType	: ���� ����
// Qualifier	:
// Last Update	: 2022/2/3 - 15:35
// Desc.		:
//=============================================================================
uint8_t CLineInfo::Get_NextEquipmentType(__in enEquipmentType IN_nEqpType)
{
	// ���� �˻� üũ
	uint8_t nNextTestOrder = 0;
	
	if (Eqp_Loader == IN_nEqpType) // Loader �̸�
	{
		if (0 < m_TestOrder.size())
		{
			nNextTestOrder = 0;
		}
		else
		{
			return Eqp_Returner;
		}
	}	
	else if (IsTester_byEqpType(IN_nEqpType)) // Tester �̸�
	{
		// ���� �˻��� ���� Ȯ��
		bool bFind = false;
		for (auto nIdx = 0; nIdx < m_TestOrder.size(); ++nIdx)
		{
			if (IN_nEqpType == m_TestOrder.at(nIdx))
			{
				// ���� �˻簡 �ִ�.
				if (nIdx + 1 < m_TestOrder.size())
				{
					nNextTestOrder = nIdx + 1;
					bFind = true;
				}
				else // ���� �˻簡 ����. ���ͳʷ� �̵�
				{
					return Eqp_Returner;
				}

				break;
			}
		}// end of for()

		// �˻� ���� ����Ʈ�� ����.
		if (false == bFind)
		{ 
			return Eqp_Returner;
		}
	}
	else // Returner & Handler
	{
		return Eqp_Returner;
	}

	// ���� �˻��� ���� ���� ã��
	uint8_t nTestType = 0;
	uint8_t nReturnEqpType = 0;
	for (auto nTestOrder = nNextTestOrder; nTestOrder < m_TestOrder.size(); ++nTestOrder)
	{
		// �˻簡 Ȱ��ȭ �Ǿ� �ִ� ��츸...
		nTestType = ConvTo_TesterType((enEquipmentType)m_TestOrder.at(nTestOrder));
		if (m_bEnable_TestType[nTestType])
		{			
			nReturnEqpType = m_TestOrder.at(nTestOrder);

			// �ش� �˻翡 �ش��ϴ� ������� Skip ���� Ȯ��
			if (false == IsSkip_EquipmentType((enEquipmentType)nReturnEqpType))
			{
				return nReturnEqpType;
			}
		}
	}// end of for()

	return Eqp_Returner;
}

uint8_t CLineInfo::Get_NextEquipmentType(__in uint8_t IN_FromEqp)
{
	return (m_EqpList.at(Get_NextEquipment(IN_FromEqp)))->Get_EquipmentType();
}

//=============================================================================
// Method		: Get_PrevEquipmentType
// Access		: public  
// Returns		: uint8_t
// Parameter	: __in enEquipmentType IN_nEqpType
// Qualifier	:
// Last Update	: 2022/2/7 - 10:51
// Desc.		:
//=============================================================================
uint8_t CLineInfo::Get_PrevEquipmentType(__in enEquipmentType IN_nEqpType)
{
	// ���� �˻� üũ
	uint8_t nPrevTestOrder = 0;

	if (Eqp_Returner == IN_nEqpType) // Loader �̸�
	{
		if (0 < m_TestOrder.size())
		{
			nPrevTestOrder = 0;
		}
		else
		{
			return Eqp_Loader;
		}
	}
	else if (IsTester_byEqpType(IN_nEqpType)) // Tester �̸�
	{
		// ���� �˻��� ���� Ȯ��
		bool bFind = false;
		for (auto nIdx = (m_TestOrder.size() - 1); 0 <= nIdx; --nIdx)
		{
			if (IN_nEqpType == m_TestOrder.at(nIdx))
			{
				// ���� �˻簡 �ִ�.
				if (0 <= (nIdx - 1))
				{
					nPrevTestOrder = static_cast<uint8_t>(nIdx - 1);
					bFind = true;
				}
				else // ���� �˻簡 ����. �δ��� �̵�
				{
					return Eqp_Loader;
				}

				break;
			}
		}// end of for()

		// �˻� ���� ����Ʈ�� ����.
		if (false == bFind)
		{
			return Eqp_Loader;
		}
	}
	else // Returner or Handler
	{
		return Eqp_Loader;
	}

	// ���� �˻��� ���� ���� ã��
	uint8_t nTestType = 0;
	uint8_t nReturnEqpType = 0;
	for (auto nTestOrder = nPrevTestOrder; 0 <= nTestOrder; --nTestOrder)
	{
		// �˻簡 Ȱ��ȭ �Ǿ� �ִ� ��츸...
		nTestType = ConvTo_TesterType((enEquipmentType)m_TestOrder.at(nTestOrder));
		if (m_bEnable_TestType[nTestType])
		{
			nReturnEqpType = m_TestOrder.at(nTestOrder);

			// �ش� �˻翡 �ش��ϴ� ������� Skip ���� Ȯ��
			if (false == IsSkip_EquipmentType((enEquipmentType)nReturnEqpType))
			{
				return nReturnEqpType;
			}
		}
	}// end of for()

	return Eqp_Loader;
}

uint8_t CLineInfo::Get_PrevEquipmentType(__in uint8_t IN_FromEqp)
{
	return m_EqpList.at(Get_PrevEquipment(IN_FromEqp))->Get_EquipmentType();
}

#ifdef 	NEW_INSPECTION_TYPE_APPLY
//=============================================================================
// Method		: Get_NextInspectionType
// Access		: public  
// Returns		: uint8_t
// Parameter	: __in enInspectionType IN_nInspType
// Parameter	: __in bool bLoader
// Qualifier	:
// Last Update	: 2022/10/21 - 10:33
// Desc.		:
//=============================================================================
uint8_t CLineInfo::Get_NextInspectionType(__in enInspectionType IN_nInspType, __in bool bLoader /*= false*/)
{
	// ���� �˻� üũ
	uint8_t nNextInspectionOrder = 0;

	if ((InspType_NotInspection == IN_nInspType) && (bReturner)) // Returner �̸�
	{
		if (0 < m_InspectionOrder.size())
		{
			nNextInspectionOrder = 0;
		}
		else
		{
			return InspType_NotInspection;
		}
	}
	else if (InspType_NotInspection < IN_nInspType)
	{
		// ���� �˻��� ���� Ȯ��
		bool bFind = false;
		for (auto nIdx = 0; nIdx < m_InspectionOrder.size(); ++nIdx)
		{
			if (IN_nInspType == m_InspectionOrder.at(nIdx))
			{
				// ���� �˻簡 �ִ�.
				if (nIdx + 1 < m_InspectionOrder.size())
				{
					nNextInspectionOrder = nIdx + 1;
					bFind = true;
				}
				else // ���� �˻簡 ����. �δ��� �̵�
				{
					return InspType_NotInspection;
				}

				break;
			}
		}// end of for()

		// �˻� ���� ����Ʈ�� ����.
		if (false == bFind)
		{
			return InspType_NotInspection;
		}
	}
	else // Returner
	{
		return InspType_NotInspection;
	}

	// ���� �˻��� ���� ���� ã��
	uint8_t nReturnInspType = 0;
	for (auto nInspOrder = nNextInspectionOrder; nInspOrder < m_InspectionOrder.size(); ++nInspOrder)
	{
		// �˻簡 Ȱ��ȭ �Ǿ� �ִ� ��츸...
		if (m_bEnable_InspType[m_InspectionOrder.at(nInspOrder)])
		{
			nReturnInspType = m_InspectionOrder.at(nInspOrder);

			// �ش� �˻翡 �ش��ϴ� ������� Skip ���� Ȯ��
			if (false == IsSkip_InspectionType((enInspectionType)nReturnInspType))
			{
				return nReturnInspType;
			}
		}
	}// end of for()

	return InspType_NotInspection;
}

uint8_t CLineInfo::Get_NextInspectionType(__in uint8_t IN_FromEqp)
{
	return (m_EqpList.at(Get_NextEquipment(IN_FromEqp)))->Get_InspectionType();
}

//=============================================================================
// Method		: Get_PrevInspectionType
// Access		: public  
// Returns		: uint8_t
// Parameter	: __in enInspectionType IN_nInspType
// Qualifier	:
// Last Update	: 2022/10/21 - 10:38
// Desc.		:
//=============================================================================
uint8_t CLineInfo::Get_PrevInspectionType(__in enInspectionType IN_nInspType, __in bool bReturner /*= false*/)
{
	// ?�전 검??체크
	uint8_t nPrevInspectionOrder = 0;

	if ((InspType_NotInspection == IN_nInspType) && (bReturner)) // Returner ?�면
	{
		if (0 < m_InspectionOrder.size())
		{
			nPrevInspectionOrder = 0;
		}
		else
		{
			return Eqp_Loader;
		}
	}
	else if (InspType_NotInspection < IN_nInspType)
	{
		// ?�재 검?�의 ?�서 ?�인
		bool bFind = false;
		for (auto nIdx = (m_InspectionOrder.size() - 1); 0 <= nIdx; --nIdx)
		{
			if (IN_nInspType == m_InspectionOrder.at(nIdx))
			{
				// ?�전 검?��? ?�다.
				if (0 <= (nIdx - 1))
				{
					nPrevInspectionOrder = static_cast<uint8_t>(nIdx - 1);
					bFind = true;
				}
				else // ?�전 검?��? ?�다. 로더�??�동
				{
					return InspType_NotInspection;
				}

				break;
			}
		}// end of for()

		// 검???�서 리스?�에 ?�다.
		if (false == bFind)
		{
			return InspType_NotInspection;
		}
	}
	else // Returner
	{
		return InspType_NotInspection;
	}

	// ?�음 검?�의 ?�비 ?�형 찾기
	uint8_t nReturnInspType = 0;
	for (auto nInspOrder = nPrevInspectionOrder; 0 <= nInspOrder; --nInspOrder)
	{
		// 검?��? ?�성???�어 ?�는 경우�?..
		if (m_bEnable_InspType[m_InspectionOrder.at(nInspOrder)])
		{
			nReturnInspType = m_InspectionOrder.at(nInspOrder);

			// ?�당 검?�에 ?�당?�는 ?�비?�의 Skip ?�태 ?�인
			if (false == IsSkip_InspectionType((enInspectionType)nReturnInspType))
			{
				return nReturnInspType;
			}
		}
	}// end of for()

	return InspType_NotInspection;
}

uint8_t CLineInfo::Get_PrevInspectionType(__in uint8_t IN_FromEqp)
{
	return m_EqpList.at(Get_PrevEquipment(IN_FromEqp))->Get_InspectionType();
}
#endif	// NEW_INSPECTION_TYPE_APPLY

//=============================================================================
// Method		: Get_TargetEquipment_inGroup
// Access		: public  
// Returns		: uint8_t					: 0 -> Error 
// Parameter	: __in enEquipmentType IN_nEqpType
// Qualifier	:
// Last Update	: 2023/4/7 - 15:27
// Desc.		:
//=============================================================================
uint8_t CLineInfo::Get_TargetEquipment_inGroup(__in enEquipmentType IN_nEqpType)
{
	// �׷쳻�� ���� ���� ���ϱ�
	if (m_pEquipment_Group[IN_nEqpType].empty())
	{
		// ������ ���� ����. (Error ��Ȳ )
		return 0;
	}

	// �������� ���ؾ� �Ѵ�... (�Ÿ��� �� ������� ����)
	uint32_t	nCheckTime			= 0;	// �ð� üũ��
	uint32_t	nElapTime			= 0;	// �˻� ��Ʈ�� ���ԵǾ� ����� �ð�
	uint8_t		nTargetEqpOrder		= 0;	// ��� ���� ����
	uint8_t		nEmptyPortCount		= 0;	// ���� ����ִ� ��Ʈ
	uint8_t		nMaxEmptyPortCount	= 0;	// 
	//-------------------------------------------------------------------------
	// 1. ��Ʈ�� ��� �ִ� ���� �켱���� ����
	//-------------------------------------------------------------------------
	for (auto Iter = m_pEquipment_Group[IN_nEqpType].rbegin(); Iter != m_pEquipment_Group[IN_nEqpType].rend(); ++Iter)
	{
		// ��� Skip�̸� error;
		if ((*Iter)->Get_Skip())
		{
			continue;
		}

		// ������ ����ִ� ��Ʈ ���ϱ�
		nEmptyPortCount = (*Iter)->Get_EmptyPortCount();

		// ����ִ� ��Ʈ�� �ִ�ġ�̸�, �ٷ� eqp�� �����Ѵ�.
		if ((*Iter)->Get_ReservablePortCount() == nEmptyPortCount)
		{
			TRACE(_T("(%s) : Max Empty Port => target: %d\n"), _T(__FUNCTION__), (*Iter)->Get_EqpOrder());
			CString szText;
			szText.Format(_T("(%s) : Max Empty Port => target: %d"), _T(__FUNCTION__), (*Iter)->Get_EqpOrder());			
			AfxGetApp()->GetMainWnd()->SendMessage(WM_LOGMSG, (WPARAM)szText.GetBuffer(), MAKELPARAM(LOGTYPE_NONE, 0));

			return (*Iter)->Get_EqpOrder();
		}

		if (nMaxEmptyPortCount < nEmptyPortCount)
		{
			nMaxEmptyPortCount = nEmptyPortCount;
			nTargetEqpOrder = (*Iter)->Get_EqpOrder();
		}
	}

	//-------------------------------------------------------------------------
	// 2. 3���� ��Ʈ�� ���/���� ���¸� üũ�Ͽ� ���/���� ������ ���� ���� ����
	//-------------------------------------------------------------------------
	if (0 < nMaxEmptyPortCount)
	{
		// ��� ���� �ĺ���
		std::vector<CEquipment*>	vTargets;
		for (auto Iter = m_pEquipment_Group[IN_nEqpType].rbegin(); Iter != m_pEquipment_Group[IN_nEqpType].rend(); ++Iter)
		{
			if ((*Iter)->Get_Skip())
			{
				continue;
			}
			if ((*Iter)->Get_EmptyPortCount() == nMaxEmptyPortCount)
			{
				vTargets.push_back(*Iter);
			}
		}
		//-------------------------------------------------------------------------
		// ��Ʈ ����ִ� ������ �����ϸ�, ���� �ð��� ���Ͽ� ������ ���� ����
		//-------------------------------------------------------------------------
		if (1 < vTargets.size())
		{
			for (auto Iter = vTargets.rbegin(); Iter != vTargets.rend(); ++Iter)
			{
				// ��� Skip�̸� error;
				if ((*Iter)->Get_Skip())
				{
					continue;
				}

				// ���� �ð��� ���� ������ ������ �ִ� ���� üũ
				uint8_t nTestParaCount = (*Iter)->Get_TestPortCount();
				for (auto nParaIdx = 0; nParaIdx < nTestParaCount; ++nParaIdx)
				{
					// ���� Test Para�� ������ �־�� �Ѵ�. (2023.04.10)
					switch ((*Iter)->Get_PortStatus_byTestPara(nParaIdx).nStatus)
					{
					case enPortStatus::PtS_Exist_Socket:
					case enPortStatus::PtS_Wait_Out:
					{
						nElapTime = (*Iter)->Get_ElapsedTime_InputPara(nParaIdx);
						if (nCheckTime < nElapTime)
						{
							nCheckTime = nElapTime;
							nTargetEqpOrder = (*Iter)->Get_EqpOrder();
						}
					}
					break;

					default:
						break;

					}

					// 					nElapTime = (*Iter)->Get_ElapsedTime_InputPara(nParaIdx);
					// 					if (nCheckTime < nElapTime)
					// 					{
					// 						nCheckTime = nElapTime;
					// 						nTargetEqpOrder = (*Iter)->Get_EqpOrder();
					// 					}
				} // End of for () �˻� �Ķ�
			} // End of for() ����

			if (0 < nCheckTime)
			{
				TRACE(_T("(%s) : Check Time => target: %d, ElapTime: %d\n"), _T(__FUNCTION__), nTargetEqpOrder, nCheckTime);
				CString szText;
				szText.Format(_T("(%s) : Check Time => target: %d, ElapTime: %d"), _T(__FUNCTION__), nTargetEqpOrder, nCheckTime);
				AfxGetApp()->GetMainWnd()->SendMessage(WM_LOGMSG, (WPARAM)szText.GetBuffer(), MAKELPARAM(LOGTYPE_NONE, 0));

				return nTargetEqpOrder;
			}
		}
		else if (1 == vTargets.size())
		{
			// �ĺ� ���� ������ 1���̸� �ٷ� �����Ѵ�.
			TRACE(_T("(%s) : 1 equipment can be reserved => target: %d\n"), _T(__FUNCTION__), nTargetEqpOrder);
			CString szText;
			szText.Format(_T("(%s) : 1 equpment can be reserved => target: %d"), _T(__FUNCTION__), nTargetEqpOrder);
			AfxGetApp()->GetMainWnd()->SendMessage(WM_LOGMSG, (WPARAM)szText.GetBuffer(), MAKELPARAM(LOGTYPE_NONE, 0));

			return nTargetEqpOrder;
		}
		else
		{
			TRACE(_T("(%s) : Compare Empty Port => target: %d\n"), _T(__FUNCTION__), nTargetEqpOrder);
			CString szText;
			szText.Format(_T("(%s) : Compare Empty Port => target: %d"), _T(__FUNCTION__), nTargetEqpOrder);
			AfxGetApp()->GetMainWnd()->SendMessage(WM_LOGMSG, (WPARAM)szText.GetBuffer(), MAKELPARAM(LOGTYPE_NONE, 0));

			return nTargetEqpOrder;
		}
	}

	//-------------------------------------------------------------------------
	// 3. 3���� ��Ʈ�� ��� ����ϴ� ���
	//-------------------------------------------------------------------------
#ifdef USE_SET_TARGET_EQP_GROUP_TRACK_IN
	// ����� ����� ������.
	//nTargetEqpOrder = (*m_pEquipment_Group[IN_nEqpType].begin())->Get_EqpOrder();
	for (auto Iter = m_pEquipment_Group[IN_nEqpType].begin(); Iter != m_pEquipment_Group[IN_nEqpType].end(); ++Iter)
	{
		// ��� Skip�̸� error;
		if (false == (*Iter)->Get_Skip())
		{
			nTargetEqpOrder = (*Iter)->Get_EqpOrder();
			break;
		}
	}

	TRACE(_T("(%s) : Near Equipment\n"), _T(__FUNCTION__), nTargetEqpOrder);
	CString szText;
	szText.Format(_T("(%s) : Near Equipment => target: %d"), _T(__FUNCTION__), nTargetEqpOrder);
	AfxGetApp()->GetMainWnd()->SendMessage(WM_LOGMSG, (WPARAM)szText.GetBuffer(), MAKELPARAM(LOGTYPE_NONE, 0));

#else
	// ���� ������ ��Ʈ�� ��� ������� ��� m_nReservedOvered�� ����Ͽ� �Ǵ��Ѵ�.
	nTargetEqpOrder = Get_TargetEquipment_inGroup_Over(IN_nEqpType);

	TRACE(_T("(%s) : Check OverReserved\n"), _T(__FUNCTION__), nTargetEqpOrder);
	CString szText;
	szText.Format(_T("(%s) : Check OverReserved => target: %d"), _T(__FUNCTION__), nTargetEqpOrder);
	AfxGetApp()->GetMainWnd()->SendMessage(WM_LOGMSG, (WPARAM)szText.GetBuffer(), MAKELPARAM(LOGTYPE_NONE, 0));
#endif

	//return Get_TargetEquipment_inGroup_Over(IN_nEqpType);
	return nTargetEqpOrder;
}

#ifdef 	NEW_INSPECTION_TYPE_APPLY
uint8_t CLineInfo::Get_TargetEquipment_inGroup(__in enInspectionType IN_nInspType)
{
	// �׷쳻�� ���� ���� ���ϱ�
	if (m_pInspection_Group[IN_nInspType].empty())
	{
		// ������ ���� ����. (Error ��Ȳ )
		return 0;
	}

	// �������� ���ؾ� �Ѵ�...
	uint32_t nCheckTime = 0;
	uint32_t nElapTime = 0;
	uint8_t nTargetEqpOrder = 0;
	uint8_t nEmptyPortCount = 0;
	uint8_t nMaxEmptyPortCount = 0;	
	//-------------------------------------------------------------------------
	// 1. ��Ʈ�� ��� �ִ� ���� �켱���� ����
	//-------------------------------------------------------------------------
	for (auto Iter = m_pInspection_Group[IN_nInspType].rbegin(); Iter != m_pInspection_Group[IN_nInspType].rend(); ++Iter)
	{
		// ��� Skip�̸� error;
		if ((*Iter)->Get_Skip())
		{
			continue;
		}

		nEmptyPortCount = (*Iter)->Get_EmptyPortCount();
		// ����ִ� ��Ʈ�� �ִ�ġ�̸�, �ٷ� eqp�� �����Ѵ�.
		if ((*Iter)->Get_ReservablePortCount() == nEmptyPortCount)
		{
			TRACE(_T("(%s) : Max Empty Port\n"), _T(__FUNCTION__));
			return (*Iter)->Get_EqpOrder();
		}

		if (nMaxEmptyPortCount < nEmptyPortCount)
		{
			nMaxEmptyPortCount = nEmptyPortCount;
			nTargetEqpOrder = (*Iter)->Get_EqpOrder();
		}
	}

	//-------------------------------------------------------------------------
	// 2. 3���� ��Ʈ�� ���/���� ���¸� üũ�Ͽ� ���/���� ������ ���� ���� ����
	//-------------------------------------------------------------------------
	if (0 < nMaxEmptyPortCount)
	{
		// ��� ���� �ĺ���	
		std::vector<CEquipment*>	vTargets;
		for (auto Iter = m_pInspection_Group[IN_nInspType].rbegin(); Iter != m_pInspection_Group[IN_nInspType].rend(); ++Iter)
		{
			if ((*Iter)->Get_EmptyPortCount() == nMaxEmptyPortCount)
			{
				vTargets.push_back(*Iter);
			}
		}

		//-------------------------------------------------------------------------
		// ��Ʈ ����ִ� ������ �����ϸ�, ���� �ð��� ���Ͽ� ������ ���� ����
		//-------------------------------------------------------------------------
		if (1 < vTargets.size())
		{
			for (auto Iter = vTargets.rbegin(); Iter != vTargets.rend(); ++Iter)
			{
				// ��� Skip�̸� error;
				if ((*Iter)->Get_Skip())
				{
					continue;
				}

				// ���� �ð��� ���� ������ ������ �ִ� ���� üũ
				uint8_t nTestParaCount = (*Iter)->Get_TestPortCount();
				for (auto nIdx = 0; nIdx < nTestParaCount; ++nIdx)
				{
					nElapTime = (*Iter)->Get_ElapsedTime_InputPara(nIdx);
					if (nCheckTime < nElapTime)
					{
						nCheckTime = nElapTime;
						nTargetEqpOrder = (*Iter)->Get_EqpOrder();
					}
				} // End of for () �˻� �Ķ�
			} // End of for() ����

			if (0 < nCheckTime)
			{
				TRACE(_T("(%s) : Check Time\n"), _T(__FUNCTION__));
				return nTargetEqpOrder;
			}
		}
		else
		{
			TRACE(_T("(%s) : Compare Empty Port\n"), _T(__FUNCTION__));
			return nTargetEqpOrder;
		}
	}

	//-------------------------------------------------------------------------
	// 3. 3���� ��Ʈ�� ��� ����ϴ� ���
	//-------------------------------------------------------------------------
	// ���� ������ ��Ʈ�� ��� ������� ��� m_nReservedOvered�� ����Ͽ� �Ǵ��Ѵ�.
	TRACE(_T("(%s) : Check OverReserved\n"), _T(__FUNCTION__));
	return Get_TargetEquipment_inGroup_Over(IN_nInspType);

}
#endif	// NEW_INSPECTION_TYPE_APPLY

//=============================================================================
// Method		: Get_TargetEquipment_inGroup_Over
// Access		: public  
// Returns		: uint8_t
// Parameter	: __in enEquipmentType IN_nEqpType
// Qualifier	:
// Last Update	: 2022/5/20 - 13:30
// Desc.		:
//=============================================================================
uint8_t CLineInfo::Get_TargetEquipment_inGroup_Over(__in enEquipmentType IN_nEqpType)
{
	uint32_t nCheckTime = 0;
	uint32_t nElapTime = 0;
	uint8_t nTargetEqpOrder = 0;

	// ���� ������ ��Ʈ�� ��� ������� ��� m_nReservedOvered�� ����Ͽ� �Ǵ��Ѵ�.
	uint8_t nOverCount = 0;
	uint8_t nMinOverCount = 0xFF;
	for (auto Iter = m_pEquipment_Group[IN_nEqpType].rbegin(); Iter != m_pEquipment_Group[IN_nEqpType].rend(); ++Iter)
	{
		// ��� Skip�̸� error;
		if ((*Iter)->Get_Skip())
		{
			continue;
		}

		// m_nReservedOvered ī��Ʈ�� ���� ���� ���� ����
		nOverCount = (*Iter)->Get_ReservedOverCnt();
		if (nOverCount < nMinOverCount)
		{
			nMinOverCount = nOverCount;
			nTargetEqpOrder = (*Iter)->Get_EqpOrder();
		}
	}

	std::vector<CEquipment*>	vTargets;
	for (auto Iter = m_pEquipment_Group[IN_nEqpType].rbegin(); Iter != m_pEquipment_Group[IN_nEqpType].rend(); ++Iter)
	{
		if ((*Iter)->Get_ReservedOverCnt() == nMinOverCount)
		{
			vTargets.push_back(*Iter);
		}
	}

	//-------------------------------------------------------------------------
	// ��Ʈ ����ִ� ������ �����ϸ�, ���� �ð��� ���Ͽ� ������ ���� ����
	//-------------------------------------------------------------------------
	if (1 < vTargets.size())
	{
		for (auto Iter = vTargets.rbegin(); Iter != vTargets.rend(); ++Iter)
		{
			// ��� Skip�̸� error;
			if ((*Iter)->Get_Skip())
			{
				continue;
			}

			// ���� �ð��� ���� ������ ������ �ִ� ���� üũ
			uint8_t nTestParaCount = (*Iter)->Get_TestPortCount();
			for (auto nIdx = 0; nIdx < nTestParaCount; ++nIdx)
			{
				nElapTime = (*Iter)->Get_ElapsedTime_InputPara(nIdx);
				if (nCheckTime < nElapTime)
				{
					nCheckTime = nElapTime;
					nTargetEqpOrder = (*Iter)->Get_EqpOrder();
				}
			}
		}

		if (0 < nCheckTime)
		{
			return nTargetEqpOrder;
		}
	}
	else
	{
		return nTargetEqpOrder;
	}

	return (*m_pEquipment_Group[IN_nEqpType].rbegin())->Get_EqpOrder();
}

#ifdef 	NEW_INSPECTION_TYPE_APPLY
uint8_t CLineInfo::Get_TargetEquipment_inGroup_Over(__in enInspectionType IN_nInspType)
{
	return 0;
}
#endif	// NEW_INSPECTION_TYPE_APPLY

//=============================================================================
// Method		: Get_TargetEquipment
// Access		: public  
// Returns		: uint8_t					: 0 -> Error
// Parameter	: __in uint8_t IN_FromEqp
// Qualifier	:
// Last Update	: 2022/2/3 - 17:02
// Desc.		:
//=============================================================================
uint8_t CLineInfo::Get_TargetEquipment(__in uint8_t IN_FromEqp)
{
	// ���� �˻��� Ȱ��ȭ üũ, ���� �˻系 ������� Skip üũ
	uint8_t nTargetEqpType = Get_NextEquipmentType((enEquipmentType)m_EqpList.at(IN_FromEqp)->Get_EquipmentType());

	// ���� ������ Returner �̸�
	if (Eqp_Returner == nTargetEqpType)
	{
		return Get_Returner().Get_EqpOrder();
	}
	
	// �˻� ������ �׷� ����
	if (Get_FirstEquipment((enEquipmentType)nTargetEqpType).Get_Group())
	{
		// �˻� ���� �׷��� ���
		return Get_TargetEquipment_inGroup((enEquipmentType)nTargetEqpType);
	}
	else
	{
		return Get_FirstEquipment((enEquipmentType)nTargetEqpType).Get_EqpOrder();
	}

}

//=============================================================================
// Method		: Get_FindEqpz_SameTarget
// Access		: public  
// Returns		: uint8_t									=> �˻��� ���� ����
// Parameter	: __in uint16_t IN_nTargetEqp				=> ���� ����
// Parameter	: __out std::vector<uint8_t> & OUT_nEqpz	=> �˻��� �����
// Qualifier	:
// Last Update	: 2022/2/17 - 20:57
// Desc.		:
//=============================================================================
uint8_t CLineInfo::Get_FindEqpz_SameTarget(__in uint8_t IN_nTargetEqp, __out std::vector<uint16_t>& OUT_nEqpz)
{
	// 0���� Loader ����, 2��° ���� ���Ŀ��� �� ���
	if (1 < IN_nTargetEqp)
	{
		uint8_t nCount = 0;

		bool bWaitOut = false;
		for (uint8_t nPrevEqp = IN_nTargetEqp - 1; 1 < nPrevEqp; --nPrevEqp)
		{
			bWaitOut = false;
			for (uint8_t nPortIdx = enPortIndex_Tester::PtI_T_Test_L; nPortIdx < GetAt(nPrevEqp).Get_PortCount(); ++nPortIdx)
			{
				if (enPortStatus::PtS_Wait_Out == GetAt(nPrevEqp).Get_PortStatus(nPortIdx).nStatus)
				{
					bWaitOut = true;
					break;
				}
			}

			/*if ((enPortStatus::PtS_Wait_Out == GetAt(nPrevEqp).Get_PortStatus(enPortIndex_Tester::PtI_T_Test_L).nStatus) ||
				(enPortStatus::PtS_Wait_Out == GetAt(nPrevEqp).Get_PortStatus(enPortIndex_Tester::PtI_T_Test_R).nStatus))*/
			if (bWaitOut)
			{
				OUT_nEqpz.push_back(nPrevEqp);
			}
		}

		return (uint8_t)OUT_nEqpz.size();
	}

	return 0;
}

//=============================================================================
// Method		: Get_FindSocketz_SameTarget
// Access		: public  
// Returns		: uint8_t
// Parameter	: __in uint8_t IN_nTargetEqp
// Parameter	: __out std::vector<CString> & OUT_Socketz
// Qualifier	:
// Last Update	: 2022/4/20 - 17:16
// Desc.		:
//=============================================================================
uint8_t CLineInfo::Get_FindSocketz_SameTarget(__in uint8_t IN_nTargetEqp, __out std::vector<CString>& OUT_Socketz)
{
	// 0���� Loader ����, 2��° ���� ���Ŀ��� �� ���
	if (1 < IN_nTargetEqp)
	{
		bool bWaitOut = false;
		for (uint8_t nPrevEqp = IN_nTargetEqp - 1; 1 < nPrevEqp; --nPrevEqp)
		{
			// ***** RFID�� ������ Crash *****
			bWaitOut = false;
			for (uint8_t nPortIdx = enPortIndex_Tester::PtI_T_Test_L; nPortIdx < GetAt(nPrevEqp).Get_PortCount(); ++nPortIdx)
			{
				if (enPortStatus::PtS_Wait_Out == GetAt(nPrevEqp).Get_PortStatus(nPortIdx).nStatus)
				{
					if (false == GetAt(nPrevEqp).Get_PortStatus(nPortIdx).szRfid.IsEmpty())
					{
						OUT_Socketz.push_back(GetAt(nPrevEqp).Get_PortStatus(nPortIdx).szRfid);
					}
				}
			}

			/*if (enPortStatus::PtS_Wait_Out == GetAt(nPrevEqp).Get_PortStatus(enPortIndex_Tester::PtI_T_Test_L).nStatus)
			{
				if (false == GetAt(nPrevEqp).Get_PortStatus(enPortIndex_Tester::PtI_T_Test_L).szRfid.IsEmpty())
				{
					OUT_Socketz.push_back(GetAt(nPrevEqp).Get_PortStatus(enPortIndex_Tester::PtI_T_Test_L).szRfid);
				}
			}

			if (enPortStatus::PtS_Wait_Out == GetAt(nPrevEqp).Get_PortStatus(enPortIndex_Tester::PtI_T_Test_R).nStatus)
			{
				if (false == GetAt(nPrevEqp).Get_PortStatus(enPortIndex_Tester::PtI_T_Test_R).szRfid.IsEmpty())
				{
					OUT_Socketz.push_back(GetAt(nPrevEqp).Get_PortStatus(enPortIndex_Tester::PtI_T_Test_R).szRfid);
				}
			}*/
		}

		return (uint8_t)OUT_Socketz.size();
	}

	return 0;
}

//#define CSV_HEADER_EQPINFO _T("EqpID,All,Pass,NG,Left Total,Left Pass,Left NG,Right Total,Right Pass,Right NG\r\n")
#define CSV_HEADER_EQPINFO _T("EqpID,All,Pass,NG,Left Total,Left Pass,Left NG,Right Total,Right Pass,Right NG,Center Total,Center Pass,Center NG\r\n")
//=============================================================================
// Method		: Write_CSV_File
// Access		: public  
// Returns		: bool
// Parameter	: LPCTSTR IN_szPath
// Qualifier	:
// Last Update	: 2022/4/20 - 17:17
// Desc.		:
//=============================================================================
bool CLineInfo::Write_CSV_File(LPCTSTR IN_szPath)
{
	SYSTEMTIME tmLc;
	GetLocalTime(&tmLc);

	CString szFile;

	szFile.Format(_T("%s\\LineYield_%04d%02d%02d_%02d%02d%02d.csv"),
		IN_szPath,
		tmLc.wYear, tmLc.wMonth, tmLc.wDay,
		tmLc.wHour, tmLc.wMinute, tmLc.wSecond
	);

	CString szData;
	CString szLine;

	for (auto nIdx = 0; nIdx < m_EqpList.size(); ++nIdx)
	{
		if (false == m_EqpList.at(nIdx)->Is_Returner())
		{
			//if (2 == m_EqpList.at(nIdx)->Get_TestPortCount())
				/*szLine.Format(_T("%s,%d,%d,%d,%d,%d,%d,%d,%d,%d\r\n"),
					m_EqpList.at(nIdx)->Get_EquipmentId(),
					m_EqpList.at(nIdx)->Get_Yield_Day().dwTotal,
					m_EqpList.at(nIdx)->Get_Yield_Day().dwPass,
					m_EqpList.at(nIdx)->Get_Yield_Day().dwFail,
					m_EqpList.at(nIdx)->Get_Yield_Day().m_Para[Para_Left].dwTotal,
					m_EqpList.at(nIdx)->Get_Yield_Day().m_Para[Para_Left].dwPass,
					m_EqpList.at(nIdx)->Get_Yield_Day().m_Para[Para_Left].dwFail,
					m_EqpList.at(nIdx)->Get_Yield_Day().m_Para[Para_Right].dwTotal,
					m_EqpList.at(nIdx)->Get_Yield_Day().m_Para[Para_Right].dwPass,
					m_EqpList.at(nIdx)->Get_Yield_Day().m_Para[Para_Right].dwFail
				);*/
			
			{
				szLine.Format(_T("%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\r\n"),
					m_EqpList.at(nIdx)->Get_EquipmentId(),
					m_EqpList.at(nIdx)->Get_Yield_Day().dwTotal,
					m_EqpList.at(nIdx)->Get_Yield_Day().dwPass,
					m_EqpList.at(nIdx)->Get_Yield_Day().dwFail,
					m_EqpList.at(nIdx)->Get_Yield_Day().m_Para[Para_Left].dwTotal,
					m_EqpList.at(nIdx)->Get_Yield_Day().m_Para[Para_Left].dwPass,
					m_EqpList.at(nIdx)->Get_Yield_Day().m_Para[Para_Left].dwFail,
					m_EqpList.at(nIdx)->Get_Yield_Day().m_Para[Para_Right].dwTotal,
					m_EqpList.at(nIdx)->Get_Yield_Day().m_Para[Para_Right].dwPass,
					m_EqpList.at(nIdx)->Get_Yield_Day().m_Para[Para_Right].dwFail,
					m_EqpList.at(nIdx)->Get_Yield_Day().m_Para[Para_Center].dwTotal,
					m_EqpList.at(nIdx)->Get_Yield_Day().m_Para[Para_Center].dwPass,
					m_EqpList.at(nIdx)->Get_Yield_Day().m_Para[Para_Center].dwFail
				);
			}
		}

		szData += szLine;
	}

	return Write_File(szFile, szData, CSV_HEADER_EQPINFO);
}

//=============================================================================
// Method		: Reset_EquipmentYield_All
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2022/4/20 - 17:17
// Desc.		:
//=============================================================================
void CLineInfo::Reset_EquipmentYield_All()
{
	for (auto nIdx = 0; nIdx < m_EqpList.size(); ++nIdx)
	{
		m_EqpList.at(nIdx)->Reset_Yield_Day();
	}
}

//=============================================================================
// Method		: Is_FirstEquipment_inGroup
// Access		: public  
// Returns		: bool
// Parameter	: __in uint8_t IN_nEqp
// Qualifier	:
// Last Update	: 2022/6/3 - 16:18
// Desc.		:
//=============================================================================
bool CLineInfo::Is_FirstEquipment_inGroup(__in uint8_t IN_nEqp)
{
	if (IN_nEqp < m_EqpList.size())
	{
		if (m_EqpList.at(IN_nEqp)->Get_Group())
		{
			if (0 == m_EqpList.at(IN_nEqp)->Get_GroupIndex())
			{
				return true;
			}
		}
	}

	return false;
}

//=============================================================================
// Method		: Get_UseHandler
// Access		: public  
// Returns		: bool
// Qualifier	:
// Last Update	: 2022/8/12 - 17:42
// Desc.		:
//=============================================================================
bool CLineInfo::Get_UseHandler()
{
	if (0 < m_EqpList.size())
	{
		for (auto nIdx = 0; nIdx < m_EqpList.size(); ++nIdx)
		{
			if (m_EqpList.at(nIdx)->Is_Handler())
			{
				return true;
			}
		}
	}

	return false;
}

//=============================================================================
// Method		: Reset_ReservedPortCnt
// Access		: public  
// Returns		: void
// Qualifier	:
// Last Update	: 2023/2/27 - 16:38
// Desc.		:
//=============================================================================
void CLineInfo::Reset_ReservedPortCnt()
{
	if (0 < m_EqpList.size())
	{
		for (auto nIdx = 0; nIdx < m_EqpList.size(); ++nIdx)
		{
			m_EqpList.at(nIdx)->Reset_ReservedPortInfo();
		}
	}
}

void CLineInfo::Set_ReservedTimeout_Second(__in double IN_dSecond)
{
	if (0 < m_EqpList.size())
	{
		for (auto nIdx = 0; nIdx < m_EqpList.size(); ++nIdx)
		{
			m_EqpList.at(nIdx)->Set_ReservedTimeout_Second(IN_dSecond);
		}
	}
}

//=============================================================================
// Method		: Set_ActiveStatus
// Access		: public  
// Returns		: void
// Parameter	: __in uint8_t IN_nStatus
// Qualifier	:
// Last Update	: 2023/3/7 - 15:22
// Desc.		:
//=============================================================================
void CLineInfo::Set_ActiveStatus(__in uint8_t IN_nStatus)
{
	m_nActiveStatus = IN_nStatus;
}

uint8_t CLineInfo::Get_ActiveStatus()
{
	return m_nActiveStatus;
}