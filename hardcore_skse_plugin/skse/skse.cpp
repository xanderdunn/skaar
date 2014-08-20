#include "skse_version.h"
#include "CommandTable.h"
#include "SafeWrite.h"
#include "PluginManager.h"
#include "Utilities.h"
#include "InternalSerialization.h"
#include <shlobj.h>

IDebugLog	gLog;

#if RUNTIME

const char * kLogPath = "\\My Games\\Skyrim\\SKSE\\skse.log";

STATIC_ASSERT(RUNTIME_VERSION == RUNTIME_VERSION_1_9_32_0);

#include "Hooks_Scaleform.h"
#include "Hooks_Gameplay.h"
#include "Hooks_ObScript.h"
#include "Hooks_DirectInput8Create.h"
#include "Hooks_Papyrus.h"
#include "Hooks_SaveLoad.h"
#include "Hooks_UI.h"
#include "Hooks_NetImmerse.h"
#include "Hooks_Debug.h"
#include "Hooks_Event.h"
#include "Hooks_Camera.h"
#include "Hooks_Threads.h"
#include "Hooks_Handlers.h"

#else

const char * kLogPath = "\\My Games\\Skyrim\\SKSE\\skse_editor.log";

#endif

void WaitForDebugger(void)
{
	while(!IsDebuggerPresent())
	{
		Sleep(10);
	}

	Sleep(1000 * 2);
}

static bool isInit = false;

void SKSE_Initialize(void)
{
	if(isInit) return;
	isInit = true;

	gLog.OpenRelative(CSIDL_MYDOCUMENTS, kLogPath);

#ifndef _DEBUG
	__try {
#endif

		FILETIME	now;
		GetSystemTimeAsFileTime(&now);

#if RUNTIME
		_MESSAGE("SKSE runtime: initialize (version = %d.%d.%d %08X %08X%08X, os = %s)",
			SKSE_VERSION_INTEGER, SKSE_VERSION_INTEGER_MINOR, SKSE_VERSION_INTEGER_BETA, RUNTIME_VERSION,
			now.dwHighDateTime, now.dwLowDateTime, GetOSInfoStr().c_str());
#else
		_MESSAGE("SKSE editor: initialize (version = %d.%d.%d %08X %08X%08X, os = %s)",
			SKSE_VERSION_INTEGER, SKSE_VERSION_INTEGER_MINOR, SKSE_VERSION_INTEGER_BETA, EDITOR_VERSION,
			now.dwHighDateTime, now.dwLowDateTime, GetOSInfoStr().c_str());
#endif
		_MESSAGE("imagebase = %08X", GetModuleHandle(NULL));

#ifdef _DEBUG
		SetPriorityClass(GetCurrentProcess(), IDLE_PRIORITY_CLASS);

		WaitForDebugger();
#endif

//		Commands_Dump();

		Hooks_Debug_Init();
		Hooks_ObScript_Init();
		Hooks_Papyrus_Init();
		Hooks_NetImmerse_Init();
		Hooks_Threads_Init();
		Hooks_Handlers_Init();

		g_pluginManager.Init();

		Hooks_Debug_Commit();
		Hooks_Threads_Commit();
		Hooks_Handlers_Commit();
		Hooks_Scaleform_Commit();
		Hooks_Gameplay_Commit();
		Hooks_ObScript_Commit();
		Hooks_Papyrus_Commit();
		Hooks_UI_Commit();
		Hooks_Camera_Commit();
		Hooks_NetImmerse_Commit();

		Hooks_SaveLoad_Commit();
		Init_CoreSerialization_Callbacks();

		Hooks_DirectInput_Commit();
		Hooks_Event_Commit();

		FlushInstructionCache(GetCurrentProcess(), NULL, 0);

#ifndef _DEBUG
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		_ERROR("exception");
	}
#endif

	_MESSAGE("init complete");
}

void SKSE_DeInitialize(void)
{
	//
}

extern "C" {

	void StartSKSE(void)
	{
		SKSE_Initialize();
	}

	BOOL WINAPI DllMain(HANDLE hDllHandle, DWORD dwReason, LPVOID lpreserved)
	{
		switch(dwReason)
		{
		case DLL_PROCESS_ATTACH:
			SKSE_Initialize();
			break;

		case DLL_PROCESS_DETACH:
			SKSE_DeInitialize();
			break;
		};

		return TRUE;
	}

};
