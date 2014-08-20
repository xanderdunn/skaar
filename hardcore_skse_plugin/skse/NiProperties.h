#pragma once

#include "skse/NiObjects.h"
#include "skse/NiMaterial.h"

class NiProperty : public NiObjectNET
{
public:
	virtual ~NiProperty();

	enum
	{
		kTypeAlpha = 0,
		kTypeShade = 1
	};

	virtual SInt32 GetType(void);
};

class NiAlphaProperty : public NiProperty
{
public:
	virtual ~NiAlphaProperty();

	UInt16	alphaFlags;			// 18
	UInt16	alphaThreshold;		// 1A
};

class NiShadeProperty : public NiProperty
{
public:
	virtual ~NiShadeProperty();

	virtual void Unk_23(void);
};

// 114EB18
class BSShaderProperty : public NiShadeProperty
{
public:
	virtual ~BSShaderProperty();

	virtual void Unk_24(UInt32 unk1, UInt32 unk2, UInt32 unk3);
	virtual void Unk_25(UInt32 unk1, UInt32 unk2, UInt32 unk3); // ??
	virtual UInt32 Unk_26(void); // ret 1
	virtual void Unk_27(UInt32 unk1); // ??
	virtual bool Unk_28(UInt32 unk1); // ??
	virtual void Unk_29(float unk1); // set (material + 0x54)
	virtual float Unk_2A(void); // get (material + 0x54)
	virtual void Unk_2B(void); // ??
	virtual void * Unk_2C(void); // ??
	virtual UInt32 Unk_2D(void); // ??
	virtual UInt32 Unk_2E(void); // type of somekind?
	virtual UInt32 Unk_2F(void); // ??
	virtual NiSourceTexture * Unk_30(void); // Gets first source texture from material
	virtual UInt32 Unk_31(void); // ??
	virtual bool Unk_32(void); // ??
	virtual bool Unk_33(UInt32 unk1, UInt32 unk2, UInt32 unk3); // Texture related
	virtual UInt32 Unk_34(UInt32 unk1, UInt32 unk2, UInt32 unk3, UInt32 unk4, UInt32 unk5, UInt32 unk6);
	virtual UInt32 Unk_35(void); // Flags?
	virtual SInt32 Unk_36(void); // ??

	enum ShaderFlags1
	{
		kSLSF1_Specular						= 1 << 0,
		kSLSF1_Skinned						= 1 << 1,
		kSLSF1_Temp_Refraction				= 1 << 2,
		kSLSF1_Vertex_Alpha					= 1 << 3,
		kSLSF1_Greyscale_To_PaletteColor	= 1 << 4,
		kSLSF1_Greyscale_To_PaletteAlpha	= 1 << 5,
		kSLSF1_Use_Falloff					= 1 << 6,
		kSLSF1_Environment_Mapping			= 1 << 7,
		kSLSF1_Recieve_Shadows				= 1 << 8,
		kSLSF1_Cast_Shadows					= 1 << 9,
		kSLSF1_Facegen_Detail_Map			= 1 << 10,
		kSLSF1_Parallax						= 1 << 11,
		kSLSF1_Model_Space_Normals			= 1 << 12,
		kSLSF1_Non_Projective_Shadows		= 1 << 13,
		kSLSF1_Landscape					= 1 << 14,
		kSLSF1_Refraction					= 1 << 15,
		kSLSF1_Fire_Refraction				= 1 << 16,
		kSLSF1_Eye_Environment_Mapping		= 1 << 17,
		kSLSF1_Hair_Soft_Lighting			= 1 << 18,
		kSLSF1_Screendoor_Alpha_Fade		= 1 << 19,
		kSLSF1_Localmap_Hide_Secret			= 1 << 20,
		kSLSF1_FaceGen_RGB_Tint				= 1 << 21,
		kSLSF1_Own_Emit						= 1 << 22,
		kSLSF1_Projected_UV					= 1 << 23,
		kSLSF1_Multiple_Textures			= 1 << 24,
		kSLSF1_Remappable_Textures			= 1 << 25,
		kSLSF1_Decal						= 1 << 26,
		kSLSF1_Dynamic_Decal				= 1 << 27,
		kSLSF1_Parallax_Occlusion			= 1 << 28,
		kSLSF1_External_Emittance			= 1 << 29,
		kSLSF1_Soft_Effect					= 1 << 30,
		kSLSF1_ZBuffer_Test					= 1 << 31
	};

	enum ShaderFlags2
	{
		kSLSF2_ZBuffer_Write					= 1 << 0,
		kSLSF2_LOD_Landscape					= 1 << 1,
		kSLSF2_LOD_Objects						= 1 << 2,
		kSLSF2_No_Fade							= 1 << 3,
		kSLSF2_Double_Sided						= 1 << 4,
		kSLSF2_Vertex_Colors					= 1 << 5,
		kSLSF2_Glow_Map							= 1 << 6,
		kSLSF2_Assume_Shadowmask				= 1 << 7,
		kSLSF2_Packed_Tangent					= 1 << 8,
		kSLSF2_Multi_Index_Snow					= 1 << 9,
		kSLSF2_Vertex_Lighting					= 1 << 10,
		kSLSF2_Uniform_Scale					= 1 << 11,
		kSLSF2_Fit_Slope						= 1 << 12,
		kSLSF2_Billboard						= 1 << 13,
		kSLSF2_No_LOD_Land_Blend				= 1 << 14,
		kSLSF2_EnvMap_Light_Fade				= 1 << 15,
		kSLSF2_Wireframe						= 1 << 16,
		kSLSF2_Weapon_Blood						= 1 << 17,
		kSLSF2_Hide_On_Local_Map				= 1 << 18,
		kSLSF2_Premult_Alpha					= 1 << 19,
		kSLSF2_Cloud_LOD						= 1 << 20,
		kSLSF2_Anisotropic_Lighting				= 1 << 21,
		kSLSF2_No_Transparency_Multisampling	= 1 << 22,
		kSLSF2_Unused01							= 1 << 23,
		kSLSF2_Multi_Layer_Parallax				= 1 << 24,
		kSLSF2_Soft_Lighting					= 1 << 25,
		kSLSF2_Rim_Lighting						= 1 << 26,
		kSLSF2_Back_Lighting					= 1 << 27,
		kSLSF2_Unused02							= 1 << 28,
		kSLSF2_Tree_Anim						= 1 << 29,
		kSLSF2_Effect_Lighting					= 1 << 30,
		kSLSF2_HD_LOD_Objects					= 1 << 31
	};

	UInt32	unk18;	// 18
	UInt32	unk1C;	// 1C
	UInt32	shaderFlags1;	// 20
	UInt32	shaderFlags2;	// 24
	UInt32	unk28;	// 28
	UInt32	unk2C;	// 2C
	UInt32	unk30;	// 30
	UInt32	unk34;	// 34
	UInt32	unk38;	// 38
	BSLightingShaderMaterial	* material; // 3C
	UInt32	unk40;	// 40
};

STATIC_ASSERT(offsetof(BSShaderProperty, material) == 0x3C);

// 1154358
// 0x50
class BSEffectShaderProperty : public BSShaderProperty
{
public:
	virtual ~BSEffectShaderProperty();

	UInt32	unk44;	// 44
	UInt32	unk48;	// 48
	UInt32	unk4C;	// 4C
};

// 1153680
class BSLightingShaderProperty : public BSShaderProperty
{
public:
	virtual ~BSLightingShaderProperty();

	UInt32	unk44;	// 44
	UInt32	unk48;	// 48
	UInt32	unk4C;	// 4C
	UInt32	unk50;	// 50
	UInt32	unk54;	// 54
	UInt32	unk58;	// 58
	NiColor	* emissiveColor;	// 5C
	float	emissiveMultiple;	// 60
	UInt32	unk64;	// 64
	float	unk68;	// 68
	float	unk6C;	// 6C
	UInt32	unk70;	// 70
	UInt32	unk74;	// 74
	UInt32	unk78;	// 78
	UInt32	unk7C;	// 7C
	float	unk80;	// 80
	float	unk84;	// 84
	float	unk88;	// 88
	float	unk8C;	// 8C
	SInt32	unk90;	// 90 inited to -1
	SInt16	unk94;	// 94 inited to -1
	UInt8	unk96;	// 96 inited to 1
	UInt8	unk97;	// 97 inited to 2
	UInt32	unk98;	// 98
	UInt32	unk9C;	// 9C
	UInt32	unkA0;	// A0
	SInt32	unkA4;	// A4 inited to -1
	UInt32	unkA8;	// A8 inited to 0
	UInt8	unkAC;	// AC inited to 0
	UInt8	padAD[3];	// AD

	MEMBER_FN_PREFIX(BSLightingShaderProperty);

	// This function seems to fix weird lighting issues when creating armors internally
	// Not particularly sure what it does but it seems to mess around with a lot material flags
	DEFINE_MEMBER_FN(InitializeShader, UInt32, 0x00C89DD0, NiGeometry * geometry);
	DEFINE_MEMBER_FN(SetMaterial, UInt32, 0x00C6F920, BSLightingShaderMaterial * material, bool unk1); // unk1 usually 1
	DEFINE_MEMBER_FN(SetFlags, UInt64, 0x00C6EEA0, UInt8 unk1, UInt8 unk2);
	DEFINE_MEMBER_FN(HasFlags, bool, 0x0045C640, UInt8 flag); // 0x15 SkinShader? 0x0A Regular?
	DEFINE_MEMBER_FN(InvalidateMaterial, bool, 0x00454EB0);
	DEFINE_MEMBER_FN(InvalidateTextures, void, 0x00C89810, UInt32 unk1); // unk1 usually 0, called after material Releases textures
};
