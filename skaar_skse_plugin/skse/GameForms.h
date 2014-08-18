#pragma once

#include "skse/GameTypes.h"
#include "skse/GameFormComponents.h"
#include "skse/GamePathing.h"
#include "skse/NiInterpolators.h"

class TESForm;
class TESObjectSTAT;
class TESFile;
class TESObjectLIGH;
class TESEffectShader;
class BGSProjectile;
class BGSArtObject;
class BGSImpactDataSet;
class TESSound;
class TESObjectREFR;
class BGSListForm;
class TESQuest;
class BGSExplosion;
class BGSDualCastData;
class TESImageSpaceModifier;
class TESWordOfPower;
class BGSHazard;
class TESNPC;
class TESWorldSpace;

typedef TESForm * (* _LookupFormByID)(UInt32 id);
extern const _LookupFormByID LookupFormByID;

// TESObject and derivatives are in GameObjects.h
// TESObjectREFR and derivatives are in GameReferences.h
// everything else deriving from TESForm goes here

/**** form types ***************************************************************
 *	
 *	TESHair and BGSCloudClusterForm were removed in 1.2
 *	
 *	1.1	1.2
 *	id	id	code	name
 *	
 *	00	00	NONE	
 *	01	01	TES4	
 *	02	02	GRUP	
 *	03	03	GMST	<nonstandard loader?>
 *	04	04	KYWD	BGSKeyword
 *	05	05	LCRT	BGSLocationRefType
 *	06	06	AACT	BGSAction
 *	07	07	TXST	BGSTextureSet
 *	08	08	MICN	BGSMenuIcon
 *	09	09	GLOB	TESGlobal
 *	0A	0A	CLAS	TESClass
 *	0B	0B	FACT	TESFaction
 *	0C	0C	HDPT	BGSHeadPart
 *	0D	xx	HAIR	TESHair
 *	0E	0D	EYES	TESEyes
 *	0F	0E	RACE	TESRace
 *	10	0F	SOUN	TESSound
 *	11	10	ASPC	BGSAcousticSpace
 *	12	11	SKIL	<missing>
 *	13	12	MGEF	EffectSetting
 *	14	13	SCPT	Script
 *	15	14	LTEX	TESLandTexture
 *	16	15	ENCH	EnchantmentItem
 *	17	16	SPEL	SpellItem
 *	18	17	SCRL	ScrollItem
 *	19	18	ACTI	TESObjectACTI
 *	1A	19	TACT	BGSTalkingActivator
 *	1B	1A	ARMO	TESObjectARMO
 *	1C	1B	BOOK	TESObjectBOOK
 *	1D	1C	CONT	TESObjectCONT
 *	1E	1D	DOOR	TESObjectDOOR
 *	1F	1E	INGR	IngredientItem
 *	20	1F	LIGH	TESObjectLIGH
 *	21	20	MISC	TESObjectMISC
 *	22	21	APPA	BGSApparatus
 *	23	22	STAT	TESObjectSTAT
 *	24	23	SCOL	BGSStaticCollection
 *	25	24	MSTT	BGSMovableStatic
 *	26	25	GRAS	TESGrass
 *	27	26	TREE	TESObjectTREE
 *	28	xx	CLDC	BGSCloudClusterForm
 *	29	27	FLOR	TESFlora
 *	2A	28	FURN	TESFurniture
 *	2B	29	WEAP	TESObjectWEAP
 *	2C	2A	AMMO	TESAmmo
 *	2D	2B	NPC_	TESNPC
 *	2E	2C	LVLN	TESLevCharacter
 *	2F	2D	KEYM	TESKey
 *	30	2E	ALCH	AlchemyItem
 *	31	2F	IDLM	BGSIdleMarker / BGSDefaultObjectManager? strange
 *	32	30	NOTE	BGSNote
 *	33	31	COBJ	BGSConstructibleObject
 *	34	32	PROJ	BGSProjectile
 *	35	33	HAZD	BGSHazard
 *	36	34	SLGM	TESSoulGem
 *	37	35	LVLI	TESLevItem
 *	38	36	WTHR	TESWeather
 *	39	37	CLMT	TESClimate
 *	3A	38	SPGD	BGSShaderParticleGeometryData
 *	3B	39	RFCT	BGSReferenceEffect
 *	3C	3A	REGN	TESRegion
 *	3D	3B	NAVI	NavMeshInfoMap
 *	3E	3C	CELL	TESObjectCELL
 *	3F	3D	REFR	TESObjectREFR / Actor
 *	40	3E	ACHR	Character / PlayerCharacter
 *	41	3F	PMIS	MissileProjectile
 *	42	40	PARW	ArrowProjectile
 *	43	41	PGRE	GrenadeProjectile
 *	44	42	PBEA	BeamProjectile
 *	45	43	PFLA	FlameProjectile
 *	46	44	PCON	ConeProjectile
 *	47	45	PBAR	BarrierProjectile
 *	48	46	PHZD	Hazard
 *	49	47	WRLD	TESWorldSpace
 *	4A	48	LAND	TESObjectLAND
 *	4B	49	NAVM	NavMesh
 *	4C	4A	TLOD	?
 *	4D	4B	DIAL	TESTopic
 *	4E	4C	INFO	TESTopicInfo
 *	4F	4D	QUST	TESQuest
 *	50	4E	IDLE	TESIdleForm
 *	51	4F	PACK	TESPackage
 *	52	50	CSTY	TESCombatStyle
 *	53	51	LSCR	TESLoadScreen
 *	54	52	LVSP	TESLevSpell
 *	55	53	ANIO	TESObjectANIO
 *	56	54	WATR	TESWaterForm
 *	57	55	EFSH	TESEffectShader
 *	58	56	TOFT	?
 *	59	57	EXPL	BGSExplosion
 *	5A	58	DEBR	BGSDebris
 *	5B	59	IMGS	TESImageSpace
 *	5C	5A	IMAD	TESImageSpaceModifier
 *	5D	5B	FLST	BGSListForm
 *	5E	5C	PERK	BGSPerk
 *	5F	5D	BPTD	BGSBodyPartData
 *	60	5E	ADDN	BGSAddonNode
 *	61	5F	AVIF	ActorValueInfo
 *	62	60	CAMS	BGSCameraShot
 *	63	61	CPTH	BGSCameraPath
 *	64	62	VTYP	BGSVoiceType
 *	65	63	MATT	BGSMaterialType
 *	66	64	IPCT	BGSImpactData
 *	67	65	IPDS	BGSImpactDataSet
 *	68	66	ARMA	TESObjectARMA
 *	69	67	ECZN	BGSEncounterZone
 *	6A	68	LCTN	BGSLocation
 *	6B	69	MESH	BGSMessage
 *	6C	6A	RGDL	BGSRagdoll
 *	6D	6B	DOBJ	?
 *	6E	6C	LGTM	BGSLightingTemplate
 *	6F	6D	MUSC	BGSMusicType
 *	70	6E	FSTP	BGSFootstep
 *	71	6F	FSTS	BGSFootstepSet
 *	72	70	SMBN	BGSStoryManagerBranchNode
 *	73	71	SMQN	BGSStoryManagerQuestNode
 *	74	72	SMEN	BGSStoryManagerEventNode
 *	75	73	DLBR	BGSDialogueBranch
 *	76	74	MUST	BGSMusicTrackFormWrapper
 *	77	75	DLVW	?
 *	78	76	WOOP	TESWordOfPower
 *	79	77	SHOU	TESShout
 *	7A	78	EQUP	BGSEquipSlot
 *	7B	79	RELA	BGSRelationship
 *	7C	7A	SCEN	BGSScene
 *	7D	7B	ASTP	BGSAssociationType
 *	7E	7C	OTFT	BGSOutfit
 *	7F	7D	ARTO	BGSArtObject
 *	80	7E	MATO	BGSMaterialObject
 *	81	7F	MOVT	BGSMovementType
 *	82	80	SNDR	BGSSoundDescriptorForm
 *	83	81	DUAL	BGSDualCastData
 *	84	82	SNCT	BGSSoundCategory
 *	85	83	SOPM	BGSSoundOutput
 *	86	84	COLL	BGSCollisionLayer
 *	87	85	CLFM	BGSColorForm
 *	88	86	REVB	BGSReverbParameters
 *		87			Unknown
 *		88			Alias
 *		89			ReferenceAlias
 *		8A			LocAlias
 *		8B			ActiveMagicEffect
 */

enum FormType {
	kFormType_None = 0,			//	NONE
	kFormType_TES4,				//	TES4
	kFormType_Group,			//	GRUP	
	kFormType_GMST,				//	GMST
	kFormType_Keyword,			//	KYWD	BGSKeyword
	kFormType_LocationRef,		//	LCRT	BGSLocationRefType
	kFormType_Action,			//	AACT	BGSAction
	kFormType_TextureSet,		//	TXST	BGSTextureSet
	kFormType_MenuIcon,			//	MICN	BGSMenuIcon
	kFormType_Global,			//	GLOB	TESGlobal
	kFormType_Class,			//	CLAS	TESClass
	kFormType_Faction,			//	FACT	TESFaction
	kFormType_HeadPart,			//	HDPT	BGSHeadPart
	kFormType_Eyes,				//	EYES	TESEyes
	kFormType_Race,				//	RACE	TESRace
	kFormType_Sound,			//	SOUN	TESSound
	kFormType_AcousticSpace,	//	ASPC	BGSAcousticSpace
	kFormType_Skill,			//	SKIL	<missing>
	kFormType_EffectSetting,	//	MGEF	EffectSetting
	kFormType_Script,			//	SCPT	Script
	kFormType_LandTexture,		//	LTEX	TESLandTexture
	kFormType_Enchantment,		//	ENCH	EnchantmentItem
	kFormType_Spell,			//	SPEL	SpellItem
	kFormType_ScrollItem,		//	SCRL	ScrollItem
	kFormType_Activator,		//	ACTI	TESObjectACTI
	kFormType_TalkingActivator,	//	TACT	BGSTalkingActivator
	kFormType_Armor,			//	ARMO	TESObjectARMO
	kFormType_Book,				//	BOOK	TESObjectBOOK
	kFormType_Container,		//	CONT	TESObjectCONT
	kFormType_Door,				//	DOOR	TESObjectDOOR
	kFormType_Ingredient,		//	INGR	IngredientItem
	kFormType_Light,			//	LIGH	TESObjectLIGH
	kFormType_Misc,				//	TESObjectMISC
	kFormType_Apparatus,		//	APPA	BGSApparatus
	kFormType_Static,			//	STAT	TESObjectSTAT
	kFormType_StaticCollection,	//	BGSStaticCollection
	kFormType_MovableStatic,	//	MSTT	BGSMovableStatic
	kFormType_Grass,			//	GRAS	TESGrass
	kFormType_Tree,				//	TREE	TESObjectTREE
	kFormType_Flora,			//	FLOR	TESFlora
	kFormType_Furniture,		//	FURN	TESFurniture
	kFormType_Weapon,			//	WEAP	TESObjectWEAP
	kFormType_Ammo,				//	AMMO	TESAmmo
	kFormType_NPC,				//	NPC_	TESNPC
	kFormType_LeveledCharacter,	//	LVLN	TESLevCharacter
	kFormType_Key,				//	KEYM	TESKey
	kFormType_Potion,			//	ALCH	AlchemyItem
	kFormType_IdleMarker,		//	IDLM	BGSIdleMarker / BGSDefaultObjectManager? strange
	kFormType_Note,				//	NOTE	BGSNote
kFormType_ConstructibleObject,	//	COBJ	BGSConstructibleObject
	kFormType_Projectile,		//	PROJ	BGSProjectile
	kFormType_Hazard,			//	HAZD	BGSHazard
	kFormType_SoulGem,			//	SLGM	TESSoulGem
	kFormType_LeveledItem,		//	LVLI	TESLevItem
	kFormType_Weather,			//	WTHR	TESWeather
	kFormType_Climate,			//	CLMT	TESClimate
	kFormType_SPGD,				//	SPGD	BGSShaderParticleGeometryData
	kFormType_ReferenceEffect,	//	RFCT	BGSReferenceEffect
	kFormType_Region,			//	REGN	TESRegion
	kFormType_NAVI,				//	NAVI	NavMeshInfoMap
	kFormType_Cell,				//	CELL	TESObjectCELL
	kFormType_Reference,		//	REFR	TESObjectREFR / Actor
	kFormType_Character,		//	ACHR	Character / PlayerCharacter
	kFormType_Missile,			//	PMIS	MissileProjectile
	kFormType_Arrow,			//	PARW	ArrowProjectile
	kFormType_Grenade,			//	PGRE	GrenadeProjectile
	kFormType_BeamProj,			//	PBEA	BeamProjectile
	kFormType_FlameProj,		//	PFLA	FlameProjectile
	kFormType_ConeProj,			//	PCON	ConeProjectile
	kFormType_BarrierProj,		//	PBAR	BarrierProjectile
	kFormType_PHZD,				//	PHZD	Hazard
	kFormType_WorldSpace,		//	WRLD	TESWorldSpace
	kFormType_Land,				//	LAND	TESObjectLAND
	kFormType_NAVM,				//	NAVM	NavMesh
	kFormType_TLOD,				//	TLOD	?
	kFormType_Topic,			//	DIAL	TESTopic
	kFormType_TopicInfo,		//	INFO	TESTopicInfo
	kFormType_Quest,			//	QUST	TESQuest
	kFormType_Idle,				//	IDLE	TESIdleForm
	kFormType_Package,			//	PACK	TESPackage
	kFormType_CombatStyle,		//	CSTY	TESCombatStyle
	kFormType_LoadScreen,		//	LSCR	TESLoadScreen
	kFormType_LeveledSpell,		//	LVSP	TESLevSpell
	kFormType_ANIO,				//	ANIO	TESObjectANIO
	kFormType_Water,			//	WATR	TESWaterForm
	kFormType_EffectShader,		//	EFSH	TESEffectShader
	kFormType_TOFT,				//	TOFT	?
	kFormType_Explosion,		//	EXPL	BGSExplosion
	kFormType_Debris,			//	DEBR	BGSDebris
	kFormType_ImageSpace,		//	IMGS	TESImageSpace
	kFormType_ImageSpaceMod,	//	IMAD	TESImageSpaceModifier
	kFormType_List,				//	FLST	BGSListForm
	kFormType_Perk,				//	PERK	BGSPerk
	kFormType_BodyPartData,		//	BPTD	BGSBodyPartData
	kFormType_AddonNode,		//	ADDN	BGSAddonNode
	kFormType_ActorValueInfo,	//	AVIF	ActorValueInfo
	kFormType_CameraShot,		//	CAMS	BGSCameraShot
	kFormType_CameraPath,		//	CPTH	BGSCameraPath
	kFormType_VoiceType,		//	VTYP	BGSVoiceType
	kFormType_MaterialType,		//	MATT	BGSMaterialType
	kFormType_ImpactData,		//	IPCT	BGSImpactData
	kFormType_ImpactDataSet,	//	IPDS	BGSImpactDataSet
	kFormType_ARMA,				//	ARMA	TESObjectARMA
	kFormType_EncounterZone,	//	ECZN	BGSEncounterZone
	kFormType_Location,			//	LCTN	BGSLocation
	kFormType_Message,			//	MESH	BGSMessage
	kFormType_Ragdoll,			//	RGDL	BGSRagdoll
	kFormType_DOBJ,				//	DOBJ	? (used for default objects, custom loader)
	kFormType_LightingTemplate,	//	LGTM	BGSLightingTemplate
	kFormType_MusicType,		//	MUSC	BGSMusicType
	kFormType_Footstep,			//	FSTP	BGSFootstep
	kFormType_FootstepSet,		//	FSTS	BGSFootstepSet
	kFormType_StoryBranchNode,	//	SMBN	BGSStoryManagerBranchNode
	kFormType_StoryQuestNode,	//	SMQN	BGSStoryManagerQuestNode
	kFormType_StoryEventNode,	//	SMEN	BGSStoryManagerEventNode
	kFormType_DialogueBranch,	//	DLBR	BGSDialogueBranch
	kFormType_MusicTrack,		//	MUST	BGSMusicTrackFormWrapper
	kFormType_DLVW,				//	DLVW	?
	kFormType_WordOfPower,		//	WOOP	TESWordOfPower
	kFormType_Shout,			//	SHOU	TESShout
	kFormType_EquipSlot,		//	EQUP	BGSEquipSlot
	kFormType_Relationship,		//	RELA	BGSRelationship
	kFormType_Scene,			//	SCEN	BGSScene
	kFormType_AssociationType,	//	ASTP	BGSAssociationType
	kFormType_Outfit,			//	OTFT	BGSOutfit
	kFormType_Art,				//	ARTO	BGSArtObject
	kFormType_Material,			//	MATO	BGSMaterialObject
	kFormType_MovementType,		//	MOVT	BGSMovementType
	kFormType_SoundDescriptor,	//	SNDR	BGSSoundDescriptorForm
	kFormType_DualCastData,		//	DUAL	BGSDualCastData
	kFormType_SoundCategory,	//	SNCT	BGSSoundCategory
	kFormType_SoundOutput,		//	SOPM	BGSSoundOutput
	kFormType_CollisionLayer,	//	COLL	BGSCollisionLayer
	kFormType_ColorForm,		//	CLFM	BGSColorForm
	kFormType_ReverbParam,		//	REVB	BGSReverbParameters
	kFormType_Unk87,
	kFormType_Alias,			//			BGSBaseAlias
	kFormType_ReferenceAlias,	//			BGSRefAlias
	kFormType_LocationAlias,	//			BGSLocAlias
	kFormType_ActiveMagicEffect,//			ActiveMagicEffect

#ifdef PAPYRUS_CUSTOM_CLASS
	kFormType_TintMask = 300,
#endif

	kFormType_Max =				kFormType_ReverbParam	// max of standard types
};

// 14
class TESForm : public BaseFormComponent
{
public:
	enum { kTypeID = 0 };	// special-case

	enum {
		kFlagPlayerKnows = 0x40
	};

	virtual void			Unk_04(void);		// reset/init? would leak pointers if called on a live object
	virtual void			Unk_05(void);		// release pointers?
	virtual bool			LoadForm(UInt32 arg);
	virtual bool			Unk_07(UInt32 arg);
	virtual bool			Unk_08(UInt32 arg);	// calls LoadForm
	virtual TESForm *		Unk_09(UInt32 arg1, void * arg2);
	virtual bool			MarkChanged(UInt32 changed);	// mark as changed?
	virtual void			Unk_0B(UInt32 arg);
	virtual bool			Unk_0C(UInt32 arg);
	virtual void			Unk_0D(UInt32 arg);
	virtual void			Unk_0E(UInt32 arg);
	virtual void			Unk_0F(void * arg);
	virtual void			Unk_10(UInt32 arg);
	virtual void			Unk_11(UInt32 arg);
	virtual void			Unk_12(UInt32 arg);
	virtual void			InitItem(void);		// called once all dependent forms are loaded
	virtual void			Unk_14(void);
	virtual UInt32			GetFormType(void);
	virtual void			GetFormDesc(char * buf, UInt32 bufLen);
	virtual bool			GetFlag00000040(void);
	virtual bool			GetFlag00010000(void);
	virtual bool			IsPlayable(void);
	virtual bool			GetFlag00080000(void);
	virtual bool			GetFlag02000000(void);
	virtual bool			Unk_1B(void);
	virtual bool			GetFlag00000200(void);
	virtual bool			GetFlag00000100(void);
	virtual void			SetFlag00000200(bool set);
	virtual bool			Unk_20(void);
	virtual void			SetFlag00000020(bool set);
	virtual void			SetFlag00000002(bool set);
	virtual void			Unk_23(void);
	virtual void			Unk_24(UInt32 arg);
	virtual bool			Unk_25(void);
	virtual bool			Unk_26(void);
	virtual bool			Has3D(void);
	virtual bool			Unk_28(void);
	virtual UInt32			Unk_29(void);
	virtual UInt32			Unk_2A(void);
	virtual UInt32			Unk_2B(void);
	virtual const char *	GetFullName(void);
	virtual void			CopyFrom(TESForm * srcForm);
	virtual bool			Compare(TESForm * rhs);
	virtual void			Unk_2F(void * dst, UInt32 unk);
	virtual const char *	GetName(void);
	virtual bool			SetName(const char * str);
	virtual bool			Unk_32(void);
	virtual bool			Unk_33(void);
	virtual bool			Unk_34(UInt32 arg);
	virtual bool			Unk_35(UInt32 arg0, UInt32 arg1, UInt32 arg2, UInt32 arg3, UInt32 arg4);
	virtual void			SetFormID(UInt32 id, UInt32 generateID);
	virtual const char *	Unk_37(void);
	virtual bool			Unk_38(void);

	bool IsWeapon() { return formType == kFormType_Weapon; }
	bool IsAmmo() { return formType == kFormType_Ammo; }
	bool IsArmor() { return formType == kFormType_Armor; }

	// bethesda removed most of the functionality from their code, this reimplements it for simple classes
	void CopyFromEx(TESForm * rhs);

	void	* unk04;	// 04
	UInt32	flags;		// 08
	UInt32	formID;		// 0C
	UInt16	unk10;		// 10
	UInt8	formType;	// 12
	UInt8	pad13;		// 13
};

// C8
class ActorValueInfo : public TESForm
{
public:
	enum { kTypeID = kFormType_ActorValueInfo };

	// parents
	TESFullName			fullName;		// 14
	TESDescription		description;	// 1C
	TESIcon				icon;			// 28

	enum {
		kSkillUseMult = 0,
		kSkillOffsetMult,
		kSkillImproveMult,
		kSkillImproveOffset
	};

	// members
	const char *		name;			// 30 - second ctor argument
	StringCache::Ref	unk34;			// 34
	UInt32				unk38;			// 38
	UInt32				unk3C;			// 3C
	UInt32				unk40;			// 40
	UInt32				unk44;			// 44
	UInt32				unk48[0x0F];	// 48 - init'd to 0xA4
	UInt32				numSubTypes;			// 84
	const char *		subTypes[0x0A];	// 88 - init'd to 0
	float *				skillUsages;	// B0
	UInt32				unkB4;			// B4
	UInt32				unkB8;			// B8
	BGSSkillPerkTreeNode *	perkTree;			// BC - BGSSkillPerkTreeNode

	UInt8	padC0[8];	// C0 - ? not initialized
};

// 28
class BGSAssociationType : public TESForm
{
public:
	enum { kTypeID = kFormType_AssociationType };

	StringCache::Ref	unk14[4];	// 14
	UInt32				unk28;		// 28
};

// 44
class BGSBodyPartData : public TESForm
{
public:
	enum { kTypeID = kFormType_BodyPartData };

	// parents
	TESModel		model;			// 14
	BGSPreloadable	preloadable;	// 28

	// members
	struct Data
	{
		StringCache::Ref	unk00;		// 00
		StringCache::Ref	unk04;		// 04
		StringCache::Ref	unk08;		// 08
		StringCache::Ref	unk0C;		// 0C
		StringCache::Ref	unk10;		// 10
		TESModel			model;		// 14
		TESModelRDT			modelRDT;	// 28
	};

	Data	* part[5];		// 2C - init'd to 0
	UInt32	unk40;			// 40 - init'd to 0
};

// 30
class BGSCameraPath : public TESForm
{
public:
	enum { kTypeID = kFormType_CameraPath };

	UInt32	unk14;		// 14
	UInt32	unk18;		// 18
	UInt32	unk1C;		// 1C
	UInt8	unk20;		// 20
	UInt8	pad21[3];	// 21
	UInt32	unk24;		// 24
	UInt32	unk28;		// 28
	UInt32	unk2C;		// 2C
};

// 74
class BGSCameraShot : public TESForm
{
public:
	enum { kTypeID = kFormType_CameraShot };

	// parents
	TESModel					model;			// 14
	TESImageSpaceModifiableForm	imageSpaceMod;	// 28

	// members
	UInt32	unk30;		// 30
	UInt32	unk34;		// 34
	UInt32	unk38;		// 38
	UInt32	unk40;		// 40
	UInt32	unk44;		// 44
	float	globalTimeMultiplier;	// 48
	UInt32	unk4C;		// 4C
	UInt32	unk50;		// 50
	UInt32	unk54;		// 54

	UInt32	unk58;		// 58
	UInt32	unk5C;		// 5C
	UInt32	unk60;		// 60
	UInt32	unk64;		// 64
	UInt32	unk68;		// 68
	UInt8	unk6C;		// 6C
	UInt8	pad6D[3];	// 6D
	UInt32	unk70;		// 70
};

// 3C
class BGSCollisionLayer : public TESForm
{
public:
	enum { kTypeID = kFormType_CollisionLayer };

	enum {
		kCollisionLayer_Unidentified = 0x00,
		kCollisionLayer_Static,
		kCollisionLayer_AnimStatic,
		kCollisionLayer_Transparent,
		kCollisionLayer_Clutter,
		kCollisionLayer_Weapon,
		kCollisionLayer_Projectile,
		kCollisionLayer_Spell,
		kCollisionLayer_Biped,
		kCollisionLayer_Trees,
		kCollisionLayer_Props,
		kCollisionLayer_Water,
		kCollisionLayer_Trigger,
		kCollisionLayer_Terrain,
		kCollisionLayer_Trap,
		kCollisionLayer_NonCollidable,
		kCollisionLayer_CloudTrap,
		kCollisionLayer_Ground,
		kCollisionLayer_Portal,
		kCollisionLayer_DebrisSmall,
		kCollisionLayer_DebrisLarge,
		kCollisionLayer_AcousticSpace,
		kCollisionLayer_ActorZone,
		kCollisionLayer_ProjectileZone,
		kCollisionLayer_GasTrap,
		kCollisionLayer_Shellcasing,
		kCollisionLayer_TransparentSmall,
		kCollisionLayer_InvisibleWall,
		kCollisionLayer_TransparentSmallAnim,
		kCollisionLayer_Ward,
		kCollisionLayer_CharController,
		kCollisionLayer_StairHelper,
		kCollisionLayer_DeadBip,
		kCollisionLayer_BipedNoCC,
		kCollisionLayer_AvoidBox,
		kCollisionLayer_CollisionBox,
		kCollisionLayer_CameraSphere,
		kCollisionLayer_DoorDetection,
		kCollisionLayer_ConeProjectile,
		kCollisionLayer_Camera,
		kCollisionLayer_ItemPicker,
		kCollisionLayer_Los,
		kCollisionLayer_PathingPick,
		kCollisionLayer_CustomPick1,
		kCollisionLayer_CustomPick2,
		kCollisionLayer_SpellExplosion,
		kCollisionLayer_DroppingPick,
		kCollisionLayer_DeadActorZone,
		kCollisionLayer_TriggerFallingTrap,
		kCollisionLayer_Navcut,
		kCollisionLayer_Critter,
		kCollisionLayer_SpellTrigger,
		kCollisionLayer_LivingAndDeadActors,
		kCollisionLayer_Detection,
		kCollisionLayer_TrapTrigger
	};

	// parents
	TESDescription				description;	// 14

	// members
	UInt32						layerId;	// 20
	UInt32						flags;	// 24
	UInt32						unk28;	// 28
	StringCache::Ref			name;	// 2C
	tArray<BGSCollisionLayer*>	interactables;	// 30 - really?
};

// 24
class BGSColorForm : public TESForm
{
public:
	enum { kTypeID = kFormType_ColorForm };

	// parents
	TESFullName     fullName;       // 14

	union {
		struct Color {
			UInt8   red, green, blue, alpha; // The alpha isn't actually used here so its usually zero
		} color;
		UInt32 abgr;    // 1C
	};
	UInt32  unk20;  // 20
};

// 30
class BGSConstructibleObject : public TESForm
{
public:
    enum { kTypeID = kFormType_ConstructibleObject };

    // members
	TESContainer	container;		// 14 - not inherited
	UInt32			unk20;			// 20 - linked list
    TESForm*		createdObject;	// 24
    BGSKeyword*		wbKeyword;		// 28
    UInt16			quantity;		// 2C
    UInt8			pad2E[2];		// 2E
};

// 20
class BGSDebris : public TESForm
{
public:
	enum { kTypeID = kFormType_Debris };

	struct Data
	{
		UInt32	unk0;
		UInt32	unk4;
	};

	// parents
	BGSPreloadable	preloadable;	// 14
	
	// members
	Data	unk18;	// 18 - linked list
};

// ?? - singleton, ID 00000031
class BGSDefaultObjectManager : public TESForm
{
public:
	enum { kTypeID = kFormType_DOBJ };

	// cleared to 0 in ctor
	TESForm	* objects[0x15A];			// 14
	UInt8	pad[0x15A];					// just pad out the rest of the space

	static BGSDefaultObjectManager *	GetSingleton(void);
};

// 24
class BGSDialogueBranch : public TESForm
{
public:
	enum { kTypeID = kFormType_DialogueBranch };

	UInt32	unk14;		// 14 - init'd to 1
	TESForm	* unk18;	// 18 - init'd to 0, TESQuest
	TESForm	* unk1C;	// 1C - init'd to 0, starting topic (type 4D)
	UInt32	unk20;		// 20 - init'd to 0
};

// 30
class BGSEncounterZone : public TESForm
{
public:
	enum { kTypeID = kFormType_EncounterZone };

	UInt8	unk14[0x0C];	// 14
	UInt8	unk20[0x10];	// 20

	// +14 - TESForm * owner?
	// +18 - TESForm * location?
};

// 24
class BGSEquipSlot : public TESForm
{
public:
	enum { kTypeID = kFormType_EquipSlot };

	tArray<BGSEquipSlot*>		parentSlots;
	UInt32						unk20;
};

// 1C
class BGSFootstep : public TESForm
{
public:
	enum { kTypeID = kFormType_Footstep };

	StringCache::Ref	unk14;	// 14
	UInt32				unk18;	// 18
};

// 50
class BGSFootstepSet : public TESForm
{
public:
	enum { kTypeID = kFormType_FootstepSet };

	UnkArray	unk14[5];	// 14
};

// 98
class BGSHeadPart : public TESForm
{
public:
	enum { kTypeID = kFormType_HeadPart };


	// parents
	TESFullName				fullName;       // 14
	TESModelTextureSwap		model;          // 1C

	// members
	UInt8					partFlags;          // 38 // Flag Inconsistencies (Is Extra Part?) (Use Solid Tint?)
	UInt8					pad39[3];       // 39
	enum {
		kTypeMisc = 0,
		kTypeFace,
		kTypeEyes,
		kTypeHair,
		kTypeFacialHair,
		kTypeScar,
		kTypeBrows,
		kNumTypes
	};

	enum 
	{
		kFlagPlayable	= 1 << 0,
		kFlagMale		= 1 << 1,
		kFlagFemale		= 1 << 2,
		kFlagExtraPart	= 1 << 3,
		kFlagSolidTint	= 1 << 4
	};

	UInt32					type;			// 3C
	tArray <BGSHeadPart *>	extraParts;		// 40
	BGSTextureSet *			textureSet;		// 4C
	TESModelTri				raceMorph;		// 50
	TESModelTri				morph;			// 64
	TESModelTri				chargenMorph;	// 78
	UInt32					unk8C;          // 8C
	BGSListForm *			validRaces;		// 90
	StringCache::Ref		partName;		// 94

	bool IsExtraPart() { return (partFlags & kFlagExtraPart) == kFlagExtraPart; }
};

// 78
class BGSImpactData : public TESForm
{
public:
	enum { kTypeID = kFormType_ImpactData };

	// 20
	struct Data54
	{
		float	unk00;		// 00 - init'd to 8
		float	unk04;		// 04 - init'd to 32
		float	unk08;		// 08 - init'd to 8
		float	unk0C;		// 0C - init'd to 32
		float	unk10;		// 10 - init'd to 32
		float	unk14;		// 14 - init'd to 4
		float	unk18;		// 18 - init'd to 1
		UInt8	unk1C;		// 1C - init'd to 4
		UInt8	unk1D;		// 1D - init'd to 0
		UInt8	pad1E[2];	// 1E
	};

	// parents
	TESModel	model;		// 14

	// members
	float	unk28;			// 28 - init'd to .25
	UInt32	unk2C;			// 2C - init'd to 2
	float	unk30;			// 30 - init'd to 15
	float	unk34;			// 34 - init'd to 16
	UInt32	unk38;			// 38 - init'd to 1
	UInt8	pad3C[4];		// 3C - init'd to 0 via memset (block from 28-40)

	BGSTextureSet			* textures[2];	// 40 - texture set
	BGSSoundDescriptorForm	* sounds[2];	// 48 - sound
	BGSHazard				* hazard;		// 50 - hazard

	Data54	unk54;			// 54
	
	UInt32	pad74;			// 74
};

// 38
class BGSImpactDataSet : public TESForm
{
public:
	enum { kTypeID = kFormType_ImpactDataSet };

	BGSPreloadable	preloadable;	// 14

	UInt32	unk18;	// 18
	UInt32	unk1C;	// 1C
	UInt32	unk20;	// 20
	UInt32	unk24;	// 24
	UInt32	unk28;	// 28
	UInt32	unk2C;	// 2C
	UInt32	unk30;	// 30
	UInt32	unk34;	// 34
};

STATIC_ASSERT(sizeof(BGSImpactDataSet) == 0x38);

// 1C
class BGSKeyword : public TESForm
{
public:
	enum { kTypeID = kFormType_Keyword };

	BSString	keyword;	// 14
};

// 20
class BGSAction : public BGSKeyword
{
public:
	enum { kTypeID = kFormType_Action };

	UInt32	idx;	// 1C - ctor sets to a unique value
};

// 1C
class BGSLocationRefType : public BGSKeyword
{
public:
	enum { kTypeID = kFormType_LocationRef };
};

// 94
class BGSLightingTemplate : public TESForm
{
public:
	enum { kTypeID = kFormType_LightingTemplate };

	// 5C
	struct Data
	{
		// 20 - used elsewhere
		struct Color
		{
			UInt32	x[2];				// 00 - init'd to 00FFFFFF
			UInt32	y[2];
			UInt32	z[2];
			UInt32	specular;			// 18 - init'd to 0
			float	fresnelPower;		// 1C - init'd to 1
		};

		UInt32	unk00;			// 00 - init'd to 0
		UInt32	unk04;			// 04 - init'd to 0
		UInt32	unk08;			// 08 - init'd to 0
		UInt32	unk0C;			// 0C - init'd to 0
		UInt32	unk10;			// 10 - init'd to 0
		UInt32	unk14;			// 14 - init'd to 0
		UInt32	unk18;			// 18 - init'd to 0
		float	unk1C;			// 1C - init'd to 1
		UInt32	unk20;			// 20 - init'd to 0
		float	unk24;			// 24 - init'd to 1
		Color	unk28;			// 28
		UInt32	unk48;			// 48 - init'd to 0
		float	unk4C;			// 4C - init'd to 1
		float	lodStartFade;	// 50 - fLightLODStartFade
		float	lodStopFade;	// 54 - fLightLODStartFade + fLightLODRange
		UInt32	unk58;			// 58 - init'd to 0000079F
	};

	Data			unk14;	// 14
	UInt32			pad70;	// 70
	Data::Color		unk74;	// 74
};

STATIC_ASSERT(sizeof(BGSLightingTemplate) == 0x94);

// 28
class BGSListForm : public TESForm
{
public:
	enum { kTypeID = kFormType_List };

	tArray<TESForm*>	forms;	// 14
	tArray<UInt32> *	addedForms;	// 20
	UInt32				unk24;	// 24

	MEMBER_FN_PREFIX(BGSListForm);
	DEFINE_MEMBER_FN(AddFormToList, void, 0x004FB380, TESForm * form);

	class Visitor
	{
	public:
		virtual bool Accept(TESForm * form) = 0;
	};

	bool Visit(BGSListForm::Visitor & visitor);
	UInt32 GetSize();
};

// 88
class BGSLocation : public TESForm
{
public:
	enum { kTypeID = kFormType_Location };

	// parents
	TESFullName		fullName;	// 14
	BGSKeywordForm	keyword;	// 1C

	// members
	UInt32						unk28;		// 28 - init'd to 0
	UInt32						unk2C;		// 2C - init'd to 0
	UInt32						unk30;		// 30 - init'd to 0
	UInt32						unk34;		// 34 - init'd to 0
	UInt32						unk38;		// 38 - init'd to 0
	UInt32						unk3C;		// 3C - init'd to 0
	UnkArray					unk40;		// 40
	UnkArray					unk4C;		// 4C
	UInt32						unk58;		// 58 - init'd to 0
	UInt32						unk5C;		// 5C - init'd to 0
	UnkArray					unk60;		// 60
	UInt32						unk6C;		// 6C - init'd to 0
	UInt32						unk70;		// 70 - init'd to 0
	UnkArray					unk74;		// 74
	UInt32						unk80;		// 80 - init'd to 0
	UInt8						unk84;		// 84 - init'd to 0
	UInt8						unk85;		// 85 - init'd to 0
	UInt8						pad86[2];	// 86
};

STATIC_ASSERT(sizeof(BGSLocation) == 0x88);

// 74
class BGSMaterialObject : public TESForm
{
public:
	enum { kTypeID = kFormType_Material };

	// parents
	TESModel			model;			// 14
	BSMaterialObject	materialObject;	// 28

	// members
	UnkArray	unk68;	// 68
};

// 38
class BGSMaterialType : public TESForm
{
public:
	enum { kTypeID = kFormType_MaterialType };

	// C
	struct Data20
	{
		float	unk0;
		float	unk4;
		float	unk8;
	};

	TESForm				* parentType;		// 14 - init'd to 0
	StringCache::Ref	unk18;				// 18 - init'd to 0
	UInt32				unk1C;				// 1C - init'd to 0
	Data20				unk20;				// 20
	float				unk2C;				// 2C - init'd to 1
	UInt32				unk30;				// 30 - init'd to 0
	TESForm				* impactDataSet;	// 34 - init'd to 0
};

// 1C
class BGSMenuIcon : public TESForm
{
public:
	enum { kTypeID = kFormType_MenuIcon };

	// parents
	TESIcon	icon;	// 14
};

// 40
class BGSMessage : public TESForm
{
public:
	enum { kTypeID = kFormType_Message };

	// parents
	TESFullName		fullName;		// 14
	TESDescription	description;	// 1C

	// members
	TESForm					* icon;		// 28 - init'd to 0
	TESQuest				* quest;	// 2C - init'd to 0
	tList<BSFixedString>	buttons;	// 30
	UInt32					unk38;		// 38 - init'd to 1
	UInt32					unk3C;		// 3C - init'd to 2

	std::string GetDescription()
	{
		BSString str;
		CALL_MEMBER_FN(&description, Get)(&str, this, 'DESC');
		return str.Get();
	};
};

// 50
class BGSMovementType : public TESForm
{
public:
	enum { kTypeID = kFormType_MovementType };

	enum {
		kType_Walk = 0,
		kType_Run,
		kType_Max
	};

	enum
	{
		kDefaultData_Left = 0,
		kDefaultData_Right,
		kDefaultData_Forward,
		kDefaultData_Back,
		kDefaultData_RotateInPlace,
		kDefaultData_Max
	};

	enum
	{
		kAnimChangeThresholds_Direction = 0,
		kAnimChangeThresholds_MovementSpeed,
		kAnimChangeThresholds_RotationSpeed,
		kAnimChangeThresholds_Max
	};

	StringCache::Ref	typeId;									// 14
	float				data[kDefaultData_Max][kType_Max];		// 18
	float				rotateWhileMoving;						// 40 - init'd to pi
	float				thresholds[kAnimChangeThresholds_Max];	// 44 - read from INAM chunk
};

// 20
class BGSMusicTrackFormWrapper : public TESForm
{
public:
	enum { kTypeID = kFormType_MusicTrack };

	// parents
	BSIMusicTrack	track;	// 14

	// members
	UInt32	unk1C;			// 1C
};

// 4C
class BGSMusicType : public TESForm
{
public:
	enum { kTypeID = kFormType_MusicType };

	// parents
	BSIMusicType		type;	// 14

	// members
	StringCache::Ref	unk48;	// 48
};

// 20
class BGSOutfit : public TESForm
{
public:
	enum { kTypeID = kFormType_Outfit };

	tArray<TESForm*>	armorOrLeveledItemArray;	// 14
};

class BGSPerkEntry;

// 4C
class BGSPerk : public TESForm
{
public:
	enum { kTypeID = kFormType_Perk };

	// parents
	TESFullName		fullName;		// 14
	TESDescription	description;	// 1C
	TESIcon			icon;			// 28

	// members
	UInt8							unk30;		// 30
	UInt8							unk31;		// 31
	UInt8							unk32;		// 32
	UInt8							unk33;		// 33
	UInt8							unk34;		// 34
	UInt8							pad35[3];	// 35
	Condition						* conditions;	// 38
	tArray<BGSPerkEntry*>			perkEntries;	// 3C
	BGSPerk *						nextPerk;		// 48
};

// 124
class BGSRagdoll : public TESForm
{
public:
	enum { kTypeID = kFormType_Ragdoll };

	// parents
	TESModel	model;	// 14

	// members

	// 4C
	struct Data38
	{
		// 3C
		struct Data
		{
			float	unk00;	// 00 - init'd to .9
			float	unk04;	// 04 - init'd to .8
			float	unk08;	// 08 - init'd to .4
			float	unk0C;	// 0C - init'd to .2
			float	unk10;	// 10 - init'd to .1
			float	unk14;	// 14 - init'd to .3
			float	unk18;	// 18 - init'd to 0
			float	unk1C;	// 1C - init'd to 50
			float	unk20;	// 20 - init'd to 50
			float	unk24;	// 24 - init'd to 25
			float	unk28;	// 28 - init'd to 25
			float	unk2C;	// 2C - init'd to 50
			float	unk30;	// 30 - init'd to 50
			UInt32	unk34;	// 34 - init'd to 10000
			UInt32	unk38;	// 38 - init'd to 30000
		};

		UInt8						unk00;		// 00
		UInt8						pad01[3];	// 01
		Data						unk04;		// 04
		UnkArray	unk40;		// 40
	};

	// 38
	struct Data84
	{
		// 1C
		struct Data
		{
			UInt32	unk00;	// 00
			UInt32	unk04;	// 04
			UInt32	unk08;	// 08
			UInt32	unk0C;	// 0C
			UInt32	unk10;	// 10
			UInt32	unk14;	// 14
			UInt32	unk18;	// 18
		};

		UInt8						unk00;		// 00
		UInt8						pad01[3];	// 01
		Data						unk04;		// 04
		UnkArray	unk20;		// 20
		UnkArray	unk2C;		// 2C
	};

	// 1C
	struct DataBC
	{
		UInt8						unk00;		// 00
		UInt8						pad01[3];	// 01
		UnkArray	unk04;		// 04
		UnkArray	unk10;		// 10
	};

	// 24
	struct DataF4
	{
		// 18
		struct Data
		{
			UInt16	unk00;	// 00 - init'd to FFFF
			UInt16	unk02;	// 02 - init'd to FFFF
			UInt16	unk04;	// 04 - init'd to FFFF
			UInt8	unk06;	// 06 - init'd to 0
			UInt8	pad07;	// 07
			UInt32	unk08;	// 08 - init'd to 0
			UInt32	unk0C;	// 0C - init'd to 0
			float	unk10;	// 10 - init'd to 0.1
			UInt32	unk14;	// 14 - init'd to 0
		};

		UInt8		unk00;		// 00
		UInt8		pad01[3];	// 01
		BSString	unk04;		// 04
		Data		unk0C;		// 0C
	};

	UInt8	pad28[0x10];	// 028

	Data38	unk38;			// 038
	Data84	unk84;			// 084
	DataBC	unkBC;			// 0BC
	DataBC	unkD8;			// 0D8
	DataF4	unkF4;			// 0F4
	UInt8	pad118[4];		// 118
	UInt32	unk11C;			// 11C
	UInt32	unk120;			// 120
};

STATIC_ASSERT(sizeof(BGSRagdoll) == 0x124);

// 20
class BGSReferenceEffect : public TESForm
{
public:
	enum { kTypeID = kFormType_ReferenceEffect };

	// C
	struct Data
	{
		UInt32	unk0;
		UInt32	unk4;
		UInt32	unk8;
	};

	Data	unk14;
};

// 24
class BGSRelationship : public TESForm
{
public:
	enum { kTypeID = kFormType_Relationship };

	UInt32	unk14;	// 14
	UInt32	unk18;	// 18
	UInt32	unk1C;	// 1C
	UInt32	unk20;	// 20
};

// 28
class BGSReverbParameters : public TESForm
{
public:
	enum { kTypeID = kFormType_ReverbParam };

	BSIReverbType	reverbType;	// 14

	UInt16	unk18;	// 18 - init'd to 1250
	UInt16	unk1A;	// 1A - init'd to 800
	UInt8	unk1C;	// 1C - init'd to 100
	UInt8	unk1D;	// 1D - init'd to 100
	UInt8	unk1E;	// 1E - init'd to 0
	UInt8	unk1F;	// 1F - init'd to 0
	UInt8	unk20;	// 20 - init'd to 100
	UInt8	unk21;	// 21 - init'd to 0
	UInt8	unk22;	// 22 - init'd to 0
	UInt8	unk23;	// 23 - init'd to 100
	UInt32	unk24;	// 24 - ???
};

// 70
class BGSScene : public TESForm
{
public:
	enum { kTypeID = kFormType_Scene };

	UnkArray	unk14;	// 14
	UnkArray	unk20;	// 20
	UnkArray	unk2C;	// 2C
	UnkArray	unk38;	// 38
	UnkArray	unk44;	// 44
	TESForm	* quest;	// 50
	UInt32	unk54;		// 54
	void	* unk58;	// 58 - linked list
	UInt8	unk5C;		// 5C
	UInt8	unk5D;		// 5D
	UInt8	unk5E;		// 5E
	UInt8	unk5F;		// 5F
	UInt8	unk60;		// 60
	UInt8	unk61;		// 61
	UInt8	unk62;		// 62
	UInt8	unk63;		// 63
	UInt8	unk64;		// 64
	UInt8	pad65[3];	// 65
	UInt32	unk68;		// 68
	UInt32	unk6C;		// 6C
};

// 28
class BGSShaderParticleGeometryData : public TESForm
{
public:
	enum { kTypeID = kFormType_SPGD };

	UnkArray	unk14;		// 14
	TESTexture					texture;	// 20
};

// 38
class BGSSoundCategory : public TESForm
{
public:
	enum { kTypeID = kFormType_SoundCategory };

	// parents
	TESFullName			fullName;		// 14
	BSISoundCategory	soundCategory;	// 1C

	// members
	UInt32	unk20;	// 20 - init'd to 0
	UInt32	unk24;	// 24 - init'd to 0 
	UInt16	unk28;	// 28 - init'd to 0
	UInt16	unk2A;	// 2A - init'd to 0
	UInt16	unk2C;	// 2C - init'd to FFFF
	UInt16	unk2E;	// 2E - init'd to FFFF
	float	unk30;	// 30 - init'd to 1
	float	unk34;	// 34 - init'd to 1
};

// 20
class BGSSoundDescriptorForm : public TESForm
{
public:
	enum { kTypeID = kFormType_SoundDescriptor };

	virtual void	Unk_39(void);	// calls something on unk18

	// parents
	BGSSoundDescriptor	soundDescriptor;	// 14

	// members
	BGSStandardSoundDef		* standardSoundDef;	// 18
	StringCache::Ref	unk1C;		// 1C
};

// 28
class BGSSoundOutput : public TESForm
{
public:
	enum { kTypeID = kFormType_SoundOutput };

	// parents
	BSISoundOutputModel	soundOutputModel;	// 14

	// members
	UInt32	unk18;		// 18
	UInt32	unk1C;		// 1C
	UInt32	unk20;		// 20
	void	* unk24;	// 24
};

// 18
class BGSStoryManagerTreeForm : public TESForm
{
public:
	virtual UInt32	Unk_39(void);
	virtual UInt32	Unk_3A(UInt32 arg);
	virtual void *	Unk_3B(void);	// pure
	virtual void	Unk_3C(void);	// pure

	UInt32	unk14;	// 14
};

class BGSStoryManagerBranchNode;

// 2C
class BGSStoryManagerNodeBase : public BGSStoryManagerTreeForm
{
public:
	BGSStoryManagerBranchNode	* unk18;	// 18
	BGSStoryManagerNodeBase		* unk1C;	// 1C
	UInt32	unk20;							// 20
	UInt32	unk24;							// 24
	void	* unk28;						// 28 - linked list
};

// 38
class BGSStoryManagerBranchNode : public BGSStoryManagerNodeBase
{
public:
	enum { kTypeID = kFormType_StoryBranchNode };

	UnkArray	unk2C;	// 2C
};

// 3C
class BGSStoryManagerEventNode : public BGSStoryManagerBranchNode
{
public:
	enum { kTypeID = kFormType_StoryEventNode };

	UInt32	unk38;	// 38
};

// 88
class BGSStoryManagerQuestNode : public BGSStoryManagerNodeBase
{
public:
	enum { kTypeID = kFormType_StoryQuestNode };

	// 20
	struct Data38
	{
		UInt8	data[0x20];	// ###
	};

	// 20
	struct Data58
	{
		UInt8	data[0x20];	// ###
	};

	UnkArray	unk2C;	// 2C
	Data38		unk38;	// 38
	Data58		unk58;	// 58
	UInt32		unk78;	// 78
	UnkArray	unk7C;	// 7C
};

STATIC_ASSERT(sizeof(BGSStoryManagerQuestNode) == 0x88);

class BGSBaseAlias // Not actually a form, but its used like one in Papyrus
{
public:
	virtual ~BGSBaseAlias();
	virtual void		Unk01(void);
	virtual void		Unk02(void);
	virtual const char	* Unk03(void);

	enum { kTypeID = kFormType_Alias };

	StringCache::Ref name;	// 04
	TESQuest * owner;		// 08
	UInt32 aliasId;			// 0C
	UInt32 flags;			// 10
};

class BGSRefAlias : public BGSBaseAlias
{
public:
	enum { kTypeID = kFormType_ReferenceAlias };

	UInt32 unk14[0x04]; // One of these is the filltype/filltype filter
};

class BGSLocAlias : public BGSBaseAlias
{
public:
	enum { kTypeID = kFormType_LocationAlias };

	UInt32 unk14[0x08];
};

// 158
class TESQuest : public BGSStoryManagerTreeForm
{
public:
	enum { kTypeID = kFormType_Quest };

	// parents
	TESFullName	fullName;	// 18

	// members

	// 20
	struct Data03C
	{
		UInt8	data[0x20];	// ### todo
	};

	// 20
	struct Data05C
	{
		UInt8	data[0x20];	// ### todo
	};

	// 8
	struct Data07C
	{
		UInt32	unk0;
		UInt16	flags;
		UInt8	priority;
		UInt8	type;
	};

	struct Objective
	{
		StringCache::Ref	displayText;
		TESQuest			* owner;
		UInt32				unk08;
		UInt32				unk0C;
		UInt16				index;
		UInt16				unk12;
		UInt32				unk14;
	};

	// 8
	struct Data088
	{
		UInt32	unk0;
		UInt32	unk4;
	};

	// 20
	struct Data0A0
	{
		UInt8	data[0x1C];	// ### todo
	};

	UnkArray	unk020;		// 020
	UInt32		unk02C;		// 02C
	tArray<BGSBaseAlias*>	aliases;		// 030
	Data03C		unk03C;		// 03C
	Data05C		unk05C;		// 05C
	Data07C		unk07C;		// 07C
	UInt32		unk084;		// 084
	Data088		unk088;		// 088
	tList<Objective>		objectives;
	void		* unk098;	// 098 - linked list
	void		* unk09C;	// 09C - linked list
	Data0A0		unk0A0;		// 0A0
	void		* unk0BC;
	Data0A0		unk0C0;
	UInt32		unk0DC;
	UnkArray	unk0E0[6];	// 0E0
	UnkArray	unk128;		// 128
	void		* unk134;	// 134 - linked list
	UInt16		unk138;		// 138
	UInt8		unk13A;		// 13A
	UInt8		pad13B;		// 13B
	BSString	questID;	// 13C
	UInt32		unk144;		// 144
	UInt32		unk148;		// 148
	UnkArray	unk14C;		// 14C

	MEMBER_FN_PREFIX(TESQuest);
	DEFINE_MEMBER_FN(ForceRefTo, UInt32, 0x005728C0, UInt32 aliasId, TESObjectREFR * reference);	
};

STATIC_ASSERT(sizeof(TESQuest) == 0x158);

// 20
class BGSVoiceType : public TESForm
{
public:
	enum { kTypeID = kFormType_VoiceType };

	UInt8		unk14;		// 14
	UInt8		pad15[3];	// 15
	BSString	editorId;		// 18
};

// F4
class EffectSetting : public TESForm
{
public:
	enum { kTypeID = kFormType_EffectSetting };

	// parents
	TESFullName				fullName;			// 14
	BGSMenuDisplayObject	menuDisplayObject;	// 1C
	BGSKeywordForm			keywordForm;		// 24

	// members

	// 98
	struct Properties
	{
		enum {
			kEffectType_Hostile	=			0x00000001,
			kEffectType_Recover =			0x00000002,
			kEffectType_Detrimental =		0x00000004,
			kEffectType_NoHitEvent =		0x00000010,
			kEffectType_DispelKeywords =	0x00000100,
			kEffectType_NoDuration =		0x00000200,
			kEffectType_NoMagnitude =		0x00000400,
			kEffectType_NoArea =			0x00000800,
			kEffectType_FXPersist =			0x00001000,
			kEffectType_GloryVisuals =		0x00004000,
			kEffectType_HideInUI =			0x00008000,
			kEffectType_NoRecast =			0x00020000,
			kEffectType_Magnitude =			0x00200000,
			kEffectType_Duration =			0x00400000,
			kEffectType_Painless =			0x04000000,
			kEffectType_NoHitEffect =		0x08000000,
			kEffectType_NoDeathDispel =		0x10000000
		};

		enum {
			kArchetype_ValueMod = 0,
			kArchetype_Script,
			kArchetype_Dispel,
			kArchetype_CureDisease,
			kArchetype_Absorb,
			kArchetype_DualValueMod,
			kArchetype_Calm,
			kArchetype_Demoralize,
			kArchetype_Frenzy,
			kArchetype_Disarm,
			kArchetype_CommandSummoned,
			kArchetype_Invisibility,
			kArchetype_Light,
			kArchetype_Lock = 15,
			kArchetype_Open,
			kArchetype_BoundWeapon,
			kArchetype_SummonCreature,
			kArchetype_DetectLife,
			kArchetype_Telekinesis,
			kArchetype_Paralysis,
			kArchetype_Reanimate,
			kArchetype_SoulTrap,
			kArchetype_TurnUndead,
			kArchetype_Guide,
			kArchetype_WerewolfFeed,
			kArchetype_CureParalysis,
			kArchetype_CureAddiction,
			kArchetype_CurePoison,
			kArchetype_Concussion,
			kArchetype_ValueAndParts,
			kArchetype_AccumulateMagnitude,
			kArchetype_Stagger,
			kArchetype_PeakValueMod,
			kArchetype_Cloak,
			kArchetype_Werewolf,
			kArchetype_SlowTime,
			kArchetype_Rally,
			kArchetype_EnhanceWeapon,
			kArchetype_SpawnHazard,
			kArchetype_Etherealize,
			kArchetype_Banish,
			kArchetype_Disguise = 44,
			kArchetype_GrabActor,
			kArchetype_VampireLord
		};

		enum {
			kCastingType_ConstantEffect = 0,
			kCastingType_FireAndForget,
			kCastingType_Concentration
		};

		enum {
			kDeliveryType_Self = 0,
			kDeliveryType_Contact,
			kDeliveryType_Aimed,
			kDeliveryType_TargetActor,
			kDeliveryType_TargetLocation
		};

		enum {
			kVolume_Loud = 0,
			kVolume_Normal,
			kVolume_Silent,
			kVolume_VeryLoud
		};

		UInt32					flags;				// 00 - init'd to 0
		float					baseCost;			// 04 - init'd to 0
		TESForm*				primaryObject;		// 08 - init'd to 0
		UInt32					school;				// 0C - init'd to FFFFFFFF
		UInt32					resistance;			// 10 - init'd to FFFFFFFF
		UInt16					unk14;				// 14 - init'd to 0 
		UInt8					pad16[2];			// 16
		TESObjectLIGH*			light;				// 18 - init'd to 0
		float					taperWeight;		// 1C - init'd to 0
		TESEffectShader*		hitShader;			// 20 - init'd to 0
		TESEffectShader*		enchantShader;		// 24 - init'd to 0
		UInt32					level;				// 28 - init'd to 0
		UInt32					area;				// 2C - init'd to 0
		float					castingTime;		// 30 - init'd to 0
		float					taperCurve;			// 34 - init'd to 0
		float					taperDuration;		// 38 - init'd to 0
		float					secondAVWeight;		// 3C - init'd to 0 float - Always 0
		UInt32					archetype;			// 40 - init'd to 0
		UInt32					primaryValue;		// 44 - init'd to FFFFFFFF
		BGSProjectile*			projectile;			// 48 - init'd to 0
		BGSExplosion*			explosion;			// 4C - init'd to 0
		UInt32					castType;			// 50 - init'd to 3 cast type?
		UInt32					deliveryType;		// 54 - init'd to 5 cast type?
		UInt32					secondaryValue;		// 58 - init'd to FFFFFFFF
		BGSArtObject*			castingArt;			// 5C - init'd to 0
		BGSArtObject*			hitEffectArt;		// 60 - init'd to 0
		BGSImpactDataSet*		impactDataSet;		// 64 - init'd to 0
		float					skillUsageMult;		// 68 - init'd to 0
		BGSDualCastData*		dualCastData;		// 6C - init'd to 0
		float					dualCastingScale;	// 70 - init'd to 1
		BGSArtObject*			enchantArt;			// 74 - init'd to 0
		UInt32					unk78;				// 78 - init'd to 0
		UInt32					unk7C;				// 7C - init'd to 0
		SpellItem*				equipAbility;		// 80 - init'd to 0
		TESImageSpaceModifier*	imageSpaceMod;		// 84 - init'd to 0
		BGSPerk*				perk;				// 88 - init'd to 0
		UInt32					soundVolume;		// 8C - init'd to 1
		float					aiScore;			// 90 - init'd to 0
		float					delayTime;			// 94 - init'd to 0
	};

	struct SoundInfo
	{
		UInt32		value;
		TESSound*	sound;
	};

	UInt32						unk30;		// 30
	UInt32						unk34;		// 34
	Properties					properties;		// 38
	tList<EffectSetting>		counterEffects;
	tArray<SoundInfo>			sounds;		// D8
	StringCache::Ref			description;		// E4
	UInt32						unkE8;		// E8
	UInt32						unkEC;		// EC
	void						* unkF0;	// F0 - linked list

	UInt32 school() { return properties.school; }
	UInt32 level() { return properties.level; }
};

// B4
class NavMesh : public TESForm
{
public:
	enum { kTypeID = kFormType_NAVM };

	// parents
	TESChildCell	childCell;	// 14
	BSNavmesh		navmesh;	// 18
};

STATIC_ASSERT(sizeof(NavMesh) == 0xB4);

// 98
class NavMeshInfoMap : public TESForm
{
public:
	enum { kTypeID = kFormType_NAVI };

	// parents
	BSNavmeshInfoMap				infoMap;	// 14
	PrecomputedNavmeshInfoPathMap	pathMap;	// 1C

	// members

	// 20
	struct Data4C
	{
		UInt32	unk00;		// 00
		UInt32	unk04;		// 04
		UInt32	unk08;		// 08 - 0
		UInt32	unk0C;		// 0C - 0
		void	* unk10;	// 10 - ptr to 0xDEADBEEF
		UInt32	unk14;		// 14 - 0
		UInt32	unk18;		// 18
		UInt32	unk1C;		// 1C - 0
	};

	// 20
	struct Data6C
	{
		UInt32	unk00;		// 00
		UInt32	unk04;		// 04
		UInt32	unk08;		// 08 - 0
		UInt32	unk0C;		// 0C - 0
		UInt32	unk10;		// 10 - 0
		void	* unk14;	// 14 - ptr to 0xDEADBEEF
		UInt32	unk18;		// 18
		UInt32	unk1C;		// 1C
	};

	// 8
	struct Data8C
	{
		UInt32	unk0;
		UInt32	unk4;
	};

	UInt8	unk48;		// 48
	UInt8	pad49[3];	// 49
	Data4C	unk4C;		// 4C
	Data6C	unk6C;		// 6C
	Data8C	unk8C;		// 8C
	UInt8	unk94;		// 94
	UInt8	pad95[3];	// 95
};

STATIC_ASSERT(sizeof(NavMeshInfoMap) == 0x98);

// 50
class Script : public TESForm
{
public:
	enum { kTypeID = kFormType_Script };

	// 14
	struct Data
	{
		UInt32	unk00;	// 00
		UInt32	unk04;	// 04 - numRefs?
		UInt32	unk08;	// 08 - dataLength?
		UInt32	unk0C;	// 0C - varCount?
		UInt32	unk10;	// 10 - type?
	};

	struct RefListEntry
	{
		void			* data;
		RefListEntry	* next;
	};

	struct VarInfoEntry
	{
		void			* data;
		VarInfoEntry	* next;
	};

	Data			unk14;				// 14
	void			* unk28;			// 28
	void			* unk2C;			// 2C
	UInt32			unk30;				// 30
	float			questDelayTimer;	// 34
	float			secondsPassed;		// 38
	TESForm			* parentQuest;		// 3C
	RefListEntry	refList;			// 40
	VarInfoEntry	varList;			// 48
};

// 54
class TESClass : public TESForm
{
public:
	enum { kTypeID = kFormType_Class };

	// parents
	TESFullName		fullName;		// 14
	TESDescription	description;	// 1C
	TESTexture		texture;		// 28

	// members

	// 24
	struct Data30
	{
		UInt8	unk00[0x4];		// 00
		UInt8	trainSkill;		// 04 (AV-6)
		UInt8	maxTrainingLevel;	// 05
		UInt8	oneHanded;		// 06
		UInt8	twoHanded;		// 07
		UInt8	archery;
		UInt8	block;
		UInt8	smithing;
		UInt8	heavyArmor;
		UInt8	lightArmor;
		UInt8	pickPocket;
		UInt8	lockpicking;
		UInt8	sneak;
		UInt8	alchemy;
		UInt8	speech;
		UInt8	alteration;
		UInt8	conjuration;
		UInt8	destruction;
		UInt8	illusion;
		UInt8	restoration;
		UInt8	enchanting;
		float	bleedout;		// 18
		UInt32	voicePoints;	// 1C
		UInt8	health;			// 20
		UInt8	magicka;		// 21
		UInt8	stamina;		// 22
		UInt8	unk23;			// 23
	};

	// 30
	Data30	unk30;				// 30
};

STATIC_ASSERT(sizeof(TESClass) == 0x54);

// 48
class TESClimate : public TESForm
{
public:
	enum { kTypeID = kFormType_Climate };

	struct Data28
	{
		UInt32	unk0;
		UInt32	unk4;
	};

	TESModel	model;			// 14
	Data28		unk28;			// 28
	TESTexture	textures[2];	// 30
	UInt8		unk40[6];		// 40
	UInt8		pad46[2];		// 46
};

// 94
class TESCombatStyle : public TESForm
{
public:
	enum { kTypeID = kFormType_CombatStyle };

	// 28
	struct General	// CSGD
	{
		float	offensiveMult;		// 00 - init'd to 0.5
		float	defensiveMult;		// 04 - init'd to 0.5
		float	groupOffensiveMult;	// 08 - init'd to 1
		float	meleeMult;			// 0C - init'd to 1
		float	magicMult;			// 10 - init'd to 1
		float	rangedMult;			// 14 - init'd to 1
		float	shoutMult;			// 18 - init'd to 1
		float	unarmedMult;			// 1C - init'd to 1
		float	staffMult;		// 20 - init'd to 1
		float	avoidThreatChance;	// 24 - init'd to 0.2
	};

	// 20
	struct Melee	// CSME
	{
		float	attackStaggeredMult;		// 00 - init'd to 1
		float	powerAttackStaggeredMult;	// 04 - init'd to 1
		float	powerAttackBlockingMult;	// 08 - init'd to 1
		float	bashMult;					// 0C - init'd to 1
		float	bashRecoiledMult;			// 10 - init'd to 1
		float	bashAttackMult;				// 14 - init'd to 1
		float	bashPowerAttackMult;		// 18 - init'd to 1
		float	specialAttackMult;			// 1C - init'd to 0.1
	};

	// 10
	struct CloseRange	// CSCR
	{
		float	duelingCircleMult;		// 00 - init'd to 0.2
		float	duelingFallbackMult;	// 04 - init'd to 0.2
		float	flankingFlankDistance;	// 08 - init'd to 0.2
		float	flankingStalkTime;		// 0C - init'd to 0.2
	};

	// 4
	struct LongRange	// CSLR
	{
		float	strafeMult;		// 00 - init'd to 0.2
	};

	// 20
	struct Flight	// CSFL
	{
		float	hoverChance;		// 00 - init'd to 0.5
		float	diveBombChance;		// 04 - init'd to 1
		float	groundAttackChance;	// 08 - init'd to 0.5
		float	hoverTime;			// 0C - init'd to 0.5
		float	unk10;				// 10 - init'd to 0.5
		float	perchAttackChance;	// 14 - init'd to 0.5
		float	unk18;				// 18 - init'd to 0.5
		float	flyingAttackChance;	// 1C - init'd to 0.75
	};

	enum {
		kFlag_Dueling = 1,
		kFlag_Flanking = 2,
		kFlag_AllowDualWielding = 4
	};

	General	general;			// 14 - CSGD
	Melee	melee;				// 3C - CSME
	CloseRange	closeRange;		// 5C - CSCR
	LongRange	longRange;		// 6C - CSLR
	Flight	flight;				// 70 - CSFL
	UInt8	flags;				// 90 - DATA
	UInt8	pad91[3];			// 91
};

// 1D4
class TESEffectShader : public TESForm
{
public:
	enum { kTypeID = kFormType_EffectShader };

	// 190?
	struct Data
	{
		UInt8	pad[0x190];	// ###
	};

	Data		unk14;
	TESTexture	unk1A4;
	TESTexture	unk1AC;
	TESTexture	unk1B4;
	TESTexture	unk1BC;
	TESTexture	unk1C4;
	UInt32		unk1CC;	// BSIntrusiveRefCounted *
	UInt32		unk1D0;	// BSIntrusiveRefCounted *
};

// 28
class TESEyes : public TESForm
{
public:
	enum { kTypeID = kFormType_Eyes };

	// parents
	TESFullName	fullName;	// 14
	TESTexture	texture;	// 1C

	// members
	UInt8		unk24;		// 24
	UInt8		pad25[3];	// 25
};

// 98
class TESFaction : public TESForm
{
public:
	enum { kTypeID = kFormType_Faction };

	// parents
	TESFullName		fullName;	// 14
	TESReactionForm	reaction;	// 1C

	// members

	// 2C
	struct Data34
	{
		UInt32	pad[0x2C / 4];
	};

	// 20
	struct Data60
	{
		UInt32	pad[0x20 / 4];
	};

	// 8
	struct Data80
	{
		UInt32	unk0;	// 0
		UInt32	unk4;	// 4
	};

	UInt32	unk2C;	// 2C
	UInt32	unk30;	// 30
	Data34	unk34;	// 34
	Data60	unk60;	// 60
	Data80	unk80;	// 80
	UInt32	unk88;	// 88
	UInt32	unk8C;	// 8C
	float	unk90;	// 90
	UInt32	unk94;	// 94
};

// 24
class TESGlobal : public TESForm
{
public:
	enum { kTypeID = kFormType_Global };

	BSString	unk14;	// 14
	UInt8		unk1C;	// 1C - type?
	UInt8		pad[3];	// 1D
	UInt32		unk20;	// 20
};

// 50
class TESHair : public TESForm
{
public:
	// parents
	TESFullName	fullName;	// 14
	TESModel	model;		// 1C
	TESTexture	texture;	// 30

	// members
	UInt8		unk38;		// 38
	UInt8		pad39[3];	// 39
	TESModel	unk40;		// 3C
};

// 3C
class TESIdleForm : public TESForm
{
public:
	enum { kTypeID = kFormType_Idle };

	void				* unk14;			// 14 - linked list
	UInt8				loopMin;			// 18
	UInt8				loopMax;			// 19
	UInt8				flags;				// 1A
	UInt8				group;				// 1B
	UInt16				unk1C;				// 1C
	UInt8				pad1E[2];			// 1E
	UInt32				unk20;				// 20 - NiFormArray
	TESIdleForm			* relatedIdle1;		// 24
	TESIdleForm			* relatedIdle2;		// 28
	StringCache::Ref	fileName;			// 2C
	StringCache::Ref	animationEvent;		// 30
	BSString			editorId;			// 34
};

// 64
class TESImageSpace : public TESForm
{
public:
	enum { kTypeID = kFormType_ImageSpace };

	// 50
	struct Data14
	{
		float	unk00;	// 00 - init'd to 3
		float	unk04;	// 04 - init'd to 7
		float	unk08;	// 08 - init'd to .6
		float	unk0C;	// 0C - init'd to .5
		float	unk10;	// 10 - init'd to .15
		float	unk14;	// 14 - init'd to .15
		float	unk18;	// 18 - init'd to 1.8
		float	unk1C;	// 1C - init'd to 1.5
		float	unk20;	// 20 - init'd to 3.5, 3
		float	unk24;	// 24 - init'd to 1.8, .8
		float	unk28;	// 28 - init'd to 1.5
		float	unk2C;	// 2C - init'd to 1.1
		float	unk30;	// 30 - init'd to 0
		float	unk34;	// 34 - init'd to 0
		float	unk38;	// 38 - init'd to 0
		float	unk3C;	// 3C - init'd to 0
		float	unk40;	// 40 - init'd to 0
		UInt32	unk44;	// 44 - init'd to 0
		UInt32	unk48;	// 48 - init'd to 0
		float	unk4C;	// 4C - init'd to 2
	};

	Data14	unk14;	// 14
};

// 72C
// ### todo
class TESImageSpaceModifier : public TESForm
{
public:
	enum { kTypeID = kFormType_ImageSpaceMod };

	// 0F4
	struct Data014
	{
		// 8
		struct Data0CC
		{
			float	unk0;
			float	unk4;
		};

		UInt8	unk000;		// 000
		UInt8	pad001[3];	// 001
		float	unk004;		// 004
		// 008

		UInt8	todo008[0xC8 - 0x08];	// 008

		UInt8	unk0C8;		// 0C8
		UInt8	unk0C9[3];	// 0C9
		Data0CC	unk0CC;		// 0CC
		// 0D4

		UInt8	todo0D4[0xE0 - 0xD4];	// 0D4

		UInt8	unk0E0;		// 0E0
		UInt8	unk0E1;		// 0E1
		UInt8	pad0E2[2];	// 0E2
		// 0E4

		UInt8	todo0E4[0xF4 - 0xE4];	// 0E4
	};

	Data014				unk014;				// 014
	NiFloatInterpolator	unk108[0x15 * 2];	// 108
	NiFloatInterpolator	unk4F8;				// 4F8
	NiFloatInterpolator	unk510;				// 510
	NiColorInterpolator	unk528;				// 528
	NiColorInterpolator	unk54C;				// 54C
	NiFloatInterpolator	unk570;				// 570
	NiFloatInterpolator	unk588;				// 588
	NiFloatInterpolator	unk5A0;				// 5A0
	NiFloatInterpolator	unk5B8;				// 5B8
	NiFloatInterpolator	unk5D0;				// 5D0
	NiFloatInterpolator	unk5E8;				// 5E8
	NiFloatInterpolator	unk600;				// 600
	NiFloatInterpolator	unk618;				// 618
	NiFloatInterpolator	unk630;				// 630
	// 648

	UInt8				todo648[0x724 - 0x648];	// 348

	BSString			unk724;				// 724
};

STATIC_ASSERT(offsetof(TESImageSpaceModifier, unk108) == 0x108);
STATIC_ASSERT(offsetof(TESImageSpaceModifier, unk4F8) == 0x4F8);
STATIC_ASSERT(sizeof(TESImageSpaceModifier) == 0x72C);

// 2C
class TESLandTexture : public TESForm
{
public:
	enum { kTypeID = kFormType_LandTexture };

	struct Data24
	{
		UInt32	unk0;	// 0
		UInt32	unk4;	// 4
	};

	UInt32	unk14;		// 14
	UInt8	unk18;		// 18
	UInt8	unk19;		// 19
	UInt8	pad1A[2];	// 1A
	UInt32	unk1C;		// 1C
	UInt8	unk20;		// 20
	UInt8	pad21[3];	// 21
	Data24	unk24;		// 24
};

// 20
class TESLoadScreen : public TESForm
{
public:
	enum { kTypeID = kFormType_LoadScreen };

	void				* unk14;	// 14 - linked list
	TESObjectSTAT		* object;	// 18
	StringCache::Ref	unk1C;		// 1C
};

// 38
class TESObjectANIO : public TESForm
{
public:
	enum { kTypeID = kFormType_ANIO };

	TESModelTextureSwap	textureSwap;	// 14
	StringCache::Ref	unk30;			// 30
	StringCache::Ref	unk34;			// 34
};

// 8C
class TESObjectCELL : public TESForm
{
public:
	enum { kTypeID = kFormType_Cell };

	// parents
	TESFullName					fullName;	// 14

	// members

	// 8
	struct Data
	{
		UInt32	unk0;
		UInt32	unk4;
	};

	struct TVDT
	{
		struct TVDT1
		{
			UInt32	* unk00;	// array?
			UInt32	* unk04;	// array?
			UInt32	unk08;		// size?
		};

		struct TVDT0
		{
			TVDT1	* unk00;
			UInt32	unk04;
			UInt32	unk08;	// skip TVDT1 if > 1
		};

		UInt32	unk00;
		UInt32	unk04;
		UInt32	unk08;
		TVDT0	* unk0C;
		// ...
	};


	Data						unk1C;		// 1C
	Data						unk24;		// 24
	UInt16						unk2C;		// 2C	1 - no 3C
	UInt16						unk2E;		// 2E
	UInt8						unk30;		// 30
	UInt8						unk31;		// 31
	UInt8						unk32;		// 32
	UInt8						pad33;		// 33
	TESAIForm::Data				unk34;		// 34 - ExtraDataList
	TVDT						* unk3C;	// 3C
	UInt32						unk40;		// 40
	UInt32						unk44;		// 44
	UInt32						unk48;		// 48
	tArray<TESObjectREFR*> objectList;		// 4C
	UInt32						unk58;		// 58
	UInt32						unk5C;		// 5C
	UnkArray					unk60;		// 60
	UnkArray					unk6C;		// 6C
	Data						unk78;		// 78
	UInt32						unk80;		// 80
	UInt32						unk84;		// 84
	UInt32						unk88;		// 88

	MEMBER_FN_PREFIX(TESObjectCELL);
	DEFINE_MEMBER_FN(GetNorthRotation, double, 0x004C0FC0);
};
STATIC_ASSERT(offsetof(TESObjectCELL, objectList) == 0x4C);

// 28
class TESObjectLAND : public TESForm
{
public:
	enum { kTypeID = kFormType_Land };

	// parents
	TESChildCell	childCell;	// 14

	// members
	UInt32		unk18;	// 18
	UInt32		unk1C;	// 1C
	UInt32		unk20;	// 20
	UInt32		unk24;	// 24
};

// 80
class TESPackage : public TESForm
{
public:
	enum { kTypeID = kFormType_Package };

	virtual bool	Unk_39(UInt32 arg0, UInt32 arg1, UInt32 arg2, UInt32 arg3);
	virtual bool	Unk_3A(UInt32 arg0, UInt32 arg1, UInt32 arg2, UInt32 arg3, UInt32 arg4);
	virtual bool	Unk_3B(UInt32 arg0, UInt32 arg1);
	virtual bool	Unk_3C(UInt32 arg0, UInt32 arg1);
	virtual bool	Unk_3D(UInt32 arg0);

	enum
	{
		kPackageType_Find = 0,
		kPackageType_Follow,
		kPackageType_Escort,
		kPackageType_Eat,
		kPackageType_Sleep,
		kPackageType_Wander,
		kPackageType_Travel,
		kPackageType_Accompany,
		kPackageType_UseItemAt,
		kPackageType_Ambush,
		kPackageType_FleeNotCombat,
		kPackageType_CastMagic,
		kPackageType_Sandbox,
		kPackageType_Patrol,
		kPackageType_Guard,
		kPackageType_Dialogue,
		kPackageType_UseWeapon,
		kPackageType_Find2,
		kPackageType_Package,
		kPackageType_PackageTemplate,
		kPackageType_Activate,
		kPackageType_Alarm,
		kPackageType_Flee,
		kPackageType_Trespass,
		kPackageType_Spectator,
		kPackageType_ReactToDead,
		kPackageType_GetUpFromChair,
		kPackageType_DoNothing,
		kPackageType_InGameDialogue,
		kPackageType_Surface,
		kPackageType_SearchForAttacker,
		kPackageType_AvoidPlayer,
		kPackageType_ReactToDestroyedObject,
		kPackageType_ReactToGrenadeOrMine,
		kPackageType_StealWarning,
		kPackageType_PickPocketWarning,
		kPackageType_MovementBlocked
	};

	// C
	struct Data30
	{
		UInt8	unk00;		// 0
		UInt8	unk01;		// 1
		UInt8	unk02;		// 2
		UInt8	unk03;		// 3
		UInt8	unk04;		// 4
		UInt8	pad05[3];	// 5
		UInt32	unk08;		// 8
	};

	// 10
	struct Data
	{
		UInt32	unk00;	// 00
		UInt32	unk04;	// 04
		UInt32	unk08;	// 08
		UInt32	unk0C;	// 0C
	};

	UInt32	packageFlags;	// 14
	UInt8	type;			// 18 - see kPackageType_ enum
	UInt8	unk19;			// 19
	UInt8	unk1A;			// 1A
	UInt8	pad1B;			// 1B
	UInt16	unk1C;			// 1C
	UInt16	unk1E;			// 1E
	UInt32	unk20;			// 20
	UInt32	unk24;			// 24
	UInt32	unk28;			// 28
	UInt32	unk2C;			// 2C
	Data30	unk30;			// 30
	void	* unk3C;		// 3C - linked list
	UInt32	unk40;			// 40
	UInt32	unk44;			// 44
	Data	unk48;			// 48
	Data	unk58;			// 58
	Data	unk68;			// 68
	UInt32	unk78;			// 78
	UInt32	unk7C;			// 7C - incremented in dtor
};

// 8C
class AlarmPackage : public TESPackage
{
public:
	UInt32	unk80;		// 80
	UInt32	unk84;		// 84
	UInt8	unk88;		// 88
	UInt8	pad89[3];	// 89
};

// A0
class DialoguePackage : public TESPackage
{
public:
	UInt32	unk80;		// 80
	UInt32	unk84;		// 84
	UInt32	unk88;		// 88
	UInt32	unk8C;		// 8C
	UInt8	unk90;		// 90
	UInt8	unk91;		// 91
	UInt8	pad92[2];	// 92
	UInt32	unk94;		// 94
	UInt32	unk98;		// 98
	UInt8	unk9C;		// 9C
	UInt8	pad9D[3];	// 9D
};

// AC
class FleePackage : public TESPackage
{
public:
	struct Data8C
	{
		UInt32	unk0;	// 0
		UInt32	unk4;	// 4
		UInt32	unk8;	// 8
	};

	UnkArray	unk80;	// 80
	Data8C	unk8C;		// 8C
	float	unk98;		// 98
	UInt32	unk9C;		// 9C
	UInt32	unkA0;		// A0
	UInt8	unkA4;		// A4
	UInt8	unkA5;		// A5
	UInt8	unkA6;		// A6
	UInt8	unkA7;		// A7
	UInt8	unkA8;		// A8
	UInt8	padA9[3];	// A9
};

// B0
class SpectatorPackage : public TESPackage
{
public:
	struct Data94
	{
		UInt32	unk0;	// 0
		UInt32	unk4;	// 4
		UInt32	unk8;	// 8
	};

	UInt32	unk80;		// 80
	UInt32	pad84;		// 84 - not init'd
	UInt32	unk88;		// 88
	UInt32	unk8C;		// 8C
	UInt8	unk90;		// 90
	UInt8	pad91[3];	// 91
	Data94	unk94;		// 94
	UnkArray	unkA0;	// A0
	UInt32	padAC;		// AC - not init'd
};

// 98
class TrespassPackage : public TESPackage
{
public:
	UInt32	unk80;	// 80
	UInt32	unk84;	// 84
	UInt32	unk88;	// 88
	UInt32	unk8C;	// 8C
	UInt32	unk90;	// 90
	UInt32	unk94;	// 94
};

// 2BC
class TESRace : public TESForm
{
public:
	enum { kTypeID = kFormType_Race };

	enum {
		kRace_Playable						= 0x00000001,
		kRace_FaceGenHead					= 0x00000002,
		kRace_Child							= 0x00000004,
		kRace_TiltFrontBack					= 0x00000008,
		kRace_TiltLeftRight					= 0x00000010,
		kRace_NoShadow						= 0x00000020,
		kRace_Swims							= 0x00000040,
		kRace_Flies							= 0x00000080,
		kRace_Walks							= 0x00000100,
		kRace_Immobile						= 0x00000200,
		kRace_NotPushable					= 0x00000400,
		kRace_NoCombatInWater				= 0x00000800,
		kRace_NoRotatingToHeadTrack			= 0x00001000,
		kRace_UseHeadTrackAnim				= 0x00008000,
		kRace_SpellsAlignWithMagicNode		= 0x00010000,
		kRace_UseWorldRaycasts				= 0x00020000,
		kRace_AllowRagdollCollision			= 0x00040000,
		kRace_CantOpenDoors					= 0x00100000,
		kRace_AllowPCDialogue				= 0x00200000,
		kRace_NoKnockdowns					= 0x00400000,
		kRace_AllowPickpocket				= 0x00800000,
		kRace_AlwaysUseProxyController		= 0x01000000,
		kRace_OverlayHeadParts				= 0x04000000,
		kRace_OverrideHeadParts				= 0x08000000,
		kRace_AllowMultipleMembraneShaders	= 0x20000000,
		kRace_CanDualWield					= 0x40000000,
		kRace_AvoidsRoads					= 0x80000000,
	};

	// parents
	TESFullName			fullName;		// 014
	TESDescription		description;	// 01C
	TESSpellList		spellList;		// 028
	BGSSkinForm			skin;			// 030
	BGSBipedObjectForm	biped;			// 038
	BGSKeywordForm		keyword;		// 044
	BGSAttackDataForm	attackData;		// 050

	// members

	enum {
		kRace_NumSkillBonuses = 7
	};

	// 84
	struct Data
	{
		struct SkillBonus
		{
			UInt8 skill;
			UInt8 bonus;
		};
		SkillBonus skillBonus[kRace_NumSkillBonuses];
		UInt8	pad0E[2];			// 0E
		float	height[2];			// 10
		float	weight[2];			// 18
		UInt32	raceFlags;			// 20 - not init'd
		float	health;				// 24
		float	magicka;			// 28
		float	stamina;			// 2C
		float	carryweight;		// 30
		float	mass;				// 34
		float	accelRate;			// 38
		float	deaccelRate;		// 3C
		UInt32	unk40;				// 40 - init'd to 1 - Flags?
		UInt32	unk44;				// 44 - init'd to FFFFFFFF
		UInt32	unk48;				// 48 - init'd to FFFFFFFF - Flags?
		UInt32	unk4C;				// 4C - not init'd
		float	injuredHealthPct;	// 50 - init'd to FFFFFFFF
		float	healthRegen;		// 54
		float	manaRegen;			// 58
		float	staminaRegen;		// 5C
		float	unarmedDamage;		// 60
		float	handReach;			// 64
		UInt32	unk68;				// 68 - init'd to FFFFFFFF - Flags?
		float	aimAngleTolerance;	// 6C
		UInt32	unk70;				// 70 - init'd to 0
		float	angleAcceleration;	// 74
		float	angleTolerance;		// 78
		UInt32	unk7C;			// 7C
		UInt32	unk80;			// 80
		UInt32	unk84;			// 84
		float	unk88;			// 88
		float	unk8C;
		float	unk90;
		float	unk94;
		float	unk98;
		float	unk9C;
		float	unk100;
		float	unk104;
		float	unk10C;
	};

	TESModel					models[2];			// 058
	Data						data;				// 080
	BGSTextureModel				textureModel[2];	// 12C
	BGSBehaviorGraphModel		behaviorGraph[2];	// 154
	StringCache::Ref			behaviorPath[2];	// 17C
	StringCache::Ref			behaviorName[2];	// 184
	BGSVoiceType				* voiceTypes[2];	// 18C
	BGSBodyPartData				* bodyPartData;		// 190
	TESForm						* decapitateArmor[2];	// 194
	UnkArray					unk180[2];
	void						* unk198[4];
	void						* unk1A8[2]; // AttackAnimationArrayMap
	StringCache::Ref			editorId;
	BGSMaterialType				* impactMaterial;
	BGSImpactDataSet			* meleeImpact;
	BGSArtObject				* decapitateBloodArt;
	BGSSoundDescriptorForm		* openCorpseSound;
	BGSSoundDescriptorForm		* closeCorpseSound;
	StringCache::Ref			bipedObjectNames[0x20];
	tArray<BGSEquipSlot*>		slotRestrictions;
	UInt32						unk274;
	BGSEquipSlot				* unarmedEquipSlot;
	TESRace						* morphRace;
	TESRace						* armorRace;
	UnkArray					unk284;
	UnkArray					unk290;
	UInt8						unk29C[0x18];

	struct CharGenData
	{
		struct TintOption
		{
			UInt32					unk00;			// 00
			TESTexture				texture;		// 04
			BGSColorForm			* defaultColor;	// 0C
			tArray<BGSColorForm*>	colors;			// 10
			tArray<float>			alpha;			// 14
			tArray<UInt32>			unk18;			// 18
		};
		UInt32	unk00[0x90 >> 2];					// 00 - Don't know what this is yet, mostly FFFFFFFF
		tArray<TintOption*>		* tintData;			// 90
		tArray<BGSTextureSet*>	* textureSet;		// 94
		BGSTextureSet			* defaultTexture;	// 98
		tArray<TESNPC*>			* presets;			// 9C
		tArray<BGSColorForm*>	* colors;			// A0
		BGSColorForm			* defaultColor;		// A4
		tArray<BGSHeadPart*>	* headParts;		// A8
	};

	CharGenData					* chargenData[2];
};
STATIC_ASSERT(offsetof(TESRace::CharGenData, tintData) == 0x90);

//STATIC_ASSERT(offsetof(TESRace, unk1B0) == 0x1B0);
//STATIC_ASSERT(sizeof(TESRace) == 0x29C);

// 30
class TESRegion : public TESForm
{
public:
	enum { kTypeID = kFormType_Region };

	virtual bool	Unk_39(void);

	// C
	struct Data14
	{
		UInt32	unk0;		// 0
		UInt32	unk4;		// 4
		UInt8	unk8;		// 8
		UInt8	unk9[3];	// 9
	};

	// 8
	struct Data18
	{
		UInt32	unk0;
		UInt32	unk4;
	};

	// C
	struct Data24
	{
		UInt32	unk0;
		UInt32	unk4;
		UInt32	unk8;
	};

	Data14	* unk14;	// 14
	Data18	* unk18;	// 18
	TESWorldSpace	* worldSpace;		// 1C
	UInt32	unk20;		// 20
	Data24	unk24;		// 24
};

// 5C
class TESShout : public TESForm
{
public:
	enum { kTypeID = kFormType_Shout };

	// parents
	TESFullName				fullName;		// 14
	BGSMenuDisplayObject	menuDispObject;	// 1C
	BGSEquipType			equipType;		// 24
	TESDescription			description;	// 2C

	// members

	// C
	struct Words
	{
		enum {
			kNumWords = 3
		};
		TESWordOfPower *	word;
		SpellItem *			spell;
		float				recoverytime;
	};

	Words	words[Words::kNumWords];	// 38
};

// 38
class TESTopic : public TESForm
{
public:
	enum { kTypeID = kFormType_Topic };

	// parents
	TESFullName			fullName;	// 14

	// members
	UInt8				unk1C[4];	// 1C
	UInt32				unk20;		// 20
	UInt32				unk24;		// 24
	UInt32				unk28;		// 28
	void				** unk2C;	// 2C - buf[unk30]
	UInt32				unk30;		// 30 - len
	StringCache::Ref	unk34;		// 34
};

// 2C
class TESTopicInfo : public TESForm
{
public:
	enum { kTypeID = kFormType_TopicInfo };

	enum
	{
		kDialogFlag_Goodbye =					1 << 0,
		kDialogFlag_Random =					1 << 1,
		kDialogFlag_SayOnce =					1 << 2,
		kDialogFlag_RandomEnd =					1 << 5,
		kDialogFlag_InvisibleContinue =			1 << 6,
		kDialogFlag_ForceSubtitle =				1 << 8,
		kDialogFlag_CanMoveWhileGreeting =		1 << 10,
		kDialogFlag_HasNoLipFile =				1 << 11,
		kDialogFlag_RequiresPostProcessing =	1 << 12,
		kDialogFlag_SpendsFavorPoints =			1 << 14,
	};

	UInt32		unk14;				// 14
	UInt32		unk18;				// 18 - show response data from info?
	Condition	* conditions;		// 1C - linked list
	UInt16		unk20;				// 20 - init'd to FFFF
	UInt8		unk22;				// 22
	UInt8		favorLevel;			// 23
	UInt16		dialogFlags;		// 24
	UInt16		hoursUntilReset;	// 26
	void		* unk28;			// 28 - quest pointer?
};

STATIC_ASSERT(sizeof(TESTopicInfo) == 0x2C);

// 1D4
class TESWaterForm : public TESForm
{
public:
	enum { kTypeID = kFormType_Water };

	// parents
	TESFullName			fullName;		// 14
	TESAttackDamageForm	attackDamage;	// 1C

	// members

	// 10
	struct Data2C
	{
		UInt32	unk0;	// 0
		UInt32	unk4;	// 4
		UInt32	unk8;	// 8
		UInt32	unkC;	// C
	};

	// E4
	struct Data80
	{
		float	unk00;		// 00 - init'd to .1
		float	unk04;		// 04 - init'd to 90
		float	unk08;		// 08 - init'd to .5
		float	unk0C;		// 0C - init'd to 1
		float	unk10;		// 10 - init'd to 50
		float	unk14;		// 14 - init'd to .5
		float	unk18;		// 18 - init'd to .025
		UInt32	unk1C;		// 1C - init'd to 0
		UInt32	unk20;		// 20 - init'd to 0
		UInt32	unk24;		// 24 - init'd to 0
		UInt32	unk28;		// 28 - init'd to 00808000
		UInt32	unk2C;		// 2C - init'd to 00190000
		UInt32	unk30;		// 30 - init'd to 00FFFFFF
		UInt8	unk34;		// 34 - init'd to 0
		UInt8	pad35[3];	// 35
		float	unk38;		// 38 - init'd to 0.1
		float	unk3C;		// 3C - init'd to 0.6
		float	unk40;		// 40 - init'd to 0.985
		float	unk44;		// 44 - init'd to 2
		float	unk48;		// 48 - init'd to .01
		float	unk4C;		// 4C - init'd to .4
		float	unk50;		// 50 - init'd to .6
		float	unk54;		// 54 - init'd to .985
		float	unk58;		// 58 - init'd to 10
		float	unk5C;		// 5C - init'd to .05
		float	unk60;		// 60 - init'd to 300
		UInt32	unk64;		// 64 - init'd to 0
		UInt32	unk68;		// 68 - init'd to 0
		UInt32	unk6C;		// 6C - init'd to 0
		UInt32	unk70;		// 70 - init'd to 0
		UInt32	unk74;		// 74 - init'd to 0
		UInt32	unk78;		// 78 - init'd to 0
		float	unk7C;		// 7C - init'd to 300
		float	unk80;		// 80 - init'd to 300
		float	unk84;		// 84 - init'd to 1
		UInt32	unk88;		// 88 - not init'd
		float	unk8C;		// 8C - init'd to 1
		UInt32	unk90;		// 90 - init'd to 0
		float	unk94;		// 94 - init'd to 1000
		float	unk98;		// 98 - init'd to 250
		float	unk9C;		// 9C - init'd to 100
		float	unkA0;		// A0 - init'd to 1
		float	unkA4;		// A4 - init'd to 10000
		float	unkA8;		// A8 - init'd to 1
		float	unkAC;		// AC - init'd to 100
		float	unkB0;		// B0 - init'd to 100
		float	unkB4;		// B4 - init'd to 100
		UInt32	unkB8;		// B8 - init'd to 0
		UInt32	unkBC;		// BC - init'd to 0
		UInt32	unkC0;		// C0 - init'd to 0
		float	unkC4;		// C4 - init'd to 1
		float	unkC8;		// C8 - init'd to 1
		float	unkCC;		// CC - init'd to 1
		float	unkD0;		// D0 - init'd to 1
		float	unkD4;		// D4 - init'd to 1
		float	unkD8;		// D8 - init'd to 1
		float	unkDC;		// DC - init'd to 1
		float	unkE0;		// E0 - init'd to 1
	};

	// C
	struct Data1C8
	{
		UInt32	unk0;	// 0
		UInt32	unk4;	// 4
		UInt32	unk8;	// 8
	};

	UInt8		unk024;		// 024
	UInt8		pad025[3];	// 025
	UInt32		unk028;		// 028
	Data2C		unk02C;		// 02C
	Data2C		unk03C;		// 03C
	Data2C		unk04C;		// 04C
	TESTexture	unk05C;		// 05C
	TESTexture	unk064;		// 064
	TESTexture	unk06C;		// 06C
	UInt8		unk074;		// 074
	UInt8		unk075;		// 075
	UInt8		pad076[2];	// 076
	UInt32		unk078;		// 078
	UInt32		unk07C;		// 07C
	Data80		unk080;		// 080
	UInt32		unk164[3];	// 164
	UInt32		unk170;		// 170
	UInt32		unk174;		// 174
	UInt32		unk178;		// 178
	UInt32		unk17C;		// 17C
	UInt32		unk180;		// 180
	float		unk184;		// 184
	float		unk188;		// 188
	float		unk18C;		// 18C
	float		unk190;		// 190
	UInt32		unk194;		// 194
	UInt32		unk198;		// 198
	UInt32		unk19C;		// 19C
	UInt32		unk1A0;		// 1A0
	UInt32		unk1A4;		// 1A4
	UInt32		unk1A8;		// 1A8
	void		* unk1AC;	// 1AC - refcounted ptr
	UInt32		unk1B0;		// 1B0
	UInt8		unk1B4;		// 1B4
	UInt8		pad1B5[3];	// 1B5
	UInt32		unk1B8;		// 1B8
	Data1C8		unk1BC;		// 1BC
	Data1C8		unk1C8;		// 1C8
};

STATIC_ASSERT(sizeof(TESWaterForm) == 0x1D4);

// 760
class TESWeather : public TESForm
{
public:
	enum { kTypeID = kFormType_Weather };

	enum { kNumColorTypes = 17 };
	enum { kNumTimeOfDay = 4 };

	enum ColorTypes {
		kColorType_SkyUpper = 0,
		kColorType_FogNear = 1,
		kColorType_Unk = 2,
		kColorType_Ambient = 3,
		kColorType_Sunlight = 4,
		kColorType_Sun = 5,
		kColorType_Stars = 6,
		kColorType_SkyLower = 7,
		kColorType_Horizon = 8,
		kColorType_EffectLighting = 9,
		kColorType_CloudLODDiffuse = 10,
		kColorType_CloudLODAmbient = 11,
		kColorType_FogFar = 12,
		kColorType_SkyStatics = 13,
		kColorType_WaterMultiplier = 14,
		kColorType_SunGlare = 15,
		kColorType_MoonGlare = 16
	};

	enum TimeOfDay {
		kTime_Sunrise = 0,
		kTime_Day = 1,
		kTime_Sunset = 2,
		kTime_Night = 3
	};

	// 110
	struct ColorType
	{
		UInt32	time[kNumTimeOfDay];
	};

	// 13
	struct General
	{
		//UInt8	unk00[0x13];			// 00
		UInt8	unk00[3];
		UInt8	transDelta;				// Div 1000
		UInt8	sunGlare;				// Div 256
		UInt8	sunDamage;				// Div 256
		UInt16	pad06;
		UInt32	unk08;
		UInt32	unk0C;
		UInt8	unk10;
		UInt8	windDirection;			// Div (256/360)
		UInt8	windDirRange;			// Div (256/180)
		UInt8	pad13;
	};

	// 20
	struct FogDistance
	{
		float	nearDay;
		float	farDay;				// 10
		float	nearNight;			// 14
		float	farNight;			// 18
		float	powerDay;			// 1C
		float	powerNight;
		float	maxDay;
		float	maxNight;
	};

	struct CloudAlpha
	{
		float	time[kNumTimeOfDay];
	};

	TESTexture1024	texture[0x20];					// 014
	UInt8			unk114[0x20];					// 114 - cleared to 0x7F
	UInt8			unk134[0x20];					// 134 - cleared to 0x7F
	UInt8			unk154[0x200];					// 154
	CloudAlpha		cloudAlpha[0x20];				// 354
	UInt32			unk554;							// 554
	General			general;						// 558
	//UInt8			pad56B;							// 56B
	FogDistance		fogDistance;					// 56C
	ColorType		colorTypes[kNumColorTypes];		// 58C
	TESAIForm::Data	unk69C;							// 69C
	UnkArray		unk6A4;							// 6A4
	UInt32			pad6B0;							// 6B0 - not init'd
	TESImageSpace	* imageSpaces[kNumTimeOfDay];
	BGSLightingTemplate::Data::Color	directionalAmbient[kNumTimeOfDay];	// 6C4
	TESModel		unk744;	// 744
	BGSShaderParticleGeometryData	* particleShader;	// 758
	BGSReferenceEffect				* referenceEffect;	// 75C
};

STATIC_ASSERT(sizeof(TESWeather) == 0x760);

// 20
class TESWordOfPower : public TESForm
{
public:
	enum { kTypeID = kFormType_WordOfPower };

	// parents
	TESFullName			fullName;	// 14

	// members
	StringCache::Ref	word;		// 1C
};

// 174
class TESWorldSpace : public TESForm
{
public:
	enum { kTypeID = kFormType_WorldSpace };

	// parents
	TESFullName		fullName;	// 14
	TESTexture		texture;	// 1C

	// members

	// 4
	struct Data038
	{
		UInt16	unk0;
		UInt16	unk2;
	};

	// 020
	struct Data03C
	{
		UInt8	unk0[0x20];
	};

	// ?
	struct OFFSET_DATA { };

	// 40
	struct Data12C
	{
		// this struct keeps showing up everywhere
		struct Entry
		{
			UInt8	pad00[8];	// 00
			UInt32	unk08;		// 08
			UInt32	unk0C;		// 0C
			UInt32	unk10;		// 10
			void	* unk14;	// 14
			UInt8	pad18[4];	// 18
			UInt32	unk1C;		// 1C
		};

		Entry	unk00;
		Entry	unk20;
	};

	typedef NiTPointerMap <int, TESObjectCELL *>	CellMap;

	CellMap	* cellMap;	// 024
	UInt32	unk028;		// 028
	UInt32	unk02C;		// 02C
	UInt32	unk030;		// 030
	UInt8	worldSpaceFlags;	// 034
	UInt8	pad035;		// 035
	UInt16	unk036;		// 036
	Data038	unk038;		// 038
	Data03C	unk03C;		// 03C
	UnkArray	unk05C;	// 05C
	UInt32	unk068;		// 068
	UInt32	unk06C;		// 06C
	TESQuest::Data05C	unk070;	// 070
	void	* unk090;	// 090 - refcounted ptr
	UInt32	unk094;		// 094
	UInt32	unk098;		// 098
	UInt32	unk09C;		// 09C
	UInt32	unk0A0;		// 0A0
	UInt32	unk0A4;		// 0A4
	UInt32	unk0A8;		// 0A8
	UInt32	unk0AC;		// 0AC
	UInt32	unk0B0;		// 0B0
	UInt32	unk0B4;		// 0B4
	UInt32	unk0B8;		// 0B8
	UInt8	unk0BC[0x10];	// 0BC
	float	unk0CC;		// 0CC
	UInt32	unk0D0;		// 0D0
	UInt32	unk0D4;		// 0D4
	UInt32	unk0D8;		// 0D8
	UInt32	unk0DC;		// 0DC
	UInt32	unk0E0;		// 0E0
	UInt32	unk0E4;		// 0E4
	UInt32	unk0E8;		// 0E8
	UInt32	unk0EC;		// 0EC
	NiTMap <TESFile *, TESWorldSpace::OFFSET_DATA *>	unk0F0;	// 0F0
	BSString	unk100;	// 100
	void		* unk108;	// 108
	UInt32		unk10C;	// 10C
	float		unk110;	// 110
	UInt32		unk114;	// 114
	UInt32		unk118;	// 118
	TESTexture	unk11C;	// 11C
	TESTexture	unk124;	// 124
	Data12C		unk12C;	// 12C
	UInt32		unk16C;	// 16C
	UInt32		unk170;	// 170
};

STATIC_ASSERT(sizeof(TESWorldSpace) == 0x174);

// 4
class IFormFactory
{
public:
	IFormFactory();
	virtual ~IFormFactory();

	virtual TESForm *		Create(void) = 0;
	virtual const char *	GetName(void) = 0;
	virtual UInt32			GetTypeID(void) = 0;
	virtual const char *	Unk_04(void);	// return NULL
	virtual UInt32			Unk_05(void);	// return 0x67
	virtual UInt32			Unk_06(void);	// return 9

//	void		** _vtbl;	// 00

	static IFormFactory *	GetFactoryForType(UInt32 type)
	{
		if(type > kFormType_Max)
			return NULL;

		IFormFactory	** kFactoryList = (IFormFactory **)0x012E57B0;

		return kFactoryList[type];
	}
};

// 8
class ConcreteFormFactory : public IFormFactory
{
public:
	const char	* name;		// 04
};
