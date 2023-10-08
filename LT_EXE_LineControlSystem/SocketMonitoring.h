//*****************************************************************************
// Filename	: 	SocketMonitoring.h
// Created	:	2021/11/18 - 14:20
// Modified	:	2021/11/18 - 14:20
//
// Author	:	piring
//	
// Purpose	:	 
//*****************************************************************************
#ifndef SocketMonitoring_h__
#define SocketMonitoring_h__


#pragma once

#include <map>
#include "Def_Socket_Info.h"
#include "Def_Yield.h"
#include "RegSocket.h"
#include "Def_DebugInfo.h"


class CLineInfo;
class CRework_Info;

//-----------------------------------------------------------------------------
// CSocketMonitoring
//-----------------------------------------------------------------------------
class CSocketMonitoring
{
public:
	CSocketMonitoring();
	~CSocketMonitoring();

	CSocketMonitoring& operator= (const CSocketList& ref);
	CSocketMonitoring& operator= (const CSocketMonitoring& ref);

//protected:

	// ?åÏºì ?ïÎ≥¥ (key: RFID)//
#ifdef _UNICODE
	std::map<std::wstring, CSocketInfo_Unit> m_Sockets;
#else
	std::map<std::string, CSocketInfo_Unit> m_Sockets;
#endif

protected:
	// ?àÏ??§Ìä∏Î¶¨Ïóê ?∞Ïù¥???Ä?•Ïö©??/
	CRegSocket		m_regSocket;

	uint8_t			m_nLineSocketJIG_Type	= 0;	// H, V

	HWND			m_hWndOwner				= nullptr;
	UINT			m_nWM_Notify			= NULL;
	inline void		WM_Update_GUI_Data		(__in LPCTSTR IN_szRFID, __in uint16_t IN_nFlag);
	inline void		WM_Update_GUI_Data_All	(__in uint16_t IN_nFlag);

	// ?ºÏù∏ ?ïÎ≥¥//
	const CLineInfo*	m_pLineInfo			= nullptr;

	// ?îÎ≤ÑÍ∑??ïÎ≥¥//
	const ST_DebugInfo*	m_pDebugInfo		= nullptr;

	// MES Rework NG Code
	int16_t				m_nMES_ReworkCode	= 0;	// REWORK ?úÌíà MES Í≤∞Í≥º ÏΩîÎìú: ?§ÎπÑ?êÏÑú ?¥ÎãπÏΩîÎìúÎ°?Î¶¨ÌÑ¥ ??Í≤ΩÏö∞ REWORK ?úÌíà?ºÎ°ú ?ëÌíà Ï≤òÎ¶¨

	// ?åÏùº or ?àÏ??§Ìä∏Î¶?Ï≤òÎ¶¨//
	bool	Save_SocketInfo				(__in LPCTSTR IN_szRFID);
	bool	Save_SocketInfo_Barcode		(__in LPCTSTR IN_szRFID);
	bool	Save_SocketInfo_Status		(__in LPCTSTR IN_szRFID);
	bool	Save_SocketInfo_Equipment	(__in LPCTSTR IN_szRFID);
	bool	Save_SocketInfo_Location	(__in LPCTSTR IN_szRFID);
	bool	Save_SocketInfo_Target		(__in LPCTSTR IN_szRFID);
	bool	Save_SocketInfo_Yield		(__in LPCTSTR IN_szRFID);
	bool	Save_SocketInfo_TestResult	(__in LPCTSTR IN_szRFID);
	bool	Save_SocketInfo_InputTime	(__in LPCTSTR IN_szRFID);
	bool	Save_SocketInfo_OutputTime	(__in LPCTSTR IN_szRFID);

	bool	Load_SocketInfo				(__in LPCTSTR IN_szRFID);

	//bool	Save_ReworkInfo				(__in LPCTSTR IN_szRFID);
	//bool	Load_ReworkInfo				(__in LPCTSTR IN_szBarcode, __in LPCTSTR IN_szRFID);
	//bool	Remove_ReworkInfo			(__in LPCTSTR IN_szBarcode);

	bool	Write_File					(__in LPCTSTR IN_szPathFile, __in LPCTSTR IN_szData, __in LPCTSTR IN_szHeader);

public:

	// ?ÑÏ≤¥ ?∞Ïù¥?? Ï¥àÍ∏∞??/
	void	Reset						();	

	// ?πÏ†ï ?åÏºì ?∞Ïù¥??Ï¥àÍ∏∞??/
	void	Reset_Socket				(__in LPCTSTR szRFID);	

	// ?åÏºì Î™©Î°ù ?ÖÎç∞?¥Ìä∏ (?åÏºì ?∞Ïù¥??Ï∂îÍ?)//
#ifdef _UNICODE
	void	Init_Sockets				(__in const std::vector<std::wstring>* IN_RFIDs);
#else
	void	Init_Sockets				(__in const std::vector<std::string>* IN_RFIDs);
#endif

	// ?ùÏÇ∞?òÎüâ Ï¶ùÍ? (equipment id, socket rfid, pass/fail)//
	void	IncreasePass_byEqp			(__in LPCTSTR szEqpID, __in LPCTSTR szRFID, __in uint8_t IN_nPara);
	void	IncreaseFail_byEqp			(__in LPCTSTR szEqpID, __in LPCTSTR szRFID, __in uint8_t IN_nPara);
	// Unload??Unregister ????Ï≤òÎ¶¨//
	void	IncreaseYield_Final			(__in LPCTSTR szRFID);

	// ?åÏºì Í∞?àò//
	size_t	GetCount					() const;

	// Î™®Îì† ?åÏºì ??†ú//
	void	RemoveAll					();

	// ?åÏºì ?ΩÏûÖ//
	bool	Socket_Insert				(__in CSocketInfo_Unit& IN_stSocket);

	// ?åÏºì ??†ú//
	void	Socket_Remove				(__in LPCTSTR szRFID);

	// ?åÏºì ?òÏ†ï//
	void	Socket_Modify				(__in CSocketInfo_Unit* IN_pstSocket);

	// RFID ?ïÎ≥¥Î°??åÏºì ?ïÎ≥¥ ?ªÍ∏∞//
	CSocketInfo_Unit&		GetAt		(__in LPCTSTR szRFID);
	const CSocketInfo_Unit& GetAt		(__in LPCTSTR szRFID) const;


public:

	// DataÎ•?GUI???úÏãú?òÍ∏∞ ?ÑÌï¥ ?¨Ïö©//
	void	Set_OwnerHWND				(__in HWND IN_hWnd);
	// Notify Windows Message ?§Ï†ï//
	void	Set_WinMsg_UpdateUI			(__in UINT IN_nWM_UpdateUI);


	// ?ÑÏû¨ ?¨Ïö© ?§Ï†ï???åÏºì JIG Type//
	void	Set_LineSocketJIG_Type		(__in uint8_t IN_nSocketType);
	uint8_t	Get_LineSocketJIG_Type		() const;

	// ?åÏºì RFID Í∏∞Ï??ºÎ°ú Socket TypeÍ≥?CommentÍ∞Ä Î≥ÄÍ≤ΩÎêú Í≤ΩÏö∞//
	void	Set_SocketConfig			(__in const CSocketList* IN_pSocketList);
	// ?åÏºì ?ïÎ≥¥ ?§Ï†ï//
	void	Set_SocketInfo				(__in LPCTSTR IN_szRFID, __in const CSocketInfo_Unit* IN_pSocketInfo);
	// ?ÑÎ°úÍ∑∏Îû® ?úÏûë ???Ä?•Îêú ?åÏºì ?ïÎ≥¥ ?§Ï†ï//
	void    Set_Stored_SocketInfo		(__in LPCTSTR IN_szRFID, __in const CSocketInfo_Unit* IN_pSocketInfo);

	void	SetPtr_LineInfo				(__in const CLineInfo* IN_pConfigLine);

	void	SetPtr_DebugMode			(__in const ST_DebugInfo* IN_pDebugInfo);

	// MES Rework NG Code
	void	Set_MES_ReworkCode			(__in int16_t IN_nReworkCode);
	int16_t	Get_MES_ReworkCode			() const;

protected:

	// Î°úÎçî ?¨ÏûÖ?úÍ∞Ñ//
	void	Set_InputTime				(__in LPCTSTR IN_szRFID);
	// ?∏Î°ú??Î∞∞Ï∂ú ?úÍ∞Ñ//
	void	Set_OutputTime				(__in LPCTSTR IN_szRFID);

	// ?åÏºì???úÌíà Î∞îÏΩî???§Ï†ï//
	void	Set_Barcode					(__in LPCTSTR IN_szRFID, __in LPCTSTR IN_szBarcode, __in bool IN_bRework = false);	
	
	// ?åÏºì???ÅÌÉú ?§Ï†ï//
	void	Set_Status					(__in LPCTSTR IN_szRFID, __in uint8_t IN_nStatus);
	
	// ?úÌíà??ÏµúÏ¢Ö ??Î∂??êÏ†ï//
	void	Set_Judgment_Final			(__in LPCTSTR IN_szRFID, __in uint8_t IN_nJudge);

	// ?åÏºì???àÎäî ?§ÎπÑ ?§Ï†ï//
	void	Set_Equipment				(__in LPCTSTR IN_szRFID, __in LPCTSTR IN_szEqpID);
	
	// ?§ÎπÑ?¥Ïùò ?åÏºì ?ÑÏπò (Port, Conveyor) ?§Ï†ï Í∞Ä?•ÌïòÎ©?.
	void	Set_Location				(__in LPCTSTR IN_szRFID, __in LPCTSTR IN_szEqpID, __in uint8_t IN_nLocation);
	
	// ?åÏºì??Î™©Ï†Å ?§ÎπÑ ?§Ï†ï//
	void	Set_TargetEquipment			(__in LPCTSTR IN_szRFID, __in uint8_t IN_nEqpOrder, __in LPCTSTR IN_szEqpID);
	
	// ?úÌíà??Í≤Ä?¨Î≥Ñ Í≤∞Í≥º ?§Ï†ï//
	void	Set_Judgment_Tester			(__in LPCTSTR IN_szRFID, __in uint8_t IN_nTester, __in uint8_t IN_nJudge);

	void	Reset_Socket_Yield			(__in LPCTSTR szRFID);
	void	Reset_Socket_Yield_All		();

	

public:
	// ?µÏã† ?¥Î≤§??Ï≤òÎ¶¨ --------------------------------------------------------
	// ?åÏºì ?¨ÏûÖ Í∞Ä???//
	bool	Is_ExistSocket				(__in LPCTSTR IN_szRFID);
	bool	Verify_Socket				(__in LPCTSTR IN_szRFID);
	bool	Register_Socket				(__in LPCTSTR IN_FromEqpID, __in LPCTSTR IN_szRFID, __in LPCTSTR IN_szBarcode);
	bool	Unregister_Socket			(__in LPCTSTR IN_FromEqpID, __in LPCTSTR IN_szRFID);
	bool	Get_TestResult				(__in LPCTSTR IN_szRFID, __out ST_TestResult& OUT_stTestResult);
	void	Check_UnloadTime			(__in LPCTSTR IN_szRFID);
	//bool	Get_TestResult				(__in LPCTSTR IN_FromEqpID, __in LPCTSTR IN_szRFID, __out ST_TestResult& OUT_stTestResult);	
	bool	Set_TestResult				(__in LPCTSTR IN_FromEqpID, __in LPCTSTR IN_szRFID, __in int16_t IN_nNGCode, __in uint8_t IN_nPara, __in uint8_t IN_nNG_EqpOrder, __in uint8_t IN_nNG_EqpType, __in bool IN_bRework = false);
	bool	Set_TestResult				(__in LPCTSTR IN_FromEqpID, __in LPCTSTR IN_szRFID, __in const ST_TestResult* IN_pstTestResult, __in bool IN_bRework = false);
	
	void	Set_SocketLocation			(__in LPCTSTR IN_szRFID, __in LPCTSTR IN_szEqpID, __in uint8_t IN_nLocation);
	void	Set_SocketTarget			(__in LPCTSTR IN_szRFID, __in uint8_t IN_nEqpOrder, __in LPCTSTR IN_szEqpID);

	// ?åÏºì ?∞Ïù¥??Ï¥àÍ∏∞??/
	void	Reset_RegisterData			(__in LPCTSTR IN_szRFID);
	void	Reset_RegisterData_All		();
	void	Reset_TestResult			(__in LPCTSTR IN_szRFID);
	void	Reset_Yield					(__in LPCTSTR IN_szRFID);
	void	Reset_Yield_All				();
	
	// ?åÏºì Í≤∞Í≥º Log//
	bool	Write_CSV_File				(__in LPCTSTR IN_szPath);
	
#if (USE_XML)
protected:
	void	Set_LOTID					(__in LPCTSTR IN_szRFID, __in LPCTSTR IN_szData);
	bool	Save_SocketInfo_LOTID		(__in LPCTSTR IN_szRFID);
public:
	bool	Is_ExistBarcode				(__in LPCTSTR IN_szBarcode);
	bool	Register_SocketLotID		(__in LPCTSTR IN_szRFID, __in uint32_t IN_TYPE, __in LPCTSTR IN_szData);
#endif
};

#endif // SocketMonitoring_h__
