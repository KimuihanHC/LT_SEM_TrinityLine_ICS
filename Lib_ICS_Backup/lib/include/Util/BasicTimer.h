#pragma once

#include "ltics_type.h"
#include "TypeDecl.h"

namespace lt
{
	class LTICS_LIB_CLASS CBasicTimer final
	{
	public:
		enum class Unit
		{
			Seconds,
			Milliseconds,
			Micoseconds,
			Nanoseconds
		};

		CBasicTimer();
		CBasicTimer(const CBasicTimer & rhs);
		~CBasicTimer() = default;

		CBasicTimer & operator=(const CBasicTimer & rhs);

		void Reset();

		uint64 GetElapsedTime(Unit unit) const;
		uint64 GetDeltaTime(Unit unit);

	public:
		static uint64 GetTimeStamp();
		static uint64 GetFrequency();

		static uint64 GetElapsedTime(uint64 start, Unit unit);
		static uint64 GetElapsedTime(uint64 start, uint64 end, Unit unit);

	private:
		uint64 m_prev = 0;
		uint64 m_start = 0;
	};
}
