#pragma once

#include "Enumerator/IEnumerator.h"

namespace lt
{
	template<typename TypeValue>
	class CEnumerator final : public IEnumerator<TypeValue>
	{
	public:
		using Iterator = IEnumerator<TypeValue>::Iterator;
		using ConstIterator = IEnumerator<TypeValue>::ConstIterator;
		using Cntr = typename Iterator::Cntr;

		CEnumerator(Cntr * pCntr)
		: m_pCntr(pCntr)
		{
		}

		~CEnumerator() = default;

		size_t GetCount() const override
		{
			return m_pCntr->GetCount();
		}

		Iterator begin() override
		{
			return Iterator(m_pCntr, 0);
		}

		ConstIterator begin() const override
		{
			return ConstIterator(m_pCntr, 0);
		}

		Iterator end() override
		{
			return Iterator(m_pCntr, m_pCntr->GetCount());
		}

		ConstIterator end() const override
		{
			return ConstIterator(m_pCntr, m_pCntr->GetCount());
		}

	private:
		Cntr * m_pCntr = nullptr;
	};
}
