#pragma once

class VMClassRegistry;
class BGSSoundDescriptorForm;
class TESFlora;
class IngredientItem;

namespace papyrusFlora
{
	void RegisterFuncs(VMClassRegistry* registry);

	IngredientItem * GetIngredient(TESFlora* thisFlora);
	void SetIngredient(TESFlora* thisFlora, IngredientItem* ingredient);

	BGSSoundDescriptorForm * GetHarvestSound(TESFlora* thisFlora);
	void SetHarvestSound(TESFlora* thisFlora, BGSSoundDescriptorForm * sound);
};
