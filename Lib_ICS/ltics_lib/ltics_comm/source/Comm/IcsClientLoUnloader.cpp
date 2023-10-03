#include "pch.h"
#include "Comm/IcsClientLoUnloader.h"
#include "Util/StringUtil.hpp"
#include <vector>

using namespace lt;

struct SendingCommand
{
	//////////////////////////////////////////////////////////////////////////////////
	// Operation
	static constexpr uint16 Loading = 0x0320;
	static constexpr uint16 TestResultRequest = 0x0321;
	static constexpr uint16 Unloading = 0x0323;
	//
	//////////////////////////////////////////////////////////////////////////////////
};

uint32 CIcsClientLoUnloader::CommandLoading(const SSocketInfo & socketInfo,
											uint64 timeout)
{
	return ProcCommand(SendingCommand::Loading,
					   {
						   #if defined(UNICODE)
						   ToMultiByte(socketInfo.rfid),
						   ToMultiByte(socketInfo.productSn)
						   #else
						   socketInfo.rfid,
						   socketInfo.productSn
						   #endif
					   },
					   timeout);
}

uint32 CIcsClientLoUnloader::CommandTestResultRequest(STestResult2 & testResult,
													  uint64 timeout)
{
	DataCntr responseDataCntr;
	auto result = ProcCommand(SendingCommand::TestResultRequest,
							  {
								  #if defined(UNICODE)
								  ToMultiByte(testResult.rfid)
								  #else
								  testResult.rfid
								  #endif
							  },
							  timeout,
							  responseDataCntr);

	if(responseDataCntr.size() < 3)
		return ICS_COMM_WRONG_RESPONSE;

	if(result == ICS_COMM_OK)
	{
		testResult.result =
			static_cast<decltype(testResult.result)>(
				strtoul(responseDataCntr[0].c_str(), nullptr, 10));
		testResult.para =
			static_cast<decltype(testResult.para)>(
				strtoul(responseDataCntr[1].c_str(), nullptr, 10));
		_tcscpy_s(testResult.equipmentName,
				  _countof(testResult.equipmentName),
				  ToTypeChar(responseDataCntr[2]).c_str());
	}

	return result;
}

uint32 CIcsClientLoUnloader::CommandUnloading(const SSocketRfid & socketRfid,
											  uint64 timeout)
{
	return ProcCommand(SendingCommand::Unloading,
					   {
						   #if defined(UNICODE)
						   ToMultiByte(socketRfid.rfid)
						   #else
						   socketRfid.rfid
						   #endif
					   },
					   timeout);
}
