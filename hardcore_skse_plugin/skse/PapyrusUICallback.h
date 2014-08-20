#pragma once

#include "GameTypes.h"
#include "PapyrusObjects.h"

struct StaticFunctionTag;
class VMClassRegistry;
template <typename T> class VMArray;

namespace papyrusUICallback
{
	RawHandleT	Create(StaticFunctionTag* thisInput, BSFixedString menuName, BSFixedString targetStr);
	bool		Send(StaticFunctionTag* thisInput, RawHandleT handle);
	void		Release(StaticFunctionTag* thisInput, RawHandleT handle);

	template <typename T>
	void		Push(StaticFunctionTag* thisInput, RawHandleT handle, T arg);

	template <typename T>
	void		PushArray(StaticFunctionTag* thisInput, RawHandleT handle, VMArray<T> args);

	void		RegisterFuncs(VMClassRegistry* registry);
	void		RevertGlobalData();
}