#include "skse/PapyrusNativeFunctions.h"
#include "skse/GameReferences.h"
#include "skse/GameExtraData.h"

namespace SkaarSpecialInventoryCrafting {
	// Add all the contents from a source container to a destination container, leaving the source container untouched
	void SkaarAddItemsFromContainerToContainer(StaticFunctionTag *base, TESObjectREFR* pSourceContainerRef, TESObjectREFR* pDestContainerRef, UInt32 typeID);

	// Any item and its count of items in the given container will be removed from the other container, leaving the first container untouched
	void SkaarRemoveItemsInContainerFromContainer(StaticFunctionTag *base, TESObjectREFR* pInContainerRef, TESObjectREFR* pFromContainerRef, UInt32 typeID);

	// Helper functions
	// Test if a given EntryData is already in a given EntryDataList
	bool SkaarEntryDataListContainsEntryData(ExtraContainerChanges::EntryDataList *entryDataList, ExtraContainerChanges::EntryData *entry);

	bool RegisterFuncs(VMClassRegistry* registry);
}
