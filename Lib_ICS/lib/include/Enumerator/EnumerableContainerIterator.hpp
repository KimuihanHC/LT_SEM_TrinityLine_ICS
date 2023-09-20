#pragma once

#include "Enumerator/IEnumerableContainer.h"

namespace lt
{
	template<typename TypeValue>
	class CEnumerableContainerIterator final
	{
	public:
		using Cntr = IEnumerableContainer<TypeValue>;
		using Value = typename Cntr::Value;

		CEnumerableContainerIterator(Cntr * pCntr, size_t index)
		: m_pCntr(pCntr)
		, m_index(index)
		{
		}

		CEnumerableContainerIterator(const CEnumerableContainerIterator & rhs)
		: CEnumerableContainerIterator(rhs.m_pCntr, rhs.m_index)
		{
		}

		~CEnumerableContainerIterator() = default;

		CEnumerableContainerIterator & operator=(const CEnumerableContainerIterator & rhs)
		{
			m_pCntr = rhs.m_pCntr;
			m_index = rhs.m_index;

			return *this;
		}

		bool operator==(const CEnumerableContainerIterator & rhs) const
		{
			return m_pCntr == rhs.m_pCntr &&
				   m_index == rhs.m_index;
		}

		bool operator!=(const CEnumerableContainerIterator & rhs) const
		{
			return !(*this == rhs);
		}

		CEnumerableContainerIterator operator+(size_t offset)
		{
			return CEnumerableContainerIterator(m_pCntr, m_index + offset);
		}

		CEnumerableContainerIterator & operator++()
		{
			++m_index;

			return *this;
		}

		CEnumerableContainerIterator operator++(int)
		{
			CEnumerableContainerIterator iter(*this);

			++*this;

			return iter;
		}

		CEnumerableContainerIterator & operator+=(size_t offset)
		{
			m_index += offset;

			return *this;
		}

		CEnumerableContainerIterator operator-(size_t offset)
		{
			return CEnumerableContainerIterator(m_pCntr, m_index - offset);
		}

		CEnumerableContainerIterator operator--()
		{
			--m_index;

			return *this;
		}

		CEnumerableContainerIterator operator--(int)
		{
			CEnumerableContainerIterator iter(*this);

			--*this;

			return iter;
		}

		CEnumerableContainerIterator & operator-=(size_t offset)
		{
			m_index -= offset;

			return *this;
		}

		Value & operator*()
		{
			return (*m_pCntr)[m_index];
		}

	private:
		Cntr * m_pCntr = nullptr;
		size_t m_index = 0;
	};

	template<typename TypeValue>
	class CEnumerableContainerConstIterator final
	{
	public:
		using Cntr = const IEnumerableContainer<TypeValue>;
		using ConstValue = typename Cntr::ConstValue;

		CEnumerableContainerConstIterator(Cntr * pCntr, size_t index)
		: m_pCntr(pCntr)
		, m_index(index)
		{
		}

		CEnumerableContainerConstIterator(const CEnumerableContainerConstIterator & rhs)
		: CEnumerableContainerConstIterator(rhs.m_pCntr, rhs.m_index)
		{
		}

		~CEnumerableContainerConstIterator() = default;

		CEnumerableContainerConstIterator & operator=(const CEnumerableContainerConstIterator & rhs)
		{
			m_pCntr = rhs.m_pCntr;
			m_index = rhs.m_index;

			return *this;
		}

		bool operator==(const CEnumerableContainerConstIterator & rhs) const
		{
			return m_pCntr == rhs.m_pCntr &&
				   m_index == rhs.m_index;
		}

		bool operator!=(const CEnumerableContainerConstIterator & rhs) const
		{
			return !(*this == rhs);
		}

		CEnumerableContainerConstIterator operator+(size_t offset)
		{
			return CEnumerableContainerConstIterator(m_pCntr, m_index + offset);
		}

		CEnumerableContainerConstIterator & operator++()
		{
			++m_index;

			return *this;
		}

		CEnumerableContainerConstIterator operator++(int)
		{
			CEnumerableContainerConstIterator iter(*this);

			++*this;

			return iter;
		}

		CEnumerableContainerConstIterator & operator+=(size_t offset)
		{
			m_index += offset;

			return *this;
		}

		CEnumerableContainerConstIterator operator-(size_t offset)
		{
			return CEnumerableContainerConstIterator(m_pCntr, m_index - offset);
		}

		CEnumerableContainerConstIterator operator--()
		{
			--m_index;

			return *this;
		}

		CEnumerableContainerConstIterator operator--(int)
		{
			CEnumerableContainerConstIterator iter(*this);

			--*this;

			return iter;
		}

		CEnumerableContainerConstIterator & operator-=(size_t offset)
		{
			m_index -= offset;

			return *this;
		}

		ConstValue & operator*() const
		{
			return (*m_pCntr)[m_index];
		}

	private:
		Cntr * m_pCntr = nullptr;
		size_t m_index = 0;
	};
}
