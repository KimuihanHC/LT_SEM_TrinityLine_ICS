#pragma once

#include "IcsRequestTypedArgsImpl.hpp"
#include "Event/CancellableEventNotifier.hpp"

namespace lt
{
	template<typename TypeSender, typename TypeArgs>
	bool RaiseEvent(TypeSender & sender,
					CCancellableEventNotifier<TypeSender, TypeArgs> & notifier)
	{
		CIcsRequestArgs requestArgs;

		notifier.OnEvent(sender, requestArgs);

		return requestArgs.GetResult();
	}

	template<typename TypeSender, typename TypeArgs>
	bool RaiseEvent(TypeSender & sender,
					CCancellableEventNotifier<TypeSender, TypeArgs> & notifier,
					typename TypeArgs::Args & args)
	{
		CIcsRequestTypedArgsImpl<
			std::remove_reference_t<decltype(args)>> requestArgs(args);

		notifier.OnEvent(sender, requestArgs);

		return requestArgs.GetResult();
	}
	//2023.06.25a uhkim [Add Raise Evente ConstStringA]
	template<typename TypeSender, typename TypeArgs>
	void EESRaiseEvent(TypeSender & sender,
		CCancellableEventNotifier<TypeSender, TypeArgs> & notifier)
	{
		CIcsRequestArgs requestArgs;

		notifier.OnEESEvent(sender, requestArgs);
	}
	template<typename TypeSender, typename TypeArgs>
	void EESRaiseEvent(TypeSender & sender,
		CCancellableEventNotifier<TypeSender, TypeArgs> & notifier,
		typename TypeArgs::Args & args)
	{
		CIcsRequestTypedArgsImpl<
			std::remove_reference_t<decltype(args)>> requestArgs(args);

		notifier.OnEESEvent(sender, requestArgs);
	}
}
