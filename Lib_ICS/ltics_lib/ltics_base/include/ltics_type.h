#pragma once

#define LTICS_LIB_EXP __declspec(dllexport)
#define LTICS_LIB_IMP __declspec(dllimport)

#if defined(LTICS_LIB_EXPORTS)
	#define LTICS_LIB_API LTICS_LIB_EXP
	#define LTICS_LIB_CLASS LTICS_LIB_EXP
#else
	#if defined(LTICS_LIB_IMPORTS)
		#define LTICS_LIB_API LTICS_LIB_IMP
		#define LTICS_LIB_CLASS LTICS_LIB_IMP
	#else
		#define LTICS_LIB_API
		#define LTCIS_LIB_CLASS
	#endif
#endif
