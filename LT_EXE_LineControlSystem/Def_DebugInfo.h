
#ifndef Def_DebugInfo_h__
#define Def_DebugInfo_h__

#pragma once

#include <stdint.h>

// ICS 운영 할 때 여러 옵션들 추가

// 커맨드 라인 매개변수를 /Debug_ICS 형태로 입력 했을때?
typedef enum
{
	Deb_NotUse,		// 미 사용
	Deb_Use,		// 사용
	Deb_MaxCount,
}enDebugUsable;

typedef enum
{
	TO_Default,		// 기본
	TO_ForcedAccept,// 배출 강제 승인

	TO_MaxCount,
}enSocketTrackOut_Cond;

typedef enum
{
	WTO_Default,	// 기본
	WTO_Use,

	WTO_MaxCount,
}enWaitingSocket_Cond;

typedef enum
{
	MTSel_Default,	// 기본
	MTSel_Far,		// 먼 설비 우선
	MTSel_Near,		// 가까운 설비 우선
	MTSel_Duration,	// 투입 시간이 오래된 설비 우선

	MTSel_MaxCount,
}enMultiTester_SelMethod;

typedef struct _tag_DebugInfo
{
	// 디버깅 모드 On/Off
	bool	m_bUse_DebugMode			= false;

	// NG 판정받은 제품을 bypass 처리하지 않음
	bool	m_bUse_NotBypass_NG			= false;

	// Process Skip Info에서 검사 skip 설정된 검사를 skip하지 않음.
	bool	m_bUse_NotBypass_TestSkip	= false;

	// 빈 소켓도 검사 구역에 투입
	bool	m_bUse_NotBypass_Empty		= false;

	// Shift 변경시 자동 수율 초기화
	bool	m_AutoReset_Yield_byShift	= false;

	// 소켓 배출 판단 조건
	uint8_t	m_nSocketTrackOut_Condition = 0;

	// 배출 대기 중 소켓 처리 조건
	uint8_t	m_nWaitingSocket_Condition	= 0;

	// 멀티 검사 설비 선택 방식
	uint8_t	m_nMultiTester_SelectMethod = 0;


	_tag_DebugInfo& operator= (const _tag_DebugInfo& ref)
	{
		m_bUse_DebugMode			= ref.m_bUse_DebugMode;
		m_bUse_NotBypass_NG			= ref.m_bUse_NotBypass_NG;
		m_bUse_NotBypass_TestSkip	= ref.m_bUse_NotBypass_TestSkip;
		m_bUse_NotBypass_Empty		= ref.m_bUse_NotBypass_Empty;
		m_AutoReset_Yield_byShift	= ref.m_AutoReset_Yield_byShift;
		m_nSocketTrackOut_Condition = ref.m_nSocketTrackOut_Condition;
		m_nWaitingSocket_Condition	= ref.m_nWaitingSocket_Condition;
		m_nMultiTester_SelectMethod = ref.m_nMultiTester_SelectMethod;

		return *this;
	}

	void Reset()
	{
		m_bUse_DebugMode			= false;
		m_bUse_NotBypass_NG			= false;
		m_bUse_NotBypass_TestSkip	= false;
		m_bUse_NotBypass_Empty		= false;
		m_AutoReset_Yield_byShift	= false;
		m_nSocketTrackOut_Condition = 0;
		m_nWaitingSocket_Condition	= 0;
		m_nMultiTester_SelectMethod = 0;
	}

	bool	Is_DebugMode() const
	{
		return m_bUse_DebugMode;
	};

	bool	Is_NotBypass_NG() const
	{
		return (m_bUse_DebugMode && m_bUse_NotBypass_NG);
	};

	bool	Is_NotBypass_TestSkip() const
	{
		return (m_bUse_DebugMode && m_bUse_NotBypass_TestSkip);
	};

	bool	Is_Use_NotBypass_Empty() const
	{
		return (m_bUse_DebugMode && m_bUse_NotBypass_Empty);
	}

	bool	Is_AutoReset_Yield_byShift() const
	{
		return (m_bUse_DebugMode && m_AutoReset_Yield_byShift);
	};

	uint8_t	Get_SocketTrackOut_Condition() const
	{
		if (m_bUse_DebugMode)
			return m_nSocketTrackOut_Condition;
		else
			return 0;
	}

	uint8_t	Get_WaitingSocket_Condition() const
	{
		if (m_bUse_DebugMode)
			return m_nWaitingSocket_Condition;
		else
			return 0;
	}

	uint8_t	Get_MultiTester_SelectMethod() const
	{
		if (m_bUse_DebugMode)
			return m_nMultiTester_SelectMethod;
		else
			return 0;
	}

}ST_DebugInfo;

#endif // Def_DebugInfo_h__