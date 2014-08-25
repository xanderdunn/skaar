#include "SkaarSpecialInventoryCrafting.h"

#include "skse/PapyrusForm.h"

#include "skse/GameAPI.h"
#include "skse/GameFormComponents.h"
#include "skse/GameForms.h"
#include "skse/GameRTTI.h"
#include "skse/GameObjects.h"
#include "skse/GameExtraData.h"
#include "skse/GameData.h"
#include "skse/PapyrusForm.h"

#include <vector>
#include <map>

// The class ExtraContainerInfo is from PapyrusObjectReference, but it isn't public so I copy/pasted it here
typedef std::vector<ExtraContainerChanges::EntryData*> ExtraDataVec;
typedef std::map<TESForm*, UInt32> ExtraContainerMap;

class ExtraContainerInfo {
	ExtraDataVec	m_vec;
	ExtraContainerMap m_map;
public:
	ExtraContainerInfo(ExtraContainerChanges::EntryDataList * entryList) : m_map(), m_vec()
	{
		m_vec.reserve(128);
		if (entryList) {
			entryList->Visit(*this);
		}
	}

	bool Accept(ExtraContainerChanges::EntryData* data)
	{
		if (data) {
			m_vec.push_back(data);
			m_map[data->type] = m_vec.size() - 1;
		}
		return true;
	}

	// Create an EntryDataList that includes all of the items in the given EntryDataList, taking into account that some items may already exist on this container
	ExtraContainerChanges::EntryDataList *SkaarEntryDataListWithExistingItemCounts(ExtraContainerChanges::EntryDataList *givenItemsList) {
		ExtraContainerChanges::EntryDataList *newEntryList = ExtraContainerChanges::EntryDataList::Create();
		for (size_t i = 0; i < givenItemsList->Count(); i++) {
			ExtraContainerChanges::EntryData *currentEntry = givenItemsList->GetNthItem(i);
			TESForm *currentForm = currentEntry->type;
			UInt32 currentCount = currentEntry->countDelta;
			_DMESSAGE("Starting search for FormID %d, name %s", currentForm->formID, papyrusForm::GetName(currentForm));

			// See if this list already has this TESForm
			ExtraContainerMap::iterator it = m_map.find(currentForm);
			ExtraContainerMap::iterator itEnd = m_map.end();
			if (it != itEnd) {	// Already in this container's list
				UInt32 index = it->second;
				ExtraContainerChanges::EntryData* pXData = m_vec[index];
				if (pXData) {
					_DMESSAGE("EXISTING FormID %d, name %s", pXData->type->formID, papyrusForm::GetName(currentForm));
					ExtraContainerChanges::EntryData *newData = ExtraContainerChanges::EntryData::Create(pXData->type, pXData->countDelta + currentCount);
					newEntryList->Push(newData);
				}
			}
			else {  // If not, create a new EntryData object and add it to m_vec
				_DMESSAGE("NEW FormID %d, name %s, count %d", currentForm->formID, papyrusForm::GetName(currentForm), currentCount);
				ExtraContainerChanges::EntryData *newData = ExtraContainerChanges::EntryData::Create(currentForm, currentCount);
				newEntryList->Push(newData);
			}
		}
		return newEntryList;
	}

	// Take an EntryDataList and self.  Any item in self that isn't already in the given EntryDataList will be added to that list
	ExtraContainerChanges::EntryDataList *SkaarAddRemainingItems(ExtraContainerChanges::EntryDataList *currentEntries, FormType type) {
		// Iterate through all the items in self
		ExtraDataVec::iterator iteratorPosition = m_vec.begin();
		ExtraDataVec::iterator iteratorEnd = m_vec.end();
		while (iteratorPosition != iteratorEnd) {
			ExtraContainerChanges::EntryData* entryData = (*iteratorPosition);
			if (entryData) {	//  && entryData->countDelta > 0
				_DMESSAGE("EXISTING PLAYER %d", entryData->type->formID);
				bool alreadyThere = SkaarSpecialInventoryCrafting::SkaarEntryDataListContainsEntryData(currentEntries, entryData);
				if (!alreadyThere) {
					currentEntries->Push(entryData);
				}
			}
			++iteratorPosition;
		}
		return currentEntries;
	}
};


namespace SkaarSpecialInventoryCrafting {
	ExtraContainerInfo SkaarItemInfoForObjectReference(TESObjectREFR* pContainerRef);
	ExtraContainerChanges::EntryDataList *SkaarAddRemainingItems(ExtraContainerChanges::EntryDataList *currentItems, TESObjectREFR *pContainerRef, FormType type);
	bool SkaarEntryDataListContainsEntryData(ExtraContainerChanges::EntryDataList *entryDataList, ExtraContainerChanges::EntryData *entry);
	ExtraContainerChanges::EntryDataList *SkaarGetAllItems(TESObjectREFR* container, FormType type);

	// Create a new list of all items of the given type that are in the given container
	ExtraContainerChanges::EntryDataList *SkaarGetAllItems(TESObjectREFR* container, FormType type) {
		_DMESSAGE("SkaarGetAllItems called");
		ExtraContainerChanges* containerChanges = static_cast<ExtraContainerChanges*>(container->extraData.GetByType(kExtraData_ContainerChanges));
		if (!containerChanges) {
			return NULL;
		}
		ExtraContainerChanges::EntryDataList *containerItems = containerChanges->data->objList;
		ExtraContainerChanges::EntryDataList *itemsList = ExtraContainerChanges::EntryDataList::Create();
		if (!itemsList || !containerItems || containerItems->Count() <= 0) {
			return NULL;
		}
		_DMESSAGE("SkaarGetAllItems about to loop");
		for (size_t i = 0; i < containerItems->Count(); i++) {
			ExtraContainerChanges::EntryData *currentEntry = containerItems->GetNthItem(i);
			if (currentEntry) {
				if (currentEntry->type->GetFormType() == type || type == kFormType_None) {
					_DMESSAGE("Found TESForm %d", currentEntry->type->formID);
					itemsList->Push(currentEntry);
				}
			}
		}
		_DMESSAGE("SkaarGetAllItems about to return");
		return itemsList;
	}

	// For each item in a given source container, add the same number of every item to the destination container
	// In the case of the workbench containers mod, the source container is the workbench's container and the destination container is the player
	void SkaarAddItemsFromContainerToContainer(StaticFunctionTag *base, TESObjectREFR* pSourceContainerRef, TESObjectREFR* pDestContainerRef, UInt32 typeID) {
		FormType type = static_cast <FormType>(typeID);
		_DMESSAGE("SkaarAddItemsFromContainerToContainer() called");

		if (!pDestContainerRef || !pSourceContainerRef) {
			return;
		}
		// Get the list of deltas on the source container
		ExtraContainerChanges::EntryDataList *sourceContainerItems = SkaarGetAllItems(pSourceContainerRef, type);
		if (!sourceContainerItems || sourceContainerItems->Count() <= 0) {
			return;
		}

		// Get the list of things to add to the destination container
		ExtraContainerInfo destContainerInfo = SkaarItemInfoForObjectReference(pDestContainerRef);
		ExtraContainerChanges::EntryDataList *newEntryDataList = destContainerInfo.SkaarEntryDataListWithExistingItemCounts(sourceContainerItems);
		if (!newEntryDataList || newEntryDataList->Count() <= 0) {
			return;
		}

		// Put into the list those items that were already on the destination but not in the source so they aren't lost
		newEntryDataList = SkaarAddRemainingItems(newEntryDataList, pDestContainerRef, type);

		_DMESSAGE("About to add ExtraContainerChanges to the player that has %d items in it", newEntryDataList->Count());
		ExtraContainerChanges* pXDestContainerChanges = static_cast<ExtraContainerChanges*>(pDestContainerRef->extraData.GetByType(kExtraData_ContainerChanges));
		pXDestContainerChanges->data->objList = newEntryDataList;
		_DMESSAGE("SkaarAddItemsFromContainerToContainer() finished");
	}

	void SkaarRemoveItemsInContainerFromContainer(StaticFunctionTag *base, TESObjectREFR* pInContainerRef, TESObjectREFR* pFromContainerRef, UInt32 typeID) {
		FormType type = static_cast <FormType>(typeID);
		if (!pInContainerRef || !pFromContainerRef) {
			return;
		}
		// Get the items from the in container
		ExtraContainerChanges::EntryDataList *inContainerItems = SkaarGetAllItems(pInContainerRef, type);
		if (!inContainerItems || !(inContainerItems->Count() > 0)) {
			return;
		}

		ExtraContainerChanges::EntryDataList *fromContainerItems = SkaarGetAllItems(pFromContainerRef, type);
		_DMESSAGE("About to remove ExtraContainerChanges from the player that has %d items in it", inContainerItems->Count());
		for (size_t i = 0; i < inContainerItems->Count(); i++) {
			ExtraContainerChanges::EntryData *currentInEntry = inContainerItems->GetNthItem(i);
			if (!currentInEntry) {
				continue;
			}
			for (size_t j = 0; j < fromContainerItems->Count(); j++) {
				ExtraContainerChanges::EntryData *currentFromEntry = fromContainerItems->GetNthItem(j);
				if (currentFromEntry && currentFromEntry->type == currentInEntry->type && currentInEntry->countDelta > 0) {	// I do actually want to ignore items with a negative count here because I do not want to add back items that were removed from the InContainer
					UINT32 delta = currentFromEntry->countDelta - currentInEntry->countDelta;
					if (delta >= 0) {
						currentFromEntry->countDelta = delta;
					}
					else {
						currentFromEntry->countDelta = 0;
					}
				}
			}
		}
	}

	ExtraContainerChanges::EntryDataList *SkaarAddRemainingItems(ExtraContainerChanges::EntryDataList *currentItems, TESObjectREFR *pContainerRef, FormType type) {
		ExtraContainerInfo containerInfo = SkaarItemInfoForObjectReference(pContainerRef);
		return containerInfo.SkaarAddRemainingItems(currentItems, type);
	}

	// Get the items info object for a given container object reference
	ExtraContainerInfo SkaarItemInfoForObjectReference(TESObjectREFR* pContainerRef) {
		if (!pContainerRef) {
			return NULL;
		}
		// Get existing changes on a container
		ExtraContainerChanges* pXContainerChanges = static_cast<ExtraContainerChanges*>(pContainerRef->extraData.GetByType(kExtraData_ContainerChanges));
		if (!pXContainerChanges) {
			return NULL;
		}

		// Get the list of deltas on this object
		ExtraContainerInfo info(pXContainerChanges ? pXContainerChanges->data->objList : NULL);

		return info;
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

	bool RegisterFuncs(VMClassRegistry* registry) {
		registry->RegisterFunction(
			new NativeFunction3 <StaticFunctionTag, void, TESObjectREFR*, TESObjectREFR*, UInt32>("SkaarAddItemsFromContainerToContainer", "SkaarSpecialInventoryCrafting", SkaarSpecialInventoryCrafting::SkaarAddItemsFromContainerToContainer, registry));

		registry->RegisterFunction(
			new NativeFunction3 <StaticFunctionTag, void, TESObjectREFR*, TESObjectREFR*, UInt32>("SkaarRemoveItemsInContainerFromContainer", "SkaarSpecialInventoryCrafting", SkaarSpecialInventoryCrafting::SkaarRemoveItemsInContainerFromContainer, registry));

		return true;
	}
}
