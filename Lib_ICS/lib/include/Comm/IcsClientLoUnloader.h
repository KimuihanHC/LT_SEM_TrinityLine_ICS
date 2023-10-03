#pragma once

#include "Comm/IcsClient.h"

namespace lt
{
	class LTICS_LIB_CLASS CIcsClientLoUnloader final : public CIcsClient
	{
	public:
		CIcsClientLoUnloader() = default;
		CIcsClientLoUnloader(const CIcsClientLoUnloader &) = delete;
		~CIcsClientLoUnloader() = default;

		CIcsClientLoUnloader & operator=(const CIcsClientLoUnloader &) = delete;

		//////////////////////////////////////////////////////////////////////////////////
		// Operation
		uint32 CommandLoading(const SSocketInfo & socketInfo,
							  uint64 timeout);

		uint32 CommandTestResultRequest(STestResult2 & socketRfid,
										uint64 timeout);

		uint32 CommandUnloading(const SSocketRfid & socketRfid,
								uint64 timeout);
		//
		//////////////////////////////////////////////////////////////////////////////////
	};
}
