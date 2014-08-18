#pragma once

//#ifdef _PPAPI
#include "skse/PluginAPI.h"
//#endif

void Hooks_Papyrus_Init(void);
void Hooks_Papyrus_Commit(void);

//#ifdef _PPAPI
bool RegisterPapyrusPlugin(SKSEPapyrusInterface::RegisterFunctions);
//#endif
