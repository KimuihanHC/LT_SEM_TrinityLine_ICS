#pragma once

#include "Util/StringUtil.hpp"

#include <vector>

#include <Rpc.h>
#pragma comment(lib, "Rpcrt4.lib")

//#include "xml3all.h"
//#include "Markup.h"
//using namespace XML3;

#define     DLL_AL_INFO_FILENAME			_T("SEM_AL_INFO.xml")

//#if USE_TEST
#define USE_UUID	0
//#define USE_XML	1
//#else
//#define USE_UUID	0
//#endif

namespace lt
{
#if USE_UUID
	using XUUID = UUID;
#else
	using XUUID = StdStringA;
#endif

	class CMSG;
	class CHEADER;
	class CRETURN;

	class CALARM_DATA
	{
	protected:
		StdStringA	ID;
		StdStringA	GRADE;
		StdStringA	MESSAGE;
	public:
		CALARM_DATA() {
			ID = "";
			GRADE = "";
			MESSAGE = "";
		};
		~CALARM_DATA() {
		};
		void Reset() {
			ID.clear();
			GRADE.clear();
			MESSAGE.clear();
		};
		StdStringA		Get_ID() const {
			return ID;
		};
		StdStringA		Get_GRADE() const {
			return GRADE;
		};
		StdStringA		Get_MESSAGE() const {
			return MESSAGE;
		};

		void		Set_ID(__in StdStringA	IN_PARA) {
			ID = IN_PARA;
		};
		void		Set_GRADE(__in StdStringA	IN_PARA) {
			GRADE = IN_PARA;
		};
		void		Set_MESSAGE(__in StdStringA	IN_PARA) {
			MESSAGE = IN_PARA;
		};
	};
	class CALARM_DataListCntr
	{
	public:
		std::vector<CALARM_DATA> LIST;
		CALARM_DataListCntr() {

		};
		CALARM_DataListCntr& operator= (const CALARM_DataListCntr& ref) {
			LIST.clear();
			LIST = ref.LIST;
			return *this;
		}
		size_t Get_Count() const {
			return LIST.size();
		};
		virtual void RemoveAll() {
			LIST.clear();
		};
		virtual void Add(
			__in CALARM_DATA IN_stTestStep) {
			LIST.push_back(IN_stTestStep);
		};
		virtual void Insert(
			__in uint16_t IN_nIdx,
			__in CALARM_DATA IN_stStep) {
			if (0 < LIST.size()) {
				LIST.insert(LIST.begin() + IN_nIdx, IN_stStep);
			}
		}
		virtual void Remove(
			__in uint16_t IN_nIdx) {
			if (0 < LIST.size()) {
				LIST.erase(LIST.begin() + IN_nIdx);
			}
		};
		virtual void Modify(
			__in uint16_t IN_nIdx,
			__in CALARM_DATA IN_stStep) {
			if (0 < LIST.size()) {
				LIST.at(IN_nIdx) = IN_stStep;
			}
		};
		virtual void Up(
			__in uint16_t IN_nIdx)
		{
			if (0 < LIST.size())
			{
				// 0ë²??¸ë±?¤ëŠ” ?„ë¡œ ?´ë™ ë¶ˆê?
				if ((0 < IN_nIdx) && (1 < LIST.size()))
				{
					CALARM_DATA stStep = LIST.at(IN_nIdx);

					LIST.erase(LIST.begin() + IN_nIdx);
					LIST.insert(LIST.begin() + IN_nIdx, stStep);
				}
			}
		};
		virtual void Down(
			__in uint16_t IN_nIdx)
		{
			if (0 < LIST.size())
			{
				// ë§ˆì?ë§??¸ë±?¤ëŠ” ?„ëž˜ë¡??´ë™ ë¶ˆê?
				if ((IN_nIdx < (LIST.size() - 1)) && (1 < LIST.size()))
				{
					CALARM_DATA stStep = LIST.at(IN_nIdx);

					LIST.erase(LIST.begin() + IN_nIdx);

					// ë³€ê²½ë˜???„ì¹˜ê°€ ìµœí•˜?¨ì´ë©? Insert ?€??Add ?¬ìš©
					if ((IN_nIdx + 1) < (LIST.size()))
					{
						LIST.insert(LIST.begin() + IN_nIdx, stStep);
					}
					else
					{
						LIST.push_back(stStep);
					}
				}
			}
		};
		virtual CALARM_DATA& GetAt(
			__in uint16_t IN_nIdx)
		{
			return LIST.at(IN_nIdx);
		}
		virtual const CALARM_DATA& GetAt(
			__in uint16_t IN_nIdx) const
		{
			return LIST.at(IN_nIdx);
		}
	};

	class CDEFINEDATA
	{
	protected:
		StdStringA	EQUIPMENTID = "";
		StdStringA	SUBEQPID = "";
		StdStringA	IPADDRESS = "";
		StdStringA	USERID = "";
		StdStringA	ONLINESTATE = "";
		StdStringA	EESMODE = "";
		StdStringA	PORTID = "";
		StdStringA	STEPID = "";
		StdStringA	LOTID = "";
		StdStringA	READBARCODEID = "";
		StdStringA	SOCKETBARCODEID = "";
		//None
		StdStringA	CARRTYPE = "";
		StdStringA	CONSUMLIST = "";
		StdStringA	EQPTYPECODE = "";
		StdStringA	SWVERSION = "";
		StdStringA	SWHASHCODE = "";
		StdStringA	SWMODULETYPE = "";
		CALARM_DataListCntr 	ALARMLIST;
	public:
		CDEFINEDATA() {
			EQUIPMENTID = "";
			SUBEQPID = "";
			IPADDRESS = "";
			USERID = "";
			ONLINESTATE = "";
			EESMODE = "";
			//PORTID = "";
			STEPID = "";
			LOTID = "";
			READBARCODEID = "";
			SOCKETBARCODEID = "";
			//None
			CARRTYPE = "";
			CONSUMLIST = "";
			EQPTYPECODE = "";
			SWVERSION = "";
			SWHASHCODE = "";
			SWMODULETYPE = "";

		};
		~CDEFINEDATA() {
		};

		CDEFINEDATA& operator= (const CDEFINEDATA& ref) {
			EQUIPMENTID = (ref.EQUIPMENTID);
			SUBEQPID = (ref.SUBEQPID);
			IPADDRESS = (ref.IPADDRESS);
			USERID = (ref.USERID);
			ONLINESTATE = (ref.ONLINESTATE);
			EESMODE = (ref.EESMODE);
			//PORTID = (ref.PORTID);
			STEPID = (ref.STEPID);
			LOTID = (ref.LOTID);
			READBARCODEID = (ref.READBARCODEID);
			SOCKETBARCODEID = (ref.SOCKETBARCODEID);
			//None
			CARRTYPE = (ref.CARRTYPE);
			CONSUMLIST = (ref.CONSUMLIST);
			EQPTYPECODE = (ref.EQPTYPECODE);
			SWVERSION = (ref.SWVERSION);
			SWHASHCODE = (ref.SWHASHCODE);
			SWMODULETYPE = (ref.SWMODULETYPE);
			ALARMLIST = (ref.ALARMLIST);
			return *this;
		};
		void Reset() {
			EQUIPMENTID.clear();
			SUBEQPID.clear();
			IPADDRESS.clear();
			USERID.clear();
			ONLINESTATE.clear();
			EESMODE.clear();
			//PORTID.clear();
			STEPID.clear();
			LOTID.clear();
			READBARCODEID.clear();
			SOCKETBARCODEID.clear();
			//None
			CARRTYPE.clear();
			CONSUMLIST.clear();
			EQPTYPECODE.clear();
			SWVERSION.clear();
			SWHASHCODE.clear();
			SWMODULETYPE.clear();
			ALARMLIST.RemoveAll();
		};
		StdStringA		Get_EQUIPMENTID() const {
			return EQUIPMENTID;
		};
		StdStringA		Get_SUBEQPID() const {
			return SUBEQPID;
		};
		StdStringA		Get_IPADDRESS() const {
			return IPADDRESS;
		};
		StdStringA		Get_USERID() const {
			return USERID;
		};
		StdStringA		Get_ONLINESTATE() const {
			return ONLINESTATE;
		};
		StdStringA		Get_EESMODE() const {
			return EESMODE;
		};
		StdStringA		Get_STEPID() const {
			return STEPID;
		};
		StdStringA		Get_LOTID() const {
			return LOTID;
		};
		StdStringA		Get_READBARCODEID() const {
			return READBARCODEID;
		};
		StdStringA		Get_SOCKETBARCODEID() const {
			return SOCKETBARCODEID;
		};
		CALARM_DataListCntr 	Get_ALARMLIST() const {
			return ALARMLIST;	};

		void		Set_EQUIPMENTID(__in StdStringA	IN_PARA) {
			EQUIPMENTID = IN_PARA;
		};
		void		Set_SUBEQPID(__in StdStringA	IN_PARA) {
			SUBEQPID = IN_PARA;
		};
		void		Set_IPADDRESS(__in StdStringA	IN_PARA) {
			IPADDRESS = IN_PARA;
		};
		void		Set_USERID(__in StdStringA	IN_PARA) {
			USERID = IN_PARA;
		};
		void		Set_ONLINESTATE(__in StdStringA	IN_PARA) {
			ONLINESTATE = IN_PARA;
		};
		void		Set_EESMODE(__in StdStringA	IN_PARA) {
			EESMODE = IN_PARA;
		};
		void		Set_STEPID(__in StdStringA	IN_PARA) {
			STEPID = IN_PARA;
		};
		void		Set_READBARCODEID(__in StdStringA	IN_PARA) {
			READBARCODEID = IN_PARA;
		};
		void		Set_SOCKETBARCODEID(__in StdStringA	IN_PARA) {
			SOCKETBARCODEID = IN_PARA;
		};
		void		Set_ALARMLIST(__in CALARM_DataListCntr 	IN_PARA) {
			ALARMLIST = IN_PARA;		};
	};

	class CHEADER 
	{
	protected:
		StdStringA	messageName;
		XUUID		transactionId;
		StdStringA	timeStamp;
	public:

		CHEADER() {
			messageName = "";
#if USE_UUID
			transactionId = {};
#else
			transactionId = "";
#endif
			timeStamp = "";			
		};
		~CHEADER() {
		};
		void Reset() {
			messageName.clear();
#if USE_UUID
			transactionId = {};
#else
			transactionId.clear();
#endif
			timeStamp.clear();
		};
		CHEADER& operator= (const CHEADER& ref) {
			messageName = (ref.messageName);
			transactionId = (ref.transactionId);
			timeStamp = (ref.timeStamp);
			return *this;
		};
		XUUID	CreateUUID() {
#if USE_UUID
			XUUID UUid;
			UuidCreate(&UUid);
			return UUid;
#else
			ConstStringA cstr;
			UUID uuid;
			UuidCreate(&uuid);
			UuidToStringA(&uuid, (RPC_CSTR*)&cstr);
			transactionId = XUUID(cstr);
			return XUUID(cstr);
#endif

		}
		StdStringA		Get_messageName() const {
			return messageName;		};
		XUUID		Get_transactionId() const {
			return transactionId;		};
		StdStringA	Get_transactionId_XuuidToStr() {
#if USE_UUID
			ConstStringA cstr;
			UuidToStringA(&transactionId, (RPC_CSTR*)&cstr);
			return StdStringA(cstr);
#else
			return transactionId;
#endif
		};
		StdStringA		Get_timeStamp() const {
			SYSTEMTIME stime = {};
			::GetLocalTime(&stime);		
			StdStringA ssa = Format(("%04d%02d%02d%02d%02d%03d"),
				stime.wYear,
				stime.wMonth,
				stime.wDay,
				stime.wHour,
				stime.wMinute,
				stime.wSecond);;
			return StdStringA(ssa);
		};
		void		Set_messageName(__in StdStringA	IN_PARA) {
			messageName = IN_PARA;		};
		void		Set_transactionId_StrToXuuid(__in StdStringA	IN_PARA) {
#if USE_UUID
			ConstStringA cst = IN_PARA.c_str();
			RPC_CSTR cstr = RPC_CSTR(cst);
			UuidFromStringA(cstr, &transactionId);
#else
			transactionId = IN_PARA;
#endif
		};
		void		Set_transactionId(__in XUUID	IN_PARA) {
			transactionId = IN_PARA;		};
		void		Set_timeStamp(__in StdStringA	IN_PARA) {
			timeStamp = IN_PARA;		};
	};
	class CRETURN
	{
	protected:
		StdStringA	returnCode;	// Version
		StdStringA	returnMessage;	// ID
	public:
		CRETURN() {
			returnCode = "";
			returnMessage = "";
		};
		~CRETURN() {
		};
		void Reset() {
			returnCode.clear();
			returnMessage.clear();
		};
		CRETURN& operator= (const CRETURN& ref) {
			returnCode = (ref.returnCode);
			returnMessage = (ref.returnMessage);
			return *this;
		};

		StdStringA		Get_returnCode() const {
			return returnCode;		};
		StdStringA		Get_returnMessage() const {
			return returnMessage;		};

		void		Set_returnCode(__in StdStringA	IN_PARA) {
			returnCode = IN_PARA;		};
		void		Set_returnMessage(__in StdStringA	IN_PARA) {
			returnMessage = IN_PARA;		};
	};
	class CMSG
	{
	protected:
		StdStringA	Txt = "";
	public:
		CMSG() {
			Txt = "";
		};
		~CMSG() {
		};		
//		CMSG& operator= (const ST_xml_Msg& ref) {
			//Txt = ToMultiByte(ref.Msg);
			//return *this;		};
		CMSG& operator= (const CMSG& ref) {
			Txt = (ref.Txt);
			return *this;		};
		void Reset() {
			Txt.clear();		};
		StdStringA		Get_MSG() const {
			return Txt;		};
		void		Set_MSG(__in StdStringA	IN_PARA) {
			Txt = IN_PARA;		};
	};

	class CREQUEST_LINK_TEST_BODY
	{
	protected:
		StdStringA	EQUIPMENTID;	// Version
		StdStringA	IPADDRESS;	// ID
		StdStringA	APPLICATIONID;		// Name
	public:
		CREQUEST_LINK_TEST_BODY() {
			EQUIPMENTID = "";
			IPADDRESS = "";
			APPLICATIONID = "";
		};
		~CREQUEST_LINK_TEST_BODY() {
		};
		void Reset() {
			EQUIPMENTID.clear();
			IPADDRESS.clear();
			APPLICATIONID.clear();
		};
		CREQUEST_LINK_TEST_BODY& operator= (const CREQUEST_LINK_TEST_BODY& ref) {
			EQUIPMENTID = (ref.EQUIPMENTID);
			IPADDRESS = (ref.IPADDRESS);
			APPLICATIONID = (ref.APPLICATIONID);
			return *this;
		};

		StdStringA		Get_EQUIPMENTID() const {
			return EQUIPMENTID;		};
		StdStringA		Get_IPADDRESS() const {
			return IPADDRESS;		};
		StdStringA		Get_APPLICATIONID() const {
			return APPLICATIONID;		};

		void		Set_EQUIPMENTID(__in StdStringA	IN_PARA) {
			EQUIPMENTID = IN_PARA;		};
		void		Set_IPADDRESS(__in StdStringA	IN_PARA) {
			IPADDRESS = IN_PARA;		};
		void		Set_APPLICATIONID(__in StdStringA	IN_PARA) {
			APPLICATIONID = IN_PARA;		};
	};
	class CREQUEST_LINK_TEST
	{
	public:
		CHEADER Hd;
		CREQUEST_LINK_TEST_BODY Body;

		CREQUEST_LINK_TEST() {
		};
		~CREQUEST_LINK_TEST() {
		};
		void Reset() {
			Body.Reset();
			Hd.Reset();
			//Msg.Reset();
		};
		CREQUEST_LINK_TEST& operator= (const CREQUEST_LINK_TEST& ref) {
			Hd.Reset();
			Hd.operator=(ref.Hd);
			Body.Reset();
			Body.operator=(ref.Body);
			return *this;
		};
	};
	class CREPLY_LINK_TEST_BODY
	{
	protected:
		StdStringA	EQUIPMENTID;	// Version
		StdStringA	IPADDRESS;	// ID
		StdStringA	APPLICATIONID;		// Name
	public:
		CREPLY_LINK_TEST_BODY() {
			EQUIPMENTID = "";
			IPADDRESS = "";
			APPLICATIONID = "";
		};
		~CREPLY_LINK_TEST_BODY() {
		};
		void Reset() {
			EQUIPMENTID.clear();
			IPADDRESS.clear();
			APPLICATIONID.clear();
		};
		CREPLY_LINK_TEST_BODY& operator= (const CREPLY_LINK_TEST_BODY& ref) {
			EQUIPMENTID = (ref.EQUIPMENTID);
			IPADDRESS = (ref.IPADDRESS);
			APPLICATIONID = (ref.APPLICATIONID);
			return *this;
		};

		StdStringA		Get_EQUIPMENTID() const {
			return EQUIPMENTID;
		};
		StdStringA		Get_IPADDRESS() const {
			return IPADDRESS;
		};
		StdStringA		Get_APPLICATIONID() const {
			return APPLICATIONID;
		};
		void		Set_EQUIPMENTID(__in StdStringA	IN_PARA) {
			EQUIPMENTID = IN_PARA;
		};
		void		Set_IPADDRESS(__in StdStringA	IN_PARA) {
			IPADDRESS = IN_PARA;
		};
		void		Set_APPLICATIONID(__in StdStringA	IN_PARA) {
			APPLICATIONID = IN_PARA;
		};
	};
	class CREPLY_LINK_TEST
	{
	public:
		CHEADER Hd;
		CREPLY_LINK_TEST_BODY Body;
		CRETURN Rtrn;
		//CMSG Msg;
		CREPLY_LINK_TEST() {
			Hd.Set_messageName("REPLY_LINK_TEST");
			//Hd.Get_timeStamp();
		};
		~CREPLY_LINK_TEST() {
		};
		void Reset() {
			Hd.Reset();
			Body.Reset();
			Rtrn.Reset();
			//Msg.Reset();
		};
		CREPLY_LINK_TEST& operator= (const CREPLY_LINK_TEST& ref) {
			Hd.Reset();
			Hd.operator=(ref.Hd);
			Body.Reset();
			Body.operator=(ref.Body);
			Rtrn.Reset();
			Rtrn.operator=(ref.Rtrn);
			//Msg.Reset();
			//Msg.operator=(ref.Msg);
			return *this;
		};
		CREPLY_LINK_TEST& operator= (const CDEFINEDATA& ref) {
			Hd.Set_timeStamp(Hd.Get_timeStamp());
			Body.Set_EQUIPMENTID(ref.Get_EQUIPMENTID());
			Body.Set_IPADDRESS(ref.Get_IPADDRESS());
			return *this;
		};
	};
	class CLINK_TEST
	{
	public:
		CREQUEST_LINK_TEST REQUEST;
		CREPLY_LINK_TEST REPLY;
		CLINK_TEST() {

		};
		~CLINK_TEST() {
		};
		void Reset() {
			REQUEST.Reset();
			REPLY.Reset();
		};
		void RequestToReply() {
			REPLY.Hd.Set_transactionId(REQUEST.Hd.Get_transactionId());
			REPLY.Body.Set_EQUIPMENTID(REQUEST.Body.Get_EQUIPMENTID());
			REPLY.Body.Set_IPADDRESS(REQUEST.Body.Get_IPADDRESS());
			REPLY.Body.Set_APPLICATIONID(REQUEST.Body.Get_APPLICATIONID());
		};
		CLINK_TEST& operator= (const CLINK_TEST& ref) {
			REQUEST.Reset();
			REQUEST.operator=(ref.REQUEST);
			REPLY.Reset();
			REPLY.operator=(ref.REPLY);
			return *this;
		};
	};

	class CREPORT_ONLINE_STATE_BODY
	{
	protected:
		StdStringA	EQUIPMENTID;
		StdStringA	SUBEQPID;
		StdStringA	IPADDRESS;
		StdStringA	USERID;
		StdStringA	EESMODE;
		StdStringA	LOTID;
		StdStringA	RECIPEID;
		StdStringA	PRODUCTID;
		StdStringA	PRGVER;
		StdStringA	ONLINESTATE;
		StdStringA	EQUIPMENTSTATE;
	public:
		CREPORT_ONLINE_STATE_BODY() {
			EQUIPMENTID = "";
			SUBEQPID = "";
			IPADDRESS = "";
			USERID = "";
			EESMODE = "";
			LOTID = "";
			RECIPEID = "";
			PRODUCTID = "";
			PRGVER = "";
			ONLINESTATE = "";
			EQUIPMENTSTATE = "";
		};
		~CREPORT_ONLINE_STATE_BODY() {
		};
		void Reset() {
			EQUIPMENTID.clear();
			SUBEQPID.clear();
			IPADDRESS.clear();
			USERID.clear();
			EESMODE.clear();
			LOTID.clear();
			RECIPEID.clear();
			PRODUCTID.clear();
			PRGVER.clear();
			ONLINESTATE.clear();
			EQUIPMENTSTATE.clear();
		};
		CREPORT_ONLINE_STATE_BODY& operator= (const CREPORT_ONLINE_STATE_BODY& ref) {
			EQUIPMENTID = (ref.EQUIPMENTID);
			SUBEQPID = (ref.SUBEQPID);
			IPADDRESS = (ref.IPADDRESS);
			USERID = (ref.USERID);
			EESMODE = (ref.EESMODE);
			LOTID = (ref.LOTID);
			RECIPEID = (ref.RECIPEID);
			PRODUCTID = (ref.PRODUCTID);
			PRGVER = (ref.PRGVER);
			ONLINESTATE = (ref.ONLINESTATE);
			EQUIPMENTSTATE = (ref.EQUIPMENTSTATE);
			return *this;
		};
		StdStringA		Get_EQUIPMENTID() const {
			return EQUIPMENTID;};
		StdStringA		Get_SUBEQPID() const {
			return SUBEQPID;};
		StdStringA		Get_IPADDRESS() const {
			return IPADDRESS;};
		StdStringA		Get_USERID() const {
			return USERID;};
		StdStringA		Get_EESMODE() const {
			return EESMODE;};
		StdStringA		Get_LOTID() const {
			return LOTID;};
		StdStringA		Get_RECIPEID() const {
			return RECIPEID;};
		StdStringA		Get_PRODUCTID() const {
			return PRODUCTID;};
		StdStringA		Get_PRGVER() const {
			return PRGVER;};
		StdStringA		Get_ONLINESTATE() const {
			return ONLINESTATE;};
		StdStringA		Get_EQUIPMENTSTATE() const {
			return EQUIPMENTSTATE;};

		void		Set_EQUIPMENTID(__in StdStringA	IN_PARA) {
			EQUIPMENTID = IN_PARA;		};
		void		Set_SUBEQPID(__in StdStringA	IN_PARA) {
			SUBEQPID = IN_PARA;		};
		void		Set_IPADDRESS(__in StdStringA	IN_PARA) {
			IPADDRESS = IN_PARA;	};
		void		Set_USERID(__in StdStringA	IN_PARA) {
			USERID = IN_PARA;		};
		void		Set_EESMODE(__in StdStringA	IN_PARA) {
			EESMODE = IN_PARA;		};
		void		Set_LOTID(__in StdStringA	IN_PARA) {
			LOTID = IN_PARA;		};
		void		Set_RECIPEID(__in StdStringA	IN_PARA) {
			RECIPEID = IN_PARA;		};
		void		Set_PRODUCTID(__in StdStringA	IN_PARA) {
			PRODUCTID = IN_PARA;		};
		void		Set_PRGVER(__in StdStringA	IN_PARA) {
			PRGVER = IN_PARA;		};
		void		Set_ONLINESTATE(__in StdStringA	IN_PARA) {
			ONLINESTATE = IN_PARA;	};
		void		Set_EQUIPMENTSTATE(__in StdStringA	IN_PARA) {
			EQUIPMENTSTATE = IN_PARA;	};
	};
	class CREPORT_ONLINE_STATE
	{
	public:
		CHEADER Hd;
		CREPORT_ONLINE_STATE_BODY Body;
		CREPORT_ONLINE_STATE() {
			Hd.Set_messageName("REPORT_ONLINE_STATE");
		};
		~CREPORT_ONLINE_STATE() {
		};
		void Reset() {
			Hd.Reset();
			Body.Reset();
		};
		CREPORT_ONLINE_STATE& operator= (const CREPORT_ONLINE_STATE& ref) {
			Hd.Reset();
			Hd.operator=(ref.Hd);
			Body.Reset();
			Body.operator=(ref.Body);
			return *this;
		};
		CREPORT_ONLINE_STATE& operator= (const CDEFINEDATA& ref) {
			Hd.Set_timeStamp(Hd.Get_timeStamp());
			Body.Set_EQUIPMENTID(ref.Get_EQUIPMENTID());
			Body.Set_SUBEQPID(ref.Get_SUBEQPID());
			Body.Set_IPADDRESS(ref.Get_IPADDRESS());
			Body.Set_EESMODE(ref.Get_EESMODE());
			Body.Set_USERID(ref.Get_USERID());
			Body.Set_ONLINESTATE(ref.Get_ONLINESTATE());
			return *this;
		};
	};
	class CONLINE_STATE
	{
	public:
		CREPORT_ONLINE_STATE REPORT;
		CONLINE_STATE() {
			REPORT.Hd.CreateUUID();
		};
		~CONLINE_STATE() {
		};
		void Reset() {
			REPORT.Reset();
		};
		CONLINE_STATE& operator= (const CONLINE_STATE& ref) {
			REPORT.Reset();
			REPORT.operator=(ref.REPORT);
			return *this;
		};
	};

	class CREPORT_USER_CHANGE_BODY
	{
	protected:
		StdStringA	EQUIPMENTID;
		StdStringA	IPADDRESS;
		StdStringA	USERID;
		StdStringA	EESMODE;
	public:
		CREPORT_USER_CHANGE_BODY() {
			EQUIPMENTID = "";
			IPADDRESS = "";
			USERID = "";
			EESMODE = "";
		};
		~CREPORT_USER_CHANGE_BODY() {
		};
		void Reset() {
			EQUIPMENTID.clear();
			IPADDRESS.clear();
			USERID.clear();
			EESMODE.clear();
		};
		CREPORT_USER_CHANGE_BODY& operator= (const CREPORT_USER_CHANGE_BODY& ref) {
			EQUIPMENTID = (ref.EQUIPMENTID);
			IPADDRESS = (ref.IPADDRESS);
			USERID = (ref.USERID);
			EESMODE = (ref.EESMODE);
			return *this;
		};
		StdStringA		Get_EQUIPMENTID() const {
			return EQUIPMENTID;		};
		StdStringA		Get_IPADDRESS() const {
			return IPADDRESS;		};
		StdStringA		Get_USERID() const {
			return USERID;		};
		StdStringA		Get_EESMODE() const {
			return EESMODE;		};

		void		Set_EQUIPMENTID(__in StdStringA	IN_PARA) {
			EQUIPMENTID = IN_PARA;		};
		void		Set_IPADDRESS(__in StdStringA	IN_PARA) {
			IPADDRESS = IN_PARA;		};
		void		Set_USERID(__in StdStringA	IN_PARA) {
			USERID = IN_PARA;		};
		void		Set_EESMODE(__in StdStringA	IN_PARA) {
			EESMODE = IN_PARA;		};
	};
	class CREPORT_USER_CHANGE
	{
	public:
		CHEADER Hd;
		CREPORT_USER_CHANGE_BODY Body;
		//CMSG Msg;
		CREPORT_USER_CHANGE() {
			Hd.Set_messageName("REPORT_USER_CHANGE");
			Hd.Get_timeStamp();
		};
		~CREPORT_USER_CHANGE() {
		};
		void Reset() {
			Hd.Reset();
			Body.Reset();
			//Msg.Reset();
		};
		CREPORT_USER_CHANGE& operator= (const CREPORT_USER_CHANGE& ref) {
			Hd.Reset();
			Hd.operator=(ref.Hd);
			Body.Reset();
			Body.operator=(ref.Body);
			return *this;
		};
		CREPORT_USER_CHANGE& operator= (const CDEFINEDATA& ref) {
			Hd.Set_timeStamp(Hd.Get_timeStamp());
			Body.Set_EQUIPMENTID(ref.Get_EQUIPMENTID());
			//Body.Set_SUBEQPID(ref.Get_SUBEQPID());
			Body.Set_IPADDRESS(ref.Get_IPADDRESS());
			Body.Set_EESMODE(ref.Get_EESMODE());
			Body.Set_USERID(ref.Get_USERID());
			//Body.Set_ONLINESTATE(ref.Get_ONLINESTATE());
			return *this;
		};
	};
	class CREQUEST_USER_COMMAND_BODY
	{
	protected:
		StdStringA	EQUIPMENTID;
		StdStringA	IPADDRESS;
		StdStringA	USERID;
	public:
		CREQUEST_USER_COMMAND_BODY() {
			EQUIPMENTID = "";
			IPADDRESS = "";
			USERID = "";
		};
		~CREQUEST_USER_COMMAND_BODY() {
		};
		void Reset() {
			EQUIPMENTID.clear();
			IPADDRESS.clear();
			USERID.clear();
		};
		CREQUEST_USER_COMMAND_BODY& operator= (const CREQUEST_USER_COMMAND_BODY& ref) {
			EQUIPMENTID = (ref.EQUIPMENTID);
			IPADDRESS = (ref.IPADDRESS);
			USERID = (ref.USERID);
			return *this;
		};
		StdStringA		Get_EQUIPMENTID() const {
			return EQUIPMENTID;		};
		StdStringA		Get_IPADDRESS() const {
			return IPADDRESS;		};
		StdStringA		Get_USERID() const {
			return USERID;		};

		void		Set_EQUIPMENTID(__in StdStringA	IN_PARA) {
			EQUIPMENTID = IN_PARA;		};
		void		Set_IPADDRESS(__in StdStringA	IN_PARA) {
			IPADDRESS = IN_PARA;		};
		void		Set_USERID(__in StdStringA	IN_PARA) {
			USERID = IN_PARA;		};
	};
	class CREQUEST_USER_COMMAND
	{
	public:
		CHEADER Hd;
		CREQUEST_USER_COMMAND_BODY Body;
		CRETURN Rtrn;
		//CMSG Msg;
		CREQUEST_USER_COMMAND() {

		};
		~CREQUEST_USER_COMMAND() {
		};
		void Reset() {
			Hd.Reset();
			Body.Reset();
			Rtrn.Reset();
			//Msg.Reset();
		};
		CREQUEST_USER_COMMAND& operator= (const CREQUEST_USER_COMMAND& ref) {
			Hd.Reset();
			Hd.operator=(ref.Hd);
			Body.Reset();
			Body.operator=(ref.Body);
			Rtrn.Reset();
			Rtrn.operator=(ref.Rtrn);
			return *this;
		};
	};
	class CREPLY_USER_COMMAND_BODY
	{
	protected:
		StdStringA	EQUIPMENTID;
		StdStringA	IPADDRESS;
		StdStringA	USERID;
	public:
		CREPLY_USER_COMMAND_BODY() {
			EQUIPMENTID = "";
			IPADDRESS = "";
			USERID = "";
		};
		~CREPLY_USER_COMMAND_BODY() {
		};
		void Reset() {
			EQUIPMENTID.clear();
			IPADDRESS.clear();
			USERID.clear();
		};
		CREPLY_USER_COMMAND_BODY& operator= (const CREPLY_USER_COMMAND_BODY& ref) {
			EQUIPMENTID = (ref.EQUIPMENTID);
			IPADDRESS = (ref.IPADDRESS);
			USERID = (ref.USERID);
			return *this;
		};
		StdStringA		Get_EQUIPMENTID() const {
			return EQUIPMENTID;
		};
		StdStringA		Get_IPADDRESS() const {
			return IPADDRESS;
		};
		StdStringA		Get_USERID() const {
			return USERID;
		};

		void		Set_EQUIPMENTID(__in StdStringA	IN_PARA) {
			EQUIPMENTID = IN_PARA;
		};
		void		Set_IPADDRESS(__in StdStringA	IN_PARA) {
			IPADDRESS = IN_PARA;
		};
		void		Set_USERID(__in StdStringA	IN_PARA) {
			USERID = IN_PARA;
		};
	};
	class CREPLY_USER_COMMAND
	{
	public:
		CHEADER Hd;
		CREPLY_USER_COMMAND_BODY Body;
		CRETURN Rtrn;
		//CMSG Msg;
		CREPLY_USER_COMMAND() {
			Hd.Set_messageName("REPLY_USER_COMMAND");
			Hd.Get_timeStamp();
		};
		~CREPLY_USER_COMMAND() {
		};
		void Reset() {
			Hd.Reset();
			Body.Reset();
			Rtrn.Reset();
			//Msg.Reset();
		};
		CREPLY_USER_COMMAND& operator= (const CREPLY_USER_COMMAND& ref) {
			Hd.Reset();
			Hd.operator=(ref.Hd);
			Body.Reset();
			Body.operator=(ref.Body);
			Rtrn.Reset();
			Rtrn.operator=(ref.Rtrn);
			return *this;
		};
		CREPLY_USER_COMMAND& operator= (const CDEFINEDATA& ref) {
			Hd.Set_timeStamp(Hd.Get_timeStamp());
			Body.Set_EQUIPMENTID(ref.Get_EQUIPMENTID());
			//Body.Set_SUBEQPID(ref.Get_SUBEQPID());
			Body.Set_IPADDRESS(ref.Get_IPADDRESS());
			//Body.Set_EESMODE(ref.Get_EESMODE());
			Body.Set_USERID(ref.Get_USERID());
			//Body.Set_ONLINESTATE(ref.Get_ONLINESTATE());
			return *this;
		};
	};
	class CUSER_COMMAND
	{
	public:
		CREPORT_USER_CHANGE REPORT;
		CREQUEST_USER_COMMAND REQUEST;
		CREPLY_USER_COMMAND REPLY;
		CUSER_COMMAND() {
			REPORT.Hd.CreateUUID();
		};
		~CUSER_COMMAND() {
		};
		void Reset() {
			REPORT.Reset();
			REQUEST.Reset();
			REPLY.Reset();
		};
		void RequestToReply() {
			REPLY.Hd.Set_transactionId(REQUEST.Hd.Get_transactionId());
			REPLY.Body.Set_EQUIPMENTID(REQUEST.Body.Get_EQUIPMENTID());
			REPLY.Body.Set_IPADDRESS(REQUEST.Body.Get_IPADDRESS());
			REPLY.Body.Set_USERID(REQUEST.Body.Get_USERID());
		};
		CUSER_COMMAND& operator= (const CUSER_COMMAND& ref) {
			REPORT.Reset();
			REPORT.operator=(ref.REPORT);
			REQUEST.Reset();
			REQUEST.operator=(ref.REQUEST);
			REPLY.Reset();
			REPLY.operator=(ref.REPLY);
			return *this;
		};
	};

	class CREPORT_EQUIPMENT_STATE_BODY
	{
	protected:
		StdStringA	EQUIPMENTID  ;
		StdStringA	SUBEQPID ;
		//StdStringA	PORTID;		//PORT IDÃß°¡
		StdStringA	IPADDRESS;
		StdStringA	USERID;
		StdStringA	EESMODE;
		StdStringA	LOTID;
		StdStringA	RECIPEID;
		StdStringA	PRODUCTID;
		StdStringA	PRGVER;
		StdStringA	EQUIPMENTSTATE;
	public:
		CREPORT_EQUIPMENT_STATE_BODY() {
			EQUIPMENTID ="";
			SUBEQPID = "";
			//PORTID= "";		//PORT IDÃß°¡
			IPADDRESS = "";
			USERID = "";
			EESMODE = "";
			LOTID = "";
			RECIPEID = "";
			PRODUCTID = "";
			PRGVER = "";
			EQUIPMENTSTATE = "";
		};
		~CREPORT_EQUIPMENT_STATE_BODY() {
		};
		void Reset() {
			EQUIPMENTID.clear();
			SUBEQPID.clear();
			IPADDRESS.clear();
			USERID.clear();
			EESMODE.clear();
			LOTID.clear();
			RECIPEID.clear();
			PRODUCTID.clear();
			PRGVER.clear();
			EQUIPMENTSTATE.clear();
		};
		CREPORT_EQUIPMENT_STATE_BODY& operator= (const CREPORT_EQUIPMENT_STATE_BODY& ref) {
			EQUIPMENTID = (ref.EQUIPMENTID);
			SUBEQPID = (ref.SUBEQPID);
			IPADDRESS = (ref.IPADDRESS);
			USERID = (ref.USERID);
			EESMODE = (ref.EESMODE);
			LOTID = (ref.LOTID);
			RECIPEID = (ref.RECIPEID);
			PRODUCTID = (ref.PRODUCTID);
			PRGVER = (ref.PRGVER);
			EQUIPMENTSTATE = (ref.EQUIPMENTSTATE);
			return *this;
		};
		StdStringA		Get_EQUIPMENTID() const {
			return EQUIPMENTID;		};
		StdStringA		Get_SUBEQPID() const {
			return SUBEQPID;		};
		StdStringA		Get_IPADDRESS() const {
			return IPADDRESS;		};
		StdStringA		Get_USERID() const {
			return USERID;		};
		StdStringA		Get_EESMODE() const {
			return EESMODE;		};
		StdStringA		Get_LOTID() const {
			return LOTID;		};
		StdStringA		Get_RECIPEID() const {
			return RECIPEID;		};
		StdStringA		Get_PRODUCTID() const {
			return PRODUCTID;		};
		StdStringA		Get_PRGVER() const {
			return PRGVER;		};
		StdStringA		Get_EQUIPMENTSTATE() const {
			return EQUIPMENTSTATE;		};

		void		Set_EQUIPMENTID(__in StdStringA	IN_PARA) {
			EQUIPMENTID = IN_PARA;		};
		void		Set_SUBEQPID(__in StdStringA	IN_PARA) {
			SUBEQPID = IN_PARA;		};
		void		Set_IPADDRESS(__in StdStringA	IN_PARA) {
			IPADDRESS = IN_PARA;		};
		void		Set_USERID(__in StdStringA	IN_PARA) {
			USERID = IN_PARA;		};
		void		Set_EESMODE(__in StdStringA	IN_PARA) {
			EESMODE = IN_PARA;		};
		void		Set_LOTID(__in StdStringA	IN_PARA) {
			LOTID = IN_PARA;		};
		void		Set_RECIPEID(__in StdStringA	IN_PARA) {
			RECIPEID = IN_PARA;		};
		void		Set_PRODUCTID(__in StdStringA	IN_PARA) {
			PRODUCTID = IN_PARA;		};
		void		Set_PRGVER(__in StdStringA	IN_PARA) {
			PRGVER = IN_PARA;		};
		void		Set_EQUIPMENTSTATE(__in StdStringA	IN_PARA) {
			EQUIPMENTSTATE = IN_PARA;		};
	};
	class CREPORT_EQUIPMENT_STATE
	{
	public:
		CHEADER Hd;
		CREPORT_EQUIPMENT_STATE_BODY Body;
		//CMSG Msg;
		CREPORT_EQUIPMENT_STATE() {
			Hd.Set_messageName("REPORT_EQUIPMENT_STATE");
			Hd.Get_timeStamp();
		};
		~CREPORT_EQUIPMENT_STATE() {
		};
		void Reset() {
			Hd.Reset();
			Body.Reset();
			//Msg.Reset();
		};
		CREPORT_EQUIPMENT_STATE& operator= (const CREPORT_EQUIPMENT_STATE& ref) {
			Hd.Reset();
			Hd.operator=(ref.Hd);
			Body.Reset();
			Body.operator=(ref.Body);
			return *this;
		};
		CREPORT_EQUIPMENT_STATE& operator= (const CDEFINEDATA& ref) {
			Hd.Set_timeStamp(Hd.Get_timeStamp());
			Body.Set_EQUIPMENTID(ref.Get_EQUIPMENTID());
			Body.Set_SUBEQPID(ref.Get_SUBEQPID());
			Body.Set_IPADDRESS(ref.Get_IPADDRESS());
			Body.Set_EESMODE(ref.Get_EESMODE());
			Body.Set_USERID(ref.Get_USERID());
			//Body.Set_ONLINESTATE(ref.Get_ONLINESTATE());
			return *this;
		};
	};
	class CREQUEST_EQUIPMENT_STATE_DISPLAY_BODY
	{
	protected:
		StdStringA	EQUIPMENTID;
		StdStringA	SUBEQPID;
		StdStringA	EQUIPMENTSTATEDISPLAY;
		StdStringA	RGBDISPLAY;
	public:
		CREQUEST_EQUIPMENT_STATE_DISPLAY_BODY() {
			EQUIPMENTID = "";
			SUBEQPID = "";
			EQUIPMENTSTATEDISPLAY = "";
			RGBDISPLAY = "";
		};
		~CREQUEST_EQUIPMENT_STATE_DISPLAY_BODY() {
		};
		void Reset() {
			EQUIPMENTID.clear();
			SUBEQPID.clear();
			EQUIPMENTSTATEDISPLAY.clear();
			RGBDISPLAY.clear();
		};
		CREQUEST_EQUIPMENT_STATE_DISPLAY_BODY& operator= (const CREQUEST_EQUIPMENT_STATE_DISPLAY_BODY& ref) {
			EQUIPMENTID = (ref.EQUIPMENTID);
			SUBEQPID = (ref.SUBEQPID);
			EQUIPMENTSTATEDISPLAY = (ref.EQUIPMENTSTATEDISPLAY);
			RGBDISPLAY = (ref.RGBDISPLAY);
			return *this;
		};
		StdStringA		Get_EQUIPMENTID() const {
			return EQUIPMENTID;		};
		StdStringA		Get_SUBEQPID() const {
			return SUBEQPID;		};
		StdStringA		Get_EQUIPMENTSTATEDISPLAY() const {
			return EQUIPMENTSTATEDISPLAY;		};
		StdStringA		Get_RGBDISPLAY() const {
			return RGBDISPLAY;
		};
		void		Set_EQUIPMENTID(__in StdStringA	IN_PARA) {
			EQUIPMENTID = IN_PARA;		};
		void		Set_SUBEQPID(__in StdStringA	IN_PARA) {
			SUBEQPID = IN_PARA;		};
		void		Set_EQUIPMENTSTATEDISPLAY(__in StdStringA	IN_PARA) {
			EQUIPMENTSTATEDISPLAY = IN_PARA;		};
		void		Set_RGBDISPLAY(__in StdStringA	IN_PARA) {
			RGBDISPLAY = IN_PARA;		};
	};
	class CREQUEST_EQUIPMENT_STATE_DISPLAY
	{
	public:
		CHEADER Hd;
		CREQUEST_EQUIPMENT_STATE_DISPLAY_BODY Body;
		CREQUEST_EQUIPMENT_STATE_DISPLAY() {

		};
		~CREQUEST_EQUIPMENT_STATE_DISPLAY() {
		};
		void Reset() {
			Hd.Reset();
			Body.Reset();
		};
		CREQUEST_EQUIPMENT_STATE_DISPLAY& operator= (const CREQUEST_EQUIPMENT_STATE_DISPLAY& ref) {
			Hd.Reset();
			Hd.operator=(ref.Hd);
			Body.Reset();
			Body.operator=(ref.Body);
			return *this;
		};
	};
	class CREPLY_EQUIPMENT_STATE_DISPLAY_BODY
	{
	protected:
		StdStringA	EQUIPMENTID;
		StdStringA	SUBEQPID;
		StdStringA	EQUIPMENTSTATEDISPLAY;
		StdStringA	RGBDISPLAY;
	public:
		CREPLY_EQUIPMENT_STATE_DISPLAY_BODY() {
			EQUIPMENTID = "";
			SUBEQPID = "";
			EQUIPMENTSTATEDISPLAY = "";
			RGBDISPLAY = "";
		};
		~CREPLY_EQUIPMENT_STATE_DISPLAY_BODY() {
		};
		void Reset() {
			EQUIPMENTID.clear();
			SUBEQPID.clear();
			EQUIPMENTSTATEDISPLAY.clear();
			RGBDISPLAY.clear();
		};
		CREPLY_EQUIPMENT_STATE_DISPLAY_BODY& operator= (const CREPLY_EQUIPMENT_STATE_DISPLAY_BODY& ref) {
			EQUIPMENTID = (ref.EQUIPMENTID);
			SUBEQPID = (ref.SUBEQPID);
			EQUIPMENTSTATEDISPLAY = (ref.EQUIPMENTSTATEDISPLAY);
			RGBDISPLAY = (ref.RGBDISPLAY);
			return *this;
		};
		StdStringA		Get_EQUIPMENTID() const {
			return EQUIPMENTID;		};
		StdStringA		Get_SUBEQPID() const {
			return SUBEQPID;		};
		StdStringA		Get_EQUIPMENTSTATEDISPLAY() const {
			return EQUIPMENTSTATEDISPLAY;		};
		StdStringA		Get_RGBDISPLAY() const {
			return RGBDISPLAY;		};
		void		Set_EQUIPMENTID(__in StdStringA	IN_PARA) {
			EQUIPMENTID = IN_PARA;		};
		void		Set_SUBEQPID(__in StdStringA	IN_PARA) {
			SUBEQPID = IN_PARA;		};
		void		Set_EQUIPMENTSTATEDISPLAY(__in StdStringA	IN_PARA) {
			EQUIPMENTSTATEDISPLAY = IN_PARA;		};
		void		Set_RGBDISPLAY(__in StdStringA	IN_PARA) {
			RGBDISPLAY = IN_PARA;		};
	};
	class CREPLY_EQUIPMENT_STATE_DISPLAY
	{
	public:
		CHEADER Hd;
		CREPLY_EQUIPMENT_STATE_DISPLAY_BODY Body;
		CRETURN Rtrn;
		//CMSG Msg;
		CREPLY_EQUIPMENT_STATE_DISPLAY() {
			Hd.Set_messageName("REPLY_EQUIPMENT_STATE_DISPLAY");
			Hd.Get_timeStamp();
		};
		~CREPLY_EQUIPMENT_STATE_DISPLAY() {
		};
		void Reset() {
			Hd.Reset();
			Body.Reset();
			Rtrn.Reset();
			//Msg.Reset();
		};
		CREPLY_EQUIPMENT_STATE_DISPLAY& operator= (const CREPLY_EQUIPMENT_STATE_DISPLAY& ref) {
			Hd.Reset();
			Hd.operator=(ref.Hd);
			Body.Reset();
			Body.operator=(ref.Body);
			Rtrn.Reset();
			Rtrn.operator=(ref.Rtrn);
			return *this;
		};
		CREPLY_EQUIPMENT_STATE_DISPLAY& operator= (const CDEFINEDATA& ref) {
			Hd.Set_timeStamp(Hd.Get_timeStamp());
			Body.Set_EQUIPMENTID(ref.Get_EQUIPMENTID());
			Body.Set_SUBEQPID(ref.Get_SUBEQPID());
			//Body.Set_IPADDRESS(ref.Get_IPADDRESS());
			//Body.Set_EESMODE(ref.Get_EESMODE());
			//Body.Set_USERID(ref.Get_USERID());
			//Body.Set_ONLINESTATE(ref.Get_ONLINESTATE());
			return *this;
		};
	};
	class CEQUIPMENT_STATE
	{
	public:
		CREPORT_EQUIPMENT_STATE REPORT;
		CREQUEST_EQUIPMENT_STATE_DISPLAY REQUEST;
		CREPLY_EQUIPMENT_STATE_DISPLAY REPLY;
		CEQUIPMENT_STATE() {
			REPORT.Hd.CreateUUID();
		};
		~CEQUIPMENT_STATE() {
		};
		void Reset() {
			REPORT.Reset();
			REQUEST.Reset();
			REPLY.Reset();
		};
		void RequestToReply() {
			REPLY.Hd.Set_transactionId(REQUEST.Hd.Get_transactionId());
			REPLY.Body.Set_EQUIPMENTID(REQUEST.Body.Get_EQUIPMENTID());
			REPLY.Body.Set_SUBEQPID(REQUEST.Body.Get_SUBEQPID());
			REPLY.Body.Set_EQUIPMENTSTATEDISPLAY(REQUEST.Body.Get_EQUIPMENTSTATEDISPLAY());
			REPLY.Body.Set_RGBDISPLAY(REQUEST.Body.Get_RGBDISPLAY());
		};
		CEQUIPMENT_STATE& operator= (const CEQUIPMENT_STATE& ref) {
			REPORT.Reset();
			REPORT.operator=(ref.REPORT);
			REQUEST.Reset();
			REQUEST.operator=(ref.REQUEST);
			REPLY.Reset();
			REPLY.operator=(ref.REPLY);
			return *this;
		};
	};


	class CREPORT_LOSS_STATE_BODY
	{
	protected:
		StdStringA	EQUIPMENTID;
		StdStringA	SUBEQPID;
		StdStringA	IPADDRESS;
		StdStringA	USERID ;
		StdStringA	EESMODE;
		StdStringA	LOTID ;
		StdStringA	RECIPEID ;
		StdStringA	PRODUCTID ;
		StdStringA	PRGVER ;
		StdStringA	EQUIPMENTSTATE ;
		StdStringA	LOSSSTATE;
		StdStringA	LOSSCOUNT;
	public:
		CREPORT_LOSS_STATE_BODY() {
			EQUIPMENTID = "";
			SUBEQPID = "";
			//PORTID= "";		//PORT IDÃß°¡
			IPADDRESS = "";
			USERID = "";
			EESMODE = "";
			LOTID = "";
			RECIPEID = "";
			PRODUCTID = "";
			PRGVER = "";
			EQUIPMENTSTATE = "";
			LOSSSTATE = "";
			LOSSCOUNT = "";
		};
		~CREPORT_LOSS_STATE_BODY() {
		};
		void Reset() {
			EQUIPMENTID.clear();
			SUBEQPID.clear();
			IPADDRESS.clear();
			USERID.clear();
			EESMODE.clear();
			LOTID.clear();
			RECIPEID.clear();
			PRODUCTID.clear();
			PRGVER.clear();
			EQUIPMENTSTATE.clear();
			LOSSSTATE.clear();
			LOSSCOUNT.clear();
		};
		CREPORT_LOSS_STATE_BODY& operator= (const CREPORT_LOSS_STATE_BODY& ref) {
			EQUIPMENTID = (ref.EQUIPMENTID);
			SUBEQPID = (ref.SUBEQPID);
			IPADDRESS = (ref.IPADDRESS);
			USERID = (ref.USERID);
			EESMODE = (ref.EESMODE);
			LOTID = (ref.LOTID);
			RECIPEID = (ref.RECIPEID);
			PRODUCTID = (ref.PRODUCTID);
			PRGVER = (ref.PRGVER);
			EQUIPMENTSTATE = (ref.EQUIPMENTSTATE);
			LOSSSTATE = (ref.LOSSSTATE);
			LOSSCOUNT = (ref.LOSSCOUNT);
			return *this;
		};
		StdStringA		Get_EQUIPMENTID() const {
			return EQUIPMENTID;		};
		StdStringA		Get_SUBEQPID() const {
			return SUBEQPID;		};
		StdStringA		Get_IPADDRESS() const {
			return IPADDRESS;		};
		StdStringA		Get_USERID() const {
			return USERID;		};
		StdStringA		Get_EESMODE() const {
			return EESMODE;		};
		StdStringA		Get_LOTID() const {
			return LOTID;		};
		StdStringA		Get_RECIPEID() const {
			return RECIPEID;		};
		StdStringA		Get_PRODUCTID() const {
			return PRODUCTID;		};
		StdStringA		Get_PRGVER() const {
			return PRGVER;		};
		StdStringA		Get_EQUIPMENTSTATE() const {
			return EQUIPMENTSTATE;		};
		StdStringA		Get_LOSSSTATE() const {
			return LOSSSTATE;		};
		StdStringA		Get_LOSSCOUNT() const {
			return LOSSCOUNT;	};

		void		Set_EQUIPMENTID(__in StdStringA	IN_PARA) {
			EQUIPMENTID = IN_PARA;		};
		void		Set_SUBEQPID(__in StdStringA	IN_PARA) {
			SUBEQPID = IN_PARA;		};
		void		Set_IPADDRESS(__in StdStringA	IN_PARA) {
			IPADDRESS = IN_PARA;		};
		void		Set_USERID(__in StdStringA	IN_PARA) {
			USERID = IN_PARA;		};
		void		Set_EESMODE(__in StdStringA	IN_PARA) {
			EESMODE = IN_PARA;		};
		void		Set_LOTID(__in StdStringA	IN_PARA) {
			LOTID = IN_PARA;		};
		void		Set_RECIPEID(__in StdStringA	IN_PARA) {
			RECIPEID = IN_PARA;		};
		void		Set_PRODUCTID(__in StdStringA	IN_PARA) {
			PRODUCTID = IN_PARA;		};
		void		Set_PRGVER(__in StdStringA	IN_PARA) {
			PRGVER = IN_PARA;		};
		void		Set_EQUIPMENTSTATE(__in StdStringA	IN_PARA) {
			EQUIPMENTSTATE = IN_PARA;		};
		void		Set_LOSSSTATE(__in StdStringA	IN_PARA) {
			LOSSSTATE = IN_PARA;		};
		void		Set_LOSSCOUNT(__in StdStringA	IN_PARA) {
			LOSSCOUNT = IN_PARA;		};
	};
	class CREPORT_LOSS_STATE
	{
	public:
		CHEADER Hd;
		CREPORT_LOSS_STATE_BODY Body;
		//CMSG Msg;
		CREPORT_LOSS_STATE() {
			Hd.Set_messageName("REPORT_LOSS_STATE");
			Hd.Get_timeStamp();
		};
		~CREPORT_LOSS_STATE() {
		};
		void Reset() {
			Hd.Reset();
			Body.Reset();
			//Msg.Reset();
		};
		CREPORT_LOSS_STATE& operator= (const CREPORT_LOSS_STATE& ref) {
			Hd.Reset();
			Hd.operator=(ref.Hd);
			Body.Reset();
			Body.operator=(ref.Body);
			return *this;
		};
		CREPORT_LOSS_STATE& operator= (const CDEFINEDATA& ref) {
			Hd.Set_timeStamp(Hd.Get_timeStamp());
			Body.Set_EQUIPMENTID(ref.Get_EQUIPMENTID());
			Body.Set_SUBEQPID(ref.Get_SUBEQPID());
			Body.Set_IPADDRESS(ref.Get_IPADDRESS());
			Body.Set_EESMODE(ref.Get_EESMODE());
			Body.Set_USERID(ref.Get_USERID());
			//Body.Set_ONLINESTATE(ref.Get_ONLINESTATE());
			return *this;
		};
	};
	class CREQUEST_LOSS_WINDOW_BODY
	{
	protected:
		StdStringA	EQUIPMENTID;
		StdStringA	SUBEQPID;
		StdStringA	IPADDRESS;
		StdStringA	LOSSWINDOW;
	public:
		CREQUEST_LOSS_WINDOW_BODY() {
			EQUIPMENTID = "";
			SUBEQPID = "";
			IPADDRESS = "";
			LOSSWINDOW = "";
		};
		~CREQUEST_LOSS_WINDOW_BODY() {
		};
		void Reset() {
			EQUIPMENTID.clear();
			SUBEQPID.clear();
			IPADDRESS.clear();
			LOSSWINDOW.clear();
		};
		CREQUEST_LOSS_WINDOW_BODY& operator= (const CREQUEST_LOSS_WINDOW_BODY& ref) {
			EQUIPMENTID = (ref.EQUIPMENTID);
			SUBEQPID = (ref.SUBEQPID);
			IPADDRESS = (ref.IPADDRESS);
			LOSSWINDOW = (ref.LOSSWINDOW);
			return *this;
		};
		StdStringA		Get_EQUIPMENTID() const {
			return EQUIPMENTID;
		};
		StdStringA		Get_SUBEQPID() const {
			return SUBEQPID;
		};
		StdStringA		Get_IPADDRESS() const {
			return IPADDRESS;
		};
		StdStringA		Get_LOSSWINDOW() const {
			return LOSSWINDOW;
		};
		void		Set_EQUIPMENTID(__in StdStringA	IN_PARA) {
			EQUIPMENTID = IN_PARA;
		};
		void		Set_SUBEQPID(__in StdStringA	IN_PARA) {
			SUBEQPID = IN_PARA;
		};
		void		Set_IPADDRESS(__in StdStringA	IN_PARA) {
			IPADDRESS = IN_PARA;
		};
		void		Set_LOSSWINDOW(__in StdStringA	IN_PARA) {
			LOSSWINDOW = IN_PARA;
		};
	};
	class CREQUEST_LOSS_WINDOW
	{
	public:
		CHEADER Hd;
		CREQUEST_LOSS_WINDOW_BODY Body;
		CREQUEST_LOSS_WINDOW() {

		};
		~CREQUEST_LOSS_WINDOW() {
		};
		void Reset() {
			Hd.Reset();
			Body.Reset();
		};
		CREQUEST_LOSS_WINDOW& operator= (const CREQUEST_LOSS_WINDOW& ref) {
			Hd.Reset();
			Hd.operator=(ref.Hd);
			Body.Reset();
			Body.operator=(ref.Body);
			return *this;
		};
	};
	class CREPLY_LOSS_WINDOW_BODY
	{
	protected:
		StdStringA	EQUIPMENTID;
		StdStringA	SUBEQPID;
		StdStringA	IPADDRESS;
		StdStringA	LOSSWINDOW;
	public:
		CREPLY_LOSS_WINDOW_BODY() {
			EQUIPMENTID = "";
			SUBEQPID = "";
			IPADDRESS = "";
			LOSSWINDOW = "";
		};
		~CREPLY_LOSS_WINDOW_BODY() {
		};
		void Reset() {
			EQUIPMENTID.clear();
			SUBEQPID.clear();
			IPADDRESS.clear();
			LOSSWINDOW.clear();
		};
		CREPLY_LOSS_WINDOW_BODY& operator= (const CREPLY_LOSS_WINDOW_BODY& ref) {
			EQUIPMENTID = (ref.EQUIPMENTID);
			SUBEQPID = (ref.SUBEQPID);
			IPADDRESS = (ref.IPADDRESS);
			LOSSWINDOW = (ref.LOSSWINDOW);
			return *this;
		};
		StdStringA		Get_EQUIPMENTID() const {
			return EQUIPMENTID;
		};
		StdStringA		Get_SUBEQPID() const {
			return SUBEQPID;
		};
		StdStringA		Get_IPADDRESS() const {
			return IPADDRESS;
		};
		StdStringA		Get_LOSSWINDOW() const {
			return LOSSWINDOW;
		};
		void		Set_EQUIPMENTID(__in StdStringA	IN_PARA) {
			EQUIPMENTID = IN_PARA;
		};
		void		Set_SUBEQPID(__in StdStringA	IN_PARA) {
			SUBEQPID = IN_PARA;
		};
		void		Set_IPADDRESS(__in StdStringA	IN_PARA) {
			IPADDRESS = IN_PARA;
		};
		void		Set_LOSSWINDOW(__in StdStringA	IN_PARA) {
			LOSSWINDOW = IN_PARA;
		};
	};
	class CREPLY_LOSS_WINDOW
	{
	public:
		CHEADER Hd;
		CREPLY_LOSS_WINDOW_BODY Body;
		CRETURN Rtrn;
		//CMSG Msg;
		CREPLY_LOSS_WINDOW() {
			Hd.Set_messageName("REPLY_LOSS_WINDOW");
			Hd.Get_timeStamp();
		};
		~CREPLY_LOSS_WINDOW() {
		};
		void Reset() {
			Hd.Reset();
			Body.Reset();
			Rtrn.Reset();
			//Msg.Reset();
		};
		CREPLY_LOSS_WINDOW& operator= (const CREPLY_LOSS_WINDOW& ref) {
			Hd.Reset();
			Hd.operator=(ref.Hd);
			Body.Reset();
			Body.operator=(ref.Body);
			Rtrn.Reset();
			Rtrn.operator=(ref.Rtrn);
			return *this;
		};
		CREPLY_LOSS_WINDOW& operator= (const CDEFINEDATA& ref) {
			Hd.Set_timeStamp(Hd.Get_timeStamp());
			Body.Set_EQUIPMENTID(ref.Get_EQUIPMENTID());
			Body.Set_SUBEQPID(ref.Get_SUBEQPID());
			Body.Set_IPADDRESS(ref.Get_IPADDRESS());
			//Body.Set_EESMODE(ref.Get_EESMODE());
			//Body.Set_USERID(ref.Get_USERID());
			//Body.Set_ONLINESTATE(ref.Get_ONLINESTATE());
			return *this;
		};
	};
	class CLOSS_WINDOW
	{
	public:
		CREPORT_LOSS_STATE REPORT;
		CREQUEST_LOSS_WINDOW REQUEST;
		CREPLY_LOSS_WINDOW REPLY;
		CLOSS_WINDOW() {
			REPORT.Hd.CreateUUID();
		};
		~CLOSS_WINDOW() {
		};
		void Reset() {
			REPORT.Reset();
			REQUEST.Reset();
			REPLY.Reset();
		};
		void RequestToReply() {
			REPLY.Hd.Set_transactionId(REQUEST.Hd.Get_transactionId());
			REPLY.Body.Set_EQUIPMENTID(REQUEST.Body.Get_EQUIPMENTID());
			REPLY.Body.Set_SUBEQPID(REQUEST.Body.Get_SUBEQPID());
			REPLY.Body.Set_IPADDRESS(REQUEST.Body.Get_IPADDRESS());
			REPLY.Body.Set_LOSSWINDOW(REQUEST.Body.Get_LOSSWINDOW());			
		};
		CLOSS_WINDOW& operator= (const CLOSS_WINDOW& ref) {
			REPORT.Reset();
			REPORT.operator=(ref.REPORT);
			REQUEST.Reset();
			REQUEST.operator=(ref.REQUEST);
			REPLY.Reset();
			REPLY.operator=(ref.REPLY);
			return *this;
		};
	};

	class CREPORT_ALARM_STATE_BODY
	{
	protected:
		StdStringA	EQUIPMENTID;
		StdStringA	SUBEQPID;
		StdStringA	IPADDRESS;
		StdStringA	USERID;
		StdStringA	EESMODE;
		StdStringA	LOTID;
		StdStringA	RECIPEID;
		StdStringA	PRODUCTID;
		StdStringA	PRGVER;
		StdStringA	ALARMCODE;
		StdStringA	ALARMID;
		StdStringA	ALARMMESSAGE;
	public:
		CREPORT_ALARM_STATE_BODY() {
			EQUIPMENTID = "";
			SUBEQPID = "";
			IPADDRESS = "";
			USERID = "";
			EESMODE = "";
			LOTID = "";
			RECIPEID = "";
			PRODUCTID = "";
			PRGVER = "";
			ALARMCODE = "";
			ALARMID = "";
			ALARMMESSAGE = "";
		};
		~CREPORT_ALARM_STATE_BODY() {
		};
		void Reset() {
			EQUIPMENTID.clear();
			SUBEQPID.clear();
			IPADDRESS.clear();
			USERID.clear();
			EESMODE.clear();
			LOTID.clear();
			RECIPEID.clear();
			PRODUCTID.clear();
			PRGVER.clear();
			ALARMCODE.clear();
			ALARMID.clear();
			ALARMMESSAGE.clear();
		};
		CREPORT_ALARM_STATE_BODY& operator= (const CREPORT_ALARM_STATE_BODY& ref) {
			EQUIPMENTID = (ref.EQUIPMENTID);
			SUBEQPID = (ref.SUBEQPID);
			IPADDRESS = (ref.IPADDRESS);
			USERID = (ref.USERID);
			EESMODE = (ref.EESMODE);
			LOTID = (ref.LOTID);
			RECIPEID = (ref.RECIPEID);
			PRODUCTID = (ref.PRODUCTID);
			PRGVER = (ref.PRGVER);
			ALARMCODE = (ref.ALARMCODE);
			ALARMID = (ref.ALARMID);
			ALARMMESSAGE = (ref.ALARMMESSAGE);
			return *this;
		};
		StdStringA		Get_EQUIPMENTID() const {
			return EQUIPMENTID;		};
		StdStringA		Get_SUBEQPID() const {
			return SUBEQPID;		};
		StdStringA		Get_IPADDRESS() const {
			return IPADDRESS;		};
		StdStringA		Get_USERID() const {
			return USERID;		};
		StdStringA		Get_EESMODE() const {
			return EESMODE;		};
		StdStringA		Get_LOTID() const {
			return LOTID;		};
		StdStringA		Get_RECIPEID() const {
			return RECIPEID;		};
		StdStringA		Get_PRODUCTID() const {
			return PRODUCTID;		};
		StdStringA		Get_PRGVER() const {
			return PRGVER;		};
		StdStringA		Get_ALARMCODE() const {
			return ALARMCODE;		};
		StdStringA		Get_ALARMID() const {
			return ALARMID;		};
		StdStringA		Get_ALARMMESSAGE() const {
			return ALARMMESSAGE;		};

		void		Set_EQUIPMENTID(__in StdStringA	IN_PARA) {
			EQUIPMENTID = IN_PARA;		};
		void		Set_SUBEQPID(__in StdStringA	IN_PARA) {
			SUBEQPID = IN_PARA;		};
		void		Set_IPADDRESS(__in StdStringA	IN_PARA) {
			IPADDRESS = IN_PARA;		};
		void		Set_USERID(__in StdStringA	IN_PARA) {
			USERID = IN_PARA;		};
		void		Set_EESMODE(__in StdStringA	IN_PARA) {
			EESMODE = IN_PARA;		};
		void		Set_LOTID(__in StdStringA	IN_PARA) {
			LOTID = IN_PARA;		};
		void		Set_RECIPEID(__in StdStringA	IN_PARA) {
			RECIPEID = IN_PARA;		};
		void		Set_PRODUCTID(__in StdStringA	IN_PARA) {
			PRODUCTID = IN_PARA;		};
		void		Set_PRGVER(__in StdStringA	IN_PARA) {
			PRGVER = IN_PARA;		};
		void		Set_ALARMCODE(__in StdStringA	IN_PARA) {
			ALARMCODE = IN_PARA;		};
		void		Set_ALARMID(__in StdStringA	IN_PARA) {
			ALARMID = IN_PARA;		};
		void		Set_ALARMMESSAGE(__in StdStringA	IN_PARA) {
			ALARMMESSAGE = IN_PARA;		};
	};
	class CREPORT_ALARM_STATE
	{
	public:
		CHEADER Hd;
		CREPORT_ALARM_STATE_BODY Body;
		CREPORT_ALARM_STATE() {
			Hd.Set_messageName("REPORT_ALARM_STATE");
			Hd.Get_timeStamp();
		};
		~CREPORT_ALARM_STATE() {
		};
		void Reset() {
			Hd.Reset();
			Body.Reset();
		};
		CREPORT_ALARM_STATE& operator= (const CREPORT_ALARM_STATE& ref) {
			Hd.Reset();
			Hd.operator=(ref.Hd);
			Body.Reset();
			Body.operator=(ref.Body);
			return *this;
		};
		CREPORT_ALARM_STATE& operator= (const CDEFINEDATA& ref) {
			Hd.Set_timeStamp(Hd.Get_timeStamp());
			Body.Set_EQUIPMENTID(ref.Get_EQUIPMENTID());
			Body.Set_SUBEQPID(ref.Get_SUBEQPID());
			Body.Set_IPADDRESS(ref.Get_IPADDRESS());
			Body.Set_EESMODE(ref.Get_EESMODE());
			Body.Set_USERID(ref.Get_USERID());
			//Body.Set_ONLINESTATE(ref.Get_ONLINESTATE());
			return *this;
		};
	};
	class CALARM_STATE
	{
	public:
		CREPORT_ALARM_STATE REPORT;
		CALARM_STATE() {
			REPORT.Hd.CreateUUID();
		};
		~CALARM_STATE() {
		};
		void Reset() {
			REPORT.Reset();
			//REQUEST.Reset();
			//REPLY.Reset();
		};
		CALARM_STATE& operator= (const CALARM_STATE& ref) {
			REPORT.Reset();
			REPORT.operator=(ref.REPORT);
			//REQUEST.Reset();
			//REQUEST.operator=(ref.REQUEST);
			//REPLY.Reset();
			//REPLY.operator=(ref.REPLY);
			return *this;
		};
	};

	class CREQUEST_ALARM_LIST_BODY
	{
	protected:
		StdStringA	EQUIPMENTID;
		StdStringA	SUBEQPID;
		StdStringA	IPADDRESS;
	public:
		CREQUEST_ALARM_LIST_BODY() {
			EQUIPMENTID = "";
			SUBEQPID = "";
			IPADDRESS = "";
		};
		~CREQUEST_ALARM_LIST_BODY() {
		};
		void Reset() {
			EQUIPMENTID.clear();
			SUBEQPID.clear();
			IPADDRESS.clear();
		};
		CREQUEST_ALARM_LIST_BODY& operator= (const CREQUEST_ALARM_LIST_BODY& ref) {
			EQUIPMENTID = (ref.EQUIPMENTID);
			SUBEQPID = (ref.SUBEQPID);
			IPADDRESS = (ref.IPADDRESS);
			return *this;
		};
		StdStringA		Get_EQUIPMENTID() const {
			return EQUIPMENTID;
		};
		StdStringA		Get_SUBEQPID() const {
			return SUBEQPID;
		};
		StdStringA		Get_IPADDRESS() const {
			return IPADDRESS;
		};
		void		Set_EQUIPMENTID(__in StdStringA	IN_PARA) {
			EQUIPMENTID = IN_PARA;
		};
		void		Set_SUBEQPID(__in StdStringA	IN_PARA) {
			SUBEQPID = IN_PARA;
		};
		void		Set_IPADDRESS(__in StdStringA	IN_PARA) {
			IPADDRESS = IN_PARA;
		};
	};
	class CREQUEST_ALARM_LIST
	{
	public:
		CHEADER Hd;
		CREQUEST_ALARM_LIST_BODY Body;
		CREQUEST_ALARM_LIST() {

		};
		~CREQUEST_ALARM_LIST() {
		};
		void Reset() {
			Hd.Reset();
			Body.Reset();
		};
		CREQUEST_ALARM_LIST& operator= (const CREQUEST_ALARM_LIST& ref) {
			Hd.Reset();
			Hd.operator=(ref.Hd);
			Body.Reset();
			Body.operator=(ref.Body);
			return *this;
		};
	};
	class CREPLY_ALARM_LIST_BODY
	{
	protected:
		StdStringA	EQUIPMENTID;
		StdStringA	SUBEQPID;
		StdStringA	IPADDRESS;
		CALARM_DataListCntr 	ALARMLIST;
	public:
		CREPLY_ALARM_LIST_BODY() {
			EQUIPMENTID = "";
			SUBEQPID = "";
			IPADDRESS = "";
			ALARMLIST = {};
		};
		~CREPLY_ALARM_LIST_BODY() {
		};
		void Reset() {
			EQUIPMENTID.clear();
			SUBEQPID.clear();
			IPADDRESS.clear();
			ALARMLIST.RemoveAll();
		};
		CREPLY_ALARM_LIST_BODY& operator= (const CREPLY_ALARM_LIST_BODY& ref) {
			EQUIPMENTID = (ref.EQUIPMENTID);
			SUBEQPID = (ref.SUBEQPID);
			IPADDRESS = (ref.IPADDRESS);
			ALARMLIST = (ref.ALARMLIST);
			return *this;
		};
		StdStringA		Get_EQUIPMENTID() const {
			return EQUIPMENTID;
		};
		StdStringA		Get_SUBEQPID() const {
			return SUBEQPID;
		};
		StdStringA		Get_IPADDRESS() const {
			return IPADDRESS;
		};
		CALARM_DataListCntr 	Get_ALARMLIST() const {
			return ALARMLIST;
		};

		void		Set_EQUIPMENTID(__in StdStringA	IN_PARA) {
			EQUIPMENTID = IN_PARA;
		};
		void		Set_SUBEQPID(__in StdStringA	IN_PARA) {
			SUBEQPID = IN_PARA;
		};
		void		Set_IPADDRESS(__in StdStringA	IN_PARA) {
			IPADDRESS = IN_PARA;
		};
		void		Set_ALARMLIST(__in CALARM_DataListCntr 	IN_PARA) {
			ALARMLIST = IN_PARA;
		};

	};
	class CREPLY_ALARM_LIST
	{
	public:
		CHEADER Hd;
		CREPLY_ALARM_LIST_BODY Body;
		CRETURN Rtrn;
		CREPLY_ALARM_LIST() {
			Hd.Set_messageName("REPLY_ALARM_LIST");
			Hd.Get_timeStamp();
		};
		~CREPLY_ALARM_LIST() {
		};
		void Reset() {
			Hd.Reset();
			Body.Reset();
			Rtrn.Reset();
			//Msg.Reset();
		};
		CREPLY_ALARM_LIST& operator= (const CREPLY_ALARM_LIST& ref) {
			Hd.Reset();
			Hd.operator=(ref.Hd);
			Body.Reset();
			Body.operator=(ref.Body);
			Rtrn.Reset();
			Rtrn.operator=(ref.Rtrn);
			return *this;
		};
		CREPLY_ALARM_LIST& operator= (const CDEFINEDATA& ref) {
			Hd.Set_timeStamp(Hd.Get_timeStamp());
			Body.Set_EQUIPMENTID(ref.Get_EQUIPMENTID());
			Body.Set_SUBEQPID(ref.Get_SUBEQPID());
			Body.Set_IPADDRESS(ref.Get_IPADDRESS());
			//Body.Set_EESMODE(ref.Get_EESMODE());
			//Body.Set_USERID(ref.Get_USERID());
			//Body.Set_ONLINESTATE(ref.Get_ONLINESTATE());
			return *this;
		};
	};
	class CALARM_LIST
	{
	public:
		//CREPORT_ALARM_STATE REPORT;
		CREQUEST_ALARM_LIST REQUEST;
		CREPLY_ALARM_LIST REPLY;
		CALARM_LIST() {
		};
		~CALARM_LIST() {
		};
		void Reset() {
			//REPORT.Reset();
			REQUEST.Reset();
			REPLY.Reset();
		};
		void RequestToReply() {
			REPLY.Hd.Set_transactionId(REQUEST.Hd.Get_transactionId());
			REPLY.Body.Set_EQUIPMENTID(REQUEST.Body.Get_EQUIPMENTID());
			REPLY.Body.Set_SUBEQPID(REQUEST.Body.Get_SUBEQPID());
			REPLY.Body.Set_IPADDRESS(REQUEST.Body.Get_IPADDRESS());
		};
		CALARM_LIST& operator= (const CALARM_LIST& ref) {
			//REPORT.Reset();
			//REPORT.operator=(ref.REPORT);
			REQUEST.Reset();
			REQUEST.operator=(ref.REQUEST);
			REPLY.Reset();
			REPLY.operator=(ref.REPLY);
			return *this;
		};
	};


	class CREPORT_RMS_MODE_BODY
	{
	protected:
		StdStringA	EQUIPMENTID;
		StdStringA	SUBEQPID;
		StdStringA	IPADDRESS;
		StdStringA	USERID;
		StdStringA	EESMODE;
		StdStringA	LOTID;
		StdStringA	RECIPEID;
		StdStringA	PRODUCTID;
		StdStringA	PRGVER;
	public:
		CREPORT_RMS_MODE_BODY() {
			EQUIPMENTID = "";
			SUBEQPID = "";
			IPADDRESS = "";
			USERID = "";
			EESMODE = "";
			LOTID = "";
			RECIPEID = "";
			PRODUCTID = "";
			PRGVER = "";
		};
		~CREPORT_RMS_MODE_BODY() {
		};
		void Reset() {
			EQUIPMENTID.clear();
			SUBEQPID.clear();
			IPADDRESS.clear();
			USERID.clear();
			EESMODE.clear();
			LOTID.clear();
			RECIPEID.clear();
			PRODUCTID.clear();
			PRGVER.clear();
		};
		CREPORT_RMS_MODE_BODY& operator= (const CREPORT_RMS_MODE_BODY& ref) {
			EQUIPMENTID = (ref.EQUIPMENTID);
			SUBEQPID = (ref.SUBEQPID);
			IPADDRESS = (ref.IPADDRESS);
			USERID = (ref.USERID);
			EESMODE = (ref.EESMODE);
			LOTID = (ref.LOTID);
			RECIPEID = (ref.RECIPEID);
			PRODUCTID = (ref.PRODUCTID);
			PRGVER = (ref.PRGVER);
			return *this;
		};
		StdStringA		Get_EQUIPMENTID() const {
			return EQUIPMENTID;
		};
		StdStringA		Get_SUBEQPID() const {
			return SUBEQPID;
		};
		StdStringA		Get_IPADDRESS() const {
			return IPADDRESS;
		};
		StdStringA		Get_USERID() const {
			return USERID;
		};
		StdStringA		Get_EESMODE() const {
			return EESMODE;
		};
		StdStringA		Get_LOTID() const {
			return LOTID;
		};
		StdStringA		Get_RECIPEID() const {
			return RECIPEID;
		};
		StdStringA		Get_PRODUCTID() const {
			return PRODUCTID;
		};
		StdStringA		Get_PRGVER() const {
			return PRGVER;
		};

		void		Set_EQUIPMENTID(__in StdStringA	IN_PARA) {
			EQUIPMENTID = IN_PARA;
		};
		void		Set_SUBEQPID(__in StdStringA	IN_PARA) {
			SUBEQPID = IN_PARA;
		};
		void		Set_IPADDRESS(__in StdStringA	IN_PARA) {
			IPADDRESS = IN_PARA;
		};
		void		Set_USERID(__in StdStringA	IN_PARA) {
			USERID = IN_PARA;
		};
		void		Set_EESMODE(__in StdStringA	IN_PARA) {
			EESMODE = IN_PARA;
		};
		void		Set_LOTID(__in StdStringA	IN_PARA) {
			LOTID = IN_PARA;
		};
		void		Set_RECIPEID(__in StdStringA	IN_PARA) {
			RECIPEID = IN_PARA;
		};
		void		Set_PRODUCTID(__in StdStringA	IN_PARA) {
			PRODUCTID = IN_PARA;
		};
		void		Set_PRGVER(__in StdStringA	IN_PARA) {
			PRGVER = IN_PARA;
		};
	};
	class CREPORT_RMS_MODE
	{
	public:
		CHEADER Hd;
		CREPORT_RMS_MODE_BODY Body;
		CREPORT_RMS_MODE() {
			Hd.Set_messageName("REPORT_RMS_MODE");
			Hd.Get_timeStamp();
		};
		~CREPORT_RMS_MODE() {
		};
		void Reset() {
			Hd.Reset();
			Body.Reset();
			//Msg.Reset();
		};
		CREPORT_RMS_MODE& operator= (const CREPORT_RMS_MODE& ref) {
			Hd.Reset();
			Hd.operator=(ref.Hd);
			Body.Reset();
			Body.operator=(ref.Body);
			return *this;
		};
		CREPORT_RMS_MODE& operator= (const CDEFINEDATA& ref) {
			Hd.Set_timeStamp(Hd.Get_timeStamp());
			Body.Set_EQUIPMENTID(ref.Get_EQUIPMENTID());
			Body.Set_SUBEQPID(ref.Get_SUBEQPID());
			Body.Set_IPADDRESS(ref.Get_IPADDRESS());
			Body.Set_EESMODE(ref.Get_EESMODE());
			Body.Set_USERID(ref.Get_USERID());
			//Body.Set_ONLINESTATE(ref.Get_ONLINESTATE());
			return *this;
		};
	};
	class CRMS_MODE
	{
	public:
		CREPORT_RMS_MODE REPORT;
		CRMS_MODE() {
			REPORT.Hd.CreateUUID();
		};
		~CRMS_MODE() {
		};
		void Reset() {
			REPORT.Reset();
			//REQUEST.Reset();
			//REPLY.Reset();
		};
		CRMS_MODE& operator= (const CRMS_MODE& ref) {
			REPORT.Reset();
			REPORT.operator=(ref.REPORT);
			//REQUEST.Reset();
			//REQUEST.operator=(ref.REQUEST);
			//REPLY.Reset();
			//REPLY.operator=(ref.REPLY);
			return *this;
		};
	};

	class CREQUEST_SET_DATETIME_BODY
	{
	protected:
		StdStringA	EQUIPMENTID;
		StdStringA	IPADDRESS;
		StdStringA	DATETIME;
	public:
		CREQUEST_SET_DATETIME_BODY() {
			EQUIPMENTID = "";
			IPADDRESS = "";
			DATETIME = "";
		};
		~CREQUEST_SET_DATETIME_BODY() {
		};
		void Reset() {
			EQUIPMENTID.clear();
			IPADDRESS.clear();
			DATETIME.clear();
		};
		CREQUEST_SET_DATETIME_BODY& operator= (const CREQUEST_SET_DATETIME_BODY& ref) {
			EQUIPMENTID = (ref.EQUIPMENTID);
			IPADDRESS = (ref.IPADDRESS);
			DATETIME = (ref.DATETIME);
			return *this;
		};
		StdStringA		Get_EQUIPMENTID() const {
			return EQUIPMENTID;
		};
		StdStringA		Get_IPADDRESS() const {
			return IPADDRESS;
		};
		StdStringA		Get_DATETIME() const {
			return DATETIME;
		};
		void		Set_EQUIPMENTID(__in StdStringA	IN_PARA) {
			EQUIPMENTID = IN_PARA;
		};
		void		Set_IPADDRESS(__in StdStringA	IN_PARA) {
			IPADDRESS = IN_PARA;
		};
		void		Set_DATETIME(__in StdStringA	IN_PARA) {
			DATETIME = IN_PARA;
		};
	};
	class CREQUEST_SET_DATETIME
	{
	public:
		CHEADER Hd;
		CREQUEST_SET_DATETIME_BODY Body;
		CREQUEST_SET_DATETIME() {

		};
		~CREQUEST_SET_DATETIME() {
		};
		void Reset() {
			Hd.Reset();
			Body.Reset();
		};
		CREQUEST_SET_DATETIME& operator= (const CREQUEST_SET_DATETIME& ref) {
			Hd.Reset();
			Hd.operator=(ref.Hd);
			Body.Reset();
			Body.operator=(ref.Body);
			//Rtrn.Reset();
			//Rtrn.operator=(ref.Rtrn);
			return *this;
		};
	};
	class CREPLY_SET_DATETIME_BODY
	{
	protected:
		StdStringA	EQUIPMENTID;
		StdStringA	IPADDRESS;
		StdStringA	DATETIME;
	public:
		CREPLY_SET_DATETIME_BODY() {
			EQUIPMENTID = "";
			IPADDRESS = "";
			DATETIME = "";
		};
		~CREPLY_SET_DATETIME_BODY() {
		};
		void Reset() {
			EQUIPMENTID.clear();
			IPADDRESS.clear();
			DATETIME.clear();
		};
		CREPLY_SET_DATETIME_BODY& operator= (const CREPLY_SET_DATETIME_BODY& ref) {
			EQUIPMENTID = (ref.EQUIPMENTID);
			IPADDRESS = (ref.IPADDRESS);
			DATETIME = (ref.DATETIME);
			return *this;
		};
		StdStringA		Get_EQUIPMENTID() const {
			return EQUIPMENTID;
		};
		StdStringA		Get_IPADDRESS() const {
			return IPADDRESS;
		};
		StdStringA		Get_DATETIME() const {
			return DATETIME;
		};
		void		Set_EQUIPMENTID(__in StdStringA	IN_PARA) {
			EQUIPMENTID = IN_PARA;
		};
		void		Set_IPADDRESS(__in StdStringA	IN_PARA) {
			IPADDRESS = IN_PARA;
		};
		void		Set_DATETIME(__in StdStringA	IN_PARA) {
			DATETIME = IN_PARA;
		};
	};
	class CREPLY_SET_DATETIME
	{
	public:
		CHEADER Hd;
		CREPLY_SET_DATETIME_BODY Body;
		CRETURN Rtrn;
		//CMSG Msg;
		CREPLY_SET_DATETIME() {
			Hd.Set_messageName("REPLY_SET_DATETIME");
			Hd.Get_timeStamp();
		};
		~CREPLY_SET_DATETIME() {
		};
		void Reset() {
			Hd.Reset();
			Body.Reset();
			Rtrn.Reset();
			//Msg.Reset();
		};
		CREPLY_SET_DATETIME& operator= (const CREPLY_SET_DATETIME& ref) {
			Hd.Reset();
			Hd.operator=(ref.Hd);
			Body.Reset();
			Body.operator=(ref.Body);
			Rtrn.Reset();
			Rtrn.operator=(ref.Rtrn);
			return *this;
		};
		CREPLY_SET_DATETIME& operator= (const CDEFINEDATA& ref) {
			Hd.Set_timeStamp(Hd.Get_timeStamp());
			Body.Set_EQUIPMENTID(ref.Get_EQUIPMENTID());
			//Body.Set_SUBEQPID(ref.Get_SUBEQPID());
			Body.Set_IPADDRESS(ref.Get_IPADDRESS());
			//Body.Set_EESMODE(ref.Get_EESMODE());
			//Body.Set_USERID(ref.Get_USERID());
			//Body.Set_ONLINESTATE(ref.Get_ONLINESTATE());
			return *this;
		};
	};
	class CSET_DATETIME
	{
	public:
		CREQUEST_SET_DATETIME REQUEST;
		CREPLY_SET_DATETIME REPLY;
		CSET_DATETIME() {
		};
		~CSET_DATETIME() {
		};
		void Reset() {
			REQUEST.Reset();
			REPLY.Reset();
		};
		void RequestToReply() {
			REPLY.Hd.Set_transactionId(REQUEST.Hd.Get_transactionId());
			REPLY.Body.Set_EQUIPMENTID(REQUEST.Body.Get_EQUIPMENTID());
			REPLY.Body.Set_IPADDRESS(REQUEST.Body.Get_IPADDRESS());
			REPLY.Body.Set_DATETIME(REPLY.Hd.Get_timeStamp());
		};
		bool GetNtPrivilege() {
			HANDLE h_token;
			TOKEN_PRIVILEGES privilege_info;
			if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &h_token))
			{
				return FALSE;
			}

			LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &privilege_info.Privileges[0].Luid);
			privilege_info.PrivilegeCount = 1;
			privilege_info.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

			AdjustTokenPrivileges(h_token, FALSE, &privilege_info, 0, (PTOKEN_PRIVILEGES)NULL, 0);
			if (GetLastError() != ERROR_SUCCESS)
			{
				return FALSE;
			}

			return TRUE;

		};
		CSET_DATETIME& operator= (const CSET_DATETIME& ref) {
			REQUEST.Reset();
			REQUEST.operator=(ref.REQUEST);
			REPLY.Reset();
			REPLY.operator=(ref.REPLY);
			return *this;
		};
	};

	class CREQUEST_TERMINAL_MESSAGE_BODY
	{
	protected:
		StdStringA	EQUIPMENTID;
		StdStringA	SUBEQPID;
		StdStringA	IPADDRESS;
		StdStringA	TERMINALMESSAGE;
	public:
		CREQUEST_TERMINAL_MESSAGE_BODY() {
			EQUIPMENTID = "";
			SUBEQPID = "";
			IPADDRESS = "";
			TERMINALMESSAGE = "";
		};
		~CREQUEST_TERMINAL_MESSAGE_BODY() {
		};
		void Reset() {
			EQUIPMENTID.clear();
			SUBEQPID.clear();
			IPADDRESS.clear();
			TERMINALMESSAGE.clear();
		};
		CREQUEST_TERMINAL_MESSAGE_BODY& operator= (const CREQUEST_TERMINAL_MESSAGE_BODY& ref) {
			EQUIPMENTID = (ref.EQUIPMENTID);
			SUBEQPID = (ref.SUBEQPID);
			IPADDRESS = (ref.IPADDRESS);
			TERMINALMESSAGE = (ref.TERMINALMESSAGE);
			return *this;
		};
		StdStringA		Get_EQUIPMENTID() const {
			return EQUIPMENTID;
		};
		StdStringA		Get_SUBEQPID() const {
			return SUBEQPID;
		};
		StdStringA		Get_IPADDRESS() const {
			return IPADDRESS;
		};
		StdStringA		Get_TERMINALMESSAGE() const {
			return TERMINALMESSAGE;
		};
		void		Set_EQUIPMENTID(__in StdStringA	IN_PARA) {
			EQUIPMENTID = IN_PARA;
		};
		void		Set_SUBEQPID(__in StdStringA	IN_PARA) {
			SUBEQPID = IN_PARA;
		};
		void		Set_IPADDRESS(__in StdStringA	IN_PARA) {
			IPADDRESS = IN_PARA;
		};
		void		Set_TERMINALMESSAGE(__in StdStringA	IN_PARA) {
			TERMINALMESSAGE = IN_PARA;
		};
	};
	class CREQUEST_TERMINAL_MESSAGE
	{
	public:
		CHEADER Hd;
		CREQUEST_TERMINAL_MESSAGE_BODY Body;
		//CRETURN Rtrn;
		//CMSG Msg;
		CREQUEST_TERMINAL_MESSAGE() {

		};
		~CREQUEST_TERMINAL_MESSAGE() {
		};
		void Reset() {
			Hd.Reset();
			Body.Reset();
			//Rtrn.Reset();
			//Msg.Reset();
		};
		CREQUEST_TERMINAL_MESSAGE& operator= (const CREQUEST_TERMINAL_MESSAGE& ref) {
			Hd.Reset();
			Hd.operator=(ref.Hd);
			Body.Reset();
			Body.operator=(ref.Body);
			//Rtrn.Reset();
			//Rtrn.operator=(ref.Rtrn);
			return *this;
		};
	};
	class CREPLY_TERMINAL_MESSAGE_BODY
	{
	protected:
		StdStringA	EQUIPMENTID;
		StdStringA	SUBEQPID;
		StdStringA	IPADDRESS;
		StdStringA	TERMINALMESSAGE;
	public:
		CREPLY_TERMINAL_MESSAGE_BODY() {
			EQUIPMENTID = "";
			SUBEQPID = "";
			IPADDRESS = "";
			TERMINALMESSAGE = "";
		};
		~CREPLY_TERMINAL_MESSAGE_BODY() {
		};
		void Reset() {
			EQUIPMENTID.clear();
			SUBEQPID.clear();
			IPADDRESS.clear();
			TERMINALMESSAGE.clear();
		};
		CREPLY_TERMINAL_MESSAGE_BODY& operator= (const CREPLY_TERMINAL_MESSAGE_BODY& ref) {
			EQUIPMENTID = (ref.EQUIPMENTID);
			SUBEQPID = (ref.SUBEQPID);
			IPADDRESS = (ref.IPADDRESS);
			TERMINALMESSAGE = (ref.TERMINALMESSAGE);
			return *this;
		};
		StdStringA		Get_EQUIPMENTID() const {
			return EQUIPMENTID;
		};
		StdStringA		Get_SUBEQPID() const {
			return SUBEQPID;
		};
		StdStringA		Get_IPADDRESS() const {
			return IPADDRESS;
		};
		StdStringA		Get_TERMINALMESSAGE() const {
			return TERMINALMESSAGE;
		};
		void		Set_EQUIPMENTID(__in StdStringA	IN_PARA) {
			EQUIPMENTID = IN_PARA;
		};
		void		Set_SUBEQPID(__in StdStringA	IN_PARA) {
			SUBEQPID = IN_PARA;
		};
		void		Set_IPADDRESS(__in StdStringA	IN_PARA) {
			IPADDRESS = IN_PARA;
		};
		void		Set_TERMINALMESSAGE(__in StdStringA	IN_PARA) {
			TERMINALMESSAGE = IN_PARA;
		};
	};
	class CREPLY_TERMINAL_MESSAGE
	{
	public:
		CHEADER Hd;
		CREPLY_TERMINAL_MESSAGE_BODY Body;
		CRETURN Rtrn;
		CREPLY_TERMINAL_MESSAGE() {
			Hd.Set_messageName("REPLY_TERMINAL_MESSAGE");
			Hd.Get_timeStamp();
		};
		~CREPLY_TERMINAL_MESSAGE() {
		};
		void Reset() {
			Hd.Reset();
			Body.Reset();
			Rtrn.Reset();
			//Msg.Reset();
		};
		CREPLY_TERMINAL_MESSAGE& operator= (const CREPLY_TERMINAL_MESSAGE& ref) {
			Hd.Reset();
			Hd.operator=(ref.Hd);
			Body.Reset();
			Body.operator=(ref.Body);
			Rtrn.Reset();
			Rtrn.operator=(ref.Rtrn);
			return *this;
		};
		CREPLY_TERMINAL_MESSAGE& operator= (const CDEFINEDATA& ref) {
			Hd.Set_timeStamp(Hd.Get_timeStamp());
			Body.Set_EQUIPMENTID(ref.Get_EQUIPMENTID());
			Body.Set_SUBEQPID(ref.Get_SUBEQPID());
			Body.Set_IPADDRESS(ref.Get_IPADDRESS());
			//Body.Set_EESMODE(ref.Get_EESMODE());
			//Body.Set_USERID(ref.Get_USERID());
			//Body.Set_ONLINESTATE(ref.Get_ONLINESTATE());
			return *this;
		};
	};
	class CTERMINAL_MESSAGE
	{
	public:
		CREQUEST_TERMINAL_MESSAGE REQUEST;
		CREPLY_TERMINAL_MESSAGE REPLY;
		CTERMINAL_MESSAGE() {
		};
		~CTERMINAL_MESSAGE() {
		};
		void Reset() {
			REQUEST.Reset();
			REPLY.Reset();
		};
		void RequestToReply() {
			REPLY.Hd.Set_transactionId(REQUEST.Hd.Get_transactionId());
			REPLY.Body.Set_EQUIPMENTID(REQUEST.Body.Get_EQUIPMENTID());
			REPLY.Body.Set_SUBEQPID(REQUEST.Body.Get_SUBEQPID());
			REPLY.Body.Set_IPADDRESS(REQUEST.Body.Get_IPADDRESS());

			REPLY.Body.Set_TERMINALMESSAGE(REQUEST.Body.Get_TERMINALMESSAGE());
		};
		CTERMINAL_MESSAGE& operator= (const CTERMINAL_MESSAGE& ref) {
			REQUEST.Reset();
			REQUEST.operator=(ref.REQUEST);
			REPLY.Reset();
			REPLY.operator=(ref.REPLY);
			return *this;
		};
	};

	class CREQUEST_OPCALL_BODY
	{
	protected:
		StdStringA	EQUIPMENTID;
		StdStringA	SUBEQPID;
		StdStringA	IPADDRESS;
		StdStringA	MESSAGE;
		StdStringA	BUZZER;
		StdStringA	TOWERLAMP;
	public:
		CREQUEST_OPCALL_BODY() {
			EQUIPMENTID = "";
			SUBEQPID = "";
			IPADDRESS = "";
			MESSAGE = "";
			BUZZER = "";
			TOWERLAMP = "";
		};
		~CREQUEST_OPCALL_BODY() {
		};
		void Reset() {
			EQUIPMENTID.clear();
			SUBEQPID.clear();
			IPADDRESS.clear();
			MESSAGE.clear();
			BUZZER.clear();
			TOWERLAMP.clear();
		};
		CREQUEST_OPCALL_BODY& operator= (const CREQUEST_OPCALL_BODY& ref) {
			EQUIPMENTID = (ref.EQUIPMENTID);
			SUBEQPID = (ref.SUBEQPID);
			IPADDRESS = (ref.IPADDRESS);
			MESSAGE = (ref.MESSAGE);
			BUZZER = (ref.BUZZER);
			TOWERLAMP = (ref.TOWERLAMP);
			return *this;
		};
		StdStringA		Get_EQUIPMENTID() const {
			return EQUIPMENTID;
		};
		StdStringA		Get_SUBEQPID() const {
			return SUBEQPID;
		};
		StdStringA		Get_IPADDRESS() const {
			return IPADDRESS;
		};
		StdStringA		Get_MESSAGE() const {
			return MESSAGE;
		};
		StdStringA		Get_BUZZER() const {
			return BUZZER;
		};
		StdStringA		Get_TOWERLAMP() const {
			return TOWERLAMP;
		};

		void		Set_EQUIPMENTID(__in StdStringA	IN_PARA) {
			EQUIPMENTID = IN_PARA;
		};
		void		Set_SUBEQPID(__in StdStringA	IN_PARA) {
			SUBEQPID = IN_PARA;
		};
		void		Set_IPADDRESS(__in StdStringA	IN_PARA) {
			IPADDRESS = IN_PARA;
		};
		void		Set_MESSAGE(__in StdStringA	IN_PARA) {
			MESSAGE = IN_PARA;
		};
		void		Set_BUZZER(__in StdStringA	IN_PARA) {
			BUZZER = IN_PARA;
		};
		void		Set_TOWERLAMP(__in StdStringA	IN_PARA) {
			TOWERLAMP = IN_PARA;
		};
	};
	class CREQUEST_OPCALL
	{
	public:
		CHEADER Hd;
		CREQUEST_OPCALL_BODY Body;
		//CRETURN Rtrn;
		//CMSG Msg;
		CREQUEST_OPCALL() {

		};
		~CREQUEST_OPCALL() {
		};
		void Reset() {
			Hd.Reset();
			Body.Reset();
			//Rtrn.Reset();
			//Msg.Reset();
		};
		CREQUEST_OPCALL& operator= (const CREQUEST_OPCALL& ref) {
			Hd.Reset();
			Hd.operator=(ref.Hd);
			Body.Reset();
			Body.operator=(ref.Body);
			//Rtrn.Reset();
			//Rtrn.operator=(ref.Rtrn);
			return *this;
		};
		//CREQUEST_OPCALL& operator= (const ST_xml_Msg& ref) {
		//	Msg.Reset();
		//	Msg.operator=(ref);
		//	return *this;
		//};
	};
	class CREPLY_OPCALL_BODY
	{
	protected:
		StdStringA	EQUIPMENTID;
		StdStringA	SUBEQPID;
		StdStringA	IPADDRESS;
		StdStringA	MESSAGE;
		StdStringA	BUZZER;
		StdStringA	TOWERLAMP;
	public:
		CREPLY_OPCALL_BODY() {
			EQUIPMENTID = "";
			SUBEQPID = "";
			IPADDRESS = "";
			MESSAGE = "";
			BUZZER = "";
			TOWERLAMP = "";
		};
		~CREPLY_OPCALL_BODY() {
		};
		void Reset() {
			EQUIPMENTID.clear();
			SUBEQPID.clear();
			IPADDRESS.clear();
			MESSAGE.clear();
			BUZZER.clear();
			TOWERLAMP.clear();
		};
		CREPLY_OPCALL_BODY& operator= (const CREPLY_OPCALL_BODY& ref) {
			EQUIPMENTID = (ref.EQUIPMENTID);
			SUBEQPID = (ref.SUBEQPID);
			IPADDRESS = (ref.IPADDRESS);
			MESSAGE = (ref.MESSAGE);
			BUZZER = (ref.BUZZER);
			TOWERLAMP = (ref.TOWERLAMP);
			return *this;
		};
		StdStringA		Get_EQUIPMENTID() const {
			return EQUIPMENTID;
		};
		StdStringA		Get_SUBEQPID() const {
			return SUBEQPID;
		};
		StdStringA		Get_IPADDRESS() const {
			return IPADDRESS;
		};
		StdStringA		Get_MESSAGE() const {
			return MESSAGE;
		};
		StdStringA		Get_BUZZER() const {
			return BUZZER;
		};
		StdStringA		Get_TOWERLAMP() const {
			return TOWERLAMP;
		};

		void		Set_EQUIPMENTID(__in StdStringA	IN_PARA) {
			EQUIPMENTID = IN_PARA;
		};
		void		Set_SUBEQPID(__in StdStringA	IN_PARA) {
			SUBEQPID = IN_PARA;
		};
		void		Set_IPADDRESS(__in StdStringA	IN_PARA) {
			IPADDRESS = IN_PARA;
		};
		void		Set_MESSAGE(__in StdStringA	IN_PARA) {
			MESSAGE = IN_PARA;
		};
		void		Set_BUZZER(__in StdStringA	IN_PARA) {
			BUZZER = IN_PARA;
		};
		void		Set_TOWERLAMP(__in StdStringA	IN_PARA) {
			TOWERLAMP = IN_PARA;
		};
	};
	class CREPLY_OPCALL
	{
	public:
		CHEADER Hd;
		CREPLY_OPCALL_BODY Body;
		CRETURN Rtrn;
		//CMSG Msg;
		CREPLY_OPCALL() {
			Hd.Set_messageName("REPLY_OPCALL");
			Hd.Get_timeStamp();
		};
		~CREPLY_OPCALL() {
		};
		void Reset() {
			Hd.Reset();
			Body.Reset();
			Rtrn.Reset();
			//Msg.Reset();
		};
		CREPLY_OPCALL& operator= (const CREPLY_OPCALL& ref) {
			Hd.Reset();
			Hd.operator=(ref.Hd);
			Body.Reset();
			Body.operator=(ref.Body);
			Rtrn.Reset();
			Rtrn.operator=(ref.Rtrn);
			return *this;
		};
		CREPLY_OPCALL& operator= (const CDEFINEDATA& ref) {
			Hd.Set_timeStamp(Hd.Get_timeStamp());
			Body.Set_EQUIPMENTID(ref.Get_EQUIPMENTID());
			Body.Set_SUBEQPID(ref.Get_SUBEQPID());
			Body.Set_IPADDRESS(ref.Get_IPADDRESS());
			//Body.Set_EESMODE(ref.Get_EESMODE());
			//Body.Set_USERID(ref.Get_USERID());
			//Body.Set_ONLINESTATE(ref.Get_ONLINESTATE());
			return *this;
		};
	};
	class COPCALL
	{
	public:
		CREQUEST_OPCALL REQUEST;
		CREPLY_OPCALL REPLY;
		COPCALL() {
		};
		~COPCALL() {
		};
		void Reset() {
			REQUEST.Reset();
			REPLY.Reset();
		};
		void RequestToReply() {
			REPLY.Hd.Set_transactionId(REQUEST.Hd.Get_transactionId());
			REPLY.Body.Set_EQUIPMENTID(REQUEST.Body.Get_EQUIPMENTID());
			REPLY.Body.Set_SUBEQPID(REQUEST.Body.Get_SUBEQPID());
			REPLY.Body.Set_IPADDRESS(REQUEST.Body.Get_IPADDRESS());
			REPLY.Body.Set_MESSAGE(REQUEST.Body.Get_MESSAGE());
			REPLY.Body.Set_BUZZER(REQUEST.Body.Get_BUZZER());
			REPLY.Body.Set_TOWERLAMP(REQUEST.Body.Get_TOWERLAMP());
		};
		COPCALL& operator= (const COPCALL& ref) {
			REQUEST.Reset();
			REQUEST.operator=(ref.REQUEST);
			REPLY.Reset();
			REPLY.operator=(ref.REPLY);
			return *this;
		};
	};
}
