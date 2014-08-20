#pragma once

#include "Utilities.h"
#include "GameTypes.h"

template <typename T> class BSTEventSink;

enum EventResult
{
	kEvent_Continue = 0,
	kEvent_Abort
};

// 030
template <typename EventT, typename EventArgT = EventT>
class EventDispatcher
{
	typedef BSTEventSink<EventT> SinkT;

	SimpleLock			lock;				// 000
	tArray<SinkT*>		eventSinks;			// 008
	tArray<SinkT*>		addBuffer;			// 014 - schedule for add
	tArray<SinkT*>		removeBuffer;		// 020 - schedule for remove
	bool				stateFlag;			// 02C - some internal state changed while sending
	char				pad[3];

	MEMBER_FN_PREFIX(EventDispatcher);
	DEFINE_MEMBER_FN(AddEventSink_Internal, void, 0x006E3E30, SinkT * eventSink);
	DEFINE_MEMBER_FN(RemoveEventSink_Internal, void, 0x008CE0C0, SinkT * eventSink);
	DEFINE_MEMBER_FN(SendEvent_Internal, void, 0x006EBC10, EventArgT * evn);

public:

	EventDispatcher() : stateFlag(false) {}

	void AddEventSink(SinkT * eventSink)		{ CALL_MEMBER_FN(this,AddEventSink_Internal)(eventSink); }
	void RemoveEventSink(SinkT * eventSink)		{ CALL_MEMBER_FN(this,RemoveEventSink_Internal)(eventSink); }
	void SendEvent(EventArgT * evn)				{ CALL_MEMBER_FN(this,SendEvent_Internal)(evn); }
};
STATIC_ASSERT(sizeof(EventDispatcher<void*>) == 0x30);

// 04
template <typename T>
class BSTEventSink
{
public:
	virtual ~BSTEventSink();
	virtual	EventResult	ReceiveEvent(T * evn, EventDispatcher<T> * dispatcher); // pure
//	void	** _vtbl;	// 00
};

// 30
template <typename T>
class BSTEventSource
{
public:
	virtual ~BSTEventSource();

//	void	** _vtbl;	// 00
	UInt32 unk04[11];	// 04
};

STATIC_ASSERT(sizeof(BSTEventSource<void*>) == 0x30);

// 08
struct TESSleepStartEvent
{
	float startTime;	// 00
	float endTime;		// 04
};

// 08
struct MenuOpenCloseEvent
{
	BSFixedString	menuName;	// 00
	bool			opening;	// 04
	char			pad[3];
};

struct TESFurnitureEvent
{
};

// Todo
struct MenuModeChangeEvent
{
};

class TESObjectREFR;
class TESForm;
class ActiveEffect;

struct TESActiveEffectApplyRemoveEvent
{
	TESObjectREFR	* caster;
	TESObjectREFR	* target;
	UInt32			unk08;
	UInt32			unk0C;
	UInt32			unk10;
	UInt32			unk14;
	UInt32			unk18; // Flags?
	UInt32			unk1C; // Use effect2 if this is 1
	TESForm			* source; // Not really sure what this is, probably the extra form
	ActiveEffect	* effect1;
	ActiveEffect	* effect2;
};

struct TESQuestStageEvent
{
	void			* finishedCallback;
	UInt32			formId;
	UInt32			stage;
};

// This isn't necessarily correct, just there to receive events
struct TESHarvestEvent
{
	struct ItemHarvested
	{
		// Unknown
	};
};

struct LevelIncrease
{
	struct Event
	{
		TESForm		* character;
		UInt32		level;
	};
};

struct SkillIncrease
{
	struct Event
	{
		// Unknown
	};
};
struct WordLearned
{
	struct Event
	{
		// Unknown
	};
};
struct WordUnlocked
{
	struct Event
	{
		// Unknown
	};
};
struct Inventory
{
	struct Event
	{
		// Unknown
	};
};
struct Bounty
{
	struct Event
	{
		// Unknown
	};
};
struct QuestStatus
{
	struct Event
	{
		// Unknown
	};
};
struct ObjectiveState
{
	struct Event
	{
		// Unknown
	};
};
struct Trespass
{
	struct Event
	{
		// Unknown
	};
};
struct FinePaid
{
	struct Event
	{
		// Unknown
	};
};
struct HoursPassed
{
	struct Event
	{
		// Unknown
	};
};
struct DaysPassed
{
	struct Event
	{
		// Unknown
	};
};
struct DaysJailed
{
	struct Event
	{
		// Unknown
	};
};
struct CriticalHitEvent
{
	struct Event
	{
		// Unknown
	};
};
struct DisarmedEvent
{
	struct Event
	{
		// Unknown
	};
};
struct ItemsPickpocketed
{
	struct Event
	{
		// Unknown
	};
};
struct ItemSteal
{
	struct Event
	{
		// Unknown
	};
};
struct ItemCrafted
{
	struct Event
	{
		// Unknown
	};
};
struct LocationDiscovery
{
	struct Event
	{
		// Unknown
	};
};
struct Jailing
{
	struct Event
	{
		// Unknown
	};
};
struct ChestsLooted
{
	struct Event
	{
		// Unknown
	};
};
struct TimesTrained
{
	struct Event
	{
		// Unknown
	};
};
struct TimesBartered
{
	struct Event
	{
		// Unknown
	};
};
struct ContractedDisease
{
	struct Event
	{
		// Unknown
	};
};
struct SpellsLearned
{
	struct Event
	{
		// Unknown
	};
};
struct DragonSoulGained
{
	struct Event
	{
		// Unknown
	};
};
struct SoulGemsUsed
{
	struct Event
	{
		// Unknown
	};
};
struct SoulsTrapped
{
	struct Event
	{
		// Unknown
	};
};
struct PoisonedWeapon
{
	struct Event
	{
		// Unknown
	};
};
struct ShoutAttack
{
	struct Event
	{
		// Unknown
	};
};
struct JailEscape
{
	struct Event
	{
		// Unknown
	};
};
struct GrandTheftHorse
{
	struct Event
	{
		// Unknown
	};
};
struct AssaultCrime
{
	struct Event
	{
		// Unknown
	};
};
struct MurderCrime
{
	struct Event
	{
		// Unknown
	};
};
struct LocksPicked
{
	struct Event
	{
		// Unknown
	};
};
struct LocationCleared
{
	struct Event
	{
		// Unknown
	};
};
struct ShoutMastered
{
	struct Event
	{
		// Unknown
	};
};

struct TESCombatEvent 
{
	TESObjectREFR	* source;	// 00
	TESObjectREFR	* target;	// 04
	UInt32			state;		// 08
};

struct TESDeathEvent
{
	TESObjectREFR	* source;	// 00
};

struct TESHitEvent
{
	TESObjectREFR	* target;			// 00
	TESObjectREFR	* caster;			// 04
	UInt32			sourceFormID;		// 08
	UInt32			projectileFormID;	// 0C

	enum
	{
		kFlag_PowerAttack = (1 << 0),
		kFlag_SneakAttack = (1 << 1),
		kFlag_Bash		  = (1 << 2),
		kFlag_Blocked	  = (1 << 3)
	};

	UInt32			flags;				// 10
	void			* unk14[7];			// 14
};

struct BGSFootstepEvent
{
	UInt32	actorHandle;
};

template <>
class BSTEventSink <TESCombatEvent>
{
public:
	virtual ~BSTEventSink() {}; // todo?
	virtual	EventResult ReceiveEvent(TESCombatEvent * evn, EventDispatcher<TESCombatEvent> * dispatcher) = 0;
};

template <>
class BSTEventSink <TESDeathEvent>
{
public:
	virtual ~BSTEventSink() {}; // todo?
	virtual	EventResult ReceiveEvent(TESDeathEvent * evn, EventDispatcher<TESDeathEvent> * dispatcher) = 0;
};

template <>
class BSTEventSink <TESSleepStartEvent>
{
public:
	virtual ~BSTEventSink() {}	// todo?
	virtual	EventResult ReceiveEvent(TESSleepStartEvent * evn, EventDispatcher<TESSleepStartEvent> * dispatcher) = 0;
};

template <>
class BSTEventSink <MenuOpenCloseEvent>
{
public:
	virtual ~BSTEventSink() {}	// todo?
	virtual	EventResult ReceiveEvent(MenuOpenCloseEvent * evn, EventDispatcher<MenuOpenCloseEvent> * dispatcher) = 0;
};

template <>
class BSTEventSink <TESQuestStageEvent>
{
public:
	virtual ~BSTEventSink() {}	// todo?
	virtual	EventResult ReceiveEvent(TESQuestStageEvent * evn, EventDispatcher<TESQuestStageEvent> * dispatcher) = 0;
};

template <>
class BSTEventSink <LevelIncrease::Event>
{
public:
	virtual ~BSTEventSink() {}	// todo?
	virtual	EventResult ReceiveEvent(LevelIncrease::Event * evn, EventDispatcher<LevelIncrease::Event> * dispatcher) = 0;
};

template <>
class BSTEventSink <TESHarvestEvent::ItemHarvested>
{
public:
	virtual ~BSTEventSink() {}	// todo?
	virtual	EventResult ReceiveEvent(TESHarvestEvent::ItemHarvested * evn, EventDispatcher<TESHarvestEvent::ItemHarvested> * dispatcher) = 0;
};

template <>
class BSTEventSink <TESHitEvent>
{
public:
	virtual ~BSTEventSink() {}	// todo?
	virtual	EventResult ReceiveEvent(TESHitEvent * evn, EventDispatcher<TESHitEvent> * dispatcher) = 0;
};

// For testing
//extern EventDispatcher<TESSleepStartEvent> * g_sleepStartEventDispatcher;
extern EventDispatcher<TESCombatEvent> * g_combatEventDispatcher;
extern EventDispatcher<TESDeathEvent> * g_deathEventDispatcher;
extern EventDispatcher<BGSFootstepEvent> * g_footstepEventDispatcher;
extern EventDispatcher<TESQuestStageEvent> * g_questStageEventDispatcher;
extern EventDispatcher<TESHarvestEvent::ItemHarvested> * g_harvestEventDispatcher;
extern EventDispatcher<LevelIncrease::Event> * g_levelIncreaseEventDispatcher;
extern EventDispatcher<TESHitEvent> * g_hitEventDispatcher;
