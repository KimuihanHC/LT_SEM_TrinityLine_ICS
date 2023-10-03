#pragma once

#include "Event/IEventArgs.h"

namespace lt
{
	template<typename TypeSender, typename TypeArgs>
	class CEventArgs : public IEventArgs<TypeSender, TypeArgs>
	{
	public:
		using Sender = TypeSender;
		using Args = TypeArgs;

		CEventArgs(Sender & sender, Args & args)
		: m_sender(sender)
		, m_args(args)
		{
		}

		CEventArgs(const CEventArgs &) = delete;
		virtual ~CEventArgs() = default;

		CEventArgs & operator=(const CEventArgs &) = delete;

		Sender & GetSender() override
		{
			return m_sender;
		}

		Args & GetArgs() override
		{
			return m_args;
		}

	private:
		Sender & m_sender;
		Args & m_args;
	};
}
