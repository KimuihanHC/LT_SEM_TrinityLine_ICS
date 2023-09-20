#pragma once

#include "ltics_type.h"
#include "TypeDecl.h"

namespace lt
{
	class LTICS_LIB_CLASS CException
	{
	protected:
		CException(ConstStringT strFile,
				   ConstStringT strFunction,
				   uint line);

	public:
		CException(ConstStringT strReason,
				   ConstStringT strFile,
				   ConstStringT strFunction,
				   uint line);
		CException(const CException & rhs);
		CException(CException && rhs);
		virtual ~CException();

		CException & operator=(const CException & rhs);
		CException & operator=(CException && rhs);

		ConstStringT GetReason() const;
		ConstStringT GetFile() const;
		ConstStringT GetFunction() const;
		uint GetLine() const;
		ConstStringT GetMessage() const;

	protected:
		void SetReason(ConstStringT strReason);

	private:
		StdStringT * m_pStrReason = nullptr;
		StdStringT * m_pStrFile = nullptr;
		StdStringT * m_pStrFunction = nullptr;
		uint m_line = 0;
		StdStringT * m_pStrMessage = nullptr;
	};
}

#define ThrowException(reason) \
	throw lt::CException((reason), \
						 __FILET__, \
						 __FUNCTIONT__, \
						 __LINE__)
