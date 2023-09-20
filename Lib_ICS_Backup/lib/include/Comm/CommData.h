#pragma once

#include "TypeDecl.h"

namespace lt
{
	struct SCommData
	{
		bool bRxClear = false;
		const int8 * bufferPtr = nullptr;
		size_t sizeOfBuffer = 0;
	};

	struct SCommTxData
	{
		const int8 * bufferPtr = nullptr;
		size_t sizeOfBuffer = 0;
		size_t sizeOfSent = 0;
	};

	struct SCommRxData
	{
		const int8 * bufferPtr = nullptr;
		size_t sizeOfReceived = 0;
	};
}
