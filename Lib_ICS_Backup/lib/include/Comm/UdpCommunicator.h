#pragma once

#include "Comm/SocketCommunicator.h"

namespace lt
{
	class LTICS_LIB_CLASS CUdpCommunicator : public CSocketCommunicator
	{
	public:
		CUdpCommunicator(ConstStringT strPrefix);
		CUdpCommunicator(const CUdpCommunicator &) = delete;
		virtual ~CUdpCommunicator() = default;

		CUdpCommunicator & operator=(const CUdpCommunicator &) = delete;

	private:
		bool ConnectComm(const SSocketCommOption & option) override;
	};
}
