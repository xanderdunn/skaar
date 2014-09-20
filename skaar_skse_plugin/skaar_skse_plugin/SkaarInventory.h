#include "skse/PapyrusNativeFunctions.h"
#include "skse/GameReferences.h"
#include "skse/GameExtraData.h"

namespace SkaarInventory {

	bool SkaarEntryDataListContainsEntryData(ExtraContainerChanges::EntryDataList *entryDataList, ExtraContainerChanges::EntryData *entry);

	UInt32 SkaarEntryDataListCountOfForm(ExtraContainerChanges::EntryDataList *entryDataList, TESForm* form);

	ExtraContainerChanges::EntryDataList *SkaarGetAllItems(TESObjectREFR* container, FormType type);

	UInt32 SkaarContainerCountOfForm(TESObjectREFR* container, TESForm *form);
}
