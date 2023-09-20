#include "pch.h"
#include "Comm/IcsCommunicator.h"
#include "Logger/Logger.h"
#include "Event/BasicEventNotifier.hpp"
#include "Event/EventMfnListener.hpp"
#include "Util/BasicTimer.h"
#include "Util/SyncUtil.hpp"
#include "Util/StringUtil.hpp"
#include <tchar.h>
#include <map>
#include <future>
#include <sstream>

using namespace lt;

CIcsCommunicator::CIcsCommunicator(CommId id)
	: m_sendingCommId(id)
	, m_receivingCommId(id == CommId::Server ? CommId::Client : CommId::Server)
{
	m_pLogger = new CLogger();

	m_pCommMutex = new StdRecursiveMutex();

	m_pRxTempBuffer = new StdStringA();

	m_pRxBufferCntr = new RxBufferCntr();
	m_pRxBufferCntrMutex = new StdMutex();

	m_pRequestProcCntr = new RequestProcCntr();
	m_pRequestProcCntrMutex = new StdMutex();

	m_pRequestDispatchProcCntr = new RequestDispatchProcCntr();
	m_pRequestDispatchProcCntrMutex = new StdMutex();

	m_pCommTxEventListener =
		new CommTxEventListenerImpl(this, &CIcsCommunicator::OnCommTxEvent);
	m_pCommRxEventListener =
		new CommRxEventListenerImpl(this, &CIcsCommunicator::OnCommRxEvent);
#if USE_XML
	m_pcstRequestProcCntr = new cstRequestProcCntr();
	m_pcstRequestProcCntrMutex = new StdMutex();
	m_pResponeDispatchProcCntr = new ResponeDispatchProcCntr();
	m_pResponeDispatchProcCntrMutex = new StdMutex();
#endif

}

CIcsCommunicator::~CIcsCommunicator()
{	
	delete m_pCommTxEventListener;
	delete m_pCommRxEventListener;
#if USE_XML
	delete m_pResponeDispatchProcCntr;
	delete m_pResponeDispatchProcCntrMutex;
	delete m_pcstRequestProcCntr;
	delete m_pcstRequestProcCntrMutex;
#endif
	delete m_pRequestDispatchProcCntr;
	delete m_pRequestDispatchProcCntrMutex;

	delete m_pRequestProcCntr;
	delete m_pRequestProcCntrMutex;

	delete m_pRxBufferCntr;
	delete m_pRxBufferCntrMutex;

	delete m_pRxTempBuffer;

	delete m_pCommMutex;

	delete m_pLogger;
}

ICommunicator * CIcsCommunicator::operator=(ICommunicator * commPtr)
{
	return Attach(commPtr);
}

bool CIcsCommunicator::operator==(const ICommunicator * commPtr) const
{
	SimpleLockPtr(m_pCommMutex);

	return m_pComm == commPtr;
}

bool CIcsCommunicator::operator!=(const ICommunicator * commPtr) const
{
	SimpleLockPtr(m_pCommMutex);

	return m_pComm != commPtr;
}

ILogDispatcher * CIcsCommunicator::SetLogDispatcher(ILogDispatcher * logDispatcherPtr)
{
	return *m_pLogger = logDispatcherPtr;
}

ICommunicator * CIcsCommunicator::Attach(ICommunicator * commPtr)
{
	SimpleLockPtr(m_pCommMutex);

	auto _commPtr = Detach();

	m_pComm = commPtr;

	if(m_pComm == nullptr)
		return _commPtr;

	m_pComm->GetCommTxEventNotifier() += GetCommTxEventListener();
	m_pComm->GetCommRxEventNotifier() += GetCommRxEventListener();

	return _commPtr;
}

ICommunicator * CIcsCommunicator::Detach()
{
	SimpleLockPtr(m_pCommMutex);

	auto _commPtr = m_pComm;

	if(m_pComm == nullptr)
		return _commPtr;

	WaitForDispatchingRequestTerminated();

	m_pComm->GetCommTxEventNotifier() -= GetCommTxEventListener();
	m_pComm->GetCommRxEventNotifier() -= GetCommRxEventListener();
	m_pComm = nullptr;

	return _commPtr;
}

bool CIcsCommunicator::IsConnected() const
{
	SimpleLockPtr(m_pCommMutex);

	return m_pComm != nullptr && m_pComm->IsConnected();
}

uint32 CIcsCommunicator::Send(const StdStringA & packet)
{
	SimpleLockPtr(m_pCommMutex);

	if(!IsConnected())
		return ICS_COMM_DISCONNECTED;

	SCommData commData;

	commData.bRxClear = true;
	commData.bufferPtr = packet.data();
	commData.sizeOfBuffer = packet.length();

	if(m_pComm->Send(commData) != commData.sizeOfBuffer)
		return ICS_COMM_TX_ERROR;

	return ICS_COMM_OK;
}

uint32 CIcsCommunicator::WaitFor(uint16 command,
								 uint64 timeout,
								 DataCntr & dataCntr)
{
	StdStringA data;

	for(CBasicTimer timer;
		timeout > timer.GetElapsedTime(CBasicTimer::Unit::Milliseconds);
		std::this_thread::sleep_for(std::chrono::milliseconds(1)))
	{
		if(!IsConnected())
			return ICS_COMM_DISCONNECTED;

		if(GetRxData(command, data))
		{
			dataCntr = ParseData(data);

			return ICS_COMM_OK;
		}
	}

	return ICS_COMM_RX_TIMEDOUT;
}

CIcsCommunicator::CommId CIcsCommunicator::GetSendingCommId() const
{
	return m_sendingCommId;
}

CIcsCommunicator::CommId CIcsCommunicator::GetReceivingCommId() const
{
	return m_receivingCommId;
}

ILoggable & CIcsCommunicator::GetLogger() const
{
	return *m_pLogger;
}

bool CIcsCommunicator::AddRequestProcedure(uint16 command, RequestProc proc)
{
	SimpleLockPtr(m_pRequestProcCntrMutex);

	auto result = m_pRequestProcCntr->emplace(command, proc);

	return result.second;
}

CIcsCommunicator::RequestProc CIcsCommunicator::GetRequestProcedure(uint16 command) const
{
	SimpleLockPtr(m_pRequestProcCntrMutex);

	auto iter = m_pRequestProcCntr->find(command);

	if(iter == m_pRequestProcCntr->end()) {
		return nullptr;
	}		
	return iter->second;
}

void CIcsCommunicator::RemoveRequestProcedure(uint16 command)
{
	SimpleLockPtr(m_pRequestProcCntrMutex);

	auto iter = m_pRequestProcCntr->find(command);

	if(iter == m_pRequestProcCntr->end())
		return;

	m_pRequestProcCntr->erase(iter);
}

uint32 CIcsCommunicator::ProcCommand(uint16 command,
									 const DataCntr & dataCntr,
									 uint64 responseTimeout,
									 DataCntr & responseDataCntr)
{
	auto strLoggingPrefix =
		Format(_T("[Command] (%0*X)"),
			   static_cast<int>(CIcsCommunicator::GetDataIndex() -
								CIcsCommunicator::GetCommandIndex()),
			   command);

	GetLogger()
		.SetLogLevel(LogLevel::Info)
		.AddLog(_T("%s Command procedure begins"),
				strLoggingPrefix.c_str());

	auto data = CIcsCommunicator::MakeData(dataCntr);

	GetLogger()
		.SetLogLevel(LogLevel::Notice)
		.AddLog(_T("%s Data = %s, Response timeout = %dms"),
				strLoggingPrefix.c_str(),
				ToTypeChar(data).c_str(),
				responseTimeout);

	uint32 error = ICS_COMM_ERROR;

	do
	{
		ClearRxData(command);

		GetLogger()
			.SetLogLevel(LogLevel::Proc)
			.AddLog(_T("%s Send the command"),
					strLoggingPrefix.c_str());

		auto requestPacket = MakePacket(command, data);

		error = Send(requestPacket);

		if(error != ICS_COMM_OK)
		{
			GetLogger()
				.SetLogLevel(LogLevel::Error)
				.AddLog(_T("%s Send(%s) failed %d"),
						strLoggingPrefix.c_str(),
						ToTypeChar(requestPacket).c_str(),
						error);

			break;
		}

		GetLogger()
			.SetLogLevel(LogLevel::Proc)
			.AddLog(_T("%s Wait for the response of the command"),
					strLoggingPrefix.c_str());

		DataCntr _responseDataCntr;

		error = WaitFor(command, responseTimeout, _responseDataCntr);

		if(error != ICS_COMM_OK)
		{
			GetLogger()
				.SetLogLevel(LogLevel::Error)
				.AddLog(_T("%s WaitFor() failed %d"),
						strLoggingPrefix.c_str(),
						error);

			break;
		}

		if(_responseDataCntr.empty())
		{
			error = ICS_COMM_WRONG_RESPONSE;

			GetLogger()
				.SetLogLevel(LogLevel::Error)
				.AddLog(_T("No response data received"),
						strLoggingPrefix.c_str());

			break;
		}

		auto iter = _responseDataCntr.begin();

		if(CIcsCommunicator::IsFailed(*iter))
			error = ICS_COMM_RESPONSE_NG;

		if(!CIcsCommunicator::IsSucceeded(*iter))
			error = ICS_COMM_WRONG_RESPONSE;

		_responseDataCntr.erase(iter);
		responseDataCntr = std::move(_responseDataCntr);
	}
	while(false);

	GetLogger()
		.SetLogLevel(LogLevel::Notice)
		.AddLog(_T("%s Command procedure ends"),
				strLoggingPrefix.c_str());

	return error;
}

uint32 CIcsCommunicator::ProcCommand(uint16 command,
									 const DataCntr & dataCntr,
									 uint64 responseTimeout)
{
	return ProcCommand(command,
					   dataCntr,
					   responseTimeout,
					   DataCntr());
}

uint32 CIcsCommunicator::ProcCommand(uint16 command,
									 uint64 responseTimeout,
									 DataCntr & responseDataCntr)
{
	return ProcCommand(command,
					   {},
					   responseTimeout,
					   responseDataCntr);
}

uint32 CIcsCommunicator::ProcCommand(uint16 command,
									 uint64 responseTimeout)
{
	return ProcCommand(command,
					   {},
					   responseTimeout);
}

void CIcsCommunicator::ProcRequest(uint16 command,
								   RequestProc proc,
								   StdStringA && data)
{
	auto strLoggingPrefix =
		Format(_T("[Request] (%0*X)"),
			   static_cast<int>(CIcsCommunicator::GetDataIndex() -
								CIcsCommunicator::GetCommandIndex()),
			   command);

	GetLogger()
		.SetLogLevel(LogLevel::Info)
		.AddLog(_T("%s Request procedure begins"),
				strLoggingPrefix.c_str());
		
	GetLogger()
		.SetLogLevel(LogLevel::Notice)
		.AddLog(_T("%s Data = %s"),
				strLoggingPrefix.c_str(),
				ToTypeChar(data).c_str());

	do
	{
		GetLogger()
			.SetLogLevel(LogLevel::Proc)
			.AddLog(_T("%s Call the request procedure"),
					strLoggingPrefix.c_str());

		DataCntr resultDataCntr;
		auto result = (this->*proc)(CIcsCommunicator::ParseData(data),
									resultDataCntr);

		resultDataCntr.emplace(resultDataCntr.begin(),
							   Format("%d", result ?
									  CIcsCommunicator::GetResponseOKChars() :
									  CIcsCommunicator::GetResponseNGChars()));


		GetLogger()
			.SetLogLevel(LogLevel::Proc)
			.AddLog(_T("%s Send the response of the request"),
					strLoggingPrefix.c_str());

		auto responsePacket = MakePacket(command,
										 CIcsCommunicator::MakeData(resultDataCntr));
		auto error = Send(responsePacket);

		if(error != ICS_COMM_OK)
		{
			GetLogger()
				.SetLogLevel(LogLevel::Error)
				.AddLog(_T("%s Send(%s) failed %d"),
						strLoggingPrefix.c_str(),
						ToTypeChar(responsePacket).c_str(),
						error);

			break;
		}
	}
	while(false);

	GetLogger()
		.SetLogLevel(LogLevel::Notice)
		.AddLog(_T("%s Request procedure ends"),
				strLoggingPrefix.c_str());
}

void CIcsCommunicator::SetRxData(uint16 command, StdStringA && data)
{
	SimpleLockPtr(m_pRxBufferCntrMutex);

	auto res = m_pRxBufferCntr->emplace(command, std::move(data));

	if(!res.second)
		(*res.first).second = std::move(data);
}

bool CIcsCommunicator::GetRxData(uint16 command, StdStringA & data) const
{
	SimpleLockPtr(m_pRxBufferCntrMutex);

	auto iter = m_pRxBufferCntr->find(command);

	if(iter == m_pRxBufferCntr->end())
		return false;

	data = (*iter).second;

	return true;
}

void CIcsCommunicator::ClearRxData(uint16 command)
{
	SimpleLockPtr(m_pRxBufferCntrMutex);

	auto iter = m_pRxBufferCntr->find(command);

	if(iter != m_pRxBufferCntr->end())
		m_pRxBufferCntr->erase(iter);
}

void CIcsCommunicator::DispatchRequest(uint16 command,
									   RequestProc proc,
									   StdStringA && data)
{
	SimpleLockPtr(m_pRequestDispatchProcCntrMutex);

	for(auto iter = m_pRequestDispatchProcCntr->begin();
		iter != m_pRequestDispatchProcCntr->end();)
	{
		if((*iter).valid() &&
		   (*iter).wait_for(std::chrono::milliseconds(0)) != std::future_status::ready)
			++iter;
		else
			iter = m_pRequestDispatchProcCntr->erase(iter);
	}

	m_pRequestDispatchProcCntr->emplace_back(
		std::async(std::launch::async,
				   &CIcsCommunicator::ProcRequest,
				   this,
				   command,
				   proc,
				   std::move(data))
	);
}

void CIcsCommunicator::WaitForDispatchingRequestTerminated()
{
	SimpleLockPtr(m_pRequestDispatchProcCntrMutex);

	for(auto & ref : *m_pRequestDispatchProcCntr)
	{
		if(ref.valid())
			ref.wait();
	}

	m_pRequestDispatchProcCntr->clear();
}

void CIcsCommunicator::ProcRxData(const SCommRxData & rxData)
{


	const auto stxChar = CIcsCommunicator::GetStxChar();
	auto bufferPtr = rxData.bufferPtr;
	auto const bufferPtrEnd = rxData.bufferPtr + rxData.sizeOfReceived;

	if(bufferPtr[0] == stxChar)
		m_pRxTempBuffer->clear();
	else if(m_pRxTempBuffer->length() == 0)
		for(; bufferPtrEnd - bufferPtr > 0 && *bufferPtr != stxChar; ++bufferPtr);

	size_t sizeOfBuffer = bufferPtrEnd - bufferPtr;

	if(sizeOfBuffer == 0)
		return;

	const auto etxChar = CIcsCommunicator::GetEtxChar();
	auto const etxBufferPtr = strchr(bufferPtr, etxChar);
	auto sizeOfPacket = sizeOfBuffer;

	if(etxBufferPtr != nullptr)
		sizeOfPacket = etxBufferPtr - bufferPtr + 1;

	m_pRxTempBuffer->append(bufferPtr, sizeOfPacket);
	bufferPtr += sizeOfPacket;
	sizeOfBuffer = bufferPtrEnd - bufferPtr;

	if(m_pRxTempBuffer->back() == etxChar)
	{
		auto packet = std::move(*m_pRxTempBuffer);
		auto commIdChar = packet.at(CIcsCommunicator::GetCommIdIndex());
#if USE_XML
		if ((commIdChar != GetReceivingCommIdChar()) &&
			(commIdChar != GetCommMsgChar())) {
			//(commIdChar != GetReceivingCommMsgChar())) {
			GetLogger()
				.SetLogLevel(LogLevel::Error)
				.AddLog(_T("Wrong packet received (CommIdChar = '%c')"),
					commIdChar);
		}
		else if (commIdChar == GetReceivingCommIdChar()) {
			if (sizeOfPacket < CIcsCommunicator::GetDataIndex() + 1)
			{
				GetLogger()
					.SetLogLevel(LogLevel::Error)
					.AddLog(_T("Wrong packet received (Packet Length = %d)"),
						sizeOfPacket);
			}
			else
			{
				DispatchRxData(std::move(packet));
			}
		}
		else if (commIdChar == GetCommMsgChar()) {
			EesXmlDispatchRxData(std::move(packet));
		}	
#else
		if (commIdChar != GetReceivingCommIdChar())
		{
			GetLogger()
				.SetLogLevel(LogLevel::Error)
				.AddLog(_T("Wrong packet received (CommIdChar = '%c')"),
					commIdChar);
		}
		else if (sizeOfPacket < CIcsCommunicator::GetDataIndex() + 1)
		{
			GetLogger()
				.SetLogLevel(LogLevel::Error)
				.AddLog(_T("Wrong packet received (Packet Length = %d)"),
					sizeOfPacket);
		}
		else
		{
			DispatchRxData(std::move(packet));
		}
#endif
	}

	if(sizeOfBuffer > 0)
		ProcRxData({bufferPtr, sizeOfBuffer});
}

void CIcsCommunicator::DispatchRxData(StdStringA && packet)
{
	auto command = CIcsCommunicator::ExtractCommand(packet);

	auto requestProc = GetRequestProcedure(command);

	if(requestProc == nullptr)
		SetRxData(command, CIcsCommunicator::ExtractData(packet));
	else {
		DispatchRequest(command, requestProc, CIcsCommunicator::ExtractData(packet));
	}		
}

StdStringA CIcsCommunicator::MakePacket(uint16 command,
										const StdStringA & data) const
{
	return CIcsCommunicator::MakePacket(GetSendingCommId(),
										command,
										data);
}

int8 CIcsCommunicator::GetSendingCommIdChar() const
{
	return CIcsCommunicator::GetCommIdChar(GetSendingCommId());
}

int8 CIcsCommunicator::GetReceivingCommIdChar() const
{
	return CIcsCommunicator::GetCommIdChar(GetReceivingCommId());
}

bool CIcsCommunicator::IsSucceeded(const StdStringA & data)
{
	return static_cast<uint8>(
		strtoul(data.c_str(), nullptr, 10)) == CIcsCommunicator::GetResponseOKChars();
}

bool CIcsCommunicator::IsFailed(const StdStringA & data)
{
	return static_cast<uint8>(
		strtoul(data.c_str(), nullptr, 10)) == CIcsCommunicator::GetResponseNGChars();
}

inline uint16 CIcsCommunicator::ExtractCommand(const StdStringA & packet)
{
	auto commandIndex = CIcsCommunicator::GetCommandIndex();
	auto commandLength = CIcsCommunicator::GetDataIndex() - commandIndex;

	return static_cast<uint16>(
		strtoul(packet.substr(commandIndex, commandLength).c_str(), nullptr, 16));
}

inline StdStringA CIcsCommunicator::ExtractData(const StdStringA & packet)
{
	return packet.substr(CIcsCommunicator::GetDataIndex(),
						 packet.length() - CIcsCommunicator::GetDataIndex() - 1);
}

inline StdStringA CIcsCommunicator::MakePacket(CommId commId,
											   uint16 command,
											   const StdStringA & data)
{
	auto sizeOfPacket = CIcsCommunicator::GetDataIndex() + data.size() + 1;
	auto packet = std::make_unique<int8[]>(sizeOfPacket);
	auto packetPtr = packet.get();

	*(packetPtr + 0) =
		CIcsCommunicator::GetStxChar();
	*(packetPtr + CIcsCommunicator::GetCommIdIndex()) =
		CIcsCommunicator::GetCommIdChar(commId);

	sprintf_s(packetPtr + CIcsCommunicator::GetCommandIndex(),
			  sizeOfPacket - CIcsCommunicator::GetCommandIndex(),
			  "%0*X",
			  static_cast<int>(CIcsCommunicator::GetDataIndex() -
							   CIcsCommunicator::GetCommandIndex()),
			  command);
	if(!data.empty())
	{
		memcpy(packetPtr + CIcsCommunicator::GetDataIndex(),
			   data.data(),
			   data.size());
	}

	*(packetPtr + (sizeOfPacket - 1)) =
		CIcsCommunicator::GetEtxChar();

	return StdStringA(packetPtr, sizeOfPacket);
}

inline CIcsCommunicator::DataCntr CIcsCommunicator::ParseData(const StdStringA & data)
{
	DataCntr cntr;	

	for(size_t index = 0; index <= data.length();)
	{
		auto end = data.find(CIcsCommunicator::GetDataDelimiter(), index);

		if(end == StdStringA::npos)
			end = data.length();

		cntr.emplace_back(data.substr(index, end - index));
		index = end + 1;
	}

	return cntr;
}

inline StdStringA CIcsCommunicator::MakeData(const DataCntr & dataCntr)
{
	if(dataCntr.empty())
		return StdStringA();

	StdOSStreamA oss;
	auto iter = dataCntr.begin();

	oss << *iter;

	for(++iter; iter != dataCntr.end(); ++iter)
	{
		oss << CIcsCommunicator::GetDataDelimiter()
			<< *iter;
	}

	return oss.str();
}

inline constexpr int8 CIcsCommunicator::GetStxChar()
{
	return 0x02; // STX
}

inline constexpr int8 CIcsCommunicator::GetEtxChar()
{
	return 0x03; // ETX
}

inline constexpr int8 CIcsCommunicator::GetCommIdChar(CommId id)
{
	static constexpr int8 CommIdChars[] =
	{
		0x53, // 'S'
		0x48 // 'H'
	};

	return CommIdChars[static_cast<int>(id)];
}

inline constexpr int8 CIcsCommunicator::GetDataDelimiter()
{
	return 0x2C; // ','
}

inline constexpr uint8 CIcsCommunicator::GetResponseOKChars()
{
	return 1;
}

inline constexpr uint8 CIcsCommunicator::GetResponseNGChars()
{
	return 0;
}

inline constexpr size_t CIcsCommunicator::GetCommIdIndex()
{
	return 1;
}

inline constexpr size_t CIcsCommunicator::GetCommandIndex()
{
	return 2;
}

inline constexpr size_t CIcsCommunicator::GetDataIndex()
{
	return 6;
}

CIcsCommunicator::CommTxEventListener * CIcsCommunicator::GetCommTxEventListener() const
{
	return m_pCommTxEventListener;
}

CIcsCommunicator::CommRxEventListener * CIcsCommunicator::GetCommRxEventListener() const
{
	return m_pCommRxEventListener;
}

void CIcsCommunicator::OnCommTxEvent(CommTxEventArgs & eventArgs)
{
	auto & txData = eventArgs.GetArgs();
	auto string = ToTypeChar(StdStringA(txData.bufferPtr, txData.sizeOfSent));
	//
	GetLogger()
		.SetLogLevel(LogLevel::Notice)
		.AddLog(_T("(Sent) %s"),
				string.c_str());
}

void CIcsCommunicator::OnCommRxEvent(CommRxEventArgs & eventArgs)
{
	auto & rxData = eventArgs.GetArgs();
	auto string = ToTypeChar(StdStringA(rxData.bufferPtr, rxData.sizeOfReceived));
	//
	GetLogger()
		.SetLogLevel(LogLevel::Notice)
		.AddLog(_T("(Recv) %s"),
				string.c_str());
	
	ProcRxData(eventArgs.GetArgs());
}
#if USE_XML
//=============================================================================
// Method		: AddRequestProcedure
// Access		: protected   
// Returns		: bool
// Parameter	: ConstStringT command
// Qualifier	:
// Last Update	: 2023/07/10 - 9:02
// Desc.		:
//=============================================================================
bool CIcsCommunicator::AddRequestProcedure(ConstStringT command, RequestProc proc)
{
	SimpleLockPtr(m_pcstRequestProcCntrMutex);

	auto result = m_pcstRequestProcCntr->emplace(command, proc);

	return result.second;
}
CIcsCommunicator::RequestProc CIcsCommunicator::GetRequestProcedure(ConstStringT command) const
{
	SimpleLockPtr(m_pcstRequestProcCntrMutex);
	for (auto iter = m_pcstRequestProcCntr->begin();
		iter != m_pcstRequestProcCntr->end(); )
	{
		auto csttCommand = (*iter).first;
		if (wcscmp(csttCommand, command) == 0) {
			return (*iter).second;
		}
		iter++;
	}
	return nullptr;
}
void CIcsCommunicator::RemoveRequestProcedure(ConstStringT command)
{
	SimpleLockPtr(m_pcstRequestProcCntrMutex);

	auto iter = m_pcstRequestProcCntr->find(command);

	if (iter == m_pcstRequestProcCntr->end())
		return;

	m_pcstRequestProcCntr->erase(iter);
}

//send wait for
uint32 CIcsCommunicator::EesXmlProcCommand(ConstStringT ReportCommand,
	const DataCntr & dataCntr)
{
	auto data = CIcsCommunicator::EesXmlMakeData(dataCntr);
	uint32 error = ICS_COMM_ERROR;
	do
	{
		auto requestPacket = EesXmlMakePacket(ReportCommand, data);
		if (requestPacket == "") {
			error = ICS_COMM_WRONG_RESPONSE;
			return error;
		}
		error = Send(requestPacket);

		if (error != ICS_COMM_OK)
		{
			break;
		}

	} while (false);

	return error;
}

void CIcsCommunicator::EesXmlProcRequest(RequestProc proc,
	StdStringA && data)
{
	DataCntr resultDataCntr;
	auto result = (this->*proc)(CIcsCommunicator::EesXmlParseData(data),
		resultDataCntr);
}

void CIcsCommunicator::EesXmlDispatchRequest(RequestProc proc,
	StdStringA && data)
{
	SimpleLockPtr(m_pRequestDispatchProcCntrMutex);

	for (auto iter = m_pRequestDispatchProcCntr->begin();
		iter != m_pRequestDispatchProcCntr->end();)
	{
		if ((*iter).valid() &&
			(*iter).wait_for(std::chrono::milliseconds(0)) != std::future_status::ready)
			++iter;
		else
			iter = m_pRequestDispatchProcCntr->erase(iter);
	}
	
	m_pRequestDispatchProcCntr->emplace_back(
		std::async(std::launch::async,
			&CIcsCommunicator::EesXmlProcRequest,
			this,
			proc,
			std::move(data))
	);
}

void CIcsCommunicator::EesXmlDispatchRxData(StdStringA && packet)
{
	//Find Msg
	//2023.06.30a uhkim Test.
	RequestProc requestProc = nullptr;
	StdStringA  ssA;
	ExtractMessageName(packet, &ssA);
	
#if defined(UNICODE)
	StdStringW	 ssW = ToWideChar(ssA);
	ConstStringT MassageName(std::move(ssW.c_str()));
#else
	ConstStringT MassageName(std::move(ssA.c_str()));
#endif

	requestProc = GetRequestProcedure(MassageName);

	if (requestProc != nullptr)
	{
		EesXmlDispatchRequest(requestProc,
			CIcsCommunicator::EesXmlExtractData(packet)
		);
	}
}

inline ConstStringA CIcsCommunicator::cstStartElement() {
	static ConstStringA StartElement = ("<messageName>");
	return static_cast<ConstStringA>(StartElement);
}
inline ConstStringA CIcsCommunicator::cstEndElement() {
	static ConstStringA EndElement = "</messageName>";
	return static_cast<ConstStringA>(EndElement);
}
void CIcsCommunicator::ExtractMessageName(const StdStringA & packet, StdStringA * csta)
{	
	auto commandIndex = packet.find(cstStartElement()) + strlen(cstStartElement());
	auto commandLength = packet.find(cstEndElement()) - commandIndex;
	*csta = packet.substr((commandIndex), (commandLength));
}
/*
inline ConstStringA CIcsCommunicator::cstTransIDStartElement() {
	static ConstStringA StartElement = ("<transactionId>");
	return static_cast<ConstStringA>(StartElement);
}
inline ConstStringA CIcsCommunicator::cstTransIDEndElement() {
	static ConstStringA EndElement = "</transactionId>";
	return static_cast<ConstStringA>(EndElement);
}
void CIcsCommunicator::ExtracttransactionId(const StdStringA & packet, StdStringA * csta)
{
	auto commandIndex = packet.find(cstTransIDStartElement()) + strlen(cstTransIDStartElement());
	auto commandLength = packet.find(cstTransIDEndElement()) - commandIndex;
	*csta = packet.substr((commandIndex), (commandLength));
}*/

inline StdStringA CIcsCommunicator::EesXmlExtractData(const StdStringA & packet)
{
	return std::move(packet);
}

inline StdStringA CIcsCommunicator::EesXmlMakePacket(ConstStringT RequestCommand,
	const StdStringA & data)
{
	auto commandStart = data.find(cstStartElement()) + strlen(cstStartElement());
	auto commandEnd = data.find(cstEndElement());

	if (commandEnd == StdStringA::npos) {
		return StdStringA();
	}
	StdStringA ssaStart = data.substr(0, commandStart);
	StdStringA ssaCommand = ToMultiByte(RequestCommand);
	StdStringA ssaEnd = data.substr(commandEnd, data.size() - commandEnd);

	auto sizeOfPacket = CIcsCommunicator::GetCommIdIndex() + ssaStart.size() + ssaCommand.size() + ssaEnd.size() + 1;
	auto packet = std::make_unique<int8[]>(sizeOfPacket);
	auto packetPtr = packet.get();

	*(packetPtr + 0) =
		CIcsCommunicator::GetStxChar();
	auto nFind = data.find(ToMultiByte(RequestCommand));
	if (!data.empty())
	{
		if ((nFind == 0 || nFind == StdStringA::npos)) {
			memcpy(packetPtr + CIcsCommunicator::GetCommIdIndex(),
				ssaStart.data(),
				ssaStart.size());
			memcpy(packetPtr + CIcsCommunicator::GetCommIdIndex() + ssaStart.size(),
				ssaCommand.data(),
				ssaCommand.size());
			memcpy(packetPtr + CIcsCommunicator::GetCommIdIndex() + ssaStart.size() + ssaCommand.size(),
				ssaEnd.data(),
				ssaEnd.size());
		}
		else {
			memcpy(packetPtr + CIcsCommunicator::GetCommIdIndex(),
				data.data(),
				data.size());
		}
	}

	*(packetPtr + (sizeOfPacket - 1)) =
		CIcsCommunicator::GetEtxChar();

	return StdStringA(packetPtr, sizeOfPacket);

}
inline constexpr int8 CIcsCommunicator::GetCommMsgChar()
{
	return 0x3C; //'<'
}

//2023.07.31a uhkim
CIcsCommunicator::DataCntr CIcsCommunicator::EesXmlParseData(const StdStringA & data)
{
	DataCntr cntr;
	for (size_t index = 0; index <= data.length();)
	{
		auto end = data.length();

		cntr.emplace_back(data.substr(index, end - index));
		index = end + 1;
	}
	return cntr;
}
StdStringA CIcsCommunicator::EesXmlMakeData(const DataCntr & dataCntr)
{
	if (dataCntr.empty())
		return StdStringA();

	StdOSStreamA oss;
	auto iter = dataCntr.begin();

	oss << *iter;

	for (++iter; iter != dataCntr.end(); ++iter)
	{
		oss << *iter;
	}

	return oss.str();
}


#endif
