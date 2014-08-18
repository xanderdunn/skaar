#pragma once

#include "GameTypes.h"
#include "PapyrusObjects.h"

struct StaticFunctionTag;
class VMClassRegistry;

namespace papyrusModEvent
{
	RawHandleT	Create(StaticFunctionTag* thisInput, BSFixedString eventName);
	bool		Send(StaticFunctionTag* thisInput, RawHandleT handle);
	void		Release(StaticFunctionTag* thisInput, RawHandleT handle);

	template <typename T>
	void		Push(StaticFunctionTag* thisInput, RawHandleT handle, T arg);

	void		RegisterFuncs(VMClassRegistry* registry);
	void		RevertGlobalData();
	
}
