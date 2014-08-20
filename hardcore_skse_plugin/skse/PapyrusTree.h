#pragma once

class VMClassRegistry;
class BGSSoundDescriptorForm;
class TESObjectTREE;
class IngredientItem;

namespace papyrusTree
{
	void RegisterFuncs(VMClassRegistry* registry);

	IngredientItem * GetIngredient(TESObjectTREE* thisTree);
	void SetIngredient(TESObjectTREE* thisTree, IngredientItem* ingredient);

	BGSSoundDescriptorForm * GetHarvestSound(TESObjectTREE* thisTree);
	void SetHarvestSound(TESObjectTREE* thisTree, BGSSoundDescriptorForm * sound);
};
