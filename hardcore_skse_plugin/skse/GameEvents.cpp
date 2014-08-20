#include "GameEvents.h"

// For testing
// TESActivateEvent																														0x012E3E60
// TESActiveEffectApplyRemoveEvent																										0x012E3E90
// TESActorLocationChangeEvent																											0x012E3EC0
// TESBookReadEvent																														0x012E3EF0
// TESCellAttachDetachEvent																												0x012E3F20
// TESCellFullyLoadedEvent																												0x012E3F50
EventDispatcher<TESCombatEvent>* g_combatEventDispatcher = (EventDispatcher<TESCombatEvent>*) 0x012E4DB0;
// TESContainerChangedEvent																												0x012E3FE0
EventDispatcher<TESDeathEvent>* g_deathEventDispatcher = (EventDispatcher<TESDeathEvent>*) 0x012E4E10;
// TESDestructionStageChangedEvent																										0x012E4E40
// TESEnterBleedoutEvent																												0x012E4E70
// TESEquipEvent																														0x012E4EA0
// TESFormDeleteEvent																													0x012E4ED0
// TESFurnitureEvent																													0x012E4F00
// TESGrabReleaseEvent																													0x012E4F30
EventDispatcher<TESHitEvent>* g_hitEventDispatcher = (EventDispatcher<TESHitEvent>*) 0x012E4F60;
// TESLoadGameEvent																														0x012E4FC0
// TESLockChangedEvent																													0x012E4FF0
// TESMagicEffectApplyEvent																												0x012E5020
// TESMagicWardHitEvent																													0x012E5050
// TESMoveAttachDetachEvent																												0x012E5080
// TESObjectLoadedEvent																													0x012E50B0
// TESObjectREFRTranslationEvent																										0x012E50E0
// TESOpenCloseEvent																													0x012E5110
// TESPackageEvent																														0x012E5140
// TESPerkEntryRunEvent																													0x012E5170
// TESQuestInitEvent																													0x012E51A0
EventDispatcher<TESQuestStageEvent>* g_questStageEventDispatcher = (EventDispatcher<TESQuestStageEvent>*) 0x012E51D0;
// TESResetEvent																														0x012E5260
// TESResolveNPCTemplatesEvent																											0x012E5290
// TESSceneEvent																														0x012E52C0
// TESSceneActionEvent																													0x012E52F0
// TESScenePhaseEvent																													0x012E5320
// TESSellEvent																															0x012E5350
//EventDispatcher<TESSleepStartEvent>* g_sleepStartEventDispatcher = (EventDispatcher<TESSleepStartEvent>*) 0x012E4580;
// TESSleepStopEvent																													0x012E53B0
// TESSpellCastEvent																													0x012E53E0
// TESTopicInfoEvent																													0x012E5440
// TESTrackedStatsEvent																													0x012E5470
// TESTrapHitEvent																														0x012E54A0
// TESTriggerEvent																														0x012E54D0
// TESTriggerEnterEvent																													0x012E5500
// TESTriggerLeaveEvent																													0x012E5530
// TESUniqueIDChangeEvent																												0x012E5560
// TESSwitchRaceCompleteEvent																											0x012E55F0
// TESPlayerBowShotEvent																												0x012E5410


EventDispatcher<BGSFootstepEvent>* g_footstepEventDispatcher = (EventDispatcher<BGSFootstepEvent>*) 0x01B2E9C0;

// Story based events
EventDispatcher<TESHarvestEvent::ItemHarvested>* g_harvestEventDispatcher = (EventDispatcher<TESHarvestEvent::ItemHarvested>*) 0x012E5A74;
// Event	ActorKill																													0xDEADBEEF
// Event	ActorItemEquipped																											0xDEADBEEF
// Event	Pickpocket																													0xDEADBEEF
// Event	BooksRead																													0xDEADBEEF
EventDispatcher<LevelIncrease::Event>* g_levelIncreaseEventDispatcher = (EventDispatcher<LevelIncrease::Event>*) 0x01B39804;
// Event	SkillIncrease																												0xDEADBEEF
// Event	WordLearned																													0xDEADBEEF
// Event	WordUnlocked																												0xDEADBEEF
// Event	Inventory																													0xDEADBEEF
// Event	Bounty																														0xDEADBEEF
// Event	QuestStatus																													0xDEADBEEF
// Event	ObjectiveState																												0xDEADBEEF
// Event	Trespass																													0xDEADBEEF
// Event	FinePaid																													0xDEADBEEF
// Event	HoursPassed																													0xDEADBEEF
// Event	DaysPassed																													0xDEADBEEF
// Event	DaysJailed																													0xDEADBEEF
// Event	CriticalHitEvent																											0xDEADBEEF
// Event	DisarmedEvent																												0xDEADBEEF
// Event	ItemsPickpocketed																											0xDEADBEEF
// Event	ItemSteal																													0xDEADBEEF
// Event	ItemCrafted																													0xDEADBEEF
// Event	LocationDiscovery																											0xDEADBEEF
// Event	Jailing																														0xDEADBEEF
// Event	ChestsLooted																												0xDEADBEEF
// Event	TimesTrained																												0xDEADBEEF
// Event	TimesBartered																												0xDEADBEEF
// Event	ContractedDisease																											0xDEADBEEF
// Event	SpellsLearned																												0xDEADBEEF
// Event	DragonSoulGained																											0xDEADBEEF
// Event	SoulGemsUsed																												0xDEADBEEF
// Event	SoulsTrapped																												0xDEADBEEF
// Event	PoisonedWeapon																												0xDEADBEEF
// Event	ShoutAttack																													0xDEADBEEF
// Event	JailEscape																													0xDEADBEEF
// Event	GrandTheftHorse																												0xDEADBEEF
// Event	AssaultCrime																												0xDEADBEEF
// Event	MurderCrime																													0xDEADBEEF
// Event	LocksPicked																													0xDEADBEEF
// Event	LocationCleared																												0xDEADBEEF
// Event	ShoutMastered																												0xDEADBEEF
