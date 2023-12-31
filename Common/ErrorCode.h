//*****************************************************************************
// Filename	: 	ErrorCode.h
//
// Created	:	
// Modified	:	2009/12/29 - 16:47
//
// Author	:	피링
//	
// Purpose	:	에러 코드 통합 처리용
//*****************************************************************************

#ifndef __ERRORCODE_H__
#define __ERRORCODE_H__

#pragma once

//WAIT_TIMEOUT

namespace PR_ErrorCode
{
	#define ERROR_CODE_BASE			0L
	#define ERR_NONE				0L

	#define ERR_BASE_SYSTEM			ERROR_CODE_BASE + 1000L
	#define ERR_BASE_COMM			ERROR_CODE_BASE + 10000L
	#define ERR_BASE_INSPECTION		ERROR_CODE_BASE	+ 20000L
	#define ERR_BASE_GUI			ERROR_CODE_BASE + 30000L
	#define ERR_BASE_DEVICE			ERROR_CODE_BASE + 40000L
	
	#define	ERR_BASE_INSPECT_UDT	ERR_BASE_INSPECTION + 1000L
	#define	ERR_BASE_INSPECT_IT		ERR_BASE_INSPECTION + 2000L
	#define	ERR_BASE_INSPECT_NG		ERR_BASE_INSPECTION + 3000L
	#define	ERR_BASE_INSPECT_BT		ERR_BASE_INSPECTION + 4000L
	#define	ERR_BASE_INSPECT_FT		ERR_BASE_INSPECTION + 5000L
	#define	ERR_BASE_INSPECT_QA		ERR_BASE_INSPECTION + 6000L

	#define ERR_BASE_SERIAL			ERR_BASE_COMM + 1000L
	#define ERR_BASE_TCPIP			ERR_BASE_COMM + 2000L
	#define ERR_BASE_SECS			ERR_BASE_COMM + 3000L

	#define ERR_BASE_PALLET			ERR_BASE_SERIAL + 100L
	#define ERR_BASE_JTAG			ERR_BASE_SERIAL + 200L
	#define ERR_BASE_BCR			ERR_BASE_SERIAL + 300L

	enum _ERROR_SYSTEM
	{
		ERR_SYSTEM					= ERR_BASE_SYSTEM,
		ERR_CRITICAL_SECTION,
		ERR_MUTEX,
		ERR_SEMAPHORE,
		ERR_EVENT_CREATE,
		ERR_EVENT_CLOSE,
		ERR_INVALID_HANDLE,
		ERR_INVALID_MSG_ID,
		ERR_WAIT_TIMEOUT,
		ERR_WAIT_ABANDONED,
	};

	enum _ERROR_COMM
	{
		ERR_COMM					= ERR_BASE_COMM,
		ERR_COMM_CONNECT,
		ERR_COMM_DISCONNECT,
		ERR_COMM_NOTCONNECTED,
	};

	enum _ERROR_SERIAL
	{
		ERR_SERIAL					= ERR_BASE_SERIAL,
		ERR_SERIAL_BROKEN_DATA,
		ERR_SERIAL_TRANSFER,
		ERR_SERIAL_ACK,
		ERR_SERIAL_ACK_TIMEOUT,
		ERR_SERIAL_ACK_PROTOCOL,
		ERR_SERIAL_ACK_NULL,
		ERR_SERIAL_ACK_UNKNOWN,
		ERR_SERIAL_CHECKSUM,
		ERR_SERIAL_STX,
		ERR_SERIAL_ETX,
		ERR_SERIAL_PROTOCOL,
	};

	enum _ERROR_PALLET
	{
		ERR_PALLET					= ERR_BASE_PALLET,
	};

	enum _ERROR_JTAG
	{
		ERR_JTAG					= ERR_BASE_JTAG,
		ERR_JTAG_CONNECT,
		ERR_JTAG_RESET,
		ERR_JTAG_ERASE,
		ERR_JTAG_ERASE_ALL,
		ERR_JTAG_READ,
		ERR_JTAG_WRITE_START,
		ERR_JTAG_WRITE,
		ERR_JTAG_DISCONNECT,		
		ERR_JTAG_HEXFILE,
		ERR_JTAG_HEXFILE_SIZE,
	};

	enum _ERROR_BCR
	{
		ERR_BCR						= ERR_BASE_BCR,
		ERR_BCR_NOREAD,
		ERR_BCR_AUTO_FRAME_FAIL,
		ERR_BCR_CAL_FAIL,
		ERR_BCR_CAL_CANCEL,
		ERR_BCR_ACK_UNKNOWN,
	};

	enum _ERROR_TCPIP
	{
		ERR_TCPIP					= ERR_BASE_TCPIP,
		ERR_TCPIP_BROKEN_DATA,
		ERR_TCPIP_TRANSFER,
		ERR_TCPIP_ACK,
		ERR_TCPIP_ACK_TIMEOUT,
		ERR_TCPIP_ACK_PROTOCOL,
		ERR_TCPIP_ACK_NULL,
		ERR_TCPIP_ACK_UNKNOWN,
		ERR_TCPIP_CHECKSUM,
		ERR_TCPIP_STX,
		ERR_TCPIP_ETX,
		ERR_TCPIP_PROTOCOL,
	};
}

#endif __ERRORCODE_H__