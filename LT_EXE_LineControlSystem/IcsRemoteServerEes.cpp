#include "stdafx.h"
#include "SvrEes.h"

#include "IcsRemoteServerEes.h"

CIcsRemoteServerEes::CIcsRemoteServerEes(CSvrEes & equipment)
	: CIcsRemoteServer(equipment)
	, m_server(equipment)
{
	m_pRemoteEes = new lt::CIcsRemoteEes();
}

CIcsRemoteServerEes::~CIcsRemoteServerEes()
{
	delete m_pRemoteEes;
}

CSvrEes & CIcsRemoteServerEes::GetServerEes()
{
	return m_server;
}

const CSvrEes & CIcsRemoteServerEes::GetServerEes() const
{
	return m_server;
}

lt::CIcsRemoteEes & CIcsRemoteServerEes::GetRemoteEes()
{
	return *m_pRemoteEes;
}
const lt::CIcsRemoteEes & CIcsRemoteServerEes::GetRemoteEes() const
{
	return *m_pRemoteEes;
}
