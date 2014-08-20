#pragma once

#include "skse/NiObjects.h"
#include "skse/NiTextures.h"

#include "skse/GameTypes.h"
#include <d3d9.h>

class NiPixelFormat;
class NiPixelData;

class NiRenderedTexture;
class NiRenderedCubeMap;

class NiRenderer : public NiObject
{
public:
	virtual ~NiRenderer();
};

class NiDX9Renderer : public NiRenderer
{
public:
	virtual ~NiDX9Renderer();

	UInt32				m_kCriticalSection;				// 08
	UInt32				m_ulThreadOwner;				// 0C
	UInt32				lockCount;						// 10
	UInt32				unk14;							// 14
	UInt32				unk18;							// 18
	UInt32				unk1C;							// 1C
	UInt32				unk20;							// 20
	UInt32				frameState;						// 24
	UInt32				unk28;							// 28
	UInt8				m_bRenderTargetGroupActive;		// 2C
	UInt8				unk2D;							// 2D
	UInt8				pad2E[2];						// 2E

	UInt32				unk30;							// 30
	UInt32				unk34;							// 34

	IDirect3DDevice9	* m_pkD3DDevice9;				// 38
	D3DCAPS9			m_kD3DCaps9;					// 3C
	HWND				m_kWndDevice;					// 16C
	HWND				m_kWndFocus;					// 170
	char				m_acDriverDesc[512];			// 174
	UInt32				m_uiAdapter;					// 374
	UInt32				m_eDevType;						// 378
	UInt32				m_uiBehaviorFlags;				// 37C
	UInt32				m_eAdapterFormat;				// 380
	UInt8				unk384;							// 384
	UInt8				unk385;							// 385
	UInt8				pad386[2];						// 386
	UInt32				m_pkAdapterDesc;				// 388
	UInt32				m_pkDeviceDesc;					// 38C

	UInt32				m_uiBackground;					// 390
	float				m_fZClear;						// 394
	UInt32				m_uiStencilClear;				// 398
	UInt32				m_uiRendFlags;					// 39C
	char				m_acBehavior[32];				// 3A0

	NiTMap<void*,void*>	m_kPrePackObjects;				// 3C0
	UInt32				unk3D0;							// 3D0
	UInt32				unk3D4;							// 3D4

	NiPoint3			m_kCamRight;					// 3D8
	NiPoint3			m_kCamUp;						// 3E4
	NiPoint3			m_kModelCamRight;				// 3F0
	NiPoint3			m_kModelCamUp;					// 3FC

	NiPoint3			unk408;							// 408
	UInt32				unk414;							// 414
	NiPoint3			unk418;							// 418
	UInt32				unk424;							// 424

	float				m_fNearDepth;					// 428
	float				m_fDepthRange;					// 42C

	D3DMATRIX			m_kD3DIdentity;					// 430

	D3DVIEWPORT9		m_kD3DPort;						// 470

	UInt32				m_uiHWBones;					// 488
	UInt32				m_uiMaxStreams;					// 48C
	UInt32				m_uiMaxPixelShaderVersion;		// 490
	UInt32				m_uiMaxVertexShaderVersion;		// 494
	UInt8				unk498;							// 498
	UInt8				pad499[3];						// 499
	UInt32				m_uiResetCounter;				// 49C
	UInt8				m_bDeviceLost;					// 4A0
	UInt8				pad4A1[3];						// 4A1
	NiPixelFormat		* m_aapkTextureFormats[4][22];	// 4A4 [TEXUSE_NUM][TEX_NUM]
	NiPixelFormat		* m_apkDefaultTextureFormat[4]; // 604 [TEXUSE_NUM]
	NiPixelData			* m_aspDefaultTextureData[4];	// 614 [TEXUSE_NUM]
	UInt32				m_eReplacementDataFormat;		// 624
	NiRenderTargetGroup * m_spDefaultRenderTargetGroup; // 628
	NiRenderTargetGroup * m_pkCurrRenderTargetGroup;	// 62C
	NiRenderTargetGroup * m_pkCurrOnscreenRenderTargetGroup; // 630
	NiTPointerMap<HWND, NiRenderTargetGroup*>	m_kOnscreenRenderTargetGroups; // 634 NiTPointerMap<HWND, NiRenderTargetGroupPtr>
	UInt32				m_uiMaxNumRenderTargets;		// 644
	bool				m_bIndependentBitDepths;		// 648
	bool				m_bMRTPostPixelShaderBlending;	// 649
	UInt8				pad64A[2];						// 64A
	UInt32				unk64C;							// 64C
	UInt32				unk650;							// 650
	UInt32				unk654;							// 654
	UInt32				unk658;							// 658
	UInt32				unk65C;							// 65C
	NiTPointerMap<NiRenderedTexture*, void*> m_kRenderedTextures;			// 660 NiTPointerMap<NiRenderedTexture*, NiDX9RenderedTextureData*>
	NiTPointerMap<NiRenderedCubeMap*, void*> m_kRenderedCubeMaps;			// 670 NiTPointerMap<NiRenderedCubeMap*, NiDX9RenderedCubeMapData*>
	UInt32				unk680;							// 680
	UInt32				unk684;							// 684
	UInt32				unk688;							// 688
	UInt32				unk68C;							// 68C
	D3DMATRIX			m_kD3DMat;						// 690 D3DXALIGNEDMATRIX
	D3DMATRIX			m_kD3DView;						// 6D0
	D3DMATRIX			m_kD3DProj;						// 710
	D3DMATRIX			unkMatrix;						// 750
	D3DMATRIX			m_kInvView;						// 790
	NiPoint2			* m_pkScreenTextureVerts;		// 7D0
	NiColorA			* m_pkScreenTextureColors;		// 7D4
	NiPoint2			* m_pkScreenTextureTexCoords;	// 7D8
	UInt16				m_usNumScreenTextureVerts;		// 7DC
	UInt16				pad7DE;
	UInt16				* m_pusScreenTextureIndices;	// 7E0
	UInt32				m_uiNumScreenTextureIndices;	// 7E4
	UInt32				m_uiCreationWidth;				// 7E8
	UInt32				m_uiCreationHeight;				// 7EC
	UInt32				m_uiCreationUseFlags;			// 7F0
	HWND				m_kCreationWndDevice;			// 7F4
	HWND				m_kCreationWndFocus;			// 7F8
	UInt32				m_uiCreationAdapter;			// 7FC
	UInt32				m_eCreationDesc;				// 800
	UInt32				m_eCreationFBFormat;			// 804
	UInt32				m_eCreationDSFormat;			// 808
	UInt32				m_eCreationPresentationInterval; // 80C
	UInt32				m_eCreationSwapEffect;			// 810
	UInt32				m_uiCreationFBMode;				// 814
	UInt32				m_uiCreationBackBufferCount;	// 818
	UInt32				m_uiCreationRefreshRate;		// 81C
	UInt32				unk820;							// 820
	UInt32				unk824;							// 824
	UInt32				unk828;							// 828
	UInt32				unk82C;							// 82C
	UInt32				unk830;							// 830
	UInt32				unk834;							// 834
	UInt32				unk838;							// 838
	UInt32				unk83C;							// 83C
	UInt32				unk840;							// 840
	UInt32				unk844;							// 844
	UInt32				unk848;							// 848
	UInt32				unk84C;							// 84C
	UInt32				unk850;							// 850
	UInt32				unk854;							// 854
	UInt32				unk858;							// 858
	UInt32				unk85C;							// 85C
	UInt32				unk860;							// 860
	NiTMap<D3DFORMAT, NiPixelFormat *>	m_kDepthStencilFormats; // 864 NiTMap<D3DFormat, NiPixelFormat *>
	UInt32				unk874;							// 874
	UInt32				unk878;							// 878
	UInt32				unk87C;							// 87C

	static NiDX9Renderer * GetSingleton();
};

STATIC_ASSERT(offsetof(NiDX9Renderer, m_pkD3DDevice9) == 0x38);

// Unknown class name, No RTTI
class NiRenderManager
{
public:
	static NiRenderManager * GetSingleton();

	MEMBER_FN_PREFIX(NiRenderManager); 
	DEFINE_MEMBER_FN(CreateRenderTarget, BSRenderTargetGroup *, 0x00C91800, NiDX9Renderer * dx9Renderer, UInt32 type, UInt32 unk1, UInt32 unk2);

	// 00C903B0 - CreateRenderTargetGroup
	// 11539D8
	// 115A2E8 - NiDX9RenderedTextureData
};

// unk2 = 1, all others zero
typedef BSRenderTargetGroup * (* _CreateRenderTargetGroup)(BSFixedString * linkName, UInt32 width, UInt32 height, NiTexture::FormatPrefs * format, UInt32 unk1, UInt8 unk2, UInt32 unk3, UInt32 unk4, UInt32 unk5, UInt32 unk6, UInt32 unk7);
extern const _CreateRenderTargetGroup CreateRenderTargetGroup;