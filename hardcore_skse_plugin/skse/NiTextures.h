#pragma once

#include "skse/NiObjects.h"
#include "skse/GameTypes.h"

#include <d3d9.h>

class NiDX9Renderer;

// 44
class NiPixelFormat
{
public:
	// 0C
	class NiComponentSpec
	{
	public:
		UInt32	type;		// 00
		UInt32	format;		// 04
		UInt8	bits;		// 08
		UInt8	isSigned;	// 09
		UInt8	pad0A;		// 0A
		UInt8	pad0B;		// 0B
	};

	UInt8	flags;			// 00
	UInt8	bpp;			// 01
	UInt8	srgb;			// 02
	UInt8	pad03;			// 03
	UInt32	format;			// 04
	UInt32	tiling;			// 08
	UInt32	rendererHint;	// 0C
	UInt32	extraData;		// 10

	NiComponentSpec	components[4];	// 14
};


class NiPersistentSrcTextureRendererData : public NiObject
{
public:
	virtual ~NiPersistentSrcTextureRendererData();
};

MAKE_NI_POINTER(NiPersistentSrcTextureRendererData);

class NiDX9PersistentSrcTextureRendererData : public NiPersistentSrcTextureRendererData
{
public:
	virtual ~NiDX9PersistentSrcTextureRendererData();
};

class NiPixelData : public NiObject
{
public:
	virtual ~NiPixelData();

	NiPixelFormat	pixelFormat;		// 08
	void *			unk0C;				// 4C

	UInt8 *			m_pucPixels;		// 50	[mipmapLevels]
	UInt32 *		m_puiWidth;			// 54	[mipmapLevels]
	UInt32 *		m_puiHeight;		// 58	[mipmapLevels]
	UInt32 *		m_puiOffset;		// 5C	[mipmapLevels + 1]

	UInt32			m_uiMipmapLevels;	// 60
	UInt32			m_uiPixelStride;	// 64

	UInt32			unk68;				// 68
	UInt32			m_uiFaces;			// 6C
	UInt8			unk70;				// 70
	UInt8			pad71[3];			// 71

	UInt32 GetWidth(UInt32 mipmapLevel)
	{
		return m_puiWidth[mipmapLevel];
	}

	UInt32 GetHeight(UInt32 mipmapLevel)
	{
		return m_puiHeight[mipmapLevel];
	}

	UInt8 * GetPixels(UInt32 uiMipmapLevel = 0, UInt32 uiFace = 0)
	{
		return m_pucPixels + uiFace*m_puiOffset[uiMipmapLevel] +
			m_puiOffset[uiMipmapLevel];
	};
};

MAKE_NI_POINTER(NiPixelData);

// 14
class Ni2DBuffer : public NiObject
{
public:
	virtual ~Ni2DBuffer();

	UInt32	width;			// 08
	UInt32	height;			// 0C
	void	* rendererData;	// 10
};

// 24
class NiTexture : public NiObject
{
public:
	virtual ~NiTexture();

	virtual void	Unk_21(void);
	virtual void	Unk_22(void);
	virtual UInt32	GetWidth(void) = 0;
	virtual UInt32	GetHeight(void) = 0;
	virtual UInt32	GetByteSize(void);

	// 0C
	struct FormatPrefs
	{
	public:
		UInt32	pixelLayout;	// 00
		UInt32	alphaFormat;	// 04
		UInt32	mipMapped;		// 08
	};

	// 70
	class RendererData : public NiObject
	{
	public:
		virtual void Unk_21(void);
		virtual void Unk_22(void);
		virtual void Unk_23(void);
		virtual void Unk_24(void);
		virtual void * Unk_25(void);

		NiTexture		* parent;	// 08
		UInt32			width;		// 0C
		UInt32			height;		// 10
		NiPixelFormat	format;		// 14
		UInt8			unk58;		// 58
		UInt8			unk59;		// 59
		UInt8			unk5A;		// 5A
		UInt8			unk5B;		// 5B
		UInt32			unk5C;		// 5C
		
	};

	class NiDX9TextureData : public RendererData
	{
	public:
		NiDX9Renderer	* renderer;		// 60
		LPDIRECT3DBASETEXTURE9 texture;	// 64
		UInt16			levels;			// 68
		UInt16			textureType;	// 6A
		UInt32			unk6C;			// 6C
	};

	FormatPrefs		formatPrefs;	// 08
	RendererData	* rendererData;	// 14
	BSFixedString	name;			// 18
	NiTexture		* prevTexture;	// 1C
	NiTexture		* nextTexture;	// 20
};

// 3C
class NiRenderedTexture : public NiTexture
{
public:
	virtual ~NiRenderedTexture();

	virtual Ni2DBuffer *	GetBuffer(void);

	Ni2DBuffer	* buffer;	// 24
	UInt32	unk28;			// 28
	UInt32	unk2C;			// 2C
	UInt8	unk30;			// 30
	UInt8	unk31;			// 31
	UInt8	align32;		// 32
	UInt8	align33;		// 33
	UInt32	unk34;			// 34
	UInt32	unk38;			// 38

	MEMBER_FN_PREFIX(NiRenderedTexture);
	DEFINE_MEMBER_FN(ctor, void, 0x00FB5E10);
	DEFINE_MEMBER_FN(UpdateVirtualImage, void, 0x00A61D20, NiRenderedTexture * newTexture);
	DEFINE_MEMBER_FN(AddVirtualImage, UInt8, 0x00A61D80, BSFixedString linkageName);
	DEFINE_MEMBER_FN(ReleaseVirtualImage, void, 0x00A61CC0);
	DEFINE_MEMBER_FN(ReleaseVirtualImage2, void, 0x00A61DB0); // Also calls ReleaseVirtualImage
};

// 58
class NiRenderedCubeMap : public NiRenderedTexture
{
public:
	virtual ~NiRenderedCubeMap();

	UInt32	unk3C;	// 3C
	UInt32	unk40;	// 40
	UInt32	unk44;	// 44
	UInt32	unk48;	// 48
	UInt32	unk4C;	// 4C
	UInt32	unk50;	// 50
	UInt32	unk54;	// 54
};

// 34
class NiSourceTexture : public NiTexture
{
public:
	virtual void LoadPixelDataFromFile(void);
	virtual void DestroyAppPixelData(void);
	virtual bool CreateRendererData(void);

	NiPersistentSrcTextureRendererDataPtr	persistentSrcRendererData;		// 24
	NiPixelDataPtr							pixelData;						// 28
	UInt32									unk2C;							// 2C
	UInt8									flags;							// 30
};
