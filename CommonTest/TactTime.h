
#ifndef Tacttime_h__
#define Tacttime_h__

#pragma once

#include <stdint.h>

//-----------------------------------------------------------------------------
// Tacttime 클래스
//-----------------------------------------------------------------------------
class CTactTime
{
public:
	CTactTime();
	~CTactTime();

	CTactTime& operator= (const CTactTime& ref);

protected:

	// tacttime 체크 시작 여부 flag
	bool		m_bFlag_timeCheck	= false;
	// 계산된 tacttime
	double		m_dTacttime			= 0.0f;	
	// 누적 평균 tacttime
	double		m_dTacttime_Ave		= 0.0f;
	// 체크 횟수
	double		m_dCount			= 0.0f;

	// 이전 소켓의 Unload 시간
	SYSTEMTIME	m_tmUnload_First;
	SYSTEMTIME	m_tmUnload_Prev;
	
	// SYSTEMTIME 시간차이 계산
	inline static double	CompareSystemTime			(__in SYSTEMTIME* pEndTime, __in SYSTEMTIME* pStartTime);
	inline static CString	SystemTimeToFormatString	(__in SYSTEMTIME* IN_pTime);
	inline static BOOL		FormatStringToSystemTime	(__in LPCTSTR IN_szTime, __out SYSTEMTIME& OUT_Time);

public:
	
	void	Reset				();

	bool	Get_Flag_timeCheck	() const;

	double	Get_Count			() const;

	double	Set_UnloadTime		(__in SYSTEMTIME* IN_pUnloadTime);

	double	Get_TactTime		() const;

	double	Get_TactTime_Ave	() const;

	const SYSTEMTIME&	Get_Time_First() const;
	const SYSTEMTIME&	Get_Time_Previous() const;

	bool	Save_Tacttime		(__in LPCTSTR IN_szRegPath);
	bool	Load_Tacttime		(__in LPCTSTR IN_szRegPath);

};

#endif // Tacttime_h__