#include "Hooks_Camera.h"
#include "GameCamera.h"
#include "PapyrusEvents.h"
#include "SafeWrite.h"
#include <queue>

static const UInt32 kInstallCameraHook_Base = 0x006533D0 + 0x57;
static const UInt32 kInstallCameraHook_Entry_retn = kInstallCameraHook_Base + 0x05;

void __stdcall InstallCameraHook(TESCameraState * oldState, TESCameraState * newState)
{
	SKSECameraEvent evn(oldState, newState);
	g_cameraEventDispatcher.SendEvent(&evn);
}

__declspec(naked) void InstallHookCamera_Entry(void)
{
	__asm
	{
		pushad
		push	ecx
		push	edi
		call	InstallCameraHook
		popad

		// overwritten code
		mov		eax, ecx
		neg		eax
		pop		edi

		jmp		[kInstallCameraHook_Entry_retn]
	}
}

void Hooks_Camera_Commit(void)
{
	WriteRelJump(kInstallCameraHook_Base, (UInt32)InstallHookCamera_Entry);
}
