#pragma once

#include "TypeDecl.h"
#include "Event/IcsCommEvent.h"
#include "Def_Enum_Cm.h"

namespace lt
{
	class CDebugLogDispatcher;
	class ILogDispatcher;
	class CLogDispatcher;
	class ILogData;
	class CLogger;
	class ILoggable;
	class CIcsTcpServer;
	class CIcsRemote;
	class CIcsRemoteLoUnloader;
	class CIcsRemoteTester;
	class CIcsRemoteReturner;
	class ICommunicator;
	template<typename TypeSender, typename TypeArgs>
	class IEventArgs;
	template<typename TypeOwner, typename TypeEventArgs>
	class CEventMfnListener;
}

class CEqpLoader;
class CEqpTester;
class CEqpReturner;
class CEqpHandler;
class CEquipment;
class CIcsRemoteEquipment;

class CIcsCommunicator final
{
private:
	CIcsCommunicator();
	CIcsCommunicator(const CIcsCommunicator &) = delete;

	CIcsCommunicator & operator=(const CIcsCommunicator &) = delete;

public:
	~CIcsCommunicator();

	static CIcsCommunicator & GetInstance();

	// Server
	bool Open(lt::ConstStringT address, lt::ushort port);
	void Close();

	bool IsOpened() const;

	// Remote
	bool CreateRemote(CEqpLoader & equipment);
	bool CreateRemote(CEqpTester & equipment);
	bool CreateRemote(CEqpReturner & equipment);
	bool CreateRemote(CEqpHandler & equipment);
	void RemoveRemote(const CEquipment & equipment);
	void RemoveRemotes();

	// 시간 동기
	bool SendTimeSync();
	bool SendTimeSync(lt::ConstStringT equipmentId);
	// 사용자
	bool SendUserLevel(enPermissionMode mode, lt::ConstStringT userId);
	bool SendUserLevel(lt::ConstStringT equipmentId,
					   enPermissionMode mode,
					   lt::ConstStringT userId);
	// 언어
	bool SendLanguage(lt::uint32 language);
	bool SendLanguage(lt::ConstStringT equipmentId,
					  lt::uint32 language);
	// 모델
	bool SendModel(lt::ConstStringT equipmentId,
				   lt::uint32 model,
				   lt::uint32 socketType);

	// 초기화
	bool SendInitialization();
	bool SendInitialization(lt::ConstStringT equipmentId);
	// 리셋
	bool SendReset();
	bool SendReset(lt::ConstStringT equipmentId);
	// 시작(오토모드)
	bool SendRun();
	bool SendRun(lt::ConstStringT equipmentId);
	// 정지(매뉴얼모드)
	bool SendStop();
	bool SendStop(lt::ConstStringT equipmentId);
	// 부저 오프
	bool SendBuzzerOff();
	bool SendBuzzerOff(lt::ConstStringT equipmentId);
	// UI 화면 제어
	bool SendUiVisible(int cmdShow);
	bool SendUiVisible(lt::ConstStringT equipmentId,
					   int cmdShow);

	// 인라인 가동/비가동
	bool SendOperationActiveStatus(lt::uint32 status);
	bool SendOperationActiveStatus(lt::ConstStringT equipmentId,
								   lt::uint32 status);

	// 소켓 투입 승인
	bool SendSocketAccept(lt::ConstStringT equipmentId,
						  lt::ConstStringT rfid,
						  lt::uint32 flag,
						  lt::ConstStringT info);
	// 배출 승인
	bool SendEjectAccept(lt::ConstStringT equipmentId,
						 lt::ConstStringT rfid);
	// 생산 시작/종료
	bool SendProductionStartEnd(lt::uint32 flag);
	bool SendProductionStartEnd(lt::ConstStringT equipmentId,
								lt::uint32 flag);
	// 강제 배출
	bool SendForceEject(bool onOff);
	bool SendForceEject(lt::ConstStringT equipmentId,
						bool onOff);

private:
	// Logger
	lt::CLogger & GetLogger();
	lt::ILoggable & GetLogger() const;

	using LogAddedEventArgs =
		lt::IEventArgs<lt::ILogDispatcher, lt::ILogData>;

	void OnLogAddedEvent(LogAddedEventArgs & eventArgs);

	// Server
	lt::CIcsTcpServer & GetServer();
	const lt::CIcsTcpServer & GetServer() const;

	using RemoteConnectionEventArgs =
		lt::IEventArgs<lt::CIcsTcpServer, lt::ICommunicator>;

	void OnRemoteConnectedEvent(RemoteConnectionEventArgs & eventArgs);
	void OnRemoteDisconnectedEvent(RemoteConnectionEventArgs & eventArgs);

	// Remote
	bool AddRemote(lt::ConstStringT equipmentId, CIcsRemoteEquipment * remotePtr);

	bool AttachRemoteComm(lt::ConstStringT address, lt::ICommunicator & comm);
	bool DetachRemoteComm(const lt::ICommunicator & comm);

	template<typename TypeRemote, typename... TypeArgs>
	bool InvokeRemote(lt::uint32 (TypeRemote::*fn)(TypeArgs... args),
					  TypeArgs... args);
	template<typename TypeRemote, typename... TypeArgs>
	lt::uint32 InvokeRemote(lt::ConstStringT equipmentId,
							lt::uint32 (TypeRemote::*fn)(TypeArgs... args),
							TypeArgs... args);
	template<typename TypeRemote1, typename TypeRemote2, typename... TypeArgs>
	lt::uint32 InvokeRemote(TypeRemote1 * remotePtr,
							lt::uint32 (TypeRemote2::*fn)(TypeArgs... args),
							TypeArgs... args);

private:
	// Logger
	lt::CDebugLogDispatcher * m_pDebugLogDispatcher = nullptr;
	lt::CLogDispatcher * m_pLogDispatcher = nullptr;
	lt::CLogger * m_pLogger = nullptr;

	using LogAddedEventListener =
		lt::CEventMfnListener<CIcsCommunicator, LogAddedEventArgs>;

	LogAddedEventListener * m_pLogAddedEventListener = nullptr;

	// Server
	lt::CIcsTcpServer * m_pServer = nullptr;

	using RemoteConnectionEventListener =
		lt::CEventMfnListener<CIcsCommunicator, RemoteConnectionEventArgs>;

	RemoteConnectionEventListener * m_pRemoteConnectedEventListener = nullptr;
	RemoteConnectionEventListener * m_pRemoteDisconnectedEventListener = nullptr;

	// Remote
	using RemoteCntr = lt::StdMapContainer<CString, CIcsRemoteEquipment *>::Map;

	RemoteCntr * m_pRemoteCntr = nullptr;
	lt::StdRecursiveMutex * m_pRemoteCntrMutex = nullptr;
};
