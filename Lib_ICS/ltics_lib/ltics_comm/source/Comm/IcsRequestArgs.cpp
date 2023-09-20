#include "pch.h"
#include "Comm/IcsRequestArgs.h"

using namespace lt;

CIcsRequestArgs & CIcsRequestArgs::SetResult(bool result)
{
	m_result = result;

	return *this;
}

bool CIcsRequestArgs::GetResult() const
{
	return m_result;
}
