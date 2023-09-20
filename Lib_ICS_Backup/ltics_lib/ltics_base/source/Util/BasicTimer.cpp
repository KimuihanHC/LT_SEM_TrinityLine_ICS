#include "pch.h"
#include "Util/BasicTimer.h"

using namespace lt;

CBasicTimer::CBasicTimer()
{
	Reset();
}

CBasicTimer::CBasicTimer(const CBasicTimer & rhs)
: m_prev(rhs.m_prev)
, m_start(rhs.m_start)
{
}

CBasicTimer & CBasicTimer::operator=(const CBasicTimer & rhs)
{
	m_prev = rhs.m_prev;
	m_start = rhs.m_start;

	return *this;
}

void CBasicTimer::Reset()
{
	m_prev = m_start = CBasicTimer::GetTimeStamp();
}

uint64 CBasicTimer::GetElapsedTime(Unit unit) const
{
	return CBasicTimer::GetElapsedTime(m_start, unit);
}

uint64 CBasicTimer::GetDeltaTime(Unit unit)
{
	auto current = CBasicTimer::GetTimeStamp();
	auto delta = CBasicTimer::GetElapsedTime(m_prev, current, unit);

	m_prev = current;

	return delta;
}

uint64 CBasicTimer::GetTimeStamp()
{
	uint64 current = 0;

	if(!::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER *>(&current)))
		current = ::GetTickCount64();

	return current;
}

uint64 CBasicTimer::GetFrequency()
{
	uint64 freq = 0;

	if(!::QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER *>(&freq)))
		freq = 1000;

	return freq;
}

uint64 CBasicTimer::GetElapsedTime(uint64 start, Unit unit)
{
	return CBasicTimer::GetElapsedTime(start, CBasicTimer::GetTimeStamp(), unit);
}

uint64 CBasicTimer::GetElapsedTime(uint64 start, uint64 end, Unit unit)
{
	static const double UnitMultipliers[] =
	{
		1.,
		1000.,
		1000000.,
		1000000000.
	};

	auto elapsed = static_cast<double>(end - start) /
				   static_cast<double>(CBasicTimer::GetFrequency());

	return static_cast<uint64>(
		elapsed * UnitMultipliers[static_cast<int>(unit)]);
}
