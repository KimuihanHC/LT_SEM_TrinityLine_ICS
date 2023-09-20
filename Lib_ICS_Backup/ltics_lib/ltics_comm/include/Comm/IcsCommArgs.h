#pragma once

#include "TypeDecl.h"

namespace lt
{
	//////////////////////////////////////////////////////////////////////////////////
	// System setting
	struct STimeSync
	{
		uint16 year = 0;
		uint16 month = 0;
		uint16 day = 0;
		uint16 hour = 0;
		uint16 minute = 0;
		uint16 second = 0;
	};

	struct SUserLevel
	{
		uint32 userLevel = 0;
		ConstStringT const userId = nullptr;
	};

	struct SLanguage
	{
		uint32 language = 0;
	};

	struct SEquipmentId
	{
		ConstStringT const equipmentId = nullptr;
	};

	struct SModel
	{
		uint32 model = 0;
		uint32 socketType = 0;
	};
	//
	//////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////
	// Control
	struct SUiVisible
	{
		int cmdShow = 0;
	};
	//
	//////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////
	// Operation
	struct SEquipmentStatus
	{
		uint32 status = 0;
		int32 alarmCode = 0;
		ConstStringT const info = nullptr;
	};

	struct SOperationMode
	{
		uint32 mode = 0;
	};

	struct SSocketRfid
	{
		ConstStringT const rfid = nullptr;
	};

	struct SSocketAccepting
	{
		ConstStringT const rfid = nullptr;
		uint32 flag = 0;
		ConstStringT const info = nullptr;
	};

	struct SProductionStartEnd
	{
		uint32 flag = 0;
	};

	struct SForceEject
	{
		uint32 onOff = 0;
	};

	struct SSocketInfo
	{
		ConstStringT const rfid = nullptr;
		ConstStringT const productSn = nullptr;
	};

	struct SSocketInfo2
	{
		tchar rfid[50] = {};
		tchar productSn[50] = {};
	};

	struct STestResult
	{
		ConstStringT const rfid = nullptr;
		int32 result = 0;
		uint32 para = 0;
	};

	struct STestResult2
	{
		ConstStringT const rfid = nullptr;
		int32 result = 0;
		uint32 para = 0;
		tchar equipmentName[50] = {};
	};

	struct SPort
	{
		uint32 port = 0;
	};

	struct SPortStatus
	{
		uint32 port = 0;
		uint32 status = 0;
		SSocketInfo socketInfo = {};
	};

	struct SPortStatus2
	{
		const uint32 port = 0;
		uint32 status = 0;
		SSocketInfo2 socketInfo = {};
	};

	struct SConveyorStatus
	{
		uint32 conveyor = 0;
		uint32 status1 = 0;
		uint32 status2 = 0;
		SSocketInfo socketInfo = {};
	};

	struct SConveyorStatus2
	{
		const uint32 conveyor = 0;
		uint32 status1 = 0;
		uint32 status2 = 0;
		SSocketInfo2 socketInfo = {};
	};
	//
}