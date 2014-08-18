#pragma once

#include "skse/NiObjects.h"
#include "skse/GameForms.h"
#include "skse/GameEvents.h"

// TESObject and children

// 14+
class TESObject : public TESForm
{
public:
	virtual UInt32	Unk_39(void);
	virtual bool	Unk_3A(void);
	virtual UInt32	Unk_3B(void);
	virtual bool	Unk_3C(void);
	virtual void	Unk_3D(UInt32 arg);
	virtual UInt32	Unk_3E(UInt32 arg0, UInt32 arg1);
	virtual void	Unk_3F(void * arg);
	virtual bool	Unk_40(void);
	virtual bool	Unk_41(void);
	virtual void	Unk_42(void);
	virtual void	Unk_43(UInt32 arg0, UInt32 arg1);
	virtual UInt32	Unk_44(void);
	virtual UInt32	Unk_45(void);
	virtual void *	Unk_46(void);
};

// 20
class TESBoundObject : public TESObject
{
public:
	struct Bound
	{
		UInt16	x;
		UInt16	y;
		UInt16	z;
	};

	virtual void	Unk_47(UInt32 arg);
	virtual UInt32	Unk_48(void);
	virtual UInt32	Unk_49(UInt32 arg);	// return Unk_3E(arg, 0);
										// model-related?
	virtual bool	Unk_4A(void);
	virtual bool	Unk_4B(void * arg0, BSString * dst);	// steal/take string
	virtual bool	Unk_4C(void * arg0, UInt8 arg1, UInt32 arg2, float arg3);
	virtual void	Unk_4D(UInt32 arg);
	virtual void	Unk_4E(UInt32 arg);
	virtual void	Unk_4F(void);
	virtual void	Unk_50(void);
	virtual void	Unk_51(void);

	Bound	bounds;		// 14
	Bound	bounds2;	// 1A
};

STATIC_ASSERT(sizeof(TESBoundObject) == 0x20);

// 2C
class BGSAcousticSpace : public TESBoundObject
{
public:
	enum { kTypeID = kFormType_AcousticSpace };

	TESForm	* loopingSound;	// 20
	TESForm	* soundRegion;	// 24
	TESForm	* reverbType;	// 28
};

// 4C
class BGSAddonNode : public TESBoundObject
{
public:
	enum { kTypeID = kFormType_AddonNode };

	// parents
	TESModelTextureSwap	texSwap;	// 20

	// members
	UInt32	unk3C;		// 3C
	UInt32	unk40;		// 40
	UInt8	unk44[4];	// 44
	UInt32	unk48;		// 48
};

// 40
class BGSArtObject : public TESBoundObject
{
public:
	enum { kTypeID = kFormType_Art };

	// parents
	TESModelTextureSwap	texSwap;	// 20

	// members
	UInt32	unk3C;	// 3C
};

// 34
class BGSCloudClusterForm : public TESBoundObject
{
public:
	// parents
	TESModel	model;	// 20
};

// 38
class BGSDualCastData : public TESBoundObject
{
public:
	enum { kTypeID = kFormType_DualCastData };

	// 18
	struct Data
	{
		UInt32	unk00;	// 00
		UInt32	unk04;	// 04
		UInt32	unk08;	// 08
		UInt32	unk0C;	// 0C
		UInt32	unk10;	// 10
		UInt32	unk14;	// 14
	};

	Data	unk20;	// 20
};

// 8C
class BGSExplosion : public TESBoundObject
{
public:
	enum { kTypeID = kFormType_Explosion };

	// parents
	TESFullName			fullName;		// 20
	TESModel			model;			// 28
	TESEnchantableForm	enchantment;	// 3C
	BGSPreloadable		preloadable;	// 4C
	TESImageSpaceModifiableForm	imageSpaceModifier;	// 50

	// members

	// 34
	struct Data
	{
		UInt32	unk00;	// 00
		UInt32	unk04;	// 04
		UInt32	unk08;	// 08
		UInt32	unk0C;	// 0C
		UInt32	unk10;	// 10
		UInt32	unk14;	// 14
		UInt32	unk18;	// 18
		UInt32	unk1C;	// 1C
		UInt32	unk20;	// 20
		UInt32	unk24;	// 24
		UInt32	unk28;	// 28
		UInt32	unk2C;	// 2C
		UInt32	unk30;	// 30
	};

	Data	data;	// 58
};

// 70
class BGSHazard : public TESBoundObject
{
public:
	enum { kTypeID = kFormType_Hazard };

	// parents
	TESFullName		fullName;		// 20
	TESModel		model;			// 28
	BGSPreloadable	preloadable;	// 3C
	TESImageSpaceModifiableForm	imageSpaceModifier;	// 40

	// members

	// 28
	struct Data
	{
		UInt32	unk00;	// 00
		UInt32	unk04;	// 04
		UInt32	unk08;	// 08
		UInt32	unk0C;	// 0C
		UInt32	unk10;	// 10
		UInt32	unk14;	// 14
		UInt32	unk18;	// 18
		UInt32	unk1C;	// 1C
		UInt32	unk20;	// 20
		UInt32	unk24;	// 24
	};

	Data	unk48;	// 48
};

// 44
class BGSIdleMarker : public TESBoundObject
{
public:
	enum { kTypeID = kFormType_IdleMarker };

	// parents
	TESModel			model;	// 20
	BGSIdleCollection	idle;	// 34
};

// 64
class BGSNote : public TESBoundObject
{
public:
	enum { kTypeID = kFormType_Note };

	// parents
	TESModel		model;		// 20
	TESFullName		fullName;	// 34
	TESIcon			icon;		// 3C
	BGSPickupPutdownSounds	sounds;	// 44

	// members
	struct Entry
	{
		void	* data;
		Entry	* next;
	};

	UInt32	unk50;		// 50
	UInt32	unk54;		// 54
	Entry	unk58;		// 58
	UInt8	unk60;		// 60
	UInt8	unk61;		// 61
	UInt8	pad62[2];	// 62
};

// BC
class BGSProjectile : public TESBoundObject
{
public:
	enum { kTypeID = kFormType_Projectile };

	// parents
	TESFullName		fullName;		// 20
	TESModel		model;			// 28
	BGSPreloadable	preloadable;	// 3C
	BGSDestructibleObjectForm	destructible;	// 40

	// members

	// 5C
	struct Data
	{
		UInt32	unk00;				// 00
		UInt32	unk04;				// 04
		float	unk08;				// 08
		float	unk0C;				// 0C
		UInt32	unk10;				// 10
		UInt32	unk14;				// 14
		UInt32	unk18;				// 18
		UInt32	unk1C;				// 1C
		UInt32	unk20;				// 20
		UInt32	unk24;				// 24
		UInt32	unk28;				// 28
		UInt32	unk2C;				// 2C
		float	unk30;				// 30
		UInt32	unk34;				// 34
		UInt32	unk38;				// 38
		UInt32	unk3C;				// 3C
		UInt32	unk40;				// 40
		UInt32	unk44;				// 44
		float	unk48;				// 48
		UInt32	unk4C;				// 4C
		float	relaunchInterval;	// 50
		UInt32	unk54;				// 54
		UInt32	unk58;				// 58
	};

	Data		data;		// 48
	TESModel	modelA4;	// A4
	UInt32		unkB8;		// B8
};

// 3C
class BGSStaticCollection : public TESBoundObject
{
public:
	enum { kTypeID = kFormType_StaticCollection };

	// parents
	TESModelTextureSwap	texSwap;	// 20
};

class BSShaderTextureSet;

// 08
class BSTextureSet : public NiObject
{
public:
	virtual const char * GetTexturePath(UInt32 index);
	virtual void	SetTexture(UInt32 index, NiSourceTexture * texture);
	virtual void	SetTexturePath(UInt32 index, const char * path);

	enum
	{
		kTextureDiffuse = 0,
		kTextureNormal,
		kTextureEnvironmentMask,
		kTextureSubsurfaceTint = kTextureEnvironmentMask,
		kTextureGlowMap,
		kTextureDetailMap = kTextureGlowMap,
		kTextureHeight,
		kTextureEnvironment,
		kTextureMultilayer,
		kTextureBacklightMask,
		kTextureSpecular = kTextureBacklightMask,
		kTextureUnused08,
		kNumTextures = 9
	};
};

// 2C
class BSShaderTextureSet : public BSTextureSet
{
public:
	UInt32	unk08;
	UInt32	unk0C;
	UInt32	unk10;
	UInt32	unk14;
	UInt32	unk18;
	UInt32	unk1C;
	UInt32	unk20;
	UInt32	unk24;
	UInt32	unk28;

	static BSShaderTextureSet * Create();

	MEMBER_FN_PREFIX(BSShaderTextureSet);
	DEFINE_MEMBER_FN(ctor, BSShaderTextureSet *, 0x00501EA0);
};
STATIC_ASSERT(sizeof(BSShaderTextureSet) == 0x2C);

// D0
class BGSTextureSet : public TESBoundObject
{
public:
	enum { kTypeID = kFormType_TextureSet };

	// parents
	BSTextureSet	textureSet;	// 20

	// members

	// 0C
	struct Data
	{
		UInt32	unk00;	// 00
		UInt32	unk04;	// 04
		UInt32	unk08;	// 08
	};

	enum
	{
		kTextureDiffuse = 0,
		kTextureNormal,
		kTextureEnvironmentMask,
		kTextureSubsurfaceTint = kTextureEnvironmentMask,
		kTextureGlowMap,
		kTextureDetailMap = kTextureGlowMap,
		kTextureHeight,
		kTextureEnvironment,
		kTextureMultilayer,
		kTextureBacklightMask,
		kTextureSpecular = kTextureBacklightMask,
		kNumTextures
	};

	TESTexture		texturePaths[kNumTextures];	// 28
	UInt32			unk68;						// 68
	UInt32			unk6C;						// 6C
	Data			unk70[kNumTextures];		// 70
};

STATIC_ASSERT(sizeof(BGSTextureSet) == 0xD0);

// 50
class MagicItem : public TESBoundObject
{
public:
	virtual UInt32	Unk_52(void);	// pure
	virtual void	Unk_53(UInt32 arg);
	virtual UInt32	Unk_54(void);	// pure
	virtual void	Unk_55(UInt32 arg);
	virtual UInt32	Unk_56(void);	// pure
	virtual bool	Unk_57(UInt32 arg);
	virtual float	Unk_58(void);
	virtual float	Unk_59(void);
	virtual bool	Unk_5A(void);
	virtual bool	Unk_5B(void);
	virtual bool	Unk_5C(void);
	virtual bool	Unk_5D(void);
	virtual bool	Unk_5E(void);
	virtual bool	Unk_5F(UInt32 arg);
	virtual bool	Unk_60(void);
	virtual bool	Unk_61(void);
	virtual void	Unk_62(UInt32 arg0, UInt32 arg1);
	virtual float	Unk_63(void);
	virtual UInt32	Unk_64(void);
	virtual UInt32	Unk_65(void);
	virtual bool	Unk_66(void);
	virtual UInt32	Unk_67(void);	// pure, returns char code
	virtual void	Unk_68(MagicItem * src);	// pure, copy data if possible?
	virtual void	Unk_69(UInt32 arg0, UInt32 arg1);	// loading-related
	virtual void	Unk_6A(UInt32 arg);
	virtual void *	Unk_6B(void);	// pure, returns data
	virtual void *	Unk_6C(void);	// pure, returns data
	virtual UInt32	Unk_6D(void);	// pure, return size of data
	virtual void	Unk_6E(void);	// pure, byteswap?

	// parents
	TESFullName		fullName;	// 20
	BGSKeywordForm	keyword;	// 28

	// members
	struct EffectItem
	{
		float	magnitude;		// 00
		UInt32	area;			// 04
		UInt32	duration;		// 08
		EffectSetting* mgef;	// 0C
		float	cost;			// 10 - ?
		UInt32	unk14;			// 14 - ?

		EffectItem()
		{
			magnitude = 0;
			area = 0;
			duration = 0;
			mgef = NULL;
			cost = 0.0;
			unk14 = 0;
		}
	};

	tArray<EffectItem*> effectItemList;	// 34
	UInt32				hostile;	// 40
	EffectSetting*		unk44;	// 44
	UInt32				unk48;	// 48
	UInt32				unk4C;	// 4C

	MEMBER_FN_PREFIX(MagicItem);
	DEFINE_MEMBER_FN(GetCostliestEffectItem, EffectItem *, 0x00407860, int arg1, bool arg2);
	DEFINE_MEMBER_FN(GetEffectiveMagickaCost, double, 0x00406EF0, Character* caster);
};

STATIC_ASSERT(sizeof(MagicItem) == 0x50);

// C0
class AlchemyItem : public MagicItem
{
public:
	enum { kTypeID = kFormType_Potion };

	enum
	{
		kFlag_ManualCalc =	0x00000001,
		kFlag_Food =		0x00000002,
		kFlag_Medicine =	0x00010000,
		kFlag_Poison =		0x00020000,
	};

	// parents
	TESModelTextureSwap	texSwap;		// 50
	TESIcon				icon;			// 6C
	BGSMessageIcon		messageIcon;	// 74
	TESWeightForm		weight;			// 80
	BGSEquipType		equipType;		// 88
	BGSDestructibleObjectForm	destructibleObj;	// 90
	BGSPickupPutdownSounds		sounds;	// 98

	// members

	// 14
	struct Data
	{
		UInt32	value;	// 00
		UInt32	flags;	// 04
		UInt32	unk08;	// 08 addiction (legacy?)
		UInt32	unk0C;	// 0C addiction chance (legacy?)
		BGSSoundDescriptorForm *	useSound;	// 10
	};

	Data	itemData;	// A4
	TESIcon	unkB8;		// B8

	bool IsFood() { return (itemData.flags & kFlag_Food) != 0; }
	bool IsPoison() { return (itemData.flags & kFlag_Poison) != 0; }
};

// 74
class EnchantmentItem : public MagicItem
{
public:
	enum { kTypeID = kFormType_Enchantment };

	enum
	{
		kFlag_ManualCalc =	0x00000001,
	};

	// 24
	struct Data
	{
		struct Calculations
		{
			UInt32	cost;	// 00
			UInt32	flags;	// 04
		};

		Calculations	calculations;	// 00
		UInt32	unk08;					// 08
		UInt32	unk0C;					// 0C
		UInt32	deliveryType;			// 10
		UInt32	unk14;					// 14
		UInt32	unk18;					// 18
		EnchantmentItem	* baseEnchantment;	// 1C
		BGSListForm		* restrictions;	// 20
	};

	Data	data;	// 50
};

// AC
class IngredientItem : public MagicItem
{
public:
	enum { kTypeID = kFormType_Ingredient };

	// parents
	TESModelTextureSwap	texSwap;	// 50
	TESIcon			icon;		// 6C
	TESWeightForm	weight;		// 74
	BGSEquipType	equipType;	// 7C
	BGSDestructibleObjectForm	destructible;	// 84
	BGSPickupPutdownSounds		sounds;			// 8C
	TESValueForm	value;		// 98

	// members
	struct DataA0
	{
		UInt32 unk00; // 00
		UInt32 unk04; // 04
	};
	DataA0 unkA0;

	// ahzaab 8-25-13
	enum   // type - these are flags
	{
		kType_NoEffect =     0,
		kType_FirstEffect =  1 << 0,
		kType_SecondEffect = 1 << 1,
		kType_ThirdEffect =  1 << 2,
		kType_FourthEffect = 1 << 3
	};
	UInt8  knownEffects;        //The lower nibble contains the known effects, the upper nibble is unknown

	struct DataA9
	{
		UInt8  unk00; // 00
		UInt16 unk01; // 01
	};

	DataA9 unkA9;
};

class Character;

// 90
class SpellItem : public MagicItem
{
public:
	enum { kTypeID = kFormType_Spell };

	// parents
	BGSEquipType			equipType;		// 50
	BGSMenuDisplayObject	dispObj;		// 58
	TESDescription			description;	// 60

	// members

	enum
	{
		kTypeSpell = 0,
		kTypeDisease,
		kTypePower,
		kTypeLesserPower,
		kTypeAbility,
		kTypePoison,
		kTypeAddition,
		kTypeVoice
	};

	// 24
	struct Data
	{
		struct Data0
		{
			UInt32	cost;	// 00
			UInt32	flags;	// 04
		};

		Data0	unk00;	// 00
		UInt32	type;	// 08
		float	castTime;	// 0C
		UInt32	castType;	// 10
		UInt32	unk14;	// 14
		UInt32	unk18;	// 18
		UInt32	unk1C;	// 1C
		BGSPerk*	spellPerk;	// 20
	};

	Data	data;	// 6C

	UInt32	GetMagickaCost() { return data.unk00.cost; }
};

// D0
class ScrollItem : public SpellItem
{
public:
	enum { kTypeID = kFormType_ScrollItem };

	// parents
	TESModelTextureSwap			texSwap;		// 90
	BGSDestructibleObjectForm	destructible;	// AC
	BGSPickupPutdownSounds		sounds;			// B4
	TESWeightForm				weight;			// C0
	TESValueForm				value;			// C8
};

// 9C
class TESAmmo : public TESBoundObject
{
public:
	enum { kTypeID = kFormType_Ammo };

	// parents
	TESFullName			fullName;		// 20
	TESModelTextureSwap	texSwap;		// 28
	TESIcon				icon;			// 44
	BGSMessageIcon		messageIcon;	// 4C
	TESValueForm		value;			// 58
	BGSDestructibleObjectForm	destructible;	// 50
	BGSPickupPutdownSounds		sounds;			// 68
	TESDescription		description;	// 74
	BGSKeywordForm		keyword;		// 80

	// members

	// 0C
	struct AmmoSettings
	{
		BGSProjectile	* projectile;
		UInt32			flags;
		float			damage;
	};

	enum {
		kIgnoreNormalResist = (1 << 0),
		kNotPlayable		= (1 << 1),
		kNotBolt			= (1 << 2)
	};

	bool isBolt() { return (settings.flags & kNotBolt) != kNotBolt; }
	bool isPlayable() { return (settings.flags & kNotPlayable) != kNotPlayable; }

	AmmoSettings		settings;	// 8C
	StringCache::Ref	unk98;	// 98
};

// 20
class TESBoundAnimObject : public TESBoundObject
{
public:
};

// C0
class TESActorBase : public TESBoundAnimObject
{
public:
	// parents
	TESActorBaseData	actorData;	// 20
	TESContainer		container;	// 58
	TESSpellList		spellList;	// 64
	TESAIForm			aiForm;		// 6C
	TESFullName			fullName;	// 84
	ActorValueOwner				actorValueOwner;	// 8C
	BGSDestructibleObjectForm	destructible;		// 90
	BGSSkinForm			skinForm;	// 98
	BGSKeywordForm		keyword;	// A0
	BGSAttackDataForm	attackData;	// AC
	BGSPerkRankArray	perkRanks;	// B4

	virtual bool			Unk_52(void);
	virtual UInt32			Unk_53(void);
	virtual void			Unk_54(UInt32 arg);
	virtual TESActorBase *	Unk_55(void);
};
STATIC_ASSERT(offsetof(TESActorBase, keyword) == 0xA0);
STATIC_ASSERT(sizeof(TESActorBase) == 0xC0);

class BSFaceGenNiNode;

// 164
class TESNPC : public TESActorBase
{
public:
	enum { kTypeID = kFormType_NPC };

	struct FaceMorphs
	{
		enum
		{
			kNumOptions =	19,
			kNumPresets =	4,
		};
		
		float	option[kNumOptions];
		UInt32	presets[kNumPresets];
	};

	struct HeadData {
		BGSColorForm * hairColor;
		BGSTextureSet * headTexture;	// Only seems to apply to the player
	};


	// parents
	TESRaceForm		race;	// 0C0
	BGSOverridePackCollection	overridePacks;	// 0C8
	BSTEventSink <void *>	menuOpenCloseEvent;	// 0DC - MenuOpenCloseEvent

	// members @0E0
	UInt8		unk0E0[0x12];	// 0E0 - init'd to 5
	UInt8		unk0F2[0x12];	// 0F2 - init'd to 0

	UInt8		pad104[0x10A - 0x104];	// 104

	UInt16		unk10A;			// 10A
	TESClass*	npcClass;		// 10C

	HeadData	* headData;		// 110
	UInt32		unk114;			// 114
	TESCombatStyle*	combatStyle;// 118
	UInt32		unk11C;			// 11C
	UInt32		unk120;			// 120
	TESNPC		* nextTemplate;	// 124
	float		height;			// 128
	float		weight;			// 12C

	UInt32		pad130;			// 130
	
	StringCache::Ref	shortName;	// 134
	TESObjectARMO*		skinFar;	// 138
	BGSOutfit*	defaultOutfit;		// 13C
	BGSOutfit*	sleepOutfit;		// 140
	UInt32		unk144;			// 144
	TESFaction*	faction;		// 148
	BGSHeadPart ** headparts;	// 14C
	UInt8		numHeadParts;	// 150
	UInt8		unk151;			// 151
	UInt8		unk152;			// 152
	UInt8		unk153;			// 153
	struct Color { // 797979 Transparent
		UInt8   red, green, blue; // 154 - 156 - Skin Color
	} color;
	UInt8		pad157;			// 157
	UInt32		unk158;			// 158 // Relationships?

	FaceMorphs	* faceMorph;	// 15C
	UInt32		unk160;			// 160

	MEMBER_FN_PREFIX(TESNPC);
	//DEFINE_MEMBER_FN(GetHeadPartByType, BGSHeadPart *, 0x00561270, UInt32);
	DEFINE_MEMBER_FN(GetSex, char, 0x0055B510);
	DEFINE_MEMBER_FN(HasOverlays, bool, 0x005681C0);

	// Swaps a headPart of the same type as target with target
	DEFINE_MEMBER_FN(ChangeHeadPart, void, 0x00567CE0, BGSHeadPart * target);

	struct MorphAction {
		BSFaceGenNiNode * faceNode;
		TESNPC * npc;
		const char ** morphName;
		float	value;
	};
	
	// Applies a morph to all parts of a head
	DEFINE_MEMBER_FN(ApplyMorph, void, 0x005A4870, MorphAction * morphAction);

	// Updates the neck seam when weight changed
	DEFINE_MEMBER_FN(UpdateNeck, void, 0x00567C30, BSFaceGenNiNode * faceNode);

	// Computes RGB SkinTone from RGBA TintMask
	DEFINE_MEMBER_FN(SetSkinFromTint, void, 0x005643C0, NiColorA * result, TintMask * tintMask, UInt32 compute, UInt32 unk1);

	void SetFaceTexture(BGSTextureSet * textureSet);
	void SetHairColor(BGSColorForm * hairColor);

	BGSHeadPart * GetHeadPartByType(UInt32 type);
	BGSHeadPart * GetHeadPartOverlayByType(UInt32 type);
	BGSHeadPart * GetCurrentHeadPartByType(UInt32 type);
	TESNPC * GetRootTemplate();
};

STATIC_ASSERT(sizeof(TESNPC) == 0x164);

// 50
class TESLevCharacter : public TESBoundAnimObject
{
public:
	enum { kTypeID = kFormType_LeveledCharacter };

	TESLeveledList		leveledList;	// 20
	TESModelTextureSwap	texSwap;		// 34
};

// 6C
class TESObjectACTI : public TESBoundAnimObject
{
public:
	enum { kTypeID = kFormType_Activator };

	// parents
	TESFullName					fullName;		// 20
	TESModelTextureSwap			texSwap;		// 28
	BGSDestructibleObjectForm	destructible;	// 44
	BGSOpenCloseForm			openClose;		// 4C
	BGSKeywordForm				keyword;		// 50

	// members
	UInt32	unk5C;		// 5C
	UInt32	unk60;		// 60
	UInt32	unk64;		// 64
	UInt16	unk68;		// 68
	UInt8	pad6A[2];	// 6A
};

// 74
class BGSTalkingActivator : public TESObjectACTI
{
public:
	enum { kTypeID = kFormType_TalkingActivator };

	void			* unk6C;	// 6C
	BGSVoiceType	* unk70;	// 70
};

// 7C
class TESFlora : public TESObjectACTI
{
public:
	enum { kTypeID = kFormType_Flora };

	// parents
	TESProduceForm	produce;	// 6C
};

// 84
class TESFurniture : public TESObjectACTI
{
public:
	enum { kTypeID = kFormType_Furniture };

	// 4
	struct Data78
	{
		UInt8	unk0;		// 0
		UInt8	unk1;		// 1
		UInt8	pad2[2];	// 2
	};

	UnkArray	unk6C;
	Data78	unk78;	// 78
	UInt32	unk7C;	// 7C
	UInt32	unk80;	// 80
};

// 74
class TESObjectCONT : public TESBoundAnimObject
{
public:
	enum { kTypeID = kFormType_Container };

	// parents
	TESContainer				container;		// 20
	TESFullName					fullName;		// 2C
	TESModelTextureSwap			texSwap;		// 34
	TESWeightForm				weight;			// 50
	BGSDestructibleObjectForm	destructible;	// 58
	BGSOpenCloseForm			openClose;		// 60

	// members
	UInt32	unk64;		// 64
	UInt32	unk68;		// 68
	UInt32	unk6C;		// 6C
	UInt8	unk70;		// 70
	UInt8	pad71[3];	// 71
};

// 70
class TESObjectDOOR : public TESBoundAnimObject
{
public:
	enum { kTypeID = kFormType_Door };

	// parents
	TESFullName					fullName;		// 20
	TESModelTextureSwap			texSwap;		// 28
	BGSDestructibleObjectForm	destructible;	// 44
	BGSOpenCloseForm			openClose;		// 4C

	// members
	UInt32	unk50;		// 50
	UInt32	unk54;		// 54
	UInt32	unk58;		// 58
	UInt32	unk5C;		// 5C
	UInt8	unk60;		// 60
	UInt8	pad61[3];	// 61
	UnkArray	unk64;	// 64
};

// B4
class TESObjectLIGH : public TESBoundAnimObject
{
public:
	enum { kTypeID = kFormType_Light };

	// parents
	TESFullName					fullName;		// 20
	TESModelTextureSwap			texSwap;		// 28
	TESIcon						icon;			// 44
	BGSMessageIcon				messageIcon;	// 4C
	TESWeightForm				weight;			// 58
	TESValueForm				value;			// 60
	BGSDestructibleObjectForm	destructible;	// 68
	BGSEquipType				equipType;		// 70

	// members (78)

	// 28
	struct Data78
	{
		UInt32	unk00;	// 00
		UInt32	unk04;	// 04
		UInt32	unk08;	// 08
		UInt32	unk0C;	// 0C
		UInt32	unk10;	// 10
		float	unk14;	// 14 - 90
		float	unk18;	// 18 - .001
		UInt32	unk1C;	// 1C
		UInt32	unk20;	// 20
		UInt32	unk24;	// 24
	};

	struct DataA8
	{
		UInt32	unk0;	// 0
		UInt32	unk4;	// 4
		UInt32	unk8;	// 8
	};

	Data78	unk78;	// 78
	float	unkA0;	// A0 - 1
	UInt32	unkA4;	// A4
	DataA8	unkA8;	// A8
};

STATIC_ASSERT(sizeof(TESObjectLIGH) == 0xB4);

// 2C
class TESSound : public TESBoundAnimObject
{
public:
	enum { kTypeID = kFormType_Sound };

	BGSSoundDescriptorForm	* descriptor;	// 20
};

// 54
class TESGrass : public TESBoundObject
{
public:
	enum { kTypeID = kFormType_Grass };

	virtual UInt8	GetUnk34(void);				// return unk34;
	virtual bool	SetUnk34(UInt8 data);		// if(data > 100) return false;
												// unk34 = data; return true;
	virtual UInt8	GetUnk35(void);				// return unk35;
	virtual bool	SetUnk35(UInt8 data);		// if(data > 90) return false;
												// if(data > unk36) return false;
												// unk35 = data; return true;
	virtual UInt8	GetUnk36(void);				// return unk36;
	virtual bool	SetUnk36(UInt8 data);		// if(data > 90) return false;
												// if(data < unk35) return false;
												// unk36 = data; return true;
	virtual float	GetUnk35_Float(void);		// return unk35 * (M_PI / 180);
	virtual float	GetUnk36_Float(void);		// return unk36 * (M_PI / 180);
	virtual UInt16	GetUnk38(void);				// return unk38;
	virtual void	SetUnk38(UInt16 data);		// unk38 = data;
	virtual UInt32	GetUnk3C(void);				// return unk3C;
	virtual void	SetUnk3C(UInt32 data);		// unk3C = data;
	virtual float	GetUnk40(void);				// return unk40;
	virtual bool	SetUnk40(float data);		// if((data < 0) || (data > 512)) return false;
												// unk40 = data; return true;
	virtual float	GetUnk44(void);				// return unk44;
	virtual bool	SetUnk44(float data);		// if((data < 0) || (data > 1)) return false;
												// unk44 = data; return true;
	virtual float	GetUnk48(void);				// return unk48;
	virtual bool	GetUnk48(float data);		// if((data < 0) || (data > 1)) return false;
												// unk48 = data; return true;
	virtual float	GetUnk4C(void);				// return unk4C;
	virtual bool	SetUnk4C(float data);		// if(data <= 0) return false;
												// unk4C = data; return true;
	virtual bool	GetUnk50_01(void);			// return (unk50 & 1) != 0;
	virtual void	SetUnk50_01(UInt8 data);	// if(data) unk50 |= 1; else unk50 &= ~1;
	virtual bool	GetUnk50_02(void);			// return (unk50 & 2) != 0;
	virtual void	SetUnk50_02(UInt8 data);	// if(data) unk50 |= 2; else unk50 &= ~2;
	virtual bool	GetUnk50_04(void);			// return (unk50 & 4) != 0;
	virtual void	SetUnk50_04(UInt8 data);	// if(data) unk50 |= 4; else unk50 &= ~4;

	// parents
	TESModel	model;		// 20

	// members
	UInt8		unk34;		// 34
	UInt8		unk35;		// 35 - degrees
	UInt8		unk36;		// 36 - degrees
	UInt8		pad37;		// 37
	UInt16		unk38;		// 38
	UInt8		pad3A[2];	// 3A
	UInt32		unk3C;		// 3C
	float		unk40;		// 40
	float		unk44;		// 44
	float		unk48;		// 48
	float		unk4C;		// 4C
	UInt8		unk50;		// 50
	UInt8		pad51[3];	// 51
};

// 34
class TESLevItem : public TESBoundObject
{
public:
	enum { kTypeID = kFormType_LeveledItem };

	// parents
	TESLeveledList	leveledList;	// 20
};

// 34
class TESLevSpell : public TESBoundObject
{
public:
	enum { kTypeID = kFormType_LeveledSpell };

	// parents
	TESLeveledList	leveledList;	// 20
};

class TESObjectARMA;

// 124
class TESObjectARMO : public TESBoundObject
{
public:
	enum { kTypeID = kFormType_Armor };

	// parents
	TESFullName					fullName;		// 020
	TESRaceForm					race;			// 028
	TESEnchantableForm			enchantable;	// 030
	TESValueForm				value;			// 03C
	TESWeightForm				weight;			// 044
	BGSDestructibleObjectForm	destructible;	// 04C
	BGSPickupPutdownSounds		pickupSounds;	// 054
	TESBipedModelForm			bipedModel;		// 060
	BGSEquipType				equipType;		// 0D8
	BGSBipedObjectForm			bipedObject;	// 0E0
	BGSBlockBashData			blockBash;		// 0EC
	BGSKeywordForm				keyword;		// 0F8
	TESDescription				description;	// 104

	// members
	UInt32						armorValTimes100;	// 110
	tArray<TESObjectARMA *>		armorAddons;		// 114
	UInt32						unk120;				// 120 - enchantment related?
};

STATIC_ASSERT(offsetof(TESObjectARMO, armorValTimes100) == 0x110);
STATIC_ASSERT(sizeof(TESObjectARMO) == 0x124);

// AC
class TESObjectBOOK : public TESBoundObject
{
public:
	enum { kTypeID = kFormType_Book };

	// parents
	TESFullName					fullName;		// 20
	TESModelTextureSwap			texSwap;		// 28
	TESIcon						icon;			// 44
	TESValueForm				value;			// 4C
	TESWeightForm				weight;			// 54
	TESDescription				description;	// 5C
	BGSDestructibleObjectForm	destructible;	// 68
	BGSMessageIcon				messageIcon;	// 70
	BGSPickupPutdownSounds		pickupSounds;	// 7C
	BGSKeywordForm				keyword;		// 88

	// members
	struct Data
	{
		enum	// type - these are technically flags
		{
			kType_None	=	0,
			kType_Skill =	1 << 0,
			kType_CantBeTaken	=	1 << 1,
			kType_Spell =	1 << 2,	// takes priority over skill
			kType_Read	=	1 << 3, // set once the book is equipped by the player, along with the CHANGE_BOOK_READ (0x40) change flag
		};

		UInt8	flags;		// 0
		UInt8	type;		// 1
		UInt16	unk02;		// 2, probably padding too
		union
		{
			UInt32		skill;
			SpellItem	* spell;
		} teaches;			// 4

		// make some sense of the flags field so we know what's in the union
		UInt32	GetSanitizedType(void);
	};

	Data			data;			// 94
	TESObjectSTAT	* bookStat;		// 9C
	TESDescription	description2;	// A0
};

STATIC_ASSERT(sizeof(TESObjectBOOK) == 0xAC);

// 88
class TESObjectMISC : public TESBoundObject
{
public:
	enum { kTypeID = kFormType_Misc };

	// parents
	TESFullName					fullName;		// 20
	TESModelTextureSwap			texSwap;		// 28
	TESIcon						icon;			// 44
	TESValueForm				value;			// 4C
	TESWeightForm				weight;			// 54
	BGSDestructibleObjectForm	destructible;	// 5C
	BGSMessageIcon				messageIcon;	// 64
	BGSPickupPutdownSounds		pickupSounds;	// 70
	BGSKeywordForm				keyword;		// 7C

	virtual void	Unk_52(void);
	virtual void	Unk_53(UInt32 arg0, UInt32 arg1);
	virtual void	Unk_54(void);
};

// 9C
class BGSApparatus : public TESObjectMISC
{
public:
	enum { kTypeID = kFormType_Apparatus };

	// parents
	TESQualityForm	quality;		// 88
	TESDescription	description;	// 90
};

// 88
class TESKey : public TESObjectMISC
{
public:
	enum { kTypeID = kFormType_Key };
};

// 90
class TESSoulGem : public TESObjectMISC
{
public:
	enum { kTypeID = kFormType_SoulGem };

	UInt32	unk88;		// 88
	UInt8	soulSize;	// 8C
	UInt8	gemSize;	// 8D
	UInt8	pad8E[2];	// 8E
};

// 44
class TESObjectSTAT : public TESBoundObject
{
public:
	enum { kTypeID = kFormType_Static };

	// parents
	TESModelTextureSwap		texSwap;	// 20
	
	// members
	// 8
	struct Data3C
	{
		float	unk0;
		UInt32	unk4;
	};

	Data3C	unk3C;	// 3C
};

// 5C
// they probably wanted this to derive from TESObjectSTAT first
// doesn't really hurt anything except performance (shims)
class BGSMovableStatic : public TESFullName
{
public:
	enum { kTypeID = kFormType_MovableStatic };

	// parents
	BGSDestructibleObjectForm	destructible;	// 08
	TESObjectSTAT				staticObj;		// 10

	// members
	UInt8	unk58;		// 58
	UInt8	pad59[3];	// 59
};

// 84
class TESObjectTREE : public TESBoundObject
{
public:
	enum { kTypeID = kFormType_Tree };

	// parents
	TESModel		model;		// 20
	TESFullName		fullName;	// 34
	TESProduceForm	produce;	// 3C
	
	// members
	// 38
	struct Data4C
	{
		float	unk00;	// 00 - init'd to 1
		float	unk04;	// 04 - init'd to 1
		float	unk08;	// 08 - init'd to .03
		float	unk0C;	// 0C - init'd to .6
		float	unk10;	// 10 - init'd to .2
		float	unk14;	// 14 - init'd to .4
		float	unk18;	// 18 - init'd to .075
		float	unk1C;	// 1C - init'd to .025
		float	unk20;	// 20 - init'd to .035
		float	unk24;	// 24 - init'd to 1
		float	unk28;	// 28 - init'd to 1
		float	unk2C;	// 2C - init'd to 1
		UInt32	unk30;	// 30
		UInt32	unk34;	// 34 - init'd to 2
	};

	Data4C	unk4C;	// 4C
};

// 134
class TESObjectWEAP : public TESBoundObject
{
public:
	enum { kTypeID = kFormType_Weapon };

	// parents
	TESFullName					fullName;		// 020
	TESModelTextureSwap			texSwap;		// 028
	TESIcon						icon;			// 044
	TESEnchantableForm			enchantable;	// 04C
	TESValueForm				value;			// 058
	TESWeightForm				weight;			// 060
	TESAttackDamageForm			damage;			// 068
	BGSDestructibleObjectForm	destructible;	// 070
	BGSEquipType				equipType;		// 078
	BGSPreloadable				preloadable;	// 080
	BGSMessageIcon				messageIcon;	// 084
	BGSPickupPutdownSounds		pickupSounds;	// 090
	BGSBlockBashData			blockBash;		// 09C
	BGSKeywordForm				keyword;		// 0A8
	TESDescription				description;	// 0B4

	// members

	// 34
	struct GameData
	{
		enum	// type
		{
			kType_HandToHandMelee = 0,
			kType_OneHandSword,
			kType_OneHandDagger,
			kType_OneHandAxe,
			kType_OneHandMace,
			kType_TwoHandSword,
			kType_TwoHandAxe,
			kType_Bow,
			kType_Staff,
			kType_CrossBow,
			kType_H2H,
			kType_1HS,
			kType_1HD,
			kType_1HA,
			kType_1HM,
			kType_2HS,
			kType_2HA,
			kType_Bow2,
			kType_Staff2,
			kType_CBow
		};

		enum Flags1 {
			kFlags_PlayerOnly = 0x01,
			kFlags_NPCUseAmmo = 0x02,
			kFlags_NoJamAfterReload = 0x04,
			kFlags_MinorCrime = 0x10,
			kFlags_NotUsedInNormalCombat = 0x40,
			kFlags_Unknown1 = 0x100,
			kFlags_LongBursts = 0x800,
			kFlags_NonHostile = 0x1000,
			kFlags_BoundWeapon = 0x2000,
		};

		enum Flags2 {
			kFlags_Hidebackpack = 0x01,
			kFlags_Automatic = 0x02,
			kFlags_CantDrop = 0x08,
			kFlags_EmbeddedWeapon = 0x20,
			kFlags_Unknown2 = 0x40,
			kFlags_NotPlayable = 0x80,
		};

		UInt32	unk00;	// 00
		float	speed;	// 04
		float	reach;	// 08
		float	minRange;	// 0C
		float	maxRange;	// 10
		float	animationMult;	// 14
		float	unk18;	// 18
		float	stagger;// 1C
		UInt32	unk20;	// 20
		UInt32	skill;	// 24
		UInt32	resist;	// 28
		UInt16	flags1;	// 2C
		UInt8	vatsChance;	// 2E
		UInt8	unk2F;	// 2F
		UInt8	unk30;	// 30
		UInt8	type;	// 31
		UInt8	flags2;	// 32
		UInt8	unk33;	// 33
	};

	// 0C
	struct CritData
	{
		float	critMult;			// 00
		SpellItem	* critEffect;	// 04
		UInt16	critDamage;			// 08
		UInt8	effectOnDeath;		// 0A
		UInt8	pad0B;				// 0B
	};

	GameData	gameData;	// 0C0
	CritData	critData;	// 0F4
	TESForm	* scopeEffect;		// 100
	BGSSoundDescriptorForm	* attackSound;		// 104
	BGSSoundDescriptorForm	* attackSound2D;	// 108
	BGSSoundDescriptorForm	* attackLoopSound;	// 10C
	BGSSoundDescriptorForm	* attackFailSound;	// 110
	BGSSoundDescriptorForm	* idleSound;		// 114
	BGSSoundDescriptorForm	* equipSound;		// 118
	BGSSoundDescriptorForm	* unequipSound;		// 11C
	BGSImpactDataSet	* impactDataSet;	// 120
	TESObjectSTAT	* model;				// 124
	TESObjectWEAP	* templateForm;			// 128 - Non-weapon templates don't make sense here and would probably crash anyway so assume it
	BSFixedString	embeddedNode;			// 12C
	UInt32	pad130;							// 130

	float speed() { return gameData.speed; }
	float reach() { return gameData.reach; }
	float stagger() { return gameData.stagger; }
	float minRange() { return gameData.minRange; }
	float maxRange() { return gameData.maxRange; }
	UInt8 type() { return gameData.type; }
	UInt16 critDamage() { return critData.critDamage; }

	void GetNodeName(char * dstBuff);
};

STATIC_ASSERT(sizeof(TESObjectWEAP) == 0x134);

// C8
class TESObjectARMA : public TESObject
{
public:
	enum { kTypeID = kFormType_ARMA };

	// parents
	TESRaceForm			race;	// 14
	BGSBipedObjectForm	biped;	// 1C

	// members

	// 0C
	struct Data2C
	{
		UInt8	priority[2];	// 00
		UInt8	unk02;			// 02
		UInt8	unk03;			// 03
		UInt8	unk04;			// 04
		UInt8	unk05;			// 05
		UInt8	unk06;			// 06
		UInt8	pad07;			// 07
		UInt32	unk08;			// 08
	};

	Data2C						data;					// 28
	TESModelTextureSwap			models[2][2];			// 34
	UInt32						unkA4;					// A4
	UInt32						unkA8;					// A8
	UInt32						unkAC;					// AC
	UInt32						unkB0;					// B0
	tArray<TESRace*>			additionalRaces;		// B4
	BGSFootstepSet				* footstepSet;			// C0
	UInt32						unkC4;					// C4

	bool isValidRace(TESRace * race) const;
	void GetNodeName(char * dstBuff, TESObjectREFR * refr, TESObjectARMO * armor, float weightOverride);
};

STATIC_ASSERT(sizeof(TESObjectARMA) == 0xC8);

class ActiveEffect;

class ActiveEffectReferenceEffectController
{
public:
	virtual ~ActiveEffectReferenceEffectController();

//	void			** _vtbl;	// 00
	ActiveEffect	* effect;	// 04
	// possibly more
};

class ActiveEffect
{
public:
	enum { kTypeID = kFormType_ActiveMagicEffect };

	enum
	{
		kFlag_Inactive = 0x8000
	};

	virtual ~ActiveEffect();

//	void					** _vtbl;		// 00
	ActiveEffectReferenceEffectController	controller;	// 04
	UInt32					unk0C[8];		// 0C
	void					* niNode;		// 2C
	MagicItem				* item;			// 30
	MagicItem::EffectItem	* effect;		// 34
	TESObjectREFR			* reference;	// 38
	TESForm					* sourceItem;	// 3C
	UInt32					unk40;			// 40
	UInt32					unk44;			// 44
	float					elapsed;		// 48
	float					duration;		// 4C
	float					magnitude;		// 50
	UInt32					flags;			// 54
	UInt32					unk58;			// 58
	UInt32					effectNum;		// 5C - Somekind of counter used to determine whether the ActiveMagicEffect handle is valid
	UInt32					unk60;			// 60
	UInt32					actorValue;		// 64 - Only seems to appear on value modifiers
	UInt32					unk68;			// 68
	UInt32					unk6C;			// 6C
};

class ScriptEffect : public ActiveEffect
{
public:
	virtual ~ScriptEffect();

	// ??
};

class ScriptedRefEffect : public ScriptEffect
{
public:
	virtual ~ScriptedRefEffect();

	// ??
};

class SlowTimeEffect : public ScriptEffect
{
public:
	virtual ~SlowTimeEffect();

	// ??
};

class ValueModifierEffect : public ActiveEffect
{
public:
	virtual ~ValueModifierEffect();

	// ??
};

class BoundItemEffect : public ActiveEffect
{
public:
	virtual ~BoundItemEffect();

	// ??
};

class CloakEffect : public ActiveEffect
{
public:
	virtual ~CloakEffect();

	// ??
};

class CommandEffect : public ActiveEffect
{
public:
	virtual ~CommandEffect();

	// ??
};

class ReanimateEffect : public CommandEffect
{
public:
	virtual ~ReanimateEffect();

	// ??
};

class CommandSummonedEffect : public ActiveEffect
{
public:
	virtual ~CommandSummonedEffect();

	// ??
};

class SummonCreatureEffect : public ActiveEffect
{
public:
	virtual ~SummonCreatureEffect();

	// ??
};

class CureEffect : public ActiveEffect
{
public:
	virtual ~CureEffect();

	// ??
};

class DetectLifeEffect : public ActiveEffect
{
public:
	virtual ~DetectLifeEffect();

	// ??
};

class StaggerEffect : public ActiveEffect
{
public:
	virtual ~StaggerEffect();

	// ??
};

class DisarmEffect : public StaggerEffect
{
public:
	virtual ~DisarmEffect();

	// ??
};

class DisguiseEffect : public ActiveEffect
{
public:
	virtual ~DisguiseEffect();

	// ??
};

class DispelEffect : public ActiveEffect
{
public:
	virtual ~DispelEffect();

	// ??
};

class EtherealizationEffect : public ActiveEffect
{
public:
	virtual ~EtherealizationEffect();

	// ??
};

class GuideEffect : public ActiveEffect
{
public:
	virtual ~GuideEffect();

	// ??
};

class LightEffect : public ActiveEffect
{
public:
	virtual ~LightEffect();

	// ??
};

class LockEffect : public ActiveEffect
{
public:
	virtual ~LockEffect();

	// ??
};

class OpenEffect : public ActiveEffect
{
public:
	virtual ~OpenEffect();

	// ??
};

class SoulTrapEffect : public ActiveEffect
{
public:
	virtual ~SoulTrapEffect();

	// ??
};

class TelekinesisEffect : public ActiveEffect
{
public:
	virtual ~TelekinesisEffect();

	// ??
};

class VampireLordEffect : public ActiveEffect
{
public:
	virtual ~VampireLordEffect();

	// ??
};

class WerewolfEffect : public ActiveEffect
{
public:
	virtual ~WerewolfEffect();

	// ??
};

class WerewolfFeedEffect : public ActiveEffect
{
public:
	virtual ~WerewolfFeedEffect();

	// ??
};

class SpawnHazardEffect : public ActiveEffect
{
public:
	virtual ~SpawnHazardEffect();

	// ??
};

class PeakValueModifierEffect : public ValueModifierEffect
{
public:
	virtual ~PeakValueModifierEffect();

	// ??
};

class DualValueModifierEffect : public ValueModifierEffect
{
public:
	virtual ~DualValueModifierEffect();

	// ??
};

class EnhanceWeaponEffect : public DualValueModifierEffect
{
public:
	virtual ~EnhanceWeaponEffect();

	// ??
};

class AccumulatingValueModifierEffect : public ValueModifierEffect
{
public:
	virtual ~AccumulatingValueModifierEffect();

	// ??
};

class InvisibilityEffect : public ValueModifierEffect
{
public:
	virtual ~InvisibilityEffect();

	// ??
};

class NightEyeEffect : public ValueModifierEffect
{
public:
	virtual ~NightEyeEffect();

	// ??
};

class ParalysisEffect : public ValueModifierEffect
{
public:
	virtual ~ParalysisEffect();

	// ??
};

class DarknessEffect : public ValueModifierEffect
{
public:
	virtual ~DarknessEffect();

	// ??
};

class AbsorbEffect : public ValueModifierEffect
{
public:
	virtual ~AbsorbEffect();

	// ??
};

class ValueAndConditionsEffect : public ValueModifierEffect
{
public:
	virtual ~ValueAndConditionsEffect();

	// ??
};

class TargetValueModifierEffect : public ValueModifierEffect
{
public:
	virtual ~TargetValueModifierEffect();

	// ??
};

class RallyEffect : public TargetValueModifierEffect
{
public:
	virtual ~RallyEffect();

	// ??
};

class DemoralizeEffect : public TargetValueModifierEffect
{
public:
	virtual ~DemoralizeEffect();

	// ??
};

class CalmEffect : public TargetValueModifierEffect
{
public:
	virtual ~CalmEffect();

	// ??
};

class FrenzyEffect : public TargetValueModifierEffect
{
public:
	virtual ~FrenzyEffect();

	// ??
};

class TurnUndeadEffect : public DemoralizeEffect
{
public:
	virtual ~TurnUndeadEffect();

	// ??
};

class BanishEffect : public DemoralizeEffect
{
public:
	virtual ~BanishEffect();

	// ??
};

class BGSEntryPointFunctionData
{
public:
	virtual ~BGSEntryPointFunctionData();
};


class BGSEntryPointFunctionDataTwoValue : public BGSEntryPointFunctionData
{
public:
	virtual ~BGSEntryPointFunctionDataTwoValue();

	enum {
		kNumValues = 2
	};

	float value[kNumValues];
};

class BGSEntryPointFunctionDataOneValue : public BGSEntryPointFunctionData
{
public:
	virtual ~BGSEntryPointFunctionDataOneValue();

	float value;
};

class BGSEntryPointFunctionDataText : public BGSEntryPointFunctionData
{
public:
	virtual ~BGSEntryPointFunctionDataText();

	StringCache::Ref text;
};

class BGSEntryPointFunctionDataSpellItem : public BGSEntryPointFunctionData
{
public:
	virtual ~BGSEntryPointFunctionDataSpellItem();

	SpellItem	* spellItem;
};

class BGSEntryPointFunctionDataLeveledList : public BGSEntryPointFunctionData
{
public:
	virtual ~BGSEntryPointFunctionDataLeveledList();

	TESLevItem	* leveledList;
};

class BGSEntryPointFunctionDataActivateChoice : public BGSEntryPointFunctionData
{
public:
	virtual ~BGSEntryPointFunctionDataActivateChoice();

	StringCache::Ref	label;
	BGSPerk				* perk;
	SpellItem			* appliedSpell;
	UInt32				flags;
	UInt32				unk14;
};

class BGSEntryPointFunctionDataBooleanGraphVariable : public BGSEntryPointFunctionData
{
public:
	virtual ~BGSEntryPointFunctionDataBooleanGraphVariable();

	StringCache::Ref	variable;
};


class BGSPerkEntry
{
public:
	virtual ~BGSPerkEntry();

	UInt8	rank;
	UInt8	priority;
	UInt16	unk06;
};

class BGSQuestPerkEntry : public BGSPerkEntry
{
public:
	virtual ~BGSQuestPerkEntry();

	TESQuest	* quest;
	UInt8		stage;
	UInt8		pad[3];
};

class BGSAbilityPerkEntry : public BGSPerkEntry
{
public:
	virtual ~BGSAbilityPerkEntry();

	SpellItem	* spellItem;
};

class BGSEntryPointPerkEntry : public BGSPerkEntry
{
public:
	virtual ~BGSEntryPointPerkEntry(); // Has a number of virtuals, don't know what they are yet

	UInt32						unk08;
	BGSEntryPointFunctionData	* data;
	void						* unk10;
	BGSPerk						* perk;
};

class BGSPrimitive
{
public:
	BGSPrimitive();
	virtual ~BGSPrimitive();

	enum {
		kPrimitive_None = 0,
		kPrimitive_Box = 1,
		kPrimitive_Sphere = 2
	};

	UInt32	type;
	float	bounds_x; // Div 2 from CK
	float	bounds_y; // Div 2 from CK
	float	bounds_z; // Div 2 from CK
};

class BGSPrimitiveBox : public BGSPrimitive
{
public:
	BGSPrimitiveBox();
	virtual ~BGSPrimitiveBox();
};

class BGSPrimitiveLine : public BGSPrimitiveBox
{
public:
	BGSPrimitiveLine();
	virtual ~BGSPrimitiveLine();
};

class BGSPrimitivePlane : public BGSPrimitive
{
public:
	BGSPrimitivePlane();
	virtual ~BGSPrimitivePlane();
};

class BGSPrimitiveSphere : public BGSPrimitive
{
public:
	BGSPrimitiveSphere();
	virtual ~BGSPrimitiveSphere();
};
