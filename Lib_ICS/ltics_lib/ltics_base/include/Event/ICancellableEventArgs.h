#pragma once

#include "Event/IEventArgs.h"

namespace lt
{
	template<typename TypeSender, typename TypeArgs>
	class ICancellableEventArgs abstract : public IEventArgs<TypeSender, TypeArgs>
	{
	public:
		virtual ~ICancellableEventArgs() = default;

		virtual void Cancel() = 0;
	};
}
