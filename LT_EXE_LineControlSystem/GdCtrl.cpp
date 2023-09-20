// GdCtrl.cpp: 구현 파일
//

#include "stdafx.h"
#include "GdCtrl.h"


// CGdCtrl

IMPLEMENT_DYNAMIC(CGdCtrl, CMFCPropertyGridCtrl)

CGdCtrl::CGdCtrl()
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif

}
CGdCtrl::CGdCtrl(int ID, int *Rslt)
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif	
	nID = ID;
	*Rslt = RSLT_OK;
}
CGdCtrl::~CGdCtrl()
{
}


BEGIN_MESSAGE_MAP(CGdCtrl, CMFCPropertyGridCtrl)
END_MESSAGE_MAP()



// CGdCtrl 메시지 처리기


