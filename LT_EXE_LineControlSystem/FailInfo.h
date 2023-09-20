#ifndef FailInfo_h__
#define FailInfo_h__

#pragma once

#include "FailInfo_Eqp.h"
//#include <vector>
#include <map>

//-----------------------------------------------------------------------------
// CFailInfo
//-----------------------------------------------------------------------------
class CFailInfo
{
public:

	CFailInfo();
	~CFailInfo();


	//std::vector<CFailInfo_Eqp> m_EqpList;
	std::map<uint8_t, CFailInfo_Eqp> m_EqpList;

protected:
	CString		m_szPath;
	CString		m_szPath_Report;

public:

	// ini 파일 저장 경로
	void	Set_Path				(__in LPCTSTR IN_szPath);
	void	Set_Path_Report			(__in LPCTSTR IN_szPath);

	// 설비 목록 제거
	void	Clear_EqpList			();
	void	Add_Equipment			(__in uint8_t IN_nEqpOrder, __in LPCTSTR IN_szEquipmentID);
	void	Add_Equipment			(__in uint8_t IN_nEqpOrder, __in LPCTSTR IN_szEquipmentID, __in uint8_t IN_nTestPortCount);
	size_t	Get_EquipmentCount		();

	// 설비별 불량 정보 클래스 얻기
	bool	IsExist					(__in uint8_t IN_nEqpOrder);
	CFailInfo_Eqp&			Get_Equipment	(__in uint8_t IN_nEqpOrder);
	const CFailInfo_Eqp&	Get_Equipment	(__in uint8_t IN_nEqpOrder) const;

	// NG Code 카운터 초기화
	void	Reset_Count_All			();
	void	Reset_Count				(__in uint8_t IN_nEqpOrder);
	void	Reset_Count_NGCode		(__in uint8_t IN_nEqpOrder, __in int16_t IN_nNGCode);

	// NG Code 카운트 증가
	void	IncreaseCount			(__in uint8_t IN_nEqpOrder, __in int16_t IN_nNGCode, __in uint8_t IN_nNG_Para);

	//        -> Eqp
	//        -> A,L,R
	// Ng Code

	bool	Save_IniFile			();
	bool	Load_IniFile			();
	
	bool	Write_CSV_File			(__in uint8_t IN_nEqpOrder, __in LPCTSTR IN_szPathFile);
	bool	Write_CSV_File			(__in LPCTSTR IN_szPath);
	bool	Write_CSV_File_Default	();
	bool	Write_CSV_File_Default	(__out CString& OUT_szPath, __out SYSTEMTIME& OUT_tmLocal);

};


#endif // FailInfo_h__