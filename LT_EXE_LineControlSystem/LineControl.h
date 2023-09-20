//*****************************************************************************
// Filename	: 	LineControl.h
// Created	:	2021/11/18 - 17:32
// Modified	:	2021/11/18 - 17:32
//
// Author	:	piring
//	
// Purpose	:	설비와 통신 및 핸들러 제어
//*****************************************************************************
#ifndef LineControl_h__
#define LineControl_h__

#pragma once

#include "LineInfo.h"

//-----------------------------------------------------------------------------
// 설비와 통신 및 핸들러 제어
//-----------------------------------------------------------------------------
class CLineControl : public CLineInfo
{
public:
	CLineControl();
	virtual ~CLineControl();

protected:

	// 소켓 정보(?)

	

	// Loader
	// Tester, Tester, ....
	// Return
	// Unloader


	// 검사 완료
	// Recv_CompleteTest( ok / ng );
	// Send_Reply_CompleteTest();



	// 전송: 서버 시간 동기화
	// Send_TimeSync ( server_time );

	// 전송: 사용자 권한, 언어, 시간

	// 전송: 설비 제어 (설비의 설비 진행 상태 변경)
	// Send_Request_Change_EqpProcessStatus( EqpProcessStatus );

	// 수신: 알람

	// 전송: 운용 모드 (설비의 운용 모드 변경)
	// Send_Request_Change_EqpOperMode( EqpOperMode );

	// 수신: 설비 상태

	// 수신: RFID 등록
	// 수신: RFID 투입 승인 요청
	// 수신: RFID 검사 결과 요청
	// 수신: RFID 검사 결과 등록
	// 수신: RFID 등록 해제

	// 전송: All Socket Clear

	// 수신: 버퍼 상태	
	// Recv_RequestArriveSocket( fromEqp, BufferType, RFID);
	// Send_ReplyArriveSocket( test/bypass/wait );

	// 수신: 컨베이어 상태


	// 수신: 검사 ZONE  상태 (L/R)

	// 수신: 설비 초기 상태 (Loader)
	// 수신: 설비 초기 상태 (Unloader)
	// 수신: 설비 초기 상태 (검사기)
	// Recv_EqpStatus ( buffer, Zone, return, eqpOpermode, EqpProcessStatus);

public:



};

#endif // LineControl_h__
