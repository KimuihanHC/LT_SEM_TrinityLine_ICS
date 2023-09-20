#pragma once

namespace lt
{
	template<typename TypeValue>
	class IEnumerableContainer abstract
	{
	public:
		using Value = TypeValue;
		using ConstValue = const Value;

		virtual ~IEnumerableContainer() = default;

		virtual Value & operator[](size_t index) = 0;
		virtual ConstValue & operator[](size_t index) const = 0;

		virtual size_t GetCount() const = 0;
	};
}
