//*****************************************************************************
// Filename	: 	LineControl.h
// Created	:	2021/11/18 - 17:32
// Modified	:	2021/11/18 - 17:32
//
// Author	:	piring
//	
// Purpose	:	����� ��� �� �ڵ鷯 ����
//*****************************************************************************
#ifndef LineControl_h__
#define LineControl_h__

#pragma once

#include "LineInfo.h"

//-----------------------------------------------------------------------------
// ����� ��� �� �ڵ鷯 ����
//-----------------------------------------------------------------------------
class CLineControl : public CLineInfo
{
public:
	CLineControl();
	virtual ~CLineControl();

protected:

	// ���� ����(?)

	

	// Loader
	// Tester, Tester, ....
	// Return
	// Unloader


	// �˻� �Ϸ�
	// Recv_CompleteTest( ok / ng );
	// Send_Reply_CompleteTest();



	// ����: ���� �ð� ����ȭ
	// Send_TimeSync ( server_time );

	// ����: ����� ����, ���, �ð�

	// ����: ���� ���� (������ ���� ���� ���� ����)
	// Send_Request_Change_EqpProcessStatus( EqpProcessStatus );

	// ����: �˶�

	// ����: ��� ��� (������ ��� ��� ����)
	// Send_Request_Change_EqpOperMode( EqpOperMode );

	// ����: ���� ����

	// ����: RFID ���
	// ����: RFID ���� ���� ��û

	// ����: RFID �˻� ��� ��û

	// ����: RFID �˻� ��� ���
	// ����: RFID ��� ����

	// ����: All Socket Clear

	// ����: ���� ����
	// Recv_RequestArriveSocket( fromEqp, BufferType, RFID);
	// Send_ReplyArriveSocket( test/bypass/wait );

	// ����: �����̾� ����


	// ����: �˻� ZONE  ���� (L/R)

	// ����: ���� �ʱ� ���� (Loader)
	// ����: ���� �ʱ� ���� (Unloader)
	// ����: ���� �ʱ� ���� (�˻��)
	// Recv_EqpStatus ( buffer, Zone, return, eqpOpermode, EqpProcessStatus);

public:



};

#endif // LineControl_h__
