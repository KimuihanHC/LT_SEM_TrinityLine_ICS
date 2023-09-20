#pragma once

#include "Comm/IcsRemote.h"

namespace lt
{
	class LTICS_LIB_CLASS CIcsRemoteLoUnloader final : public CIcsRemote
	{
	public:
		CIcsRemoteLoUnloader();
		CIcsRemoteLoUnloader(const CIcsRemoteLoUnloader &) = delete;
		~CIcsRemoteLoUnloader();

		CIcsRemoteLoUnloader & operator=(const CIcsRemoteLoUnloader &) = delete;

		//////////////////////////////////////////////////////////////////////////////////
		// Operation
		LoadingEventNotifier<CIcsRemoteLoUnloader> &
			GetLoadingEventNotifier();
		TestResultRequestEventNotifier<CIcsRemoteLoUnloader> &
			GetTestResultRequestEventNotifier();
		UnloadingEventNotifier<CIcsRemoteLoUnloader> &
			GetUnloadingEventNotifier();
		//
		//////////////////////////////////////////////////////////////////////////////////

	private:
		//////////////////////////////////////////////////////////////////////////////////
		// Operation
		bool ProcRequestLoading(const DataCntr & dataCntr,
								DataCntr & resultDataCntr);
		bool ProcRequestTestResultRequest(const DataCntr & dataCntr,
										  DataCntr & resultDataCntr);
		bool ProcRequestUnloading(const DataCntr & dataCntr,
								  DataCntr & resultDataCntr);

		bool RaiseLoadingEvent(LoadingArgs::Args & args);
		bool RaiseTestResultRequestEvent(TestResultRequestArgs::Args & args);
		bool RaiseUnloadingEvent(UnloadingArgs::Args & args);
		//
		//////////////////////////////////////////////////////////////////////////////////

	private:
		//////////////////////////////////////////////////////////////////////////////////
		// Operation
		using LoadingEventNotifierImpl =
			CCancellableEventNotifier<CIcsRemoteLoUnloader, LoadingArgs>;
		using TestResultRequestEventNotifierImpl =
			CCancellableEventNotifier<CIcsRemoteLoUnloader, TestResultRequestArgs>;
		using UnloadingEventNotifierImpl =
			CCancellableEventNotifier<CIcsRemoteLoUnloader, UnloadingArgs>;

		LoadingEventNotifierImpl * m_pLoadingEventNotifier = nullptr;
		TestResultRequestEventNotifierImpl * m_pTestResultRequestEventNotifier = nullptr;
		UnloadingEventNotifierImpl * m_pUnloadingEventNotifier = nullptr;
		//
		//////////////////////////////////////////////////////////////////////////////////
	};
}
