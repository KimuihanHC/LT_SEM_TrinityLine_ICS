#pragma once

#include "Event/ICancellableEventArgs.h"

namespace lt
{
	template<typename TypeSender, typename TypeArgs>
	class CCancellableEventArgs final : public ICancellableEventArgs<TypeSender, TypeArgs>
	{
	public:
		using Sender = TypeSender;
		using Args = TypeArgs;

		CCancellableEventArgs(Sender & sender, Args & args)
		: m_sender(sender)
		, m_args(args)
		{
		}

		CCancellableEventArgs(const CCancellableEventArgs &) = delete;
		~CCancellableEventArgs() = default;

		CCancellableEventArgs & operator=(const CCancellableEventArgs &) = delete;

		bool IsCancelled() const
		{
			return m_bCancelled;
		}

		void Cancel() override
		{
			m_bCancelled = true;
		}

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
		bool m_bCancelled = false;
	};
}
