#pragma once

namespace lt
{
	template<typename TypeSender, typename TypeArgs>
	class IEventArgs abstract
	{
	public:
		using Sender = TypeSender;
		using Args = TypeArgs;

		virtual ~IEventArgs() = default;

		virtual Sender & GetSender() = 0;

		virtual Args & GetArgs() = 0;
	};
}
