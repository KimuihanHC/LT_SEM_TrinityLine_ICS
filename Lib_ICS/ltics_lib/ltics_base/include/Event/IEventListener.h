#pragma once

namespace lt
{
	template<typename TypeEventArgs>
	class IEventListener abstract
	{
	public:
		using EventArgs = TypeEventArgs;

		virtual ~IEventListener() = default;

		virtual void operator()(EventArgs & eventArgs) const = 0;
	};
}
