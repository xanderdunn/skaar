#pragma once

#include "GameTypes.h"
#include "GameForms.h"
#include "GameObjects.h"
#include "GameReferences.h"
#include "GameResources.h"

class BSFile;

struct FormRecordData
{
	UInt8		typeID;		// corresponds to kFormType_XXX
	UInt32		typeCode;	// i.e. 'GMST', 'FACT'
	UInt32		unk08;		// only seen zero
};

struct ChunkHeader
{
	UInt32	type : 4;	// i.e. 'XGRD', 'DATA'
	UInt16	size : 2;
};

struct ModInfo		// referred to by game as TESFile
{
	ModInfo();
	~ModInfo();

	// 18 info about currently loading form
	struct FormInfo
	{
		UInt32		recordType;			// 00 i.e. 'FACT', 'GMST'
		UInt32		unk04;				// 04 looks like size of entire record
		UInt32		formFlags;			// 08 copied to TESForm->flags
		UInt32		formID;				// 0C 
		UInt32		unk10;				// 10
		UInt16		unk14;				// 14 always initialized to 0F on SaveForm. 
		UInt16		unk16;
	};

	tList<UInt32>						unkList;			// 000
	UInt32 /*NiTPointerMap<TESFile*>*/	* pointerMap;		// 008
	UInt32								unk00C;				// 00C
	BSFile*								unkFile;			// 010
	UInt32								unk014;				// 014 
	void								* unk018;			// 018 seen all zeroes. size unknown
	void								* unk01C;			// 01C as above
	char								name[0x104];		// 020
	char								filepath[0x104];	// 124
	UInt32								unk228;				// 228
	UInt32								unk22C;				// init'd to dword_F469CC (0x2800) same val as BSFile+10?
	UInt32								unk230;				// 230
	UInt32								unk234;				// 234
	UInt32								unk238;				// 238
	UInt32								unk23C;				// 23C
	FormInfo							formInfo;			// 240
	ChunkHeader							subRecord;			// 258
	UInt32								unk260;				// 260
	UInt32								fileOffset;			// 264
	UInt32								dataOffset;			// 268 index into dataBuf
	UInt32								subrecordBytesRead;	// 26C generates error on Read if != expected length
	UInt32						unk268[(0x298-0x270) >> 2];	// 270
	UInt8								unk298;				// 298
	UInt8								bIsBigEndian;		// 299
	UInt8								unk29A;				// 29A
	UInt8								pad29B;
	WIN32_FIND_DATA						fileData;			// 29C
	float								unk3DC;				// 3DC init'd to 0.94
	UInt32								unk3E0;				// 3E0
	UInt32								flags;				// 3E4 init'd to 0x00000800. 4000 and 40000 do stuff
	UInt8								unk3E8;				// 3E8
	UInt8								pad3E9[3];
	UInt32								unk3EC;				// 3EC
	UInt32								unk3F0;				// 3F0
	UInt32								unk3F4;				// 3F4
	UInt32								unk3F8;				// 3F8
	UInt32								numRefMods;			// 3FC related to modindex; see 4472D0
																// formIDs in mod are as saved in GECK, must fix up at runtime
	ModInfo								** refModInfo;		// 400 used to look up modInfo based on fixed mod index, double-check
	UInt32								unk404;				// 404
	UInt32								unk408;				// 408
	UInt8								modIndex;			// 40C init to 0xFF
	UInt8								pad40D[3];
	BSString							author;				// 410
	BSString							description;		// 418
	void								* dataBuf;			// 420 
	UInt32								unk424;				// 424 looks like size of entire record
	UInt8								unk428;				// 428
	UInt8								pad429[3];
	
	bool IsLoaded() const { return true; }
};

struct ModList
{
	tList<ModInfo>		modInfoList;
	UInt32				loadedModCount;
	ModInfo*			loadedMods[0xFF];
};

// E8
class TES
{
public:
	virtual ~TES();

	UInt32 unk04;
	UInt32 unk08;
	UInt32 unk0C;
	UInt32 unk10;
	UInt32 unk14;
	UInt32 unk18;
	UInt32 unk1C;
	UInt32 unk20;
	UInt32 unk24;
	UInt32 unk28;
	UInt32 unk2C;
	UInt32 unk30;
	UInt32 unk34;
	UInt32 unk38;
	UInt32 unk3C;
	UInt32 gridCellArray; //40 GridCellArray 0x24
	NiNode * objectLODRoot; // 44
	NiNode * landLOD; // 48
	NiNode * waterLOD; // 4C
	UInt32 tempNodeManager;//BSTempNodeManager
	UInt32 unk54;
	UInt32 unk58;
	UInt32 unk5C; // 7FFFFFFF
	UInt32 unk60; // 7FFFFFFF
	UInt32 unk64; // 7FFFFFFF
	UInt32 unk68; // 7FFFFFFF
	TESObjectCELL * currentCell;
	TESObjectCELL **  interiorCellBuffer; // idk, visited cells perhaps?
	UInt32 unk74;
	UInt32 unk78; // 0
	UInt32 unk7C; // 0
	UInt32 unk80; // 7FFFFFFF
	UInt32 unk84; // 7FFFFFFF
	UInt32 unk88;
	UInt32 unk8C;
	UInt32 unk90;
	UInt32 sky; // Sky
	UInt32 imageSpaceModifier; // ImageSpaceModifierInstanceForm
	UInt32 unk9C; // ImageSpaceModifierInstanceDOF ** ??
	UInt32 unkA0;
	UInt32 unkA4;
	UInt8  unkA8;
	UInt8  unkA9;
	UInt8  unkAA;
	UInt8  unkAB;
	UInt8  unkAC;
	UInt8  unkAD;
	UInt8  unkAE;
	UInt8  unkAF;
	UInt8  unkB0;
	UInt8  padB1[3];
	UInt32 unkB4; // 4579A000
	UInt32 unkB8; // 457D2000
	UInt32 worldSpace; // TESWorldSpace
	UInt32 npcs; // TESNPC ** ??
	UInt32 unkC4; // TESNPC next?
	UInt32 queuedFile; // QueuedFile
	NiSourceTexture* someTexture;
	UInt32 queuedFile1; // QueuedFile
	UInt32 queuedFile2;
	UInt32 unkD8; // BSFadeNode ** ??
	UInt32 unkDC;
	UInt32 navMeshInfoMap; // NavMeshInfoMap
	LoadedAreaBound * loadedAreaBound;
};
STATIC_ASSERT(sizeof(TES) == 0xE8);
STATIC_ASSERT(offsetof(TES, loadedAreaBound)  == 0xE4);

extern TES ** g_TES;

class DataHandler
{
public:
	static DataHandler* GetSingleton();

	// loads of tArrays of object types, at least a good number in formType order
	UInt32						unk000;
	UInt32						unk004;
	UnkFormArray				unk008;
	UnkFormArray				unk014;
	UnkFormArray				unk020;
	UnkFormArray				unk02C;
	tArray<BGSKeyword*>			keywords;
	tArray<BGSLocationRefType*>	locRefTypes;
	tArray<BGSAction*>			actions;
	tArray<BGSTextureSet*>		textureSets;
	tArray<BGSMenuIcon*>			menuIcons;
	tArray<TESGlobal*>			globals;
	tArray<TESClass*>			classes;
	tArray<TESFaction*>			factions;
	tArray<BGSHeadPart*>			headParts;
	tArray<TESEyes*>				eyes;
	tArray<TESRace*>				races;
	tArray<TESSound*>			sounds;
	tArray<BGSAcousticSpace*>	acousticSpaces;
	UnkFormArray				unkSkills;
	tArray<EffectSetting*>		magicEffects;
	tArray<Script*>				scripts;
	tArray<TESLandTexture*>		landTextures;
	tArray<EnchantmentItem*>	enchantments;
	tArray<SpellItem*>			spellItems;
	tArray<ScrollItem*>			scrolls;
	tArray<TESObjectACTI*>		activators;
	tArray<BGSTalkingActivator*>	talkingActivators;
	tArray<TESObjectARMO*>		armors;
	tArray<TESObjectBOOK*>		books;
	tArray<TESObjectCONT*>		containers;
	tArray<TESObjectDOOR*>		doors;
	tArray<IngredientItem*>		ingredients;
	tArray<TESObjectLIGH*>		lights;
	tArray<TESObjectMISC*>		miscObjects;
	tArray<BGSApparatus*>		apparatuses;
	tArray<TESObjectSTAT*>		statics;
	tArray<BGSStaticCollection*>	staticCollections;
	tArray<BGSMovableStatic*>	movableStatics;
	tArray<TESGrass*>			grasses;
	tArray<TESObjectTREE*>		trees;
	tArray<TESFlora*>			flora;
	tArray<TESFurniture*>		furniture;
	tArray<TESObjectWEAP*>		weapons;
	tArray<TESAmmo*>				ammo;
	tArray<TESNPC*>				npcs;
	tArray<TESLevCharacter*>		levCharacters;
	tArray<TESKey*>				keys;
	tArray<AlchemyItem*>			potions;
	tArray<BGSIdleMarker*>		idleMarkers;
	tArray<BGSNote*>				notes;
	tArray<BGSConstructibleObject*> constructibles;
	tArray<BGSProjectile*>		projectiles;
	tArray<BGSHazard*>			bgsHazards;
	tArray<TESSoulGem*>			soulGems;
	tArray<TESLevItem*>			levItems;
	tArray<TESWeather*>			weather;
	tArray<TESClimate*>			climates;
	tArray<BGSShaderParticleGeometryData*>	shaderParticleGeometryData;
	tArray<BGSReferenceEffect*>	referenceEffects;
	tArray<TESRegion*>			regions;
	tArray<NavMeshInfoMap*>		navMeshInfoMaps;
	tArray<TESObjectCELL*>		cells;
	tArray<TESObjectREFR*>		refs;	// could be actors
	tArray<Character*>			characters;
	tArray<MissileProjectile*>	missleProjs;
	tArray<ArrowProjectile*>		arrowProjs;
	tArray<GrenadeProjectile*>	grenadeProjs;
	tArray<BeamProjectile*>		beamProjs;
	tArray<FlameProjectile*>		flameProjs;
	tArray<ConeProjectile*>		coneProjs;
	tArray<BarrierProjectile*>	barrierProjs;
	tArray<Hazard*>				hazards;
	tArray<TESWorldSpace*>		worldSpaces;
	tArray<TESObjectLAND*>		lands;
	tArray<NavMesh*>				navMeshes;
	UnkFormArray				unkTLOD;
	tArray<TESTopic*>			topics;
	tArray<TESTopicInfo*>		topicInfos;
	tArray<TESQuest*>			quests;
	tArray<TESIdleForm*>			idleForms;
	tArray<TESPackage*>			packages;
	tArray<TESCombatStyle*>		combatStyles;
	tArray<TESLoadScreen*>		loadScreens;
	tArray<TESLevSpell*>			levSpells;
	tArray<TESObjectANIO*>		anios;
	tArray<TESWaterForm*>		waterForms;
	tArray<TESEffectShader*>		effectShaders;
	UnkFormArray				unkTOFTs;
	tArray<BGSExplosion*>		explosions;
	tArray<BGSDebris*>			debris;
	tArray<TESImageSpace*>		imageSpaces;
	tArray<TESImageSpaceModifier*>	imageSpaceModifiers;
	tArray<BGSListForm*>			listForms;
	tArray<BGSPerk*>				perks;
	tArray<BGSBodyPartData*>		bodyPartData;
	tArray<BGSAddonNode*>		addonNodes;
	tArray<ActorValueInfo*>		actorValueInfos;
	tArray<BGSCameraShot*>		cameraShots;
	tArray<BGSCameraPath*>		cameraPaths;
	tArray<BGSVoiceType*>		voiceTypes;
	tArray<BGSMaterialType*>		materialTypes;
	tArray<BGSImpactData*>		impactData;
	tArray<BGSImpactDataSet*>	impactDataSets;
	tArray<TESObjectARMA*>		armorAddons;
	tArray<BGSEncounterZone*>	encounterZones;
	tArray<BGSLocation*>			locations;
	tArray<BGSMessage*>			messages;
	tArray<BGSRagdoll*>			ragdolls;
	UnkFormArray				unkDOBJs;
	tArray<BGSLightingTemplate*>	lightingTemplates;
	tArray<BGSMusicType*>		musicTypes;
	tArray<BGSFootstep*>			footsteps;
	tArray<BGSFootstepSet*>		footstepSets;
	tArray<BGSStoryManagerBranchNode*>	branchNodes;
	tArray<BGSStoryManagerQuestNode*>	questNodes;
	tArray<BGSStoryManagerEventNode*>	eventNodes;
	tArray<BGSDialogueBranch*>	dialogBranches;
	tArray<BGSMusicTrackFormWrapper*>	musicTrackFormWrappers;
	UnkFormArray				unkDLVWs;
	tArray<TESWordOfPower*>		wordOfPowers;
	tArray<TESShout*>			shouts;
	tArray<BGSEquipSlot*>			equipSlots;
	tArray<BGSRelationship*>		relationships;
	tArray<BGSScene*>			scenes;
	tArray<BGSAssociationType*>	associationTypes;
	tArray<BGSOutfit*>			outfits;
	tArray<BGSArtObject*>		artObjects;
	tArray<BGSMaterialObject*>	materialObjects;
	tArray<BGSMovementType*>		movementTypes;
	tArray<BGSSoundDescriptorForm*>	soundDescriptors;
	tArray<BGSDualCastData*>		dualCastData;
	tArray<BGSSoundCategory*>	soundCategories;
	tArray<BGSSoundOutput*>		soundOutputs;
	tArray<BGSCollisionLayer*>	collisonLayers;
	tArray<BGSColorForm*>		colors;
	tArray<BGSReverbParameters*>	reverbParams;
	UInt32 unks[0x0E]; // 03 Cell** 06 TESGlobal**
	ModList							modList;
	UInt32 moreunks[100];

	const ModInfo* LookupModByName(const char* modName);
	UInt8 GetModIndex(const char* modName);
};

// 58
class BGSSaveLoadManager
{
public:
	enum
	{
		kEvent_Autosave =	1 << 0,
		kEvent_Save =		1 << 1,
		kEvent_Unk02 =		1 << 2,
		kEvent_Unk03 =		1 << 3,
		kEvent_Unk04 =		1 << 4,

		kEvent_Unk07 =		1 << 7,
	};

	static BGSSaveLoadManager *	GetSingleton(void);

	void	Save(const char * name);
	void	Load(const char * name);

	// used by Hooks_SaveLoad
	void	SaveGame_Hook(const char * saveName);
	bool	LoadGame_Hook(const char * saveName, bool unk1);
	void	ProcessEvents_Hook(void);

	// use these when calling from a papyrus thread
	void	RequestSave(const char * name);
	void	RequestLoad(const char * name);

	MEMBER_FN_PREFIX(BGSSaveLoadManager);

	tList<const char*>	* saveList;			// 00
	UInt32				unk04;				// 04
	UInt32				unk08;				// 08
	UInt32				unk0C;				// 0C
	UInt8				unk10;				// 10
	UInt8				unk11;				// 11
	UInt8				pad12[2];			// 12
	UInt32				pendingEvents;		// 14
	UInt32				unk18;				// 18
	UInt32				startTickCount;		// 1C - GetTickCount when constructed
	UInt8				unk20;				// 20 - init'd to 0x01
	UInt8				pad21[3];			// 21
	UInt32				unk24;				// 24
	UInt32				unk28;				// 28 - init'd to 0xFFFFFFFF
	bool				unk2C;				// 2C
	UInt8				pad2E[3];			// 2E
	void				* unk30;			// 30
	UInt8				unk34;				// 34 - init'd to 0x01
	UInt8				unk35;				// 35
	UInt8				pad36[2];			// 36
	UInt32				unk38;				// 38
	UInt32				unk3C;				// 3C
	UInt32				unk40;				// 40
	UInt32				unk44;				// 44
	UInt32				unk48;				// 48
	UInt32				unk4C;				// 4C
	UInt32				unk50;				// 50
	void				* unk54;			// 54

private:
	DEFINE_MEMBER_FN(Save_Internal, bool, 0x006814D0, const char * name, int unk1, UInt32 unk2);
	DEFINE_MEMBER_FN(Load_Internal, bool, 0x006821C0, const char * name, int unk1, UInt32 unk2, UInt32 unk3);

	DEFINE_MEMBER_FN(SaveGame_HookTarget, void, 0x00679200, const char * fileName);
	DEFINE_MEMBER_FN(LoadGame_HookTarget, bool, 0x0067B720, const char * fileName, bool unk0);

	DEFINE_MEMBER_FN(ProcessEvents_Internal, void, 0x00682400);
};

STATIC_ASSERT(sizeof(BGSSaveLoadManager) == 0x58);

class MiscStatManager
{
public:
	static MiscStatManager *	GetSingleton(void);

	// 14
	struct MiscStat
	{
		const char	* name;		// 00
		const char	* unk04;	// 04
		UInt32		value;		// 08
		UInt32		unk0C;		// 0C
		UInt8		unk10;		// 10
		UInt8		pad11[3];	// 11
	};

	class Visitor
	{
	public:
		virtual void	Visit(MiscStat * stat, void * stat_unk04, UInt32 stat_unk0C, UInt32 value, UInt32 stat_unk10) = 0;
	};

	MEMBER_FN_PREFIX(MiscStatManager);
	DEFINE_MEMBER_FN(Visit, void, 0x00488120, Visitor ** visitor);

	MiscStat	* m_stats;	// 00
	UInt32		unk04;		// 04
	UInt32		m_numStats;	// 08

	MiscStat *	Get(const char * name);
};
 
class EquipManager
{
public:
	virtual ~EquipManager();
 
	static EquipManager *   GetSingleton(void);

	MEMBER_FN_PREFIX(EquipManager);
	DEFINE_MEMBER_FN(EquipItem, void, 0x006EF3E0, Actor * actor, TESForm * item, BaseExtraList * extraData, SInt32 count, BGSEquipSlot * equipSlot, bool withEquipSound, bool preventUnequip, bool showMsg, void * unk);
	DEFINE_MEMBER_FN(UnequipItem, bool, 0x006EE560, Actor * actor, TESForm * item, BaseExtraList * extraData, SInt32 count, BGSEquipSlot * equipSlot, bool unkFlag1 , bool preventEquip, bool unkFlag2, bool unkFlag3, void * unk);
};

typedef BGSEquipSlot * (* _GetEitherHandSlot)();
extern const _GetEitherHandSlot GetEitherHandSlot;
 
typedef BGSEquipSlot * (* _GetRightHandSlot)();
extern const _GetRightHandSlot GetRightHandSlot;
 
typedef BGSEquipSlot * (* _GetLeftHandSlot)();
extern const _GetLeftHandSlot GetLeftHandSlot;

typedef UInt32 (* _LookupActorValueByName)(const char * name);
extern const _LookupActorValueByName LookupActorValueByName;

class ActorValueList
{
public:
	enum {
		kNumActorValues = 164
	};

	static ActorValueList * GetSingleton(void);
	ActorValueInfo * GetActorValue(UInt32 id);

	static UInt32 ResolveActorValueByName(const char * name);

private:
	UInt32 unk04;
	ActorValueInfo * actorValues[kNumActorValues];
};

class DefaultObjectList
{
public:
	enum {
		kNumDefaultObjects = 0x15A
	};
	struct DefaultObject
	{
		const char	* description;	// 00
		UInt32		unk04;			// 04
		UInt32		key;			// 08
		UInt32		unk0C;			// 0C
	};

	static DefaultObjectList * GetSingleton(void);

	DefaultObject	objects[kNumDefaultObjects];
};

class FaceMorphList
{
public:
	enum {
		kNumMorphs = 19
	};

	enum {
		kMorph_NoseShortLong = 0,
		kMorph_NoseDownUp,
		kMorph_JawUpDown,
		kMorph_JawNarrowWide,
		kMorph_JawBackForward,
		kMorph_CheeksDownUp,
		kMorph_CheeksInOut,
		kMorph_EyesMoveDownUp,
		kMorph_EyesMoveInOut,
		kMorph_BrowDownUp,
		kMorph_BrowInOut,
		kMorph_BrowBackForward,
		kMorph_LipMoveDownUp,
		kMorph_LipMoveInOut,
		kMorph_ChinThinWide,
		kMorph_ChinMoveUpDown,
		kMorph_OverbiteUnderbite,
		kMorph_EyesBackForward,
		kMorph_Vampire
	};

	static FaceMorphList * GetSingleton(void);

	struct Morph
	{
		UInt32 type;
		const char * lowerName;
		const char * upperName;
	};

	Morph morphs[kNumMorphs];
};

class FacePresetData
{
public:
	virtual ~FacePresetData();

	UInt32 unk08;	// Always 10?
	const char * gameSettingName;
};

class FacePresetList
{
public:
	enum {
		kNumPresets = 4
	};
	enum {
		kPreset_NoseType,
		kPreset_BrowType,
		kPreset_EyesType,
		kPreset_LipType
	};

	static FacePresetList * GetSingleton(void);

	struct Preset
	{
		const char * presetName;
		FacePresetData * data;
	};

	Preset presets[kNumPresets];
};

// 0x00882290 RaceMenu ctor
// 0x0087F6E0 Morph Callback Handler
// 0x005A4870 Apply Morph?
// 0x005610F0 GetMorphName by Index and value
// 0x00561180 SetMorph?

class FaceGen
{
public:
	static FaceGen *	GetSingleton(void);

	struct Action {
		BSFixedString name;
		UInt32	unk04;
		float	delta;
	};

	class MorphDatabase
	{
	public:
		MEMBER_FN_PREFIX(MorphDatabase);
		DEFINE_MEMBER_FN(GetFaceGenModelMapEntry, bool, 0x005A6230, const char * meshPath, BSFaceGenModelMap ** entry);
		DEFINE_MEMBER_FN(SetFaceGenModelMapEntry, void, 0x005A6540, const char * meshPath, BSFaceGenModel * model);

		UInt32 unk00;	// 00 - Doesn't seem to be anything here?
	};

	UInt32			unk00;						// 00
	UInt32			unk04;						// 04
	UInt32			unk08;						// 08
	UInt32			unk0C;						// 0C
	MorphDatabase	morphDatabase;				// 10
	UInt32			unk14[(0x3C - 0x14) >> 2];	// 14
	UInt8			isReset;					// 3C
	UInt8			pad3D[3];					// 3D

	MEMBER_FN_PREFIX(FaceGen);
	DEFINE_MEMBER_FN(RegenerateHead, void, 0x005A4B80, BSFaceGenNiNode * headNode, BGSHeadPart * head, TESNPC * npc);
	DEFINE_MEMBER_FN(ApplyMorph, void, 0x005A4070, BSFaceGenNiNode * faceGenNode, BGSHeadPart * headPart, BSFixedString * morphName, float relative);
};
STATIC_ASSERT(offsetof(FaceGen, isReset) == 0x3C);

// Changes one HeadPart to another
typedef void (* _ChangeActorHeadPart)(Actor*, BGSHeadPart* oldPart, BGSHeadPart* newPart);
extern const _ChangeActorHeadPart ChangeActorHeadPart;

// Regenerates dynamic tints
typedef UInt32 (* _UpdatePlayerTints)();
extern const _UpdatePlayerTints UpdatePlayerTints;

typedef BGSHeadPart ** (* _GetActorBaseOverlays)(TESNPC * npc);
extern const _GetActorBaseOverlays GetActorBaseOverlays;

typedef UInt32 (* _GetNumActorBaseOverlays)(TESNPC * npc);
extern const _GetNumActorBaseOverlays GetNumActorBaseOverlays;

typedef bool (* _ApplyMasksToRenderTarget)(tArray<TintMask*> * tintMask, BSRenderTargetGroup ** renderTarget);
extern const _ApplyMasksToRenderTarget ApplyMasksToRenderTarget;

typedef UInt32 (* _UpdateModelSkin)(NiNode*, NiColorA**);
extern const _UpdateModelSkin UpdateModelSkin;

typedef UInt32 (* _UpdateModelHair)(NiNode*, NiColorA**);
extern const _UpdateModelHair UpdateModelHair;

typedef UInt32 (* _UpdateModelFace)(NiNode*);
extern const _UpdateModelFace UpdateModelFace;

// Loads a TRI file into the FaceGenDB, parameters are unknown ptrs
// unk1 seems to be inited to zero before calling however
// unk2 is a numeric value from some other object it seems
// making it zero seems to cache anyway
typedef bool (* _CacheTRIFile)(const char * filePath, BSFaceGenDB::TRI::DBTraits::MorphSet ** morphSet, UInt32 * unk1);
extern const _CacheTRIFile CacheTRIFile;

// 20
class MagicFavorites
{
	//	void			** _vtbl;	// 00
	UInt32			unk004;		// 04
	UnkFormArray	spells;		// 08
	UnkFormArray	hotkeys;	// 14

public:
	virtual	~MagicFavorites();

	void		SetHotkey(TESForm * form, SInt8 idx);
	void		ClearHotkey(SInt8 idx);
	TESForm	*	GetSpell(SInt8 idx);
	bool		IsFavorited(TESForm * form);

	static MagicFavorites * GetSingleton(void)
	{
		return *((MagicFavorites **)0x01B2E39C);
	}
};

// 84?
class PersistentFormManager
{
public:
	struct EnchantData
	{
		EnchantmentItem *	enchantment;	// 00
		volatile SInt32		refCount;		// 04
	};

	UInt32	unk00;	// 00
	tArray<EnchantData>	weaponEnchants;	// 04
	tArray<EnchantData>	armorEnchants;	// 10
	UInt32	unk1C;	// 1C
	UInt32	unk20;	// 20
	UInt32	unk24[(0x80 - 0x24) >> 2];	// 24

	static PersistentFormManager * GetSingleton(void)
	{
		return *((PersistentFormManager **)0x012E3300);
	}

	void IncRefEnchantment(EnchantmentItem * enchantment)
	{
		if(enchantment && enchantment->formID >= 0xFF000000) {
			for(UInt32 i = 0; i < weaponEnchants.count; i++) {
				EnchantData foundData;
				weaponEnchants.GetNthItem(i, foundData);
				if(foundData.enchantment == enchantment) {
					InterlockedIncrement(&weaponEnchants[i].refCount);
					break;
				}
			}
			for(UInt32 i = 0; i < armorEnchants.count; i++) {
				EnchantData foundData;
				armorEnchants.GetNthItem(i, foundData);
				if(foundData.enchantment == enchantment) {
					InterlockedIncrement(&armorEnchants[i].refCount);
					break;
				}
			}
		}
	}

	// The game doesn't bother to dec ref or even delete custom enchants
	// when they are no longer used, maybe we can fix this?
	void DecRefEnchantment(EnchantmentItem * enchantment)
	{
		if(enchantment && enchantment->formID >= 0xFF000000) {
			for(UInt32 i = 0; i < weaponEnchants.count; i++) {
				EnchantData foundData;
				weaponEnchants.GetNthItem(i, foundData);
				if(foundData.enchantment == enchantment) {
					if(!InterlockedDecrement(&weaponEnchants[i].refCount))
						CALL_MEMBER_FN(this, ScheduleForDeletion)(enchantment);
					break;
				}
			}
			for(UInt32 i = 0; i < armorEnchants.count; i++) {
				EnchantData foundData;
				armorEnchants.GetNthItem(i, foundData);
				if(foundData.enchantment == enchantment) {
					if(!InterlockedDecrement(&armorEnchants[i].refCount))
						CALL_MEMBER_FN(this, ScheduleForDeletion)(enchantment);
					break;
				}
			}
		}
	}

	MEMBER_FN_PREFIX(PersistentFormManager);
	DEFINE_MEMBER_FN(CreateOffensiveEnchantment, EnchantmentItem *, 0x00689D30, tArray<MagicItem::EffectItem> * effectArray);
	DEFINE_MEMBER_FN(CreateDefensiveEnchantment, EnchantmentItem *, 0x00689D80, tArray<MagicItem::EffectItem> * effectArray);
	DEFINE_MEMBER_FN(CreatePotion, void, 0x0068ACB0, AlchemyItem ** potion, tArray<MagicItem::EffectItem> * effectArray);
	//DEFINE_MEMBER_FN(AddPersistentForm, void, 0x0068A0F0, TESForm *);
	DEFINE_MEMBER_FN(ScheduleForDeletion, void, 0x0068A1B0, TESForm *);
};

class AnimationSpeedData
{
public:
	virtual ~AnimationSpeedData();

	class SpeedData
	{
	public:
		UInt32	unk00;
		UInt32	unk04;	// 8?
		UInt32	unk08;	// 5?
		UInt32	unk0C;	// 8?
		void	* unk10;	// DEADBEEF
		UInt32	unk14;
		UInt32	unk18;
	};

	UInt32	unk04;
	UInt32	unk08;
	SpeedData	unk0C;
	UInt32	unk28;
	UInt32	unk2C;
};

class GarbageCollector
{
public:
	virtual ~GarbageCollector();

	static GarbageCollector * GetSingleton(void);

	MEMBER_FN_PREFIX(GarbageCollector);
	DEFINE_MEMBER_FN(AddNiAVObject, UInt32, 0x0068FCF0, NiAVObject * object);
};

class MenuTopicManager
{
public:
	virtual ~MenuTopicManager();
	virtual void Unk_01(void);

	TESObjectREFR * GetDialogueTarget();

	static MenuTopicManager * GetSingleton(void);

	BSTEventSink<void>	playerPositionEvent;	// 04
	UInt32	unk08[(0x38 - 0x08) >> 2];			// 08
	UInt32	talkingHandle;						// 38
	UInt32	handle2;							// 3C
	UInt32	unk40[(0x60 - 0x40) >> 2];			// 40
	UInt8	unk60;
	UInt8	unk61;
	UInt8	unk62;
	UInt8	unk63;
	UInt8	unk64;
	UInt8	unk65;
	UInt8	unk66;
	UInt8	unk67;
	UInt8	unk68;
	UInt8	unk69;
	UInt8	unk6A;
	UInt8	unk6B;
};
STATIC_ASSERT(offsetof(MenuTopicManager, talkingHandle) == 0x38);

// 04
class SkyObject
{
public:
	virtual ~SkyObject();
	virtual void Unk_01(void);
	virtual void Unk_02(void);
	virtual void Unk_03(void);

	UInt32	m_refCount;	// 04
};

// 1C
class Atmosphere : public SkyObject
{
public:
	virtual ~Atmosphere();
	virtual void Unk_04(void);

	UInt32	unk08;	// 08
	UInt32	unk0C;	// 0C
	UInt32	unk10;	// 10
	UInt32	unk14;	// 14
	UInt32	unk18;	// 18
};

// 10
class Stars : public SkyObject
{
public:
	virtual ~Stars();

	UInt32	unk08;	// 08
	UInt32	unk0C;	// 0C
};

// 30
class Sun : public SkyObject
{
public:
	virtual ~Sun();

	UInt32	unk08;	// 08
	UInt32	unk0C;	// 0C
	UInt32	unk10;	// 10
	UInt32	unk14;	// 14
	UInt32	unk18;	// 18
	UInt32	unk1C;	// 1C
	UInt32	unk20;	// 20
	UInt32	unk24;	// 24
	UInt8	unk28;	// 28
	UInt8	pad29[3];	// 29
	UInt32	unk2C;	// 2C - BSShaderAccumulator
};

// 38C
class Clouds : public SkyObject
{
public:
	virtual ~Clouds();

	UInt32	unk08[(0x38C - 0x08) >> 2];		// 08
};
STATIC_ASSERT(sizeof(Clouds) == 0x38C);

// 7C
class Moon : public SkyObject
{
public:
	virtual ~Moon();

	UInt32	unk08;			// 08 - NiNode?
	UInt32	unk0C;			// 0C - NiNode?
	UInt32	unk10;			// 10 - NiNode?
	UInt32	unk14;			// 14 - NiNode?
	char	* unk18;		// 18 - Data/Textures/Sky/%s_full.dds
	UInt32	unk1C;			// 1C
	char	* unk20;		// 20 - Data/Textures/Sky/%s_three_wan.dds
	UInt32	unk24;			// 24
	char	* unk28;		// 28 - Data/Textures/Sky/%s_half_wan.dds
	UInt32	unk2C;			// 3C
	char	* unk30;		// 30 - Data/Textures/Sky/%s_one_wan.dds
	UInt32	unk34;			// 34
	UInt32	unk38;			// 38
	UInt32	unk3C;			// 3C
	char	* unk40;		// 40 - Data/Textures/Sky/%s_one_wax.dds
	UInt32	unk44;			// 44
	char	* unk48;		// 48 - Data/Textures/Sky/%s_half_wax.dds
	UInt32	unk4C;			// 4C
	char	* unk50;		// 50 - Data/Textures/Sky/%s_three_wax.dds
	UInt32	unk54;			// 54
	UInt32	unk58;			// 58
	UInt32	unk7C[(0x7C - 0x58) >> 2];		// 08
};

// 1C
class Precipitation : public SkyObject
{
public:
	virtual ~Precipitation();

	UInt32	unk08;	// 08
	float	unk0C;	// 0C
	float	unk10;	// 10
	float	unk14;	// 14
	float	unk18;	// 18
};
STATIC_ASSERT(sizeof(Precipitation) == 0x1C);

// 238
class Sky
{
public:
	virtual ~Sky();

	void			* unk04;			// 04 - BSMultiBoundNode
	NiNode			* unk08;			// 08
	UInt32			unk0C;				// 0C
	UInt32			unk10;				// 10
	UInt32			unk14;				// 14
	UInt32			unk18;				// 18
	UInt32			unk1C;				// 1C
	UInt32			unk20;				// 20
	UInt32			unk24;				// 24
	TESClimate		* climate;			// 28
	TESWeather		* weather;			// 2C
	UInt32			unk30;				// 30
	UInt32			unk34;				// 34
	UInt32			unk38;				// 38
	TESRegion		* region;			// 3C
	Atmosphere		* atmosphere;		// 40
	Stars			* stars;			// 44
	Sun				* sun;				// 48
	Clouds			* clouds;			// 4C
	Moon			* masser;			// 50
	Moon			* secunda;			// 54
	Precipitation	* precipitation;	// 58
	float			unk5C[6];			// 5C
	UInt32			unk74[3];			// 74
	float			unk80[6];			// 80
	UInt32			unk98[6];			// 98
	float			unkB0[9];			// B0
	UInt32			unkD4[6];			// D4
	float			unkEC[9];			// EC
	UInt32			unk110[9];			// 110
	float			unk134[7];			// 134
	UInt32			unk150[3];			// 150
	float			unk15C[5];			// 15C
	UInt32			unk170;				// 170
	void			* unk174;			// 174
	UInt32			unk178[3];			// 178
	float			unk184;				// 184
	UInt32			unk188[6];			// 188
	float			unk1A0[(0x210 - 0x1A0) >> 2];	// 1A0
	void			* skyEffectController;			// 210
	UInt32			unk214[(0x238 - 0x214) >> 2];	// 214

	MEMBER_FN_PREFIX(Sky);
	DEFINE_MEMBER_FN(IsSnowing, bool, 0x00504350);
};
STATIC_ASSERT(sizeof(Sky) == 0x238);

typedef Sky * (* _GetSky)();
extern const _GetSky GetSky;

typedef bool (* _HasLOS)(Actor* source, TESObjectREFR* target, UInt8 * unk1);
extern const _HasLOS HasLOS;

class PerkEntryPoints
{
public:
	static PerkEntryPoints * GetSingleton(void);

	enum
	{
		kEntryPoint_Calculate_Weapon_Damage,
		kEntryPoint_Calculate_My_Critical_Hit_Chance,
		kEntryPoint_Calculate_My_Critical_Hit_Damage,
		kEntryPoint_Calculate_Mine_Explode_Chance,
		kEntryPoint_Adjust_Limb_Damage,
		kEntryPoint_Adjust_Book_Skill_Points,
		kEntryPoint_Modify_Recovered_Health,
		kEntryPoint_Get_Should_Attack,
		kEntryPoint_Modify_Buy_Prices,
		kEntryPoint_Add_Leveled_List_On_Death,
		kEntryPoint_Get_Max_Carry_Weight,
		kEntryPoint_Modify_Addiction_Chance,
		kEntryPoint_Modify_Addiction_Duration,
		kEntryPoint_Modify_Positive_Chem_Duration,
		kEntryPoint_Activate,
		kEntryPoint_Ignore_Running_During_Detection,
		kEntryPoint_Ignore_Broken_Lock,
		kEntryPoint_Modify_Enemy_Critical_Hit_Chance,
		kEntryPoint_Modify_Sneak_Attack_Mult,
		kEntryPoint_Modify_Max_Placeable_Mines,
		kEntryPoint_Modify_Bow_Zoom,
		kEntryPoint_Modify_Recover_Arrow_Chance,
		kEntryPoint_Modify_Skill_Use,
		kEntryPoint_Modify_Telekinesis_Distance,
		kEntryPoint_Modify_Telekinesis_Damage_Mult,
		kEntryPoint_Modify_Telekinesis_Damage,
		kEntryPoint_Mod_Bashing_Damage,
		kEntryPoint_Mod_Power_Attack_Stamina,
		kEntryPoint_Mod_Power_Attack_Damage,
		kEntryPoint_Mod_Spell_Magnitude,
		kEntryPoint_Mod_Spell_Duration,
		kEntryPoint_Mod_Secondary_Value_Weight,
		kEntryPoint_Mod_Armor_Weight,
		kEntryPoint_Mod_Incoming_Stagger,
		kEntryPoint_Mod_Target_Stagger,
		kEntryPoint_Mod_Attack_Damage,
		kEntryPoint_Mod_Incoming_Damage,
		kEntryPoint_Mod_Target_Damage_Resistance,
		kEntryPoint_Mod_Spell_Cost,
		kEntryPoint_Mod_Percent_Blocked,
		kEntryPoint_Mod_Shield_Deflect_Arrow_Chance,
		kEntryPoint_Mod_Incoming_Spell_Magnitude,
		kEntryPoint_Mod_Incoming_Spell_Duration,
		kEntryPoint_Mod_Player_Intimidation,
		kEntryPoint_Mod_Player_Reputation,
		kEntryPoint_Mod_Favor_Points,
		kEntryPoint_Mod_Bribe_Amount,
		kEntryPoint_Mod_Detection_Light,
		kEntryPoint_Mod_Detection_Movement,
		kEntryPoint_Mod_Soul_Gem_Recharge,
		kEntryPoint_Set_Sweep_Attack,
		kEntryPoint_Apply_Combat_Hit_Spell,
		kEntryPoint_Apply_Bashing_Spell,
		kEntryPoint_Apply_Reanimate_Spell,
		kEntryPoint_Set_Boolean_Graph_Variable,
		kEntryPoint_Mod_Spell_Casting_Sound_Event,
		kEntryPoint_Modify_Pickpocket_Chance,
		kEntryPoint_Modify_Detection_Sneak_Skill,
		kEntryPoint_Modify_Falling_Damage,
		kEntryPoint_Modify_Lockpick_Sweet_Spot,
		kEntryPoint_Modify_Sell_Prices,
		kEntryPoint_Can_Pickpocket_Equipped_Item,
		kEntryPoint_Modify_Lockpick_level_allowed,
		kEntryPoint_Set_Lockpick_Starting_Arc,
		kEntryPoint_Set_Progression_Picking,
		kEntryPoint_Make_lockpicks_unbreakable_,
		kEntryPoint_Modify_Alchemy_Effectiveness,
		kEntryPoint_Apply_Weapon_Swing_Spell,
		kEntryPoint_Modify_Commanded_Actor_Limit,
		kEntryPoint_Apply_Sneaking_Spell,
		kEntryPoint_Modify_Player_Magic_Slowdown,
		kEntryPoint_Modify_Ward_Magicka_Absorption_Pct,
		kEntryPoint_Modify_Initial_Ingredient_Effects_Learned,
		kEntryPoint_Purify_Alchemy_Ingredients,
		kEntryPoint_Filter_Activation,
		kEntryPoint_Can_dual_cast_spell,
		kEntryPoint_Modify_Tempering_Health,
		kEntryPoint_Modify_Enchantment_Power,
		kEntryPoint_Modify_Soul_Pct_Captured_to_Weapon,
		kEntryPoint_Mod_Soul_Gem_Enchanting,
		kEntryPoint_Mod_applied_enchantments_allowed,
		kEntryPoint_Set_Activate_Label,
		kEntryPoint_Mod_Shout_OK,
		kEntryPoint_Mod_Poison_Dose_Count,
		kEntryPoint_Should_Apply_Placed_Item,
		kEntryPoint_Modify_Armor_Rating,
		kEntryPoint_Modify_lockpicking_crime_chance,
		kEntryPoint_Modify_ingredients_harvested,
		kEntryPoint_Modify_Spell_Range_Target_Loc,
		kEntryPoint_Modify_Potions_Created,
		kEntryPoint_Modify_lockpicking_key_reward_chance,
		kEntryPoint_Allow_Mount_Actor,
		kNumPerkEntryPoints
	};

	struct Data
	{
		const char	* name;	// 00
		UInt32		unk04;	// 04
		UInt32		unk08;	// 08
		UInt32		unk0C;	// 0C
	};

	Data	entryPoints[kNumPerkEntryPoints];
};

typedef bool (* _CalculatePerkData)(UInt32 entryPointType, TESObjectREFR* source, ...);
extern const _CalculatePerkData CalculatePerkData;