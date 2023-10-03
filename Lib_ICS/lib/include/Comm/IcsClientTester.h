#pragma once

#include "Comm/IcsClient.h"

namespace lt
{
	class LTICS_LIB_CLASS CIcsClientTester final : public CIcsClient
	{
	public:
		CIcsClientTester() = default;
		CIcsClientTester(const CIcsClientTester &) = delete;
		~CIcsClientTester() = default;

		CIcsClientTester & operator=(const CIcsClientTester &) = delete;

		//////////////////////////////////////////////////////////////////////////////////
		// Operation
		uint32 CommandTestResult(const STestResult & testResult,
								 uint64 timeout);
		//
		//////////////////////////////////////////////////////////////////////////////////
	};
}
