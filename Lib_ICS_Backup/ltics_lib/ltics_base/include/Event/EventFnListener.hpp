#pragma once

#include "Event/IEventListener.h"

namespace lt
{
	template<typename TypeEventArgs>
	class CEventFnListener : public IEventListener<TypeEventArgs>
	{
	public:
		using EventArgs = TypeEventArgs;
		using FnEvent = void (*)(EventArgs &);

		CEventFnListener(FnEvent fn)
		: m_fn(fn)
		{
		}

		CEventFnListener(const CEventFnListener & rhs)
		: m_fn(rhs.m_fn)
		{
		}

		CEventFnListener(CEventFnListener && rhs)
		: m_fn(rhs.m_fn)
		{
			rhs.m_fn = nullptr;
		}

		virtual ~CEventFnListener() = default;

		CEventFnListener & operator=(const CEventFnListener & rhs)
		{
			m_fn = rhs.m_fn;

			return *this;
		}

		CEventFnListener & operator=(CEventFnListener && rhs)
		{
			m_fn = rhs.m_fn;

			rhs.m_fn = nullptr;

			return *this;
		}

		void operator()(EventArgs & eventArgs) const override
		{
			if(m_fn != nullptr)
				m_fn(eventArgs);
		}

	private:
		FnEvent m_fn;
	};
}
