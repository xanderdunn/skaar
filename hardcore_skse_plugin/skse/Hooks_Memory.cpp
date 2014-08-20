#include "Hooks_Memory.h"
#include "SafeWrite.h"
#include "Utilities.h"

// greets to sheson for his work on this
// it must run before static initializers are run

void Hooks_Memory_PreloadCommit(void)
{
	// these are the values used by the game
	const UInt32 kVanilla_defaultHeapInitialAllocMB = 256 + 256;
	const UInt32 kVanilla_scrapHeapSizeMB = 256;

	// clamp to these values because going over that is stupid
	const UInt32 kMax_defaultHeapInitialAllocMB = 1024 + 256;
	const UInt32 kMax_scrapHeapSizeMB = 512;

	// in megabytes, -256 depending on bInitiallyLoadAllClips:Animation
	UInt32 defaultHeapInitialAllocMB = kVanilla_defaultHeapInitialAllocMB;
	UInt32 scrapHeapSizeMB = kVanilla_scrapHeapSizeMB;

	GetConfigOption_UInt32("Memory", "DefaultHeapInitialAllocMB", &defaultHeapInitialAllocMB);
	GetConfigOption_UInt32("Memory", "ScrapHeapSizeMB", &scrapHeapSizeMB);

	if(	(defaultHeapInitialAllocMB != kVanilla_defaultHeapInitialAllocMB) ||
		(scrapHeapSizeMB != kVanilla_scrapHeapSizeMB))
	{
		_MESSAGE("overriding memory pool sizes");

		if(defaultHeapInitialAllocMB > kMax_defaultHeapInitialAllocMB)
		{
			_MESSAGE("%dMB default heap is too large, clamping to %dMB. using your value will make the game unstable.", defaultHeapInitialAllocMB, kMax_defaultHeapInitialAllocMB);
			defaultHeapInitialAllocMB = kMax_defaultHeapInitialAllocMB;
		}

		if(scrapHeapSizeMB > kMax_scrapHeapSizeMB)
		{
			_MESSAGE("%dMB scrap heap is too large, clamping to %dMB. using your value will make the game unstable.", defaultHeapInitialAllocMB, kMax_defaultHeapInitialAllocMB);
			scrapHeapSizeMB = kMax_scrapHeapSizeMB;
		}

		_MESSAGE("default heap = %dMB (effective %dMB if not preloading animations)", defaultHeapInitialAllocMB, defaultHeapInitialAllocMB - 0x100);
		_MESSAGE("scrap heap = %dMB", scrapHeapSizeMB);

		SafeWrite32(0x00687E87 + 2, defaultHeapInitialAllocMB); // will eventually be multiplied by 1024 * 1024
		SafeWrite32(0x00A4E6BE + 1, scrapHeapSizeMB * 1024 * 1024); // passed directly to VirtualAlloc
	}
}
