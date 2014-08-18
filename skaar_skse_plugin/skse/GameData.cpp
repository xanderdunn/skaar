#include "GameData.h"

TES	** g_TES = (TES **)0x01B2E864;

DataHandler * DataHandler::GetSingleton()
{
	return *((DataHandler **)0x012E2CB8);
}

class LoadedModFinder
{
	const char * m_stringToFind;

public:
	LoadedModFinder(const char * str) : m_stringToFind(str) { }

	bool Accept(ModInfo* modInfo)
	{
		return _stricmp(modInfo->name, m_stringToFind) == 0;
	}
};

const ModInfo * DataHandler::LookupModByName(const char * modName)
{
	return modList.modInfoList.Find(LoadedModFinder(modName));
}

UInt8 DataHandler::GetModIndex(const char* modName)
{
	return modList.modInfoList.GetIndexOf(LoadedModFinder(modName));
}

void BGSSaveLoadManager::Save(const char * name)
{
	CALL_MEMBER_FN(this, Save_Internal)(name, -1, 0);
}

void BGSSaveLoadManager::Load(const char * name)
{
	CALL_MEMBER_FN(this, Load_Internal)(name, -1, 0, 1);
}

BGSSaveLoadManager * BGSSaveLoadManager::GetSingleton(void)
{
	return *((BGSSaveLoadManager **)0x01B2E3A8);
}

MiscStatManager * MiscStatManager::GetSingleton(void)
{
	return (MiscStatManager *)0x012E6C34;
}

MiscStatManager::MiscStat * MiscStatManager::Get(const char * name)
{
	for(UInt32 i = 0; i < m_numStats; i++)
	{
		MiscStat	* stat = &m_stats[i];

		if(!_stricmp(name, stat->name))
		{
			return stat;
		}
	}

	return NULL;
}

EquipManager * EquipManager::GetSingleton(void)
{
	return *((EquipManager **)0x012E5FAC);
}

const _GetEitherHandSlot GetEitherHandSlot = (_GetEitherHandSlot)0x0054C870;
const _GetRightHandSlot GetRightHandSlot = (_GetRightHandSlot)0x0054C840;
const _GetLeftHandSlot GetLeftHandSlot = (_GetLeftHandSlot)0x0054C810;
const _LookupActorValueByName LookupActorValueByName = (_LookupActorValueByName)0x005AD5F0;

ActorValueList * ActorValueList::GetSingleton(void)
{
	return *((ActorValueList **)0x012E2BDC);
}

ActorValueInfo * ActorValueList::GetActorValue(UInt32 id)
{
	return (id < kNumActorValues) ? actorValues[id] : NULL;
}

UInt32 ActorValueList::ResolveActorValueByName(const char * name)
{
	UInt32 id = LookupActorValueByName(name);
	if(id >= kNumActorValues)
		return 255;

	return id;
}

DefaultObjectList * DefaultObjectList::GetSingleton(void)
{
	return (DefaultObjectList*)0x01245C60;
}

FaceMorphList * FaceMorphList::GetSingleton(void)
{
	return (FaceMorphList *)0x012593A8;
}

FacePresetList * FacePresetList::GetSingleton(void)
{
	return (FacePresetList *)0x0125948C;
}

const _ChangeActorHeadPart ChangeActorHeadPart = (_ChangeActorHeadPart)0x005AA4E0;
const _UpdatePlayerTints UpdatePlayerTints = (_UpdatePlayerTints)0x0087EE70;

FaceGen * FaceGen::GetSingleton(void)
{
	return *((FaceGen **)0x012E3538);
}

const _GetActorBaseOverlays GetActorBaseOverlays = (_GetActorBaseOverlays)0x00568280;
const _GetNumActorBaseOverlays GetNumActorBaseOverlays = (_GetNumActorBaseOverlays)0x005682F0;
const _ApplyMasksToRenderTarget ApplyMasksToRenderTarget = (_ApplyMasksToRenderTarget)0x005A90A0;

const _UpdateModelSkin UpdateModelSkin = (_UpdateModelSkin)0x005A9810; // Applies tint to ShaderType 5 nodes
const _UpdateModelHair UpdateModelHair = (_UpdateModelHair)0x005A9890; // Applies tint to ShaderType 6 nodes
const _UpdateModelFace UpdateModelFace = (_UpdateModelFace)0x005AA210;

const _CacheTRIFile CacheTRIFile = (_CacheTRIFile)0x005A2B40;

void MagicFavorites::ClearHotkey(SInt8 idx)
{
	if (idx < 0 || idx >= hotkeys.count)
		return;

	hotkeys[idx] = NULL;
}

void MagicFavorites::SetHotkey(TESForm * form, SInt8 idx)
{
	if (idx < 0 || idx >= hotkeys.count)
		return;

	SInt8 oldIdx = hotkeys.GetItemIndex(form);
	if (idx == oldIdx)
		return;

	if (IsFavorited(form))
	{
		hotkeys[oldIdx] = NULL;
		hotkeys[idx] = form;
	}	
}

TESForm * MagicFavorites::GetSpell(SInt8 idx)
{
	TESForm * form = NULL;
	hotkeys.GetNthItem(idx, form);
	return form;
}

bool MagicFavorites::IsFavorited(TESForm * form)
{
	SInt32 indexOut = -1;
	if (GetSortIndex(spells, form, indexOut) && indexOut != -1)
		return true;
	else
		return false;
}

GarbageCollector * GarbageCollector::GetSingleton(void)
{
	return *((GarbageCollector **)0x012E3540);
}

MenuTopicManager * MenuTopicManager::GetSingleton(void)
{
	return *((MenuTopicManager **)0x013105D8);
}

TESObjectREFR * MenuTopicManager::GetDialogueTarget()
{
	TESObjectREFR * refr = NULL;
	if(talkingHandle == (*g_invalidRefHandle) || talkingHandle == 0)
		return NULL;

	LookupREFRByHandle(&talkingHandle, &refr);
	return refr;
}

const _GetSky GetSky = (_GetSky)0x0043B920;
const _HasLOS HasLOS = (_HasLOS)0x008C0FA0;

PerkEntryPoints * PerkEntryPoints::GetSingleton(void)
{
	return (PerkEntryPoints *)0x012560D0;
}

const _CalculatePerkData CalculatePerkData = (_CalculatePerkData)0x0054B400;