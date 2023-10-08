//*****************************************************************************
// Filename	: 	LineControl.h
// Created	:	2021/11/18 - 17:32
// Modified	:	2021/11/18 - 17:32
//
// Author	:	piring
//	
// Purpose	:	?¤ë¹„?€ ?µì‹  ë°??¸ë“¤???œì–´
//*****************************************************************************
#ifndef LineControl_h__
#define LineControl_h__

#pragma once

#include "LineInfo.h"

//-----------------------------------------------------------------------------
// ?¤ë¹„?€ ?µì‹  ë°??¸ë“¤???œì–´
//-----------------------------------------------------------------------------
class CLineControl : public CLineInfo
{
public:
	CLineControl();
	virtual ~CLineControl();

protected:

	// ?Œì¼“ ?•ë³´(?)

	

	// Loader
	// Tester, Tester, ....
	// Return
	// Unloader


	// ê²€???„ë£Œ
	// Recv_CompleteTest( ok / ng );
	// Send_Reply_CompleteTest();



	// ?„ì†¡: ?œë²„ ?œê°„ ?™ê¸°??	// Send_TimeSync ( server_time );

	// ?„ì†¡: ?¬ìš©??ê¶Œí•œ, ?¸ì–´, ?œê°„

	// ?„ì†¡: ?¤ë¹„ ?œì–´ (?¤ë¹„???¤ë¹„ ì§„í–‰ ?íƒœ ë³€ê²?
	// Send_Request_Change_EqpProcessStatus( EqpProcessStatus );

	// ?˜ì‹ : ?ŒëŒ

	// ?„ì†¡: ?´ìš© ëª¨ë“œ (?¤ë¹„???´ìš© ëª¨ë“œ ë³€ê²?
	// Send_Request_Change_EqpOperMode( EqpOperMode );

	// ?˜ì‹ : ?¤ë¹„ ?íƒœ

	// ?˜ì‹ : RFID ?±ë¡
	// ?˜ì‹ : RFID ?¬ì… ?¹ì¸ ?”ì²­
	// ?˜ì‹ : RFID ê²€??ê²°ê³¼ ?”ì²­
	// ?˜ì‹ : RFID ê²€??ê²°ê³¼ ?±ë¡
	// ?˜ì‹ : RFID ?±ë¡ ?´ì œ

	// ?„ì†¡: All Socket Clear

	// ?˜ì‹ : ë²„í¼ ?íƒœ	
	// Recv_RequestArriveSocket( fromEqp, BufferType, RFID);
	// Send_ReplyArriveSocket( test/bypass/wait );

	// ?˜ì‹ : ì»¨ë² ?´ì–´ ?íƒœ


	// ?˜ì‹ : ê²€??ZONE  ?íƒœ (L/R)

	// ?˜ì‹ : ?¤ë¹„ ì´ˆê¸° ?íƒœ (Loader)
	// ?˜ì‹ : ?¤ë¹„ ì´ˆê¸° ?íƒœ (Unloader)
	// ?˜ì‹ : ?¤ë¹„ ì´ˆê¸° ?íƒœ (ê²€?¬ê¸°)
	// Recv_EqpStatus ( buffer, Zone, return, eqpOpermode, EqpProcessStatus);

public:



};

#endif // LineControl_h__
