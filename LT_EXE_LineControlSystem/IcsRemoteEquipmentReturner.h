#pragma once

#include "IcsRemoteEquipment.h"

class CEqpReturner;

class CIcsRemoteEquipmentReturner final : public CIcsRemoteEquipment
{
public:
	CIcsRemoteEquipmentReturner(CEqpReturner & equipment);
	CIcsRemoteEquipmentReturner(const CIcsRemoteEquipmentReturner &) = delete;
	~CIcsRemoteEquipmentReturner();

	CIcsRemoteEquipmentReturner & operator=(const CIcsRemoteEquipmentReturner &) = delete;

protected:
	lt::CIcsRemote & GetRemote() override;
	const lt::CIcsRemote & GetRemote() const override;

private:
	CEqpReturner & GetEquipmentReturner();
	const CEqpReturner & GetEquipmentReturner() const;

private:
	CEqpReturner & m_equipment;

	lt::CIcsRemote * m_pRemote = nullptr;
};
