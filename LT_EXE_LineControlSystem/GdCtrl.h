#pragma once

#include "Def.h"
// CGdCtrl

class CGdCtrl : public CMFCPropertyGridCtrl
{
	DECLARE_DYNAMIC(CGdCtrl)

public:
	int nID = NULL;

	CGdCtrl();	
	CGdCtrl(int ID, int *Rslt);
	virtual ~CGdCtrl();

protected:
	DECLARE_MESSAGE_MAP()
};


