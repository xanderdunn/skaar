#pragma once

class VMClassRegistry;
struct StaticFunctionTag;

#include "GameTypes.h"

namespace papyrusSKSE
{
	void RegisterFuncs(VMClassRegistry* registry);

	UInt32 GetVersion(StaticFunctionTag* base);
	UInt32 GetVersionMinor(StaticFunctionTag* base);
	UInt32 GetVersionBeta(StaticFunctionTag* base);
	UInt32 GetVersionRelease(StaticFunctionTag* base);
	UInt32 GetPluginVersion(StaticFunctionTag* base, BSFixedString name);
}
