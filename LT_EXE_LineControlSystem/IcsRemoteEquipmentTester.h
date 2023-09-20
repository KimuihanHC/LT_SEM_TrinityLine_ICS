#pragma once

#include "IcsRemoteEquipment.h"

namespace lt
{
	class CIcsRemoteTester;
}

class CEqpTester;

class CIcsRemoteEquipmentTester final : public CIcsRemoteEquipment
{
public:
	CIcsRemoteEquipmentTester(CEqpTester & equipment);
	CIcsRemoteEquipmentTester(const CIcsRemoteEquipmentTester &) = delete;
	~CIcsRemoteEquipmentTester();

	CIcsRemoteEquipmentTester & operator=(const CIcsRemoteEquipmentTester &) = delete;

protected:
	lt::CIcsRemote & GetRemote() override;
	const lt::CIcsRemote & GetRemote() const override;

private:
	CEqpTester & GetEquipmentTester();
	const CEqpTester & GetEquipmentTester() const;

	lt::CIcsRemoteTester & GetRemoteTester();
	const lt::CIcsRemoteTester & GetRemoteTester() const;

	using TestResultEventArgs =
		lt::TestResultEventArgs<lt::CIcsRemoteTester>;

	void OnTestResultEvent(TestResultEventArgs & eventArgs);

private:
	CEqpTester & m_equipment;

	lt::CIcsRemoteTester * m_pRemote = nullptr;

	using TestResultEventListener =
		lt::CEventMfnListener<CIcsRemoteEquipmentTester, TestResultEventArgs>;

	TestResultEventListener * m_pTestResultEventListener = nullptr;
};
