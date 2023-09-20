#pragma once

#include "Comm/Communicator.h"
#include "Comm/SerialCommOption.h"

namespace lt
{
	class LTICS_LIB_CLASS CSerialCommunicator : public CCommunicator
	{
	public:
		CSerialCommunicator(ConstStringT strPrefix);
		CSerialCommunicator(const CSerialCommunicator &) = delete;
		virtual ~CSerialCommunicator();

		CSerialCommunicator & operator=(const CSerialCommunicator &) = delete;

		const SCommOption & GetOption() const override;

	private:
		bool SetOption(const SCommOption & option) override;

		bool ConnectComm() override;
		void DisconnectComm() override;

		size_t SendComm(const int8 * bufferPtr, size_t sizeOfBuffer) override;
		size_t ReceiveComm(int8 * bufferPtr, size_t sizeOfBuffer) override;

		bool ClearRx() override;

	private:
		bool Clear(bool bTxClear, bool bRxClear);

	private:
		SSerialCommOption * m_pOption = nullptr;
		HANDLE m_hComm = nullptr;
		COMMTIMEOUTS m_commTimeouts = {};
		DCB m_dcb = {};
		OVERLAPPED m_ovWrite = {};
		OVERLAPPED m_ovRead = {};
	};
}
