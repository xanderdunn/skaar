#include "GameAPI.h"
#include "Utilities.h"

Heap * g_formHeap = (Heap *)0x01B418B0;

void * FormHeap_Allocate(UInt32 size)
{
	return CALL_MEMBER_FN(g_formHeap, Allocate)(size, 0, false);
}

void FormHeap_Free(void * ptr)
{
	CALL_MEMBER_FN(g_formHeap, Free)(ptr, false);
}

PlayerCharacter	** g_thePlayer = (PlayerCharacter **)0x01B2E8E4;
const UInt32 * g_TlsIndexPtr = (UInt32 *)0x01BBEB54;

struct TLSData
{
	// thread local storage

	UInt32	pad000[(0x2F8 - 0x000) >> 2];	// 000
	UInt8	consoleMode;					// 2F8
	UInt8	pad2F9[3];						// 2F9
};

static TLSData * GetTLSData()
{
	UInt32 TlsIndex = *g_TlsIndexPtr;
	TLSData * data = NULL;

	__asm {
		mov		ecx,	[TlsIndex]
		mov		edx,	fs:[2Ch]	// linear address of thread local storage array
		mov		eax,	[edx+ecx*4]
		mov		[data], eax
	}

	return data;
}

void Console_Print(const char * fmt, ...)
{
	ConsoleManager	* mgr = ConsoleManager::GetSingleton();
	if(mgr)
	{
		va_list	args;

		va_start(args, fmt);

		CALL_MEMBER_FN(mgr, Print)(fmt, args);

		va_end(args);
	}
}

bool IsConsoleMode(void)
{
	return GetTLSData()->consoleMode != 0;
}
