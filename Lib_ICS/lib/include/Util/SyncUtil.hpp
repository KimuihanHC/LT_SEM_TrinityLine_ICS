#pragma once

#include "TypeDecl.h"
#include <mutex>

namespace lt
{
	#define SimpleLock(mutex_obj, name) lt::StdLocker<std::remove_reference_t<decltype((mutex_obj))>>::SimpleLocker _##name((mutex_obj))
	#define SimpleLockRef(mutex_ref) SimpleLock((mutex_ref), mutex_ref)
	#define SimpleLockPtr(mutex_ptr) SimpleLock(*(mutex_ptr), mutex_ptr)

	inline BOOL SetOrResetEvent(HANDLE handle, bool bSetting)
	{
		static BOOL (__stdcall *Fn[])(HANDLE) =
		{
			&::ResetEvent,
			&::SetEvent
		};

		return Fn[bSetting](handle);
	}
}
