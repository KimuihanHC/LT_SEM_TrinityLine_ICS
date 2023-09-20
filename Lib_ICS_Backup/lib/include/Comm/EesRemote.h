#pragma once

#include "Comm/IcsCommunicator.h"
#include "Event/IcsCommEvent.h"

namespace lt
{
	template<typename TypeSender, typename TypeArgs>
	class CCancellableEventNotifier;

	class LTICS_LIB_CLASS CEesRemote : public CIcsCommunicator
	{
	public:
		CEesRemote();
		CEesRemote(const CEesRemote &) = delete;
		virtual ~CEesRemote();

		CEesRemote & operator=(const CEesRemote &) = delete;

		//2022.12.06a uhkim
		CLogger & CEesRemote::GetLogger();
		//Send¿Ã»ƒ
		uint32 CommandxmlRequestMassage(const ST_xml_Base & Massage,
			uint64 timeout);
		uint32 CommandxmlReportMassage(const ST_xml_Base & Massage,
			uint64 timeout);
		XmlEventNotifier<CEesRemote> &
			GetXmlEventNotifier();
	private:
		CLogger * m_pLogger = nullptr;
	private:
		ILoggable & GetLogger() const;
	private:
		//////////////////////////////////////////////////////////////////////////////////
		//2022.12.26a uhkim 
		//xml Recive Event
		bool ProcRequestXmlARequest(const DataCntr & dataCntr,
			DataCntr & resultDataCntr);
		//2022.01.04a
		bool ProcResponeXmlARequest(const DataCntr & dataCntr);
		//
		//xml Recive Event
		ConstStringA RaiseXmlAEvent(XmlArgs::Args & args);
		ConstStringT RaiseXmlTEvent(XmlArgs::Args & args);
		ConstStringW RaiseXmlWEvent(XmlArgs::Args & args);
		//
	private:
		//2022.12.26a uhkim
		//xml Recive Event 
		using xmlEventNotifierImpl =
			CCancellableEventNotifier<CEesRemote, XmlArgs>;
		xmlEventNotifierImpl * m_pXmlEventNotifier = nullptr;

		//xmlEventNotifierImpl * m_pXmlReortReqEventNotifier = nullptr;
		//xmlEventNotifierImpl * m_pXmlReortRepEventNotifier = nullptr;

		//////////////////////////////////////////////////////////////////////////////////
		
	};
}
