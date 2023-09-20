#pragma once

#include "Comm/Communicator.h"
#include "Comm/SocketCommOption.h"
//2022.11.28a uhkim header
#include "Logger/ILoggable.h"


struct WSAData;
struct sockaddr_in;

namespace lt
{
	class LTICS_LIB_CLASS CSocketCommunicator : public CCommunicator
	{
	public:
		CSocketCommunicator(ConstStringT strPrefix);
		CSocketCommunicator(const CSocketCommunicator &) = delete;
		virtual ~CSocketCommunicator();

		CSocketCommunicator & operator=(const CSocketCommunicator &) = delete;

		const SCommOption & GetOption() const override;

		ushort GetLocalPort() const;
		ConstStringT GetLocalAddress() const;
		ushort GetRemotePort() const;
		ConstStringT GetRemoteAddress() const;

	protected:
		virtual bool ConnectComm(const SSocketCommOption & option) = 0;

		enum class SocketType
		{
			Stream,
			Dgram
		};

		bool sStartup();
		void sCleanup();

		bool sAttach(uintPtr sock,
					 ushort localPort, ConstStringT strLocalAddress,
					 ushort remotePort, ConstStringT strRemoteAddress);

		bool sOpen(SocketType type);
		void sClose();
		bool sIsOpened() const;

		bool sBind(ushort port, ConstStringT strAddress);
		bool sConnect(ushort port, ConstStringT strAddress);

		bool sListen();
		bool sAccept(uintPtr & sock, ushort & port, tchar * strAddress, size_t sizeOfAddress);

	private:
		CLogger * m_pLogger = nullptr;

		bool SetOption(const SCommOption & option) override;

		bool ConnectComm() override;
		void DisconnectComm() override;

		size_t SendComm(const int8 * bufferPtr, size_t sizeOfBuffer) override;
		size_t ReceiveComm(int8 * bufferPtr, size_t sizeOfBuffer) override;

		bool ClearRx() override;

		static sockaddr_in GetSockAddr(ushort port, ConstStringT strAddress);
		static ushort GetPort(const sockaddr_in & addr);
		static StdStringT GetAddress(const sockaddr_in & addr);

	private:
		WSAData * m_pWsaData = nullptr;
		SSocketCommOption * m_pOption = nullptr;
		uintPtr m_socket = ~0;
		ushort m_localPort = 0;
		StdStringT * m_pStrLocalAddress = nullptr;
		ushort m_remotePort = 0;
		StdStringT * m_pStrRemoteAddress = nullptr;
	};
}
