#include "PapyrusTree.h"
#include "GameObjects.h"

namespace papyrusTree
{
	IngredientItem * GetIngredient(TESObjectTREE* thisTree)
	{
		return (thisTree) ? thisTree->produce.produce : NULL;
	}

	void SetIngredient(TESObjectTREE* thisTree, IngredientItem* ingredient)
	{
		if (thisTree) {
			thisTree->produce.produce = ingredient;
		}
	}

	BGSSoundDescriptorForm * GetHarvestSound(TESObjectTREE* thisTree)
	{
		return (thisTree) ? thisTree->produce.harvestSound : NULL;
	}

	void SetHarvestSound(TESObjectTREE* thisTree, BGSSoundDescriptorForm * sound)
	{
		if (thisTree) {
			thisTree->produce.harvestSound = sound;
		}
	}
}

#include "PapyrusVM.h"
#include "PapyrusNativeFunctions.h"

void papyrusTree::RegisterFuncs(VMClassRegistry* registry)
{
	registry->RegisterForm(TESObjectTREE::kTypeID, "Tree");

	registry->RegisterFunction(
		new NativeFunction0<TESObjectTREE, IngredientItem*>("GetIngredient", "Tree", papyrusTree::GetIngredient, registry));

	registry->RegisterFunction(
		new NativeFunction0<TESObjectTREE, BGSSoundDescriptorForm*>("GetHarvestSound", "Tree", papyrusTree::GetHarvestSound, registry));
}
