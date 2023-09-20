#ifndef IcsRemoteServerEes_h__
#define IcsRemoteServerEes_h__

#pragma once

#include "IcsRemoteServer.h"

class CSvrEes;

class CIcsRemoteServerEes final : public CIcsRemoteServer
{
public:
	CIcsRemoteServerEes(CSvrEes & equipment);
	CIcsRemoteServerEes(const CIcsRemoteServerEes &) = delete;
	~CIcsRemoteServerEes();

	CIcsRemoteServerEes & operator=(const CIcsRemoteServerEes &) = delete;

protected:
	lt::CIcsRemoteEes & GetRemoteEes() override;
	const lt::CIcsRemoteEes & GetRemoteEes() const override;

private:
	CSvrEes & GetServerEes();
	const CSvrEes & GetServerEes() const;

private:
	CSvrEes & m_server;
	lt::CIcsRemoteEes * m_pRemoteEes = nullptr;

}; 

#endif	//IcsRemoteServerEes_h__