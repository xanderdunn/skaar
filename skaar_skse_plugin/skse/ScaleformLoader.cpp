#include "ScaleformLoader.h"
#include "Translation.h"

bool g_logScaleform = false;

UInt32 GFxLoader::ctor_Hook(void)
{
	UInt32 result = CALL_MEMBER_FN(this, ctor)();

	// Read plugin list, load translation files
	Translation::ImportTranslationFiles(stateBag->GetTranslator());

	if(g_logScaleform) {
		SKSEGFxLogger * logger = new SKSEGFxLogger();
		stateBag->SetState(GFxState::kInterface_Log, (void*)logger);
	}

	return result;
}

GFxLoader * GFxLoader::GetSingleton()
{
	return *((GFxLoader **)0x01B2E9B0);
}