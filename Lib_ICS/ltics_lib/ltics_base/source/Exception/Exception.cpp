#include "pch.h"
#include "Exception/Exception.h"
#include "Util/MemoryUtil.hpp"
#include "Util/StringUtil.hpp"

using namespace lt;

CException::CException(ConstStringT strFile,
					   ConstStringT strFunction,
					   uint line)
: m_line(line)
{
	m_pStrReason = new StdStringT();
	m_pStrFile = new StdStringT(strFile);
	m_pStrFunction = new StdStringT(strFunction);
	m_pStrMessage = new StdStringT();
}

CException::CException(ConstStringT strReason,
					   ConstStringT strFile,
					   ConstStringT strFunction,
					   uint line)
: m_line(line)
{
	m_pStrReason = new StdStringT(strReason);
	m_pStrFile = new StdStringT(strFile);
	m_pStrFunction = new StdStringT(strFunction);
	m_pStrMessage = new StdStringT();
}

CException::CException(const CException & rhs)
: m_line(rhs.m_line)
{
	m_pStrReason = new StdStringT(*rhs.m_pStrReason);
	m_pStrFile = new StdStringT(*rhs.m_pStrFile);
	m_pStrFunction = new StdStringT(*rhs.m_pStrFunction);
	m_line = rhs.m_line;
	m_pStrMessage = new StdStringT(*rhs.m_pStrMessage);
}

CException::CException(CException && rhs)
{
	Swap(m_pStrReason, rhs.m_pStrReason);
	Swap(m_pStrFile, rhs.m_pStrFile);
	Swap(m_pStrFunction, rhs.m_pStrFunction);
	Swap(m_line, rhs.m_line);
	Swap(m_pStrMessage, rhs.m_pStrMessage);
}

CException::~CException()
{
	SafeDelete(m_pStrReason);
	SafeDelete(m_pStrFile);
	SafeDelete(m_pStrFunction);
	SafeDelete(m_pStrMessage);
}

CException & CException::operator=(const CException & rhs)
{
	*m_pStrReason = *rhs.m_pStrReason;
	*m_pStrFile = *rhs.m_pStrFile;
	*m_pStrFunction = *rhs.m_pStrFunction;
	m_line = rhs.m_line;
	*m_pStrMessage = *rhs.m_pStrMessage;

	return *this;
}

CException & CException::operator=(CException && rhs)
{
	SafeDelete(m_pStrReason);
	SafeDelete(m_pStrFile);
	SafeDelete(m_pStrFunction);
	m_line = 0;
	SafeDelete(m_pStrMessage);

	Swap(m_pStrReason, rhs.m_pStrReason);
	Swap(m_pStrFile, rhs.m_pStrFile);
	Swap(m_pStrFunction, rhs.m_pStrFunction);
	Swap(m_line, rhs.m_line);
	Swap(m_pStrMessage, rhs.m_pStrMessage);

	return *this;
}

ConstStringT CException::GetReason() const
{
	return m_pStrReason->c_str();
}

ConstStringT CException::GetFile() const
{
	return m_pStrFile->c_str();
}

ConstStringT CException::GetFunction() const
{
	return m_pStrFunction->c_str();
}

uint CException::GetLine() const
{
	return m_line;
}

ConstStringT CException::GetMessage() const
{
	if(m_pStrMessage->empty())
	{
		*m_pStrMessage = std::move(Format(_T("%s\r\n%s (%d) : %s"),
										  GetReason(),
										  GetFile(),
										  GetLine(),
										  GetFunction()));
	}

	return m_pStrMessage->c_str();
}

void CException::SetReason(ConstStringT strReason)
{
	*m_pStrReason = strReason;
}
