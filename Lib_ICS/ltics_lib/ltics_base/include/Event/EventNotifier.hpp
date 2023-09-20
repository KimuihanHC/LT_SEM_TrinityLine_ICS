#pragma once

#include "Event/IEventNotifier.h"
#include "TypeDecl.h"
#include "Util/SyncUtil.hpp"
#include <vector>
#include <mutex>

namespace lt
{
	template<typename TypeEventArgs>
	class CEventNotifier : public IEventNotifier<TypeEventArgs>
	{
	private:
		using Base = IEventNotifier<TypeEventArgs>;

	public:
		using EventArgs = Base::EventArgs;
		using EventListener = Base::EventListener;
		using EventListenerPtr = Base::EventListenerPtr;

	private:
		using Cntr = typename StdSequenceContainer<EventListenerPtr>::Vector;

	public:
		CEventNotifier() = default;
		CEventNotifier(const CEventNotifier &) = delete;
		virtual ~CEventNotifier() = default;

		CEventNotifier & operator=(const CEventNotifier &) = delete;

	protected:
		void RaiseEvent(EventArgs & eventArgs)
		{
			SimpleLockRef(m_cntrMutex);

			for(auto & ref : m_cntr)
				(*ref)(eventArgs);
		}

	public:
		IEventNotifier & AddEventListener(EventListenerPtr pListener) override
		{
			SimpleLockRef(m_cntrMutex);

			m_cntr.emplace_back(pListener);

			return *this;
		}

		IEventNotifier & RemoveEventListener(EventListenerPtr pListener) override
		{
			SimpleLockRef(m_cntrMutex);

			auto iter = std::find(m_cntr.begin(), m_cntr.end(), pListener);

			if(iter != m_cntr.end())
				m_cntr.erase(iter);

			return *this;
		}

		IEventNotifier & operator+=(EventListenerPtr pListener) override
		{
			return AddEventListener(pListener);
		}

		IEventNotifier & operator-=(EventListenerPtr pListener) override
		{
			return RemoveEventListener(pListener);
		}

	private:
		Cntr m_cntr;
		StdRecursiveMutex m_cntrMutex;
	};
}
