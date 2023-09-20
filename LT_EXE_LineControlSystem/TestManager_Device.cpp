//*****************************************************************************
// Filename	: 	TestManager_Device.cpp
// Created	:	2016/10/6 - 13:46
// Modified	:	2016/10/6 - 13:46
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
#include "stdafx.h"
#include "TestManager_Device.h"
#include "CommonFunction.h"

CTestManager_Device::CTestManager_Device()
{
	//OnInitialize();
	m_pIcsComm = &CIcsCommunicator::GetInstance();
	

}


CTestManager_Device::~CTestManager_Device()
{
	TRACE(_T("<<< Start ~CTestManager_Device >>> \n"));

	//OnFinalize();
	if (m_pIcsComm->IsOpened())
	{
		m_pIcsComm->Close();
	}
	
//	delete m_pIcsComm;


	TRACE(_T("<<< End ~CTestManager_Device >>> \n"));
}

//=============================================================================
// Method		: OnLoadOption
// Access		: virtual protected  
// Returns		: BOOL
// Qualifier	:
// Last Update	: 2016/5/18 - 18:58
// Desc.		:
//=============================================================================
BOOL CTestManager_Device::OnLoad_Option()
{
	CLT_Option	stOption;
	stOption.SetInspectorType((enInsptrSysType)SET_INSPECTOR);
	BOOL		bReturn = TRUE;

	stOption.SetRegistryPath(REG_PATH_OPTION_BASE);
	bReturn &= stOption.LoadOption_Inspector(m_stOption.Inspector);
	bReturn &= stOption.LoadOption_Server(m_stOption.Server);
	//bReturn &= stOption.LoadOption_Misc(m_stOption.Misc);
	
	return bReturn;
}

//=============================================================================
// Method		: InitDevicez
// Access		: virtual protected  
// Returns		: void
// Parameter	: HWND hWndOwner
// Qualifier	:
// Last Update	: 2016/5/18 - 18:41
// Desc.		: 주변 장치 초기화
//=============================================================================
void CTestManager_Device::OnInit_Devicez(HWND hWndOwner /*= NULL*/)
{
	

}

//=============================================================================
// Method		: OnConnect_Devicez
// Access		: virtual protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/1/29 - 14:39
// Desc.		:
//=============================================================================
void CTestManager_Device::OnConnect_Devicez()
{
	OnShow_SplashScreen(TRUE, _T("Starting Server"));
	
	CString szText;
	szText = ConvIPAddrToString(m_stOption.Server.Address.dwAddress);

	if (m_pIcsComm->Open(szText.GetBuffer(), static_cast<lt::ushort>(m_stOption.Server.Address.dwPort)))
	{
		OnLog(_T("Sever Starting.. (port: %d)"), m_stOption.Server.Address.dwPort);

		OnSetStatus_Server(COMM_STATUS_CONNECT);
	}
	else
	{
		OnLog_Err(_T("Sever Start failed!! (port: %d)"), m_stOption.Server.Address.dwPort);

		OnSetStatus_Server(COMM_STATUS_NOTCONNECTED);
	}

	//OnShow_SplashScreen(FALSE);
}

//=============================================================================
// Method		: OnDisconnect_Devicez
// Access		: virtual protected  
// Returns		: void
// Qualifier	:
// Last Update	: 2021/1/29 - 14:39
// Desc.		:
//=============================================================================
void CTestManager_Device::OnDisconnect_Devicez()
{
	OnShow_SplashScreen(TRUE, _T("Close Server"));

	OnLog(_T("Sever Closing.. (port: %d)"), m_stOption.Server.Address.dwPort);
	m_pIcsComm->Close();
	

	OnSetStatus_Server(COMM_STATUS_NOTCONNECTED);
	
	//OnShow_SplashScreen(FALSE);
}

//=============================================================================
// Method		: OnInitialize
// Access		: virtual public  
// Returns		: void
// Qualifier	:
// Last Update	: 2016/9/28 - 20:10
// Desc.		:
//=============================================================================
void CTestManager_Device::OnInitialize()
{
	// 테스트 코드

}

//=============================================================================
// Method		: OnFinalize
// Access		: virtual public  
// Returns		: void
// Qualifier	:
// Last Update	: 2016/9/28 - 20:10
// Desc.		:
//=============================================================================
void CTestManager_Device::OnFinalize()
{

}
