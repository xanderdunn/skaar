#include "PapyrusFlora.h"
#include "GameObjects.h"

namespace papyrusFlora
{
	IngredientItem * GetIngredient(TESFlora* thisFlora)
	{
		return (thisFlora) ? thisFlora->produce.produce : NULL;
	}

	void SetIngredient(TESFlora* thisFlora, IngredientItem* ingredient)
	{
		if (thisFlora) {
			thisFlora->produce.produce = ingredient;
		}
	}

	BGSSoundDescriptorForm * GetHarvestSound(TESFlora* thisFlora)
	{
		return (thisFlora) ? thisFlora->produce.harvestSound : NULL;
	}

	void SetHarvestSound(TESFlora* thisFlora, BGSSoundDescriptorForm * sound)
	{
		if (thisFlora) {
			thisFlora->produce.harvestSound = sound;
		}
	}
}

#include "PapyrusVM.h"
#include "PapyrusNativeFunctions.h"

void papyrusFlora::RegisterFuncs(VMClassRegistry* registry)
{
	registry->RegisterFunction(
		new NativeFunction0<TESFlora, IngredientItem*>("GetIngredient", "Flora", papyrusFlora::GetIngredient, registry));

	registry->RegisterFunction(
		new NativeFunction0<TESFlora, BGSSoundDescriptorForm*>("GetHarvestSound", "Flora", papyrusFlora::GetHarvestSound, registry));
}
