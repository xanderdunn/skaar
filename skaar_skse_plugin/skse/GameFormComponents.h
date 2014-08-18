#pragma once

// everything deriving from BaseFormComponent but not TESForm goes here

#include "skse/GameTypes.h"

class TESObjectSTAT;
class BGSSoundDescriptorForm;
class BGSSoundOutput;
class BGSSoundCategory;
class BGSKeyword;
class TESForm;
class SpellItem;
class TESShout;
class IngredientItem;
class TESRace;
class EnchantmentItem;
class TESFaction;
class BGSVoiceType;
class BGSEquipSlot;
class Actor;
class TESObjectARMO;
class TESIdleForm;
class BGSPerk;
class ActorValueInfo;
class TESGlobal;
class TESRegion;
class BGSMusicType;
class TESWeather;

//// root

// 04
class BaseFormComponent
{
public:
	BaseFormComponent();
	virtual ~BaseFormComponent();

	virtual void	Init(void);
	virtual void	ReleaseRefs(void);
	virtual void	CopyFromBase(BaseFormComponent * rhs);

	//	void	** _vtbl;	// 00
};

//// members of other components

// 08
class TESTexture : public BaseFormComponent
{
public:
	virtual UInt32			Unk_04(void);
	virtual void			GetNormalMapName(BSString * out);	// might have const char * retn type
	virtual const char *	GetSearchDir(void);

	StringCache::Ref	str;	// 04
};

// 08
class TESIcon : public TESTexture
{

};

// 08
class TESTexture1024 : public TESTexture
{
	
};

//// leaf nodes

// 08
class BGSAttackDataForm : public BaseFormComponent
{
public:
	BSTSmartPointer	<void>	unk04;	// 04
};

// 10 - 1.6.89 and earlier
// 0C - 1.7.7 and after
class BGSBipedObjectForm : public BaseFormComponent
{
public:
	// applicable to DefaultRace
	enum
	{
		kPart_Head =		1 << 0,
		kPart_Hair =		1 << 1,
		kPart_Body =		1 << 2,
		kPart_Hands =		1 << 3,
		kPart_Forearms =	1 << 4,
		kPart_Amulet =		1 << 5,
		kPart_Ring =		1 << 6,
		kPart_Feet =		1 << 7,
		kPart_Calves =		1 << 8,
		kPart_Shield =		1 << 9,
		kPart_Unnamed10 =	1 << 10,
		kPart_LongHair =	1 << 11,
		kPart_Circlet =		1 << 12,
		kPart_Ears =		1 << 13,
		kPart_Unnamed14 =	1 << 14,
		kPart_Unnamed15 =	1 << 15,
		kPart_Unnamed16 =	1 << 16,
		kPart_Unnamed17 =	1 << 17,
		kPart_Unnamed18 =	1 << 18,
		kPart_Unnamed19 =	1 << 19,
		kPart_Unnamed20 =	1 << 20,
		kPart_Unnamed21 =	1 << 21,
		kPart_Unnamed22 =	1 << 22,
		kPart_Unnamed23 =	1 << 23,
		kPart_Unnamed24 =	1 << 24,
		kPart_Unnamed25 =	1 << 25,
		kPart_Unnamed26 =	1 << 26,
		kPart_Unnamed27 =	1 << 27,
		kPart_Unnamed28 =	1 << 28,
		kPart_Unnamed29 =	1 << 29,
		kPart_Unnamed30 =	1 << 30,
		kPart_FX01 =		1 << 31,
	};

	enum
	{
		kWeight_Light =	0,
		kWeight_Heavy,
		kWeight_None,
	};

	struct Data
	{
		UInt32	parts;			// 00 - init'd to 0
		UInt32	weightClass;	// 04 - init'd to 2 (none)
	};

	Data	data;	// 04

	UInt32 GetSlotMask() const { return data.parts; }
	void SetSlotMask(UInt32 mask) { data.parts = mask; }
	UInt32 AddSlotToMask(UInt32 mask);
	UInt32 RemoveSlotFromMask(UInt32 mask);
};

// 0C
class BGSBlockBashData : public BaseFormComponent
{
public:
	UInt32	unk04;	// 04
	UInt32	unk08;	// 08
};

// 08
class BGSDestructibleObjectForm : public BaseFormComponent
{
public:
	// 14
	struct Data
	{
		UInt32	unk00;		// 00
		UInt8	unk04;		// 04
		UInt8	unk05;		// 05
		UInt8	pad06[2];	// 06
		UInt32	unk08;		// 08
		UInt32	unk0C;		// 0C
		UInt32	unk10;		// 10 - object
	};

	Data	* data;	// 04
};

// 08
class BGSEquipType : public BaseFormComponent
{
public:
	virtual	BGSEquipSlot *	GetEquipSlot(void);
	virtual void			SetEquipSlot(BGSEquipSlot * type);

	BGSEquipType * unk04;	// 04
};

// 10
class BGSIdleCollection : public BaseFormComponent
{
public:
	UInt8	unk04;		// 04
	UInt8	unk05;		// 05 - unk08 length
	UInt8	unk06[2];	// 06
	void	** unk08;	// 08 - copyable struct
	UInt32	unk0C;		// 0C
};

// 0C
class BGSKeywordForm : public BaseFormComponent
{
public:
	virtual bool	HasKeyword(UInt32 keyword);
	virtual UInt32	GetDefaultKeyword(void);

	BGSKeyword	** keywords;	// 04
	UInt32		numKeywords;	// 08
};

// 08
class BGSMenuDisplayObject : public BaseFormComponent
{
public:
	virtual UInt32	GetMenuDisplayObject(void);

	TESObjectSTAT	* worldStatic;	// 04 - copyable
};

// 0C
class BGSMessageIcon : public BaseFormComponent
{
public:
	TESIcon	icon;	// 04
};

// 14
class BGSOverridePackCollection : public BaseFormComponent
{
public:
	UInt32	unk04;	// 04
	UInt32	unk08;	// 08
	UInt32	unk0C;	// 0C
	UInt32	unk10;	// 10
};

// 0C
class BGSPerkRankArray : public BaseFormComponent
{
public:
	// 08
	struct Data
	{
		BGSPerk	* perk;		// 00
		UInt8	rank;		// 04
		UInt8	pad05[3];	// 05
	};

	Data	* perkRanks;	// 04
	UInt32	numPerkRanks;	// 08
};

// 0C
class BGSPickupPutdownSounds : public BaseFormComponent
{
public:
	BGSSoundDescriptorForm*	pickUp;	// 04
	BGSSoundDescriptorForm*	putDown;	// 08
};

// 04
class BGSPreloadable : public BaseFormComponent
{
public:
	virtual void	Unk_04(void);	// pure virtual
};

// 08
class BGSSkinForm : public BaseFormComponent
{
public:
	TESObjectARMO	* skin;	// 04
};

// 18
class TESAIForm : public BaseFormComponent
{
public:
	// 8 - might be ExtraDataList
	struct Data
	{
		UInt32	unk0;	// 0
		Data	* next;	// 4
	};

	UInt32	flags;		// 04
	UInt16	unk08[3];	// 08
	UInt8	pad0D[2];	// 0D
	Data	unk10;		// 10
};

// 38
class TESActorBaseData : public BaseFormComponent
{
public:
	virtual void	Unk_04(UInt32 arg);
	virtual bool	GetFlag20000000(void);
	virtual bool	GetFlag80000000(void);
	virtual bool	Unk_07(void);
	virtual UInt32	GetUnk20(void);

	enum {
		kFlag_PCLevelMult = 0x80
	};

	UInt32				flags;	// 04 - init'd to 0
	UInt16				unk08;	// 08 - init'd to 0
	UInt16				unk0A;	// 0A - init'd to 0
	UInt16				level;	// 0C - init'd to 1	(CK Multiplier * 1000 if PCLevelMult is true)
	UInt16				minLevel;	// 0E - init'd to 0
	UInt16				maxLevel;	// 10 - init'd to 0
	UInt16				unk12;	// 12 - init'd to 0x64 (100)
	UInt16				unk14;	// 14 - init'd to iBaseDisposition (35)
	UInt16				unk16;	// 16 - init'd to 0
	UInt16				unk18;	// 18 - init'd to 0
	UInt16				unk1A;	// 1A - init'd to 0
	UInt32				unk1C;	// 1C
	BGSVoiceType		* voiceType;	// 20
	UInt32				unk24;	// 24
	UInt32				unk28;	// 28 - init'd to 0
	tArray<TESFaction*>	unk2C;	// 2C
};

// 8
class TESAttackDamageForm : public BaseFormComponent
{
public:
	virtual UInt16	GetAttackDamage(void);

	UInt16	attackDamage;	// 04
	UInt8	pad06[2];		// 06
};

class BGSTextureSet;

// 14
class TESModel : public BaseFormComponent
{
public:
	virtual const char *	GetModelName(void);
	virtual void			SetModelName(const char * name);
	virtual UInt32			Unk_06(void);

	StringCache::Ref	name;	// 04

	UInt32				unk08;	// 08
	UInt32				unk0C;	// 0C
	UInt16				unk10;	// 10
	UInt8				unk12;	// 12
	UInt8				unk13;	// 13
};

// 1C
class TESModelTextureSwap : public TESModel
{
public:
	// 0C
	struct SwapInfo
	{
		BGSTextureSet	* textureSet;	// 00
		UInt32			unk04;			// 04 - index?
		BSFixedString	name;			// 08
	};

	SwapInfo		* swaps;	// 14
	UInt32			count;		// 18
};

// 14
class TESModelRDT : public TESModel
{
public:
};

// 78
class TESBipedModelForm : public BaseFormComponent
{
public:
	TESModelTextureSwap	textureSwap[2];	// 04
	TESIcon				icon[2];		// 3C
	BGSMessageIcon		messageIcon[2];	// 4C
	TESModelRDT			modelRDT;		// 64
};

// 0C
class TESContainer : public BaseFormComponent
{
public:
	struct Entry
	{
		struct Data
		{
			UInt32	unk00;
			UInt32	unk04;
			float	unk08;
		};

		UInt32	count;
		TESForm* form;
		Data	* data; // extra data?
	};

	Entry	** entries;	// 04
	UInt32	numEntries;	// 08

	template <class Op>
	UInt32 CountIf(Op& op) const
	{
		UInt32 count = 0;
		for (UInt32 n = 0; n < numEntries; n++) {
			Entry* pEntry = entries[n];
			if (pEntry && op.Accept(pEntry))
				count++;
		}
		return count;
	}

	template <class Op>
	Entry* Find(Op& op) const
	{
		bool bFound = false;
		UInt32 n = 0;
		Entry* pEntry = NULL;
		for (UInt32 n = 0; n < numEntries && !bFound; n++) {
			pEntry = entries[n];
			if (pEntry) {
				bFound = op.Accept(pEntry);
			}
		}
		return (bFound && pEntry) ? pEntry : NULL;
	}

	template <class Op>
	void Visit(Op& op) const {
		bool bContinue = true;
		for (UInt32 n = 0; n < numEntries && bContinue; n++) {
			Entry* pEntry = entries[n];
			if (pEntry) {
				bContinue = op.Accept(pEntry);
			}
		}
	}

	UInt32 CountItem(TESForm* item) const;
};

// 0C
class TESDescription : public BaseFormComponent
{
public:
	UInt32	unk04;	// 04
	UInt32	unk08;	// 08 - init'd to FFFFFFFF

	MEMBER_FN_PREFIX(TESDescription);
	DEFINE_MEMBER_FN(Get, void, 0x0044FB70, BSString * out, TESForm * parent, UInt32 fieldType);
};

// C
class TESEnchantableForm : public BaseFormComponent
{
public:
	virtual UInt16	Unk_04(void);		// return unk08

	EnchantmentItem*	enchantment;	// 04 - init'd to 0
	UInt16				unk08;			// 08 - init'd to 3
	UInt16				maxCharge;
};

// 08
class TESFullName : public BaseFormComponent
{
public:
	virtual UInt32	Unk_04(void);
	virtual const char *	GetName(void);

	StringCache::Ref	name;	// 04
};

// 08
class TESImageSpaceModifiableForm : public BaseFormComponent
{
public:
	UInt32	unk04;	// 04
};

// 14
class TESLeveledList : public BaseFormComponent
{
public:
	struct Entry
	{
		TESForm * form;	// 00 - init'd to 0
		UInt16	count;	// 04 - init'd to 1
		UInt16	level;	// 06 - init'd to 1
		UInt32	unk8;	// 08 - init'd to 0
	};

	enum
	{
		kFlagCalculateFromAllLevelsLTPCLevel =	1 << 0,
		kFlagCalculateForEachItemInCount	 =	1 << 1,
		kFlagUseAll							 =	1 << 2,
		kFlagSpecialLoot					 =	1 << 3
	};

	virtual bool	Unk_04(void);
	virtual bool	GetFlag02(void);
	virtual UInt32	Unk_06(void);
	virtual void	Unk_07(void);	// pure

	Entry	* entries;	// 04
	UInt8	chanceValue;		// 08
	UInt8	flags;		// 09
	UInt8	length;		// 0A
	UInt8	unk0B;		// 0B
	void	* unk0C;	// 0C
	TESGlobal * chanceGlobal;		// 10
};

// 14
class BGSBehaviorGraphModel : public TESModel
{
public:
};

// 14
class BGSTextureModel : public TESModel
{
public:
};

// 14
class TESModelPSA : public TESModel
{
public:
};

// 14
class TESModelTri : public TESModel
{
public:
};

// 10?
class TESProduceForm : public BaseFormComponent
{
public:
	BGSSoundDescriptorForm	* harvestSound;	// 04
	IngredientItem			* produce;		// 08
	UInt8					unk0C[4];		// 0C
};

// 08
class TESQualityForm : public BaseFormComponent
{
public:
	UInt32	unk04;		// 04
};

// 08
class TESRaceForm : public BaseFormComponent
{
public:
	TESRace	* race;	// 04
};

// 10
class TESReactionForm : public BaseFormComponent
{
public:
	// 0C
	struct Data
	{
		UInt32	unk0;	// 00
		UInt32	unk4;	// 04
		UInt32	unk8;	// 08
	};

	// 08
	struct Entry
	{
		Data	* data;	// 00
		Entry	* unk4;	// 04
	};

	Entry	entry;		// 04
	UInt8	unk0C;		// 0C
	UInt8	pad0D[3];	// 0D
};

// 08
class TESSpellList : public BaseFormComponent
{
public:
	UInt32		GetSpellCount() const;
	SpellItem *	GetNthSpell(UInt32 n) const;

	UInt32		GetShoutCount() const;
	TESShout *	GetNthShout(UInt32 n) const;

	struct Data
	{
		SpellItem**	spells;	// SpellItem**  (null terminated array of spells)
		void	* unk4;
		TESShout**	shouts;
		UInt32		numSpells;
		UInt32		numUnk4;
		UInt32		numShouts;
	};

	Data	* unk04;	// 04
};

// 08
class TESValueForm : public BaseFormComponent
{
public:
	UInt32	value;	// 04
};

// 08
class TESWeightForm : public BaseFormComponent
{
public:
	float	weight;	// 04
};

//// the following are not form components
//// they live here until there's a better header file

// 40
class BSMaterialObject
{
public:
	// 30
	struct Data
	{
		// 0C
		struct Data10
		{
			UInt32	unk0;	// 0 - init'd to 0
			UInt32	unk4;	// 4 - init'd to 0
			float	unk8;	// 8 - init'd to -1
		};

		float	unk00;	// 00 - init'd to 0.4
		float	unk04;	// 04 - init'd to 0.4
		float	unk08;	// 08 - init'd to 512
		float	unk0C;	// 0C - init'd to 512
		Data10	unk10;	// 10
		float	unk1C;	// 1C - init'd to 1
		UInt32	unk20;	// 20
		UInt32	unk24;	// 24
		UInt32	unk28;	// 28
		UInt32	unk2C;	// 2C
	};

	BSMaterialObject();
	virtual ~BSMaterialObject();

	virtual void	Unk_01(void);

//	void	** _vtbl;	// 00
	
	Data		unk04;	// 04
	UnkArray	unk34;	// 34
};

STATIC_ASSERT(sizeof(BSMaterialObject) == 0x40);

// 04
class TESChildCell
{
public:
	virtual ~TESChildCell();

	virtual	UInt32	Unk_01(void);

//	void	** _vtbl;
};

// 04
class ActorValueOwner
{
public:
	virtual ~ActorValueOwner();

	// Argument is the ActorValue ID
	virtual float	GetCurrent(UInt32 arg);
	virtual float	GetMaximum(UInt32 arg);
	virtual float	GetBase(UInt32 arg);
	virtual void	SetBase(UInt32 arg0, float arg1);
	virtual void	ModBase(UInt32 arg0, float arg1);
	virtual void	Unk_06(UInt32 arg0, UInt32 arg1, UInt32 arg2); // Force/Mod AV?
	virtual void	SetCurrent(UInt32 arg0, float arg1);
	virtual bool	Unk_08(void);

//	void	** _vtbl;	// 00
};

STATIC_ASSERT(sizeof(ActorValueOwner) == 0x4);

// 08
class BSIMusicTrack
{
public:
	BSIMusicTrack();
	virtual ~BSIMusicTrack();

	virtual void	Unk_01(void);
	virtual void	Unk_02(void);
	virtual void	Unk_03(void);
	virtual void	Unk_04(void);
	virtual void	Unk_05(void);
	virtual void	Unk_06(void);
	virtual bool	Unk_07(void);
	virtual UInt32	Unk_08(void);	// return unk04
	virtual void	Unk_09(UInt32 arg);
	virtual void	Unk_0A(void);

//	void	** _vtbl;	// 00
	UInt32	unk04;		// 04
};

// 30
class BSIMusicType
{
public:
	virtual void	Unk_00(void);
	virtual void	Unk_01(void);
	virtual void	Unk_02(void);
	virtual void	Unk_03(void);
	virtual void	Unk_04(void);
	virtual void	Unk_05(void);
	virtual void	Unk_06(void);
	virtual void	Unk_07(void);

//	void		** _vtbl;	// 00
	UInt32		unk04;		// 04
	UInt8		unk08;		// 08
	UInt8		unk09;		// 09
	UInt16		unk0A;		// 0A
	float		unk0C;		// 0C
	UInt32		unk10;		// 10
	UnkArray	unk14;		// 14
	UnkArray	unk20;		// 20
	UInt32		unk2C;		// 2C
};

// 04
class BSIReverbType
{
public:
	virtual void	Unk_00(void);
	virtual void	Unk_01(void);
	virtual void	Unk_02(void);
	virtual void	Unk_03(void);
	virtual void	Unk_04(void);
	virtual void	Unk_05(void);
	virtual void	Unk_06(void);
	virtual void	Unk_07(void);
	virtual void	Unk_08(void);
	virtual void	Unk_09(void);
	virtual void	Unk_0A(void);

//	void	** _vtbl;	// 00
};

// 04
class BSISoundCategory
{
public:
	virtual ~BSISoundCategory();

	virtual void	Unk_01(void);
	virtual void	Unk_02(void);
	virtual void	Unk_03(void);
	virtual void	Unk_04(void);
	virtual void	Unk_05(void);
	virtual void	Unk_06(void);
	virtual void	Unk_07(void);
	virtual void	Unk_08(void);
	virtual void	Unk_09(void);
	virtual void	Unk_0A(void);
	virtual void	Unk_0B(void);

//	void	** _vtbl;	// 00
};

// 04
class BSISoundDescriptor
{
public:
	virtual ~BSISoundDescriptor();

	virtual void	Unk_01(void) = 0;
	virtual void	Unk_02(void) = 0;

//	void	** _vtbl;	// 00
};

class BGSSoundDescriptor : public BSISoundDescriptor
{
public:
	virtual void	Unk_01(void);
	virtual void	Unk_02(void);

	//	void	** _vtbl;	// 00
};

// 04
class BSISoundOutputModel
{
public:
	virtual ~BSISoundOutputModel();

	virtual void	Unk_01(void);
	virtual void	Unk_02(void);
	virtual void	Unk_03(void);
	virtual void	Unk_04(void);
	virtual void	Unk_05(void);
	virtual void	Unk_06(void);
	virtual void	Unk_07(void);
	virtual void	Unk_08(void);
	virtual void	Unk_09(void);

//	void	** _vtbl;	// 00
};

class BSIPlaybackCharacteristics
{
public:
	virtual ~BSIPlaybackCharacteristics();

	virtual UInt8	GetFrequencyShift(void) = 0;
	virtual UInt8	GetFrequencyVariance(void) = 0;
	virtual UInt8	GetPriority(void) = 0;
	virtual UInt16	GetDBAttenuation(void) = 0;
	virtual UInt8	GetDBVariance(void) = 0;
};

class SoundPlaybackCharacteristics : public BSIPlaybackCharacteristics
{
public:
	virtual UInt8	GetFrequencyShift(void);
	virtual UInt8	GetFrequencyVariance(void);
	virtual UInt8	GetPriority(void);
	virtual UInt16	GetDBAttenuation(void);
	virtual UInt8	GetDBVariance(void);

	UInt8	frequencyShift;
	UInt8	frequencyVariance;
	UInt8	priority;
	UInt8	dbVariance;
	UInt16	dbAttenuation;			// CK Value * 100
	UInt16	pad1A;
	UInt32	unk20;
	BGSSoundOutput	* soundOutput;
	UInt32	unk28;
	UInt32	unk2C;
};

class BGSStandardSoundDef : public BGSSoundDescriptor
{
public:
	virtual ~BGSStandardSoundDef();

	BGSSoundCategory	* soundCategory;
	UInt32				unk08;
	UInt32				unk0C;
	UInt32				unk10;
	UInt32				unk14;
	SoundPlaybackCharacteristics soundCharacteristics;
};

// 04
class BGSOpenCloseForm
{
public:
	virtual ~BGSOpenCloseForm();

//	void	** _vtbl;	// 00
};

class ActiveEffect;
class Character;
// C
class MagicTarget
{
public:
	virtual ~MagicTarget();

	virtual UInt8					Unk_01(int);
	virtual Character				* Unk_02(void);
	virtual UInt8					Unk_03(void);
	virtual bool					Unk_04(void);
	virtual int						Unk_05(int);
	virtual bool					Unk_06(void); // pure   018
	virtual tList<ActiveEffect>		* GetActiveEffects(void); // pure     01C
	virtual int						Unk_08(int);
	virtual void					Unk_09(int);
	virtual double					Unk_0A(int, int, int);
	virtual UInt8					Unk_0B(int, int, int);

//	void	** _vtbl;	// 00
	UInt32 unk04;		// 04
	UInt32 unk08;		// 08
};

STATIC_ASSERT(sizeof(MagicTarget) == 0xC);

// 20
class Condition
{
public:
	enum ComparisonFlags {
		kComparisonFlag_And = 0x00,
		kComparisonFlag_Or = 0x01,
		kComparisonFlag_Equal = 0x00,
		kComparisonFlag_UseAliases = 0x02,
		kComparisonFlag_Global = 0x04,
		kComparisonFlag_UsePackData = 0x08,
		kComparisonFlag_SwapTarget = 0x10,
		kComparisonFlag_NotEqual = 0x20,
		kComparisonFlag_Greater = 0x40,
		kComparisonFlag_GreaterEqual = 0x60,
		kComparisonFlag_Less = 0x80,
		kComparisonFlag_LessEqual = 0xA0
	};
	enum ReferenceTypes {
		kReferenceType_None = 0,
		kReferenceType_Target,
		kReferenceType_Reference,
		kReferenceType_CombatTarget,
		kReferenceType_LinkedRef,
		kReferenceType_Alias,
		kReferenceType_PackageData,
		kReferenceType_EventData
	};
	Condition	* next;					// 00
	UInt32		compareValue;			// 04
	UInt32		unk08;					// 08
	UInt32		unk0C;					// 0C - FFFFFFFF
	UInt16		functionId;				// 10
	UInt8		unk12;					// 12
	UInt8		unk13;					// 13
	UInt32		param1;					// 14
	UInt32		param2;					// 18
	UInt8		comparisonType;			// 1C
	UInt8		referenceType;			// 1D
};

STATIC_ASSERT(sizeof(Condition) == 0x20);


class TintMask
{
public:
	TintMask()
	{
		alpha = 0.0;
		tintType = 0;
		texture = NULL;
	};
	~TintMask() { };

#ifdef PAPYRUS_CUSTOM_CLASS
	enum { kTypeID = 300 };
#endif

	enum {
		kMaskType_Frekles = 0,
		kMaskType_Lips,
		kMaskType_Cheeks,
		kMaskType_Eyeliner,
		kMaskType_UpperEyeSocket,
		kMaskType_LowerEyeSocket,
		kMaskType_SkinTone,
		kMaskType_WarPaint,
		kMaskType_FrownLines,
		kMaskType_LowerCheeks,
		kMaskType_Nose,
		kMaskType_Chin,
		kMaskType_Neck,
		kMaskType_Forehead,
		kMaskType_Dirt
	};
	TESTexture	* texture;

	union {
		struct Color {
			UInt8   red, green, blue, alpha; // The alpha isn't actually used here so its usually zero
		} color;
		UInt32 abgr;
	};

	float		alpha;
	UInt32		tintType;

	UInt32 ToARGB();
};

// ??
class ActorWeightData
{
public:
	UInt32	unk00;		// 00 - Refcount?
	void	* unk04;	// 04
	void	* unk08;	// 08

	MEMBER_FN_PREFIX(ActorWeightData);
	DEFINE_MEMBER_FN(UpdateWeightData, void, 0x0046D690);
	// DEFINE_MEMBER_FN(Unk_02, void, 0x004145F0);
};

// ??
class ActorWeightModel
{
public:
	enum {
		kWeightModel_Small = 0,
		kWeightModel_Large = 1
	};
	ActorWeightData * weightData;
};

class BSFixedStringCI;

template<class A>
class DoNothingUnhandledPolicy
{
public:
	virtual ~DoNothingUnhandledPolicy();
};

// 2C
template<class A, class B, class C, class D>
class BSResponse : public BSIntrusiveRefCounted, public DoNothingUnhandledPolicy<C>
{
public:
	UInt32	unk08;	// 08
	UInt32	unk0C;	// 0C
	UInt32	unk10;	// 10 - NiNode?
	UInt32	unk14;	// 14 - NiNode?
	UInt32	unk18;	// 18 - NiNode?
	UInt32	unk1C;	// 1C
	UInt32	unk20;	// 20 - NiNode?
	UInt32	unk24;	// 24
	UInt32	unk28;	// 28
};
STATIC_ASSERT(sizeof(BSResponse<BSFixedStringCI, Actor, BSFixedStringCI, DoNothingUnhandledPolicy<BSFixedStringCI>>) == 0x2C);

class MiddleProcess
{
public:
	// 8
	struct EffectListNode
	{
		ActiveEffect	* effect;	// 0
		EffectListNode	* next;		// 4
	};

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
	float	unk2C;	// 2C

	struct Data30
	{
		UInt32	unk00;	// 00
		UInt32	unk04;	// 04
		UInt32	unk08;	// 08
		UInt32	unk0C;	// 0C
		UInt32	unk10;	// 10
		UInt32	unk14;	// 14
		UInt32	unk18;	// 18 - (AggroRadius Violated - 14)
	};

	Data30	* unk30;	// 30
	UInt32	unk34;	// 34
	UInt32	unk38;	// 38
	UInt32	unk3C;	// 3C
	float	unk40;	// 40
	UInt32	unk44;	// 44
	UInt32	unk48;	// 48
	UInt32	unk4C;	// 4C
	UInt32	unk50;	// 50
	UInt32	unk54;	// 54
	UInt32	unk58;	// 58
	UInt32	unk5C;	// 5C
	float	unk60;	// 60
	UInt32	unk64;	// 64
	UInt32	unk68;	// 68
	UInt32	unk6C;	// 6C
	UInt32	unk70;	// 70
	UInt32	unk74;	// 74
	UInt32	unk78;	// 78
	UInt32	unk7C;	// 7C
	UInt32	unk80;	// 80
	float	unk84;	// 84
	float	unk88;	// 88
	float	unk8C;	// 8C
	float	unk90;	// 90
	float	unk94;	// 94
	float	unk98;	// 98
	UInt32	unk9C;	// 9C
	UInt32	unkA0;	// A0
	BSResponse<BSFixedStringCI, Actor, BSFixedStringCI, DoNothingUnhandledPolicy<BSFixedStringCI>>	unkA4;	// A4
	UInt32	unkD0;	// D0
	void *	unkD4;	// D4 - NiNode?
	void *	unkD8;	// D8 - NiNode?
	UInt32	unkDC;	// D8
	void *	unkE0;	// E0 - BSFaceGenNiNode?
	void *	unkE4;	// E4
	UInt32	unkE8;	// E8
	UInt32	unkEC;	// EC
	UInt32	unkF0;	// F0
	UInt32	unkF4;	// F4
	EffectListNode *	effectList;	// F8
	void * unkFC;	// FC
	void * unk100;	// 100
	void * unk104;	// 104
	UInt32	unk108;	// 108
	UInt32	unk10C;	// 10C
	UInt32	unk110;	// 110
	UInt32	unk114;	// 114
	UInt32	unk118;	// 118
	UInt32	unk11C;	// 11C
	UInt32	unk120;	// 120
	UInt32	unk124;	// 124
	UInt32	unk128;	// 128
	UInt32	unk12C;	// 12C - FFFFFFFF
	UInt32	unk130;	// 130
	UInt32	unk134;	// 134 - FF7FFFFF
	UInt32	furnitureHandle;	// 138
	UInt32	unk13C;	// 13C
	UInt32	unk140;	// 140
	UInt32	unk144;	// 144
	TESIdleForm	* currentIdle;	// 148
	UInt32	unk14C; // 14C
	UInt32	unk150;	// 150
	UInt32	unk154;	// 154
	UInt32	unk158;	// 158
	void *	unk15C;	// 15C - hkCharacterProxy?
	void *	unk160;	// 160
	UInt32	unk164;	// 164
	UInt32	unk168;	// 168
	UInt32	unk16C;	// 16C
	UInt32	unk170;	// 170
	UInt32	unk174;	// 174
	void *	unk178;	// 178
	UInt32	unk17C;	// 17C
	UInt32	unk180;	// 180
	UInt32	unk184;	// 184
	UInt32	unk188;	// 188
	UInt32	unk18C;	// 18C
	UInt32	unk190;	// 190
	UInt32	unk194;	// 194
	UInt32	unk198;	// 198
	UInt32	unk19C;	// 19C
	UInt32	unk1A0;	// 1A0
	UInt32	unk1A4;	// 1A4
	float	unk1A8;	// 1A8
	UInt32	unk1AC;	// 1AC
	UInt32	unk1B0;	// 1B0
	float	actorAlpha;	// 1B4
	// More?...
};

STATIC_ASSERT(offsetof(MiddleProcess, currentIdle) == 0x148);
STATIC_ASSERT(offsetof(MiddleProcess, furnitureHandle) == 0x138);
STATIC_ASSERT(offsetof(MiddleProcess, actorAlpha) == 0x1B4);

// A0
class ActorProcessManager
{
public:
	enum {
		kEquippedHand_Left = 0,
		kEquippedHand_Right = 1
	};
	enum {
		kFlags_None = 0,
		kFlags_Unk01 = 1,
		kFlags_Unk02 = 2,
		kFlags_Unk03 = 4,
		kFlags_DrawHead = 8,
		kFlags_Mobile = 16,
		kFlags_Reset = 32
	};
	UInt32	unk00;						// 00
	MiddleProcess	* middleProcess;	// 04
	void	* unk08;					// 08
	UInt32	unk0C[(0x54 - 0x0C) >> 2];	// 0C
	float	timeOfDeath;				// 54 - GetTimeDead = (GameDaysPassed*24) - timeOfDeath
	UInt32	unk58[(0x68 - 0x58) >> 2];	// 58
	TESForm	* equippedObject[2];		// 68
	UInt32	unk70[(0x98 - 0x70) >> 2];	// 70
	UInt8	unk98;						// 98
	UInt8	unk9A;						// 9A
	SInt8	unk9B;						// 9B
	UInt8	unk9C[(0xA0 - 0x9C)];		// 9C

	MEMBER_FN_PREFIX(ActorProcessManager);
	DEFINE_MEMBER_FN(SetEquipFlag, void, 0x0071F520, UInt8 flags);
	DEFINE_MEMBER_FN(UpdateEquipment, void, 0x007031A0, Actor * actor);
	DEFINE_MEMBER_FN(SetDataFlag, void, 0x006FD1A0, UInt32 flag); // Sets a number on the 0x08 object
};

STATIC_ASSERT(offsetof(ActorProcessManager, equippedObject) == 0x68);
STATIC_ASSERT(sizeof(ActorProcessManager) == 0xA0);

// 3C
class BGSSkillPerkTreeNode
{
public:
	virtual ~BGSSkillPerkTreeNode();

	UInt32	index;		// 04
	tArray<BGSSkillPerkTreeNode*> connections;	// 08
	tArray<BGSSkillPerkTreeNode*> connectees;	// 14
	BGSPerk	* perk;		// 20
	UInt32	unk24;		// 24
	UInt32	gridX;		// 28
	UInt32	gridY;		// 2C
	ActorValueInfo	* skill;	// 30
	float	horizontalPos;		// 34
	float	verticalPos;		// 38

	class PerkVisitor
	{
	public:
		virtual bool Accept(BGSPerk * node) = 0;
	};

	bool VisitPerks(PerkVisitor & visitor)
	{
		if(visitor.Accept(perk))
			return true;

		for(UInt32 i = 0; i < connections.count; i++)
		{
			BGSSkillPerkTreeNode* connector = NULL;
			connections.GetNthItem(i, connector);

			if(connector->VisitPerks(visitor))
				return true;
		}

		return false;
	}
};

class PlayerSkills
{
public:

	enum {
		kAdvanceableSkillOffset = 6,
		kNumAdvanceableSkills = 18
	};

	struct StatData {
		struct LevelData {
			float level;		// 00
			float points;		// 04
			float pointsMax;	// 08
		};

		float		levelPoints;							// 00
		float		levelPointsMax;							// 04
		LevelData	levelData[kNumAdvanceableSkills];		// 08
		UInt32		legendaryLevel[kNumAdvanceableSkills];	// 90
	};

	StatData * data;

	SInt32 ResolveAdvanceableSkillId(SInt32 actorValue);

	float GetSkillPoints(BSFixedString actorValue);
	void SetSkillPoints(BSFixedString actorValue, float points);

	SInt32 GetSkillLegendaryLevel(BSFixedString actorValue);
	void SetSkillLegendaryLevel(BSFixedString actorValue, UInt32 level);

	MEMBER_FN_PREFIX(PlayerSkills);
	DEFINE_MEMBER_FN(GetSkillData, UInt32, 0x00760150, UInt32 actorValue, float * level, float * points, float * pointsMax, UInt32 * unk6);
	DEFINE_MEMBER_FN(IncrementLegendary, UInt32, 0x00760110, UInt32 actorValue);
};

// 08
class TESRegionData
{
public:
	virtual ~TESRegionData();

	virtual void Unk_01(void);
	virtual void Unk_02(void);
	virtual SInt32 GetType(void); // pure
	virtual void Unk_04(void); // pure - Init?
	virtual void Unk_05(void); // pure
	virtual void Unk_06(void); // pure
	virtual void Unk_07(void); // pure

	UInt8	unk04;	// 04
	UInt8	unk05;	// 05
	UInt8	unk06;	// 06
	UInt8	pad07;	// 07
};

class TESRegionObjectBase
{
public:
	virtual ~TESRegionObjectBase();

	virtual void Unk_01(void); // pure
	virtual void Unk_02(void); // pure
};

// 0C
class TESRegionGrassObject : public TESRegionObjectBase
{
public:
	virtual ~TESRegionGrassObject();

	virtual void Unk_03(void);
	virtual void Unk_04(void);
	virtual void Unk_05(void);
	virtual void Unk_06(void);

	UInt32	unk04;	// 04
	UInt32	unk08;	// 08
};

// 14
class TESRegionGrassObjectList : public tList<TESRegionGrassObject*>
{
public:
	virtual ~TESRegionGrassObjectList();

	UInt32	unk0C;	// 0C
	UInt32	unk10;	// 10
};

// 10
class TESRegionList : public tList<TESRegion*>
{
public:
	virtual ~TESRegionList();

	UInt32	unk0C;	// 0C
};

// 0C
class TESRegionDataGrass : public TESRegionData
{
public:
	virtual ~TESRegionDataGrass();

	TESRegionGrassObjectList * grassObjectList;	// 0C
};

// 18
class TESRegionDataSound : public TESRegionData
{
public:
	virtual ~TESRegionDataSound();

	BGSMusicType	* musicType;	// 08

	struct SoundData
	{
		BGSSoundDescriptorForm	* soundDescriptor;	// 00
		UInt32					flags;				// 04
		float					chance;				// 08
	};

	tArray<SoundData*>	soundData;	// 0C
};

// 0C
class TESRegionDataLandscape : public TESRegionData
{
public:
	virtual ~TESRegionDataLandscape();

	UInt32	unk08;	// 08
};

// 0C
class TESRegionDataMap : public TESRegionData
{
public:
	virtual ~TESRegionDataMap();

	UInt32	unk08;	// 08
};

// 10
class TESRegionDataWeather : public TESRegionData
{
public:
	virtual ~TESRegionDataWeather();

	struct WeatherData
	{
		TESWeather	* weather;		// 00
		UInt32		chance;			// 04
		TESGlobal	* globalChance;	// 08
	};

	tList<WeatherData*> weatherData;	// 08
};