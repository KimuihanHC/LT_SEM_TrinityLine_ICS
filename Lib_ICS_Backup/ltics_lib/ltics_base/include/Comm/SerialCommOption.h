#pragma once

#include "Comm/CommOption.h"

namespace lt
{
	enum class SerialCommParity
	{
		None,
		Odd,
		Even,
		Mark,
		Space
	};

	enum class SerialCommStopBit
	{
		One,
		One5,
		Two
	};

	#pragma pack(push, 1)
	struct SSerialCommOption : public SCommOption
	{
		uint port = 1;
		uint baudRate = 115200;
		uint dataBits = 8;
		SerialCommParity parity = SerialCommParity::None;
		SerialCommStopBit stopBit = SerialCommStopBit::One;
	};
	#pragma pack(pop)
}
