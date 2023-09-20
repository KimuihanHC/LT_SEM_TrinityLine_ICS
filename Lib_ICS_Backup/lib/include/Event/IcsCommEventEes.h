#pragma once

#include "Event/IEventNotifier.h"
#include "Event/ICancellableEventArgs.h"
#include "Comm/IcsRequestTypedArgs.h"
//#include "Comm/IcsCommArgs.h"

namespace lt
{
//#if USE_XML
	//CLINK_TEST
	using Request_Link_Test_Args =
		CIcsRequestTypedArgs<class CREQUEST_LINK_TEST>;
	template<typename TypeSender>
	using Request_Link_Test_EventArgs =
		ICancellableEventArgs<TypeSender, Request_Link_Test_Args>;
	template<typename TypeSender>
	using Request_Link_Test_EventNotifier =
		IEventNotifier<Request_Link_Test_EventArgs<TypeSender>>;
	using Reply_Link_Test_Args =
		CIcsRequestTypedArgs<class CREPLY_LINK_TEST>;
	using Link_Test_Args =
		CIcsRequestTypedArgs<class CLINK_TEST>;

	//CONLINE_STATE
	using Report_Online_State_Args =
		CIcsRequestTypedArgs<class CREPORT_ONLINE_STATE>;
	using Online_State_Args =
		CIcsRequestTypedArgs<class CONLINE_STATE>;

	//CUSER_COMMAND
	using Report_User_Change_Args =
		CIcsRequestTypedArgs<class CREPORT_USER_CHANGE>;
	using Request_User_Cpmmand_Args =
		CIcsRequestTypedArgs<class CREQUEST_USER_COMMAND>;
	template<typename TypeSender>
	using Request_User_Command_EventArgs =
		ICancellableEventArgs<TypeSender, Request_User_Cpmmand_Args>;
	template<typename TypeSender>
	using Request_User_Command_EventNotifier =
		IEventNotifier<Request_User_Command_EventArgs<TypeSender>>;
	using Reply_User_Change_Args =
		CIcsRequestTypedArgs<class CREPLY_USER_COMMAND>;
	using User_Change_Args =
		CIcsRequestTypedArgs<class CUSER_COMMAND>;

	//CEQUIPMENT_STATE
	using Report_Equipment_State_Args =
		CIcsRequestTypedArgs<class CREPORT_EQUIPMENT_STATE>;
	using Request_Equipment_State_Display_Args =
		CIcsRequestTypedArgs<class CREQUEST_EQUIPMENT_STATE_DISPLAY>;
	template<typename TypeSender>
	using Request_Equipment_State_Display_EventArgs =
		ICancellableEventArgs<TypeSender, Request_Equipment_State_Display_Args>;
	template<typename TypeSender>
	using Equipment_State_Display_EventNotifier =
		IEventNotifier<Request_Equipment_State_Display_EventArgs<TypeSender>>;
	using Reply_Equipment_State_Display_Args =
		CIcsRequestTypedArgs<class CREPLY_EQUIPMENT_STATE_DISPLAY>;
	using Equipment_State_Args =
		CIcsRequestTypedArgs<class CEQUIPMENT_STATE>;

	//CLOSS_WINDOW
	using Report_Loss_State_Args =
		CIcsRequestTypedArgs<class CREPORT_LOSS_STATE>;
	using Request_Loss_Window_Args =
		CIcsRequestTypedArgs<class CREQUEST_LOSS_WINDOW>;
	template<typename TypeSender>
	using Request_Loss_Window_EventArgs =
		ICancellableEventArgs<TypeSender, Request_Loss_Window_Args>;
	template<typename TypeSender>
	using Request_Loss_Window_EventNotifier =
		IEventNotifier<Request_Loss_Window_EventArgs<TypeSender>>;
	using Reply_Loss_Window_Args =
		CIcsRequestTypedArgs<class CREPLY_LOSS_WINDOW>;
	using Loss_Window_Args =
		CIcsRequestTypedArgs<class CLOSS_WINDOW>;

	//CALARM_STATE
	using Report_Alarm_State_Args =
		CIcsRequestTypedArgs<class CREPORT_ALARM_STATE>;
	using Alarm_State_Args =
		CIcsRequestTypedArgs<class CALARM_STATE>;

	//CALARM_LIST
	using Request_Alarm_List_Args =
		CIcsRequestTypedArgs<class CREQUEST_ALARM_LIST>;
	template<typename TypeSender>
	using Request_Alarm_List_EventArgs =
		ICancellableEventArgs<TypeSender, Request_Alarm_List_Args>;
	template<typename TypeSender>
	using Request_Alarm_List_EventNotifier =
		IEventNotifier<Request_Alarm_List_EventArgs<TypeSender>>;
	using Reply_Alarm_List_Args =
		CIcsRequestTypedArgs<class CREPLY_ALARM_LIST>;
	using Alarm_List_Args =
		CIcsRequestTypedArgs<class CALARM_LIST>;
	
	//CRMS_MODE
	using Report_Rms_Mode_Args =
		CIcsRequestTypedArgs<class CREPORT_RMS_MODE>;
	using Rms_Mode_Args =
		CIcsRequestTypedArgs<class CRMS_MODE>;

	//CSET_DATETIME
	using Request_Set_DateTime_Args =
		CIcsRequestTypedArgs<class CREQUEST_SET_DATETIME>;
	template<typename TypeSender>
	using Request_Set_DateTime_EventArgs =
		ICancellableEventArgs<TypeSender, Request_Set_DateTime_Args>;
	template<typename TypeSender>
	using Request_Set_DateTime_EventNotifier =
		IEventNotifier<Request_Set_DateTime_EventArgs<TypeSender>>;
	using Reply_Set_DateTime_Args =
		CIcsRequestTypedArgs<class CREPLY_SET_DATETIME>;
	using Set_DateTime_Args =
		CIcsRequestTypedArgs<class CSET_DATETIME>;

	//CTERMINAL_MESSAGE
	using Request_Terminal_Message_Args =
		CIcsRequestTypedArgs<class CREQUEST_TERMINAL_MESSAGE>;
	template<typename TypeSender>
	using Request_Terminal_Message_EventArgs =
		ICancellableEventArgs<TypeSender, Request_Terminal_Message_Args>;
	template<typename TypeSender>
	using Request_Terminal_Message_EventNotifier =
		IEventNotifier<Request_Terminal_Message_EventArgs<TypeSender>>;
	using Reply_Terminal_Message_Args =
		CIcsRequestTypedArgs<class CREPLY_TERMINAL_MESSAGE>;
	using Terminal_Message_Args =
		CIcsRequestTypedArgs<class CTERMINAL_MESSAGE>;

	//COPCALL
	using Request_Opcall_Args =
		CIcsRequestTypedArgs<class CREQUEST_OPCALL>;
	template<typename TypeSender>
	using Request_Opcall_EventArgs =
		ICancellableEventArgs<TypeSender, Request_Opcall_Args>;
	template<typename TypeSender>
	using Request_Opcall_EventNotifier =
		IEventNotifier<Request_Opcall_EventArgs<TypeSender>>;
	using Reply_Opcall_Args =
		CIcsRequestTypedArgs<class CREPLY_OPCALL>;
	using Opcall_Args =
		CIcsRequestTypedArgs<class COPCALL>;
	//////////////////////////////////////////////////////////////////////////////////
//#endif
}
