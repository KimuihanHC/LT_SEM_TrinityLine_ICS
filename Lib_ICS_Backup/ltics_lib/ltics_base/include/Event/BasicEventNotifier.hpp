#pragma once

#include "Event/EventNotifier.hpp"
#include "Event/EventArgs.hpp"

namespace lt
{
	template<typename TypeSender, typename TypeArgs>
	class CBasicEventNotifier : public CEventNotifier<IEventArgs<TypeSender, TypeArgs>>
	{
	public:
		using Sender = TypeSender;
		using Args = TypeArgs;

		CBasicEventNotifier() = default;
		CBasicEventNotifier(const CBasicEventNotifier &) = delete;
		virtual ~CBasicEventNotifier() = default;

		CBasicEventNotifier & operator=(const CBasicEventNotifier &) = delete;

	public:
		void OnEvent(Sender & sender, Args & args)
		{
			RaiseEvent(CEventArgs<Sender, Args>(sender, args));
		}
	};
}
