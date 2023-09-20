#include "stdafx.h"
#include "IcsRemoteEquipmentLoUnloader.h"
#include "EqpLoader.h"
#include "Comm/IcsRemoteLoUnloader.h"
#include "Logger/Logger.h"
#include "Event/EventMfnListener.hpp"

CIcsRemoteEquipmentLoUnloader::CIcsRemoteEquipmentLoUnloader(CEqpLoader & equipment)
: CIcsRemoteEquipment(equipment)
, m_equipment(equipment)
{
	m_pRemote = new lt::CIcsRemoteLoUnloader();
	m_pLoadingEventListener =
		new LoadingEventListener(this, &CIcsRemoteEquipmentLoUnloader::OnLoadingEvent);
	m_pTestResultRequestEventListener =
		new TestResultRequestEventListener(this, &CIcsRemoteEquipmentLoUnloader::OnTestResultRequestEvent);
	m_pUnloadingEventListener =
		new UnloadingEventListener(this, &CIcsRemoteEquipmentLoUnloader::OnUnloadingEvent);

	m_pRemote->GetLoadingEventNotifier() +=
		m_pLoadingEventListener;
	m_pRemote->GetTestResultRequestEventNotifier() +=
		m_pTestResultRequestEventListener;
	m_pRemote->GetUnloadingEventNotifier() +=
		m_pUnloadingEventListener;
}

CIcsRemoteEquipmentLoUnloader::~CIcsRemoteEquipmentLoUnloader()
{
	m_pRemote->GetLoadingEventNotifier() -=
		m_pLoadingEventListener;
	m_pRemote->GetTestResultRequestEventNotifier() -=
		m_pTestResultRequestEventListener;
	m_pRemote->GetUnloadingEventNotifier() -=
		m_pUnloadingEventListener;

	delete m_pLoadingEventListener;
	delete m_pTestResultRequestEventListener;
	delete m_pUnloadingEventListener;
	delete m_pRemote;
}

CEqpLoader & CIcsRemoteEquipmentLoUnloader::GetEquipmentLoader()
{
	return m_equipment;
}

const CEqpLoader & CIcsRemoteEquipmentLoUnloader::GetEquipmentLoader() const
{
	return m_equipment;
}

lt::CIcsRemoteLoUnloader & CIcsRemoteEquipmentLoUnloader::GetRemoteLoUnloader()
{
	return *m_pRemote;
}

const lt::CIcsRemoteLoUnloader & CIcsRemoteEquipmentLoUnloader::GetRemoteLoUnloader() const
{
	return *m_pRemote;
}

lt::CIcsRemote & CIcsRemoteEquipmentLoUnloader::GetRemote()
{
	return GetRemoteLoUnloader();
}

const lt::CIcsRemote & CIcsRemoteEquipmentLoUnloader::GetRemote() const
{
	return GetRemoteLoUnloader();
}

void CIcsRemoteEquipmentLoUnloader::OnLoadingEvent(LoadingEventArgs & eventArgs)
{
	auto & args = eventArgs.GetArgs();
	auto & reqArgs = args.GetTypedArgs();

	GetLogger()
		.SetLogLevel(lt::LogLevel::Notice)
		.AddLog(_T("Loading (rfid = %s, productSn = %s)"),
				reqArgs.rfid,
				reqArgs.productSn);

	auto result = GetEquipmentLoader().Recv_RegisterSocket(reqArgs.rfid, reqArgs.productSn);
	
	args.SetResult(result);
	eventArgs.Cancel();
}

void CIcsRemoteEquipmentLoUnloader::OnTestResultRequestEvent(TestResultRequestEventArgs & eventArgs)
{
	auto & args = eventArgs.GetArgs();
	auto & reqArgs = args.GetTypedArgs();
	auto socketAvailable = false;

	ST_TestResult stResult;
	if (GetEquipmentLoader().Recv_ReqTestResult(reqArgs.rfid, stResult))
	{
		reqArgs.result = stResult.m_nNG_Code;
		reqArgs.para = stResult.m_nNG_Para;
		_tcscpy_s(reqArgs.equipmentName,
			_countof(reqArgs.equipmentName),
			stResult.m_szNG_EqpID);

		socketAvailable = true;
	}
	else
	{
		reqArgs.result = 0;
		reqArgs.para = 0;
		_tcscpy_s(reqArgs.equipmentName,
			_countof(reqArgs.equipmentName),
			GetEquipmentLoader().Get_Alias());

		socketAvailable = false;
	}

	GetLogger()
		.SetLogLevel(lt::LogLevel::Notice)
		.AddLog(_T("Test Result Request (rfid = %s, result = %d, para = %d, equip = %s)"),
				reqArgs.rfid,
				reqArgs.result,
				reqArgs.para,
				reqArgs.equipmentName);

	args.SetResult(socketAvailable);
	eventArgs.Cancel();
}

void CIcsRemoteEquipmentLoUnloader::OnUnloadingEvent(UnloadingEventArgs & eventArgs)
{
	auto & args = eventArgs.GetArgs();
	auto & reqArgs = args.GetTypedArgs();
	//auto socketAvailable = _tcscmp(reqArgs.rfid, _T("")) == 0;

	GetLogger()
		.SetLogLevel(lt::LogLevel::Notice)
		.AddLog(_T("Unloading (rfid = %s)"),
				reqArgs.rfid);

	auto socketAvailable = GetEquipmentLoader().Recv_UnregisterSocket(reqArgs.rfid);

	args.SetResult(socketAvailable);
	eventArgs.Cancel();
}
