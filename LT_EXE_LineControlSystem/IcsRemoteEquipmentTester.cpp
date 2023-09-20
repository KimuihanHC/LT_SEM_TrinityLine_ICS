#include "stdafx.h"
#include "IcsRemoteEquipmentTester.h"
#include "EqpTester.h"
#include "Comm/IcsRemoteTester.h"
#include "Logger/Logger.h"
#include "Event/EventMfnListener.hpp"

CIcsRemoteEquipmentTester::CIcsRemoteEquipmentTester(CEqpTester & equipment)
: CIcsRemoteEquipment(equipment)
, m_equipment(equipment)
{
	m_pRemote = new lt::CIcsRemoteTester();
	m_pTestResultEventListener =
		new TestResultEventListener(this, &CIcsRemoteEquipmentTester::OnTestResultEvent);

	m_pRemote->GetTestResultEventNotifier() +=
		m_pTestResultEventListener;
}

CIcsRemoteEquipmentTester::~CIcsRemoteEquipmentTester()
{
	m_pRemote->GetTestResultEventNotifier() -=
		m_pTestResultEventListener;

	delete m_pTestResultEventListener;
	delete m_pRemote;
}

CEqpTester & CIcsRemoteEquipmentTester::GetEquipmentTester()
{
	return m_equipment;
}

const CEqpTester & CIcsRemoteEquipmentTester::GetEquipmentTester() const
{
	return m_equipment;
}

lt::CIcsRemoteTester & CIcsRemoteEquipmentTester::GetRemoteTester()
{
	return *m_pRemote;
}

const lt::CIcsRemoteTester & CIcsRemoteEquipmentTester::GetRemoteTester() const
{
	return *m_pRemote;
}

lt::CIcsRemote & CIcsRemoteEquipmentTester::GetRemote()
{
	return GetRemoteTester();
}

const lt::CIcsRemote & CIcsRemoteEquipmentTester::GetRemote() const
{
	return GetRemoteTester();
}

void CIcsRemoteEquipmentTester::OnTestResultEvent(TestResultEventArgs & eventArgs)
{
	auto & args = eventArgs.GetArgs();
	auto & reqArgs = args.GetTypedArgs();

	GetLogger()
		.SetLogLevel(lt::LogLevel::Notice)
		.AddLog(_T("Test Result (rfid = %s, result = %d, para = %d)"),
				reqArgs.rfid,
				reqArgs.result,
				reqArgs.para);

	auto result = GetEquipmentTester().Recv_NotifyTestResult(reqArgs.rfid, reqArgs.result, reqArgs.para);

	args.SetResult(result);
	eventArgs.Cancel();
}
