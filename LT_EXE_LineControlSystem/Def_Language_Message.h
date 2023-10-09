//*****************************************************************************
// Filename	: 	Def_Language_Message.h
// Created	:	2022/7/20 - 18:59
// Modified	:	2022/7/20 - 18:59
//
// Author	:	piring 
//	
// Purpose	:	
//*****************************************************************************

#ifndef Def_LanguageMessage_h__
#define Def_LanguageMessage_h__

#include "Def_Language.h"

#pragma once

// warning C4566: 해결용
//#pragma execution_character_set("utf-8")	//20231003

// Common (File, Apply)

// Line
// Equipment Control
// Socket JIG
// User
// NG Code

typedef enum
{
	MB_Common_ExitProgram,
	MB_Common_SaveFile_Succeed,
	MB_Common_SaveFile_Failed,
	MB_Common_LoadFile_Failed,
	MB_Common_SelectItem,
	MB_Common_Apply,

	MB_Line_NoConfig,
	MB_Line_DataCount_Error,
	MB_Line_Delete_AllEqp,
	MB_Line_EqpID_Overlapped,
	MB_Line_IpAddr_Overlapped,
	MB_Line_SettingOK,
	MB_Line_IpAddr_Invalid,
	MB_Line_IpAddr_Blank,
	MB_Line_Delete_EditData,
	MB_Line_EqpCfg_Error_Fix,
	MB_Line_EqpCfg_Error_Cancel,

	MB_Eqp_Comm_Error,
	MB_Eqp_Enable_Status,
	MB_Eqp_Disable_Status,
	MB_Eqp_Disabled,
	MB_Eqp_NotTester,
	MB_Eqp_SaveYield_CSV,
	MB_Eqp_AllEqp_Control,
	MB_Eqp_SelectEqp_Control,

	MB_Socket_Reload,
	MB_Socket_InsertFailed,
	MB_Socket_Add_Selected,
	MB_Socket_Add_Failed_Batch,
	MB_Socket_Add_Failed_Batch_No,
	MB_Socket_Delete_Selected,
	MB_Socket_Delete_All,
	MB_Socket_NoFound,
	MB_Socket_PointerIsNull,
	MB_Socket_Reset_Yield_Selected,
	MB_Socket_Reset_Yield_All,
	MB_Socket_Reset_Data_Selected,
	MB_Socket_Reset_Data_All,

	MB_User_Password_Incorrect,
	MB_User_ID_Select,
	MB_User_Admin_CantDelete,
	MB_User_LoginUser_CantDelete,
	MB_User_Remove_User,
	MB_User_Authority_Select,
	MB_User_Name_Enter,
	MB_User_Name_Exist,
	MB_User_ID_Enter,
	MB_User_ID_Exist,
	MB_User_Password_Enter,
	MB_User_PasswordConfirm_Enter,
	MB_User_Password_NotMatch,

	MB_Debug_Apply_System,
	MB_Debug_Apply_System_Failed,

	MB_NGCode_Code_Exist,
	MB_NGCode_Delete_Select,

	MB_NGCount_Save_Failed_CSV,
	MB_NGCount_PointerNull,
	MB_NGCount_ResetCount_All,
	MB_NGCount_RefreshCount,
   
	MB_Eqp_Comm_Disconnected,
	MB_MaxCount,
}enLangMessageBox;



static LPCTSTR g_szMessageBox_T[][Lang_MaxCount] =
{
	// 공통
	{ // MB_Common_ExitProgram
		_T("프로그램을 종료 하시겠습니까?"),											// 한국어
		_T("Are you sure you want to exit the program?"),							// 영어
		_T("Are you sure you want to exit the program?"),							// 베트남어
		_T("您确定要退出该程序吗? \r\n(Are you sure you want to exit the program?)"),	// 중국어
	},

	{ // MB_Common_SaveFile_Succeed
		_T("파일 저장에 성공 했습니다."),									// 한국어
		_T("The file has been saved successfully."),					// 영어
		_T("The file has been saved successfully."),					// 베트남어
		_T("文件保存成功 \r\n(The file has been saved successfully)"),	// 중국어
	},

	{ // MB_Common_SaveFile_Failed
		_T("파일 저장에 실패 했습니다!"),						// 한국어
		_T("Failed to save file !!"),						// 영어
		_T("Failed to save file !!"),						// 베트남어
		_T("保存文件失败 !! \r\n(Failed to save file !!)"),	// 중국어
	},

	{ // MB_Common_LoadFile_Failed
		_T("파일 불러오기에 실패 했습니다 !"),					// 한국어
		_T("Failed to load file !!"),						// 영어
		_T("Failed to load file !!"),						// 베트남어
		_T("加载文件失败 !! \r\n(Failed to load file !!)"),	// 중국어
	},

	{ // MB_Common_SelectItem
		_T("항목을 선택하세요."),								// 한국어
		_T("Please select an item."),						// 영어
		_T("Please select an item."),						// 베트남어
		_T("请选择一个项目. \r\n(Please select an item.)"),	// 중국어
	},

	{ // MB_Common_Apply
		_T("변경된 설정을 적용 하시겠습니까?"),					// 한국어
		_T("Apply the changed settings?"),						// 영어
		_T("Apply the changed settings?"),						// 베트남어
		_T("应用更改的设置？ \r\n(Apply the changed settings?)"),	// 중국어
	},

	//==== TestManager_EQP.cpp ====
	{ // MB_Line_NoConfig
		_T("인라인 설정이 되지 않았습니다."),					// 한국어
		_T("No Inline configuration."),						// 영어
		_T("No Inline configuration."),						// 베트남어
		_T("没有内联设置 \r\n(No Inline configuration.)"),	// 중국어
	},

	//==== List_LineConfig.cpp ====
	{ // MB_Line_DataCount_Error
		_T("목록에 설비 추가: 데이터 개수 오류!"),						// 한국어
		_T("CList_LineConfig::Item_Add() Data Count Error!"),		// 영어
		_T("CList_LineConfig::Item_Add() Data Count Error!"),		// 베트남어
		_T("CList_LineConfig::Item_Add() Data Count Error!"),		// 중국어
	},
	
	//==== Wnd_Cfg_Line.cpp ====
	//==== Dlg_ModifyEquipment.cpp ====
	{ // MB_Line_Delete_AllEqp
		_T("리스트의 모든 설비를 삭제 하시겠습니까?"),																// 한국어
		_T("Are you sure you want to delete all equipments in the list?"),										// 영어
		_T("Are you sure you want to delete all equipments in the list?"),										// 베트남어
		_T("您确定要从列表中删除所有设备吗? \r\n(Are you sure you want to delete all equipments in the list?)"),	// 중국어
	},

	{ // MB_Line_EqpID_Overlapped
		_T("중복된 \"설비 ID\"가 있습니다. 확인하세요."),													// 한국어
		_T("There are Overlapped \"Equipment ID\". please check."),										// 영어
		_T("There are Overlapped \"Equipment ID\". please check."),										// 베트남어
		_T("有一个重复的\"设施 ID\". 请检查 \r\n(There are Overlapped \"Equipment ID\". please check.)"),	// 중국어
	},

	{ // MB_Line_IpAddr_Overlapped
		_T("중복된 \"IP 주소\"가 있습니다. 확인하세요."),													// 한국어
		_T("There are Overlapped \"IP Address\". please check"),										// 영어
		_T("There are Overlapped \"IP Address\". please check"),										// 베트남어
		_T("有一个重复的\"IP 地址\". 请检查. \r\n(There are Overlapped \"IP Address\". please check)"),	// 중국어
	},

	{ // MB_Line_SettingOK
		_T("설정이 정상입니다."),						// 한국어
		_T("Settings are OK."),						// 영어
		_T("Settings are OK."),						// 베트남어
		_T("设置没问题. \r\n(Settings are OK.)"),	// 중국어
	},

	{ // MB_Line_IpAddr_Invalid
		_T("IP 주소가 유효하지 않습니다."),					// 한국어
		_T("Not a valid IP Address!"),						// 영어
		_T("Not a valid IP Address!"),						// 베트남어
		_T("IP 地址无效! \r\n(Not a valid IP Address!)"),	// 중국어
	},

	{ // MB_Line_IpAddr_Blank
		_T("IP 주소가 비어있습니다."),						// 한국어
		_T("IP Address is Blank!"),							// 영어
		_T("IP Address is Blank!"),							// 베트남어
		_T("IP 地址为空白! \r\n(IP Address is Blank!)"),		// 중국어
	},

	{ // MB_Line_Delete_EditData
		_T("편집 중인 데이터가 삭제됩니다. \r\n 계속 하시겠습니까?"),															// 한국어
		_T("The data being edited is deleted.\r\n Do you want to continue?"),												// 영어
		_T("The data being edited is deleted.\r\n Do you want to continue?"),												// 베트남어
		_T("正在编辑的数据被删除. \r\n 你确定要继续吗? \r\n(The data being edited is deleted.\r\n Do you want to continue?)"),	// 중국어
	},

	{ // MB_Line_EqpCfg_Error_Fix
		_T("설비 구성에 문제가 있습니다. 수정하세요."),															// 한국어
		_T("There is a problem with the equipment configuration. Please fix."),								// 영어
		_T("There is a problem with the equipment configuration. Please fix."),								// 베트남어
		_T("设施配置有问题. 请改正. \r\n(There is a problem with the equipment configuration. Please fix.)"),	// 중국어
	},

	{ // MB_Line_EqpCfg_Error_Cancel
		_T("설비 구성에 문제가 있습니다. 저장을 취소합니다."),														// 한국어
		_T("There is a problem with the equipment configuration. Cancel save."),								// 영어
		_T("There is a problem with the equipment configuration. Cancel save."),								// 베트남어
		_T("设施配置有问题. 取消保存. \r\n(There is a problem with the equipment configuration. Cancel save.)"),	// 중국어
	},

	//==== View_MainCtrl_ICS.cpp ====
	{ // MB_Eqp_Comm_Error
		_T("설비와의 통신에서 오류가 발생 했습니다."),											// 한국어
		_T("A communication error with the equipment occurred."),							// 영어
		_T("A communication error with the equipment occurred."),							// 베트남어
		_T("与设施的通信发生错误. \r\n(A communication error with the equipment occurred.)"),	// 중국어
	},

	{ // MB_Eqp_Enable_Status
		_T("설비를 \"활성화\" 상태로 변경하시겠습니까?"),																	// 한국어
		_T("Are you sure you want to set the Equipment to \"Enabled\" status?"),										// 영어
		_T("Are you sure you want to set the Equipment to \"Enabled\" status?"),										// 베트남어
		_T("您确定要将设施更改为\"活动\"状态吗? \r\n(Are you sure you want to set the Equipment to \"Enabled\" status?)"),	// 중국어
	},

	{ // MB_Eqp_Disable_Status
		_T("설비를 \"비활성화\" 상태로 변경하시겠습니까?"),																		// 한국어
		_T("Are you sure you want to set the Equipment to \"Disabled\" status?"),											// 영어
		_T("Are you sure you want to set the Equipment to \"Disabled\" status?"),											// 베트남어
		_T("您确定要将设施更改为\"非活动\"状态吗? \r\n(Are you sure you want to set the Equipment to \"Disabled\" status?)"),	// 중국어
	},

	{ // MB_Eqp_Disabled
		_T("비활성화된 설비입니다."),							// 한국어
		_T("Disabled equipment."),							// 영어
		_T("Disabled equipment."),							// 베트남어
		_T("它是一个非活动设备. \r\n(Disabled equipment.)"),	// 중국어
	},

	{ // MB_Eqp_NotTester
		_T("검사 설비가 아닙니다."),										// 한국어
		_T("It is not an inspection equipment."),						// 영어
		_T("It is not an inspection equipment."),						// 베트남어
		_T("它不是检查设施. \r\n(It is not an inspection equipment.)"),	// 중국어
	},

	{ // MB_Eqp_SaveYield_CSV
		_T("수율을 .csv 파일로 저장 하시겠습니까?"),							// 한국어
		_T("Save the yield as a csv file?"),								// 영어
		_T("Save the yield as a csv file?"),								// 베트남어
		_T("将产量另存为 .csv 文件? \r\n(Save the yield as a csv file?)"),		// 중국어
	},

	{ // MB_Eqp_AllEqp_Control
		_T("모든 설비를 \"%s\" 하시겠습니까?"),									// 한국어
		_T("Do you want to \"%s\" all equipment?"),								// 영어
		_T("Do you want to \"%s\" all equipment?"),								// 베트남어
		_T("是否要 \"%s\" 所有设备? \r\n(Do you want to \"%s\" all equipment?)"),	// 중국어
	},

	{ // MB_Eqp_SelectEqp_Control
		_T("선택한 설비를 \"%s\" 하시겠습니까?"),															// 한국어
		_T("Are you sure you want to \"%s\" the selected equipment?"),									// 영어
		_T("Are you sure you want to \"%s\" the selected equipment?"),									// 베트남어
		_T("您确定要 \"%s\" 选定的设备吗? \r\n(Are you sure you want to \"%s\" the selected equipment?)"),	// 중국어
	},

	//==== Wnd_Cfg_Socket.cpp ====
	{ // MB_Socket_Reload
		_T("소켓 정보를 파일에서 다시 읽어 오겠습니까?"),								// 한국어
		_T("Reload socket information from file?"),									// 영어
		_T("Reload socket information from file?"),									// 베트남어
		_T("从文件中读取套接字信息? \r\n(Reload socket information from file?)"),		// 중국어
	},

	{ // MB_Socket_InsertFailed
		_T("추가에 실패 했습니다, RFID가 이미 존재 합니다."),						// 한국어
		_T("Insert failed, RFID already exists."),								// 영어
		_T("Insert failed, RFID already exists."),								// 베트남어
		_T("添加失败，RFID 已存在. \r\n(Insert failed, RFID already exists.)"),	// 중국어
	},

	{ // MB_Socket_Add_Selected
		_T("총 %d 개의 소켓을 추가하시겠습니까?"),												// 한국어
		_T("Do you want to add a total of %d sockets?"),									// 영어
		_T("Do you want to add a total of %d sockets?"),									// 베트남어
		_T("您要添加总共 %d 个套接字吗? \r\n(Do you want to add a total of %d sockets?)"),	// 중국어
	},

	{ // MB_Socket_Add_Failed_Batch
		_T("일부의 소켓 추가를 실패습니다. 일부의 RFID는 이미 존재 합니다."),									// 한국어
		_T("Some Insert failed, Some RFID already exists."),												// 영어
		_T("Some Insert failed, Some RFID already exists."),												// 베트남어
		_T("未能添加一些套接字. 一些 RFID 已经存在. \r\n(Some Insert failed, Some RFID already exists.)"),		// 중국어
	},

	{ // MB_Socket_Add_Failed_Batch_No
		_T("종료 번호가 시작 번호보다 커야 합니다."),													// 한국어
		_T("The end number must be greater than the begin number."),								// 영어
		_T("The end number must be greater than the begin number."),								// 베트남어
		_T("结束编号必须大于开始编号. \r\n(The end number must be greater than the begin number.)"),	// 중국어
	},

	{ // MB_Socket_Delete_Selected
		_T("목록에서 \"%s\" 소켓을 제거 하시겠습니까?"),																// 한국어
		_T("Are you sure you want to delete \"%s\" socket in the list?"),											// 영어
		_T("Are you sure you want to delete \"%s\" socket in the list?"),											// 베트남어
		_T("您确定要从列表中删除套接字\"%s\"吗? \r\n(Are you sure you want to delete \"%s\" socket in the list?)"),	// 중국어
	},

	{ // MB_Socket_Delete_All
		_T("리스트의 모든 소켓을 삭제 하시겠습니까?"),																// 한국어
		_T("Are you sure you want to delete all sockets in the list?"),											// 영어
		_T("Are you sure you want to delete all sockets in the list?"),											// 베트남어
		_T("您确定要删除列表中的所有套接字吗? \r\n(Are you sure you want to delete all sockets in the list?)"),	// 중국어
	},

	//==== Wnd_SocketView.cpp ====
	//==== View_SubMonitoring.cpp ====
	{ // MB_Socket_NoFound
		_T("소켓을 찾지 못했습니다."),					// 한국어
		_T("No socket found."),							// 영어
		_T("No socket found."),							// 베트남어
		_T("未找到套接字. \r\n(No socket found.)"),		// 중국어
	},

	{ // MB_Socket_PointerIsNull
		_T("SocketInfo Pointer is null !!"),		// 한국어
		_T("SocketInfo Pointer is null !!"),		// 영어
		_T("SocketInfo Pointer is null !!"),		// 베트남어
		_T("SocketInfo Pointer is null !!"),		// 중국어
	},

	{ // MB_Socket_Reset_Yield_Selected
		_T("선택한 소켓 지그의 수율을 초기화 하시겠습니까?"),							// 한국어
		_T("Reset yield of selected Socket JIG?"),									// 영어
		_T("Reset yield of selected Socket JIG?"),									// 베트남어
		_T("重置所选\"插座夹具\"的良率? \r\n(Reset yield of selected Socket JIG?)"),	// 중국어
	},

	{ // MB_Socket_Reset_Yield_All
		_T("모든 소켓 지그의 수율을 초기화 하시겠습니까?"),								// 한국어
		_T("Reset the yield of all socket JIG?"),									// 영어
		_T("Reset the yield of all socket JIG?"),									// 베트남어
		_T("重置所有\"插座夹具\"的良率? \r\n(Reset the yield of all socket JIG?)"),	// 중국어
	},

	{ // MB_Socket_Reset_Data_Selected
		_T("선택한 소켓 지그의 데이터를 초기화 하시겠습니까?"),										// 한국어
		_T("Initialize data of selected socket JIG?"),											// 영어
		_T("Initialize data of selected socket JIG?"),											// 베트남어
		_T("是否要重置所选\"插座夹具\"的良品率? \r\n(Initialize data of selected socket JIG?)"),	// 중국어
	},

	{ // MB_Socket_Reset_Data_All
		_T("모든 소켓 지그의 데이터를 초기화 하시겠습니까?"),									// 한국어
		_T("Initialize data of All socket JIG?"),											// 영어
		_T("Initialize data of All socket JIG?"),											// 베트남어
		_T("你想初始化所有\"插座夹具\"的数据吗? \r\n(Initialize data of All socket JIG?)"),	// 중국어
	},

	//==== Dlg_UserLogin.cpp ====
	{ // MB_User_Password_Incorrect
		_T("비밀번호가 다릅니다!"),									// 한국어
		_T("The password is incorrect."),							// 영어
		_T("The password is incorrect."),							// 베트남어
		_T("你的密码不一样! \r\n(The password is incorrect.)"),		// 중국어
	},

	{ // MB_User_ID_Select
		_T("ID를 선택하세요."),							// 한국어
		_T("Please select ID."),						// 영어
		_T("Please select ID."),						// 베트남어
		_T("请选择身份证. \r\n(Please select ID.)"),		// 중국어
	},

	//==== Wnd_Cfg_User.cpp ====
	//==== Dlg_ModifyUser.cpp ====
	{ // MB_User_Admin_CantDelete
		_T("\"Admin\" 사용자는 삭제 할 수 없습니다."),								// 한국어
		_T("\"Admin\" user cannot be deleted."),								// 영어
		_T("\"Admin\" user cannot be deleted."),								// 베트남어
		_T("无法删除用户 \"Admin\" \r\n(\"Admin\" user cannot be deleted.)"),		// 중국어
	},

	{ // MB_User_LoginUser_CantDelete
		_T("현재 로그인 되어있는 사용자는 삭제 할 수 없습니다. \r\n 로그아웃 후 삭제 하세요."),															// 한국어
		_T("The currently logged in user cannot be deleted. \r\n Please log out and delete."),														// 영어
		_T("The currently logged in user cannot be deleted. \r\n Please log out and delete."),														// 베트남어
		_T("当前登录的用户不能被删除. \r\n 请退出并删除. \r\n(The currently logged in user cannot be deleted. \r\n Please log out and delete.)"),		// 중국어
	},

	{ // MB_User_Remove_User
		_T("목록에서 선택한 사용자를 삭제 하시겠습니까?"),														// 한국어
		_T("Are you sure you want to remove a user from the list?"),										// 영어
		_T("Are you sure you want to remove a user from the list?"),										// 베트남어
		_T("您确定要从列表中删除选定的用户吗? \r\n(Are you sure you want to remove a user from the list?)"),	// 중국어
	},
	
	{ // MB_User_Authority_Select
		_T("권한을 선택하세요."),								// 한국어
		_T("Select Authority."),							// 영어
		_T("Select Authority."),							// 베트남어
		_T("您必须选择一个权限. \r\n(Select Authority.)"),	// 중국어
	},

	{ // MB_User_Name_Enter
		_T("이름을 입력하세요."),							// 한국어
		_T("Please enter a Name."),						// 영어
		_T("Please enter a Name."),						// 베트남어
		_T("输入名字 \r\n(Please enter a Name.)"),		// 중국어
	},

	{ // MB_User_Name_Exist
		_T("입력한 이름은 이미 존재 합니다."),										// 한국어
		_T("The name you entered already exists."),								// 영어
		_T("The name you entered already exists."),								// 베트남어
		_T("您输入的名称已经存在 \r\n(The name you entered already exists.)"),	// 중국어
	},

	{ // MB_User_ID_Enter
		_T("\"ID\"를 입력하세요."),							// 한국어
		_T("Please enter a \"ID\"."),						// 영어
		_T("Please enter a \"ID\"."),						// 베트남어
		_T("请输入\"ID\". \r\n(Please enter a \"ID\".)"),	// 중국어
	},

	{ // MB_User_ID_Exist
		_T("입력한 ID는 이미 존재합니다."),									// 한국어
		_T("The ID you entered already exists."),							// 영어
		_T("The ID you entered already exists."),							// 베트남어
		_T("您输入的 ID 已存在. \r\n(The ID you entered already exists.)"),	// 중국어
	},

	{ // MB_User_Password_Enter
		_T("\"비밀번호\"를 입력하세요."),								// 한국어
		_T("Please enter a \"Password\"."),							// 영어
		_T("Please enter a \"Password\"."),							// 베트남어
		_T("请输入\"密码\". \r\n(Please enter a \"Password\".)"),	// 중국어
	},

	{ // MB_User_PasswordConfirm_Enter
		_T("\"비밀번호 확인\"을 입력하세요."),									// 한국어
		_T("Please enter a \"Confirm Password\"."),							// 영어
		_T("Please enter a \"Confirm Password\"."),							// 베트남어
		_T("输入\"确认密码\" \r\n(Please enter a \"Confirm Password\".)"),	// 중국어
	},

	{ // MB_User_Password_NotMatch
		_T("\"비밀번호\"와 \"비밀번호 확인\"이 서로 다릅니다."),												// 한국어
		_T("\"Password\" and \"Confirm Password\" does not match."),										// 영어
		_T("\"Password\" and \"Confirm Password\" does not match."),										// 베트남어
		_T("\"密码\"和\"确认密码\"是不同的. \r\n(\"Password\" and \"Confirm Password\" does not match.)"),	// 중국어
	},

	//==== Wnd_Cfg_Debug.cpp ====
	{ // MB_Debug_Apply_System
		_T("디버그 정보가 시스템에 적용되었습니다."),											// 한국어
		_T("Debug Infomation has been applied to the System"),								// 영어
		_T("Debug Infomation has been applied to the System"),								// 베트남어
		_T("调试信息已应用于系统. \r\n(Debug Infomation has been applied to the System)"),	// 중국어
	},

	{ // MB_Debug_Apply_System_Failed
		_T("시스템 적용에 실패 하였습니다."),								// 한국어
		_T("failed apply to system."),									// 영어
		_T("failed apply to system."),									// 베트남어
		_T("无法将调试信息应用于系统. \r\n(failed apply to system.)"),		// 중국어
	},	

 	
 	//==== Wnd_Cfg_NGCode.cpp ====
	{ // MB_NGCode_Code_Exist
		_T("NG 코드(%d)가 이미 추가되어 있습니다."),						// 한국어
		_T("NG code(%d) already added !"),								// 영어
		_T("NG code(%d) already added !"),								// 베트남어
		_T("NG 代码(%d) 已存在. \r\n(NG code(%d) already added !)"),		// 중국어
	},

	{ // MB_NGCode_Delete_Select
		_T("목록에 있는 \"NG 코드: %d\"를 삭제 하시겠습니까?"),															// 한국어
		_T("Are you sure you want to delete \"NG Code: %d\" in the list?"),												// 영어
		_T("Are you sure you want to delete \"NG Code: %d\" in the list?"),												// 베트남어
		_T("您确定要从列表中删除\"NG 代码：%d\"吗? \r\n(Are you sure you want to delete \"NG Code: %d\" in the list?)"),	// 중국어
	},

 	//==== Wnd_FailInfo.cpp ====
	{ // MB_NGCount_Save_Failed_CSV
		_T(".csv 파일 저장에 실패 했습니다."),						// 한국어
		_T("Failed to save .csv file."),							// 영어
		_T("Failed to save .csv file."),							// 베트남어
		_T("无法保存 .csv 文件. \r\n(Failed to save .csv file.)"),	// 중국어
	},

	{ // MB_NGCount_PointerNull
		_T("FailInfo Pointer is null !!"),		// 한국어
		_T("FailInfo Pointer is null !!"),		// 영어
		_T("FailInfo Pointer is null !!"),		// 베트남어
		_T("FailInfo Pointer is null !!"),		// 중국어
	},

	{ // MB_NGCount_ResetCount_All
		_T("모든 설비의 NG 카운트를 초기화 하시겠습니까?"),									// 한국어
		_T("Reset the NG Count of all Equipment?"),										// 영어
		_T("Reset the NG Count of all Equipment?"),										// 베트남어
		_T("是否要重置所有安装的 NG 计数? \r\n(Reset the NG Count of all Equipment?)"),	// 중국어
	},

	{ // MB_NGCount_RefreshCount
		_T("모든 설비의 NG 카운트를 다시 읽어 들이겠습니까?"),								// 한국어
		_T("Refresh the NG Count of all Equipment?"),									// 영어
		_T("Refresh the NG Count of all Equipment?"),									// 베트남어
		_T("重读所有装备的\"NG 计数\"? \r\n(Refresh the NG Count of all Equipment?)"),	// 중국어
	},


	//==== Wnd_TesterSelection.cpp ====


	//==== TestManager_EQP_ICS.cpp ====
	{ // MB_Eqp_Comm_Disconnected
		_T("Auto 상태의 설비(equipment)와 TCP/IP 통신이 끊겼습니다."),								// 한국어
		_T("TCP/IP communication was lost with the equipment in the Auto state."),				// 영어
		_T("TCP/IP communication was lost with the equipment in the Auto state."),				// 베트남어
		_T("与处于自动状态的设备失去了TCP/IP通信. \r\n(TCP/IP communication was lost with the equipment in the Auto state.)"),	// 중국어
	},



};






#endif // Def_LanguageMessage_h__
