#pragma once

#include "IcsRemoteEquipment.h"

namespace lt
{
	class CIcsRemoteLoUnloader;
}

class CEqpLoader;

class CIcsRemoteEquipmentLoUnloader final : public CIcsRemoteEquipment
{
public:
	CIcsRemoteEquipmentLoUnloader(CEqpLoader & equipment);
	CIcsRemoteEquipmentLoUnloader(const CIcsRemoteEquipmentLoUnloader &) = delete;
	~CIcsRemoteEquipmentLoUnloader();

	CIcsRemoteEquipmentLoUnloader & operator=(const CIcsRemoteEquipmentLoUnloader &) = delete;

protected:
	lt::CIcsRemote & GetRemote() override;
	const lt::CIcsRemote & GetRemote() const override;

private:
	CEqpLoader & GetEquipmentLoader();
	const CEqpLoader & GetEquipmentLoader() const;

	lt::CIcsRemoteLoUnloader & GetRemoteLoUnloader();
	const lt::CIcsRemoteLoUnloader & GetRemoteLoUnloader() const;

	using LoadingEventArgs =
		lt::LoadingEventArgs<lt::CIcsRemoteLoUnloader>;
	using TestResultRequestEventArgs =
		lt::TestResultRequestEventArgs<lt::CIcsRemoteLoUnloader>;
	using UnloadingEventArgs =
		lt::UnloadingEventArgs<lt::CIcsRemoteLoUnloader>;

	void OnLoadingEvent(LoadingEventArgs & eventArgs);
	void OnTestResultRequestEvent(TestResultRequestEventArgs & eventArgs);
	void OnUnloadingEvent(UnloadingEventArgs & eventArgs);

private:
	CEqpLoader & m_equipment;

	lt::CIcsRemoteLoUnloader * m_pRemote = nullptr;

	using LoadingEventListener =
		lt::CEventMfnListener<CIcsRemoteEquipmentLoUnloader, LoadingEventArgs>;
	using TestResultRequestEventListener =
		lt::CEventMfnListener<CIcsRemoteEquipmentLoUnloader, TestResultRequestEventArgs>;
	using UnloadingEventListener =
		lt::CEventMfnListener<CIcsRemoteEquipmentLoUnloader, UnloadingEventArgs>;

	LoadingEventListener * m_pLoadingEventListener = nullptr;
	TestResultRequestEventListener * m_pTestResultRequestEventListener = nullptr;
	UnloadingEventListener * m_pUnloadingEventListener = nullptr;
};
