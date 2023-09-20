#include "stdafx.h"
#include "IcsRemoteEquipmentReturner.h"
#include "EqpReturner.h"
#include "Comm/IcsRemote.h"
#include "Logger/Logger.h"

CIcsRemoteEquipmentReturner::CIcsRemoteEquipmentReturner(CEqpReturner & equipment)
: CIcsRemoteEquipment(equipment)
, m_equipment(equipment)
{
	m_pRemote = new lt::CIcsRemote();
}

CIcsRemoteEquipmentReturner::~CIcsRemoteEquipmentReturner()
{
	delete m_pRemote;
}

CEqpReturner & CIcsRemoteEquipmentReturner::GetEquipmentReturner()
{
	return m_equipment;
}

const CEqpReturner & CIcsRemoteEquipmentReturner::GetEquipmentReturner() const
{
	return m_equipment;
}

lt::CIcsRemote & CIcsRemoteEquipmentReturner::GetRemote()
{
	return *m_pRemote;
}

const lt::CIcsRemote & CIcsRemoteEquipmentReturner::GetRemote() const
{
	return *m_pRemote;
}
