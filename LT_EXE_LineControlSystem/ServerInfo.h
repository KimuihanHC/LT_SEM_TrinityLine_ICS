//*****************************************************************************
// Filename	: 	LineInfo.h
// Created	:	2021/11/22 - 18:00
// Modified	:	2021/11/22 - 18:00
//
// Author	:	piring
//	
// Purpose	:	 라인의 설비들의 데이터 처리//
//*****************************************************************************
#ifndef ServerInfo_h__
#define ServerInfo_h__

#pragma once

#include <vector>
#include "Server.h"
#include "Def_ConfigLineServer.h"
#include "Def_ModelConfig.h"
#include "Def_DebugInfo.h"


//-----------------------------------------------------------------------------
// CLineInfo
//-----------------------------------------------------------------------------
class CServerInfo : public CConfig_Model
{
public:
	CServerInfo();
	~CServerInfo();

	CServerInfo& operator= (const CConfig_Model& ref)
	{
		CConfig_Model::operator=(ref);

		return *this;
	};

	CServerInfo& operator= (const CServerInfo& ref)
	{
		CConfig_Model::operator=(ref);

		return *this;
	};

protected:

	// 검사기들 구성 정보//
	std::vector<CServer*>	m_SvrList;

	// 설비 종류별 시작 설비
	CServer*				m_pFstEqp_EqpType[SERVER_MAX] = { nullptr, };
	// 설비 종류별 그룹//
	std::vector<CServer*>	m_pEquipment_Group[SERVER_MAX];

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

	// 디버그 정보
	const ST_DebugInfo*			m_pDebugInfo		= nullptr;

	// 이전 설비 구성과 현재 새로운 설비 구성이 같은지 비교 (같으면: true)
	bool	Compare_PreConfigLine		(__in const CConfig_Server* IN_pCConfig_Line);

	// 설비 유형에 해당하는 모든 설비가 Skip으로 설정되어 있나?
	inline bool IsSkip_ServerType		(__in enServerType IN_nEqpType);

#ifdef 	NEW_INSPECTION_TYPE_APPLY
	//inline bool IsSkip_InspectionType	(__in enInspectionType IN_nInspType);
#endif	// NEW_INSPECTION_TYPE_APPLY

	bool	Write_File					(__in LPCTSTR IN_szPathFile, __in LPCTSTR IN_szData, __in LPCTSTR IN_szHeader);

public:	
	// 스텝 갯수
	size_t				GetCount			() const;
	void				RemoveAll			();
	CServer&			GetAt				(__in uint16_t IN_nIdx);
	const CServer&		GetAt				(__in uint16_t IN_nIdx) const;
	int					Get_ServerIndex		(__in LPCTSTR IN_szEqpId);

	// 설비유형의 첫번째 설비 구하기//
	CServer&			Get_FirstServer(__in enServerType IN_nEqpType);
	const CServer&		Get_FirstServer(__in enServerType IN_nEqpType) const;
	CServer&			Get_LastServer(__in enServerType IN_nEqpType);
	const CServer&		Get_LastServer(__in enServerType IN_nEqpType) const;

	CServer&			Get_EES();
	const CServer&		Get_EES() const;
	/*
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
	*/
#ifdef 	NEW_INSPECTION_TYPE_APPLY
	CEquipment&			Get_FirstTester		(__in enInspectionType IN_nInspType);
	const CEquipment&	Get_FirstTester		(__in enInspectionType IN_nInspType) const;
	CEquipment&			Get_LastTester		(__in enInspectionType IN_nInspType);
	const CEquipment&	Get_LastTester		(__in enInspectionType IN_nInspType) const;

#endif	// NEW_INSPECTION_TYPE_APPLY


 	uint8_t Get_NextServer(__in uint8_t IN_FromEqp);
 	uint8_t Get_PrevServer(__in uint8_t IN_FromEqp);

	uint8_t	Get_TesterCount					();

public:

	void	Set_Config_Line				(__in const CConfig_Server* IN_pConfig_Line);
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
	uint8_t	Get_NextServerType			(__in enServerType IN_nEqpType);
	uint8_t	Get_NextServerType			(__in uint8_t IN_FromEqp);
	uint8_t	Get_PrevServerType			(__in enServerType IN_nEqpType);
	uint8_t	Get_PrevServerType			(__in uint8_t IN_FromEqp);
	// 목적 설비 찾기 그룹 내//
	uint8_t Get_TargetEquipment_inGroup	(__in enServerType IN_nEqpType);
	uint8_t Get_TargetEquipment_inGroup_Over(__in enServerType IN_nEqpType);

#ifdef 	NEW_INSPECTION_TYPE_APPLY
	// 목적 검사 찾기 (검사 유형 반환)
	uint8_t	Get_NextInspectionType		(__in enInspectionType IN_nInspType, __in bool bLoader = false);
	uint8_t	Get_NextInspectionType		(__in uint8_t IN_FromEqp);
	uint8_t	Get_PrevInspectionType		(__in enInspectionType IN_nInspType, __in bool bReturner = false);
	uint8_t	Get_PrevInspectionType		(__in uint8_t IN_FromEqp);
	// 목적 설비 찾기 그룹 내
	uint8_t Get_TargetEquipment_inGroup	(__in enInspectionType IN_nInspType);
	uint8_t Get_TargetEquipment_inGroup_Over(__in enInspectionType IN_nInspType);


#endif	// NEW_INSPECTION_TYPE_APPLY


	// 목적 설비 찾기
	uint8_t Get_TargetEquipment			(__in uint8_t IN_FromEqp);
	
	// 목적 설비로 설정된 앞단계의 설비들 찾기
	uint8_t	Get_FindEqpz_SameTarget		(__in uint8_t IN_nTargetEqp, __out std::vector<uint16_t>& OUT_nEqpz);
	uint8_t	Get_FindSocketz_SameTarget	(__in uint8_t IN_nTargetEqp, __out std::vector<CString>& OUT_Socketz);

	// 수율 저장//
	bool	Write_CSV_File				(__in LPCTSTR IN_szPath);

	// 수율 초기화//
	void	Reset_EquipmentYield_All	();

	// 검사 그룹의 첫번째 설비인가?
	bool	Is_FirstEquipment_inGroup	(__in uint8_t IN_nEqp);

	// 라인 구성에 핸들러가 있는가?
	//bool	Get_UseHandler				();

};

#endif // LineInfo_h__
