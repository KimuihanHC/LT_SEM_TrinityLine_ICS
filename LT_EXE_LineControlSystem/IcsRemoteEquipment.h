#pragma once

#include "TypeDecl.h"
#include "Event/IcsCommEvent.h"
#include "Comm/IcsCommError.h"
#include "Def_ConfigEquipment.h"
#include "Def_Enum_Cm.h"

namespace lt
{
	class CDebugLogDispatcher;
	class CLogger;
	class ILoggable;
	class CIcsRemote;
	class ICommunicator;
	template<typename TypeSender, typename TypeArgs>
	class IEventArgs;
	template<typename TypeOwner, typename TypeEventArgs>
	class CEventMfnListener;
}

class CEquipment;

class CIcsRemoteEquipment abstract
{
public:
	CIcsRemoteEquipment(CEquipment & equipment);
	CIcsRemoteEquipment(const CIcsRemoteEquipment &) = delete;
	virtual ~CIcsRemoteEquipment();

	CIcsRemoteEquipment & operator=(const CIcsRemoteEquipment &) = delete;

	bool operator==(const lt::ICommunicator * commPtr) const;
	bool operator!=(const lt::ICommunicator * commPtr) const;

	lt::ICommunicator * Attach(lt::ICommunicator * commPtr);
	lt::ICommunicator * Detach();

	bool IsConnected() const;

	const CConfig_Eqp & GetConfig() const;

	// Remote
	// 시간 동기
	lt::uint32 SendTimeSync();
	// 사용자
	lt::uint32 SendUserLevel(enPermissionMode mode, lt::ConstStringT userId);
	// 언어
	lt::uint32 SendLanguage(lt::uint32 language);
	// 모델
	lt::uint32 SendModel(lt::uint32 model,
						 lt::uint32 socketType);

	// 초기화
	lt::uint32 SendInitialization();
	// 리셋
	lt::uint32 SendReset();
	// 시작(오토모드)
	lt::uint32 SendRun();
	// 정지(매뉴얼모드)
	lt::uint32 SendStop();
	// 부저 오프
	lt::uint32 SendBuzzerOff();
	// UI 화면 제어
	lt::uint32 SendUiVisible(int cmdShow);

	// 소켓 투입 승인
	lt::uint32 SendSocketAccept(lt::ConstStringT rfid,
								lt::uint32 flag,
								lt::ConstStringT info);
	// 배출 승인
	lt::uint32 SendEjectAccept(lt::ConstStringT rfid);
	// 생산 시작/종료
	lt::uint32 SendProductionStartEnd(lt::uint32 flag);
	// 강제 배출
	lt::uint32 SendForceEject(bool onOff);

	/*
	void	SetSValue(__in uint32_t nSVID, LPCTSTR IN_SValue);
	CString GetSValue(__in uint32_t nSVID);

	uint32_t GetSVID(uint32_t nSVID);
	*/
protected:
	virtual lt::CIcsRemote & GetRemote() = 0;
	virtual const lt::CIcsRemote & GetRemote() const = 0;

	// Logger
	lt::CLogger & GetLogger();
	lt::ILoggable & GetLogger() const;

private:
	CEquipment & GetEquipment();
	const CEquipment & GetEquipment() const;

	// Remote
	using EquipmentIdEventArgs =
		lt::EquipmentIdEventArgs<lt::CIcsRemote>;
	using EquipmentStatusEventArgs =
		lt::EquipmentStatusEventArgs<lt::CIcsRemote>;
	using OperationModeEventArgs =
		lt::OperationModeEventArgs<lt::CIcsRemote>;
	using SocketRequestEventArgs =
		lt::SocketRequestEventArgs<lt::CIcsRemote>;
	using PortClearEventArgs =
		lt::PortClearEventArgs<lt::CIcsRemote>;
	using PortStatusEventArgs =
		lt::PortStatusEventArgs<lt::CIcsRemote>;
	using PortStatusRequestEventArgs =
		lt::PortStatusRequestEventArgs<lt::CIcsRemote>;
	using ConveyorStatusEventArgs =
		lt::ConveyorStatusEventArgs<lt::CIcsRemote>;
	using ConveyorStatusRequestEventArgs =
		lt::ConveyorStatusRequestEventArgs<lt::CIcsRemote>;

	void OnEquipmentIdEvent(EquipmentIdEventArgs & eventArgs);
	void OnEquipmentStatusEvent(EquipmentStatusEventArgs & eventArgs);
	void OnOperationModeEvent(OperationModeEventArgs & eventArgs);
	void OnSocketRequestEvent(SocketRequestEventArgs & eventArgs);
	void OnPortClearEvent(PortClearEventArgs & eventArgs);
	void OnPortStatusEvent(PortStatusEventArgs & eventArgs);
	void OnPortStatusRequestEvent(PortStatusRequestEventArgs & eventArgs);
	void OnConveyorStatusEvent(ConveyorStatusEventArgs & eventArgs);
	void OnConveyorStatusRequestEvent(ConveyorStatusRequestEventArgs & eventArgs);
private:
	CEquipment & m_equipment;

	// Logger
	lt::CDebugLogDispatcher * m_pDebugLogDispatcher = nullptr;
	lt::CLogger * m_pLogger = nullptr;

	// Remote
	using EquipmentIdEventListener =
		lt::CEventMfnListener<CIcsRemoteEquipment, EquipmentIdEventArgs>;
	using EquipmentStatusEventListener =
		lt::CEventMfnListener<CIcsRemoteEquipment, EquipmentStatusEventArgs>;
	using OperationModeEventListener =
		lt::CEventMfnListener<CIcsRemoteEquipment, OperationModeEventArgs>;
	using SocketRequestEventListener =
		lt::CEventMfnListener<CIcsRemoteEquipment, SocketRequestEventArgs>;
	using PortClearEventListener =
		lt::CEventMfnListener<CIcsRemoteEquipment, PortClearEventArgs>;
	using PortStatusEventListener =
		lt::CEventMfnListener<CIcsRemoteEquipment, PortStatusEventArgs>;
	using PortStatusRequestEventListener =
		lt::CEventMfnListener<CIcsRemoteEquipment, PortStatusRequestEventArgs>;
	using ConveyorStatusEventListener =
		lt::CEventMfnListener<CIcsRemoteEquipment, ConveyorStatusEventArgs>;
	using ConveyorStatusRequestEventListener =
		lt::CEventMfnListener<CIcsRemoteEquipment, ConveyorStatusRequestEventArgs>;

	EquipmentIdEventListener * m_pEquipmentIdEventListener = nullptr;
	EquipmentStatusEventListener * m_pEquipmentStatusEventListener = nullptr;
	OperationModeEventListener * m_pOperationModeEventListener = nullptr;
	SocketRequestEventListener * m_pSocketRequestEventListener = nullptr;
	PortClearEventListener * m_pPortClearEventListener = nullptr;
	PortStatusEventListener * m_pPortStatusEventListener = nullptr;
	PortStatusRequestEventListener * m_pPortStatusRequestEventListener = nullptr;
	ConveyorStatusEventListener * m_pConveyorStatusEventListener = nullptr;
	ConveyorStatusRequestEventListener * m_pConveyorStatusRequestEventListener = nullptr;
};
