#include "pch.h"
#include "Comm/IcsRemoteLoUnloader.h"
#include "IcsRequestEvent.hpp"
#include "Util/StringUtil.hpp"

using namespace lt;

struct ReceivingCommand
{
	//////////////////////////////////////////////////////////////////////////////////
	// Operation
	static constexpr uint16 Loading = 0x0320;
	static constexpr uint16 TestResultRequest = 0x0321;
	static constexpr uint16 Unloading = 0x0323;
	//
	//////////////////////////////////////////////////////////////////////////////////
};

CIcsRemoteLoUnloader::CIcsRemoteLoUnloader()
: CIcsRemote()
{
	//////////////////////////////////////////////////////////////////////////////////
	// Operation
	m_pLoadingEventNotifier = new LoadingEventNotifierImpl();
	m_pTestResultRequestEventNotifier = new TestResultRequestEventNotifierImpl();
	m_pUnloadingEventNotifier = new UnloadingEventNotifierImpl();

	AddRequestProcedure(ReceivingCommand::Loading,
						reinterpret_cast<RequestProc>(
							&CIcsRemoteLoUnloader::ProcRequestLoading));
	AddRequestProcedure(ReceivingCommand::TestResultRequest,
						reinterpret_cast<RequestProc>(
							&CIcsRemoteLoUnloader::ProcRequestTestResultRequest));
	AddRequestProcedure(ReceivingCommand::Unloading,
						reinterpret_cast<RequestProc>(
							&CIcsRemoteLoUnloader::ProcRequestUnloading));
	//
	//////////////////////////////////////////////////////////////////////////////////
}

CIcsRemoteLoUnloader::~CIcsRemoteLoUnloader()
{
	//////////////////////////////////////////////////////////////////////////////////
	// Operation
	delete m_pLoadingEventNotifier;
	delete m_pTestResultRequestEventNotifier;
	delete m_pUnloadingEventNotifier;
	//
	//////////////////////////////////////////////////////////////////////////////////
}

bool CIcsRemoteLoUnloader::ProcRequestLoading(const DataCntr & dataCntr,
											 DataCntr & /*resultDataCntr*/)
{
	if(dataCntr.size() < 2)
		return false;

	return RaiseLoadingEvent(
		{
			ToTypeChar(dataCntr[0]).c_str(),
			ToTypeChar(dataCntr[1]).c_str()
		}
	);
}

bool CIcsRemoteLoUnloader::ProcRequestTestResultRequest(const DataCntr & dataCntr,
													   DataCntr & resultDataCntr)
{
	if(dataCntr.empty())
		return false;

	auto rfid = ToTypeChar(dataCntr[0]);
	TestResultRequestArgs::Args args = { rfid.c_str(), };
	auto result = RaiseTestResultRequestEvent(args);

	resultDataCntr = {
		Format("%d", args.result),
		Format("%d", args.para),
		#if defined(UNICODE)
		ToMultiByte(args.equipmentName)
		#else
		args.equipment
		#endif
	};

	return result;
}

bool CIcsRemoteLoUnloader::ProcRequestUnloading(const DataCntr & dataCntr,
											   DataCntr & /*resultDataCntr*/)
{
	if(dataCntr.empty())
		return false;

	return RaiseUnloadingEvent(
		{
			ToTypeChar(dataCntr[0]).c_str()
		}
	);
}

bool CIcsRemoteLoUnloader::RaiseLoadingEvent(LoadingArgs::Args & args)
{
	return RaiseEvent(*this,
					  *m_pLoadingEventNotifier,
					  args);
}

bool CIcsRemoteLoUnloader::RaiseTestResultRequestEvent(TestResultRequestArgs::Args & args)
{
	return RaiseEvent(*this,
					  *m_pTestResultRequestEventNotifier,
					  args);
}

bool CIcsRemoteLoUnloader::RaiseUnloadingEvent(UnloadingArgs::Args & args)
{
	return RaiseEvent(*this,
					  *m_pUnloadingEventNotifier,
					  args);
}

LoadingEventNotifier<CIcsRemoteLoUnloader> &
CIcsRemoteLoUnloader::GetLoadingEventNotifier()
{
	return *m_pLoadingEventNotifier;
}

TestResultRequestEventNotifier<CIcsRemoteLoUnloader> &
CIcsRemoteLoUnloader::GetTestResultRequestEventNotifier()
{
	return *m_pTestResultRequestEventNotifier;
}

UnloadingEventNotifier<CIcsRemoteLoUnloader> &
CIcsRemoteLoUnloader::GetUnloadingEventNotifier()
{
	return *m_pUnloadingEventNotifier;
}
