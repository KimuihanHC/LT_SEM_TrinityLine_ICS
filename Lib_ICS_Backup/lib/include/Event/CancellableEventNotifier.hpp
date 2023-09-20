 #pragma once

#include "Event/EventNotifier.hpp"
#include "Event/CancellableEventArgs.hpp"

namespace lt
{
	template<typename TypeSender, typename TypeArgs>
	class CCancellableEventNotifier : public CEventNotifier<ICancellableEventArgs<TypeSender, TypeArgs>>
	{
	public:
		using Sender = TypeSender;
		using Args = TypeArgs;

		CCancellableEventNotifier() = default;
		CCancellableEventNotifier(const CCancellableEventNotifier &) = delete;
		virtual ~CCancellableEventNotifier() = default;

		CCancellableEventNotifier & operator=(const CCancellableEventNotifier &) = delete;

	public:
		bool OnEvent(Sender & sender, Args & args)
		{
			CCancellableEventArgs<Sender, Args> eventArgs(sender, args);

			RaiseEvent(eventArgs);

			return !eventArgs.IsCancelled();
		}
		//2023.06.25a uhkim
		void OnEESEvent(Sender & sender, Args & args)
		{
			CCancellableEventArgs<Sender, Args> eventArgs(sender, args);

			RaiseEvent(eventArgs);

			//return !eventArgs.IsCancelled();
		}
	};
}
