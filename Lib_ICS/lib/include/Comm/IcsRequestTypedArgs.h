#pragma once

#include "Comm/IcsRequestArgs.h"

namespace lt
{
	template<typename TypeArgs>
	class CIcsRequestTypedArgs abstract : public CIcsRequestArgs
	{
	public:
		using Args = TypeArgs;

		CIcsRequestTypedArgs() = default;
		virtual ~CIcsRequestTypedArgs() = default;

		virtual Args & GetTypedArgs() const = 0;
	};
}
