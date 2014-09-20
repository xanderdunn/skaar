#include "SkaarInventory.h"

#include "skse/GameExtraData.h"
#include "skse/PapyrusObjectReference.h"
#include "skse/GameExtraData.h"


namespace SkaarInventory {

	// TODO: Use GetAllForms instead of this
	// Create a new list of all items of the given type that are in the given container
	ExtraContainerChanges::EntryDataList *SkaarGetAllItems(TESObjectREFR* container, FormType type) {
		// _DMESSAGE("SkaarGetAllItems called");
		ExtraContainerChanges* containerChanges = static_cast<ExtraContainerChanges*>(container->extraData.GetByType(kExtraData_ContainerChanges));
		if (!containerChanges) {
			return NULL;
		}
		ExtraContainerChanges::EntryDataList *containerItems = containerChanges->data->objList;
		ExtraContainerChanges::EntryDataList *itemsList = ExtraContainerChanges::EntryDataList::Create();
		if (!itemsList || !containerItems || containerItems->Count() <= 0) {
			return NULL;
		}
		// _DMESSAGE("SkaarGetAllItems about to loop");
		for (size_t i = 0; i < containerItems->Count(); i++) {
			ExtraContainerChanges::EntryData *currentEntry = containerItems->GetNthItem(i);
			if (currentEntry) {
				if (currentEntry->type->GetFormType() == type || type == kFormType_None) {
					//_DMESSAGE("Found TESForm %d", currentEntry->type->formID);
					itemsList->Push(currentEntry);
				}
			}
		}
		// _DMESSAGE("SkaarGetAllItems about to return");
		return itemsList;
	}

	bool SkaarEntryDataListContainsEntryData(ExtraContainerChanges::EntryDataList *entryDataList, ExtraContainerChanges::EntryData *entry) {
		for (size_t i = 0; i < entryDataList->Count(); i++) {
			ExtraContainerChanges::EntryData *currentEntry = entryDataList->GetNthItem(i);
			if (currentEntry->type == entry->type) {
				return true;
			}
		}
		return false;
	}

	UInt32 SkaarEntryDataListCountOfForm(ExtraContainerChanges::EntryDataList *entryDataList, TESForm *form) {
		for (size_t i = 0; i < entryDataList->Count(); i++) {
			ExtraContainerChanges::EntryData *currentEntry = entryDataList->GetNthItem(i);
			if (currentEntry->type == form) {
				return currentEntry->countDelta;
			}
		}
		return 0;
	}

	UInt32 SkaarContainerCountOfForm(TESObjectREFR* container, TESForm *form) {
		ExtraContainerChanges::EntryDataList *entryDataList = SkaarGetAllItems(container, kFormType_None);
		return SkaarEntryDataListCountOfForm(entryDataList, form);
	}
}
