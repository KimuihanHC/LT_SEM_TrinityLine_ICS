#pragma once

#include "ltics_type.h"
#include "TypeDecl.h"
#include "Enumerator/IEnumerableContainer.h"
#include "Enumerator/IEnumerator.h"
#include <minwinbase.h>

namespace lt
{
	template<typename TypeValue>
	class CEnumerator;

	class LTICS_LIB_CLASS CFileFinder final : public IEnumerableContainer<WIN32_FIND_DATA>
	{
	private:
		using Base = IEnumerableContainer<WIN32_FIND_DATA>;
		using Value = Base::Value;
		using ConstValue = Base::ConstValue;
		using Cntr = StdSequenceContainer<Value>::Vector;
		using FoundFilesImpl = CEnumerator<Value>;

	public:
		using FoundFiles = const IEnumerator<Value>;

	public:
		CFileFinder();
		CFileFinder(const CFileFinder &) = delete;
		~CFileFinder();

		CFileFinder & operator=(const CFileFinder &) = delete;
		FoundFiles & operator()(ConstStringT strPath);

		FoundFiles & GetFoundFiles() const;

	private:
		Value & operator[](size_t index) override;
		ConstValue & operator[](size_t index) const override;

		size_t GetCount() const override;

	private:
		Cntr * m_pCntr = nullptr;
		StdMutex * m_pCntrMutex = nullptr;
		FoundFilesImpl * m_pEnumerator = nullptr;
	};
}
