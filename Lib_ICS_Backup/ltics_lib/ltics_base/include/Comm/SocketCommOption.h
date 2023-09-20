#pragma once

#include "Comm/CommOption.h"

namespace lt
{
	#pragma pack(push, 1)
	struct SSocketCommOption : public SCommOption
	{
		uintPtr socket = 0;
		ushort localPort = 0;
		tchar localAddress[16] = {};
		ushort remotePort = 0;
		tchar remoteAddress[16] = {};
	};
	#pragma pack(pop)
}
