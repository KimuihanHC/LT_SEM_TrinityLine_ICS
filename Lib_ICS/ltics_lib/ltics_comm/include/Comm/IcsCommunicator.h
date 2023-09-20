#pragma once

#include "Comm/ICommunicator.h"
#include "Comm/IcsCommError.h"
#include "Logger/ILogDispatcher.h"


namespace lt
{
	class ILoggable;
	class CLogger;
	template<typename TypeEventArgs>
	class IEventListener;
	template<typename TypeOwner, typename TypeEventArgs>
	class CEventMfnListener;

	class LTICS_LIB_CLASS CIcsCommunicator abstract
	{
	public:
		virtual ~CIcsCommunicator();

		ICommunicator * operator=(ICommunicator * commPtr);
			 
		bool operator==(const ICommunicator * commPtr) const;
		bool operator!=(const ICommunicator * commPtr) const;

		ILogDispatcher * SetLogDispatcher(ILogDispatcher * logDispatcherPtr);

		ICommunicator * Attach(ICommunicator * commPtr);
		ICommunicator * Detach();

		bool IsConnected() const;

	protected:
		enum class CommId
		{
			Server,
			Client
		};

		CIcsCommunicator(CommId id);
		CIcsCommunicator(const CIcsCommunicator &) = delete;

		CIcsCommunicator & operator=(const CIcsCommunicator &) = delete;

		CommId GetSendingCommId() const;
		CommId GetReceivingCommId() const;

		ILoggable & GetLogger() const;

		using DataCntr = StdSequenceContainer<StdStringA>::Vector;
		using RequestProc = bool (CIcsCommunicator::*)(const DataCntr &, DataCntr &);

		bool AddRequestProcedure(uint16 command, RequestProc proc);
		RequestProc GetRequestProcedure(uint16 command) const;
		void RemoveRequestProcedure(uint16 command);

		uint32 ProcCommand(uint16 command,
						   const DataCntr & dataCntr,
						   uint64 responseTimeout,
						   DataCntr & responseDataCntr);
		uint32 ProcCommand(uint16 command,
						   const DataCntr & dataCntr,
						   uint64 responseTimeout);
		uint32 ProcCommand(uint16 command,
						   uint64 responseTimeout,
						   DataCntr & responseDataCntr);
		uint32 ProcCommand(uint16 command,
						   uint64 responseTimeout);

	private:
		uint32 Send(const StdStringA & packet);

		uint32 WaitFor(uint16 command, uint64 timeout, DataCntr & dataCntr);

		void ProcRequest(uint16 command, RequestProc proc, StdStringA && data);

		void SetRxData(uint16 command, StdStringA && data);
		bool GetRxData(uint16 command, StdStringA & data) const;
		void ClearRxData(uint16 command);

		void DispatchRequest(uint16 command, RequestProc proc, StdStringA && data);
		void WaitForDispatchingRequestTerminated();

		void ProcRxData(const SCommRxData & rxData);
		void DispatchRxData(StdStringA && packet);

		StdStringA MakePacket(uint16 command,
							  const StdStringA & data) const;

		int8 GetSendingCommIdChar() const;
		int8 GetReceivingCommIdChar() const;

		static bool IsSucceeded(const StdStringA & data);
		static bool IsFailed(const StdStringA & data);

		static inline uint16 ExtractCommand(const StdStringA & packet);
		static inline StdStringA ExtractData(const StdStringA & packet);

		static inline StdStringA MakePacket(CommId id,
											uint16 command,
											const StdStringA & data);

		static inline DataCntr ParseData(const StdStringA & data);
		static inline StdStringA MakeData(const DataCntr & dataCntr);

		static inline constexpr int8 GetStxChar();
		static inline constexpr int8 GetEtxChar();
		static inline constexpr int8 GetCommIdChar(CommId id);
		static inline constexpr int8 GetDataDelimiter();

		static inline constexpr uint8 GetResponseOKChars();
		static inline constexpr uint8 GetResponseNGChars();

		static inline constexpr size_t GetCommIdIndex();
		static inline constexpr size_t GetCommandIndex();
		static inline constexpr size_t GetDataIndex();

		using CommTxEventArgs = IEventArgs<ICommunicator, const SCommTxData>;
		using CommTxEventListener = IEventListener<CommTxEventArgs>;

		CommTxEventListener * GetCommTxEventListener() const;

		using CommRxEventArgs = IEventArgs<ICommunicator, const SCommRxData>;
		using CommRxEventListener = IEventListener<CommRxEventArgs>;

		CommRxEventListener * GetCommRxEventListener() const;

		void OnCommTxEvent(CommTxEventArgs & eventArgs);
		void OnCommRxEvent(CommRxEventArgs & eventArgs);

	private:
		CommId m_sendingCommId;
		CommId m_receivingCommId;

		CLogger * m_pLogger = nullptr;

		ICommunicator * m_pComm = nullptr;
		StdRecursiveMutex * m_pCommMutex = nullptr;

		StdStringA * m_pRxTempBuffer = nullptr;

		using RxBufferCntr = StdMapContainer<uint16, StdStringA>::Map;

		RxBufferCntr * m_pRxBufferCntr = nullptr;
		StdMutex * m_pRxBufferCntrMutex = nullptr;

		using RequestProcCntr = StdMapContainer<uint16, RequestProc>::Map;

		RequestProcCntr * m_pRequestProcCntr = nullptr;
		StdMutex * m_pRequestProcCntrMutex = nullptr;

		using RequestDispatchProcCntr = StdSequenceContainer<StdFuture<void>>::Vector;

		RequestDispatchProcCntr * m_pRequestDispatchProcCntr = nullptr;
		StdMutex * m_pRequestDispatchProcCntrMutex = nullptr;

		using CommTxEventListenerImpl =
			CEventMfnListener<CIcsCommunicator, CommTxEventListener::EventArgs>;

		CommTxEventListenerImpl * m_pCommTxEventListener = nullptr;

		using CommRxEventListenerImpl =
			CEventMfnListener<CIcsCommunicator, CommRxEventListener::EventArgs>;

		CommRxEventListenerImpl * m_pCommRxEventListener = nullptr;


	//========================================================================================
	//신규 메세지.
	//========================================================================================
#if (USE_XML) || 1
	protected:
		//받고 결과 보고.
		bool AddRequestProcedure(ConstStringT command, RequestProc proc);
		RequestProc GetRequestProcedure(ConstStringT command) const;		
		void RemoveRequestProcedure(ConstStringT command);		

		uint32		EesXmlProcCommand(ConstStringT ReportCommand,
			const DataCntr & dataCntr);
	private:
		void EesXmlProcRequest(RequestProc proc, StdStringA && data);

		void EesXmlDispatchRequest(RequestProc proc,
			StdStringA && data);
		void EesXmlDispatchRxData(StdStringA && packet);
		static inline ConstStringA cstStartElement();
		static inline ConstStringA cstEndElement();
		void ExtractMessageName(const StdStringA & packet, StdStringA * csta);

		//static inline ConstStringA cstTransIDStartElement();
		//static inline ConstStringA cstTransIDEndElement();
		//void ExtracttransactionId(const StdStringA & packet, StdStringA * csta);

		static inline StdStringA EesXmlExtractData(const StdStringA & packet);
		static inline StdStringA EesXmlMakePacket(ConstStringT RequestCommand,
			const StdStringA & data);
		static inline constexpr int8 GetCommMsgChar();
	private:
		using cstRequestProcCntr = StdMapContainer<ConstStringT, RequestProc>::Map;
		cstRequestProcCntr * m_pcstRequestProcCntr = nullptr;
		StdMutex * m_pcstRequestProcCntrMutex = nullptr;

		using ResponeDispatchProcCntr = StdSequenceContainer<StdFuture<void>>::Vector;
		ResponeDispatchProcCntr * m_pResponeDispatchProcCntr = nullptr;
		StdMutex * m_pResponeDispatchProcCntrMutex = nullptr;

	protected:
		DataCntr	EesXmlParseData(const StdStringA & data);
		StdStringA	EesXmlMakeData(const DataCntr & dataCntr);

#endif
	};
}
