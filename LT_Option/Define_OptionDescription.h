//*****************************************************************************
// Filename	: Define_OptionDescription.h
// Created	: 2010/8/26
// Modified	: 2010/8/26 - 11:35
//
// Author	: PiRing
//	
// Purpose	: 옵션 설명 헤더 파일
//*****************************************************************************
#ifndef Define_OptionDescription_h__
#define Define_OptionDescription_h__


// OPT_KEY	=> 항목명 (Registry, ini 파일 저장 할때)
// OPT_DESC	=> 항목에 대한 설명

// 시리얼 통신 포트 설정
#define OPT_KEY_COMPORT					_T("Com_Port")
#define OPT_KEY_BAUDRATE				_T("Baud_Rate")
#define OPT_KEY_BYTESIZE				_T("ByteSize")
#define OPT_KEY_PARITY					_T("Parity")
#define OPT_KEY_STOPBITS				_T("StopBits")

#define OPT_DESC_COMPORT				_T("Device name.")
#define OPT_DESC_BAUDRATE				_T("Baud rate such as 9600 or 115200 etc")
#define OPT_DESC_BYTESIZE				_T("Number of data bits. Possible values. (4 ~ 8)")
#define OPT_DESC_PARITY					_T("Enable parity checking.")
#define OPT_DESC_STOPBITS				_T("Number of stop bits.")

// 이더넷 통신
#define OPT_KEY_IP_ADDRESS				_T("IP_Address")
#define OPT_KEY_IP_PORT					_T("IP_Port")
#define OPT_DESC_IP_ADDRESS				_T("Internet Protocol Address")
#define OPT_DESC_IP_PORT				_T("Internet Address Port Number")

// 검사기 설정
#define OPT_KEY_USE_AUTO_RESTART		_T("UseAutoRestart")
#define OPT_KEY_USE_DEVICE_INIFO_PANE	_T("UseDeviceInfoPane")
#define OPT_KEY_USE_AUTO_CONN			_T("UseAutoConnection")
#define OPT_KEY_CHARTMONITOR_INDEX		_T("ChartMonitorIndex")
#define OPT_KEY_ACK_TIMEOUT				_T("ACKTimeOut")
#define OPT_KEY_PASSWORD				_T("Password_Admin")

#define OPT_DESC_USE_AUTO_RESTART		_T("프로그램 비 정상 종료 시에 자동으로 재 실행되는 기능 사용 여부")
#define OPT_DESC_USE_DEVICE_INFO_PANE	_T("장치 연결 상태표시 윈도우 사용여부")
#define OPT_DESC_USE_AUTO_CONN			_T("자동 통신 연결 사용 여부")
#define OPT_DESC_CHART_INDEX			_T("Chart Monitor Index")
#define OPT_DESC_ACK_TIMEOUT			_T("Remote 통신 시 응답 대기 시간.\r\n 단위 : 밀리 초")
#define OPT_DESC_PASSWORD				_T("프로그램에서 사용되는 암호 설정")


// 기타 설정
#define OPT_KEY_SHIFT_CHG_TIME			_T("ShiftChangeTime")
#define OPT_DESC_SHIFT_CHG_TIME			_T("교대 시간")

#define OPT_KEY_MONITORING_CYCLE		_T("MonitoringCycle")
#define OPT_DESC_MONITORING_CYCLE		_T("모니터링 주기 (mSec)")

// AJin Digital I/O
#define OPT_KEY_USE_INTERRUPT			_T("UseInterrupt")
#define OPT_DESC_USE_INTERRUPT			_T("인터럽트 사용여부")

#define OPT_KEY_PULSE_DELAY				_T("PulseDelay")
#define OPT_DESC_PULSE_DELAY			_T("Pulse 신호 지속 시간")
#define OPT_KEY_TOGGLE_CNT				_T("ToggleCount")
#define OPT_DESC_TOGGLE_CNT				_T("Toggle 신호 반복 횟수")
#define OPT_KEY_TOGGLE_ON_TIME			_T("ToggleOnTimeDelay")
#define OPT_DESC_TOGGLE_ON_TIME			_T("Toggle 신호 On 상태 지속 시간")
#define OPT_KEY_TOGGLE_OFF_TIME			_T("ToggleOffTimeDelay")
#define OPT_DESC_TOGGLE_OFF_TIME		_T("Toggle 신호 Off 상태 지속 시간")


#define OPT_KEY_RECORD_TYPE				_T("RecordType")
#define OPT_DESC_RECORD_TYPE			_T("영상녹화 방식 선택\r\n Source Frame - Based : 소스에서 수신되는 영상 프레임 기반으로 녹화\r\n Time - Based : 설정된 FPS기반으로 영상 녹화")

#define OPT_KEY_FRAME_SKIP				_T("FrameSkip")
#define OPT_DESC_FRAME_SKIP				_T("녹화중에 UI에 표시되는 영상은 프레임 스킵을 한다. \r\n녹화 파일에는 적용되지 않는다. (CPU 부하 감소)")


#endif // Define_OptionDescription_h__
