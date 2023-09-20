#pragma once

#include "Event/IEventListener.h"

namespace lt
{
	template<typename TypeOwner, typename TypeEventArgs>
	class CEventMfnListener : public IEventListener<TypeEventArgs>
	{
	public:
		using Owner = TypeOwner;
		using EventArgs = TypeEventArgs;
		using MfnEvent = void (Owner::*)(EventArgs &);

		CEventMfnListener(Owner * owner, MfnEvent mfn)
		: m_owner(owner)
		, m_mfn(mfn)
		{
		}

		CEventMfnListener(const CEventMfnListener & rhs)
		: m_owner(rhs.m_owner)
		, m_mfn(rhs.m_mfn)
		{
		}

		CEventMfnListener(CEventMfnListener && rhs)
		: m_owner(rhs.m_owner)
		, m_mfn(rhs.m_mfn)
		{
			rhs.m_owner = nullptr;
			rhs.m_mfn = nullptr;
		}

		virtual ~CEventMfnListener() = default;

		CEventMfnListener & operator=(const CEventMfnListener & rhs)
		{
			m_owner = rhs.m_owner;
			m_mfn = rhs.m_mfn;

			return *this;
		}

		CEventMfnListener & operator=(CEventMfnListener && rhs)
		{
			m_owner = rhs.m_owner;
			m_mfn = rhs.m_mfn;

			rhs.m_owner = nullptr;
			rhs.m_mfn = nullptr;

			return *this;
		}

		void operator()(EventArgs & eventArgs) const override
		{
			if(m_mfn != nullptr)
				(m_owner->*m_mfn)(eventArgs);
		}

	private:
		Owner * m_owner = nullptr;
		MfnEvent m_mfn = nullptr;
	};
}
