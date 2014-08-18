#include "PapyrusUtility.h"

#include "GameForms.h"
#include "GameObjects.h"
#include "GameReferences.h"
#include "GameExtraData.h"
#include "GameRTTI.h"
#include "GameSettings.h"

#include "PapyrusVM.h"
#include "PapyrusNativeFunctions.h"

namespace papyrusUtility
{
	float GetINIFloat(StaticFunctionTag* base, BSFixedString ini)
	{
		Setting * setting = GetINISetting(ini.data);
		if(!setting || setting->GetType() != Setting::kType_Float)
			return 0.0;

		return setting->data.f32;
	}

	UInt32 GetINIInt(StaticFunctionTag* base, BSFixedString ini)
	{
		Setting * setting = GetINISetting(ini.data);
		if(!setting || setting->GetType() != Setting::kType_Integer)
			return 0;

		return setting->data.s32;
	}

	bool GetINIBool(StaticFunctionTag* base, BSFixedString ini)
	{
		Setting * setting = GetINISetting(ini.data);
		if(!setting || setting->GetType() != Setting::kType_Bool)
			return false;

		return setting->data.u8 >= 1 ? true : false;
	}

	BSFixedString GetINIString(StaticFunctionTag* base, BSFixedString ini)
	{
		Setting * setting = GetINISetting(ini.data);
		if(!setting || setting->GetType() != Setting::kType_String)
			return NULL;

		return setting->data.s;
	}
}

void papyrusUtility::RegisterFuncs(VMClassRegistry* registry)
{
	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, float, BSFixedString>("GetINIFloat", "Utility", papyrusUtility::GetINIFloat, registry));

	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, UInt32, BSFixedString>("GetINIInt", "Utility", papyrusUtility::GetINIInt, registry));

	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, bool, BSFixedString>("GetINIBool", "Utility", papyrusUtility::GetINIBool, registry));

	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, BSFixedString, BSFixedString>("GetINIString", "Utility", papyrusUtility::GetINIString, registry));

	registry->SetFunctionFlags("Utility", "GetINIFloat", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Utility", "GetINIInt", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Utility", "GetINIBool", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("Utility", "GetINIString", VMClassRegistry::kFunctionFlag_NoWait);
}
