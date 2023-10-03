#pragma once

#include "TypeDecl.h"
#include <memory>

namespace lt
{
	inline void SafeFree(void *& ptr)
	{
		if(ptr != nullptr)
		{
			free(ptr);
			ptr = nullptr;
		}
	}

	template<typename Type>
	inline void SafeDelete(Type *& ptr)
	{
		if(ptr != nullptr)
		{
			delete ptr;
			ptr = nullptr;
		}
	}

	template<typename TypeArr>
	inline void SafeDeleteArray(TypeArr *& ptr)
	{
		if(ptr != nullptr)
		{
			delete [] ptr;
			ptr = nullptr;
		}
	}

	template<typename Type>
	inline void SafeTypeDelete(void *& ptr)
	{
		SafeDelete(reinterpret_cast<Type *&>(ptr));
	}

	template<typename Type>
	inline void SafeRelease(Type *& ptr)
	{
		if(ptr != nullptr)
		{
			ptr->Release();
			ptr = nullptr;
		}
	}

	inline void SafeDeleteObject(HGDIOBJ & handle)
	{
		if(handle != nullptr)
		{
			::DeleteObject(handle);
			handle = nullptr;
		}
	}

	inline void SafeCloseHandle(HANDLE & handle)
	{
		if(handle != nullptr)
		{
			::CloseHandle(handle);
			handle = nullptr;
		}
	}

	inline void SafeFreeLibrary(HMODULE & handle)
	{
		if(handle != nullptr)
		{
			::FreeLibrary(handle);
			handle = nullptr;
		}
	}

	template<typename Type>
	inline void Swap(Type & t1, Type & t2)
	{
		auto temp = std::move(t1);
		t1 = std::move(t2);
		t2 = std::move(temp);
	}
}
