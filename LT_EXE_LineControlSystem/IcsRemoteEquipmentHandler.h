#pragma once

#include "IcsRemoteEquipment.h"

class CEqpHandler;

class CIcsRemoteEquipmentHandler final : public CIcsRemoteEquipment
{
public:
	CIcsRemoteEquipmentHandler(CEqpHandler & equipment);
	CIcsRemoteEquipmentHandler(const CIcsRemoteEquipmentHandler &) = delete;
	~CIcsRemoteEquipmentHandler();

	CIcsRemoteEquipmentHandler & operator=(const CIcsRemoteEquipmentHandler &) = delete;

protected:
	lt::CIcsRemote & GetRemote() override;
	const lt::CIcsRemote & GetRemote() const override;

private:
	CEqpHandler & GetEquipmentHandler();
	const CEqpHandler & GetEquipmentHandler() const;

private:
	CEqpHandler & m_equipment;

	lt::CIcsRemote * m_pRemote = nullptr;
};
