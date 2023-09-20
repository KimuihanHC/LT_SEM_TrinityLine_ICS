#include "stdafx.h"
#include "IcsRemoteEquipmentHandler.h"
#include "EqpHandler.h"
#include "Comm/IcsRemote.h"
#include "Logger/Logger.h"

CIcsRemoteEquipmentHandler::CIcsRemoteEquipmentHandler(CEqpHandler & equipment)
: CIcsRemoteEquipment(equipment)
, m_equipment(equipment)
{
	m_pRemote = new lt::CIcsRemote();
}

CIcsRemoteEquipmentHandler::~CIcsRemoteEquipmentHandler()
{
	delete m_pRemote;
}

CEqpHandler & CIcsRemoteEquipmentHandler::GetEquipmentHandler()
{
	return m_equipment;
}

const CEqpHandler & CIcsRemoteEquipmentHandler::GetEquipmentHandler() const
{
	return m_equipment;
}

lt::CIcsRemote & CIcsRemoteEquipmentHandler::GetRemote()
{
	return *m_pRemote;
}

const lt::CIcsRemote & CIcsRemoteEquipmentHandler::GetRemote() const
{
	return *m_pRemote;
}
