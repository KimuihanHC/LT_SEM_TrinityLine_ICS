#pragma once

#include "Enumerator/EnumerableContainerIterator.hpp"

namespace lt
{
	template<typename TypeValue>
	class IEnumerator abstract
	{
	public:
		using Value = TypeValue;
		using Iterator = CEnumerableContainerIterator<Value>;
		using ConstIterator = CEnumerableContainerConstIterator<Value>;

		virtual ~IEnumerator() = default;

		virtual size_t GetCount() const = 0;

		virtual Iterator begin() = 0;
		virtual ConstIterator begin() const = 0;

		virtual Iterator end() = 0;
		virtual ConstIterator end() const = 0;
	};
}
