#pragma once

#include "ltics_type.h"
#include "TypeDecl.h"

namespace lt
{
	class LTICS_LIB_CLASS CIcsRequestArgs
	{
	public:
		CIcsRequestArgs() = default;
		virtual ~CIcsRequestArgs() = default;

		CIcsRequestArgs & SetResult(bool result);
		bool GetResult() const;

	private:
		bool m_result = false;
	};
}
