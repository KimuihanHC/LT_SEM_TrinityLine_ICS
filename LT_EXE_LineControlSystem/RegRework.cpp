//*****************************************************************************
// Filename	: 	RegRework.cpp
// Created	:	2021/12/21 - 13:46
// Modified	:	2021/12/21 - 13:46
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#include "stdafx.h"
#include "RegRework.h"
#include "Registry.h"
#include "Rework_Info.h"
#include "CommonFunction.h"

CRegRework::CRegRework()
{
	m_szRegPath.Format(_T("%s\\ReworkInfo"), REG_PATH_APP_BASE);

	// ReworkInfo\\BarcodeList\\
	// ReworkInfo\\Product\\

}

CRegRework::CRegRework(__in LPCTSTR lpszRegPath)
{
	m_szRegPath.Format(_T("%s\\ReworkInfo"), lpszRegPath);
}

CRegRework::~CRegRework()
{

}

//=============================================================================
// Method		: Set_RegitryPath
// Access		: public  
// Returns		: void
// Parameter	: __in LPCTSTR lpszRegPath
// Qualifier	:
// Last Update	: 2022/1/10 - 18:10
// Desc.		:
//=============================================================================
void CRegRework::Set_RegitryPath(__in LPCTSTR lpszRegPath)
{
	m_szRegPath.Format(_T("%s\\ReworkInfo"), lpszRegPath);
}

//=============================================================================
// Method		: Set_ReworkBarcodeList
// Access		: public  
// Returns		: void
// Parameter	: __in CRework_Info IN_pReworkInfo
// Qualifier	:
// Last Update	: 2022/1/10 - 18:10
// Desc.		:
//=============================================================================
bool CRegRework::Set_ReworkBarcodeList(const CRework_Info * IN_pReworkInfo)
{
	CRegistry	m_Reg;
	CString		szRegPath;
	CString		szKey;
	CString		szValue;
	szRegPath.Format(_T("%s\\BarcodeList"), m_szRegPath.GetBuffer());

	// 기존 데이터 삭제
	m_Reg.DeleteKey(HKEY_CURRENT_USER, szRegPath);

	// 신규 데이터 생성
	if (!m_Reg.VerifyKey(HKEY_CURRENT_USER, szRegPath))
	{
		m_Reg.CreateKey(HKEY_CURRENT_USER, szRegPath);
	}

	if (m_Reg.Open(HKEY_CURRENT_USER, szRegPath))
	{
		uint16_t nIdx = 0;

		// 제품 개수
		m_Reg.WriteDWORD(_T("Rework_Count"), static_cast<DWORD>(IN_pReworkInfo->m_ReworkInfo.size()));

		for (auto Iter = IN_pReworkInfo->m_ReworkInfo.begin(); Iter != IN_pReworkInfo->m_ReworkInfo.end(); Iter++)
		{
			// 바코드
			szKey.Format(_T("%d"), nIdx++);
			m_Reg.WriteString(szKey.GetBuffer(), Iter->first.c_str());
		}
	}
	else
	{
		return false;
	}

	m_Reg.Close();

	return true;
}

//=============================================================================
// Method		: Set_ReworkProduct
// Access		: public  
// Returns		: bool
// Parameter	: __in LPCTSTR IN_szBarcode
// Parameter	: __in const ST_ReworkProduct * IN_pReworkPrd
// Qualifier	:
// Last Update	: 2021/12/21 - 16:15
// Desc.		:
//=============================================================================
bool CRegRework::Set_ReworkProduct(__in LPCTSTR IN_szBarcode, const ST_ReworkProduct * IN_pReworkPrd)
{
	CRegistry	m_Reg;
	CString		szRegPath;
	CString		szValue;
	szRegPath.Format(_T("%s\\Product\\%s"), m_szRegPath.GetBuffer(), IN_szBarcode);

	if (!m_Reg.VerifyKey(HKEY_CURRENT_USER, szRegPath))
	{
		m_Reg.CreateKey(HKEY_CURRENT_USER, szRegPath);
	}

	if (m_Reg.Open(HKEY_CURRENT_USER, szRegPath))
	{
		//CString		szBarcode;		// 제품 바코드 S/N
		//szValue = IN_pReworkPrd->szBarcode;
		//m_Reg.WriteString(_T("Barcode"), szValue.GetBuffer());

		//uint8_t		nNG_EqpType;	// Fail 처리된 검사 유형
		m_Reg.WriteDWORD(_T("NG_EqpType"), IN_pReworkPrd->nNG_EqpType);

		//int16_t		nNG_Code;		// Fail 사유 ?
		szValue.Format(_T("%d"), IN_pReworkPrd->nNG_Code);
		m_Reg.WriteString(_T("NG_Code"), szValue.GetBuffer());

		//SYSTEMTIME	InputTime;		// 제품 투입된 시간
		szValue = SystemTimeToFormatString(IN_pReworkPrd->InputTime);
		m_Reg.WriteString(_T("InputTime"), szValue.GetBuffer());
	}
	else
	{
		return false;
	}

	m_Reg.Close();

	return true;
}

//=============================================================================
// Method		: Set_ReworkInfo
// Access		: public  
// Returns		: bool
// Parameter	: __in const CRework_Info * IN_pReworkInfo
// Qualifier	:
// Last Update	: 2021/12/21 - 16:15
// Desc.		:
//=============================================================================
bool CRegRework::Set_ReworkInfo(const CRework_Info * IN_pReworkInfo)
{
	CRegistry	m_Reg;
	CString		szRegPath;
	CString		szKey;
	CString		szValue;
	szRegPath.Format(_T("%s\\BarcodeList"), m_szRegPath.GetBuffer());

	// 기존 데이터 삭제
	m_Reg.DeleteKey(HKEY_CURRENT_USER, szRegPath);

	// 신규 데이터 생성
	if (!m_Reg.VerifyKey(HKEY_CURRENT_USER, szRegPath))
	{
		m_Reg.CreateKey(HKEY_CURRENT_USER, szRegPath);
	}

	if (m_Reg.Open(HKEY_CURRENT_USER, szRegPath))
	{
		uint16_t nIdx = 0;

		// 제품 개수
		m_Reg.WriteDWORD(_T("Rework_Count"), static_cast<DWORD>(IN_pReworkInfo->m_ReworkInfo.size()));

		for (auto Iter = IN_pReworkInfo->m_ReworkInfo.begin(); Iter != IN_pReworkInfo->m_ReworkInfo.end(); Iter++)
		{
			// 바코드
			szKey.Format(_T("%d"), nIdx++);
			m_Reg.WriteString(szKey.GetBuffer(), Iter->first.c_str());

			// 개별 제품 정보 저장
			Set_ReworkProduct(Iter->first.c_str(), &Iter->second);
		}
	}
	else
	{
		return false;
	}

	m_Reg.Close();

	return true;
}

//=============================================================================
// Method		: Get_ReworkProduct
// Access		: public  
// Returns		: bool
// Parameter	: __in LPCTSTR IN_szBarcode
// Parameter	: __out ST_ReworkProduct & OUT_ReworkPrd
// Qualifier	:
// Last Update	: 2021/12/21 - 16:15
// Desc.		:
//=============================================================================
bool CRegRework::Get_ReworkProduct(LPCTSTR IN_szBarcode, ST_ReworkProduct & OUT_ReworkPrd)
{
	CRegistry	m_Reg;
	CString		szRegPath;
	CString		szValue;
	DWORD		dwValue = 0;
	szRegPath.Format(_T("%s\\Product\\%s"), m_szRegPath.GetBuffer(), IN_szBarcode);

	if (m_Reg.Open(HKEY_CURRENT_USER, szRegPath))
	{
		//CString		szBarcode;		// 제품 바코드 S/N
		/*if (m_Reg.ReadString(_T("Barcode"), szValue))
			OUT_ReworkPrd.szBarcode = szValue.GetBuffer();
		else
			OUT_ReworkPrd.szBarcode.Empty();*/

		//uint8_t		nNG_EqpType;	// Fail 처리된 검사 유형
		if (m_Reg.ReadDWORD(_T("NG_EqpType"), dwValue))
			OUT_ReworkPrd.nNG_EqpType = static_cast<uint8_t>(dwValue);
		else
			OUT_ReworkPrd.nNG_EqpType = 0;

		//int16_t		nNG_Code;		// Fail 사유 ?
		if (m_Reg.ReadString(_T("NG_Code"), szValue))
			OUT_ReworkPrd.nNG_Code = static_cast<int16_t>(_ttoi(szValue.GetBuffer()));
		else
			OUT_ReworkPrd.nNG_Code = 0;

		//SYSTEMTIME	InputTime;		// 제품 투입된 시간
		if (m_Reg.ReadString(_T("InputTime"), szValue))
			FormatStringToSystemTime(szValue.GetBuffer(), OUT_ReworkPrd.InputTime);
		else
			OUT_ReworkPrd.InputTime;
	}
	else
	{
		return false;
	}

	m_Reg.Close();

	return true;
}

//=============================================================================
// Method		: Get_ReworkInfo
// Access		: public  
// Returns		: bool
// Parameter	: __out std::map<std::wstring, ST_ReworkProduct> & OUT_ReworkInfo
// Qualifier	:
// Last Update	: 2021/12/21 - 16:33
// Desc.		:
//=============================================================================
#ifdef _UNICODE
bool CRegRework::Get_ReworkInfo(__out std::map<std::wstring, ST_ReworkProduct>& OUT_ReworkInfo)
#else
bool CRegRework::Get_ReworkInfo(__out std::map<std::string, ST_ReworkProduct>& OUT_ReworkInfo)
#endif
{
	// 기존 데이터 전부 제거
	OUT_ReworkInfo.clear();

	// 바코드 읽고, 데이터 읽고
	CRegistry	m_Reg;
	CString		szRegPath;
	CString		szKey;
	CString		szValue;
	DWORD		dwValue = 0;
	szRegPath.Format(_T("%s\\BarcodeList"), m_szRegPath.GetBuffer());

	if (m_Reg.Open(HKEY_CURRENT_USER, szRegPath))
	{
		// 제품 개수
		DWORD dwCount = 0;
		if (m_Reg.ReadDWORD(_T("Rework_Count"), dwValue))
			dwCount = dwValue;
		
		for (DWORD nIdx = 0; nIdx < dwCount; ++nIdx)
		{
			szKey.Format(_T("%d"), nIdx);
			if (m_Reg.ReadString(szKey.GetBuffer(), szValue))
			{
				ST_ReworkProduct ReworkUnit;

				// 레지스트리에 있는 소켓 정보 구하기
				if (Get_ReworkProduct(szValue.GetBuffer(), ReworkUnit))
				{
					OUT_ReworkInfo.insert({ szValue.GetBuffer(), ReworkUnit });
				}
			}
		}
	}
	else
	{
		return false;
	}

	m_Reg.Close();

	return true;



	// OUT_ReworkInto에 있는 기본 소켓 Config 정보를 이용해서 레지스트리에 저장된 정보 불러옴

	//auto eIter = OUT_ReworkInfo.m_ReworkInfo.end();
	//auto Iter = OUT_ReworkInfo.m_ReworkInfo.begin();
	//while (Iter != eIter)
	//{
	//	ST_ReworkProduct ReworkUnit;

	//	// 레지스트리에 있는 소켓 정보 구하기
	//	if (Get_ReworkProduct (Iter->first.c_str(), ReworkUnit))
	//	{
	//		OUT_ReworkInfo.Add_ReworkProduct(Iter->first.c_str(), &ReworkUnit);
	//	}

	//	Iter++;
	//}

	//return true;
}

//bool CRegRework::Remove_ReworkProduct(LPCTSTR IN_szBarcode)
//{
//	// 불량 제품 1개 삭제
//	// 불량 제품 카운트 1개 삭제
//
//	CRegistry	m_Reg;
//	CString		szRegPath;
//	szRegPath.Format(_T("%s\\Product\\%s"), m_szRegPath.GetBuffer(), IN_szBarcode);
//
//	if (m_Reg.DeleteKey(HKEY_CURRENT_USER, szRegPath.GetBuffer()))
//	{
//		return true;
//	}
//
//	return false;
//}

bool CRegRework::Remove_ReworkProduct(LPCTSTR IN_szBarcode, const CRework_Info * IN_pReworkInfo)
{
	bool bReturn = false;

	CRegistry	m_Reg;
	CString		szRegPath;
	szRegPath.Format(_T("%s\\Product\\%s"), m_szRegPath.GetBuffer(), IN_szBarcode);

	if (m_Reg.DeleteKey(HKEY_CURRENT_USER, szRegPath.GetBuffer()))
	{
		bReturn = true;
	}

	// 바코드 리스트 갱신
	if (Set_ReworkBarcodeList(IN_pReworkInfo))
	{
		bReturn &= true;
	}

	return bReturn;
}

bool CRegRework::Remove_ReworkInfo()
{
	bool bReturn = false;

	// 모든 데이터 삭제
	CRegistry	m_Reg;
	CString		szRegPath;
	szRegPath.Format(_T("%s\\Product"), m_szRegPath.GetBuffer());
	if (m_Reg.DeleteKey(HKEY_CURRENT_USER, szRegPath.GetBuffer()))
	{
		bReturn = true;
	}

	szRegPath.Format(_T("%s\\BarcodeList"), m_szRegPath.GetBuffer());
	if (m_Reg.DeleteKey(HKEY_CURRENT_USER, szRegPath))
	{
		bReturn &= true;
	}

	return bReturn;
}
