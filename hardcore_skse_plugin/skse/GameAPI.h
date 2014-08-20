#pragma once

#include "skse/Utilities.h"

class PlayerCharacter;

// it seems like there are multiple buckets this time - investigate
// move this in to GameMemory.h or something like that if further investigated
class Heap
{
public:
	MEMBER_FN_PREFIX(Heap);
	// haven't verified alignment vars
	DEFINE_MEMBER_FN(Allocate, void *, 0x00A48D60, UInt32 size, UInt32 alignment, bool aligned);
	DEFINE_MEMBER_FN(Free, void, 0x00A487B0, void * buf, bool aligned);
};

extern Heap * g_formHeap;

void * FormHeap_Allocate(UInt32 size);
void FormHeap_Free(void * ptr);

extern PlayerCharacter ** g_thePlayer;

void Console_Print(const char * fmt, ...);
bool IsConsoleMode(void);

class ConsoleManager
{
public:
	MEMBER_FN_PREFIX(ConsoleManager);
	DEFINE_MEMBER_FN(Print, void, 0x008486F0, const char * fmt, va_list args);

	ConsoleManager();
	~ConsoleManager();

	static ConsoleManager * GetSingleton(void)
	{
		return *((ConsoleManager **)0x01B10B58);
	}

	void	* scriptContext;	// 00
};