#pragma once

#include "Event/IEventListener.h"

namespace lt
{
	template<typename TypeEventArgs>
	class IEventNotifier abstract
	{
	public:
		using EventArgs = TypeEventArgs;
		using EventListener = IEventListener<EventArgs>;
		using EventListenerPtr = EventListener *;

		virtual ~IEventNotifier() = default;

		virtual IEventNotifier & AddEventListener(EventListenerPtr pListener) = 0;
		virtual IEventNotifier & RemoveEventListener(EventListenerPtr pListener) = 0;

		virtual IEventNotifier & operator+=(EventListenerPtr pListener) = 0;
		virtual IEventNotifier & operator-=(EventListenerPtr pListener) = 0;
	};
}
