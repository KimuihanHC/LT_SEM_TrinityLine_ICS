#pragma once

#include "Comm/IcsRequestTypedArgs.h"

namespace lt
{
	template<typename TypeArgs>
	class CIcsRequestTypedArgsImpl final : public CIcsRequestTypedArgs<TypeArgs>
	{
	public:
		using Args = CIcsRequestTypedArgs<TypeArgs>::Args;

		CIcsRequestTypedArgsImpl(Args & args)
		: CIcsRequestTypedArgs()
		, m_args(args)
		{
		}

		~CIcsRequestTypedArgsImpl() = default;

		Args & GetTypedArgs() const override
		{
			return m_args;
		}

	private:
		Args & m_args;
	};
}
