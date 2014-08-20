#include "Hooks_NetImmerse.h"
#include "SafeWrite.h"
#include "Utilities.h"

#include "common/ICriticalSection.h"

UInt32 g_tintTextureResolution = 256;

_ClipTextureDimensions ClipTextureDimensions = (_ClipTextureDimensions)0x00C90AC0;


void * __stdcall ClipTextureDimensions_Hook(NiDX9Renderer * renderer, UInt32 type, UInt32 * height, UInt32 * width, UInt32 * a5, UInt32 * a6, UInt32 * a7, UInt32 * a8, UInt32 * a9, UInt32 * a10)
{
	void * result = ClipTextureDimensions(renderer, type, height, width, a5, a6, a7, a8, a9, a10);
	switch(type) {
		case 32: // Not sure what all the types are yet
			{
				*height = g_tintTextureResolution;
				*width = g_tintTextureResolution;
			}
			break;
	}
	return result;
}

void Hooks_NetImmerse_Init(void)
{
	UInt32	tintTextureResolution = 0;
	if(GetConfigOption_UInt32("Display", "iTintTextureResolution", &tintTextureResolution))
	{
		g_tintTextureResolution = tintTextureResolution;
	}
}

void Hooks_NetImmerse_Commit(void)
{
	WriteRelCall(0x00C91800 + 0x3C, (UInt32)ClipTextureDimensions_Hook);
	WriteRelCall(0x00C92330 + 0x4C, (UInt32)ClipTextureDimensions_Hook);
}
