//*****************************************************************************
// Filename	: 	LineInfo.h
// Created	:	2021/11/22 - 18:00
// Modified	:	2021/11/22 - 18:00
//
// Author	:	piring
//	
// Purpose	:	������ ������� ������ ó��
//*****************************************************************************
#ifndef LineInfo_h__
#define LineInfo_h__

#pragma once

#include <vector>
#include "Equipment.h"
#include "Def_ConfigLine.h"
#include "Def_ModelConfig.h"
#include "Def_DebugInfo.h"


//-----------------------------------------------------------------------------
// CLineInfo
//-----------------------------------------------------------------------------
class CLineInfo : public CConfig_Model
{
public:
	CLineInfo();
	~CLineInfo();

	CLineInfo& operator= (const CConfig_Model& ref)
	{
		CConfig_Model::operator=(ref);

		return *this;
	};

	CLineInfo& operator= (const CLineInfo& ref)
	{
		CConfig_Model::operator=(ref);

		return *this;
	};

protected:

	// �˻��� ���� ����
	std::vector<CEquipment*>	m_EqpList;

	// ���� ������ ���� ����
	CEquipment*					m_pFstEqp_EqpType[Max_EqpTypeCount] = { nullptr, };
	// ���� ������ �׷�
	std::vector<CEquipment*>	m_pEquipment_Group[Max_EqpTypeCount];

#ifdef 	NEW_INSPECTION_TYPE_APPLY
	// �˻� ���� �׷�, �� �˻� ���� �׷� ������ üũ
	std::vector<CEquipment*>	m_pInspection_Group[Max_InspTypeCount];
#endif	// NEW_INSPECTION_TYPE_APPLY

	// �˻� ���� <Equipment Type> : Loader, Returner ������ �˻��� ����
	std::vector<uint8_t>		m_TestOrder;
#ifdef 	NEW_INSPECTION_TYPE_APPLY
	// �˻� ���� <Inspection Type> : Loader, Returner ������ �˻��� ����
	std::vector<uint8_t>		m_InspectionOrder;
#endif	// NEW_INSPECTION_TYPE_APPLY

	// � ��� (auto, manual)
	uint8_t						m_OperateMode		= 0;	
	//uint8_t						m_nLanguage			= enLanguage::Lang_English;

	// ���� (����) ���
	bool						m_bEndOfProduction	= false;

	// ������ ����/�񰡵� ����
	uint8_t						m_nActiveStatus = OAS_InitStatus;

	// ����� ����
	const ST_DebugInfo*			m_pDebugInfo		= nullptr;

	// ���� ���� ������ ���� ���ο� ���� ������ ������ �� (������: true)
	bool	Compare_PreConfigLine		(__in const CConfig_Line* IN_pCConfig_Line);

	// ���� ������ �ش��ϴ� ��� ���� Skip���� �����Ǿ� �ֳ�?
	inline bool IsSkip_EquipmentType	(__in enEquipmentType IN_nEqpType);

#ifdef 	NEW_INSPECTION_TYPE_APPLY
	inline bool IsSkip_InspectionType	(__in enInspectionType IN_nInspType);
#endif	// NEW_INSPECTION_TYPE_APPLY

	bool	Write_File					(__in LPCTSTR IN_szPathFile, __in LPCTSTR IN_szData, __in LPCTSTR IN_szHeader);


public:	
	// ���� ����
	size_t				GetCount			() const;
	void				RemoveAll			();
	CEquipment&			GetAt				(__in uint16_t IN_nIdx);
	const CEquipment&	GetAt				(__in uint16_t IN_nIdx) const;
	int					Get_EquipmentIndex	(__in LPCTSTR IN_szEqpId);

	// ���������� ù��° ���� ���ϱ�
	CEquipment&			Get_FirstEquipment	(__in enEquipmentType IN_nEqpType);
	const CEquipment&	Get_FirstEquipment	(__in enEquipmentType IN_nEqpType) const;
	CEquipment&			Get_LastEquipment	(__in enEquipmentType IN_nEqpType);
	const CEquipment&	Get_LastEquipment	(__in enEquipmentType IN_nEqpType) const;


	CEquipment&			Get_Loader			();
	const CEquipment&	Get_Loader			() const;
	CEquipment&			Get_Returner		();
	const CEquipment&	Get_Returner		() const;
	CEquipment&			Get_Handler			();
	const CEquipment&	Get_Handler			() const;


	CEquipment&			Get_FirstTester		(__in enTesterType IN_nTesterType);
	const CEquipment&	Get_FirstTester		(__in enTesterType IN_nTesterType) const;
	CEquipment&			Get_LastTester		(__in enTesterType IN_nTesterType);
	const CEquipment&	Get_LastTester		(__in enTesterType IN_nTesterType) const;

#ifdef 	NEW_INSPECTION_TYPE_APPLY
	CEquipment&			Get_FirstTester		(__in enInspectionType IN_nInspType);
	const CEquipment&	Get_FirstTester		(__in enInspectionType IN_nInspType) const;
	CEquipment&			Get_LastTester		(__in enInspectionType IN_nInspType);
	const CEquipment&	Get_LastTester		(__in enInspectionType IN_nInspType) const;

#endif	// NEW_INSPECTION_TYPE_APPLY


 	uint8_t Get_NextEquipment				(__in uint8_t IN_FromEqp);
 	uint8_t Get_PrevEquipment				(__in uint8_t IN_FromEqp);

	uint8_t	Get_TesterCount					();

	//2023.01.27a uhkim
	//size_t					GetSvrCount() const;
	//void					SvrRemoveAll();
	//CEquipment&				GetSvrAt(__in uint16_t IN_nIdx);
	//const CEquipment&		GetSvrAt(__in uint16_t IN_nIdx) const;
	//int						Get_ServerIndex(__in LPCTSTR IN_szEqpId);
public:

	void	Set_Config_Line				(__in const CConfig_Line* IN_pConfig_Line);
	void	Set_Config_Model			(__in const CConfig_Model* IN_pConfig_Model);
	void	Set_Path					(__in LPCTSTR IN_szLog, __in LPCTSTR IN_szReport);
	void	SetPtr_DebugMode			(__in const ST_DebugInfo* IN_pDebugInfo);


	uint8_t	Get_OperateMode				() const;
	void	Set_OperateMode				(__in uint8_t IN_nOperateMode);


	// ���� ���� �����ΰ�? (��ǰ ������ �� �ܰ� ������� ��� �ֳ� üũ)
	bool	IsEndOfProduction			(__in uint8_t IN_nEqpOrder);

	bool	Get_EndOfProduction			() const;
	void	Set_EndOfProduction			(__in bool IN_bEnd);	// 0: start, 1: end

	// ��� ����
	//uint8_t	Get_Language				() const;
	//void	Set_Language				(__in uint8_t IN_nLanguage);

	// ���� �˻� ã�� (���� ���� ��ȯ)
	uint8_t	Get_NextEquipmentType		(__in enEquipmentType IN_nEqpType);
	uint8_t	Get_NextEquipmentType		(__in uint8_t IN_FromEqp);
	uint8_t	Get_PrevEquipmentType		(__in enEquipmentType IN_nEqpType);
	uint8_t	Get_PrevEquipmentType		(__in uint8_t IN_FromEqp);
	// ���� ���� ã�� �׷� ��
	uint8_t Get_TargetEquipment_inGroup	(__in enEquipmentType IN_nEqpType);
	uint8_t Get_TargetEquipment_inGroup_Over(__in enEquipmentType IN_nEqpType);

#ifdef 	NEW_INSPECTION_TYPE_APPLY
	// ���� �˻� ã�� (�˻� ���� ��ȯ)
	uint8_t	Get_NextInspectionType		(__in enInspectionType IN_nInspType, __in bool bLoader = false);
	uint8_t	Get_NextInspectionType		(__in uint8_t IN_FromEqp);
	uint8_t	Get_PrevInspectionType		(__in enInspectionType IN_nInspType, __in bool bReturner = false);
	uint8_t	Get_PrevInspectionType		(__in uint8_t IN_FromEqp);
	// ���� ���� ã�� �׷� ��
	uint8_t Get_TargetEquipment_inGroup(__in enInspectionType IN_nInspType);
	uint8_t Get_TargetEquipment_inGroup_Over(__in enInspectionType IN_nInspType);


#endif	// NEW_INSPECTION_TYPE_APPLY


	// ���� ���� ã��
	uint8_t Get_TargetEquipment			(__in uint8_t IN_FromEqp);
	
	// ���� ����� ������ �մܰ��� ����� ã��
	uint8_t	Get_FindEqpz_SameTarget		(__in uint8_t IN_nTargetEqp, __out std::vector<uint16_t>& OUT_nEqpz);
	uint8_t	Get_FindSocketz_SameTarget	(__in uint8_t IN_nTargetEqp, __out std::vector<CString>& OUT_Socketz);

	// ���� ����
	bool	Write_CSV_File				(__in LPCTSTR IN_szPath);

	// ���� �ʱ�ȭ
	void	Reset_EquipmentYield_All	();

	// �˻� �׷��� ù��° �����ΰ�?
	bool	Is_FirstEquipment_inGroup	(__in uint8_t IN_nEqp);

	// ���� ������ �ڵ鷯�� �ִ°�?
	bool	Get_UseHandler				();

	// ���� ���� ��Ʈ �ʱ�ȭ (�߰��� ������ �������� �����ϴ� ��쿡 ���� ��Ʈ ��꿡 ������ �߻���)
	void	Reset_ReservedPortCnt();
	// ����� ������ �����Ǿ��� ��� timeout ó���ϸ鼭 ���� �������� ���� �� �� �� �ð�
	void	Set_ReservedTimeout_Second(__in double IN_dSecond);

	// ������ ����/�񰡵� ����
	void	Set_ActiveStatus(__in uint8_t IN_nStatus);
	uint8_t	Get_ActiveStatus();
};

#endif // LineInfo_h__
