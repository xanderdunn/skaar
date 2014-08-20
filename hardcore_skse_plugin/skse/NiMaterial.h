#pragma once

#include "skse/NiTextures.h"
#include "skse/GameTypes.h"

MAKE_NI_POINTER(BSTextureSet);

class BSShaderMaterial
{
public:
	virtual ~BSShaderMaterial();
	virtual BSShaderMaterial * Create(void);
	virtual void Copy(BSShaderMaterial * source); // Must be same type
	virtual bool Unk_03(void * unk1);
	virtual SInt32 Unk_04(void * unk1);
	virtual void * Unk_05(void);
	virtual UInt32 GetShaderType(void);
	virtual UInt32 Unk_07(void);	// Always seems to be 2

	enum {
		kShaderType_Default = 0,
		kShaderType_EnvironmentMap,
		kShaderType_GlowShader,
		kShaderType_Heightmap,
		kShaderType_FaceTint,
		kShaderType_SkinTint,
		kShaderType_HairTint,
		kShaderType_ParallaxOccMaterial,
		kShaderType_WorldMultitexture,
		kShaderType_WorldMap1,
		kShaderType_Unknown,
		kShaderType_MultilayerParallax
	};

	UInt32	unk04;	// 04 BSIntrusiveRefCounted?
	UInt32	unk08;	// 08 inited to 0
	UInt32	unk0C;	// 0C inited to 0
	UInt32	unk10;	// 10 inited to 0
	UInt32	unk14;	// 14 inited to 0
	float	unk18;	// 18 inited to 1.0
	float	unk1C;	// 1C inited to 1.0
	float	unk20;	// 20 inited to 1.0
	float	unk24;	// 24 inited to 1.0
	UInt32	unk28;	// 28 inited to -1 flags?
};

class BSShaderMaterialBase : public BSShaderMaterial
{
public:
	virtual ~BSShaderMaterialBase();

	virtual void SetTexture(UInt32 index, BSTextureSet * texture, SInt32 unk1);
	virtual void ReleaseTextures(void); // ReleaseRefs
	virtual void Unk_0A(UInt8 unk1, UInt8 unk2, UInt8 unk3, UInt8 unk4, UInt8 unk5, UInt32 unk6, UInt32 unk7); // AddRefs
	virtual void Unk_0B(void * unk1, UInt32 unk2);
	virtual void * Unk_0C(void * unk1);
	virtual void * Unk_0D(void * unk1);

	UInt32	unk2C;	// 2C flags?

};

class BSEffectShaderMaterial : public BSShaderMaterialBase
{
public:
	virtual ~BSEffectShaderMaterial();

	float	falloffStartAngle;			// 30
	float	falloffStopAngle;			// 34
	float	falloffStartOpacity;		// 38
	float	falloffStopOpacity;			// 3C
	NiColorA	emissiveColor;			// 40
	NiSourceTexture	* unk50;			// 50
	NiSourceTexture	* unk54;			// 54
	float	softFalloffDepth;			// 58
	float	emissiveMultiple;			// 5C
	BSFixedString	sourceTexture;		// 60
	BSFixedString	greyscaleTexture;	// 64
};

class BSLightingShaderMaterial : public BSShaderMaterialBase
{
public:
	virtual ~BSLightingShaderMaterial();

	float	unk30;
	float	unk34;
	float	unk38;
	NiSourceTexture	* diffuse;	// 3C inited to 0
	NiSourceTexture	* normalMap;	// 40 inited to 0
	NiSourceTexture	* heightMap;	// 44 inited to 0
	NiSourceTexture	* specular;	// 48 inited to 0
	UInt32	unk4C;				// 4C inited to 3
	BSTextureSetPtr	textureSet;		// 50 inited to 0
	float	alpha;				// 54 inited to 1.0
	float	unk58;				// 58 inited to 0
	float	glossiness;			// 5C inited to 1.0
	float	specularStrength;	// 60 inited to 1.0
	float	lightingEffect1;	// 64 inited to 0
	float	lightingEffect2;	// 68 inited to 0
	UInt32	unk6C;				// 6C inited to 0

	void SetTextureSet(BSTextureSet * textureSet);

	MEMBER_FN_PREFIX(BSLightingShaderMaterial);
	DEFINE_MEMBER_FN(CopyFrom, void, 0x00C97AC0, BSLightingShaderMaterial * other);
};

// vtbl - 011541CC
class BSMaskedShaderMaterial : public BSLightingShaderMaterial
{
public:
	virtual ~BSMaskedShaderMaterial();

	NiRenderedTexture * renderedTexture;	// 70 inited to 0
	NiSourceTexture * unk74;				// 74 inited to 0
	NiSourceTexture * unk78;				// 78 inited to 0

	//static BSMaskedShaderMaterial * CreateFromLightingShader(BSLightingShaderMaterial * source);
};

// vtbl - 01154204
class BSTintedShaderMaterial : public BSLightingShaderMaterial
{
public:
	virtual ~BSTintedShaderMaterial();

	NiColor	tintColor;	// 70

	// ctor - C98AB0
	// copyFrom - C97AC0
};

typedef BSShaderMaterialBase * (* _CreateShaderMaterial)(UInt32 shaderType);
extern const _CreateShaderMaterial CreateShaderMaterial;

STATIC_ASSERT(offsetof(BSMaskedShaderMaterial, renderedTexture) == 0x70);