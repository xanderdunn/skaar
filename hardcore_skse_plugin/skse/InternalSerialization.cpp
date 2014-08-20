#include <map>
#include "InternalSerialization.h"
#include "common/IFileStream.h"
#include <algorithm>
#include <string>
#include "PapyrusEvents.h"
#include "GameData.h"

// Internal

static UInt8	s_savefileIndexMap[0xFF];
static UInt8	s_numSavefileMods = 0;

void LoadModList(SKSESerializationInterface * intfc)
{
	_MESSAGE("Loading mod list:");

	DataHandler * dhand = DataHandler::GetSingleton();

	char name[0x104] = { 0 };
	UInt16 nameLen = 0;

	intfc->ReadRecordData(&s_numSavefileMods, sizeof(s_numSavefileMods));
	for (UInt32 i = 0; i < s_numSavefileMods; i++)
	{
		intfc->ReadRecordData(&nameLen, sizeof(nameLen));
		intfc->ReadRecordData(&name, nameLen);
		name[nameLen] = 0;

		UInt8 newIndex = dhand->GetModIndex(name);
		s_savefileIndexMap[i] = newIndex;
		_MESSAGE("\t(%d -> %d)\t%s", i, newIndex, &name);
	}
}

void SaveModList(SKSESerializationInterface * intfc)
{
	DataHandler * dhand = DataHandler::GetSingleton();
	UInt8 modCount = dhand->modList.loadedModCount;

	intfc->OpenRecord('MODS', 0);
	intfc->WriteRecordData(&modCount, sizeof(modCount));

	_MESSAGE("Saving mod list:");

	for (UInt32 i = 0; i < modCount; i++)
	{
		ModInfo * modInfo = dhand->modList.loadedMods[i];
		UInt16 nameLen = strlen(modInfo->name);
		intfc->WriteRecordData(&nameLen, sizeof(nameLen));
		intfc->WriteRecordData(modInfo->name, nameLen);
		_MESSAGE("\t(%d)\t%s", i, &modInfo->name);
	}
}

UInt8 ResolveModIndex(UInt8 modIndex)
{
	return (modIndex < s_numSavefileMods) ? s_savefileIndexMap[modIndex] : 0xFF;
}

//// Callbacks

void Core_RevertCallback(SKSESerializationInterface * intfc)
{
	g_menuOpenCloseRegs.Clear();
	g_inputKeyEventRegs.Clear();
	g_inputControlEventRegs.Clear();
	g_modCallbackRegs.Clear();
}

void Core_SaveCallback(SKSESerializationInterface * intfc)
{
	SaveModList(intfc);

	_MESSAGE("Saving menu open/close event registrations...");
	g_menuOpenCloseRegs.Save(intfc, 'MENR', 1);

	_MESSAGE("Saving key input event registrations...");
	g_inputKeyEventRegs.Save(intfc, 'KEYR', 1);

	_MESSAGE("Saving control input event registrations...");
	g_inputControlEventRegs.Save(intfc, 'CTLR', 1);

	_MESSAGE("Saving mod callback event registrations...");
	g_modCallbackRegs.Save(intfc, 'MCBR', 1);

	_MESSAGE("Saving crosshair ref event registrations...");
	g_crosshairRefEventRegs.Save(intfc, 'CHRR', 1);

	_MESSAGE("Saving camera event registrations...");
	g_cameraEventRegs.Save(intfc, 'CAMR', 1);

	_MESSAGE("Saving actor action event registrations...");
	g_actionEventRegs.Save(intfc, 'AACT', 1);
}

void Core_LoadCallback(SKSESerializationInterface * intfc)
{
	UInt32 type, version, length;

	while (intfc->GetNextRecordInfo(&type, &version, &length))
	{
		switch (type)
		{
		// Mod list
		case 'MODS':
			LoadModList(intfc);
			break;

		// Menu open/close events
		case 'MENR':
			_MESSAGE("Loading menu open/close event registrations...");
			g_menuOpenCloseRegs.Load(intfc, 1);
			
			break;

		// Key input events
		case 'KEYR':
			_MESSAGE("Loading key input event registrations...");
			g_inputKeyEventRegs.Load(intfc, 1);
			break;

		// Control input events
		case 'CTLR':
			_MESSAGE("Loading control input event registrations...");
			g_inputControlEventRegs.Load(intfc, 1);
			break;

		// Custom mod events
		case 'MCBR':
			_MESSAGE("Loading mod callback event registrations...");
			g_modCallbackRegs.Load(intfc, 1);
			break;

		// Crosshair rev events
		case 'CHRR':
			_MESSAGE("Loading crosshair ref event registrations...");
			g_crosshairRefEventRegs.Load(intfc, 1);
			break;

		// Camera events
		case 'CAMR':
			_MESSAGE("Loading camera event registrations...");
			g_cameraEventRegs.Load(intfc, 1);
			break;

		// Actor Actions events
		case 'AACT':
			_MESSAGE("Loading actor action event registrations...");
			g_actionEventRegs.Load(intfc, 1);
			break;

		default:
			_MESSAGE("Unhandled chunk type in Core_LoadCallback: %08X (%.4s)", type, &type);
			continue;
		}
	}
}

void Init_CoreSerialization_Callbacks()
{
	Serialization::SetUniqueID(0, 0);
	Serialization::SetRevertCallback(0, Core_RevertCallback);
	Serialization::SetSaveCallback(0, Core_SaveCallback);
	Serialization::SetLoadCallback(0, Core_LoadCallback);
}
