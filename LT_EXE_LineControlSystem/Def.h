#pragma once

enum {
	FORM_NULL,
	FORM_NEW,
	FORM_CREATE,
	FORM_DUMMY,
	FORM_MAX,
};


enum {
	MDL_T,
	MDL_MAX
};

static char * chModuleView[] = {
	"Test,",
	"",
};

enum {
	GC_0,
	GC_MAX,
};

enum {
	RSLT_OK,
	RSLT_NG,
	RSLT_FAIL,
	RSLT_MAX,
};
static char * chRslt[] = {
	"Ok,", 
	"NG", 
	"Fail",	
	"",
};