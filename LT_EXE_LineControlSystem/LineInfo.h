//*****************************************************************************
// Filename	: 	LineInfo.h
// Created	:	2021/11/22 - 18:00
// Modified	:	2021/11/22 - 18:00
//
// Author	:	piring
//	
// Purpose	:	라인의 설비들의 데이터 처리
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

	// 검사기들 구성 정보
	std::vector<CEquipment*>	m_EqpList;

	// 설비 종류별 시작 설비
	CEquipment*					m_pFstEqp_EqpType[Max_EqpTypeCount] = { nullptr, };
	// 설비 종류별 그룹
	std::vector<CEquipment*>	m_pEquipment_Group[Max_EqpTypeCount];

#ifdef 	NEW_INSPECTION_TYPE_APPLY
	// 검사 설비 그룹, 비 검사 설비 그룹 별도로 체크
	std::vector<CEquipment*>	m_pInspection_Group[Max_InspTypeCount];
#endif	// NEW_INSPECTION_TYPE_APPLY

	// 검사 순서 <Equipment Type> : Loader, Returner 제외한 검사의 순서
	std::vector<uint8_t>		m_TestOrder;
#ifdef 	NEW_INSPECTION_TYPE_APPLY
	// 검사 순서 <Inspection Type> : Loader, Returner 제외한 검사의 순서
	std::vector<uint8_t>		m_InspectionOrder;
#endif	// NEW_INSPECTION_TYPE_APPLY

	// 운영 모드 (auto, manual)
	uint8_t						m_OperateMode		= 0;	
	//uint8_t						m_nLanguage			= enLanguage::Lang_English;

	// 생산 (종료) 모드
	bool						m_bEndOfProduction	= false;

	// 라인의 가동/비가동 상태
	uint8_t						m_nActiveStatus = OAS_InitStatus;

	// 디버그 정보
	const ST_DebugInfo*			m_pDebugInfo		= nullptr;

	// 이전 설비 구성과 현재 새로운 설비 구성이 같은지 비교 (같으면: true)
	bool	Compare_PreConfigLine		(__in const CConfig_Line* IN_pCConfig_Line);

	// 설비 유형에 해당하는 모든 설비가 Skip으로 설정되어 있나?
	inline bool IsSkip_EquipmentType	(__in enEquipmentType IN_nEqpType);

#ifdef 	NEW_INSPECTION_TYPE_APPLY
	inline bool IsSkip_InspectionType	(__in enInspectionType IN_nInspType);
#endif	// NEW_INSPECTION_TYPE_APPLY

	bool	Write_File					(__in LPCTSTR IN_szPathFile, __in LPCTSTR IN_szData, __in LPCTSTR IN_szHeader);


public:	
	// 스텝 갯수
	size_t				GetCount			() const;
	void				RemoveAll			();
	CEquipment&			GetAt				(__in uint16_t IN_nIdx);
	const CEquipment&	GetAt				(__in uint16_t IN_nIdx) const;
	int					Get_EquipmentIndex	(__in LPCTSTR IN_szEqpId);

	// 설비유형의 첫번째 설비 구하기
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


	// 생산 종료 상태인가? (제품 도착시 앞 단계 설비들이 비어 있나 체크)
	bool	IsEndOfProduction			(__in uint8_t IN_nEqpOrder);

	bool	Get_EndOfProduction			() const;
	void	Set_EndOfProduction			(__in bool IN_bEnd);	// 0: start, 1: end

	// 언어 설정
	//uint8_t	Get_Language				() const;
	//void	Set_Language				(__in uint8_t IN_nLanguage);

	// 목적 검사 찾기 (설비 유형 반환)
	uint8_t	Get_NextEquipmentType		(__in enEquipmentType IN_nEqpType);
	uint8_t	Get_NextEquipmentType		(__in uint8_t IN_FromEqp);
	uint8_t	Get_PrevEquipmentType		(__in enEquipmentType IN_nEqpType);
	uint8_t	Get_PrevEquipmentType		(__in uint8_t IN_FromEqp);
	// 목적 설비 찾기 그룹 내
	uint8_t Get_TargetEquipment_inGroup	(__in enEquipmentType IN_nEqpType);
	uint8_t Get_TargetEquipment_inGroup_Over(__in enEquipmentType IN_nEqpType);

#ifdef 	NEW_INSPECTION_TYPE_APPLY
	// 목적 검사 찾기 (검사 유형 반환)
	uint8_t	Get_NextInspectionType		(__in enInspectionType IN_nInspType, __in bool bLoader = false);
	uint8_t	Get_NextInspectionType		(__in uint8_t IN_FromEqp);
	uint8_t	Get_PrevInspectionType		(__in enInspectionType IN_nInspType, __in bool bReturner = false);
	uint8_t	Get_PrevInspectionType		(__in uint8_t IN_FromEqp);
	// 목적 설비 찾기 그룹 내
	uint8_t Get_TargetEquipment_inGroup(__in enInspectionType IN_nInspType);
	uint8_t Get_TargetEquipment_inGroup_Over(__in enInspectionType IN_nInspType);


#endif	// NEW_INSPECTION_TYPE_APPLY


	// 목적 설비 찾기
	uint8_t Get_TargetEquipment			(__in uint8_t IN_FromEqp);
	
	// 목적 설비로 설정된 앞단계의 설비들 찾기
	uint8_t	Get_FindEqpz_SameTarget		(__in uint8_t IN_nTargetEqp, __out std::vector<uint16_t>& OUT_nEqpz);
	uint8_t	Get_FindSocketz_SameTarget	(__in uint8_t IN_nTargetEqp, __out std::vector<CString>& OUT_Socketz);

	// 수율 저장
	bool	Write_CSV_File				(__in LPCTSTR IN_szPath);

	// 수율 초기화
	void	Reset_EquipmentYield_All	();

	// 검사 그룹의 첫번째 설비인가?
	bool	Is_FirstEquipment_inGroup	(__in uint8_t IN_nEqp);

	// 라인 구성에 핸들러가 있는가?
	bool	Get_UseHandler				();

	// 설비별 예약 포트 초기화 (중간에 소켓을 수동으로 제거하는 경우에 예약 포트 계산에 문제가 발생함)
	void	Reset_ReservedPortCnt();
	// 예약된 소켓이 오래되었을 경우 timeout 처리하면서 예약 정보에서 제거 할 때 비교 시간
	void	Set_ReservedTimeout_Second(__in double IN_dSecond);

	// 라인의 가동/비가동 상태
	void	Set_ActiveStatus(__in uint8_t IN_nStatus);
	uint8_t	Get_ActiveStatus();
};

#endif // LineInfo_h__
