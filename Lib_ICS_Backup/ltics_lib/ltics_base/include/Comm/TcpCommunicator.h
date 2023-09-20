#pragma once

#include "Comm/SocketCommunicator.h"

namespace lt
{
	class LTICS_LIB_CLASS CTcpCommunicator : public CSocketCommunicator
	{
	public:
		CTcpCommunicator(ConstStringT strPrefix);
		CTcpCommunicator(const CTcpCommunicator &) = delete;
		virtual ~CTcpCommunicator() = default;

		CTcpCommunicator & operator=(const CTcpCommunicator &) = delete;

		bool WaitForIncomingConnection(uintPtr & sock,
									   ushort & port,
									   tchar * strAddress,
									   size_t sizeOfAddress);

	private:
		bool ConnectComm(const SSocketCommOption & option) override;
	};
}
