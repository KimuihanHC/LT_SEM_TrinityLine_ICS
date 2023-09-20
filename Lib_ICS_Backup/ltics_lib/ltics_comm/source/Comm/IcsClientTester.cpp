#include "pch.h"
#include "Comm/IcsClientTester.h"
#include "IcsRequestEvent.hpp"
#include "Util/StringUtil.hpp"

using namespace lt;

struct SendingCommand
{
	//////////////////////////////////////////////////////////////////////////////////
	// Operation
	static constexpr uint16 TestResult = 0x0322;
	//
	//////////////////////////////////////////////////////////////////////////////////
};

uint32 CIcsClientTester::CommandTestResult(const STestResult & testResult,
										   uint64 timeout)
{
	return ProcCommand(SendingCommand::TestResult,
					   {
						   #if defined(UNICODE)
						   ToMultiByte(testResult.rfid),
						   #else
						   testResult.rfid,
						   #endif
						   Format("%d", testResult.result),
						   Format("%d", testResult.para)
					   },
					   timeout);
}
