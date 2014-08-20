#pragma once

#include "skse/NiRenderer.h"

void Hooks_NetImmerse_Init(void);
void Hooks_NetImmerse_Commit(void);

extern UInt32 g_tintTextureResolution;

typedef void * (__stdcall * _ClipTextureDimensions)(NiDX9Renderer * a1, UInt32 a2, UInt32 * a3, UInt32 * a4, UInt32 * a5, UInt32 * a6, UInt32 * a7, UInt32 * a8, UInt32 * a9, UInt32 * a10);
extern _ClipTextureDimensions ClipTextureDimensions;